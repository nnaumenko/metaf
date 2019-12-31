/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.01 / 2;

TEST(Distance, constructor) {
	const auto d = metaf::Distance(2, metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d.unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d.modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d.integer().has_value());
	EXPECT_EQ(d.integer().value(), 2u);
	EXPECT_FALSE(d.numerator().has_value());
	EXPECT_FALSE(d.denominator().has_value());
}

TEST(Distance, fromMeterString6500) {
	const auto d = metaf::Distance::fromMeterString("6500");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 6500u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromMeterString1000) {
	const auto d = metaf::Distance::fromMeterString("1000");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 1000u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromMeterString0400) {
	const auto d = metaf::Distance::fromMeterString("0400");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 400u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromMeterString0150) {
	const auto d = metaf::Distance::fromMeterString("0150");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 150u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromMeterString0050) {
	const auto d = metaf::Distance::fromMeterString("0050");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 50u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromMeterString0000) {
	const auto d = metaf::Distance::fromMeterString("0000");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 0u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromMeterStringNotReported) {
	const auto d = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->integer().has_value());
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromMeterString9999) {
	const auto d = metaf::Distance::fromMeterString("9999");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::MORE_THAN);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 10000u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
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

TEST(Distance, fromMileString4SM) {
	const auto d = metaf::Distance::fromMileString("3SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 3u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromMileString16SM) {
	const auto d = metaf::Distance::fromMileString("15SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 15u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromMileString0SM) {
	const auto d = metaf::Distance::fromMileString("0SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 0u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromMileString1_2SM) {
	const auto d = metaf::Distance::fromMileString("1/2SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->integer().has_value());
	ASSERT_TRUE(d->numerator().has_value());
	ASSERT_TRUE(d->denominator().has_value());
	EXPECT_EQ(d->numerator().value(), 1u);
	EXPECT_EQ(d->denominator().value(), 2u);
}

TEST(Distance, fromMileString3_16SM) {
	const auto d = metaf::Distance::fromMileString("3/16SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->integer().has_value());
	ASSERT_TRUE(d->numerator().has_value());
	ASSERT_TRUE(d->denominator().has_value());
	EXPECT_EQ(d->numerator().value(), 3u);
	EXPECT_EQ(d->denominator().value(), 16u);
}

TEST(Distance, fromMileStringM1_8SM) {
	const auto d = metaf::Distance::fromMileString("M1/8SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_FALSE(d->integer().has_value());
	ASSERT_TRUE(d->numerator().has_value());
	ASSERT_TRUE(d->denominator().has_value());
	EXPECT_EQ(d->numerator().value(), 1u);
	EXPECT_EQ(d->denominator().value(), 8u);
}

TEST(Distance, fromMileStringM1SM) {
	const auto d = metaf::Distance::fromMileString("M1SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::LESS_THAN);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	EXPECT_EQ(d->integer().value(), 1u);
}

TEST(Distance, fromMileStringP6SM) {
	const auto d = metaf::Distance::fromMileString("P6SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::MORE_THAN);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	EXPECT_EQ(d->integer().value(), 6u);
}

TEST(Distance, fromMileStringP15SM) {
	const auto d = metaf::Distance::fromMileString("P15SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::MORE_THAN);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	EXPECT_EQ(d->integer().value(), 15u);
}

TEST(Distance, fromMileStringP3_4SM) {
	const auto d = metaf::Distance::fromMileString("P3/4SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_FALSE(d->integer().has_value());
	ASSERT_TRUE(d->numerator().has_value());
	ASSERT_TRUE(d->denominator().has_value());
	EXPECT_EQ(d->numerator().value(), 3u);
	EXPECT_EQ(d->denominator().value(), 4u);
}

TEST(Distance, fromMileString21_4SM) {
	const auto d = metaf::Distance::fromMileString("21/4SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	ASSERT_TRUE(d->numerator().has_value());
	ASSERT_TRUE(d->denominator().has_value());
	EXPECT_EQ(d->integer().value(), 2u);
	EXPECT_EQ(d->numerator().value(), 1u);
	EXPECT_EQ(d->denominator().value(), 4u);
}

TEST(Distance, fromMileStringM11_2SM) {
	const auto d = metaf::Distance::fromMileString("M11/2SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::LESS_THAN);
	ASSERT_TRUE(d->integer().has_value());
	ASSERT_TRUE(d->numerator().has_value());
	ASSERT_TRUE(d->denominator().has_value());
	EXPECT_EQ(d->integer().value(), 1u);
	EXPECT_EQ(d->numerator().value(), 1u);
	EXPECT_EQ(d->denominator().value(), 2u);
}

TEST(Distance, fromMileStringNotReported) {
	const auto d = metaf::Distance::fromMileString("////SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->integer().has_value());
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
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
}

TEST(Distance, fromHeightString370) {
	const auto d = metaf::Distance::fromHeightString("370");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 37000u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromHeightString145) {
	const auto d = metaf::Distance::fromHeightString("145");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 14500u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromHeightString044) {
	const auto d = metaf::Distance::fromHeightString("044");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 4400u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromHeightString001) {
	const auto d = metaf::Distance::fromHeightString("001");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 100u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromHeightString000) {
	const auto d = metaf::Distance::fromHeightString("000");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer().value(), 0u);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
}

TEST(Distance, fromHeightStringNotReported) {
	const auto d = metaf::Distance::fromHeightString("///");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->integer().has_value());
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
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

TEST(Distance, fromRvrString3500) {
	const auto d = metaf::Distance::fromRvrString("3500", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer(), 3500u);
}

TEST(Distance, fromRvrString3500feet) {
	const auto d = metaf::Distance::fromRvrString("3500", true);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer(), 3500u);
}

TEST(Distance, fromRvrString0800) {
	const auto d = metaf::Distance::fromRvrString("0800", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer(), 800u);
}

TEST(Distance, fromRvrString0200) {
	const auto d = metaf::Distance::fromRvrString("0200", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer(), 200u);
}

TEST(Distance, fromRvrStringM0200) {
	const auto d = metaf::Distance::fromRvrString("M0200", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer(), 200u);
}

TEST(Distance, fromRvrStringP2000) {
	const auto d = metaf::Distance::fromRvrString("P2000", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer(), 2000u);
}

TEST(Distance, fromRvrString9999) {
	const auto d = metaf::Distance::fromRvrString("9999", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer(), 9999u);
}

TEST(Distance, fromRvrString0000) {
	const auto d = metaf::Distance::fromRvrString("0000", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
	ASSERT_TRUE(d->integer().has_value());
	EXPECT_EQ(d->integer(), 0u);
}

TEST(Distance, fromRvrStringNotReported) {
	const auto d = metaf::Distance::fromRvrString("////", false);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d->integer().has_value());
	EXPECT_FALSE(d->numerator().has_value());
	EXPECT_FALSE(d->denominator().has_value());
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

TEST(Distance, fromLayerString) {
	const auto d = metaf::Distance::fromLayerString("0314");
	ASSERT_TRUE(d.has_value());

	const auto d1 = std::get<0>(d.value());
	EXPECT_EQ(d1.unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d1.modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d1.integer().has_value());
	EXPECT_EQ(d1.integer().value(), 3100u);
	EXPECT_FALSE(d1.numerator().has_value());
	EXPECT_FALSE(d1.denominator().has_value());


	const auto d2 = std::get<1>(d.value());
	EXPECT_EQ(d2.unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(d2.modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(d2.integer().has_value());
	EXPECT_EQ(d2.integer().value(), 7100u);
	EXPECT_FALSE(d2.numerator().has_value());
	EXPECT_FALSE(d2.denominator().has_value());
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

TEST(Distance, fromKmString) {
	const auto d4 = metaf::Distance::fromKmString("4KM");
	ASSERT_TRUE(d4.has_value());
	EXPECT_EQ(d4->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d4->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d4->numerator().has_value());
	EXPECT_FALSE(d4->denominator().has_value());
	ASSERT_TRUE(d4->integer().has_value());
	EXPECT_EQ(d4->integer(), 4000u);

	const auto d18 = metaf::Distance::fromKmString("18KM");
	ASSERT_TRUE(d18.has_value());
	EXPECT_EQ(d18->unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d18->modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(d18->numerator().has_value());
	EXPECT_FALSE(d18->denominator().has_value());
	ASSERT_TRUE(d18->integer().has_value());
	EXPECT_EQ(d18->integer(), 18000u);
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

TEST(Distance, fromIntegerAndFraction) {
	const auto integer = metaf::Distance(2, metaf::Distance::Unit::STATUTE_MILES);
	const auto fraction = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(fraction.has_value());
	const auto combined = metaf::Distance::fromIntegerAndFraction(integer, fraction.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(combined->integer().has_value());
	EXPECT_EQ(combined->integer(), 2u);
	ASSERT_TRUE(combined->numerator().has_value());
	EXPECT_EQ(combined->numerator(), 1u);
	ASSERT_TRUE(combined->denominator().has_value());
	EXPECT_EQ(combined->denominator(), 4u);
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

TEST(Distance, cavokVisibilityMeters) {
	const auto d = metaf::Distance::cavokVisibility();
	ASSERT_TRUE(d.integer().has_value());
	EXPECT_EQ(d.integer().value(), 10000u);
	EXPECT_FALSE(d.numerator().has_value());
	EXPECT_FALSE(d.denominator().has_value());
	EXPECT_EQ(d.unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(d.modifier(), metaf::Distance::Modifier::MORE_THAN);
}

TEST(Distance, cavokVisibilityMiles) {
	const auto d = metaf::Distance::cavokVisibility(true);
	ASSERT_TRUE(d.integer().has_value());
	EXPECT_EQ(d.integer().value(), 6u);
	EXPECT_FALSE(d.numerator().has_value());
	EXPECT_FALSE(d.denominator().has_value());
	EXPECT_EQ(d.unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(d.modifier(), metaf::Distance::Modifier::MORE_THAN);
}

TEST(Distance, makeDistant) {
	const auto d = metaf::Distance::makeDistant();
	EXPECT_FALSE(d.integer().has_value());
	EXPECT_FALSE(d.numerator().has_value());
	EXPECT_FALSE(d.denominator().has_value());
	EXPECT_EQ(d.modifier(), metaf::Distance::Modifier::DISTANT);
}

TEST(Distance, makeVicinity) {
	const auto d = metaf::Distance::makeVicinity();
	EXPECT_FALSE(d.integer().has_value());
	EXPECT_FALSE(d.numerator().has_value());
	EXPECT_FALSE(d.denominator().has_value());
	EXPECT_EQ(d.modifier(), metaf::Distance::Modifier::VICINITY);
}

TEST(Distance, isIntegerTrue) {
	const auto d = metaf::Distance::fromMileString("3SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_TRUE(d->isInteger());
}

TEST(Distance, isIntegerFalseNotReported) {
	const auto d = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->isInteger());
}

TEST(Distance, isIntegerFalseFraction) {
	const auto d1 = metaf::Distance::fromMileString("1/2SM");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->isInteger());

	const auto d2 = metaf::Distance::fromMileString("11/2SM");
	ASSERT_TRUE(d2.has_value());
	ASSERT_TRUE(d2->integer().has_value());
	EXPECT_FALSE(d2->isInteger());
}

TEST(Distance, isFractionTrue) {
	const auto d = metaf::Distance::fromMileString("1/2SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_TRUE(d->isFraction());
}

TEST(Distance, isFractionFalseNotReported) {
	const auto d = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->isFraction());
}

TEST(Distance, isFractionFalseInteger) {
	const auto d1 = metaf::Distance::fromMileString("1SM");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->isFraction());

	const auto d2 = metaf::Distance::fromMileString("11/2SM");
	ASSERT_TRUE(d2.has_value());
	ASSERT_TRUE(d2->integer().has_value());
	EXPECT_FALSE(d2->isFraction());
}

TEST(Distance, isReportedTrue) {
	const auto d1 = metaf::Distance::fromMileString("3SM");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isReported());

	const auto d2 = metaf::Distance::fromMileString("1/2SM");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isReported());

	const auto d3 = metaf::Distance::fromMileString("11/2SM");
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isReported());

	const auto d4 = metaf::Distance::fromMeterString("2000");
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->isReported());

	const auto d5 = metaf::Distance::fromHeightString("030");
	ASSERT_TRUE(d5.has_value());
	EXPECT_TRUE(d5->isReported());

	const auto d6 = metaf::Distance::fromRvrString("2000", false);
	ASSERT_TRUE(d6.has_value());
	EXPECT_TRUE(d6->isReported());

	const auto d7 = metaf::Distance::makeDistant();
	EXPECT_TRUE(d7.isReported());

	const auto d8 = metaf::Distance::makeVicinity();
	EXPECT_TRUE(d8.isReported());

	const auto d9 = metaf::Distance::fromKmString("4KM");
	ASSERT_TRUE(d9.has_value());
	EXPECT_TRUE(d9->isReported());
}

TEST(Distance, isReportedFalse) {
	const auto d1 = metaf::Distance::fromMileString("////SM");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->isReported());

	const auto d2 = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d2.has_value());
	EXPECT_FALSE(d2->isReported());

	const auto d3 = metaf::Distance::fromHeightString("///");
	ASSERT_TRUE(d3.has_value());
	EXPECT_FALSE(d3->isReported());

	const auto d4 = metaf::Distance::fromRvrString("////", false);
	ASSERT_TRUE(d4.has_value());
	EXPECT_FALSE(d4->isReported());
}

TEST(Distance, isValueTrue) {
	const auto d1 = metaf::Distance::fromMileString("3SM");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isValue());

	const auto d2 = metaf::Distance::fromMileString("1/2SM");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isValue());

	const auto d3 = metaf::Distance::fromMileString("11/2SM");
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isValue());

	const auto d4 = metaf::Distance::fromMeterString("2000");
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->isValue());

	const auto d5 = metaf::Distance::fromHeightString("030");
	ASSERT_TRUE(d5.has_value());
	EXPECT_TRUE(d5->isValue());

	const auto d6 = metaf::Distance::fromRvrString("2000", false);
	ASSERT_TRUE(d6.has_value());
	EXPECT_TRUE(d6->isValue());

	const auto d7 = metaf::Distance::fromKmString("4KM");
	ASSERT_TRUE(d7.has_value());
	EXPECT_TRUE(d7->isValue());
}

TEST(Distance, isValueFalse) {
	const auto d1 = metaf::Distance::fromMileString("////SM");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->isValue());

	const auto d2 = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d2.has_value());
	EXPECT_FALSE(d2->isValue());

	const auto d3 = metaf::Distance::fromHeightString("///");
	ASSERT_TRUE(d3.has_value());
	EXPECT_FALSE(d3->isValue());

	const auto d4 = metaf::Distance::fromRvrString("////", false);
	ASSERT_TRUE(d4.has_value());
	EXPECT_FALSE(d4->isValue());

	const auto d5 = metaf::Distance::makeDistant();
	EXPECT_FALSE(d5.isValue());

	const auto d6 = metaf::Distance::makeVicinity();
	EXPECT_FALSE(d6.isValue());
}

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

TEST(Distance, isValidFalse) {
	const auto d1 = metaf::Distance::fromMileString("1/0SM");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->isValid());

	const auto d2 = metaf::Distance::fromMileString("0/2SM");
	ASSERT_TRUE(d2.has_value());
	EXPECT_FALSE(d2->isValid());

	const auto d3 = metaf::Distance::fromMileString("10/2SM");
	ASSERT_TRUE(d3.has_value());
	EXPECT_FALSE(d3->isValid());
}

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
	const auto d = metaf::Distance::fromMeterString("2000");
	ASSERT_TRUE(d.has_value());
	ASSERT_TRUE(d->toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_NEAR(d->toUnit(metaf::Distance::Unit::STATUTE_MILES).value(), 1.24, margin);
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

TEST(Distance, toUnitDenominatorZero) {
	const auto d = metaf::Distance::fromMileString("1/0SM");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->toUnit(metaf::Distance::Unit::METERS).has_value());
	EXPECT_FALSE(d->toUnit(metaf::Distance::Unit::STATUTE_MILES).has_value());
	EXPECT_FALSE(d->toUnit(metaf::Distance::Unit::FEET).has_value());
}

TEST(Distance, hasIntegerTrue) {
	const auto d1 = metaf::Distance::fromMeterString("2000");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->hasInteger());

	const auto d2 = metaf::Distance::fromMileString("8SM");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->hasInteger());

	const auto d3 = metaf::Distance::fromHeightString("020");
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->hasInteger());

	const auto d4 = metaf::Distance::fromRvrString("4500", true);
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->hasInteger());

	const auto d5 = metaf::Distance::fromMileString("11/2SM");
	ASSERT_TRUE(d5.has_value());
	EXPECT_TRUE(d5->hasInteger());
}

TEST(Distance, hasIntegerFalse) {
	const auto d1 = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->hasInteger());

	const auto d2 = metaf::Distance::fromMileString("////SM");
	ASSERT_TRUE(d2.has_value());
	EXPECT_FALSE(d2->hasInteger());

	const auto d3 = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(d3.has_value());
	EXPECT_FALSE(d3->hasInteger());
}

TEST(Distance, hasFractionTrue) {
	const auto d1 = metaf::Distance::fromMileString("11/2SM");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->hasFraction());

	const auto d2 = metaf::Distance::fromMileString("1/4SM");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->hasFraction());
}

TEST(Distance, hasFractionFalse) {
	const auto d1 = metaf::Distance::fromMeterString("2000");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->hasFraction());

	const auto d2 = metaf::Distance::fromMileString("8SM");
	ASSERT_TRUE(d2.has_value());
	EXPECT_FALSE(d2->hasFraction());

	const auto d3 = metaf::Distance::fromHeightString("020");
	ASSERT_TRUE(d3.has_value());
	EXPECT_FALSE(d3->hasFraction());

	const auto d4 = metaf::Distance::fromRvrString("4500", true);
	ASSERT_TRUE(d4.has_value());
	EXPECT_FALSE(d4->hasFraction());

	const auto d5 = metaf::Distance::fromMeterString("////");
	ASSERT_TRUE(d5.has_value());
	EXPECT_FALSE(d5->hasInteger());

	const auto d6 = metaf::Distance::fromMileString("////SM");
	ASSERT_TRUE(d6.has_value());
	EXPECT_FALSE(d6->hasInteger());
}

