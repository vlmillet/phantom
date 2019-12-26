#pragma once
// haunt {

#include "Alias.h"

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
#include <phantom/friend>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Alias")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Alias)
        {
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::reflection::Symbols>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::Symbol>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<Alias*(Symbol*, StringView, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
            .constructor<void(Symbol*, StringView, Modifiers, uint)>()["\"\""]["0"]["0"]
            .method<Type*() const, virtual_|override_>("toType", &_::toType)
            .method<Namespace*() const, virtual_|override_>("toNamespace", &_::toNamespace)
            .method<Alias*() const, virtual_|override_>("asAlias", &_::asAlias)
            .method<Symbol*() const>("getAliasedSymbol", &_::getAliasedSymbol)
            .method<void(Symbol*)>("setAliasedSymbol", &_::setAliasedSymbol)
            .method<void(Alias*)>("addAlias", &_::addAlias)
            .method<Alias*(StringView) const>("getAlias", &_::getAlias)
            .method<void(Symbols&, bool) const, virtual_>("fetchAccessibleSymbols", &_::fetchAccessibleSymbols)["true"]
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Alias")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
