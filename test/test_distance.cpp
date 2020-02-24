/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.01 / 2;

///////////////////////////////////////////////////////////////////////////////
// Constructor test
// Purpose: to confirm that constructor correctly initialises the instance
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, constructor) {
	const auto d = metaf::Distance(2, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d.modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d.isReported());
	ASSERT_TRUE(d.distance().has_value());
	EXPECT_NEAR(d.distance().value(), 2, margin);
	EXPECT_EQ(d.unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d.isValue());
}

///////////////////////////////////////////////////////////////////////////////
// Parse strings in meters
// Purpose: to confirm that distance strings specified in meters are parsed
// correctly and malformed strings are not parsed 
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, fromMeterString6500) {
	const auto d = metaf::Distance::fromMeterString("6500");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 6500, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMeterString1000) {
	const auto d = metaf::Distance::fromMeterString("1000");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 1000, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMeterString0400) {
	const auto d = metaf::Distance::fromMeterString("0400");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 400, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMeterString0150) {
	const auto d = metaf::Distance::fromMeterString("0150");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 150, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMeterString0050) {
	const auto d = metaf::Distance::fromMeterString("0050");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 50, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMeterString0000) {
	const auto d = metaf::Distance::fromMeterString("0000");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 0, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMeterStringNotReported) {
	const auto d = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->isReported());
	EXPECT_FALSE(d->distance().has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(d->isValue());
}

