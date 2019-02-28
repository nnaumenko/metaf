/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

static const auto margin = 0.01;

TEST(Temperature, fromStringPositive) {
	const auto t = metaf::Temperature::fromString("17");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->temperature().has_value());
	EXPECT_EQ(t->temperature().value(), 17);
	EXPECT_EQ(t->unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(t->isFreezing());
}

TEST(Temperature, fromStringNegative) {
	const auto t = metaf::Temperature::fromString("M09");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->temperature().has_value());
	EXPECT_EQ(t->temperature().value(), -9);
	EXPECT_EQ(t->unit(), metaf::Temperature::Unit::C);
	EXPECT_TRUE(t->isFreezing());
}

TEST(Temperature, fromStringAboveZero) {
	const auto t = metaf::Temperature::fromString("00");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->temperature().has_value());
	EXPECT_EQ(t->temperature().value(), 0);
	EXPECT_EQ(t->unit(), metaf::Temperature::Unit::C);
	EXPECT_FALSE(t->isFreezing());
}

TEST(Temperature, fromStringBelowZero) {
	const auto t = metaf::Temperature::fromString("M00");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->temperature().has_value());
	EXPECT_EQ(t->temperature().value(), 0);
	EXPECT_EQ(t->unit(), metaf::Temperature::Unit::C);
	EXPECT_TRUE(t->isFreezing());
}

TEST(Temperature, fromStringNotReported) {
	const auto t = metaf::Temperature::fromString("//");
	ASSERT_TRUE(t.has_value());
	EXPECT_FALSE(t->temperature().has_value());
	EXPECT_FALSE(t->isFreezing());
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

TEST(Temperature, toUnitSameUnit) {
	const auto t = metaf::Temperature::fromString("07");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->toUnit(metaf::Temperature::Unit::C).has_value());
	EXPECT_NEAR(t->toUnit(metaf::Temperature::Unit::C).value(), 7, margin);
}

TEST(Temperature, toUnitCtoF) {
	const auto temp00 = metaf::Temperature::fromString("00");
	ASSERT_TRUE(temp00.has_value());
	ASSERT_TRUE(temp00->toUnit(metaf::Temperature::Unit::F).has_value());
	EXPECT_NEAR(temp00->toUnit(metaf::Temperature::Unit::F).value(), 32, margin);

	const auto tempM00 = metaf::Temperature::fromString("M00");
	ASSERT_TRUE(tempM00.has_value());
	ASSERT_TRUE(tempM00->toUnit(metaf::Temperature::Unit::F).has_value());
	EXPECT_NEAR(tempM00->toUnit(metaf::Temperature::Unit::F).value(), 32, margin);

	const auto tempM40 = metaf::Temperature::fromString("M40");
	ASSERT_TRUE(tempM40.has_value());
	ASSERT_TRUE(tempM40->toUnit(metaf::Temperature::Unit::F).has_value());
	EXPECT_NEAR(tempM40->toUnit(metaf::Temperature::Unit::F).value(), -40, margin);
}

TEST(Temperature, toUnitNotReported) {
	const auto t = metaf::Temperature::fromString("//");
	ASSERT_TRUE(t.has_value());
	EXPECT_FALSE(t->temperature().has_value());
	EXPECT_FALSE(t->toUnit(metaf::Temperature::Unit::C).has_value());
	EXPECT_FALSE(t->toUnit(metaf::Temperature::Unit::F).has_value());
}
