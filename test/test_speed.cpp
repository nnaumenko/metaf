/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.1 / 2;

TEST(Speed, unitToStringKnots) {
	const auto unit = metaf::Speed::unitFromString("KT");
	ASSERT_TRUE(unit.has_value());
	EXPECT_EQ(unit, metaf::Speed::Unit::KNOTS);
}

TEST(Speed, unitToStringMetersPerSecond) {
	const auto unit = metaf::Speed::unitFromString("MPS");
	ASSERT_TRUE(unit.has_value());
	EXPECT_EQ(unit, metaf::Speed::Unit::METERS_PER_SECOND);
}

TEST(Speed, unitToStringKilometersPerHour) {
	const auto unit = metaf::Speed::unitFromString("KMH");
	ASSERT_TRUE(unit.has_value());
	EXPECT_EQ(unit, metaf::Speed::Unit::KILOMETERS_PER_HOUR);
}

TEST(Speed, unitToStringWrongUnit) {
	EXPECT_FALSE(metaf::Speed::unitFromString("").has_value());
	EXPECT_FALSE(metaf::Speed::unitFromString("MPH").has_value());
	EXPECT_FALSE(metaf::Speed::unitFromString("123").has_value());
	EXPECT_FALSE(metaf::Speed::unitFromString("///").has_value());
	EXPECT_FALSE(metaf::Speed::unitFromString("AA").has_value());
}

TEST (Speed, fromStringTwoDigit) {
	const auto s = metaf::Speed::fromString("25", metaf::Speed::Unit::METERS_PER_SECOND);
	ASSERT_TRUE(s.has_value());
	ASSERT_TRUE(s->speed().has_value());
	EXPECT_EQ(s->speed().value(), 25u);
	EXPECT_EQ(s->unit(), metaf::Speed::Unit::METERS_PER_SECOND);
}

