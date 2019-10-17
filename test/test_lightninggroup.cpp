/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

TEST(LightningGroup, parseLtg) {
	const auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
}

TEST(LightningGroup, parseLtgDsnt) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("DSNT", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_TRUE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
}

TEST(LightningGroup, parseOcnlLtg) {
	auto lg = metaf::LightningGroup::parse("OCNL", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("LTG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::OCCASSIONAL);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
}

TEST(LightningGroup, parseFrqLtg) {
	auto lg = metaf::LightningGroup::parse("FRQ", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("LTG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::FREQUENT);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
}

TEST(LightningGroup, parseConsLtg) {
	auto lg = metaf::LightningGroup::parse("CONS", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("LTG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::CONSTANT);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
}

TEST(LightningGroup, parseLtgNE) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("NE", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0), metaf::Direction::Cardinal::NE);
}

TEST(LightningGroup, parseLtgNES) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("N-E-S", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 5u);
	EXPECT_EQ(lg->directions().at(0), metaf::Direction::Cardinal::N);
	EXPECT_EQ(lg->directions().at(1), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(lg->directions().at(2), metaf::Direction::Cardinal::E);
	EXPECT_EQ(lg->directions().at(3), metaf::Direction::Cardinal::SE);
	EXPECT_EQ(lg->directions().at(4), metaf::Direction::Cardinal::S);
}

TEST(LightningGroup, parseLtgAlqds) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("ALQDS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0), metaf::Direction::Cardinal::ALQDS);
}

TEST(LightningGroup, parseLtgNESandOHD) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("N-E-S", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 6u);
	EXPECT_EQ(lg->directions().at(0), metaf::Direction::Cardinal::N);
	EXPECT_EQ(lg->directions().at(1), metaf::Direction::Cardinal::NE);
	EXPECT_EQ(lg->directions().at(2), metaf::Direction::Cardinal::E);
	EXPECT_EQ(lg->directions().at(3), metaf::Direction::Cardinal::SE);
	EXPECT_EQ(lg->directions().at(4), metaf::Direction::Cardinal::S);
	EXPECT_EQ(lg->directions().at(5), metaf::Direction::Cardinal::OHD);
}

TEST(LightningGroup, parseLtgSWNWandSES) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("SW-NW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("SE-S", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 5u);
	EXPECT_EQ(lg->directions().at(0), metaf::Direction::Cardinal::SW);
	EXPECT_EQ(lg->directions().at(1), metaf::Direction::Cardinal::W);
	EXPECT_EQ(lg->directions().at(2), metaf::Direction::Cardinal::NW);
	EXPECT_EQ(lg->directions().at(3), metaf::Direction::Cardinal::SE);
	EXPECT_EQ(lg->directions().at(4), metaf::Direction::Cardinal::S);
}

TEST(LightningGroup, parseLtgSWandE) {
	auto lg = metaf::LightningGroup::parse("LTG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("E", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 2u);
	EXPECT_EQ(lg->directions().at(0), metaf::Direction::Cardinal::SW);
	EXPECT_EQ(lg->directions().at(1), metaf::Direction::Cardinal::E);
}

TEST(LightningGroup, parseConsLtgOhdAndAlqds) {
	auto lg = metaf::LightningGroup::parse("CONS", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("LTG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("AND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(lg->append("ALQDS", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::CONSTANT);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 2u);
	EXPECT_EQ(lg->directions().at(0), metaf::Direction::Cardinal::OHD);
	EXPECT_EQ(lg->directions().at(1), metaf::Direction::Cardinal::ALQDS);
}

TEST(LightningGroup, parseLtgIcCcCgOhd) {
	auto lg = metaf::LightningGroup::parse("LTGICCCCG", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_TRUE(lg->isCloudGround());
	EXPECT_TRUE(lg->isInCloud());
	EXPECT_TRUE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0), metaf::Direction::Cardinal::OHD);
}

TEST(LightningGroup, parseLtgCcCaOhd) {
	auto lg = metaf::LightningGroup::parse("LTGCCCA", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->append("OHD", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_TRUE(lg->isCloudCloud());
	EXPECT_TRUE(lg->isCloudAir());
	EXPECT_FALSE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 1u);
	EXPECT_EQ(lg->directions().at(0), metaf::Direction::Cardinal::OHD);
}

TEST(LightningGroup, parseLtgCb) {
	auto lg = metaf::LightningGroup::parse("LTGCB", metaf::ReportPart::RMK);
	ASSERT_TRUE(lg.has_value());

	EXPECT_EQ(lg->frequency(), metaf::LightningGroup::Frequency::NONE);
	EXPECT_FALSE(lg->isDistant());
	EXPECT_FALSE(lg->isCloudGround());
	EXPECT_FALSE(lg->isInCloud());
	EXPECT_FALSE(lg->isCloudCloud());
	EXPECT_FALSE(lg->isCloudAir());
	EXPECT_TRUE(lg->isUnknownType());
	EXPECT_EQ(lg->directions().size(), 0u);
}
