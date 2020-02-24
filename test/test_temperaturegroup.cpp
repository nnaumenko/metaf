/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.1/2;
static const auto rhMargin = 0.1/2;

///////////////////////////////////////////////////////////////////////////////
// Temperature and dew point groups
// Purpose: to confirm that groups reporting ambient air temperature and dew
// point included in METAR report body are parsed correctly, and that 
// malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureGroup, parseTemperatureAndDewPoint) {
	const auto tg = metaf::TemperatureGroup::parse("25/18", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg->airTemperature().isReported());
	EXPECT_NEAR(tg->airTemperature().temperature().value(), 25, margin);
	EXPECT_FALSE(tg->airTemperature().isPrecise());
	ASSERT_TRUE(tg->dewPoint().temperature().has_value());
	EXPECT_NEAR(tg->dewPoint().temperature().value(), 18, margin);
	EXPECT_FALSE(tg->dewPoint().isPrecise());
}

TEST(TemperatureGroup, parseTemperatureAndDewPointNegativeValues) {
	const auto tg1 = metaf::TemperatureGroup::parse("05/M02", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg1->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg1->airTemperature().isReported());
	EXPECT_NEAR(tg1->airTemperature().temperature().value(), 5, margin);
	EXPECT_FALSE(tg1->airTemperature().isPrecise());
	ASSERT_TRUE(tg1->dewPoint().temperature().has_value());
	EXPECT_NEAR(tg1->dewPoint().temperature().value(), -2, margin);
	EXPECT_FALSE(tg1->dewPoint().isPrecise());

	const auto tg2 = metaf::TemperatureGroup::parse("M02/M04", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg2->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg2->airTemperature().isReported());
	EXPECT_NEAR(tg2->airTemperature().temperature().value(), -2, margin);
	EXPECT_FALSE(tg2->airTemperature().isPrecise());
	ASSERT_TRUE(tg2->dewPoint().temperature().has_value());
	EXPECT_NEAR(tg2->dewPoint().temperature().value(), -4, margin);
	EXPECT_FALSE(tg2->dewPoint().isPrecise());
}

