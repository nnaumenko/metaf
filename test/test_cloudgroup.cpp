/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

const auto heightMargin = 1.0 / 2;

///////////////////////////////////////////////////////////////////////////////
// Cloud layer
// Purpose: to confirm that cloud layer groups are parsed correctly, malformed
// cloud layer groups cannot be parsed, and cloud layer groups cannot be 
// appended to each other
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseCloudLayerMetar) {
	const auto cg = metaf::CloudGroup::parse("SCT040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 4000, heightMargin);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerTaf) {
	const auto cg = metaf::CloudGroup::parse("SCT040", metaf::ReportPart::TAF);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 4000, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerFew) {
	const auto cg = metaf::CloudGroup::parse("FEW117", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::FEW);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 11700, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerScattered) {
	const auto cg = metaf::CloudGroup::parse("SCT370", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 37000, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerBroken) {
	const auto cg = metaf::CloudGroup::parse("BKN026", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::BROKEN);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 2600, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerOvercast) {
	const auto cg = metaf::CloudGroup::parse("OVC002", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OVERCAST);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 200, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerToweringCumulus) {
	const auto cg = metaf::CloudGroup::parse("SCT025TCU", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::TOWERING_CUMULUS);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerCumulonimbus) {
	const auto cg = metaf::CloudGroup::parse("BKN012CB", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::BROKEN);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 1200, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::CUMULONIMBUS);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerConvectiveTypeNotReported) {
	const auto cg = metaf::CloudGroup::parse("SCT016///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NOT_REPORTED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerAmountAndTypeNotReported) {
	const auto cg = metaf::CloudGroup::parse("///074///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 7400, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NOT_REPORTED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerAmountAndHeightNotReported) {
	const auto cg = metaf::CloudGroup::parse("//////TCU", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::TOWERING_CUMULUS);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerNotReported) {
	const auto cg = metaf::CloudGroup::parse("/////////", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NOT_REPORTED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCloudLayerWrongReportPart) {
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040", metaf::ReportPart::HEADER).has_value());
}

TEST(CloudGroup, parseCloudLayerWrongFormat) {
	EXPECT_FALSE(metaf::CloudGroup::parse("AAA040", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("BCC040", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT", metaf::ReportPart::METAR).has_value());
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

TEST(CloudGroup, appendNonVariableCloudLayer) {
	const metaf::ReportPart metarRp = metaf::ReportPart::METAR;

	const std::string cgStr1 ("CLR"); 
	const std::string cgStr2 ("SKC"); 
	const std::string cgStr3 ("NCD"); 
	const std::string cgStr4 ("NSC"); 
	const std::string cgStr5 ("FEW040"); 
	const std::string cgStr6 ("VV004");
	const std::string cgStr7 ("SCT020TCU");
	const std::string cgStr8 ("SCT020CB");
	const std::string tStr1 ("RMK");
	const std::string tStr2 ("TEST"); 

	auto cg1 = metaf::CloudGroup::parse(cgStr1, metarRp);
	ASSERT_TRUE(cg1.has_value());

	auto cg2 = metaf::CloudGroup::parse(cgStr2, metarRp);
	ASSERT_TRUE(cg2.has_value());

	auto cg3 = metaf::CloudGroup::parse(cgStr3, metarRp);
	ASSERT_TRUE(cg3.has_value());

	auto cg4 = metaf::CloudGroup::parse(cgStr4, metarRp);
	ASSERT_TRUE(cg4.has_value());

	auto cg5 = metaf::CloudGroup::parse(cgStr5, metarRp);
	ASSERT_TRUE(cg5.has_value());

	auto cg6 = metaf::CloudGroup::parse(cgStr6, metarRp);
	ASSERT_TRUE(cg6.has_value());

	auto cg7 = metaf::CloudGroup::parse(cgStr7, metarRp);
	ASSERT_TRUE(cg7.has_value());

	auto cg8 = metaf::CloudGroup::parse(cgStr8, metarRp);
	ASSERT_TRUE(cg8.has_value());

	EXPECT_EQ(cg1->append(cgStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append(cgStr2, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append(cgStr3, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append(cgStr4, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append(cgStr5, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append(cgStr6, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append(cgStr7, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append(cgStr8, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append(tStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append(tStr2, metarRp), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg2->append(cgStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append(cgStr2, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append(cgStr3, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append(cgStr4, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append(cgStr5, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append(cgStr6, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append(cgStr7, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append(cgStr8, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append(tStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append(tStr2, metarRp), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg3->append(cgStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append(cgStr2, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append(cgStr3, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append(cgStr4, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append(cgStr5, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append(cgStr6, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append(cgStr7, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append(cgStr8, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append(tStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append(tStr2, metarRp), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg4->append(cgStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg4->append(cgStr2, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg4->append(cgStr3, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg4->append(cgStr4, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg4->append(cgStr5, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg4->append(cgStr6, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg4->append(cgStr7, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg4->append(cgStr8, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg4->append(tStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg4->append(tStr2, metarRp), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg5->append(cgStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg5->append(cgStr2, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg5->append(cgStr3, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg5->append(cgStr4, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg5->append(cgStr5, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg5->append(cgStr6, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg5->append(cgStr7, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg5->append(cgStr8, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg5->append(tStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg5->append(tStr2, metarRp), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg6->append(cgStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg6->append(cgStr2, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg6->append(cgStr3, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg6->append(cgStr4, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg6->append(cgStr5, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg6->append(cgStr6, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg6->append(cgStr7, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg6->append(cgStr8, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg6->append(tStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg6->append(tStr2, metarRp), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg7->append(cgStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg7->append(cgStr2, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg7->append(cgStr3, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg7->append(cgStr4, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg7->append(cgStr5, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg7->append(cgStr6, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg7->append(cgStr7, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg7->append(cgStr8, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg7->append(tStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg7->append(tStr2, metarRp), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg8->append(cgStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg8->append(cgStr2, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg8->append(cgStr3, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg8->append(cgStr4, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg8->append(cgStr5, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg8->append(cgStr6, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg8->append(cgStr7, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg8->append(cgStr8, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg8->append(tStr1, metarRp), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg8->append(tStr2, metarRp), metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// Vertical visibility
// Purpose: to confirm that vertical visibility groups are parsed correctly, 
// malformed vertical visibility groups cannot be parsed, and vertical 
// visibility groups cannot be appended to each other
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseVerticalVisibilityMetar) {
	const auto cg = metaf::CloudGroup::parse("VV016", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VERTICAL_VISIBILITY);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	ASSERT_TRUE(cg->verticalVisibility().isReported());
	EXPECT_NEAR(cg->verticalVisibility().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg->verticalVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseVerticalVisibilityTaf) {
	const auto cg = metaf::CloudGroup::parse("VV016", metaf::ReportPart::TAF);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VERTICAL_VISIBILITY);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	ASSERT_TRUE(cg->verticalVisibility().isReported());
	EXPECT_NEAR(cg->verticalVisibility().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg->verticalVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseVerticalVisibilityZero) {
	const auto cg = metaf::CloudGroup::parse("VV000", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VERTICAL_VISIBILITY);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	ASSERT_TRUE(cg->verticalVisibility().isReported());
	EXPECT_NEAR(cg->verticalVisibility().distance().value(), 0, heightMargin);
	EXPECT_EQ(cg->verticalVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseVerticalVisibilityNotReported) {
	const auto cg = metaf::CloudGroup::parse("VV///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VERTICAL_VISIBILITY);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
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

///////////////////////////////////////////////////////////////////////////////
// Variable cloud cover
// Purpose: to confirm that variable cloud cover groups included in remarks are 
// parsed and appended correctly, and malformed variable cloud cover groups 
// cannot be parsed or appended 
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseVariableCloudCoverWithHeight) {
	auto cg1 = metaf::CloudGroup::parse("FEW016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::VARIABLE_FEW_SCATTERED);
	ASSERT_TRUE(cg1->height().isReported());
	EXPECT_NEAR(cg1->height().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg1->cloudType(), metaf::CloudGroup::CloudType::NONE);

	auto cg2 = metaf::CloudGroup::parse("SCT016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::VARIABLE_SCATTERED_BROKEN);
	ASSERT_TRUE(cg2->height().isReported());
	EXPECT_NEAR(cg2->height().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg2->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg2->cloudType(), metaf::CloudGroup::CloudType::NONE);

	auto cg3 = metaf::CloudGroup::parse("BKN016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg3->amount(), metaf::CloudGroup::Amount::VARIABLE_BROKEN_OVERCAST);
	ASSERT_TRUE(cg3->height().isReported());
	EXPECT_NEAR(cg3->height().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg3->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg3->cloudType(), metaf::CloudGroup::CloudType::NONE);
}

TEST(CloudGroup, parseVariableCloudCoverWithoutHeight) {
	auto cg1 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::VARIABLE_FEW_SCATTERED);
	EXPECT_EQ(cg1->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg1->height().isReported());

	auto cg2 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::VARIABLE_SCATTERED_BROKEN);
	EXPECT_EQ(cg2->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg2->height().isReported());

	auto cg3 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg3->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg3->amount(), metaf::CloudGroup::Amount::VARIABLE_BROKEN_OVERCAST);
	EXPECT_EQ(cg3->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg3->height().isReported());
}

TEST(CloudGroup, parseVariableCloudCoverWrongReportPart) {
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT", metaf::ReportPart::HEADER).has_value());	
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT", metaf::ReportPart::TAF).has_value());	
}

TEST(CloudGroup, parseVariableCloudCoverWrongFormatFirstCover) {
	EXPECT_FALSE(metaf::CloudGroup::parse("CLR", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SKC", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("NCD", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("NSC", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("AAA", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("AAA040", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("BCC040", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("///040", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("FEW///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("FEW040///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("FEW040TCU", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("FEW040CB", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT0040", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT40", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCC040", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT04A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040TCB", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040CCU", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040////", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT040//", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("////040", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("//040", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT////", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SCT//", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV002", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("VV", metaf::ReportPart::RMK).has_value());
}

TEST(CloudGroup, parseVariableCloudCoverWrongFormatV) {
	auto cg1 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("A", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg2 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("BKN010", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg4 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("///", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg5 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("VV002", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg6 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("VV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(CloudGroup, parseVariableCloudCoverWrongFormatSecondCover) {
	auto cg1 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg2 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("AAA", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("SCT010", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg4 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("///", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(CloudGroup, parseVariableCloudCoverWrongCombinationsFew) {
	auto cg1 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("FEW", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg2 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg4 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("VV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg5 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg5->append("CLR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg6 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg6->append("SKC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg7 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg7.has_value());
	EXPECT_EQ(cg7->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg7->append("NSC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg8 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg8.has_value());
	EXPECT_EQ(cg8->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg8->append("NCD", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(CloudGroup, parseVariableCloudCoverWrongCombinationsSct) {
	auto cg1 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg2 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("FEW", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg4 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("VV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg5 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg5->append("CLR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg6 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg6->append("SKC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg7 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg7.has_value());
	EXPECT_EQ(cg7->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg7->append("NSC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg8 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg8.has_value());
	EXPECT_EQ(cg8->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg8->append("NCD", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(CloudGroup, parseVariableCloudCoverWrongCombinationsBkn) {
	auto cg1 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg2 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("FEW", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg4 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("VV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg5 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg5->append("CLR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg6 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg6->append("SKC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg7 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg7.has_value());
	EXPECT_EQ(cg7->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg7->append("NSC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg8 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg8.has_value());
	EXPECT_EQ(cg8->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg8->append("NCD", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(CloudGroup, parseVariableCloudCoverWrongCombinationsOvc) {
	auto cg1 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg2 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("FEW", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg4 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("VV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg5 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg5->append("CLR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg6 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg6->append("SKC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg7 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg7.has_value());
	EXPECT_EQ(cg7->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg7->append("NSC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg8 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg8.has_value());
	EXPECT_EQ(cg8->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg8->append("NCD", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg9 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg9.has_value());
	EXPECT_EQ(cg9->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg9->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

///////////////////////////////////////////////////////////////////////////////
// 'No cloud' conditions: CLR, SKC, NCD, NSC
// Purpose: to confirm that groups indicating nil or non-significant clouds are 
// parsed correctly, malformed groups of this type cannot be parsed, and 
// groups of this type cannot be appended to each other
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseClr) {
	static const std::string gs("CLR");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NONE_CLR;
	static const auto expectedType = metaf::CloudGroup::Type::NO_CLOUDS;
	static const auto expectedCloudType = metaf::CloudGroup::CloudType::NONE;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->type(), expectedType);
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->cloudType(), expectedCloudType);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	EXPECT_FALSE(cg1->runway().has_value());
	EXPECT_FALSE(cg1->direction().has_value());

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->type(), expectedType);
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->cloudType(), expectedCloudType);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	EXPECT_FALSE(cg2->runway().has_value());
	EXPECT_FALSE(cg2->direction().has_value());
}

TEST(CloudGroup, parseSkc) {
	static const std::string gs("SKC");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NONE_SKC;
	static const auto expectedType = metaf::CloudGroup::Type::NO_CLOUDS;
	static const auto expectedCloudType = metaf::CloudGroup::CloudType::NONE;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->type(), expectedType);
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->cloudType(), expectedCloudType);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	EXPECT_FALSE(cg1->runway().has_value());
	EXPECT_FALSE(cg1->direction().has_value());

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->type(), expectedType);
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->cloudType(), expectedCloudType);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	EXPECT_FALSE(cg2->runway().has_value());
	EXPECT_FALSE(cg2->direction().has_value());
}

TEST(CloudGroup, parseNcd) {
	static const std::string gs("NCD");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NCD;
	static const auto expectedType = metaf::CloudGroup::Type::NO_CLOUDS;
	static const auto expectedCloudType = metaf::CloudGroup::CloudType::NONE;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->type(), expectedType);
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->cloudType(), expectedCloudType);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	EXPECT_FALSE(cg1->runway().has_value());
	EXPECT_FALSE(cg1->direction().has_value());

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->type(), expectedType);
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->cloudType(), expectedCloudType);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	EXPECT_FALSE(cg2->runway().has_value());
	EXPECT_FALSE(cg2->direction().has_value());
}

TEST(CloudGroup, parseNsc) {
	static const std::string gs("NSC");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NSC;
	static const auto expectedType = metaf::CloudGroup::Type::NO_CLOUDS;
	static const auto expectedCloudType = metaf::CloudGroup::CloudType::NONE;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->type(), expectedType);
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->cloudType(), expectedCloudType);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	EXPECT_FALSE(cg1->runway().has_value());
	EXPECT_FALSE(cg1->direction().has_value());

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->type(), expectedType);
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->cloudType(), expectedCloudType);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	EXPECT_FALSE(cg2->runway().has_value());
	EXPECT_FALSE(cg2->direction().has_value());
}

TEST(CloudGroup, parseNoCloudWrongReportPart) {
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

///////////////////////////////////////////////////////////////////////////////
// Ceiling Height and Variable Ceiling Height
// Purpose: to confirm that ceiling height groups included in remarks are 
// parsed and appended correctly, and malformed ceiling height groups cannot be 
// parsed or appended 
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseCeilingNoDetails) {
	auto cg = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("025", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CEILING);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);

	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCeilingDirection) {
	auto cg = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("025", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->append("NE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CEILING);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);

	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	ASSERT_TRUE(cg->direction().has_value());
	EXPECT_EQ(cg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);	
	EXPECT_EQ(cg->direction()->cardinal(), metaf::Direction::Cardinal::NE);
}

TEST(CloudGroup, parseCeilingRunway) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("025", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("R18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::CEILING);
	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	ASSERT_TRUE(cg1->height().isReported());
	EXPECT_NEAR(cg1->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(cg1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg1->cloudType(), metaf::CloudGroup::CloudType::NONE);

	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	ASSERT_TRUE(cg1->runway().has_value());
	EXPECT_EQ(cg1->runway()->number(), 18u);
	EXPECT_EQ(cg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(cg1->direction().has_value());


	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("017", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("RWY24L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::CEILING);
	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	ASSERT_TRUE(cg2->height().isReported());
	EXPECT_NEAR(cg2->height().distance().value(), 1700, heightMargin);
	EXPECT_EQ(cg2->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg2->cloudType(), metaf::CloudGroup::CloudType::NONE);

	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	ASSERT_TRUE(cg2->runway().has_value());
	EXPECT_EQ(cg2->runway()->number(), 24u);
	EXPECT_EQ(cg2->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_FALSE(cg2->direction().has_value());
}

TEST(CloudGroup, parseVariableCeilingNoDetails) {
	auto cg = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("008V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VARIABLE_CEILING);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	ASSERT_TRUE(cg->minHeight().isReported());
	EXPECT_NEAR(cg->minHeight().distance().value(), 800, heightMargin);
	EXPECT_EQ(cg->minHeight().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->minHeight().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(cg->maxHeight().isReported());
	EXPECT_NEAR(cg->maxHeight().distance().value(), 1400, heightMargin);
	EXPECT_EQ(cg->maxHeight().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->maxHeight().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseVariableCeilingRunway) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("008V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("R18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::VARIABLE_CEILING);
	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	ASSERT_TRUE(cg1->minHeight().isReported());
	EXPECT_NEAR(cg1->minHeight().distance().value(), 800, heightMargin);
	EXPECT_EQ(cg1->minHeight().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg1->minHeight().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(cg1->maxHeight().isReported());
	EXPECT_NEAR(cg1->maxHeight().distance().value(), 1400, heightMargin);
	EXPECT_EQ(cg1->maxHeight().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg1->maxHeight().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(cg1->runway().has_value());
	EXPECT_EQ(cg1->runway()->number(), 18u);
	EXPECT_EQ(cg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(cg1->direction().has_value());

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("002V006", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("RWY24R", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::VARIABLE_CEILING);	
	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	ASSERT_TRUE(cg2->minHeight().isReported());
	EXPECT_NEAR(cg2->minHeight().distance().value(), 200, heightMargin);
	EXPECT_EQ(cg2->minHeight().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg2->minHeight().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(cg2->maxHeight().isReported());
	EXPECT_NEAR(cg2->maxHeight().distance().value(), 600, heightMargin);
	EXPECT_EQ(cg2->maxHeight().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg2->maxHeight().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(cg2->runway().has_value());
	EXPECT_EQ(cg2->runway()->number(), 24u);
	EXPECT_EQ(cg2->runway()->designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_FALSE(cg2->direction().has_value());
}

TEST(CloudGroup, parseVariableCeilingDirection) {
	auto cg = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("008V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->append("NE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VARIABLE_CEILING);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	ASSERT_TRUE(cg->minHeight().isReported());
	EXPECT_NEAR(cg->minHeight().distance().value(), 800, heightMargin);
	EXPECT_EQ(cg->minHeight().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->minHeight().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(cg->maxHeight().isReported());
	EXPECT_NEAR(cg->maxHeight().distance().value(), 1400, heightMargin);
	EXPECT_EQ(cg->maxHeight().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->maxHeight().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(cg->runway().has_value());
	ASSERT_TRUE(cg->direction().has_value());
	EXPECT_EQ(cg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);	
	EXPECT_EQ(cg->direction()->cardinal(), metaf::Direction::Cardinal::NE);	
}


TEST(CloudGroup, ceilingAppendWrongGroupCig) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("R22", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("///", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("///V///", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(CloudGroup, ceilingAppendWrongGroupCigValue) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("002", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("004", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("002", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("X32", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg3 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("002", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg4 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("002", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(CloudGroup, ceilingAppendWrongGroupCigVariable) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("004", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("X32", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg3 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg4 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(CloudGroup, ceilingAppendToCompleteGroups) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("002", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("RWY32", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("RWY12", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("002", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("RWY32", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("RWY12", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("002", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(CloudGroup, parseCeilingWrongReportPart) {
	EXPECT_FALSE(metaf::CloudGroup::parse("CIG", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("CIG", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("CIG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("CIG", metaf::ReportPart::TAF).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// CHINO 
// Purpose: to confirm that CHINO groups included in remarks are parsed and 
// appended correctly, and malformed CHINO groups cannot be parsed or appended 
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseChinoNoDetails) {
	auto cg = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CHINO);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseChinoRunway) {
	auto cg1 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	EXPECT_EQ(cg1->append("R18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_TRUE(cg1.has_value());

	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::CHINO);
	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	ASSERT_TRUE(cg1->runway().has_value());
	EXPECT_EQ(cg1->runway()->number(), 18u);
	EXPECT_EQ(cg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(cg1->direction().has_value());

	auto cg2 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());

	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::CHINO);
	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	EXPECT_EQ(cg2->append("RWY24L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_TRUE(cg2->runway().has_value());
	EXPECT_EQ(cg2->runway()->number(), 24u);
	EXPECT_EQ(cg2->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_FALSE(cg2->direction().has_value());
}

TEST(CloudGroup, parseChinoDirection) {
	auto cg = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CHINO);
	EXPECT_EQ(cg->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CHINO);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	ASSERT_TRUE(cg->direction().has_value());
	EXPECT_EQ(cg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(cg->direction()->cardinal(), metaf::Direction::Cardinal::SW);
}

TEST(CloudGroup, parseChinoWrongReportPart) {
	EXPECT_FALSE(metaf::CloudGroup::parse("CHINO", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("CHINO", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("CHINO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("CHINO", metaf::ReportPart::TAF).has_value());
}

TEST(CloudGroup, chinoAppendToCompleteGroup) {
	// TODO
}

TEST(CloudGroup, chinoAppendWrongGroup) {
	// TODO
}

///////////////////////////////////////////////////////////////////////////////
// Tests for CLD MISG
// Purpose: to confirm that CLD MISG group included in remarks is parsed and 
// appended correctly 
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseCldMisg) {
	auto cg = metaf::CloudGroup::parse("CLD", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLD_MISG);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->cloudType(), metaf::CloudGroup::CloudType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
}

TEST(CloudGroup, parseCldAndOther) {
	static const char gs[] = "CLD";

	auto fg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::CloudGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::CloudGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

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

TEST(CloudGroup, isValidNoClouds) {
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

TEST(CloudGroup, isValidVariableCloudLayer) {
	//TODO
}

TEST(CloudGroup, isValidCeilingValidRunway) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("025", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg1->isValid());

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("012V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg2->isValid());
}

TEST(CloudGroup, isValidCeilingWrongRunway) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("025", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(cg1->isValid());

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("012V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(cg2->isValid());
}

TEST(CloudGroup, isValidCeilingNoDetails) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("025", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_TRUE(cg1->isValid());

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("012V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_TRUE(cg2->isValid());
}

TEST(CloudGroup, isValidCeilingDirection) {
	//TODO
}

TEST(CloudGroup, isValidChinoValidRunway) {
	auto cg = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg->isValid());
}

TEST(CloudGroup, isValidChinoWrongRunway) {
	auto cg = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("RWY37", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(cg->isValid());
}

TEST(CloudGroup, isValidChinoNoDetails) {
	auto cg = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_TRUE(cg->isValid());
}

TEST(CloudGroup, isValidChinoDirection) {
	auto cg = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg->isValid());
}

TEST(CloudGroup, isValidCld) {
	//TODO
}

TEST(CloudGroup, isValidCldMisg) {
	//TODO
}

