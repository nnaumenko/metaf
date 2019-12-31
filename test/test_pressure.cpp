/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.01 / 2;
static const auto slpMargin = 0.1 / 2;


TEST(Pressure, fromStringHectopascal) {
	const auto p = metaf::Pressure::fromString("Q0994");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	ASSERT_TRUE(p->pressure().has_value());
	EXPECT_NEAR(p->pressure().value(), 994, margin);
}

TEST(Pressure, fromStringHectopascalNotReported) {
	const auto p = metaf::Pressure::fromString("Q////");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_FALSE(p->pressure().has_value());
}

TEST(Pressure, fromStringInHg) {
	const auto p = metaf::Pressure::fromString("A2934");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Pressure::Unit::INCHES_HG);
	ASSERT_TRUE(p->pressure().has_value());
	EXPECT_NEAR(p->pressure().value(), 29.34, margin);
}

TEST(Pressure, fromStringInHgNotReported) {
	const auto p = metaf::Pressure::fromString("A////");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_FALSE(p->pressure().has_value());
}

TEST(Pressure, fromStringWrongFormat) {
	EXPECT_FALSE(metaf::Pressure::fromString(""));
	EXPECT_FALSE(metaf::Pressure::fromString("A296"));
	EXPECT_FALSE(metaf::Pressure::fromString("A29600"));
	EXPECT_FALSE(metaf::Pressure::fromString("Q999"));
	EXPECT_FALSE(metaf::Pressure::fromString("Q01000"));
	EXPECT_FALSE(metaf::Pressure::fromString("R1000"));
	EXPECT_FALSE(metaf::Pressure::fromString("A290A"));
	EXPECT_FALSE(metaf::Pressure::fromString("Q999A"));
	EXPECT_FALSE(metaf::Pressure::fromString("Q///"));
	EXPECT_FALSE(metaf::Pressure::fromString("Q/////"));
	EXPECT_FALSE(metaf::Pressure::fromString("A///"));
	EXPECT_FALSE(metaf::Pressure::fromString("A/////"));
}

TEST(Pressure, fromForecastString) {
	const auto p = metaf::Pressure::fromForecastString("QNH2979INS");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Pressure::Unit::INCHES_HG);
	ASSERT_TRUE(p->pressure().has_value());
	EXPECT_NEAR(p->pressure().value(), 29.79, margin);
}

TEST(Pressure, fromForecastStringWrongFormat) {
	EXPECT_FALSE(metaf::Pressure::fromForecastString(""));
	EXPECT_FALSE(metaf::Pressure::fromForecastString("XNH2979INS"));
	EXPECT_FALSE(metaf::Pressure::fromForecastString("QNH2979ANS"));
	EXPECT_FALSE(metaf::Pressure::fromForecastString("QNH297INS"));
	EXPECT_FALSE(metaf::Pressure::fromForecastString("QNH02979INS"));
	EXPECT_FALSE(metaf::Pressure::fromForecastString("QNH////INS"));
}

