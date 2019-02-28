/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

static const auto margin = 0.01/2;

TEST(PressureGroup, parseInHg) {
	const auto pg = metaf::PressureGroup::parse("A2724", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg.has_value());
	ASSERT_TRUE(pg->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_NEAR(pg->atmosphericPressure().pressure().value(), 27.24, margin);
}

TEST(PressureGroup, parseInHgNotReported) {
	const auto pg = metaf::PressureGroup::parse("A////", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg.has_value());
	EXPECT_FALSE(pg->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::INCHES_HG);
}

TEST(PressureGroup, parseHectopascal) {
	const auto pg = metaf::PressureGroup::parse("Q1033", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg.has_value());
	ASSERT_TRUE(pg->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_NEAR(pg->atmosphericPressure().pressure().value(), 1033, margin);
}

TEST(PressureGroup, parseHectopascalNotReported) {
	const auto pg = metaf::PressureGroup::parse("Q////", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg.has_value());
	EXPECT_FALSE(pg->atmosphericPressure().pressure().has_value());
	EXPECT_EQ(pg->atmosphericPressure().unit(), metaf::Pressure::Unit::HECTOPASCAL);
}

TEST(PressureGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::PressureGroup::parse("Q1033", metaf::ReportPart::RMK).has_value());
}

TEST(PressureGroup, parseWrongFormat) {
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
