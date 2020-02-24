/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.1 / 2;

///////////////////////////////////////////////////////////////////////////////
// 24-hourly observation group
// Purpose: to confirm that observed 24-hourly minimum and maximum temperature 
// groups are parsed correctly, that malformed groups cannot be parsed, and 
// that 
///////////////////////////////////////////////////////////////////////////////

TEST(MinMaxTemperatureGroup, parse24hourly) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("401120084", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_24_HOURLY);
	EXPECT_TRUE(mmtg1->minimum().isReported());
	ASSERT_TRUE(mmtg1->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg1->minimum().temperature().value(), 8.4, margin);
	EXPECT_EQ(mmtg1->minimum().unit(), metaf::Temperature::Unit::C);
	EXPECT_TRUE(mmtg1->maximum().isReported());
	ASSERT_TRUE(mmtg1->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg1->maximum().temperature().value(), 11.2, margin);
	EXPECT_EQ(mmtg1->maximum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtg1->minimumTime().has_value());
	EXPECT_FALSE(mmtg1->maximumTime().has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("401001015", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_24_HOURLY);
	EXPECT_TRUE(mmtg2->minimum().isReported());
	ASSERT_TRUE(mmtg2->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg2->minimum().temperature().value(), -1.5, margin);
	EXPECT_EQ(mmtg2->minimum().unit(), metaf::Temperature::Unit::C);
	EXPECT_TRUE(mmtg2->maximum().isReported());
	ASSERT_TRUE(mmtg2->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg2->maximum().temperature().value(), 10.0, margin);
	EXPECT_EQ(mmtg2->maximum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtg2->minimumTime().has_value());
	EXPECT_FALSE(mmtg2->maximumTime().has_value());
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

	// TODO: check group's stored data
}

TEST(MinMaxTemperatureGroup, appendOtherTo24hourly) {
	//TODO
}

///////////////////////////////////////////////////////////////////////////////
// 6-hourly observation group
// Purpose: to confirm that observed 6-hourly minimum and maximum temperature 
// groups are parsed and appended correctly, and malformed groups cannot be 
// parsed or appended.
///////////////////////////////////////////////////////////////////////////////

