/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Note: Tests below assume that TerrainVisibility class and its parsing is
// thoroughly tested in test_terrainvisibility.cpp. Here tests focus on actual
// group parsing rather than all options for terrain visibility.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// MT OBSC group sequence
// Purpose: to confirm that MT OBSC group sequence is parsed correctly and the 
// malformed group sequences are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(TerrainGroup, parseMtObsc) {
	auto tg = metaf::TerrainGroup::parse("MT", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("OBSC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(tg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

    EXPECT_EQ(tg->type(), metaf::TerrainGroup::Type::TERRAIN_VISIBILITY);

    EXPECT_EQ(tg->terrainVisibility().size(), 1u);

    const auto tv = tg->terrainVisibility().at(0);

    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_OBSCURED);
    EXPECT_TRUE(tv.direction().empty());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
    EXPECT_TRUE(tv.isValid());

    EXPECT_TRUE(tg->isValid());

}

TEST(TerrainGroup, parseMt) {
	auto tg = metaf::TerrainGroup::parse("MT", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(TerrainGroup, parseMtObscSw) {
	auto tg = metaf::TerrainGroup::parse("MT", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("OBSC", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(tg->append("SW", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}
