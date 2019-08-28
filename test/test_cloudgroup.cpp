/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(CloudGroup, parseCloudLayerMetar) {
	const auto cg = metaf::CloudGroup::parse("SCT040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->height().isInteger());
	ASSERT_TRUE(cg->height().integer().has_value());
	EXPECT_EQ(cg->height().integer().value(), 4000u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerTaf) {
	const auto cg = metaf::CloudGroup::parse("SCT040", metaf::ReportPart::TAF);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->height().isInteger());
	ASSERT_TRUE(cg->height().integer().has_value());
	EXPECT_EQ(cg->height().integer().value(), 4000u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerFew) {
	const auto cg = metaf::CloudGroup::parse("FEW117", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::FEW);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->height().isInteger());
	ASSERT_TRUE(cg->height().integer().has_value());
	EXPECT_EQ(cg->height().integer().value(), 11700u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerScattered) {
	const auto cg = metaf::CloudGroup::parse("SCT370", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->height().isInteger());
	ASSERT_TRUE(cg->height().integer().has_value());
	EXPECT_EQ(cg->height().integer().value(), 37000u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerBroken) {
	const auto cg = metaf::CloudGroup::parse("BKN026", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::BROKEN);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->height().isInteger());
	ASSERT_TRUE(cg->height().integer().has_value());
	EXPECT_EQ(cg->height().integer().value(), 2600u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerOvercast) {
	const auto cg = metaf::CloudGroup::parse("OVC002", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OVERCAST);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->height().isInteger());
	ASSERT_TRUE(cg->height().integer().has_value());
	EXPECT_EQ(cg->height().integer().value(), 200u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerToweringCumulus) {
	const auto cg = metaf::CloudGroup::parse("SCT025TCU", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->height().isInteger());
	ASSERT_TRUE(cg->height().integer().has_value());
	EXPECT_EQ(cg->height().integer().value(), 2500u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::TOWERING_CUMULUS);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerCumulonimbus) {
	const auto cg = metaf::CloudGroup::parse("BKN012CB", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::BROKEN);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->height().isInteger());
	ASSERT_TRUE(cg->height().integer().has_value());
	EXPECT_EQ(cg->height().integer().value(), 1200u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CUMULONIMBUS);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerConvectiveTypeNotReported) {
	const auto cg = metaf::CloudGroup::parse("SCT016///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->height().isInteger());
	ASSERT_TRUE(cg->height().integer().has_value());
	EXPECT_EQ(cg->height().integer().value(), 1600u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NOT_REPORTED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerAmountAndTypeNotReported) {
	const auto cg = metaf::CloudGroup::parse("///074///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->height().isInteger());
	ASSERT_TRUE(cg->height().integer().has_value());
	EXPECT_EQ(cg->height().integer().value(), 7400u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NOT_REPORTED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerAmountAndHeightNotReported) {
	const auto cg = metaf::CloudGroup::parse("//////TCU", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::TOWERING_CUMULUS);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerNotReported) {
	const auto cg = metaf::CloudGroup::parse("/////////", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NOT_REPORTED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
}

TEST(CloudGroup, parseCloudLayerWrongReportPart) {
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040", metaf::ReportPart::RMK).has_value());
}

TEST(CloudGroup, parseCloudLayerWrongFormat) {
	EXPECT_FALSE(metaf::CloudGroup::parse("AAA040", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("BCC040", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT0040", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT40", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCC040", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT04A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040TCB", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040CCU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040//", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("////040", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("//040", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT//", metaf::ReportPart::METAR).has_value());
}

TEST(CloudGroup, parseVerticalVisibilityMetar) {
	const auto cg = metaf::CloudGroup::parse("VV016", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	EXPECT_EQ(cg->verticalVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->verticalVisibility().isInteger());
	ASSERT_TRUE(cg->verticalVisibility().integer().has_value());
	EXPECT_EQ(cg->verticalVisibility().integer().value(), 1600u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NONE);
}

TEST(CloudGroup, parseVerticalVisibilityTaf) {
	const auto cg = metaf::CloudGroup::parse("VV016", metaf::ReportPart::TAF);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	EXPECT_EQ(cg->verticalVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->verticalVisibility().isInteger());
	ASSERT_TRUE(cg->verticalVisibility().integer().has_value());
	EXPECT_EQ(cg->verticalVisibility().integer().value(), 1600u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg->height().isReported());
}

TEST(CloudGroup, parseVerticalVisibilityZero) {
	const auto cg = metaf::CloudGroup::parse("VV000", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	EXPECT_EQ(cg->verticalVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg->verticalVisibility().isInteger());
	ASSERT_TRUE(cg->verticalVisibility().integer().has_value());
	EXPECT_EQ(cg->verticalVisibility().integer().value(), 0u);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg->height().isReported());
}

TEST(CloudGroup, parseVerticalVisibilityNotReported) {
	const auto cg = metaf::CloudGroup::parse("VV///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	EXPECT_EQ(cg->verticalVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg->height().isReported());
}

TEST(CloudGroup, parseVerticalVisibilityWrongReportPart) {
	EXPECT_FALSE(metaf::CloudGroup::parse("VV016", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV016", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV016", metaf::ReportPart::RMK).has_value());
}

TEST(CloudGroup, parseVerticalVisibilityWrongFormat) {
	EXPECT_FALSE(metaf::CloudGroup::parse("VV16", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV0016", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV01A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV//", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV016TCU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV016CB", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV016///", metaf::ReportPart::METAR).has_value());
}

TEST(CloudGroup, parseClr) {
	static const std::string gs("CLR");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NONE_CLR;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::NONE);

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::NONE);
}

TEST(CloudGroup, parseSkc) {
	static const std::string gs("SKC");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NONE_SKC;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::NONE);

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::NONE);
}

TEST(CloudGroup, parseNcd) {
	static const std::string gs("NCD");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NCD;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::NONE);

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::NONE);
}

TEST(CloudGroup, parseNsc) {
	static const std::string gs("NSC");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NSC;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::NONE);

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::NONE);
}

TEST(CloudGroup, parseFixedWrongReportPart) {
	static const std::string s1("CLR");
	EXPECT_FALSE(metaf::CloudGroup::parse(s1, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse(s1, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse(s1, metaf::ReportPart::RMK).has_value());

	static const std::string s2("SKC");
	EXPECT_FALSE(metaf::CloudGroup::parse(s2, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse(s2, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse(s2, metaf::ReportPart::RMK).has_value());

	static const std::string s3("NCD");
	EXPECT_FALSE(metaf::CloudGroup::parse(s3, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse(s3, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse(s3, metaf::ReportPart::RMK).has_value());

	static const std::string s4("NSC");
	EXPECT_FALSE(metaf::CloudGroup::parse(s4, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse(s4, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse(s4, metaf::ReportPart::RMK).has_value());
}

TEST(CloudGroup, isCloudLayerTrue) {
	const auto cg1 = metaf::CloudGroup::parse("FEW040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_TRUE(cg1->isCloudLayer());

	const auto cg2 = metaf::CloudGroup::parse("SCT040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_TRUE(cg2->isCloudLayer());

	const auto cg3 = metaf::CloudGroup::parse("BKN040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_TRUE(cg3->isCloudLayer());

	const auto cg4 = metaf::CloudGroup::parse("OVC040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_TRUE(cg4->isCloudLayer());
}

TEST(CloudGroup, isCloudLayerFalse) {
	const auto cg1 = metaf::CloudGroup::parse("CLR", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_FALSE(cg1->isCloudLayer());

	const auto cg2 = metaf::CloudGroup::parse("SKC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_FALSE(cg2->isCloudLayer());

	const auto cg3 = metaf::CloudGroup::parse("NCD", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_FALSE(cg3->isCloudLayer());

	const auto cg4 = metaf::CloudGroup::parse("NSC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_FALSE(cg4->isCloudLayer());

	const auto cg5 = metaf::CloudGroup::parse("///040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_FALSE(cg5->isCloudLayer());

	const auto cg6 = metaf::CloudGroup::parse("VV004", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_FALSE(cg6->isCloudLayer());
}

TEST(CloudGroup, isNoCloudsTrue) {
	const auto cg1 = metaf::CloudGroup::parse("CLR", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_TRUE(cg1->isNoClouds());

	const auto cg2 = metaf::CloudGroup::parse("SKC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_TRUE(cg2->isNoClouds());

	const auto cg3 = metaf::CloudGroup::parse("NCD", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_TRUE(cg3->isNoClouds());

	const auto cg4 = metaf::CloudGroup::parse("NSC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_TRUE(cg4->isNoClouds());
}

TEST(CloudGroup, isNoCloudsFalse) {
	const auto cg1 = metaf::CloudGroup::parse("FEW040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_FALSE(cg1->isNoClouds());

	const auto cg2 = metaf::CloudGroup::parse("SCT040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_FALSE(cg2->isNoClouds());

	const auto cg3 = metaf::CloudGroup::parse("BKN040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_FALSE(cg3->isNoClouds());

	const auto cg4 = metaf::CloudGroup::parse("OVC040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_FALSE(cg4->isNoClouds());

	const auto cg5 = metaf::CloudGroup::parse("///040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_FALSE(cg5->isNoClouds());

	const auto cg6 = metaf::CloudGroup::parse("VV004", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_FALSE(cg6->isNoClouds());
}

TEST(CloudGroup, isVerticalVisibilityTrue) {
	const auto cg1 = metaf::CloudGroup::parse("VV004", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_TRUE(cg1->isVerticalVisibility());

	const auto cg2 = metaf::CloudGroup::parse("VV///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_TRUE(cg2->isVerticalVisibility());
}

TEST(CloudGroup, isVerticalVisibilityFalse) {
	const auto cg1 = metaf::CloudGroup::parse("FEW040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_FALSE(cg1->isVerticalVisibility());

	const auto cg2 = metaf::CloudGroup::parse("SCT040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_FALSE(cg2->isVerticalVisibility());

	const auto cg3 = metaf::CloudGroup::parse("BKN040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_FALSE(cg3->isVerticalVisibility());

	const auto cg4 = metaf::CloudGroup::parse("OVC040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_FALSE(cg4->isVerticalVisibility());

	const auto cg5 = metaf::CloudGroup::parse("///040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_FALSE(cg5->isVerticalVisibility());

	const auto cg6 = metaf::CloudGroup::parse("CLR", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_FALSE(cg6->isVerticalVisibility());

	const auto cg7 = metaf::CloudGroup::parse("SKC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg7.has_value());
	EXPECT_FALSE(cg7->isVerticalVisibility());

	const auto cg8 = metaf::CloudGroup::parse("NSC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg8.has_value());
	EXPECT_FALSE(cg8->isVerticalVisibility());

	const auto cg9 = metaf::CloudGroup::parse("NCD", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg9.has_value());
	EXPECT_FALSE(cg9->isVerticalVisibility());
}

TEST(CloudGroup, combine) {
	const auto cg1 = metaf::CloudGroup::parse("CLR", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_FALSE(cg1->isCloudLayer());

	const auto cg2 = metaf::CloudGroup::parse("SKC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_FALSE(cg2->isCloudLayer());

	const auto cg3 = metaf::CloudGroup::parse("NCD", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_FALSE(cg3->isCloudLayer());

	const auto cg4 = metaf::CloudGroup::parse("NSC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_FALSE(cg4->isCloudLayer());

	const auto cg5 = metaf::CloudGroup::parse("FEW040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg5.has_value());

	const auto cg6 = metaf::CloudGroup::parse("VV004", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg6.has_value());

	const auto rmk = metaf::FixedGroup::parse("RMK", metaf::ReportPart::METAR);
	ASSERT_TRUE(rmk.has_value());

	const auto text = metaf::PlainTextGroup::parse("TEST", metaf::ReportPart::METAR);
	ASSERT_TRUE(text.has_value());

	EXPECT_FALSE(cg1->combine(cg1.value()).has_value());
	EXPECT_FALSE(cg1->combine(cg2.value()).has_value());
	EXPECT_FALSE(cg1->combine(cg3.value()).has_value());
	EXPECT_FALSE(cg1->combine(cg4.value()).has_value());
	EXPECT_FALSE(cg1->combine(cg5.value()).has_value());
	EXPECT_FALSE(cg1->combine(cg6.value()).has_value());
	EXPECT_FALSE(cg1->combine(rmk.value()).has_value());
	EXPECT_FALSE(cg1->combine(text.value()).has_value());

	EXPECT_FALSE(cg2->combine(cg1.value()).has_value());
	EXPECT_FALSE(cg2->combine(cg2.value()).has_value());
	EXPECT_FALSE(cg2->combine(cg3.value()).has_value());
	EXPECT_FALSE(cg2->combine(cg4.value()).has_value());
	EXPECT_FALSE(cg2->combine(cg5.value()).has_value());
	EXPECT_FALSE(cg2->combine(cg6.value()).has_value());
	EXPECT_FALSE(cg2->combine(rmk.value()).has_value());
	EXPECT_FALSE(cg2->combine(text.value()).has_value());

	EXPECT_FALSE(cg3->combine(cg1.value()).has_value());
	EXPECT_FALSE(cg3->combine(cg2.value()).has_value());
	EXPECT_FALSE(cg3->combine(cg3.value()).has_value());
	EXPECT_FALSE(cg3->combine(cg4.value()).has_value());
	EXPECT_FALSE(cg3->combine(cg5.value()).has_value());
	EXPECT_FALSE(cg3->combine(cg6.value()).has_value());
	EXPECT_FALSE(cg3->combine(rmk.value()).has_value());
	EXPECT_FALSE(cg3->combine(text.value()).has_value());

	EXPECT_FALSE(cg4->combine(cg1.value()).has_value());
	EXPECT_FALSE(cg4->combine(cg2.value()).has_value());
	EXPECT_FALSE(cg4->combine(cg3.value()).has_value());
	EXPECT_FALSE(cg4->combine(cg4.value()).has_value());
	EXPECT_FALSE(cg4->combine(cg5.value()).has_value());
	EXPECT_FALSE(cg4->combine(cg6.value()).has_value());
	EXPECT_FALSE(cg4->combine(rmk.value()).has_value());
	EXPECT_FALSE(cg4->combine(text.value()).has_value());

	EXPECT_FALSE(cg5->combine(cg1.value()).has_value());
	EXPECT_FALSE(cg5->combine(cg2.value()).has_value());
	EXPECT_FALSE(cg5->combine(cg3.value()).has_value());
	EXPECT_FALSE(cg5->combine(cg4.value()).has_value());
	EXPECT_FALSE(cg5->combine(cg5.value()).has_value());
	EXPECT_FALSE(cg5->combine(cg6.value()).has_value());
	EXPECT_FALSE(cg5->combine(rmk.value()).has_value());
	EXPECT_FALSE(cg5->combine(text.value()).has_value());

	EXPECT_FALSE(cg6->combine(cg1.value()).has_value());
	EXPECT_FALSE(cg6->combine(cg2.value()).has_value());
	EXPECT_FALSE(cg6->combine(cg3.value()).has_value());
	EXPECT_FALSE(cg6->combine(cg4.value()).has_value());
	EXPECT_FALSE(cg6->combine(cg5.value()).has_value());
	EXPECT_FALSE(cg6->combine(cg6.value()).has_value());
	EXPECT_FALSE(cg6->combine(rmk.value()).has_value());
	EXPECT_FALSE(cg6->combine(text.value()).has_value());
}

TEST(CloudGroup, isValidClear) {
	const auto cg1 = metaf::CloudGroup::parse("CLR", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_TRUE(cg1->isValid());

	const auto cg2 = metaf::CloudGroup::parse("SKC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_TRUE(cg2->isValid());

	const auto cg3 = metaf::CloudGroup::parse("NCD", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_TRUE(cg3->isValid());

	const auto cg4 = metaf::CloudGroup::parse("NSC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_TRUE(cg4->isValid());
}

TEST(CloudGroup, isValidCloudTypes) {
	const auto cg1 = metaf::CloudGroup::parse("FEW040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_TRUE(cg1->isValid());

	const auto cg2 = metaf::CloudGroup::parse("SCT017", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_TRUE(cg2->isValid());

	const auto cg3 = metaf::CloudGroup::parse("BKN026", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_TRUE(cg3->isValid());

	const auto cg4 = metaf::CloudGroup::parse("OVC002", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_TRUE(cg4->isValid());

	const auto cg5 = metaf::CloudGroup::parse("SCT014TCU", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_TRUE(cg5->isValid());

	const auto cg6 = metaf::CloudGroup::parse("SCT014CB", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_TRUE(cg6->isValid());

	const auto cg7 = metaf::CloudGroup::parse("///014", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg7.has_value());
	EXPECT_TRUE(cg7->isValid());

	const auto cg8 = metaf::CloudGroup::parse("FEW///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg8.has_value());
	EXPECT_TRUE(cg8->isValid());

	const auto cg9 = metaf::CloudGroup::parse("SCT014///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg9.has_value());
	EXPECT_TRUE(cg9->isValid());

	const auto cg10 = metaf::CloudGroup::parse("BKN000", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg10.has_value());
	EXPECT_TRUE(cg10->isValid());
}

TEST(CloudGroup, isValidVertVisibility) {
	const auto cg1 = metaf::CloudGroup::parse("VV001", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_TRUE(cg1->isValid());

	const auto cg2 = metaf::CloudGroup::parse("VV000", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_TRUE(cg2->isValid());

	const auto cg3 = metaf::CloudGroup::parse("VV///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_TRUE(cg3->isValid());
}
