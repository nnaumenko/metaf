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

TEST(MiscGroup, parseCorrectedObservation) {
	const auto mg1 = metaf::MiscGroup::parse("CCA", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg1.has_value());
	EXPECT_EQ(mg1->type(), metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION);
	ASSERT_TRUE(mg1->value().has_value());
	EXPECT_EQ(static_cast<int>(mg1->value().value()), 1);

	const auto mg2 = metaf::MiscGroup::parse("CCZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(mg2.has_value());
	EXPECT_EQ(mg2->type(), metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION);
	ASSERT_TRUE(mg1->value().has_value());
	EXPECT_EQ(static_cast<int>(mg2->value().value()), 26);
}

TEST(MiscGroup, parseCorrectedObservationWrongReportPart) {
	EXPECT_FALSE(metaf::MiscGroup::parse("CCA", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::MiscGroup::parse("CCA", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::MiscGroup::parse("CCA", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::MiscGroup::parse("CCA", metaf::ReportPart::RMK));
}

TEST(MiscGroup, parseCorrectedObservationDurationWrongFormat) {
	EXPECT_FALSE(metaf::MiscGroup::parse("CC", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CC/", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CC0", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CC-", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CCAA", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CA", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("XCA", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::MiscGroup::parse("CXA", metaf::ReportPart::METAR));
}

TEST(MiscGroup, combine) {
	const auto ssg1 = metaf::MiscGroup::parse("98096", metaf::ReportPart::RMK);
	ASSERT_TRUE(ssg1.has_value());

	const auto ssg2 = metaf::MiscGroup::parse("CCA", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg2.has_value());

	const auto rmk = metaf::FixedGroup::parse("RMK", metaf::ReportPart::METAR);
	ASSERT_TRUE(rmk.has_value());

	EXPECT_FALSE(ssg1->combine(rmk.value()).has_value());
	EXPECT_FALSE(ssg1->combine(ssg1.value()).has_value());
	EXPECT_FALSE(ssg1->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(ssg2->combine(rmk.value()).has_value());
	EXPECT_FALSE(ssg2->combine(ssg2.value()).has_value());
	EXPECT_FALSE(ssg2->combine(metaf::PlainTextGroup("TEST")).has_value());
}

TEST(MiscGroup, isValid) {
	const auto ssg1 = metaf::MiscGroup::parse("98096", metaf::ReportPart::RMK);
	ASSERT_TRUE(ssg1.has_value());
	EXPECT_TRUE(ssg1->isValid());

	const auto ssg2 = metaf::MiscGroup::parse("CCA", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg2.has_value());
	EXPECT_TRUE(ssg2->isValid());
}
