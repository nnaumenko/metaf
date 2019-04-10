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