TEST(Pressure, fromSlpString) {
	const auto p1 = metaf::Pressure::fromSlpString("SLP982");
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	ASSERT_TRUE(p1->pressure().has_value());
	EXPECT_NEAR(p1->pressure().value(), 998.2, slpMargin);

	const auto p2 = metaf::Pressure::fromSlpString("SLP015");
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	ASSERT_TRUE(p2->pressure().has_value());
	EXPECT_NEAR(p2->pressure().value(), 1001.5, slpMargin);

	const auto p3 = metaf::Pressure::fromSlpString("SLP221");
	ASSERT_TRUE(p3.has_value());
	EXPECT_EQ(p3->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	ASSERT_TRUE(p3->pressure().has_value());
	EXPECT_NEAR(p3->pressure().value(), 1022.1, slpMargin);

	const auto p4 = metaf::Pressure::fromSlpString("SLP499");
	ASSERT_TRUE(p4.has_value());
	EXPECT_EQ(p4->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	ASSERT_TRUE(p4->pressure().has_value());
	EXPECT_NEAR(p4->pressure().value(), 1049.9, slpMargin);

	const auto p5 = metaf::Pressure::fromSlpString("SLP500");
	ASSERT_TRUE(p5.has_value());
	EXPECT_EQ(p5->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	ASSERT_TRUE(p5->pressure().has_value());
	EXPECT_NEAR(p5->pressure().value(), 950.0, slpMargin);
}

TEST(Pressure, fromSlpStringWrongFormat) {
	EXPECT_FALSE(metaf::Pressure::fromSlpString(""));
	EXPECT_FALSE(metaf::Pressure::fromSlpString("SL0015"));
	EXPECT_FALSE(metaf::Pressure::fromSlpString("SLP"));
	EXPECT_FALSE(metaf::Pressure::fromSlpString("XLP215"));
	EXPECT_FALSE(metaf::Pressure::fromSlpString("SLPA15"));
	EXPECT_FALSE(metaf::Pressure::fromSlpString("SLP01A"));
	EXPECT_FALSE(metaf::Pressure::fromSlpString("SLP///"));
	EXPECT_FALSE(metaf::Pressure::fromSlpString("SLP215A"));
	EXPECT_FALSE(metaf::Pressure::fromSlpString("SLP2"));
	EXPECT_FALSE(metaf::Pressure::fromSlpString("SLP21"));
	EXPECT_FALSE(metaf::Pressure::fromSlpString("SLP0215"));
}

TEST(Pressure, fromQfeString) {
	const auto p1 = metaf::Pressure::fromQfeString("QFE750");
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->unit(), metaf::Pressure::Unit::MM_HG);
	ASSERT_TRUE(p1->pressure().has_value());
	EXPECT_NEAR(p1->pressure().value(), 750.0, margin);

	const auto p2 = metaf::Pressure::fromQfeString("QFE761/1015");
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->unit(), metaf::Pressure::Unit::MM_HG);
	ASSERT_TRUE(p2->pressure().has_value());
	EXPECT_NEAR(p2->pressure().value(), 761.0, margin);
}

TEST(Pressure, fromQfeStringWrongFormat) {
	EXPECT_FALSE(metaf::Pressure::fromQfeString(""));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QF0755"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("XFE755"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFEA55"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE75A"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE///"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE75A"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE7"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE75"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE0755"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE0755/"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE0755/999"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE0755/01000"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE0755.1015"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE0755:1015"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE////1015"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE755/////"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE////////"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE76A/1015"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFEA61/1015"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE761/A015"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE761/101A"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE761Q/1015"));
	EXPECT_FALSE(metaf::Pressure::fromQfeString("QFE761/1015Q"));
}

TEST(Pressure, fromTendencyString) {
	const auto p = metaf::Pressure::fromTendencyString("032");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	ASSERT_TRUE(p->pressure().has_value());
	EXPECT_NEAR(p->pressure().value(), 3.2, margin);
}

TEST(Pressure, fromTendencyStringNotReported) {
	const auto p = metaf::Pressure::fromTendencyString("///");
	ASSERT_TRUE(p.has_value());
	EXPECT_FALSE(p->pressure().has_value());
}

TEST(Pressure, fromTendencyStringWrongFormat) {
	EXPECT_FALSE(metaf::Pressure::fromTendencyString(""));
	EXPECT_FALSE(metaf::Pressure::fromTendencyString("//"));
	EXPECT_FALSE(metaf::Pressure::fromTendencyString("////"));
	EXPECT_FALSE(metaf::Pressure::fromTendencyString("0032"));
	EXPECT_FALSE(metaf::Pressure::fromTendencyString("32"));
	EXPECT_FALSE(metaf::Pressure::fromTendencyString("A32"));
	EXPECT_FALSE(metaf::Pressure::fromTendencyString("03A"));
	EXPECT_FALSE(metaf::Pressure::fromTendencyString("03.2"));
	EXPECT_FALSE(metaf::Pressure::fromTendencyString("3.2"));
}

TEST(Pressure, toUnitSameUnit) {
	const auto p1 = metaf::Pressure::fromString("A2934");
	ASSERT_TRUE(p1.has_value());
	ASSERT_TRUE(p1->pressure().has_value());
	EXPECT_EQ(p1->unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_TRUE(p1->toUnit(p1->unit()).has_value());
	EXPECT_EQ(p1->pressure().value(), p1->toUnit(p1->unit()).value());

	const auto p2 = metaf::Pressure::fromString("Q0994");
	ASSERT_TRUE(p2.has_value());
	ASSERT_TRUE(p2->pressure().has_value());
	EXPECT_EQ(p2->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_TRUE(p2->toUnit(p2->unit()).has_value());
	EXPECT_EQ(p2->pressure().value(), p2->toUnit(p2->unit()).value());

	const auto p3 = metaf::Pressure::fromQfeString("QFE750");
	ASSERT_TRUE(p3.has_value());
	ASSERT_TRUE(p3->pressure().has_value());
	EXPECT_EQ(p3->unit(), metaf::Pressure::Unit::MM_HG);
	EXPECT_TRUE(p3->toUnit(p3->unit()).has_value());
	EXPECT_EQ(p3->pressure().value(), p3->toUnit(p3->unit()).value());
}

TEST(Pressure, toUnitNotReported) {
	const auto p1 = metaf::Pressure::fromString("A////");
	ASSERT_TRUE(p1.has_value());
	EXPECT_FALSE(p1->pressure().has_value());
	EXPECT_FALSE(p1->toUnit(metaf::Pressure::Unit::INCHES_HG).has_value());
	EXPECT_FALSE(p1->toUnit(metaf::Pressure::Unit::HECTOPASCAL).has_value());

	const auto p2 = metaf::Pressure::fromString("Q////");
	ASSERT_TRUE(p2.has_value());
	EXPECT_FALSE(p2->pressure().has_value());
	EXPECT_FALSE(p2->toUnit(metaf::Pressure::Unit::INCHES_HG).has_value());
	EXPECT_FALSE(p2->toUnit(metaf::Pressure::Unit::HECTOPASCAL).has_value());
}

TEST(Pressure, toUnitHectopascalToInHg) {
	const auto p = metaf::Pressure::fromString("Q1000");
	ASSERT_TRUE(p.has_value());
	EXPECT_TRUE(p->toUnit(metaf::Pressure::Unit::INCHES_HG).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Pressure::Unit::INCHES_HG).value(), 29.53, margin);
}

TEST(Pressure, toUnitHectopascalToMmHg) {
	const auto p = metaf::Pressure::fromString("Q1000");
	ASSERT_TRUE(p.has_value());
	EXPECT_TRUE(p->toUnit(metaf::Pressure::Unit::MM_HG).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Pressure::Unit::MM_HG).value(), 750.075, margin);
}

TEST(Pressure, toUnitInHgToHectopascal) {
	const auto p = metaf::Pressure::fromString("A2953");
	ASSERT_TRUE(p.has_value());
	EXPECT_TRUE(p->toUnit(metaf::Pressure::Unit::HECTOPASCAL).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Pressure::Unit::HECTOPASCAL).value(), 1000, margin);
}

