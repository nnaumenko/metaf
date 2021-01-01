/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////
// Memory test
// Purpose: to confirm that that Group type occupies a contiguous memory area,
// Note: if Group occupies a contiguous memory area, then every alternative 
// included in Group variant also occupies a contiguous memory area
///////////////////////////////////////////////////////////////////////////////

TEST(Group, is_standard_layout) {
    EXPECT_TRUE(std::is_standard_layout<metaf::Group>::value);
}