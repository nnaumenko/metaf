/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

static const auto margin = 0.1 / 2;

TEST(MinMaxTemperatureGroup, parse24hourly) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("401120084", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS24);
	ASSERT_TRUE(mmtg1->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg1->minimum().temperature().value(), 8.4, margin);
	ASSERT_TRUE(mmtg1->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg1->maximum().temperature().value(), 11.2, margin);

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("401001015", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS24);
	ASSERT_TRUE(mmtg2->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg2->minimum().temperature().value(), -1.5, margin);
	ASSERT_TRUE(mmtg2->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg2->maximum().temperature().value(), 10.0, margin);
}

TEST(MinMaxTemperatureGroup, parse24hourlyWrongReportPart) {
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"401120084", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"401120084", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"401120084", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"401120084", metaf::ReportPart::TAF));
}

TEST(MinMaxTemperatureGroup, parse24hourlyWrongFormat) {
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"001120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"101120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"201120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"301120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"501120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"601120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"701120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"801120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"901120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"A01120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T01120084", metaf::ReportPart::RMK));

	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"4001120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"40120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"4A1120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"4011A0084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"40112A084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"40112008A", metaf::ReportPart::RMK));

	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"421120084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"401122084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"4////0084", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"40112////", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"4////////", metaf::ReportPart::RMK));
}

TEST(MinMaxTemperatureGroup, parse6hourlyMinimum) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("21001", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	ASSERT_TRUE(mmtg1->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg1->minimum().temperature().value(), -0.1, margin);
	EXPECT_FALSE(mmtg1->maximum().temperature().has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("20012", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	ASSERT_TRUE(mmtg2->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg2->minimum().temperature().value(), 1.2, margin);
	EXPECT_FALSE(mmtg2->maximum().temperature().has_value());
}

TEST(MinMaxTemperatureGroup, parse6hourlyMaximum) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("11021", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	ASSERT_TRUE(mmtg1->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg1->maximum().temperature().value(), -2.1, margin);
	EXPECT_FALSE(mmtg1->minimum().temperature().has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("10142", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	ASSERT_TRUE(mmtg2->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg2->maximum().temperature().value(), 14.2, margin);
	EXPECT_FALSE(mmtg2->minimum().temperature().has_value());
}

TEST(MinMaxTemperatureGroup, parse6hourlyWrongReportPart) {
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"20012", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"20012", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"20012", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"20012", metaf::ReportPart::TAF));
}

TEST(MinMaxTemperatureGroup, parse6hourlyWrongFormat) {
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"00142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"30142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"40142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"50142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"60142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"70142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"80142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"90142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"A0142", metaf::ReportPart::RMK));

	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"12142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"13142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"14142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"15142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"16142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"17142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"18142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"19142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"1A142", metaf::ReportPart::RMK));

	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"100142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"1142", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"1////", metaf::ReportPart::RMK));
}

TEST(MinMaxTemperatureGroup, combine24hourly) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("401120084", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("401001015", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());

	EXPECT_FALSE(mmtg1->combine(mmtg2.value()).has_value());
	EXPECT_FALSE(mmtg2->combine(mmtg1.value()).has_value());
}

TEST(MinMaxTemperatureGroup, combine6hourlyMinMax) {
	const auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse("11021", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());

	const auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse("21001", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());

	const auto combined = mmtgMin->combine(mmtgMax.value());
	ASSERT_TRUE(combined.has_value());

	ASSERT_TRUE(std::holds_alternative<metaf::MinMaxTemperatureGroup>(combined.value()));

	const auto mmtgCombined = std::get<metaf::MinMaxTemperatureGroup>(combined.value());

	EXPECT_EQ(mmtgCombined.observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	ASSERT_TRUE(mmtgCombined.maximum().temperature().has_value());
	EXPECT_NEAR(mmtgCombined.maximum().temperature().value(), -2.1, margin);
	ASSERT_TRUE(mmtgCombined.minimum().temperature().has_value());
	EXPECT_NEAR(mmtgCombined.minimum().temperature().value(), -0.1, margin);
}

TEST(MinMaxTemperatureGroup, combine6hourlyMaxMin) {
	const auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse("11021", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());

	const auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse("21001", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());

	const auto combined = mmtgMax->combine(mmtgMin.value());
	ASSERT_TRUE(combined.has_value());

	ASSERT_TRUE(std::holds_alternative<metaf::MinMaxTemperatureGroup>(combined.value()));

	const auto mmtgCombined = std::get<metaf::MinMaxTemperatureGroup>(combined.value());

	EXPECT_EQ(mmtgCombined.observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	ASSERT_TRUE(mmtgCombined.maximum().temperature().has_value());
	EXPECT_NEAR(mmtgCombined.maximum().temperature().value(), -2.1, margin);
	ASSERT_TRUE(mmtgCombined.minimum().temperature().has_value());
	EXPECT_NEAR(mmtgCombined.minimum().temperature().value(), -0.1, margin);
}

TEST(MinMaxTemperatureGroup, combine6hourlyMinMin) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("21001", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("20012", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());

	EXPECT_FALSE(mmtg1->combine(mmtg2.value()).has_value());
	EXPECT_FALSE(mmtg2->combine(mmtg1.value()).has_value());
}

TEST(MinMaxTemperatureGroup, combine6hourlyMaxMax) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("11021", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("10142", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());

	EXPECT_FALSE(mmtg1->combine(mmtg2.value()).has_value());
	EXPECT_FALSE(mmtg2->combine(mmtg1.value()).has_value());
}
