/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

const auto marginSunshineDuration = 1.0 / 2;

///////////////////////////////////////////////////////////////////////////////
// Sunshine duration
// Purpose: to confirm that sunshine duration groups are parsed correctly, and 
// malformed groups cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseSunshineDuration) {
	const auto mg1 = metaf::MiscGroup::parse("98096", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES);
	ASSERT_TRUE(mg1->data().has_value());
	EXPECT_NEAR(mg1->data().value(), 96, marginSunshineDuration);

	const auto mg2 = metaf::MiscGroup::parse("98000", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES);
	ASSERT_TRUE(mg1->data().has_value());
	EXPECT_NEAR(mg2->data().value(), 0, marginSunshineDuration);
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

///////////////////////////////////////////////////////////////////////////////
// Corrected observation
// Purpose: to confirm that groups which contain the number of the correctional 
// observation are parsed correctly, and malformed groups cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseCorrectedObservation) {
	const auto mg1 = metaf::MiscGroup::parse("CCA", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION);
	ASSERT_TRUE(mg1->data().has_value());
	EXPECT_EQ(static_cast<int>(mg1->data().value()), 1);

	const auto mg2 = metaf::MiscGroup::parse("CCZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION);
	ASSERT_TRUE(mg1->data().has_value());
	EXPECT_EQ(static_cast<int>(mg2->data().value()), 26);
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

///////////////////////////////////////////////////////////////////////////////
// Density altitude 
// Purpose: to confirm that density altitude groups are parsed and appended 
// correctly, and malformed density altitude groups cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseDensityAlt) {
	auto mg1 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("100FT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::DENSITY_ALTITUDE);
	ASSERT_TRUE(mg1->data().has_value());
	EXPECT_EQ(static_cast<int>(mg1->data().value()), 100);

	auto mg2 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("4700FT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::DENSITY_ALTITUDE);
	ASSERT_TRUE(mg2->data().has_value());
	EXPECT_EQ(static_cast<int>(mg2->data().value()), 4700);
}

TEST(MiscGroup, parseDensityAltMisg) {
	auto mg = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::DENSITY_ALTITUDE);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseDensityWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::TAF));
}

