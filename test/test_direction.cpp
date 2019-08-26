/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(Direction, fromCardinalStringN) {
	const auto d = metaf::Direction::fromCardinalString("N");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 360u);
	EXPECT_EQ(d->status(), metaf::Direction::Status::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringNW) {
	const auto d = metaf::Direction::fromCardinalString("NW");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 315u);
	EXPECT_EQ(d->status(), metaf::Direction::Status::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringW) {
	const auto d = metaf::Direction::fromCardinalString("W");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 270u);
	EXPECT_EQ(d->status(), metaf::Direction::Status::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringSW) {
	const auto d = metaf::Direction::fromCardinalString("SW");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 225u);
	EXPECT_EQ(d->status(), metaf::Direction::Status::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringS) {
	const auto d = metaf::Direction::fromCardinalString("S");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 180u);
	EXPECT_EQ(d->status(), metaf::Direction::Status::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringSE) {
	const auto d = metaf::Direction::fromCardinalString("SE");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 135u);
	EXPECT_EQ(d->status(), metaf::Direction::Status::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringE) {
	const auto d = metaf::Direction::fromCardinalString("E");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 90u);
	EXPECT_EQ(d->status(), metaf::Direction::Status::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringNE) {
	const auto d = metaf::Direction::fromCardinalString("NE");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 45u);
	EXPECT_EQ(d->status(), metaf::Direction::Status::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringOmmitted) {
	const auto d = metaf::Direction::fromCardinalString("");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->status(), metaf::Direction::Status::OMMITTED);
}

TEST(Direction, fromCardinalStringNDV) {
	const auto d = metaf::Direction::fromCardinalString("NDV");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->status(), metaf::Direction::Status::NDV);
}

TEST(Direction, fromCardinalStringSpecialOther) {
	EXPECT_FALSE(metaf::Direction::fromCardinalString("///").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("VRB").has_value());
}

TEST(Direction, fromCardinalStringOther) {
	EXPECT_FALSE(metaf::Direction::fromCardinalString("AAA").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("212").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("SDV").has_value());
}

TEST(Direction, fromDegreesString) {
	const auto d1 = metaf::Direction::fromDegreesString("010");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->degrees(), 10u);
	EXPECT_EQ(d1->status(), metaf::Direction::Status::VALUE_DEGREES);

	const auto d2 = metaf::Direction::fromDegreesString("270");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->degrees(), 270u);
	EXPECT_EQ(d2->status(), metaf::Direction::Status::VALUE_DEGREES);
}

TEST(Direction, fromDegreesStringEmpty) {
	const auto d = metaf::Direction::fromDegreesString("");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->status(), metaf::Direction::Status::OMMITTED);
}

TEST(Direction, fromDegreesStringVariable) {
	const auto d = metaf::Direction::fromDegreesString("VRB");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->status(), metaf::Direction::Status::VARIABLE);
}

TEST(Direction, fromDegreesStringNotReported) {
	const auto d = metaf::Direction::fromDegreesString("///");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->status(), metaf::Direction::Status::NOT_REPORTED);
}

TEST(Direction, fromDegreesStringLastDigitNonZero) {
	EXPECT_FALSE(metaf::Direction::fromDegreesString("272").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("001").has_value());
}

TEST(Direction, fromDegreesStringWrongFormat) {
	EXPECT_FALSE(metaf::Direction::fromDegreesString("120A").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("A12").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("AAA").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("12").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("0120").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("//").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("////").has_value());
}

TEST(Direction, isValidCorrect) {
	auto d1 = metaf::Direction::fromDegreesString("000");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isValid());

	auto d2 = metaf::Direction::fromDegreesString("040");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isValid());

	auto d3 = metaf::Direction::fromDegreesString("360");
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isValid());

	auto d4 = metaf::Direction::fromDegreesString("///");
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->isValid());

	auto d5 = metaf::Direction::fromDegreesString("VRB");
	ASSERT_TRUE(d5.has_value());
	EXPECT_TRUE(d5->isValid());

	auto d6 = metaf::Direction::fromCardinalString("");
	ASSERT_TRUE(d6.has_value());
	EXPECT_TRUE(d6->isValid());

	auto d7 = metaf::Direction::fromCardinalString("NDV");
	ASSERT_TRUE(d7.has_value());
	EXPECT_TRUE(d7->isValid());

	auto d8 = metaf::Direction::fromCardinalString("NW");
	ASSERT_TRUE(d8.has_value());
	EXPECT_TRUE(d8->isValid());
}

