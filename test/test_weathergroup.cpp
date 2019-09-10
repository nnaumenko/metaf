/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(WeatherGroup, parseWeatherDz) {
	const auto wg = metaf::WeatherGroup::parse("DZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::DRIZZLE);
}

TEST(WeatherGroup, parseWeatherRa) {
	const auto wg = metaf::WeatherGroup::parse("RA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::RAIN);
}

TEST(WeatherGroup, parseWeatherSn) {
	const auto wg = metaf::WeatherGroup::parse("SN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::SNOW);
}

TEST(WeatherGroup, parseWeatherSg) {
	const auto wg = metaf::WeatherGroup::parse("SG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::SNOW_GRAINS);
}

TEST(WeatherGroup, parseWeatherIc) {
	const auto wg = metaf::WeatherGroup::parse("IC", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::ICE_CRYSTALS);
}

TEST(WeatherGroup, parseWeatherPl) {
	const auto wg = metaf::WeatherGroup::parse("PL", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::ICE_PELLETS);
}

TEST(WeatherGroup, parseWeatherGr) {
	const auto wg = metaf::WeatherGroup::parse("GR", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::HAIL);
}

TEST(WeatherGroup, parseWeatherGs) {
	const auto wg = metaf::WeatherGroup::parse("GS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::SMALL_HAIL);
}

TEST(WeatherGroup, parseWeatherUp) {
	const auto wg = metaf::WeatherGroup::parse("UP", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::UNDETERMINED);
}

TEST(WeatherGroup, parseWeatherBr) {
	const auto wg = metaf::WeatherGroup::parse("BR", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::MIST);
}

TEST(WeatherGroup, parseWeatherFg) {
	const auto wg = metaf::WeatherGroup::parse("FG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::FOG);
}

TEST(WeatherGroup, parseWeatherFu) {
	const auto wg = metaf::WeatherGroup::parse("FU", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::SMOKE);
}

TEST(WeatherGroup, parseWeatherVa) {
	const auto wg = metaf::WeatherGroup::parse("VA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::VOLCANIC_ASH);
}

TEST(WeatherGroup, parseWeatherDu) {
	const auto wg = metaf::WeatherGroup::parse("DU", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::DUST);
}

TEST(WeatherGroup, parseWeatherSa) {
	const auto wg = metaf::WeatherGroup::parse("SA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::SAND);
}

TEST(WeatherGroup, parseWeatherHz) {
	const auto wg = metaf::WeatherGroup::parse("HZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::HAZE);
}

TEST(WeatherGroup, parseWeatherPy) {
	const auto wg = metaf::WeatherGroup::parse("BLPY", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::BLOWING);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::SPRAY);
}

TEST(WeatherGroup, parseWeatherPo) {
	const auto wg = metaf::WeatherGroup::parse("PO", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::DUST_WHIRLS);
}

TEST(WeatherGroup, parseWeatherSq) {
	const auto wg = metaf::WeatherGroup::parse("SQ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::SQUALLS);
}

TEST(WeatherGroup, parseWeatherFc) {
	const auto wg = metaf::WeatherGroup::parse("FC", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::FUNNEL_CLOUD);
}

TEST(WeatherGroup, parseWeatherSs) {
	const auto wg = metaf::WeatherGroup::parse("SS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::SANDSTORM);
}

TEST(WeatherGroup, parseWeatherDs) {
	const auto wg = metaf::WeatherGroup::parse("DS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::DUSTSTORM);
}

TEST(WeatherGroup, parseWeatherNotReported) {
	const auto wg = metaf::WeatherGroup::parse("//", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::NOT_REPORTED);
}

TEST(WeatherGroup, parseWeatherMultiplePhenomena) {
	const auto wg1 = metaf::WeatherGroup::parse("RASN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 2u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::RAIN);
	EXPECT_EQ(weather1.at(1), metaf::WeatherGroup::Weather::SNOW);

	const auto wg2 = metaf::WeatherGroup::parse("RAGR", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 2u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::RAIN);
	EXPECT_EQ(weather2.at(1), metaf::WeatherGroup::Weather::HAIL);

	const auto wg3 = metaf::WeatherGroup::parse("DZSG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 2u);
	EXPECT_EQ(weather3.at(0), metaf::WeatherGroup::Weather::DRIZZLE);
	EXPECT_EQ(weather3.at(1), metaf::WeatherGroup::Weather::SNOW_GRAINS);

	const auto wg4 = metaf::WeatherGroup::parse("DSSS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg4->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather4 = wg4->weather();
	EXPECT_EQ(weather4.size(), 2u);
	EXPECT_EQ(weather4.at(0), metaf::WeatherGroup::Weather::DUSTSTORM);
	EXPECT_EQ(weather4.at(1), metaf::WeatherGroup::Weather::SANDSTORM);
}

TEST(WeatherGroup, parseDescriptorMi) {
	const auto wg = metaf::WeatherGroup::parse("MIFG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::SHALLOW);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::FOG);
}

TEST(WeatherGroup, parseDescriptorPr) {
	const auto wg = metaf::WeatherGroup::parse("PRFG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::PARTIAL);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::FOG);
}

TEST(WeatherGroup, parseDescriptorBc) {
	const auto wg = metaf::WeatherGroup::parse("BCFG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::PATCHES);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::FOG);
}

TEST(WeatherGroup, parseDescriptorDr) {
	const auto wg1 = metaf::WeatherGroup::parse("DRSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::LOW_DRIFTING);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::SNOW);

	const auto wg2 = metaf::WeatherGroup::parse("DRSA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::LOW_DRIFTING);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::SAND);

	const auto wg3 = metaf::WeatherGroup::parse("DRDU", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherGroup::Descriptor::LOW_DRIFTING);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::WeatherGroup::Weather::DUST);
}

TEST(WeatherGroup, parseDescriptorBl) {
	const auto wg1 = metaf::WeatherGroup::parse("BLSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::BLOWING);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::SNOW);

	const auto wg2 = metaf::WeatherGroup::parse("BLSA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::BLOWING);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::SAND);

	const auto wg3 = metaf::WeatherGroup::parse("BLDU", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherGroup::Descriptor::BLOWING);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::WeatherGroup::Weather::DUST);
}

TEST(WeatherGroup, parseDescriptorSh) {
	const auto wg1 = metaf::WeatherGroup::parse("SHRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::SHOWERS);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::RAIN);

	const auto wg2 = metaf::WeatherGroup::parse("SHSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::SHOWERS);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::SNOW);

	const auto wg3 = metaf::WeatherGroup::parse("SHSG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherGroup::Descriptor::SHOWERS);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::WeatherGroup::Weather::SNOW_GRAINS);

	const auto wg4 = metaf::WeatherGroup::parse("SHPL", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg4->descriptor(), metaf::WeatherGroup::Descriptor::SHOWERS);
	const auto weather4 = wg4->weather();
	EXPECT_EQ(weather4.size(), 1u);
	EXPECT_EQ(weather4.at(0), metaf::WeatherGroup::Weather::ICE_PELLETS);

}

TEST(WeatherGroup, parseDescriptorShMultiplePhenomena) {
	const auto wg1 = metaf::WeatherGroup::parse("SHRASN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::SHOWERS);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 2u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::RAIN);
	EXPECT_EQ(weather1.at(1), metaf::WeatherGroup::Weather::SNOW);

	const auto wg2 = metaf::WeatherGroup::parse("SHRAGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::SHOWERS);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 2u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::RAIN);
	EXPECT_EQ(weather2.at(1), metaf::WeatherGroup::Weather::SMALL_HAIL);
}

TEST(WeatherGroup, parseDescriptorTs) {
	const auto wg = metaf::WeatherGroup::parse("TSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::RAIN);
}

TEST(WeatherGroup, parseDescriptorTsStandalone) {
	const auto wg = metaf::WeatherGroup::parse("TS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wg->weather().size(), 0u);
}

TEST(WeatherGroup, parseDescriptorFz) {
	const auto wg1 = metaf::WeatherGroup::parse("FZDZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::FREEZING);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::DRIZZLE);

	const auto wg2 = metaf::WeatherGroup::parse("FZRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::FREEZING);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::RAIN);

	const auto wg3 = metaf::WeatherGroup::parse("FZFG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherGroup::Descriptor::FREEZING);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::WeatherGroup::Weather::FOG);
}

TEST(WeatherGroup, parseDescriptorFzMultiplePhenomena) {
	const auto wg = metaf::WeatherGroup::parse("FZRASN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::FREEZING);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 2u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::RAIN);
	EXPECT_EQ(weather.at(1), metaf::WeatherGroup::Weather::SNOW);
}

TEST(WeatherGroup, parseQualifierLightPrecipitation) {
	const auto wg = metaf::WeatherGroup::parse("-RA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::RAIN);
}

TEST(WeatherGroup, parseQualifierLightWithDescriptor) {
	const auto wg1 = metaf::WeatherGroup::parse("-TSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::RAIN);

	const auto wg2 = metaf::WeatherGroup::parse("-SHSG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::SHOWERS);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::SNOW_GRAINS);

	const auto wg3 = metaf::WeatherGroup::parse("-FZDZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherGroup::Descriptor::FREEZING);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::WeatherGroup::Weather::DRIZZLE);
}

TEST(WeatherGroup, parseQualifierLightMultiplePhenomena) {
	const auto wg = metaf::WeatherGroup::parse("-DZRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 2u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::DRIZZLE);
	EXPECT_EQ(weather.at(1), metaf::WeatherGroup::Weather::RAIN);
}

TEST(WeatherGroup, parseQualifierLightMultiplePhenomenaWithDescriptor) {
	const auto wg = metaf::WeatherGroup::parse("-TSRAGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 2u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::RAIN);
	EXPECT_EQ(weather.at(1), metaf::WeatherGroup::Weather::SMALL_HAIL);
}

TEST(WeatherGroup, parseQualifierHeavy) {
	const auto wg = metaf::WeatherGroup::parse("+RA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::RAIN);
}

TEST(WeatherGroup, parseQualifierHeavyWithDescriptor) {
	const auto wg1 = metaf::WeatherGroup::parse("+TSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::RAIN);

	const auto wg2 = metaf::WeatherGroup::parse("+SHSG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::SHOWERS);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::SNOW_GRAINS);

	const auto wg3 = metaf::WeatherGroup::parse("+FZDZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherGroup::Descriptor::FREEZING);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::WeatherGroup::Weather::DRIZZLE);
}

TEST(WeatherGroup, parseQualifierHeavyOtherPhenomena) {
	const auto wg1 = metaf::WeatherGroup::parse("+FC", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::FUNNEL_CLOUD);

	const auto wg2 = metaf::WeatherGroup::parse("+DS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::DUSTSTORM);

	const auto wg3 = metaf::WeatherGroup::parse("+SS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::WeatherGroup::Weather::SANDSTORM);
}

TEST(WeatherGroup, parseQualifierHeavyMultiplePhenomena) {
	const auto wg1 = metaf::WeatherGroup::parse("+DZRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 2u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::DRIZZLE);
	EXPECT_EQ(weather1.at(1), metaf::WeatherGroup::Weather::RAIN);

	const auto wg2 = metaf::WeatherGroup::parse("+DSSS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 2u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::DUSTSTORM);
	EXPECT_EQ(weather2.at(1), metaf::WeatherGroup::Weather::SANDSTORM);

}

TEST(WeatherGroup, parseQualifierHeavyMultiplePhenomenaWithDescriptor) {
	const auto wg = metaf::WeatherGroup::parse("+TSRAGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 2u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::RAIN);
	EXPECT_EQ(weather.at(1), metaf::WeatherGroup::Weather::SMALL_HAIL);
}



TEST(WeatherGroup, parseQualifierVicinityWeather) {
	const auto wg1 = metaf::WeatherGroup::parse("VCFG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::FOG);

	const auto wg2 = metaf::WeatherGroup::parse("VCVA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::VOLCANIC_ASH);

	const auto wg3 = metaf::WeatherGroup::parse("VCDS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::WeatherGroup::Weather::DUSTSTORM);

	const auto wg4 = metaf::WeatherGroup::parse("VCSS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg4->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather4 = wg4->weather();
	EXPECT_EQ(weather4.size(), 1u);
	EXPECT_EQ(weather4.at(0), metaf::WeatherGroup::Weather::SANDSTORM);

	const auto wg5 = metaf::WeatherGroup::parse("VCPO", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg5->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather5 = wg5->weather();
	EXPECT_EQ(weather5.size(), 1u);
	EXPECT_EQ(weather5.at(0), metaf::WeatherGroup::Weather::DUST_WHIRLS);
}

TEST(WeatherGroup, parseQualifierVicinityDescriptorOnly) {
	const auto wg1 = metaf::WeatherGroup::parse("VCSH", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::SHOWERS);
	EXPECT_EQ(wg1->weather().size(), 0u);

	const auto wg2 = metaf::WeatherGroup::parse("VCTS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wg2->weather().size(), 0u);
}

TEST(WeatherGroup, parseQualifierVicinityDescriptorAndWeather) {
	const auto wg1 = metaf::WeatherGroup::parse("VCBLSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::BLOWING);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::SNOW);

	const auto wg2 = metaf::WeatherGroup::parse("VCTSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::RAIN);
}

TEST(WeatherGroup, parseQualifierRecentWeather) {
	const auto wg = metaf::WeatherGroup::parse("RERA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::RAIN);
}

TEST(WeatherGroup, parseQualifierRecentWeatherWithDescriptor) {
	const auto wg1 = metaf::WeatherGroup::parse("RESHSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherGroup::Descriptor::SHOWERS);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::WeatherGroup::Weather::SNOW);

	const auto wg2 = metaf::WeatherGroup::parse("RETSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::WeatherGroup::Weather::RAIN);
}

TEST(WeatherGroup, parseQualifierRecentWeatherDescriptorOnly) {
	const auto wg = metaf::WeatherGroup::parse("RETS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wg->weather().size(), 0u);
}

TEST(WeatherGroup, parseRecentWeatherNotReported) {
	const auto wg = metaf::WeatherGroup::parse("RE//", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::NOT_REPORTED);
}

TEST(WeatherGroup, parseQualifierModerateRainAndHail) {
	// Rain requires MODERATE qualifier and Hail does not: MODERATE qualifier is used
	const auto wg1 = metaf::WeatherGroup::parse("RAGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
}

TEST(WeatherGroup, parseWeatherTaf) {
	const auto wg = metaf::WeatherGroup::parse("RA", metaf::ReportPart::TAF);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherGroup::Descriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::WeatherGroup::Weather::RAIN);
}

TEST(WeatherGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::RMK).has_value());
}

TEST(WeatherGroup, parseWrongFormat) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("/", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("///", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RES", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RERAS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("R0", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("0R", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSRAR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+TSRAR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCTSRAR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSRZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+TSRZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCTSRZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSRARZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+TSRARZ", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, contains) {
	const auto wgPlusTsRaGr = metaf::WeatherGroup::parse("+TSRAGR", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgPlusTsRaGr.has_value());
	EXPECT_TRUE(wgPlusTsRaGr->contains(metaf::WeatherGroup::Weather::RAIN));
	EXPECT_TRUE(wgPlusTsRaGr->contains(metaf::WeatherGroup::Weather::HAIL));
	EXPECT_FALSE(wgPlusTsRaGr->contains(metaf::WeatherGroup::Weather::SNOW));
	EXPECT_FALSE(wgPlusTsRaGr->contains(metaf::WeatherGroup::Weather::NOT_REPORTED));

	const auto wgNr = metaf::WeatherGroup::parse("//", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgNr.has_value());
	EXPECT_FALSE(wgNr->contains(metaf::WeatherGroup::Weather::RAIN));
	EXPECT_TRUE(wgNr->contains(metaf::WeatherGroup::Weather::NOT_REPORTED));
}

TEST(WeatherGroup, combine) {
	const std::string wgStr1("RERA");
	const std::string wgStr2("RE//");
	const std::string wgStr3("VCSH");
	const std::string wgStr4("TS");
	const std::string wgStr5("SHRASN");
	const std::string wgStr6("BLDU");
	const std::string wgStr7("FG");
	const std::string wgStr8("//");
	const std::string tStr1("RMK");
	const std::string tStr2("TEST");

	auto wg1 = metaf::WeatherGroup::parse(wgStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());

	auto wg2 = metaf::WeatherGroup::parse(wgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());

	auto wg3 = metaf::WeatherGroup::parse(wgStr3, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());

	auto wg4 = metaf::WeatherGroup::parse(wgStr4, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());

	auto wg5 = metaf::WeatherGroup::parse(wgStr5, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg5.has_value());

	auto wg6 = metaf::WeatherGroup::parse(wgStr6, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg6.has_value());

	auto wg7 = metaf::WeatherGroup::parse(wgStr7, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg7.has_value());

	auto wg8 = metaf::WeatherGroup::parse(wgStr8, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg8.has_value());

	EXPECT_EQ(wg1->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg2->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg3->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg4->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg5->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg6->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg7->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg8->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
}
