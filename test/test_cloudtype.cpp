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
// Constructor test
// Purpose: to confirm that default consturctor correctly initialises the data.
///////////////////////////////////////////////////////////////////////////////

TEST(CloudType, constructor) {
	const auto ct = metaf::CloudType();
	EXPECT_EQ(ct.type(), metaf::CloudType::Type::NOT_REPORTED);
	EXPECT_FALSE(ct.height().isReported());
	EXPECT_EQ(ct.okta(), 0u);
	EXPECT_FALSE(ct.isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Parse format without base height
// Purpose: to confirm that all cloud types and obscurations are parsed 
// correctly from the format with type and okta (e.g. BLSN4 or SC3).
///////////////////////////////////////////////////////////////////////////////

TEST(CloudType, parseCumulonimbusNoHeight) {
	const auto ct = metaf::CloudType::fromString("CB5");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::CUMULONIMBUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 5u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseToweringCumulusNoHeight) {
	const auto ct = metaf::CloudType::fromString("TCU3");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::TOWERING_CUMULUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 3u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseCumulusNoHeight) {
	const auto ct = metaf::CloudType::fromString("CU2");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::CUMULUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 2u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseCumulusFractusNoHeight) {
	const auto ct = metaf::CloudType::fromString("CF1");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::CUMULUS_FRACTUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 1u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseStratocumulusNoHeight) {
	const auto ct = metaf::CloudType::fromString("SC7");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::STRATOCUMULUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 7u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseNimbostratusNoHeight) {
	const auto ct = metaf::CloudType::fromString("NS8");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::NIMBOSTRATUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 8u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseStratusNoHeight) {
	const auto ct = metaf::CloudType::fromString("ST8");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::STRATUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 8u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseStratusFractusNoHeight) {
	const auto ct = metaf::CloudType::fromString("SF4");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::STRATUS_FRACTUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 4u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseAltostratusNoHeight) {
	const auto ct = metaf::CloudType::fromString("AS8");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::ALTOSTRATUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 8u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseAltocumulusNoHeight) {
	const auto ct = metaf::CloudType::fromString("AC6");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::ALTOCUMULUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 6u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseAltocumulusCastellanusNoHeight) {
	const auto ct = metaf::CloudType::fromString("ACC6");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::ALTOCUMULUS_CASTELLANUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 6u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseCirrusNoHeight) {
	const auto ct = metaf::CloudType::fromString("CI3");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::CIRRUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 3u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseCirrostratusNoHeight) {
	const auto ct = metaf::CloudType::fromString("CS8");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::CIRROSTRATUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 8u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseCirrocumulusNoHeight) {
	const auto ct = metaf::CloudType::fromString("CC5");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::CIRROCUMULUS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 5u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseBlowingSnowNoHeight) {
	const auto ct = metaf::CloudType::fromString("BLSN4");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::BLOWING_SNOW);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 4u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseBlowingDustNoHeight) {
	const auto ct = metaf::CloudType::fromString("BLDU3");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::BLOWING_DUST);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 3u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseBlowingSandNoHeight) {
	const auto ct = metaf::CloudType::fromString("BLSA3");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::BLOWING_SAND);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 3u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseIceCrystalsNoHeight) {
	const auto ct = metaf::CloudType::fromString("IC2");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::ICE_CRYSTALS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 2u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseRainNoHeight) {
	const auto ct = metaf::CloudType::fromString("RA3");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::RAIN);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 3u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseDrizzleNoHeight) {
	const auto ct = metaf::CloudType::fromString("DZ1");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::DRIZZLE);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 1u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseSnowNoHeight) {
	const auto ct = metaf::CloudType::fromString("SN6");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::SNOW);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 6u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseIcePelletsNoHeight) {
	const auto ct = metaf::CloudType::fromString("PL5");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::ICE_PELLETS);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 5u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseSmokeNoHeight) {
	const auto ct = metaf::CloudType::fromString("FU8");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::SMOKE);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 8u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseFogNoHeight) {
	const auto ct = metaf::CloudType::fromString("FG3");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::FOG);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 3u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseMistNoHeight) {
	const auto ct = metaf::CloudType::fromString("BR3");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::MIST);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 3u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseHazeNoHeight) {
	const auto ct = metaf::CloudType::fromString("HZ7");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::HAZE);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 7u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseInvalidOktaNoHeight) {
	const auto ct1 = metaf::CloudType::fromString("SC0");
	ASSERT_TRUE(ct1.has_value());
	EXPECT_EQ(ct1->type(), metaf::CloudType::Type::STRATOCUMULUS);
	EXPECT_FALSE(ct1->height().isReported());
	EXPECT_EQ(ct1->okta(), 0u);
	EXPECT_FALSE(ct1->isValid());

	const auto ct2 = metaf::CloudType::fromString("SC9");
	ASSERT_TRUE(ct2.has_value());
	EXPECT_EQ(ct2->type(), metaf::CloudType::Type::STRATOCUMULUS);
	EXPECT_FALSE(ct2->height().isReported());
	EXPECT_EQ(ct2->okta(), 9u);
	EXPECT_FALSE(ct2->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Parse format with base height
// Purpose: to confirm that all cloud types are parsed correctly from the 
// format with okta, type and base height (8NS070 or 3TCU022) and obscurations
// are not parsed.
///////////////////////////////////////////////////////////////////////////////

TEST(CloudType, parseCumulonimbusWithHeight) {
	const auto ct = metaf::CloudType::fromString("1CB025");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::CUMULONIMBUS);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 1u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseToweringCumulusWithHeight) {
	const auto ct = metaf::CloudType::fromString("1TCU020");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::TOWERING_CUMULUS);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 2000, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 1u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseCumulusWithHeight) {
	const auto ct = metaf::CloudType::fromString("6CU020");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::CUMULUS);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 2000, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 6u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseStratocumulusWithHeight) {
	const auto ct = metaf::CloudType::fromString("6SC060");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::STRATOCUMULUS);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 6000, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 6u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseNimbostratusWithHeight) {
	const auto ct = metaf::CloudType::fromString("8NS035");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::NIMBOSTRATUS);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 3500, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 8u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseStratusWithHeight) {
	const auto ct = metaf::CloudType::fromString("1ST005");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::STRATUS);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 500, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 1u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseAltostratusWithHeight) {
	const auto ct = metaf::CloudType::fromString("8AS100");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::ALTOSTRATUS);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 10000, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 8u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseAltocumulusWithHeight) {
	const auto ct = metaf::CloudType::fromString("7AC150");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::ALTOCUMULUS);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 15000, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 7u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseCirrusWithHeight) {
	const auto ct = metaf::CloudType::fromString("5CI230");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::CIRRUS);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 23000, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 5u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseInvalidOktaWithHeight) {
	const auto ct1 = metaf::CloudType::fromString("0CU025");
	ASSERT_TRUE(ct1.has_value());
	EXPECT_EQ(ct1->type(), metaf::CloudType::Type::CUMULUS);
	EXPECT_TRUE(ct1->height().isReported());
	EXPECT_EQ(ct1->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct1->height().distance().has_value());
	EXPECT_NEAR(ct1->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(ct1->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct1->okta(), 0u);
	EXPECT_FALSE(ct1->isValid());

	const auto ct2 = metaf::CloudType::fromString("9CU025");
	ASSERT_TRUE(ct2.has_value());
	EXPECT_EQ(ct2->type(), metaf::CloudType::Type::CUMULUS);
	EXPECT_TRUE(ct2->height().isReported());
	EXPECT_EQ(ct2->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct2->height().distance().has_value());
	EXPECT_NEAR(ct2->height().distance().value(), 2500, heightMargin);
	EXPECT_EQ(ct2->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct2->okta(), 9u);
	EXPECT_FALSE(ct2->isValid());
}

TEST(CloudType, parseGroundBasedWithHeight) {
	const auto ct = metaf::CloudType::fromString("6CU000");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::CUMULUS);
	EXPECT_TRUE(ct->height().isReported());
	EXPECT_EQ(ct->height().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(ct->height().distance().has_value());
	EXPECT_NEAR(ct->height().distance().value(), 0, heightMargin);
	EXPECT_EQ(ct->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(ct->okta(), 6u);
	EXPECT_TRUE(ct->isValid());
}

TEST(CloudType, parseObsurations) {
	EXPECT_FALSE(metaf::CloudType::fromString("6BLSN000").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6BLDU000").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6BLSA000").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6IC000").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6RA000").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6SN000").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6DZ000").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6FU000").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6FG000").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6BR000").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6HZ000").has_value());

	EXPECT_FALSE(metaf::CloudType::fromString("6BLSN001").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6BLDU001").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6BLSA001").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6IC001").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6RA001").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6SN001").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6DZ001").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6FU001").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6FG001").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6BR001").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("6HZ001").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Parse obscurations