TEST(Direction, isValidOutOfRange) {
	const auto d = metaf::Direction::fromDegreesString("370");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->isValid());
}

TEST(Direction, сardinalNorth) {
	const auto d1 = metaf::Direction::fromDegreesString("020");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::N);

	const auto d2 = metaf::Direction::fromDegreesString("340");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::N);
}

TEST(Direction, сardinalNorthEast) {
	const auto d1 = metaf::Direction::fromDegreesString("030");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::NE);

	const auto d2 = metaf::Direction::fromDegreesString("060");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::NE);
}

TEST(Direction, сardinalEast) {
	const auto d1 = metaf::Direction::fromDegreesString("070");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::E);

	const auto d2 = metaf::Direction::fromDegreesString("110");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::E);
}

TEST(Direction, сardinalSouthEast) {
	const auto d1 = metaf::Direction::fromDegreesString("120");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::SE);

	const auto d2 = metaf::Direction::fromDegreesString("150");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::SE);
}

TEST(Direction, сardinalSouth) {
	const auto d1 = metaf::Direction::fromDegreesString("160");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::S);

	const auto d2 = metaf::Direction::fromDegreesString("200");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::S);
}

TEST(Direction, toCardinalSouthWest) {
	const auto d1 = metaf::Direction::fromDegreesString("210");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::SW);

	const auto d2 = metaf::Direction::fromDegreesString("240");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::SW);
}

TEST(Direction, сardinalWest) {
	const auto d1 = metaf::Direction::fromDegreesString("250");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::W);

	const auto d2 = metaf::Direction::fromDegreesString("290");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::W);
}

TEST(Direction, сardinalInvalid) {
	const auto d1 = metaf::Direction::fromDegreesString("370");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::NONE);

	const auto d2 = metaf::Direction::fromDegreesString("720");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::NONE);
}

TEST(Direction, сardinalNorthWest) {
	const auto d1 = metaf::Direction::fromDegreesString("300");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::NW);

	const auto d2 = metaf::Direction::fromDegreesString("330");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::NW);
}

TEST(Direction, сardinalTrueDirectionsDisabled) {
	const auto d1 = metaf::Direction::fromDegreesString("000");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::N);

	const auto d2 = metaf::Direction::fromDegreesString("360");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::N);

	const auto d3 = metaf::Direction::fromDegreesString("090");
	ASSERT_TRUE(d3.has_value());
	EXPECT_EQ(d3->cardinal(), metaf::Direction::Cardinal::E);

	const auto d4 = metaf::Direction::fromDegreesString("180");
	ASSERT_TRUE(d4.has_value());
	EXPECT_EQ(d4->cardinal(), metaf::Direction::Cardinal::S);

	const auto d5 = metaf::Direction::fromDegreesString("270");
	ASSERT_TRUE(d5.has_value());
	EXPECT_EQ(d5->cardinal(), metaf::Direction::Cardinal::W);
}

TEST(Direction, сardinalTrueDirectionsEnabled) {
	const auto d1 = metaf::Direction::fromDegreesString("360");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(true), metaf::Direction::Cardinal::TRUE_N);

	const auto d2 = metaf::Direction::fromDegreesString("090");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(true), metaf::Direction::Cardinal::TRUE_E);

	const auto d3 = metaf::Direction::fromDegreesString("180");
	ASSERT_TRUE(d3.has_value());
	EXPECT_EQ(d3->cardinal(true), metaf::Direction::Cardinal::TRUE_S);

	const auto d4 = metaf::Direction::fromDegreesString("270");
	ASSERT_TRUE(d4.has_value());
	EXPECT_EQ(d4->cardinal(true), metaf::Direction::Cardinal::TRUE_W);
}

TEST(Direction, сardinalNdv) {
	auto d = metaf::Direction::fromCardinalString("NDV");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->cardinal(), metaf::Direction::Cardinal::NDV);
}

TEST(Direction, сardinalNone) {
	auto d1 = metaf::Direction::fromCardinalString("");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::NONE);

	const auto d2 = metaf::Direction::fromDegreesString("///");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->status(), metaf::Direction::Status::NOT_REPORTED);
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::NONE);

	const auto d3 = metaf::Direction::fromDegreesString("VRB");
	ASSERT_TRUE(d3.has_value());
	EXPECT_EQ(d3->status(), metaf::Direction::Status::VARIABLE);
	EXPECT_EQ(d3->cardinal(), metaf::Direction::Cardinal::NONE);
}

