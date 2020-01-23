/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.01 / 2;

///////////////////////////////////////////////////////////////////////////////
// fromRainfallString() tests
// Purpose: to confirm that rainfall string from groups included in METAR 
// report in Australia are parsed correctly and incorrect string is not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(Precipitation, fromRainfallString) {
	const auto p1 = metaf::Precipitation::fromRainfallString("120.7");
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p1->amount().has_value());
	EXPECT_NEAR(p1->amount().value(), 120.7, margin);

	const auto p2 = metaf::Precipitation::fromRainfallString("061.5");
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p2->amount().has_value());
	EXPECT_NEAR(p2->amount().value(), 61.5, margin);

	const auto p3 = metaf::Precipitation::fromRainfallString("60.1");
	ASSERT_TRUE(p3.has_value());
	EXPECT_EQ(p3->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p3->amount().has_value());
	EXPECT_NEAR(p3->amount().value(), 60.1, margin);

	const auto p4 = metaf::Precipitation::fromRainfallString("04.7");
	ASSERT_TRUE(p4.has_value());
	EXPECT_EQ(p4->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p4->amount().has_value());
	EXPECT_NEAR(p4->amount().value(), 4.7, margin);

	const auto p5 = metaf::Precipitation::fromRainfallString("000.0");
	ASSERT_TRUE(p5.has_value());
	EXPECT_EQ(p5->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p5->amount().has_value());
	EXPECT_NEAR(p5->amount().value(), 0.0, margin);

	const auto p6 = metaf::Precipitation::fromRainfallString("00.0");
	ASSERT_TRUE(p6.has_value());
	EXPECT_EQ(p6->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p6->amount().has_value());
	EXPECT_NEAR(p6->amount().value(), 0.0, margin);
}

TEST(Precipitation, fromRainfallStringNotReported) {
	const auto p1 = metaf::Precipitation::fromRainfallString("///./");
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->unit(), metaf::Precipitation::Unit::MM);
	EXPECT_FALSE(p1->amount().has_value());

	const auto p2 = metaf::Precipitation::fromRainfallString("//./");
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->unit(), metaf::Precipitation::Unit::MM);
	EXPECT_FALSE(p2->amount().has_value());
}

TEST(Precipitation, fromRainfallStringWrongFormat) {
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString(" ").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("0040.2").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("4.2").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("040").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("40").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("040.").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("40.").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString(".2").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString(".").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("///.//").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("/./").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("///.").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("040,2").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("A40.2").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("040.A").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromRainfallString() tests
// Purpose: to confirm that runway deposit accumulation string from groups 
// included in METAR report in Europe are parsed correctly and incorrect string
// is not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(Precipitation, fromRunwayDepositsValue) {
	const auto p1 = metaf::Precipitation::fromRunwayDeposits("90");
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p1->amount().has_value());
	EXPECT_NEAR(p1->amount().value(), 90.0, margin);

	const auto p2 = metaf::Precipitation::fromRunwayDeposits("01");
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p2->amount().has_value());
	EXPECT_NEAR(p2->amount().value(), 1.0, margin);

	const auto p3 = metaf::Precipitation::fromRunwayDeposits("00");
	ASSERT_TRUE(p3.has_value());
	EXPECT_EQ(p3->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p3->amount().has_value());
	EXPECT_NEAR(p3->amount().value(), 0.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecialReserved) {
	EXPECT_FALSE(metaf::Precipitation::fromRunwayDeposits("91").has_value());
}

TEST(Precipitation, fromRunwayDepositsSpecial10cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("92");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->amount().has_value());
	EXPECT_NEAR(p->amount().value(), 100.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial15cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("93");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->amount().has_value());
	EXPECT_NEAR(p->amount().value(), 150.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial20cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("94");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->amount().has_value());
	EXPECT_NEAR(p->amount().value(), 200.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial25cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("95");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->amount().has_value());
	EXPECT_NEAR(p->amount().value(), 250.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial30cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("96");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->amount().has_value());
	EXPECT_NEAR(p->amount().value(), 300.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial35cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("97");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->amount().has_value());
	EXPECT_NEAR(p->amount().value(), 350.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial40cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("98");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->amount().has_value());
	EXPECT_NEAR(p->amount().value(), 400.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecialRunwayNotOperational) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("99");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	EXPECT_FALSE(p->amount().has_value());
}

