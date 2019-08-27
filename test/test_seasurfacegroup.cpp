/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

static const auto margin = 0.1 / 2;

TEST(SeaSurfaceGroup, parseStateOfSeaSurface) {
	const auto ssg = metaf::SeaSurfaceGroup::parse("W15/S4", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg.has_value());
	EXPECT_EQ(ssg->surfaceTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(ssg->surfaceTemperature().temperature().has_value());
	EXPECT_EQ(ssg->surfaceTemperature().temperature().value(), 15);
	EXPECT_EQ(ssg->waves().type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(ssg->waves().stateOfSurface(), metaf::WaveHeight::StateOfSurface::MODERATE);
}

TEST(SeaSurfaceGroup, parseStateOfSeaSurfaceTemperatureNotReported) {
	const auto ssg = metaf::SeaSurfaceGroup::parse("W///S4", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg.has_value());
	EXPECT_FALSE(ssg->surfaceTemperature().temperature().has_value());
	EXPECT_EQ(ssg->waves().type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(ssg->waves().stateOfSurface(), metaf::WaveHeight::StateOfSurface::MODERATE);
}

TEST(SeaSurfaceGroup, parseStateOfSeaSurfaceStateNotReported) {
	const auto ssg = metaf::SeaSurfaceGroup::parse("W15/S/", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg.has_value());
	EXPECT_EQ(ssg->surfaceTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(ssg->surfaceTemperature().temperature().has_value());
	EXPECT_EQ(ssg->surfaceTemperature().temperature().value(), 15);
	EXPECT_EQ(ssg->waves().type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(ssg->waves().stateOfSurface(), metaf::WaveHeight::StateOfSurface::NOT_REPORTED);
}

TEST(SeaSurfaceGroup, parseStateOfSeaSurfaceTemperatureAndStateNotReported) {
	const auto ssg = metaf::SeaSurfaceGroup::parse("W///S/", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg.has_value());
	EXPECT_FALSE(ssg->surfaceTemperature().temperature().has_value());
	EXPECT_EQ(ssg->waves().type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(ssg->waves().stateOfSurface(), metaf::WaveHeight::StateOfSurface::NOT_REPORTED);
}

TEST(SeaSurfaceGroup, parseStateOfSeaSurfaceWrongReportPart) {
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/S4", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/S4", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/S4", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/S4", metaf::ReportPart::RMK));
}

TEST(SeaSurfaceGroup, parseStateOfSeaSurfaceWrongFormat) {
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("T15/S4", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/T4", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W5/S4", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("WM05/S4", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("WM5/S4", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W015/S4", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/S04", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W1A/S4", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/SA", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W//S4", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W////S4", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/S", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/S//", metaf::ReportPart::METAR));
}

TEST(SeaSurfaceGroup, parseWaveHeightOneDigit) {
	const auto ssg = metaf::SeaSurfaceGroup::parse("W16/H7", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg.has_value());
	EXPECT_EQ(ssg->surfaceTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(ssg->surfaceTemperature().temperature().has_value());
	EXPECT_EQ(ssg->surfaceTemperature().temperature().value(), 16);
	EXPECT_EQ(ssg->waves().type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(ssg->waves().unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(ssg->waves().waveHeight().has_value());
	EXPECT_NEAR(ssg->waves().waveHeight().value(), 0.7, margin);
}

TEST(SeaSurfaceGroup, parseWaveHeightTwoDigits) {
	const auto ssg = metaf::SeaSurfaceGroup::parse("W15/H23", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg.has_value());
	EXPECT_EQ(ssg->surfaceTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(ssg->surfaceTemperature().temperature().has_value());
	EXPECT_EQ(ssg->surfaceTemperature().temperature().value(), 15);
	EXPECT_EQ(ssg->waves().type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(ssg->waves().unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(ssg->waves().waveHeight().has_value());
	EXPECT_NEAR(ssg->waves().waveHeight().value(), 2.3, margin);
}

TEST(SeaSurfaceGroup, parseWaveHeightThreeDigits) {
	const auto ssg = metaf::SeaSurfaceGroup::parse("W17/H100", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg.has_value());
	EXPECT_EQ(ssg->surfaceTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(ssg->surfaceTemperature().temperature().has_value());
	EXPECT_EQ(ssg->surfaceTemperature().temperature().value(), 17);
	EXPECT_EQ(ssg->waves().type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(ssg->waves().unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(ssg->waves().waveHeight().has_value());
	EXPECT_NEAR(ssg->waves().waveHeight().value(), 10.0, margin);
}

TEST(SeaSurfaceGroup, parseWaveHeightTemperatureNotReported) {
	const auto ssg = metaf::SeaSurfaceGroup::parse("W///H23", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg.has_value());
	EXPECT_FALSE(ssg->surfaceTemperature().temperature().has_value());
	EXPECT_EQ(ssg->waves().type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(ssg->waves().unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(ssg->waves().waveHeight().has_value());
	EXPECT_NEAR(ssg->waves().waveHeight().value(), 2.3, margin);
}

TEST(SeaSurfaceGroup, parseWaveHeightWaveHeightNotReported) {
	const auto ssg = metaf::SeaSurfaceGroup::parse("W15/H///", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg.has_value());
	EXPECT_EQ(ssg->surfaceTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(ssg->surfaceTemperature().temperature().has_value());
	EXPECT_EQ(ssg->surfaceTemperature().temperature().value(), 15);
	EXPECT_EQ(ssg->waves().unit(), metaf::WaveHeight::Unit::METERS);
	EXPECT_FALSE(ssg->waves().waveHeight().has_value());
}

TEST(SeaSurfaceGroup, parseWaveHeightTemperatureAndWaveHeightNotReported) {
	const auto ssg = metaf::SeaSurfaceGroup::parse("W///H///", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg.has_value());
	EXPECT_FALSE(ssg->surfaceTemperature().temperature().has_value());
	EXPECT_EQ(ssg->waves().type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_FALSE(ssg->waves().waveHeight().has_value());
}

TEST(SeaSurfaceGroup, parseWaveHeightWrongReportPart) {
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/H23", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/H23", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/H23", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/H23", metaf::ReportPart::RMK).has_value());
}

TEST(SeaSurfaceGroup, parseWaveHeightWrongFormat) {
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("T15/H23", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/T23", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W5/H23", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("WM5/H23", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("WM15/H23", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W015/H23", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/H0023", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W1A/H23", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/H2A", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W//H23", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W////H23", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/H", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/H//", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::SeaSurfaceGroup::parse("W15/H////", metaf::ReportPart::METAR));
}

TEST(SeaSurfaceGroup, combine) {
	const auto ssg1 = metaf::SeaSurfaceGroup::parse("W15/S4", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg1.has_value());

	const auto ssg2 = metaf::SeaSurfaceGroup::parse("W20/H5", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg2.has_value());

	const auto rmk = metaf::FixedGroup::parse("RMK", metaf::ReportPart::METAR);
	ASSERT_TRUE(rmk.has_value());

	EXPECT_FALSE(ssg1->combine(rmk.value()).has_value());
	EXPECT_FALSE(ssg1->combine(ssg1.value()).has_value());
	EXPECT_FALSE(ssg1->combine(ssg2.value()).has_value());
	EXPECT_FALSE(ssg1->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(ssg2->combine(rmk.value()).has_value());
	EXPECT_FALSE(ssg2->combine(ssg1.value()).has_value());
	EXPECT_FALSE(ssg2->combine(ssg2.value()).has_value());
	EXPECT_FALSE(ssg2->combine(metaf::PlainTextGroup("TEST")).has_value());
}
