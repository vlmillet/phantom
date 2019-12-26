// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "HasField.h"

#include <phantom/fwd.h>

PHANTOM_DEFINE_HAS_FIELD_NO_BASE_MACRO_PARAM(PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD);

namespace phantom
{
/// @cond INTERNAL

template<typename t_Ty>
struct HasEmbeddedRtti
    : public PHANTOM_PP_CAT(HasField_, PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD)<t_Ty, EmbeddedRtti>
{
};

template<typename t_Ty>
struct HasEmbeddedProxyRtti
    : public PHANTOM_PP_CAT(HasField_, PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD)<t_Ty, EmbeddedProxyRtti>
{
};

template<typename t_Ty>
struct HasEmbeddedRttiNoBase
    : public PHANTOM_PP_CAT(HasFieldNoBase_, PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD)<t_Ty, EmbeddedRtti>
{
};

template<typename t_Ty>
struct HasEmbeddedProxyRttiNoBase
    : public PHANTOM_PP_CAT(HasFieldNoBase_,
                            PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD)<t_Ty, EmbeddedProxyRtti>
{
};
} // namespace phantom
