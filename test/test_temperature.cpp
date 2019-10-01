/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto tempMargin = 0.1/2;
static const auto rhMargin = 0.1/2;

TEST(Temperature, fromStringPositive) {
	const auto t = metaf::Temperature::fromString("17");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->temperature().has_value());
	EXPECT_NEAR(t->temperature().value(), 17, tempMargin);
	EXPECT_EQ(t->unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(t->isFreezing());
	EXPECT_FALSE(t->isPrecise());
}

TEST(Temperature, fromStringNegative) {
	const auto t = metaf::Temperature::fromString("M09");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->temperature().has_value());
	EXPECT_NEAR(t->temperature().value(), -9, tempMargin);
	EXPECT_EQ(t->unit(), metaf::Temperature::Unit::C);
	EXPECT_TRUE(t->isFreezing());
	EXPECT_FALSE(t->isPrecise());
}

TEST(Temperature, fromStringAboveZero) {
	const auto t = metaf::Temperature::fromString("00");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->temperature().has_value());
	EXPECT_NEAR(t->temperature().value(), 0, tempMargin);
	EXPECT_EQ(t->unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(t->isFreezing());
	EXPECT_FALSE(t->isPrecise());
}

TEST(Temperature, fromStringBelowZero) {
	const auto t = metaf::Temperature::fromString("M00");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->temperature().has_value());
	EXPECT_NEAR(t->temperature().value(), 0, tempMargin);
	EXPECT_EQ(t->unit(), metaf::Temperature::Unit::C);
	EXPECT_TRUE(t->isFreezing());
	EXPECT_FALSE(t->isPrecise());
}

TEST(Temperature, fromStringNotReported) {
	const auto t = metaf::Temperature::fromString("//");
	ASSERT_TRUE(t.has_value());
	EXPECT_FALSE(t->temperature().has_value());
	EXPECT_FALSE(t->isFreezing());
	EXPECT_FALSE(t->isPrecise());
}

TEST(Temperature, fromStringWrongFormat) {
	EXPECT_FALSE(metaf::Temperature::fromString("").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("017").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("7").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("M017").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("M7").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("T12").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("TX10").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("M//").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("/").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("///").has_value());
}

TEST(Temperature, fromRemarkStringPositive) {
	const auto t = metaf::Temperature::fromRemarkString("0147");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->temperature().has_value());
	EXPECT_NEAR(t->temperature().value(), 14.7, tempMargin);
	EXPECT_EQ(t->unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(t->isFreezing());
	EXPECT_TRUE(t->isPrecise());
}

TEST(Temperature, fromRemarkStringNegative) {
	const auto t = metaf::Temperature::fromRemarkString("1274");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->temperature().has_value());
	EXPECT_NEAR(t->temperature().value(), -27.4, tempMargin);
	EXPECT_EQ(t->unit(), metaf::Temperature::Unit::C);
	EXPECT_TRUE(t->isFreezing());
	EXPECT_TRUE(t->isPrecise());
}

TEST(Temperature, fromRemarkStringWrongFormat) {
	EXPECT_FALSE(metaf::Temperature::fromString("").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("2123").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("3123").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("4123").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("5123").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("6123").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("7123").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("8123").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("M123").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("012").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("00123").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("0///").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("M///").has_value());
	EXPECT_FALSE(metaf::Temperature::fromString("////").has_value());
}

TEST(Temperature, toUnitSameUnit) {
	const auto t = metaf::Temperature::fromString("07");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->toUnit(metaf::Temperature::Unit::C).has_value());
	EXPECT_NEAR(t->toUnit(metaf::Temperature::Unit::C).value(), 7, tempMargin);
}

