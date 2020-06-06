/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto sfMargin = 0.01 / 2;

///////////////////////////////////////////////////////////////////////////////
// Test parsing of Rxx/xxxxxxxx groups
// Purpose: to confirm that the groups which describe runway state and 
// identify non-operational runway are parsed correctly and only within correct
// report part, and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(RunwayStateGroup, parseRwState) {
	const auto rsg = metaf::RunwayStateGroup::parse("R16/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->runway().number(), 16u);
	EXPECT_EQ(rsg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(rsg->type(), metaf::RunwayStateGroup::Type::RUNWAY_STATE);
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES);
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT);
	EXPECT_EQ(rsg->depositDepth().amount(), 01u);
	EXPECT_EQ(rsg->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);
	ASSERT_TRUE(rsg->surfaceFriction().coefficient().has_value());
	EXPECT_NEAR(rsg->surfaceFriction().coefficient().value(), 0.55, sfMargin);
}

TEST(RunwayStateGroup, parseRwStateRunwayWithDesignator) {
	const auto rsg1 = metaf::RunwayStateGroup::parse("R16R/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	EXPECT_EQ(rsg1->runway().number(), 16u);
	EXPECT_EQ(rsg1->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rsg1->type(), metaf::RunwayStateGroup::Type::RUNWAY_STATE);

	const auto rsg2 = metaf::RunwayStateGroup::parse("R16L/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	EXPECT_EQ(rsg2->runway().number(), 16u);
	EXPECT_EQ(rsg2->runway().designator(), metaf::Runway::Designator::LEFT);
	EXPECT_EQ(rsg2->type(), metaf::RunwayStateGroup::Type::RUNWAY_STATE);

	const auto rsg3 = metaf::RunwayStateGroup::parse("R16C/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());
	EXPECT_EQ(rsg3->runway().number(), 16u);
	EXPECT_EQ(rsg3->runway().designator(), metaf::Runway::Designator::CENTER);
	EXPECT_EQ(rsg3->type(), metaf::RunwayStateGroup::Type::RUNWAY_STATE);
}

TEST(RunwayStateGroup, parseRwStateRunwayNotOperational) {
	const auto rsg = metaf::RunwayStateGroup::parse("R16/8999//", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->runway().number(), 16u);
	EXPECT_EQ(rsg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(rsg->type(), metaf::RunwayStateGroup::Type::RUNWAY_NOT_OPERATIONAL);
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::COMPACTED_OR_ROLLED_SNOW);
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT);
	EXPECT_FALSE(rsg->depositDepth().isReported());
	EXPECT_EQ(rsg->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::NOT_REPORTED);
}

TEST(RunwayStateGroup, parseRwStateDepositDepthNotReported) {
	const auto rsg = metaf::RunwayStateGroup::parse("R16/29//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->runway().number(), 16u);
	EXPECT_EQ(rsg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(rsg->type(), metaf::RunwayStateGroup::Type::RUNWAY_STATE);
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES);
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT);
	EXPECT_FALSE(rsg->depositDepth().isReported());
	EXPECT_EQ(rsg->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);
	ASSERT_TRUE(rsg->surfaceFriction().coefficient().has_value());
	EXPECT_NEAR(rsg->surfaceFriction().coefficient().value(), 0.55, sfMargin);
}

TEST(RunwayStateGroup, parseRwStateSurfaceFrictionNotReported) {
	const auto rsg = metaf::RunwayStateGroup::parse("R16/2901//", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->runway().number(), 16u);
	EXPECT_EQ(rsg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(rsg->type(), metaf::RunwayStateGroup::Type::RUNWAY_STATE);
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES);
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT);
	EXPECT_EQ(rsg->depositDepth().amount(), 01u);
	EXPECT_EQ(rsg->surfaceFriction().type(), metaf::SurfaceFriction::Type::NOT_REPORTED);
}

TEST(RunwayStateGroup, parseRwStateDepositValues) {
	const auto rsg0 = metaf::RunwayStateGroup::parse("R16/09//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg0.has_value());
	EXPECT_EQ(rsg0->deposits(), metaf::RunwayStateGroup::Deposits::CLEAR_AND_DRY);

	const auto rsg1 = metaf::RunwayStateGroup::parse("R16/19//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	EXPECT_EQ(rsg1->deposits(), metaf::RunwayStateGroup::Deposits::DAMP);

	const auto rsg2 = metaf::RunwayStateGroup::parse("R16/29//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	EXPECT_EQ(rsg2->deposits(), metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES);

	const auto rsg3 = metaf::RunwayStateGroup::parse("R16/39//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());
	EXPECT_EQ(rsg3->deposits(), metaf::RunwayStateGroup::Deposits::RIME_AND_FROST_COVERED);

	const auto rsg4 = metaf::RunwayStateGroup::parse("R16/49//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg4.has_value());
	EXPECT_EQ(rsg4->deposits(), metaf::RunwayStateGroup::Deposits::DRY_SNOW);

	const auto rsg5 = metaf::RunwayStateGroup::parse("R16/59//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg5.has_value());
	EXPECT_EQ(rsg5->deposits(), metaf::RunwayStateGroup::Deposits::WET_SNOW);

	const auto rsg6 = metaf::RunwayStateGroup::parse("R16/69//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg6.has_value());
	EXPECT_EQ(rsg6->deposits(), metaf::RunwayStateGroup::Deposits::SLUSH);

	const auto rsg7 = metaf::RunwayStateGroup::parse("R16/79//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg7.has_value());
	EXPECT_EQ(rsg7->deposits(), metaf::RunwayStateGroup::Deposits::ICE);

	const auto rsg8 = metaf::RunwayStateGroup::parse("R16/89//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg8.has_value());
	EXPECT_EQ(rsg8->deposits(), metaf::RunwayStateGroup::Deposits::COMPACTED_OR_ROLLED_SNOW);

	const auto rsg9 = metaf::RunwayStateGroup::parse("R16/99//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg9.has_value());
	EXPECT_EQ(rsg9->deposits(), metaf::RunwayStateGroup::Deposits::FROZEN_RUTS_OR_RIDGES);
}

TEST(RunwayStateGroup, parseRwStateDepositsNotReported) {
	const auto rsg = metaf::RunwayStateGroup::parse("R16//9//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::NOT_REPORTED);
}

TEST(RunwayStateGroup, parseRwStateExtentValues) {
	const auto rsg0 = metaf::RunwayStateGroup::parse("R16/700155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg0.has_value());
	EXPECT_EQ(rsg0->contaminationExtent(), metaf::RunwayStateGroup::Extent::NONE);

	const auto rsg1 = metaf::RunwayStateGroup::parse("R16/710155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	EXPECT_EQ(rsg1->contaminationExtent(), metaf::RunwayStateGroup::Extent::LESS_THAN_10_PERCENT);

	const auto rsg2 = metaf::RunwayStateGroup::parse("R16/720155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	EXPECT_EQ(rsg2->contaminationExtent(), metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT);

	const auto rsg3 = metaf::RunwayStateGroup::parse("R16/730155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());
	EXPECT_EQ(rsg3->contaminationExtent(), metaf::RunwayStateGroup::Extent::RESERVED_3);

	const auto rsg4 = metaf::RunwayStateGroup::parse("R16/740155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg4.has_value());
	EXPECT_EQ(rsg4->contaminationExtent(), metaf::RunwayStateGroup::Extent::RESERVED_4);

	const auto rsg5 = metaf::RunwayStateGroup::parse("R16/750155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg5.has_value());
	EXPECT_EQ(rsg5->contaminationExtent(), metaf::RunwayStateGroup::Extent::FROM_26_TO_50_PERCENT);

	const auto rsg6 = metaf::RunwayStateGroup::parse("R16/760155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg6.has_value());
	EXPECT_EQ(rsg6->contaminationExtent(), metaf::RunwayStateGroup::Extent::RESERVED_6);

	const auto rsg7 = metaf::RunwayStateGroup::parse("R16/770155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg7.has_value());
	EXPECT_EQ(rsg7->contaminationExtent(), metaf::RunwayStateGroup::Extent::RESERVED_7);

	const auto rsg8 = metaf::RunwayStateGroup::parse("R16/780155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg8.has_value());
	EXPECT_EQ(rsg8->contaminationExtent(), metaf::RunwayStateGroup::Extent::RESERVED_8);

	const auto rsg9 = metaf::RunwayStateGroup::parse("R16/790155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg9.has_value());
	EXPECT_EQ(rsg9->contaminationExtent(), metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT);
}

TEST(RunwayStateGroup, parseRwStateExtentNotReported) {
	const auto rsg = metaf::RunwayStateGroup::parse("R16/7/0155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::NOT_REPORTED);
}

TEST(RunwayStateGroup, parseRwStateAllNotReported) {
	const auto rsg = metaf::RunwayStateGroup::parse("R33///////", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->runway().number(), 33u);
	EXPECT_EQ(rsg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(rsg->type(), metaf::RunwayStateGroup::Type::RUNWAY_STATE);
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::NOT_REPORTED);
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::NOT_REPORTED);
	EXPECT_FALSE(rsg->depositDepth().isReported());
	EXPECT_EQ(rsg->surfaceFriction().type(), metaf::SurfaceFriction::Type::NOT_REPORTED);
}

TEST(RunwayStateGroup, parseRwStateWrongReportPart) {
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R16/290155", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R16/290155", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R16/290155", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R16/290155", metaf::ReportPart::RMK));
}

TEST(RunwayStateGroup, parseRwStateWrongFormat) {
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21V290155", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R1/290155", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R112/290155", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R1A/290155", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21Z/290155", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/2900155", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/29155", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/A90155", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/2A0155", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/290A55", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/29010A", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21//////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21////////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/A/////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21//A////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21////A//", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21//////A", metaf::ReportPart::METAR));
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of Rxx/CLRDxx groups
// Purpose: to confirm that the groups which identify cleared runway deposits 
// are parsed correctly and only within correct report part, and that malformed 
// groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(RunwayStateGroup, parseClrdSurfaceFrictionReported) {
	const auto rsg = metaf::RunwayStateGroup::parse("R88/CLRD70", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->runway().number(), 88u);
	EXPECT_EQ(rsg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(rsg->type(), metaf::RunwayStateGroup::Type::RUNWAY_CLRD);
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::NOT_REPORTED);
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::NOT_REPORTED);
	EXPECT_FALSE(rsg->depositDepth().isReported());
	EXPECT_EQ(rsg->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);
	ASSERT_TRUE(rsg->surfaceFriction().coefficient().has_value());
	EXPECT_NEAR(rsg->surfaceFriction().coefficient().value(), 0.70, sfMargin);
}

TEST(RunwayStateGroup, parseClrdSurfaceFrictionNotReported) {
	const auto rsg = metaf::RunwayStateGroup::parse("R99/CLRD//", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->runway().number(), 99u);
	EXPECT_EQ(rsg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(rsg->type(), metaf::RunwayStateGroup::Type::RUNWAY_CLRD);
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::NOT_REPORTED);
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::NOT_REPORTED);
	EXPECT_FALSE(rsg->depositDepth().isReported());
	EXPECT_EQ(rsg->surfaceFriction().type(), metaf::SurfaceFriction::Type::NOT_REPORTED);
}

TEST(RunwayStateGroup, parseClrdRunwayWithDesignator) {
	const auto rsg1 = metaf::RunwayStateGroup::parse("R21R/CLRD70", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	EXPECT_EQ(rsg1->runway().number(), 21u);
	EXPECT_EQ(rsg1->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rsg1->type(), metaf::RunwayStateGroup::Type::RUNWAY_CLRD);
	EXPECT_EQ(rsg1->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);

	const auto rsg2 = metaf::RunwayStateGroup::parse("R21L/CLRD70", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	EXPECT_EQ(rsg2->runway().number(), 21u);
	EXPECT_EQ(rsg2->runway().designator(), metaf::Runway::Designator::LEFT);
	EXPECT_EQ(rsg2->type(), metaf::RunwayStateGroup::Type::RUNWAY_CLRD);
	EXPECT_EQ(rsg2->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);

	const auto rsg3 = metaf::RunwayStateGroup::parse("R21C/CLRD70", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());
	EXPECT_EQ(rsg3->runway().number(), 21u);
	EXPECT_EQ(rsg3->runway().designator(), metaf::Runway::Designator::CENTER);
	EXPECT_EQ(rsg3->type(), metaf::RunwayStateGroup::Type::RUNWAY_CLRD);
	EXPECT_EQ(rsg3->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);
}

TEST(RunwayStateGroup, parseClrdWrongReportPart) {
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21L/CLRD70", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21L/CLRD70", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21L/CLRD70", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21L/CLRD70", metaf::ReportPart::RMK));
}

TEST(RunwayStateGroup, parseClrdWrongFormat) {
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21VCLRD70", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R1/CLRD70", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R112/CLRD70", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R1A/CLRD70", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21Z/CLRD70", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/CRRD70", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/CRRD", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/CRRD7", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/CRRD070", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/CRRD7A", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/CRRD/", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/CRRD///", metaf::ReportPart::METAR));
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of Rxx/D and Rxx/xxD groups
// Purpose: to confirm that the groups which identify cleared runway deposits 
// reported by aviationweather.gov are parsed correctly and only within correct 
// report part, and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(RunwayStateGroup, parseClrdAvcSurfaceFrictionReported) {
	const auto rsg = metaf::RunwayStateGroup::parse("R88/70D", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->runway().number(), 88u);
	EXPECT_EQ(rsg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(rsg->type(), metaf::RunwayStateGroup::Type::RUNWAY_CLRD);
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::NOT_REPORTED);
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::NOT_REPORTED);
	EXPECT_FALSE(rsg->depositDepth().isReported());
	EXPECT_EQ(rsg->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);
	ASSERT_TRUE(rsg->surfaceFriction().coefficient().has_value());
	EXPECT_NEAR(rsg->surfaceFriction().coefficient().value(), 0.70, sfMargin);
}

TEST(RunwayStateGroup, parseClrdAvcSurfaceFrictionMissing) {
	const auto rsg = metaf::RunwayStateGroup::parse("R13/D", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->runway().number(), 13u);
	EXPECT_EQ(rsg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(rsg->type(), metaf::RunwayStateGroup::Type::RUNWAY_CLRD);
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::NOT_REPORTED);
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::NOT_REPORTED);
	EXPECT_FALSE(rsg->depositDepth().isReported());
	EXPECT_EQ(rsg->surfaceFriction().type(), metaf::SurfaceFriction::Type::NOT_REPORTED);
}

TEST(RunwayStateGroup, parseClrdAvcRunwayWithDesignator) {
	const auto rsg1 = metaf::RunwayStateGroup::parse("R21R/70D", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	EXPECT_EQ(rsg1->runway().number(), 21u);
	EXPECT_EQ(rsg1->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rsg1->type(), metaf::RunwayStateGroup::Type::RUNWAY_CLRD);
	EXPECT_EQ(rsg1->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);

	const auto rsg2 = metaf::RunwayStateGroup::parse("R21L/70D", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	EXPECT_EQ(rsg2->runway().number(), 21u);
	EXPECT_EQ(rsg2->runway().designator(), metaf::Runway::Designator::LEFT);
	EXPECT_EQ(rsg2->type(), metaf::RunwayStateGroup::Type::RUNWAY_CLRD);
	EXPECT_EQ(rsg2->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);

	const auto rsg3 = metaf::RunwayStateGroup::parse("R21C/70D", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());
	EXPECT_EQ(rsg3->runway().number(), 21u);
	EXPECT_EQ(rsg3->runway().designator(), metaf::Runway::Designator::CENTER);
	EXPECT_EQ(rsg3->type(), metaf::RunwayStateGroup::Type::RUNWAY_CLRD);
	EXPECT_EQ(rsg3->surfaceFriction().type(), 
		metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED);
}

TEST(RunwayStateGroup, parseClrdAvcWrongReportPart) {
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21L/70D", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21L/70D", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21L/70D", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21L/70D", metaf::ReportPart::RMK));
}

TEST(RunwayStateGroup, parseClrdAvcWrongFormat) {
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21V70D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R1/70D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R112/70D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R1A/70D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21Z/70D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/7D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/070D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/7AD", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21///D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21//4D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/4/D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21//ZD", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/Z/D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21//D", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21////D", metaf::ReportPart::METAR));
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of Rxx/SNOCLO groups
// Purpose: to confirm that the groups which identify runway closed due to
// snow accumulation are parsed correctly and only within correct report part,
// and that malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(RunwayStateGroup, parseSnocloRunway) {
	const auto rsg = metaf::RunwayStateGroup::parse("R21/SNOCLO", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg.has_value());
	EXPECT_EQ(rsg->runway().number(), 21u);
	EXPECT_EQ(rsg->runway().designator(), metaf::Runway::Designator::NONE);
	EXPECT_EQ(rsg->type(), metaf::RunwayStateGroup::Type::RUNWAY_SNOCLO);
	EXPECT_EQ(rsg->deposits(), metaf::RunwayStateGroup::Deposits::NOT_REPORTED);
	EXPECT_EQ(rsg->contaminationExtent(), metaf::RunwayStateGroup::Extent::NOT_REPORTED);
	EXPECT_FALSE(rsg->depositDepth().isReported());
	EXPECT_EQ(rsg->surfaceFriction().type(), metaf::SurfaceFriction::Type::NOT_REPORTED);
}

TEST(RunwayStateGroup, parseSnocloRunwayWithDesignator) {
	const auto rsg1 = metaf::RunwayStateGroup::parse("R21R/SNOCLO", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	EXPECT_EQ(rsg1->runway().number(), 21u);
	EXPECT_EQ(rsg1->runway().designator(), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(rsg1->type(), metaf::RunwayStateGroup::Type::RUNWAY_SNOCLO);

	const auto rsg2 = metaf::RunwayStateGroup::parse("R21L/SNOCLO", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	EXPECT_EQ(rsg2->runway().number(), 21u);
	EXPECT_EQ(rsg2->runway().designator(), metaf::Runway::Designator::LEFT);
	EXPECT_EQ(rsg2->type(), metaf::RunwayStateGroup::Type::RUNWAY_SNOCLO);

	const auto rsg3 = metaf::RunwayStateGroup::parse("R21C/SNOCLO", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());
	EXPECT_EQ(rsg3->runway().number(), 21u);
	EXPECT_EQ(rsg3->runway().designator(), metaf::Runway::Designator::CENTER);
	EXPECT_EQ(rsg3->type(), metaf::RunwayStateGroup::Type::RUNWAY_SNOCLO);
}

TEST(RunwayStateGroup, parseSnocloRunwayWrongReportPart) {
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/SNOCLO", metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/SNOCLO", metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/SNOCLO", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/SNOCLO", metaf::ReportPart::RMK));
}

TEST(RunwayStateGroup, parseSnocloRunwayWrongFormat) {
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21VSNOCLO", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R1/SNOCLO", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R112/SNOCLO", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R1A/SNOCLO", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21Z/SNOCLO", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/SNOCLOO", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R21/SNOCL", metaf::ReportPart::METAR));
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of SNOCLO and R/SNOCLO
// Purpose: to confirm that the groups which identify aerodrome closed due to
// snow accumulation are parsed correctly and only within correct report part
///////////////////////////////////////////////////////////////////////////////

TEST(RunwayStateGroup, parseSnocloAerodrome) {
	auto rsg1 = metaf::RunwayStateGroup::parse("SNOCLO", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	EXPECT_EQ(rsg1->type(), metaf::RunwayStateGroup::Type::AERODROME_SNOCLO);
	EXPECT_TRUE(rsg1->runway().isAllRunways());

	auto rsg2 = metaf::RunwayStateGroup::parse("R/SNOCLO", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	EXPECT_EQ(rsg2->type(), metaf::RunwayStateGroup::Type::AERODROME_SNOCLO);
	EXPECT_TRUE(rsg2->runway().isAllRunways());
}

TEST(RunwayStateGroup, parseSnocloAerodromeWrongReportPart) {
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("SNOCLO", 
		metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("SNOCLO", 
		metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("SNOCLO", 
		metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("SNOCLO", 
		metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R_SNOCLO", 
		metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R_SNOCLO", 
		metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R_SNOCLO", 
		metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::RunwayStateGroup::parse("R_SNOCLO", 
		metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(RunwayStateGroup, isValidRunwayTrue) {
	const auto rsg1 = metaf::RunwayStateGroup::parse("R88/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	ASSERT_TRUE(rsg1->runway().isValid());
	EXPECT_TRUE(rsg1->isValid());

	const auto rsg2 = metaf::RunwayStateGroup::parse("R99/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	ASSERT_TRUE(rsg2->runway().isValid());
	EXPECT_TRUE(rsg2->isValid());

	const auto rsg3 = metaf::RunwayStateGroup::parse("R36/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());
	ASSERT_TRUE(rsg3->runway().isValid());
	EXPECT_TRUE(rsg3->isValid());

	const auto rsg4 = metaf::RunwayStateGroup::parse("R06L/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg4.has_value());
	ASSERT_TRUE(rsg4->runway().isValid());
	EXPECT_TRUE(rsg4->isValid());
}

TEST(RunwayStateGroup, isValidRunwayFalse) {
	const auto rsg1 = metaf::RunwayStateGroup::parse("R88R/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	ASSERT_FALSE(rsg1->runway().isValid());
	EXPECT_FALSE(rsg1->isValid());

	const auto rsg2 = metaf::RunwayStateGroup::parse("R99C/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	ASSERT_FALSE(rsg2->runway().isValid());
	EXPECT_FALSE(rsg2->isValid());

	const auto rsg3 = metaf::RunwayStateGroup::parse("R37/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());
	ASSERT_FALSE(rsg3->runway().isValid());
	EXPECT_FALSE(rsg3->isValid());
}

TEST(RunwayStateGroup, isValidExtentTrue) {
	const auto rsg1 = metaf::RunwayStateGroup::parse("R88/00//55", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	ASSERT_TRUE(rsg1->runway().isValid());
	EXPECT_TRUE(rsg1->isValid());

	const auto rsg2 = metaf::RunwayStateGroup::parse("R88/210155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	ASSERT_TRUE(rsg2->runway().isValid());
	EXPECT_TRUE(rsg2->isValid());

	const auto rsg3 = metaf::RunwayStateGroup::parse("R88/220155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());
	ASSERT_TRUE(rsg3->runway().isValid());
	EXPECT_TRUE(rsg3->isValid());

	const auto rsg4 = metaf::RunwayStateGroup::parse("R88/250155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg4.has_value());
	ASSERT_TRUE(rsg4->runway().isValid());
	EXPECT_TRUE(rsg4->isValid());

	const auto rsg5 = metaf::RunwayStateGroup::parse("R88/290155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg5.has_value());
	ASSERT_TRUE(rsg5->runway().isValid());
	EXPECT_TRUE(rsg5->isValid());

	const auto rsg6 = metaf::RunwayStateGroup::parse("R88/2/0155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg6.has_value());
	ASSERT_TRUE(rsg6->runway().isValid());
	EXPECT_TRUE(rsg6->isValid());
}

TEST(RunwayStateGroup, isValidExtentFalse) {
	const auto rsg1 = metaf::RunwayStateGroup::parse("R88/230155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());
	ASSERT_TRUE(rsg1->runway().isValid());
	EXPECT_FALSE(rsg1->isValid());

	const auto rsg2 = metaf::RunwayStateGroup::parse("R88/240155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());
	ASSERT_TRUE(rsg2->runway().isValid());
	EXPECT_FALSE(rsg2->isValid());

	const auto rsg3 = metaf::RunwayStateGroup::parse("R88/260155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());
	ASSERT_TRUE(rsg3->runway().isValid());
	EXPECT_FALSE(rsg3->isValid());

	const auto rsg4 = metaf::RunwayStateGroup::parse("R88/270155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg4.has_value());
	ASSERT_TRUE(rsg4->runway().isValid());
	EXPECT_FALSE(rsg4->isValid());

	const auto rsg5 = metaf::RunwayStateGroup::parse("R88/280155", metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg5.has_value());
	ASSERT_TRUE(rsg5->runway().isValid());
	EXPECT_FALSE(rsg5->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Appending tests
// Purpose: to confirm that no group can append to RunwayStateGroup
///////////////////////////////////////////////////////////////////////////////

TEST(RunwayStateGroup, append) {
	const std::string rsgStr1("R16/710155");
	const std::string rsgStr2("R21R/CLRD70");
	const std::string rsgStr3("R21/SNOCLO");
	const std::string rsgStr4("R21///////");
	const std::string tStr1("RMK");
	const std::string tStr2("TEST");

	auto rsg1 = metaf::RunwayStateGroup::parse(rsgStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg1.has_value());

	auto rsg2 = metaf::RunwayStateGroup::parse(rsgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg2.has_value());

	auto rsg3 = metaf::RunwayStateGroup::parse(rsgStr3, metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg3.has_value());

	auto rsg4 = metaf::RunwayStateGroup::parse(rsgStr4, metaf::ReportPart::METAR);
	ASSERT_TRUE(rsg4.has_value());

	EXPECT_EQ(rsg1->append(rsgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg1->append(rsgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg1->append(rsgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg1->append(rsgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg1->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg1->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(rsg2->append(rsgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg2->append(rsgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg2->append(rsgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg2->append(rsgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg2->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg2->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(rsg3->append(rsgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg3->append(rsgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg3->append(rsgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg3->append(rsgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg3->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg3->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(rsg4->append(rsgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg4->append(rsgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg4->append(rsgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg4->append(rsgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg4->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rsg4->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
}
