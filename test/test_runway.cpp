/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// makeAllRunways() test
// Purpose: to confirm that makeAllRunways() method initialises Runway instance 
// with 'all runways' data.
///////////////////////////////////////////////////////////////////////////////

TEST(Runway, makeAllRunways) {
	EXPECT_TRUE(metaf::Runway::makeAllRunways().isAllRunways());
}

///////////////////////////////////////////////////////////////////////////////
// Parsing tests
// Purpose: to confirm that various formats of runway included in METAR and TAF
// groups are parsed correctly, and other formats are not parsed
///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////
// Tests for isAllRunways() and isMessageRepetitionTrue()
// Purpose: to confirm that isAllRunways() and isMessageRepetitionTrue() 
// methods correctly identify 'all runways' and 'last message repetition' data
// coded as R88 and R99 in the reports. 
///////////////////////////////////////////////////////////////////////////////

TEST(Runway, isAllRunwaysTrue) {
	EXPECT_TRUE(metaf::Runway::fromString("R88")->isAllRunways());
}

TEST(Runway, isAllRunwaysFalse) {
	EXPECT_FALSE(metaf::Runway::fromString("R25L")->isAllRunways());
	EXPECT_FALSE(metaf::Runway::fromString("R25")->isAllRunways());
	EXPECT_FALSE(metaf::Runway::fromString("R88C")->isAllRunways());
	EXPECT_FALSE(metaf::Runway::fromString("R99")->isAllRunways());
}

TEST(Runway, isMessageRepetitionTrue) {
	EXPECT_TRUE(metaf::Runway::fromString("R99")->isMessageRepetition());
}

TEST(Runway, isMessageRepetitionFalse) {
	EXPECT_FALSE(metaf::Runway::fromString("R25L")->isMessageRepetition());
	EXPECT_FALSE(metaf::Runway::fromString("R25")->isMessageRepetition());
	EXPECT_FALSE(metaf::Runway::fromString("R99C")->isMessageRepetition());
	EXPECT_FALSE(metaf::Runway::fromString("R88")->isMessageRepetition());
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(Runway, isValidRunwayNumberCorrect) {
	EXPECT_TRUE(metaf::Runway::fromString("R00")->isValid());
	EXPECT_TRUE(metaf::Runway::fromString("R21")->isValid());
	EXPECT_TRUE(metaf::Runway::fromString("R36")->isValid());
	EXPECT_TRUE(metaf::Runway::fromString("R00R")->isValid());
	EXPECT_TRUE(metaf::Runway::fromString("R36R")->isValid());
}

TEST(Runway, isValidRunwayNumberIncorrect) {
	EXPECT_FALSE(metaf::Runway::fromString("R37")->isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R72")->isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R37R")->isValid());
}

TEST(Runway, isValidSpecialCodes) {
	EXPECT_TRUE(metaf::Runway::fromString("R88")->isValid());
	EXPECT_TRUE(metaf::Runway::fromString("R99")->isValid());
}

TEST(Runway, isValidSpecialCodesIncorrectDesignator) {
	EXPECT_FALSE(metaf::Runway::fromString("R88R")->isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R88C")->isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R88L")->isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R99R")->isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R99C")->isValid());
	EXPECT_FALSE(metaf::Runway::fromString("R99L")->isValid());
}