TEST(Distance, fromMeterString9999) {
	const auto d = metaf::Distance::fromMeterString("9999");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 10000, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMeterStringWrongFormat) {
	EXPECT_FALSE(metaf::Distance::fromMeterString("").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("2000N").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("////SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("1SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("1/2SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("120").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("12000").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("1/16").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("1").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("ZZZZ").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("015A").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("///").has_value());
	EXPECT_FALSE(metaf::Distance::fromMeterString("/////").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Parse strings in miles
// Purpose: to confirm that distance strings specified in statute miles are 
// parsed correctly and malformed strings are not parsed 
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, fromMileString3SM) {
	const auto d = metaf::Distance::fromMileString("3SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 3, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileString16SM) {
	const auto d = metaf::Distance::fromMileString("15SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 15, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileString0SM) {
	const auto d = metaf::Distance::fromMileString("0SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 0, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileString1_2SM) {
	const auto d = metaf::Distance::fromMileString("1/2SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 1.0/2.0, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileString3_16SM) {
	const auto d = metaf::Distance::fromMileString("3/16SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 3.0/16.0, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileStringM1_8SM) {
	const auto d = metaf::Distance::fromMileString("M1/8SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 1.0/8.0, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileStringM1SM) {
	const auto d = metaf::Distance::fromMileString("M1SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 1, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileStringP6SM) {
	const auto d = metaf::Distance::fromMileString("P6SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 6, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileStringP15SM) {
	const auto d = metaf::Distance::fromMileString("P15SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 15, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileStringP3_4SM) {
	const auto d = metaf::Distance::fromMileString("P3/4SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 3.0/4.0, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileString21_4SM) {
	const auto d = metaf::Distance::fromMileString("21/4SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), (2.0 + 1.0/4.0), margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileStringM11_2SM) {
	const auto d = metaf::Distance::fromMileString("M11/2SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), (1.0 + 1.0/2.0), margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromMileStringNotReported) {
	const auto d = metaf::Distance::fromMileString("////SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->isReported());
	EXPECT_FALSE(d->distance().has_value());
	EXPECT_FALSE(d->isValue());
}

TEST(Distance, fromMileStringWrongFormat) {
	EXPECT_FALSE(metaf::Distance::fromMileString("").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("2000").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("////").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("015SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("001/4SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("1/004SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("ZSM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("Z6SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("Z/2SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("1/ZSM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("///SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("/////SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("1/0SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("0/2SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromMileString("10/2SM").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Parse strings in feet
// Purpose: to confirm that distance and height strings specified in feet are 
// parsed correctly and malformed strings are not parsed 
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, fromHeightString370) {
	const auto d = metaf::Distance::fromHeightString("370");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 37000, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromHeightString145) {
	const auto d = metaf::Distance::fromHeightString("145");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 14500, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromHeightString044) {
	const auto d = metaf::Distance::fromHeightString("044");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 4400, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromHeightString001) {
	const auto d = metaf::Distance::fromHeightString("001");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 100, margin);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromHeightString000) {
	const auto d = metaf::Distance::fromHeightString("000");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 0, margin);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromHeightStringNotReported) {
	const auto d = metaf::Distance::fromHeightString("///");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->isReported());
	EXPECT_FALSE(d->isValue());
}

TEST(Distance, fromHeightStringWrongFormat) {
	EXPECT_FALSE(metaf::Distance::fromHeightString("").has_value());
	EXPECT_FALSE(metaf::Distance::fromHeightString("1000").has_value());
	EXPECT_FALSE(metaf::Distance::fromHeightString("10").has_value());
	EXPECT_FALSE(metaf::Distance::fromHeightString("1SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromHeightString("1/2SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromHeightString("P100").has_value());
	EXPECT_FALSE(metaf::Distance::fromHeightString("M001").has_value());
	EXPECT_FALSE(metaf::Distance::fromHeightString("00A").has_value());
	EXPECT_FALSE(metaf::Distance::fromHeightString("////SM").has_value());
	EXPECT_FALSE(metaf::Distance::fromHeightString("//").has_value());
	EXPECT_FALSE(metaf::Distance::fromHeightString("////").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Parse Runway Visual Range strings
// Purpose: to confirm that RVR strings specified in meters and feet are parsed
// correctly and malformed strings are not parsed 
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, fromRvrString3500) {
	const auto d = metaf::Distance::fromRvrString("3500", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 3500, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromRvrString3500feet) {
	const auto d = metaf::Distance::fromRvrString("3500", true);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 3500, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromRvrString0800) {
	const auto d = metaf::Distance::fromRvrString("0800", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 800, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromRvrString0200) {
	const auto d = metaf::Distance::fromRvrString("0200", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 200, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromRvrStringM0200) {
	const auto d = metaf::Distance::fromRvrString("M0200", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 200, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromRvrStringP2000) {
	const auto d = metaf::Distance::fromRvrString("P2000", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 2000, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromRvrString9999) {
	const auto d = metaf::Distance::fromRvrString("9999", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 9999, margin);
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromRvrString0000) {
	const auto d = metaf::Distance::fromRvrString("0000", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d->isReported());
	ASSERT_TRUE(d->distance().has_value());
	EXPECT_NEAR(d->distance().value(), 0, margin);
	EXPECT_TRUE(d->isValue());
}

TEST(Distance, fromRvrStringNotReported) {
	const auto d = metaf::Distance::fromRvrString("////", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->isReported());
	EXPECT_FALSE(d->isValue());
}

TEST(Distance, fromRvrStringWrongFormat) {
	EXPECT_FALSE(metaf::Distance::fromRvrString("", false).has_value());
	EXPECT_FALSE(metaf::Distance::fromRvrString("700", false).has_value());
	EXPECT_FALSE(metaf::Distance::fromRvrString("10000", false).has_value());
	EXPECT_FALSE(metaf::Distance::fromRvrString("X2000", false).has_value());
	EXPECT_FALSE(metaf::Distance::fromRvrString("00A0", false).has_value());
	EXPECT_FALSE(metaf::Distance::fromRvrString("1/16SM", false).has_value());
	EXPECT_FALSE(metaf::Distance::fromRvrString("////SM", false).has_value());
	EXPECT_FALSE(metaf::Distance::fromRvrString("///", false).has_value());
	EXPECT_FALSE(metaf::Distance::fromRvrString("/////", false).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Parse atmospheric layer string
// Purpose: to confirm that atmospheric layer base and height strings are 
// parsed correctly and malformed strings are not parsed 
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, fromLayerString) {
	const auto d = metaf::Distance::fromLayerString("0314");
	ASSERT_TRUE(d.has_value());

	const auto d1 = std::get<0>(d.value());
	EXPECT_EQ(d1.modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d1.isReported());
	ASSERT_TRUE(d1.distance().has_value());
	EXPECT_NEAR(d1.distance().value(), 3100, margin);
	EXPECT_EQ(d1.unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(d1.isValue());

	const auto d2 = std::get<1>(d.value());
	EXPECT_EQ(d2.modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d2.isReported());
	ASSERT_TRUE(d2.distance().has_value());
	EXPECT_NEAR(d2.distance().value(), 7100, margin);
	EXPECT_EQ(d2.unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(d2.isValue());
}

TEST(Distance, fromLayerStringWrongFormat) {
	EXPECT_FALSE(metaf::Distance::fromLayerString("").has_value());
	EXPECT_FALSE(metaf::Distance::fromLayerString("////").has_value());
	EXPECT_FALSE(metaf::Distance::fromLayerString("003/").has_value());
	EXPECT_FALSE(metaf::Distance::fromLayerString("///3").has_value());
	EXPECT_FALSE(metaf::Distance::fromLayerString("00304").has_value());
	EXPECT_FALSE(metaf::Distance::fromLayerString("304").has_value());
	EXPECT_FALSE(metaf::Distance::fromLayerString("030A").has_value());
	EXPECT_FALSE(metaf::Distance::fromLayerString("A304").has_value());
	EXPECT_FALSE(metaf::Distance::fromLayerString("03A4").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Parse kilometer string
// Purpose: to confirm that distance strings in kilometers specified in remarks
// are parsed correctly and malformed strings are not parsed 
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, fromKmString) {
	const auto d4 = metaf::Distance::fromKmString("4KM");
	ASSERT_TRUE(d4.has_value());
	EXPECT_EQ(d4->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d4->isReported());
	ASSERT_TRUE(d4->distance().has_value());
	EXPECT_NEAR(d4->distance().value(), 4000, margin);
	EXPECT_EQ(d4->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d4->isValue());

	const auto d18 = metaf::Distance::fromKmString("18KM");
	ASSERT_TRUE(d18.has_value());
	EXPECT_EQ(d18->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(d18->isReported());
	ASSERT_TRUE(d18->distance().has_value());
	EXPECT_NEAR(d18->distance().value(), 18000, margin);
	EXPECT_EQ(d18->unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d18->isValue());
}

TEST(Distance, fromKmStringWrongFormat) {
	EXPECT_FALSE(metaf::Distance::fromKmString("").has_value());
	EXPECT_FALSE(metaf::Distance::fromKmString("//KM").has_value());
	EXPECT_FALSE(metaf::Distance::fromKmString("DSNT").has_value());
	EXPECT_FALSE(metaf::Distance::fromKmString("KM").has_value());
	EXPECT_FALSE(metaf::Distance::fromKmString("012KM").has_value());
	EXPECT_FALSE(metaf::Distance::fromKmString("A0KM").has_value());
	EXPECT_FALSE(metaf::Distance::fromKmString("0AKM").has_value());
	EXPECT_FALSE(metaf::Distance::fromKmString("AKM").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Test fromIntegerAndFraction()
// Purpose: to confirm that distance value can be combined from integer and 
// fraction part, e.g. from 1 and 1/4SM. 
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, fromIntegerAndFraction) {
	const auto integer = metaf::Distance(2, metaf::Distance::Unit::STATUTE_MILES);
	const auto fraction = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(fraction.has_value());
	const auto combined = metaf::Distance::fromIntegerAndFraction(integer, fraction.value());
	ASSERT_TRUE(combined.has_value());

	EXPECT_EQ(combined->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(combined->isReported());
	ASSERT_TRUE(combined->distance().has_value());
	EXPECT_NEAR(combined->distance().value(), 2.25, margin);
	EXPECT_EQ(combined->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(combined->isValue());
}

TEST(Distance, fromIntegerAndFractionWrongCombination) {
	const auto integer = metaf::Distance(2, metaf::Distance::Unit::STATUTE_MILES);
	const auto fraction = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(fraction);
	const auto distanceMeters = metaf::Distance::fromMeterString("6000");
	ASSERT_TRUE(distanceMeters);
	const auto distanceNotReported = metaf::Distance::fromMileString("////SM");
	ASSERT_TRUE(distanceNotReported);
	const auto fractionWithModifier = metaf::Distance::fromMileString("M1/4SM");
	ASSERT_TRUE(fractionWithModifier);
	const auto integerWithModifier = metaf::Distance::fromMileString("M1SM");
	ASSERT_TRUE(integerWithModifier);

	EXPECT_FALSE(metaf::Distance::fromIntegerAndFraction(fraction.value(), 
		integer).has_value());
	EXPECT_FALSE(metaf::Distance::fromIntegerAndFraction(distanceMeters.value(), 
		fraction.value()).has_value());
	EXPECT_FALSE(metaf::Distance::fromIntegerAndFraction(distanceNotReported.value(), 
		fraction.value()).has_value());
	EXPECT_FALSE(metaf::Distance::fromIntegerAndFraction(integer, 
		distanceNotReported.value()).has_value());
	EXPECT_FALSE(metaf::Distance::fromIntegerAndFraction(distanceNotReported.value(),
		fraction.value()).has_value());
	EXPECT_FALSE(metaf::Distance::fromIntegerAndFraction(integer, 
		fractionWithModifier.value()).has_value());
	EXPECT_FALSE(metaf::Distance::fromIntegerAndFraction(integerWithModifier.value(), 
		fraction.value()).has_value());
	EXPECT_FALSE(metaf::Distance::fromIntegerAndFraction(integerWithModifier.value(), 
		fractionWithModifier.value()).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Test miscellaneous named constructors
// Purpose: to confirm that cavokVisibility(), makeDistant(), and 
// makeVicinity() methods correctly initialise distance values. 
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, cavokVisibilityMeters) {
	const auto d = metaf::Distance::cavokVisibility();
	EXPECT_EQ(d.modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(d.isReported());
	ASSERT_TRUE(d.distance().has_value());
	EXPECT_NEAR(d.distance().value(), 10000, margin);
	EXPECT_EQ(d.unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(d.isValue());
}

TEST(Distance, cavokVisibilityMiles) {
	const auto d = metaf::Distance::cavokVisibility(true);
	EXPECT_TRUE(d.isReported());
	EXPECT_EQ(d.modifier(), metaf::Distance::Modifier::MORE_THAN);
	ASSERT_TRUE(d.distance().has_value());
	EXPECT_NEAR(d.distance().value(), 6, margin);
	EXPECT_EQ(d.unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(d.isValue());
}

TEST(Distance, makeDistant) {
	const auto d = metaf::Distance::makeDistant();
	EXPECT_EQ(d.modifier(), metaf::Distance::Modifier::DISTANT);
	EXPECT_FALSE(d.distance().has_value());
	EXPECT_TRUE(d.isReported());
	EXPECT_FALSE(d.isValue());
}

TEST(Distance, makeVicinity) {
	const auto d = metaf::Distance::makeVicinity();
	EXPECT_EQ(d.modifier(), metaf::Distance::Modifier::VICINITY);
	EXPECT_FALSE(d.distance().has_value());
	EXPECT_TRUE(d.isReported());
	EXPECT_FALSE(d.isValue());
}

///////////////////////////////////////////////////////////////////////////////
// Tests for miles()
// Purpose: to confirm that miles() method initialises Miles structure with 
// correct values regardless of the units initially used to specify the 
// distance, and returns empty optional for non-reported distance.
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, miles_0) {
	const auto d1 = metaf::Distance::fromMileString("0SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::NONE);

	const auto d2 = metaf::Distance::fromMeterString("0000");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::NONE);
}

TEST(Distance, miles_1_16) {
	const auto d1 = metaf::Distance::fromMileString("1/16SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_1_16);

	const auto d2 = metaf::Distance::fromMeterString("0100");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_1_16);
}

TEST(Distance, miles_1_8) {
	const auto d1 = metaf::Distance::fromMileString("1/8SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_1_8);

	const auto d2 = metaf::Distance::fromMeterString("0200");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_1_8);
}

TEST(Distance, miles_3_16) {
	const auto d1 = metaf::Distance::fromMileString("3/16SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_3_16);

	const auto d2 = metaf::Distance::fromMeterString("0300");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_3_16);
}

TEST(Distance, miles_1_4) {
	const auto d1 = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_1_4);

	const auto d2 = metaf::Distance::fromMeterString("0400");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_1_4);
}

TEST(Distance, miles_5_16) {
	const auto d1 = metaf::Distance::fromMileString("5/16SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_5_16);

	const auto d2 = metaf::Distance::fromMeterString("0500");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_5_16);
}

TEST(Distance, miles_3_8) {
	const auto d1 = metaf::Distance::fromMileString("3/8SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_3_8);

	const auto d2 = metaf::Distance::fromMeterString("0600");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_3_8);
}

TEST(Distance, miles_1_2) {
	const auto d1 = metaf::Distance::fromMileString("1/2SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_1_2);

	const auto d2 = metaf::Distance::fromMeterString("0800");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_1_2);
}

TEST(Distance, miles_5_8) {
	const auto d1 = metaf::Distance::fromMileString("5/8SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_5_8);

	const auto d2 = metaf::Distance::fromMeterString("1000");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_5_8);
}

TEST(Distance, miles_3_4) {
	const auto d1 = metaf::Distance::fromMileString("3/4SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_3_4);

	const auto d2 = metaf::Distance::fromMeterString("1200");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_3_4);
}

TEST(Distance, miles_7_8) {
	const auto d1 = metaf::Distance::fromMileString("7/8SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_7_8);

	const auto d2 = metaf::Distance::fromMeterString("1400");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 0u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_7_8);
}

TEST(Distance, miles_1) {
	const auto d1 = metaf::Distance::fromMileString("1SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::NONE);

	const auto d2 = metaf::Distance::fromMeterString("1600");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::NONE);
}

TEST(Distance, miles_1_1_8) {
	const auto df = metaf::Distance::fromMileString("1/8SM");
	ASSERT_TRUE(df.has_value());
	const auto d1 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(1, metaf::Distance::Unit::STATUTE_MILES), df.value());
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_1_8);

	const auto d2 = metaf::Distance::fromMeterString("1800");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_1_8);
}

TEST(Distance, miles_1_1_4) {
	const auto df = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(df.has_value());
	const auto d1 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(1, metaf::Distance::Unit::STATUTE_MILES), df.value());
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_1_4);

	const auto d2 = metaf::Distance::fromMeterString("2000");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_1_4);
}

TEST(Distance, miles_1_3_8) {
	const auto df = metaf::Distance::fromMileString("3/8SM");
	ASSERT_TRUE(df.has_value());
	const auto d1 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(1, metaf::Distance::Unit::STATUTE_MILES), df.value());
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_3_8);

	const auto d2 = metaf::Distance::fromMeterString("2200");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_3_8);
}

TEST(Distance, miles_1_1_2) {
	const auto df = metaf::Distance::fromMileString("1/2SM");
	ASSERT_TRUE(df.has_value());
	const auto d1 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(1, metaf::Distance::Unit::STATUTE_MILES), df.value());
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_1_2);

	const auto d2 = metaf::Distance::fromMeterString("2400");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_1_2);
}

TEST(Distance, miles_1_5_8) {
	const auto df = metaf::Distance::fromMileString("5/8SM");
	ASSERT_TRUE(df.has_value());
	const auto d1 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(1, metaf::Distance::Unit::STATUTE_MILES), df.value());
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_5_8);

	const auto d2 = metaf::Distance::fromMeterString("2600");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_5_8);
}

TEST(Distance, miles_1_3_4) {
	const auto df = metaf::Distance::fromMileString("3/4SM");
	ASSERT_TRUE(df.has_value());
	const auto d1 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(1, metaf::Distance::Unit::STATUTE_MILES), df.value());
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_3_4);

	const auto d2 = metaf::Distance::fromMeterString("2800");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_3_4);
}

TEST(Distance, miles_1_7_8) {
	const auto df = metaf::Distance::fromMileString("7/8SM");
	ASSERT_TRUE(df.has_value());
	const auto d1 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(1, metaf::Distance::Unit::STATUTE_MILES), df.value());
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_7_8);

	const auto d2 = metaf::Distance::fromMeterString("3000");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 1u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_7_8);
}

TEST(Distance, miles_2) {
	const auto d1 = metaf::Distance::fromMileString("2SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 2u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::NONE);

	const auto d2 = metaf::Distance::fromMeterString("3200");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 2u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::NONE);
}

TEST(Distance, miles_2_1_4) {
	const auto df = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(df.has_value());
	const auto d1 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(2, metaf::Distance::Unit::STATUTE_MILES), df.value());
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 2u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_1_4);

	const auto d2 = metaf::Distance::fromMeterString("3600");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 2u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_1_4);
}

TEST(Distance, miles_2_1_2) {
	const auto df = metaf::Distance::fromMileString("1/2SM");
	ASSERT_TRUE(df.has_value());
	const auto d1 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(2, metaf::Distance::Unit::STATUTE_MILES), df.value());
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 2u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_1_2);

	const auto d2 = metaf::Distance::fromMeterString("4000");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 2u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_1_2);
}

TEST(Distance, miles_2_3_4) {
	const auto df = metaf::Distance::fromMileString("3/4SM");
	ASSERT_TRUE(df.has_value());
	const auto d1 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(2, metaf::Distance::Unit::STATUTE_MILES), df.value());
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 2u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::F_3_4);

	const auto d2 = metaf::Distance::fromMeterString("4400");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 2u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::F_3_4);
}

TEST(Distance, miles_3) {
	const auto d1 = metaf::Distance::fromMileString("3SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 3u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::NONE);

	const auto d2 = metaf::Distance::fromMeterString("4800");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 3u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::NONE);

	const auto df14 = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(df14.has_value());
	const auto d3 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(3, metaf::Distance::Unit::STATUTE_MILES), df14.value());
	ASSERT_TRUE(d3.has_value());
	const auto m3 = d1->miles();
	ASSERT_TRUE(m3.has_value());
	EXPECT_EQ(std::get<unsigned int>(m3.value()), 3u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m3.value()), 
		metaf::Distance::MilesFraction::NONE);

	const auto df34 = metaf::Distance::fromMileString("3/4SM");
	ASSERT_TRUE(df34.has_value());
	const auto d4 = metaf::Distance::fromIntegerAndFraction(
		metaf::Distance(3, metaf::Distance::Unit::STATUTE_MILES), df34.value());
	ASSERT_TRUE(d4.has_value());
	const auto m4 = d1->miles();
	ASSERT_TRUE(m4.has_value());
	EXPECT_EQ(std::get<unsigned int>(m4.value()), 3u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m4.value()), 
		metaf::Distance::MilesFraction::NONE);
}

TEST(Distance, miles_4) {
	const auto d1 = metaf::Distance::fromMileString("4SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 4u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::NONE);

	const auto d2 = metaf::Distance::fromMeterString("6400");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 4u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::NONE);
}

TEST(Distance, miles_5) {
	const auto d1 = metaf::Distance::fromMileString("5SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 5u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::NONE);

	const auto d2 = metaf::Distance::fromMeterString("8000");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 5u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::NONE);
}

TEST(Distance, miles_6) {
	const auto d1 = metaf::Distance::fromMileString("6SM");
	ASSERT_TRUE(d1.has_value());
	const auto m1 = d1->miles();
	ASSERT_TRUE(m1.has_value());
	EXPECT_EQ(std::get<unsigned int>(m1.value()), 6u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m1.value()), 
		metaf::Distance::MilesFraction::NONE);

	const auto d2 = metaf::Distance::fromMeterString("9700");
	ASSERT_TRUE(d2.has_value());
	const auto m2 = d2->miles();
	ASSERT_TRUE(m2.has_value());
	EXPECT_EQ(std::get<unsigned int>(m2.value()), 6u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(m2.value()), 
		metaf::Distance::MilesFraction::NONE);
}

TEST(Distance, miles_7_to_14) {
	const metaf::Distance d7(7, metaf::Distance::Unit::STATUTE_MILES);
	ASSERT_TRUE(d7.miles().has_value());
	EXPECT_EQ(std::get<unsigned int>(d7.miles().value()), 7u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d7.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d8(8, metaf::Distance::Unit::STATUTE_MILES);
	ASSERT_TRUE(d8.miles().has_value());
	EXPECT_EQ(std::get<unsigned int>(d8.miles().value()), 8u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d8.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d9(9, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d9.miles().value()), 9u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d9.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d10(10, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d10.miles().value()), 10u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d10.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d11(11, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d11.miles().value()), 11u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d11.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d12(12, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d12.miles().value()), 12u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d12.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d13(13, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d13.miles().value()), 13u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d13.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d14(14, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d14.miles().value()), 14u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d14.miles().value()), 
		metaf::Distance::MilesFraction::NONE);
}

TEST(Distance, miles_15_to_20) {
	const metaf::Distance d15(15, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d15.miles().value()), 15u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d15.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d16(16, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d16.miles().value()), 15u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d16.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d17(17, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d17.miles().value()), 15u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d17.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d18(18, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d18.miles().value()), 15u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d18.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d19(19, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d19.miles().value()), 15u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d19.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d20(20, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d20.miles().value()), 20u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d20.miles().value()), 
		metaf::Distance::MilesFraction::NONE);
}

