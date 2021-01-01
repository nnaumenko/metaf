/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
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
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerTaf) {
	const auto cg = metaf::CloudGroup::parse("SCT040", metaf::ReportPart::TAF);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 4000, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerFew) {
	const auto cg = metaf::CloudGroup::parse("FEW117", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::FEW);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 11700, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerScattered) {
	const auto cg = metaf::CloudGroup::parse("SCT370", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 37000, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerBroken) {
	const auto cg = metaf::CloudGroup::parse("BKN026", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::BROKEN);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 2600, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerOvercast) {
	const auto cg = metaf::CloudGroup::parse("OVC002", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OVERCAST);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 200, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerToweringCumulus) {
	const auto cg = metaf::CloudGroup::parse("SCT025TCU", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::TOWERING_CUMULUS);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerCumulonimbus) {
	const auto cg = metaf::CloudGroup::parse("BKN012CB", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::BROKEN);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 1200, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::CUMULONIMBUS);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerConvectiveTypeNotReported) {
	const auto cg = metaf::CloudGroup::parse("SCT016///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NOT_REPORTED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerAmountAndTypeNotReported) {
	const auto cg = metaf::CloudGroup::parse("///074///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	ASSERT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 7400, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NOT_REPORTED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerAmountAndHeightNotReported) {
	const auto cg = metaf::CloudGroup::parse("//////TCU", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::TOWERING_CUMULUS);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCloudLayerNotReported) {
	const auto cg = metaf::CloudGroup::parse("/////////", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CLOUD_LAYER);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NOT_REPORTED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
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
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseVerticalVisibilityTaf) {
	const auto cg = metaf::CloudGroup::parse("VV016", metaf::ReportPart::TAF);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VERTICAL_VISIBILITY);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	ASSERT_TRUE(cg->verticalVisibility().isReported());
	EXPECT_NEAR(cg->verticalVisibility().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg->verticalVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseVerticalVisibilityZero) {
	const auto cg = metaf::CloudGroup::parse("VV000", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VERTICAL_VISIBILITY);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	ASSERT_TRUE(cg->verticalVisibility().isReported());
	EXPECT_NEAR(cg->verticalVisibility().distance().value(), 0, heightMargin);
	EXPECT_EQ(cg->verticalVisibility().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseVerticalVisibilityNotReported) {
	const auto cg = metaf::CloudGroup::parse("VV///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VERTICAL_VISIBILITY);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
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
// Sky obscured
// Purpose: to confirm that remarks SKY OBSCURED and SKY OBSC are parsed and 
// appended correctly
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseSkyObscured) {
	auto cg = metaf::CloudGroup::parse("SKY", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("OBSCURED", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VERTICAL_VISIBILITY);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseSkyObsc) {
	auto cg = metaf::CloudGroup::parse("SKY", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("OBSC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VERTICAL_VISIBILITY);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OBSCURED);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseSkyObscuredWrongReportPart) {
	EXPECT_FALSE(metaf::CloudGroup::parse("SKY", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SKY", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SKY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("SKY", metaf::ReportPart::TAF).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Variable cloud cover
// Purpose: to confirm that variable cloud cover groups included in remarks are 
// parsed and appended correctly, and malformed variable cloud cover groups 
// cannot be parsed or appended 
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseVariableCloudCoverWithoutHeight) {
	auto cg1 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::FEW);
	EXPECT_EQ(cg1->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->variableAmount(), metaf::CloudGroup::Amount::SCATTERED);

	auto cg2 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::FEW);
	EXPECT_EQ(cg2->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->variableAmount(), metaf::CloudGroup::Amount::BROKEN);

	auto cg3 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg3->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg3->amount(), metaf::CloudGroup::Amount::FEW);
	EXPECT_EQ(cg3->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg3->height().isReported());
	EXPECT_EQ(cg3->variableAmount(), metaf::CloudGroup::Amount::OVERCAST);

	auto cg4 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("FEW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg4->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg4->amount(), metaf::CloudGroup::Amount::SCATTERED);
	EXPECT_EQ(cg4->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg4->height().isReported());
	EXPECT_EQ(cg4->variableAmount(), metaf::CloudGroup::Amount::FEW);

	auto cg5 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg5->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg5->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg5->amount(), metaf::CloudGroup::Amount::SCATTERED);
	EXPECT_EQ(cg5->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg5->height().isReported());
	EXPECT_EQ(cg5->variableAmount(), metaf::CloudGroup::Amount::BROKEN);

	auto cg6 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg6->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg6->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg6->amount(), metaf::CloudGroup::Amount::SCATTERED);
	EXPECT_EQ(cg6->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg6->height().isReported());
	EXPECT_EQ(cg6->variableAmount(), metaf::CloudGroup::Amount::OVERCAST);

	auto cg7 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg7.has_value());
	EXPECT_EQ(cg7->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg7->append("FEW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg7->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg7->amount(), metaf::CloudGroup::Amount::BROKEN);
	EXPECT_EQ(cg7->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg7->height().isReported());
	EXPECT_EQ(cg7->variableAmount(), metaf::CloudGroup::Amount::FEW);

	auto cg8 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg8.has_value());
	EXPECT_EQ(cg8->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg8->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg8->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg8->amount(), metaf::CloudGroup::Amount::BROKEN);
	EXPECT_EQ(cg8->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg8->height().isReported());
	EXPECT_EQ(cg8->variableAmount(), metaf::CloudGroup::Amount::SCATTERED);

	auto cg9 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg9.has_value());
	EXPECT_EQ(cg9->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg9->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg9->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg9->amount(), metaf::CloudGroup::Amount::BROKEN);
	EXPECT_EQ(cg9->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg9->height().isReported());
	EXPECT_EQ(cg9->variableAmount(), metaf::CloudGroup::Amount::OVERCAST);

	auto cg10 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg10.has_value());
	EXPECT_EQ(cg10->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg10->append("FEW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg10->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg10->amount(), metaf::CloudGroup::Amount::OVERCAST);
	EXPECT_EQ(cg10->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg10->height().isReported());
	EXPECT_EQ(cg10->variableAmount(), metaf::CloudGroup::Amount::FEW);

	auto cg11 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg11.has_value());
	EXPECT_EQ(cg11->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg11->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg11->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg11->amount(), metaf::CloudGroup::Amount::OVERCAST);
	EXPECT_EQ(cg11->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg11->height().isReported());
	EXPECT_EQ(cg11->variableAmount(), metaf::CloudGroup::Amount::SCATTERED);

	auto cg12 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg12.has_value());
	EXPECT_EQ(cg12->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg12->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg12->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg12->amount(), metaf::CloudGroup::Amount::OVERCAST);
	EXPECT_EQ(cg12->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg12->height().isReported());
	EXPECT_EQ(cg12->variableAmount(), metaf::CloudGroup::Amount::BROKEN);
}

TEST(CloudGroup, parseVariableCloudCoverWithHeight) {
	auto cg1 = metaf::CloudGroup::parse("FEW016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::FEW);
	ASSERT_TRUE(cg1->height().isReported());
	EXPECT_NEAR(cg1->height().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg1->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_EQ(cg1->variableAmount(), metaf::CloudGroup::Amount::SCATTERED);

	auto cg2 = metaf::CloudGroup::parse("OVC016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::OVERCAST);
	ASSERT_TRUE(cg2->height().isReported());
	EXPECT_NEAR(cg2->height().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg2->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg2->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_EQ(cg2->variableAmount(), metaf::CloudGroup::Amount::BROKEN);

	auto cg3 = metaf::CloudGroup::parse("BKN016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->type(), metaf::CloudGroup::Type::VARIABLE_COVER);
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg3->amount(), metaf::CloudGroup::Amount::BROKEN);
	ASSERT_TRUE(cg3->height().isReported());
	EXPECT_NEAR(cg3->height().distance().value(), 1600, heightMargin);
	EXPECT_EQ(cg3->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg3->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_EQ(cg3->variableAmount(), metaf::CloudGroup::Amount::OVERCAST);
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
	EXPECT_EQ(cg2->append("VV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("CLR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg4 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("SKC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg5 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg5->append("NSC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg6 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg6->append("NCD", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(CloudGroup, parseVariableCloudCoverWrongCombinationsSct) {
	auto cg1 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg2 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("VV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("CLR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg4 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("SKC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg5 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg5->append("NSC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg6 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg6->append("NCD", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(CloudGroup, parseVariableCloudCoverWrongCombinationsBkn) {
	auto cg1 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("VV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg4 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("CLR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg5 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg5->append("SKC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg6 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg6->append("NSC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg7 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg7.has_value());
	EXPECT_EQ(cg7->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg7->append("NCD", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(CloudGroup, parseVariableCloudCoverWrongCombinationsOvc) {
	auto cg1 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg2 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("VV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg3 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("CLR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg5 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg5->append("SKC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg6 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg6->append("NSC", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto cg7 = metaf::CloudGroup::parse("OVC", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg7.has_value());
	EXPECT_EQ(cg7->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg7->append("NCD", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
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
	static const auto expectedCloudType = metaf::CloudGroup::ConvectiveType::NONE;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->type(), expectedType);
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->convectiveType(), expectedCloudType);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	EXPECT_FALSE(cg1->runway().has_value());
	EXPECT_FALSE(cg1->direction().has_value());
	EXPECT_EQ(cg1->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->type(), expectedType);
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->convectiveType(), expectedCloudType);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	EXPECT_FALSE(cg2->runway().has_value());
	EXPECT_FALSE(cg2->direction().has_value());
	EXPECT_EQ(cg2->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseSkc) {
	static const std::string gs("SKC");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NONE_SKC;
	static const auto expectedType = metaf::CloudGroup::Type::NO_CLOUDS;
	static const auto expectedCloudType = metaf::CloudGroup::ConvectiveType::NONE;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->type(), expectedType);
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->convectiveType(), expectedCloudType);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	EXPECT_FALSE(cg1->runway().has_value());
	EXPECT_FALSE(cg1->direction().has_value());
	EXPECT_EQ(cg1->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->type(), expectedType);
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->convectiveType(), expectedCloudType);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	EXPECT_FALSE(cg2->runway().has_value());
	EXPECT_FALSE(cg2->direction().has_value());
	EXPECT_EQ(cg2->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseNcd) {
	static const std::string gs("NCD");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NCD;
	static const auto expectedType = metaf::CloudGroup::Type::NO_CLOUDS;
	static const auto expectedCloudType = metaf::CloudGroup::ConvectiveType::NONE;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->type(), expectedType);
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->convectiveType(), expectedCloudType);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	EXPECT_FALSE(cg1->runway().has_value());
	EXPECT_FALSE(cg1->direction().has_value());
	EXPECT_EQ(cg1->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->type(), expectedType);
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->convectiveType(), expectedCloudType);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	EXPECT_FALSE(cg2->runway().has_value());
	EXPECT_FALSE(cg2->direction().has_value());
	EXPECT_EQ(cg2->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseNsc) {
	static const std::string gs("NSC");
	static const auto expectedAmount = metaf::CloudGroup::Amount::NSC;
	static const auto expectedType = metaf::CloudGroup::Type::NO_CLOUDS;
	static const auto expectedCloudType = metaf::CloudGroup::ConvectiveType::NONE;

	const auto cg1 = metaf::CloudGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->type(), expectedType);
	EXPECT_EQ(cg1->amount(), expectedAmount);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->convectiveType(), expectedCloudType);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	EXPECT_FALSE(cg1->runway().has_value());
	EXPECT_FALSE(cg1->direction().has_value());
	EXPECT_EQ(cg1->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);

	const auto cg2 = metaf::CloudGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->type(), expectedType);
	EXPECT_EQ(cg2->amount(), expectedAmount);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->convectiveType(), expectedCloudType);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	EXPECT_FALSE(cg2->runway().has_value());
	EXPECT_FALSE(cg2->direction().has_value());
	EXPECT_EQ(cg2->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
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
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);

	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
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
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);

	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	ASSERT_TRUE(cg->direction().has_value());
	EXPECT_EQ(cg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);	
	EXPECT_EQ(cg->direction()->cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
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
	EXPECT_EQ(cg1->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);

	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	ASSERT_TRUE(cg1->runway().has_value());
	EXPECT_EQ(cg1->runway()->number(), 18u);
	EXPECT_EQ(cg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(cg1->direction().has_value());
	EXPECT_EQ(cg1->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);


	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("017", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("RWY24L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::CEILING);
	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	ASSERT_TRUE(cg2->height().isReported());
	EXPECT_NEAR(cg2->height().distance().value(), 1700, heightMargin);
	EXPECT_EQ(cg2->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(cg2->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);

	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	ASSERT_TRUE(cg2->runway().has_value());
	EXPECT_EQ(cg2->runway()->number(), 24u);
	EXPECT_EQ(cg2->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_FALSE(cg2->direction().has_value());
	EXPECT_EQ(cg2->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseVariableCeilingNoDetails) {
	auto cg = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("008V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::VARIABLE_CEILING);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
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
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseVariableCeilingRunway) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("008V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("R18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::VARIABLE_CEILING);
	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
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
	EXPECT_EQ(cg1->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("002V006", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("RWY24R", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::VARIABLE_CEILING);	
	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
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
	EXPECT_EQ(cg2->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
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
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
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
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
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
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseChinoRunway) {
	auto cg1 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("R18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::CHINO);
	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());
	EXPECT_FALSE(cg1->minHeight().isReported());
	EXPECT_FALSE(cg1->maxHeight().isReported());
	ASSERT_TRUE(cg1->runway().has_value());
	EXPECT_EQ(cg1->runway()->number(), 18u);
	EXPECT_EQ(cg1->runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_FALSE(cg1->direction().has_value());
	EXPECT_EQ(cg1->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);

	auto cg2 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("RWY24L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::CHINO);
	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());
	EXPECT_FALSE(cg2->minHeight().isReported());
	EXPECT_FALSE(cg2->maxHeight().isReported());
	ASSERT_TRUE(cg2->runway().has_value());
	EXPECT_EQ(cg2->runway()->number(), 24u);
	EXPECT_EQ(cg2->runway()->designator(), metaf::Runway::Designator::LEFT);
	EXPECT_FALSE(cg2->direction().has_value());
	EXPECT_EQ(cg2->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseChinoDirection) {
	auto cg = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CHINO);
	EXPECT_EQ(cg->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CHINO);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	ASSERT_TRUE(cg->direction().has_value());
	EXPECT_EQ(cg->direction()->type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(cg->direction()->cardinal(), metaf::Direction::Cardinal::SW);
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseChinoWrongReportPart) {
	EXPECT_FALSE(metaf::CloudGroup::parse("CHINO", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("CHINO", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("CHINO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("CHINO", metaf::ReportPart::TAF).has_value());
}

TEST(CloudGroup, chinoAppendToCompleteGroup) {
	auto cg1 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("CHINO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("RWY32", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("R12", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("002", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg1->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg2 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("RWY24L", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("CHINO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("RWY32", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("R12", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("002", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg2->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg3 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append("CHINO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append("RWY32", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append("R12", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append("002", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(cg3->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(CloudGroup, chinoAppendWrongGroup) {
	auto cg1 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("CHINO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg2 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("CHINO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg3 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("002", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg4 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("014V016", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg5 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("ALL", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg6 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("RWY", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto cg7 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg7.has_value());
	EXPECT_EQ(cg7->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
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
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
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

	auto fg5 = metaf::CloudGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg5.has_value());
	EXPECT_EQ(fg5->append("", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(CloudGroup, parseMisg) {
	EXPECT_FALSE(metaf::CloudGroup::parse("MISG", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("MISG", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("MISG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("MISG", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::CloudGroup::parse("MISG", metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Ground-based or aloft obscuration
// Purpose: to confirm that groups representing ground-based and aloft 
// obscurations (e.g. FG SCT000 or FU BKN020) are parsed and appended 
// correctly, and that malformed groups of this type cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseObscurationGroundBased) {
	auto cg = metaf::CloudGroup::parse("BLSN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("BKN000", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::BROKEN);

	EXPECT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 0, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);

	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);

	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::BLOWING_SNOW);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 0, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 7u);
}

TEST(CloudGroup, parseObscurationAloft) {
	auto cg = metaf::CloudGroup::parse("FU", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("BKN002", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::BROKEN);

	EXPECT_TRUE(cg->height().isReported());
	EXPECT_NEAR(cg->height().distance().value(), 200, heightMargin);
	EXPECT_EQ(cg->height().unit(), metaf::Distance::Unit::FEET);

	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);

	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::SMOKE);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 200, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 7u);
}

TEST(CloudGroup, parseObscurationFew) {
	auto cg = metaf::CloudGroup::parse("FG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("FEW000", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::FEW);

	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::FOG);
	EXPECT_EQ(ct->okta(), 2u);
}

TEST(CloudGroup, parseObscurationScattered) {
	auto cg = metaf::CloudGroup::parse("FG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("SCT000", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::SCATTERED);

	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::FOG);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 4u);
}

TEST(CloudGroup, parseObscurationBroken) {
	auto cg = metaf::CloudGroup::parse("FG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("BKN000", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::BROKEN);

	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::FOG);
	EXPECT_EQ(ct->okta(), 7u);
}

TEST(CloudGroup, parseObscurationOvercast) {
	auto cg = metaf::CloudGroup::parse("FG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("OVC000", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::OVERCAST);

	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::FOG);
	EXPECT_EQ(ct->okta(), 8u);
}

TEST(CloudGroup, parseObscurationBlowingSnow) {
	auto cg = metaf::CloudGroup::parse("BLSN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("BKN000", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::BLOWING_SNOW);
}

TEST(CloudGroup, parseObscurationBlowingDust) {
	auto cg = metaf::CloudGroup::parse("BLDU", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("BKN000", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::BLOWING_DUST);
}

TEST(CloudGroup, parseObscurationBlowingSand) {
	auto cg = metaf::CloudGroup::parse("BLSA", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("BKN000", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::BLOWING_SAND);
}

TEST(CloudGroup, parseObscurationFog) {
	auto cg = metaf::CloudGroup::parse("FG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("BKN000", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::FOG);
}

TEST(CloudGroup, parseObscurationSmoke) {
	auto cg = metaf::CloudGroup::parse("FU", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("BKN020", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::SMOKE);
}

TEST(CloudGroup, parseObscurationVolcanicAsh) {
	auto cg = metaf::CloudGroup::parse("VA", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("SCT085", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::OBSCURATION);
	const auto ct = cg->cloudType();
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::VOLCANIC_ASH);
}

///////////////////////////////////////////////////////////////////////////////
// Tests for CIG RAG and CIG DFUS
// Purpose: to confirm that groups CIG RAG AND CIG DFUS included in remarks 
// are parsed and appended correctly 
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, parseCigRag) {
	auto cg = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("RAG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CIG_RAG);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

TEST(CloudGroup, parseCigDfus) {
	auto cg = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("DFUS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(cg->type(), metaf::CloudGroup::Type::CIG_DFUS);
	EXPECT_EQ(cg->amount(), metaf::CloudGroup::Amount::NOT_REPORTED);
	EXPECT_FALSE(cg->height().isReported());
	EXPECT_EQ(cg->convectiveType(), metaf::CloudGroup::ConvectiveType::NONE);
	EXPECT_FALSE(cg->verticalVisibility().isReported());
	EXPECT_FALSE(cg->minHeight().isReported());
	EXPECT_FALSE(cg->maxHeight().isReported());
	EXPECT_FALSE(cg->runway().has_value());
	EXPECT_FALSE(cg->direction().has_value());
	EXPECT_EQ(cg->variableAmount(), metaf::CloudGroup::Amount::NOT_REPORTED);
}

// TODO: additional appending tests

///////////////////////////////////////////////////////////////////////////////
// Tests for cloudType()
// Purpose: to confirm that cloudType() method correctly initialises a
// CloudType instance 
// Note: except case where type() == CloudGroup::Type::OBSCURATION, which is
// tested separately
///////////////////////////////////////////////////////////////////////////////

TEST(CloudGroup, cloudTypeNoClouds) {
	auto cg1 = metaf::CloudGroup::parse("CLR", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_FALSE(cg1->cloudType().has_value());

	auto cg2 = metaf::CloudGroup::parse("SKC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_FALSE(cg2->cloudType().has_value());

	auto cg3 = metaf::CloudGroup::parse("NCD", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_FALSE(cg3->cloudType().has_value());

	auto cg4 = metaf::CloudGroup::parse("NSC", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_FALSE(cg4->cloudType().has_value());
}

TEST(CloudGroup, cloudTypeVerticalVisibility) {
	auto cg1 = metaf::CloudGroup::parse("VV002", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_FALSE(cg1->cloudType().has_value());

	auto cg2 = metaf::CloudGroup::parse("VV///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_FALSE(cg2->cloudType().has_value());
}

TEST(CloudGroup, cloudTypeChino) {
	auto cg1 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_FALSE(cg1->cloudType().has_value());

	auto cg2 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	EXPECT_EQ(cg2->append("R18", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(cg2->cloudType().has_value());

	auto cg3 = metaf::CloudGroup::parse("CHINO", metaf::ReportPart::RMK);
	EXPECT_EQ(cg3->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(cg3->cloudType().has_value());
}

TEST(CloudGroup, cloudTypeCldMisg) {
	auto cg = metaf::CloudGroup::parse("CLD", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(cg->cloudType().has_value());
}

TEST(CloudGroup, cloudTypeCloudLayer) {
	const auto cg1 = metaf::CloudGroup::parse("FEW117", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	const auto ct1 = cg1->cloudType();
	ASSERT_TRUE(ct1.has_value());

	EXPECT_EQ(ct1->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct1->height().isReported());
	EXPECT_EQ(ct1->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct1->height().distance().has_value());
	EXPECT_NEAR(ct1->height().distance().value(), 11700, heightMargin);
	EXPECT_EQ(ct1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct1->okta(), 2u);

	const auto cg2 = metaf::CloudGroup::parse("SCT040", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	const auto ct2 = cg2->cloudType();
	ASSERT_TRUE(ct2.has_value());

	EXPECT_EQ(ct2->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct2->height().isReported());
	EXPECT_EQ(ct2->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct2->height().distance().has_value());
	EXPECT_NEAR(ct2->height().distance().value(), 4000, heightMargin);
	EXPECT_EQ(ct2->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct2->okta(), 4u);

	const auto cg3 = metaf::CloudGroup::parse("BKN026", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	const auto ct3 = cg3->cloudType();
	ASSERT_TRUE(ct3.has_value());

	EXPECT_EQ(ct3->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct3->height().isReported());
	EXPECT_EQ(ct3->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct3->height().distance().has_value());
	EXPECT_NEAR(ct3->height().distance().value(), 2600, heightMargin);
	EXPECT_EQ(ct3->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct3->okta(), 7u);

	const auto cg4 = metaf::CloudGroup::parse("OVC002", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg4.has_value());
	const auto ct4 = cg4->cloudType();
	ASSERT_TRUE(ct4.has_value());

	EXPECT_EQ(ct4->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct4->height().isReported());
	EXPECT_EQ(ct4->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct4->height().distance().has_value());
	EXPECT_NEAR(ct4->height().distance().value(), 200, heightMargin);
	EXPECT_EQ(ct4->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct4->okta(), 8u);
}

TEST(CloudGroup, cloudTypeCloudLayerType) {
	const auto cg1 = metaf::CloudGroup::parse("SCT025TCU", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	const auto ct1 = cg1->cloudType();
	ASSERT_TRUE(ct1.has_value());

	EXPECT_EQ(ct1->type(), metaf::CloudType::Type::TOWERING_CUMULUS);
	EXPECT_TRUE(ct1->height().isReported());
	EXPECT_EQ(ct1->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct1->height().distance().has_value());
	EXPECT_NEAR(ct1->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(ct1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct1->okta(), 4u);

	const auto cg2 = metaf::CloudGroup::parse("BKN012CB", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	const auto ct2 = cg2->cloudType();
	ASSERT_TRUE(ct2.has_value());

	EXPECT_EQ(ct2->type(), metaf::CloudType::Type::CUMULONIMBUS);
	EXPECT_TRUE(ct2->height().isReported());
	EXPECT_EQ(ct2->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct2->height().distance().has_value());
	EXPECT_NEAR(ct2->height().distance().value(), 1200, heightMargin);
	EXPECT_EQ(ct2->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct2->okta(), 7u);
}

TEST(CloudGroup, cloudTypeCloudLayerNotReported) {
	const auto cg1 = metaf::CloudGroup::parse("///074", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg1.has_value());
	const auto ct1 = cg1->cloudType();
	ASSERT_TRUE(ct1.has_value());

	EXPECT_EQ(ct1->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct1->height().isReported());
	EXPECT_EQ(ct1->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct1->height().distance().has_value());
	EXPECT_NEAR(ct1->height().distance().value(), 7400, heightMargin);
	EXPECT_EQ(ct1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct1->okta(), 0u);

	const auto cg2 = metaf::CloudGroup::parse("FEW///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg2.has_value());
	const auto ct2 = cg2->cloudType();
	ASSERT_TRUE(ct2.has_value());

	EXPECT_EQ(ct2->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_FALSE(ct2->height().isReported());
	EXPECT_EQ(ct2->okta(), 2u);

	const auto cg3 = metaf::CloudGroup::parse("SCT016///", metaf::ReportPart::METAR);
	ASSERT_TRUE(cg3.has_value());
	const auto ct3 = cg3->cloudType();
	ASSERT_TRUE(ct3.has_value());

	EXPECT_EQ(ct3->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct3->height().isReported());
	EXPECT_EQ(ct3->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct3->height().distance().has_value());
	EXPECT_NEAR(ct3->height().distance().value(), 1600, heightMargin);
	EXPECT_EQ(ct3->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct3->okta(), 4u);
}

TEST(CloudGroup, cloudTypeCloudLayerVariable) {
	auto cg1 = metaf::CloudGroup::parse("FEW016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	const auto ct1 = cg1->cloudType();
	ASSERT_TRUE(ct1.has_value());

	EXPECT_EQ(ct1->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct1->height().isReported());
	EXPECT_EQ(ct1->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct1->height().distance().has_value());
	EXPECT_NEAR(ct1->height().distance().value(), 1600, heightMargin);
	EXPECT_EQ(ct1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct1->okta(), 4u);

	auto cg2 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	const auto ct2 = cg2->cloudType();
	ASSERT_TRUE(ct2.has_value());

	EXPECT_EQ(ct2->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_FALSE(ct2->height().isReported());
	EXPECT_EQ(ct2->okta(), 7u);

	auto cg3 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	const auto ct3 = cg3->cloudType();
	ASSERT_TRUE(ct3.has_value());

	EXPECT_EQ(ct3->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_FALSE(ct3->height().isReported());
	EXPECT_EQ(ct3->okta(), 8u);
}

TEST(CloudGroup, cloudTypeCeiling) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("025", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	const auto ct1 = cg1->cloudType();
	ASSERT_TRUE(ct1.has_value());

	EXPECT_EQ(ct1->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct1->height().isReported());
	EXPECT_EQ(ct1->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct1->height().distance().has_value());
	EXPECT_NEAR(ct1->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(ct1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct1->okta(), 0u);

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("025", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	const auto ct2 = cg2->cloudType();
	ASSERT_TRUE(ct2.has_value());

	EXPECT_EQ(ct2->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct2->height().isReported());
	EXPECT_EQ(ct2->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct2->height().distance().has_value());
	EXPECT_NEAR(ct2->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(ct2->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct2->okta(), 0u);

	auto cg3 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("025", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	const auto ct3 = cg3->cloudType();
	ASSERT_TRUE(ct3.has_value());

	EXPECT_EQ(ct3->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct3->height().isReported());
	EXPECT_EQ(ct3->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct3->height().distance().has_value());
	EXPECT_NEAR(ct3->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(ct3->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct3->okta(), 0u);
}

TEST(CloudGroup, cloudTypeCeilingVariable) {
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("012V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	const auto ct1 = cg1->cloudType();
	ASSERT_TRUE(ct1.has_value());

	EXPECT_EQ(ct1->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct1->height().isReported());
	EXPECT_EQ(ct1->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct1->height().distance().has_value());
	EXPECT_NEAR(ct1->height().distance().value(), 1200, heightMargin);
	EXPECT_EQ(ct1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct1->okta(), 0u);

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("012V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("RWY36", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	const auto ct2 = cg2->cloudType();
	ASSERT_TRUE(ct2.has_value());

	EXPECT_EQ(ct2->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct2->height().isReported());
	EXPECT_EQ(ct2->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct2->height().distance().has_value());
	EXPECT_NEAR(ct2->height().distance().value(), 1200, heightMargin);
	EXPECT_EQ(ct2->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct2->okta(), 0u);

	auto cg3 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("012V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	const auto ct3 = cg3->cloudType();
	ASSERT_TRUE(ct3.has_value());

	EXPECT_EQ(ct3->type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_TRUE(ct3->height().isReported());
	EXPECT_EQ(ct3->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct3->height().distance().has_value());
	EXPECT_NEAR(ct3->height().distance().value(), 1200, heightMargin);
	EXPECT_EQ(ct3->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct3->okta(), 0u);
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
	auto cg1 = metaf::CloudGroup::parse("FEW016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg1->isValid());

	auto cg2 = metaf::CloudGroup::parse("SCT016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg2->isValid());

	auto cg3 = metaf::CloudGroup::parse("BKN016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg3->isValid());

	auto cg4 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_EQ(cg4->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg4->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg4->isValid());

	auto cg5 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg5.has_value());
	EXPECT_EQ(cg5->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg5->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg5->isValid());

	auto cg6 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg6.has_value());
	EXPECT_EQ(cg6->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg6->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg6->isValid());
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
	auto cg1 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	EXPECT_EQ(cg1->append("025", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg1->isValid());

	auto cg2 = metaf::CloudGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("012V014", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg2->isValid());
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

TEST(CloudGroup, isValidCldMisg) {
	auto cg = metaf::CloudGroup::parse("CLD", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg.has_value());
	EXPECT_EQ(cg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(cg->isValid());
}

// TODO: isValid for SKY OBSCURED