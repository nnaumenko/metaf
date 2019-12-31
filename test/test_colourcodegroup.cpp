/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

TEST(ColourCodeGroup, parseBlue) {
	const auto group = metaf::ColourCodeGroup::parse("BLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::BLUE);
	EXPECT_FALSE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseWhite) {
	const auto group = metaf::ColourCodeGroup::parse("WHT", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::WHITE);
	EXPECT_FALSE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseGreen) {
	const auto group = metaf::ColourCodeGroup::parse("GRN", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::GREEN);
	EXPECT_FALSE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseYellow1) {
	const auto group = metaf::ColourCodeGroup::parse("YLO1", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::YELLOW1);
	EXPECT_FALSE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseYellow2) {
	const auto group = metaf::ColourCodeGroup::parse("YLO2", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::YELLOW2);
	EXPECT_FALSE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseAmber) {
	const auto group = metaf::ColourCodeGroup::parse("AMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::AMBER);
	EXPECT_FALSE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseRed) {
	const auto group = metaf::ColourCodeGroup::parse("RED", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::RED);
	EXPECT_FALSE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseBlackBlue) {
	const auto group = metaf::ColourCodeGroup::parse("BLACKBLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::BLUE);
	EXPECT_TRUE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseBlackWhite) {
	const auto group = metaf::ColourCodeGroup::parse("BLACKWHT", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::WHITE);
	EXPECT_TRUE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseBlackGreen) {
	const auto group = metaf::ColourCodeGroup::parse("BLACKGRN", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::GREEN);
	EXPECT_TRUE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseBlackYellow1) {
	const auto group = metaf::ColourCodeGroup::parse("BLACKYLO1", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::YELLOW1);
	EXPECT_TRUE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseBlackYellow2) {
	const auto group = metaf::ColourCodeGroup::parse("BLACKYLO2", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::YELLOW2);
	EXPECT_TRUE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseBlackAmber) {
	const auto group = metaf::ColourCodeGroup::parse("BLACKAMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::AMBER);
	EXPECT_TRUE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseBlackRed) {
	const auto group = metaf::ColourCodeGroup::parse("BLACKRED", metaf::ReportPart::METAR);
	ASSERT_TRUE(group.has_value());
	EXPECT_EQ(group->code(), metaf::ColourCodeGroup::Code::RED);
	EXPECT_TRUE(group->isCodeBlack());
}

TEST(ColourCodeGroup, parseWrongReportPart) {
	static const std::string gs("BLU");
	ASSERT_TRUE(metaf::ColourCodeGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(ColourCodeGroup, parseWrongFormat) {
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("ZZZZZZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("BLACK", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("BLUBLACK", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("BLUWHT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("BLUE", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("BLLU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("BL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("BLACKBLUE", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("BLACKBLLU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::ColourCodeGroup::parse("BLACKBL", metaf::ReportPart::METAR).has_value());
}

TEST(ColourCodeGroup, isValidNoCodeBlack) {
	const auto ccg1 = metaf::ColourCodeGroup::parse("BLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg1.has_value());
	EXPECT_TRUE(ccg1->isValid());

	const auto ccg2 = metaf::ColourCodeGroup::parse("WHT", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg2.has_value());
	EXPECT_TRUE(ccg2->isValid());

	const auto ccg3 = metaf::ColourCodeGroup::parse("GRN", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg3.has_value());
	EXPECT_TRUE(ccg3->isValid());

	const auto ccg4 = metaf::ColourCodeGroup::parse("YLO1", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg4.has_value());
	EXPECT_TRUE(ccg4->isValid());

	const auto ccg5 = metaf::ColourCodeGroup::parse("YLO2", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg5.has_value());
	EXPECT_TRUE(ccg5->isValid());

	const auto ccg6 = metaf::ColourCodeGroup::parse("AMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg6.has_value());
	EXPECT_TRUE(ccg6->isValid());

	const auto ccg7 = metaf::ColourCodeGroup::parse("RED", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg7.has_value());
	EXPECT_TRUE(ccg7->isValid());
}

TEST(ColourCodeGroup, isValidCodeBlack) {
	const auto ccg1 = metaf::ColourCodeGroup::parse("BLACKBLU", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg1.has_value());
	EXPECT_TRUE(ccg1->isValid());

	const auto ccg2 = metaf::ColourCodeGroup::parse("BLACKWHT", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg2.has_value());
	EXPECT_TRUE(ccg2->isValid());

	const auto ccg3 = metaf::ColourCodeGroup::parse("BLACKGRN", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg3.has_value());
	EXPECT_TRUE(ccg3->isValid());

	const auto ccg4 = metaf::ColourCodeGroup::parse("BLACKYLO1", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg4.has_value());
	EXPECT_TRUE(ccg4->isValid());

	const auto ccg5 = metaf::ColourCodeGroup::parse("BLACKYLO2", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg5.has_value());
	EXPECT_TRUE(ccg5->isValid());

	const auto ccg6 = metaf::ColourCodeGroup::parse("BLACKAMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg6.has_value());
	EXPECT_TRUE(ccg6->isValid());

	const auto ccg7 = metaf::ColourCodeGroup::parse("BLACKRED", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg7.has_value());
	EXPECT_TRUE(ccg7->isValid());
}


TEST(ColourCodeGroup, append) {
	auto ccg1 = metaf::ColourCodeGroup::parse("BLACKAMB", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg1.has_value());

	auto ccg2 = metaf::ColourCodeGroup::parse("RED", metaf::ReportPart::METAR);
	ASSERT_TRUE(ccg2.has_value());

	EXPECT_EQ(ccg1->append("BLACKAMB", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ccg1->append("RED", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ccg1->append("RMK", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ccg1->append("TEST", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ccg2->append("BLACKAMB", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ccg2->append("RED", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ccg2->append("RMK", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ccg2->append("TEST", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
}
