/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.1 / 2;

///////////////////////////////////////////////////////////////////////////////
// State of sea surface group
// Purpose: to confirm that group which contains descriptive state of sea 
// surface is parsed correctly, that each descriptive sea surface state is 
// identified correctly, and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////
// Wave height group
// Purpose: to confirm that group which contains wave height in decimeters is 
// parsed correctly, that each descriptive sea surface state is identified 
// correctly, and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////
// Appending tests
// Purpose: to confirm that sea surface state and wave height groups cannot be
// appended
///////////////////////////////////////////////////////////////////////////////

TEST(SeaSurfaceGroup, append) {
	const std::string ssgStr1("W15/S4");
	const std::string ssgStr2("W15/S/");
	const std::string ssgStr3("W///S4");
	const std::string ssgStr4("W///S/");
	const std::string ssgStr5("W20/H5");
	const std::string ssgStr6("W20/H/");
	const std::string ssgStr7("W///H5");
	const std::string ssgStr8("W///H/");
	const std::string tStr1("RMK");
	const std::string tStr2("TEST");

	auto ssg1 = metaf::SeaSurfaceGroup::parse(ssgStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg1.has_value());

	auto ssg2 = metaf::SeaSurfaceGroup::parse(ssgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg2.has_value());

	auto ssg3 = metaf::SeaSurfaceGroup::parse(ssgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg3.has_value());

	auto ssg4 = metaf::SeaSurfaceGroup::parse(ssgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg4.has_value());

	auto ssg5 = metaf::SeaSurfaceGroup::parse(ssgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg5.has_value());

	auto ssg6 = metaf::SeaSurfaceGroup::parse(ssgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg6.has_value());

	auto ssg7 = metaf::SeaSurfaceGroup::parse(ssgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg7.has_value());

	auto ssg8 = metaf::SeaSurfaceGroup::parse(ssgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg8.has_value());

	EXPECT_EQ(ssg1->append(ssgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg1->append(ssgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg1->append(ssgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg1->append(ssgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg1->append(ssgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg1->append(ssgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg1->append(ssgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg1->append(ssgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg1->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg1->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ssg2->append(ssgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg2->append(ssgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg2->append(ssgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg2->append(ssgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg2->append(ssgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg2->append(ssgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg2->append(ssgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg2->append(ssgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg2->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg2->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ssg3->append(ssgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg3->append(ssgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg3->append(ssgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg3->append(ssgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg3->append(ssgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg3->append(ssgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg3->append(ssgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg3->append(ssgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg3->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg3->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ssg4->append(ssgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg4->append(ssgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg4->append(ssgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg4->append(ssgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg4->append(ssgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg4->append(ssgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg4->append(ssgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg4->append(ssgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg4->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg4->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ssg5->append(ssgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg5->append(ssgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg5->append(ssgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg5->append(ssgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg5->append(ssgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg5->append(ssgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg5->append(ssgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg5->append(ssgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg5->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg5->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ssg6->append(ssgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg6->append(ssgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg6->append(ssgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg6->append(ssgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg6->append(ssgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg6->append(ssgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg6->append(ssgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg6->append(ssgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg6->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg6->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ssg7->append(ssgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg7->append(ssgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg7->append(ssgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg7->append(ssgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg7->append(ssgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg7->append(ssgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg7->append(ssgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg7->append(ssgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg7->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg7->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ssg8->append(ssgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg8->append(ssgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg8->append(ssgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg8->append(ssgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg8->append(ssgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg8->append(ssgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg8->append(ssgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg8->append(ssgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg8->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ssg8->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method returns true for all values
///////////////////////////////////////////////////////////////////////////////

TEST(SeaSurfaceGroup, isValid) {
	const auto ssg1 = metaf::SeaSurfaceGroup::parse("W16/H7", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg1.has_value());
	EXPECT_TRUE(ssg1->isValid());

	const auto ssg2 = metaf::SeaSurfaceGroup::parse("W15/H23", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg2.has_value());
	EXPECT_TRUE(ssg2->isValid());

	const auto ssg3 = metaf::SeaSurfaceGroup::parse("W17/H100", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg3.has_value());
	EXPECT_TRUE(ssg3->isValid());

	const auto ssg4 = metaf::SeaSurfaceGroup::parse("W15/H///", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg4.has_value());
	EXPECT_TRUE(ssg4->isValid());

	const auto ssg5 = metaf::SeaSurfaceGroup::parse("W///H///", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg5.has_value());
	EXPECT_TRUE(ssg5->isValid());

	const auto ssg6 = metaf::SeaSurfaceGroup::parse("W15/S4", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg6.has_value());
	EXPECT_TRUE(ssg6->isValid());

	const auto ssg7 = metaf::SeaSurfaceGroup::parse("W///S4", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg7.has_value());
	EXPECT_TRUE(ssg7->isValid());

	const auto ssg8 = metaf::SeaSurfaceGroup::parse("W15/S/", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg8.has_value());
	EXPECT_TRUE(ssg8->isValid());

	const auto ssg9 = metaf::SeaSurfaceGroup::parse("W///S/", metaf::ReportPart::METAR);
	ASSERT_TRUE(ssg9.has_value());
	EXPECT_TRUE(ssg9->isValid());
}
