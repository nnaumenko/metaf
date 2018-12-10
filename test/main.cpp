/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"

int main(int argc, char ** argv) {
	//using gtest
	::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
