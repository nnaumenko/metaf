/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(CloudLayersGroup, parseLowLayer0) {
	const auto clg = metaf::CloudLayersGroup::parse("8/000", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::NONE);
}

TEST(CloudLayersGroup, parseLowLayer1) {
	const auto clg = metaf::CloudLayersGroup::parse("8/100", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::CU_HU_CU_FR);
}

TEST(CloudLayersGroup, parseLowLayer2) {
	const auto clg = metaf::CloudLayersGroup::parse("8/200", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::CU_MED_CU_CON);
}

TEST(CloudLayersGroup, parseLowLayer3) {
	const auto clg = metaf::CloudLayersGroup::parse("8/300", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::CB_CAL);
}

TEST(CloudLayersGroup, parseLowLayer4) {
	const auto clg = metaf::CloudLayersGroup::parse("8/400", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::SC_CUGEN);
}

TEST(CloudLayersGroup, parseLowLayer5) {
	const auto clg = metaf::CloudLayersGroup::parse("8/500", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::SC_NON_CUGEN);
}

TEST(CloudLayersGroup, parseLowLayer6) {
	const auto clg = metaf::CloudLayersGroup::parse("8/600", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::ST_NEB_ST_FR);
}

TEST(CloudLayersGroup, parseLowLayer7) {
	const auto clg = metaf::CloudLayersGroup::parse("8/700", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::ST_FR_CU_FR_PANNUS);
}

TEST(CloudLayersGroup, parseLowLayer8) {
	const auto clg = metaf::CloudLayersGroup::parse("8/800", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::CU_SC_NON_CUGEN_DIFFERENT_LEVELS);
}

TEST(CloudLayersGroup, parseLowLayer9) {
	const auto clg = metaf::CloudLayersGroup::parse("8/900", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::CB_CAP);
}

TEST(CloudLayersGroup, parseLowLayerNo) {
	const auto clg = metaf::CloudLayersGroup::parse("8//00", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::CloudLayersGroup::LowLayer::NOT_OBSERVABLE);
}

TEST(CloudLayersGroup, parseMidLayer0) {
	const auto clg = metaf::CloudLayersGroup::parse("8/000", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::NONE);
}

TEST(CloudLayersGroup, parseMidLayer1) {
	const auto clg = metaf::CloudLayersGroup::parse("8/010", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::AS_TR);
}

TEST(CloudLayersGroup, parseMidLayer2) {
	const auto clg = metaf::CloudLayersGroup::parse("8/020", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::AS_OP_NS);
}

TEST(CloudLayersGroup, parseMidLayer3) {
	const auto clg = metaf::CloudLayersGroup::parse("8/030", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::AC_TR);
}

TEST(CloudLayersGroup, parseMidLayer4) {
	const auto clg = metaf::CloudLayersGroup::parse("8/040", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::AC_TR_LEN_PATCHES);
}

TEST(CloudLayersGroup, parseMidLayer5) {
	const auto clg = metaf::CloudLayersGroup::parse("8/050", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::AC_TR_AC_OP_SPREADING);
}

TEST(CloudLayersGroup, parseMidLayer6) {
	const auto clg = metaf::CloudLayersGroup::parse("8/060", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::AC_CUGEN_AC_CBGEN);
}

TEST(CloudLayersGroup, parseMidLayer7) {
	const auto clg = metaf::CloudLayersGroup::parse("8/070", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::AC_DU_AC_OP_AC_WITH_AS_OR_NS);
}

TEST(CloudLayersGroup, parseMidLayer8) {
	const auto clg = metaf::CloudLayersGroup::parse("8/080", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::AC_CAS_AC_FLO);
}

TEST(CloudLayersGroup, parseMidLayer9) {
	const auto clg = metaf::CloudLayersGroup::parse("8/090", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::AC_OF_CHAOTIC_SKY);
}

TEST(CloudLayersGroup, parseMidLayerNo) {
	const auto clg = metaf::CloudLayersGroup::parse("8/0/0", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::CloudLayersGroup::MidLayer::NOT_OBSERVABLE);
}

