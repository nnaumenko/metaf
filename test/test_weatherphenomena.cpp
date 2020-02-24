/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: descriptors applicable to obscurations
// Purpose: to confirm that weather phenomena which contain descriptors used
// only with obscurations are parsed correctly, and incorrect strings are not
// parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, obscurations_descriptorMI) {
	const auto wp = metaf::WeatherPhenomena::fromString("MIFG");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::SHALLOW);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::FOG);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorMI_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("MIRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("MIFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("MISQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+MIFG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCMIFG").has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorPR) {
	const auto wp = metaf::WeatherPhenomena::fromString("PRFG");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::PARTIAL);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::FOG);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorPR_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("PRRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("PRFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("PRSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+PRFG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCPRFG").has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorBC) {
	const auto wp = metaf::WeatherPhenomena::fromString("BCFG");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::PATCHES);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::FOG);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorBC_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BCRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BCFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BCSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+BCFG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCBCFG").has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorDR) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("DRDU");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::LOW_DRIFTING);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DUST);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("DRSA");
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::LOW_DRIFTING);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::SAND);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("DRSN");
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::LOW_DRIFTING);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorDR_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("DRRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("DRDZ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("DRFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("DRFG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("DRSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+DRDU", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCDRDU", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCDRSA", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCDRSN", true).has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorBL) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("BLDU");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::BLOWING);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DUST);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("BLSA");
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::BLOWING);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::SAND);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("BLSN");
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::BLOWING);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("BLPY");
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::BLOWING);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::SPRAY);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorBL_qualifierVC) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCBLDU").has_value());
	const auto wp1 = metaf::WeatherPhenomena::fromString("VCBLDU", true);
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::BLOWING);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DUST);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCBLSA").has_value());
	const auto wp2 = metaf::WeatherPhenomena::fromString("VCBLSA", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::BLOWING);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::SAND);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCBLSN").has_value());
	const auto wp3 = metaf::WeatherPhenomena::fromString("VCBLSN", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::BLOWING);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorBL_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BLRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BLDZ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BLFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BLFG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BLSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+BLDU", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("-BLDU", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("REBLDU", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCBLPY", true).has_value());	
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("REBLPY", true).has_value());	
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorFZ) {
	const auto wp = metaf::WeatherPhenomena::fromString("FZFG");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::FOG);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorFZ_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZBR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZSA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+FZFG", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCFZFG").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: obscurations
// Purpose: to confirm that obscuration weather phenomena are parsed correctly,
// and incorrect strings are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, obscurations_BR) {
	const auto wp = metaf::WeatherPhenomena::fromString("BR");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::MIST);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_FG) {
	const auto wp = metaf::WeatherPhenomena::fromString("FG");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::FOG);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_FU) {
	const auto wp = metaf::WeatherPhenomena::fromString("FU");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::SMOKE);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_VA) {
	const auto wp = metaf::WeatherPhenomena::fromString("VA");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::VOLCANIC_ASH);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_DU) {
	const auto wp = metaf::WeatherPhenomena::fromString("DU");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::DUST);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_SA) {
	const auto wp = metaf::WeatherPhenomena::fromString("SA");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::SAND);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_HZ) {
	const auto wp = metaf::WeatherPhenomena::fromString("HZ");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::HAZE);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_qualifier_VC) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("VCFG", true);
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::FOG);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("VCVA", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::VOLCANIC_ASH);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("PY").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BRFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("HZDU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BRBR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VASADU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+DU", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("-FG", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("REFG", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCBR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCDU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCHZ").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: descriptors used alone
// Purpose: to confirm that descriptors which can be used alone, without any 
// weather phenomena are parsed correctly, and incorrect strings are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, other_descriptorTS) {
	const auto wp = metaf::WeatherPhenomena::fromString("TS");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp->weather().size(), 0u);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, other_descriptorTS_qualifierVC) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCTS").has_value());
	const auto wp = metaf::WeatherPhenomena::fromString("VCTS", true);
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp->weather().size(), 0u);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptor_TS_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSTS").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+TS", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("-TS", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZTS").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSBR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSFG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSDU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSSA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSHZ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSFG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSFC").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSDS").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSSS").has_value());
}