TEST(Temperature, toUnitCtoF) {
	const auto temp00 = metaf::Temperature::fromString("00");
	ASSERT_TRUE(temp00.has_value());
	ASSERT_TRUE(temp00->toUnit(metaf::Temperature::Unit::F).has_value());
	EXPECT_NEAR(temp00->toUnit(metaf::Temperature::Unit::F).value(), 32, tempMargin);

	const auto tempM00 = metaf::Temperature::fromString("M00");
	ASSERT_TRUE(tempM00.has_value());
	ASSERT_TRUE(tempM00->toUnit(metaf::Temperature::Unit::F).has_value());
	EXPECT_NEAR(tempM00->toUnit(metaf::Temperature::Unit::F).value(), 32, tempMargin);

	const auto tempM40 = metaf::Temperature::fromString("M40");
	ASSERT_TRUE(tempM40.has_value());
	ASSERT_TRUE(tempM40->toUnit(metaf::Temperature::Unit::F).has_value());
	EXPECT_NEAR(tempM40->toUnit(metaf::Temperature::Unit::F).value(), -40, tempMargin);
}

TEST(Temperature, toUnitNotReported) {
	const auto t = metaf::Temperature::fromString("//");
	ASSERT_TRUE(t.has_value());
	EXPECT_FALSE(t->temperature().has_value());
	EXPECT_FALSE(t->toUnit(metaf::Temperature::Unit::C).has_value());
	EXPECT_FALSE(t->toUnit(metaf::Temperature::Unit::F).has_value());
}

TEST(Temperature, relativeHumidity) {
	const auto t35 = metaf::Temperature::fromString("35");
	ASSERT_TRUE(t35.has_value());
	ASSERT_TRUE(t35->temperature().has_value());
	ASSERT_NEAR(t35->temperature().value(), 35, tempMargin);

	const auto t05 = metaf::Temperature::fromString("05");
	ASSERT_TRUE(t05.has_value());
	ASSERT_TRUE(t05->temperature().has_value());
	ASSERT_NEAR(t05->temperature().value(), 5, tempMargin);

	const auto t17 = metaf::Temperature::fromString("17");
	ASSERT_TRUE(t17.has_value());
	ASSERT_TRUE(t17->temperature().has_value());
	ASSERT_NEAR(t17->temperature().value(), 17, tempMargin);

	const auto t08 = metaf::Temperature::fromString("08");
	ASSERT_TRUE(t08.has_value());
	ASSERT_TRUE(t08->temperature().has_value());
	ASSERT_NEAR(t08->temperature().value(), 8, tempMargin);

	const auto t22_5 = metaf::Temperature::fromRemarkString("0225");
	ASSERT_TRUE(t22_5.has_value());
	ASSERT_TRUE(t22_5->temperature().has_value());
	ASSERT_NEAR(t22_5->temperature().value(), 22.5, tempMargin);

	const auto tm05 = metaf::Temperature::fromString("M05");
	ASSERT_TRUE(tm05.has_value());
	ASSERT_TRUE(tm05->temperature().has_value());
	ASSERT_NEAR(tm05->temperature().value(), -5, tempMargin);

	const auto t00 = metaf::Temperature::fromString("00");
	ASSERT_TRUE(t00.has_value());
	ASSERT_TRUE(t00->temperature().has_value());
	ASSERT_NEAR(t00->temperature().value(), 0, tempMargin);

	const auto tm00 = metaf::Temperature::fromString("M00");
	ASSERT_TRUE(tm00.has_value());
	ASSERT_TRUE(tm00->temperature().has_value());
	ASSERT_NEAR(tm00->temperature().value(), 0, tempMargin);

	const auto rh1 = metaf::Temperature::relativeHumidity(t35.value(), t05.value());
	ASSERT_TRUE(rh1.has_value());
	EXPECT_NEAR(rh1.value(), 15.6, rhMargin);

	const auto rh2 = metaf::Temperature::relativeHumidity(t17.value(), t08.value());
	ASSERT_TRUE(rh2.has_value());
	EXPECT_NEAR(rh2.value(), 55.4, rhMargin);

	const auto rh3 = metaf::Temperature::relativeHumidity(t22_5.value(), t08.value());
	ASSERT_TRUE(rh3.has_value());
	EXPECT_NEAR(rh3.value(), 39.4, rhMargin);

	const auto rh4 = metaf::Temperature::relativeHumidity(t17.value(), t00.value());
	ASSERT_TRUE(rh4.has_value());
	EXPECT_NEAR(rh4.value(), 31.6, rhMargin);

	const auto rh5 = metaf::Temperature::relativeHumidity(t17.value(), tm00.value());
	ASSERT_TRUE(rh5.has_value());
	EXPECT_NEAR(rh5.value(), 31.6, rhMargin);

	const auto rh6 = metaf::Temperature::relativeHumidity(t35.value(), tm05.value());
	ASSERT_TRUE(rh6.has_value());
	EXPECT_NEAR(rh6.value(), 7.5, rhMargin);
}

