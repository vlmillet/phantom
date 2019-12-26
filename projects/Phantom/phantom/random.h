// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

// This file is part of the Phantom library. Copyright 2015 Vivien Millet.
// Distributed under the Lesser GNU Lesser General Public Licence, Version 3.0. (See
// accompanying file LICENSE.txt or copy at
// http://www.wiwila.com/open-source-tools/phantom/licence/)
// ]

#pragma once

#include <phantom/String.h>
#include <phantom/phantom.h>

namespace phantom
{
namespace random
{
PHANTOM_EXPORT_PHANTOM void str(char*      a_OutBuffer,
                                StringView model = "%%%%%%%%-%%%%-%%%%-%%%%%%%%%%%%");
PHANTOM_EXPORT_PHANTOM ulonglong integer();
PHANTOM_EXPORT_PHANTOM double    normalized();
} // namespace random
} // namespace phantom
