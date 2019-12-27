#pragma once

// haunt {

#include "Package.h"

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
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Package")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Package)
        {
            using PackageFolders = typedef_< phantom::reflection::PackageFolders>;
            using Sources = typedef_< phantom::reflection::Sources>;
            using String = typedef_< phantom::String>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::reflection::Symbol>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<bool(StringView)>("IsValidName", &_::IsValidName)
        
        .public_()
            .constructor<void(StringView)>()
            .method<Package*() const, virtual_|override_>("asPackage", &_::asPackage)
            .method<Source*(StringView) const>("getSource", &_::getSource)
            .method<PackageFolder*() const>("getPackageFolder", &_::getPackageFolder)
            .method<Sources const&()>("getSources", &_::getSources)
            .method<void(Source*)>("addSource", &_::addSource)
            .method<Namespace*() const>("getCounterpartNamespace", &_::getCounterpartNamespace)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<bool() const, virtual_|override_>("canBeUnloaded", &_::canBeUnloaded)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<String() const>("pathName", &_::pathName)
            .method<void(PackageFolders&) const>("getPackageFolders", &_::getPackageFolders)
        
        .public_()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("sourceAdded", &_::sourceAdded)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("sourceAboutToBeRemoved", &_::sourceAboutToBeRemoved)
        
        .protected_()
            .field("m_pNamespace", &_::m_pNamespace)
            .field("m_pFolder", &_::m_pFolder)
            .field("m_Sources", &_::m_Sources)
            .field("m_ArchivedSources", &_::m_ArchivedSources)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Package")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