// Purpose: to confirm that all ground-based or aloft obscurations included 
// in remarks are parsed correctly.
///////////////////////////////////////////////////////////////////////////////

TEST(CloudType, parseObscurationBlowingSnow) {
	const auto ct = metaf::CloudType::fromStringObscuration("BLSN");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::BLOWING_SNOW);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 0u);
	EXPECT_FALSE(ct->isValid());
}

TEST(CloudType, parseObscurationBlowingDust) {
	const auto ct = metaf::CloudType::fromStringObscuration("BLDU");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::BLOWING_DUST);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 0u);
	EXPECT_FALSE(ct->isValid());
}

TEST(CloudType, parseObscurationBlowingSand) {
	const auto ct = metaf::CloudType::fromStringObscuration("BLSA");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::BLOWING_SAND);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 0u);
	EXPECT_FALSE(ct->isValid());
}

TEST(CloudType, parseObscurationFog) {
	const auto ct = metaf::CloudType::fromStringObscuration("FG");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::FOG);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 0u);
	EXPECT_FALSE(ct->isValid());
}

TEST(CloudType, parseObscurationSmoke) {
	const auto ct = metaf::CloudType::fromStringObscuration("FU");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::SMOKE);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 0u);
	EXPECT_FALSE(ct->isValid());
}

TEST(CloudType, parseObscurationVolcanicAsh) {
	const auto ct = metaf::CloudType::fromStringObscuration("VA");
	ASSERT_TRUE(ct.has_value());
	EXPECT_EQ(ct->type(), metaf::CloudType::Type::VOLCANIC_ASH);
	EXPECT_FALSE(ct->height().isReported());
	EXPECT_EQ(ct->okta(), 0u);
	EXPECT_FALSE(ct->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Parse invalid format
// Purpose: to confirm that invalid formats would not parse.
///////////////////////////////////////////////////////////////////////////////

TEST(CloudType, parseOther) {
	EXPECT_FALSE(metaf::CloudType::fromString("").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("1").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("41").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("022").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("Z").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("CB").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("TCU").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("BLSN").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("ZCB").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("ZTCU").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("ZBLSN").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("ZB4").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("ZCU4").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("ZLSN4").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("/4").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("//4").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("///4").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("////4").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("CB/").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("TCU/").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("BLSN/").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("3CB///").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("/CB015").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("3//015").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("3///015").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("ZCB015").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("3CBZ15").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("3CB01Z").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("3CB15").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("CB015").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("03CB015").has_value());
	EXPECT_FALSE(metaf::CloudType::fromString("3CB0015").has_value());
}