TEST(CloudLayersGroup, parseHighLayer0) {
	const auto clg = metaf::CloudLayersGroup::parse("8/000", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::NONE);
}

TEST(CloudLayersGroup, parseHighLayer1) {
	const auto clg = metaf::CloudLayersGroup::parse("8/001", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::CI_FIB_CI_UNC);
}

TEST(CloudLayersGroup, parseHighLayer2) {
	const auto clg = metaf::CloudLayersGroup::parse("8/002", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::CI_SPI_CI_CAS_CI_FLO);
}

TEST(CloudLayersGroup, parseHighLayer3) {
	const auto clg = metaf::CloudLayersGroup::parse("8/003", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::CI_SPI_CBGEN);
}

TEST(CloudLayersGroup, parseHighLayer4) {
	const auto clg = metaf::CloudLayersGroup::parse("8/004", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::CI_FIB_CI_UNC_SPREADING);
}

TEST(CloudLayersGroup, parseHighLayer5) {
	const auto clg = metaf::CloudLayersGroup::parse("8/005", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::CI_CS_LOW_ABOVE_HORIZON);
}

TEST(CloudLayersGroup, parseHighLayer6) {
	const auto clg = metaf::CloudLayersGroup::parse("8/006", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::CI_CS_HIGH_ABOVE_HORIZON);
}

TEST(CloudLayersGroup, parseHighLayer7) {
	const auto clg = metaf::CloudLayersGroup::parse("8/007", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::CS_NEB_CS_FIB_COVERING_ENTIRE_SKY);
}

TEST(CloudLayersGroup, parseHighLayer8) {
	const auto clg = metaf::CloudLayersGroup::parse("8/008", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::CS);
}

TEST(CloudLayersGroup, parseHighLayer9) {
	const auto clg = metaf::CloudLayersGroup::parse("8/009", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::CC);
}

TEST(CloudLayersGroup, parseHighLayerNo) {
	const auto clg = metaf::CloudLayersGroup::parse("8/00/", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::CloudLayersGroup::HighLayer::NOT_OBSERVABLE);
}

TEST(CloudLayersGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8/000", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8/000", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8/000", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8/000", metaf::ReportPart::TAF).has_value());
}

TEST(CloudLayersGroup, parseWrongFormat) {
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8000", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8/0000", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8/00", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8/A00", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8/0A0", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8/00A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudLayersGroup::parse("8/", metaf::ReportPart::RMK).has_value());
}

TEST(CloudLayersGroup, isValidTrue) {
	const auto clg1 = metaf::CloudLayersGroup::parse("8/578", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg1.has_value());
	EXPECT_TRUE(clg1->isValid());

	const auto clg2 = metaf::CloudLayersGroup::parse("8/52/", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg2.has_value());
	EXPECT_TRUE(clg2->isValid());

	const auto clg3 = metaf::CloudLayersGroup::parse("8/4//", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg3.has_value());
	EXPECT_TRUE(clg3->isValid());

	const auto clg4 = metaf::CloudLayersGroup::parse("8////", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg4.has_value());
	EXPECT_TRUE(clg4->isValid());
}

TEST(CloudLayersGroup, isValidFalse) {
	const auto clg1 = metaf::CloudLayersGroup::parse("8//00", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg1.has_value());
	EXPECT_FALSE(clg1->isValid());

	const auto clg2 = metaf::CloudLayersGroup::parse("8/0/0", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg2.has_value());
	EXPECT_FALSE(clg2->isValid());

	const auto clg3 = metaf::CloudLayersGroup::parse("8///0", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg3.has_value());
	EXPECT_FALSE(clg3->isValid());
}

TEST(CloudLayersGroup, append) {
	auto clg1 = metaf::CloudLayersGroup::parse("8/578", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg1.has_value());

	auto clg2 = metaf::CloudLayersGroup::parse("8/52/", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg2.has_value());

	EXPECT_EQ(clg1->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(clg1->append("8/578", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(clg1->append("8/52/", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(clg1->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(clg1->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(clg1->append("8/578", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(clg1->append("8/52/", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(clg1->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}
