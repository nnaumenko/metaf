/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(VisibilityGroup, parseMetersMetar) {
	const auto vg = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 1600u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersTaf) {
	const auto vg = metaf::VisibilityGroup::parse("1600", metaf::ReportPart::TAF);
	ASSERT_TRUE(vg.has_value());
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 1600u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMetersNDV) {
	const auto vg = metaf::VisibilityGroup::parse("9999NDV", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 10000u);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::NDV);
}

TEST(VisibilityGroup, parseMetersDirectionNorth) {
	const auto vg = metaf::VisibilityGroup::parse("9999N", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::N);
}

TEST(VisibilityGroup, parseMetersDirectionSouth) {
	const auto vg = metaf::VisibilityGroup::parse("9999S", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::S);
}

TEST(VisibilityGroup, parseMetersDirectionWest) {
	const auto vg = metaf::VisibilityGroup::parse("9999W", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::W);
}

TEST(VisibilityGroup, parseMetersDirectionEast) {
	const auto vg = metaf::VisibilityGroup::parse("9999E", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::E);
}

TEST(VisibilityGroup, parseMetersDirectionNorthWest) {
	const auto vg = metaf::VisibilityGroup::parse("9999NW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::NW);
}

TEST(VisibilityGroup, parseMetersDirectionNorthEast) {
	const auto vg = metaf::VisibilityGroup::parse("9999NE", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::NE);
}

TEST(VisibilityGroup, parseMetersDirectionSouthWest) {
	const auto vg = metaf::VisibilityGroup::parse("9999SW", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::SW);
}

TEST(VisibilityGroup, parseMetersDirectionSouthEast) {
	const auto vg = metaf::VisibilityGroup::parse("9999SE", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::VALUE_CARDINAL);
	EXPECT_EQ(vg->direction().cardinal(), metaf::Direction::Cardinal::SE);
}

TEST(VisibilityGroup, parseMetersNotReported) {
	const auto vg = metaf::VisibilityGroup::parse("////", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_TRUE(!vg->visibility().isReported());
	EXPECT_FALSE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
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
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 3u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMilesIntegerTwoDigit) {
	const auto vg = metaf::VisibilityGroup::parse("15SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 15u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMilesIntegerWithModifier) {
	const auto vg = metaf::VisibilityGroup::parse("P6SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_TRUE(vg->visibility().isInteger());
	ASSERT_TRUE(vg->visibility().integer().has_value());
	EXPECT_EQ(vg->visibility().integer().value(), 6u);
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::MORE_THAN);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMilesFraction) {
	const auto vg = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_TRUE(vg->visibility().isFraction());
	ASSERT_TRUE(vg->visibility().numerator().has_value());
	EXPECT_EQ(vg->visibility().numerator().value(), 1u);
	ASSERT_TRUE(vg->visibility().denominator().has_value());
	EXPECT_EQ(vg->visibility().denominator().value(), 4u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMilesFractionWithModifier) {
	const auto vg = metaf::VisibilityGroup::parse("M1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_TRUE(vg->visibility().isFraction());
	EXPECT_EQ(vg->visibility().modifier(), metaf::Distance::Modifier::LESS_THAN);
	ASSERT_TRUE(vg->visibility().numerator().has_value());
	EXPECT_EQ(vg->visibility().numerator().value(), 1u);
	ASSERT_TRUE(vg->visibility().denominator().has_value());
	EXPECT_EQ(vg->visibility().denominator().value(), 4u);
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, parseMilesIntegerNotReported) {
	const auto vg = metaf::VisibilityGroup::parse("////SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg.has_value());
	EXPECT_FALSE(vg->visibility().isReported());
	EXPECT_EQ(vg->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg->direction().status(), metaf::Direction::Status::OMMITTED);
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
	EXPECT_TRUE(vg1->visibility().isInteger());
	ASSERT_TRUE(vg1->visibility().integer().has_value());
	EXPECT_EQ(vg1->visibility().integer().value(), 1u);
	EXPECT_EQ(vg1->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg1->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg1->direction().status(), metaf::Direction::Status::OMMITTED);

	const auto vg9 = metaf::VisibilityGroup::parse("9", metaf::ReportPart::METAR);
	ASSERT_TRUE(vg9.has_value());
	EXPECT_TRUE(vg9->visibility().isInteger());
	ASSERT_TRUE(vg9->visibility().integer().has_value());
	EXPECT_EQ(vg9->visibility().integer().value(), 9u);
	EXPECT_EQ(vg9->visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg9->visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vg9->direction().status(), metaf::Direction::Status::OMMITTED);

}

TEST(VisibilityGroup, parseMilesIncompleteWrongFormat) {
	EXPECT_FALSE(metaf::VisibilityGroup::parse("A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::VisibilityGroup::parse("/", metaf::ReportPart::METAR).has_value());
}

TEST(VisibilityGroup, combineIncompleteAndFraction) {
	const auto vgIncomplete = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgIncomplete.has_value());
	const auto vgFraction = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFraction.has_value());

	const auto combined = vgIncomplete->combine(vgFraction.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::VisibilityGroup>(combined.value()));

	const auto vgCombined = std::get<metaf::VisibilityGroup>(combined.value());
	ASSERT_TRUE(vgCombined.visibility().integer().has_value());
	EXPECT_EQ(vgCombined.visibility().integer().value(), 2u);
	ASSERT_TRUE(vgCombined.visibility().numerator().has_value());
	EXPECT_EQ(vgCombined.visibility().numerator().value(), 1u);
	ASSERT_TRUE(vgCombined.visibility().denominator().has_value());
	EXPECT_EQ(vgCombined.visibility().denominator().value(), 4u);
	EXPECT_EQ(vgCombined.visibility().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vgCombined.visibility().unit(), metaf::Distance::Unit::STATUTE_MILES);
	EXPECT_EQ(vgCombined.direction().status(), metaf::Direction::Status::OMMITTED);
}

TEST(VisibilityGroup, combineIncompleteAndOther) {
	const auto vgIncomplete = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgIncomplete.has_value());
	const auto vgMeters = metaf::VisibilityGroup::parse("1200", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgMeters.has_value());
	const auto vgMilesInteger = metaf::VisibilityGroup::parse("1SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgMilesInteger.has_value());
	const auto vgFractionWithModifier = metaf::VisibilityGroup::parse("M1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFractionWithModifier.has_value());

	EXPECT_FALSE(vgIncomplete->combine(vgIncomplete.value()).has_value());
	EXPECT_FALSE(vgIncomplete->combine(vgMeters.value()).has_value());
	EXPECT_FALSE(vgIncomplete->combine(vgMilesInteger.value()).has_value());
	EXPECT_FALSE(vgIncomplete->combine(vgFractionWithModifier.value()).has_value());
}

TEST(VisibilityGroup, combineFractionAndIncomplete) {
	const auto vgIncomplete = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgIncomplete.has_value());
	const auto vgFraction = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFraction.has_value());

	EXPECT_FALSE(vgFraction->combine(vgIncomplete.value()).has_value());
}

TEST(VisibilityGroup, combineOtherAndFraction) {
	const auto vgFraction = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFraction.has_value());
	const auto vgMeters = metaf::VisibilityGroup::parse("1200", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgMeters.has_value());
	const auto vgMilesInteger = metaf::VisibilityGroup::parse("1SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgMilesInteger.has_value());
	const auto vgMilesIntegerWithModifier = metaf::VisibilityGroup::parse("P2SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgMilesIntegerWithModifier.has_value());

	EXPECT_FALSE(vgMeters->combine(vgFraction.value()).has_value());
	EXPECT_FALSE(vgFraction->combine(vgFraction.value()).has_value());
	EXPECT_FALSE(vgMilesInteger->combine(vgFraction.value()).has_value());
	EXPECT_FALSE(vgMilesIntegerWithModifier->combine(vgFraction.value()).has_value());
}

TEST(VisibilityGroup, combineInvalidFraction) {
	const auto vgIncomplete = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgIncomplete.has_value());
	const auto vgFraction = metaf::VisibilityGroup::parse("1/0SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFraction.has_value());
	
	EXPECT_FALSE(vgIncomplete->combine(vgFraction.value()).has_value());
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

TEST(VisibilityGroup, isValidTrueCombined) {
	const auto vgIncomplete = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgIncomplete.has_value());
	const auto vgFraction = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFraction.has_value());

	const auto combined = vgIncomplete->combine(vgFraction.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::VisibilityGroup>(combined.value()));

	EXPECT_TRUE(std::get<metaf::VisibilityGroup>(combined.value()).isValid());
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

TEST(VisibilityGroup, isPrevailingCombined) {
	const auto vgIncomplete = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgIncomplete.has_value());
	const auto vgFraction = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFraction.has_value());

	const auto vg = vgIncomplete->combine(vgFraction.value());
	ASSERT_TRUE(vg.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::VisibilityGroup>(vg.value()));

	EXPECT_TRUE(std::get<metaf::VisibilityGroup>(vg.value()).isPrevailing());
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

TEST(VisibilityGroup, isDirectionalCombined) {
	const auto vgIncomplete = metaf::VisibilityGroup::parse("2", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgIncomplete.has_value());
	const auto vgFraction = metaf::VisibilityGroup::parse("1/4SM", metaf::ReportPart::METAR);
	ASSERT_TRUE(vgFraction.has_value());

	const auto vg1 = vgIncomplete->combine(vgFraction.value());
	ASSERT_TRUE(vg1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::VisibilityGroup>(vg1.value()));

	EXPECT_FALSE(std::get<metaf::VisibilityGroup>(vg1.value()).isDirectional());
}
