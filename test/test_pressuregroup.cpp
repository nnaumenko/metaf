/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.01/2;

///////////////////////////////////////////////////////////////////////////////
// Observed atmospheric pressure groups
// Purpose: to confirm that SLP-normalised observed atmospheric pressure groups 
// are parsed correctly, and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(PressureGroup, parseObservedInHg) {
	const auto pg1 = metaf::PressureGroup::parse("A2724", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg1.has_value());
	ASSERT_TRUE(pg1->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg1->atmosphericPressure().unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_NEAR(pg1->atmosphericPressure().pressure().value(), 27.24, margin);
	EXPECT_EQ(pg1->type(), metaf::PressureGroup::Type::OBSERVED_QNH);

	const auto pg2 = metaf::PressureGroup::parse("A2724", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	ASSERT_TRUE(pg2->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg2->atmosphericPressure().unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_NEAR(pg2->atmosphericPressure().pressure().value(), 27.24, margin);
	EXPECT_EQ(pg2->type(), metaf::PressureGroup::Type::OBSERVED_QNH);
}

TEST(PressureGroup, parseObservedInHgNotReported) {
	const auto pg1 = metaf::PressureGroup::parse("A////", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_FALSE(pg1->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg1->atmosphericPressure().unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_EQ(pg1->type(), metaf::PressureGroup::Type::OBSERVED_QNH);

	const auto pg2 = metaf::PressureGroup::parse("A////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_FALSE(pg2->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg2->atmosphericPressure().unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_EQ(pg2->type(), metaf::PressureGroup::Type::OBSERVED_QNH);
}

TEST(PressureGroup, parseObservedHectopascal) {
	const auto pg1 = metaf::PressureGroup::parse("Q1033", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg1.has_value());
	ASSERT_TRUE(pg1->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg1->atmosphericPressure().unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_NEAR(pg1->atmosphericPressure().pressure().value(), 1033, margin);
	EXPECT_EQ(pg1->type(), metaf::PressureGroup::Type::OBSERVED_QNH);

	const auto pg2 = metaf::PressureGroup::parse("Q1033", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	ASSERT_TRUE(pg2->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg2->atmosphericPressure().unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_NEAR(pg2->atmosphericPressure().pressure().value(), 1033, margin);
	EXPECT_EQ(pg2->type(), metaf::PressureGroup::Type::OBSERVED_QNH);
}

TEST(PressureGroup, parseObservedHectopascalNotReported) {
	const auto pg1 = metaf::PressureGroup::parse("Q////", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_FALSE(pg1->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg1->atmosphericPressure().unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_EQ(pg1->type(), metaf::PressureGroup::Type::OBSERVED_QNH);

	const auto pg2 = metaf::PressureGroup::parse("Q////", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_FALSE(pg2->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg2->atmosphericPressure().unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_EQ(pg2->type(), metaf::PressureGroup::Type::OBSERVED_QNH);
}

TEST(PressureGroup, parseObservedWrongReportPart) {
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::TAF).has_value());
}

TEST(PressureGroup, parseObservedWrongFormat) {
	EXPECT_FALSE(metaf::PressureGroup::parse("Q01033", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q999", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("A02724", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("A272", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Z2724", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("A272A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q101A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("1000", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("A///", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("A/////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q///", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q/////", metaf::ReportPart::METAR).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Forecast lowest atmospheric pressure
// Purpose: to confirm that forecast lowest atmospheric pressure groups are 
// parsed correctly, and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(PressureGroup, parseForecast) {
	const auto pg = metaf::PressureGroup::parse("QNH2957INS", metaf::ReportPart::TAF);
	ASSERT_TRUE(pg.has_value());
	ASSERT_TRUE(pg->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_NEAR(pg->atmosphericPressure().pressure().value(), 29.57, margin);
	EXPECT_EQ(pg->type(), metaf::PressureGroup::Type::FORECAST_LOWEST_QNH);
}

TEST(PressureGroup, parseForecastWrongReportPart) {
	EXPECT_FALSE(metaf::PressureGroup::parse("QNH2957INS", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QNH2957INS", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QNH2957INS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QNH2957INS", metaf::ReportPart::RMK).has_value());
}

TEST(PressureGroup, parseForecastWrongFormat) {
	EXPECT_FALSE(metaf::PressureGroup::parse("XNH2979INS", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QNH2979ANS", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QNH297INS", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QNH02979INS", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QNH////INS", metaf::ReportPart::TAF).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Auxiliary sea level pressure groups
// Purpose: to confirm that auxiliary sea-level pressure groups included in 
// the remarks are parsed correctly, and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(PressureGroup, parseSlpRemark) {
	const auto pg = metaf::PressureGroup::parse("SLP982", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_NEAR(pg->atmosphericPressure().pressure().value(), 998.2, margin);
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_EQ(pg->type(), metaf::PressureGroup::Type::OBSERVED_SLP);
}

TEST(PressureGroup, parseSlpRemarkWrongReportPart) {
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP982", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP982", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP982", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP982", metaf::ReportPart::TAF).has_value());
}

TEST(PressureGroup, parseSlpRemarkWrongFormat) {
	EXPECT_FALSE(metaf::PressureGroup::parse("SL0015", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("XLP215", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLPA15", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP01A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP215A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP2", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLP0215", metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Actual non-normalised atmospheric pressure groups
// Purpose: to confirm that groups indicating non-normalised to sea level 
// atmospheric pressure (i.e. actual barometer readings) are parsed correctly, 
// and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(PressureGroup, parseQfeRemark) {
	const auto pg1 = metaf::PressureGroup::parse("QFE750", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_NEAR(pg1->atmosphericPressure().pressure().value(), 750.0, margin);
	EXPECT_EQ(pg1->atmosphericPressure().unit(), metaf::Pressure::Unit::MM_HG);
	EXPECT_EQ(pg1->type(), metaf::PressureGroup::Type::OBSERVED_QFE);

	const auto pg2 = metaf::PressureGroup::parse("QFE761/1015", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_NEAR(pg2->atmosphericPressure().pressure().value(), 761.0, margin);
	EXPECT_EQ(pg2->atmosphericPressure().unit(), metaf::Pressure::Unit::MM_HG);
	EXPECT_EQ(pg2->type(), metaf::PressureGroup::Type::OBSERVED_QFE);
}

TEST(PressureGroup, parseQfeRemarkWrongReportPart) {
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE750", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE750", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE750", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE750", metaf::ReportPart::TAF).has_value());
}

TEST(PressureGroup, parseQfeRemarkWrongFormat) {
	EXPECT_FALSE(metaf::PressureGroup::parse("", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QF0755", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("XFE755", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFEA55", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE75A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE75A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE7", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE75", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE0755", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE0755/", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE0755/999", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE0755/01000", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE0755.1015", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE0755:1015", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE////1015", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE755/////", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE////////", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE76A/1015", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFEA61/1015", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE761/A015", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE761/101A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE761Q/1015", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("QFE761/1015Q", metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// SLPNO group
// Purpose: to confirm that SLPNO group is parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(PressureGroup, parseSlpno) {
	const auto pg = metaf::PressureGroup::parse("SLPNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_FALSE(pg->atmosphericPressure().isReported());
	EXPECT_EQ(pg->type(), metaf::PressureGroup::Type::SLPNO);
}

TEST(PressureGroup, parseSlpnoWrongReportPart) {
	EXPECT_FALSE(metaf::PressureGroup::parse("SLPNO", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLPNO", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLPNO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("SLPNO", metaf::ReportPart::TAF).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// PRES MISG groups
// Purpose: to confirm that PRES MISG groups are parsed and appended correctly
///////////////////////////////////////////////////////////////////////////////

TEST(PressureGroup, parsePresMisg) {
	auto pg = metaf::PressureGroup::parse("PRES", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_EQ(pg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(pg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg->type(), metaf::PressureGroup::Type::PRES_MISG);
	EXPECT_FALSE(pg->atmosphericPressure().isReported());
}

TEST(PressureGroup, parsePresWrongReportPart) {
	EXPECT_FALSE(metaf::PressureGroup::parse("PRES", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("PRES", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("PRES", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("PRES", metaf::ReportPart::TAF).has_value());
}

TEST(PressureGroup, parseMisg) {
	EXPECT_FALSE(metaf::PressureGroup::parse("MISG", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("MISG", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("MISG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("MISG", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("MISG", metaf::ReportPart::RMK).has_value());
}

TEST(PressureGroup, appendOtherToPres) {
	auto pg1 = metaf::PressureGroup::parse("PRES", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto pg2 = metaf::PressureGroup::parse("PRES", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->append("A2974", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto pg3 = metaf::PressureGroup::parse("PRES", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->append("Q0991", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto pg4 = metaf::PressureGroup::parse("PRES", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg4.has_value());
	EXPECT_EQ(pg4->append("QFE750", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto pg5 = metaf::PressureGroup::parse("PRES", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg5.has_value());
	EXPECT_EQ(pg5->append("PRES", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

///////////////////////////////////////////////////////////////////////////////
// Appending tests
// Purpose: to confirm that no other group can be appended to complete groups.
///////////////////////////////////////////////////////////////////////////////

TEST(PressureGroup, append) {
	const std::string pgStr1("A2724");
	const std::string pgStr2("Q1033");
	const std::string pgStr3("A////");
	const std::string pgStr4("Q////");
	const std::string pgStr5("QNH2957INS");
	const std::string pgStr6("QFE750");
	const std::string pgStr7("QFE761/1015");
	const std::string pgStr8("SLPNO");
	const std::string pgStr9("PRES");
	const std::string pgStr10("MISG");
	const std::string tStr1("RMK");
	const std::string tStr2("TEST");

	auto pg1 = metaf::PressureGroup::parse(pgStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(pg1.has_value());

	auto pg2 = metaf::PressureGroup::parse(pgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(pg2.has_value());

	auto pg3 = metaf::PressureGroup::parse(pgStr3, metaf::ReportPart::METAR);
	ASSERT_TRUE(pg3.has_value());

	auto pg4 = metaf::PressureGroup::parse(pgStr4, metaf::ReportPart::METAR);
	ASSERT_TRUE(pg4.has_value());

	auto pg5 = metaf::PressureGroup::parse(pgStr5, metaf::ReportPart::TAF);
	ASSERT_TRUE(pg5.has_value());

	auto pg6 = metaf::PressureGroup::parse(pgStr6, metaf::ReportPart::RMK);
	ASSERT_TRUE(pg6.has_value());

	auto pg7 = metaf::PressureGroup::parse(pgStr7, metaf::ReportPart::RMK);
	ASSERT_TRUE(pg7.has_value());

	auto pg8 = metaf::PressureGroup::parse(pgStr8, metaf::ReportPart::RMK);
	ASSERT_TRUE(pg8.has_value());

	auto pg9 = metaf::PressureGroup::parse(pgStr9, metaf::ReportPart::RMK);
	ASSERT_TRUE(pg9.has_value());
	ASSERT_EQ(pg9->append(pgStr10, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(pg1->append(pgStr1, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(pgStr2, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(pgStr3, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(pgStr4, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(pgStr5, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(pgStr6, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(pgStr7, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(pgStr8, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(pgStr9, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(pgStr10, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(tStr1, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg1->append(tStr2, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg2->append(pgStr1, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(pgStr2, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(pgStr3, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(pgStr4, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(pgStr5, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(pgStr6, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(pgStr7, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(pgStr8, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(pgStr9, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(pgStr10, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(tStr1, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg2->append(tStr2, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg3->append(pgStr1, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(pgStr2, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(pgStr3, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(pgStr4, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(pgStr5, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(pgStr6, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(pgStr7, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(pgStr8, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(pgStr9, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(pgStr10, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(tStr1, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg3->append(tStr2, metaf::ReportPart::TAF), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg4->append(pgStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(pgStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(pgStr3, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(pgStr4, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(pgStr5, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(pgStr6, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(pgStr7, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(pgStr8, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(pgStr9, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(pgStr10, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(tStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg4->append(tStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg5->append(pgStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(pgStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(pgStr3, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(pgStr4, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(pgStr5, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(pgStr6, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(pgStr7, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(pgStr8, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(pgStr9, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(pgStr10, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(tStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg5->append(tStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg6->append(pgStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(pgStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(pgStr3, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(pgStr4, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(pgStr5, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(pgStr6, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(pgStr7, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(pgStr8, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(pgStr9, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(pgStr10, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(tStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg6->append(tStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg7->append(pgStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(pgStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(pgStr3, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(pgStr4, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(pgStr5, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(pgStr6, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(pgStr7, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(pgStr8, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(pgStr9, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(pgStr10, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(tStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg7->append(tStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg8->append(pgStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(pgStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(pgStr3, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(pgStr4, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(pgStr5, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(pgStr6, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(pgStr7, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(pgStr8, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(pgStr9, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(pgStr10, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(tStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg8->append(tStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg9->append(pgStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(pgStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(pgStr3, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(pgStr4, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(pgStr5, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(pgStr6, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(pgStr7, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(pgStr8, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(pgStr9, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(pgStr10, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(tStr1, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg9->append(tStr2, metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(PressureGroup, isValidTrue) {
	const auto pg1 = metaf::PressureGroup::parse("A2724", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_TRUE(pg1->isValid());

	const auto pg2 = metaf::PressureGroup::parse("A////", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_TRUE(pg2->isValid());

	const auto pg3 = metaf::PressureGroup::parse("Q1033", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_TRUE(pg3->isValid());

	const auto pg4 = metaf::PressureGroup::parse("Q////", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg4.has_value());
	EXPECT_TRUE(pg4->isValid());

	const auto pg5 = metaf::PressureGroup::parse("QNH2957INS", metaf::ReportPart::TAF);
	ASSERT_TRUE(pg5.has_value());
	EXPECT_TRUE(pg5->isValid());

	const auto pg6 = metaf::PressureGroup::parse("QFE750", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg6.has_value());
	EXPECT_TRUE(pg6->isValid());

	const auto pg7 = metaf::PressureGroup::parse("QFE761/1015", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg7.has_value());
	EXPECT_TRUE(pg7->isValid());

	const auto pg8 = metaf::PressureGroup::parse("SLP982", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg8.has_value());
	EXPECT_TRUE(pg8->isValid());

	const auto pg9 = metaf::PressureGroup::parse("SLPNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg9.has_value());
	EXPECT_TRUE(pg9->isValid());

	auto pg10 = metaf::PressureGroup::parse("PRES", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg10.has_value());
	EXPECT_EQ(pg10->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(pg10->isValid());
}

TEST(PressureGroup, isValidFalsePres) {
	auto pg = metaf::PressureGroup::parse("PRES", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_FALSE(pg->isValid());
}