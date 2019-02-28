/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(MetafTime, fromStringDDHHMMwithDay) {
	const auto t = metaf::MetafTime::fromStringDDHHMM("120830");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 12u);
	EXPECT_EQ(t->hour(), 8u);
	EXPECT_EQ(t->minute(), 30u);
}

TEST(MetafTime, fromStringDDHHMMwithoutDay) {
	const auto t = metaf::MetafTime::fromStringDDHHMM("0830");
	ASSERT_TRUE(t.has_value());
	EXPECT_FALSE(t->day().has_value());
	EXPECT_EQ(t->hour(), 8u);
	EXPECT_EQ(t->minute(), 30u);
}

TEST(MetafTime, fromStringDDHHMM_wrongFormat) {
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("12/08:30").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("A20830").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("//////").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("20830").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("1208030").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("120830Z").has_value());
}

TEST(MetafTime, fromStringDDHH) {
	const auto t = metaf::MetafTime::fromStringDDHH("1208");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 12u);
	EXPECT_EQ(t->hour(), 8u);
	EXPECT_EQ(t->minute(), 0u);
}

TEST(MetafTime, fromStringDDHH_wrongFormat) {
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("12:08").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("12/08").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("A208").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("208").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("////").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("12080").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("1208Z").has_value());
}

TEST(MetafTime, isValidCorrect) {
	const auto t1 = metaf::MetafTime::fromStringDDHHMM("312459");
	ASSERT_TRUE(t1.has_value());
	EXPECT_TRUE(t1->isValid());

	const auto t2 = metaf::MetafTime::fromStringDDHHMM("010000");
	ASSERT_TRUE(t2.has_value());
	EXPECT_TRUE(t2->isValid());

	const auto t3 = metaf::MetafTime::fromStringDDHHMM("2459");
	ASSERT_TRUE(t3.has_value());
	EXPECT_TRUE(t3->isValid());

	const auto t4 = metaf::MetafTime::fromStringDDHHMM("0000");
	ASSERT_TRUE(t4.has_value());
	EXPECT_TRUE(t4->isValid());

	const auto t5 = metaf::MetafTime::fromStringDDHH("3124");
	ASSERT_TRUE(t5.has_value());
	EXPECT_TRUE(t5->isValid());

	const auto t6 = metaf::MetafTime::fromStringDDHH("0100");
	ASSERT_TRUE(t6.has_value());
	EXPECT_TRUE(t6->isValid());

}

TEST(MetafTime, isValidIncorrect) {
	const auto t1 = metaf::MetafTime::fromStringDDHHMM("322459");
	ASSERT_TRUE(t1.has_value());
	EXPECT_FALSE(t1->isValid());

	const auto t2 = metaf::MetafTime::fromStringDDHHMM("312559");
	ASSERT_TRUE(t2.has_value());
	EXPECT_FALSE(t2->isValid());

	const auto t3 = metaf::MetafTime::fromStringDDHHMM("312460");
	ASSERT_TRUE(t3.has_value());
	EXPECT_FALSE(t3->isValid());

	const auto t4 = metaf::MetafTime::fromStringDDHHMM("002459");
	ASSERT_TRUE(t4.has_value());
	EXPECT_FALSE(t4->isValid());

	const auto t5 = metaf::MetafTime::fromStringDDHH("3224");
	ASSERT_TRUE(t5.has_value());
	EXPECT_FALSE(t5->isValid());

	const auto t6 = metaf::MetafTime::fromStringDDHH("0024");
	ASSERT_TRUE(t6.has_value());
	EXPECT_FALSE(t6->isValid());

	const auto t7 = metaf::MetafTime::fromStringDDHH("3125");
	ASSERT_TRUE(t7.has_value());
	EXPECT_FALSE(t7->isValid());
}
