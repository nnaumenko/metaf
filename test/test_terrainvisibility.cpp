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
    EXPECT_FALSE(tv.isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Parse MT OBSC
// Purpose: to confirm that MT OBSC sequence is parsed correctly
// Note: MT OBSC is an isolated case as other groups have format either MON ...
// or VAL ... , and tested here completely, independent of MON ... and VAL ...
// group parsing tests
///////////////////////////////////////////////////////////////////////////////

TEST(TerrainVisibility, mtObsc) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MT"));
    EXPECT_TRUE(tv.addString("OBSC"));
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
}

TEST(TerrainVisibility, mtAddString) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MT"));
    EXPECT_FALSE(tv.addString("MON"));
    EXPECT_FALSE(tv.addString("VAL"));
    EXPECT_FALSE(tv.addString("GEN"));
    EXPECT_FALSE(tv.addString("CLD"));
    EXPECT_FALSE(tv.addString("CNS"));
    EXPECT_FALSE(tv.addString("VERS"));
    EXPECT_FALSE(tv.addString("INC"));
    EXPECT_FALSE(tv.addString("FOSCHIA"));
    EXPECT_FALSE(tv.addString("MAR"));
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
    EXPECT_FALSE(tv.addString("SLW"));
    EXPECT_FALSE(tv.addString("RAPID"));
    EXPECT_FALSE(tv.addString("N"));
    EXPECT_FALSE(tv.addString("NE"));
    EXPECT_FALSE(tv.addString("E"));
    EXPECT_FALSE(tv.addString("SE"));
    EXPECT_FALSE(tv.addString("S"));
    EXPECT_FALSE(tv.addString("SW"));
    EXPECT_FALSE(tv.addString("W"));
    EXPECT_FALSE(tv.addString("NW"));
    EXPECT_FALSE(tv.addString(""));
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
    EXPECT_FALSE(tv.addString("SLW"));
    EXPECT_FALSE(tv.addString("RAPID"));
    EXPECT_FALSE(tv.addString("N"));
    EXPECT_FALSE(tv.addString("NE"));
    EXPECT_FALSE(tv.addString("E"));
    EXPECT_FALSE(tv.addString("SE"));
    EXPECT_FALSE(tv.addString("S"));
    EXPECT_FALSE(tv.addString("SW"));
    EXPECT_FALSE(tv.addString("W"));
    EXPECT_FALSE(tv.addString("NW"));
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
}

TEST(TerrainVisibility, monCldSct) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
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
}

TEST(TerrainVisibility, monCldCime) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("CIME"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_SUMMITS_IN_CLOUDS);
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
}

TEST(TerrainVisibility, monVersInc) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("VERS"));
    EXPECT_TRUE(tv.addString("INC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_SLOPES_IN_CLOUDS);
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
}

TEST(TerrainVisibility, monCnsPost) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CNS"));
    EXPECT_TRUE(tv.addString("POST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_OBSERVED_SIDE_VISIBILE);
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
}

TEST(TerrainVisibility, monGenInc) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("GEN"));
    EXPECT_TRUE(tv.addString("INC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_MOSTLY_IN_CLOUDS);
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
}

TEST(TerrainVisibility, monInc) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("INC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_CLOUDS);
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
}

TEST(TerrainVisibility, monInvis) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("INVIS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_INVISIBLE);
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

TEST(TerrainVisibility, valNil) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("NIL"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_NOT_OBSCURED);
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
}

TEST(TerrainVisibility, valFoschia) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("FOSCHIA"));
    EXPECT_FALSE(tv.isValid());
    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_MIST);
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
}

TEST(TerrainVisibility, valFoschiaSkcSup) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("FOSCHIA"));
    EXPECT_TRUE(tv.addString("SKC"));
    EXPECT_TRUE(tv.addString("SUP"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_LOW_MIST);
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
}

TEST(TerrainVisibility, valNebbia) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("NEBBIA"));
    EXPECT_FALSE(tv.isValid());
    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_FOG);
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
}

TEST(TerrainVisibility, valNebbiaSct) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("NEBBIA"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_FOG);
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
}

TEST(TerrainVisibility, valCldSct) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_FALSE(tv.isValid());
    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_CLOUDS);
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
}

TEST(TerrainVisibility, valCldSctNebbiaInf) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("NEBBIA"));
    EXPECT_TRUE(tv.addString("INF"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_CLOUDS_FOG_BELOW);
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
}

TEST(TerrainVisibility, valMarCld) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("MAR"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_CLOUD_LAYER);
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
}

TEST(TerrainVisibility, valInvis) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("INVIS"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_INVISIBLE);
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
}

