/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

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

TEST(PressureTendencyGroup, append) {
	const std::string ptgStr1("56132");
	const std::string ptgStr2("52///");
	const std::string ptgStr3("5/132");
	const std::string ptgStr4("5////");
	const std::string ptgStr5("PRESRR");
	const std::string ptgStr6("PRESFR");
	const std::string tStr1("RMK");
	const std::string tStr2("TEST");

	auto ptg1 = metaf::PressureTendencyGroup::parse(ptgStr1, metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg1.has_value());

	auto ptg2 = metaf::PressureTendencyGroup::parse(ptgStr2, metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg2.has_value());

	auto ptg3 = metaf::PressureTendencyGroup::parse(ptgStr3, metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg3.has_value());

	auto ptg4 = metaf::PressureTendencyGroup::parse(ptgStr4, metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg4.has_value());

	auto ptg5 = metaf::PressureTendencyGroup::parse(ptgStr5, metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg5.has_value());

	auto ptg6 = metaf::PressureTendencyGroup::parse(ptgStr6, metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg6.has_value());

	EXPECT_EQ(ptg1->append(ptgStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg1->append(ptgStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg1->append(ptgStr3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg1->append(ptgStr4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg1->append(ptgStr5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg1->append(ptgStr6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg1->append(tStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg1->append(tStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ptg2->append(ptgStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg2->append(ptgStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg2->append(ptgStr3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg2->append(ptgStr4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg2->append(ptgStr5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg2->append(ptgStr6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg2->append(tStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg2->append(tStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ptg3->append(ptgStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg3->append(ptgStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg3->append(ptgStr3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg3->append(ptgStr4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg3->append(ptgStr5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg3->append(ptgStr6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg3->append(tStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg3->append(tStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ptg4->append(ptgStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg4->append(ptgStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg4->append(ptgStr3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg4->append(ptgStr4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg4->append(ptgStr5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg4->append(ptgStr6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg4->append(tStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg4->append(tStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ptg5->append(ptgStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg5->append(ptgStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg5->append(ptgStr3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg5->append(ptgStr4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg5->append(ptgStr5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg5->append(ptgStr6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg5->append(tStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg5->append(tStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ptg6->append(ptgStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg6->append(ptgStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg6->append(ptgStr3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg6->append(ptgStr4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg6->append(ptgStr5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg6->append(ptgStr6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg6->append(tStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ptg6->append(tStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
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