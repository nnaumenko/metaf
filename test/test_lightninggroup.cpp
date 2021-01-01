/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// LTG group
// Purpose: to confirm that single LTG group is parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(LightningGroup, parseLtg) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_TRUE(lg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Distant and vicinity groups
// Purpose: to confirm that LTG DSNT and LTG VC groups with or without 
// direction or direction sectors specified are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(LightningGroup, parseLtgDsnt) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("DSNT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::DISTANT);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgDsntAlqds) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("DSNT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("ALQDS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::DISTANT);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::ALQDS);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgVc) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("VC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::VICINITY);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgVcAlqds) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("VC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("ALQDS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::VICINITY);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::ALQDS);
	EXPECT_TRUE(lg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Frequency
// Purpose: to confirm that each lightning frequence is parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(LightningGroup, parseOcnlLtg) {
	auto lg = metaf::LightningGroup::parse("OCNL", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("LTG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::OCCASIONAL);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseFrqLtg) {
	auto lg = metaf::LightningGroup::parse("FRQ", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("LTG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::FREQUENT);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseConsLtg) {
	auto lg = metaf::LightningGroup::parse("CONS", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("LTG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::CONSTANT);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_TRUE(lg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Directions and direction sectors
// Purpose: to confirm that lightning groups with directions and direction
// sectors specified are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(LightningGroup, parseLtgNE) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("NE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgNES) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("N-E-S", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 5u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(lg->directions().at(1).cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(lg->directions().at(2).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(lg->directions().at(3).cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_EQ(lg->directions().at(4).cardinal(), metaf::Direction::Cardinal::S);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgAlqds) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("ALQDS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::ALQDS);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgNESandOHD) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("N-E-S", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 6u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(lg->directions().at(1).cardinal(), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(lg->directions().at(2).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_EQ(lg->directions().at(3).cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_EQ(lg->directions().at(4).cardinal(), metaf::Direction::Cardinal::S);
	EXPECT_EQ(lg->directions().at(5).cardinal(), metaf::Direction::Cardinal::OHD);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgSWNWandSES) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("SW-NW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("SE-S", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 5u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::SW);
	EXPECT_EQ(lg->directions().at(1).cardinal(), metaf::Direction::Cardinal::W);
	EXPECT_EQ(lg->directions().at(2).cardinal(), metaf::Direction::Cardinal::NW);
	EXPECT_EQ(lg->directions().at(3).cardinal(), metaf::Direction::Cardinal::SE);
	EXPECT_EQ(lg->directions().at(4).cardinal(), metaf::Direction::Cardinal::S);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgSWandE) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("E", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 2u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::SW);
	EXPECT_EQ(lg->directions().at(1).cardinal(), metaf::Direction::Cardinal::E);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseConsLtgOhdAndAlqds) {
	auto lg = metaf::LightningGroup::parse("CONS", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("LTG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("ALQDS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::CONSTANT);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 2u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::OHD);
	EXPECT_EQ(lg->directions().at(1).cardinal(), metaf::Direction::Cardinal::ALQDS);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgIcCcCgOhd) {
	auto lg = metaf::LightningGroup::parse("LTGICCCCG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(lg->isCloudGround());
	EXPECT_TRUE(lg->isInCloud());
	EXPECT_TRUE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::OHD);
	EXPECT_TRUE(lg->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Lightning types
// Purpose: to confirm that each lightning type (cloud-cloud, cloud-ground, 
// in-cloud, cloud-air) is parsed correctly, that unrecognised lightning types
// are ignored, and that maximum number of lightning types can be recognised
///////////////////////////////////////////////////////////////////////////////

TEST(LightningGroup, parseLtgCc) {
	auto lg = metaf::LightningGroup::parse("LTGCC", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_TRUE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgCg) {
	auto lg = metaf::LightningGroup::parse("LTGCG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_TRUE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgIc) {
	auto lg = metaf::LightningGroup::parse("LTGIC", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_TRUE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgCa) {
	auto lg = metaf::LightningGroup::parse("LTGCA", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_TRUE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgCcCaOhd) {
	auto lg = metaf::LightningGroup::parse("LTGCCCA", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_TRUE(lg->isCloudCloud());
	EXPECT_TRUE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::OHD);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgCaCcOhd) {
	auto lg = metaf::LightningGroup::parse("LTGCACC", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_TRUE(lg->isCloudCloud());
	EXPECT_TRUE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0).cardinal(), metaf::Direction::Cardinal::OHD);
	EXPECT_TRUE(lg->isValid());
}

TEST(LightningGroup, parseLtgCb) {
	auto lg = metaf::LightningGroup::parse("LTGCB", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_TRUE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_FALSE(lg->isValid());
}

TEST(LightningGroup, parseLtgICC) {
	auto lg = metaf::LightningGroup::parse("LTGICC", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());
	EXPECT_EQ(lg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_TRUE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_TRUE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
	EXPECT_FALSE(lg->isValid());
}

TEST(LightningGroup, parseLtgICICICICICICCC) {
	//Here assuming that max 6 lightning types are recognised by group
	auto lg = metaf::LightningGroup::parse("LTGICICICICICICCC", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_EQ(lg->distance().modifier(), metaf::Distance::Modifier::NONE);
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_TRUE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
}