///////////////////////////////////////////////////////////////////////////////
// Parse invalid VAL sequences
// Purpose: to confirm that invalid VAL sequences cannot be parsed (such as
// // VAL SKC, VAL SUP, VAL SCT, VAL INF, VAL CLD SCT FOSCHIA, etc)
///////////////////////////////////////////////////////////////////////////////

TEST(TerrainVisibility, valInvalid) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));

    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("TEST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("VAL"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("SKC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("SUP"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("SCT"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("INV"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_TRUE(tv.addString("NIL"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_NOT_OBSCURED);
    EXPECT_TRUE(tv.isValid());
}

TEST(TerrainVisibility, valFoschiaInvalid) {
	auto tv1 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv1.addString("VAL"));
    EXPECT_TRUE(tv1.addString("FOSCHIA"));
    EXPECT_EQ(tv1.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv1.isValid());
    EXPECT_FALSE(tv1.addString("MON"));
    EXPECT_EQ(tv1.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_MIST);
    EXPECT_TRUE(tv1.isValid());

	auto tv2 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv2.addString("VAL"));
    EXPECT_TRUE(tv2.addString("FOSCHIA"));
    EXPECT_EQ(tv2.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv2.isValid());
    EXPECT_FALSE(tv2.addString("TEST"));
    EXPECT_EQ(tv2.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_MIST);
    EXPECT_TRUE(tv2.isValid());

	auto tv3 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv3.addString("VAL"));
    EXPECT_TRUE(tv3.addString("FOSCHIA"));
    EXPECT_EQ(tv3.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv3.isValid());
    EXPECT_FALSE(tv3.addString("VAL"));
    EXPECT_EQ(tv3.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_MIST);
    EXPECT_TRUE(tv3.isValid());

	auto tv4 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv4.addString("VAL"));
    EXPECT_TRUE(tv4.addString("FOSCHIA"));
    EXPECT_EQ(tv4.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv4.isValid());
    EXPECT_FALSE(tv4.addString("GEN"));
    EXPECT_EQ(tv4.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_MIST);
    EXPECT_TRUE(tv4.isValid());

	auto tv5 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv5.addString("VAL"));
    EXPECT_TRUE(tv5.addString("FOSCHIA"));
    EXPECT_EQ(tv5.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv5.isValid());
    EXPECT_FALSE(tv5.addString("SUP"));
    EXPECT_EQ(tv5.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_MIST);
    EXPECT_TRUE(tv5.isValid());

	auto tv6 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv6.addString("VAL"));
    EXPECT_TRUE(tv6.addString("FOSCHIA"));
    EXPECT_EQ(tv6.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv6.isValid());
    EXPECT_FALSE(tv6.addString("SCT"));
    EXPECT_EQ(tv6.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_MIST);
    EXPECT_TRUE(tv6.isValid());

	auto tv7 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv7.addString("VAL"));
    EXPECT_TRUE(tv7.addString("FOSCHIA"));
    EXPECT_EQ(tv7.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv7.isValid());
    EXPECT_FALSE(tv7.addString("INV"));
    EXPECT_EQ(tv7.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_MIST);
    EXPECT_TRUE(tv7.isValid());
}

