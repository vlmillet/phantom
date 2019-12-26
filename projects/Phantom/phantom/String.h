// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "SmallString.h"

namespace phantom
{
PHANTOM_FORCEINLINE const char* cstr(const String& str)
{
    return str.c_str();
}
} // namespace phantom
