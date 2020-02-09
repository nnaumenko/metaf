/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.01 / 2;

///////////////////////////////////////////////////////////////////////////////
// Parsing tests
// Purpose: to confirm that surface friction and braking action strings are 
// parsed correctly, the reserved values and 'unreliable or unmeasurable' value
// are correctly identified, and that other string formats are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(SurfaceFriction, fromStringFrictionCoefficient) {
	const auto sf1 = metaf::SurfaceFriction::fromString("90");
	ASSERT_TRUE(sf1.has_value());
	EXPECT_EQ(sf1->type(), metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);
	ASSERT_TRUE(sf1->coefficient().has_value());
	EXPECT_NEAR(sf1->coefficient().value(), 0.9, margin);

	const auto sf2 = metaf::SurfaceFriction::fromString("62");
	ASSERT_TRUE(sf2.has_value());
	EXPECT_EQ(sf2->type(), metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);
	ASSERT_TRUE(sf2->coefficient().has_value());
	EXPECT_NEAR(sf2->coefficient().value(), 0.62, margin);

	const auto sf3 = metaf::SurfaceFriction::fromString("08");
	ASSERT_TRUE(sf3.has_value());
	EXPECT_EQ(sf3->type(), metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);
	ASSERT_TRUE(sf3->coefficient().has_value());
	EXPECT_NEAR(sf3->coefficient().value(), 0.08, margin);

	const auto sf4 = metaf::SurfaceFriction::fromString("00");
	ASSERT_TRUE(sf4.has_value());
	EXPECT_EQ(sf4->type(), metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);
	ASSERT_TRUE(sf4->coefficient().has_value());
	EXPECT_NEAR(sf4->coefficient().value(), 0.0, margin);
}

TEST(SurfaceFriction, fromStringBrakingActionPoor) {
	const auto sf = metaf::SurfaceFriction::fromString("91");
	ASSERT_TRUE(sf.has_value());
	EXPECT_EQ(sf->type(), metaf::SurfaceFriction::Type::BRAKING_ACTION_REPORTED);
	ASSERT_TRUE(sf->coefficient().has_value());
	EXPECT_NEAR(sf->coefficient().value(), 0.0, margin);
}

TEST(SurfaceFriction, fromStringBrakingActionMediumPoor) {
	const auto sf = metaf::SurfaceFriction::fromString("92");
	ASSERT_TRUE(sf.has_value());
	EXPECT_EQ(sf->type(), metaf::SurfaceFriction::Type::BRAKING_ACTION_REPORTED);
	ASSERT_TRUE(sf->coefficient().has_value());
	EXPECT_NEAR(sf->coefficient().value(), 0.26, margin);
}

TEST(SurfaceFriction, fromStringBrakingActionMedium) {
	const auto sf = metaf::SurfaceFriction::fromString("93");
	ASSERT_TRUE(sf.has_value());
	EXPECT_EQ(sf->type(), metaf::SurfaceFriction::Type::BRAKING_ACTION_REPORTED);
	ASSERT_TRUE(sf->coefficient().has_value());
	EXPECT_NEAR(sf->coefficient().value(), 0.30, margin);
}

TEST(SurfaceFriction, fromStringBrakingActionMediumGood) {
	const auto sf = metaf::SurfaceFriction::fromString("94");
	ASSERT_TRUE(sf.has_value());
	EXPECT_EQ(sf->type(), metaf::SurfaceFriction::Type::BRAKING_ACTION_REPORTED);
	ASSERT_TRUE(sf->coefficient().has_value());
	EXPECT_NEAR(sf->coefficient().value(), 0.36, margin);
}

TEST(SurfaceFriction, fromStringBrakingActionGood) {
	const auto sf = metaf::SurfaceFriction::fromString("95");
	ASSERT_TRUE(sf.has_value());
	EXPECT_EQ(sf->type(), metaf::SurfaceFriction::Type::BRAKING_ACTION_REPORTED);
	ASSERT_TRUE(sf->coefficient().has_value());
	EXPECT_NEAR(sf->coefficient().value(), 0.40, margin);
}

TEST(SurfaceFriction, fromStringReservedValues) {
	EXPECT_FALSE(metaf::SurfaceFriction::fromString("96").has_value());
	EXPECT_FALSE(metaf::SurfaceFriction::fromString("97").has_value());
	EXPECT_FALSE(metaf::SurfaceFriction::fromString("98").has_value());
}

TEST(SurfaceFriction, fromStringUnreliable) {
	const auto sf = metaf::SurfaceFriction::fromString("99");
	ASSERT_TRUE(sf.has_value());
	EXPECT_EQ(sf->type(), metaf::SurfaceFriction::Type::UNRELIABLE);
	EXPECT_FALSE(sf->coefficient().has_value());
}

TEST(SurfaceFriction, fromStringNotReported) {
	const auto sf = metaf::SurfaceFriction::fromString("//");
	ASSERT_TRUE(sf.has_value());
	EXPECT_EQ(sf->type(), metaf::SurfaceFriction::Type::NOT_REPORTED);
	EXPECT_FALSE(sf->coefficient().has_value());
}