TEST(Distance, miles_25_to_50) {
	const metaf::Distance d25(25, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d25.miles().value()), 25u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d25.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d30(30, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d30.miles().value()), 30u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d30.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d35(35, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d35.miles().value()), 35u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d35.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d40(40, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d40.miles().value()), 40u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d40.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d45(45, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d45.miles().value()), 45u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d45.miles().value()), 
		metaf::Distance::MilesFraction::NONE);

	const metaf::Distance d50(50, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(std::get<unsigned int>(d50.miles().value()), 50u);
	EXPECT_EQ(std::get<metaf::Distance::MilesFraction>(d50.miles().value()), 
		metaf::Distance::MilesFraction::NONE);
}

TEST(Distance, milesFromOtherNotReported) {
	const auto d1 = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->miles().has_value());

	const auto d2 = metaf::Distance::fromMileString("////SM");
	ASSERT_TRUE(d2.has_value());
	EXPECT_FALSE(d2->miles().has_value());
}

TEST(Distance, milesFromOtherDistant) {
	const auto d = metaf::Distance::makeDistant();
	const auto miles = d.miles();
	EXPECT_FALSE(miles.has_value());
}

TEST(Distance, milesFromOtherVicinity) {
	const auto d = metaf::Distance::makeVicinity();
	const auto miles = d.miles();
	EXPECT_FALSE(miles.has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Test isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, isValidTrueMeters) {
	const auto d1 = metaf::Distance::fromMeterString("6500");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isValid());

	const auto d2 = metaf::Distance::fromMeterString("9999");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isValid());

	const auto d3 = metaf::Distance::fromMeterString("0000");
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isValid());

	const auto d4 = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->isValid());
}