TEST(Precipitation, fromRunwayDepositsNotReported) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("//");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	EXPECT_FALSE(p->amount().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromRemarkString() tests
// Purpose: to confirm that precipitation/accumulation string from groups 
// included in METAR remarks in North and South America are parsed correctly, 
// incorrect string is not parsed, and value factor is applied correctly
///////////////////////////////////////////////////////////////////////////////

TEST(Precipitation, fromRemarkString3digits) {
	const auto factor = 0.01;
	const auto unit = metaf::Precipitation::Unit::INCHES;
	const auto p = metaf::Precipitation::fromRemarkString("135", factor);
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), unit);
	ASSERT_TRUE(p->amount().has_value());
	EXPECT_NEAR(p->amount().value(), 1.35, factor / 2);
}
	
TEST(Precipitation, fromRemarkString4digits) {
	const auto factor = 0.01;
	const auto unit = metaf::Precipitation::Unit::INCHES;
	const auto p = metaf::Precipitation::fromRemarkString("1352", factor);
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), unit);
	ASSERT_TRUE(p->amount().has_value());
	EXPECT_NEAR(p->amount().value(), 13.52, factor / 2);
}

TEST(Precipitation, fromRemarkStringFactor) {
	const auto valueStr = "1482";
	const auto unit = metaf::Precipitation::Unit::INCHES;
	const auto value = std::stof(valueStr);

	const auto factor1 = 0.01;
	const auto p1 = metaf::Precipitation::fromRemarkString(valueStr, factor1);
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->unit(), unit);
	ASSERT_TRUE(p1->amount().has_value());
	EXPECT_NEAR(p1->amount().value(), value*factor1, factor1 / 2);

	const auto factor2 = 0.1;
	const auto p2 = metaf::Precipitation::fromRemarkString(valueStr, factor2);
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->unit(), unit);
	ASSERT_TRUE(p2->amount().has_value());
	EXPECT_NEAR(p2->amount().value(), value*factor2, factor2 / 2);

	const auto factor3 = 1;
	const auto p3 = metaf::Precipitation::fromRemarkString(valueStr, factor3);
	ASSERT_TRUE(p3.has_value());
	EXPECT_EQ(p3->unit(), unit);
	ASSERT_TRUE(p3->amount().has_value());
	EXPECT_NEAR(p3->amount().value(), value*factor3, factor3 / 2);
}

TEST(Precipitation, fromRemarkStringUnit) {
	const auto valueStr = "417";
	const auto factor = 0.01;
	const auto value = std::stof(valueStr) * factor;

	const auto unit1 = metaf::Precipitation::Unit::INCHES;
	const auto p1 = metaf::Precipitation::fromRemarkString(valueStr, factor, unit1);
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->unit(), unit1);
	ASSERT_TRUE(p1->amount().has_value());
	EXPECT_NEAR(p1->amount().value(), value, factor / 2);

	const auto unit2 = metaf::Precipitation::Unit::MM;
	const auto p2 = metaf::Precipitation::fromRemarkString(valueStr, factor, unit2);
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->unit(), unit2);
	ASSERT_TRUE(p2->amount().has_value());
	EXPECT_NEAR(p2->amount().value(), value, factor / 2);
}