TEST(MiscGroup, parseDensityAltWrongFormat) {
	static const std::string g1 = "ALT";
	static const std::string g2 = "3200FT";

	EXPECT_FALSE(metaf::MiscGroup::parse(g1, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse(g1, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse(g1, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse(g1, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::MiscGroup::parse(g1, metaf::ReportPart::RMK));

	EXPECT_FALSE(metaf::MiscGroup::parse(g2, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse(g2, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse(g2, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse(g2, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::MiscGroup::parse(g2, metaf::ReportPart::RMK));
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

///////////////////////////////////////////////////////////////////////////////
// Largest hailstone size
// Purpose: to confirm that largest hailstone groups are parsed and appended 
// correctly, and malformed largest hailstone groups cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseGr) {
	auto mg1 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg1->data().has_value());
	EXPECT_EQ(mg1->data().value(), 1.75);

	auto mg2 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg2->data().has_value());
	EXPECT_EQ(mg2->data().value(), 0.5);

	auto mg3 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("1/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg3->data().has_value());
	EXPECT_EQ(mg3->data().value(), 0.25);

	auto mg4 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg4->data().has_value());
	EXPECT_EQ(mg4->data().value(), 0.75);
}

TEST(MiscGroup, parseGrWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("GR", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse("GR", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse("GR", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("GR", metaf::ReportPart::TAF));
}

TEST(MiscGroup, parseGrWrongFormat) {
	static const std::string g1 = "1/4";
	static const std::string g2 = "1";

	EXPECT_FALSE(metaf::MiscGroup::parse(g1, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse(g1, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse(g1, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse(g1, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::MiscGroup::parse(g1, metaf::ReportPart::RMK));

	EXPECT_FALSE(metaf::MiscGroup::parse(g2, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse(g2, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse(g2, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse(g2, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::MiscGroup::parse(g2, metaf::ReportPart::RMK));
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

///////////////////////////////////////////////////////////////////////////////
// Colour codes
// Purpose: to confirm that NATO colour code groups are parsed, and malformed 
// groups cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseColourCodeBlue) {
	const auto mg = metaf::MiscGroup::parse("BLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLUE);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeWhite) {
	const auto mg = metaf::MiscGroup::parse("WHT", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_WHITE);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeGreen) {
	const auto mg = metaf::MiscGroup::parse("GRN", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_GREEN);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeYellow1) {
	const auto mg = metaf::MiscGroup::parse("YLO1", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_YELLOW1);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeYellow2) {
	const auto mg = metaf::MiscGroup::parse("YLO2", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_YELLOW2);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeAmber) {
	const auto mg = metaf::MiscGroup::parse("AMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_AMBER);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeRed) {
	const auto mg = metaf::MiscGroup::parse("RED", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_RED);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeBlackBlue) {
	const auto mg = metaf::MiscGroup::parse("BLACKBLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKBLUE);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeBlackWhite) {
	const auto mg = metaf::MiscGroup::parse("BLACKWHT", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKWHITE);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeBlackGreen) {
	const auto mg = metaf::MiscGroup::parse("BLACKGRN", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKGREEN);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeBlackYellow1) {
	const auto mg = metaf::MiscGroup::parse("BLACKYLO1", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW1);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeBlackYellow2) {
	const auto mg = metaf::MiscGroup::parse("BLACKYLO2", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW2);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeBlackAmber) {
	const auto mg = metaf::MiscGroup::parse("BLACKAMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKAMBER);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseColourCodeBlackRed) {
	const auto mg = metaf::MiscGroup::parse("BLACKRED", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKRED);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseCoulourCodeWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("BLU", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLU", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLU", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLU", metaf::ReportPart::RMK).has_value());
}

TEST(MiscGroup, parseColourCodeWrongFormat) {
	EXPECT_FALSE(metaf::MiscGroup::parse("", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("ZZZZZZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLACK", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLUBLACK", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLUWHT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLUE", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLLU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLACKBLUE", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLACKBLLU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLACKBL", metaf::ReportPart::METAR).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Frost on instrument
// Purpose: to confirm that FROIN group is parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseFroin) {
	const auto mg = metaf::MiscGroup::parse("FROIN", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::FROIN);
	EXPECT_FALSE(mg->data().has_value());
}

TEST(MiscGroup, parseFroinWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("FROIN", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FROIN", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FROIN", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FROIN", metaf::ReportPart::METAR).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Miscellaenous appending tests
// Purpose: to confirm that the groups cannot append in wrong combinations
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, appendToCompleteGroups) {
	const std::string smg1 = "98096";
	const std::string smg2 = "CCA";
	const std::string smg3 = "DENSITY";
	const std::string smg4 = "ALT";
	const std::string smg5 = "1700FT";
	const std::string smg6 = "GR";
	const std::string smg7 = "1";
	const std::string smg8 = "1/4";
	const std::string smg9 = "AMB";
	const std::string smg10 = "BLACKAMB";
	const std::string smg11 = "FROIN";

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

	auto mg6 = metaf::MiscGroup::parse(smg9, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg6.has_value());

	auto mg7 = metaf::MiscGroup::parse(smg10, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg7.has_value());

	auto mg8 = metaf::MiscGroup::parse(smg11, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg8.has_value());

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
	EXPECT_EQ(mg1->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append(smg11, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg2->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append(smg11, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg3->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append(smg11, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg4->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append(smg11, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg6->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg11, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mg6->append(smg1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg11, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mg7->append(smg1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg11, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mg8->append(smg1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg11, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, isValidSunshineDuration) {
	const auto mg = metaf::MiscGroup::parse("98096", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg.has_value());
	EXPECT_TRUE(mg->isValid());
}

TEST(MiscGroup, isValidCorrection) {
	const auto mg = metaf::MiscGroup::parse("CCA", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_TRUE(mg->isValid());
}

TEST(MiscGroup, isValidDensityAltitude) {
	auto mg = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg.has_value());
	EXPECT_FALSE(mg->isValid());
	ASSERT_EQ(mg->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(mg->isValid());
	ASSERT_EQ(mg->append("3700FT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mg->isValid());
}

TEST(MiscGroup, isValidHailstoneSize) {
	auto mg1 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_FALSE(mg1->isValid());
	ASSERT_EQ(mg1->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(mg1->isValid());
	ASSERT_EQ(mg1->append("1/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mg1->isValid());

	auto mg2 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_FALSE(mg2->isValid());
	ASSERT_EQ(mg2->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(mg2->isValid());
}

TEST(MiscGroup, isValidColourCodes) {
	const auto mg1 = metaf::MiscGroup::parse("BLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_TRUE(mg1->isValid());

	const auto mg2 = metaf::MiscGroup::parse("WHT", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_TRUE(mg2->isValid());

	const auto mg3 = metaf::MiscGroup::parse("GRN", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_TRUE(mg3->isValid());

	const auto mg4 = metaf::MiscGroup::parse("YLO1", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_TRUE(mg4->isValid());

	const auto mg5 = metaf::MiscGroup::parse("YLO2", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_TRUE(mg5->isValid());

	const auto mg6 = metaf::MiscGroup::parse("AMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg6.has_value());
	EXPECT_TRUE(mg6->isValid());

	const auto mg7 = metaf::MiscGroup::parse("RED", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg7.has_value());
	EXPECT_TRUE(mg7->isValid());

	const auto mg8 = metaf::MiscGroup::parse("BLACKBLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg8.has_value());
	EXPECT_TRUE(mg8->isValid());

	const auto mg9 = metaf::MiscGroup::parse("BLACKWHT", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg9.has_value());
	EXPECT_TRUE(mg9->isValid());

	const auto mg10 = metaf::MiscGroup::parse("BLACKGRN", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg10.has_value());
	EXPECT_TRUE(mg10->isValid());

	const auto mg11 = metaf::MiscGroup::parse("BLACKYLO1", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg11.has_value());
	EXPECT_TRUE(mg11->isValid());

	const auto mg12 = metaf::MiscGroup::parse("BLACKYLO2", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg12.has_value());
	EXPECT_TRUE(mg12->isValid());

	const auto mg13 = metaf::MiscGroup::parse("BLACKAMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg13.has_value());
	EXPECT_TRUE(mg13->isValid());

	const auto mg14 = metaf::MiscGroup::parse("BLACKRED", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg14.has_value());
	EXPECT_TRUE(mg14->isValid());
}

TEST(MiscGroup, isValidFroin) {
	const auto mg1 = metaf::MiscGroup::parse("FROIN", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_TRUE(mg1->isValid());
}