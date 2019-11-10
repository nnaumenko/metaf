/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"


// TODO: add test for // and RE// using weatherPhenomena()

///////////////////////////////////////////////////////////////////////////////

// TODO: remove the following section when transitioning to version 4.0.0
// since it duplicates WeatherPhenomena tests now

TEST(WeatherGroup, parseWeatherDz) {
	const auto wg = metaf::WeatherGroup::parse("DZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::DRIZZLE);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherRa) {
	const auto wg = metaf::WeatherGroup::parse("RA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::RAIN);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherSn) {
	const auto wg = metaf::WeatherGroup::parse("SN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::SNOW);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherSg) {
	const auto wg = metaf::WeatherGroup::parse("SG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::SNOW_GRAINS);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherIc) {
	const auto wg = metaf::WeatherGroup::parse("IC", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::ICE_CRYSTALS);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherIcCombination) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("BRIC", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, parseWeatherPl) {
	const auto wg = metaf::WeatherGroup::parse("PL", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::ICE_PELLETS);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherGr) {
	const auto wg = metaf::WeatherGroup::parse("GR", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::HAIL);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherGs) {
	const auto wg = metaf::WeatherGroup::parse("GS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::SMALL_HAIL);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherUp) {
	const auto wg = metaf::WeatherGroup::parse("UP", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::UNDETERMINED);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherBr) {
	const auto wg = metaf::WeatherGroup::parse("BR", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::MIST);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherFg) {
	const auto wg = metaf::WeatherGroup::parse("FG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::FOG);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherFu) {
	const auto wg = metaf::WeatherGroup::parse("FU", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::SMOKE);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherVa) {
	const auto wg = metaf::WeatherGroup::parse("VA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::VOLCANIC_ASH);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherDu) {
	const auto wg = metaf::WeatherGroup::parse("DU", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::DUST);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherSa) {
	const auto wg = metaf::WeatherGroup::parse("SA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::SAND);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherHz) {
	const auto wg = metaf::WeatherGroup::parse("HZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::HAZE);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherPy) {
	const auto wg = metaf::WeatherGroup::parse("BLPY", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::BLOWING);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::SPRAY);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherPo) {
	const auto wg = metaf::WeatherGroup::parse("PO", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::DUST_WHIRLS);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherSq) {
	const auto wg = metaf::WeatherGroup::parse("SQ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::SQUALLS);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherFc) {
	const auto wg = metaf::WeatherGroup::parse("FC", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::FUNNEL_CLOUD);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherSs) {
	const auto wg = metaf::WeatherGroup::parse("SS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::SANDSTORM);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherDs) {
	const auto wg = metaf::WeatherGroup::parse("DS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::DUSTSTORM);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherNotReported) {
	const auto wg = metaf::WeatherGroup::parse("//", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::NOT_REPORTED);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseWeatherMultiplePhenomena) {
	const auto wg1 = metaf::WeatherGroup::parse("RASN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 2u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::RAIN);
	EXPECT_EQ(weather1.at(1), metaf::Weather::SNOW);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("RAGR", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 2u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::RAIN);
	EXPECT_EQ(weather2.at(1), metaf::Weather::HAIL);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("DZSG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 2u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::DRIZZLE);
	EXPECT_EQ(weather3.at(1), metaf::Weather::SNOW_GRAINS);
	EXPECT_TRUE(wg3->isValid());

	const auto wg4 = metaf::WeatherGroup::parse("DSSS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg4->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather4 = wg4->weather();
	EXPECT_EQ(weather4.size(), 2u);
	EXPECT_EQ(weather4.at(0), metaf::Weather::DUSTSTORM);
	EXPECT_EQ(weather4.at(1), metaf::Weather::SANDSTORM);
	EXPECT_TRUE(wg4->isValid());
}

TEST(WeatherGroup, duplicatePhenomena) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("FGFG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RASNRA", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, phenomenaAllowedAlone) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("ICBR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BRDZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FGDZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FUDZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VAFU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FUDU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FUSA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BRHZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("DZPY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("HZPO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("SNSQ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BRFC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("DSDU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("SSSA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("DSSSFU", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, parseDescriptorMi) {
	const auto wg = metaf::WeatherGroup::parse("MIFG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::SHALLOW);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::FOG);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseInvalidDescriptorMi) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIDZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIRA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MISN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MISG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIIC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIPL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIGR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIGS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIUP", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIBR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIFU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIVA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIDU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MISA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIHZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIPY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIPO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MISQ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIFC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MISS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIDS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+MI", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-MI", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCMI", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MI", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIFGFU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MIBLSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCMIFG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+MIFG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-MIFG", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, parseDescriptorPr) {
	const auto wg = metaf::WeatherGroup::parse("PRFG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::PARTIAL);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::FOG);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseInvalidDescriptorPartial) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRDZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRRA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRSG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRIC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRPL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRGR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRGS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRUP", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRBR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRFU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRVA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRDU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRSA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRHZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRPY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRPO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRSQ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRFC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRSS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRDS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+PR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-PR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCPR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRFGFU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PRBLSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCPRFG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+PRFG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-PRFG", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, parseDescriptorBc) {
	const auto wg = metaf::WeatherGroup::parse("BCFG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::PATCHES);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::FOG);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseInvalidDescriptorPatches) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCDZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCRA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCSG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCIC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCPL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCGR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCGS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCUP", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCBR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCFU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCVA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCDU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCSA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCHZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCPY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCPO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCSQ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCFC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCSS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCDS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+BC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-BC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCBC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCFGDZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BCBLSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCPRFG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+PRFG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-PRFG", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, parseDescriptorDr) {
	const auto wg1 = metaf::WeatherGroup::parse("DRSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::LOW_DRIFTING);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::SNOW);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("DRSA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::LOW_DRIFTING);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::SAND);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("DRDU", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::LOW_DRIFTING);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::DUST);
	EXPECT_TRUE(wg3->isValid());
}

TEST(WeatherGroup, parseInvalidDescriptorDr) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRDZ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRRA", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRSG", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRIC", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRPL", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRGR", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRGS", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRUP", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRBR", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRFG", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRFU", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRVA", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRHZ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRPY", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRPO", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRSQ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRFC", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRSS", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRDS", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRRASN", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("DRSHSN", metaf::ReportPart::METAR).has_value());	
}

TEST(WeatherGroup, parseDescriptorBl) {
	const auto wg1 = metaf::WeatherGroup::parse("BLSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::BLOWING);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::SNOW);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("BLSA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::BLOWING);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::SAND);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("BLDU", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::BLOWING);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::DUST);
	EXPECT_TRUE(wg3->isValid());

	//For BLPY see parseWeatherPy
}

TEST(WeatherGroup, parseInvalidDescriptorBl) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLDZ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLRA", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLSG", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLIC", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLPL", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLGR", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLGS", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLUP", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLBR", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLFG", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLFU", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLVA", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLHZ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLPO", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLSQ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLFC", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLSS", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLDS", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLRASN", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("BLSHSN", metaf::ReportPart::METAR).has_value());	
}

TEST(WeatherGroup, parseDescriptorSh) {
	const auto wg1 = metaf::WeatherGroup::parse("SHRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::RAIN);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("SHSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::SNOW);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("SHPL", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::ICE_PELLETS);
	EXPECT_TRUE(wg3->isValid());

	const auto wg4 = metaf::WeatherGroup::parse("SHGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg4->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather4 = wg4->weather();
	EXPECT_EQ(weather4.size(), 1u);
	EXPECT_EQ(weather4.at(0), metaf::Weather::SMALL_HAIL);
	EXPECT_TRUE(wg4->isValid());

	const auto wg5 = metaf::WeatherGroup::parse("SHGR", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg5->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather5 = wg5->weather();
	EXPECT_EQ(weather5.size(), 1u);
	EXPECT_EQ(weather5.at(0), metaf::Weather::HAIL);
	EXPECT_TRUE(wg5->isValid());

	const auto wg6 = metaf::WeatherGroup::parse("SHUP", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg6->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather6 = wg6->weather();
	EXPECT_EQ(weather6.size(), 1u);
	EXPECT_EQ(weather6.at(0), metaf::Weather::UNDETERMINED);
	EXPECT_TRUE(wg6->isValid());
}

TEST(WeatherGroup, parseDescriptorShMultiplePhenomena) {
	const auto wg1 = metaf::WeatherGroup::parse("SHRASN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 2u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::RAIN);
	EXPECT_EQ(weather1.at(1), metaf::Weather::SNOW);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("SHRAGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 2u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::RAIN);
	EXPECT_EQ(weather2.at(1), metaf::Weather::SMALL_HAIL);
	EXPECT_TRUE(wg2->isValid());
}

TEST(WeatherGroup, parseInvalidDescriptorSh) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHDZ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHSG", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHIC", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHBR", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHFG", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHFU", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHVA", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHDU", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHSA", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHHZ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHPY", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHPO", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHSQ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHFC", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHDS", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("SHSS", metaf::ReportPart::METAR).has_value());	
}

TEST(WeatherGroup, parseDescriptorTs) {
	const auto wg0 = metaf::WeatherGroup::parse("TSDZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg0.has_value());
	EXPECT_EQ(wg0->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg0->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather0 = wg0->weather();
	EXPECT_EQ(weather0.size(), 1u);
	EXPECT_EQ(weather0.at(0), metaf::Weather::DRIZZLE);
	EXPECT_TRUE(wg0->isValid());

	const auto wg1 = metaf::WeatherGroup::parse("TSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::RAIN);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("TSSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::SNOW);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("TSPL", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::ICE_PELLETS);
	EXPECT_TRUE(wg3->isValid());

	const auto wg4 = metaf::WeatherGroup::parse("TSGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg4->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather4 = wg4->weather();
	EXPECT_EQ(weather4.size(), 1u);
	EXPECT_EQ(weather4.at(0), metaf::Weather::SMALL_HAIL);
	EXPECT_TRUE(wg4->isValid());

	const auto wg5 = metaf::WeatherGroup::parse("TSGR", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg5->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather5 = wg5->weather();
	EXPECT_EQ(weather5.size(), 1u);
	EXPECT_EQ(weather5.at(0), metaf::Weather::HAIL);
	EXPECT_TRUE(wg5->isValid());

	const auto wg6 = metaf::WeatherGroup::parse("TSSG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg6->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather6 = wg6->weather();
	EXPECT_EQ(weather6.size(), 1u);
	EXPECT_EQ(weather6.at(0), metaf::Weather::SNOW_GRAINS);
	EXPECT_TRUE(wg6->isValid());

	const auto wg7 = metaf::WeatherGroup::parse("TSUP", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg7.has_value());
	EXPECT_EQ(wg7->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg7->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather7 = wg7->weather();
	EXPECT_EQ(weather7.size(), 1u);
	EXPECT_EQ(weather7.at(0), metaf::Weather::UNDETERMINED);
	EXPECT_TRUE(wg7->isValid());
}

TEST(WeatherGroup, parseDescriptorTsStandalone) {
	const auto wg = metaf::WeatherGroup::parse("TS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	EXPECT_EQ(wg->weather().size(), 0u);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseDescriptorTsMultiplePhenomena) {
	const auto wg1 = metaf::WeatherGroup::parse("TSRASN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 2u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::RAIN);
	EXPECT_EQ(weather1.at(1), metaf::Weather::SNOW);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("TSRADZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 2u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::RAIN);
	EXPECT_EQ(weather2.at(1), metaf::Weather::DRIZZLE);
	EXPECT_TRUE(wg2->isValid());
}

TEST(WeatherGroup, parseInvalidDescriptorTs) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSIC", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSBR", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSFG", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSFU", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSVA", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSDU", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSSA", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSHZ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSPY", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSPO", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSSQ", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSFC", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSDS", metaf::ReportPart::METAR).has_value());	
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSSS", metaf::ReportPart::METAR).has_value());	
}

TEST(WeatherGroup, parseDescriptorFz) {
	const auto wg1 = metaf::WeatherGroup::parse("FZDZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::FREEZING);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::DRIZZLE);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("FZRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::FREEZING);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::RAIN);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("FZFG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::NONE);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::FREEZING);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::FOG);
	EXPECT_TRUE(wg3->isValid());

	const auto wg4 = metaf::WeatherGroup::parse("FZUP", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg4->descriptor(), metaf::WeatherDescriptor::FREEZING);
	const auto weather4 = wg4->weather();
	EXPECT_EQ(weather4.size(), 1u);
	EXPECT_EQ(weather4.at(0), metaf::Weather::UNDETERMINED);
	EXPECT_TRUE(wg4->isValid());
}

TEST(WeatherGroup, parseDescriptorFzMultiplePhenomena) {
	const auto wg = metaf::WeatherGroup::parse("FZRASN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::FREEZING);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 2u);
	EXPECT_EQ(weather.at(0), metaf::Weather::RAIN);
	EXPECT_EQ(weather.at(1), metaf::Weather::SNOW);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseInvalidDescriptorFz) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZSG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZIC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZPL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZGR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZGS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZBR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZFU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZVA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZDU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZSA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZHZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZPY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZPO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZSQ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZFC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZDS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("FZSS", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, parseQualifierLightPrecipitation) {
	const auto wg1 = metaf::WeatherGroup::parse("-DZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::DRIZZLE);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("-RA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::RAIN);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("-SN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::SNOW);
	EXPECT_TRUE(wg3->isValid());

	const auto wg4 = metaf::WeatherGroup::parse("-SG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg4->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather4 = wg4->weather();
	EXPECT_EQ(weather4.size(), 1u);
	EXPECT_EQ(weather4.at(0), metaf::Weather::SNOW_GRAINS);
	EXPECT_TRUE(wg4->isValid());

	const auto wg5 = metaf::WeatherGroup::parse("-PL", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg5->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather5 = wg5->weather();
	EXPECT_EQ(weather5.size(), 1u);
	EXPECT_EQ(weather5.at(0), metaf::Weather::ICE_PELLETS);
	EXPECT_TRUE(wg5->isValid());

	const auto wg6 = metaf::WeatherGroup::parse("-UP", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg6->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather6 = wg6->weather();
	EXPECT_EQ(weather6.size(), 1u);
	EXPECT_EQ(weather6.at(0), metaf::Weather::UNDETERMINED);
	EXPECT_TRUE(wg6->isValid());
}

TEST(WeatherGroup, parseQualifierLightWithDescriptor) {
	const auto wg1 = metaf::WeatherGroup::parse("-TSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::RAIN);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("-SHPL", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::ICE_PELLETS);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("-FZDZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::FREEZING);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::DRIZZLE);
	EXPECT_TRUE(wg3->isValid());
}

TEST(WeatherGroup, parseQualifierLightMultiplePhenomena) {
	const auto wg = metaf::WeatherGroup::parse("-DZRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 2u);
	EXPECT_EQ(weather.at(0), metaf::Weather::DRIZZLE);
	EXPECT_EQ(weather.at(1), metaf::Weather::RAIN);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseQualifierLightMultiplePhenomenaWithDescriptor) {
	const auto wg = metaf::WeatherGroup::parse("-TSRAGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::LIGHT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 2u);
	EXPECT_EQ(weather.at(0), metaf::Weather::RAIN);
	EXPECT_EQ(weather.at(1), metaf::Weather::SMALL_HAIL);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseQualifierHeavy) {
	const auto wg1 = metaf::WeatherGroup::parse("+DZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::DRIZZLE);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("+RA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::RAIN);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("+SN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::SNOW);
	EXPECT_TRUE(wg3->isValid());

	const auto wg4 = metaf::WeatherGroup::parse("+SG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg4->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather4 = wg4->weather();
	EXPECT_EQ(weather4.size(), 1u);
	EXPECT_EQ(weather4.at(0), metaf::Weather::SNOW_GRAINS);
	EXPECT_TRUE(wg4->isValid());

	const auto wg5 = metaf::WeatherGroup::parse("+PL", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg5->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather5 = wg5->weather();
	EXPECT_EQ(weather5.size(), 1u);
	EXPECT_EQ(weather5.at(0), metaf::Weather::ICE_PELLETS);
	EXPECT_TRUE(wg5->isValid());

	const auto wg6 = metaf::WeatherGroup::parse("+UP", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg6->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather6 = wg6->weather();
	EXPECT_EQ(weather6.size(), 1u);
	EXPECT_EQ(weather6.at(0), metaf::Weather::UNDETERMINED);
	EXPECT_TRUE(wg6->isValid());

	const auto wg7 = metaf::WeatherGroup::parse("+FC", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg7.has_value());
	EXPECT_EQ(wg7->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg7->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather7 = wg7->weather();
	EXPECT_EQ(weather7.size(), 1u);
	EXPECT_EQ(weather7.at(0), metaf::Weather::FUNNEL_CLOUD);
	EXPECT_TRUE(wg7->isValid());

	const auto wg8 = metaf::WeatherGroup::parse("+DS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg8.has_value());
	EXPECT_EQ(wg8->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg8->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather8 = wg8->weather();
	EXPECT_EQ(weather8.size(), 1u);
	EXPECT_EQ(weather8.at(0), metaf::Weather::DUSTSTORM);
	EXPECT_TRUE(wg8->isValid());

	const auto wg9 = metaf::WeatherGroup::parse("+SS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg9.has_value());
	EXPECT_EQ(wg9->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg9->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather9 = wg9->weather();
	EXPECT_EQ(weather9.size(), 1u);
	EXPECT_EQ(weather9.at(0), metaf::Weather::SANDSTORM);
	EXPECT_TRUE(wg9->isValid());
}

TEST(WeatherGroup, parseQualifierHeavyWithDescriptor) {
	const auto wg1 = metaf::WeatherGroup::parse("+TSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::RAIN);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("+SHPL", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::ICE_PELLETS);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("+FZDZ", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::FREEZING);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::DRIZZLE);
	EXPECT_TRUE(wg3->isValid());
}

TEST(WeatherGroup, parseQualifierHeavyMultiplePhenomena) {
	const auto wg1 = metaf::WeatherGroup::parse("+DZRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 2u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::DRIZZLE);
	EXPECT_EQ(weather1.at(1), metaf::Weather::RAIN);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("+DSSS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 2u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::DUSTSTORM);
	EXPECT_EQ(weather2.at(1), metaf::Weather::SANDSTORM);
	EXPECT_TRUE(wg2->isValid());
}

TEST(WeatherGroup, parseQualifierHeavyMultiplePhenomenaWithDescriptor) {
	const auto wg = metaf::WeatherGroup::parse("+TSRAGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::HEAVY);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 2u);
	EXPECT_EQ(weather.at(0), metaf::Weather::RAIN);
	EXPECT_EQ(weather.at(1), metaf::Weather::SMALL_HAIL);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseInvalidIntensityQualifier) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("-IC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+IC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-DRSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+DRSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-BLSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+BLSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-BR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+BR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-FG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+FG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-FU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+FU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-VA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+VA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-DU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+DU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-SA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+SA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-HZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+HZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-PY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-PO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+PO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-SQ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+SQ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-FC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-DS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("-SS", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, parseQualifierVicinityWeather) {
	const auto wg1 = metaf::WeatherGroup::parse("VCFG", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::FOG);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("VCVA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::VOLCANIC_ASH);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("VCDS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 1u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::DUSTSTORM);
	EXPECT_TRUE(wg3->isValid());

	const auto wg4 = metaf::WeatherGroup::parse("VCSS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg4->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather4 = wg4->weather();
	EXPECT_EQ(weather4.size(), 1u);
	EXPECT_EQ(weather4.at(0), metaf::Weather::SANDSTORM);
	EXPECT_TRUE(wg4->isValid());

	const auto wg5 = metaf::WeatherGroup::parse("VCPO", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg5->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather5 = wg5->weather();
	EXPECT_EQ(weather5.size(), 1u);
	EXPECT_EQ(weather5.at(0), metaf::Weather::DUST_WHIRLS);
	EXPECT_TRUE(wg5->isValid());

	const auto wg6 = metaf::WeatherGroup::parse("VCFC", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg6->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather6 = wg6->weather();
	EXPECT_EQ(weather6.size(), 1u);
	EXPECT_EQ(weather6.at(0), metaf::Weather::FUNNEL_CLOUD);
	EXPECT_TRUE(wg6->isValid());
}

TEST(WeatherGroup, parseQualifierVicinityDescriptorOnly) {
	const auto wg1 = metaf::WeatherGroup::parse("VCSH", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	EXPECT_EQ(wg1->weather().size(), 0u);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("VCTS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	EXPECT_EQ(wg2->weather().size(), 0u);
	EXPECT_TRUE(wg2->isValid());
}

TEST(WeatherGroup, parseInvalidVicinityQualifier) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCDZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCRA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCSN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCSG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCIC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCPL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCGR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCUP", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCBR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCFU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCDU", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCSA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCHZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCPY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCSQ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCSHRA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCRASN", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCTSRA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+VC", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+VCRA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCFZRA", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, parseQualifierVicinityDescriptorAndWeather) {
	const auto wg = metaf::WeatherGroup::parse("VCBLSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::VICINITY);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::BLOWING);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::SNOW);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseQualifierRecentWeather) {
	const auto wg = metaf::WeatherGroup::parse("RERA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::RAIN);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseQualifierRecentWeatherWithDescriptor) {
	const auto wg1 = metaf::WeatherGroup::parse("RESHSN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg1->descriptor(), metaf::WeatherDescriptor::SHOWERS);
	const auto weather1 = wg1->weather();
	EXPECT_EQ(weather1.size(), 1u);
	EXPECT_EQ(weather1.at(0), metaf::Weather::SNOW);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("RETSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg2->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather2 = wg2->weather();
	EXPECT_EQ(weather2.size(), 1u);
	EXPECT_EQ(weather2.at(0), metaf::Weather::RAIN);
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WeatherGroup::parse("RETSRAGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg3->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	const auto weather3 = wg3->weather();
	EXPECT_EQ(weather3.size(), 2u);
	EXPECT_EQ(weather3.at(0), metaf::Weather::RAIN);
	EXPECT_EQ(weather3.at(1), metaf::Weather::SMALL_HAIL);
	EXPECT_TRUE(wg3->isValid());
}

TEST(WeatherGroup, parseQualifierRecentWeatherDescriptorOnly) {
	const auto wg = metaf::WeatherGroup::parse("RETS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::THUNDERSTORM);
	EXPECT_EQ(wg->weather().size(), 0u);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseRecentWeatherNotReported) {
	const auto wg = metaf::WeatherGroup::parse("RE//", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->qualifier(), metaf::WeatherGroup::Qualifier::RECENT);
	EXPECT_EQ(wg->descriptor(), metaf::WeatherDescriptor::NONE);
	const auto weather = wg->weather();
	EXPECT_EQ(weather.size(), 1u);
	EXPECT_EQ(weather.at(0), metaf::Weather::NOT_REPORTED);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseQualifierModerateRainAndHail) {
	// Rain requires MODERATE qualifier and Hail does not: MODERATE qualifier is used
	const auto wg1 = metaf::WeatherGroup::parse("RAGS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->qualifier(), metaf::WeatherGroup::Qualifier::MODERATE);
	EXPECT_TRUE(wg1->isValid());
}

TEST(WeatherGroup, contains) {
	const auto wgPlusTsRaGr = metaf::WeatherGroup::parse("+TSRAGR", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgPlusTsRaGr.has_value());
	EXPECT_TRUE(wgPlusTsRaGr->contains(metaf::Weather::RAIN));
	EXPECT_TRUE(wgPlusTsRaGr->contains(metaf::Weather::HAIL));
	EXPECT_FALSE(wgPlusTsRaGr->contains(metaf::Weather::SNOW));
	EXPECT_FALSE(wgPlusTsRaGr->contains(metaf::Weather::NOT_REPORTED));

	const auto wgNr = metaf::WeatherGroup::parse("//", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgNr.has_value());
	EXPECT_FALSE(wgNr->contains(metaf::Weather::RAIN));
	EXPECT_TRUE(wgNr->contains(metaf::Weather::NOT_REPORTED));
}

///////////////////////////////////////////////////////////////////////////////

//TODO: recent weather, non reported weather, non reported recent weather
//all of the above allowed for METAR but not for TAF

TEST(WeatherGroup, parseWeatherMetar) {
	const auto wg = metaf::WeatherGroup::parse("TSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->isValid());
	const auto wp = wg->weatherPhenomena();
	EXPECT_EQ(wp.qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp.descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp.weather().size(), 1u);
	EXPECT_EQ(wp.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp.time().has_value());
}

TEST(WeatherGroup, parseWeatherTaf) {
	const auto wg = metaf::WeatherGroup::parse("TSRA", metaf::ReportPart::TAF);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->isValid());
	const auto wp = wg->weatherPhenomena();
	EXPECT_EQ(wp.qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp.descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp.weather().size(), 1u);
	EXPECT_EQ(wp.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp.time().has_value());
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

TEST(WeatherGroup, append) {
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
