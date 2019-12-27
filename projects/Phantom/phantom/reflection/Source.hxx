#pragma once

// haunt {

#include "Source.h"

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
#include <phantom/struct>
#include <phantom/enum>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Source")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Source)
        {
            using Import = typedef_<_::Import>;
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using Sources = typedef_< phantom::reflection::Sources>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using TypesView = typedef_< phantom::reflection::TypesView>;
            this_()
            .inherits<::phantom::reflection::Symbol, ::phantom::reflection::Scope>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .struct_<Import>()
                .field("source", &_::Import::source)
                .field("isPublic", &_::Import::isPublic)
                .field("isStatic", &_::Import::isStatic)
                .field("alias", &_::Import::alias)
            .end()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .typedef_<Imports>("Imports")
        
        .public_()
            .enum_<>().values({
                {"InvalidVersionNumber",_::InvalidVersionNumber}})
            .end()
        
        .public_()
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
            .method<Package*() const>("getPackage", &_::getPackage)
            .method<Source*() const>("getNativeArchive", &_::getNativeArchive)
            .method<SourceStream*() const>("getSourceStream", &_::getSourceStream)
            .method<FunctionType*(Type*, TypesView, Modifiers, uint)>("functionType", &_::functionType)["0"]["0"]
            /// missing symbol(s) reflection (phantom::reflection::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<FunctionPointer*(Type*, ABI, TypesView, Modifiers, uint)>("functionPointerType", &_::functionPointerType)["0"]["0"]
            /// missing symbol(s) reflection (phantom::reflection::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<FunctionPointer*(FunctionType*, ABI, Modifiers, uint)>("functionPointerType", &_::functionPointerType)["0"]["0"]
            .method<InitializerListType*(TypesView)>("initializerListType", &_::initializerListType)
            .method<MethodPointer*(ClassType*, Type*, TypesView, Modifiers, uint)>("methodPointerType", &_::methodPointerType)["0"]["0"]
            .method<FieldPointer*(ClassType*, Type*, Modifiers, uint)>("fieldPointerType", &_::fieldPointerType)["0"]["0"]
            .method<void(SourceStream*)>("setSourceStream", &_::setSourceStream)
            .method<Imports const&() const>("getImports", &_::getImports)
            .method<void(Sources&) const>("getImported", &_::getImported)
            .method<Sources const&() const>("getImportings", &_::getImportings)
            .method<bool(Source*, bool, bool)>("addImport", &_::addImport)["false"]["false"]
            .method<bool(StringView, bool, bool)>("addImport", &_::addImport)["false"]["false"]
            .method<void(Source*)>("removeImport", &_::removeImport)
            .method<bool(Source*) const>("hasImport", &_::hasImport)
            .method<bool(Source*) const>("hasImported", &_::hasImported)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<bool(Source*, Access, Modifiers, uint, SmallMap<Symbol*, Symbols>*) const>("canImport", &_::canImport)["Access::Public"]["0"]["0"]["nullptr"]
            .method<const Sources&() const>("getDependencies", &_::getDependencies)
            .method<bool(Source*) const>("hasDependency", &_::hasDependency)
            .method<bool(Source*) const>("hasDependencyCascade", &_::hasDependencyCascade)
            .method<void(Source*)>("addDependency", &_::addDependency)
            .method<bool(StringView)>("addDependency", &_::addDependency)
            .method<void(Source*)>("removeDependency", &_::removeDependency)
            .method<Sources const&() const>("getDependings", &_::getDependings)
            .method<void()>("clearImports", &_::clearImports)
            .method<void()>("clearDependencies", &_::clearDependencies)
            .method<Source*() const>("asSource", &_::asSource)
            .method<Scope*() const, virtual_|override_>("asScope", &_::asScope)
            .method<bool() const>("canBeUnloaded", &_::canBeUnloaded)
            .method<void(StringBuffer&) const>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const>("getUniqueName", &_::getUniqueName)
            .method<void(LanguageElement*)>("addElement", &_::addElement)
            .method<void(LanguageElement*)>("removeElement", &_::removeElement)
            .method<Source*() const>("getCodeLocationSource", &_::getCodeLocationSource)
        
        .protected_()
            .constructor<void(Package*, StringView, Modifiers, uint)>()["0"]
        
        .protected_()
            .method<void(Symbol*), virtual_|override_>("addScopeElement", &_::addScopeElement)
            .method<void(Symbol*), virtual_|override_>("removeScopeElement", &_::removeScopeElement)
            .method<void(LanguageElement*)>("onElementAdded", &_::onElementAdded)
            .method<void(LanguageElement*)>("onElementRemoved", &_::onElementRemoved)
            .method<void(LanguageElement*)>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<hash64() const, virtual_|override_>("computeHash", &_::computeHash)
        
        .public_()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("sourceStreamChanged", &_::sourceStreamChanged)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("buildSucceeded", &_::buildSucceeded)
        
        .protected_()
            .field("m_pSourceStream", &_::m_pSourceStream)
            .field("m_pNativeArchive", &_::m_pNativeArchive)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Source")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