TEST(Precipitation, fromRemarkStringNotReported) {
	EXPECT_FALSE(metaf::Precipitation::fromRemarkString("////", 
		0.01,
		metaf::Precipitation::Unit::INCHES).has_value());

	const auto p = metaf::Precipitation::fromRemarkString("////", 
		0.01, 
		metaf::Precipitation::Unit::INCHES, 
		true);
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::INCHES);
	EXPECT_FALSE(p->amount().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// toUnit() tests
// Purpose: to confirm that precipitation value is correctly converted into
// all measurement units, and non-reported values are handled correctly
///////////////////////////////////////////////////////////////////////////////

TEST(Precipitation, toUnitSameUnit) {
	const auto p1 = metaf::Precipitation::fromRainfallString("07.5");
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p1->toUnit(metaf::Precipitation::Unit::MM).has_value());
	EXPECT_NEAR(p1->toUnit(metaf::Precipitation::Unit::MM).value(), 7.5, margin);

	const auto p2 = metaf::Precipitation::fromRemarkString("025", 0.1);
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(p2->toUnit(metaf::Precipitation::Unit::INCHES).has_value());
	EXPECT_NEAR(p2->toUnit(metaf::Precipitation::Unit::INCHES).value(), 2.5, margin);
}

TEST(Precipitation, toUnitNotReported) {
	const auto p = metaf::Precipitation::fromRemarkString("///", 
		0.1, 
		metaf::Precipitation::Unit::INCHES,
		true);
	ASSERT_TRUE(p.has_value());
	EXPECT_FALSE(p->toUnit(metaf::Precipitation::Unit::MM).has_value());
	EXPECT_FALSE(p->toUnit(metaf::Precipitation::Unit::INCHES).has_value());
}

TEST(Precipitation, toUnitMmToInches) {
	const auto p = metaf::Precipitation::fromRainfallString("50.8");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->toUnit(metaf::Precipitation::Unit::INCHES).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Precipitation::Unit::INCHES).value(), 2.0, margin);
}

TEST(Precipitation, toUnitInchesToMm) {
	const auto p = metaf::Precipitation::fromRemarkString("010", 0.1);
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(p->toUnit(metaf::Precipitation::Unit::MM).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Precipitation::Unit::MM).value(), 25.4, margin);
}

///////////////////////////////////////////////////////////////////////////////
// isReported() tests
// Purpose: to confirm that isReported() method correctly distinguishes 
// between reported and non-reported 
///////////////////////////////////////////////////////////////////////////////

TEST(Precipitation, isReported) {
	const auto pRf0615 = metaf::Precipitation::fromRainfallString("061.5");
	ASSERT_TRUE(pRf0615.has_value());
	EXPECT_TRUE(pRf0615->isReported());

	const auto pRf508 = metaf::Precipitation::fromRainfallString("50.8");
	ASSERT_TRUE(pRf508.has_value());
	EXPECT_TRUE(pRf508->isReported());

	const auto pRfNr5 = metaf::Precipitation::fromRainfallString("///./");
	ASSERT_TRUE(pRfNr5.has_value());
	EXPECT_FALSE(pRfNr5->isReported());

	const auto pRnNr4 = metaf::Precipitation::fromRainfallString("//./");
	ASSERT_TRUE(pRnNr4.has_value());
	EXPECT_FALSE(pRnNr4->isReported());

	const auto pRd01 = metaf::Precipitation::fromRunwayDeposits("01");
	ASSERT_TRUE(pRd01.has_value());
	EXPECT_TRUE(pRd01->isReported());

	const auto pRdNr = metaf::Precipitation::fromRunwayDeposits("//");
	ASSERT_TRUE(pRdNr.has_value());
	EXPECT_FALSE(pRdNr->isReported());

	const auto pRmk010 = metaf::Precipitation::fromRemarkString("010", 0.1);
	ASSERT_TRUE(pRmk010.has_value());
	EXPECT_TRUE(pRmk010->isReported());

	const auto pRmkNr = metaf::Precipitation::fromRemarkString("////", 
		0.01, 
		metaf::Precipitation::Unit::INCHES, 
		true);
	ASSERT_TRUE(pRmkNr.has_value());
	EXPECT_FALSE(pRmkNr->isReported());
}