TEST(WeatherPhenomena_fromString, other_descriptorSH_qualifierVC) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSH").has_value());
	const auto wp = metaf::WeatherPhenomena::fromString("VCSH", true);
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp->weather().size(), 0u);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, obscurations_descriptorSH_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SH").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHSH").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+SH", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("-SH", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZSH").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHBR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHFG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHDU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHSA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHHZ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHFG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHFC").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHDS").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHSS").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: other phenomena
// Purpose: to confirm that 'other' weather phenomena (dust or sand whirls, 
// squalls, funnel cloud) are parsed correctly, and incorrect strings are not 
// parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, other_PO) {
	const auto wp = metaf::WeatherPhenomena::fromString("PO");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::DUST_WHIRLS);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, other_SQ) {
	const auto wp = metaf::WeatherPhenomena::fromString("SQ");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::SQUALLS);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, other_FC) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("FC");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::FUNNEL_CLOUD);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+FC").has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("+FC", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::FUNNEL_CLOUD);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());
}

TEST(WeatherPhenomena_fromString, other_qualifier_VC) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("VCPO", true);
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DUST_WHIRLS);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("VCFC", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::FUNNEL_CLOUD);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());
}

TEST(WeatherPhenomena_fromString, other_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FCSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FCPO").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSQ", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+PO", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+SQ", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("-FC", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RESQ", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+VCPO", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+VCFC", true).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: sandstorm and duststorm
// Purpose: to confirm that sandstorm and duststorm weather phenomena are 
// parsed correctly, and incorrect strings are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, other_DS) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("DS");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DUSTSTORM);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+DS").has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("+DS", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::DUSTSTORM);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());
}

TEST(WeatherPhenomena_fromString, other_SS) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("SS");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::SANDSTORM);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+SS").has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("+SS", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::SANDSTORM);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());
}

TEST(WeatherPhenomena_fromString, other_DSSS) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("DSSS");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DUSTSTORM);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::SANDSTORM);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+DSSS").has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("+DSSS", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DUSTSTORM);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::SANDSTORM);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());
}

TEST(WeatherPhenomena_fromString, other_SSDS) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("SSDS");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::SANDSTORM);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::DUSTSTORM);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+SSDS").has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("+SSDS", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::SANDSTORM);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::DUSTSTORM);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());
}

