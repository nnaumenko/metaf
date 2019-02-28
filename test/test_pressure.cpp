/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

static const auto margin = 0.01 / 2;


TEST(Pressure, fromStringHectopascal) {
	const auto p = metaf::Pressure::fromString("Q0994");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	ASSERT_TRUE(p->pressure().has_value());
	EXPECT_NEAR(p->pressure().value(), 994, margin);
}

TEST(Pressure, fromStringHectopascalNotReported) {
	const auto p = metaf::Pressure::fromString("Q////");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_FALSE(p->pressure().has_value());
}

TEST(Pressure, fromStringInHg) {
	static const auto margin = 0.01 / 2;
	const auto p = metaf::Pressure::fromString("A2934");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Pressure::Unit::INCHES_HG);
	ASSERT_TRUE(p->pressure().has_value());
	EXPECT_NEAR(p->pressure().value(), 29.34, margin);
}

TEST(Pressure, fromStringInHgNotReported) {
	const auto p = metaf::Pressure::fromString("A////");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_FALSE(p->pressure().has_value());
}

TEST(Pressure, fromStringWrongFormat) {
	EXPECT_FALSE(metaf::Pressure::fromString(""));
	EXPECT_FALSE(metaf::Pressure::fromString("A296"));
	EXPECT_FALSE(metaf::Pressure::fromString("A29600"));
	EXPECT_FALSE(metaf::Pressure::fromString("Q999"));
	EXPECT_FALSE(metaf::Pressure::fromString("Q01000"));
	EXPECT_FALSE(metaf::Pressure::fromString("R1000"));
	EXPECT_FALSE(metaf::Pressure::fromString("A290A"));
	EXPECT_FALSE(metaf::Pressure::fromString("Q999A"));
	EXPECT_FALSE(metaf::Pressure::fromString("Q///"));
	EXPECT_FALSE(metaf::Pressure::fromString("Q/////"));
	EXPECT_FALSE(metaf::Pressure::fromString("A///"));
	EXPECT_FALSE(metaf::Pressure::fromString("A/////"));
}

TEST(Pressure, toUnitSameUnit) {
	const auto p1 = metaf::Pressure::fromString("A2934");
	ASSERT_TRUE(p1.has_value());
	ASSERT_TRUE(p1->pressure().has_value());
	EXPECT_EQ(p1->unit(), metaf::Pressure::Unit::INCHES_HG);
	EXPECT_TRUE(p1->toUnit(p1->unit()).has_value());
	EXPECT_EQ(p1->pressure().value(), p1->toUnit(p1->unit()).value());

	const auto p2 = metaf::Pressure::fromString("Q0994");
	ASSERT_TRUE(p2.has_value());
	ASSERT_TRUE(p2->pressure().has_value());
	EXPECT_EQ(p2->unit(), metaf::Pressure::Unit::HECTOPASCAL);
	EXPECT_TRUE(p2->toUnit(p2->unit()).has_value());
	EXPECT_EQ(p2->pressure().value(), p2->toUnit(p2->unit()).value());
}

TEST(Pressure, toUnitNotReported) {
	const auto p1 = metaf::Pressure::fromString("A////");
	ASSERT_TRUE(p1.has_value());
	EXPECT_FALSE(p1->pressure().has_value());
	EXPECT_FALSE(p1->toUnit(metaf::Pressure::Unit::INCHES_HG).has_value());
	EXPECT_FALSE(p1->toUnit(metaf::Pressure::Unit::HECTOPASCAL).has_value());

	const auto p2 = metaf::Pressure::fromString("Q////");
	ASSERT_TRUE(p2.has_value());
	EXPECT_FALSE(p2->pressure().has_value());
	EXPECT_FALSE(p2->toUnit(metaf::Pressure::Unit::INCHES_HG).has_value());
	EXPECT_FALSE(p2->toUnit(metaf::Pressure::Unit::HECTOPASCAL).has_value());
}

TEST(Pressure, toUnitHectopascalToInHg) {
	const auto p = metaf::Pressure::fromString("Q1000");
	ASSERT_TRUE(p.has_value());
	EXPECT_TRUE(p->toUnit(metaf::Pressure::Unit::INCHES_HG).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Pressure::Unit::INCHES_HG).value(), 29.53, margin);
}

TEST(Pressure, toUnitInHgToHectopascal) {
	const auto p = metaf::Pressure::fromString("A2953");
	ASSERT_TRUE(p.has_value());
	EXPECT_TRUE(p->toUnit(metaf::Pressure::Unit::HECTOPASCAL).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Pressure::Unit::HECTOPASCAL).value(), 1000, margin);
}
