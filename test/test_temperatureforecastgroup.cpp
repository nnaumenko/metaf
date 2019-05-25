/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(TemperatureForecastGroup, parseMinimum) {
	const auto tfg = metaf::TemperatureForecastGroup::parse("TN03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg.has_value());
	EXPECT_EQ(tfg->point(), metaf::TemperatureForecastGroup::Point::MINIMUM);
	EXPECT_EQ(tfg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tfg->airTemperature().temperature().has_value());
	EXPECT_EQ(tfg->airTemperature().temperature().value(), 3);
	ASSERT_TRUE(tfg->time().day().has_value());
	EXPECT_EQ(tfg->time().day().value(), 6u);
	EXPECT_EQ(tfg->time().hour(), 20u);
	EXPECT_EQ(tfg->time().minute(), 0u);
}

TEST(TemperatureForecastGroup, parseMinimumNegative) {
	const auto tfg = metaf::TemperatureForecastGroup::parse("TNM03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg.has_value());
	EXPECT_EQ(tfg->point(), metaf::TemperatureForecastGroup::Point::MINIMUM);
	EXPECT_EQ(tfg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tfg->airTemperature().temperature().has_value());
	EXPECT_EQ(tfg->airTemperature().temperature().value(), -3);
	ASSERT_TRUE(tfg->time().day().has_value());
	EXPECT_EQ(tfg->time().day().value(), 6u);
	EXPECT_EQ(tfg->time().hour(), 20u);
	EXPECT_EQ(tfg->time().minute(), 0u);
}

TEST(TemperatureForecastGroup, parseMaximum) {
	const auto tfg = metaf::TemperatureForecastGroup::parse("TX03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg.has_value());
	EXPECT_EQ(tfg->point(), metaf::TemperatureForecastGroup::Point::MAXIMUM);
	EXPECT_EQ(tfg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tfg->airTemperature().temperature().has_value());
	EXPECT_EQ(tfg->airTemperature().temperature().value(), 3);
	ASSERT_TRUE(tfg->time().day().has_value());
	EXPECT_EQ(tfg->time().day().value(), 6u);
	EXPECT_EQ(tfg->time().hour(), 20u);
	EXPECT_EQ(tfg->time().minute(), 0u);
}

TEST(TemperatureForecastGroup, parseMaximumNegative) {
	const auto tfg = metaf::TemperatureForecastGroup::parse("TXM03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg.has_value());
	EXPECT_EQ(tfg->point(), metaf::TemperatureForecastGroup::Point::MAXIMUM);
	EXPECT_EQ(tfg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tfg->airTemperature().temperature().has_value());
	EXPECT_EQ(tfg->airTemperature().temperature().value(), -3);
	ASSERT_TRUE(tfg->time().day().has_value());
	EXPECT_EQ(tfg->time().day().value(), 6u);
	EXPECT_EQ(tfg->time().hour(), 20u);
	EXPECT_EQ(tfg->time().minute(), 0u);
}

TEST(TemperatureForecastGroup, parseNotSpecified) {
	const auto tfg = metaf::TemperatureForecastGroup::parse("T24/1306Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg.has_value());
	EXPECT_EQ(tfg->point(), metaf::TemperatureForecastGroup::Point::NOT_SPECIFIED);
	EXPECT_EQ(tfg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tfg->airTemperature().temperature().has_value());
	EXPECT_EQ(tfg->airTemperature().temperature().value(), 24);
	ASSERT_TRUE(tfg->time().day().has_value());
	EXPECT_EQ(tfg->time().day().value(), 13u);
	EXPECT_EQ(tfg->time().hour(), 6u);
	EXPECT_EQ(tfg->time().minute(), 0u);
}

TEST(TemperatureForecastGroup, parseNotSpecifiedNegative) {
	const auto tfg = metaf::TemperatureForecastGroup::parse("TM03/0911Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg.has_value());
	EXPECT_EQ(tfg->point(), metaf::TemperatureForecastGroup::Point::NOT_SPECIFIED);
	EXPECT_EQ(tfg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tfg->airTemperature().temperature().has_value());
	EXPECT_EQ(tfg->airTemperature().temperature().value(), -3);
	ASSERT_TRUE(tfg->time().day().has_value());
	EXPECT_EQ(tfg->time().day().value(), 9u);
	EXPECT_EQ(tfg->time().hour(), 11u);
	EXPECT_EQ(tfg->time().minute(), 0u);
}

TEST(TemperatureForecastGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse("TN03/0620Z", 
		metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse("TN03/0620Z", 
		metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse("TN03/0620Z", 
		metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse("TN03/0620Z", 
		metaf::ReportPart::RMK).has_value());
}

TEST(TemperatureForecastGroup, parseWrongFormat) {
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TZ03/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"X03/0620Z", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TN003/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TN3/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TN03/00620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TN03/620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TN03/0620", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TN03/0620T", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"NN03/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TN0A/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TN03/060AZ", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TN///0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TN03/////Z", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TX003/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TX3/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TX03/00620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TX03/620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TX03/0620", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TX03/0620T", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"NX03/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TX0A/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TX03/060AZ", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TX///0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"TX03/////Z", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"T003/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"T3/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"T03/00620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"T03/620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"T03/0620", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"T03/0620T", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"T0A/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"T03/060AZ", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"T///0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureForecastGroup::parse(
		"T03/////Z", metaf::ReportPart::TAF).has_value());
}

TEST(TemperatureForecastGroup, isValidTrue) {
	const auto tfg1 = metaf::TemperatureForecastGroup::parse("TN03/3124Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg1.has_value());
	EXPECT_TRUE(tfg1->isValid());

	const auto tfg2 = metaf::TemperatureForecastGroup::parse("TN03/0100Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg2.has_value());
	EXPECT_TRUE(tfg2->isValid());
}

TEST(TemperatureForecastGroup, isValidFalse) {
	const auto tfg1 = metaf::TemperatureForecastGroup::parse("TN03/0024Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg1.has_value());
	EXPECT_FALSE(tfg1->isValid());

	const auto tfg2 = metaf::TemperatureForecastGroup::parse("TN03/3224Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg2.has_value());
	EXPECT_FALSE(tfg2->isValid());

	const auto tfg3 = metaf::TemperatureForecastGroup::parse("TN03/3125Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(tfg3.has_value());
	EXPECT_FALSE(tfg3->isValid());
}