TEST(WeatherPhenomena_fromString, other_DS_SS_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("DSSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SSSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("-DS", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("-SS", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("DSDS").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SSSS").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+DSDS", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+SSSS", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("-DSSS", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("-SSDS", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("REDSSS", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RESSDS", true).has_value());
}

TEST(WeatherPhenomena_fromString, DS_SS_qualifier_vc) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("VCSS", true);
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::SANDSTORM);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("VCDS", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::DUSTSTORM);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: ice crystals
// Purpose: to confirm that 'ice crystals' weather phenomena are parsed 
// correctly, and incorrect strings are not parsed
// Note: Ice Crystals is technically precipitation but used in the same way as 
// obscurations
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_IC) {
	const auto wp = metaf::WeatherPhenomena::fromString("IC");
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp->weather().size(), 1u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::ICE_CRYSTALS);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

TEST(WeatherPhenomena_fromString, fromString_precipitation_IC_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("ICRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("ICBR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("ICPO").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+IC", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("-IC", true).has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("REIC", true).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: non-mixed precipitation
// Purpose: to confirm that single precipitation weather phenomena, with or 
// without intensity qualifier are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_DZ) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("DZ");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("DZ", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-DZ", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+DZ", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_RA) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("RA");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("RA", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-RA", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+RA", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_SN) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("SN");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("SN", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-SN", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+SN", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_SG) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("SG");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("SG", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-SG", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+SG", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_PL) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("PL");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("PL", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-PL", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+PL", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_GR) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("GR");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("GR", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-GR", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+GR", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_GS) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("GS");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("GS", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-GS", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+GS", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_UP) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("UP");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("UP", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-UP", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+UP", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: mixed precipitation
// Purpose: to confirm that mixed precipitation weather phenomena without 
// intensity qualifier (e.g. rain and snow mix) are parsed correctly, and 
// incorrect strings are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_mixed) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("DZSG");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("RAPL");
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 2u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("SNGS");
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 2u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp3->weather().at(1), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("RAGR");
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 2u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->weather().at(1), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("RAUP");
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp5->weather().size(), 2u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp5->weather().at(1), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());

	const auto wp6 = metaf::WeatherPhenomena::fromString("RASNGS");
	ASSERT_TRUE(wp6.has_value());
	EXPECT_EQ(wp6->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp6->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp6->weather().size(), 3u);
	EXPECT_EQ(wp6->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp6->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp6->weather().at(2), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp6->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp6->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_mixed_too_long) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("DZRASNPL").has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_mixed_duplicate) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNSN").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RARASN").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SNRASN").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RARA").has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_mixed_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNZZ").has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNSS").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNDS").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNBR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNFG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNFU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNHZ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNVA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNIC").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNDU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNDU").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNSA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNPY").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNPO").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNSQ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("RASNFC").has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_mixed_incorrect_descriptor) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("MIRAGR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("PRRAGR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BCRAGR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("DRRAGR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("BLRAGR").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: mixed precipitation with intensity qualifier
// Purpose: to confirm that mixed precipitation weather phenomena with 
// intensity qualifier (e.g. light rain and snow mix) are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_mixed_qualifier_moderate) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("DZSG", true);
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("RAPL", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 2u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("SNGS", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 2u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp3->weather().at(1), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("RAGR", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 2u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->weather().at(1), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("RAUP", true);
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp5->weather().size(), 2u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp5->weather().at(1), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());

	const auto wp6 = metaf::WeatherPhenomena::fromString("RASNGS", true);
	ASSERT_TRUE(wp6.has_value());
	EXPECT_EQ(wp6->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp6->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp6->weather().size(), 3u);
	EXPECT_EQ(wp6->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp6->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp6->weather().at(2), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp6->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp6->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_mixed_qualifier_light) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("-DZSG", true);
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("-RAPL", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 2u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-SNGS", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 2u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp3->weather().at(1), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("-RAGR", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 2u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->weather().at(1), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("-RAUP", true);
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp5->weather().size(), 2u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp5->weather().at(1), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());

	const auto wp6 = metaf::WeatherPhenomena::fromString("-RASNGS", true);
	ASSERT_TRUE(wp6.has_value());
	EXPECT_EQ(wp6->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp6->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp6->weather().size(), 3u);
	EXPECT_EQ(wp6->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp6->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp6->weather().at(2), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp6->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp6->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_mixed_qualifier_heavy) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("+DZSG", true);
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("+RAPL", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 2u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("+SNGS", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 2u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp3->weather().at(1), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+RAGR", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 2u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->weather().at(1), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("+RAUP", true);
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp5->weather().size(), 2u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp5->weather().at(1), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());

	const auto wp6 = metaf::WeatherPhenomena::fromString("+RASNGS", true);
	ASSERT_TRUE(wp6.has_value());
	EXPECT_EQ(wp6->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp6->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp6->weather().size(), 3u);
	EXPECT_EQ(wp6->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp6->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp6->weather().at(2), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp6->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp6->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_mixed_qualifier_recent) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("REDZSG", true);
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("RERAPL", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 2u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("RESNGS", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3->weather().size(), 2u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp3->weather().at(1), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("RERAGR", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp4->weather().size(), 2u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->weather().at(1), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("RERAUP", true);
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp5->weather().size(), 2u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp5->weather().at(1), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());

	const auto wp6 = metaf::WeatherPhenomena::fromString("RERASNGS", true);
	ASSERT_TRUE(wp6.has_value());
	EXPECT_EQ(wp6->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp6->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp6->weather().size(), 3u);
	EXPECT_EQ(wp6->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp6->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp6->weather().at(2), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp6->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp6->time().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: precipitation with TS descriptor
// Purpose: to confirm that precipitation weather phenomena with thunderstorm
// descriptor and without intensity qualifier are parsed correctly, and 
// incorrect strings are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_descriptor_ts) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("TSRA");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("TSSN");
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("TSPL");
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("TSDZ");
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("TSSG");
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp5->weather().size(), 1u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());

	const auto wp6 = metaf::WeatherPhenomena::fromString("TSGR");
	ASSERT_TRUE(wp6.has_value());
	EXPECT_EQ(wp6->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp6->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp6->weather().size(), 1u);
	EXPECT_EQ(wp6->weather().at(0), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp6->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp6->time().has_value());

	const auto wp7 = metaf::WeatherPhenomena::fromString("TSGS");
	ASSERT_TRUE(wp7.has_value());
	EXPECT_EQ(wp7->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp7->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp7->weather().size(), 1u);
	EXPECT_EQ(wp7->weather().at(0), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp7->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp7->time().has_value());

	const auto wp8 = metaf::WeatherPhenomena::fromString("TSUP");
	ASSERT_TRUE(wp8.has_value());
	EXPECT_EQ(wp8->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp8->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp8->weather().size(), 1u);
	EXPECT_EQ(wp8->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp8->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp8->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_descriptor_ts_mixed) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("TSRAGR");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("TSRASNPL");
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp2->weather().size(), 3u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp2->weather().at(2), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("TSRAGS");
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp3->weather().size(), 2u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp3->weather().at(1), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: precipitation with TS descriptor and intensity qualifier
// Purpose: to confirm that precipitation weather phenomena with thunderstorm
// descriptor and with intensity qualifier are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_descriptor_ts_qualifier) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("TSRA");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("TSRA", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-TSRA", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+TSRA", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("TSUP", true);
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp5->weather().size(), 1u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());

	const auto wp6 = metaf::WeatherPhenomena::fromString("+TSUP", true);
	ASSERT_TRUE(wp6.has_value());
	EXPECT_EQ(wp6->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp6->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp6->weather().size(), 1u);
	EXPECT_EQ(wp6->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp6->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp6->time().has_value());

	const auto wp7 = metaf::WeatherPhenomena::fromString("-TSUP", true);
	ASSERT_TRUE(wp7.has_value());
	EXPECT_EQ(wp7->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp7->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp7->weather().size(), 1u);
	EXPECT_EQ(wp7->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp7->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp7->time().has_value());

	const auto wp8 = metaf::WeatherPhenomena::fromString("RETSUP", true);
	ASSERT_TRUE(wp8.has_value());
	EXPECT_EQ(wp8->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp8->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp8->weather().size(), 1u);
	EXPECT_EQ(wp8->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp8->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp8->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_descriptor_ts_mixed_qualifier) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("TSRAGR");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("TSRAGR", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp2->weather().size(), 2u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-TSRAGR", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp3->weather().size(), 2u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp3->weather().at(1), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+TSRAGR", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp4->weather().size(), 2u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->weather().at(1), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: precipitation with SH descriptor
// Purpose: to confirm that precipitation weather phenomena with showery 
// precipitation descriptor and without intensity qualifier are parsed 
// correctly, and incorrect strings are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_descriptor_sh) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("SHRA");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("SHSN");
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("SHPL");
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("SHGR");
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::HAIL);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("SHGS");
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp5->weather().size(), 1u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());

	const auto wp6 = metaf::WeatherPhenomena::fromString("SHUP");
	ASSERT_TRUE(wp6.has_value());
	EXPECT_EQ(wp6->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp6->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp6->weather().size(), 1u);
	EXPECT_EQ(wp6->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp6->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp6->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_descriptorSH_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHDZ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHSG").has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSHDZ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSHRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSHSN").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSHPL").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSHSG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSHGR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSHGS").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCSHUP").has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_descriptorSH_mixed) {
	const auto wp = metaf::WeatherPhenomena::fromString("SHPLSG", true);
	ASSERT_TRUE(wp.has_value());
	EXPECT_EQ(wp->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp->weather().size(), 2u);
	EXPECT_EQ(wp->weather().at(0), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW_GRAINS);
	EXPECT_EQ(wp->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp->time().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: precipitation with SH descriptor and intensity qualifier
// Purpose: to confirm that precipitation weather phenomena with showery
// precipitation descriptor and with intensity qualifier are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_descriptor_sh_qualifier) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("SHRA", true);
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("+SHRA", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-SHRA", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("RESHRA", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("SHUP", true);
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp5->weather().size(), 1u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());

	const auto wp6 = metaf::WeatherPhenomena::fromString("+SHUP", true);
	ASSERT_TRUE(wp6.has_value());
	EXPECT_EQ(wp6->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp6->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp6->weather().size(), 1u);
	EXPECT_EQ(wp6->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp6->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp6->time().has_value());

	const auto wp7 = metaf::WeatherPhenomena::fromString("-SHUP", true);
	ASSERT_TRUE(wp7.has_value());
	EXPECT_EQ(wp7->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp7->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp7->weather().size(), 1u);
	EXPECT_EQ(wp7->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp7->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp7->time().has_value());

	const auto wp8 = metaf::WeatherPhenomena::fromString("RESHUP", true);
	ASSERT_TRUE(wp8.has_value());
	EXPECT_EQ(wp8->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp8->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp8->weather().size(), 1u);
	EXPECT_EQ(wp8->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp8->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp8->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_descriptor_sh_mixed_qualifier) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("SHRASN");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("SHRASN", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp2->weather().size(), 2u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-SHRASN", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp3->weather().size(), 2u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp3->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+SHRASN", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp4->weather().size(), 2u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("RESHRASN", true);
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp5->weather().size(), 2u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp5->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: precipitation with FZ descriptor
// Purpose: to confirm that precipitation weather phenomena with freezing 
// precipitation descriptor and without intensity qualifier are parsed 
// correctly and incorrect strings are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_descriptor_fz) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("FZRA", true);
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("FZDZ", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("FZUP", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_descriptor_fz_multpiple) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("FZRASN");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("FZDZPL");
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp2->weather().size(), 2u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_descriptor_fz_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZSN").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZSG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZPL").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZSG").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZGR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZGS").has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZSNPLGS").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZSNGR").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("FZSNSG").has_value());

	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCFZRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCFZDZ").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("VCFZUP").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: precipitation with FZ descriptor and intensity qualifier
// Purpose: to confirm that precipitation weather phenomena with freezing
// precipitation descriptor and with intensity qualifier are parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_descriptor_fz_qualifier) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("FZRA");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("FZRA", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-FZRA", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp3->weather().size(), 1u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+FZRA", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp4->weather().size(), 1u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("REFZRA", true);
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp5->weather().size(), 1u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());
}

