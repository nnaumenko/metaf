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
	EXPECT_FALSE(rvrg->isVariableVisualRange());
	EXPECT_FALSE(rvrg->minVisualRange().isReported());
	EXPECT_FALSE(rvrg->maxVisualRange().isReported());
	EXPECT_EQ(rvrg->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseRunwayWithDesignator) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R05R/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 5u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_TRUE(rvrg1->visualRange().isReported());
	EXPECT_FALSE(rvrg1->isVariableVisualRange());
	EXPECT_FALSE(rvrg1->minVisualRange().isReported());
	EXPECT_FALSE(rvrg1->maxVisualRange().isReported());
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R05C/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 5u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::CENTER);
	EXPECT_TRUE(rvrg2->visualRange().isReported());
	EXPECT_FALSE(rvrg2->isVariableVisualRange());
	EXPECT_FALSE(rvrg2->minVisualRange().isReported());
	EXPECT_FALSE(rvrg2->maxVisualRange().isReported());
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R05L/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	EXPECT_EQ(rvrg3->runway().number(), 5u);
	EXPECT_EQ(rvrg3->runway().designator(), metaf::Runway::Designator::LEFT);
	EXPECT_TRUE(rvrg3->visualRange().isReported());
	EXPECT_FALSE(rvrg3->isVariableVisualRange());
	EXPECT_FALSE(rvrg3->minVisualRange().isReported());
	EXPECT_FALSE(rvrg3->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg1->isVariableVisualRange());
	EXPECT_FALSE(rvrg1->minVisualRange().isReported());
	EXPECT_FALSE(rvrg1->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg2->isVariableVisualRange());
	EXPECT_FALSE(rvrg2->minVisualRange().isReported());
	EXPECT_FALSE(rvrg2->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg->isVariableVisualRange());
	EXPECT_FALSE(rvrg->minVisualRange().isReported());
	EXPECT_FALSE(rvrg->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg1->isVariableVisualRange());
	EXPECT_FALSE(rvrg1->minVisualRange().isReported());
	EXPECT_FALSE(rvrg1->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg2->isVariableVisualRange());
	EXPECT_FALSE(rvrg2->minVisualRange().isReported());
	EXPECT_FALSE(rvrg2->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg1->isVariableVisualRange());
	EXPECT_FALSE(rvrg1->minVisualRange().isReported());
	EXPECT_FALSE(rvrg1->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg2->isVariableVisualRange());
	EXPECT_FALSE(rvrg2->minVisualRange().isReported());
	EXPECT_FALSE(rvrg2->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg3->isVariableVisualRange());
	EXPECT_FALSE(rvrg3->minVisualRange().isReported());
	EXPECT_FALSE(rvrg3->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg4->isVariableVisualRange());
	EXPECT_FALSE(rvrg4->minVisualRange().isReported());
	EXPECT_FALSE(rvrg4->maxVisualRange().isReported());
	EXPECT_EQ(rvrg4->trend(), metaf::RunwayVisualRangeGroup::Trend::NEUTRAL);
}

