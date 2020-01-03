#include "registration.h"

#include "GlobalRegistrer.h"
#include "Namespace.h"
#include "PackageSource.h"
#include "Template.h"
#include "Type.h"
#include "phantom/detail/phantom_priv.h"
#include "phantom/reflection/EnumT.h"
#include "phantom/reflection/Module.h"

#include <phantom/reflection/Alias.h>
#include <phantom/reflection/Application.h>
#include <phantom/reflection/Method.h>
#include <phantom/reflection/Namespace.h>
#include <phantom/reflection/Package.h>
#include <phantom/reflection/Source.h>
#include <phantom/reflection/Template.h>
#include <phantom/reflection/TemplateParameter.h>
#include <phantom/reflection/TemplateSignature.h>
#include <phantom/reflection/TemplateSpecialization.h>
#include <phantom/utils/StringUtil.h>

namespace phantom
{
namespace reflection
{
SourcePusher::SourcePusher(StringView a_Name)
{
    PHANTOM_ASSERT(a_Name.size());
    PHANTOM_ASSERT_DEBUG(reflection::Package::IsValidName(a_Name), "source name must be \"dot.separated\"");
    detail::pushSourceName(a_Name);
}

PackagePusher::PackagePusher(StringView a_Name)
{
    PHANTOM_ASSERT(a_Name.size());
    PHANTOM_ASSERT_DEBUG(reflection::Package::IsValidName(a_Name), "package name must be  \"dot.separated\"");
    PHANTOM_ASSERT(detail::currentSourceName().empty(),
                   "invalid PHANTOM_PACKAGE(...) declaration inside a PHANTOM_R_SOURCE(...)/PHANTOM_R_END() block "
                   "(have you forgotten a PHANTOM_R_END(...) ?)");
    detail::pushPackageName(a_Name);
}

SourceOrPackagePoper::SourceOrPackagePoper(StringView a_Name)
{
    if (detail::currentSourceName().size())
    {
        PHANTOM_WARNING(a_Name.empty() || a_Name == detail::currentSourceName(),
                        "mismatching PHANTOM_SOURCE(\"%.*s\")/PHANTOM_R_END(\"%.*s\")",
                        PHANTOM_STRING_AS_PRINTF_ARG(detail::currentSourceName()),
                        PHANTOM_STRING_AS_PRINTF_ARG(a_Name));
        detail::popSourceName();
    }
    else
    {
        PHANTOM_ASSERT(detail::getCurrentPackageName().size(),
                       "missing PHANTOM_PACKAGE(\"name\") or PHANTOM_SOURCE(\"name\") declaration");
        PHANTOM_WARNING(a_Name.empty() || a_Name == detail::getCurrentPackageName(),
                        "mismatching PHANTOM_PACKAGE(\"%.*s\")/PHANTOM_END(\"%.*s\")",
                        PHANTOM_STRING_AS_PRINTF_ARG(detail::currentSourceName()),
                        PHANTOM_STRING_AS_PRINTF_ARG(a_Name));
        detail::popPackageName();
    }
}

void MemberBuilder::_apply(reflection::Symbol* a_pSymbol) const
{
    if (metaDatas.size())
    {
        a_pSymbol->setMetaDatas(metaDatas);
    }
}

void MemberBuilder::_apply(reflection::Subroutine* a_pSubroutine) const
{
    _apply(static_cast<reflection::Symbol*>(a_pSubroutine));
    if (defaultArguments.size())
        a_pSubroutine->setNativeDefaultArgumentStrings(defaultArguments);
}

ClassType* MemberBuilder::classType() const
{
    return static_cast<reflection::ClassType*>(owner);
}

Class* MemberBuilder::class_() const
{
    return static_cast<reflection::Class*>(owner);
}

TypeBuilderBase::TypeBuilderBase(reflection::Source* a_pSource, Scope* a_pNamingScope, Type* a_pType,
                                 TemplateSpecArgumentRegistrer a_Arguments)
    : m_pNamingScope(a_pNamingScope),
      m_TypeInstallationInfo(a_pType, a_pSource, TypeInstallFunc(this, &TypeBuilderBase::_installFunc)),
      m_TemplateSpecArgumentRegistrer(a_Arguments)
{
    if (m_TemplateSpecArgumentRegistrer)
        a_pType->addFlags(PHANTOM_R_FLAG_TEMPLATE_ELEM);
    else
    {
        if (a_pNamingScope != a_pSource)
            a_pNamingScope->addType(a_pType);
        a_pSource->addType(a_pType);
    }
}
TypeBuilderBase::TypeBuilderBase(reflection::Scope* a_pOwner, Scope* a_pNamingScope, Type* a_pType,
                                 TemplateSpecArgumentRegistrer a_Arguments)
    : m_pNamingScope(a_pNamingScope),
      m_TypeInstallationInfo(a_pType, nullptr, TypeInstallFunc(this, &TypeBuilderBase::_installFunc)),
      m_TemplateSpecArgumentRegistrer(a_Arguments)
{
    if (m_TemplateSpecArgumentRegistrer)
        a_pType->addFlags(PHANTOM_R_FLAG_TEMPLATE_ELEM);
    else
    {
        if (a_pNamingScope != a_pOwner)
            a_pNamingScope->addType(a_pType);
        a_pOwner->addType(a_pType);
    }
}

void TypeBuilderBase::_installFunc(reflection::Type* a_pType, TypeInstallationStep a_Step)
{
    switch (a_Step)
    {
    case TypeInstallationStep::TemplateSignature:
        if (m_MetaDatas.size())
            a_pType->setMetaDatas(std::move(m_MetaDatas));
        if (m_TemplateSpecArgumentRegistrer)
        {
            Template* pTemplate = this->_getClassTemplate(static_cast<reflection::ClassType*>(a_pType), m_pNamingScope);
            PHANTOM_ASSERT(pTemplate);
            auto                    args = m_TemplateSpecArgumentRegistrer();
            TemplateSpecialization* pSpec = pTemplate->getTemplateInstantiation(args);
            if (pSpec == nullptr || pSpec->getModule() != detail::currentModule())
            {
                detail::newTemplateSpecialization(pTemplate, args, static_cast<reflection::ClassType*>(a_pType));
            }
            else
            {
                // garbage orphan elements
                std::sort(args.begin(), args.end());
                args.erase(std::unique(args.begin(), args.end()), args.end());
                for (auto arg : args)
                {
                    if (arg->getOwner() == nullptr)
                        PHANTOM_DELETE_DYN arg;
                }
            }
        }
        break;
    case TypeInstallationStep::Inheritance:
        if (m_Inheritance)
            m_Inheritance(static_cast<reflection::Class*>(a_pType));
        break;
    case TypeInstallationStep::Members:
        for (MemberBuilder const& member : m_Members)
        {
            member.registrer(member);
        }
        if (auto pClass = a_pType->asClass())
            pClass->finalizeNative();
        break;
    default:
        break;
    }
}

ClassType* TypeBuilderBase::_PHNTM_getOwnerScope()
{
    PHANTOM_ASSERT(m_TypeInstallationInfo.type->asClassType());
    return static_cast<reflection::ClassType*>(m_TypeInstallationInfo.type);
}
ClassType* TypeBuilderBase::_PHNTM_getNamingScope()
{
    return _PHNTM_getOwnerScope();
}

void TypeBuilderBase::_registerTypeInstallationInfo(TypeInstallationInfo* a_pTii)
{
    detail::registerTypeInstallationInfo(a_pTii);
}

Template* TypeBuilderBase::_getClassTemplate(reflection::ClassType* a_pClass, Namespace* a_pScope)
{
    return _getClassTemplate(a_pClass, static_cast<Scope*>(a_pScope));
}

Template* TypeBuilderBase::_getClassTemplate(reflection::ClassType* a_pClass, Scope* a_pScope)
{
    PHANTOM_ASSERT(a_pScope);
    auto name = a_pClass->getName();
    for (Template* pTemplate : a_pScope->getTemplates())
    {
        if (pTemplate->getName() == name)
        {
            if (!pTemplate->isNative())
            {
                pTemplate = pTemplate->getSource()->getNativeArchive()->getTemplate(name);
                PHANTOM_ASSERT(pTemplate->isNative());
            }
            return pTemplate;
        }
    }
    return nullptr;
}

void TypeBuilderBase::_splitEnumValues(StringView a_NameList, StringViews& a_Names)
{
    phantom::StringUtil::Split(a_Names, a_NameList, ",");
    for (StringView& name : a_Names)
    {
        size_t pos = name.find_last_of(':');
        if (pos != StringView::npos)
            name = name.substr(pos + 1);
        while (std::isblank(name[0], std::locale()))
            name = name.substr(1);
    }
}

void TypeBuilderBase::operator()(reflection::MetaDatas&& a_MD)
{
    reflection::MetaDatas& dest = m_Members.size() ? m_Members.back().metaDatas : m_MetaDatas;
    PHANTOM_ASSERT(m_Members.empty() || m_Members.back().isSymbol, "last declaration does not accept meta data");
    if (dest.empty())
    {
        dest = std::move(a_MD);
    }
    else
    {
        for (auto& md : a_MD)
            dest.insert(std::move(md));
    }
}

void TypeBuilderBase::operator()(uint a_Flags)
{
    m_TypeInstallationInfo.type->addFlags(a_Flags);
    // 			uint& dest = m_Members.size() ? m_Members.back().flags : m_Flags;
    // 			PHANTOM_ASSERT(m_Members.empty() || m_Members.back().isSymbol, "last declaration does
    // not accept meta data"); 			dest |= a_Flags;
}

void TypeBuilderBase::operator()(StringView a_Annot)
{
    Strings& dest = m_Members.size() ? m_Members.back().annotations : m_Annotations;
    PHANTOM_ASSERT(m_Members.empty() || m_Members.back().isSymbol, "last declaration does not accept meta data");
    dest.push_back(a_Annot);
}

void TypeBuilderBase::operator()(StringView a_Name, Variant&& a_Value)
{
    reflection::MetaDatas& dest = m_Members.size() ? m_Members.back().metaDatas : m_MetaDatas;
    PHANTOM_ASSERT(m_Members.empty() || m_Members.back().isSymbol, "last declaration does not accept meta data");
    dest[StringWithHash(a_Name)] = std::move(a_Value);
}

void TypeBuilderBase::operator()(reflection::Modifiers a_Modifiers)
{
    m_TypeInstallationInfo.type->addModifiers(a_Modifiers);
    // 			reflection::Modifiers& dest = m_Members.size() ? m_Members.back().modifiers
    // : m_Modifiers; 			PHANTOM_ASSERT(m_Members.empty() || m_Members.back().isSymbol, "last
    // declaration does not accept meta data"); 			dest |= a_Modifiers;
}

NamespaceBuilder::NamespaceBuilder(_PHNTM_GlobalRegistrer* a_pRegistrer) : _PHNTM_pRegistrer(a_pRegistrer)
{
    _PHNTM_pNamespace = a_pRegistrer->_PHNTM_getNamingScope();
    _PHNTM_pSource = a_pRegistrer->_PHNTM_getOwnerScope();
}

NamespaceBuilder& NamespaceBuilder::using_(StringView a_Name)
{
    phantom::reflection::Symbols symbols;
    phantom::reflection::Application::Get()->findCppSymbols(a_Name, symbols, _PHNTM_pNamespace);
    PHANTOM_ASSERT(symbols.size(), "using_ : symbol not found '%.*s'", PHANTOM_STRING_AS_PRINTF_ARG(a_Name));
    for (auto pSymbol : symbols)
    {
        auto pAlias = _PHNTM_pNamespace->addAlias(pSymbol, pSymbol->getName(), Modifier::None, PHANTOM_R_FLAG_NATIVE);
        _PHNTM_pSource->addAlias(pAlias);
        _PHNTM_pRegistrer->_PHNTM_setLastSymbol(pAlias);
    }
    return *this;
}

NamespaceBuilder& NamespaceBuilder::namespace_alias(StringView a_Name, StringView a_Namespace)
{
    Symbol* pSymbol = phantom::reflection::Application::Get()->findCppSymbol(a_Namespace, _PHNTM_pNamespace);
    PHANTOM_ASSERT(pSymbol && pSymbol->asNamespace(), "cannot find namespace '%.*s' use in namespace alias '%.*s'",
                   PHANTOM_STRING_AS_PRINTF_ARG(a_Name), PHANTOM_STRING_AS_PRINTF_ARG(a_Namespace));
    _PHNTM_pNamespace->addNamespaceAlias(a_Name, static_cast<Namespace*>(pSymbol));
    return *this;
}

Source* NamespaceBuilder::_PHNTM_getSource()
{
    return _PHNTM_pSource;
}

NamespaceBuilder& NamespaceBuilder::_PHNTM_typedef(StringView a_Name, uint64_t a_Hash, Type* a_pType)
{
    static StaticGlobal<SmallMap<uint64, Aliases, 16>> MultiLevelTypedefs;

    if (a_pType)
    {
        auto pAlias = _PHNTM_pNamespace->addAlias(a_pType, a_Name);
        pAlias->setFlag(PHANTOM_R_FLAG_NATIVE);
        _PHNTM_pSource->addAlias(pAlias);
        _PHNTM_pRegistrer->_PHNTM_setLastSymbol(pAlias);
        auto foundDeferred = MultiLevelTypedefs->find(a_Hash);
        if (foundDeferred != MultiLevelTypedefs->end())
        {
            for (auto pAlias2 : foundDeferred->second)
            {
                pAlias2->setAliasedSymbol(a_pType);
            }
        }
    }
    else
    {
        auto pAlias = _PHNTM_pNamespace->addAlias(phantom::reflection::BuiltInTypes::TYPE_INT,
                                                  a_Name); // temporary set as 'int'
        pAlias->setFlag(PHANTOM_R_FLAG_NATIVE);
        _PHNTM_pSource->addAlias(pAlias);
        _PHNTM_pRegistrer->_PHNTM_setLastSymbol(pAlias);
        (*MultiLevelTypedefs)[a_Hash].push_back(pAlias);
    }
    return *this;
}

void TypeBuilderBase::_addAccess(reflection::Symbol* a_pOwner, Access a_Access)
{
    MemberBuilder member{};
    member.registrer = [](MemberBuilder const& m) { m.classType()->setDefaultAccess(reflection::Access(m.filter)); };
    member.filter = uint(a_Access);
    member.owner = a_pOwner;
    m_Members.push_back(std::move(member));
}
void TypeBuilderBase::_addSymbol(reflection::Symbol* a_pOwner, StringView a_Name, MemberRegistrer a_Reg)
{
    MemberBuilder member{};
    member.name = a_Name;
    member.registrer = a_Reg;
    member.owner = a_pOwner;
    member.isSymbol = true;
    m_Members.push_back(std::move(member));
}
void TypeBuilderBase::_addSymbol(reflection::Symbol* a_pOwner, StringView a_Name, std::initializer_list<ArgFwd> a_Args,
                                 MemberRegistrer a_Reg)
{
    MemberBuilder member{};
    member.name = a_Name;
    member.registrer = a_Reg;
    member.owner = a_pOwner;
    member.isSymbol = true;
    member.fwdArgs = a_Args;
    m_Members.push_back(std::move(member));
}
void TypeBuilderBase::_addCtor(reflection::Symbol* a_pOwner, MemberRegistrer a_Reg)
{
    MemberBuilder member{};
    member.registrer = a_Reg;
    member.owner = a_pOwner;
    member.isSymbol = true;
    member.isFunc = true;
    m_Members.push_back(std::move(member));
}
void TypeBuilderBase::_addFunc(reflection::Symbol* a_pOwner, StringView a_Name, ArgFwd a_Arg, MemberRegistrer a_Reg)
{
    MemberBuilder member{};
    member.registrer = a_Reg;
    member.name = a_Name;
    member.owner = a_pOwner;
    member.isSymbol = true;
    member.isFunc = true;
    member.fwdArgs = {a_Arg};
    m_Members.push_back(std::move(member));
}
void TypeBuilderBase::_addMethod(reflection::Symbol* a_pOwner, StringView a_Name, ArgFwd a_Arg, MemberRegistrer a_Reg)
{
    _addFunc(a_pOwner, a_Name, a_Arg, a_Reg);
}
void TypeBuilderBase::_addField(reflection::Symbol* a_pOwner, StringView a_Name, ArgFwd a_Arg, uint a_Filter,
                                MemberRegistrer a_Reg)
{
    MemberBuilder member{};
    member.registrer = a_Reg;
    member.name = a_Name;
    member.owner = a_pOwner;
    member.isSymbol = true;
    member.fwdArgs = {a_Arg};
    member.filter = a_Filter;
    m_Members.push_back(std::move(member));
}
void TypeBuilderBase::_addProperty(reflection::Symbol* a_pOwner, StringView a_Name,
                                   std::initializer_list<ArgFwd> a_Args, uint a_Filter, MemberRegistrer a_Reg)
{
    MemberBuilder member{};
    member.registrer = a_Reg;
    member.name = a_Name;
    member.owner = a_pOwner;
    member.isSymbol = true;
    member.fwdArgs = a_Args;
    member.filter = a_Filter;
    m_Members.push_back(std::move(member));
}
void TypeBuilderBase::_addCustom(reflection::Symbol* a_pOwner, std::initializer_list<ArgFwd> a_Args, uint a_UserInt,
                                 MemberRegistrer a_Reg)
{
    MemberBuilder member{};
    member.registrer = a_Reg;
    member.owner = a_pOwner;
    member.fwdArgs = a_Args;
    member.filter = a_UserInt;
    m_Members.push_back(std::move(member));
}
void TypeBuilderBase::addMember(reflection::Symbol*, MemberBuilder&& a_Member)
{
    m_Members.push_back(std::move(a_Member));
}
void TypeBuilderBase::addMember(reflection::Symbol* a_pOwner, StringView a_Name, std::initializer_list<ArgFwd> a_Args,
                                uint a_UserInt, MemberRegistrer a_Reg)
{
    MemberBuilder member{};
    member.registrer = a_Reg;
    member.name = a_Name;
    member.owner = a_pOwner;
    member.fwdArgs = a_Args;
    member.filter = a_UserInt;
    m_Members.push_back(std::move(member));
}
phantom::reflection::SymbolWrapper& SymbolWrapper::operator()(reflection::MetaDatas&& a_MD)
{
    m_pSymbol->addMetaDatas(std::move(a_MD));
    return *this;
}

phantom::reflection::SymbolWrapper& SymbolWrapper::operator()(StringView a_Name, Variant&& a_Value)
{
    m_pSymbol->setMetaData(a_Name, std::move(a_Value));
    return *this;
}

phantom::reflection::SymbolWrapper& SymbolWrapper::operator()(StringView a_Annot)
{
    m_pSymbol->addAnnotation(a_Annot);
    return *this;
}

void PhantomBuilderBase::addSubPhantomBuilderBase(PhantomBuilderBase* a_pSub)
{
    _PHNTM_SubRegistrers.push_back(a_pSub);
}

TemplateRegistrer::TemplateRegistrer(StringView (*func)(int), const char* a_strFile, int line, int tag)
    : _PHNTM_StaticGlobalRegistrer(PHANTOM_MODULE_HANDLE(this), dynamic_initializer_()->currentPackage(),
                                   dynamic_initializer_()->currentSource(), a_strFile, line, tag,
                                   {::phantom::RegistrationStep::ClassTypes}),
      m_func(func)
{
    _PHNTM_attach();
}

void TemplateRegistrer::_PHNTM_process(phantom::RegistrationStep)
{
    phantom::reflection::Symbol* pNamingScope =
    (m_func(0).empty()) ? Namespace::Global() : Namespace::Global()->findOrCreateNamespace(m_func(0));
    PHANTOM_ASSERT(pNamingScope,
                   "template scope has not been registered => ensure that the nesting class of "
                   "your template is registered before it (above in the translation unit)");
    PHANTOM_ASSERT(pNamingScope->asScope());
    if (pNamingScope->asScope()->getTemplate(m_func(3)) == nullptr)
    {
        reflection::Template* pTemplate =
        Template::Parse(m_func(1), m_func(2), m_func(3), pNamingScope, 0, PHANTOM_R_FLAG_NATIVE);
        // for(auto e : _PHNTM_EXTENDERS) e(pTemplate);
        pNamingScope->asScope()->addTemplate(pTemplate);
        if (pTemplate->getOwner() == nullptr)
        {
            /// At source scope
            detail::nativeSource(_PHNTM_file, _PHNTM_package, _PHNTM_source)->addTemplate(pTemplate);
            detail::nativeSource(_PHNTM_file, _PHNTM_package, _PHNTM_source)
            ->addTemplateSpecialization(pTemplate->getEmptyTemplateSpecialization());
        }
        else
        {
            /// At class type scope
            pTemplate->getOwner()->asScope()->addTemplateSpecialization(pTemplate->getEmptyTemplateSpecialization());
        }
    }
}

namespace detail
{
struct PerThreadScope
{
    PerThreadScope()
    {
        m_Sources.push_back(nullptr);
        m_Modules.push_back(nullptr);
    }
    ~PerThreadScope()
    {
        PHANTOM_ASSERT(m_Modules.size() == 1 && m_Modules.back() == nullptr);
        PHANTOM_ASSERT(m_Sources.size() == 1 && m_Sources.back() == nullptr);
    }
    SmallVector<reflection::Source*> m_Sources;
    SmallVector<reflection::Module*> m_Modules;
};

static thread_local typename IntTypeBySize<PHANTOM_ALIGNOF(PerThreadScope)>::type
                                    g_pPerThread_buffer[sizeof(PerThreadScope) / PHANTOM_ALIGNOF(PerThreadScope)];
static thread_local PerThreadScope* g_pPerThread = nullptr;
struct PerThreadDtor
{
    ~PerThreadDtor()
    {
        if (g_pPerThread)
        {
            if (!isMainThread())
            {
                g_pPerThread->~PerThreadScope();
                g_pPerThread = nullptr;
            }
        }
    }
};
static thread_local PerThreadDtor g_PerThreadDtor;

static void BuildPerThreadScope()
{
    g_pPerThread = new (g_pPerThread_buffer) PerThreadScope;
}

static PerThreadScope& GetPerThreadScope()
{
    if (!g_pPerThread)
    {
        PHANTOM_ASSERT(!isMainThread(), "main thread scope should have been initialized manually");
        BuildPerThreadScope();
    }
    return *g_pPerThread;
}

void InitGlobals()
{
    PHANTOM_ASSERT(!g_pPerThread);
    BuildPerThreadScope();
}

void ReleaseGlobals()
{
    g_pPerThread->~PerThreadScope();
    g_pPerThread = nullptr;
}

PHANTOM_EXPORT_PHANTOM void newTemplateSpecialization(Template* a_pTemplate, const LanguageElements& a_Arguments,
                                                      Symbol* a_pBody)
{
    _PHNTM_R_MTX_GUARD();
    PHANTOM_ASSERT(a_pTemplate->getTemplateSignature()->getTemplateParameters().size() == a_Arguments.size() ||
                   (a_pTemplate->getTemplateSignature()->isVariadic() &&
                    a_Arguments.size() >= (a_pTemplate->getTemplateSignature()->getTemplateParameters().size() - 1)));
    PHANTOM_ASSERT(a_pBody->testFlags(PHANTOM_R_FLAG_TEMPLATE_ELEM));
    Source* pSource = currentSource();
    if (pSource == nullptr)
    {
        PHANTOM_ASSERT(currentModule());
        pSource = currentModule()->getAnonymousSource();
    }
    PHANTOM_ASSERT(a_pTemplate->isNative());
    TemplateSpecialization* pSpec = pSource->addTemplateSpecialization(
    a_pTemplate, phantom::reflection::TemplateSignature::Create(PHANTOM_R_FLAG_NATIVE), a_Arguments, a_pBody);
    pSpec->setFlags(PHANTOM_R_FLAG_NATIVE | PHANTOM_R_FLAG_IMPLICIT |
                    pSpec->getFlags()); // every native TemplateSpecialization is an instantiation
}

PHANTOM_EXPORT_PHANTOM reflection::Source* nativeSource(StringView a_strFile, StringView a_strPackage,
                                                        StringView a_strSource)
{
    return dynamic_initializer_()->nativeSource(a_strFile, a_strPackage, a_strSource);
}
PHANTOM_EXPORT_PHANTOM void installModules()
{
    dynamic_initializer_()->installModules();
}

PHANTOM_EXPORT_PHANTOM Enum* newAnonEnum(PrimitiveType*)
{
    return PHANTOM_NEW(EnumT<int64>)(Modifier::None);
}

PHANTOM_EXPORT_PHANTOM Alias* newAlias(Symbol* a_pSymbol, StringView a_strAlias, Modifiers a_Modifiers, uint a_uiFlags)
{
    return Alias::Create(a_pSymbol, a_strAlias, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE);
}

PHANTOM_EXPORT_PHANTOM void pushSource(Source* a_pSource)
{
    PHANTOM_ASSERT(a_pSource != nullptr);
    GetPerThreadScope().m_Sources.push_back(a_pSource);
}

PHANTOM_EXPORT_PHANTOM Source* currentSource()
{
    return GetPerThreadScope().m_Sources.back();
}

PHANTOM_EXPORT_PHANTOM Source* currentSourceOrAnonymous()
{
    return GetPerThreadScope().m_Sources.back() ? GetPerThreadScope().m_Sources.back()
                                                : currentModule()->getAnonymousSource();
}

PHANTOM_EXPORT_PHANTOM void popSource()
{
    PHANTOM_ASSERT(GetPerThreadScope().m_Sources.back() != nullptr);
    GetPerThreadScope().m_Sources.pop_back();
}

PHANTOM_EXPORT_PHANTOM void pushModule(Module* a_pModule)
{
    GetPerThreadScope().m_Modules.push_back(a_pModule);
}

PHANTOM_EXPORT_PHANTOM Module* currentModule()
{
    return GetPerThreadScope().m_Modules.size() ? GetPerThreadScope().m_Modules.back() : nullptr;
}

PHANTOM_EXPORT_PHANTOM void popModule()
{
    GetPerThreadScope().m_Modules.pop_back();
}

PHANTOM_EXPORT_PHANTOM Type* findUsableType(size_t a_ModuleHandle, const TypeInfos& a_TypeInfos)
{
    Module* pModule = Application::Get()->getModuleByHandle(a_ModuleHandle);
    Type*   pType;
    if (pModule && (pType = pModule->findUsableType(a_TypeInfos.hash)))
        return pType;
    return nullptr;
}

PHANTOM_EXPORT_PHANTOM Type* findUsableType(const TypeInfos& a_TypeInfos)
{
    if (Type* pType = Application::Get()->findType(a_TypeInfos.hash))
    {
        return pType;
    }
    // for member types inside classes
    return Application::Get()->findCppType(a_TypeInfos.qualifiedDecoratedName(), Namespace::Global());
}

PHANTOM_EXPORT_PHANTOM StringView getModuleName(size_t a_ModuleHandle)
{
    return Application::Get()->getModuleByHandle(a_ModuleHandle)->getName();
}

PHANTOM_EXPORT_PHANTOM StringView getModuleName(Type* a_pType)
{
    if (Module* pModule = a_pType->getModule())
        return pModule->getName();
    return StringView();
}

PHANTOM_EXPORT_PHANTOM size_t getModuleHandle(Type* a_pType)
{
    Module* pModule = a_pType->getModule();
    return pModule ? pModule->getHandle() : 0;
}

PHANTOM_EXPORT_PHANTOM Symbol* findCppSymbol(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError)
{
    return Application::Get()->findCppSymbol(a_Text, a_pScope, a_pLastError);
}
PHANTOM_EXPORT_PHANTOM Type* findCppType(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError)
{
    return Application::Get()->findCppType(a_Text, a_pScope, a_pLastError);
}
PHANTOM_EXPORT_PHANTOM Class* findCppClass(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError)
{
    return Application::Get()->findCppClass(a_Text, a_pScope, a_pLastError);
}
PHANTOM_EXPORT_PHANTOM bool findCppSymbols(StringView a_Text, Symbols& a_Sym, LanguageElement* a_pScope,
                                           StringBuffer* a_pLastError)
{
    return Application::Get()->findCppSymbols(a_Text, a_Sym, a_pScope, a_pLastError);
}

PHANTOM_EXPORT_PHANTOM void use_dynamic_initializer()
{
    dynamic_initializer_();
}

PHANTOM_EXPORT_PHANTOM void pushSourceName(StringView a_strSource)
{
    dynamic_initializer_()->pushSource(a_strSource);
}
PHANTOM_EXPORT_PHANTOM void popSourceName()
{
    dynamic_initializer_()->popSource();
}
PHANTOM_EXPORT_PHANTOM void pushPackageName(StringView a_strPackage)
{
    dynamic_initializer_()->pushPackage(a_strPackage);
}
PHANTOM_EXPORT_PHANTOM void popPackageName()
{
    dynamic_initializer_()->popPackage();
}
PHANTOM_EXPORT_PHANTOM StringView currentSourceName()
{
    return dynamic_initializer_()->currentSource();
}
PHANTOM_EXPORT_PHANTOM StringView getCurrentPackageName()
{
    return dynamic_initializer_()->currentPackage();
}
PHANTOM_EXPORT_PHANTOM void registerModule(size_t a_ModuleHandle, StringView a_strName, StringView a_strBinaryFileName,
                                           StringView a_strSource, uint a_uiFlags,
                                           std::initializer_list<StringView> a_Dependencies, void (*onLoad)(),
                                           void (*onUnload)())
{
    dynamic_initializer_()->registerModule(a_ModuleHandle, a_strName, a_strBinaryFileName, a_strSource, a_uiFlags,
                                           a_Dependencies, onLoad, onUnload);
}
PHANTOM_EXPORT_PHANTOM void stepTypeInstallation(reflection::Type* a_pType)
{
    dynamic_initializer_()->stepTypeInstallation(a_pType);
}
PHANTOM_EXPORT_PHANTOM void stepTemplateInstanceInstallation(size_t a_ModuleHandle, Type* a_pType)
{
    dynamic_initializer_()->stepTemplateInstanceInstallation(a_ModuleHandle, a_pType);
}
PHANTOM_EXPORT_PHANTOM Type* registeredTypeByHash(size_t a_ModuleHandle, hash64 a_Hash)
{
    return dynamic_initializer_()->registeredTypeByHash(a_ModuleHandle, a_Hash);
}
PHANTOM_EXPORT_PHANTOM void unregisterModule(size_t a_ModuleHandle)
{
    dynamic_initializer_()->unregisterModule(a_ModuleHandle);
}
PHANTOM_EXPORT_PHANTOM bool isActive()
{
    return dynamic_initializer_()->isActive();
}
PHANTOM_EXPORT_PHANTOM bool isAutoRegistrationLocked()
{
    return dynamic_initializer_()->isAutoRegistrationLocked();
}
PHANTOM_EXPORT_PHANTOM void pushInstallation()
{
    dynamic_initializer_()->pushInstallation();
}
PHANTOM_EXPORT_PHANTOM void popInstallation()
{
    dynamic_initializer_()->popInstallation();
}
PHANTOM_EXPORT_PHANTOM void setAutoRegistrationLocked(bool a_bLocked)
{
    dynamic_initializer_()->setAutoRegistrationLocked(a_bLocked);
}

PHANTOM_EXPORT_PHANTOM void registerTypeInstallationInfo(reflection::TypeInstallationInfo* a_pTypeInstallInfo)
{
    dynamic_initializer_()->registerTypeInstallationInfo(a_pTypeInstallInfo);
}
PHANTOM_EXPORT_PHANTOM void registerTemplateInstance(size_t a_ModuleHandle, reflection::TypeInstallationInfo* a_pTii)
{
    dynamic_initializer_()->registerTemplateInstance(a_ModuleHandle, a_pTii);
}
PHANTOM_EXPORT_PHANTOM void registerType(size_t a_ModuleHandle, hash64 a_Hash, StringView a_ScopeName,
                                         reflection::Type* a_pType)
{
    dynamic_initializer_()->registerType(a_ModuleHandle, a_Hash, a_ScopeName, a_pType);
}
PHANTOM_EXPORT_PHANTOM void registerType(size_t a_ModuleHandle, hash64 a_Hash, reflection::Type* a_pType)
{
    dynamic_initializer_()->registerType(a_ModuleHandle, a_Hash, a_pType);
}
} // namespace detail

PHANTOM_EXPORT_PHANTOM void SolveAliasTemplateDefaultArguments(TemplateSignature* a_pTS, StringView a_Defaults)
{
    if (a_Defaults.size() && a_Defaults.front() != '$')
    {
        TemplateSignature* pTS = a_pTS;
        StringViews        defaultParams;
        size_t             start = 0;
        size_t             size = 0;
        int                tplLvl = 0;
        for (size_t i = 0; i < a_Defaults.size(); ++i)
        {
            char c = a_Defaults[i];
            switch (c)
            {
            case ',':
                if (tplLvl == 0)
                {
                    // new parameter
                    size_t paramStart = start;
                    size_t paramSize = size;
                    while (a_Defaults[paramStart] == '(')
                    {
                        paramStart++;
                        paramSize -= 2;
                        PHANTOM_ASSERT(a_Defaults[paramStart + paramSize] == ')');
                    }
                    defaultParams.push_back(a_Defaults.substr(paramStart, paramSize));
                    start = i + 1;
                    while (::isblank(a_Defaults[start]))
                    {
                        start++;
                        i++;
                    }
                    size = 0;
                    continue;
                }
                break;

            case '<':
                ++tplLvl;
                break;

            case '>':
                PHANTOM_ASSERT(tplLvl);
                --tplLvl;
                break;
            }
            ++size;
        }
        PHANTOM_ASSERT(tplLvl == 0);
        if (size)
        {
            while (a_Defaults[start] == '(')
            {
                start++;
                size -= 2;
                PHANTOM_ASSERT(a_Defaults[start + size] == ')');
            }
            defaultParams.push_back(a_Defaults.substr(start, size));
        }

        TemplateParameters const& params = pTS->getTemplateParameters();
        size_t                    i = 0;
        for (; i < defaultParams.size(); ++i)
        {
            Symbol* sym = Application::Get()->findCppSymbol(defaultParams[i], pTS);
            PHANTOM_ASSERT(sym);
            params[params.size() - defaultParams.size() + i]->setDefaultArgument(sym);
        }
    }
}

} // namespace reflection
} // namespace phantom
