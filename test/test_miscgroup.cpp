/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

const auto marginSunshineDuration = 1.0 / 2;

// Sunshine duration

TEST(MiscGroup, parseSunshineDuration) {
	const auto mg1 = metaf::MiscGroup::parse("98096", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES);
	ASSERT_TRUE(mg1->value().has_value());
	EXPECT_NEAR(mg1->value().value(), 96, marginSunshineDuration);

	const auto mg2 = metaf::MiscGroup::parse("98000", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES);
	ASSERT_TRUE(mg1->value().has_value());
	EXPECT_NEAR(mg2->value().value(), 0, marginSunshineDuration);
}

TEST(MiscGroup, parseSunshineDurationWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("98096", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse("98096", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse("98096", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("98096", metaf::ReportPart::TAF));
}

TEST(MiscGroup, parseSunshineDurationWrongFormat) {
	EXPECT_FALSE(metaf::MiscGroup::parse("98", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("98///", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("970096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("990096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("1000096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("90096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("9800096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("9A0096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("98A096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("98000A", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("18096", metaf::ReportPart::RMK));
}

// Corrected observation

TEST(MiscGroup, parseCorrectedObservation) {
	const auto mg1 = metaf::MiscGroup::parse("CCA", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION);
	ASSERT_TRUE(mg1->value().has_value());
	EXPECT_EQ(static_cast<int>(mg1->value().value()), 1);

	const auto mg2 = metaf::MiscGroup::parse("CCZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION);
	ASSERT_TRUE(mg1->value().has_value());
	EXPECT_EQ(static_cast<int>(mg2->value().value()), 26);
}

TEST(MiscGroup, parseCorrectedObservationWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("CCA", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse("CCA", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse("CCA", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::MiscGroup::parse("CCA", metaf::ReportPart::RMK));
}

TEST(MiscGroup, parseCorrectedObservationDurationWrongFormat) {
	EXPECT_FALSE(metaf::MiscGroup::parse("CC", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CC/", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CC0", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CC-", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CCAA", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CA", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("XCA", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CXA", metaf::ReportPart::METAR));
}

// Density altitude 

TEST(MiscGroup, parseDensityAlt) {
	auto mg1 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("100FT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::DENSITY_ALTITUDE);
	ASSERT_TRUE(mg1->value().has_value());
	EXPECT_EQ(static_cast<int>(mg1->value().value()), 100);

	auto mg2 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("4700FT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::DENSITY_ALTITUDE);
	ASSERT_TRUE(mg2->value().has_value());
	EXPECT_EQ(static_cast<int>(mg2->value().value()), 4700);
}

TEST(MiscGroup, parseDensityAltMisg) {
	auto mg = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::DENSITY_ALTITUDE);
	EXPECT_FALSE(mg->value().has_value());
}

TEST(MiscGroup, parseDensityWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::TAF));
}

TEST(MiscGroup, parseDensityAppendOther) {
	auto mg1 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("100FT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg2 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("4200FT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg3 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("4200", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg4 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("A", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg5 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("FT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg6 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg6.has_value());
	EXPECT_EQ(mg6->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(MiscGroup, parseDensityAltAppendOther) {
	auto mg1 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("4200", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg2 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("A", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg3 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("FT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg4 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("AFT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg5 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("AFT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

// Largest hailstone size

TEST(MiscGroup, parseGr) {
	auto mg1 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg1->value().has_value());
	EXPECT_EQ(mg1->value().value(), 1.75);

	auto mg2 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg2->value().has_value());
	EXPECT_EQ(mg2->value().value(), 0.5);

	auto mg3 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("1/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg3->value().has_value());
	EXPECT_EQ(mg3->value().value(), 0.25);

	auto mg4 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg4->value().has_value());
	EXPECT_EQ(mg4->value().value(), 0.75);
}

TEST(MiscGroup, parseGrWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("GR", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse("GR", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse("GR", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("GR", metaf::ReportPart::TAF));
}

TEST(MiscGroup, parseGrAppendOther) {
	auto mg1 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("10", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg2 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("0", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg3 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("GR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg4 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("A", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg5 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("1000FT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg6 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg6.has_value());
	EXPECT_EQ(mg6->append("1/3", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg7 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg7.has_value());
	EXPECT_EQ(mg7->append("1/5", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg8 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg8.has_value());
	EXPECT_EQ(mg8->append("7/8", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg9 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg9.has_value());
	EXPECT_EQ(mg9->append("11/16", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg10 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg10.has_value());
	EXPECT_EQ(mg10->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(MiscGroup, parseGrIntAppendOther) {
	auto mg1 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("10", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg2 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("0", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg3 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("GR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg4 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("A", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg5 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("1000FT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg6 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg6.has_value());
	EXPECT_EQ(mg6->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg6->append("1/3", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg7 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg7.has_value());
	EXPECT_EQ(mg7->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg7->append("1/5", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg8 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg8.has_value());
	EXPECT_EQ(mg8->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg8->append("7/8", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg9 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg9.has_value());
	EXPECT_EQ(mg9->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg9->append("11/16", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg10 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg10.has_value());
	EXPECT_EQ(mg10->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg10->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

// Miscellaneous

TEST(MiscGroup, appendToCompleteGroups) {
	const std::string smg1 = "98096";
	const std::string smg2 = "CCA";
	const std::string smg3 = "DENSITY";
	const std::string smg4 = "ALT";
	const std::string smg5 = "1700FT";
	const std::string smg6 = "GR";
	const std::string smg7 = "1";
	const std::string smg8 = "1/4";

	auto mg1 = metaf::MiscGroup::parse(smg1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());

	auto mg2 = metaf::MiscGroup::parse(smg2, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());

	auto mg3 = metaf::MiscGroup::parse(smg3, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(smg4, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(smg5, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	auto mg4 = metaf::MiscGroup::parse(smg6, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append(smg7, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	auto mg5 = metaf::MiscGroup::parse(smg6, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(mg1->append(smg1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append(smg2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append(smg3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append(smg4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append(smg5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append(smg6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append(smg7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mg2->append(smg1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append(smg2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append(smg3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append(smg4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append(smg5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append(smg6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append(smg7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append(smg8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append("RMK", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append("TEST", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mg3->append(smg1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append(smg2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append(smg3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append(smg4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append(smg5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append(smg6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append(smg7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mg4->append(smg1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append(smg2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append(smg3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append(smg4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append(smg5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append(smg6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append(smg7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mg5->append(smg1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append(smg2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append(smg3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append(smg4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append(smg5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append(smg6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append(smg7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}

TEST(MiscGroup, isValid) {
	const auto ssg1 = metaf::MiscGroup::parse("98096", metaf::ReportPart::RMK);
	ASSERT_TRUE(ssg1.has_value());
	EXPECT_TRUE(ssg1->isValid());

	const auto ssg2 = metaf::MiscGroup::parse("CCA", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg2.has_value());
	EXPECT_TRUE(ssg2->isValid());

	auto ssg3 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(ssg3.has_value());
	EXPECT_FALSE(ssg3->isValid());
	ASSERT_EQ(ssg3->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(ssg3->isValid());
	ASSERT_EQ(ssg3->append("3700FT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(ssg3->isValid());

	auto ssg4 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(ssg4.has_value());
	EXPECT_FALSE(ssg4->isValid());
	ASSERT_EQ(ssg4->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(ssg4->isValid());
	ASSERT_EQ(ssg4->append("1/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(ssg4->isValid());

	auto ssg5 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(ssg5.has_value());
	EXPECT_FALSE(ssg5->isValid());
	ASSERT_EQ(ssg5->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(ssg5->isValid());
}