TEST(TemperatureGroup, parseTemperatureAndDewPointZero) {
	const auto tg1 = metaf::TemperatureGroup::parse("00/M02", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg1->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg1->airTemperature().isReported());
	EXPECT_NEAR(tg1->airTemperature().temperature().value(), 0, margin);
	EXPECT_FALSE(tg1->airTemperature().isFreezing());
	EXPECT_FALSE(tg1->airTemperature().isPrecise());
	ASSERT_TRUE(tg1->dewPoint().temperature().has_value());
	EXPECT_NEAR(tg1->dewPoint().temperature().value(), -2, margin);
	EXPECT_TRUE(tg1->dewPoint().isFreezing());
	EXPECT_FALSE(tg1->dewPoint().isPrecise());

	const auto tg2 = metaf::TemperatureGroup::parse("M00/M02", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg2->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg2->airTemperature().isReported());
	EXPECT_NEAR(tg2->airTemperature().temperature().value(), 0, margin);
	EXPECT_TRUE(tg2->airTemperature().isFreezing());
	EXPECT_FALSE(tg2->airTemperature().isPrecise());
	ASSERT_TRUE(tg2->dewPoint().temperature().has_value());
	EXPECT_NEAR(tg2->dewPoint().temperature().value(), -2, margin);
	EXPECT_TRUE(tg2->dewPoint().isFreezing());
	EXPECT_FALSE(tg2->dewPoint().isPrecise());

	const auto tg3 = metaf::TemperatureGroup::parse("02/M00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_EQ(tg3->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg3->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg3->airTemperature().isReported());
	EXPECT_NEAR(tg3->airTemperature().temperature().value(), 2, margin);
	EXPECT_FALSE(tg3->airTemperature().isFreezing());
	EXPECT_FALSE(tg3->airTemperature().isPrecise());
	ASSERT_TRUE(tg3->dewPoint().temperature().has_value());
	EXPECT_NEAR(tg3->dewPoint().temperature().value(), 0, margin);
	EXPECT_TRUE(tg3->dewPoint().isFreezing());
	EXPECT_FALSE(tg3->dewPoint().isPrecise());

	const auto tg4 = metaf::TemperatureGroup::parse("02/00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg4.has_value());
	EXPECT_EQ(tg4->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg4->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg4->airTemperature().isReported());
	EXPECT_NEAR(tg4->airTemperature().temperature().value(), 2, margin);
	EXPECT_FALSE(tg4->airTemperature().isFreezing());
	EXPECT_FALSE(tg4->airTemperature().isPrecise());
	ASSERT_TRUE(tg4->dewPoint().temperature().has_value());
	EXPECT_NEAR(tg4->dewPoint().temperature().value(), 0, margin);
	EXPECT_FALSE(tg4->dewPoint().isFreezing());
	EXPECT_FALSE(tg4->dewPoint().isPrecise());
}

TEST(TemperatureGroup, parseTemperatureOnly) {
	const auto tg = metaf::TemperatureGroup::parse("M70/", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg->airTemperature().isReported());
	EXPECT_NEAR(tg->airTemperature().temperature().value(), -70, margin);
	EXPECT_FALSE(tg->airTemperature().isPrecise());
	EXPECT_FALSE(tg->dewPoint().temperature().has_value());
}

TEST(TemperatureGroup, parseDewPointNotReported) {
	const auto tg = metaf::TemperatureGroup::parse("25///", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg->airTemperature().isReported());
	EXPECT_NEAR(tg->airTemperature().temperature().value(), 25, margin);
	EXPECT_FALSE(tg->airTemperature().isPrecise());
	EXPECT_FALSE(tg->dewPoint().temperature().has_value());
}

TEST(TemperatureGroup, parseNotReported) {
	const auto tg = metaf::TemperatureGroup::parse("/////", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg->airTemperature().unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(tg->airTemperature().isReported());
	EXPECT_FALSE(tg->dewPoint().temperature().has_value());
}

TEST(TemperatureGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/18", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/18", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/18", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/18", metaf::ReportPart::RMK).has_value());
}

TEST(TemperatureGroup, parseWrongFormat) {
	EXPECT_FALSE(metaf::TemperatureGroup::parse("12:08", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("2518", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("025/18", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/018", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("025/018", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("2A/18", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/1A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25//", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("//////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("25/M//", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("M/////", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("///M//", metaf::ReportPart::METAR).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Precise temperature and dew point groups
// Purpose: to confirm that groups reporting ambient air temperature and dew
// point with precision of 0.1 C and included in remarks are parsed correctly, 
// and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureGroup, parseRemark) {
	const auto tg1 = metaf::TemperatureGroup::parse("T00560028", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg1->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg1->airTemperature().isReported());
	EXPECT_NEAR(tg1->airTemperature().temperature().value(), 5.6, margin);
	EXPECT_TRUE(tg1->airTemperature().isPrecise());
	ASSERT_TRUE(tg1->dewPoint().temperature().has_value());
	EXPECT_NEAR(tg1->dewPoint().temperature().value(), 2.8, margin);
	EXPECT_TRUE(tg1->dewPoint().isPrecise());

	const auto tg2 = metaf::TemperatureGroup::parse("T10171028", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg2->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg2->airTemperature().isReported());
	EXPECT_NEAR(tg2->airTemperature().temperature().value(), -1.7, margin);
	ASSERT_TRUE(tg2->dewPoint().temperature().has_value());
	EXPECT_NEAR(tg2->dewPoint().temperature().value(), -2.8, margin);
	EXPECT_TRUE(tg2->dewPoint().isPrecise());
}

TEST(TemperatureGroup, parseRemarkTemperatureOnly) {
	const auto tg = metaf::TemperatureGroup::parse("T0261", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT);
	EXPECT_EQ(tg->airTemperature().unit(), metaf::Temperature::Unit::C);
	ASSERT_TRUE(tg->airTemperature().isReported());
	EXPECT_NEAR(tg->airTemperature().temperature().value(), 26.1, margin);
	EXPECT_TRUE(tg->airTemperature().isPrecise());
	EXPECT_FALSE(tg->dewPoint().temperature().has_value());
}

TEST(TemperatureGroup, parseRemarkWrongReportPart) {
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00560028", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00560028", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00560028", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00560028", metaf::ReportPart::TAF).has_value());
}

TEST(TemperatureGroup, parseRemarkWrongFormat) {
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T20560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T30560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T40560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T50560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T60560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T70560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T80560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T90560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("TA0560028", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00562028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00563028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00564028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00565028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00566028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00567028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00568028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T00569028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T0056A028", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(metaf::TemperatureGroup::parse("A00560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T////0028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T0056////", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T////////", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(metaf::TemperatureGroup::parse("TM0560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T0056M028", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(metaf::TemperatureGroup::parse("T000560028", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("T005600028", metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// T MISG and TD MISG groups
// Purpose: to confirm that groups T MISG and TD MISG are parsed and appended 
// correctly, and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureGroup, tMisg) {
	auto tg = metaf::TemperatureGroup::parse("T", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(tg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->type(), metaf::TemperatureGroup::Type::T_MISG);
	EXPECT_FALSE(tg->airTemperature().isReported());
	EXPECT_FALSE(tg->dewPoint().isReported());
}

TEST(TemperatureGroup, appendOtherToT) {
	auto tg1 = metaf::TemperatureGroup::parse("T", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->append("005600028", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto tg2 = metaf::TemperatureGroup::parse("T", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto tg3 = metaf::TemperatureGroup::parse("T", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_EQ(tg3->append("11/07", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto tg4 = metaf::TemperatureGroup::parse("T", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg4.has_value());
	EXPECT_EQ(tg4->append("/////", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(TemperatureGroup, tdMisg) {
	auto tg = metaf::TemperatureGroup::parse("TD", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(tg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->type(), metaf::TemperatureGroup::Type::TD_MISG);
	EXPECT_FALSE(tg->airTemperature().isReported());
	EXPECT_FALSE(tg->dewPoint().isReported());
}

TEST(TemperatureGroup, appendOtherToTd) {
	auto tg1 = metaf::TemperatureGroup::parse("TD", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->append("005600028", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto tg2 = metaf::TemperatureGroup::parse("TD", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto tg3 = metaf::TemperatureGroup::parse("TD", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_EQ(tg3->append("11/07", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto tg4 = metaf::TemperatureGroup::parse("TD", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg4.has_value());
	EXPECT_EQ(tg4->append("/////", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(TemperatureGroup, parseMisg) {
	EXPECT_FALSE(metaf::TemperatureGroup::parse("MISG", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("MISG", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("MISG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("MISG", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::TemperatureGroup::parse("MISG", metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureGroup, isValidNotReported) {
	const auto tg1 = metaf::TemperatureGroup::parse("M70/", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_TRUE(tg1->isValid());

	const auto tg2 = metaf::TemperatureGroup::parse("25///", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_TRUE(tg2->isValid());

	const auto tg3 = metaf::TemperatureGroup::parse("/////", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_TRUE(tg3->isValid());
}

TEST(TemperatureGroup, isValidTrue) {
	const auto tg1 = metaf::TemperatureGroup::parse("25/24", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_TRUE(tg1->isValid());

	const auto tg2 = metaf::TemperatureGroup::parse("25/25", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_TRUE(tg2->isValid());
}

TEST(TemperatureGroup, isValidTrueZero) {
	const auto tg1 = metaf::TemperatureGroup::parse("00/00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_TRUE(tg1->isValid());

	const auto tg2 = metaf::TemperatureGroup::parse("M00/M00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_TRUE(tg2->isValid());

	const auto tg3 = metaf::TemperatureGroup::parse("00/M00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_TRUE(tg3->isValid());
}

TEST(TemperatureGroup, isValidTrueTMisgTdMisg) {
	auto tg1 = metaf::TemperatureGroup::parse("T", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg1.has_value());
	ASSERT_EQ(tg1->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(tg1->isValid());

	auto tg2 = metaf::TemperatureGroup::parse("TD", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg2.has_value());
	ASSERT_EQ(tg2->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(tg2->isValid());
}

TEST(TemperatureGroup, isValidFalse) {
	const auto tg1 = metaf::TemperatureGroup::parse("25/26", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_FALSE(tg1->isValid());

	const auto tg2 = metaf::TemperatureGroup::parse("M00/00", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_FALSE(tg2->isValid());
}

TEST(TemperatureGroup, isValidFalseTTd) {
	auto tg1 = metaf::TemperatureGroup::parse("T", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_FALSE(tg1->isValid());

	auto tg2 = metaf::TemperatureGroup::parse("TD", metaf::ReportPart::RMK);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_FALSE(tg2->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Tests for relativeHumidity()
// Purpose: to confirm that relativeHumidity() method produces correct relative 
// humidity value
///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureGroup, relativeHumidity) {
	const auto tg1 = metaf::TemperatureGroup::parse("35/05", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	const auto rh1 = tg1->relativeHumidity();
	EXPECT_NEAR(rh1.value(), 15.6, rhMargin);

	const auto tg2 = metaf::TemperatureGroup::parse("17/08", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	const auto rh2 = tg2->relativeHumidity();
	EXPECT_NEAR(rh2.value(), 55.4, rhMargin);
}

///////////////////////////////////////////////////////////////////////////////
// Appending tests
// Purpose: to confirm that no other group can be appended to a complete group
///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureGroup, append) {
	const std::string tgStr1("07/03");
	const std::string tgStr2("02/M00");
	const std::string tgStr3("M03/M07");
	const std::string tgStr4("///12");
	const std::string tgStr5("00///");
	const std::string tgStr6("/////");
	const std::string tgStr7("T");
	const std::string tgStr8("TD");
	const std::string tgStr9("MISG");
	const std::string tStr1("RMK");
	const std::string tStr2("TEST");

	auto tg1 = metaf::TemperatureGroup::parse(tgStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());

	auto tg2 = metaf::TemperatureGroup::parse(tgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());

	auto tg3 = metaf::TemperatureGroup::parse(tgStr3, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());

	auto tg4 = metaf::TemperatureGroup::parse(tgStr4, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg4.has_value());

	auto tg5 = metaf::TemperatureGroup::parse(tgStr5, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg5.has_value());

	auto tg6 = metaf::TemperatureGroup::parse(tgStr6, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg6.has_value());

	auto tg7 = metaf::TemperatureGroup::parse(tgStr7, metaf::ReportPart::RMK);
	ASSERT_TRUE(tg7.has_value());
	ASSERT_EQ(tg7->append(tgStr9, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	auto tg8 = metaf::TemperatureGroup::parse(tgStr8, metaf::ReportPart::RMK);
	ASSERT_TRUE(tg8.has_value());
	ASSERT_EQ(tg8->append(tgStr9, metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(tg1->append(tgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg1->append(tgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg1->append(tgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg1->append(tgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg1->append(tgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg1->append(tgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg1->append(tgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg1->append(tgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg1->append(tgStr9, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg1->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg1->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg2->append(tgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg2->append(tgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg2->append(tgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg2->append(tgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg2->append(tgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg2->append(tgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg2->append(tgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg2->append(tgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg2->append(tgStr9, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg2->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg2->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg3->append(tgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg3->append(tgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg3->append(tgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg3->append(tgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg3->append(tgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg3->append(tgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg3->append(tgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg3->append(tgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg3->append(tgStr9, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg3->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg3->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg4->append(tgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg4->append(tgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg4->append(tgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg4->append(tgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg4->append(tgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg4->append(tgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg4->append(tgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg4->append(tgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg4->append(tgStr9, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg4->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg4->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg5->append(tgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg5->append(tgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg5->append(tgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg5->append(tgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg5->append(tgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg5->append(tgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg5->append(tgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg5->append(tgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg5->append(tgStr9, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg5->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg5->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg6->append(tgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg6->append(tgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg6->append(tgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg6->append(tgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg6->append(tgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg6->append(tgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg6->append(tgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg6->append(tgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg6->append(tgStr9, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg6->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg6->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg7->append(tgStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg7->append(tgStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg7->append(tgStr3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg7->append(tgStr4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg7->append(tgStr5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg7->append(tgStr6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg7->append(tgStr7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg7->append(tgStr8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg7->append(tgStr9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg7->append(tStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg7->append(tStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg8->append(tgStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg8->append(tgStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg8->append(tgStr3, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg8->append(tgStr4, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg8->append(tgStr5, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg8->append(tgStr6, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg8->append(tgStr7, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg8->append(tgStr8, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg8->append(tgStr9, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg8->append(tStr1, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg8->append(tStr2, metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}