TEST(RunwayVisualRangeGroup, parseVariableValueMeters) {
	const auto rvrg = metaf::RunwayVisualRangeGroup::parse("R09/0500V0800", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg.has_value());
	EXPECT_EQ(rvrg->runway().number(), 9u);
	EXPECT_EQ(rvrg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg->visualRange().isReported());
	EXPECT_TRUE(rvrg->isVariableVisualRange());
	EXPECT_TRUE(rvrg->minVisualRange().isReported());
	EXPECT_EQ(rvrg->minVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg->minVisualRange().integer().value(), 500u);
	EXPECT_EQ(rvrg->minVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg->maxVisualRange().isReported());
	EXPECT_EQ(rvrg->maxVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg->maxVisualRange().integer().value(), 800u);
	EXPECT_EQ(rvrg->maxVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseVariableValueFeet) {
	const auto rvrg = metaf::RunwayVisualRangeGroup::parse("R14/0600V1200FT", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg.has_value());
	EXPECT_EQ(rvrg->runway().number(), 14u);
	EXPECT_EQ(rvrg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg->visualRange().isReported());
	EXPECT_TRUE(rvrg->isVariableVisualRange());
	EXPECT_TRUE(rvrg->minVisualRange().isReported());
	EXPECT_EQ(rvrg->minVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg->minVisualRange().integer().value(), 600u);
	EXPECT_EQ(rvrg->minVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg->maxVisualRange().isReported());
	EXPECT_EQ(rvrg->maxVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg->maxVisualRange().integer().value(), 1200u);
	EXPECT_EQ(rvrg->maxVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseVariableValueMetersWithModifiers) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R09/M0500V0800", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 9u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg1->visualRange().isReported());
	EXPECT_TRUE(rvrg1->isVariableVisualRange());
	EXPECT_TRUE(rvrg1->minVisualRange().isReported());
	EXPECT_EQ(rvrg1->minVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg1->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg1->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg1->minVisualRange().integer().value(), 500u);
	EXPECT_EQ(rvrg1->minVisualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(rvrg1->maxVisualRange().isReported());
	EXPECT_EQ(rvrg1->maxVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg1->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg1->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg1->maxVisualRange().integer().value(), 800u);
	EXPECT_EQ(rvrg1->maxVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R09/0500VP0800", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 9u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg2->visualRange().isReported());
	EXPECT_TRUE(rvrg2->isVariableVisualRange());
	EXPECT_TRUE(rvrg2->minVisualRange().isReported());
	EXPECT_TRUE(rvrg2->minVisualRange().isReported());
	EXPECT_EQ(rvrg2->minVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg2->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg2->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg2->minVisualRange().integer().value(), 500u);
	EXPECT_EQ(rvrg2->minVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg2->maxVisualRange().isReported());
	EXPECT_EQ(rvrg2->maxVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg2->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg2->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg2->maxVisualRange().integer().value(), 800u);
	EXPECT_EQ(rvrg2->maxVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R09/M0500VP0800", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	EXPECT_EQ(rvrg3->runway().number(), 9u);
	EXPECT_EQ(rvrg3->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg3->visualRange().isReported());
	EXPECT_TRUE(rvrg3->isVariableVisualRange());
	EXPECT_TRUE(rvrg3->minVisualRange().isReported());
	EXPECT_EQ(rvrg3->minVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg3->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg3->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg3->minVisualRange().integer().value(), 500u);
	EXPECT_EQ(rvrg3->minVisualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(rvrg3->maxVisualRange().isReported());
	EXPECT_EQ(rvrg3->maxVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg3->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg3->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg3->maxVisualRange().integer().value(), 800u);
	EXPECT_EQ(rvrg3->maxVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg3->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseVariableValueFeetWithModifiers) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R14/M0600V1200FT", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 14u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg1->visualRange().isReported());
	EXPECT_TRUE(rvrg1->isVariableVisualRange());
	EXPECT_TRUE(rvrg1->minVisualRange().isReported());
	EXPECT_EQ(rvrg1->minVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg1->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg1->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg1->minVisualRange().integer().value(), 600u);
	EXPECT_EQ(rvrg1->minVisualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(rvrg1->maxVisualRange().isReported());
	EXPECT_EQ(rvrg1->maxVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg1->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg1->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg1->maxVisualRange().integer().value(), 1200u);
	EXPECT_EQ(rvrg1->maxVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R14/0600VP1200FT", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 14u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg2->visualRange().isReported());
	EXPECT_TRUE(rvrg2->isVariableVisualRange());
	EXPECT_TRUE(rvrg2->minVisualRange().isReported());
	EXPECT_EQ(rvrg2->minVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg2->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg2->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg2->minVisualRange().integer().value(), 600u);
	EXPECT_EQ(rvrg2->minVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg2->maxVisualRange().isReported());
	EXPECT_EQ(rvrg2->maxVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg2->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg2->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg2->maxVisualRange().integer().value(), 1200u);
	EXPECT_EQ(rvrg2->maxVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R14/M0600VP1200FT", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	EXPECT_EQ(rvrg3->runway().number(), 14u);
	EXPECT_EQ(rvrg3->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg3->visualRange().isReported());
	EXPECT_TRUE(rvrg3->isVariableVisualRange());
	EXPECT_TRUE(rvrg3->minVisualRange().isReported());
	EXPECT_EQ(rvrg3->minVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg3->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg3->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg3->minVisualRange().integer().value(), 600u);
	EXPECT_EQ(rvrg3->minVisualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(rvrg3->maxVisualRange().isReported());
	EXPECT_EQ(rvrg3->maxVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg3->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg3->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg3->maxVisualRange().integer().value(), 1200u);
	EXPECT_EQ(rvrg3->maxVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg3->trend(), metaf::RunwayVisualRangeGroup::Trend::NONE);
}

TEST(RunwayVisualRangeGroup, parseVariableValueWithTrends) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R17/0250V0450N", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_EQ(rvrg1->runway().number(), 17u);
	EXPECT_EQ(rvrg1->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg1->visualRange().isReported());
	EXPECT_TRUE(rvrg1->isVariableVisualRange());
	EXPECT_TRUE(rvrg1->minVisualRange().isReported());
	EXPECT_EQ(rvrg1->minVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg1->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg1->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg1->minVisualRange().integer().value(), 250u);
	EXPECT_EQ(rvrg1->minVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg1->maxVisualRange().isReported());
	EXPECT_EQ(rvrg1->maxVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg1->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg1->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg1->maxVisualRange().integer().value(), 450u);
	EXPECT_EQ(rvrg1->maxVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NEUTRAL);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R17/M0250VP0450D", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 17u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg2->visualRange().isReported());
	EXPECT_TRUE(rvrg2->isVariableVisualRange());
	EXPECT_TRUE(rvrg2->minVisualRange().isReported());
	EXPECT_EQ(rvrg2->minVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg2->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg2->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg2->minVisualRange().integer().value(), 250u);
	EXPECT_EQ(rvrg2->minVisualRange().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(rvrg2->maxVisualRange().isReported());
	EXPECT_EQ(rvrg2->maxVisualRange().unit(), metaf::Distance::Unit::METERS);
	EXPECT_TRUE(rvrg2->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg2->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg2->maxVisualRange().integer().value(), 450u);
	EXPECT_EQ(rvrg2->maxVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(rvrg2->trend(), metaf::RunwayVisualRangeGroup::Trend::DOWNWARD);

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R26/2600V3200FT/D", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	EXPECT_EQ(rvrg3->runway().number(), 26u);
	EXPECT_EQ(rvrg3->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg3->visualRange().isReported());
	EXPECT_TRUE(rvrg3->isVariableVisualRange());
	EXPECT_TRUE(rvrg3->minVisualRange().isReported());
	EXPECT_EQ(rvrg3->minVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg3->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg3->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg3->minVisualRange().integer().value(), 2600u);
	EXPECT_EQ(rvrg3->minVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg3->maxVisualRange().isReported());
	EXPECT_EQ(rvrg3->maxVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg3->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg3->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg3->maxVisualRange().integer().value(), 3200u);
	EXPECT_EQ(rvrg3->maxVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(rvrg3->trend(), metaf::RunwayVisualRangeGroup::Trend::DOWNWARD);

	const auto rvrg4 = metaf::RunwayVisualRangeGroup::parse("R30/5000VP6000FT/U", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg4.has_value());
	EXPECT_EQ(rvrg4->runway().number(), 30u);
	EXPECT_EQ(rvrg4->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(rvrg4->visualRange().isReported());
	EXPECT_TRUE(rvrg4->isVariableVisualRange());
	EXPECT_TRUE(rvrg4->minVisualRange().isReported());
	EXPECT_EQ(rvrg4->minVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg4->minVisualRange().isInteger());
	ASSERT_TRUE(rvrg4->minVisualRange().integer().has_value());
	EXPECT_EQ(rvrg4->minVisualRange().integer().value(), 5000u);
	EXPECT_EQ(rvrg4->minVisualRange().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(rvrg4->maxVisualRange().isReported());
	EXPECT_EQ(rvrg4->maxVisualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(rvrg4->maxVisualRange().isInteger());
	ASSERT_TRUE(rvrg4->maxVisualRange().integer().has_value());
	EXPECT_EQ(rvrg4->maxVisualRange().integer().value(), 6000u);
	EXPECT_EQ(rvrg4->maxVisualRange().modifier(), metaf::Distance::Modifier::MORE_THAN);
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
	EXPECT_FALSE(rvrg1->isVariableVisualRange());
	EXPECT_FALSE(rvrg1->minVisualRange().isReported());
	EXPECT_FALSE(rvrg1->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg2->isVariableVisualRange());
	EXPECT_FALSE(rvrg2->minVisualRange().isReported());
	EXPECT_FALSE(rvrg2->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg->isVariableVisualRange());
	EXPECT_FALSE(rvrg->minVisualRange().isReported());
	EXPECT_FALSE(rvrg->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg->isVariableVisualRange());
	EXPECT_FALSE(rvrg->minVisualRange().isReported());
	EXPECT_FALSE(rvrg->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg1->isVariableVisualRange());
	EXPECT_FALSE(rvrg1->minVisualRange().isReported());
	EXPECT_FALSE(rvrg1->maxVisualRange().isReported());
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NEUTRAL);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R06R/////FT/N", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 6u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(rvrg2->visualRange().isReported());
	EXPECT_FALSE(rvrg2->isVariableVisualRange());
	EXPECT_FALSE(rvrg2->minVisualRange().isReported());
	EXPECT_FALSE(rvrg2->maxVisualRange().isReported());
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
	EXPECT_FALSE(rvrg1->isVariableVisualRange());
	EXPECT_FALSE(rvrg1->minVisualRange().isReported());
	EXPECT_FALSE(rvrg1->maxVisualRange().isReported());
	EXPECT_EQ(rvrg1->trend(), metaf::RunwayVisualRangeGroup::Trend::NOT_REPORTED);

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R06R/////FT//", 
		metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	EXPECT_EQ(rvrg2->runway().number(), 6u);
	EXPECT_EQ(rvrg2->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rvrg2->visualRange().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(rvrg2->visualRange().isReported());
	EXPECT_FALSE(rvrg2->isVariableVisualRange());
	EXPECT_FALSE(rvrg2->minVisualRange().isReported());
	EXPECT_FALSE(rvrg2->maxVisualRange().isReported());
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

TEST(RunwayVisualRangeGroup, isValidTrue) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R88/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	ASSERT_TRUE(rvrg1->runway().isValid());
	EXPECT_TRUE(rvrg1->isValid());

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R99/2600V3200FT/D", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	ASSERT_TRUE(rvrg2->runway().isValid());
	EXPECT_TRUE(rvrg2->isValid());

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R36R/////FT/N", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	ASSERT_TRUE(rvrg3->runway().isValid());
	EXPECT_TRUE(rvrg3->isValid());

	const auto rvrg4 = metaf::RunwayVisualRangeGroup::parse("R06L//////", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg4.has_value());
	ASSERT_TRUE(rvrg4->runway().isValid());
	EXPECT_TRUE(rvrg4->isValid());
}

TEST(RunwayVisualRangeGroup, isValidFalse) {
	const auto rvrg1 = metaf::RunwayVisualRangeGroup::parse("R88R/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());
	EXPECT_FALSE(rvrg1->runway().isValid());
	EXPECT_FALSE(rvrg1->isValid());

	const auto rvrg2 = metaf::RunwayVisualRangeGroup::parse("R99C/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());
	ASSERT_FALSE(rvrg2->runway().isValid());
	EXPECT_FALSE(rvrg2->isValid());

	const auto rvrg3 = metaf::RunwayVisualRangeGroup::parse("R37/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());
	ASSERT_FALSE(rvrg3->runway().isValid());
	EXPECT_FALSE(rvrg3->isValid());
}

TEST(RunwayVisualRangeGroup, append) {

	const std::string rvrgStr1("R88/1100");
	const std::string rvrgStr2("R99/2600V3200FT/D");
	const std::string rvrgStr3("R36R/////FT/N");
	const std::string rvrgStr4("R06L//////");

	const std::string tStr1("RMK");
	const std::string tStr2("TEST");

	auto rvrg1 = metaf::RunwayVisualRangeGroup::parse(rvrgStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg1.has_value());

	auto rvrg2 = metaf::RunwayVisualRangeGroup::parse(rvrgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg2.has_value());

	auto rvrg3 = metaf::RunwayVisualRangeGroup::parse(rvrgStr3, metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg3.has_value());

	auto rvrg4 = metaf::RunwayVisualRangeGroup::parse(rvrgStr4, metaf::ReportPart::METAR);
	ASSERT_TRUE(rvrg4.has_value());

	EXPECT_EQ(rvrg1->append(rvrgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg1->append(rvrgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg1->append(rvrgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg1->append(rvrgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg1->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg1->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(rvrg2->append(rvrgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg2->append(rvrgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg2->append(rvrgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg2->append(rvrgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg2->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg2->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(rvrg3->append(rvrgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg3->append(rvrgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg3->append(rvrgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg3->append(rvrgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg3->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg3->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(rvrg4->append(rvrgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg4->append(rvrgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg4->append(rvrgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg4->append(rvrgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg4->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rvrg4->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
}
