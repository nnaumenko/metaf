/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.1 / 2;

///////////////////////////////////////////////////////////////////////////////
// Parse descriptive state of sea surface
// Purpose: to confirm that strings which contain descriptive state of sea 
// surface are parsed correctly and other string formats are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WaveHeight, fromStringStateOfSurfaceCalmGlassy) {
	const auto wh = metaf::WaveHeight::fromString("S0");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::CALM_GLASSY);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 0.0, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfaceCalmRippled) {
	const auto wh = metaf::WaveHeight::fromString("S1");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::CALM_RIPPLED);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 0.1, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfaceSmooth) {
	const auto wh = metaf::WaveHeight::fromString("S2");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::SMOOTH);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 0.5, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfaceSlight) {
	const auto wh = metaf::WaveHeight::fromString("S3");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::SLIGHT);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 1.2, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfaceModerate) {
	const auto wh = metaf::WaveHeight::fromString("S4");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::MODERATE);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 2.5, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfaceRough) {
	const auto wh = metaf::WaveHeight::fromString("S5");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::ROUGH);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 4.0, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfaceVeryRough) {
	const auto wh = metaf::WaveHeight::fromString("S6");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::VERY_ROUGH);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 6.0, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfaceHigh) {
	const auto wh = metaf::WaveHeight::fromString("S7");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::HIGH);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 9.0, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfaceVeryHigh) {
	const auto wh = metaf::WaveHeight::fromString("S8");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::VERY_HIGH);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 14.0, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfacePhenomenal) {
	const auto wh = metaf::WaveHeight::fromString("S9");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::PHENOMENAL);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 14.1, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfaceNotReported) {
	const auto wh = metaf::WaveHeight::fromString("S/");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::NOT_REPORTED);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	EXPECT_FALSE(wh->waveHeight().has_value());
	EXPECT_FALSE(wh->isReported());
}

TEST(WaveHeight, fromStringStateOfSurfaceWrongFormat) {
	EXPECT_FALSE(metaf::WaveHeight::fromString("").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("S").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("S01").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("S.").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("S//").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("SA").has_value());
}	

///////////////////////////////////////////////////////////////////////////////
// Parse wave height
// Purpose: to confirm that strings which contain wave height in decimeters
// are parsed correctly and other string formats are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WaveHeight, fromStringWaveHeightThreeDigit) {
	const auto wh1 = metaf::WaveHeight::fromString("H105");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(wh1->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh1->waveHeight().has_value());
	EXPECT_NEAR(wh1->waveHeight().value(), 10.5, margin);
	EXPECT_TRUE(wh1->isReported());

	const auto wh2 = metaf::WaveHeight::fromString("H037");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(wh2->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh2->waveHeight().has_value());
	EXPECT_NEAR(wh2->waveHeight().value(), 3.7, margin);
	EXPECT_TRUE(wh2->isReported());

	const auto wh3 = metaf::WaveHeight::fromString("H008");
	ASSERT_TRUE(wh3.has_value());
	EXPECT_EQ(wh3->type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(wh3->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh3->waveHeight().has_value());
	EXPECT_NEAR(wh3->waveHeight().value(), 0.8, margin);
	EXPECT_TRUE(wh3->isReported());
}

TEST(WaveHeight, fromStringWaveHeightTwoDigit) {
	const auto wh1 = metaf::WaveHeight::fromString("H14");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(wh1->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh1->waveHeight().has_value());
	EXPECT_NEAR(wh1->waveHeight().value(), 1.4, margin);
	EXPECT_TRUE(wh1->isReported());

	const auto wh2 = metaf::WaveHeight::fromString("H07");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(wh2->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh2->waveHeight().has_value());
	EXPECT_NEAR(wh2->waveHeight().value(), 0.7, margin);
	EXPECT_TRUE(wh2->isReported());
}

