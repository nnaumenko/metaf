/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Keyword groups
// Purpose: to confirm that keyword groups are correctly identified as groups
// significant for METAR and TAF syntax.
///////////////////////////////////////////////////////////////////////////////

TEST(getSyntaxGroup, METAR) {
	const auto g = metaf::KeywordGroup::parse("METAR", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::METAR);
}

TEST(getSyntaxGroup, SPECI) {
	const auto g = metaf::KeywordGroup::parse("SPECI", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::SPECI);
}

TEST(getSyntaxGroup, TAF) {
	const auto g = metaf::KeywordGroup::parse("TAF", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::TAF);
}

TEST(getSyntaxGroup, COR) {
	const auto g = metaf::KeywordGroup::parse("COR", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::COR);
}

TEST(getSyntaxGroup, AMD) {
	const auto g = metaf::KeywordGroup::parse("AMD", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::AMD);
}

TEST(getSyntaxGroup, NIL) {
	const auto g = metaf::KeywordGroup::parse("NIL", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::NIL);
}

TEST(getSyntaxGroup, CNL) {
	const auto g = metaf::KeywordGroup::parse("CNL", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::CNL);
}

TEST(getSyntaxGroup, RMK) {
	const auto g = metaf::KeywordGroup::parse("RMK", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::RMK);
}

TEST(getSyntaxGroup, maintenanceIndicator) {
	const auto g = metaf::KeywordGroup::parse("$", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::MAINTENANCE_INDICATOR);
}

///////////////////////////////////////////////////////////////////////////////
// Other keyword groups
// Purpose: to confirm that keyword groups are correctly identified as groups
// NOT significant for report syntax.
///////////////////////////////////////////////////////////////////////////////