TEST(WeatherPhenomena_fromString, precipitation_descriptor_fz_mixed_qualifier) {
	const auto wp1 = metaf::WeatherPhenomena::fromString("FZDZPL");
	ASSERT_TRUE(wp1.has_value());
	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp1->weather().size(), 2u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp1->weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1->time().has_value());

	const auto wp2 = metaf::WeatherPhenomena::fromString("FZDZPL", true);
	ASSERT_TRUE(wp2.has_value());
	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp2->weather().size(), 2u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2->time().has_value());

	const auto wp3 = metaf::WeatherPhenomena::fromString("-FZDZPL", true);
	ASSERT_TRUE(wp3.has_value());
	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp3->weather().size(), 2u);
	EXPECT_EQ(wp3->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp3->weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3->time().has_value());

	const auto wp4 = metaf::WeatherPhenomena::fromString("+FZDZPL", true);
	ASSERT_TRUE(wp4.has_value());
	EXPECT_EQ(wp4->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp4->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp4->weather().size(), 2u);
	EXPECT_EQ(wp4->weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp4->weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp4->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp4->time().has_value());

	const auto wp5 = metaf::WeatherPhenomena::fromString("FZUP", true);
	ASSERT_TRUE(wp5.has_value());
	EXPECT_EQ(wp5->qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp5->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp5->weather().size(), 1u);
	EXPECT_EQ(wp5->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp5->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp5->time().has_value());

	const auto wp6 = metaf::WeatherPhenomena::fromString("+FZUP", true);
	ASSERT_TRUE(wp6.has_value());
	EXPECT_EQ(wp6->qualifier(), metaf::WeatherPhenomena::Qualifier::HEAVY);
	EXPECT_EQ(wp6->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp6->weather().size(), 1u);
	EXPECT_EQ(wp6->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp6->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp6->time().has_value());

	const auto wp7 = metaf::WeatherPhenomena::fromString("-FZUP", true);
	ASSERT_TRUE(wp7.has_value());
	EXPECT_EQ(wp7->qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp7->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp7->weather().size(), 1u);
	EXPECT_EQ(wp7->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp7->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp7->time().has_value());

	const auto wp8 = metaf::WeatherPhenomena::fromString("REFZUP", true);
	ASSERT_TRUE(wp8.has_value());
	EXPECT_EQ(wp8->qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp8->descriptor(), metaf::WeatherPhenomena::Descriptor::FREEZING);
	EXPECT_EQ(wp8->weather().size(), 1u);
	EXPECT_EQ(wp8->weather().at(0), metaf::WeatherPhenomena::Weather::UNDETERMINED);
	EXPECT_EQ(wp8->event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp8->time().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromString() tests: incorrect combinations of descriptors and qualifiers
// Purpose: to confirm that incorrect combinations of descriptors and 
// qualifiers used with precipitation weather phenomena are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromString, precipitation_descriptor_qualifier_incorrect) {
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("TSSHRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("SHTSRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("ZZRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("ZRA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("++RA").has_value());
	EXPECT_FALSE(metaf::WeatherPhenomena::fromString("+-RA").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// fromWeatherBeginEndString() tests: first event (e.g. RAB1456)
// Purpose: to confirm that first weather event in a group (i.e. with time, 
// event type, and weather phenomena specified) are parsed correctly
// Note: fromWeatherBeginEndString() relies on fromString(), here assuming
// that fromString() is fully tested
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena_fromWeatherBeginEndString, first_event) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("120830");
	ASSERT_TRUE(reportTime.has_value());

	const auto wp1 = metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB1456", 
		metaf::MetafTime(), 
		metaf::WeatherPhenomena());
	ASSERT_TRUE(wp1.has_value());

	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::BEGINNING);
	ASSERT_TRUE(wp1->time().has_value());
	EXPECT_FALSE(wp1->time()->day().has_value());
	EXPECT_EQ(wp1->time()->hour(), 14u);
	EXPECT_EQ(wp1->time()->minute(), 56u);

	const auto wp2 = metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"SHRASNE0212", 
		metaf::MetafTime(), 
		metaf::WeatherPhenomena());
	ASSERT_TRUE(wp2.has_value());

	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp2->weather().size(), 2u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->weather().at(1), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::ENDING);
	ASSERT_TRUE(wp2->time().has_value());
	EXPECT_FALSE(wp2->time()->day().has_value());
	EXPECT_EQ(wp2->time()->hour(), 02u);
	EXPECT_EQ(wp2->time()->minute(), 12u);

	const auto wp3 = metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"TSB25", 
		reportTime.value(), 
		metaf::WeatherPhenomena());
	ASSERT_TRUE(wp3.has_value());

	EXPECT_EQ(wp3->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp3->descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp3->weather().size(), 0u);
	EXPECT_EQ(wp3->event(), metaf::WeatherPhenomena::Event::BEGINNING);
	ASSERT_TRUE(wp3->time().has_value());
	EXPECT_FALSE(wp3->time()->day().has_value());
	EXPECT_EQ(wp3->time()->hour(), reportTime->hour());
	EXPECT_EQ(wp3->time()->minute(), 25u);
}