TEST(Temperature, relativeHumidity100percent) {
	const auto t25 = metaf::Temperature::fromString("25");
	ASSERT_TRUE(t25.has_value());
	ASSERT_TRUE(t25->temperature().has_value());
	ASSERT_NEAR(t25->temperature().value(), 25, tempMargin);

	const auto t25_1 = metaf::Temperature::fromRemarkString("0251");
	ASSERT_TRUE(t25_1.has_value());
	ASSERT_TRUE(t25_1->temperature().has_value());
	ASSERT_NEAR(t25_1->temperature().value(), 25.1, tempMargin);

	const auto t26 = metaf::Temperature::fromString("26");
	ASSERT_TRUE(t26.has_value());
	ASSERT_TRUE(t26->temperature().has_value());
	ASSERT_NEAR(t26->temperature().value(), 26, tempMargin);

	const auto t00 = metaf::Temperature::fromString("00");
	ASSERT_TRUE(t00.has_value());
	ASSERT_TRUE(t00->temperature().has_value());
	ASSERT_NEAR(t00->temperature().value(), 0, tempMargin);

	const auto tm00 = metaf::Temperature::fromString("M00");
	ASSERT_TRUE(tm00.has_value());
	ASSERT_TRUE(tm00->temperature().has_value());
	ASSERT_NEAR(tm00->temperature().value(), 0, tempMargin);

	const auto rh1 = metaf::Temperature::relativeHumidity(t25.value(), t25.value());
	ASSERT_TRUE(rh1.has_value());
	EXPECT_NEAR(rh1.value(), 100.0, rhMargin);

	const auto rh2 = metaf::Temperature::relativeHumidity(t25.value(), t26.value());
	ASSERT_TRUE(rh2.has_value());
	EXPECT_NEAR(rh2.value(), 100.0, rhMargin);	

	const auto rh3 = metaf::Temperature::relativeHumidity(t25.value(), t25_1.value());
	ASSERT_TRUE(rh3.has_value());
	EXPECT_NEAR(rh3.value(), 100.0, rhMargin);	

	const auto rh4 = metaf::Temperature::relativeHumidity(t25.value(), t25_1.value());
	ASSERT_TRUE(rh4.has_value());
	EXPECT_NEAR(rh4.value(), 100.0, rhMargin);	

	const auto rh5 = metaf::Temperature::relativeHumidity(t00.value(), tm00.value());
	ASSERT_TRUE(rh5.has_value());
	EXPECT_NEAR(rh5.value(), 100.0, rhMargin);	

	const auto rh6 = metaf::Temperature::relativeHumidity(tm00.value(), t00.value());
	ASSERT_TRUE(rh6.has_value());
	EXPECT_NEAR(rh6.value(), 100.0, rhMargin);	
}

TEST(Temperature, relativeHumidityNotDefined) {
	const auto t17 = metaf::Temperature::fromString("17");
	ASSERT_TRUE(t17.has_value());
	ASSERT_TRUE(t17->temperature().has_value());
	ASSERT_NEAR(t17->temperature().value(), 17, tempMargin);

	const auto tnr = metaf::Temperature::fromString("//");
	ASSERT_TRUE(tnr.has_value());
	ASSERT_FALSE(tnr->temperature().has_value());

	EXPECT_FALSE(metaf::Temperature::relativeHumidity(t17.value(), tnr.value()).has_value());
	EXPECT_FALSE(metaf::Temperature::relativeHumidity(tnr.value(), t17.value()).has_value());
}

