#pragma once

// haunt {

#include "TemplateDependantType.h"

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
#include <phantom/constructor>
#include <phantom/friend>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("TemplateDependantType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateDependantType)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::Type>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .constructor<void()>()
            .constructor<void(TemplateDependantElement*)>()
            .method<bool(Type*) const, virtual_|override_>("isA", &_::isA)
            .method<TemplateDependantType*() const, virtual_>("asTemplateDependantType", &_::asTemplateDependantType)
            .method<TemplateDependantElement*() const>("getTemplateDependantElement", &_::getTemplateDependantElement)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateDependantType")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
