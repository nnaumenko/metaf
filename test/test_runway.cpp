/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(Runway, fromStringWithoutDesignator) {
	const auto r = metaf::Runway::fromString("R00");
	ASSERT_TRUE(r.has_value());
	EXPECT_EQ(r->number(), 0u);
	EXPECT_EQ(r->designator(), metaf::Runway::Designator::NONE);
}

TEST(Runway, fromStringWithDesignatorL) {
	const auto r = metaf::Runway::fromString("R30L");
	ASSERT_TRUE(r.has_value());
	EXPECT_EQ(r->number(), 30u);
	EXPECT_EQ(r->designator(), metaf::Runway::Designator::LEFT);
}

TEST(Runway, fromStringWithDesignatorC) {
	const auto r = metaf::Runway::fromString("R30C");
	ASSERT_TRUE(r.has_value());
	EXPECT_EQ(r->number(), 30u);
	EXPECT_EQ(r->designator(), metaf::Runway::Designator::CENTER);
}

TEST(Runway, fromStringWithDesignatorR) {
	const auto r = metaf::Runway::fromString("R30R");
	ASSERT_TRUE(r.has_value());
	EXPECT_EQ(r->number(), 30u);
	EXPECT_EQ(r->designator(), metaf::Runway::Designator::RIGHT);
}

TEST(Runway, fromStringAllRunways) {
	const auto r = metaf::Runway::fromString("R88");
	ASSERT_TRUE(r.has_value());
	EXPECT_EQ(r->number(), 88u);
	EXPECT_EQ(r->designator(), metaf::Runway::Designator::NONE);
}

TEST(Runway, fromStringLastMessageRepetition) {
	const auto r = metaf::Runway::fromString("R99");
	ASSERT_TRUE(r.has_value());
	EXPECT_EQ(r->number(), 99u);
	EXPECT_EQ(r->designator(), metaf::Runway::Designator::NONE);
}

TEST(Runway, fromStringWrongFormat){
	EXPECT_FALSE(metaf::Runway::fromString("").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("00").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R0A").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RR00").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RW00").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R00X").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R00/").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R00R/").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R//").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R0").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R0C").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RWY").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RWY0").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RWY0C").has_value());
}

TEST(Runway, fromStringRwy){
	EXPECT_FALSE(metaf::Runway::fromString("RWY00").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RWY00R").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RWY00C").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RWY00L").has_value());
}

TEST(Runway, fromStringWithRwyOptionR) {
	const auto r1 = metaf::Runway::fromString("R36", true);
	ASSERT_TRUE(r1.has_value());
	EXPECT_EQ(r1->number(), 36u);
	EXPECT_EQ(r1->designator(), metaf::Runway::Designator::NONE);

	const auto r2 = metaf::Runway::fromString("R36R", true);
	ASSERT_TRUE(r2.has_value());
	EXPECT_EQ(r2->number(), 36u);
	EXPECT_EQ(r2->designator(), metaf::Runway::Designator::RIGHT);

	const auto r3 = metaf::Runway::fromString("R36C", true);
	ASSERT_TRUE(r3.has_value());
	EXPECT_EQ(r3->number(), 36u);
	EXPECT_EQ(r3->designator(), metaf::Runway::Designator::CENTER);

	const auto r4 = metaf::Runway::fromString("R36L", true);
	ASSERT_TRUE(r4.has_value());
	EXPECT_EQ(r4->number(), 36u);
	EXPECT_EQ(r4->designator(), metaf::Runway::Designator::LEFT);
}

TEST(Runway, fromStringWithRwyOptionRWY) {
	const auto r1 = metaf::Runway::fromString("RWY36", true);
	ASSERT_TRUE(r1.has_value());
	EXPECT_EQ(r1->number(), 36u);
	EXPECT_EQ(r1->designator(), metaf::Runway::Designator::NONE);

	const auto r2 = metaf::Runway::fromString("RWY36R", true);
	ASSERT_TRUE(r2.has_value());
	EXPECT_EQ(r2->number(), 36u);
	EXPECT_EQ(r2->designator(), metaf::Runway::Designator::RIGHT);

	const auto r3 = metaf::Runway::fromString("RWY36C", true);
	ASSERT_TRUE(r3.has_value());
	EXPECT_EQ(r3->number(), 36u);
	EXPECT_EQ(r3->designator(), metaf::Runway::Designator::CENTER);

	const auto r4 = metaf::Runway::fromString("RWY36L", true);
	ASSERT_TRUE(r4.has_value());
	EXPECT_EQ(r4->number(), 36u);
	EXPECT_EQ(r4->designator(), metaf::Runway::Designator::LEFT);
}

TEST(Runway, fromStringWrongFormatWithRwyOption){
	EXPECT_FALSE(metaf::Runway::fromString("", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("00", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R0A", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RR00", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RW00", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R00X", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R00/", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R00R/", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R//", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R0", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R0C", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RWY", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RWY0", true).has_value());
	EXPECT_FALSE(metaf::Runway::fromString("RWY0C", true).has_value());
}

TEST(Runway, isAllRunwaysTrue) {
	EXPECT_TRUE(metaf::Runway::fromString("R88").value().isAllRunways());
}

TEST(Runway, isAllRunwaysFalse) {
	EXPECT_FALSE(metaf::Runway::fromString("R25L").value().isAllRunways());
	EXPECT_FALSE(metaf::Runway::fromString("R25").value().isAllRunways());
	EXPECT_FALSE(metaf::Runway::fromString("R88C").value().isAllRunways());
	EXPECT_FALSE(metaf::Runway::fromString("R99").value().isAllRunways());
}

TEST(Runway, isMessageRepetitionTrue) {
	EXPECT_TRUE(metaf::Runway::fromString("R99").value().isMessageRepetition());
}

TEST(Runway, isMessageRepetitionFalse) {
	EXPECT_FALSE(metaf::Runway::fromString("R25L").value().isMessageRepetition());
	EXPECT_FALSE(metaf::Runway::fromString("R25").value().isMessageRepetition());
	EXPECT_FALSE(metaf::Runway::fromString("R99C").value().isMessageRepetition());
	EXPECT_FALSE(metaf::Runway::fromString("R88").value().isMessageRepetition());
}

TEST(Runway, isValidRunwayNumberCorrect) {
	EXPECT_TRUE(metaf::Runway::fromString("R00").value().isValid());
	EXPECT_TRUE(metaf::Runway::fromString("R21").value().isValid());
	EXPECT_TRUE(metaf::Runway::fromString("R36").value().isValid());
	EXPECT_TRUE(metaf::Runway::fromString("R00R").value().isValid());
	EXPECT_TRUE(metaf::Runway::fromString("R36R").value().isValid());
}

TEST(Runway, isValidRunwayNumberIncorrect) {
	EXPECT_FALSE(metaf::Runway::fromString("R37").value().isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R72").value().isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R37R").value().isValid());
}

TEST(Runway, isValidSpecialCodes) {
	EXPECT_TRUE(metaf::Runway::fromString("R88").value().isValid());
	EXPECT_TRUE(metaf::Runway::fromString("R99").value().isValid());
}

TEST(Runway, isValidSpecialCodesIncorrectDesignator) {
	EXPECT_FALSE(metaf::Runway::fromString("R88R").value().isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R88C").value().isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R88L").value().isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R99R").value().isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R99C").value().isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R99L").value().isValid());
}

TEST(Runway, makeAllRunways) {
	EXPECT_TRUE(metaf::Runway::makeAllRunways().isAllRunways());
}