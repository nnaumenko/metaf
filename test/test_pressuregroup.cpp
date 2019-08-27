/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

static const auto margin = 0.01/2;

TEST(PressureGroup, parseObservedInHg) {
	const auto pg = metaf::PressureGroup::parse("A2724", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg.has_value());
	ASSERT_TRUE(pg->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_NEAR(pg->atmosphericPressure().pressure().value(), 27.24, margin);
	EXPECT_EQ(pg->type(), metaf::PressureGroup::Type::OBSERVED_QNH);
}

TEST(PressureGroup, parseObservedInHgNotReported) {
	const auto pg = metaf::PressureGroup::parse("A////", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg.has_value());
	EXPECT_FALSE(pg->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_EQ(pg->type(), metaf::PressureGroup::Type::OBSERVED_QNH);
}

TEST(PressureGroup, parseObservedHectopascal) {
	const auto pg = metaf::PressureGroup::parse("Q1033", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg.has_value());
	ASSERT_TRUE(pg->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_NEAR(pg->atmosphericPressure().pressure().value(), 1033, margin);
	EXPECT_EQ(pg->type(), metaf::PressureGroup::Type::OBSERVED_QNH);
}

TEST(PressureGroup, parseObservedHectopascalNotReported) {
	const auto pg = metaf::PressureGroup::parse("Q////", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg.has_value());
	EXPECT_FALSE(pg->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_EQ(pg->type(), metaf::PressureGroup::Type::OBSERVED_QNH);
}

TEST(PressureGroup, parseObservedWrongReportPart) {
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::RMK).has_value());
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

TEST(PressureGroup, parseSlpRemark) {
	const auto pg = metaf::PressureGroup::parse("SLP982", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_NEAR(pg->atmosphericPressure().pressure().value(), 998.2, margin);
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_EQ(pg->type(), metaf::PressureGroup::Type::OBSERVED_QNH);
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

TEST(PressureGroup, combine) {
	const auto pg1 = metaf::PressureGroup::parse("A2724", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg1.has_value());

	const auto pg2 = metaf::PressureGroup::parse("Q1033", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg2.has_value());

	const auto pg3 = metaf::PressureGroup::parse("QNH2957INS", metaf::ReportPart::TAF);
	ASSERT_TRUE(pg3.has_value());

	const auto pg4 = metaf::PressureGroup::parse("QFE750", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg4.has_value());

	const auto rmk = metaf::FixedGroup::parse("RMK", metaf::ReportPart::METAR);
	ASSERT_TRUE(rmk.has_value());

	EXPECT_FALSE(pg1->combine(rmk.value()).has_value());
	EXPECT_FALSE(pg1->combine(pg1.value()).has_value());
	EXPECT_FALSE(pg1->combine(pg2.value()).has_value());
	EXPECT_FALSE(pg1->combine(pg3.value()).has_value());
	EXPECT_FALSE(pg1->combine(pg4.value()).has_value());
	EXPECT_FALSE(pg1->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(pg2->combine(rmk.value()).has_value());
	EXPECT_FALSE(pg2->combine(pg1.value()).has_value());
	EXPECT_FALSE(pg2->combine(pg2.value()).has_value());
	EXPECT_FALSE(pg2->combine(pg3.value()).has_value());
	EXPECT_FALSE(pg2->combine(pg4.value()).has_value());
	EXPECT_FALSE(pg2->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(pg3->combine(rmk.value()).has_value());
	EXPECT_FALSE(pg3->combine(pg1.value()).has_value());
	EXPECT_FALSE(pg3->combine(pg2.value()).has_value());
	EXPECT_FALSE(pg3->combine(pg3.value()).has_value());
	EXPECT_FALSE(pg3->combine(pg4.value()).has_value());
	EXPECT_FALSE(pg3->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(pg4->combine(rmk.value()).has_value());
	EXPECT_FALSE(pg4->combine(pg1.value()).has_value());
	EXPECT_FALSE(pg4->combine(pg2.value()).has_value());
	EXPECT_FALSE(pg4->combine(pg3.value()).has_value());
	EXPECT_FALSE(pg4->combine(pg4.value()).has_value());
	EXPECT_FALSE(pg4->combine(metaf::PlainTextGroup("TEST")).has_value());
}
