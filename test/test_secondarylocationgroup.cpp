/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 1.0 / 100 / 2;

///////////////////////////////////////////////////////////////////////////////
// VISNO
///////////////////////////////////////////////////////////////////////////////

TEST(SecondaryLocationGroup, parseVisno) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg1->type(), metaf::SecondaryLocationGroup::Type::VISNO);
	EXPECT_FALSE(slg1->runway().has_value());
	EXPECT_FALSE(slg1->direction().has_value());
	EXPECT_FALSE(slg1->height().isReported());
	EXPECT_FALSE(slg1->minHeight().isReported());
	EXPECT_FALSE(slg1->maxHeight().isReported());
	EXPECT_FALSE(slg1->visibility().isReported());
	EXPECT_FALSE(slg1->minVisibility().isReported());
	EXPECT_FALSE(slg1->maxVisibility().isReported());

	auto slg2 = metaf::SecondaryLocationGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->type(), metaf::SecondaryLocationGroup::Type::VISNO);
	EXPECT_EQ(slg2->append("R18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_TRUE(slg2->runway().has_value());
	EXPECT_EQ(slg2->runway()->number(), 18u);
	EXPECT_EQ(slg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(slg2->direction().has_value());
	EXPECT_FALSE(slg2->height().isReported());
	EXPECT_FALSE(slg2->minHeight().isReported());
	EXPECT_FALSE(slg2->maxHeight().isReported());
	EXPECT_FALSE(slg2->visibility().isReported());
	EXPECT_FALSE(slg2->minVisibility().isReported());
	EXPECT_FALSE(slg2->maxVisibility().isReported());

	auto slg3 = metaf::SecondaryLocationGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->type(), metaf::SecondaryLocationGroup::Type::VISNO);
	EXPECT_EQ(slg3->append("RWY24L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_TRUE(slg3->runway().has_value());
	EXPECT_EQ(slg3->runway()->number(), 24u);
	EXPECT_EQ(slg3->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_FALSE(slg3->direction().has_value());
	EXPECT_FALSE(slg3->height().isReported());
	EXPECT_FALSE(slg3->minHeight().isReported());
	EXPECT_FALSE(slg3->maxHeight().isReported());
	EXPECT_FALSE(slg3->visibility().isReported());
	EXPECT_FALSE(slg3->minVisibility().isReported());
	EXPECT_FALSE(slg3->maxVisibility().isReported());

	auto slg4 = metaf::SecondaryLocationGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg4.has_value());
	EXPECT_EQ(slg4->type(), metaf::SecondaryLocationGroup::Type::VISNO);
	EXPECT_EQ(slg4->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg4->runway().has_value());
	ASSERT_TRUE(slg4->direction().has_value());
	EXPECT_EQ(slg4->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(slg4->direction()->cardinal(), metaf::Direction::Cardinal::SW);
	EXPECT_FALSE(slg4->height().isReported());
	EXPECT_FALSE(slg4->minHeight().isReported());
	EXPECT_FALSE(slg4->maxHeight().isReported());
	EXPECT_FALSE(slg4->visibility().isReported());
	EXPECT_FALSE(slg4->minVisibility().isReported());
	EXPECT_FALSE(slg4->maxVisibility().isReported());
}

TEST(SecondaryLocationGroup, isValidVisnoValidRunway) {
	auto slg = metaf::SecondaryLocationGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg.has_value());
	EXPECT_EQ(slg->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg->isValid());
}

TEST(SecondaryLocationGroup, isValidVisnoWrongRunway) {
	auto slg = metaf::SecondaryLocationGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg.has_value());
	EXPECT_EQ(slg->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg->isValid());
}

TEST(SecondaryLocationGroup, isValidVisnoNoRunway) {
	auto slg = metaf::SecondaryLocationGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg.has_value());
	EXPECT_EQ(slg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_TRUE(slg->isValid());
}

TEST(SecondaryLocationGroup, isValidVisnoDirection) {
	auto slg = metaf::SecondaryLocationGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg.has_value());
	EXPECT_EQ(slg->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Runway visibility
///////////////////////////////////////////////////////////////////////////////

TEST(SecondaryLocationGroup, parseRunwayVariableVisibility) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("RWY24L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg1->type(), metaf::SecondaryLocationGroup::Type::VARIABLE_VISIBILITY);
	ASSERT_TRUE(slg1->runway().has_value());
	EXPECT_EQ(slg1->runway()->number(), 24u);
	EXPECT_EQ(slg1->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_FALSE(slg1->direction().has_value());
	EXPECT_FALSE(slg1->height().isReported());
	EXPECT_FALSE(slg1->minHeight().isReported());
	EXPECT_FALSE(slg1->maxHeight().isReported());
	EXPECT_FALSE(slg1->visibility().isReported());
	ASSERT_TRUE(slg1->minVisibility().distance().has_value());
	EXPECT_EQ(slg1->minVisibility().distance().value(), 1u);
	EXPECT_EQ(slg1->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg1->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(slg1->maxVisibility().distance().has_value());
	EXPECT_EQ(slg1->maxVisibility().distance().value(), 3u);
	EXPECT_EQ(slg1->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg1->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("1/4V2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->append("R18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->type(), metaf::SecondaryLocationGroup::Type::VARIABLE_VISIBILITY);
	ASSERT_TRUE(slg2->runway().has_value());
	EXPECT_EQ(slg2->runway()->number(), 18u);
	EXPECT_EQ(slg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(slg2->direction().has_value());
	EXPECT_FALSE(slg2->height().isReported());
	EXPECT_FALSE(slg2->minHeight().isReported());
	EXPECT_FALSE(slg2->maxHeight().isReported());
	EXPECT_FALSE(slg2->visibility().isReported());
	ASSERT_TRUE(slg2->minVisibility().distance().has_value());
	EXPECT_NEAR(slg2->minVisibility().distance().value(), 0.25, margin);
	EXPECT_EQ(slg2->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg2->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(slg2->maxVisibility().distance().has_value());
	EXPECT_NEAR(slg2->maxVisibility().distance().value(), 2, margin);
	EXPECT_EQ(slg2->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg2->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("1/8V1/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("RWY24R", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->type(), metaf::SecondaryLocationGroup::Type::VARIABLE_VISIBILITY);
	ASSERT_TRUE(slg3->runway().has_value());
	EXPECT_EQ(slg3->runway()->number(), 24u);
	EXPECT_EQ(slg3->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_FALSE(slg3->direction().has_value());
	EXPECT_FALSE(slg3->height().isReported());
	EXPECT_FALSE(slg3->minHeight().isReported());
	EXPECT_FALSE(slg3->maxHeight().isReported());
	EXPECT_FALSE(slg3->visibility().isReported());
	EXPECT_EQ(slg3->minVisibility().distance().value(), 1.0 / 8);
	EXPECT_EQ(slg3->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg3->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(slg3->maxVisibility().distance().value(), 1.0 / 4);
	EXPECT_EQ(slg3->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg3->minVisibility().modifier(), metaf::Distance::Modifier::NONE);

	auto slg4 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg4.has_value());
	EXPECT_EQ(slg4->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("3/4V2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("RWY24C", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->type(), metaf::SecondaryLocationGroup::Type::VARIABLE_VISIBILITY);
	ASSERT_TRUE(slg4->runway().has_value());
	EXPECT_EQ(slg4->runway()->number(), 24u);
	EXPECT_EQ(slg4->runway()->designator(), metaf::Runway::Designator::CENTER);
	EXPECT_FALSE(slg4->direction().has_value());
	EXPECT_FALSE(slg4->height().isReported());
	EXPECT_FALSE(slg4->minHeight().isReported());
	EXPECT_FALSE(slg4->maxHeight().isReported());
	EXPECT_FALSE(slg4->visibility().isReported());
	ASSERT_TRUE(slg4->minVisibility().distance().has_value());
	EXPECT_NEAR(slg4->minVisibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(slg4->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg4->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(slg4->maxVisibility().distance().has_value());
	EXPECT_NEAR(slg4->maxVisibility().distance().value(), 2, margin);
	EXPECT_EQ(slg4->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg4->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
}

TEST(SecondaryLocationGroup, parseRunwayNonVariableVisibility) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("RWY24L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg1->type(), metaf::SecondaryLocationGroup::Type::VISIBILITY);
	ASSERT_TRUE(slg1->runway().has_value());
	EXPECT_EQ(slg1->runway()->number(), 24u);
	EXPECT_EQ(slg1->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_FALSE(slg1->direction().has_value());
	EXPECT_FALSE(slg1->height().isReported());
	EXPECT_FALSE(slg1->minHeight().isReported());
	EXPECT_FALSE(slg1->maxHeight().isReported());
	ASSERT_TRUE(slg1->visibility().distance().has_value());
	EXPECT_NEAR(slg1->visibility().distance().value(), 2, margin);
	EXPECT_EQ(slg1->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg1->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(slg1->minVisibility().isReported());
	EXPECT_FALSE(slg1->maxVisibility().isReported());

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->append("R18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->type(), metaf::SecondaryLocationGroup::Type::VISIBILITY);
	ASSERT_TRUE(slg2->runway().has_value());
	EXPECT_EQ(slg2->runway()->number(), 18u);
	EXPECT_EQ(slg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(slg2->direction().has_value());
	EXPECT_FALSE(slg2->height().isReported());
	EXPECT_FALSE(slg2->minHeight().isReported());
	EXPECT_FALSE(slg2->maxHeight().isReported());
	EXPECT_NEAR(slg2->visibility().distance().value(), 3.0 / 4, margin);
	EXPECT_EQ(slg2->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg2->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(slg2->minVisibility().isReported());
	EXPECT_FALSE(slg2->maxVisibility().isReported());

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("15", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("RWY24R", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->type(), metaf::SecondaryLocationGroup::Type::VISIBILITY);
	ASSERT_TRUE(slg3->runway().has_value());
	EXPECT_EQ(slg3->runway()->number(), 24u);
	EXPECT_EQ(slg3->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_FALSE(slg3->direction().has_value());
	EXPECT_FALSE(slg3->height().isReported());
	EXPECT_FALSE(slg3->minHeight().isReported());
	EXPECT_FALSE(slg3->maxHeight().isReported());
	ASSERT_TRUE(slg3->visibility().isReported());
	EXPECT_NEAR(slg3->visibility().distance().value(), 15, margin);
	EXPECT_EQ(slg3->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg3->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(slg3->minVisibility().isReported());
	EXPECT_FALSE(slg3->maxVisibility().isReported());

	auto slg4 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg4.has_value());
	EXPECT_EQ(slg4->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("RWY24C", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->type(), metaf::SecondaryLocationGroup::Type::VISIBILITY);
	ASSERT_TRUE(slg4->runway().has_value());
	EXPECT_EQ(slg4->runway()->number(), 24u);
	EXPECT_EQ(slg4->runway()->designator(), metaf::Runway::Designator::CENTER);
	EXPECT_FALSE(slg4->direction().has_value());
	EXPECT_FALSE(slg4->height().isReported());
	EXPECT_FALSE(slg4->minHeight().isReported());
	EXPECT_FALSE(slg4->maxHeight().isReported());
	ASSERT_TRUE(slg4->visibility().isReported());
	EXPECT_EQ(slg4->visibility().distance().value(), 1.0 + 3.0 / 4.0);
	EXPECT_EQ(slg4->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg4->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(slg4->minVisibility().isReported());
	EXPECT_FALSE(slg4->maxVisibility().isReported());
}

TEST(SecondaryLocationGroup, parseVis) {
	auto slg = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg.has_value());

	EXPECT_FALSE(slg->runway().has_value());
	EXPECT_FALSE(slg->direction().has_value());

	EXPECT_FALSE(slg->height().isReported());
	EXPECT_FALSE(slg->minHeight().isReported());
	EXPECT_FALSE(slg->maxHeight().isReported());

	EXPECT_FALSE(slg->visibility().isReported());
	EXPECT_FALSE(slg->minVisibility().isReported());
	EXPECT_FALSE(slg->maxVisibility().isReported());
}

TEST(SecondaryLocationGroup, appendOtherToVis) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("RWY32", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("CHINO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("VISNO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg4 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg4.has_value());
	EXPECT_EQ(slg4->append("CIG", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg5 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg5.has_value());
	EXPECT_EQ(slg5->append("WS", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
	
	auto slg6 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg6.has_value());
	EXPECT_EQ(slg6->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg7 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg7.has_value());
	EXPECT_EQ(slg7->append("100", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg8 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg8.has_value());
	EXPECT_EQ(slg8->append("5SM", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg9 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg9.has_value());
	EXPECT_EQ(slg9->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg10 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg10.has_value());
	EXPECT_EQ(slg10->append("M1/4", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg11 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg11.has_value());
	EXPECT_EQ(slg11->append("P6", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(SecondaryLocationGroup, parseVisInteger) {
	auto slg = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg.has_value());
	EXPECT_EQ(slg->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_FALSE(slg->runway().has_value());
	EXPECT_FALSE(slg->direction().has_value());

	EXPECT_FALSE(slg->height().isReported());
	EXPECT_FALSE(slg->minHeight().isReported());
	EXPECT_FALSE(slg->maxHeight().isReported());

	EXPECT_TRUE(slg->visibility().isReported());
	EXPECT_FALSE(slg->minVisibility().isReported());
	EXPECT_FALSE(slg->maxVisibility().isReported());
}

TEST(SecondaryLocationGroup, appendOtherToVisInteger) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("1/2SM", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->append("CHINO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("VISNO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg4 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg4.has_value());
	EXPECT_EQ(slg4->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("CIG", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg5 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg5.has_value());
	EXPECT_EQ(slg5->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg5->append("WS", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
	
	auto slg6 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg6.has_value());
	EXPECT_EQ(slg6->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg6->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg7 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg7.has_value());
	EXPECT_EQ(slg7->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg7->append("100", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg8 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg8.has_value());
	EXPECT_EQ(slg8->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg8->append("5SM", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg9 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg9.has_value());
	EXPECT_EQ(slg9->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg9->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg10 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg10.has_value());
	EXPECT_EQ(slg10->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg10->append("2", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg11 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg11.has_value());
	EXPECT_EQ(slg11->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg11->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(SecondaryLocationGroup, parseAppendOtherToVisNonVariable) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->append("3", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg4 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg4.has_value());
	EXPECT_EQ(slg4->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("R0A", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg5 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg5.has_value());
	EXPECT_EQ(slg5->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg5->append("R32Z", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg6 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg6.has_value());
	EXPECT_EQ(slg6->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg6->append("R182", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(SecondaryLocationGroup, parseAppendOtherToVisVariable) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("2V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("2V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->append("3", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("2V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg4 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg4.has_value());
	EXPECT_EQ(slg4->append("2V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("R0A", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg5 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg5.has_value());
	EXPECT_EQ(slg5->append("2V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg5->append("R32Z", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto slg6 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg6.has_value());
	EXPECT_EQ(slg6->append("2V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg6->append("R182", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(SecondaryLocationGroup, parseAppendOtherToCompleteGroup) {
	auto slg = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg.has_value());
	EXPECT_EQ(slg->append("2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg->append("RWY24L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(slg->type(), metaf::SecondaryLocationGroup::Type::VISIBILITY);
	ASSERT_TRUE(slg->runway().has_value());
	EXPECT_EQ(slg->runway()->number(), 24u);
	EXPECT_EQ(slg->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_FALSE(slg->direction().has_value());
	EXPECT_FALSE(slg->height().isReported());
	EXPECT_FALSE(slg->minHeight().isReported());
	EXPECT_FALSE(slg->maxHeight().isReported());
	ASSERT_TRUE(slg->visibility().isReported());
	EXPECT_EQ(slg->visibility().distance().value(), 2u);
	EXPECT_EQ(slg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(slg->minVisibility().isReported());
	EXPECT_FALSE(slg->maxVisibility().isReported());

	EXPECT_EQ(slg->append("RWY24R", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("RWY24L", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("WS", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("VISNO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("CHINO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("CIG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("7", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg->append("10", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(slg->type(), metaf::SecondaryLocationGroup::Type::VISIBILITY);
	ASSERT_TRUE(slg->runway().has_value());
	EXPECT_EQ(slg->runway()->number(), 24u);
	EXPECT_EQ(slg->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_FALSE(slg->direction().has_value());
	EXPECT_FALSE(slg->height().isReported());
	EXPECT_FALSE(slg->minHeight().isReported());
	EXPECT_FALSE(slg->maxHeight().isReported());
	ASSERT_TRUE(slg->visibility().isReported());
	EXPECT_EQ(slg->visibility().distance().value(), 2u);
	EXPECT_EQ(slg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(slg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(slg->minVisibility().isReported());
	EXPECT_FALSE(slg->maxVisibility().isReported());
}

TEST(SecondaryLocationGroup, isValidNonVariableVisibilityValidRunway) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg1->isValid());

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg2->isValid());

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg3->isValid());
}

TEST(SecondaryLocationGroup, isValidNonVariableVisibilityWrongRunway) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg1->isValid());

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg2->isValid());

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg3->isValid());
}

TEST(SecondaryLocationGroup, isValidVariableVisibilityValidRunway) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("3V5", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg1->isValid());

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("1/2V4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg2->isValid());

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("1/4V1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg3->isValid());

	auto slg4 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg4.has_value());
	EXPECT_EQ(slg4->append("2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg4->isValid());
}

TEST(SecondaryLocationGroup, isValidVariableVisibilityWrongRunway) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("3V5", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg1->isValid());

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("1/2V4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg2->isValid());

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("1/4V1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg3->isValid());

	auto slg4 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg4.has_value());
	EXPECT_EQ(slg4->append("2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg4->isValid());
}

TEST(SecondaryLocationGroup, isValidVariableVisibilityWrongValues) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg1.has_value());
	EXPECT_EQ(slg1->append("3V2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("RWY18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg1->isValid());

	auto slg2 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg2.has_value());
	EXPECT_EQ(slg2->append("1V1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg2->append("RWY18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg2->isValid());

	auto slg3 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg3.has_value());
	EXPECT_EQ(slg3->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("1/2V1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg3->append("RWY18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg3->isValid());

	auto slg4 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg4.has_value());
	EXPECT_EQ(slg4->append("1V1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg4->append("RWY18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg4->isValid());

	auto slg5 = metaf::SecondaryLocationGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(slg5.has_value());
	EXPECT_EQ(slg5->append("1/4V1/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg5->append("RWY18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg5->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Wrong format generally
///////////////////////////////////////////////////////////////////////////////

TEST(SecondaryLocationGroup, wrongFormat) {
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("W0", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("ALL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("RWY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("R32", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("R27C", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("010V012", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("010", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("CCG", metaf::ReportPart::METAR).has_value());
}