TEST(SurfaceFriction, fromStringWrongFormat) {
	EXPECT_FALSE(metaf::SurfaceFriction::fromString("").has_value());
	EXPECT_FALSE(metaf::SurfaceFriction::fromString("6").has_value());
	EXPECT_FALSE(metaf::SurfaceFriction::fromString("062").has_value());
	EXPECT_FALSE(metaf::SurfaceFriction::fromString("0A").has_value());
	EXPECT_FALSE(metaf::SurfaceFriction::fromString("///").has_value());
	EXPECT_FALSE(metaf::SurfaceFriction::fromString("/").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Braking action
// Purpose: to confirm that correct braking action values are returned for 
// specified surface friction values
///////////////////////////////////////////////////////////////////////////////

TEST(SurfaceFriction, brakingActionPoor) {
	const auto sf1 = metaf::SurfaceFriction::fromString("00");
	ASSERT_TRUE(sf1.has_value());
	EXPECT_EQ(sf1->brakingAction(), metaf::SurfaceFriction::BrakingAction::POOR);

	const auto sf2 = metaf::SurfaceFriction::fromString("25");
	ASSERT_TRUE(sf2.has_value());
	EXPECT_EQ(sf2->brakingAction(), metaf::SurfaceFriction::BrakingAction::POOR);
}

TEST(SurfaceFriction, brakingActionMediumPoor) {
	const auto sf1 = metaf::SurfaceFriction::fromString("26");
	ASSERT_TRUE(sf1.has_value());
	EXPECT_EQ(sf1->brakingAction(), metaf::SurfaceFriction::BrakingAction::MEDIUM_POOR);

	const auto sf2 = metaf::SurfaceFriction::fromString("29");
	ASSERT_TRUE(sf2.has_value());
	EXPECT_EQ(sf2->brakingAction(), metaf::SurfaceFriction::BrakingAction::MEDIUM_POOR);
}

TEST(SurfaceFriction, brakingActionMedium) {
	const auto sf1 = metaf::SurfaceFriction::fromString("30");
	ASSERT_TRUE(sf1.has_value());
	EXPECT_EQ(sf1->brakingAction(), metaf::SurfaceFriction::BrakingAction::MEDIUM);

	const auto sf2 = metaf::SurfaceFriction::fromString("35");
	ASSERT_TRUE(sf2.has_value());
	EXPECT_EQ(sf2->brakingAction(), metaf::SurfaceFriction::BrakingAction::MEDIUM);
}

TEST(SurfaceFriction, brakingActionMediumGood) {
	const auto sf1 = metaf::SurfaceFriction::fromString("36");
	ASSERT_TRUE(sf1.has_value());
	EXPECT_EQ(sf1->brakingAction(), metaf::SurfaceFriction::BrakingAction::MEDIUM_GOOD);

	const auto sf2 = metaf::SurfaceFriction::fromString("39");
	ASSERT_TRUE(sf2.has_value());
	EXPECT_EQ(sf2->brakingAction(), metaf::SurfaceFriction::BrakingAction::MEDIUM_GOOD);
}

TEST(SurfaceFriction, brakingActionGood) {
	const auto sf1 = metaf::SurfaceFriction::fromString("40");
	ASSERT_TRUE(sf1.has_value());
	EXPECT_EQ(sf1->brakingAction(), metaf::SurfaceFriction::BrakingAction::GOOD);

	const auto sf2 = metaf::SurfaceFriction::fromString("90");
	ASSERT_TRUE(sf2.has_value());
	EXPECT_EQ(sf2->brakingAction(), metaf::SurfaceFriction::BrakingAction::GOOD);
}

TEST(SurfaceFriction, brakingActionUnreliable) {
	const auto sf = metaf::SurfaceFriction::fromString("99");
	ASSERT_TRUE(sf.has_value());
	EXPECT_EQ(sf->brakingAction(), metaf::SurfaceFriction::BrakingAction::NONE);
}

TEST(SurfaceFriction, brakingActionNotReported) {
	const auto sf = metaf::SurfaceFriction::fromString("//");
	ASSERT_TRUE(sf.has_value());
	EXPECT_EQ(sf->brakingAction(), metaf::SurfaceFriction::BrakingAction::NONE);
}

///////////////////////////////////////////////////////////////////////////////
// isReported() test
// Purpose: to confirm that isReported() method correctly distingishes between
// reported and non-reported values.
///////////////////////////////////////////////////////////////////////////////

TEST(SurfaceFriction, isReported) {
	const auto sf62 = metaf::SurfaceFriction::fromString("62");
	ASSERT_TRUE(sf62.has_value());
	EXPECT_TRUE(sf62->isReported());

	const auto sfNr = metaf::SurfaceFriction::fromString("//");
	ASSERT_TRUE(sfNr.has_value());
	EXPECT_FALSE(sfNr->isReported());
}

///////////////////////////////////////////////////////////////////////////////
// isUnreliable() test
// Purpose: to confirm that isUnreliable() method correctly identifies value of
// 'unreliable or unmeasurable' kind.
///////////////////////////////////////////////////////////////////////////////

TEST(SurfaceFriction, isUnreliable) {
	const auto sf99 = metaf::SurfaceFriction::fromString("99");
	ASSERT_TRUE(sf99.has_value());
	EXPECT_TRUE(sf99->isUnreliable());

	const auto sf93 = metaf::SurfaceFriction::fromString("93");
	ASSERT_TRUE(sf93.has_value());
	EXPECT_FALSE(sf93->isUnreliable());

	const auto sf39 = metaf::SurfaceFriction::fromString("39");
	ASSERT_TRUE(sf39.has_value());
	EXPECT_FALSE(sf39->isUnreliable());

	const auto sfNr = metaf::SurfaceFriction::fromString("//");
	ASSERT_TRUE(sfNr.has_value());
	EXPECT_FALSE(sfNr->isUnreliable());
}