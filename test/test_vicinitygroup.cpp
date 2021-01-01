/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 1.0 / 100 / 2;

///////////////////////////////////////////////////////////////////////////////
// Phenomena tests
// Purpose: to confirm that all weather phenomena in vicinity group are parsed
// and appended (if applicable) correctly
///////////////////////////////////////////////////////////////////////////////

TEST(VicinityGroup, parseCB_N) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCBMAM_N) {
	auto vg = metaf::VicinityGroup::parse("CBMAM", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS_MAMMATUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseTS_N) {
	auto vg = metaf::VicinityGroup::parse("TS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::THUNDERSTORM);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseTCU_N) {
	auto vg = metaf::VicinityGroup::parse("TCU", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::TOWERING_CUMULUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseACC_N) {
	auto vg = metaf::VicinityGroup::parse("ACC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::ALTOCUMULUS_CASTELLANUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseSCSL_N) {
	auto vg = metaf::VicinityGroup::parse("SCSL", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::STRATOCUMULUS_STANDING_LENTICULAR);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseACSL_N) {
	auto vg = metaf::VicinityGroup::parse("ACSL", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::ALTOCUMULUS_STANDING_LENTICULAR);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCCSL_N) {
	auto vg = metaf::VicinityGroup::parse("CCSL", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CIRROCUMULUS_STANDING_LENTICULAR);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseVIRGA_N) {
	auto vg = metaf::VicinityGroup::parse("VIRGA", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::VIRGA);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseVCSH_N) {
	auto vg = metaf::VicinityGroup::parse("VCSH", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::PRECIPITATION_IN_VICINITY);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseFOG_N) {
	auto vg1 = metaf::VicinityGroup::parse("FOG", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());

	EXPECT_EQ(vg1->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg1->type(), metaf::VicinityGroup::Type::FOG);
	EXPECT_EQ(vg1->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg1->distance().isReported());
	EXPECT_EQ(vg1->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg1->directions().size(), 1u);
	EXPECT_EQ(vg1->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg1->isValid());

	auto vg2 = metaf::VicinityGroup::parse("FG", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());

	EXPECT_EQ(vg2->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg2->type(), metaf::VicinityGroup::Type::FOG);
	EXPECT_EQ(vg2->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg2->distance().isReported());
	EXPECT_EQ(vg2->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg2->directions().size(), 1u);
	EXPECT_EQ(vg2->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg2->isValid());
}

TEST(VicinityGroup, parseMIFG_N) {
	auto vg = metaf::VicinityGroup::parse("MIFG", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::FOG_SHALLOW);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseBCFG_N) {
	auto vg = metaf::VicinityGroup::parse("BCFG", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::FOG_PATCHES);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseHAZE_N) {
	auto vg1 = metaf::VicinityGroup::parse("HAZE", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());

	EXPECT_EQ(vg1->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg1->type(), metaf::VicinityGroup::Type::HAZE);
	EXPECT_EQ(vg1->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg1->distance().isReported());
	EXPECT_EQ(vg1->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg1->directions().size(), 1u);
	EXPECT_EQ(vg1->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg1->isValid());

	auto vg2 = metaf::VicinityGroup::parse("HZ", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());

	EXPECT_EQ(vg2->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg2->type(), metaf::VicinityGroup::Type::HAZE);
	EXPECT_EQ(vg2->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg2->distance().isReported());
	EXPECT_EQ(vg2->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg2->directions().size(), 1u);
	EXPECT_EQ(vg2->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg2->isValid());
}

TEST(VicinityGroup, parseBLSN_N) {
	auto vg = metaf::VicinityGroup::parse("BLSN", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::BLOWING_SNOW);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseBLSA_N) {
	auto vg = metaf::VicinityGroup::parse("BLSA", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::BLOWING_SAND);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseBLDU_N) {
	auto vg = metaf::VicinityGroup::parse("BLDU", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::BLOWING_DUST);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseROTOR_CLD_N) {
	auto vg = metaf::VicinityGroup::parse("ROTOR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("CLD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::ROTOR_CLOUD);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCIG_LWR_N) {
	auto vg = metaf::VicinityGroup::parse("CIG", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("LWR", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::LOWER_CEILING);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Direction tests
// Purpose: to confirm that directions and direction sectors in vicinity group 
// are parsed and appended correctly
///////////////////////////////////////////////////////////////////////////////

TEST(VicinityGroup, parseCB_N_SE) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 4u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->directions().at(2).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(3).cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_N__SE) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_N_SE_AND_W) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 5u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->directions().at(2).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(3).cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_EQ(vg->directions().at(4).cardinal(), metaf::Direction::Cardinal::W);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_OHD_AND_W) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::OHD);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::W);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_W_AND_OHD) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::W);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::OHD);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_ALQDS) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("ALQDS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::ALQDS);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Movement direction tests
// Purpose: to confirm that movement directions in vicinity group are parsed  
// and appended correctly
///////////////////////////////////////////////////////////////////////////////

TEST(VicinityGroup, parseCB_N_SE_MOV_N) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NW);
	EXPECT_EQ(vg->directions().size(), 4u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->directions().at(2).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(3).cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_OHD_AND_W_MOV_S) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("S", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::S);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::OHD);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::W);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Distance tests
