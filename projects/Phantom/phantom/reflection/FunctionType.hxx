#pragma once

// haunt {

#include "FunctionType.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/enum>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("FunctionType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FunctionType)
        {
            using EState = typedef_<_::EState>;
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Types = typedef_< phantom::reflection::Types>;
            using TypesView = typedef_< phantom::reflection::TypesView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::Type>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<FunctionType*(Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .staticMethod<FunctionType*(Type*, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .staticMethod<FunctionType*(Type*, Type*, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .staticMethod<FunctionType*(Type*, TypesView, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .enum_<EState>().values({
                {"eState_Virgin",_::eState_Virgin},
                {"eState_UnderConstruction",_::eState_UnderConstruction},
                {"eState_Final",_::eState_Final}})
            .end()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, Modifiers, uint)>()["0"]["0"]
            .constructor<void(StringView, LanguageElement*, Modifiers, uint)>()["nullptr"]["0"]["0"]
            .constructor<void(Type*, Type*, Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, TypesView, Modifiers, uint)>()["0"]["0"]
        
        .public_()
            .method<FunctionType*() const, virtual_|override_>("asFunctionType", &_::asFunctionType)
            .method<bool() const>("isRVOCandidate", &_::isRVOCandidate)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<bool(FunctionType*) const>("isSame", &_::isSame)
            .using_("Type::isSame")
            .method<bool(TypesView, Modifiers, uint) const>("matches", &_::matches)["0"]["0"]
            .method<bool(FunctionType*) const>("matches", &_::matches)
            .method<void(StringView, LanguageElement*)>("parse", &_::parse)["nullptr"]
            .method<size_t() const>("getParameterTypeCount", &_::getParameterTypeCount)
            .method<Type*(size_t) const>("getParameterType", &_::getParameterType)
            .method<void(Type*)>("addParameterType", &_::addParameterType)
            .method<TypesView() const>("getParameterTypes", &_::getParameterTypes)
            .method<void(Type*)>("setReturnType", &_::setReturnType)
            .method<Type*() const>("getReturnType", &_::getReturnType)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getUniqueName")
            .method<bool() const>("hasEllipsis", &_::hasEllipsis)
            .method<Types::const_iterator() const>("beginParameterTypes", &_::beginParameterTypes)
            .method<Types::const_iterator() const>("endParameterTypes", &_::endParameterTypes)
            .method<Type*() const, virtual_|override_>("addPointer", &_::addPointer)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
        
        .protected_()
            .field("m_pReturnType", &_::m_pReturnType)
            .field("m_ParameterTypes", &_::m_ParameterTypes)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctionType")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
