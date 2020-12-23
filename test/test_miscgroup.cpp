/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

const auto marginSunshineDuration = 1.0 / 2;
const auto marginIssuerId = 1.0 / 2;

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
	EXPECT_FALSE(mg1->time().has_value());

	const auto mg2 = metaf::MiscGroup::parse("98000", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES);
	ASSERT_TRUE(mg1->data().has_value());
	EXPECT_NEAR(mg2->data().value(), 0, marginSunshineDuration);
	EXPECT_FALSE(mg2->time().has_value());
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
	EXPECT_FALSE(mg1->time().has_value());

	const auto mg2 = metaf::MiscGroup::parse("CCZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION);
	ASSERT_TRUE(mg1->data().has_value());
	EXPECT_EQ(static_cast<int>(mg2->data().value()), 26);
	EXPECT_FALSE(mg2->time().has_value());
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
	EXPECT_FALSE(mg1->time().has_value());

	auto mg2 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("4700FT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::DENSITY_ALTITUDE);
	ASSERT_TRUE(mg2->data().has_value());
	EXPECT_EQ(static_cast<int>(mg2->data().value()), 4700);
	EXPECT_FALSE(mg2->time().has_value());
}

TEST(MiscGroup, parseDensityAltMisg) {
	auto mg = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::DENSITY_ALTITUDE);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
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
	EXPECT_FALSE(mg1->time().has_value());

	auto mg2 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg2->data().has_value());
	EXPECT_EQ(mg2->data().value(), 0.5);
	EXPECT_FALSE(mg2->time().has_value());

	auto mg3 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("1/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg3->data().has_value());
	EXPECT_EQ(mg3->data().value(), 0.25);
	EXPECT_FALSE(mg3->time().has_value());

	auto mg4 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->type(), metaf::MiscGroup::Type::HAILSTONE_SIZE);
	ASSERT_TRUE(mg4->data().has_value());
	EXPECT_EQ(mg4->data().value(), 0.75);
	EXPECT_FALSE(mg4->time().has_value());
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