TEST(Distance, isValidTrueMiles) {
	const auto d1 = metaf::Distance::fromMileString("3SM");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isValid());

	const auto d2 = metaf::Distance::fromMileString("1/2SM");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isValid());

	const auto d3 = metaf::Distance::fromMileString("11/2SM");
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isValid());

	const auto d4 = metaf::Distance::fromMileString("M1/4SM");
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->isValid());

	const auto d5 = metaf::Distance::fromMileString("P15SM");
	ASSERT_TRUE(d5.has_value());
	EXPECT_TRUE(d5->isValid());

	const auto d6 = metaf::Distance::fromMileString("0SM");
	ASSERT_TRUE(d6.has_value());
	EXPECT_TRUE(d6->isValid());

	const auto d7 = metaf::Distance::fromMileString("////SM");
	ASSERT_TRUE(d7.has_value());
	EXPECT_TRUE(d7->isValid());
}

TEST(Distance, isValidTrueHeight) {
	const auto d1 = metaf::Distance::fromHeightString("370");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isValid());

	const auto d2 = metaf::Distance::fromHeightString("000");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isValid());

	const auto d3 = metaf::Distance::fromHeightString("///");
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isValid());
}

TEST(Distance, isValidTrueRvr) {
	const auto d1 = metaf::Distance::fromRvrString("2000", false);
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isValid());

	const auto d2 = metaf::Distance::fromRvrString("M0200", true);
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isValid());

	const auto d3 = metaf::Distance::fromRvrString("P6000", true);
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isValid());

	const auto d4 = metaf::Distance::fromRvrString("0000", false);
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->isValid());
	
	const auto d5 = metaf::Distance::fromRvrString("////", false);
	ASSERT_TRUE(d5.has_value());
	EXPECT_TRUE(d5->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Test toUnit()
// Purpose: to confirm that toUnit() method correctly converts the data to 
// various distance units and that non-reported values result in empty optional
///////////////////////////////////////////////////////////////////////////////

TEST(Distance, toUnitSameUnit) {
	const auto d1 = metaf::Distance::fromMeterString("2500");
	ASSERT_TRUE(d1.has_value());
	ASSERT_TRUE(d1->toUnit(metaf::Distance::Unit::METERS).has_value());
	EXPECT_NEAR(d1->toUnit(metaf::Distance::Unit::METERS).value(), 2500, margin);

	const auto d2 = metaf::Distance::fromMileString("3SM");
	ASSERT_TRUE(d2.has_value());
	ASSERT_TRUE(d1->toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_NEAR(d2->toUnit(metaf::Distance::Unit::STATUTE_MILES).value(), 3, margin);

	const auto d3 = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(d3.has_value());
	ASSERT_TRUE(d1->toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_NEAR(d3->toUnit(metaf::Distance::Unit::STATUTE_MILES).value(), 0.25, margin);

	const auto d4 = metaf::Distance::fromHeightString("044");
	ASSERT_TRUE(d4.has_value());
	ASSERT_TRUE(d4->toUnit(metaf::Distance::Unit::FEET).has_value());
	EXPECT_NEAR(d4->toUnit(metaf::Distance::Unit::FEET).value(), 4400, margin);
}

TEST(Distance, toUnitNotReported) {
	const auto d1 = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->toUnit(metaf::Distance::Unit::METERS).has_value());
	EXPECT_FALSE(d1->toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_FALSE(d1->toUnit(metaf::Distance::Unit::FEET).has_value());

	const auto d2 = metaf::Distance::fromMileString("////SM");
	ASSERT_TRUE(d2.has_value());
	EXPECT_FALSE(d2->toUnit(metaf::Distance::Unit::METERS).has_value());
	EXPECT_FALSE(d2->toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_FALSE(d2->toUnit(metaf::Distance::Unit::FEET).has_value());

	const auto d3 = metaf::Distance::fromHeightString("///");
	ASSERT_TRUE(d3.has_value());
	EXPECT_FALSE(d3->toUnit(metaf::Distance::Unit::METERS).has_value());
	EXPECT_FALSE(d3->toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_FALSE(d3->toUnit(metaf::Distance::Unit::FEET).has_value());
}

TEST(Distance, toUnitMetersToMiles) {
	const auto d1 = metaf::Distance::fromMeterString("2000");
	ASSERT_TRUE(d1.has_value());
	ASSERT_TRUE(d1->toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_NEAR(d1->toUnit(metaf::Distance::Unit::STATUTE_MILES).value(), 1.24, margin);

	const auto d2 = metaf::Distance(24140, metaf::Distance::Unit::METERS);
	ASSERT_TRUE(d2.toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_NEAR(d2.toUnit(metaf::Distance::Unit::STATUTE_MILES).value(), 15, margin);

	const auto d3 = metaf::Distance(80467, metaf::Distance::Unit::METERS);
	ASSERT_TRUE(d3.toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_NEAR(d3.toUnit(metaf::Distance::Unit::STATUTE_MILES).value(), 50, margin);
}

TEST(Distance, toUnitMetersToFeet) {
	const auto d = metaf::Distance::fromMeterString("2000");
	ASSERT_TRUE(d.has_value());
	ASSERT_TRUE(d->toUnit(metaf::Distance::Unit::FEET).has_value());
	EXPECT_NEAR(d->toUnit(metaf::Distance::Unit::FEET).value(), 6561.68, margin);
}

TEST(Distance, toUnitMilesToMeters) {
	const auto d = metaf::Distance::fromMileString("3SM");
	ASSERT_TRUE(d.has_value());
	ASSERT_TRUE(d->toUnit(metaf::Distance::Unit::METERS).has_value());
	EXPECT_NEAR(d->toUnit(metaf::Distance::Unit::METERS).value(), 4828.04, margin);
}

TEST(Distance, toUnitMilesToFeet) {
	const auto d = metaf::Distance::fromMileString("3SM");
	ASSERT_TRUE(d.has_value());
	ASSERT_TRUE(d->toUnit(metaf::Distance::Unit::FEET).has_value());
	EXPECT_NEAR(d->toUnit(metaf::Distance::Unit::FEET).value(), 15840.00, margin);
}

TEST(Distance, toUnitFeetToMeters) {
	const auto d = metaf::Distance::fromHeightString("330");
	ASSERT_TRUE(d.has_value());
	ASSERT_TRUE(d->toUnit(metaf::Distance::Unit::METERS).has_value());
	EXPECT_NEAR(d->toUnit(metaf::Distance::Unit::METERS).value(), 10058.40, margin);
}

TEST(Distance, toUnitFeetToMiles) {
	const auto d = metaf::Distance::fromHeightString("330");
	ASSERT_TRUE(d.has_value());
	ASSERT_TRUE(d->toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_NEAR(d->toUnit(metaf::Distance::Unit::STATUTE_MILES).value(), 6.25, margin);
}

TEST(Distance, toUnitFraction) {
	const auto d = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(d.has_value());
	ASSERT_TRUE(d->toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_NEAR(d->toUnit(metaf::Distance::Unit::STATUTE_MILES).value(), 0.25, margin);
}