TEST(Temperature, heatIndex) {
	const auto t42 = metaf::Temperature::fromString("42");
	ASSERT_TRUE(t42.has_value());
	ASSERT_TRUE(t42->temperature().has_value());
	ASSERT_NEAR(t42->temperature().value(), 42, tempMargin);
	ASSERT_EQ(t42->unit(), metaf::Temperature::Unit::C);

	const auto t27 = metaf::Temperature::fromString("27");
	ASSERT_TRUE(t27.has_value());
	ASSERT_TRUE(t27->temperature().has_value());
	ASSERT_NEAR(t27->temperature().value(), 27, tempMargin);
	ASSERT_EQ(t27->unit(), metaf::Temperature::Unit::C);

	const auto t33_5 = metaf::Temperature::fromRemarkString("0335");
	ASSERT_TRUE(t33_5.has_value());
	ASSERT_TRUE(t33_5->temperature().has_value());
	ASSERT_NEAR(t33_5->temperature().value(), 33.5, tempMargin);
	ASSERT_EQ(t33_5->unit(), metaf::Temperature::Unit::C);

	const auto hi1 = metaf::Temperature::heatIndex(t27.value(), 40);
	ASSERT_TRUE(hi1.temperature().has_value());
	EXPECT_NEAR(hi1.temperature().value(), 26.9, tempMargin);
	EXPECT_EQ(hi1.unit(), metaf::Temperature::Unit::C);

	const auto hi2 = metaf::Temperature::heatIndex(t27.value(), 100);
	ASSERT_TRUE(hi2.temperature().has_value());
	EXPECT_NEAR(hi2.temperature().value(), 31.8, tempMargin);
	EXPECT_EQ(hi2.unit(), metaf::Temperature::Unit::C);

	const auto hi3 = metaf::Temperature::heatIndex(t42.value(), 40);
	ASSERT_TRUE(hi3.temperature().has_value());
	EXPECT_NEAR(hi3.temperature().value(), 53.7, tempMargin);
	EXPECT_EQ(hi3.unit(), metaf::Temperature::Unit::C);

	const auto hi4 = metaf::Temperature::heatIndex(t42.value(), 68);
	ASSERT_TRUE(hi4.temperature().has_value());
	EXPECT_NEAR(hi4.temperature().value(), 79.9, tempMargin);
	EXPECT_EQ(hi4.unit(), metaf::Temperature::Unit::C);

	const auto hi5 = metaf::Temperature::heatIndex(t33_5.value(), 65);
	ASSERT_TRUE(hi5.temperature().has_value());
	EXPECT_NEAR(hi5.temperature().value(), 42.9, tempMargin);
	EXPECT_EQ(hi5.unit(), metaf::Temperature::Unit::C);
}

TEST(Temperature, heatIndexNotDefined) {
	const auto t26_9 = metaf::Temperature::fromRemarkString("0269");
	ASSERT_TRUE(t26_9.has_value());
	ASSERT_TRUE(t26_9->temperature().has_value());
	ASSERT_NEAR(t26_9->temperature().value(), 26.9, tempMargin);

	const auto t27 = metaf::Temperature::fromString("27");
	ASSERT_TRUE(t27.has_value());
	ASSERT_TRUE(t27->temperature().has_value());
	ASSERT_NEAR(t27->temperature().value(), 27, tempMargin);

	const auto t0 = metaf::Temperature::fromString("00");
	ASSERT_TRUE(t0.has_value());
	ASSERT_TRUE(t0->temperature().has_value());
	ASSERT_NEAR(t0->temperature().value(), 0, tempMargin);

	const auto tm0 = metaf::Temperature::fromString("M00");
	ASSERT_TRUE(tm0.has_value());
	ASSERT_TRUE(tm0->temperature().has_value());
	ASSERT_NEAR(tm0->temperature().value(), 0, tempMargin);

	const auto tm0_1 = metaf::Temperature::fromRemarkString("1001");
	ASSERT_TRUE(tm0_1.has_value());
	ASSERT_TRUE(tm0_1->temperature().has_value());
	ASSERT_NEAR(tm0_1->temperature().value(), -0.1, tempMargin);	

	const auto tnr = metaf::Temperature::fromString("//");
	ASSERT_TRUE(tnr.has_value());
	ASSERT_FALSE(tnr->temperature().has_value());

	EXPECT_FALSE(
		metaf::Temperature::heatIndex(t26_9.value(), 50.0).temperature().has_value());
	EXPECT_FALSE(
		metaf::Temperature::heatIndex(t0.value(), 50.0).temperature().has_value());
	EXPECT_FALSE(
		metaf::Temperature::heatIndex(tm0.value(), 50.0).temperature().has_value());
	EXPECT_FALSE(
		metaf::Temperature::heatIndex(tm0_1.value(), 50.0).temperature().has_value());
	EXPECT_FALSE(
		metaf::Temperature::heatIndex(t27.value(), 39.9).temperature().has_value());
	EXPECT_FALSE(
		metaf::Temperature::heatIndex(tnr.value(), 50.0).temperature().has_value());
}

