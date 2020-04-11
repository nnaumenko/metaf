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
// Test parsing of prevailing and directional visibility in meters (e.g. 1600, 
// 9999, ////, 9999NDV, 1500S, etc.), specified in report body or trend
// Purpose: to confirm that prevaling or directional visibility, or visibility
// value with no directional variation specified in METAR or TAF report body 
// or trend is parsed correctly, and malformed strings are not parsed.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseMetersMetar) {
	const auto vg = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersTaf) {
	const auto vg = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::TAF);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersNDV) {
	const auto vg = metaf::VisibilityGroup::parse("9999NDV", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_NDV);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 10000, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::NDV);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionNorth) {
	const auto vg = metaf::VisibilityGroup::parse("1600N", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionSouth) {
	const auto vg = metaf::VisibilityGroup::parse("1600S", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::S);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionWest) {
	const auto vg = metaf::VisibilityGroup::parse("1600W", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionEast) {
	const auto vg = metaf::VisibilityGroup::parse("1600E", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionNorthWest) {
	const auto vg = metaf::VisibilityGroup::parse("1600NW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::NW);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionNorthEast) {
	const auto vg = metaf::VisibilityGroup::parse("1600NE", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionSouthWest) {
	const auto vg = metaf::VisibilityGroup::parse("1600SW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::SW);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersDirectionSouthEast) {
	const auto vg = metaf::VisibilityGroup::parse("1600SE", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1600, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersNotReported) {
	const auto vg = metaf::VisibilityGroup::parse("////", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMetersNotReportedNdv) {
	const auto vg = metaf::VisibilityGroup::parse("////NDV", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_NDV);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::NDV);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(metaf::VisibilityGroup::parse("P4000", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("M4000", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("P////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("M////", metaf::ReportPart::METAR).has_value());
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of prevailing visibility in statute miles (e.g. 3SM, 15SM, 
// P6SM, 1/4SM, M1/4SM, ////SM, 11/2SM, 2 1/4SM, etc), specified in report body
// or trend.
// Purpose: to confirm that prevaling visibility values specified in METAR or 
// TAF report body or trend are parsed and appended correctly, and malformed 
// strings are not parsed.
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseMilesIntegerNotReported) {
	const auto vg = metaf::VisibilityGroup::parse("////SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
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
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_FALSE(vg1->isValid());

	const auto vg9 = metaf::VisibilityGroup::parse("9", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg9.has_value());
	EXPECT_EQ(vg9->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_EQ(vg9->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg9->visibility().isReported());
	ASSERT_TRUE(vg9->visibility().distance().has_value());
	EXPECT_NEAR(vg9->visibility().distance().value(), 9, margin);
	EXPECT_EQ(vg9->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg9->direction().has_value());
	EXPECT_FALSE(vg9->runway().has_value());
	EXPECT_FALSE(vg9->minVisibility().isReported());
	EXPECT_FALSE(vg9->maxVisibility().isReported());
	EXPECT_EQ(vg9->sectorDirections().size(), 0u);
	EXPECT_EQ(vg9->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_FALSE(vg9->isValid());

	const auto vg10 = metaf::VisibilityGroup::parse("10", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg10.has_value());
	EXPECT_EQ(vg10->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_EQ(vg10->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg10->visibility().isReported());
	ASSERT_TRUE(vg10->visibility().distance().has_value());
	EXPECT_NEAR(vg10->visibility().distance().value(), 10, margin);
	EXPECT_EQ(vg10->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_FALSE(vg10->direction().has_value());
	EXPECT_FALSE(vg10->runway().has_value());
	EXPECT_FALSE(vg10->minVisibility().isReported());
	EXPECT_FALSE(vg10->maxVisibility().isReported());
	EXPECT_EQ(vg10->sectorDirections().size(), 0u);
	EXPECT_EQ(vg10->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_FALSE(vg10->isValid());
}

TEST(VisibilityGroup, parseMilesIncompleteWrongFormat) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("M", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("P", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("/", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("M1", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("P1", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("M15", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("P15", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("M/", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("P/", metaf::ReportPart::METAR).has_value());
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of VIS MISG
// Purpose: to confirm that VIS MISG group is parsed and appended correctly.
// Note: cases when wrong strings are appended to VIS are tested in the other 
// sections where various VIS remarks are tested.
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
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseVisWrongReportPart) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::TAF).has_value());
}

TEST(VisibilityGroup, parseMisg) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("MISG", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("MISG", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("MISG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("MISG", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("MISG", metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of RVR MISG
// Purpose: to confirm that RVR MISG group is parsed and appended correctly.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRvrMisg) {
	auto vg = metaf::VisibilityGroup::parse("RVR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RVR_MISG);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, appendRvrAndOther) {
	auto vg1 = metaf::VisibilityGroup::parse("RVR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("RVR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("1/16SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("RVR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("0200", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VisibilityGroup::parse("RVR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("NW", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg5 = metaf::VisibilityGroup::parse("RVR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("R26", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRvrWrongReportPart) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("RVR", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("RVR", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("RVR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("RVR", metaf::ReportPart::TAF).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of RVRNO
// Purpose: to confirm that RVRNO group is parsed correctly and no other group
// can be appended.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRvrno) {
	auto vg = metaf::VisibilityGroup::parse("RVRNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RVRNO);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRvrnoWrongReportPart) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("RVRNO", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("RVRNO", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("RVRNO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("RVRNO", metaf::ReportPart::TAF).has_value());
}

TEST(VisibilityGroup, appendRvrno) {
	auto vg = metaf::VisibilityGroup::parse("RVRNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

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
	EXPECT_EQ(vg->append("NW", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RVRNO);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of VISNO
// Purpose: to confirm that VISNO groups for a direction or runway are parsed 
// and appended correctly.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseVisno) {
	auto vg = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VISNO);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseVisnoRunwayWithoutDesignator) {
	auto vg1 = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::VISNO);
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 22u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(vg1->visibility().isReported());
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg1->isValid());

	auto vg2 = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("RWY22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::VISNO);
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 22u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(vg2->visibility().isReported());
	EXPECT_FALSE(vg2->minVisibility().isReported());
	EXPECT_FALSE(vg2->maxVisibility().isReported());
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg2->isValid());
}

TEST(VisibilityGroup, parseVisnoRunwayWithDesignator) {
	auto vg1 = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("R22R", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::VISNO);
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 22u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_FALSE(vg1->visibility().isReported());
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg1->isValid());

	auto vg2 = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("R22L", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::VISNO);
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 22u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_FALSE(vg2->visibility().isReported());
	EXPECT_FALSE(vg2->minVisibility().isReported());
	EXPECT_FALSE(vg2->maxVisibility().isReported());
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg2->isValid());

	auto vg3 = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("R22C", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->type(), metaf::VisibilityGroup::Type::VISNO);
	EXPECT_FALSE(vg3->direction().has_value());
	ASSERT_TRUE(vg3->runway().has_value());
	EXPECT_EQ(vg3->runway()->number(), 22u);
	EXPECT_EQ(vg3->runway()->designator(), metaf::Runway::Designator::CENTER);
	EXPECT_FALSE(vg3->visibility().isReported());
	EXPECT_FALSE(vg3->minVisibility().isReported());
	EXPECT_FALSE(vg3->maxVisibility().isReported());
	EXPECT_EQ(vg3->sectorDirections().size(), 0u);
	EXPECT_EQ(vg3->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg3->isValid());
}

TEST(VisibilityGroup, parseVisnoAllRunways) {
	auto vg = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("R88", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VISNO);
	EXPECT_FALSE(vg->direction().has_value());
	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_TRUE(vg->runway()->isAllRunways());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseVisnoDirection) {
	auto vg = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("SW", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VISNO);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::SW);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseVisnoWrongReportPart) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::TAF).has_value());
}

TEST(VisibilityGroup, appendOtherToVisno) {
	auto vg = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

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
	EXPECT_EQ(vg->append("OHD", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("ALQDS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("UNKNOWN", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VISNO);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
}

TEST(VisibilityGroup, appendToVisnoDirection) {
	auto vg = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->append("", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
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
	EXPECT_EQ(vg->append("NW", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("OHD", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("ALQDS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("UNKNOWN", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VISNO);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::NW);
	EXPECT_FALSE(vg->runway().has_value());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
}

TEST(VisibilityGroup, appendToVisnoRunway) {
	auto vg = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->append("", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
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
	EXPECT_EQ(vg->append("NW", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("OHD", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("ALQDS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("UNKNOWN", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VISNO);
	EXPECT_FALSE(vg->direction().has_value());
	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
}

TEST(VisibilityGroup, isValidVisnoInvalidRunway) {
	auto vg = metaf::VisibilityGroup::parse("VISNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("R37", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Variable visibility in statute miles
// Purpose: to confirm that variable visibility groups in statute miles 
// specified in remarks (e.g. VIS 1 1/2V2, VIS 1V3, VIS 1/8V1/4, etc.) are 
// parsed and appended correctly.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRmkVisIntegerVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisFractionVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/2V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 2.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 3, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisFractionVariableFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/8V3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisIntegerFractionVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisIntegerTwoDigitVariableIntegerTwoDigit) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("10V15", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisIntegerTwoDigitFractionVariableIntegerTwoDigit) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("10", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/2V11", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisFractionTwoDigitVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("11/16V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisFractionVariableFractionTwoDigit) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/8V15/16", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisFractionVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("3/4V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 3.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 1.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisIntegerFractionVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkAppendToCompleteVarVisGroups) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg1->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg1->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg1->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/8V1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
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

	auto vg5 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg5->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg5->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg5->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg5->append("RWY22", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(VisibilityGroup, parseRmkVisModifierFractionVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("M1/4V5", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 5, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisFractionVariableModifierInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/4VP6", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisModifierFractionVariableModifierInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("M1/4VP6", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisModifierFractionVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("M1/4V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisModifierIntegerFractionVariableModifierIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("M1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4VP1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Variable visibility in meters
// Purpose: to confirm that variable visibility groups in meters specified in 
// remarks (e.g. 1500V3000) are parsed and appended correctly.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRmkVisVariableMeters) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1500V3000", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_PREVAILING);
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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Malformed variable visibility groups
// Purpose: to confirm that malformed variable visibility groups are not 
// parsed or appended, and that variable visibility groups where minimum
// and maximum values are not consistent are correctly identified
///////////////////////////////////////////////////////////////////////////////

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
	EXPECT_EQ(vg3->append("1500S", metaf::ReportPart::RMK), 
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
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("1000", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("1000W", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("01000V02000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("500V800", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg5 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("900V1800", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg6 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg6.has_value());
	EXPECT_EQ(vg6->append("0800V900", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg7 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg7.has_value());
	EXPECT_EQ(vg7->append("M0800V5000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg8 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg8.has_value());
	EXPECT_EQ(vg8->append("0800VP5000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
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

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("////V////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("M////V5000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, invalidVarVisGroupWrongVariableRangeMiles) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("1/2V7/16", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_FALSE(vg->isValid());
}

TEST(VisibilityGroup, invalidVarVisGroupWrongVariableRangeMeters) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("1500V1400", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_FALSE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Prevailing non-variable visibility specified in remarks
// Purpose: to confirm that VIS group which do not specify variable, 
// directional or runway visibility are not parsed.
// Note: this value must be specified in report body or trend instead, 
// (e.g. 1/2SM or 5SM or 1 1/2SM or 1500). When the similar VIS groups 
// (e.g. VIS 1/2 or VIS 5 or VIS 1 1/2 or VIS 1500) are specified in remarks,
// they are not recognised by parser.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRmkVisIncorrectNotVariable) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkVisIncorrectNonVariableMeters) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("1500", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

///////////////////////////////////////////////////////////////////////////////
// Directional variable visibility specified in statute miles and meters
// Purpose: to confirm that variable directional visibility groups specified in 
// remarks (e.g. VIS NW 1V3, VIS N 1500V3000, VIS N 1/2V1, VIS N 1/4V1/2, 
// VIS N 1 1/4V1 1/2) are parsed and appended correctly.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRmkVisDirectionalIntegerVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_FALSE(vg->runway().has_value());

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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalFractionVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/2V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_FALSE(vg->runway().has_value());

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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalFractionVariableFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/8V3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_FALSE(vg->runway().has_value());

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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalIntegerFractionVariableInteger) {
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

	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalFractionVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/2V2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 2.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 2.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalIntegerFractionVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4V2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 2.0 + 1.0 / 2.0, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_FALSE(vg->runway().has_value());

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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalModifierFractionVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("M1/4V5", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::NW);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 5, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalFractionVariableModifierInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4VP6", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::NW);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalModifierFractionVariableModifierInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("M1/4VP6", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::NW);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalModifierFractionVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("M1/4V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::NW);

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalModifierIntegerFractionVariableModifierIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("M1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4VP1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL);
	EXPECT_FALSE(vg->visibility().isReported());
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::NW);

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}


///////////////////////////////////////////////////////////////////////////////
// Directional non-variable visibility included in remarks
// Purpose: to confirm that non-variable directional visibility groups 
// specified in remarks (e.g. VIS W 1 3/4, VIS W 1/16, VIS W 2) are parsed and 
// appended correctly.
///////////////////////////////////////////////////////////////////////////////

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

	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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

	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 2, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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

	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 3.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkAppendToCompleteNonVarVisGroups) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg1->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg1->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg1->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("S", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg2->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg2->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg2->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(VisibilityGroup, parseRmkVisDirectionalNonVariableModifierIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("M1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalNonVariableModifierInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("P6", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisDirectionalNonVariableModifierFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("M1/16", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	ASSERT_TRUE(vg->direction().has_value());
	EXPECT_EQ(vg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction()->cardinal(), metaf::Direction::Cardinal::W);

	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 / 16.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Malformed directional visibility groups in statute miles
// Purpose: to confirm that malformed variable visibility groups are not 
// parsed or appended, and that variable directional visibility groups where
// minimum and maximum values are not consistent are correctly identified
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRmkVisDirectionalVariableAppendRunway) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("SE", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("1V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("R18", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("SE", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("1/2V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("RWY18", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}

TEST(VisibilityGroup, parseRmkVisDirectionalNonVariableAppendRunway) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("SE", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("1/2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("R18", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("SE", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("RWY18", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
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

TEST(VisibilityGroup, invalidDirVarVisGroupWrongVariableRangeMiles) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/2V7/16", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_FALSE(vg->isValid());
}

TEST(VisibilityGroup, invalidDirVarVisGroupWrongVariableRangeMeters) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1500V1400", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_FALSE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Non-variable directional visibility groups in meters
// Purpose: to confirm that non-variable directional visibility groups where  
// the value is specified in meters are not parsed or appended.
// Note: such groups (e.g. VIS W 1500) must be specified in METAR or TAF report
// body or trend instead (e.g 1500W). Non-variable directional VIS groups where
// visibility value is specified in meters are not parsed in remarks.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRmkVisDirectionalNonVariableMeters) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1500", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

///////////////////////////////////////////////////////////////////////////////
// Variable runway visibility specified in statute miles and meters
// Purpose: to confirm that variable runway visibility groups specified in 
// remarks (e.g. VIS 1V3 RWY27, VIS 1500V3000 RWY27, VIS 1/2V1 RWY27, 
// VIS 1/4V1/2 RWY27, VIS 1 1/4V1 1/2 RWY27) are parsed and appended correctly.
// Note: malformed variable VIS groups were already tested above.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRmkVisRunwayIntegerVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayFractionVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/2V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayFractionVariableFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/8V3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayIntegerFractionVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4V3", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayFractionVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/2V2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 2.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 2.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayIntegerFractionVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4V2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/2", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 2.0 + 1.0 / 2.0, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayVariableMeters) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1200V9999", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

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
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayModifierFractionVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("M1/4V5", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 5, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayFractionVariableModifierInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/4VP6", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayModifierFractionVariableModifierInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("M1/4VP6", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayModifierFractionVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("M1/4V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayModifierIntegerFractionVariableModifierIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("M1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4VP1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RUNWAY);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 1.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);

	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Non-variable runway visibility specified in statute miles and meters
// Purpose: to confirm that non-variable runway visibility groups specified in 
// remarks (e.g. VIS 1 3/4 RWY26, VIS 1/16 RWY26, VIS 3 RWY26, VIS 0660 RWY26) 
// are parsed and appended correctly.
// Note: malformed non-variable VIS groups were already tested above.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRmkVisRunwaylNonVariableIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RUNWAY);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayNonVariableInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RUNWAY);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 2, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayNonVariableFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RUNWAY);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 3.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayVisAppendOther) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("1/2", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("RWY19", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("RWY22", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(VisibilityGroup, parseRmkVisRunwayNonVariableMeters) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("1500", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RUNWAY);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1500, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayNonVariableMeters9999) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("9999", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RUNWAY);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 10000, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayNonVariableModifierIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("M1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RUNWAY);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayNonVariableModifierInteger) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("P6", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RUNWAY);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayNonVariableModifierFraction) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("M1/16", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RUNWAY);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 / 16.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Test various runway formats specified in runway visibility groups
// Purpose: to confirm that all applicable runway specification formats (Rxx, 
// RxxL, RxxR, RxxC, RWYxx, RWYxxL, RWYxxR, RWYxxC) are parsed correctly as a
// part of runway visibility groups.
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRmkVisRunwayRWithoutDesignator) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RUNWAY);

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayRWithDesignator) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("R22R", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::RUNWAY);
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 22u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_TRUE(vg1->isValid());

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("R22C", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::RUNWAY);
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 22u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::CENTER);
	EXPECT_TRUE(vg2->isValid());
	
	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("R22L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->type(), metaf::VisibilityGroup::Type::RUNWAY);
	ASSERT_TRUE(vg3->runway().has_value());
	EXPECT_EQ(vg3->runway()->number(), 22u);
	EXPECT_EQ(vg3->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_TRUE(vg3->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayRwyWithoutDesignator) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("RWY22", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RUNWAY);

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 22u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkVisRunwayRwyWithDesignator) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("RWY22R", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::RUNWAY);
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 22u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_TRUE(vg1->isValid());

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("RWY22C", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::RUNWAY);
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 22u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::CENTER);
	EXPECT_TRUE(vg2->isValid());
	
	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("3/4", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("RWY22L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->type(), metaf::VisibilityGroup::Type::RUNWAY);
	ASSERT_TRUE(vg3->runway().has_value());
	EXPECT_EQ(vg3->runway()->number(), 22u);
	EXPECT_EQ(vg3->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_TRUE(vg3->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Malformed runway visibility groups
// Purpose: to confirm that variable runway visibility groups where minimum
// and maximum values are not consistent, and runway visibility group where
// invalid runway is specified are identified correctly
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, invalidRunwayVarVisGroupWrongVariableRangeMiles) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("1/2V7/16", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(vg->isValid());
}

TEST(VisibilityGroup, invalidRunwayVarVisGroupWrongVariableRangeMeters) {
	auto vg = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("1500V1400", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("R22", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(vg->isValid());
}

TEST(VisibilityGroup, invalidRunwayVisibilityGroupWrongRunway) {
	auto vg1 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("R88R", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_FALSE(vg1->runway()->isValid());
	EXPECT_FALSE(vg1->isValid());

	auto vg2 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("R99C", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	ASSERT_TRUE(vg2->runway().has_value());
	ASSERT_FALSE(vg2->runway()->isValid());
	EXPECT_FALSE(vg2->isValid());

	auto vg3 = metaf::VisibilityGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("R37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	ASSERT_TRUE(vg3->runway().has_value());
	ASSERT_FALSE(vg3->runway()->isValid());
	EXPECT_FALSE(vg3->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Surface visibility in statute miles and meters
// Purpose: to confirm that surface visibility groups specified in remarks 
// (e.g. SFC VIS 1 3/4, SFC VIS 1/8, SFC VIS 4, SFC VIS 4000) are parsed and 
// appended correctly.
///////////////////////////////////////////////////////////////////////////////

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

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 3.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 4, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 / 8.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkSfcVisMeters) {
	auto vg = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1500", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1500, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkSfcMeters9999) {
	auto vg = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("9999", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 10000, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkAppendToCompleteSfcVis) {
	auto vg = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("RWY22", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(VisibilityGroup, parseRmkSfcVisModifierIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("M1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkSfcVisModifierInteger) {
	auto vg = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("P6", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkSfcVisModifierFraction) {
	auto vg = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("M1/16", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::SURFACE);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 / 16.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Malformed surface visibility groups
// Purpose: to confirm that malformed surface visibility groups are not parsed 
// or appended.
///////////////////////////////////////////////////////////////////////////////

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
	EXPECT_EQ(vg3->append("////SM", metaf::ReportPart::RMK), 
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

	auto vg4 = metaf::VisibilityGroup::parse("SFC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("1/2V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

///////////////////////////////////////////////////////////////////////////////
// Tower visibility in statute miles and meters
// Purpose: to confirm that tower visibility groups specified in remarks 
// (e.g. TWR VIS 1 3/4, TWR VIS 1/8, TWR VIS 4, TWR VIS 4000) are parsed and 
// appended correctly.
///////////////////////////////////////////////////////////////////////////////

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

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 3.0/4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 4, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
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

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER);
	EXPECT_FALSE(vg->direction().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 / 8.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkTwrVisMeters) {
	auto vg = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1500", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1500, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkTwrMeters9999) {
	auto vg = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("9999", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 10000, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkTwrVisModifierIntegerFraction) {
	auto vg = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("M1", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("1/4", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 + 1.0 / 4.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkTwrVisModifierInteger) {
	auto vg = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("P6", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 6, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRmkTwrVisModifierFraction) {
	auto vg = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("M1/16", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::TOWER);
	EXPECT_FALSE(vg->direction().has_value());
	EXPECT_FALSE(vg->runway().has_value());

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg->visibility().isReported());
	EXPECT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1.0 / 16.0, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Malformed tower visibility groups
// Purpose: to confirm that malformed tower visibility groups are not parsed 
// or appended.
///////////////////////////////////////////////////////////////////////////////

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
	EXPECT_EQ(vg3->append("////SM", metaf::ReportPart::RMK), 
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

	auto vg4 = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("1", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("1/2V1", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, parseRmkAppendToCompleteTwrVis) {
	auto vg = metaf::VisibilityGroup::parse("TWR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->append("VIS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("3", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("1V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("1/2V3", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("2", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("RWY22", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// Non-variable runway visual range in meters and feet
// Purpose: to confirm that non-variable runway visual range groups (including 
// values with modifiers and groups with trends) are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRvrMeters) {
	const auto vg = metaf::VisibilityGroup::parse("R05/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 5u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1100, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRvrMetersWithModifiers) {
	const auto vg1 = metaf::VisibilityGroup::parse("R04/P2000", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 4u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg1->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg1->visibility().isReported());
	ASSERT_TRUE(vg1->visibility().distance().has_value());
	EXPECT_NEAR(vg1->visibility().distance().value(), 2000, margin);
	EXPECT_EQ(vg1->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R21/M0200", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 21u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg2->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg2->visibility().isReported());
	ASSERT_TRUE(vg2->visibility().distance().has_value());
	EXPECT_NEAR(vg2->visibility().distance().value(), 200, margin);
	EXPECT_EQ(vg2->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg2->minVisibility().isReported());
	EXPECT_FALSE(vg2->maxVisibility().isReported());
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg2->isValid());
}

TEST(VisibilityGroup, parseRvrFeet) {
	const auto vg = metaf::VisibilityGroup::parse("R11/4500FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 11u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 4500, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::FEET);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRvrFeetWithModifiers) {
	const auto vg1 = metaf::VisibilityGroup::parse("R11/P6000FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 11u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg1->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg1->visibility().isReported());
	ASSERT_TRUE(vg1->visibility().distance().has_value());
	EXPECT_NEAR(vg1->visibility().distance().value(), 6000, margin);
	EXPECT_EQ(vg1->visibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R12/M0400FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 12u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg2->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg2->visibility().isReported());
	ASSERT_TRUE(vg2->visibility().distance().has_value());
	EXPECT_NEAR(vg2->visibility().distance().value(), 400, margin);
	EXPECT_EQ(vg2->visibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(vg2->minVisibility().isReported());
	EXPECT_FALSE(vg2->maxVisibility().isReported());
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg2->isValid());
}

TEST(VisibilityGroup, parseRvrWithTrends) {
	const auto vg1 = metaf::VisibilityGroup::parse("R05/1100U", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 5u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg1->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg1->visibility().isReported());
	ASSERT_TRUE(vg1->visibility().distance().has_value());
	EXPECT_NEAR(vg1->visibility().distance().value(), 1100, margin);
	EXPECT_EQ(vg1->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::UPWARD);
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R05/1000FT/D", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 5u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg2->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg2->visibility().isReported());
	ASSERT_TRUE(vg2->visibility().distance().has_value());
	EXPECT_NEAR(vg2->visibility().distance().value(), 1000, margin);
	EXPECT_EQ(vg2->visibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(vg2->minVisibility().isReported());
	EXPECT_FALSE(vg2->maxVisibility().isReported());
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::DOWNWARD);
	EXPECT_TRUE(vg2->isValid());

	const auto vg3 = metaf::VisibilityGroup::parse("R05/M1100FT/N", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg3->direction().has_value());
	ASSERT_TRUE(vg3->runway().has_value());
	EXPECT_EQ(vg3->runway()->number(), 5u);
	EXPECT_EQ(vg3->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg3->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg3->visibility().isReported());
	ASSERT_TRUE(vg3->visibility().distance().has_value());
	EXPECT_NEAR(vg3->visibility().distance().value(), 1100, margin);
	EXPECT_EQ(vg3->visibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(vg3->minVisibility().isReported());
	EXPECT_FALSE(vg3->maxVisibility().isReported());
	EXPECT_EQ(vg3->sectorDirections().size(), 0u);
	EXPECT_EQ(vg3->trend(), metaf::VisibilityGroup::Trend::NEUTRAL);
	EXPECT_TRUE(vg3->isValid());

	const auto vg4 = metaf::VisibilityGroup::parse("R05/P1000/", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg4->direction().has_value());
	ASSERT_TRUE(vg4->runway().has_value());
	EXPECT_EQ(vg4->runway()->number(), 5u);
	EXPECT_EQ(vg4->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg4->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg4->visibility().isReported());
	ASSERT_TRUE(vg4->visibility().distance().has_value());
	EXPECT_NEAR(vg4->visibility().distance().value(), 1000, margin);
	EXPECT_EQ(vg4->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg4->minVisibility().isReported());
	EXPECT_FALSE(vg4->maxVisibility().isReported());
	EXPECT_EQ(vg4->sectorDirections().size(), 0u);
	EXPECT_EQ(vg4->trend(), metaf::VisibilityGroup::Trend::NOT_REPORTED);
	EXPECT_TRUE(vg4->isValid());
}

TEST(VisibilityGroup, parseRvrNotReported) {
	const auto vg1 = metaf::VisibilityGroup::parse("R06R/////", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 6u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_FALSE(vg1->visibility().isReported());
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R06R/////FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 6u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_FALSE(vg2->visibility().isReported());
	EXPECT_FALSE(vg2->minVisibility().isReported());
	EXPECT_FALSE(vg2->maxVisibility().isReported());
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg2->isValid());
}

TEST(VisibilityGroup, parseRvrNotReportedWithTrend) {
	const auto vg1 = metaf::VisibilityGroup::parse("R06R/////N", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 6u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_FALSE(vg1->visibility().isReported());
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NEUTRAL);
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R06R/////FT/N", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 6u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_FALSE(vg2->visibility().isReported());
	EXPECT_FALSE(vg2->minVisibility().isReported());
	EXPECT_FALSE(vg2->maxVisibility().isReported());
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::NEUTRAL);
	EXPECT_TRUE(vg2->isValid());
}

TEST(VisibilityGroup, parseRvrNotReportedWithTrendNotReported) {
	const auto vg1 = metaf::VisibilityGroup::parse("R06R//////", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 6u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_FALSE(vg1->visibility().isReported());
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NOT_REPORTED);
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R06R/////FT//", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 6u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_FALSE(vg2->visibility().isReported());
	EXPECT_FALSE(vg2->minVisibility().isReported());
	EXPECT_FALSE(vg2->maxVisibility().isReported());
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::NOT_REPORTED);
	EXPECT_TRUE(vg2->isValid());
}

TEST(VisibilityGroup, parseRvrFeetExtraTrendDelimiter) {
	const auto vg = metaf::VisibilityGroup::parse("R08/6000FT/", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 8u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 6000, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::FEET);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseRvrFeetMissingTrendDelimiter) {
	const auto vg = metaf::VisibilityGroup::parse("R08/6000FTD", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 8u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 6000, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::FEET);

	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::DOWNWARD);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Variable runway visual range in meters and feet
// Purpose: to confirm that variable runway visual range groups (including 
// values with modifiers and groups with trends) are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseVariableRvrMeters) {
	const auto vg = metaf::VisibilityGroup::parse("R09/0500V0800", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 9u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 500, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 800, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::METERS);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseVariableRvrMetersWithModifiers) {
	const auto vg1 = metaf::VisibilityGroup::parse("R09/M0500V0800", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg1->visibility().isReported());
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 9u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg1->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg1->minVisibility().isReported());
	ASSERT_TRUE(vg1->minVisibility().distance().has_value());
	EXPECT_NEAR(vg1->minVisibility().distance().value(), 500, margin);
	EXPECT_EQ(vg1->minVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg1->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg1->maxVisibility().isReported());
	ASSERT_TRUE(vg1->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg1->maxVisibility().distance().value(), 800, margin);
	EXPECT_EQ(vg1->maxVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R09/0500VP0800", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg2->visibility().isReported());
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 9u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg2->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg2->minVisibility().isReported());
	ASSERT_TRUE(vg2->minVisibility().distance().has_value());
	EXPECT_NEAR(vg2->minVisibility().distance().value(), 500, margin);
	EXPECT_EQ(vg2->minVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg2->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg2->maxVisibility().isReported());
	ASSERT_TRUE(vg2->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg2->maxVisibility().distance().value(), 800, margin);
	EXPECT_EQ(vg2->maxVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg2->isValid());

	const auto vg3 = metaf::VisibilityGroup::parse("R09/M0500VP0800", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg3->visibility().isReported());
	EXPECT_FALSE(vg3->direction().has_value());
	ASSERT_TRUE(vg3->runway().has_value());
	EXPECT_EQ(vg3->runway()->number(), 9u);
	EXPECT_EQ(vg3->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg3->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg3->minVisibility().isReported());
	ASSERT_TRUE(vg3->minVisibility().distance().has_value());
	EXPECT_NEAR(vg3->minVisibility().distance().value(), 500, margin);
	EXPECT_EQ(vg3->minVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg3->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg3->maxVisibility().isReported());
	ASSERT_TRUE(vg3->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg3->maxVisibility().distance().value(), 800, margin);
	EXPECT_EQ(vg3->maxVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg3->sectorDirections().size(), 0u);
	EXPECT_EQ(vg3->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg3->isValid());}

TEST(VisibilityGroup, parseVariableRvrFeet) {
	const auto vg = metaf::VisibilityGroup::parse("R14/0600V1200FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());

	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 14u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 600, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::FEET);

	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 1200, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::FEET);

	EXPECT_EQ(vg->sectorDirections().size(), 0u);

	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, parseVariableRvrFeetWithModifiers) {
	const auto vg1 = metaf::VisibilityGroup::parse("R14/M0600V1200FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg1->visibility().isReported());
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 14u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg1->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg1->minVisibility().isReported());
	ASSERT_TRUE(vg1->minVisibility().distance().has_value());
	EXPECT_NEAR(vg1->minVisibility().distance().value(), 600, margin);
	EXPECT_EQ(vg1->minVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(vg1->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg1->maxVisibility().isReported());
	ASSERT_TRUE(vg1->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg1->maxVisibility().distance().value(), 1200, margin);
	EXPECT_EQ(vg1->maxVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R14/0600VP1200FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg2->visibility().isReported());
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 14u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg2->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg2->minVisibility().isReported());
	ASSERT_TRUE(vg2->minVisibility().distance().has_value());
	EXPECT_NEAR(vg2->minVisibility().distance().value(), 600, margin);
	EXPECT_EQ(vg2->minVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(vg2->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg2->maxVisibility().isReported());
	ASSERT_TRUE(vg2->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg2->maxVisibility().distance().value(), 1200, margin);
	EXPECT_EQ(vg2->maxVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg2->isValid());

	const auto vg3 = metaf::VisibilityGroup::parse("R14/M0600VP1200FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg3->visibility().isReported());
	EXPECT_FALSE(vg3->direction().has_value());
	ASSERT_TRUE(vg3->runway().has_value());
	EXPECT_EQ(vg3->runway()->number(), 14u);
	EXPECT_EQ(vg3->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg3->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg3->minVisibility().isReported());
	ASSERT_TRUE(vg3->minVisibility().distance().has_value());
	EXPECT_NEAR(vg3->minVisibility().distance().value(), 600, margin);
	EXPECT_EQ(vg3->minVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(vg3->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg3->maxVisibility().isReported());
	ASSERT_TRUE(vg3->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg3->maxVisibility().distance().value(), 1200, margin);
	EXPECT_EQ(vg3->maxVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(vg3->sectorDirections().size(), 0u);
	EXPECT_EQ(vg3->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg3->isValid());
}

TEST(VisibilityGroup, parseVariableRvrWithTrends) {
	const auto vg1 = metaf::VisibilityGroup::parse("R17/0250V0450N", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg1->visibility().isReported());
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 17u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg1->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg1->minVisibility().isReported());
	ASSERT_TRUE(vg1->minVisibility().distance().has_value());
	EXPECT_NEAR(vg1->minVisibility().distance().value(), 250, margin);
	EXPECT_EQ(vg1->minVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg1->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg1->maxVisibility().isReported());
	ASSERT_TRUE(vg1->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg1->maxVisibility().distance().value(), 450, margin);
	EXPECT_EQ(vg1->maxVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NEUTRAL);
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R17/M0250VP0450D", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg2->visibility().isReported());
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 17u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg2->minVisibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	EXPECT_TRUE(vg2->minVisibility().isReported());
	ASSERT_TRUE(vg2->minVisibility().distance().has_value());
	EXPECT_NEAR(vg2->minVisibility().distance().value(), 250, margin);
	EXPECT_EQ(vg2->minVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg2->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg2->maxVisibility().isReported());
	ASSERT_TRUE(vg2->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg1->maxVisibility().distance().value(), 450, margin);
	EXPECT_EQ(vg2->maxVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::DOWNWARD);
	EXPECT_TRUE(vg2->isValid());

	const auto vg3 = metaf::VisibilityGroup::parse("R26/2600V3200FT//", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg3->visibility().isReported());
	EXPECT_FALSE(vg3->direction().has_value());
	ASSERT_TRUE(vg3->runway().has_value());
	EXPECT_EQ(vg3->runway()->number(), 26u);
	EXPECT_EQ(vg3->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg3->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg3->minVisibility().isReported());
	ASSERT_TRUE(vg3->minVisibility().distance().has_value());
	EXPECT_NEAR(vg3->minVisibility().distance().value(), 2600, margin);
	EXPECT_EQ(vg3->minVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(vg3->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg3->maxVisibility().isReported());
	ASSERT_TRUE(vg3->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg3->maxVisibility().distance().value(), 3200, margin);
	EXPECT_EQ(vg3->maxVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(vg3->sectorDirections().size(), 0u);
	EXPECT_EQ(vg3->trend(), metaf::VisibilityGroup::Trend::NOT_REPORTED);
	EXPECT_TRUE(vg3->isValid());

	const auto vg4 = metaf::VisibilityGroup::parse("R30/5000VP6000FT/U", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg4->visibility().isReported());
	EXPECT_FALSE(vg4->direction().has_value());
	ASSERT_TRUE(vg4->runway().has_value());
	EXPECT_EQ(vg4->runway()->number(), 30u);
	EXPECT_EQ(vg4->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg4->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg4->minVisibility().isReported());
	ASSERT_TRUE(vg4->minVisibility().distance().has_value());
	EXPECT_NEAR(vg4->minVisibility().distance().value(), 5000, margin);
	EXPECT_EQ(vg4->minVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(vg4->maxVisibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_TRUE(vg4->maxVisibility().isReported());
	ASSERT_TRUE(vg4->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg4->maxVisibility().distance().value(), 6000, margin);
	EXPECT_EQ(vg4->maxVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(vg4->sectorDirections().size(), 0u);
	EXPECT_EQ(vg4->trend(), metaf::VisibilityGroup::Trend::UPWARD);
	EXPECT_TRUE(vg4->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Miscellaneous runway visual range tests
// Purpose: to confirm that runway visual range groups where runway has a 
// designator are parsed correctly, that malformed runway visual range 
// groups are not parsed, and that no other group can be appended to a 
// runway visual range group
///////////////////////////////////////////////////////////////////////////////

TEST(VisibilityGroup, parseRvrRunwayWithDesignator) {
	const auto vg1 = metaf::VisibilityGroup::parse("R05L/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg1->direction().has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	EXPECT_EQ(vg1->runway()->number(), 5u);
	EXPECT_EQ(vg1->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_EQ(vg1->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg1->visibility().isReported());
	ASSERT_TRUE(vg1->visibility().distance().has_value());
	EXPECT_NEAR(vg1->visibility().distance().value(), 1100, margin);
	EXPECT_EQ(vg1->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorDirections().size(), 0u);
	EXPECT_EQ(vg1->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R05R/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg2->direction().has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	EXPECT_EQ(vg2->runway()->number(), 5u);
	EXPECT_EQ(vg2->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(vg2->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg2->visibility().isReported());
	ASSERT_TRUE(vg2->visibility().distance().has_value());
	EXPECT_NEAR(vg2->visibility().distance().value(), 1100, margin);
	EXPECT_EQ(vg2->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg2->minVisibility().isReported());
	EXPECT_FALSE(vg2->maxVisibility().isReported());
	EXPECT_EQ(vg2->sectorDirections().size(), 0u);
	EXPECT_EQ(vg2->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg2->isValid());

	const auto vg3 = metaf::VisibilityGroup::parse("R05C/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg3->direction().has_value());
	ASSERT_TRUE(vg3->runway().has_value());
	EXPECT_EQ(vg3->runway()->number(), 5u);
	EXPECT_EQ(vg3->runway()->designator(), metaf::Runway::Designator::CENTER);
	EXPECT_EQ(vg3->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg3->visibility().isReported());
	ASSERT_TRUE(vg3->visibility().distance().has_value());
	EXPECT_NEAR(vg3->visibility().distance().value(), 1100, margin);
	EXPECT_EQ(vg3->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg3->minVisibility().isReported());
	EXPECT_FALSE(vg3->maxVisibility().isReported());
	EXPECT_EQ(vg3->sectorDirections().size(), 0u);
	EXPECT_EQ(vg3->trend(), metaf::VisibilityGroup::Trend::NONE);
	EXPECT_TRUE(vg3->isValid());
}

TEST(VisibilityGroup, parseRvrGroupWrongReportPart) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100", metaf::ReportPart::RMK));
}

TEST(VisibilityGroup, parseRvrGroupWrongFormat) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R5/1100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R005/1100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05X/1100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05R1100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/110", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/01100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/110A", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/Z1100", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100VZ2000", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100Z2000", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100V2000FF", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100V2000A", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100V2000/N", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100V2000FT/A", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100V////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/0400V800", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R05/1100V02000", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R06R////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::VisibilityGroup::parse("R06R///////", metaf::ReportPart::METAR));
}

TEST(VisibilityGroup, appendToRvrGroup) {
	auto vg = metaf::VisibilityGroup::parse("R05L/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("R16/1200", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("R30/5000VP6000FT/U", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
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

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::RVR);
	EXPECT_FALSE(vg->direction().has_value());
	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 5u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->visibility().isReported());
	ASSERT_TRUE(vg->visibility().distance().has_value());
	EXPECT_NEAR(vg->visibility().distance().value(), 1100, margin);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
}

TEST(VisibilityGroup, appendToVariableRvrGroup) {
	auto vg = metaf::VisibilityGroup::parse("R09/0500V0800", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("R16/1200", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(vg->append("R30/5000VP6000FT/U", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
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

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::VARIABLE_RVR);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_FALSE(vg->direction().has_value());
	ASSERT_TRUE(vg->runway().has_value());
	EXPECT_EQ(vg->runway()->number(), 9u);
	EXPECT_EQ(vg->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(vg->minVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->minVisibility().isReported());
	ASSERT_TRUE(vg->minVisibility().distance().has_value());
	EXPECT_NEAR(vg->minVisibility().distance().value(), 500, margin);
	EXPECT_EQ(vg->minVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->maxVisibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(vg->maxVisibility().isReported());
	ASSERT_TRUE(vg->maxVisibility().distance().has_value());
	EXPECT_NEAR(vg->maxVisibility().distance().value(), 800, margin);
	EXPECT_EQ(vg->maxVisibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->sectorDirections().size(), 0u);
	EXPECT_EQ(vg->trend(), metaf::VisibilityGroup::Trend::NONE);
}

TEST(VisibilityGroup, invalidRvrGroupWrongRunway) {
	const auto vg1 = metaf::VisibilityGroup::parse("R88R/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	ASSERT_TRUE(vg1->runway().has_value());
	ASSERT_FALSE(vg1->runway()->isValid());
	EXPECT_FALSE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("R99C/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	ASSERT_TRUE(vg2->runway().has_value());
	ASSERT_FALSE(vg2->runway()->isValid());
	EXPECT_FALSE(vg2->isValid());

	const auto vg3 = metaf::VisibilityGroup::parse("R37/1100", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg3.has_value());
	ASSERT_TRUE(vg3->runway().has_value());
	ASSERT_FALSE(vg3->runway()->isValid());
	EXPECT_FALSE(vg3->isValid());
}

TEST(VisibilityGroup, invalidRvrGroupWrongVariableRangeMeters) {
	const auto vg = metaf::VisibilityGroup::parse("R09/0500V0400", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_FALSE(vg->isValid());
}

TEST(VisibilityGroup, invalidRvrGroupWrongVariableRangeFeet) {
	const auto vg = metaf::VisibilityGroup::parse("R22/0800V0700FT", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_FALSE(vg->isValid());
}
