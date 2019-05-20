/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

const auto marginSunshineDuration = 1.0 / 2;

TEST(MiscGroup, parseSunshineDuration) {
	const auto mg1 = metaf::MiscGroup::parse("98096", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES);
	ASSERT_TRUE(mg1->value().has_value());
	EXPECT_NEAR(mg1->value().value(), 96, marginSunshineDuration);

	const auto mg2 = metaf::MiscGroup::parse("98000", metaf::ReportPart::RMK);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES);
	ASSERT_TRUE(mg1->value().has_value());
	EXPECT_NEAR(mg2->value().value(), 0, marginSunshineDuration);
}

TEST(MiscGroup, parseSunshineDurationWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("98096", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse("98096", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse("98096", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("98096", metaf::ReportPart::TAF));
}

TEST(MiscGroup, parseSunshineDurationWrongFormat) {
	EXPECT_FALSE(metaf::MiscGroup::parse("98", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("98///", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("970096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("990096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("1000096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("90096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("9800096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("9A0096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("98A096", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("98000A", metaf::ReportPart::RMK));
	EXPECT_FALSE(metaf::MiscGroup::parse("18096", metaf::ReportPart::RMK));
}

