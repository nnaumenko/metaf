/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

//Confirm that passing FixedGroup of Type::METAR to getSyntaxGroup() results 
//in SyntaxGroup::METAR
TEST(getSyntaxGroup, METAR) {
	const metaf::Group 
		group = metaf::FixedGroup(metaf::FixedGroup::Type::METAR);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::METAR);
}

//Confirm that passing FixedGroup of Type::SPECI to getSyntaxGroup() results 
//in SyntaxGroup::SPECI
TEST(getSyntaxGroup, SPECI) {
	const metaf::Group 
		group = metaf::FixedGroup(metaf::FixedGroup::Type::SPECI);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::SPECI);
}
//Confirm that passing FixedGroup of Type::TAF to getSyntaxGroup()
//results in SyntaxGroup::TAF
TEST(getSyntaxGroup, TAF) {
	const metaf::Group 
		group = metaf::FixedGroup(metaf::FixedGroup::Type::TAF);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::TAF);
}

//Confirm that passing FixedGroup of Type::COR to getSyntaxGroup()
//results in SyntaxGroup::COR
TEST(getSyntaxGroup, COR) {
	const metaf::Group group = 
		metaf::FixedGroup(metaf::FixedGroup::Type::COR);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::COR);
}

//Confirm that passing FixedGroup of Type::AMD to getSyntaxGroup()
//results in SyntaxGroup::AMD
TEST(getSyntaxGroup, AMD) {
	const metaf::Group group = 
		metaf::FixedGroup(metaf::FixedGroup::Type::AMD);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::AMD);
}

//Confirm that passing FixedGroup of Type::NIL to getSyntaxGroup()
//results in SyntaxGroup::NIL
TEST(getSyntaxGroup, NIL) {
	const metaf::Group group = 
		metaf::FixedGroup(metaf::FixedGroup::Type::NIL);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::NIL);
}

//Confirm that passing FixedGroup of Type::CNL to getSyntaxGroup()
//results in SyntaxGroup::CNL
TEST(getSyntaxGroup, CNL) {
	const metaf::Group group = 
		metaf::FixedGroup(metaf::FixedGroup::Type::CNL);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::CNL);
}

//Confirm that passing FixedGroup of Type::RMK to getSyntaxGroup()
//results in SyntaxGroup::RMK
TEST(getSyntaxGroup, RMK) {
	const metaf::Group group = 
		metaf::FixedGroup(metaf::FixedGroup::Type::RMK);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::RMK);
}

//Confirm that passing LocationGroup to getSyntaxGroup() results in 
//SyntaxGroup::LOCATION
TEST(getSyntaxGroup, LOCATION) {
	const metaf::Group group = metaf::LocationGroup("ABCD");
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::LOCATION);
}

//Confirm that passing ReportTimeGroup to getSyntaxGroup() results in 
//SyntaxGroup::REPORT_TIME
TEST(getSyntaxGroup, REPORT_TIME) {
	const metaf::Group group = metaf::ReportTimeGroup(metaf::MetafTime(8, 12, 30));
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::REPORT_TIME);
}

//Confirm that passing TimeSpanGroup to getSyntaxGroup() results in 
//SyntaxGroup::TIME_SPAN
TEST(getSyntaxGroup, TIME_SPAN) {
	const metaf::Group group = metaf::TimeSpanGroup(
		metaf::MetafTime(8, 12), 
		metaf::MetafTime(8, 24));
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::TIME_SPAN);
}

//Confirm that passing FixedGroup of the type AUTO to getSyntaxGroup() results 
//in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_AUTO) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::AUTO);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing FixedGroup of the type CLR to getSyntaxGroup() results 
//in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_CLR) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::CLR);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing FixedGroup of the type NCD to getSyntaxGroup() results 
//in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_NCD) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::NCD);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing FixedGroup of the type NOSIG to getSyntaxGroup() results 
//in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_NOSIG) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing FixedGroup of the type AIRPORT_SNOCLO to getSyntaxGroup() 
//results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_AIRPORT_SNOCLO) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::AIRPORT_SNOCLO);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing FixedGroup of the type CAVOK to getSyntaxGroup() results 
//in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_CAVOK) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::CAVOK);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing FixedGroup of the type NSC to getSyntaxGroup() results 
//in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_NSC) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::NSC);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing FixedGroup of the type NSW to getSyntaxGroup() results 
//in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_NSW) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::NSW);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing FixedGroup of the type TEMPO to getSyntaxGroup() results 
//in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_TEMPO) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::TEMPO);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing FixedGroup of the type BECMG to getSyntaxGroup() results 
//in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_BECMG) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::BECMG);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing FixedGroup of the type INTER to getSyntaxGroup() results 
//in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_INTER) {
		const metaf::Group group = 
			metaf::FixedGroup(metaf::FixedGroup::Type::INTER);
		EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
	}