TEST(WaveHeight, fromStringWaveHeightOneDigit) {
	const auto wh = metaf::WaveHeight::fromString("H8");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 0.8, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromStringWaveHeightNotReported) {
	const auto wh = metaf::WaveHeight::fromString("H///");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::WAVE_HEIGHT);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	EXPECT_FALSE(wh->waveHeight().has_value());
	EXPECT_FALSE(wh->isReported());
}

TEST(WaveHeight, fromStringWaveHeightWrongFormat) {
	EXPECT_FALSE(metaf::WaveHeight::fromString("").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("H").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("H0001").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("H0.1").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("H//").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("H////").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("H00A").has_value());
}	

TEST(WaveHeight, stateOfSurfaceCalmGlassy) {
	const auto wh1 = metaf::WaveHeight::fromString("H000");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->stateOfSurface(), metaf::WaveHeight::StateOfSurface::CALM_GLASSY);

	const auto wh2 = metaf::WaveHeight::fromString("S0");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->stateOfSurface(), metaf::WaveHeight::StateOfSurface::CALM_GLASSY);
}

TEST(WaveHeight, stateOfSurfaceCalmRippled) {
	const auto wh1 = metaf::WaveHeight::fromString("H001");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->stateOfSurface(), metaf::WaveHeight::StateOfSurface::CALM_RIPPLED);

	const auto wh2 = metaf::WaveHeight::fromString("S1");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->stateOfSurface(), metaf::WaveHeight::StateOfSurface::CALM_RIPPLED);
}

TEST(WaveHeight, stateOfSurfaceSmooth) {
	const auto wh1 = metaf::WaveHeight::fromString("H005");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->stateOfSurface(), metaf::WaveHeight::StateOfSurface::SMOOTH);

	const auto wh2 = metaf::WaveHeight::fromString("H002");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->stateOfSurface(), metaf::WaveHeight::StateOfSurface::SMOOTH);

	const auto wh3 = metaf::WaveHeight::fromString("S2");
	ASSERT_TRUE(wh3.has_value());
	EXPECT_EQ(wh3->stateOfSurface(), metaf::WaveHeight::StateOfSurface::SMOOTH);
}

TEST(WaveHeight, stateOfSurfaceSlight) {
	const auto wh1 = metaf::WaveHeight::fromString("H012");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->stateOfSurface(), metaf::WaveHeight::StateOfSurface::SLIGHT);

	const auto wh2 = metaf::WaveHeight::fromString("H006");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->stateOfSurface(), metaf::WaveHeight::StateOfSurface::SLIGHT);

	const auto wh3 = metaf::WaveHeight::fromString("S3");
	ASSERT_TRUE(wh3.has_value());
	EXPECT_EQ(wh3->stateOfSurface(), metaf::WaveHeight::StateOfSurface::SLIGHT);
}

TEST(WaveHeight, stateOfSurfaceModerate) {
	const auto wh1 = metaf::WaveHeight::fromString("H025");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->stateOfSurface(), metaf::WaveHeight::StateOfSurface::MODERATE);

	const auto wh2 = metaf::WaveHeight::fromString("H013");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->stateOfSurface(), metaf::WaveHeight::StateOfSurface::MODERATE);

	const auto wh3 = metaf::WaveHeight::fromString("S4");
	ASSERT_TRUE(wh3.has_value());
	EXPECT_EQ(wh3->stateOfSurface(), metaf::WaveHeight::StateOfSurface::MODERATE);
}

TEST(WaveHeight, stateOfSurfaceRough) {
	const auto wh1 = metaf::WaveHeight::fromString("H040");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->stateOfSurface(), metaf::WaveHeight::StateOfSurface::ROUGH);

	const auto wh2 = metaf::WaveHeight::fromString("H026");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->stateOfSurface(), metaf::WaveHeight::StateOfSurface::ROUGH);

	const auto wh3 = metaf::WaveHeight::fromString("S5");
	ASSERT_TRUE(wh3.has_value());
	EXPECT_EQ(wh3->stateOfSurface(), metaf::WaveHeight::StateOfSurface::ROUGH);
}

