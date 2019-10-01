/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

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

TEST(MinMaxTemperatureGroup, parse6hourlyNotReported) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("1////", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	EXPECT_FALSE(mmtg1->maximum().temperature().has_value());
	EXPECT_FALSE(mmtg1->minimum().temperature().has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("2////", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	EXPECT_FALSE(mmtg2->maximum().temperature().has_value());
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
}

TEST(MinMaxTemperatureGroup, append24hourly) {
	const std::string s1 = "401120084";
	const std::string s2 = "401001015";

	auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse(s1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());

	auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse(s2, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());

	EXPECT_EQ(mmtg1->append(s2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mmtg2->append(s1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}

TEST(MinMaxTemperatureGroup, append6hourlyMinMax) {
	const std::string sMax = "11021";
	const std::string sMin = "21001";

	auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse(sMin, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append(sMax, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMin->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	ASSERT_TRUE(mmtgMin->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->maximum().temperature().value(), -2.1, margin);
	ASSERT_TRUE(mmtgMin->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->minimum().temperature().value(), -0.1, margin);
}

TEST(MinMaxTemperatureGroup, append6hourlyMaxMin) {
	const std::string sMax = "11021";
	const std::string sMin = "21001";

	auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMax->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	ASSERT_TRUE(mmtgMax->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->maximum().temperature().value(), -2.1, margin);
	ASSERT_TRUE(mmtgMax->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->minimum().temperature().value(), -0.1, margin);
}

TEST(MinMaxTemperatureGroup, append6hourlyMinMin) {
	const std::string sMin1 = "21001";
	const std::string sMin2 = "20012";

	auto mmtg = 
		metaf::MinMaxTemperatureGroup::parse(sMin1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg.has_value());

	EXPECT_EQ(mmtg->append(sMin2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_FALSE(mmtg->maximum().isReported());
	ASSERT_TRUE(mmtg->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg->minimum().temperature().value(), -0.1, margin);
}

TEST(MinMaxTemperatureGroup, append6hourlyMaxMax) {
	const std::string sMax1 = "11021";
	const std::string sMax2 = "10142";

	auto mmtg = 
		metaf::MinMaxTemperatureGroup::parse(sMax1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg.has_value());

	EXPECT_EQ(mmtg->append(sMax2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_FALSE(mmtg->minimum().isReported());
	ASSERT_TRUE(mmtg->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg->maximum().temperature().value(), -2.1, margin);
}

TEST(MinMaxTemperatureGroup, append6hourlyMinNotReportedMax) {
	const std::string sMax = "11021";
	const std::string sMin = "2////";

	auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse(sMin, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append(sMax, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMin->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	ASSERT_TRUE(mmtgMin->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->maximum().temperature().value(), -2.1, margin);
	EXPECT_FALSE(mmtgMin->minimum().isReported());

	auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMax->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	ASSERT_TRUE(mmtgMax->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->maximum().temperature().value(), -2.1, margin);
	EXPECT_FALSE(mmtgMax->minimum().isReported());
}

TEST(MinMaxTemperatureGroup, append6hourlyMinMaxNotRepoted) {
	const std::string sMax = "1////";
	const std::string sMin = "21001";

	auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse(sMin, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append(sMax, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMin->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	EXPECT_FALSE(mmtgMin->maximum().isReported());
	ASSERT_TRUE(mmtgMin->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->minimum().temperature().value(), -0.1, margin);

	auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMax->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	EXPECT_FALSE(mmtgMax->maximum().isReported());
	ASSERT_TRUE(mmtgMax->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->minimum().temperature().value(), -0.1, margin);
}

TEST(MinMaxTemperatureGroup, append6hourlyNotRepoted) {
	const std::string sMax = "1////";
	const std::string sMin = "2////";

	auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append(sMax, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMin->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	EXPECT_FALSE(mmtgMin->maximum().isReported());
	EXPECT_FALSE(mmtgMin->minimum().isReported());

	auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse(sMin, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMax->observationPeriod(),
		metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6);
	EXPECT_FALSE(mmtgMax->maximum().isReported());
	EXPECT_FALSE(mmtgMax->minimum().isReported());
}

TEST(MinMaxTemperatureGroup, isValid24hourly) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("401120084", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_TRUE(mmtg1->isValid());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("401001015", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_TRUE(mmtg2->isValid());
}

TEST(MinMaxTemperatureGroup, isValid6hourly) {
	const std::string sMax = "11021";
	const std::string sMin = "21001";
	const std::string sMaxNr = "1////";
	const std::string sMinNr = "2////";

	auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());
	EXPECT_TRUE(mmtgMax->isValid());
	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mmtgMax->isValid());

	auto mmtgMaxNr = 
		metaf::MinMaxTemperatureGroup::parse(sMaxNr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMaxNr.has_value());
	EXPECT_TRUE(mmtgMaxNr->isValid());
	EXPECT_EQ(mmtgMaxNr->append(sMinNr, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mmtgMaxNr->isValid());
}