// Purpose: to confirm that distance in form of DSNT, VC, or value specified in 
// kilometers in the vicinity group are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(VicinityGroup, parseCB_DSNT) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("DSNT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::DISTANT);
	EXPECT_FALSE(vg->distance().isValue());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_DSNT_N_SE) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("DSNT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::DISTANT);
	EXPECT_FALSE(vg->distance().isValue());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 4u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->directions().at(2).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(3).cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_VC) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::VICINITY);
	EXPECT_FALSE(vg->distance().isValue());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 0u);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_VC_N_SE) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("VC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::VICINITY);
	EXPECT_FALSE(vg->distance().isValue());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 4u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->directions().at(2).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(3).cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_15KM_E) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("15KM", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("E", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->distance().distance().has_value());
	EXPECT_NEAR(vg->distance().distance().value(), 15000, margin);
	EXPECT_EQ(vg->distance().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_DSNT_E_SE_MOV_E) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("DSNT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("E-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("E", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::DISTANT);
	EXPECT_FALSE(vg->distance().isValue());
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_5KM_E_AND_OHD_MOV_E) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("5KM", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("E", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("E", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->distance().distance().has_value());
	EXPECT_NEAR(vg->distance().distance().value(), 5000, margin);
	EXPECT_EQ(vg->distance().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(1).cardinal(), metaf::Direction::Cardinal::OHD);
	EXPECT_TRUE(vg->isValid());
}

TEST(VicinityGroup, parseCB_40KM_E_MOV_UNKNOWN) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("40KM", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("E", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("UNKNOWN", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(vg->distance().distance().has_value());
	EXPECT_NEAR(vg->distance().distance().value(), 40000, margin);
	EXPECT_EQ(vg->distance().unit(), metaf::Distance::Unit::METERS);
	EXPECT_EQ(vg->movingDirection().cardinal(), metaf::Direction::Cardinal::UNKNOWN);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_TRUE(vg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Invalid groups test
// Purpose: to confirm that invalid sequences of groups are not appended to
// VicinityGroup's
///////////////////////////////////////////////////////////////////////////////

TEST(VicinityGroup, parseROTOR_other) {
	auto vg1 = metaf::VicinityGroup::parse("ROTOR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("N", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VicinityGroup::parse("ROTOR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("DSNT", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VicinityGroup::parse("ROTOR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("TEST", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VicinityGroup::parse("ROTOR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg5 = metaf::VicinityGroup::parse("ROTOR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg6 = metaf::VicinityGroup::parse("ROTOR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg6.has_value());
	EXPECT_EQ(vg6->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg7 = metaf::VicinityGroup::parse("ROTOR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg7.has_value());
	EXPECT_EQ(vg7->append("ROTOR", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VicinityGroup, parseCB_other) {
	auto vg1 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("CLD", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("TCU", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("TEST", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("UNKNOWN", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg5 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg6 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg6.has_value());
	EXPECT_EQ(vg6->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg7 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg7.has_value());
	EXPECT_EQ(vg7->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VicinityGroup, parseCB_W_AND_NW_MOV_MOV) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VicinityGroup, parseCB_N_AND_other) {
	auto vg1 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("CLD", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("TCU", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg3 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("TEST", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("UNKNOWN", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg5 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg5.has_value());
	EXPECT_EQ(vg5->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg5->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg6 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg6.has_value());
	EXPECT_EQ(vg6->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg6->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg6->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg7 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg7.has_value());
	EXPECT_EQ(vg7->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg7->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg7->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(VicinityGroup, parseCB_N_SE_MOV_other) {
	auto vg1 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("CB", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg2 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("10KM", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
	
	auto vg3 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("TEST", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto vg4 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg4.has_value());
	EXPECT_EQ(vg4->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg4->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}