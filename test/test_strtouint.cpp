/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Number parsing tests
// Purpose: to confirm that number strings and substrings are correctly parsed
///////////////////////////////////////////////////////////////////////////////

TEST(StrToUint, number1digit) {
	const auto r1 = metaf::strToUint("0", 0, 1);
	ASSERT_TRUE(r1.has_value());
	EXPECT_EQ(r1.value(), 0u);

	const auto r2 = metaf::strToUint("1", 0, 1);
	ASSERT_TRUE(r2.has_value());
	EXPECT_EQ(r2.value(), 1u);

	const auto r3 = metaf::strToUint("9", 0, 1);
	ASSERT_TRUE(r3.has_value());
	EXPECT_EQ(r3.value(), 9u);

	const auto r4 = metaf::strToUint("A5A", 1, 1);
	ASSERT_TRUE(r4.has_value());
	EXPECT_EQ(r4.value(), 5u);

	const auto r5 = metaf::strToUint("A5", 1, 1);
	ASSERT_TRUE(r5.has_value());
	EXPECT_EQ(r5.value(), 5u);
}

TEST(StrToUint, number2digit) {
	const auto r1 = metaf::strToUint("00", 0, 2);
	ASSERT_TRUE(r1.has_value());
	EXPECT_EQ(r1.value(), 0u);

	const auto r2 = metaf::strToUint("01", 0, 2);
	ASSERT_TRUE(r2.has_value());
	EXPECT_EQ(r2.value(), 1u);

	const auto r3 = metaf::strToUint("42", 0, 2);
	ASSERT_TRUE(r3.has_value());
	EXPECT_EQ(r3.value(), 42u);

	const auto r4 = metaf::strToUint("99", 0, 2);
	ASSERT_TRUE(r4.has_value());
	EXPECT_EQ(r4.value(), 99u);

	const auto r5 = metaf::strToUint("A74A", 1, 2);
	ASSERT_TRUE(r5.has_value());
	EXPECT_EQ(r5.value(), 74u);

	const auto r6 = metaf::strToUint("A74", 1, 2);
	ASSERT_TRUE(r6.has_value());
	EXPECT_EQ(r6.value(), 74u);
}

TEST(StrToUint, number3digit) {
	const auto r1 = metaf::strToUint("000", 0, 3);
	ASSERT_TRUE(r1.has_value());
	EXPECT_EQ(r1.value(), 0u);

	const auto r2 = metaf::strToUint("001", 0, 3);
	ASSERT_TRUE(r2.has_value());
	EXPECT_EQ(r2.value(), 1u);

	const auto r3 = metaf::strToUint("019", 0, 3);
	ASSERT_TRUE(r3.has_value());
	EXPECT_EQ(r3.value(), 19u);

	const auto r4 = metaf::strToUint("417", 0, 3);
	ASSERT_TRUE(r4.has_value());
	EXPECT_EQ(r4.value(), 417u);

	const auto r5 = metaf::strToUint("999", 0, 3);
	ASSERT_TRUE(r5.has_value());
	EXPECT_EQ(r5.value(), 999u);

	const auto r6 = metaf::strToUint("AAAAAA319A", 6, 3);
	ASSERT_TRUE(r6.has_value());
	EXPECT_EQ(r6.value(), 319u);

	const auto r7 = metaf::strToUint("AAAAAA319", 6, 3);
	ASSERT_TRUE(r7.has_value());
	EXPECT_EQ(r7.value(), 319u);

}

TEST(StrToUint, number4digit) {
	const auto r1 = metaf::strToUint("0000", 0, 4);
	ASSERT_TRUE(r1.has_value());
	EXPECT_EQ(r1.value(), 0u);

	const auto r2 = metaf::strToUint("0001", 0, 4);
	ASSERT_TRUE(r2.has_value());
	EXPECT_EQ(r2.value(), 1u);

	const auto r3 = metaf::strToUint("0019", 0, 4);
	ASSERT_TRUE(r3.has_value());
	EXPECT_EQ(r3.value(), 19u);

	const auto r4 = metaf::strToUint("0417", 0, 4);
	ASSERT_TRUE(r4.has_value());
	EXPECT_EQ(r4.value(), 417u);

	const auto r5 = metaf::strToUint("1482", 0, 4);
	ASSERT_TRUE(r5.has_value());
	EXPECT_EQ(r5.value(), 1482u);

	const auto r6 = metaf::strToUint("9999", 0, 4);
	ASSERT_TRUE(r6.has_value());
	EXPECT_EQ(r6.value(), 9999u);

	const auto r7 = metaf::strToUint("A7531A", 1, 4);
	ASSERT_TRUE(r7.has_value());
	EXPECT_EQ(r7.value(), 7531u);

	const auto r8 = metaf::strToUint("A7531", 1, 4);
	ASSERT_TRUE(r8.has_value());
	EXPECT_EQ(r8.value(), 7531u);
}

///////////////////////////////////////////////////////////////////////////////
// Incorrect cases
// Purpose: to confirm that invalid arguments and non-number strings result in
// empty optional
///////////////////////////////////////////////////////////////////////////////

TEST(StrToUint, stringTooShort) {
	EXPECT_FALSE(metaf::strToUint("1", 0, 2).has_value());
	EXPECT_FALSE(metaf::strToUint("1", 1, 1).has_value());
	EXPECT_FALSE(metaf::strToUint("A1", 1, 2).has_value());

	EXPECT_FALSE(metaf::strToUint("01", 0, 3).has_value());
	EXPECT_FALSE(metaf::strToUint("01", 1, 2).has_value());
	EXPECT_FALSE(metaf::strToUint("A01", 1, 3).has_value());

	EXPECT_FALSE(metaf::strToUint("001", 0, 4).has_value());
	EXPECT_FALSE(metaf::strToUint("001", 1, 3).has_value());
	EXPECT_FALSE(metaf::strToUint("A001", 1, 4).has_value());

	EXPECT_FALSE(metaf::strToUint("0001", 0, 5).has_value());
	EXPECT_FALSE(metaf::strToUint("0001", 1, 4).has_value());
	EXPECT_FALSE(metaf::strToUint("A0001", 1, 5).has_value());
}

TEST(StrToUint, wrongChars) {
	EXPECT_FALSE(metaf::strToUint("0A", 0, 2).has_value());
	EXPECT_FALSE(metaf::strToUint("A0", 0, 2).has_value());
	EXPECT_FALSE(metaf::strToUint("M00", 0, 3).has_value());
	EXPECT_FALSE(metaf::strToUint(" 0", 0, 2).has_value());
	EXPECT_FALSE(metaf::strToUint("-0", 0, 2).has_value());
	EXPECT_FALSE(metaf::strToUint("+0", 0, 2).has_value());
	EXPECT_FALSE(metaf::strToUint("0.0", 0, 3 ).has_value());
	EXPECT_FALSE(metaf::strToUint("0x0", 0, 3).has_value());
	EXPECT_FALSE(metaf::strToUint("//", 0, 2).has_value());
}

TEST(StrToUint, emptyString) {
	EXPECT_FALSE(metaf::strToUint("", 0, 1).has_value());
	EXPECT_FALSE(metaf::strToUint("", 1, 1).has_value());
	EXPECT_FALSE(metaf::strToUint("", 0, 0).has_value());
}

TEST(StrToUint, zeroDigits) {
	EXPECT_FALSE(metaf::strToUint("123", 0, 0).has_value());
	EXPECT_FALSE(metaf::strToUint("123", 1, 0).has_value());
	EXPECT_FALSE(metaf::strToUint("123", 3, 0).has_value());
}