TEST(Temperature, heatIndexTempDewPoint) {
	const auto t30 = metaf::Temperature::fromString("30");
	ASSERT_TRUE(t30.has_value());
	ASSERT_TRUE(t30->temperature().has_value());
	ASSERT_NEAR(t30->temperature().value(), 30, tempMargin);
	ASSERT_EQ(t30->unit(), metaf::Temperature::Unit::C);

	const auto t33 = metaf::Temperature::fromString("33");
	ASSERT_TRUE(t33.has_value());
	ASSERT_TRUE(t33->temperature().has_value());
	ASSERT_NEAR(t33->temperature().value(), 33, tempMargin);
	ASSERT_EQ(t33->unit(), metaf::Temperature::Unit::C);

	const auto rh = metaf::Temperature::relativeHumidity(t33.value(), t30.value());
	ASSERT_TRUE(rh.has_value());
	ASSERT_NEAR(rh.value(), 84.4, rhMargin);

	const auto hi1 = metaf::Temperature::heatIndex(t33.value(), rh.value());
	ASSERT_TRUE(hi1.temperature().has_value());
	EXPECT_NEAR(hi1.temperature().value(), 50.4, tempMargin);
	EXPECT_EQ(hi1.unit(), metaf::Temperature::Unit::C);

	const auto hi2 = metaf::Temperature::heatIndex(t33.value(), t30.value());
	ASSERT_TRUE(hi2.temperature().has_value());
	EXPECT_NEAR(hi2.temperature().value(), 50.4, tempMargin);
	EXPECT_EQ(hi2.unit(), metaf::Temperature::Unit::C);
}

