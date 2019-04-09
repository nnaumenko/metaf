/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

static const auto margin = 0.1 / 2;

TEST(Precipitation, fromRainfallString) {
	const auto p1 = metaf::Precipitation::fromRainfallString("120.7");
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p1->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p1->precipitation().has_value());
	EXPECT_NEAR(p1->precipitation().value(), 120.7, margin);

	const auto p2 = metaf::Precipitation::fromRainfallString("061.5");
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p2->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p2->precipitation().has_value());
	EXPECT_NEAR(p2->precipitation().value(), 61.5, margin);

	const auto p3 = metaf::Precipitation::fromRainfallString("60.1");
	ASSERT_TRUE(p3.has_value());
	EXPECT_EQ(p3->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p3->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p3->precipitation().has_value());
	EXPECT_NEAR(p3->precipitation().value(), 60.1, margin);

	const auto p4 = metaf::Precipitation::fromRainfallString("04.7");
	ASSERT_TRUE(p4.has_value());
	EXPECT_EQ(p4->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p4->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p4->precipitation().has_value());
	EXPECT_NEAR(p4->precipitation().value(), 4.7, margin);

	const auto p5 = metaf::Precipitation::fromRainfallString("000.0");
	ASSERT_TRUE(p5.has_value());
	EXPECT_EQ(p5->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p5->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p5->precipitation().has_value());
	EXPECT_NEAR(p5->precipitation().value(), 0.0, margin);

	const auto p6 = metaf::Precipitation::fromRainfallString("00.0");
	ASSERT_TRUE(p6.has_value());
	EXPECT_EQ(p6->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p6->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p6->precipitation().has_value());
	EXPECT_NEAR(p6->precipitation().value(), 0.0, margin);
}

TEST(Precipitation, fromRainfallStringNotSpecified) {
	const auto p = metaf::Precipitation::fromRainfallString("");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	EXPECT_FALSE(p->precipitation().has_value());
}

TEST(Precipitation, fromRainfallStringNotReported) {
	const auto p1 = metaf::Precipitation::fromRainfallString("///./");
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_EQ(p1->unit(), metaf::Precipitation::Unit::MM);
	EXPECT_FALSE(p1->precipitation().has_value());

	const auto p2 = metaf::Precipitation::fromRainfallString("//./");
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_EQ(p2->unit(), metaf::Precipitation::Unit::MM);
	EXPECT_FALSE(p2->precipitation().has_value());
}

TEST(Precipitation, fromRainfallStringWrongFormat) {
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("0040.2").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("4.2").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("040").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("40").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("040.").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("40.").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString(".2").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString(".").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("///.//").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("/./").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("///.").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("040,2").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("A40.2").has_value());
	EXPECT_FALSE(metaf::Precipitation::fromRainfallString("040.A").has_value());
}

TEST(Precipitation, fromRunwayDepositsValue) {
	const auto p1 = metaf::Precipitation::fromRunwayDeposits("90");
	ASSERT_TRUE(p1.has_value());
	EXPECT_EQ(p1->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p1->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p1->precipitation().has_value());
	EXPECT_NEAR(p1->precipitation().value(), 90.0, margin);

	const auto p2 = metaf::Precipitation::fromRunwayDeposits("01");
	ASSERT_TRUE(p2.has_value());
	EXPECT_EQ(p2->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p2->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p2->precipitation().has_value());
	EXPECT_NEAR(p2->precipitation().value(), 1.0, margin);

	const auto p3 = metaf::Precipitation::fromRunwayDeposits("00");
	ASSERT_TRUE(p3.has_value());
	EXPECT_EQ(p3->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p3->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p3->precipitation().has_value());
	EXPECT_NEAR(p3->precipitation().value(), 0.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecialReserved) {
	EXPECT_FALSE(metaf::Precipitation::fromRunwayDeposits("91").has_value());
}

TEST(Precipitation, fromRunwayDepositsSpecial10cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("92");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->precipitation().has_value());
	EXPECT_NEAR(p->precipitation().value(), 100.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial15cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("93");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->precipitation().has_value());
	EXPECT_NEAR(p->precipitation().value(), 150.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial20cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("94");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->precipitation().has_value());
	EXPECT_NEAR(p->precipitation().value(), 200.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial25cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("95");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->precipitation().has_value());
	EXPECT_NEAR(p->precipitation().value(), 250.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial30cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("96");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->precipitation().has_value());
	EXPECT_NEAR(p->precipitation().value(), 300.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial35cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("97");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->precipitation().has_value());
	EXPECT_NEAR(p->precipitation().value(), 350.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecial40cm) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("98");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->precipitation().has_value());
	EXPECT_NEAR(p->precipitation().value(), 400.0, margin);
}

TEST(Precipitation, fromRunwayDepositsSpecialRunwayNotOperational) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("99");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->status(), metaf::Precipitation::Status::RUNWAY_NOT_OPERATIONAL);
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	EXPECT_FALSE(p->precipitation().has_value());
}

TEST(Precipitation, fromRunwayDepositsNotReported) {
	const auto p = metaf::Precipitation::fromRunwayDeposits("//");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	EXPECT_FALSE(p->precipitation().has_value());
}

TEST(Precipitation, toUnitSameUnit) {
	const auto p = metaf::Precipitation::fromRainfallString("07.5");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->toUnit(metaf::Precipitation::Unit::MM).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Precipitation::Unit::MM).value(), 7.5, margin);
}

TEST(Precipitation, toUnitNotReported) {
	const auto p = metaf::Precipitation::fromRainfallString("");
	ASSERT_TRUE(p.has_value());
	EXPECT_FALSE(p->toUnit(metaf::Precipitation::Unit::MM).has_value());
	EXPECT_FALSE(p->toUnit(metaf::Precipitation::Unit::INCHES).has_value());
}

TEST(Precipitation, toUnitMmToInches) {
	const auto p = metaf::Precipitation::fromRainfallString("50.8");
	ASSERT_TRUE(p.has_value());
	EXPECT_EQ(p->unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(p->toUnit(metaf::Precipitation::Unit::INCHES).has_value());
	EXPECT_NEAR(p->toUnit(metaf::Precipitation::Unit::INCHES).value(), 2.0, margin);
}