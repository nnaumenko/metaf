/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

TEST(FractionStrToUint, numerator1digit_denominator1digit) {
	const auto r3_8 = metaf::fractionStrToUint("3/8", 0, 3);
	ASSERT_TRUE(r3_8.has_value());
	EXPECT_EQ(std::get<0>(r3_8.value()), 3u);
	EXPECT_EQ(std::get<1>(r3_8.value()), 8u);

	const auto r0_9 = metaf::fractionStrToUint("0/9", 0, 3);
	ASSERT_TRUE(r0_9.has_value());
	EXPECT_EQ(std::get<0>(r0_9.value()), 0u);
	EXPECT_EQ(std::get<1>(r0_9.value()), 9u);

	const auto r9_0 = metaf::fractionStrToUint("9/0", 0, 3);
	ASSERT_TRUE(r9_0.has_value());
	EXPECT_EQ(std::get<0>(r9_0.value()), 9u);
	EXPECT_EQ(std::get<1>(r9_0.value()), 0u);
}

TEST(FractionStrToUint, numerator1digit_denominator2digit) {
	const auto r5_16 = metaf::fractionStrToUint("5/16", 0, 4);
	ASSERT_TRUE(r5_16.has_value());
	EXPECT_EQ(std::get<0>(r5_16.value()), 5u);
	EXPECT_EQ(std::get<1>(r5_16.value()), 16u);

	const auto r0_99 = metaf::fractionStrToUint("0/99", 0, 4);
	ASSERT_TRUE(r0_99.has_value());
	EXPECT_EQ(std::get<0>(r0_99.value()), 0u);
	EXPECT_EQ(std::get<1>(r0_99.value()), 99u);

	const auto r9_00 = metaf::fractionStrToUint("9/00", 0, 4);
	ASSERT_TRUE(r9_00.has_value());
	EXPECT_EQ(std::get<0>(r9_00.value()), 9u);
	EXPECT_EQ(std::get<1>(r9_00.value()), 0u);
}

TEST(FractionStrToUint, numerator2digit_denominator1digit) {
	const auto r11_2 = metaf::fractionStrToUint("11/2", 0, 4);
	ASSERT_TRUE(r11_2.has_value());
	EXPECT_EQ(std::get<0>(r11_2.value()), 11u);
	EXPECT_EQ(std::get<1>(r11_2.value()), 2u);

	const auto r00_9 = metaf::fractionStrToUint("00/9", 0, 4);
	ASSERT_TRUE(r00_9.has_value());
	EXPECT_EQ(std::get<0>(r00_9.value()), 0u);
	EXPECT_EQ(std::get<1>(r00_9.value()), 9u);

	const auto r9_00 = metaf::fractionStrToUint("9/00" , 0, 4);
	ASSERT_TRUE(r9_00.has_value());
	EXPECT_EQ(std::get<0>(r9_00.value()), 9u);
	EXPECT_EQ(std::get<1>(r9_00.value()), 0u);
}

TEST(FractionStrToUint, numerator2digit_denominator2digit) {
	const auto r11_16 = metaf::fractionStrToUint("11/16", 0, 5);
	ASSERT_TRUE(r11_16.has_value());
	EXPECT_EQ(std::get<0>(r11_16.value()), 11u);
	EXPECT_EQ(std::get<1>(r11_16.value()), 16u);

	const auto r00_99 = metaf::fractionStrToUint("00/99", 0, 5);
	ASSERT_TRUE(r00_99.has_value());
	EXPECT_EQ(std::get<0>(r00_99.value()), 0u);
	EXPECT_EQ(std::get<1>(r00_99.value()), 99u);

	const auto r99_00 = metaf::fractionStrToUint("99/00", 0, 5);
	ASSERT_TRUE(r99_00.has_value());
	EXPECT_EQ(std::get<0>(r99_00.value()), 99u);
	EXPECT_EQ(std::get<1>(r99_00.value()), 0u);
}

TEST(FractionStrToUint, length) {
	const auto r1 = metaf::fractionStrToUint("3/4SM", 0, 3);
	ASSERT_TRUE(r1.has_value());
	EXPECT_EQ(std::get<0>(r1.value()), 3u);
	EXPECT_EQ(std::get<1>(r1.value()), 4u);

	const auto r2 = metaf::fractionStrToUint("5/16", 0, 50);
	ASSERT_TRUE(r2.has_value());
	EXPECT_EQ(std::get<0>(r2.value()), 5u);
	EXPECT_EQ(std::get<1>(r2.value()), 16u);
}

TEST(FractionStrToUint, startPosAndLength) {
	const auto r1 = metaf::fractionStrToUint("AAAA3/4SM", 4, 3);
	ASSERT_TRUE(r1.has_value());
	EXPECT_EQ(std::get<0>(r1.value()), 3u);
	EXPECT_EQ(std::get<1>(r1.value()), 4u);

	const auto r2 = metaf::fractionStrToUint("A5/16", 1, 50);
	ASSERT_TRUE(r2.has_value());
	EXPECT_EQ(std::get<0>(r2.value()), 5u);
	EXPECT_EQ(std::get<1>(r2.value()), 16u);
}