TEST(WaveHeight, stateOfSurfaceVeryRough) {
	const auto wh1 = metaf::WaveHeight::fromString("H060");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->stateOfSurface(), metaf::WaveHeight::StateOfSurface::VERY_ROUGH);

	const auto wh2 = metaf::WaveHeight::fromString("H041");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->stateOfSurface(), metaf::WaveHeight::StateOfSurface::VERY_ROUGH);

	const auto wh3 = metaf::WaveHeight::fromString("S6");
	ASSERT_TRUE(wh3.has_value());
	EXPECT_EQ(wh3->stateOfSurface(), metaf::WaveHeight::StateOfSurface::VERY_ROUGH);
}

TEST(WaveHeight, stateOfSurfaceHigh) {
	const auto wh1 = metaf::WaveHeight::fromString("H090");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->stateOfSurface(), metaf::WaveHeight::StateOfSurface::HIGH);

	const auto wh2 = metaf::WaveHeight::fromString("H061");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->stateOfSurface(), metaf::WaveHeight::StateOfSurface::HIGH);

	const auto wh3 = metaf::WaveHeight::fromString("S7");
	ASSERT_TRUE(wh3.has_value());
	EXPECT_EQ(wh3->stateOfSurface(), metaf::WaveHeight::StateOfSurface::HIGH);
}

TEST(WaveHeight, stateOfSurfaceVeryHigh) {
	const auto wh1 = metaf::WaveHeight::fromString("H140");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->stateOfSurface(), metaf::WaveHeight::StateOfSurface::VERY_HIGH);

	const auto wh2 = metaf::WaveHeight::fromString("H091");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->stateOfSurface(), metaf::WaveHeight::StateOfSurface::VERY_HIGH);

	const auto wh3 = metaf::WaveHeight::fromString("S8");
	ASSERT_TRUE(wh3.has_value());
	EXPECT_EQ(wh3->stateOfSurface(), metaf::WaveHeight::StateOfSurface::VERY_HIGH);
}

TEST(WaveHeight, stateOfSurfacePhenomenal) {
	const auto wh1 = metaf::WaveHeight::fromString("H320");
	ASSERT_TRUE(wh1.has_value());
	EXPECT_EQ(wh1->stateOfSurface(), metaf::WaveHeight::StateOfSurface::PHENOMENAL);

	const auto wh2 = metaf::WaveHeight::fromString("H141");
	ASSERT_TRUE(wh2.has_value());
	EXPECT_EQ(wh2->stateOfSurface(), metaf::WaveHeight::StateOfSurface::PHENOMENAL);

	const auto wh3 = metaf::WaveHeight::fromString("S9");
	ASSERT_TRUE(wh3.has_value());
	EXPECT_EQ(wh3->stateOfSurface(), metaf::WaveHeight::StateOfSurface::PHENOMENAL);
}

TEST(WaveHeight, waveHeightToUnitMeters) {
	const auto wh1 = metaf::WaveHeight::fromString("H10");
	ASSERT_TRUE(wh1.has_value());
	ASSERT_TRUE(wh1->toUnit(metaf::WaveHeight::Unit::METERS).has_value());
	EXPECT_NEAR(wh1->toUnit(metaf::WaveHeight::Unit::METERS).value(), 1.0, margin);

	const auto wh2 = metaf::WaveHeight::fromString("H59");
	ASSERT_TRUE(wh2.has_value());
	ASSERT_TRUE(wh2->toUnit(metaf::WaveHeight::Unit::METERS).has_value());
	EXPECT_NEAR(wh2->toUnit(metaf::WaveHeight::Unit::METERS).value(), 5.9, margin);
}