///////////////////////////////////////////////////////////////////////////////
// fromWeatherBeginEndString() tests: subsequent event (e.g. E0812)
// Purpose: to confirm that subsequent weather events in a group (i.e. with 
// time and event type specified only) are parsed correctly
// Note: fromWeatherBeginEndString() relies on fromString(), here assuming
// that fromString() is fully tested
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena, fromWeatherBeginEndString_subsequent) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("120830");
	ASSERT_TRUE(reportTime.has_value());

	const auto previous = metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB0756", 
		metaf::MetafTime(), 
		metaf::WeatherPhenomena());
	ASSERT_TRUE(previous.has_value());

	const auto wp1 = metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"E0812",
		metaf::MetafTime(), 
		previous.value());
	ASSERT_TRUE(wp1.has_value());

	EXPECT_EQ(wp1->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1->weather().size(), 1u);
	EXPECT_EQ(wp1->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1->event(), metaf::WeatherPhenomena::Event::ENDING);
	ASSERT_TRUE(wp1->time().has_value());
	EXPECT_FALSE(wp1->time()->day().has_value());
	EXPECT_EQ(wp1->time()->hour(), 8u);
	EXPECT_EQ(wp1->time()->minute(), 12u);

	const auto wp2 = metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"E12", 
		reportTime.value(),
		previous.value());
	ASSERT_TRUE(wp2.has_value());

	EXPECT_EQ(wp2->qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2->descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2->weather().size(), 1u);
	EXPECT_EQ(wp2->weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp2->event(), metaf::WeatherPhenomena::Event::ENDING);
	ASSERT_TRUE(wp2->time().has_value());
	EXPECT_FALSE(wp2->time()->day().has_value());
	EXPECT_EQ(wp2->time()->hour(), 8u);
	EXPECT_EQ(wp2->time()->minute(), 12u);
}

