/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

const auto margin = 0.1 / 2;

TEST(PressureTendencyGroup, pressureDifference) {
	const auto ptg = metaf::PressureTendencyGroup::parse("52132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	ASSERT_TRUE(ptg->difference().pressure().has_value());
	EXPECT_EQ(ptg->difference().unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_NEAR(ptg->difference().pressure().value(), 13.2, margin);
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::INCREASING);
}

TEST(PressureTendencyGroup, pressureDifferenceNotReported) {
	const auto ptg = metaf::PressureTendencyGroup::parse("52///", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_FALSE(ptg->difference().pressure().has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::INCREASING);
}

TEST(PressureTendencyGroup, notReported) {
	const auto ptg = metaf::PressureTendencyGroup::parse("5////", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_FALSE(ptg->difference().pressure().has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::NOT_REPORTED);
}

TEST(PressureTendencyGroup, typeIncreasingThenDecreasing) {
	const auto ptg = metaf::PressureTendencyGroup::parse("50132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::INCREASING_THEN_DECREASING);
}

TEST(PressureTendencyGroup, typeIncreasingMoreSlowly) {
	const auto ptg = metaf::PressureTendencyGroup::parse("51132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::INCREASING_MORE_SLOWLY);
}

TEST(PressureTendencyGroup, typeIncreasing) {
	const auto ptg = metaf::PressureTendencyGroup::parse("52132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::INCREASING);
}

TEST(PressureTendencyGroup, typeIncreasingMoreRapidly) {
	const auto ptg = metaf::PressureTendencyGroup::parse("53132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::INCREASING_MORE_RAPIDLY);
}

TEST(PressureTendencyGroup, typeSteady) {
	const auto ptg = metaf::PressureTendencyGroup::parse("54132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::STEADY);
}

TEST(PressureTendencyGroup, typeDecreasingThenIncreasing) {
	const auto ptg = metaf::PressureTendencyGroup::parse("55132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::DECREASING_THEN_INCREASING);
}

TEST(PressureTendencyGroup, typeDecreasingMoreSlowly) {
	const auto ptg = metaf::PressureTendencyGroup::parse("56132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::DECREASING_MORE_SLOWLY);
}

TEST(PressureTendencyGroup, typeDecreasing) {
	const auto ptg = metaf::PressureTendencyGroup::parse("57132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::DECREASING);
}

TEST(PressureTendencyGroup, typeDecreasingMoreRapidly) {
	const auto ptg = metaf::PressureTendencyGroup::parse("58132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::DECREASING_MORE_RAPIDLY);
}

TEST(PressureTendencyGroup, pressureRisingRapidly) {
	const auto ptg = metaf::PressureTendencyGroup::parse("PRESRR", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::RISING_RAPIDLY);
	EXPECT_FALSE(ptg->difference().isReported());
}

TEST(PressureTendencyGroup, pressureFallingRapidly) {
	const auto ptg = metaf::PressureTendencyGroup::parse("PRESFR", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::FALLING_RAPIDLY);
	EXPECT_FALSE(ptg->difference().isReported());
}

TEST(PressureTendencyGroup, typeNotReported) {
	const auto ptg = metaf::PressureTendencyGroup::parse("5/132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg.has_value());
	EXPECT_EQ(ptg->type(), metaf::PressureTendencyGroup::Type::NOT_REPORTED);
}

TEST(PressureTendencyGroup, wrongReportPart) {
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("52132", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("52132", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("52132", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("52132", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("PRESRR", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("PRESRR", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("PRESRR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("PRESRR", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("PRESFR", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("PRESFR", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("PRESFR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("PRESFR", metaf::ReportPart::TAF).has_value());
}

TEST(PressureTendencyGroup, wrongFormat) {
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("02132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("12132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("22132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("32132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("42132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("62132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("72132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("82132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("92132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("A2132", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("59132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("5A132", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("5/////", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("5///", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("520132", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("5232", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("52A32", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("520A2", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PressureTendencyGroup::parse("5203A", metaf::ReportPart::RMK).has_value());
}

TEST(PressureTendencyGroup, trend) {
	EXPECT_EQ(metaf::PressureTendencyGroup::trend(
		metaf::PressureTendencyGroup::Type::INCREASING_THEN_DECREASING),
		metaf::PressureTendencyGroup::Trend::HIGHER_OR_SAME);

	EXPECT_EQ(metaf::PressureTendencyGroup::trend(
		metaf::PressureTendencyGroup::Type::INCREASING_MORE_SLOWLY),
		metaf::PressureTendencyGroup::Trend::HIGHER);

	EXPECT_EQ(metaf::PressureTendencyGroup::trend(
		metaf::PressureTendencyGroup::Type::INCREASING),
		metaf::PressureTendencyGroup::Trend::HIGHER);

	EXPECT_EQ(metaf::PressureTendencyGroup::trend(
		metaf::PressureTendencyGroup::Type::INCREASING_MORE_RAPIDLY),
		metaf::PressureTendencyGroup::Trend::HIGHER);

	EXPECT_EQ(metaf::PressureTendencyGroup::trend(
		metaf::PressureTendencyGroup::Type::STEADY),
		metaf::PressureTendencyGroup::Trend::SAME);

	EXPECT_EQ(metaf::PressureTendencyGroup::trend(
		metaf::PressureTendencyGroup::Type::DECREASING_THEN_INCREASING),
		metaf::PressureTendencyGroup::Trend::LOWER_OR_SAME);

	EXPECT_EQ(metaf::PressureTendencyGroup::trend(
		metaf::PressureTendencyGroup::Type::DECREASING_MORE_SLOWLY),
		metaf::PressureTendencyGroup::Trend::LOWER);

	EXPECT_EQ(metaf::PressureTendencyGroup::trend(
		metaf::PressureTendencyGroup::Type::DECREASING),
		metaf::PressureTendencyGroup::Trend::LOWER);

	EXPECT_EQ(metaf::PressureTendencyGroup::trend(
		metaf::PressureTendencyGroup::Type::DECREASING_MORE_RAPIDLY),
		metaf::PressureTendencyGroup::Trend::LOWER);

	EXPECT_EQ(metaf::PressureTendencyGroup::trend(
		metaf::PressureTendencyGroup::Type::NOT_REPORTED),
		metaf::PressureTendencyGroup::Trend::NOT_REPORTED);
}

TEST(PressureTendencyGroup, combine) {
	const auto ptg1 = metaf::PressureTendencyGroup::parse("56132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg1.has_value());

	const auto ptg2 = metaf::PressureTendencyGroup::parse("52///", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg2.has_value());

	const auto ptg3 = metaf::PressureTendencyGroup::parse("5/132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg3.has_value());

	const auto ptg4 = metaf::PressureTendencyGroup::parse("5////", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg4.has_value());

	const auto ptg5 = metaf::PressureTendencyGroup::parse("PRESRR", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg5.has_value());

	const auto ptg6 = metaf::PressureTendencyGroup::parse("PRESFR", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg6.has_value());

	const auto rmk = metaf::FixedGroup::parse("RMK", metaf::ReportPart::METAR);
	ASSERT_TRUE(rmk.has_value());

	EXPECT_FALSE(ptg1->combine(rmk.value()).has_value());
	EXPECT_FALSE(ptg1->combine(ptg1.value()).has_value());
	EXPECT_FALSE(ptg1->combine(ptg2.value()).has_value());
	EXPECT_FALSE(ptg1->combine(ptg3.value()).has_value());
	EXPECT_FALSE(ptg1->combine(ptg4.value()).has_value());
	EXPECT_FALSE(ptg1->combine(ptg5.value()).has_value());
	EXPECT_FALSE(ptg1->combine(ptg6.value()).has_value());
	EXPECT_FALSE(ptg1->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(ptg2->combine(rmk.value()).has_value());
	EXPECT_FALSE(ptg2->combine(ptg1.value()).has_value());
	EXPECT_FALSE(ptg2->combine(ptg2.value()).has_value());
	EXPECT_FALSE(ptg2->combine(ptg3.value()).has_value());
	EXPECT_FALSE(ptg2->combine(ptg4.value()).has_value());
	EXPECT_FALSE(ptg2->combine(ptg5.value()).has_value());
	EXPECT_FALSE(ptg2->combine(ptg6.value()).has_value());
	EXPECT_FALSE(ptg2->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(ptg3->combine(rmk.value()).has_value());
	EXPECT_FALSE(ptg3->combine(ptg1.value()).has_value());
	EXPECT_FALSE(ptg3->combine(ptg2.value()).has_value());
	EXPECT_FALSE(ptg3->combine(ptg3.value()).has_value());
	EXPECT_FALSE(ptg3->combine(ptg4.value()).has_value());
	EXPECT_FALSE(ptg3->combine(ptg5.value()).has_value());
	EXPECT_FALSE(ptg3->combine(ptg6.value()).has_value());
	EXPECT_FALSE(ptg3->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(ptg4->combine(rmk.value()).has_value());
	EXPECT_FALSE(ptg4->combine(ptg1.value()).has_value());
	EXPECT_FALSE(ptg4->combine(ptg2.value()).has_value());
	EXPECT_FALSE(ptg4->combine(ptg3.value()).has_value());
	EXPECT_FALSE(ptg4->combine(ptg4.value()).has_value());
	EXPECT_FALSE(ptg4->combine(ptg5.value()).has_value());
	EXPECT_FALSE(ptg4->combine(ptg6.value()).has_value());
	EXPECT_FALSE(ptg4->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(ptg5->combine(rmk.value()).has_value());
	EXPECT_FALSE(ptg5->combine(ptg1.value()).has_value());
	EXPECT_FALSE(ptg5->combine(ptg2.value()).has_value());
	EXPECT_FALSE(ptg5->combine(ptg3.value()).has_value());
	EXPECT_FALSE(ptg5->combine(ptg4.value()).has_value());
	EXPECT_FALSE(ptg5->combine(ptg5.value()).has_value());
	EXPECT_FALSE(ptg5->combine(ptg6.value()).has_value());
	EXPECT_FALSE(ptg5->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(ptg6->combine(rmk.value()).has_value());
	EXPECT_FALSE(ptg6->combine(ptg1.value()).has_value());
	EXPECT_FALSE(ptg6->combine(ptg2.value()).has_value());
	EXPECT_FALSE(ptg6->combine(ptg3.value()).has_value());
	EXPECT_FALSE(ptg6->combine(ptg4.value()).has_value());
	EXPECT_FALSE(ptg6->combine(ptg5.value()).has_value());
	EXPECT_FALSE(ptg6->combine(ptg6.value()).has_value());
	EXPECT_FALSE(ptg6->combine(metaf::PlainTextGroup("TEST")).has_value());
}

TEST(PressureTendencyGroup, isValid) {
	const auto ptg1 = metaf::PressureTendencyGroup::parse("56132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg1.has_value());
	EXPECT_TRUE(ptg1->isValid());	

	const auto ptg2 = metaf::PressureTendencyGroup::parse("52///", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg2.has_value());
	EXPECT_TRUE(ptg2->isValid());	

	const auto ptg3 = metaf::PressureTendencyGroup::parse("5/132", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg3.has_value());
	EXPECT_TRUE(ptg3->isValid());	

	const auto ptg4 = metaf::PressureTendencyGroup::parse("5////", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg4.has_value());
	EXPECT_TRUE(ptg4->isValid());	

	const auto ptg5 = metaf::PressureTendencyGroup::parse("PRESRR", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg5.has_value());
	EXPECT_TRUE(ptg5->isValid());	

	const auto ptg6 = metaf::PressureTendencyGroup::parse("PRESFR", metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg6.has_value());
	EXPECT_TRUE(ptg6->isValid());	
}