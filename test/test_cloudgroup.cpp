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
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT0040", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT40", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCC040", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT04A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040TCB", metaf::ReportPart::METAR).has_value());
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