///////////////////////////////////////////////////////////////////////////////
// fromWeatherBeginEndString() tests: incorrect groups 
// Purpose: to confirm that incorrect groups which cannot be parsed together
// or groups with incorrect formats are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena, fromWeatherBeginEndString_incorrectPrevious) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("120830");
	ASSERT_TRUE(reportTime.has_value());

	const auto previous = metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB0756", 
		metaf::MetafTime(), 
		metaf::WeatherPhenomena());
	ASSERT_TRUE(previous.has_value());

	ASSERT_TRUE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
			"B25",
			reportTime.value(), 
			previous.value()).has_value());

	EXPECT_FALSE(
		metaf::WeatherPhenomena::fromWeatherBeginEndString(
			"B25",
			reportTime.value(), 
			metaf::WeatherPhenomena()).has_value());
}

TEST(WeatherPhenomena, fromWeatherBeginEndString_incorrectFormat) {
	const auto rt = metaf::MetafTime::fromStringDDHHMM("120830");
	ASSERT_TRUE(rt.has_value());
	const auto reportTime = rt.value();

	const auto p = metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB0756", 
		metaf::MetafTime(), 
		metaf::WeatherPhenomena());
	ASSERT_TRUE(p.has_value());
	const auto previous = p.value();

	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"MIBRB1225", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"+RAB1225", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"VCSHB1225", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RERAB1225", reportTime, previous));

	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"ZZB1225", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAZ1225", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RABB1225", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RA1225", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB", reportTime, previous));

	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB01225", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB225", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB025", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB5", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB1A25", reportTime, previous));
	EXPECT_FALSE(metaf::WeatherPhenomena::fromWeatherBeginEndString(
		"RAB0A", reportTime, previous));
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherPhenomena, isValidTrueOther) {
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("MIFG", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("PRFG", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("BCFG", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("FG", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("DRDU", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("BLDU", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("DU", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("DRSA", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("BLSA", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("SA", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("DRSN", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("BLSN", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("BLPY", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("TS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("IC", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("BR", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("FU", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VA", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("HZ", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("PO", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("SQ", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("FC", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("DS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("SS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("DSSS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("SSDS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("+FC", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("+DS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("+SS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("+DSSS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("+SSDS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCSH", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCTS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCFG", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCVA", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCFC", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCPO", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCBLDU", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCBLSA", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCBLSN", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCDS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("VCSS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("RA", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("SN", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("DZ", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("SG", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("PL", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("GR", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("GS", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("UP", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("+TSRAGR", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("-SHRAPL", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("SHUP", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("TSUP", true)->isValid());
	EXPECT_TRUE(metaf::WeatherPhenomena::fromString("RERAPL", true)->isValid());
}

TEST(WeatherPhenomena, isValidFalseEmpty) {
	EXPECT_FALSE(metaf::WeatherPhenomena().isValid());
}
