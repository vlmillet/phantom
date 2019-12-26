#pragma once

// haunt {

#include "Collection.h"

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
#include <phantom/typedef>
#include <phantom/alias_template>

#include <phantom/template-only-push>

#include "SmallVector.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("Collection")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_ALIAS_TEMPLATE((size_t), (StaticCapacity), SmallCollection, (SmallVector<phantom::Variant, StaticCapacity>));
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Collection>("Collection"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Collection")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
