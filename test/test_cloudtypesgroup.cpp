/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto heightMargin = 1.0 / 2;


///////////////////////////////////////////////////////////////////////////////
// Parse multiple cloud types / obscurations in single group
// Purpose: to confirm that group with format which contains multiple cloud
// layer or obscuration types with their octa (e.g. BLSN1SC1SC1CI3) are parsed
// correctly and malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(CloudTypesGroup, parseFormatWithoutHeightSingleLayer) {
	const auto ctg = metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->cloudTypes();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(ctv[0].type(), metaf::CloudType::Type::CUMULONIMBUS);
	EXPECT_FALSE(ctv[0].height().isReported());
	EXPECT_EQ(ctv[0].okta(), 5u);
}

TEST(CloudTypesGroup, parseFormatWithoutHeightObscurationAndCloudLayer) {
	const auto ctg = metaf::CloudTypesGroup::parse("IC1SC3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->cloudTypes();
	ASSERT_EQ(ctv.size(), 2u);
	EXPECT_EQ(ctv[0].type(), metaf::CloudType::Type::ICE_CRYSTALS);
	EXPECT_FALSE(ctv[0].height().isReported());
	EXPECT_EQ(ctv[0].okta(), 1u);
	EXPECT_EQ(ctv[1].type(), metaf::CloudType::Type::STRATOCUMULUS);
	EXPECT_FALSE(ctv[1].height().isReported());
	EXPECT_EQ(ctv[1].okta(), 3u);
}

TEST(CloudTypesGroup, parseFormatWithoutHeightThreeLayers) {
	const auto ctg = metaf::CloudTypesGroup::parse("SC1AC6CI1", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->cloudTypes();
	ASSERT_EQ(ctv.size(), 3u);
	EXPECT_EQ(ctv[0].type(), metaf::CloudType::Type::STRATOCUMULUS);
	EXPECT_FALSE(ctv[0].height().isReported());
	EXPECT_EQ(ctv[0].okta(), 1u);
	EXPECT_EQ(ctv[1].type(), metaf::CloudType::Type::ALTOCUMULUS);
	EXPECT_FALSE(ctv[1].height().isReported());
	EXPECT_EQ(ctv[1].okta(), 6u);
	EXPECT_EQ(ctv[2].type(), metaf::CloudType::Type::CIRRUS);
	EXPECT_FALSE(ctv[2].height().isReported());
	EXPECT_EQ(ctv[2].okta(), 1u);
}

TEST(CloudTypesGroup, parseFormatWithoutHeightThreeLayersAndObscuration) {
	const auto ctg = metaf::CloudTypesGroup::parse("BLSN1SC1SC1CI3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->cloudTypes();
	ASSERT_EQ(ctv.size(), 4u);
	EXPECT_EQ(ctv[0].type(), metaf::CloudType::Type::BLOWING_SNOW);
	EXPECT_FALSE(ctv[0].height().isReported());
	EXPECT_EQ(ctv[0].okta(), 1u);
	EXPECT_EQ(ctv[1].type(), metaf::CloudType::Type::STRATOCUMULUS);
	EXPECT_FALSE(ctv[1].height().isReported());
	EXPECT_EQ(ctv[1].okta(), 1u);
	EXPECT_EQ(ctv[2].type(), metaf::CloudType::Type::STRATOCUMULUS);
	EXPECT_FALSE(ctv[2].height().isReported());
	EXPECT_EQ(ctv[2].okta(), 1u);
	EXPECT_EQ(ctv[3].type(), metaf::CloudType::Type::CIRRUS);
	EXPECT_FALSE(ctv[3].height().isReported());
	EXPECT_EQ(ctv[3].okta(), 3u);
}

TEST(CloudTypesGroup, parseFormatWithoutHeightFiveLayers) {
	const auto ctg = metaf::CloudTypesGroup::parse("CF1CU3SC1CS1CI2", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->cloudTypes();
	ASSERT_EQ(ctv.size(), 5u);
	EXPECT_EQ(ctv[0].type(), metaf::CloudType::Type::CUMULUS_FRACTUS);
	EXPECT_FALSE(ctv[0].height().isReported());
	EXPECT_EQ(ctv[0].okta(), 1u);
	EXPECT_EQ(ctv[1].type(), metaf::CloudType::Type::CUMULUS);
	EXPECT_FALSE(ctv[1].height().isReported());
	EXPECT_EQ(ctv[1].okta(), 3u);
	EXPECT_EQ(ctv[2].type(), metaf::CloudType::Type::STRATOCUMULUS);
	EXPECT_FALSE(ctv[2].height().isReported());
	EXPECT_EQ(ctv[2].okta(), 1u);
	EXPECT_EQ(ctv[3].type(), metaf::CloudType::Type::CIRROSTRATUS);
	EXPECT_FALSE(ctv[3].height().isReported());
	EXPECT_EQ(ctv[3].okta(), 1u);
	EXPECT_EQ(ctv[4].type(), metaf::CloudType::Type::CIRRUS);
	EXPECT_FALSE(ctv[4].height().isReported());
	EXPECT_EQ(ctv[4].okta(), 2u);
}

TEST(CloudTypesGroup, parseFormatWithoutHeightTooManyLayers) {
	const auto ctg = 
		metaf::CloudTypesGroup::parse("SC1SC1SC1SC1SC1SC1SC1SC1SC1", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->cloudTypes();
	ASSERT_EQ(ctv.size(), 8u);
}

TEST(CloudTypesGroup, parseFormatWithoutHeightWrongReportPart) {
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::TAF).has_value());
}

TEST(CloudTypesGroup, parseFormatWithoutHeightWrongFormat) {
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("AB5", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("AC/", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("//7", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("ACX", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("AB5CF2CI3", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CF2AB5CI3", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CF2CI3AB5", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CF2/CI3", metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Parsing single cloud type with base height in single group
// Purpose: to confirm that group with format which contains single cloud
// layer type with base height and octa (e.g. 8NS035) are parsed correctly and 
// malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(CloudTypesGroup, parseFormatWithHeight) {
	const auto ctg = metaf::CloudTypesGroup::parse("8NS035", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->cloudTypes();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(ctv[0].type(), metaf::CloudType::Type::NIMBOSTRATUS);
	EXPECT_TRUE(ctv[0].height().isReported());
	EXPECT_EQ(ctv[0].height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ctv[0].height().distance().has_value());
	EXPECT_NEAR(ctv[0].height().distance().value(), 3500, heightMargin);
	EXPECT_EQ(ctv[0].height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ctv[0].okta(), 8u);
	EXPECT_TRUE(ctv[0].isValid());
}

TEST(CloudTypesGroup, parseFormatWithHeightWrongReportPart) {
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("8NS035", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("8NS035", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("8NS035", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("8NS035", metaf::ReportPart::TAF).has_value());
}

TEST(CloudTypesGroup, parseFormatWithHeightWrongFormat) {
	//TODO
}

///////////////////////////////////////////////////////////////////////////////
// Appending tests
// Purpose: to confirm that CloudTypesGroup do not append to each other
///////////////////////////////////////////////////////////////////////////////

TEST(CloudTypesGroup, append) {
	auto ctg1 = metaf::CloudTypesGroup::parse("AC4CI6", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg1.has_value());

	auto ctg2 = metaf::CloudTypesGroup::parse("SC3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg2.has_value());

	auto ctg3 = metaf::CloudTypesGroup::parse("3CU012", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg3.has_value());

	EXPECT_EQ(ctg1->append("AC1CI6", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg1->append("SC3", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg1->append("6SC009", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg1->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg1->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ctg2->append("AC1CI6", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg2->append("SC3", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg2->append("6SC009", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg2->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg2->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ctg3->append("AC1CI6", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg3->append("SC3", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg3->append("6SC009", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg3->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg3->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(CloudTypesGroup, isValidTrue) {
	const auto ctg1 = metaf::CloudTypesGroup::parse("AC4CI6", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg1.has_value());
	EXPECT_TRUE(ctg1->isValid());

	const auto ctg2 = metaf::CloudTypesGroup::parse("SC3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg2.has_value());
	EXPECT_TRUE(ctg2->isValid());

	const auto ctg3 = metaf::CloudTypesGroup::parse("BLSN1SC1SC1CI3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg3.has_value());
	EXPECT_TRUE(ctg3->isValid());

	const auto ctg4 = metaf::CloudTypesGroup::parse("8NS035", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg4.has_value());
	EXPECT_TRUE(ctg4->isValid());
}

TEST(CloudTypesGroup, isValidFalse) {
	const auto ctg1 = metaf::CloudTypesGroup::parse("AC4CI9", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg1.has_value());
	EXPECT_FALSE(ctg1->isValid());

	const auto ctg2 = metaf::CloudTypesGroup::parse("SC0", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg2.has_value());
	EXPECT_FALSE(ctg2->isValid());

	const auto ctg3 = metaf::CloudTypesGroup::parse("BLSN9SC1SC1CI3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg3.has_value());
	EXPECT_FALSE(ctg3->isValid());

	const auto ctg4 = metaf::CloudTypesGroup::parse("0NS035", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg4.has_value());
	EXPECT_FALSE(ctg4->isValid());

	const auto ctg5 = metaf::CloudTypesGroup::parse("9NS035", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg5.has_value());
	EXPECT_FALSE(ctg5->isValid());
}