#pragma once

// haunt {

#include "CppSymbolParser.h"

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
#include <phantom/struct>
#include <phantom/method>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("CppSymbolParser")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(CppSymbolParser)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::reflection::Symbols>;
            this_()
            .method<Symbol*(StringView, LanguageElement*, StringBuffer*)>("parse", &_::parse)["nullptr"]
            .method<bool(StringView, Symbols&, LanguageElement*, StringBuffer*)>("parse", &_::parse)["nullptr"]
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CppSymbolParser")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
