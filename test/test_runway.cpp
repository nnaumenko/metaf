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
	EXPECT_FALSE(metaf::Runway::fromString("R00X").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R00/").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R00R/").has_value());
	EXPECT_FALSE(metaf::Runway::fromString("R//").has_value());
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