TEST(Pressure, toUnitInHgToMmHg) {
	const auto p = metaf::Pressure::fromString("A2953");
	ASSERT_TRUE(p.has_value());
	EXPECT_TRUE(p->toUnit(metaf::Pressure::Unit::MM_HG).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Pressure::Unit::MM_HG).value(), 750.06, margin);
}

TEST(Pressure, toUnitMmHgToHectopascal) {
	const auto p = metaf::Pressure::fromQfeString("QFE765");
	ASSERT_TRUE(p.has_value());
	EXPECT_TRUE(p->toUnit(metaf::Pressure::Unit::HECTOPASCAL).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Pressure::Unit::HECTOPASCAL).value(), 1019.90, margin);
}

TEST(Pressure, toUnitMmHgToInHg) {
	const auto p = metaf::Pressure::fromQfeString("QFE765");
	ASSERT_TRUE(p.has_value());
	EXPECT_TRUE(p->toUnit(metaf::Pressure::Unit::INCHES_HG).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Pressure::Unit::INCHES_HG).value(), 30.12, margin);
}

TEST(Pressure, isReported) {
	const auto pQ0994 = metaf::Pressure::fromString("Q0994");
	ASSERT_TRUE(pQ0994.has_value());
	EXPECT_TRUE(pQ0994->isReported());

	const auto pSlp982 = metaf::Pressure::fromSlpString("SLP982");
	ASSERT_TRUE(pSlp982.has_value());
	EXPECT_TRUE(pSlp982->isReported());

	const auto pQfe750 = metaf::Pressure::fromQfeString("QFE750");
	ASSERT_TRUE(pQfe750.has_value());
	EXPECT_TRUE(pQfe750->isReported());

	const auto pT032 = metaf::Pressure::fromTendencyString("032");
	ASSERT_TRUE(pT032.has_value());
	EXPECT_TRUE(pT032->isReported());

	const auto pTnr = metaf::Pressure::fromTendencyString("///");
	ASSERT_TRUE(pTnr.has_value());
	EXPECT_FALSE(pTnr->isReported());

	const auto pnr = metaf::Pressure::fromString("Q////");
	ASSERT_TRUE(pnr.has_value());
	EXPECT_FALSE(pnr->isReported());
}