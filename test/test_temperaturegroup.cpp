/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

static const auto rhMargin = 0.1;

TEST(Temperature, parseTemperatureAndDewPoint) {
	const auto tg = metaf::TemperatureGroup::parse("25/18", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg->airTemperature().temperature().has_value());
	EXPECT_EQ(tg->airTemperature().temperature().value(), 25);
	ASSERT_TRUE(tg->dewPoint().temperature().has_value());
	EXPECT_EQ(tg->dewPoint().temperature().value(), 18);
}

TEST(Temperature, parseTemperatureAndDewPointNegativeValues) {
	const auto tg1 = metaf::TemperatureGroup::parse("05/M02", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg1->airTemperature().temperature().has_value());
	EXPECT_EQ(tg1->airTemperature().temperature().value(), 5);
	ASSERT_TRUE(tg1->dewPoint().temperature().has_value());
	EXPECT_EQ(tg1->dewPoint().temperature().value(), -2);

	const auto tg2 = metaf::TemperatureGroup::parse("M02/M04", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg2->airTemperature().temperature().has_value());
	EXPECT_EQ(tg2->airTemperature().temperature().value(), -2);
	ASSERT_TRUE(tg2->dewPoint().temperature().has_value());
	EXPECT_EQ(tg2->dewPoint().temperature().value(), -4);
}

TEST(Temperature, parseTemperatureAndDewPointZero) {
	const auto tg1 = metaf::TemperatureGroup::parse("00/M02", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg1->airTemperature().temperature().has_value());
	EXPECT_EQ(tg1->airTemperature().temperature().value(), 0);
	EXPECT_FALSE(tg1->airTemperature().isFreezing());
	ASSERT_TRUE(tg1->dewPoint().temperature().has_value());
	EXPECT_EQ(tg1->dewPoint().temperature().value(), -2);
	EXPECT_TRUE(tg1->dewPoint().isFreezing());

	const auto tg2 = metaf::TemperatureGroup::parse("M00/M02", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg2->airTemperature().temperature().has_value());
	EXPECT_EQ(tg2->airTemperature().temperature().value(), 0);
	EXPECT_TRUE(tg2->airTemperature().isFreezing());
	ASSERT_TRUE(tg2->dewPoint().temperature().has_value());
	EXPECT_EQ(tg2->dewPoint().temperature().value(), -2);
	EXPECT_TRUE(tg2->dewPoint().isFreezing());

	const auto tg3 = metaf::TemperatureGroup::parse("02/M00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_EQ(tg3->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg3->airTemperature().temperature().has_value());
	EXPECT_EQ(tg3->airTemperature().temperature().value(), 2);
	EXPECT_FALSE(tg3->airTemperature().isFreezing());
	ASSERT_TRUE(tg3->dewPoint().temperature().has_value());
	EXPECT_EQ(tg3->dewPoint().temperature().value(), 0);
	EXPECT_TRUE(tg3->dewPoint().isFreezing());

	const auto tg4 = metaf::TemperatureGroup::parse("02/00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg4.has_value());
	EXPECT_EQ(tg4->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg4->airTemperature().temperature().has_value());
	EXPECT_EQ(tg4->airTemperature().temperature().value(), 2);
	EXPECT_FALSE(tg4->airTemperature().isFreezing());
	ASSERT_TRUE(tg4->dewPoint().temperature().has_value());
	EXPECT_EQ(tg4->dewPoint().temperature().value(), 0);
	EXPECT_FALSE(tg4->dewPoint().isFreezing());
}

TEST(Temperature, parseTemperatureOnly) {
	const auto tg = metaf::TemperatureGroup::parse("M70/", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg->airTemperature().temperature().has_value());
	EXPECT_EQ(tg->airTemperature().temperature().value(), -70);
	EXPECT_FALSE(tg->dewPoint().temperature().has_value());
}

TEST(Temperature, parseDewPointNotReported) {
	const auto tg = metaf::TemperatureGroup::parse("25///", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg->airTemperature().temperature().has_value());
	EXPECT_EQ(tg->airTemperature().temperature().value(), 25);
	EXPECT_FALSE(tg->dewPoint().temperature().has_value());
}

TEST(Temperature, parseNotReported) {
	const auto tg = metaf::TemperatureGroup::parse("/////", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->airTemperature().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(tg->airTemperature().temperature().has_value());
	EXPECT_FALSE(tg->dewPoint().temperature().has_value());
}

TEST(Temperature, parseWrongReportPart) {
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/18", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/18", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/18", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/18", metaf::ReportPart::RMK).has_value());
}

TEST(Temperature, parseWrongFormat) {
	EXPECT_FALSE(metaf::TemperatureGroup::parse("12:08", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("2518", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("025/18", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/018", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("025/018", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("2A/18", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/1A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25//", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("//////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/M//", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("M/////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("///M//", metaf::ReportPart::METAR).has_value());
}

TEST(Temperature, isValidNotReported) {
	const auto tg1 = metaf::TemperatureGroup::parse("M70/", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_TRUE(tg1->isValid());

	const auto tg2 = metaf::TemperatureGroup::parse("25///", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_TRUE(tg2->isValid());

	const auto tg3 = metaf::TemperatureGroup::parse("/////", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_TRUE(tg3->isValid());
}

TEST(Temperature, isValidTrue) {
	const auto tg1 = metaf::TemperatureGroup::parse("25/24", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_TRUE(tg1->isValid());

	const auto tg2 = metaf::TemperatureGroup::parse("25/25", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_TRUE(tg2->isValid());
}

TEST(Temperature, isValidTrueZero) {
	const auto tg1 = metaf::TemperatureGroup::parse("00/00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_TRUE(tg1->isValid());

	const auto tg2 = metaf::TemperatureGroup::parse("M00/M00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_TRUE(tg2->isValid());

	const auto tg3 = metaf::TemperatureGroup::parse("00/M00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_TRUE(tg3->isValid());
}

TEST(Temperature, isValidFalse) {
	const auto tg1 = metaf::TemperatureGroup::parse("25/26", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_FALSE(tg1->isValid());

	const auto tg2 = metaf::TemperatureGroup::parse("M00/00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_FALSE(tg2->isValid());
}

TEST(Temperature, relativeHumidity) {
	const auto tg1 = metaf::TemperatureGroup::parse("25/25", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	ASSERT_TRUE(tg1->isValid());
	const auto rh1 = tg1->relativeHumidity();
	EXPECT_NEAR(*rh1, 100.0, rhMargin);

	const auto tg2 = metaf::TemperatureGroup::parse("25/26", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	ASSERT_FALSE(tg2->isValid());
	const auto rh2 = tg2->relativeHumidity();
	EXPECT_NEAR(*rh2, 100.0, rhMargin);

	const auto tg3 = metaf::TemperatureGroup::parse("35/05", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());
	ASSERT_TRUE(tg3->isValid());
	const auto rh3 = tg3->relativeHumidity();
	EXPECT_NEAR(*rh3, 15.6, rhMargin);

	const auto tg4 = metaf::TemperatureGroup::parse("17/08", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg4.has_value());
	ASSERT_TRUE(tg4->isValid());
	const auto rh4 = tg4->relativeHumidity();
	EXPECT_NEAR(*rh4, 55.4, rhMargin);
}