TEST(getSyntaxGroup, OTHER_AUTO) {
	const auto g = metaf::KeywordGroup::parse("AUTO", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_CAVOK) {
	const auto g = metaf::KeywordGroup::parse("CAVOK", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_AO1) {
	const auto g = metaf::KeywordGroup::parse("AO1", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_AO2) {
	const auto g = metaf::KeywordGroup::parse("AO2", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_AO1A) {
	const auto g = metaf::KeywordGroup::parse("AO1A", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_AO2A) {
	const auto g = metaf::KeywordGroup::parse("AO2A", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_NOSPECI) {
	const auto g = metaf::KeywordGroup::parse("NOSPECI", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

///////////////////////////////////////////////////////////////////////////////
// Location, report time, and time span
// Purpose: to confirm that location groups, report release time groups, and
// time span trend groups are correctly identified as groups significant for 
// METAR and TAF syntax.
///////////////////////////////////////////////////////////////////////////////

TEST(getSyntaxGroup, LOCATION) {
	const auto g = metaf::LocationGroup::parse("UKLL", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::LOCATION);
}

TEST(getSyntaxGroup, REPORT_TIME) {
	const auto g = metaf::ReportTimeGroup::parse("081230Z", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::REPORT_TIME);
}

TEST(getSyntaxGroup, TREND_TIME_SPAN) {
	const auto g = metaf::TrendGroup::parse("0812/0824", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::TIME_SPAN);
}

///////////////////////////////////////////////////////////////////////////////
// Other trends
// Purpose: to confirm that trend groups other than time span are correctly 
// identified as groups NOT significant for METAR and TAF syntax.
///////////////////////////////////////////////////////////////////////////////

TEST(getSyntaxGroup, OTHER_NOSIG) {
	const auto g = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_TEMPO) {
	const auto g = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_BECMG) {
	const auto g = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_INTER) {
	const auto g = metaf::TrendGroup::parse("INTER", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_TrendFrom) {
	const auto g1 = metaf::TrendGroup::parse("FM191445", metaf::ReportPart::TAF);
	ASSERT_TRUE(g1.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g1.value()), metaf::SyntaxGroup::OTHER);

	const auto g2 = metaf::TrendGroup::parse("FM1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(g2.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g2.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_TrendUntil) {
	const auto g = metaf::TrendGroup::parse("TL1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_TrendAt) {
	const auto g = metaf::TrendGroup::parse("AT1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_TrendFromUntil) {
	auto g = metaf::TrendGroup::parse("FM1230", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(g->append("TL1415", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_Probability) {
	const auto g = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_MetarTimeSpan_HHMM_HHMM) {
	const auto g = metaf::TrendGroup::parse("0812/0824", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::TIME_SPAN);
}

///////////////////////////////////////////////////////////////////////////////
// Other group
// Purpose: to confirm that any other group is identified as NOT significant
// for repot syntax
///////////////////////////////////////////////////////////////////////////////

TEST(getSyntaxGroup, OTHER_WindGroup) {
	const auto g = metaf::WindGroup::parse("27005KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_VisibilityGroup) {
	const auto g = metaf::VisibilityGroup::parse("9999NDV", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_CloudGroup) {
	const auto g = metaf::CloudGroup::parse("BKN024", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_CloudClr) {
	const auto g = metaf::CloudGroup::parse("CLR", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_CloudSkc) {
	const auto g = metaf::CloudGroup::parse("SKC", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_CloudNsc) {
	const auto g = metaf::CloudGroup::parse("NSC", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_CloudNcd) {
	const auto g = metaf::CloudGroup::parse("NCD", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_CloudVerticalVisibility) {
	const auto g = metaf::CloudGroup::parse("VV015", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_WeatherGroup) {
	const auto g = metaf::WeatherGroup::parse("+SHSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_TemperatureGroup) {
	const auto g = metaf::TemperatureGroup::parse("07/02", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_PressureGroup) {
	const auto g = metaf::PressureGroup::parse("Q1033", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_RunwayStateGroup) {
	const auto g = metaf::RunwayStateGroup::parse("R16/750155", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_SeaSurfaceGroup) {
	const auto g = metaf::SeaSurfaceGroup::parse("W15/S4", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_MinMaxTemperatureGroup) {
	const auto g = metaf::MinMaxTemperatureGroup::parse("401120084", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_PrecipitationGroup) {
	const auto g = metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_LayerForecastGroup) {
	const auto g = metaf::LayerForecastGroup::parse("620304", metaf::ReportPart::TAF);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_PressureTendencyGroup) {
	const auto g = metaf::PressureTendencyGroup::parse("52032", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_CloudTypesGroup) {
	const auto g = metaf::CloudTypesGroup::parse("CF1CU3SC1CS1CI2", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_LowMidHighCloudGroup) {
	const auto g = metaf::LowMidHighCloudGroup::parse("8/578", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_LightningGroup) {
	const auto g = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_VicinityGroup) {
	auto g = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	ASSERT_EQ(g->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_MiscGroup) {
	const auto g1 = metaf::MiscGroup::parse("98062", metaf::ReportPart::RMK);
	ASSERT_TRUE(g1.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g1.value()), metaf::SyntaxGroup::OTHER);

	const auto g2 = metaf::MiscGroup::parse("CCA", metaf::ReportPart::METAR);
	ASSERT_TRUE(g2.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g2.value()), metaf::SyntaxGroup::OTHER);

	auto g3 = metaf::MiscGroup::parse("GR", metaf::ReportPart::RMK);
	ASSERT_TRUE(g3.has_value());
	EXPECT_EQ(g3->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(g3->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(g3.value()), metaf::SyntaxGroup::OTHER);

	auto g4 = metaf::MiscGroup::parse("DENSITY", metaf::ReportPart::RMK);
	ASSERT_TRUE(g4.has_value());
	EXPECT_EQ(g4->append("ALT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(g4->append("100FT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(g4.value()), metaf::SyntaxGroup::OTHER);

	const auto g5 = metaf::MiscGroup::parse("BLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(g5.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g5.value()), metaf::SyntaxGroup::OTHER);

	const auto g6 = metaf::MiscGroup::parse("BLACKAMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(g6.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g6.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_UnknownGroup) {
	const auto g = metaf::UnknownGroup::parse("A1B2C3D4", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}
