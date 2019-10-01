/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

TEST(ReportTimeGroup, parseReportTime) {
	const auto rtg = metaf::ReportTimeGroup::parse("201730Z", metaf::ReportPart::HEADER);
	ASSERT_TRUE(rtg.has_value());
	ASSERT_TRUE(rtg->time().day().has_value());
	EXPECT_EQ(rtg->time().day().value(), 20u);
	EXPECT_EQ(rtg->time().hour(), 17u);
	EXPECT_EQ(rtg->time().minute(), 30u);
}

TEST(ReportTimeGroup, parseWrongReportPart) {
	const std::string gs("201730Z");
	ASSERT_TRUE(metaf::ReportTimeGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::ReportTimeGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::ReportTimeGroup::parse(gs, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::ReportTimeGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::ReportTimeGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(ReportTimeGroup, parseWrongFormat) {
	EXPECT_FALSE(
		metaf::ReportTimeGroup::parse("", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::ReportTimeGroup::parse("//////Z", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::ReportTimeGroup::parse("2017030Z", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::ReportTimeGroup::parse("20173Z", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::ReportTimeGroup::parse("201730", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::ReportTimeGroup::parse("05025KT", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::ReportTimeGroup::parse("1709/1721", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::ReportTimeGroup::parse("FM122230", metaf::ReportPart::HEADER).has_value());
}

TEST(ReportTimeGroup, isValidCorrect) {
	const auto rtg1 = metaf::ReportTimeGroup::parse("312459Z", metaf::ReportPart::HEADER);
	ASSERT_TRUE(rtg1.has_value());
	EXPECT_TRUE(rtg1->isValid());

	const auto rtg2 = metaf::ReportTimeGroup::parse("010000Z", metaf::ReportPart::HEADER);
	ASSERT_TRUE(rtg2.has_value());
	EXPECT_TRUE(rtg2->isValid());
}

TEST(ReportTimeGroup, isValidDayIncorrect) {
	const auto rtg1 = metaf::ReportTimeGroup::parse("321730Z", metaf::ReportPart::HEADER);
	ASSERT_TRUE(rtg1.has_value());
	EXPECT_FALSE(rtg1->isValid());

	const auto rtg2 = metaf::ReportTimeGroup::parse("001730Z", metaf::ReportPart::HEADER);
	ASSERT_TRUE(rtg2.has_value());
	EXPECT_FALSE(rtg2->isValid());
}

TEST(ReportTimeGroup, isValidHourIncorrect) {
	const auto rtg = metaf::ReportTimeGroup::parse("202530Z", metaf::ReportPart::HEADER);
	ASSERT_TRUE(rtg.has_value());
	EXPECT_FALSE(rtg->isValid());
}

TEST(ReportTimeGroup, isValidMinuteIncorrect) {
	const auto rtg = metaf::ReportTimeGroup::parse("201760Z", metaf::ReportPart::HEADER);
	ASSERT_TRUE(rtg.has_value());
	EXPECT_FALSE(rtg->isValid());
}

TEST(ReportTimeGroup, append) {
	auto rtg1 = metaf::ReportTimeGroup::parse("201700Z", metaf::ReportPart::HEADER);
	ASSERT_TRUE(rtg1.has_value());

	auto rtg2 = metaf::ReportTimeGroup::parse("201730Z", metaf::ReportPart::HEADER);
	ASSERT_TRUE(rtg2.has_value());

	EXPECT_EQ(rtg1->append("201730Z", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg1->append("FM1250", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg1->append("TL1830", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg1->append("AT1800", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg1->append("2016/2019", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg1->append("AUTO", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg1->append("31003KT", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg1->append("RMK", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(rtg2->append("201730Z", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg2->append("FM1250", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg2->append("TL1830", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg2->append("AT1800", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg2->append("2016/2019", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg2->append("AUTO", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg2->append("31003KT", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rtg2->append("RMK", metaf::ReportPart::HEADER), 
		metaf::AppendResult::NOT_APPENDED);
}
