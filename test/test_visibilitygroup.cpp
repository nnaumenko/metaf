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
// Test parsing of prevailing & directional visibility in meters (e.g. 1600, 
// 9999, ////, 9999NDV, 1500S, etc.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseMetersMetar) {
	const auto vg = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersTaf) {
	const auto vg = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::TAF);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersNDV) {
	const auto vg = metaf::VisibilityGroup::parse("9999NDV", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_NDV);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 10000, margin);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::NDV);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionNorth) {
	const auto vg = metaf::VisibilityGroup::parse("9999N", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionSouth) {
	const auto vg = metaf::VisibilityGroup::parse("9999S", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::S);
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionWest) {
	const auto vg = metaf::VisibilityGroup::parse("9999W", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionEast) {
	const auto vg = metaf::VisibilityGroup::parse("9999E", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionNorthWest) {
	const auto vg = metaf::VisibilityGroup::parse("9999NW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::NW);
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionNorthEast) {
	const auto vg = metaf::VisibilityGroup::parse("9999NE", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionSouthWest) {
	const auto vg = metaf::VisibilityGroup::parse("9999SW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::SW);
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionSouthEast) {
	const auto vg = metaf::VisibilityGroup::parse("9999SE", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersNotReported) {
	const auto vg = metaf::VisibilityGroup::parse("////", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(!vg->visibility().isReported());
	EXPECT_FALSE(vg->visibility().distance().has_value());
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersWrongFormat) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("04000", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("400", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("400A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("4000NS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("4000WN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("4000SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("4000N0V", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("/////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("///", metaf::ReportPart::METAR).has_value());
}

TEST(VisibilityGroup, parseMetersWrongReportPart) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("1600", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("1600", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("1600", metaf::ReportPart::RMK).has_value());
}

TEST(VisibilityGroup, appendMetersAndFraction) {
	const std::string fractionStr("1/4SM");

	const auto vgFraction = metaf::VisibilityGroup::parse(fractionStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFraction.has_value());

	auto vg = metaf::VisibilityGroup::parse("1200", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append(fractionStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1200, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, appendMetersAndRunway) {
	auto vg = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("R05", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("RWY05", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of prevailing visibility in statute miles (e.g. 3SM, 15SM, 
// P6SM, 1/4SM, M1/4SM, ////SM, 11/2SM, 2 1/4SM, etc.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseMilesInteger) {
	const auto vg = metaf::VisibilityGroup::parse("3SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 3, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMilesIntegerTwoDigit) {
	const auto vg = metaf::VisibilityGroup::parse("15SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 15, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMilesIntegerWithModifier) {
	const auto vg = metaf::VisibilityGroup::parse("P6SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMilesFraction) {
	const auto vg = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0/4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMilesFractionWithModifier) {
	const auto vg = metaf::VisibilityGroup::parse("M1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0/4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMilesIntegerNotReported) {
	const auto vg = metaf::VisibilityGroup::parse("////SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
}

TEST(VisibilityGroup, parseMilesWrongFormat) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("/2SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("1/2", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("A/2SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("P6SMNW", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("X6SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("/////SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("///SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("1/0SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("M1/0SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("0/2SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("M0/2SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("10/2SM", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("M10/2SM", metaf::ReportPart::METAR).has_value());
}

TEST(VisibilityGroup, parseMilesIncomplete) {
	const auto vg1 = metaf::VisibilityGroup::parse("1", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_EQ(vg1->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg1->visibility().isReported());
	ASSERT_TRUE(vg1->visibility().distance().has_value());
	EXPECT_NEAR(vg1->visibility().distance().value(), 1, margin);
	EXPECT_EQ(vg1->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg1->direction().has_value());
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);

	const auto vg9 = metaf::VisibilityGroup::parse("9", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg9.has_value());
	EXPECT_EQ(vg9->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_EQ(vg9->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg9->visibility().isReported());
	ASSERT_TRUE(vg9->visibility().distance().has_value());
	EXPECT_NEAR(vg9->visibility().distance().value(), 9, margin);
	EXPECT_EQ(vg9->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg9->direction().has_value());
	EXPECT_FALSE(vg9->minVisibility().isReported());
	EXPECT_FALSE(vg9->maxVisibility().isReported());
	EXPECT_EQ(vg9->sectorDirections().size(), 0u);

	const auto vg10 = metaf::VisibilityGroup::parse("10", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg10.has_value());
	EXPECT_EQ(vg10->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_EQ(vg10->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg10->visibility().isReported());
	ASSERT_TRUE(vg10->visibility().distance().has_value());
	EXPECT_NEAR(vg10->visibility().distance().value(), 10, margin);
	EXPECT_EQ(vg10->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg10->direction().has_value());
	EXPECT_FALSE(vg10->minVisibility().isReported());
	EXPECT_FALSE(vg10->maxVisibility().isReported());
	EXPECT_EQ(vg10->sectorDirections().size(), 0u);
}

TEST(VisibilityGroup, parseMilesIncompleteWrongFormat) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("/", metaf::ReportPart::METAR).has_value());
}

TEST(VisibilityGroup, appendIncompleteAndFraction) {
	auto vg = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/4SM", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 2.0 + 1.0/4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, appendIncompleteAndOther) {
	auto vg = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("2", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);
	EXPECT_EQ(vg->append("1200", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);
	EXPECT_EQ(vg->append("1SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);
	EXPECT_EQ(vg->append("M1/4SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);
	EXPECT_EQ(vg->append("R06", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);
	EXPECT_EQ(vg->append("RWY06", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 2, margin);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
}

TEST(VisibilityGroup, appendFractionAndIncomplete) {
	const std::string incompleteStr ("2");

	const auto vgIncomplete = metaf::VisibilityGroup::parse(incompleteStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(vgIncomplete.has_value());

	auto vg = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append(incompleteStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, appendFractionAndRunway) {
	auto vg = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("RWY05", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("R05", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_EQ(vg->visibility().distance().value(), 1.0 / 4.0);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, appendMilesIntegerAndFraction) {
	const std::string fractionStr("1/4SM");

	const auto vgFraction = metaf::VisibilityGroup::parse(fractionStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFraction.has_value());

	auto vg = metaf::VisibilityGroup::parse("1SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append(fractionStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, appendIntegerMilesWithModifierAndFraction) {
	const std::string fractionStr("1/4SM");

	const auto vgFraction = metaf::VisibilityGroup::parse(fractionStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFraction.has_value());

	auto vg = metaf::VisibilityGroup::parse("P2SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append(fractionStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 2, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, appendInvalidFraction) {
	auto vg = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/0SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, appendToCompleteIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/4SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->append("9999", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("9999NDV", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("3000E", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("1", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("1/2SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("3SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("P6SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("////", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("TEST", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("RMK", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("R05", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("RWY05", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 2.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseInvalidVisbilityMiles) {
	const auto vg = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_FALSE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of VIS MISG
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseVisMisg) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VIS_MISG);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of variable, directional, surface, tower visibility,  given in 
// remarks, e.g. VIS 1 1/2V2, VIS 1V3, VIS 1/8V1/4, VIS N 1 1/2, VIS NW 1V3, 
// VIS N 1/2V1, VIS N 1/4V1/2, SFC VIS 4, SFC VIS 1/2, TWR VIS 3, TWR VIS 1/2
///////////////////////////////////////////////////////////////////////////////

// Variable visibility

TEST(VisibilityGroup, parseRmkVisVariableIntegerInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 3, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisVariableFractionInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/2V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 2.0, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 3, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisVariableFractionFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/8V3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 8.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 3.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisIntegerVariableFractionInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 2.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 3, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisVariableIntegerIntegerTwoDigit) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("10V15", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 10, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 15, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisIntegerTwoDigitVariable) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("10", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/2V11", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 10.0 + 1.0 / 2.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 11, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisVariableFirstFractionTwoDigit) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("11/16V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 11.0 / 16.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 1, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisVariableSecondFractionTwoDigit) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/8V15/16", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 8.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 15.0 / 16.0, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

// Variable visibility in meters

TEST(VisibilityGroup, parseRmkVisVariableMeters) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1500V3000", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_VARIABLE);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_FALSE(vg->visibility().isReported());

	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1500, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 3000, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisVariableMetersIncorrectNotReported) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("////V3000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("1200V////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

// Incorrect visibility groups

TEST(VisibilityGroup, incompleteRmkVis){
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	ASSERT_EQ(vg->append("2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisIncorrectFraction) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	ASSERT_EQ(vg1->append("0/2", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	ASSERT_EQ(vg2->append("0/2V2", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	ASSERT_EQ(vg3->append("1/8V0/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	ASSERT_EQ(vg4->append("1/0", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg5 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	ASSERT_EQ(vg5->append("1/0V2", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg6 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg6.has_value());
	ASSERT_EQ(vg6->append("1/8V1/0", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkAppendOtherToVis) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("100", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("1/8SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("1500", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg5 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("////SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkAppendOtherToVisInteger) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("100", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/8SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1500", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg5 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("////SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg6 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg6.has_value());
	EXPECT_EQ(vg6->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg6->append("2", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkVisIncorrectMeters) {
	//TODO
}

// Prevailing non-variable visibility not allowed in remarks, e.g. VIS 1/2 
// or VIS 1500 (it is specified in report body or trend instead, e.g. 1/2SM or 1500)

TEST(VisibilityGroup, parseRmkVisIncorrectNotVariable) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkVisIncorrectNonVariableMeters) {
	//TODO
}

// Appending runway is not allowed since this is SecondaryLocationGroup rather
// than VisibilityGroup

TEST(VisibilityGroup, parseRmkVisAppendRunway) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("1V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("R18", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("1/2V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("RWY18", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("RWY18", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkVisMetersAppendRunway) {
	//TODO
}

// Directional variable visibility

TEST(VisibilityGroup, parseRmkVisDirectionalVariableIntegerInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 3, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalVariableFractionInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/2V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 2.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 3, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalVariableFractionFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/8V3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 8.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_EQ(vg->maxVisibility().distance().value(), 3.0 / 4.0);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalIntegerVariableFractionInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 2.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 3, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalVariableMeters) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1200V9999", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL_VARIABLE);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1200, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 10000, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

// Directional non-variable visibility

TEST(VisibilityGroup, parseRmkVisDirectionalNonVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalNonVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 2, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalNonVariableFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 3.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

// Incorrect directional visibility groups

TEST(VisibilityGroup, parseRmkVisDirectionalVariableAppendRunway) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("SE", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("1V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("R18", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("SE", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/2V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("RWY18", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkVisDirectionalNonVariableAppendRunway) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("SE", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("1/2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("R18", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("SE", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("RWY18", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkAppendOtherToVisDirection) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("100", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/8SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1500", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg5 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("////SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkAppendOtherToVisDirectionInteger) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("100", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/8SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1500", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	auto vg4 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	auto vg5 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("////SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	auto vg6 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg6.has_value());
	EXPECT_EQ(vg6->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg6->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg6->append("2", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}

// Non-variable directional visibility in meters is not allowed (e.g. VIS W 1500),
// it is specified in the report body or trend instead (e.g 1500W)

TEST(VisibilityGroup, parseRmkVisDirectionalNonVariableMeters) {
	//TODO
}

// Surface visibility

TEST(VisibilityGroup, parseRmkSfcVisIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE_VISIBILITY);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkSfcVisInteger) {
	auto vg = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE_VISIBILITY);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 4, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkSfcVisFraction) {
	auto vg = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/8", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE_VISIBILITY);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 / 8.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

// Incorrect surface visibility groups

TEST(VisibilityGroup, parseRmkSfcVisAppendRunway) {
	auto vg1 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	ASSERT_EQ(vg1->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	ASSERT_EQ(vg1->append("4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("R18", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	auto vg2 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	ASSERT_EQ(vg2->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	ASSERT_EQ(vg2->append("4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("RWY18", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}

TEST(VisibilityGroup, parseRmkSfcVisIncorrectDirectional) {
	auto vg = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkAppendOtherToSfc) {
	auto vg1 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("1/8", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("2", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

}

TEST(VisibilityGroup, parseRmkAppendOtherToSfcVis) {
	auto vg1 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("100", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/8SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1500", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkAppendVariableToSfc) {
	auto vg1 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("1V2", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/8V1/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1/2V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

// Tower visibility

TEST(VisibilityGroup, parseRmkTwrVisIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER_VISIBILITY);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 3.0/4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkTwrVisInteger) {
	auto vg = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER_VISIBILITY);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 4, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkTwrVisFraction) {
	auto vg = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/8", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER_VISIBILITY);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 / 8.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_TRUE(vg->isValid());
}

// Incorrect tower visibility groups

TEST(VisibilityGroup, parseRmkTwrVisAppendRunway) {
	auto vg1 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	ASSERT_EQ(vg1->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	ASSERT_EQ(vg1->append("4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("R18", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	auto vg2 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	ASSERT_EQ(vg2->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	ASSERT_EQ(vg2->append("4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("RWY18", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}

TEST(VisibilityGroup, parseRmkTwrVisIncorrectDirectional) {
	auto vg = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkAppendOtherToTwr) {
	auto vg1 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("1/8", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("2", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

}

TEST(VisibilityGroup, parseRmkAppendOtherToTwrVis) {
	auto vg1 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/8SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1500", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkAppendVariableToTwr) {
	auto vg1 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("1V2", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/8V1/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1/2V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

// Attempt to append subsequent group to complete groups

TEST(VisibilityGroup, parseRmkAppendToCompleteRmkGroups) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg1->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg1->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg1->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg2->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg2->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg2->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("S", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg3->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg3->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg3->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto vg4 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("4V6", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg4->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg4->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg4->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto vg5 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg5->append("3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg5->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg5->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg5->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto vg6 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg6.has_value());
	EXPECT_EQ(vg6->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg6->append("3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg6->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg6->append("3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg6->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg6->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg6->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}