TEST (Speed, fromStringThreeDigit) {
	const auto s = metaf::Speed::fromString("125", metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(s.has_value());
	ASSERT_TRUE(s->speed().has_value());
	EXPECT_EQ(s->speed().value(), 125u);
	EXPECT_EQ(s->unit(), metaf::Speed::Unit::KNOTS);
}

TEST (Speed, fromStringNotReported) {
	const auto s = metaf::Speed::fromString("//", metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(s.has_value());
	EXPECT_FALSE(s->speed().has_value());
	EXPECT_EQ(s->unit(), metaf::Speed::Unit::KNOTS);
}

TEST (Speed, fromStringEmpty) {
	const auto s = metaf::Speed::fromString("", metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(s.has_value());
	EXPECT_FALSE(s->speed().has_value());
	EXPECT_EQ(s->unit(), metaf::Speed::Unit::KNOTS);
}

TEST (Speed, fromStringWrongFormat) {
	EXPECT_FALSE(metaf::Speed::fromString("AA", metaf::Speed::Unit::KNOTS).has_value());
	EXPECT_FALSE(metaf::Speed::fromString("1", metaf::Speed::Unit::KNOTS).has_value());
	EXPECT_FALSE(metaf::Speed::fromString("1A", metaf::Speed::Unit::KNOTS).has_value());
	EXPECT_FALSE(metaf::Speed::fromString("020", metaf::Speed::Unit::KNOTS).has_value());
	EXPECT_FALSE(metaf::Speed::fromString("///", metaf::Speed::Unit::KNOTS).has_value());
	EXPECT_FALSE(metaf::Speed::fromString("/", metaf::Speed::Unit::KNOTS).has_value());
}

TEST (Speed, speedNotReported) {
	EXPECT_FALSE(metaf::Speed().speed().has_value());
}

TEST(Speed, toUnitSameUnit) {
	const auto speedKnots = 
		metaf::Speed::fromString("10", metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(speedKnots.has_value());
	ASSERT_TRUE(speedKnots->toUnit(metaf::Speed::Unit::KNOTS).has_value());
	EXPECT_NEAR(speedKnots->toUnit(metaf::Speed::Unit::KNOTS).value(), 10, margin);

	const auto speedMps = 
		metaf::Speed::fromString("10", metaf::Speed::Unit::METERS_PER_SECOND);
	ASSERT_TRUE(speedMps.has_value());
	ASSERT_TRUE(speedMps->toUnit(metaf::Speed::Unit::METERS_PER_SECOND).has_value());
	EXPECT_NEAR(speedMps->toUnit(metaf::Speed::Unit::METERS_PER_SECOND).value(), 10, margin);

	const auto speedKmh = 
		metaf::Speed::fromString("10", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(speedKmh.has_value());
	ASSERT_TRUE(speedKmh->toUnit(metaf::Speed::Unit::KILOMETERS_PER_HOUR).has_value());
	EXPECT_NEAR(speedKmh->toUnit(metaf::Speed::Unit::KILOMETERS_PER_HOUR).value(), 10, margin);

	const auto speedMph = 
		 metaf::Speed::fromString("10", metaf::Speed::Unit::MILES_PER_HOUR);
	ASSERT_TRUE(speedMph.has_value());
	ASSERT_TRUE(speedMph->toUnit(metaf::Speed::Unit::MILES_PER_HOUR).has_value());
	EXPECT_NEAR(speedMph->toUnit(metaf::Speed::Unit::MILES_PER_HOUR).value(), 10, margin);
}

TEST(Speed, toUnitNotReported) {
	EXPECT_FALSE(metaf::Speed().toUnit(metaf::Speed::Unit::KNOTS).has_value());
	EXPECT_FALSE(metaf::Speed().toUnit(metaf::Speed::Unit::METERS_PER_SECOND).has_value());
	EXPECT_FALSE(metaf::Speed().toUnit(metaf::Speed::Unit::KILOMETERS_PER_HOUR).has_value());
	EXPECT_FALSE(metaf::Speed().toUnit(metaf::Speed::Unit::MILES_PER_HOUR).has_value());
}

TEST(Speed, toUnitKnotsToMps) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::METERS_PER_SECOND);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 5.1, margin);
}

TEST(Speed, toUnitKnotsToKmh) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 18.5, margin);
}

TEST(Speed, toUnitKnotsToMph) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::MILES_PER_HOUR);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 11.5, margin);
}

TEST(Speed, toUnitMpsToKnots) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::METERS_PER_SECOND);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 19.4, margin);
}

TEST(Speed, toUnitMpsToKmh) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::METERS_PER_SECOND);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 36, margin);
}

TEST(Speed, toUnitMpsToMph) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::METERS_PER_SECOND);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::MILES_PER_HOUR);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 22.4, margin);
}

TEST(Speed, toUnitKmhToKnots) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 5.4, margin);
}

TEST(Speed, toUnitKmhToMps) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::METERS_PER_SECOND);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 2.8, margin);
}

TEST(Speed, toUnitKmhToMph) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::MILES_PER_HOUR);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 6.2, margin);
}

TEST(Speed, toUnitMphToKnots) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::MILES_PER_HOUR);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 8.7, margin);
}

TEST(Speed, toUnitMphToMps) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::MILES_PER_HOUR);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::METERS_PER_SECOND);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 4.5, margin);
}

TEST(Speed, toUnitMphToKmh) {
	const auto speed = metaf::Speed::fromString("10", metaf::Speed::Unit::MILES_PER_HOUR);
	ASSERT_TRUE(speed.has_value());
	const auto s = speed->toUnit(metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(s.has_value());
	EXPECT_NEAR(s.value(), 16.1, margin);
}

TEST(Speed, isReported) {
	const auto snr = metaf::Speed::fromString("//", metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(snr.has_value());
	EXPECT_FALSE(snr->isReported());

	const auto s12 = metaf::Speed::fromString("12", metaf::Speed::Unit::KNOTS);
	ASSERT_TRUE(s12.has_value());
	EXPECT_TRUE(s12->isReported());
}