TEST(Temperature, windChill) {
	const auto t10 = metaf::Temperature::fromString("10");
	ASSERT_TRUE(t10.has_value());
	ASSERT_TRUE(t10->temperature().has_value());
	ASSERT_NEAR(t10->temperature().value(), 10.0, tempMargin);
	ASSERT_EQ(t10->unit(), metaf::Temperature::Unit::C);

	const auto tm14_5 = metaf::Temperature::fromRemarkString("1145");
	ASSERT_TRUE(tm14_5.has_value());
	ASSERT_TRUE(tm14_5->temperature().has_value());
	ASSERT_NEAR(tm14_5->temperature().value(), -14.5, tempMargin);
	ASSERT_EQ(tm14_5->unit(), metaf::Temperature::Unit::C);

	const auto t0 = metaf::Temperature::fromString("00");
	ASSERT_TRUE(t0.has_value());
	ASSERT_TRUE(t0->temperature().has_value());
	ASSERT_NEAR(t0->temperature().value(), 0, tempMargin);
	ASSERT_EQ(t0->unit(), metaf::Temperature::Unit::C);

	const auto tm0 = metaf::Temperature::fromString("M00");
	ASSERT_TRUE(tm0.has_value());
	ASSERT_TRUE(tm0->temperature().has_value());
	ASSERT_NEAR(tm0->temperature().value(), 0, tempMargin);
	ASSERT_EQ(tm0->unit(), metaf::Temperature::Unit::C);

	const auto s5 = metaf::Speed::fromString("05", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(s5.has_value());
	ASSERT_TRUE(s5->speed().has_value());
	ASSERT_EQ(s5->speed().value(), 5u);

	const auto s15 = metaf::Speed::fromString("15", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(s15.has_value());
	ASSERT_TRUE(s15->speed().has_value());
	ASSERT_EQ(s15->speed().value(), 15u);

	const auto s35 = metaf::Speed::fromString("35", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(s35.has_value());
	ASSERT_TRUE(s35->speed().has_value());
	ASSERT_EQ(s35->speed().value(), 35u);

	const auto wc1 = metaf::Temperature::windChill(t10.value(), s5.value());
	ASSERT_TRUE(wc1.temperature().has_value());
	EXPECT_NEAR(wc1.temperature().value(), 9.8, tempMargin);
	EXPECT_EQ(wc1.unit(), metaf::Temperature::Unit::C);

	const auto wc2 = metaf::Temperature::windChill(tm14_5.value(), s35.value());
	ASSERT_TRUE(wc2.temperature().has_value());
	EXPECT_NEAR(wc2.temperature().value(), -26.1, tempMargin);
	EXPECT_EQ(wc2.unit(), metaf::Temperature::Unit::C);

	const auto wc3 = metaf::Temperature::windChill(tm14_5.value(), s15.value());
	ASSERT_TRUE(wc3.temperature().has_value());
	EXPECT_NEAR(wc3.temperature().value(), -22.3, tempMargin);
	EXPECT_EQ(wc3.unit(), metaf::Temperature::Unit::C);

	const auto wc4 = metaf::Temperature::windChill(t0.value(), s15.value());
	ASSERT_TRUE(wc4.temperature().has_value());
	EXPECT_NEAR(wc4.temperature().value(), -4.4, tempMargin);
	EXPECT_EQ(wc4.unit(), metaf::Temperature::Unit::C);

	const auto wc5 = metaf::Temperature::windChill(tm0.value(), s15.value());
	ASSERT_TRUE(wc5.temperature().has_value());
	EXPECT_NEAR(wc5.temperature().value(), -4.4, tempMargin);
	EXPECT_EQ(wc5.unit(), metaf::Temperature::Unit::C);
}

TEST(Temperature, windChillNotDefined) {
	const auto t10_1 = metaf::Temperature::fromRemarkString("0101");
	ASSERT_TRUE(t10_1.has_value());
	ASSERT_TRUE(t10_1->temperature().has_value());
	ASSERT_NEAR(t10_1->temperature().value(), 10.1, tempMargin);
	ASSERT_EQ(t10_1->unit(), metaf::Temperature::Unit::C);

	const auto t9 = metaf::Temperature::fromString("09");
	ASSERT_TRUE(t9.has_value());
	ASSERT_TRUE(t9->temperature().has_value());
	ASSERT_NEAR(t9->temperature().value(), 9, tempMargin);
	ASSERT_EQ(t9->unit(), metaf::Temperature::Unit::C);

	const auto tnr = metaf::Temperature::fromString("//");
	ASSERT_TRUE(tnr.has_value());
	ASSERT_FALSE(tnr->isReported());

	const auto s4 = metaf::Speed::fromString("04", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(s4.has_value());
	ASSERT_TRUE(s4->speed().has_value());
	ASSERT_EQ(s4->speed().value(), 4u);
	ASSERT_EQ(s4->unit(), metaf::Speed::Unit::KILOMETERS_PER_HOUR);

	const auto s5 = metaf::Speed::fromString("05", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(s5.has_value());
	ASSERT_TRUE(s5->speed().has_value());
	ASSERT_EQ(s5->speed().value(), 5u);
	ASSERT_EQ(s5->unit(), metaf::Speed::Unit::KILOMETERS_PER_HOUR);

	const auto snr = metaf::Speed::fromString("//", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	ASSERT_TRUE(snr.has_value());
	ASSERT_FALSE(snr->isReported());

	EXPECT_FALSE(
		metaf::Temperature::windChill(t10_1.value(), s5.value()).isReported());
	EXPECT_FALSE(
		metaf::Temperature::windChill(t9.value(), s4.value()).isReported());

	EXPECT_FALSE(
		metaf::Temperature::windChill(tnr.value(), s5.value()).isReported());
	EXPECT_FALSE(
		metaf::Temperature::windChill(t9.value(), snr.value()).isReported());

}

TEST(Temperature, isReported) {
	const auto t17 = metaf::Temperature::fromString("17");
	ASSERT_TRUE(t17.has_value());
	EXPECT_TRUE(t17->isReported());

	const auto tnr = metaf::Temperature::fromString("//");
	ASSERT_TRUE(tnr.has_value());
	EXPECT_FALSE(tnr->isReported());
}
