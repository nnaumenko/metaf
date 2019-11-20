/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

TEST(VicinityGroup, parseCB_N) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

TEST(VicinityGroup, parseCBMAM_N) {
	auto vg = metaf::VicinityGroup::parse("CBMAM", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS_MAMMATUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

TEST(VicinityGroup, parseTS_N) {
	auto vg = metaf::VicinityGroup::parse("TS", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::THUNDERSTORM);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

TEST(VicinityGroup, parseTCU_N) {
	auto vg = metaf::VicinityGroup::parse("TCU", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::TOWERING_CUMULUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

TEST(VicinityGroup, parseACC_N) {
	auto vg = metaf::VicinityGroup::parse("ACC", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::ALTOCUMULUS_CASTELLANUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

TEST(VicinityGroup, parseSCSL_N) {
	auto vg = metaf::VicinityGroup::parse("SCSL", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::STRATOCUMULUS_STANDING_LENTICULAR);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

TEST(VicinityGroup, parseACSL_N) {
	auto vg = metaf::VicinityGroup::parse("ACSL", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::ALTOCUMULUS_STANDING_LENTICULAR);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

TEST(VicinityGroup, parseCCSL_N) {
	auto vg = metaf::VicinityGroup::parse("CCSL", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CIRROCUMULUS_STANDING_LENTICULAR);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

TEST(VicinityGroup, parseVIRGA_N) {
	auto vg = metaf::VicinityGroup::parse("VIRGA", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::VIRGA);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

TEST(VicinityGroup, parseVCSH_N) {
	auto vg = metaf::VicinityGroup::parse("VCSH", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::PRECIPITATION_IN_VICINITY);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

TEST(VicinityGroup, parseROTOR_CLD_N) {
	auto vg = metaf::VicinityGroup::parse("ROTOR", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("CLD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::ROTOR_CLOUD);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
}

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
}

TEST(VicinityGroup, parseCB_N_SE) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 4u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->directions().at(1), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->directions().at(2), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(3), metaf::Direction::Cardinal::SE);
}

TEST(VicinityGroup, parseCB_N_SE_AND_W) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 5u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->directions().at(1), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->directions().at(2), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(3), metaf::Direction::Cardinal::SE);
	EXPECT_EQ(vg->directions().at(4), metaf::Direction::Cardinal::W);
}

TEST(VicinityGroup, parseCB_OHD_AND_W) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::OHD);
	EXPECT_EQ(vg->directions().at(1), metaf::Direction::Cardinal::W);
}

TEST(VicinityGroup, parseCB_W_AND_OHD) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::W);
	EXPECT_EQ(vg->directions().at(1), metaf::Direction::Cardinal::OHD);
}

TEST(VicinityGroup, parseCB_ALQDS) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("ALQDS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::ALQDS);
}

TEST(VicinityGroup, parseCB_N_AND_other) {
	auto vg1 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("CLD", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto vg2 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("TCU", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto vg3 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("N", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("TEST", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(VicinityGroup, parseCB_N_SE_MOV_N) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("NW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NW);
	EXPECT_EQ(vg->directions().size(), 4u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->directions().at(1), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->directions().at(2), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(3), metaf::Direction::Cardinal::SE);
}

TEST(VicinityGroup, parseCB_OHD_AND_W_MOV_S) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("W", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("S", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_FALSE(vg->distance().isReported());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::S);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::OHD);
	EXPECT_EQ(vg->directions().at(1), metaf::Direction::Cardinal::W);
}

TEST(VicinityGroup, parseCB_N_SE_MOV_other) {
	auto vg1 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg1.has_value());
	EXPECT_EQ(vg1->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg1->append("CB", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto vg2 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg2.has_value());
	EXPECT_EQ(vg2->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg2->append("10KM", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	
	auto vg3 = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg3.has_value());
	EXPECT_EQ(vg3->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg3->append("TEST", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(VicinityGroup, parseCB_DSNT) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("DSNT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	
	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::DISTANT);
	EXPECT_FALSE(vg->distance().isValue());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 0u);
}

TEST(VicinityGroup, parseCB_DSNT_N_SE) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("DSNT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("N-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::DISTANT);
	EXPECT_FALSE(vg->distance().isValue());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 4u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::N);
	EXPECT_EQ(vg->directions().at(1), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(vg->directions().at(2), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(3), metaf::Direction::Cardinal::SE);
}

TEST(VicinityGroup, parseCB_15KM_E) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("15KM", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("E", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg->distance().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->distance().integer().has_value());
	EXPECT_EQ(vg->distance().integer().value(), 15000u);
	EXPECT_FALSE(vg->distance().numerator().has_value());
	EXPECT_FALSE(vg->distance().denominator().has_value());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::NONE);
	EXPECT_EQ(vg->directions().size(), 1u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::E);
}

TEST(VicinityGroup, parseCB_DSNT_E_SE_MOV_E) {
	auto vg = metaf::VicinityGroup::parse("CB", metaf::ReportPart::RMK);
	ASSERT_TRUE(vg.has_value());

	EXPECT_EQ(vg->append("DSNT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("E-SE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("MOV", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(vg->append("E", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::DISTANT);
	EXPECT_FALSE(vg->distance().isValue());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(1), metaf::Direction::Cardinal::SE);
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

	EXPECT_EQ(vg->type(), metaf::VicinityGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(vg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_EQ(vg->distance().unit(), metaf::Distance::Unit::METERS);
	ASSERT_TRUE(vg->distance().integer().has_value());
	EXPECT_EQ(vg->distance().integer(), 5000u);
	EXPECT_FALSE(vg->distance().numerator().has_value());
	EXPECT_FALSE(vg->distance().denominator().has_value());
	EXPECT_EQ(vg->movingDirection(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().size(), 2u);
	EXPECT_EQ(vg->directions().at(0), metaf::Direction::Cardinal::E);
	EXPECT_EQ(vg->directions().at(1), metaf::Direction::Cardinal::OHD);
}
