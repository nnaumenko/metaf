/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(getSyntaxGroup, METAR) {
	const auto g = metaf::FixedGroup::parse("METAR", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::METAR);
}

TEST(getSyntaxGroup, SPECI) {
	const auto g = metaf::FixedGroup::parse("SPECI", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::SPECI);
}

TEST(getSyntaxGroup, TAF) {
	const auto g = metaf::FixedGroup::parse("TAF", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::TAF);
}

TEST(getSyntaxGroup, COR) {
	const auto g = metaf::FixedGroup::parse("COR", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::COR);
}

TEST(getSyntaxGroup, AMD) {
	const auto g = metaf::FixedGroup::parse("AMD", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::AMD);
}

TEST(getSyntaxGroup, NIL) {
	const auto g = metaf::FixedGroup::parse("NIL", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::NIL);
}

TEST(getSyntaxGroup, CNL) {
	const auto g = metaf::FixedGroup::parse("CNL", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::CNL);
}

TEST(getSyntaxGroup, RMK) {
	const auto g = metaf::FixedGroup::parse("RMK", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::RMK);
}

TEST(getSyntaxGroup, maintenanceIndicator) {
	const auto g = metaf::FixedGroup::parse("$", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::MAINTENANCE_INDICATOR);
}

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

TEST(getSyntaxGroup, TIME_SPAN) {
	const auto g = metaf::TrendGroup::parse("0812/0824", metaf::ReportPart::HEADER);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::TIME_SPAN);
}

TEST(getSyntaxGroup, OTHER_AUTO) {
	const auto g = metaf::FixedGroup::parse("AUTO", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_R_SNOCLO) {
	const auto g = metaf::FixedGroup::parse("SNOCLO", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_CAVOK) {
	const auto g = metaf::FixedGroup::parse("CAVOK", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_NSW) {
	const auto g = metaf::FixedGroup::parse("NSW", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_WSCONDS) {
	const auto g = metaf::FixedGroup::parse("WSCONDS", metaf::ReportPart::TAF);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_AO1) {
	const auto g = metaf::FixedGroup::parse("AO1", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_AO2) {
	const auto g = metaf::FixedGroup::parse("AO2", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_NOSPECI) {
	const auto g = metaf::FixedGroup::parse("NOSPECI", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_RVRNO) {
	const auto g = metaf::FixedGroup::parse("RVRNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_PWINO) {
	const auto g = metaf::FixedGroup::parse("PWINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_PNO) {
	const auto g = metaf::FixedGroup::parse("PNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_FZRANO) {
	const auto g = metaf::FixedGroup::parse("FZRANO", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_TSNO) {
	const auto g = metaf::FixedGroup::parse("TSNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_SLPNO) {
	const auto g = metaf::FixedGroup::parse("SLPNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_FROIN) {
	const auto g = metaf::FixedGroup::parse("FROIN", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, CLD_MISG) {
	auto fg = metaf::FixedGroup::parse("CLD", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, ICG_MISG) {
	auto fg = metaf::FixedGroup::parse("ICG", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, PCPN_MISG) {
	auto fg = metaf::FixedGroup::parse("PCPN", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, PRES_MISG) {
	auto fg = metaf::FixedGroup::parse("PRES", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, RVR_MISG) {
	auto fg = metaf::FixedGroup::parse("RVR", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, T_MISG) {
	auto fg = metaf::FixedGroup::parse("T", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}


TEST(getSyntaxGroup, TD_MISG) {
	auto fg = metaf::FixedGroup::parse("TD", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, VIS_MISG) {
	auto fg = metaf::FixedGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, WND_MISG) {
	auto fg = metaf::FixedGroup::parse("WND", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, WX_MISG) {
	auto fg = metaf::FixedGroup::parse("WX", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, TS_LTNG_TEMPO_UNAVBL) {
	auto fg = metaf::FixedGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(metaf::getSyntaxGroup(fg.value()), metaf::SyntaxGroup::OTHER);
}

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
	const auto g = metaf::TrendGroup::parse("FM191445", metaf::ReportPart::TAF);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_TrendTime) {
	const auto g = metaf::TrendGroup::parse("TL1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_Probability) {
	const auto g = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_UnknownGroup) {
	const auto g = metaf::UnknownGroup::parse("A1B2C3D4", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_WindGroup) {
	const auto g = metaf::WindGroup::parse("27005KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_VarWindGroup) {
	const auto g = metaf::WindGroup::parse("240V250", metaf::ReportPart::METAR);
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

TEST(getSyntaxGroup, OTHER_TemperatureForecastGroup) {
	const auto g = metaf::TemperatureForecastGroup::parse("TX07/1105Z", metaf::ReportPart::TAF);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_PressureGroup) {
	const auto g = metaf::PressureGroup::parse("Q1033", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_RunwayVisualRangeGroup) {
	const auto g = metaf::RunwayVisualRangeGroup::parse("R30/5000VP6000FT/U", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_RunwayStateGroup) {
	const auto g = metaf::RunwayStateGroup::parse("R16/750155", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_SecondaryLocationGroup) {
	const auto g = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_RainfallGroup) {
	const auto g = metaf::RainfallGroup::parse("RF00.0/000.0", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_SeaSurfaceGroup) {
	const auto g = metaf::SeaSurfaceGroup::parse("W15/S4", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_ColourCodeGroup) {
	const auto g = metaf::ColourCodeGroup::parse("AMB", metaf::ReportPart::METAR);
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

TEST(getSyntaxGroup, OTHER_CloudLayersGroup) {
	const auto g = metaf::CloudLayersGroup::parse("8/578", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_MiscGroupSunshineDuration) {
	const auto g = metaf::MiscGroup::parse("98062", metaf::ReportPart::RMK);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}

TEST(getSyntaxGroup, OTHER_MiscGroupCorrectionNo) {
	const auto g = metaf::MiscGroup::parse("CCA", metaf::ReportPart::METAR);
	ASSERT_TRUE(g.has_value());
	EXPECT_EQ(metaf::getSyntaxGroup(g.value()), metaf::SyntaxGroup::OTHER);
}
