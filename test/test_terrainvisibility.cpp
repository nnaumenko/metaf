/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// TerrainVisibility constructor
// Purpose: to confirm that TerrainVisibility default constructor initialises
// the instance as expected
///////////////////////////////////////////////////////////////////////////////

TEST(TerrainVisibility, constructor) {
	const auto tv = metaf::TerrainVisibility();
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_EQ(tv.direction().cardinal(), metaf::Direction::Cardinal::ALQDS);
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
    EXPECT_FALSE(tv.isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Parse MT OBSC
// Purpose: to confirm that MT OBSC sequence is parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(TerrainVisibility, mtObsc) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MT"));
    EXPECT_TRUE(tv.addString("OBSC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_OBSCURED);
    EXPECT_EQ(tv.direction().cardinal(), metaf::Direction::Cardinal::ALQDS);
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
}

TEST(TerrainVisibility, mtObscAddString) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MT"));
    EXPECT_TRUE(tv.addString("OBSC"));
    EXPECT_FALSE(tv.addString("MON"));
    EXPECT_FALSE(tv.addString("VAL"));
    EXPECT_FALSE(tv.addString("NC"));
    EXPECT_FALSE(tv.addString("CUF"));
    EXPECT_FALSE(tv.addString("NEBBBIA"));
    EXPECT_FALSE(tv.addString("INTER"));
    EXPECT_FALSE(tv.addString("VAR"));
    EXPECT_FALSE(tv.addString("STF"));
    EXPECT_FALSE(tv.addString("ELEV"));
    EXPECT_FALSE(tv.addString("ABB"));
    EXPECT_FALSE(tv.addString("DIM"));
    EXPECT_FALSE(tv.addString("AUM"));
    EXPECT_FALSE(tv.addString("SLF"));
    EXPECT_FALSE(tv.addString("RAPID"));
    EXPECT_FALSE(tv.addString(""));
}

///////////////////////////////////////////////////////////////////////////////
// Parse valid MON sequences without trends
// Purpose: to confirm that string sequences MON LIB, MON CLD SCT, MON CLD CIME
// MON VERS INC, MON CNS POST, MON GEN INC, MON INC, MON INVIS are parsed 
// correctly
///////////////////////////////////////////////////////////////////////////////

TEST(TerrainVisibility, monLib) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("LIB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_NOT_OBSCURED);
    EXPECT_EQ(tv.direction().cardinal(), metaf::Direction::Cardinal::ALQDS);
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
}

TEST(TerrainVisibility, monCldSct) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
    EXPECT_EQ(tv.direction().cardinal(), metaf::Direction::Cardinal::ALQDS);
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
}

TEST(TerrainVisibility, monCldCime) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("CIME"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_SUMMITS_IN_CLOUDS);
    EXPECT_EQ(tv.direction().cardinal(), metaf::Direction::Cardinal::ALQDS);
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
}

TEST(TerrainVisibility, monVersInc) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("VERS"));
    EXPECT_TRUE(tv.addString("INC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_SLOPES_IN_CLOUDS);
    EXPECT_EQ(tv.direction().cardinal(), metaf::Direction::Cardinal::ALQDS);
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
}

TEST(TerrainVisibility, monCnsPost) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CNS"));
    EXPECT_TRUE(tv.addString("POST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_OBSERVED_SIDE_VISIBILE);
    EXPECT_EQ(tv.direction().cardinal(), metaf::Direction::Cardinal::ALQDS);
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
}

TEST(TerrainVisibility, monGenInc) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("GEN"));
    EXPECT_TRUE(tv.addString("INC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_MOSTLY_IN_CLOUDS);
    EXPECT_EQ(tv.direction().cardinal(), metaf::Direction::Cardinal::ALQDS);
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
}

TEST(TerrainVisibility, monInc) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("INC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_CLOUDS);
    EXPECT_EQ(tv.direction().cardinal(), metaf::Direction::Cardinal::ALQDS);
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
}

TEST(TerrainVisibility, monInvis) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("INVIS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_INVISIBLE);
    EXPECT_EQ(tv.direction().cardinal(), metaf::Direction::Cardinal::ALQDS);
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
}

///////////////////////////////////////////////////////////////////////////////
// Parse invalid MON sequences
// Purpose: to confirm that invalid MON sequences such as MON SCT, MON CIME, 
// MON POST, MON NEBBIA, MON CLD LIB, MON VERS CLD, MON CNS INC, MON GEN CLD, 
// etc. cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(TerrainVisibility, monInvalid) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));

    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("TEST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("MON"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("SCT"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("CIME"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("POST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("NEBBIA"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_TRUE(tv.addString("LIB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_NOT_OBSCURED);
    EXPECT_TRUE(tv.isValid());
}

TEST(TerrainVisibility, monCldInvalid) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));

    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("TEST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("MON"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("LIB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("INVIS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("POST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("NEBBIA"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("VERS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("INC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_TRUE(tv.addString("CIME"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_SUMMITS_IN_CLOUDS);
    EXPECT_TRUE(tv.isValid());
}

TEST(TerrainVisibility, monVersInvalid) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("VERS"));

    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("TEST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("MON"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("LIB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("INVIS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("POST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("NEBBIA"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("VERS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("CIME"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_TRUE(tv.addString("INC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_SLOPES_IN_CLOUDS);
    EXPECT_TRUE(tv.isValid());
}

TEST(TerrainVisibility, monCnsInvalid) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CNS"));

    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("TEST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("MON"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("LIB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("INVIS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("VERS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("NEBBIA"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("CNS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("CIME"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_TRUE(tv.addString("POST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_OBSERVED_SIDE_VISIBILE);
    EXPECT_TRUE(tv.isValid());
}

TEST(TerrainVisibility, monGenInvalid) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("GEN"));

    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("TEST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("MON"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("LIB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("INVIS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("POST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("NEBBIA"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("GEN"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("CIME"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_TRUE(tv.addString("INC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_MOSTLY_IN_CLOUDS);
    EXPECT_TRUE(tv.isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Parse valid VAL sequences without trends
// Purpose: to confirm that string sequences VAL NIL, VAL FOSCHIA, 
// VAL FOSCHIA SKC SUP, VAL NEBBIA, VAL NEBBIA SCT, VAL CLD SCT, 
// VAL CLD SCT NEBBIA INF, VAL MAR CLD, VAL INVIS 
///////////////////////////////////////////////////////////////////////////////

// TODO

///////////////////////////////////////////////////////////////////////////////
// Parse trends
// Purpose: to confirm that trend strings NC, CUF, NEBBIA INTER, VAR, STF, 
// ELEV, ABB, DIM, AUM, SLW, RAPID can be appended to MON and VAL sequences,
// and that several trend strings may be appended to 
///////////////////////////////////////////////////////////////////////////////

// TODO

///////////////////////////////////////////////////////////////////////////////
// Invalid trends
// Purpose: to confirm that contradicting trends result in Terrain Visibility 
// recognised as invalid: NC and VAR, DIM and AUM, ELEV and ABB, SLW and RAPID
///////////////////////////////////////////////////////////////////////////////

// TODO