TEST(TerrainVisibility, valCldInvalid) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("CLD"));

    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("TEST"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("VAL"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_FALSE(tv.addString("MAR"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv.isValid());

    EXPECT_TRUE(tv.addString("SCT"));

    EXPECT_FALSE(tv.addString("SUP"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());

    EXPECT_FALSE(tv.addString("CLD"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());

    EXPECT_FALSE(tv.addString("FOSCHIA"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());

    EXPECT_FALSE(tv.addString("INF"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());

    EXPECT_FALSE(tv.addString(""));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
}

TEST(TerrainVisibility, valNebbiaInvalid) {
	auto tv1 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv1.addString("VAL"));
    EXPECT_TRUE(tv1.addString("NEBBIA"));
    EXPECT_EQ(tv1.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv1.isValid());
    EXPECT_FALSE(tv1.addString("MON"));
    EXPECT_EQ(tv1.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_FOG);
    EXPECT_TRUE(tv1.isValid());

	auto tv2 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv2.addString("VAL"));
    EXPECT_TRUE(tv2.addString("NEBBIA"));
    EXPECT_EQ(tv2.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv2.isValid());
    EXPECT_FALSE(tv2.addString("TEST"));
    EXPECT_EQ(tv2.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_FOG);
    EXPECT_TRUE(tv2.isValid());

	auto tv3 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv3.addString("VAL"));
    EXPECT_TRUE(tv3.addString("NEBBIA"));
    EXPECT_EQ(tv3.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv3.isValid());
    EXPECT_FALSE(tv3.addString("VAL"));
    EXPECT_EQ(tv3.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_FOG);
    EXPECT_TRUE(tv3.isValid());

	auto tv4 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv4.addString("VAL"));
    EXPECT_TRUE(tv4.addString("NEBBIA"));
    EXPECT_EQ(tv4.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv4.isValid());
    EXPECT_FALSE(tv4.addString("GEN"));
    EXPECT_EQ(tv4.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_FOG);
    EXPECT_TRUE(tv4.isValid());

	auto tv5 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv5.addString("VAL"));
    EXPECT_TRUE(tv5.addString("NEBBIA"));
    EXPECT_EQ(tv5.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv5.isValid());
    EXPECT_FALSE(tv5.addString("SUP"));
    EXPECT_EQ(tv5.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_FOG);
    EXPECT_TRUE(tv5.isValid());

	auto tv6 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv6.addString("VAL"));
    EXPECT_TRUE(tv6.addString("NEBBIA"));
    EXPECT_EQ(tv6.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv6.isValid());
    EXPECT_FALSE(tv6.addString("INV"));
    EXPECT_EQ(tv6.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_FOG);
    EXPECT_TRUE(tv6.isValid());

	auto tv7 = metaf::TerrainVisibility();
    EXPECT_TRUE(tv7.addString("VAL"));
    EXPECT_TRUE(tv7.addString("NEBBIA"));
    EXPECT_EQ(tv7.description(), metaf::TerrainVisibility::Description::NOT_SPECIFIED);
    EXPECT_FALSE(tv7.isValid());
    EXPECT_FALSE(tv7.addString(""));
    EXPECT_EQ(tv7.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_FOG);
    EXPECT_TRUE(tv7.isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Parse trends
// Purpose: to confirm that trend strings NC, CUF, NEBBIA INTER, VAR, STF, 
// ELEV, ABB, DIM, AUM, SLW, RAPID can be appended to MON and VAL sequences,
// and that several trend strings may be appended to 
///////////////////////////////////////////////////////////////////////////////

TEST(TerrainVisibility, trendNc) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("LIB"));
    EXPECT_TRUE(tv.addString("NC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_NOT_OBSCURED);
    EXPECT_TRUE(tv.isValid());
    EXPECT_TRUE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendCuf) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("LIB"));
    EXPECT_TRUE(tv.addString("CUF"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_NOT_OBSCURED);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_TRUE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendStf) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("STF"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_TRUE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendElev) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("ELEV"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_TRUE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendAbb) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("ABB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_TRUE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendDim) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("DIM"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_TRUE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendAum) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("AUM"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_TRUE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendSlw) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("SLW"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_TRUE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendRapid) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("RAPID"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_TRUE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendVarRapid) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("VAR"));
    EXPECT_TRUE(tv.addString("RAPID"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_TRUE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_TRUE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendNebbiaInter) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("NEBBIA"));
    EXPECT_TRUE(tv.addString("INTER"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_TRUE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendAfterValFoschia) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("FOSCHIA"));
    EXPECT_TRUE(tv.addString("NC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_MIST);
    EXPECT_TRUE(tv.isValid());
    EXPECT_TRUE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendAfterValFoschiaSkcSup) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("FOSCHIA"));
    EXPECT_TRUE(tv.addString("SKC"));
    EXPECT_TRUE(tv.addString("SUP"));
    EXPECT_TRUE(tv.addString("NC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_LOW_MIST);
    EXPECT_TRUE(tv.isValid());
    EXPECT_TRUE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendAfterValNebbia) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("NEBBIA"));
    EXPECT_TRUE(tv.addString("NC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_FOG);
    EXPECT_TRUE(tv.isValid());
    EXPECT_TRUE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendAfterValNebbiaSct) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("NEBBIA"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("NC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_FOG);
    EXPECT_TRUE(tv.isValid());
    EXPECT_TRUE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendAfterValCldSct) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("NC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_CLOUDS);
    EXPECT_TRUE(tv.isValid());
    EXPECT_TRUE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

TEST(TerrainVisibility, trendAfterValCldSctNebbiaInter) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("VAL"));
    EXPECT_TRUE(tv.addString("CLD"));
    EXPECT_TRUE(tv.addString("SCT"));
    EXPECT_TRUE(tv.addString("NEBBIA"));
    EXPECT_TRUE(tv.addString("INF"));
    EXPECT_TRUE(tv.addString("NC"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::VALLEYS_IN_SCATTERED_CLOUDS_FOG_BELOW);
    EXPECT_TRUE(tv.isValid());
    EXPECT_TRUE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

///////////////////////////////////////////////////////////////////////////////
// Invalid trends
// Purpose: to confirm that contradicting trends result in Terrain Visibility 
// recognised as invalid: NC and VAR, DIM and AUM, ELEV and ABB, SLW and RAPID
///////////////////////////////////////////////////////////////////////////////

TEST(TerrainVisibility, noTrendAfterMtObs) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MT"));
    EXPECT_TRUE(tv.addString("OBSC"));
    EXPECT_FALSE(tv.addString("CUF"));
    EXPECT_FALSE(tv.addString("NEBBIA"));
    EXPECT_FALSE(tv.addString("INTER"));
    EXPECT_FALSE(tv.addString("VAR"));
    EXPECT_FALSE(tv.addString("STF"));
    EXPECT_FALSE(tv.addString("ELEV"));
    EXPECT_FALSE(tv.addString("ABB"));
    EXPECT_FALSE(tv.addString("DIM"));
    EXPECT_FALSE(tv.addString("AUM"));
    EXPECT_FALSE(tv.addString("SLW"));
    EXPECT_FALSE(tv.addString("RAPID"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_OBSCURED);
    EXPECT_TRUE(tv.isValid());
    EXPECT_FALSE(tv.isTrendNoChanges());
    EXPECT_FALSE(tv.isTrendCumulusFormation());
    EXPECT_FALSE(tv.isTrendIntermittentFog());
    EXPECT_FALSE(tv.isTrendStratification());
    EXPECT_FALSE(tv.isTrendRising());
    EXPECT_FALSE(tv.isTrendLowering());
    EXPECT_FALSE(tv.isTrendChanging());
    EXPECT_FALSE(tv.isTrendDiminising());
    EXPECT_FALSE(tv.isTrendIncreasing());
    EXPECT_FALSE(tv.isTrendSlowly());
    EXPECT_FALSE(tv.isTrendRapidly());
}

///////////////////////////////////////////////////////////////////////////////
// Direction
// Purpose: to confirm that terrain visibility information which contains 
// a cardinal direction is parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(TerrainVisibility, monDirLib) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("W"));
    EXPECT_TRUE(tv.addString("LIB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_NOT_OBSCURED);
    EXPECT_EQ(tv.direction().size(), 1u);
    EXPECT_EQ(tv.direction().at(0).cardinal(), metaf::Direction::Cardinal::W);
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

TEST(TerrainVisibility, monDirDirLib) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("W"));
    EXPECT_TRUE(tv.addString("NW"));
    EXPECT_TRUE(tv.addString("LIB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_NOT_OBSCURED);
    EXPECT_EQ(tv.direction().size(), 2u);
    EXPECT_EQ(tv.direction().at(0).cardinal(), metaf::Direction::Cardinal::W);
    EXPECT_EQ(tv.direction().at(1).cardinal(), metaf::Direction::Cardinal::NW);
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

TEST(TerrainVisibility, monDirDirDirLib) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("SW"));
    EXPECT_TRUE(tv.addString("W"));
    EXPECT_TRUE(tv.addString("NW"));
    EXPECT_TRUE(tv.addString("LIB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_NOT_OBSCURED);
    EXPECT_EQ(tv.direction().size(), 3u);
    EXPECT_EQ(tv.direction().at(0).cardinal(), metaf::Direction::Cardinal::SW);
    EXPECT_EQ(tv.direction().at(1).cardinal(), metaf::Direction::Cardinal::W);
    EXPECT_EQ(tv.direction().at(2).cardinal(), metaf::Direction::Cardinal::NW);
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

TEST(TerrainVisibility, monLibTooManyDirections) {
	auto tv = metaf::TerrainVisibility();
    EXPECT_TRUE(tv.addString("MON"));
    EXPECT_TRUE(tv.addString("SW"));
    EXPECT_TRUE(tv.addString("W"));
    EXPECT_TRUE(tv.addString("NW"));
    EXPECT_FALSE(tv.addString("N"));
    EXPECT_TRUE(tv.addString("LIB"));
    EXPECT_EQ(tv.description(), metaf::TerrainVisibility::Description::MOUNTAINS_NOT_OBSCURED);
    EXPECT_EQ(tv.direction().size(), 3u);
    EXPECT_EQ(tv.direction().at(0).cardinal(), metaf::Direction::Cardinal::SW);
    EXPECT_EQ(tv.direction().at(1).cardinal(), metaf::Direction::Cardinal::W);
    EXPECT_EQ(tv.direction().at(2).cardinal(), metaf::Direction::Cardinal::NW);
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
