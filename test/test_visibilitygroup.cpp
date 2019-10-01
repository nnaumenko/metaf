/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

TEST(VisibilityGroup, parseMetersMetar) {
	const auto vg = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 1600u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersTaf) {
	const auto vg = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::TAF);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 1600u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersNDV) {
	const auto vg = metaf::VisibilityGroup::parse("9999NDV", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING_NDV);
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 10000u);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::NDV);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersDirectionNorth) {
	const auto vg = metaf::VisibilityGroup::parse("9999N", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersDirectionSouth) {
	const auto vg = metaf::VisibilityGroup::parse("9999S", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::S);
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersDirectionWest) {
	const auto vg = metaf::VisibilityGroup::parse("9999W", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::W);
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersDirectionEast) {
	const auto vg = metaf::VisibilityGroup::parse("9999E", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersDirectionNorthWest) {
	const auto vg = metaf::VisibilityGroup::parse("9999NW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::NW);
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersDirectionNorthEast) {
	const auto vg = metaf::VisibilityGroup::parse("9999NE", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersDirectionSouthWest) {
	const auto vg = metaf::VisibilityGroup::parse("9999SW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::SW);
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersDirectionSouthEast) {
	const auto vg = metaf::VisibilityGroup::parse("9999SE", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::DIRECTIONAL);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersNotReported) {
	const auto vg = metaf::VisibilityGroup::parse("////", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(!vg->visibility().isReported());
	EXPECT_FALSE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
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

TEST(VisibilityGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("1600", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("1600", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("1600", metaf::ReportPart::RMK).has_value());
}

TEST(VisibilityGroup, parseMilesInteger) {
	const auto vg = metaf::VisibilityGroup::parse("3SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 3u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMilesIntegerTwoDigit) {
	const auto vg = metaf::VisibilityGroup::parse("15SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 15u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMilesIntegerWithModifier) {
	const auto vg = metaf::VisibilityGroup::parse("P6SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 6u);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMilesFraction) {
	const auto vg = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isFraction());
	ASSERT_TRUE(vg->visibility().numerator().has_value());
	EXPECT_EQ(vg->visibility().numerator().value(), 1u);
	ASSERT_TRUE(vg->visibility().denominator().has_value());
	EXPECT_EQ(vg->visibility().denominator().value(), 4u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMilesFractionWithModifier) {
	const auto vg = metaf::VisibilityGroup::parse("M1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isFraction());
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	ASSERT_TRUE(vg->visibility().numerator().has_value());
	EXPECT_EQ(vg->visibility().numerator().value(), 1u);
	ASSERT_TRUE(vg->visibility().denominator().has_value());
	EXPECT_EQ(vg->visibility().denominator().value(), 4u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMilesIntegerNotReported) {
	const auto vg = metaf::VisibilityGroup::parse("////SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
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
}

TEST(VisibilityGroup, parseMilesIncomplete) {
	const auto vg1 = metaf::VisibilityGroup::parse("1", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg1->visibility().isInteger());
	ASSERT_TRUE(vg1->visibility().integer().has_value());
	EXPECT_EQ(vg1->visibility().integer().value(), 1u);
	EXPECT_EQ(vg1->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg1->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg1->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg1->minVisibility().isReported());
	EXPECT_FALSE(vg1->maxVisibility().isReported());
	EXPECT_EQ(vg1->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg1->sectorEnd().status(), metaf::Direction::Status::OMMITTED);

	const auto vg9 = metaf::VisibilityGroup::parse("9", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg9.has_value());
	EXPECT_EQ(vg9->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg9->visibility().isInteger());
	ASSERT_TRUE(vg9->visibility().integer().has_value());
	EXPECT_EQ(vg9->visibility().integer().value(), 9u);
	EXPECT_EQ(vg9->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg9->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg9->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg9->minVisibility().isReported());
	EXPECT_FALSE(vg9->maxVisibility().isReported());
	EXPECT_EQ(vg9->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg9->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
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
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 2u);
	ASSERT_TRUE(vg->visibility().numerator().has_value());
	EXPECT_EQ(vg->visibility().numerator().value(), 1u);
	ASSERT_TRUE(vg->visibility().denominator().has_value());
	EXPECT_EQ(vg->visibility().denominator().value(), 4u);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
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

	EXPECT_EQ(vg->type(), metaf::VisibilityGroup::Type::PREVAILING);
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 2u);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
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
	EXPECT_TRUE(vg->visibility().isFraction());
	ASSERT_TRUE(vg->visibility().numerator().has_value());
	EXPECT_EQ(vg->visibility().numerator().value(), 1u);
	ASSERT_TRUE(vg->visibility().denominator().has_value());
	EXPECT_EQ(vg->visibility().denominator().value(), 4u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
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
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 1200u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
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
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 1u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
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
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 2u);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(vg->minVisibility().isReported());
	EXPECT_FALSE(vg->maxVisibility().isReported());
	EXPECT_EQ(vg->sectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(vg->sectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, appendInvalidFraction) {
	auto vg = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/0SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VisibilityGroup, appendToAppended) {
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
}

TEST(VisibilityGroup, isValidTrueSingle) {
	const auto vg1 = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_TRUE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("9999", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_TRUE(vg2->isValid());

	const auto vg3 = metaf::VisibilityGroup::parse("4000NW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_TRUE(vg3->isValid());

	const auto vg4 = metaf::VisibilityGroup::parse("9999NDV", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_TRUE(vg4->isValid());

	const auto vg5 = metaf::VisibilityGroup::parse("1SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_TRUE(vg5->isValid());

	const auto vg6 = metaf::VisibilityGroup::parse("1/2SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg6.has_value());
	EXPECT_TRUE(vg6->isValid());

	const auto vg7 = metaf::VisibilityGroup::parse("11/2SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg7.has_value());
	EXPECT_TRUE(vg7->isValid());

	const auto vg8 = metaf::VisibilityGroup::parse("////SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg8.has_value());
	EXPECT_TRUE(vg8->isValid());
}

TEST(VisibilityGroup, isValidTrueAppended) {
	auto vg = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/4SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(vg->isValid());
}

TEST(VisibilityGroup, isValidFalse) {
	const auto vg1 = metaf::VisibilityGroup::parse("1/0SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_FALSE(vg1->isValid());

	const auto vg2 = metaf::VisibilityGroup::parse("M1/0SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_FALSE(vg2->isValid());

	const auto vg3 = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_FALSE(vg3->isValid());
}

TEST(VisibilityGroup, isPrevailingTrue) {
	const auto vg1 = metaf::VisibilityGroup::parse("1/2SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_TRUE(vg1->isPrevailing());

	const auto vg2 = metaf::VisibilityGroup::parse("M1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_TRUE(vg2->isPrevailing());

	const auto vg3 = metaf::VisibilityGroup::parse("4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_TRUE(vg3->isPrevailing());

	const auto vg4 = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_TRUE(vg4->isPrevailing());

	const auto vg5 = metaf::VisibilityGroup::parse("9999NDV", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_TRUE(vg5->isPrevailing());

	const auto vg6 = metaf::VisibilityGroup::parse("////SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg6.has_value());
	EXPECT_TRUE(vg6->isPrevailing());

	const auto vg7 = metaf::VisibilityGroup::parse("////", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg7.has_value());
	EXPECT_TRUE(vg7->isPrevailing());
}

TEST(VisibilityGroup, isPrevailingFalse) {
	const auto vg1 = metaf::VisibilityGroup::parse("9999W", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_FALSE(vg1->isPrevailing());

	const auto vg2 = metaf::VisibilityGroup::parse("9999NW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_FALSE(vg2->isPrevailing());
}

TEST(VisibilityGroup, isPrevailingAppended) {
	auto vg = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/4SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(vg->isPrevailing());
}

TEST(VisibilityGroup, isDirectionalTrue) {
	const auto vg1 = metaf::VisibilityGroup::parse("9999W", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_TRUE(vg1->isDirectional());

	const auto vg2 = metaf::VisibilityGroup::parse("9999NW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_TRUE(vg2->isDirectional());
}

TEST(VisibilityGroup, isDirectionalFalse) {
	const auto vg1 = metaf::VisibilityGroup::parse("1/2SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_FALSE(vg1->isDirectional());

	const auto vg2 = metaf::VisibilityGroup::parse("M1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_FALSE(vg2->isDirectional());

	const auto vg3 = metaf::VisibilityGroup::parse("4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_FALSE(vg3->isDirectional());

	const auto vg4 = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_FALSE(vg4->isDirectional());

	const auto vg5 = metaf::VisibilityGroup::parse("////SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_FALSE(vg5->isDirectional());

	const auto vg6 = metaf::VisibilityGroup::parse("////", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg6.has_value());
	EXPECT_FALSE(vg6->isDirectional());

	const auto vg7 = metaf::VisibilityGroup::parse("9999NDV", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg7.has_value());
	EXPECT_FALSE(vg7->isDirectional());
}

TEST(VisibilityGroup, isDirectionalAppended) {
	auto vg = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("1/4SM", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(vg->isDirectional());
}
