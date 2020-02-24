/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Low-layer clouds
// Purpose: to confirm that every type of low-layer clouds is parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(LowMidHighCloudGroup, parseLowLayer0) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/000", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::NONE);
}

TEST(LowMidHighCloudGroup, parseLowLayer1) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/100", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::CU_HU_CU_FR);
}

TEST(LowMidHighCloudGroup, parseLowLayer2) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/200", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::CU_MED_CU_CON);
}

TEST(LowMidHighCloudGroup, parseLowLayer3) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/300", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::CB_CAL);
}

TEST(LowMidHighCloudGroup, parseLowLayer4) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/400", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::SC_CUGEN);
}

TEST(LowMidHighCloudGroup, parseLowLayer5) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/500", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::SC_NON_CUGEN);
}

TEST(LowMidHighCloudGroup, parseLowLayer6) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/600", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::ST_NEB_ST_FR);
}

TEST(LowMidHighCloudGroup, parseLowLayer7) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/700", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::ST_FR_CU_FR_PANNUS);
}

TEST(LowMidHighCloudGroup, parseLowLayer8) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/800", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::CU_SC_NON_CUGEN_DIFFERENT_LEVELS);
}

TEST(LowMidHighCloudGroup, parseLowLayer9) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/900", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::CB_CAP);
}

TEST(LowMidHighCloudGroup, parseLowLayerNo) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8//00", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->lowLayer(), metaf::LowMidHighCloudGroup::LowLayer::NOT_OBSERVABLE);
}

///////////////////////////////////////////////////////////////////////////////
// Mid-layer clouds
// Purpose: to confirm that every type of mid-layer clouds is parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(LowMidHighCloudGroup, parseMidLayer0) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/000", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::NONE);
}

TEST(LowMidHighCloudGroup, parseMidLayer1) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/010", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::AS_TR);
}

TEST(LowMidHighCloudGroup, parseMidLayer2) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/020", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::AS_OP_NS);
}

TEST(LowMidHighCloudGroup, parseMidLayer3) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/030", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::AC_TR);
}

TEST(LowMidHighCloudGroup, parseMidLayer4) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/040", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::AC_TR_LEN_PATCHES);
}

TEST(LowMidHighCloudGroup, parseMidLayer5) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/050", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::AC_TR_AC_OP_SPREADING);
}

TEST(LowMidHighCloudGroup, parseMidLayer6) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/060", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::AC_CUGEN_AC_CBGEN);
}

TEST(LowMidHighCloudGroup, parseMidLayer7) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/070", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::AC_DU_AC_OP_AC_WITH_AS_OR_NS);
}

TEST(LowMidHighCloudGroup, parseMidLayer8) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/080", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::AC_CAS_AC_FLO);
}

TEST(LowMidHighCloudGroup, parseMidLayer9) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/090", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::AC_OF_CHAOTIC_SKY);
}

TEST(LowMidHighCloudGroup, parseMidLayerNo) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/0/0", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->midLayer(), metaf::LowMidHighCloudGroup::MidLayer::NOT_OBSERVABLE);
}

///////////////////////////////////////////////////////////////////////////////
// High-layer clouds
// Purpose: to confirm that every type of low clouds is parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(LowMidHighCloudGroup, parseHighLayer0) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/000", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::NONE);
}

TEST(LowMidHighCloudGroup, parseHighLayer1) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/001", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::CI_FIB_CI_UNC);
}

TEST(LowMidHighCloudGroup, parseHighLayer2) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/002", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::CI_SPI_CI_CAS_CI_FLO);
}

TEST(LowMidHighCloudGroup, parseHighLayer3) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/003", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::CI_SPI_CBGEN);
}

TEST(LowMidHighCloudGroup, parseHighLayer4) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/004", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::CI_FIB_CI_UNC_SPREADING);
}

TEST(LowMidHighCloudGroup, parseHighLayer5) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/005", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::CI_CS_LOW_ABOVE_HORIZON);
}

TEST(LowMidHighCloudGroup, parseHighLayer6) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/006", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::CI_CS_HIGH_ABOVE_HORIZON);
}

TEST(LowMidHighCloudGroup, parseHighLayer7) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/007", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::CS_NEB_CS_FIB_COVERING_ENTIRE_SKY);
}

TEST(LowMidHighCloudGroup, parseHighLayer8) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/008", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::CS);
}

TEST(LowMidHighCloudGroup, parseHighLayer9) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/009", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::CC);
}

TEST(LowMidHighCloudGroup, parseHighLayerNo) {
	const auto clg = metaf::LowMidHighCloudGroup::parse("8/00/", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg.has_value());
	EXPECT_EQ(clg->highLayer(), metaf::LowMidHighCloudGroup::HighLayer::NOT_OBSERVABLE);
}

///////////////////////////////////////////////////////////////////////////////
// Incorrect groups
// Purpose: to confirm that malformed groups are not parsed.
///////////////////////////////////////////////////////////////////////////////

TEST(LowMidHighCloudGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8/000", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8/000", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8/000", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8/000", metaf::ReportPart::TAF).has_value());
}

TEST(LowMidHighCloudGroup, parseWrongFormat) {
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8000", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8/0000", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8/00", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8/A00", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8/0A0", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8/00A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::LowMidHighCloudGroup::parse("8/", metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(LowMidHighCloudGroup, isValidTrue) {
	const auto clg1 = metaf::LowMidHighCloudGroup::parse("8/578", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg1.has_value());
	EXPECT_TRUE(clg1->isValid());

	const auto clg2 = metaf::LowMidHighCloudGroup::parse("8/52/", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg2.has_value());
	EXPECT_TRUE(clg2->isValid());

	const auto clg3 = metaf::LowMidHighCloudGroup::parse("8/4//", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg3.has_value());
	EXPECT_TRUE(clg3->isValid());

	const auto clg4 = metaf::LowMidHighCloudGroup::parse("8////", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg4.has_value());
	EXPECT_TRUE(clg4->isValid());
}

TEST(LowMidHighCloudGroup, isValidFalse) {
	const auto clg1 = metaf::LowMidHighCloudGroup::parse("8//00", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg1.has_value());
	EXPECT_FALSE(clg1->isValid());

	const auto clg2 = metaf::LowMidHighCloudGroup::parse("8/0/0", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg2.has_value());
	EXPECT_FALSE(clg2->isValid());

	const auto clg3 = metaf::LowMidHighCloudGroup::parse("8///0", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg3.has_value());
	EXPECT_FALSE(clg3->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Appending tests
// Purpose: to confirm that no group can be appended to group of this type
///////////////////////////////////////////////////////////////////////////////

TEST(LowMidHighCloudGroup, append) {
	auto clg1 = metaf::LowMidHighCloudGroup::parse("8/578", metaf::ReportPart::RMK);
	ASSERT_TRUE(clg1.has_value());

	auto clg2 = metaf::LowMidHighCloudGroup::parse("8/52/", metaf::ReportPart::RMK);
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