//Confirm that passing PlainTextGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_PlainTextGroup) {
	const metaf::Group group = metaf::PlainTextGroup("A1B2C3D4");
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing TrendTimeGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_TrendTimeGroup) {
	const metaf::Group group = 
		metaf::TrendTimeGroup(metaf::TrendTimeGroup::Type::FROM, metaf::MetafTime(11, 14, 00));
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing ProbabilityGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_ProbabilityGroup) {
	const metaf::Group group = metaf::ProbabilityGroup(30);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing WindGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_WindGroup) {
	const metaf::Group group = 
		metaf::WindGroup(240, 
			metaf::Speed(2, metaf::Speed::Unit::KNOTS), 
			metaf::Speed(7, metaf::Speed::Unit::KNOTS));
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing VarWindGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_VarWindGroup) {
	const metaf::Group group = metaf::VarWindGroup(180, 240);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing WindShearGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_WindShearGroup) {
	const metaf::Group group = 
		metaf::WindShearGroup(2200, 30, metaf::Speed(45, metaf::Speed::Unit::KNOTS));
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing VisibilityGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_VisibilityGroup) {
	const metaf::Group group = 
		metaf::VisibilityGroup::makeVisibilityMeters(6500);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing CloudGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_CloudGroup) {
	const metaf::Group group = 
		metaf::CloudGroup(metaf::CloudGroup::Amount::BROKEN, 2400);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing VerticalVisibilityGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_VerticalVisibilityGroup) {
	const metaf::Group group = metaf::VerticalVisibilityGroup(1500);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing WeatherGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_WeatherGroup) {
	const metaf::Group group = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SHOWERS, 
			metaf::WeatherGroup::Weather::SNOW);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing TemperatureGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_TemperatureGroup) {
	const metaf::Group group = metaf::TemperatureGroup(7, 2);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing MinMaxTemperatureGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_MinMaxTemperatureGroup) {
	const metaf::Group group = metaf::MinMaxTemperatureGroup::makeMax(
		metaf::Temperature(7), 
		metaf::MetafTime(15, 45));
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing PressureGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_PressureGroup) {
	const metaf::Group group = 
		metaf::PressureGroup(1033, metaf::PressureGroup::Unit::HECTOPASCAL);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing RunwayVisualRangeGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_RunwayVisualRangeGroup) {
	const metaf::Group group = 
		metaf::RunwayVisualRangeGroup(metaf::Runway(5), 
			1500, 
			metaf::DistanceUnit::METERS);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing RunwayStateGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_RunwayStateGroup) {
	const metaf::Group group = 
		metaf::RunwayStateGroup(metaf::Runway(16, metaf::Runway::Designator::RIGHT),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES, 
			metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT,
			metaf::RunwayStateGroup::DepositDepth(1),
			metaf::RunwayStateGroup::SurfaceFriction(0.55));
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing RainfallGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_RainfallGroup) {
	const metaf::Group group = metaf::RainfallGroup(0.0, 0.0);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing SeaSurfaceGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_SeaSurfaceGroup) {
	const metaf::Group group = 
		metaf::SeaSurfaceGroup(15, metaf::SeaSurfaceGroup::StateOfSurface::MODERATE);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing SeaWavesGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_SeaWavesGroup) {
	const metaf::Group group = metaf::SeaWavesGroup(9, 15);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}

//Confirm that passing ColourCodeGroup to getSyntaxGroup() results in SyntaxGroup::OTHER
TEST(getSyntaxGroup, OTHER_ColourCodeGroup) {
	const metaf::Group group = metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::AMBER);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::OTHER);
}