TEST(MinMaxTemperatureGroup, parse6hourlyMinimum) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("21001", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_TRUE(mmtg1->minimum().isReported());
	ASSERT_TRUE(mmtg1->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg1->minimum().temperature().value(), -0.1, margin);
	EXPECT_EQ(mmtg1->minimum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtg1->maximum().isReported());
	EXPECT_FALSE(mmtg1->minimumTime().has_value());
	EXPECT_FALSE(mmtg1->maximumTime().has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("20012", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_TRUE(mmtg2->minimum().isReported());
	ASSERT_TRUE(mmtg2->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg2->minimum().temperature().value(), 1.2, margin);
	EXPECT_EQ(mmtg2->minimum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtg2->maximum().isReported());
	EXPECT_FALSE(mmtg2->minimumTime().has_value());
	EXPECT_FALSE(mmtg2->maximumTime().has_value());
}

TEST(MinMaxTemperatureGroup, parse6hourlyMaximum) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("11021", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_TRUE(mmtg1->maximum().isReported());
	ASSERT_TRUE(mmtg1->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg1->maximum().temperature().value(), -2.1, margin);
	EXPECT_EQ(mmtg1->maximum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtg1->minimum().isReported());
	EXPECT_FALSE(mmtg1->minimumTime().has_value());
	EXPECT_FALSE(mmtg1->maximumTime().has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("10142", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_TRUE(mmtg2->maximum().isReported());
	ASSERT_TRUE(mmtg2->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg2->maximum().temperature().value(), 14.2, margin);
	EXPECT_EQ(mmtg2->maximum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtg2->minimum().isReported());
	EXPECT_FALSE(mmtg2->minimumTime().has_value());
	EXPECT_FALSE(mmtg2->maximumTime().has_value());
}

TEST(MinMaxTemperatureGroup, parse6hourlyNotReported) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("1////", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_FALSE(mmtg1->maximum().isReported());
	EXPECT_FALSE(mmtg1->minimum().isReported());
	EXPECT_FALSE(mmtg1->minimumTime().has_value());
	EXPECT_FALSE(mmtg1->maximumTime().has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("2////", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_FALSE(mmtg2->maximum().isReported());
	EXPECT_FALSE(mmtg2->minimum().isReported());
	EXPECT_FALSE(mmtg2->minimumTime().has_value());
	EXPECT_FALSE(mmtg2->maximumTime().has_value());
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

TEST(MinMaxTemperatureGroup, append6hourlyMinMax) {
	const std::string sMax = "11021";
	const std::string sMin = "21001";

	auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse(sMin, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append(sMax, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMin->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_TRUE(mmtgMin->maximum().isReported());
	ASSERT_TRUE(mmtgMin->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->maximum().temperature().value(), -2.1, margin);
	EXPECT_EQ(mmtgMin->maximum().unit(), metaf::Temperature::Unit::C);
	EXPECT_TRUE(mmtgMin->maximum().isReported());
	ASSERT_TRUE(mmtgMin->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->minimum().temperature().value(), -0.1, margin);
	EXPECT_EQ(mmtgMin->minimum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtgMin->minimumTime().has_value());
	EXPECT_FALSE(mmtgMin->maximumTime().has_value());
}

TEST(MinMaxTemperatureGroup, append6hourlyMaxMin) {
	const std::string sMax = "11021";
	const std::string sMin = "21001";

	auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMax->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_TRUE(mmtgMax->maximum().isReported());
	ASSERT_TRUE(mmtgMax->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->maximum().temperature().value(), -2.1, margin);
	EXPECT_EQ(mmtgMax->maximum().unit(), metaf::Temperature::Unit::C);
	EXPECT_TRUE(mmtgMax->minimum().isReported());
	ASSERT_TRUE(mmtgMax->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->minimum().temperature().value(), -0.1, margin);
	EXPECT_EQ(mmtgMax->minimum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtgMax->minimumTime().has_value());
	EXPECT_FALSE(mmtgMax->maximumTime().has_value());
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
	EXPECT_EQ(mmtg->minimum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtg->minimumTime().has_value());
	EXPECT_FALSE(mmtg->maximumTime().has_value());
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
	EXPECT_EQ(mmtg->maximum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtg->minimumTime().has_value());
	EXPECT_FALSE(mmtg->maximumTime().has_value());
}

TEST(MinMaxTemperatureGroup, append6hourlyMinNotReportedMax) {
	const std::string sMax = "11021";
	const std::string sMin = "2////";

	auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse(sMin, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append(sMax, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMin->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_TRUE(mmtgMin->maximum().isReported());
	ASSERT_TRUE(mmtgMin->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->maximum().temperature().value(), -2.1, margin);
	EXPECT_EQ(mmtgMin->maximum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtgMin->minimum().isReported());
	EXPECT_FALSE(mmtgMin->minimumTime().has_value());
	EXPECT_FALSE(mmtgMin->maximumTime().has_value());

	auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMax->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_TRUE(mmtgMax->maximum().isReported());
	ASSERT_TRUE(mmtgMax->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->maximum().temperature().value(), -2.1, margin);
	EXPECT_EQ(mmtgMax->minimum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtgMax->minimum().isReported());
	EXPECT_FALSE(mmtgMax->minimumTime().has_value());
	EXPECT_FALSE(mmtgMax->maximumTime().has_value());
}

TEST(MinMaxTemperatureGroup, append6hourlyMinMaxNotRepoted) {
	const std::string sMax = "1////";
	const std::string sMin = "21001";

	auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse(sMin, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append(sMax, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMin->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_FALSE(mmtgMin->maximum().isReported());
	ASSERT_TRUE(mmtgMin->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->minimum().temperature().value(), -0.1, margin);
	EXPECT_EQ(mmtgMin->minimum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtgMin->minimumTime().has_value());
	EXPECT_FALSE(mmtgMin->maximumTime().has_value());

	auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMax->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_FALSE(mmtgMax->maximum().isReported());
	EXPECT_TRUE(mmtgMax->minimum().isReported());
	ASSERT_TRUE(mmtgMax->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->minimum().temperature().value(), -0.1, margin);
	EXPECT_EQ(mmtgMax->minimum().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(mmtgMax->minimumTime().has_value());
	EXPECT_FALSE(mmtgMax->maximumTime().has_value());
}

TEST(MinMaxTemperatureGroup, append6hourlyNotRepoted) {
	const std::string sMax = "1////";
	const std::string sMin = "2////";

	auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append(sMax, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMin->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_FALSE(mmtgMin->maximum().isReported());
	EXPECT_FALSE(mmtgMin->minimum().isReported());
	EXPECT_FALSE(mmtgMin->minimumTime().has_value());
	EXPECT_FALSE(mmtgMin->maximumTime().has_value());

	auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse(sMin, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMax->type(),
		metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY);
	EXPECT_FALSE(mmtgMax->maximum().isReported());
	EXPECT_FALSE(mmtgMax->minimum().isReported());
	EXPECT_FALSE(mmtgMax->minimumTime().has_value());
	EXPECT_FALSE(mmtgMax->maximumTime().has_value());
}

TEST(MinMaxTemperatureGroup, appendOtherTo6hourly) {
	//TODO
}

TEST(MinMaxTemperatureGroup, appendTo6hourlyMinMax) {
	//TODO
}

///////////////////////////////////////////////////////////////////////////////
// Forecast group
// Purpose: to confirm that forecast minimum and maximum temperature groups are
// parsed and appended correctly, and malformed groups cannot be parsed or 
// appended.
///////////////////////////////////////////////////////////////////////////////

TEST(MinMaxTemperatureGroup, parseForecastMinimum) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("TN03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtg1->minimum().isReported());
	ASSERT_TRUE(mmtg1->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg1->minimum().temperature().value(), 3, margin);
	EXPECT_EQ(mmtg1->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg1->minimumTime().has_value());
	ASSERT_TRUE(mmtg1->minimumTime()->day().has_value());
	EXPECT_EQ(mmtg1->minimumTime()->day().value(), 6u);
	EXPECT_EQ(mmtg1->minimumTime()->hour(), 20u);
	EXPECT_EQ(mmtg1->minimumTime()->minute(), 0u);	
	EXPECT_FALSE(mmtg1->maximum().isReported());
	EXPECT_FALSE(mmtg1->maximumTime().has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("TNM23/0617Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtg2->minimum().isReported());
	ASSERT_TRUE(mmtg2->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg2->minimum().temperature().value(), -23, margin);
	EXPECT_EQ(mmtg2->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg2->minimumTime().has_value());
	ASSERT_TRUE(mmtg2->minimumTime()->day().has_value());
	EXPECT_EQ(mmtg2->minimumTime()->day().value(), 6u);
	EXPECT_EQ(mmtg2->minimumTime()->hour(), 17u);
	EXPECT_EQ(mmtg2->minimumTime()->minute(), 0u);	
	EXPECT_FALSE(mmtg2->maximum().isReported());
	EXPECT_FALSE(mmtg2->maximumTime().has_value());
}

TEST(MinMaxTemperatureGroup, parseForecastMaximum) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("TX03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtg1->maximum().isReported());
	ASSERT_TRUE(mmtg1->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg1->maximum().temperature().value(), 3, margin);
	EXPECT_EQ(mmtg1->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg1->maximumTime().has_value());
	ASSERT_TRUE(mmtg1->maximumTime()->day().has_value());
	EXPECT_EQ(mmtg1->maximumTime()->day().value(), 6u);
	EXPECT_EQ(mmtg1->maximumTime()->hour(), 20u);
	EXPECT_EQ(mmtg1->maximumTime()->minute(), 0u);	
	EXPECT_FALSE(mmtg1->minimum().isReported());
	EXPECT_FALSE(mmtg1->minimumTime().has_value());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("TXM23/0617Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtg2->maximum().isReported());
	ASSERT_TRUE(mmtg2->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg2->maximum().temperature().value(), -23, margin);
	EXPECT_EQ(mmtg2->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg2->maximumTime().has_value());
	ASSERT_TRUE(mmtg2->maximumTime()->day().has_value());
	EXPECT_EQ(mmtg2->maximumTime()->day().value(), 6u);
	EXPECT_EQ(mmtg2->maximumTime()->hour(), 17u);
	EXPECT_EQ(mmtg2->maximumTime()->minute(), 0u);	
	EXPECT_FALSE(mmtg2->minimum().isReported());
	EXPECT_FALSE(mmtg2->minimumTime().has_value());
}

TEST(MinMaxTemperatureGroup, parseForecastUnspesified) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("T24/1306Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtg1->minimum().isReported());
	ASSERT_TRUE(mmtg1->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg1->minimum().temperature().value(), 24, margin);
	EXPECT_EQ(mmtg1->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg1->minimumTime().has_value());
	ASSERT_TRUE(mmtg1->minimumTime()->day().has_value());
	EXPECT_EQ(mmtg1->minimumTime()->day().value(), 13u);
	EXPECT_EQ(mmtg1->minimumTime()->hour(), 6u);
	EXPECT_EQ(mmtg1->minimumTime()->minute(), 0u);	
	EXPECT_TRUE(mmtg1->maximum().isReported());
	ASSERT_TRUE(mmtg1->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg1->maximum().temperature().value(), 24, margin);
	EXPECT_EQ(mmtg1->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg1->maximumTime().has_value());
	ASSERT_TRUE(mmtg1->maximumTime()->day().has_value());
	EXPECT_EQ(mmtg1->maximumTime()->day().value(), 13u);
	EXPECT_EQ(mmtg1->maximumTime()->hour(), 6u);
	EXPECT_EQ(mmtg1->maximumTime()->minute(), 0u);	

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("TM03/0911Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtg2->maximum().isReported());
	ASSERT_TRUE(mmtg2->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg2->maximum().temperature().value(), -3, margin);
	EXPECT_EQ(mmtg2->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg2->maximumTime().has_value());
	ASSERT_TRUE(mmtg2->maximumTime()->day().has_value());
	EXPECT_EQ(mmtg2->maximumTime()->day().value(), 9u);
	EXPECT_EQ(mmtg2->maximumTime()->hour(), 11u);
	EXPECT_EQ(mmtg2->maximumTime()->minute(), 0u);	
	EXPECT_TRUE(mmtg2->minimum().isReported());
	ASSERT_TRUE(mmtg2->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg2->minimum().temperature().value(), -3, margin);
	EXPECT_EQ(mmtg2->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg2->minimumTime().has_value());
	ASSERT_TRUE(mmtg2->minimumTime()->day().has_value());
	EXPECT_EQ(mmtg2->minimumTime()->day().value(), 9u);
	EXPECT_EQ(mmtg2->minimumTime()->hour(), 11u);
	EXPECT_EQ(mmtg2->minimumTime()->minute(), 0u);	
}

TEST(MinMaxTemperatureGroup, parseForecastWrongReportPart) {
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN03/0620Z", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN03/0620Z", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN03/0620Z", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN03/0620Z", metaf::ReportPart::RMK));
}

TEST(MinMaxTemperatureGroup, parseForecastWrongFormat) {
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TZ03/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"X03/0620Z", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN003/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN3/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN03/00620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN03/620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN03/0620", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN03/0620T", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"NN03/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN0A/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN03/060AZ", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN///0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TN03/////Z", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TX003/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TX3/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TX03/00620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TX03/620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TX03/0620", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TX03/0620T", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"NX03/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TX0A/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TX03/060AZ", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TX///0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"TX03/////Z", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T003/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T3/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T03/00620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T03/620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T03/0620", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T03/0620T", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T0A/0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T03/060AZ", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T///0620Z", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MinMaxTemperatureGroup::parse(
		"T03/////Z", metaf::ReportPart::TAF).has_value());
}

TEST(MinMaxTemperatureGroup, appendForecastMinMax) {
	const std::string sMax = "TX03/0620Z";
	const std::string sMin = "TNM02/0709Z";

	auto mmtgMin = metaf::MinMaxTemperatureGroup::parse(sMin, metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append(sMax, metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMin->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtgMin->maximum().isReported());
	ASSERT_TRUE(mmtgMin->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->maximum().temperature().value(), 3, margin);
	EXPECT_EQ(mmtgMin->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtgMin->maximumTime().has_value());
	ASSERT_TRUE(mmtgMin->maximumTime()->day().has_value());
	EXPECT_EQ(mmtgMin->maximumTime()->day().value(), 6u);
	EXPECT_EQ(mmtgMin->maximumTime()->hour(), 20u);
	EXPECT_EQ(mmtgMin->maximumTime()->minute(), 0u);	
	EXPECT_TRUE(mmtgMin->maximum().isReported());
	ASSERT_TRUE(mmtgMin->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->minimum().temperature().value(), -2, margin);
	EXPECT_EQ(mmtgMin->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtgMin->minimumTime().has_value());
	ASSERT_TRUE(mmtgMin->minimumTime()->day().has_value());
	EXPECT_EQ(mmtgMin->minimumTime()->day().value(), 7u);
	EXPECT_EQ(mmtgMin->minimumTime()->hour(), 9u);
	EXPECT_EQ(mmtgMin->minimumTime()->minute(), 0u);	
}

TEST(MinMaxTemperatureGroup, appendForecastMaxMin) {
	const std::string sMax = "TX03/0620Z";
	const std::string sMin = "TNM02/0709Z";

	auto mmtgMax = metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtgMax->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtgMax->maximum().isReported());
	ASSERT_TRUE(mmtgMax->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->maximum().temperature().value(), 3, margin);
	EXPECT_EQ(mmtgMax->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtgMax->maximumTime().has_value());
	ASSERT_TRUE(mmtgMax->maximumTime()->day().has_value());
	EXPECT_EQ(mmtgMax->maximumTime()->day().value(), 6u);
	EXPECT_EQ(mmtgMax->maximumTime()->hour(), 20u);
	EXPECT_EQ(mmtgMax->maximumTime()->minute(), 0u);	
	EXPECT_TRUE(mmtgMax->maximum().isReported());
	ASSERT_TRUE(mmtgMax->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->minimum().temperature().value(), -2, margin);
	EXPECT_EQ(mmtgMax->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtgMax->minimumTime().has_value());
	ASSERT_TRUE(mmtgMax->minimumTime()->day().has_value());
	EXPECT_EQ(mmtgMax->minimumTime()->day().value(), 7u);
	EXPECT_EQ(mmtgMax->minimumTime()->hour(), 9u);
	EXPECT_EQ(mmtgMax->minimumTime()->minute(), 0u);	
}

TEST(MinMaxTemperatureGroup, appendForecastMaxMax) {
	auto mmtgMax = metaf::MinMaxTemperatureGroup::parse("TX03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append("TX05/0623Z", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mmtgMax->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtgMax->maximum().isReported());
	ASSERT_TRUE(mmtgMax->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->maximum().temperature().value(), 3, margin);
	EXPECT_EQ(mmtgMax->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtgMax->maximumTime().has_value());
	ASSERT_TRUE(mmtgMax->maximumTime()->day().has_value());
	EXPECT_EQ(mmtgMax->maximumTime()->day().value(), 6u);
	EXPECT_EQ(mmtgMax->maximumTime()->hour(), 20u);
	EXPECT_EQ(mmtgMax->maximumTime()->minute(), 0u);	
	EXPECT_FALSE(mmtgMax->minimum().isReported());
	EXPECT_FALSE(mmtgMax->minimumTime().has_value());
}

TEST(MinMaxTemperatureGroup, appendForecastMinMin) {
	auto mmtgMin = metaf::MinMaxTemperatureGroup::parse("TN03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append("TN01/0623Z", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mmtgMin->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtgMin->minimum().isReported());
	ASSERT_TRUE(mmtgMin->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->minimum().temperature().value(), 3, margin);
	EXPECT_EQ(mmtgMin->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtgMin->minimumTime().has_value());
	ASSERT_TRUE(mmtgMin->minimumTime()->day().has_value());
	EXPECT_EQ(mmtgMin->minimumTime()->day().value(), 6u);
	EXPECT_EQ(mmtgMin->minimumTime()->hour(), 20u);
	EXPECT_EQ(mmtgMin->minimumTime()->minute(), 0u);
	EXPECT_FALSE(mmtgMin->maximum().isReported());
	EXPECT_FALSE(mmtgMin->maximumTime().has_value());
}

TEST(MinMaxTemperatureGroup, appendForecastMaxUnspecified) {
	auto mmtgMax = metaf::MinMaxTemperatureGroup::parse("TX03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtgMax.has_value());

	EXPECT_EQ(mmtgMax->append("T24/1306Z", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mmtgMax->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtgMax->maximum().isReported());
	ASSERT_TRUE(mmtgMax->maximum().temperature().has_value());
	EXPECT_NEAR(mmtgMax->maximum().temperature().value(), 3, margin);
	EXPECT_EQ(mmtgMax->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtgMax->maximumTime().has_value());
	ASSERT_TRUE(mmtgMax->maximumTime()->day().has_value());
	EXPECT_EQ(mmtgMax->maximumTime()->day().value(), 6u);
	EXPECT_EQ(mmtgMax->maximumTime()->hour(), 20u);
	EXPECT_EQ(mmtgMax->maximumTime()->minute(), 0u);	
	EXPECT_FALSE(mmtgMax->minimum().isReported());
	EXPECT_FALSE(mmtgMax->minimumTime().has_value());
}

TEST(MinMaxTemperatureGroup, appendForecastMinUnspecified) {
	auto mmtgMin = metaf::MinMaxTemperatureGroup::parse("TN03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtgMin.has_value());

	EXPECT_EQ(mmtgMin->append("T24/1306Z", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mmtgMin->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtgMin->minimum().isReported());
	ASSERT_TRUE(mmtgMin->minimum().temperature().has_value());
	EXPECT_NEAR(mmtgMin->minimum().temperature().value(), 3, margin);
	EXPECT_EQ(mmtgMin->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtgMin->minimumTime().has_value());
	ASSERT_TRUE(mmtgMin->minimumTime()->day().has_value());
	EXPECT_EQ(mmtgMin->minimumTime()->day().value(), 6u);
	EXPECT_EQ(mmtgMin->minimumTime()->hour(), 20u);
	EXPECT_EQ(mmtgMin->minimumTime()->minute(), 0u);
	EXPECT_FALSE(mmtgMin->maximum().isReported());
	EXPECT_FALSE(mmtgMin->maximumTime().has_value());
}

TEST(MinMaxTemperatureGroup, appendForecastMinOrMaxToUnspecified) {
	auto mmtg = metaf::MinMaxTemperatureGroup::parse("T24/1306Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg.has_value());

	EXPECT_EQ(mmtg->append("TN20/1304Z", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mmtg->append("TX28/1317Z", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);


	EXPECT_EQ(mmtg->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtg->minimum().isReported());
	ASSERT_TRUE(mmtg->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg->minimum().temperature().value(), 24, margin);
	EXPECT_EQ(mmtg->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg->minimumTime().has_value());
	ASSERT_TRUE(mmtg->minimumTime()->day().has_value());
	EXPECT_EQ(mmtg->minimumTime()->day().value(), 13u);
	EXPECT_EQ(mmtg->minimumTime()->hour(), 6u);
	EXPECT_EQ(mmtg->minimumTime()->minute(), 0u);	
	EXPECT_TRUE(mmtg->maximum().isReported());
	ASSERT_TRUE(mmtg->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg->maximum().temperature().value(), 24, margin);
	EXPECT_EQ(mmtg->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg->maximumTime().has_value());
	ASSERT_TRUE(mmtg->maximumTime()->day().has_value());
	EXPECT_EQ(mmtg->maximumTime()->day().value(), 13u);
	EXPECT_EQ(mmtg->maximumTime()->hour(), 6u);
	EXPECT_EQ(mmtg->maximumTime()->minute(), 0u);	
}

TEST(MinMaxTemperatureGroup, appendForecastUnspecifiedMinUnspecifiedMax) {
	auto mmtg = metaf::MinMaxTemperatureGroup::parse("T24/1306Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg.has_value());

	EXPECT_EQ(mmtg->append("T30/1218Z", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtg->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtg->minimum().isReported());
	ASSERT_TRUE(mmtg->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg->minimum().temperature().value(), 24, margin);
	EXPECT_EQ(mmtg->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg->minimumTime().has_value());
	ASSERT_TRUE(mmtg->minimumTime()->day().has_value());
	EXPECT_EQ(mmtg->minimumTime()->day().value(), 13u);
	EXPECT_EQ(mmtg->minimumTime()->hour(), 6u);
	EXPECT_EQ(mmtg->minimumTime()->minute(), 0u);	
	EXPECT_TRUE(mmtg->maximum().isReported());
	ASSERT_TRUE(mmtg->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg->maximum().temperature().value(), 30, margin);
	EXPECT_EQ(mmtg->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg->maximumTime().has_value());
	ASSERT_TRUE(mmtg->maximumTime()->day().has_value());
	EXPECT_EQ(mmtg->maximumTime()->day().value(), 12u);
	EXPECT_EQ(mmtg->maximumTime()->hour(), 18u);
	EXPECT_EQ(mmtg->maximumTime()->minute(), 0u);	
}

TEST(MinMaxTemperatureGroup, appendForecastUnspecifiedMaxUnspecifiedMin) {
	auto mmtg = metaf::MinMaxTemperatureGroup::parse("T30/1218Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg.has_value());

	EXPECT_EQ(mmtg->append("T24/1306Z", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mmtg->type(), metaf::MinMaxTemperatureGroup::Type::FORECAST);
	EXPECT_TRUE(mmtg->minimum().isReported());
	ASSERT_TRUE(mmtg->minimum().temperature().has_value());
	EXPECT_NEAR(mmtg->minimum().temperature().value(), 24, margin);
	EXPECT_EQ(mmtg->minimum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg->minimumTime().has_value());
	ASSERT_TRUE(mmtg->minimumTime()->day().has_value());
	EXPECT_EQ(mmtg->minimumTime()->day().value(), 13u);
	EXPECT_EQ(mmtg->minimumTime()->hour(), 6u);
	EXPECT_EQ(mmtg->minimumTime()->minute(), 0u);	
	EXPECT_TRUE(mmtg->maximum().isReported());
	ASSERT_TRUE(mmtg->maximum().temperature().has_value());
	EXPECT_NEAR(mmtg->maximum().temperature().value(), 30, margin);
	EXPECT_EQ(mmtg->maximum().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(mmtg->maximumTime().has_value());
	ASSERT_TRUE(mmtg->maximumTime()->day().has_value());
	EXPECT_EQ(mmtg->maximumTime()->day().value(), 12u);
	EXPECT_EQ(mmtg->maximumTime()->hour(), 18u);
	EXPECT_EQ(mmtg->maximumTime()->minute(), 0u);
}

TEST(MinMaxTemperatureGroup, appendOtherToForecast) {
	//TODO
}

TEST(MinMaxTemperatureGroup, appendToForecastMinMax) {
	//TODO
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(MinMaxTemperatureGroup, isValidTrue24hourly) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("401120084", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_TRUE(mmtg1->isValid());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("401001015", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_TRUE(mmtg2->isValid());
}

TEST(MinMaxTemperatureGroup, isValidFalse24hourlyInconsistentValue) {
	const auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("400840112", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_FALSE(mmtg1->isValid());

	const auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("410151014", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_FALSE(mmtg2->isValid());

	const auto mmtg3 = 
		metaf::MinMaxTemperatureGroup::parse("410000000", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg3.has_value());
	EXPECT_FALSE(mmtg3->isValid());

	const auto mmtg4 = 
		metaf::MinMaxTemperatureGroup::parse("410150100", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg4.has_value());
	EXPECT_FALSE(mmtg4->isValid());
}

TEST(MinMaxTemperatureGroup, isValidTrue6hourly) {
	const std::string sMax = "11001";
	const std::string sMin = "21021";
	const std::string sMaxNr = "1////";
	const std::string sMinNr = "2////";

	auto mmtgMax = 
		metaf::MinMaxTemperatureGroup::parse(sMax, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMax.has_value());
	EXPECT_TRUE(mmtgMax->isValid());
	EXPECT_EQ(mmtgMax->append(sMin, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mmtgMax->isValid());

	auto mmtgMin = 
		metaf::MinMaxTemperatureGroup::parse(sMin, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMin.has_value());
	EXPECT_TRUE(mmtgMin->isValid());
	EXPECT_EQ(mmtgMin->append(sMax, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mmtgMin->isValid());

	auto mmtgMaxNr = 
		metaf::MinMaxTemperatureGroup::parse(sMaxNr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMaxNr.has_value());
	EXPECT_TRUE(mmtgMaxNr->isValid());
	EXPECT_EQ(mmtgMaxNr->append(sMinNr, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mmtgMaxNr->isValid());

	auto mmtgMinNr = 
		metaf::MinMaxTemperatureGroup::parse(sMinNr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtgMinNr.has_value());
	EXPECT_TRUE(mmtgMinNr->isValid());
	EXPECT_EQ(mmtgMinNr->append(sMinNr, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mmtgMinNr->isValid());
}

TEST(MinMaxTemperatureGroup, isValidTrue6hourlyAppended) {
	auto mmtg = 
		metaf::MinMaxTemperatureGroup::parse("20004", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg.has_value());
	EXPECT_EQ(mmtg->append("10021", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mmtg->isValid());
}

TEST(MinMaxTemperatureGroup, isValidFalse6hourlyInconsistentValues) {
	auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("20010", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_EQ(mmtg1->append("11010", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(mmtg1->isValid());

	auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("20162", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_EQ(mmtg2->append("10015", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(mmtg2->isValid());

	auto mmtg3 = 
		metaf::MinMaxTemperatureGroup::parse("21022", metaf::ReportPart::RMK);
	ASSERT_TRUE(mmtg3.has_value());
	EXPECT_EQ(mmtg3->append("11074", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(mmtg3->isValid());
}

TEST(MinMaxTemperatureGroup, isValidTrueForecastSingle) {
	auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("TX03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_TRUE(mmtg1->isValid());

	auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("TNM23/0617Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_TRUE(mmtg2->isValid());
}

TEST(MinMaxTemperatureGroup, isValidTrueForecastAppended) {
	auto mmtg = 
		metaf::MinMaxTemperatureGroup::parse("TX03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg.has_value());
	EXPECT_EQ(mmtg->append("TNM23/0617Z", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mmtg->isValid());
}

TEST(MinMaxTemperatureGroup, isValidFalseForecastSingleUnspecifiedPoint) {
	auto mmtg = 
		metaf::MinMaxTemperatureGroup::parse("T03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg.has_value());
	EXPECT_FALSE(mmtg->isValid());
}

TEST(MinMaxTemperatureGroup, isValidFalseForecastInconsistentValues) {
	auto mmtg = 
		metaf::MinMaxTemperatureGroup::parse("TX03/0620Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg.has_value());
	EXPECT_EQ(mmtg->append("TN04/0608Z", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(mmtg->isValid());
}

TEST(MinMaxTemperatureGroup, isValidFalseForecastInvalidTime) {
	auto mmtg1 = 
		metaf::MinMaxTemperatureGroup::parse("TN03/0024Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg1.has_value());
	EXPECT_FALSE(mmtg1->isValid());

	auto mmtg2 = 
		metaf::MinMaxTemperatureGroup::parse("TN03/3224Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg2.has_value());
	EXPECT_FALSE(mmtg2->isValid());

	auto mmtg3 = 
		metaf::MinMaxTemperatureGroup::parse("TN03/3125Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(mmtg3.has_value());
	EXPECT_FALSE(mmtg3->isValid());
}

