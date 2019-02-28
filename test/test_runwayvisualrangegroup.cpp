/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(RunwayVisualRangeGroup, parseSingleValueMeters) {
	const auto rvrg = metaf::RunwayVisualRangeGroup::parse("R05/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg.has_value());
	EXPECT_EQ(rvrg->runway().number(), 5u);
	EXPECT_EQ(rvrg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg->visualRange().isReported());
	EXPECT_EQ(rvrg->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg->visualRange().isInteger());
	ASSERT_TRUE(rvrg->visualRange().integer().has_value());
	EXPECT_EQ(rvrg->visualRange().integer().value(), 1100u);
	EXPECT_EQ(rvrg->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(rvrg->variableVisualRange().isReported());
	EXPECT_EQ(rvrg->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseRunwayWithDesignator) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R05R/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 5u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_TRUE(rvrg1->visualRange().isReported());
	EXPECT_FALSE(rvrg1->variableVisualRange().isReported());
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R05C/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 5u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::CENTER);
	EXPECT_TRUE(rvrg2->visualRange().isReported());
	EXPECT_FALSE(rvrg2->variableVisualRange().isReported());
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R05L/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	EXPECT_EQ(rvrg3->runway().number(), 5u);
	EXPECT_EQ(rvrg3->runway().designator(), metaf::Runway::Designator::LEFT);
	EXPECT_TRUE(rvrg3->visualRange().isReported());
	EXPECT_FALSE(rvrg3->variableVisualRange().isReported());
	EXPECT_EQ(rvrg3->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseSingleValueMetersWithModifiers) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R04/P2000", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 4u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg1->visualRange().isReported());
	EXPECT_EQ(rvrg1->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg1->visualRange().isInteger());
	ASSERT_TRUE(rvrg1->visualRange().integer().has_value());
	EXPECT_EQ(rvrg1->visualRange().integer().value(), 2000u);
	EXPECT_EQ(rvrg1->visualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_FALSE(rvrg1->variableVisualRange().isReported());
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R21/M0200", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 21u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg2->visualRange().isReported());
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg2->visualRange().isInteger());
	ASSERT_TRUE(rvrg2->visualRange().integer().has_value());
	EXPECT_EQ(rvrg2->visualRange().integer().value(), 200u);
	EXPECT_EQ(rvrg2->visualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_FALSE(rvrg2->variableVisualRange().isReported());
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseSingleValueFeet) {
	const auto rvrg = metaf::RunwayVisualRangeGroup::parse("R11/4500FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg.has_value());
	EXPECT_EQ(rvrg->runway().number(), 11u);
	EXPECT_EQ(rvrg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg->visualRange().isReported());
	EXPECT_EQ(rvrg->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg->visualRange().isInteger());
	ASSERT_TRUE(rvrg->visualRange().integer().has_value());
	EXPECT_EQ(rvrg->visualRange().integer().value(), 4500u);
	EXPECT_EQ(rvrg->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(rvrg->variableVisualRange().isReported());
	EXPECT_EQ(rvrg->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseSingleValueFeetWithModifiers) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R11/P6000FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 11u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg1->visualRange().isReported());
	EXPECT_EQ(rvrg1->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg1->visualRange().isInteger());
	ASSERT_TRUE(rvrg1->visualRange().integer().has_value());
	EXPECT_EQ(rvrg1->visualRange().integer().value(), 6000u);
	EXPECT_EQ(rvrg1->visualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_FALSE(rvrg1->variableVisualRange().isReported());
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R12/M0400FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 12u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg2->visualRange().isReported());
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg2->visualRange().isInteger());
	ASSERT_TRUE(rvrg2->visualRange().integer().has_value());
	EXPECT_EQ(rvrg2->visualRange().integer().value(), 400u);
	EXPECT_EQ(rvrg2->visualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_FALSE(rvrg2->variableVisualRange().isReported());
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseSingleValueWithTrends) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R05/1100U", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 5u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg1->visualRange().isReported());
	EXPECT_EQ(rvrg1->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg1->visualRange().isInteger());
	ASSERT_TRUE(rvrg1->visualRange().integer().has_value());
	EXPECT_EQ(rvrg1->visualRange().integer().value(), 1100u);
	EXPECT_EQ(rvrg1->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(rvrg1->variableVisualRange().isReported());
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::UPWARD);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R05/1000FT/D", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 5u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg2->visualRange().isReported());
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg2->visualRange().isInteger());
	ASSERT_TRUE(rvrg2->visualRange().integer().has_value());
	EXPECT_EQ(rvrg2->visualRange().integer().value(), 1000u);
	EXPECT_EQ(rvrg2->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(rvrg2->variableVisualRange().isReported());
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::DOWNWARD);

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R05/M1100FT/N", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	EXPECT_EQ(rvrg3->runway().number(), 5u);
	EXPECT_EQ(rvrg3->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg3->visualRange().isReported());
	EXPECT_EQ(rvrg3->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg3->visualRange().isInteger());
	ASSERT_TRUE(rvrg3->visualRange().integer().has_value());
	EXPECT_EQ(rvrg3->visualRange().integer().value(), 1100u);
	EXPECT_EQ(rvrg3->visualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_FALSE(rvrg3->variableVisualRange().isReported());
	EXPECT_EQ(rvrg3->trend(), metaf::RunwayVisualRangeGroup::Trend::NEUTRAL);

	const auto rvrg4 = metaf::RunwayVisualRangeGroup::parse("R05/P1000N", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg4.has_value());
	EXPECT_EQ(rvrg4->runway().number(), 5u);
	EXPECT_EQ(rvrg4->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg4->visualRange().isReported());
	EXPECT_EQ(rvrg4->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg4->visualRange().isInteger());
	ASSERT_TRUE(rvrg4->visualRange().integer().has_value());
	EXPECT_EQ(rvrg4->visualRange().integer().value(), 1000u);
	EXPECT_EQ(rvrg4->visualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_FALSE(rvrg4->variableVisualRange().isReported());
	EXPECT_EQ(rvrg4->trend(), metaf::RunwayVisualRangeGroup::Trend::NEUTRAL);
}

TEST(RunwayVisualRangeGroup, parseVariableValueMeters) {
	const auto rvrg = metaf::RunwayVisualRangeGroup::parse("R09/0500V0800", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg.has_value());
	EXPECT_EQ(rvrg->runway().number(), 9u);
	EXPECT_EQ(rvrg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg->visualRange().isReported());
	EXPECT_EQ(rvrg->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg->visualRange().isInteger());
	ASSERT_TRUE(rvrg->visualRange().integer().has_value());
	EXPECT_EQ(rvrg->visualRange().integer().value(), 500u);
	EXPECT_EQ(rvrg->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg->variableVisualRange().isReported());
	EXPECT_EQ(rvrg->variableVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg->variableVisualRange().integer().value(), 800u);
	EXPECT_EQ(rvrg->variableVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseVariableValueFeet) {
	const auto rvrg = metaf::RunwayVisualRangeGroup::parse("R14/0600V1200FT", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg.has_value());
	EXPECT_EQ(rvrg->runway().number(), 14u);
	EXPECT_EQ(rvrg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg->visualRange().isReported());
	EXPECT_EQ(rvrg->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg->visualRange().isInteger());
	ASSERT_TRUE(rvrg->visualRange().integer().has_value());
	EXPECT_EQ(rvrg->visualRange().integer().value(), 600u);
	EXPECT_EQ(rvrg->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg->variableVisualRange().isReported());
	EXPECT_EQ(rvrg->variableVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg->variableVisualRange().integer().value(), 1200u);
	EXPECT_EQ(rvrg->variableVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseVariableValueMetersWithModifiers) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R09/M0500V0800", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 9u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg1->visualRange().isReported());
	EXPECT_EQ(rvrg1->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg1->visualRange().isInteger());
	ASSERT_TRUE(rvrg1->visualRange().integer().has_value());
	EXPECT_EQ(rvrg1->visualRange().integer().value(), 500u);
	EXPECT_EQ(rvrg1->visualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(rvrg1->variableVisualRange().isReported());
	EXPECT_EQ(rvrg1->variableVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg1->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg1->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg1->variableVisualRange().integer().value(), 800u);
	EXPECT_EQ(rvrg1->variableVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R09/0500VP0800", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 9u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg2->visualRange().isReported());
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg2->visualRange().isInteger());
	ASSERT_TRUE(rvrg2->visualRange().integer().has_value());
	EXPECT_EQ(rvrg2->visualRange().integer().value(), 500u);
	EXPECT_EQ(rvrg2->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg2->variableVisualRange().isReported());
	EXPECT_EQ(rvrg2->variableVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg2->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg2->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg2->variableVisualRange().integer().value(), 800u);
	EXPECT_EQ(rvrg2->variableVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R09/M0500VP0800", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	EXPECT_EQ(rvrg3->runway().number(), 9u);
	EXPECT_EQ(rvrg3->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg3->visualRange().isReported());
	EXPECT_EQ(rvrg3->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg3->visualRange().isInteger());
	ASSERT_TRUE(rvrg3->visualRange().integer().has_value());
	EXPECT_EQ(rvrg3->visualRange().integer().value(), 500u);
	EXPECT_EQ(rvrg3->visualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(rvrg3->variableVisualRange().isReported());
	EXPECT_EQ(rvrg3->variableVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg3->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg3->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg3->variableVisualRange().integer().value(), 800u);
	EXPECT_EQ(rvrg3->variableVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg3->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseVariableValueFeetWithModifiers) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R14/M0600V1200FT", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 14u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg1->visualRange().isReported());
	EXPECT_EQ(rvrg1->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg1->visualRange().isInteger());
	ASSERT_TRUE(rvrg1->visualRange().integer().has_value());
	EXPECT_EQ(rvrg1->visualRange().integer().value(), 600u);
	EXPECT_EQ(rvrg1->visualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(rvrg1->variableVisualRange().isReported());
	EXPECT_EQ(rvrg1->variableVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg1->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg1->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg1->variableVisualRange().integer().value(), 1200u);
	EXPECT_EQ(rvrg1->variableVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R14/0600VP1200FT", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 14u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg2->visualRange().isReported());
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg2->visualRange().isInteger());
	ASSERT_TRUE(rvrg2->visualRange().integer().has_value());
	EXPECT_EQ(rvrg2->visualRange().integer().value(), 600u);
	EXPECT_EQ(rvrg2->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg2->variableVisualRange().isReported());
	EXPECT_EQ(rvrg2->variableVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg2->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg2->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg2->variableVisualRange().integer().value(), 1200u);
	EXPECT_EQ(rvrg2->variableVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R14/M0600VP1200FT", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	EXPECT_EQ(rvrg3->runway().number(), 14u);
	EXPECT_EQ(rvrg3->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg3->visualRange().isReported());
	EXPECT_EQ(rvrg3->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg3->visualRange().isInteger());
	ASSERT_TRUE(rvrg3->visualRange().integer().has_value());
	EXPECT_EQ(rvrg3->visualRange().integer().value(), 600u);
	EXPECT_EQ(rvrg3->visualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(rvrg3->variableVisualRange().isReported());
	EXPECT_EQ(rvrg3->variableVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg3->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg3->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg3->variableVisualRange().integer().value(), 1200u);
	EXPECT_EQ(rvrg3->variableVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg3->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseVariableValueWithTrends) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R17/0250V0450N", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 17u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg1->visualRange().isReported());
	EXPECT_EQ(rvrg1->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg1->visualRange().isInteger());
	ASSERT_TRUE(rvrg1->visualRange().integer().has_value());
	EXPECT_EQ(rvrg1->visualRange().integer().value(), 250u);
	EXPECT_EQ(rvrg1->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg1->variableVisualRange().isReported());
	EXPECT_EQ(rvrg1->variableVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg1->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg1->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg1->variableVisualRange().integer().value(), 450u);
	EXPECT_EQ(rvrg1->variableVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NEUTRAL);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R17/M0250VP0450D", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 17u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg2->visualRange().isReported());
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg2->visualRange().isInteger());
	ASSERT_TRUE(rvrg2->visualRange().integer().has_value());
	EXPECT_EQ(rvrg2->visualRange().integer().value(), 250u);
	EXPECT_EQ(rvrg2->visualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(rvrg2->variableVisualRange().isReported());
	EXPECT_EQ(rvrg2->variableVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg2->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg2->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg2->variableVisualRange().integer().value(), 450u);
	EXPECT_EQ(rvrg2->variableVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::DOWNWARD);

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R26/2600V3200FT/D", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	EXPECT_EQ(rvrg3->runway().number(), 26u);
	EXPECT_EQ(rvrg3->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg3->visualRange().isReported());
	EXPECT_EQ(rvrg3->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg3->visualRange().isInteger());
	ASSERT_TRUE(rvrg3->visualRange().integer().has_value());
	EXPECT_EQ(rvrg3->visualRange().integer().value(), 2600u);
	EXPECT_EQ(rvrg3->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg3->variableVisualRange().isReported());
	EXPECT_EQ(rvrg3->variableVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg3->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg3->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg3->variableVisualRange().integer().value(), 3200u);
	EXPECT_EQ(rvrg3->variableVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg3->trend(), metaf::RunwayVisualRangeGroup::Trend::DOWNWARD);

	const auto rvrg4 = metaf::RunwayVisualRangeGroup::parse("R30/5000VP6000FT/U", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg4.has_value());
	EXPECT_EQ(rvrg4->runway().number(), 30u);
	EXPECT_EQ(rvrg4->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg4->visualRange().isReported());
	EXPECT_EQ(rvrg4->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg4->visualRange().isInteger());
	ASSERT_TRUE(rvrg4->visualRange().integer().has_value());
	EXPECT_EQ(rvrg4->visualRange().integer().value(), 5000u);
	EXPECT_EQ(rvrg4->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg4->variableVisualRange().isReported());
	EXPECT_EQ(rvrg4->variableVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg4->variableVisualRange().isInteger());
	ASSERT_TRUE(rvrg4->variableVisualRange().integer().has_value());
	EXPECT_EQ(rvrg4->variableVisualRange().integer().value(), 6000u);
	EXPECT_EQ(rvrg4->variableVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg4->trend(), metaf::RunwayVisualRangeGroup::Trend::UPWARD);
}

TEST(RunwayVisualRangeGroup, parseMalformedTrends) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R08/6000FT/", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 8u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg1->visualRange().isReported());
	EXPECT_EQ(rvrg1->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg1->visualRange().isInteger());
	ASSERT_TRUE(rvrg1->visualRange().integer().has_value());
	EXPECT_EQ(rvrg1->visualRange().integer().value(), 6000u);
	EXPECT_EQ(rvrg1->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(rvrg1->variableVisualRange().isReported());
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R08/6000FTD", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 8u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(rvrg2->visualRange().isReported());
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg2->visualRange().isInteger());
	ASSERT_TRUE(rvrg2->visualRange().integer().has_value());
	EXPECT_EQ(rvrg2->visualRange().integer().value(), 6000u);
	EXPECT_EQ(rvrg2->visualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(rvrg2->variableVisualRange().isReported());
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::DOWNWARD);
}

TEST(RunwayVisualRangeGroup, parseValueMetersNotReported) {
	const auto rvrg = metaf::RunwayVisualRangeGroup::parse("R06R/////", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg.has_value());
	EXPECT_EQ(rvrg->runway().number(), 6u);
	EXPECT_EQ(rvrg->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rvrg->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(rvrg->visualRange().isReported());
	EXPECT_FALSE(rvrg->variableVisualRange().isReported());
	EXPECT_EQ(rvrg->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseValueFeetNotReported) {
	const auto rvrg = metaf::RunwayVisualRangeGroup::parse("R06R/////FT", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg.has_value());
	EXPECT_EQ(rvrg->runway().number(), 6u);
	EXPECT_EQ(rvrg->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rvrg->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(rvrg->visualRange().isReported());
	EXPECT_FALSE(rvrg->variableVisualRange().isReported());
	EXPECT_EQ(rvrg->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseNotReportedWithTrend) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R06R/////N", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 6u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rvrg1->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(rvrg1->visualRange().isReported());
	EXPECT_FALSE(rvrg1->variableVisualRange().isReported());
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NEUTRAL);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R06R/////FT/N", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 6u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(rvrg2->visualRange().isReported());
	EXPECT_FALSE(rvrg2->variableVisualRange().isReported());
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::NEUTRAL);
}

TEST(RunwayVisualRangeGroup, parseNotReportedWithTrendNotReported) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R06R//////", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 6u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rvrg1->visualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(rvrg1->visualRange().isReported());
	EXPECT_FALSE(rvrg1->variableVisualRange().isReported());
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NOT_REPORTED);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R06R/////FT//", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 6u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(rvrg2->visualRange().isReported());
	EXPECT_FALSE(rvrg2->variableVisualRange().isReported());
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::NOT_REPORTED);
}

TEST(RunwayVisualRangeGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100", metaf::ReportPart::RMK));
}

TEST(RunwayVisualRangeGroup, parseWrongFormat) {
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R5/1100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R005/1100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05X/1100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05R1100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/110", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/01100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/110A", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/Z1100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100VZ2000", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100Z2000", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100V2000FF", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100V2000A", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100V2000/N", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100V2000FT/A", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100V////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/0400V800", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R05/1100V02000", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R06R////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayVisualRangeGroup::parse("R06R///////", metaf::ReportPart::METAR));
}
