/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

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

TEST(CloudGroup, parseVariableCloudCoverWithHeight) {
	auto cg1 = metaf::CloudGroup::parse("FEW016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::VARIABLE_FEW_SCATTERED);
	EXPECT_EQ(cg1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg1->height().isInteger());
	ASSERT_TRUE(cg1->height().integer().has_value());
	EXPECT_EQ(cg1->height().integer().value(), 1600u);
	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());

	auto cg2 = metaf::CloudGroup::parse("SCT016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::VARIABLE_SCATTERED_BROKEN);
	EXPECT_EQ(cg2->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg2->height().isInteger());
	ASSERT_TRUE(cg2->height().integer().has_value());
	EXPECT_EQ(cg2->height().integer().value(), 1600u);
	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());

	auto cg3 = metaf::CloudGroup::parse("BKN016", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->amount(), metaf::CloudGroup::Amount::VARIABLE_BROKEN_OVERCAST);
	EXPECT_EQ(cg3->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(cg3->height().isInteger());
	ASSERT_TRUE(cg3->height().integer().has_value());
	EXPECT_EQ(cg3->height().integer().value(), 1600u);
	EXPECT_EQ(cg3->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg3->verticalVisibility().isReported());
}

TEST(CloudGroup, parseVariableCloudCoverWithoutHeight) {
	auto cg1 = metaf::CloudGroup::parse("FEW", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg1.has_value());
	EXPECT_EQ(cg1->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->append("SCT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg1->amount(), metaf::CloudGroup::Amount::VARIABLE_FEW_SCATTERED);
	EXPECT_FALSE(cg1->height().isReported());
	EXPECT_EQ(cg1->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg1->verticalVisibility().isReported());

	auto cg2 = metaf::CloudGroup::parse("SCT", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_EQ(cg2->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->append("BKN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg2->amount(), metaf::CloudGroup::Amount::VARIABLE_SCATTERED_BROKEN);
	EXPECT_FALSE(cg2->height().isReported());
	EXPECT_EQ(cg2->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg2->verticalVisibility().isReported());

	auto cg3 = metaf::CloudGroup::parse("BKN", metaf::ReportPart::RMK);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_EQ(cg3->append("V", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->append("OVC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(cg3->amount(), metaf::CloudGroup::Amount::VARIABLE_BROKEN_OVERCAST);
	EXPECT_FALSE(cg3->height().isReported());
	EXPECT_EQ(cg3->type(), metaf::CloudGroup::Type::NONE);
	EXPECT_FALSE(cg3->verticalVisibility().isReported());
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

TEST(CloudGroup, append) {
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
	EXPECT_FALSE(cg1->isCloudLayer());

	auto cg2 = metaf::CloudGroup::parse(cgStr2, metarRp);
	ASSERT_TRUE(cg2.has_value());
	EXPECT_FALSE(cg2->isCloudLayer());

	auto cg3 = metaf::CloudGroup::parse(cgStr3, metarRp);
	ASSERT_TRUE(cg3.has_value());
	EXPECT_FALSE(cg3->isCloudLayer());

	auto cg4 = metaf::CloudGroup::parse(cgStr4, metarRp);
	ASSERT_TRUE(cg4.has_value());
	EXPECT_FALSE(cg4->isCloudLayer());

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

TEST(CloudGroup, isValidVariable) {
	//TODO
}
