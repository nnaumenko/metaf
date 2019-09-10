/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(SecondaryLocationGroup, parseWs) {
	const auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());
	EXPECT_EQ(slg->type(), metaf::SecondaryLocationGroup::Type::INCOMPLETE);
	EXPECT_FALSE(slg->runway().has_value());
	EXPECT_FALSE(slg->direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsAll) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("ALL", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_FALSE(slg->runway().has_value());
	EXPECT_FALSE(slg->direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsAllRwy) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("ALL", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg->append("RWY", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	ASSERT_TRUE(slg->runway().has_value());
	EXPECT_TRUE(slg->runway()->isAllRunways());
	EXPECT_FALSE(slg->direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsR32) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("R32", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_EQ(slg->type(), metaf::SecondaryLocationGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	ASSERT_TRUE(slg->runway().has_value());
	EXPECT_EQ(slg->runway()->number(), 32u);
	EXPECT_EQ(slg->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(slg->direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsR27C) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("R27C", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	ASSERT_TRUE(slg->runway().has_value());
	EXPECT_EQ(slg->runway()->number(), 27u);
	EXPECT_EQ(slg->runway()->designator(), metaf::Runway::Designator::CENTER);
	EXPECT_FALSE(slg->direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsRWY32) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("RWY32", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	ASSERT_TRUE(slg->runway().has_value());
	EXPECT_EQ(slg->runway()->number(), 32u);
	EXPECT_EQ(slg->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(slg->direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsRWY27C) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("RWY27C", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	ASSERT_TRUE(slg->runway().has_value());
	EXPECT_EQ(slg->runway()->number(), 27u);
	EXPECT_EQ(slg->runway()->designator(), metaf::Runway::Designator::CENTER);
	EXPECT_FALSE(slg->direction().has_value());
}

TEST(SecondaryLocationGroup, windShearAppendWrongGroupWs) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("X37", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);
	EXPECT_EQ(slg->append("TEST", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(SecondaryLocationGroup, windShearAppendWrongGroupWsAll) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg->append("R22", metaf::ReportPart::METAR), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(SecondaryLocationGroup, windShearAppendToCompleteGroups) {
	auto slg1 = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg1.has_value());

	EXPECT_EQ(slg1->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg1->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(slg1->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg1->append("RWY32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg1->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg1->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	ASSERT_TRUE(slg1->runway().has_value());
	EXPECT_TRUE(slg1->runway()->isAllRunways());
	EXPECT_FALSE(slg1->direction().has_value());

	auto slg2 = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg2.has_value());

	EXPECT_EQ(slg2->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(slg2->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg2->append("RWY32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg2->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(slg2->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	ASSERT_TRUE(slg2->runway().has_value());
	EXPECT_EQ(slg2->runway()->number(), 32u);
	EXPECT_EQ(slg2->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(slg2->direction().has_value());
}

TEST(SecondaryLocationGroup, parseWindShearWrongReportPart) {
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::RMK).has_value());
}

TEST(SecondaryLocationGroup, parseWindShearWrongFormat) {
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("W0", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("ALL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("RWY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("R32", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("R27C", metaf::ReportPart::METAR).has_value());
}

TEST(SecondaryLocationGroup, isValidValidRunway) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("RWY32", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg->isValid());
}

TEST(SecondaryLocationGroup, isValidWindShearWrongRunway) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("RWY37", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(slg->isValid());
}

TEST(SecondaryLocationGroup, isValidWindShearAllRunways) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());

	EXPECT_EQ(slg->append("ALL", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(slg->append("RWY", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(slg->isValid());
}

TEST(SecondaryLocationGroup, isValidWindShearIncomplete) {
	auto slg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());
	ASSERT_EQ(slg->type(), metaf::SecondaryLocationGroup::Type::INCOMPLETE);
	EXPECT_FALSE(slg->isValid());

	EXPECT_EQ(slg->append("ALL", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	ASSERT_EQ(slg->type(), metaf::SecondaryLocationGroup::Type::INCOMPLETE);
	EXPECT_FALSE(slg->isValid());
}
