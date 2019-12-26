// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include "ExecutionContext.h"

#include <phantom/reflection/Destructor.h>
#include <phantom/reflection/NativeVTableInspector.h>
#include <phantom/reflection/Signature.h>
#include <phantom/traits/HasVirtualDestructor.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
namespace reflection
{
template<typename t_Ty>
class DestructorT : public Destructor
{
    typedef DestructorT<t_Ty> self_type;

public:
    DestructorT(StringView a_strName)
        : Destructor(a_strName, Signature::Create(PHANTOM_TYPEOF(void), PHANTOM_R_FLAG_NATIVE),
                     Modifiers(HasVirtualDestructor<t_Ty>::value ? PHANTOM_R_VIRTUAL : 0), PHANTOM_R_FLAG_NATIVE)
    {
    }

    virtual size_t computeNativeVirtualIndex() const override
    {
        return phantom::virtualDestructorIndex<t_Ty>();
    }

    virtual Closure getClosure() const override
    {
        return MethodClosure(&self_type::closure_wrapper);
    }

    OpaqueDelegate getOpaqueDelegate(void* a_pObject) const override
    {
        typedef void (t_Ty::*ptr)();
        return Delegate<void()>(reinterpret_cast<t_Ty*>(a_pObject), (ptr)&self_type::closure_wrapper).getOpaque();
    }

    virtual void invoke(void* a_pInstance, void**) const override
    {
        Dtor<t_Ty>::apply(a_pInstance);
    }

    virtual void invoke(void* a_pInstance, void**, void*) const override
    {
        Dtor<t_Ty>::apply(a_pInstance);
    }

    virtual void call(void** a_pArgs) const override
    {
        Dtor<t_Ty>::apply(*(void**)(a_pArgs[0]));
    }

    virtual void call(void** a_pArgs, void*) const override
    {
        Dtor<t_Ty>::apply(*(void**)(a_pArgs[0]));
    }

    virtual void call(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        PHANTOM_ASSERT(a_Context.resultPointer() == nullptr, "expecting return address from a void function call");
        Dtor<t_Ty>::apply(*(void**)(a_pArgs[0]));
    }

    virtual void placementCall(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        PHANTOM_ASSERT(a_Context.resultPointer() == nullptr, "expecting return address from a void function call");
        Dtor<t_Ty>::apply(*(void**)(a_pArgs[0]));
    }

private:
    void closure_wrapper()
    {
        Dtor<t_Ty>::apply(this);
    }
};

} // namespace reflection
} // namespace phantom

/// @endcond