TEST(MiscGroup, parseColourCodeBluePlus) {
	const auto mg = metaf::MiscGroup::parse("BLU+", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLUE_PLUS);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeBlue) {
	const auto mg = metaf::MiscGroup::parse("BLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLUE);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeWhite) {
	const auto mg = metaf::MiscGroup::parse("WHT", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_WHITE);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeGreen) {
	const auto mg = metaf::MiscGroup::parse("GRN", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_GREEN);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeYellow) {
	const auto mg = metaf::MiscGroup::parse("YLO", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_YELLOW);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeYellow1) {
	const auto mg = metaf::MiscGroup::parse("YLO1", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_YELLOW1);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeYellow2) {
	const auto mg = metaf::MiscGroup::parse("YLO2", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_YELLOW2);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeAmber) {
	const auto mg = metaf::MiscGroup::parse("AMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_AMBER);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeRed) {
	const auto mg = metaf::MiscGroup::parse("RED", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_RED);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeBlackBluePlus) {
	const auto mg = metaf::MiscGroup::parse("BLACKBLU+", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKBLUE_PLUS);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeBlackBlue) {
	const auto mg = metaf::MiscGroup::parse("BLACKBLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKBLUE);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeBlackWhite) {
	const auto mg = metaf::MiscGroup::parse("BLACKWHT", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKWHITE);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeBlackGreen) {
	const auto mg = metaf::MiscGroup::parse("BLACKGRN", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKGREEN);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeBlackYellow) {
	const auto mg = metaf::MiscGroup::parse("BLACKYLO", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeBlackYellow1) {
	const auto mg = metaf::MiscGroup::parse("BLACKYLO1", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW1);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeBlackYellow2) {
	const auto mg = metaf::MiscGroup::parse("BLACKYLO2", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW2);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeBlackAmber) {
	const auto mg = metaf::MiscGroup::parse("BLACKAMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKAMBER);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseColourCodeBlackRed) {
	const auto mg = metaf::MiscGroup::parse("BLACKRED", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::COLOUR_CODE_BLACKRED);
	EXPECT_FALSE(mg->data().has_value());
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseCoulourCodeRmk) {
	EXPECT_TRUE(metaf::MiscGroup::parse("BLU+", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("BLU", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("WHT", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("GRN", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("YLO", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("YLO1", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("YLO2", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("AMB", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("RED", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("BLACKBLU+", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("BLACKBLU", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("BLACKWHT", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("BLACKGRN", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("BLACKYLO", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("BLACKYLO1", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("BLACKYLO2", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("BLACKAMB", metaf::ReportPart::RMK).has_value());
	EXPECT_TRUE(metaf::MiscGroup::parse("BLACKRED", metaf::ReportPart::RMK).has_value());
}

TEST(MiscGroup, parseCoulourCodeWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("BLU", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLU", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("BLU", metaf::ReportPart::TAF).has_value());
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

	EXPECT_FALSE(metaf::MiscGroup::parse("WHT+", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("GRN+", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("YLO+", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("YLO1+", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("YLO2+", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("AMB+", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("RED+", metaf::ReportPart::METAR).has_value());
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
	EXPECT_FALSE(mg->time().has_value());
}

TEST(MiscGroup, parseFroinWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("FROIN", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FROIN", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FROIN", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FROIN", metaf::ReportPart::METAR).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Issuer ID
// Purpose: to confirm that issuer id groups are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseIssuerIdFn) {
	const auto mg = metaf::MiscGroup::parse("FN20002", metaf::ReportPart::TAF);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::ISSUER_ID_FN);
	EXPECT_TRUE(mg->data().has_value());
	EXPECT_NEAR(mg->data().value(), 20002.0, marginIssuerId);
}

TEST(MiscGroup, parseIssuerIdFs) {
	const auto mg = metaf::MiscGroup::parse("FS30122", metaf::ReportPart::TAF);
	ASSERT_TRUE(mg.has_value());
	EXPECT_EQ(mg->type(), metaf::MiscGroup::Type::ISSUER_ID_FS);
	EXPECT_TRUE(mg->data().has_value());
	EXPECT_NEAR(mg->data().value(), 30122.0, marginIssuerId);
}

TEST(MiscGroup, parseIssuerIdWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("FN20002", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FN20002", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FN20002", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FN20002", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::MiscGroup::parse("FS30122", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FS30122", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FS30122", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FS30122", metaf::ReportPart::METAR).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// First report
// Purpose: to confirm that group FIRST is passed correctly and no group can
// be appended to it.
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseFirst) {
	const auto mg1 = metaf::MiscGroup::parse("FIRST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::FIRST);
	EXPECT_FALSE(mg1->data().has_value());
	EXPECT_FALSE(mg1->time().has_value());

	const auto mg2 = metaf::MiscGroup::parse("FIRST", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::FIRST);
	EXPECT_FALSE(mg2->data().has_value());
	EXPECT_FALSE(mg2->time().has_value());

	const auto mg3 = metaf::MiscGroup::parse("FIRST", metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::FIRST);
	EXPECT_FALSE(mg3->data().has_value());
	EXPECT_FALSE(mg3->time().has_value());
}

TEST(MiscGroup, parseFirstWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("FIRST", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FIRST", metaf::ReportPart::HEADER).has_value());
}

TEST(MiscGroup, appendFirst) {
	auto mg1 = metaf::MiscGroup::parse("FIRST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("NEXT", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto mg2 = metaf::MiscGroup::parse("FIRST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("192046Z", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto mg3 = metaf::MiscGroup::parse("FIRST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("FIRST", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// Last report
// Purpose: to confirm that groups indicating last report, last staffed
// observation, optionally with next scheduled report time, are parsed and 
// appended correctly.
// Note 1/4: The group formats are LAST, LAST OBS, LAST STFD OBS, optionally 
// followed by NEXT and time of the next scheduled report.
// Note 2/4: after OBS and NEXT character / may be used as delimiter, for 
// example: LAST OBS/NEXT xxxxZ or LAST OBS/NEXT/xxxxZ.
// Note 3/4: OBS may be substituted with OBS., OB, OBS. and NEXT may be 
// substituted with NXT. The parsing result is the same.
// Note 4/4: the following day and time formats are recognised for the next
// scheduled report time: DDHH, DDHHZ, DDHHMMZ, DDHHMM, DD/HHMM.
///////////////////////////////////////////////////////////////////////////////

// TODO

///////////////////////////////////////////////////////////////////////////////
// Next report
// Purpose: to confirm that groups NEXT and NXT which specify next scheduled 
// report time (without preceeding LAST, LAST OBS, or LAST STFD OBS), are 
// parsed and appended correctly.
// Note: the format is NEXT or NXT followed by any of the following day and 
// time formats: DDHH, DDHHZ, DDHHMMZ, DDHHMM, DD/HHMM. Between NEXT or NXT
// and time delimiter character / may be used, for example NEXT/2807.
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseNext) {
	static const auto nextStr = std::string("NEXT"); 
	static const auto timeStr = std::string("2009"); 

	auto mg1 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 9u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(timeStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 0u);

	auto mg3 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(timeStr, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg3->time()->minute(), 0u);
}

TEST(MiscGroup, parseNxt) {
	static const auto nextStr = std::string("NXT"); 
	static const auto timeStr = std::string("2009"); 

	auto mg1 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 9u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(timeStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 0u);

	auto mg3 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(timeStr, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg3->time()->minute(), 0u);
}

TEST(MiscGroup, parseNextWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("NEXT", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("NEXT", metaf::ReportPart::HEADER).has_value());

	EXPECT_FALSE(metaf::MiscGroup::parse("NXT", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("NXT", metaf::ReportPart::HEADER).has_value());

	EXPECT_FALSE(metaf::MiscGroup::parse("NEXT/2009", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("NXT/2009", metaf::ReportPart::HEADER).has_value());
}

TEST(MiscGroup, parseNextTimeFormats) {
	static const auto nextStr = std::string("NEXT"); 

	auto mg1 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("2009", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 9u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("2009Z", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 0u);

	auto mg3 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("200943", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg3->time()->minute(), 43u);

	auto mg4 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("200943Z", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg4->data().has_value());
	ASSERT_TRUE(mg4->time().has_value());
	ASSERT_TRUE(mg4->time()->day().has_value());
	EXPECT_EQ(mg4->time()->day().value(), 20u);
	EXPECT_EQ(mg4->time()->hour(), 9u);
	EXPECT_EQ(mg4->time()->minute(), 43u);

	auto mg5 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("20/0943", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg5->data().has_value());
	ASSERT_TRUE(mg5->time().has_value());
	ASSERT_TRUE(mg5->time()->day().has_value());
	EXPECT_EQ(mg5->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg5->time()->minute(), 43u);
}

TEST(MiscGroup, parseNextSlashDelimiter) {
	static const auto nextStr = std::string("NEXT/2009"); 
	static const auto timeStr = std::string("2009"); 

	auto mg1 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 9u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(nextStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 0u);

	auto mg3 = metaf::MiscGroup::parse("NXT/2009", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg3->time()->minute(), 0u);
}

// TODO: append wrong groups, append to complete groups

///////////////////////////////////////////////////////////////////////////////
// No amendments after
// Purpose: to confirm that group sequence NO AMDS AFT followed by time are 
// parsed and appended correctly.
// Note: the day and time formats are: DDHH, DDHHZ, DDHHMMZ, DDHHMM, DD/HHMM, 
// also HHMM may be used. See MetafTime tests for additional tests which 
// confirm that formats DDHH and HHMM are distinguished correctly.
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseNoAmdsAft) {
	static const auto groupStr1 = std::string("NO");
	static const auto groupStr2 = std::string("AMDS");
	static const auto groupStr3 = std::string("AFT");
	static const auto timeStr = std::string("201041");
	static const auto groupType = metaf::MiscGroup::Type::NO_AMENDMENTS_AFTER;

	auto mg1 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), groupType);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 10u);
	EXPECT_EQ(mg1->time()->minute(), 41u);

	auto mg2 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(groupStr2, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr3, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(timeStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), groupType);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 10u);
	EXPECT_EQ(mg2->time()->minute(), 41u);

	auto mg3 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(groupStr2, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr3, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(timeStr, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), groupType);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 10u);
	EXPECT_EQ(mg3->time()->minute(), 41u);
}

TEST(MiscGroup, parseNoAmdsAftTimeFormats) {
	static const auto groupStr1 = std::string("NO");
	static const auto groupStr2 = std::string("AMDS");
	static const auto groupStr3 = std::string("AFT");
	static const auto groupType = metaf::MiscGroup::Type::NO_AMENDMENTS_AFTER;

	auto mg1 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("2810", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), groupType);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 28u);
	EXPECT_EQ(mg1->time()->hour(), 10u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("0945", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), groupType);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	EXPECT_FALSE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 45u);

	auto mg3 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("2010Z", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), groupType);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 10u);
	EXPECT_EQ(mg3->time()->minute(), 0u);

	auto mg4 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("200945Z", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->type(), groupType);
	EXPECT_FALSE(mg4->data().has_value());
	ASSERT_TRUE(mg4->time().has_value());
	ASSERT_TRUE(mg4->time()->day().has_value());
	EXPECT_EQ(mg4->time()->day().value(), 20u);
	EXPECT_EQ(mg4->time()->hour(), 9u);
	EXPECT_EQ(mg4->time()->minute(), 45u);

	auto mg5 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("200945", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->type(), groupType);
	EXPECT_FALSE(mg5->data().has_value());
	ASSERT_TRUE(mg5->time().has_value());
	ASSERT_TRUE(mg5->time()->day().has_value());
	EXPECT_EQ(mg5->time()->day().value(), 20u);
	EXPECT_EQ(mg5->time()->hour(), 9u);
	EXPECT_EQ(mg5->time()->minute(), 45u);

	auto mg6 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg6.has_value());
	EXPECT_EQ(mg6->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg6->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg6->append("20/0945", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg6->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->type(), groupType);
	EXPECT_FALSE(mg6->data().has_value());
	ASSERT_TRUE(mg6->time().has_value());
	ASSERT_TRUE(mg6->time()->day().has_value());
	EXPECT_EQ(mg6->time()->day().value(), 20u);
	EXPECT_EQ(mg6->time()->hour(), 9u);
	EXPECT_EQ(mg6->time()->minute(), 45u);
}

TEST(MiscGroup, parseNoAmdsAftWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("NO", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("NO", metaf::ReportPart::HEADER).has_value());
}

// TODO: append wrong groups, append to complete groups

///////////////////////////////////////////////////////////////////////////////
// Amended time
// Purpose: to confirm that group AMD followed by amendment time is parsed and 
// appended correctly.
// Note: the day and time formats are: DDHH, DDHHZ, DDHHMMZ, DDHHMM, DD/HHMM, 
// also HHMM may be used. See MetafTime tests for additional tests which 
// confirm that formats DDHH and HHMM are distinguished correctly.
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseAmd) {
	static const auto groupStr = std::string("AMD");
	static const auto timeStr = std::string("201041");
	static const auto groupType = metaf::MiscGroup::Type::AMENDED_AT;

	auto mg1 = metaf::MiscGroup::parse(groupStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), groupType);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 10u);
	EXPECT_EQ(mg1->time()->minute(), 41u);

	auto mg2 = metaf::MiscGroup::parse(groupStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(timeStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), groupType);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 10u);
	EXPECT_EQ(mg2->time()->minute(), 41u);

	auto mg3 = metaf::MiscGroup::parse(groupStr, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(timeStr, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), groupType);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 10u);
	EXPECT_EQ(mg3->time()->minute(), 41u);
}

TEST(MiscGroup, parseAmdTimeFormats) {
	static const auto groupStr = std::string("AMD");
	static const auto groupType = metaf::MiscGroup::Type::AMENDED_AT;

	auto mg1 = metaf::MiscGroup::parse(groupStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("2810", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), groupType);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 28u);
	EXPECT_EQ(mg1->time()->hour(), 10u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(groupStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("0945", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), groupType);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	EXPECT_FALSE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 45u);

	auto mg3 = metaf::MiscGroup::parse(groupStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("2010Z", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), groupType);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 10u);
	EXPECT_EQ(mg3->time()->minute(), 0u);

	auto mg4 = metaf::MiscGroup::parse(groupStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("200945Z", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->type(), groupType);
	EXPECT_FALSE(mg4->data().has_value());
	ASSERT_TRUE(mg4->time().has_value());
	ASSERT_TRUE(mg4->time()->day().has_value());
	EXPECT_EQ(mg4->time()->day().value(), 20u);
	EXPECT_EQ(mg4->time()->hour(), 9u);
	EXPECT_EQ(mg4->time()->minute(), 45u);

	auto mg5 = metaf::MiscGroup::parse(groupStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("200945", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->type(), groupType);
	EXPECT_FALSE(mg5->data().has_value());
	ASSERT_TRUE(mg5->time().has_value());
	ASSERT_TRUE(mg5->time()->day().has_value());
	EXPECT_EQ(mg5->time()->day().value(), 20u);
	EXPECT_EQ(mg5->time()->hour(), 9u);
	EXPECT_EQ(mg5->time()->minute(), 45u);

	auto mg6 = metaf::MiscGroup::parse(groupStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg6.has_value());
	EXPECT_EQ(mg6->append("20/0945", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg6->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->type(), groupType);
	EXPECT_FALSE(mg6->data().has_value());
	ASSERT_TRUE(mg6->time().has_value());
	ASSERT_TRUE(mg6->time()->day().has_value());
	EXPECT_EQ(mg6->time()->day().value(), 20u);
	EXPECT_EQ(mg6->time()->hour(), 9u);
	EXPECT_EQ(mg6->time()->minute(), 45u);
}

TEST(MiscGroup, parseAmdWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("AMD", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("AMD", metaf::ReportPart::HEADER).has_value());
}

// TODO: append wrong groups, append to complete groups

///////////////////////////////////////////////////////////////////////////////
// Next forecast
// Purpose: to confirm that group sequences NEXT FCST BY, NEXT FCST AT, 
// NEXT FCST, and NEXT FCST WILL BE ISSUED AT, followed by time are parsed and 
// appended correctly. Time formats are: DDHH, DDHHZ, DDHHMMZ, DDHHMM, DD/HHMM.
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseNextFcstBy) {
	static const auto groupStr1 = std::string("NEXT"); 
	static const auto groupStr2 = std::string("FCST"); 
	static const auto groupStr3 = std::string("BY"); 
	static const auto timeStr = std::string("2009"); 

	auto mg1 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 9u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(groupStr2, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr3, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(timeStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 0u);

	auto mg3 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(groupStr2, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr3, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(timeStr, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg3->time()->minute(), 0u);
}

TEST(MiscGroup, parseNextFcstAt) {
	static const auto groupStr1 = std::string("NEXT"); 
	static const auto groupStr2 = std::string("FCST"); 
	static const auto groupStr3 = std::string("AT"); 
	static const auto timeStr = std::string("2009"); 

	auto mg1 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 9u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(groupStr2, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr3, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(timeStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 0u);

	auto mg3 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(groupStr2, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr3, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(timeStr, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg3->time()->minute(), 0u);
}

TEST(MiscGroup, parseNxtFcstBy) {
	static const auto groupStr1 = std::string("NXT"); 
	static const auto groupStr2 = std::string("FCST"); 
	static const auto groupStr3 = std::string("BY"); 
	static const auto timeStr = std::string("2009"); 

	auto mg1 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 9u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(groupStr2, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr3, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(timeStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 0u);

	auto mg3 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(groupStr2, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr3, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(timeStr, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg3->time()->minute(), 0u);
}

TEST(MiscGroup, parseNxtFcstAt) {
	static const auto groupStr1 = std::string("NXT"); 
	static const auto groupStr2 = std::string("FCST"); 
	static const auto groupStr3 = std::string("AT"); 
	static const auto timeStr = std::string("2009"); 

	auto mg1 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 9u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(groupStr2, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr3, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(timeStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 0u);

	auto mg3 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(groupStr2, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr3, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(timeStr, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg3->time()->minute(), 0u);
}

TEST(MiscGroup, parseNextFcstWillBeReleasedAt) {
	static const auto groupStr1 = std::string("NEXT"); 
	static const auto groupStr2 = std::string("FCST"); 
	static const auto groupStr3 = std::string("WILL"); 
	static const auto groupStr4 = std::string("BE"); 
	static const auto groupStr5 = std::string("ISSUED"); 
	static const auto groupStr6 = std::string("AT");
	static const auto timeStr = std::string("231657Z"); 

	auto mg1 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr4, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr5, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr6, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 23u);
	EXPECT_EQ(mg1->time()->hour(), 16u);
	EXPECT_EQ(mg1->time()->minute(), 57u);

	auto mg2 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(groupStr2, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr3, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr4, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr5, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr6, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(timeStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 23u);
	EXPECT_EQ(mg2->time()->hour(), 16u);
	EXPECT_EQ(mg2->time()->minute(), 57u);

	auto mg3 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(groupStr2, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr3, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr4, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr5, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr6, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(timeStr, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 23u);
	EXPECT_EQ(mg3->time()->hour(), 16u);
	EXPECT_EQ(mg3->time()->minute(), 57u);
}

TEST(MiscGroup, parseNxtFcstWillBeReleasedAt) {
	static const auto groupStr1 = std::string("NXT"); 
	static const auto groupStr2 = std::string("FCST"); 
	static const auto groupStr3 = std::string("WILL"); 
	static const auto groupStr4 = std::string("BE"); 
	static const auto groupStr5 = std::string("ISSUED"); 
	static const auto groupStr6 = std::string("AT");
	static const auto timeStr = std::string("231657Z"); 

	auto mg1 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr4, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr5, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr6, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(timeStr, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 23u);
	EXPECT_EQ(mg1->time()->hour(), 16u);
	EXPECT_EQ(mg1->time()->minute(), 57u);

	auto mg2 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(groupStr2, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr3, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr4, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr5, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr6, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(timeStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 23u);
	EXPECT_EQ(mg2->time()->hour(), 16u);
	EXPECT_EQ(mg2->time()->minute(), 57u);

	auto mg3 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(groupStr2, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr3, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr4, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr5, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr6, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(timeStr, metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 23u);
	EXPECT_EQ(mg3->time()->hour(), 16u);
	EXPECT_EQ(mg3->time()->minute(), 57u);
}

TEST(MiscGroup, parseNextFcstWrongReportPart) {
	// This test largely repeats parseNextWrongReportPart
	EXPECT_FALSE(metaf::MiscGroup::parse("NEXT", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("NEXT", metaf::ReportPart::HEADER).has_value());

	EXPECT_FALSE(metaf::MiscGroup::parse("NXT", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("NXT", metaf::ReportPart::HEADER).has_value());
}

TEST(MiscGroup, parseNextFcstTimeFormats) {
	static const auto groupStr1 = std::string("NEXT"); 
	static const auto groupStr2 = std::string("FCST"); 
	static const auto groupStr3 = std::string("BY"); 

	auto mg1 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("2009", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg1->data().has_value());
	ASSERT_TRUE(mg1->time().has_value());
	ASSERT_TRUE(mg1->time()->day().has_value());
	EXPECT_EQ(mg1->time()->day().value(), 20u);
	EXPECT_EQ(mg1->time()->hour(), 9u);
	EXPECT_EQ(mg1->time()->minute(), 0u);

	auto mg2 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("2009Z", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg2->data().has_value());
	ASSERT_TRUE(mg2->time().has_value());
	ASSERT_TRUE(mg2->time()->day().has_value());
	EXPECT_EQ(mg2->time()->day().value(), 20u);
	EXPECT_EQ(mg2->time()->hour(), 9u);
	EXPECT_EQ(mg2->time()->minute(), 0u);

	auto mg3 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("200943", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg3->data().has_value());
	ASSERT_TRUE(mg3->time().has_value());
	ASSERT_TRUE(mg3->time()->day().has_value());
	EXPECT_EQ(mg3->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg3->time()->minute(), 43u);

	auto mg4 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("200943Z", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg4->data().has_value());
	ASSERT_TRUE(mg4->time().has_value());
	ASSERT_TRUE(mg4->time()->day().has_value());
	EXPECT_EQ(mg4->time()->day().value(), 20u);
	EXPECT_EQ(mg4->time()->hour(), 9u);
	EXPECT_EQ(mg4->time()->minute(), 43u);

	auto mg5 = metaf::MiscGroup::parse(groupStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append(groupStr2, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append(groupStr3, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("20/0943", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->type(), metaf::MiscGroup::Type::NEXT_FORECAST);
	EXPECT_FALSE(mg5->data().has_value());
	ASSERT_TRUE(mg5->time().has_value());
	ASSERT_TRUE(mg5->time()->day().has_value());
	EXPECT_EQ(mg5->time()->day().value(), 20u);
	EXPECT_EQ(mg3->time()->hour(), 9u);
	EXPECT_EQ(mg5->time()->minute(), 43u);
}

// TODO: append wrong groups, append to complete groups NEXT FCST BY, NEXT FCST AT,
// NEXT FCST WILL BE ISSUED AT

///////////////////////////////////////////////////////////////////////////////
// Forecast based on auto observation
// Purpose: to confirm that group sequence FCST BASED ON AUTO OBS are parsed 
// and appended correctly. 
///////////////////////////////////////////////////////////////////////////////

TEST(MiscGroup, parseFcstBasedOnAutoObs) {
	auto mg1 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::FORECAST_BASED_ON_AUTO_OBSERVATION);
	EXPECT_FALSE(mg1->data().has_value());
	EXPECT_FALSE(mg1->time().has_value());

	auto mg2 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("BASED", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("ON", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("AUTO", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("OBS", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::FORECAST_BASED_ON_AUTO_OBSERVATION);
	EXPECT_FALSE(mg2->data().has_value());
	EXPECT_FALSE(mg2->time().has_value());

	auto mg3 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::TAF);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("BASED", metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("ON", metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("AUTO", metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("OBS", metaf::ReportPart::TAF), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->type(), metaf::MiscGroup::Type::FORECAST_BASED_ON_AUTO_OBSERVATION);
	EXPECT_FALSE(mg3->data().has_value());
	EXPECT_FALSE(mg3->time().has_value());
}

TEST(MiscGroup, parseFcstBasedOnAutoObsWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("FCST", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::MiscGroup::parse("FCST", metaf::ReportPart::HEADER).has_value());
}

TEST(MiscGroup, parseFcstBasedOnAutoObsAppendFcst) {
	auto mg1 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("FCST", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg2 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg3 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg4 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg5 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(MiscGroup, parseFcstBasedOnAutoObsAppendFcstBased) {
	auto mg1 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg2 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	EXPECT_EQ(mg2->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg3 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg4 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg5 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(MiscGroup, parseFcstBasedOnAutoObsAppendFcstBasedOn) {
	auto mg1 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("FCST", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg2 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	EXPECT_EQ(mg2->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg3 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg4 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg5 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(MiscGroup, parseFcstBasedOnAutoObsAppendFcstBasedOnAuto) {
	auto mg1 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("FCST", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg2 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg3 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg4 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto mg5 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(MiscGroup, parseFcstBasedOnAutoObsAppendToCompleteGroup) {
	auto mg1 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto mg2 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg2->append("120700Z", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto mg3 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg3.has_value());
	EXPECT_EQ(mg3->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg3->append("NEXT", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto mg4 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg4.has_value());
	EXPECT_EQ(mg4->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg4->append("FCST", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto mg5 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg5.has_value());
	EXPECT_EQ(mg5->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg5->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto mg6 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg6.has_value());
	EXPECT_EQ(mg6->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg6->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg6->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg6->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg6->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto mg7 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg7.has_value());
	EXPECT_EQ(mg7->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg7->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg7->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg7->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg7->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto mg8 = metaf::MiscGroup::parse("FCST", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg8.has_value());
	EXPECT_EQ(mg8->append("BASED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg8->append("ON", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg8->append("AUTO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg8->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(mg8->append("OBS", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
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
	const std::string smg12 = "FS49728";
	const std::string smg13 = "FN07053";

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

	auto mg9 = metaf::MiscGroup::parse(smg12, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg9.has_value());

	auto mg10 = metaf::MiscGroup::parse(smg13, metaf::ReportPart::TAF);
	ASSERT_TRUE(mg10.has_value());

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
	EXPECT_EQ(mg1->append(smg12, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg1->append(smg13, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg2->append(smg12, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg2->append(smg13, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg3->append(smg12, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg3->append(smg13, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg4->append(smg12, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg4->append(smg13, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg5->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg11, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append(smg12, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg5->append(smg13, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg6->append(smg12, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg6->append(smg13, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg7->append(smg12, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg7->append(smg13, metaf::ReportPart::RMK), 
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
	EXPECT_EQ(mg8->append(smg12, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append(smg13, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg8->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mg9->append(smg1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg11, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg12, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append(smg13, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg9->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(mg10->append(smg1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg10, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg11, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg12, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append(smg13, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(mg10->append("TEST", metaf::ReportPart::RMK), 
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

// TODO: isValid() tests for IssuerId
// TODO: isValid() tests for FIRST
// TODO: isValid() tests for LAST, LAST OBS, LAST STFD OBS
// TODO: isValid() tests for LAST NEXT time, LAST OBS NEXT time, LAST STFD OBS NEXT time
// TODO: isValid() tests for NEXT time
// TODO: isValid() tests for NEXT FCST BY time, NEXT FCST AT time, NEXT FCST WILL BE ISSUED AT time
// TODO: isValid() tests for AMD time
// TODO: isValid() tests for NO AMDS AFT time
// TODO: isValid() tests for FCST BASED ON AUTO OBS