TEST(WaveHeight, waveHeightToUnitFeet) {
	const auto wh1 = metaf::WaveHeight::fromString("H10");
	ASSERT_TRUE(wh1.has_value());
	ASSERT_TRUE(wh1->toUnit(metaf::WaveHeight::Unit::FEET).has_value());
	EXPECT_NEAR(wh1->toUnit(metaf::WaveHeight::Unit::FEET).value(), 3.28, margin);

	const auto wh2 = metaf::WaveHeight::fromString("H59");
	ASSERT_TRUE(wh2.has_value());
	ASSERT_TRUE(wh2->toUnit(metaf::WaveHeight::Unit::FEET).has_value());
	EXPECT_NEAR(wh2->toUnit(metaf::WaveHeight::Unit::FEET).value(), 19.36, margin);
}

TEST(WaveHeight, waveHeightToUnitNotReported) {
	const auto wh = metaf::WaveHeight::fromString("H///");
	ASSERT_TRUE(wh.has_value());
	EXPECT_FALSE(wh->toUnit(metaf::WaveHeight::Unit::METERS).has_value());
	EXPECT_FALSE(wh->toUnit(metaf::WaveHeight::Unit::FEET).has_value());
}

TEST(WaveHeight, fromStringWrongDesignator) {
	EXPECT_FALSE(metaf::WaveHeight::fromString("X59").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromString("X5").has_value());
}	

///////////////////////////////////////////////////////////////////////////////
// Parse QUK code (same as descriptive state of sea surface)
// Purpose: to confirm that QUK Q-code string which contains the same 
// descriptive state of sea surface as above is parsed correctly and other 
// string formats are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(WaveHeight, fromQukStringCalmGlassy) {
	const auto wh = metaf::WaveHeight::fromQukString("0");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::CALM_GLASSY);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 0.0, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromQukStringCalmRippled) {
	const auto wh = metaf::WaveHeight::fromQukString("1");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::CALM_RIPPLED);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 0.1, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromQukStringSmooth) {
	const auto wh = metaf::WaveHeight::fromQukString("2");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::SMOOTH);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 0.5, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromQukStringSlight) {
	const auto wh = metaf::WaveHeight::fromQukString("3");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::SLIGHT);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 1.2, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromQukStringModerate) {
	const auto wh = metaf::WaveHeight::fromQukString("4");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::MODERATE);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 2.5, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromQukStringRough) {
	const auto wh = metaf::WaveHeight::fromQukString("5");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::ROUGH);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 4.0, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromQukStringVeryRough) {
	const auto wh = metaf::WaveHeight::fromQukString("6");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::VERY_ROUGH);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 6.0, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromQukStringHigh) {
	const auto wh = metaf::WaveHeight::fromQukString("7");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::HIGH);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 9.0, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromQukStringVeryHigh) {
	const auto wh = metaf::WaveHeight::fromQukString("8");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::VERY_HIGH);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 14.0, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromQukStringPhenomenal) {
	const auto wh = metaf::WaveHeight::fromQukString("9");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::PHENOMENAL);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	ASSERT_TRUE(wh->waveHeight().has_value());
	EXPECT_NEAR(wh->waveHeight().value(), 14.1, margin);
	EXPECT_TRUE(wh->isReported());
}

TEST(WaveHeight, fromQukStringNotReported) {
	const auto wh = metaf::WaveHeight::fromQukString("/");
	ASSERT_TRUE(wh.has_value());
	EXPECT_EQ(wh->type(), metaf::WaveHeight::Type::STATE_OF_SURFACE);
	EXPECT_EQ(wh->stateOfSurface(), metaf::WaveHeight::StateOfSurface::NOT_REPORTED);
	EXPECT_EQ(wh->unit(), metaf::WaveHeight::Unit::METERS);
	EXPECT_FALSE(wh->waveHeight().has_value());
	EXPECT_FALSE(wh->isReported());
}

TEST(WaveHeight, fromQukStringWrongFormat) {
	EXPECT_FALSE(metaf::WaveHeight::fromQukString("").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromQukString("S1").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromQukString("H04").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromQukString("01").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromQukString("1/").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromQukString("A").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromQukString("A1").has_value());
	EXPECT_FALSE(metaf::WaveHeight::fromQukString("1A").has_value());
}	

