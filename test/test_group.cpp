/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "test.h"
#include "test_group.h"
#include "metaf.h"
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////

//Confirm that Group occupies a contiguous memory area
TEST(Group, is_standard_layout) {
    EXPECT_TRUE(std::is_standard_layout<metaf::Group>::value);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default Runway structs are equal
TEST(Runway, defaultEqual) {
	const metaf::Runway r1;
	const metaf::Runway r2;
	EXPECT_TRUE(r1 == r2);
	EXPECT_FALSE(r1 != r2);
}

TEST(Runway, sameDataEqual) {
	//Confirm that Runway structs initialised with the same data are equal
	metaf::Runway r1;
	r1.number = 21;
	r1.designator = metaf::Runway::Designator::LEFT;
	metaf::Runway r2;
	r2.number = 21;
	r2.designator = metaf::Runway::Designator::LEFT;
	EXPECT_EQ(r1, r2);
}

//Confirm that Runway structs initialised with the different data are not equal
TEST(Runway, differentDataNotEqual) {
	metaf::Runway r;
	r.number = 21;
	r.designator = metaf::Runway::Designator::LEFT;
	metaf::Runway r1(r);
	r1.number = 22;
	metaf::Runway r2(r);
	r2.designator = metaf::Runway::Designator::NONE;
	EXPECT_NE(r, r1);
	EXPECT_NE(r, r2);
}

//Confirm that constructor initialises the Runway struct with correct data
TEST(Runway, constructor) {
	const metaf::Runway r(25, metaf::Runway::Designator::NONE);
	metaf::Runway r1;
	r1.number = 25;
	r1.designator = metaf::Runway::Designator::NONE;
	EXPECT_EQ(r, r1);
}

//Confirm that designatorFromChar correctly decodes a runway designator
TEST(Runway, designatorFromChar) {
	EXPECT_EQ(metaf::Runway::designatorFromChar('R'), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(metaf::Runway::designatorFromChar('C'), metaf::Runway::Designator::CENTER);
	EXPECT_EQ(metaf::Runway::designatorFromChar('L'), metaf::Runway::Designator::LEFT);
	EXPECT_EQ(metaf::Runway::designatorFromChar(' '), metaf::Runway::Designator::NONE);
	EXPECT_EQ(metaf::Runway::designatorFromChar('\0'), metaf::Runway::Designator::NONE);
	EXPECT_EQ(metaf::Runway::designatorFromChar('M'), metaf::Runway::Designator::UNKNOWN);
	EXPECT_EQ(metaf::Runway::designatorFromChar('5'), metaf::Runway::Designator::UNKNOWN);
}

//Confirm that designatorFromString correctly decodes a runway designator
TEST(Runway, designatorFromString) {
	EXPECT_EQ(metaf::Runway::designatorFromString("R"), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(metaf::Runway::designatorFromString("C"),	metaf::Runway::Designator::CENTER);
	EXPECT_EQ(metaf::Runway::designatorFromString("L"),	metaf::Runway::Designator::LEFT);
	EXPECT_EQ(metaf::Runway::designatorFromString(" "),	metaf::Runway::Designator::NONE);
	EXPECT_EQ(metaf::Runway::designatorFromString("\0"), metaf::Runway::Designator::NONE);
	EXPECT_EQ(metaf::Runway::designatorFromString(""), metaf::Runway::Designator::NONE);
	EXPECT_EQ(metaf::Runway::designatorFromString("X"), metaf::Runway::Designator::UNKNOWN);
	EXPECT_EQ(metaf::Runway::designatorFromString("RR"), metaf::Runway::Designator::UNKNOWN);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(PlainTextGroup, defaultEqual) {
	const metaf::PlainTextGroup group1;
	const metaf::PlainTextGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups initialised with the same data are equal
TEST(PlainTextGroup, sameDataEqual) {
	static const char testStr[] = "A1B2C3D4";
	metaf::PlainTextGroup group1;
	metaf::PlainTextGroup group2;
	strcpy(group1.text, testStr);
	strcpy(group2.text, testStr);
	EXPECT_EQ(group1, group2);
}

//Confirm that groups initialised with different data are not equal
TEST(PlainTextGroup, differentDataNotEqual) {
	static const char testStr[] = "A1B2C3D4";
	static const char testStr1[] = "A9B0";
	metaf::PlainTextGroup group;
	metaf::PlainTextGroup group1;
	strcpy(group.text, testStr);
	strcpy(group1.text, testStr1);
	EXPECT_NE(group, group1);
}

//Confirm that constructor initialises the group with correct string
TEST(PlainTextGroup, constructor) {
	static const char testStr[] = "A1B2C3D4";
	const metaf::PlainTextGroup group(testStr);
	metaf::PlainTextGroup group1;
	strcpy(group1.text, testStr);
	EXPECT_EQ(group, group1);
}

//Confirm that default groups are equal
TEST(FixedGroup, defaultEqual) {
	const metaf::FixedGroup group1;
	const metaf::FixedGroup group2;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the same type are equal 
TEST(FixedGroup, sameDataEqual) {
	static const auto type = metaf::FixedGroup::Type::SPECI;
	metaf::FixedGroup group1;
	metaf::FixedGroup group2;
	group1.type = type;
	group2.type = type;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different type are not equal 
TEST(FixedGroup, differentDataNotEqual) {
	metaf::FixedGroup group;
	group.type = metaf::FixedGroup::Type::METAR;
	metaf::FixedGroup group1(group);
	group1.type = metaf::FixedGroup::Type::SPECI;
	EXPECT_NE(group, group1);
}

//Confirm that constructor initialises the group with correct type
TEST(FixedGroup, constructor) {
	static const auto type = metaf::FixedGroup::Type::METAR;
	const metaf::FixedGroup group(type);
	metaf::FixedGroup group1;
	group1.type = type;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(LocationGroup, defaultEqual) {
	const metaf::LocationGroup group1;
	const metaf::LocationGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups initialised with the same data are equal 
TEST(LocationGroup, sameDataEqual) {
	static const char testLocation[] = "ICAO";
	metaf::LocationGroup group1;
	metaf::LocationGroup group2;
	strcpy(group1.location, testLocation);
	strcpy(group2.location, testLocation);
	EXPECT_EQ(group1, group2);
}

//Confirm that groups initialised with different data are not equal
TEST(LocationGroup, differentDataNotEqual) {
	static const char testLocation1[] = "ICAO";
	static const char testLocation2[] = "ICA0";
	metaf::LocationGroup group1;
	metaf::LocationGroup group2;
	strcpy(group1.location, testLocation1);
	strcpy(group2.location, testLocation2);
	EXPECT_NE(group1, group2);
}


//Confirm that constructor initialises the group with correct string
TEST(LocationGroup, constructor) {
	static const char testLocation[] = "ICAO";
	const metaf::LocationGroup group(testLocation);
	metaf::LocationGroup group1;
	strcpy(group1.location, testLocation);
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(ReportTimeGroup, defaultEqual) {
	const metaf::ReportTimeGroup group1;
	const metaf::ReportTimeGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups with the same day/time are equal
TEST(ReportTimeGroup, sameDataEqual) {
	static const auto day = 29;
	static const auto hour = 23;
	static const auto minute = 55;
	metaf::ReportTimeGroup group1;
	metaf::ReportTimeGroup group2;
	group1.day = day;
	group1.hour = hour;
	group1.minute = minute;
	group2.day = day;
	group2.hour = hour;
	group2.minute = minute;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups with different day/time are not equal
TEST(ReportTimeGroup, differentDataNotEqual) {
	metaf::ReportTimeGroup group;
	group.day = 29;
	group.hour = 23;
	group.minute = 55;
	metaf::ReportTimeGroup group1(group);
	group1.day = 28;
	metaf::ReportTimeGroup group2(group);
	group2.hour = 12;
	metaf::ReportTimeGroup group3(group);
	group3.minute = 0;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
}

//Confirm that constructor initialises the group with correct data
TEST(ReportTimeGroup, constructor) {
	static const auto day = 29;
	static const auto hour = 23;
	static const auto minute = 55;
	const metaf::ReportTimeGroup group(day, hour, minute);
	metaf::ReportTimeGroup group1;
	group1.day = day;
	group1.hour = hour;
	group1.minute = minute;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(TimeSpanGroup, defaultDataEqual) {
	const metaf::TimeSpanGroup group1;
	const metaf::TimeSpanGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups with the same time are equal
TEST(TimeSpanGroup, sameDataEqual) {
	static const auto dayFrom = 29;
	static const auto hourFrom = 23;
	static const auto dayTill = 30;
	static const auto hourTill = 02;
	metaf::TimeSpanGroup group1;
	metaf::TimeSpanGroup group2;
	group1.dayFrom = dayFrom;
	group1.hourFrom = hourFrom;
	group1.dayTill = dayTill;
	group1.hourTill = hourTill;
	group2.dayFrom = dayFrom;
	group2.hourFrom = hourFrom;
	group2.dayTill = dayTill;
	group2.hourTill = hourTill;
	EXPECT_EQ(group1, group2);
}

TEST(TimeSpanGroup, differentDataNotEqual) {
	//Confirm that groups with different time are not equal
	metaf::TimeSpanGroup group;
	group.dayFrom = 29;
	group.hourFrom = 23;
	group.dayTill = 30;
	group.hourTill = 02;
	metaf::TimeSpanGroup group1(group);
	group1.dayFrom = 28;
	metaf::TimeSpanGroup group2(group);
	group2.hourFrom = 24;
	metaf::TimeSpanGroup group3(group);
	group3.dayTill = 31;
	metaf::TimeSpanGroup group4(group);
	group4.hourTill = 03;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
}

TEST(TimeSpanGroup, constructor) {
	//Confirm that constructor initialises the group with correct data
	static const auto dayFrom = 29;
	static const auto hourFrom = 23;
	static const auto dayTill = 30;
	static const auto hourTill = 02;
	const metaf::TimeSpanGroup group(dayFrom, hourFrom, dayTill, hourTill);
	metaf::TimeSpanGroup group1;
	group1.dayFrom = dayFrom;
	group1.hourFrom = hourFrom;
	group1.dayTill = dayTill;
	group1.hourTill = hourTill;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(TrendTimeGroup, defaultDataEqual) {
	const metaf::TrendTimeGroup group1;
	const metaf::TrendTimeGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that the groups without day reported and with the same time are equal
TEST(TrendTimeGroup, sameDataEqual) {
	static const auto type = metaf::TrendTimeGroup::Type::FROM;
	static const auto hour = 17;
	static const auto minute = 30;
	metaf::TrendTimeGroup group1;
	metaf::TrendTimeGroup group2;
	group1.type = type;
	group1.hour = hour;
	group1.minute = minute;
	group2.type = type;
	group2.hour = hour;
	group2.minute = minute;
	EXPECT_EQ(group1, group2);
}

//Confirm that the groups with the day reported and same day/time are equal
TEST(TrendTimeGroup, comparisonDayReported) {
	static const auto type = metaf::TrendTimeGroup::Type::FROM;
	static const auto day = 28;
	static const auto hour = 17;
	static const auto minute = 30;
	static const auto dayReported = true;
	metaf::TrendTimeGroup group1;
	metaf::TrendTimeGroup group2;
	group1.type = type;
	group1.dayReported = dayReported;
	group1.day = day;
	group1.hour = hour;
	group1.minute = minute;
	group2.type = type;
	group2.dayReported = dayReported;
	group2.day = day;
	group2.hour = hour;
	group2.minute = minute;
	EXPECT_EQ(group1, group2);
}

//Confirm that if day is not reported, its value does not affect comparison
TEST(TrendTimeGroup, comparisonDayNotReported) {
	static const auto type = metaf::TrendTimeGroup::Type::FROM;
	static const auto hour = 17;
	static const auto minute = 30;
	static const auto dayReported = false;
	metaf::TrendTimeGroup group1;
	metaf::TrendTimeGroup group2;
	group1.type = type;
	group1.dayReported = dayReported;
	group1.day = 28;
	group1.hour = hour;
	group1.minute = minute;
	group2.type = metaf::TrendTimeGroup::Type::FROM;
	group2.dayReported = dayReported;
	group2.day = 29;
	group2.hour = hour;
	group2.minute = minute;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding different data are not equal
TEST(TrendTimeGroup, differentDataNotEqual) {
	metaf::TrendTimeGroup group;
	group.type = metaf::TrendTimeGroup::Type::FROM;
	group.dayReported = true;
	group.day = 28;
	group.hour = 17;
	group.minute = 30;
	metaf::TrendTimeGroup group1(group);
	group1.type = metaf::TrendTimeGroup::Type::AT;
	metaf::TrendTimeGroup group2(group);
	group2.dayReported = false;
	metaf::TrendTimeGroup group3(group);
	group3.day = 29;
	metaf::TrendTimeGroup group4(group);
	group4.hour = 18;
	metaf::TrendTimeGroup group5(group);
	group5.minute = 25;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
	EXPECT_NE(group, group5);
}

//Confirm that long trend time group constructor initialises the group with 
//correct data
TEST(TrendTimeGroup, constructorLongGroup) {
	static const auto type = metaf::TrendTimeGroup::Type::FROM;
	static const auto day = 28;
	static const auto hour = 17;
	static const auto minute = 30;
	const metaf::TrendTimeGroup group(type, day, hour, minute);
	metaf::TrendTimeGroup group1;
	group1.type = type;
	group1.dayReported = true;
	group1.day = day;
	group1.hour = hour;
	group1.minute = minute;
	EXPECT_EQ(group, group1);
}

//Confirm that short trend time group constructor initialises the group with 
//correct data
TEST(TrendTimeGroup, constructorShortGroup) {
	static const auto type = metaf::TrendTimeGroup::Type::FROM;
	static const auto hour = 17;
	static const auto minute = 30;
	const metaf::TrendTimeGroup group(type, hour, minute);
	metaf::TrendTimeGroup group1;
	group1.type = type;
	group1.hour = hour;
	group1.minute = minute;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(ProbabilityGroup, defaultDataEqual) {
	const metaf::ProbabilityGroup group1;
	const metaf::ProbabilityGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal 
TEST(ProbabilityGroup, sameDataEqual) {
	static const auto probability = 30;
	metaf::ProbabilityGroup group1;
	metaf::ProbabilityGroup group2;
	group1.probability = probability;
	group2.probability = probability;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different value are not equal 
TEST(ProbabilityGroup, differentDataNotEqual) {
	metaf::ProbabilityGroup group1;
	metaf::ProbabilityGroup group2;
	group1.probability = 30;
	group2.probability = 40;
	EXPECT_NE(group1, group2);
}

//Confirm that constructor initialises the group with correct data
TEST(ProbabilityGroup, constructor) {
	static const auto probability = 30;
	const metaf::ProbabilityGroup group(probability);
	metaf::ProbabilityGroup group1;
	group1.probability = probability;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(WindGroup, defaultDataEqual) {
	const metaf::WindGroup group1;
	const metaf::WindGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same data (direction, speed, gust speed, 
//unit) are equal
TEST(WindGroup, sameDataEqual) {
	static const auto direction = 120;
	static const auto directionReported = true;
	static const auto speed = 12;
	static const auto speedReported = true;
	static const auto gustSpeed = 20;
	static const auto gustSpeedReported = true;
	static const auto unit = metaf::SpeedUnit::METERS_PER_SECOND;
	metaf::WindGroup group1;
	metaf::WindGroup group2;
	group1.direction = direction;
	group1.directionReported = directionReported;
	group1.speed = speed;
	group1.speedReported = speedReported;
	group1.gustSpeed = gustSpeed;
	group1.gustSpeedReported = gustSpeedReported;
	group1.unit = unit;
	group2.direction = direction;
	group2.directionReported = directionReported;
	group2.speed = speed;
	group2.speedReported = speedReported;
	group2.gustSpeed = gustSpeed;
	group2.gustSpeedReported = gustSpeedReported;
	group2.unit = unit;
	EXPECT_EQ(group1, group2);
}

//Confirm that if the wind is variable, direction does not affect the 
//comparison
TEST(WindGroup, comparisonVariableWind) {
	metaf::WindGroup group;
	group.direction = 0;
	group.directionReported = true;
	group.directionVariable = true;
	group.speed = 12;
	group.speedReported = true;
	group.gustSpeed = 20;
	group.gustSpeedReported = true;
	group.unit = metaf::SpeedUnit::METERS_PER_SECOND;
	metaf::WindGroup group1(group);
	group1.direction = 180;
	EXPECT_EQ(group, group1);
}

//Confirm that if the direction is not reported, its value does not affect the 
//comparison
TEST(WindGroup, comparisonDirectionNotReported) {
	metaf::WindGroup group;
	group.direction = 0;
	group.directionReported = false;
	group.speed = 12;
	group.speedReported = true;
	group.gustSpeed = 20;
	group.gustSpeedReported = true;
	group.unit = metaf::SpeedUnit::METERS_PER_SECOND;
	metaf::WindGroup group1(group);
	group1.direction = 180;
	EXPECT_EQ(group, group1);
}

//Confirm that if wind speed is not reported, its value does not affect the 
//comparison
TEST(WindGroup, comparisonWindSpeedNotReported) {
	metaf::WindGroup group;
	group.direction = 120;
	group.directionReported = true;
	group.speed = 12;
	group.speedReported = false;
	group.gustSpeed = 20;
	group.gustSpeedReported = true;
	group.unit = metaf::SpeedUnit::METERS_PER_SECOND;
	metaf::WindGroup group1(group);
	group1.speed = 20;
	EXPECT_EQ(group, group1);
}

//Confirm that if gust speed is not reported, its value does not affect the 
//comparison
TEST(WindGroup, comparisonGustSpeedNotReported) {
	metaf::WindGroup group;
	group.direction = 120;
	group.directionReported = true;
	group.speed = 12;
	group.speedReported = true;
	group.gustSpeed = 20;
	group.gustSpeedReported = false;
	group.unit = metaf::SpeedUnit::METERS_PER_SECOND;
	metaf::WindGroup group1(group);
	group1.gustSpeed = 30;
	EXPECT_EQ(group, group1);
}

//Confirm that groups holding the different data (direction, speed, gust 
//speed, unit) are not equal
TEST(WindGroup, differentDataNotEqual) {
	metaf::WindGroup group;
	group.direction = 120;
	group.directionReported = true;
	group.directionVariable = false;
	group.speed = 12;
	group.speedReported = true;
	group.gustSpeed = 20;
	group.gustSpeedReported = true;
	group.unit = metaf::SpeedUnit::METERS_PER_SECOND;
	metaf::WindGroup group1(group);
	group1.direction = 240;
	metaf::WindGroup group2(group);
	group2.directionReported = false;
	metaf::WindGroup group3(group);
	group3.directionVariable = true;
	metaf::WindGroup group4(group);
	group4.speed = 10;
	metaf::WindGroup group5(group);
	group5.speedReported = false;
	metaf::WindGroup group6(group);
	group6.gustSpeed = 25;
	metaf::WindGroup group7(group);
	group7.gustSpeedReported = false;
	metaf::WindGroup group8(group);
	group8.unit = metaf::SpeedUnit::KNOTS;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
	EXPECT_NE(group, group5);
	EXPECT_NE(group, group6);
	EXPECT_NE(group, group7);
	EXPECT_NE(group, group8);
}

//Confirm that 'not reported' constructor initialises the group with correct 
//data
TEST(WindGroup, constructorNotReported) {
	static const auto unit = metaf::SpeedUnit::KNOTS;
	const metaf::WindGroup group(unit);
	metaf::WindGroup group1;
	group1.unit = unit;
	EXPECT_EQ(group, group1);
}

//Confirm that 'reported' constructor initialises the group with correct data
TEST(WindGroup, constructorReported) {
	static const auto direction = 240;
	static const auto unit = metaf::SpeedUnit::KNOTS;
	static const auto speed = 5;
	static const auto gustSpeed = 12;
	const metaf::WindGroup group(direction, unit, speed, gustSpeed);
	metaf::WindGroup group1;
	group1.unit = unit;
	group1.direction = direction;
	group1.speed = speed;
	group1.gustSpeed = gustSpeed;
	group1.directionReported = true;
	group1.speedReported = true;
	group1.gustSpeedReported = true;
	EXPECT_EQ(group, group1);
}

//Confirm that 'makeVariableDirection' named constructor initialises the group 
//with correct data
TEST(WindGroup, makeVariableDirection) {
	static const auto unit = metaf::SpeedUnit::KNOTS;
	static const auto speed = 5;
	static const auto gustSpeed = 12;
	const metaf::WindGroup group = 
		metaf::WindGroup::makeVariableDirection(unit, speed, gustSpeed);
	metaf::WindGroup group1;
	group1.unit = metaf::SpeedUnit::KNOTS;
	group1.speed = speed;
	group1.gustSpeed = gustSpeed;
	group1.directionReported = true;
	group1.directionVariable = true;
	group1.speedReported = true;
	group1.gustSpeedReported = true;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(VarWindGroup, defaultDataEqual) {
	const metaf::VarWindGroup group1;
	const metaf::VarWindGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal 
TEST(VarWindGroup, sameDataEqual) {
	static const auto directionFrom = 60;
	static const auto directionTo = 120;
	metaf::VarWindGroup group1;
	metaf::VarWindGroup group2;
	group1.directionFrom = directionFrom;
	group1.directionTo = directionTo;
	group2.directionFrom = directionFrom;
	group2.directionTo = directionTo;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different values are not equal 
TEST(VarWindGroup, differentDataNotEqual) {
	metaf::VarWindGroup group;
	group.directionFrom = 60;
	group.directionTo = 120;
	metaf::VarWindGroup group1(group);
	group1.directionFrom = 80;
	metaf::VarWindGroup group2(group);
	group2.directionTo = 180;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
}

//Confirm that constructor initialises the group with correct data
TEST(VarWindGroup, constructor) {
	static const auto directionFrom = 120;
	static const auto directionTo = 150;
	const metaf::VarWindGroup group(directionFrom, directionTo);
	metaf::VarWindGroup group1;
	group1.directionFrom = directionFrom;
	group1.directionTo = directionTo;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(WindShearGroup, defaultDataEqual) {
	const metaf::WindShearGroup group1;
	const metaf::WindShearGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal 
TEST(WindShearGroup, sameDataEqual) {
	static const auto direction = 240;
	static const auto height = 4000;
	static const auto speed = 20;
	static const auto speedUnit = metaf::SpeedUnit::KNOTS;
	metaf::WindShearGroup group1;
	metaf::WindShearGroup group2;
	group1.direction = direction;
	group1.height = height;
	group1.speed = speed;
	group1.speedUnit = speedUnit;
	group2.direction = direction;
	group2.height = height;
	group2.speed = speed;
	group2.speedUnit = speedUnit;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different values are not equal 
TEST(WindShearGroup, differentDataNotEqual) {
	metaf::WindShearGroup group;
	group.direction = 240;
	group.height = 4000;
	group.speed = 20;
	group.speedUnit = metaf::SpeedUnit::KNOTS;
	metaf::WindShearGroup group1(group);
	group1.direction = 180;
	metaf::WindShearGroup group2(group);
	group2.height = 6000;
	metaf::WindShearGroup group3(group);
	group3.speed = 22;
	metaf::WindShearGroup group4(group);
	group4.speedUnit = metaf::SpeedUnit::METERS_PER_SECOND;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
}

//Confirm that constructor initialises the group with correct data
TEST(WindShearGroup, constructor) {
	static const auto direction = 240;
	static const auto height = 4000;
	static const auto speed = 20;
	static const auto speedUnit = metaf::SpeedUnit::KNOTS;
	const metaf::WindShearGroup group(height, direction, speed, speedUnit);
	metaf::WindShearGroup group1;
	group1.height = height;
	group1.direction = direction;
	group1.speed = speed;
	group1.speedUnit = speedUnit;
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(VisibilityGroup, defaultDataEqual) {
	const metaf::VisibilityGroup group1;
	const metaf::VisibilityGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal 
TEST(VisibilityGroup, sameDataEqual) {
	static const auto unit = metaf::DistanceUnit::STATUTE_MILES;
	static const auto direction = metaf::VisibilityGroup::Direction::NONE;
	static const auto integer = 1;
	static const auto numerator = 3;
	static const auto denominator = 4;
	static const auto reported = true;
	metaf::VisibilityGroup group1;
	metaf::VisibilityGroup group2;
	group1.unit = unit;
	group1.direction = direction;
	group1.integer = integer;
	group1.numerator = numerator;
	group1.denominator = denominator;
	group1.reported = reported;
	group2.unit = unit;
	group2.direction = direction;
	group2.integer = integer;
	group2.numerator = numerator;
	group2.denominator = denominator;
	group2.reported = reported;
	EXPECT_EQ(group1, group2);
}

//Confirm that if visibility is not reported, the following fields do NOT 
//affect the comparison result: incompleteInteger, lessThan, integer, 
//numerator, denominator
TEST(VisibilityGroup, comparisonVisibilityNotReported1) {
	metaf::VisibilityGroup group;
	group.unit = metaf::DistanceUnit::METERS;
	group.direction = metaf::VisibilityGroup::Direction::NDV;
	metaf::VisibilityGroup group1(group);
	group1.incompleteInteger = true;
	metaf::VisibilityGroup group2(group);
	group2.modifier = metaf::ValueModifier::LESS_THAN;
	metaf::VisibilityGroup group3(group);
	group3.integer = 1;
	metaf::VisibilityGroup group4(group);
	group4.numerator = 1;
	metaf::VisibilityGroup group5(group);
	group5.denominator = 1;
	EXPECT_EQ(group, group1);
	EXPECT_EQ(group, group2);
	EXPECT_EQ(group, group3);
	EXPECT_EQ(group, group4);
	EXPECT_EQ(group, group5);
}

//Confirm that if visibility is not reported, unit and direction DO affect 
//the comparison result
TEST(VisibilityGroup, comparisonVisibilityNotReported2) {
	metaf::VisibilityGroup group;
	group.unit = metaf::DistanceUnit::METERS;
	group.direction = metaf::VisibilityGroup::Direction::NDV;
	metaf::VisibilityGroup group1(group);
	group1.unit = metaf::DistanceUnit::STATUTE_MILES;
	metaf::VisibilityGroup group2(group);
	group2.direction = metaf::VisibilityGroup::Direction::NONE;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
}

//Confirm that if visibility is reported but incomplete, lessThan, numerator, 
//and denominator do NOT affect the comparison results
TEST(VisibilityGroup, comparisonVisibilityIncomplete1) {
	metaf::VisibilityGroup group;
	group.unit = metaf::DistanceUnit::STATUTE_MILES;
	group.direction = metaf::VisibilityGroup::Direction::NONE;
	group.reported = true;
	group.integer = 1;
	group.incompleteInteger = true;
	metaf::VisibilityGroup group1(group);
	group1.modifier = metaf::ValueModifier::LESS_THAN;
	metaf::VisibilityGroup group2(group);
	group2.numerator = 1;
	metaf::VisibilityGroup group3(group);
	group3.denominator = 1;
	EXPECT_EQ(group, group1);
	EXPECT_EQ(group, group2);
	EXPECT_EQ(group, group3);
}

//Confirm that if visibility is reported but incomplete, integer DOES affect
//the comparison result
TEST(VisibilityGroup, comparisonVisibilityIncomplete2) {
	metaf::VisibilityGroup group;
	group.unit = metaf::DistanceUnit::STATUTE_MILES;
	group.direction = metaf::VisibilityGroup::Direction::NONE;
	group.reported = true;
	group.integer = 1;
	group.incompleteInteger = true;
	metaf::VisibilityGroup group1(group);
	group1.integer = 2;
	EXPECT_NE(group, group1);
}

//Confirm that if visibility is reported and complete, all fields affect comparison result
TEST(VisibilityGroup, comparisonVisibilityReportedAndComplete) {
	metaf::VisibilityGroup group;
	group.unit = metaf::DistanceUnit::STATUTE_MILES;
	group.direction = metaf::VisibilityGroup::Direction::NONE;
	group.reported = true;
	group.integer = 1;
	group.numerator = 3;
	group.denominator = 4;
	metaf::VisibilityGroup group1(group);
	group1.unit = metaf::DistanceUnit::METERS;
	metaf::VisibilityGroup group2(group);
	group2.direction = metaf::VisibilityGroup::Direction::NW;
	metaf::VisibilityGroup group3(group);
	group3.reported = false;
	metaf::VisibilityGroup group4(group);
	group4.modifier = metaf::ValueModifier::LESS_THAN;
	metaf::VisibilityGroup group5(group);
	group5.incompleteInteger = true;
	metaf::VisibilityGroup group6(group);
	group6.integer = 2;
	metaf::VisibilityGroup group7(group);
	group7.numerator = 2;
	metaf::VisibilityGroup group8(group);
	group8.denominator = 2;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
	EXPECT_NE(group, group5);
	EXPECT_NE(group, group6);
	EXPECT_NE(group, group7);
	EXPECT_NE(group, group8);
}

//Confirm that 'not reported' version of makeVisibilityMeters initialises the 
//group with correct data
TEST(VisibilityGroup, makeVisibilityMeters1) {
	static const auto direction = metaf::VisibilityGroup::Direction::NDV;
	const metaf::VisibilityGroup group = 
		metaf::VisibilityGroup::makeVisibilityMeters(direction);
	metaf::VisibilityGroup group1;
	group1.unit = metaf::DistanceUnit::METERS;
	group1.reported = false;
	group1.direction = direction;
	EXPECT_EQ(group, group1);
}

//Confirm that 'reported' version of makeVisibilityMeters initialises the group 
//with correct data
TEST(VisibilityGroup, makeVisibilityMeters2) {
	static const auto direction = metaf::VisibilityGroup::Direction::NW;
	static const auto visibility = 2000;
	const metaf::VisibilityGroup group = 
		metaf::VisibilityGroup::makeVisibilityMeters(visibility, direction);
	metaf::VisibilityGroup group1;
	group1.unit = metaf::DistanceUnit::METERS;
	group1.reported = true;
	group1.integer = visibility;
	group1.direction = direction;
	EXPECT_EQ(group, group1);
}

//Confirm that 'not reported' version of makeVisibilityMiles initialises the 
//group with correct data
TEST(VisibilityGroup, makeVisibilityMiles1) {
	const metaf::VisibilityGroup group = metaf::VisibilityGroup::makeVisibilityMiles();
	metaf::VisibilityGroup group1;
	group1.reported = false;
	group1.direction = metaf::VisibilityGroup::Direction::NONE;
	group1.unit = metaf::DistanceUnit::STATUTE_MILES;
	EXPECT_EQ(group, group1);
}

//Confirm that 'reported' version of makeVisiblityMiles initialises the group 
//with correct data
TEST(VisibilityGroup, makeVisibilityMiles2) {
	static const auto integer = 0;
	static const auto numerator = 1;
	static const auto denominator = 4;
	static const auto modifier = metaf::ValueModifier::LESS_THAN;
	const metaf::VisibilityGroup group = metaf::VisibilityGroup::makeVisibilityMiles(
		integer, 
		numerator, 
		denominator, 
		modifier);
	metaf::VisibilityGroup group1;
	group1.direction = metaf::VisibilityGroup::Direction::NONE;
	group1.unit = metaf::DistanceUnit::STATUTE_MILES;
	group1.reported = true;
	group1.integer = integer;
	group1.numerator = numerator;
	group1.denominator = denominator;
	group1.modifier = metaf::ValueModifier::LESS_THAN;
	EXPECT_EQ(group, group1);
}

//Confirm that makeVisiblityMilesIncomplete initialises the group with correct 
//data
TEST(VisibilityGroup, makeVisibilityMilesIncomplete) {
	static const auto integer = 1;
	const metaf::VisibilityGroup group = 
		metaf::VisibilityGroup::makeVisibilityMilesIncomplete(integer);
	metaf::VisibilityGroup group1;
	group1.direction = metaf::VisibilityGroup::Direction::NONE;
	group1.unit = metaf::DistanceUnit::STATUTE_MILES;
	group1.reported = true;
	group1.integer = integer;
	group1.incompleteInteger = true;
	EXPECT_EQ(group, group1);
}

//Confirm that incomplete integer group correctly merges following fraction group
TEST(VisibilityGroup, mergeVisibilityGroups1) {
	static const auto integer = 1;
	static const auto numerator = 3;
	static const auto denominator = 4;
	const metaf::VisibilityGroup completeGroup = 
		metaf::VisibilityGroup::makeVisibilityMiles(integer, numerator, denominator);
	const metaf::VisibilityGroup incompleteIntegerGroup = 
		metaf::VisibilityGroup::makeVisibilityMilesIncomplete(integer);
	const metaf::VisibilityGroup fractionGroup = 
		metaf::VisibilityGroup::makeVisibilityMiles(0, numerator, denominator);
	metaf::VisibilityGroup mergeGroup(incompleteIntegerGroup);
	bool result = mergeGroup.nextGroup(fractionGroup);
	EXPECT_TRUE(result);
	EXPECT_EQ(mergeGroup, completeGroup);
}

//Confirm that fraction group before incomplete integer (wrong order, 
//e.g 1/2 1 instead of 1 1/2) groups do not merge
TEST(VisibilityGroup, mergeVisibilityGroups2) {
	static const auto integer = 1;
	static const auto numerator = 3;
	static const auto denominator = 4;
	const metaf::VisibilityGroup incompleteIntegerGroup = 
		metaf::VisibilityGroup::makeVisibilityMilesIncomplete(integer);
	const metaf::VisibilityGroup fractionGroup = 
		metaf::VisibilityGroup::makeVisibilityMiles(0, numerator, denominator);
	metaf::VisibilityGroup mergeGroup(fractionGroup);
	bool result = mergeGroup.nextGroup(incompleteIntegerGroup);
	EXPECT_FALSE(result);
	EXPECT_EQ(mergeGroup, fractionGroup);
}

//Confirm that incomplete integer group does not merge the following groups:
// - Not reported visibility
// - Visibility in meters
// - Visibility in miles with non-zero integer
// - Visibility in miles with zero numerator
// - Visibility in miles with zero denominator
// - Visibility in miles with lessThan flag
// - Another incomplete visibility group
TEST(VisibilityGroup, mergeVisibilityGroups3) {
	const metaf::VisibilityGroup incompleteIntegerGroup = 
		metaf::VisibilityGroup::makeVisibilityMilesIncomplete(1);

	const metaf::VisibilityGroup notReportedVisibility = 
		metaf::VisibilityGroup::makeVisibilityMiles();
	metaf::VisibilityGroup mergeGroup1(incompleteIntegerGroup);
	EXPECT_FALSE(mergeGroup1.nextGroup(notReportedVisibility));
	EXPECT_EQ(mergeGroup1, incompleteIntegerGroup);

	const metaf::VisibilityGroup visibilityMeters = 
		metaf::VisibilityGroup::makeVisibilityMeters(1000);
	metaf::VisibilityGroup mergeGroup2(incompleteIntegerGroup);
	EXPECT_FALSE(mergeGroup2.nextGroup(visibilityMeters));
	EXPECT_EQ(mergeGroup2, incompleteIntegerGroup);

	const metaf::VisibilityGroup visibilityMilesInteger = 
		metaf::VisibilityGroup::makeVisibilityMiles(1);
	metaf::VisibilityGroup mergeGroup3(incompleteIntegerGroup);
	EXPECT_FALSE(mergeGroup3.nextGroup(visibilityMilesInteger));
	EXPECT_EQ(mergeGroup3, incompleteIntegerGroup);

	const metaf::VisibilityGroup visibilityMilesZeroNumerator = 
		metaf::VisibilityGroup::makeVisibilityMiles(0, 0, 2);
	metaf::VisibilityGroup mergeGroup4(incompleteIntegerGroup);
	EXPECT_FALSE(mergeGroup4.nextGroup(visibilityMilesZeroNumerator));
	EXPECT_EQ(mergeGroup4, incompleteIntegerGroup);

	const metaf::VisibilityGroup visibilityMilesZeroDenominator = 
		metaf::VisibilityGroup::makeVisibilityMiles(0, 1, 0);
	metaf::VisibilityGroup mergeGroup5(incompleteIntegerGroup);
	EXPECT_FALSE(mergeGroup5.nextGroup(visibilityMilesZeroDenominator));
	EXPECT_EQ(mergeGroup5, incompleteIntegerGroup);

	const metaf::VisibilityGroup visibilityMilesLessThan = 
		metaf::VisibilityGroup::makeVisibilityMiles(0, 1, 4, metaf::ValueModifier::LESS_THAN);
	metaf::VisibilityGroup mergeGroup6(incompleteIntegerGroup);
	EXPECT_FALSE(mergeGroup6.nextGroup(visibilityMilesLessThan));
	EXPECT_EQ(mergeGroup6, incompleteIntegerGroup);

	metaf::VisibilityGroup mergeGroup7(incompleteIntegerGroup);
	EXPECT_FALSE(mergeGroup7.nextGroup(incompleteIntegerGroup));
	EXPECT_EQ(mergeGroup7, incompleteIntegerGroup);
}

//Confirm that the following groups do not merge with fraction group: 
// - Not reported visibility
// - Visibility in meters
// - Visibility in miles (integer)
// - Visibility in miles (integer and fraction)
// - Visibility in miles with lessThan flag
TEST(VisibilityGroup, mergeVisibilityGroups4) {
	const metaf::VisibilityGroup incompleteIntegerGroup = 
		metaf::VisibilityGroup::makeVisibilityMilesIncomplete(1);
	
	const metaf::VisibilityGroup notReportedVisibility = 
		metaf::VisibilityGroup::makeVisibilityMiles();
	metaf::VisibilityGroup mergeGroup1(notReportedVisibility);
	EXPECT_FALSE(mergeGroup1.nextGroup(incompleteIntegerGroup));
	EXPECT_EQ(mergeGroup1, notReportedVisibility);

	const metaf::VisibilityGroup visibilityMeters = 
		metaf::VisibilityGroup::makeVisibilityMeters(1000);
	metaf::VisibilityGroup mergeGroup2(visibilityMeters);
	EXPECT_FALSE(mergeGroup2.nextGroup(incompleteIntegerGroup));
	EXPECT_EQ(mergeGroup2, visibilityMeters);

	const metaf::VisibilityGroup visibilityMilesInteger = 
		metaf::VisibilityGroup::makeVisibilityMiles(1);
	metaf::VisibilityGroup mergeGroup3(visibilityMilesInteger);
	EXPECT_FALSE(mergeGroup3.nextGroup(incompleteIntegerGroup));
	EXPECT_EQ(mergeGroup3, visibilityMilesInteger);

	const metaf::VisibilityGroup visibilityMilesIntegerFraction = 
		metaf::VisibilityGroup::makeVisibilityMiles(1, 3, 4);
	metaf::VisibilityGroup mergeGroup4(visibilityMilesIntegerFraction);
	EXPECT_FALSE(mergeGroup4.nextGroup(incompleteIntegerGroup));
	EXPECT_EQ(mergeGroup4, visibilityMilesIntegerFraction);

	const metaf::VisibilityGroup visibilityMilesLessThan = 
		metaf::VisibilityGroup::makeVisibilityMiles(0, 1, 4, metaf::ValueModifier::LESS_THAN);
	metaf::VisibilityGroup mergeGroup5(visibilityMilesLessThan);
	EXPECT_FALSE(mergeGroup5.nextGroup(incompleteIntegerGroup));
	EXPECT_EQ(mergeGroup5, visibilityMilesLessThan);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(CloudGroup, defaultDataEqual) {
	const metaf::CloudGroup group1;
	const metaf::CloudGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(CloudGroup, sameDataEqual) {
	static const auto amount = metaf::CloudGroup::Amount::FEW;
	static const auto height = 2000;
	static const auto heightReported = true;
	static const auto type = metaf::CloudGroup::Type::NONE;
	metaf::CloudGroup group1;
	metaf::CloudGroup group2;
	group1.amount = amount;
	group1.heightReported = heightReported;
	group1.height = height;
	group1.type = type;
	group2.amount = amount;
	group2.heightReported = heightReported;
	group2.height = height;
	group2.type = type;
	EXPECT_EQ(group1, group2);
}

//Confirm that if height is not reported, it does not affect the comparison result
TEST(CloudGroup, comparisonHeightNotReported) {
	metaf::CloudGroup group;
	group.amount = metaf::CloudGroup::Amount::FEW;
	group.height = 2000;
	group.type = metaf::CloudGroup::Type::NONE;
	metaf::CloudGroup group1 (group);
	group1.height = 1000;
	EXPECT_EQ(group, group1);
}

//Confirm that groups holding the different values are not equal
TEST(CloudGroup, differentDataNotEqual) {
	metaf::CloudGroup group;
	group.amount = metaf::CloudGroup::Amount::FEW;
	group.heightReported = true;
	group.height = 2000;
	group.type = metaf::CloudGroup::Type::NONE;
	metaf::CloudGroup group1;
	group1.amount = metaf::CloudGroup::Amount::BROKEN;
	metaf::CloudGroup group2;
	group2.heightReported = false;
	metaf::CloudGroup group3;
	group3.height = 1000;
	metaf::CloudGroup group4;
	group4.type = metaf::CloudGroup::Type::TOWERING_CUMULUS;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
}

//Confirm that constructor with height initialises the group with correct data
TEST(CloudGroup, constructorHeightReported) {
	static const auto amount = metaf::CloudGroup::Amount::OVERCAST;
	static const auto height = 1000;
	static const auto type = metaf::CloudGroup::Type::NONE;
	const metaf::CloudGroup group(amount, height, type);
	metaf::CloudGroup group1;
	group1.amount = amount;
	group1.height = height;
	group1.heightReported = true;
	group1.type = type;
	EXPECT_EQ(group, group1);
}

//Confirm that constructor without height initialises the group with correct data
TEST(CloudGroup, constructorHeightNotReported) {
	static const auto amount = metaf::CloudGroup::Amount::OVERCAST;
	static const auto type = metaf::CloudGroup::Type::NONE;
	const metaf::CloudGroup group(amount, type);
	metaf::CloudGroup group1;
	group1.amount = amount;
	group1.type = type;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(VerticalVisibilityGroup, defaultDataEqual) {
	const metaf::VerticalVisibilityGroup group1;
	const metaf::VerticalVisibilityGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(VerticalVisibilityGroup, sameDataEqual) {
	static const auto vertVisibility = 100;
	static const auto reported = true;
	metaf::VerticalVisibilityGroup group1;
	metaf::VerticalVisibilityGroup group2;
	group1.reported = reported;
	group1.vertVisibility = vertVisibility;
	group2.reported = reported;
	group2.vertVisibility = vertVisibility;
	EXPECT_EQ(group1, group2);
}

//Confirm that if visibility is not reported, it does NOT affect the comparison result
TEST(VerticalVisibilityGroup, comparisonVisibilityNotReported) {
	metaf::VerticalVisibilityGroup group;
	group.vertVisibility = 100;
	metaf::VerticalVisibilityGroup group1(group);
	group1.vertVisibility = 0;
	EXPECT_EQ(group, group1);
}

//Confirm that groups holding the different values are not equal 
TEST(VerticalVisibilityGroup, differentDataNotEqual) {
	metaf::VerticalVisibilityGroup group;
	group.reported = true;
	group.vertVisibility = 100;
	metaf::VerticalVisibilityGroup group1(group);
	group1.reported = false;
	metaf::VerticalVisibilityGroup group2(group);
	group2.vertVisibility = 0;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
}

//Confirm that constructor initialises the group with correct data
TEST(VerticalVisibilityGroup, constructor) {
	static const auto vertVisibility = 700;
	const metaf::VerticalVisibilityGroup group(vertVisibility);
	metaf::VerticalVisibilityGroup group1;
	group1.vertVisibility = vertVisibility;
	group1.reported = true;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(WeatherGroup, defaultDataEqual) {
	const metaf::WeatherGroup group1;
	const metaf::WeatherGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(WeatherGroup, sameDataEqual) {
	static const auto prefix = metaf::WeatherGroup::Prefix::VICINITY;
	static const auto weatherSize = 3;
	static const auto weather1 = metaf::WeatherGroup::Weather::SHOWERS;
	static const auto weather2 = metaf::WeatherGroup::Weather::RAIN;
	static const auto weather3 = metaf::WeatherGroup::Weather::SNOW;
	metaf::WeatherGroup group1;
	metaf::WeatherGroup group2;
	group1.prefix = prefix;
	group1.weather[0] = weather1;
	group1.weather[1] = weather2;
	group1.weather[2] = weather3;
	group1.weatherSize = weatherSize;
	group2.prefix = prefix;
	group2.weather[0] = weather1;
	group2.weather[1] = weather2;
	group2.weather[2] = weather3;
	group2.weatherSize = weatherSize;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different values are not equal
TEST(WeatherGroup, differentDataNotEqual) {
	metaf::WeatherGroup group;
	group.prefix = metaf::WeatherGroup::Prefix::VICINITY;
	group.weather[0] = metaf::WeatherGroup::Weather::SHOWERS;
	group.weather[1] = metaf::WeatherGroup::Weather::RAIN;
	group.weatherSize = 2;
	metaf::WeatherGroup group1(group);
	group1.prefix = metaf::WeatherGroup::Prefix::NONE;
	metaf::WeatherGroup group2(group);
	group2.weather[1] = metaf::WeatherGroup::Weather::SNOW;
	metaf::WeatherGroup group3(group);
	group3.weatherSize = 1;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
}

//Confirm that constructor from vector initialises the group with the correct 
//data
TEST(WeatherGroup, constructorVector) {
	static const auto prefix = metaf::WeatherGroup::Prefix::VICINITY;
	const std::vector<metaf::WeatherGroup::Weather> weather = {
		metaf::WeatherGroup::Weather::SHOWERS,
		metaf::WeatherGroup::Weather::RAIN,
		metaf::WeatherGroup::Weather::SNOW,
	};
	const metaf::WeatherGroup group(prefix, weather);
	metaf::WeatherGroup group1;
	group1.prefix = prefix;
	ASSERT_LT(weather.size(), metaf::WeatherGroup::maxWeatherSize);
	group1.weatherSize = weather.size();
	for (auto i=0; i<weather.size(); i++) {
		group1.weather[i] = weather.at(i);
	}
	EXPECT_EQ(group, group1);
}

//Confirm that constructor from fixed list initialises the group with the 
//correct data
TEST(WeatherGroup, constructorFixedList) {
	static const auto prefix = metaf::WeatherGroup::Prefix::VICINITY;
	static const auto weather1 = metaf::WeatherGroup::Weather::SHOWERS;
	static const auto weather2 = metaf::WeatherGroup::Weather::RAIN;
	const metaf::WeatherGroup group(prefix, weather1, weather2);
	metaf::WeatherGroup group1;
	group1.prefix = prefix;
	group1.weatherSize = 2;
	group1.weather[0] = weather1;
	group1.weather[1] = weather2;
	EXPECT_EQ(group, group1);
}

//Confirm that makeNotReported initialises the group with correct data
TEST(WeatherGroup, makeNotReported) {
	static const auto recent = true;
	const metaf::WeatherGroup group = metaf::WeatherGroup::makeNotReported(true);
	metaf::WeatherGroup group1;
	group1.prefix = metaf::WeatherGroup::Prefix::NONE;
	if (recent) group1.prefix = metaf::WeatherGroup::Prefix::RECENT;
	group1.weatherSize = 1;
	group1.weather[0] = metaf::WeatherGroup::Weather::NOT_REPORTED;
	EXPECT_EQ(group, group1);
}

//Confirm that toVector() returns the correct data
TEST(WeatherGroup, toVector1) {
	static const auto prefix = metaf::WeatherGroup::Prefix::VICINITY;
	const std::vector<metaf::WeatherGroup::Weather> weather = {
		metaf::WeatherGroup::Weather::SHOWERS,
		metaf::WeatherGroup::Weather::RAIN,
		metaf::WeatherGroup::Weather::SNOW,
	};
	const metaf::WeatherGroup group(prefix, weather);
	EXPECT_EQ(group.weatherToVector(), weather);
}

//Confirm that if no weather is specified, toVector returns an empty vector
TEST(WeatherGroup, toVector2) {
	const metaf::WeatherGroup group;
	EXPECT_FALSE(group.weatherToVector().size());
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(TemperatureGroup, defaultDataEqual) {
	const metaf::TemperatureGroup group1;
	const metaf::TemperatureGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(TemperatureGroup, sameDataEqual) {
	static const auto temperature = 2;
	static const auto temperatureReported = true;
	static const auto dewPoint = -1;
	static const auto dewPointReported = true;
	metaf::TemperatureGroup group1;
	group1.temperature = temperature;
	group1.temperatureReported = temperatureReported;
	group1.dewPoint = dewPoint;
	group1.dewPointReported = dewPointReported;
	metaf::TemperatureGroup group2;
	group2.temperature = temperature;
	group2.temperatureReported = temperatureReported;
	group2.dewPoint = dewPoint;
	group2.dewPointReported = dewPointReported;
	EXPECT_EQ(group1, group2);
}

//Confirm that if temperature or dewpoing are NOT reported, they do NOT affect 
//the comparison result
TEST(TemperatureGroup, comparisonTemperatureDewpointNotReported) {
	metaf::TemperatureGroup group;
	group.temperature = 2;
	group.dewPoint = -1;
	metaf::TemperatureGroup group1(group);
	group1.temperature = 1;
	group1.dewPoint = -2;
	EXPECT_EQ(group, group1);
}

//Confirm that groups holding the different values are not equal
TEST(TemperatureGroup, differentDataNotEqual) {
	metaf::TemperatureGroup group;
	group.temperature = 2;
	group.temperatureReported = true;
	group.dewPoint = -1;
	group.dewPointReported = true;
	metaf::TemperatureGroup group1(group);
	group1.temperature = 1;
	metaf::TemperatureGroup group2(group);
	group2.temperatureReported = false;
	metaf::TemperatureGroup group3(group);
	group3.dewPoint = -2;
	metaf::TemperatureGroup group4(group);
	group4.dewPointReported = false;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
}

//Confirm that temperature-only constructor initialises the group with correct 
//data
TEST(TemperatureGroup, constructorTempertureOnly) {
	static const auto temperature = 5;
	const metaf::TemperatureGroup group(temperature);
	metaf::TemperatureGroup group1;
	group1.temperature = temperature;
	group1.temperatureReported = true;
	EXPECT_EQ(group, group1);
}

//Confirm that temperature & dewpoint constructor initialises the group with 
//correct data
TEST(TemperatureGroup, constructorTemperatureDewpoint) {
	static const auto temperature = 5;
	static const auto dewPoint = 0;
	const metaf::TemperatureGroup group(temperature, dewPoint);
	metaf::TemperatureGroup group1;
	group1.temperature = temperature;
	group1.temperatureReported = true;
	group1.dewPoint = dewPoint;
	group1.dewPointReported = true;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(MinMaxTemperatureGroup, defaultDataEqual) {
	const metaf::MinMaxTemperatureGroup group1;
	const metaf::MinMaxTemperatureGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(MinMaxTemperatureGroup, sameDataEqual) {
	static const auto point = metaf::MinMaxTemperatureGroup::Point::MINIMUM;
	static const auto temperature = -1;
	static const auto day = 12;
	static const auto hour = 17;
	metaf::MinMaxTemperatureGroup group1;
	metaf::MinMaxTemperatureGroup group2;
	group1.point = point;
	group1.temperature = temperature;
	group1.day = day;
	group1.hour = hour;
	group2.point = point;
	group2.temperature = temperature;
	group2.day = day;
	group2.hour = hour;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different value are not equal
TEST(MinMaxTemperatureGroup, differentDataNotEqual) {
	metaf::MinMaxTemperatureGroup group;
	group.point = metaf::MinMaxTemperatureGroup::Point::MINIMUM;
	group.temperature = -1;
	group.day = 12;
	group.hour = 17;
	metaf::MinMaxTemperatureGroup group1(group);
	group1.point = metaf::MinMaxTemperatureGroup::Point::MAXIMUM;
	metaf::MinMaxTemperatureGroup group2(group);
	group2.temperature = -2;
	metaf::MinMaxTemperatureGroup group3(group);
	group3.day = 13;
	metaf::MinMaxTemperatureGroup group4(group);
	group4.hour = 18;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
}

//Confirm that makeMin initialises the group with correct data
TEST(MinMaxTemperatureGroup, makeMin) {
	static const auto temperature = 1;
	static const auto day = 25;
	static const auto hour = 12;
	const metaf::MinMaxTemperatureGroup group = 
		metaf::MinMaxTemperatureGroup::makeMin(temperature, day, hour);
	metaf::MinMaxTemperatureGroup group1;
	group1.point = metaf::MinMaxTemperatureGroup::Point::MINIMUM;
	group1.temperature = temperature;
	group1.day = day;
	group1.hour = hour;
	EXPECT_EQ(group, group1);
}

//Confirm that makeMax initialises the group with correct data
TEST(MinMaxTemperatureGroup, makeMax) {
	static const auto temperature = 1;
	static const auto day = 25;
	static const auto hour = 12;
	const metaf::MinMaxTemperatureGroup group = 
		metaf::MinMaxTemperatureGroup::makeMax(temperature, day, hour);
	metaf::MinMaxTemperatureGroup group1;
	group1.point = metaf::MinMaxTemperatureGroup::Point::MAXIMUM;
	group1.temperature = temperature;
	group1.day = day;
	group1.hour = hour;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(PressureGroup, defaultDataEqual) {
	const metaf::PressureGroup group1;
	const metaf::PressureGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(PressureGroup, sameDataEqual) {
	static const auto unit = metaf::PressureGroup::Unit::INCHES_HG;
	static const auto pressure = 30.31;
	static const auto reported = true;
	metaf::PressureGroup group1;
	metaf::PressureGroup group2;
	group1.unit = unit;
	group1.pressure = pressure;
	group1.reported = reported;
	group2.unit = metaf::PressureGroup::Unit::INCHES_HG;
	group2.pressure = pressure;
	group2.reported = reported;
	EXPECT_EQ(group1, group2);
}

//Confirm that if pressure value is not reported, it does not affect the comparison result
TEST(PressureGroup, comparisonDayNotReported) {
	metaf::PressureGroup group;
	group.unit = metaf::PressureGroup::Unit::INCHES_HG;
	group.pressure = 30.31;
	metaf::PressureGroup group1(group);
	group1.pressure = 30.30;
	EXPECT_EQ(group, group1);
}

//Confirm that groups holding the different values are not equal
TEST(PressureGroup, differentDataNotEqual) {
	metaf::PressureGroup group;
	group.unit = metaf::PressureGroup::Unit::INCHES_HG;
	group.pressure = 30.31;
	group.reported = true;
	metaf::PressureGroup group1(group);
	group1.unit = metaf::PressureGroup::Unit::HECTOPASCAL;
	metaf::PressureGroup group2(group);
	group2.pressure = 30.30;
	metaf::PressureGroup group3(group);
	group3.reported = false;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
}

//Confirm that 'not reported' constructor initialises the group with correct 
//data
TEST(PressureGroup, constructorNotReported) {
	static const auto unit = metaf::PressureGroup::Unit::INCHES_HG;
	const metaf::PressureGroup group(unit);
	metaf::PressureGroup group1;
	group1.unit = unit;
	EXPECT_EQ(group, group1);
}

//Confirm that constructor initialises the group with correct data
TEST(PressureGroup, constructorReported) {
	static const auto unit = metaf::PressureGroup::Unit::INCHES_HG;
	static const auto pressure = 30.31;
	const metaf::PressureGroup group(pressure, unit);
	metaf::PressureGroup group1;
	group1.unit = unit;
	group1.pressure = pressure;
	group1.reported = true;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(RunwayVisualRangeGroup, defaultDataEqual) {
	const metaf::RunwayVisualRangeGroup group1;
	const metaf::RunwayVisualRangeGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same values are equal
TEST(RunwayVisualRangeGroup, sameDataEqual) {
	static const auto runway = metaf::Runway(22);
	static const auto visRange = 4500;
	static const auto visModifier = metaf::ValueModifier::NONE;
	static const auto varVisRange = 6500;
	static const auto varVisModifier = metaf::ValueModifier::NONE;
	static const auto visRangeReported = true;
	static const auto varVisRangeReported = true;
	static const auto unit = metaf::DistanceUnit::METERS;
	static const auto trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
	metaf::RunwayVisualRangeGroup group1;
	metaf::RunwayVisualRangeGroup group2;
	group1.runway = runway;
	group1.visRange = visRange;
	group1.visModifier = visModifier;
	group1.varVisRange = varVisRange;
	group1.varVisModifier = varVisModifier;
	group1.visRangeReported = visRangeReported;
	group1.varVisRangeReported = varVisRangeReported;
	group1.unit = unit;
	group1.trend = trend;
	group2.runway = runway;
	group2.visRange = visRange;
	group2.visModifier = visModifier;
	group2.varVisRange = varVisRange;
	group2.varVisModifier = varVisModifier;
	group2.visRangeReported = visRangeReported;
	group2.varVisRangeReported = varVisRangeReported;
	group2.unit = unit;
	group2.trend = trend;
	EXPECT_EQ(group1, group2);
}

//Confirm that if runway visual range is not reported, then visual range & 
//modifier, variable visual range & modifier, and variable visual range 
//reported flag do not affect the comparison result
TEST(RunwayVisualRangeGroup, comparisonRvrNotReported) {
	metaf::RunwayVisualRangeGroup group;
	group.runway = metaf::Runway(22);
	group.unit = metaf::DistanceUnit::METERS;
	group.trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
	group.visRangeReported = false;
	group.visRange = 4500;
	group.visModifier = metaf::ValueModifier::NONE;
	group.varVisRange = 6500;
	group.varVisModifier = metaf::ValueModifier::NONE;
	group.varVisRangeReported = true;
	metaf::RunwayVisualRangeGroup group1(group);
	group1.visRange = 4600;
	metaf::RunwayVisualRangeGroup group2(group);
	group2.visModifier = metaf::ValueModifier::LESS_THAN;
	metaf::RunwayVisualRangeGroup group3(group);
	group3.varVisRange = 6600;
	metaf::RunwayVisualRangeGroup group4(group);
	group4.varVisModifier = metaf::ValueModifier::MORE_THAN;
	metaf::RunwayVisualRangeGroup group5(group);
	group5.varVisRangeReported = false;
	EXPECT_EQ(group, group1);
	EXPECT_EQ(group, group2);
	EXPECT_EQ(group, group3);
	EXPECT_EQ(group, group4);
	EXPECT_EQ(group, group5);
}

//Confirm that if variable visual range is not reported, then variable 
//visual range value and modifier do not affect the comparison result
TEST(RunwayVisualRangeGroup, comparisonVariableRvr) {
	metaf::RunwayVisualRangeGroup group;
	group.runway = metaf::Runway(22);
	group.unit = metaf::DistanceUnit::METERS;
	group.visRangeReported = true;
	group.visRange = 4500;
	group.visModifier = metaf::ValueModifier::NONE;
	group.varVisRangeReported = false;
	group.varVisRange = 6500;
	group.varVisModifier = metaf::ValueModifier::NONE;
	group.unit = metaf::DistanceUnit::METERS;
	group.trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
	metaf::RunwayVisualRangeGroup group1(group);
	group1.varVisRange = 6500;
	metaf::RunwayVisualRangeGroup group2(group);
	group2.varVisModifier = metaf::ValueModifier::LESS_THAN;
	EXPECT_EQ(group, group1);
	EXPECT_EQ(group, group2);
}

//Confirm that groups holding different values are not equal
TEST(RunwayVisualRangeGroup, differentDataNotEqual) {
	metaf::RunwayVisualRangeGroup group;
	group.runway = metaf::Runway(22);
	group.visRange = 4500;
	group.visModifier = metaf::ValueModifier::NONE;
	group.varVisRange = 6500;
	group.varVisModifier = metaf::ValueModifier::NONE;
	group.visRangeReported = true;
	group.varVisRangeReported = true;
	group.unit = metaf::DistanceUnit::METERS;
	group.trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
	metaf::RunwayVisualRangeGroup group1(group);
	group1.runway = metaf::Runway(21);
	metaf::RunwayVisualRangeGroup group2(group);
	group.runway = metaf::Runway(22, metaf::Runway::Designator::CENTER);
	metaf::RunwayVisualRangeGroup group3(group);
	group3.visRange = 4600;
	metaf::RunwayVisualRangeGroup group4(group);
	group4.visModifier = metaf::ValueModifier::LESS_THAN;
	metaf::RunwayVisualRangeGroup group5(group);
	group5.varVisRange = 6600;
	metaf::RunwayVisualRangeGroup group6(group);
	group6.varVisModifier = metaf::ValueModifier::MORE_THAN;
	metaf::RunwayVisualRangeGroup group7(group);
	group7.visRangeReported = false;
	metaf::RunwayVisualRangeGroup group8(group);
	group8.varVisRangeReported = false;
	metaf::RunwayVisualRangeGroup group9(group);
	group9.unit = metaf::DistanceUnit::FEET;
	metaf::RunwayVisualRangeGroup group10(group);
	group10.trend = metaf::RunwayVisualRangeGroup::Trend::DOWNWARD;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
	EXPECT_NE(group, group5);
	EXPECT_NE(group, group6);
	EXPECT_NE(group, group7);
	EXPECT_NE(group, group8);
	EXPECT_NE(group, group9);
	EXPECT_NE(group, group10);
}

//Confirm that 'not reported' constructor initialises the group with correct 
//data
TEST(RunwayVisualRangeGroup, constructorNotReported) {
	static const auto runway = metaf::Runway(22);
	static const auto unit = metaf::DistanceUnit::METERS;
	static const auto trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
	metaf::RunwayVisualRangeGroup group(runway, unit, trend);
	metaf::RunwayVisualRangeGroup group1;
	group1.runway = runway;
	group1.unit = unit;
	group1.trend = trend;
	EXPECT_EQ(group, group1);
}

//Confirm that 'single rvr value' constructor initialises
//the group with correct data
TEST(RunwayVisualRangeGroup, constructorSingleRvrValue) {
	static const auto runway = metaf::Runway(22);
	static const auto visRange = 6500;
	static const auto modifier = metaf::ValueModifier::NONE;
	static const auto unit = metaf::DistanceUnit::METERS;
	static const auto trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
	metaf::RunwayVisualRangeGroup group(runway, 
		visRange, 
		unit, 
		modifier,
		trend);
	metaf::RunwayVisualRangeGroup group1;
	group1.runway = runway;
	group1.visRange = visRange;
	group1.visRangeReported = true;
	group1.visModifier = modifier;
	group1.unit = unit;
	group1.trend = trend;
	EXPECT_EQ(group, group1);
}

//Confirm that 'min/max visibility' constructor initialises the group with 
//correct data
TEST(RunwayVisualRangeGroup, constructorMinMaxRvrValue) {
	static const auto runway = metaf::Runway(22);
	static const auto visRange = 4500;
	static const auto modifier = metaf::ValueModifier::LESS_THAN;
	static const auto varVisRange = 6500;
	static const auto varVisModifier = metaf::ValueModifier::MORE_THAN;
	static const auto unit = metaf::DistanceUnit::FEET;
	static const auto trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
	metaf::RunwayVisualRangeGroup group(runway, 
		visRange, 
		varVisRange,
		unit, 
		modifier,
		varVisModifier,
		trend);
	metaf::RunwayVisualRangeGroup group1;
	group1.runway = runway;
	group1.visRange = visRange;
	group1.visRangeReported = true;
	group1.visModifier = modifier;
	group1.varVisRange = varVisRange;
	group1.varVisRangeReported = true;
	group1.varVisModifier = varVisModifier;
	group1.unit = unit;
	group1.trend = trend;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(RunwayStateGroup, defaultDataEqual) {
	const metaf::RunwayStateGroup group1;
	const metaf::RunwayStateGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(RunwayStateGroup, sameDataEqual) {
	static const auto runway = metaf::Runway(22);
	static const auto status = metaf::RunwayStateGroup::Status::NONE;
	static const auto deposits = metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES;
	static const auto contaminationExtent = metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT;
	static const metaf::RunwayStateGroup::DepositDepth dd(1);
	static const metaf::RunwayStateGroup::SurfaceFriction sf(0.65);
	metaf::RunwayStateGroup group1;
	metaf::RunwayStateGroup group2;
	group1.runway = runway;
	group1.status = status;
	group1.deposits = deposits;
	group1.contaminationExtent = contaminationExtent;
	group1.depositDepth = dd;
	group1.surfaceFriction = sf;
	group2.runway = runway;
	group2.status = status;
	group2.deposits = deposits;
	group2.contaminationExtent = contaminationExtent;
	group2.depositDepth = dd;
	group2.surfaceFriction = sf;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different values are not equal
TEST(RunwayStateGroup, differentDataNotEqual) {
	static const metaf::RunwayStateGroup::DepositDepth dd(1);
	static const metaf::RunwayStateGroup::SurfaceFriction sf(0.65);
	metaf::RunwayStateGroup group;
	group.runway = metaf::Runway(22);
	group.status = metaf::RunwayStateGroup::Status::NONE;
	group.deposits = metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES;
	group.contaminationExtent = metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT;
	group.depositDepth = dd;
	group.surfaceFriction = sf;
	metaf::RunwayStateGroup group1(group);
	group1.runway = metaf::Runway(23);
	metaf::RunwayStateGroup group2(group);
	group2.runway = metaf::Runway(22, metaf::Runway::Designator::CENTER);
	metaf::RunwayStateGroup group3(group);
	group3.status = metaf::RunwayStateGroup::Status::SNOCLO;
	metaf::RunwayStateGroup group4(group);
	group4.deposits = metaf::RunwayStateGroup::Deposits::DAMP;
	metaf::RunwayStateGroup group5(group);
	group5.contaminationExtent = metaf::RunwayStateGroup::Extent::LESS_THAN_10_PERCENT;
	metaf::RunwayStateGroup group6(group);
	group6.depositDepth.depth = 2;
	metaf::RunwayStateGroup group7(group);
	group7.surfaceFriction.coefficient = 0.66;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
	EXPECT_NE(group, group5);
	EXPECT_NE(group, group6);
	EXPECT_NE(group, group7);
}

//Confirm that constructor initialises the group with correct data
TEST(RunwayStateGroup, constructor) {
	static const auto runway = metaf::Runway(21, metaf::Runway::Designator::CENTER);
	static const auto status = metaf::RunwayStateGroup::Status::NONE;
	static const auto deposits = metaf::RunwayStateGroup::Deposits::DRY_SNOW;
	static const auto extent = metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT;
	static const auto ddDepth = 5;
	static const auto sfBrakingAction = 
		metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
	const metaf::RunwayStateGroup group(
		runway,
		status,
		deposits,
		extent,
		metaf::RunwayStateGroup::DepositDepth(ddDepth),
		metaf::RunwayStateGroup::SurfaceFriction(sfBrakingAction));
	metaf::RunwayStateGroup group1;
	group1.runway = runway;
	group1.status = status;
	group1.deposits = deposits;
	group1.contaminationExtent = extent;
	group1.depositDepth.status = metaf::RunwayStateGroup::DepositDepth::Status::REPORTED;
	group1.depositDepth.depth = ddDepth;
	group1.surfaceFriction.status = 
		metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED;
	group1.surfaceFriction.brakingAction = sfBrakingAction;
	EXPECT_EQ(group, group1);
}

//Confirm that makeSnoclo initialises the group with correct data
TEST(RunwayStateGroup, makeSnoclo) {
	static const auto runway = metaf::Runway(21, metaf::Runway::Designator::CENTER);
	const metaf::RunwayStateGroup group = metaf::RunwayStateGroup::makeSnoclo(runway);
	metaf::RunwayStateGroup group1;
	group1.runway = runway;
	group1.status = metaf::RunwayStateGroup::Status::SNOCLO;
	EXPECT_EQ(group, group1);
}

//Confirm that makeClrd initialises the group with correct data
TEST(RunwayStateGroup, makeClrd) {
	static const auto runway = metaf::Runway(21, metaf::Runway::Designator::CENTER);
	static const auto surfaceFriction = 0.67;
	const metaf::RunwayStateGroup group = metaf::RunwayStateGroup::makeClrd(runway,
		metaf::RunwayStateGroup::SurfaceFriction(surfaceFriction));
	metaf::RunwayStateGroup group1;
	group1.runway = runway;
	group1.status = metaf::RunwayStateGroup::Status::CLRD;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default-initialised structs are equal
TEST(RunwayStateGroup_DepositDepth, defaultDataEqual) {
	const metaf::RunwayStateGroup::DepositDepth dd1;
	const metaf::RunwayStateGroup::DepositDepth dd2;
	EXPECT_TRUE(dd1 == dd2);
	EXPECT_FALSE(dd1 != dd2);
}

//Confirm that structs holding the same value are equal
TEST(RunwayStateGroup_DepositDepth, sameDataEqual) {
	static const auto status = metaf::RunwayStateGroup::DepositDepth::Status::REPORTED;
	static const auto depth = 10;
	metaf::RunwayStateGroup::DepositDepth dd1;
	metaf::RunwayStateGroup::DepositDepth dd2;
	dd1.status = status;
	dd1.depth = depth;
	dd2.status = status;
	dd2.depth = depth;
	EXPECT_EQ(dd1, dd2);
}

//Confirm that if depth is not reported, it does not affect the comparison result
TEST(RunwayStateGroup_DepositDepth, comparisonDepthNotReported) {
	metaf::RunwayStateGroup::DepositDepth dd;
	dd.status = metaf::RunwayStateGroup::DepositDepth::Status::NOT_REPORTED;
	dd.depth = 10;
	metaf::RunwayStateGroup::DepositDepth dd1(dd);
	dd1.depth = 20;
	EXPECT_EQ(dd, dd1);
}

//Confirm that structs holding the different values are not equal
TEST(RunwayStateGroup_DepositDepth, differentDataNotEqual) {
	metaf::RunwayStateGroup::DepositDepth dd;
	dd.status = metaf::RunwayStateGroup::DepositDepth::Status::REPORTED;
	dd.depth = 10;
	metaf::RunwayStateGroup::DepositDepth dd1(dd);
	dd1.status = metaf::RunwayStateGroup::DepositDepth::Status::NOT_REPORTED;
	metaf::RunwayStateGroup::DepositDepth dd2(dd);
	dd2.depth = 20;
	EXPECT_NE(dd, dd1);
	EXPECT_NE(dd, dd2);
}

//Confirm that constructor initialises the struct with correct data
TEST(RunwayStateGroup_DepositDepth, constructor) {
	static const auto depth = 5;
	const metaf::RunwayStateGroup::DepositDepth dd(depth);
	metaf::RunwayStateGroup::DepositDepth dd1;
	dd1.status = metaf::RunwayStateGroup::DepositDepth::Status::REPORTED;
	dd1.depth = depth;
	EXPECT_EQ(dd, dd1);
}

//Confirm that makeNotReported initialises the struct with correct data
TEST(RunwayStateGroup_DepositDepth, makeNotReported) {
	const metaf::RunwayStateGroup::DepositDepth dd = 
		metaf::RunwayStateGroup::DepositDepth::makeNotReported();
	metaf::RunwayStateGroup::DepositDepth dd1;
	dd1.status = metaf::RunwayStateGroup::DepositDepth::Status::NOT_REPORTED;
	EXPECT_EQ(dd, dd1);
}

//Confirm that makeRunwayNotOperational initialises the struct with correct data
TEST(RunwayStateGroup_DepositDepth, makeRunwayNotOperational) {
	const metaf::RunwayStateGroup::DepositDepth dd = 
		metaf::RunwayStateGroup::DepositDepth::makeRunwayNotOperational();
	metaf::RunwayStateGroup::DepositDepth dd1;
	dd1.status = metaf::RunwayStateGroup::DepositDepth::Status::RUNWAY_NOT_OPERATIONAL;
	EXPECT_EQ(dd, dd1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default-initialised structs are equal
TEST(RunwayStateGroupSurfaceFriction, defaultDataEqual) {
	const metaf::RunwayStateGroup::SurfaceFriction sf1;
	const metaf::RunwayStateGroup::SurfaceFriction sf2;
	EXPECT_TRUE(sf1 == sf2);
	EXPECT_FALSE(sf1 != sf2);
}

//Confirm that if surface friction is reported, the braking action value does 
//NOT affect the comparison result
TEST(RunwayStateGroupSurfaceFriction, comparisonSurfaceFrictionReported1) {
	metaf::RunwayStateGroup::SurfaceFriction sf;
	sf.status = metaf::RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED;
	sf.coefficient = 0.65;
	sf.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
	metaf::RunwayStateGroup::SurfaceFriction sf1(sf);
	sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED;
	sf1.coefficient = 0.65;
	sf1.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
	EXPECT_EQ(sf, sf1);
}

//Confirm that if surface friction is reported, the surface friction value DOES 
//affect the comparison result
TEST(RunwayStateGroupSurfaceFriction, comparisonSurfaceFrictionReported2) {
	metaf::RunwayStateGroup::SurfaceFriction sf;
	sf.status = metaf::RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED;
	sf.coefficient = 0.65;
	sf.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
	metaf::RunwayStateGroup::SurfaceFriction sf1(sf);
	sf1.coefficient = 0.66;
	EXPECT_NE(sf, sf1);
}

//Confirm that if braking action is reported, the surface friction value does 
//NOT affect the comparison result
TEST(RunwayStateGroupSurfaceFriction, comparisonBrakingActionReported1) {
	metaf::RunwayStateGroup::SurfaceFriction sf1;
	metaf::RunwayStateGroup::SurfaceFriction sf2;
	sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED;
	sf1.coefficient = 0.64;
	sf1.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
	sf2.status = metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED;
	sf2.coefficient = 0.65;
	sf2.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
	EXPECT_EQ(sf1, sf2);
}

//Confirm that if braking action is reported, the braking action value DOES 
//affect the comparison result
TEST(RunwayStateGroupSurfaceFriction, comparisonBrakingActionReported2) {
	metaf::RunwayStateGroup::SurfaceFriction sf;
	sf.status = metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED;
	sf.coefficient = 0.65;
	sf.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
	metaf::RunwayStateGroup::SurfaceFriction sf1(sf);
	sf1.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
	EXPECT_NE(sf, sf1);
}

//Confirm that if neither braking action nor surface friction are reported, 
//these values do not affect the comparison result
TEST(RunwayStateGroupSurfaceFriction, comparisonNotReported) {
	metaf::RunwayStateGroup::SurfaceFriction sf;
	sf.status = metaf::RunwayStateGroup::SurfaceFriction::Status::NOT_REPORTED; 
	sf.coefficient = 0.64;
	sf.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
	metaf::RunwayStateGroup::SurfaceFriction sf1(sf);
	sf1.coefficient = 0.65;
	sf1.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
	EXPECT_EQ(sf, sf1);
}

//Confirm that if the status is 'unreliable', then neither braking action nor
//surface friction affect the comparison result
TEST(RunwayStateGroupSurfaceFriction, comparisonUnreliable) {
	metaf::RunwayStateGroup::SurfaceFriction sf;
	sf.status = metaf::RunwayStateGroup::SurfaceFriction::Status::UNRELIABLE; 
	sf.coefficient = 0.64;
	sf.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
	metaf::RunwayStateGroup::SurfaceFriction sf1(sf);
	sf1.coefficient = 0.65;
	sf1.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
	EXPECT_EQ(sf, sf1);
}

//Confirm that braking action constructor initialises the group with correct data
TEST(RunwayStateGroupSurfaceFriction, constructorBrakingAction) {
	static const auto ba = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::GOOD;
	const metaf::RunwayStateGroup::SurfaceFriction sf(ba);
	metaf::RunwayStateGroup::SurfaceFriction sf1;
	sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED;
	sf1.brakingAction = ba;
	EXPECT_EQ(sf, sf1);
}

//Confirm that surface friction constructor initialises the group with correct data
TEST(RunwayStateGroupSurfaceFriction, constructorSurfaceFriction) {
	static const auto sfValue = 0.7;
	const metaf::RunwayStateGroup::SurfaceFriction sf(sfValue);
	metaf::RunwayStateGroup::SurfaceFriction sf1;
	sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED;
	sf1.coefficient = sfValue;
	EXPECT_EQ(sf, sf1);
}

//Confirm that makeNotReported initialises the group with correct data
TEST(RunwayStateGroupSurfaceFriction, makeNotReported) {
	const metaf::RunwayStateGroup::SurfaceFriction sf = 
		metaf::RunwayStateGroup::SurfaceFriction::makeNotReported();
	metaf::RunwayStateGroup::SurfaceFriction sf1;
	sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::NOT_REPORTED;
	EXPECT_EQ(sf, sf1);
}

//Confirm that makeUnreliable initialises the group with correct data
TEST(RunwayStateGroupSurfaceFriction, makeUnreliable) {
	const metaf::RunwayStateGroup::SurfaceFriction sf = 
		metaf::RunwayStateGroup::SurfaceFriction::makeUnreliable();
	metaf::RunwayStateGroup::SurfaceFriction sf1;
	sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::UNRELIABLE;
	EXPECT_EQ(sf, sf1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(RainfallGroup, defaultDataEqual) {
	const metaf::RainfallGroup group1;
	const metaf::RainfallGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(RainfallGroup, sameDataEqual) {
	static const auto last10Minutes = 0.2;
	static const auto since9AM = 7.0;
	static const auto last60Minutes = 2.5;
	static const auto last60MinutesReported = true;
	metaf::RainfallGroup group1;
	metaf::RainfallGroup group2;
	group1.last10Minutes = last10Minutes;
	group1.since9AM = since9AM;
	group1.last60Minutes = last60Minutes;
	group1.last60MinutesReported = last60MinutesReported;
	group2.last10Minutes = last10Minutes;
	group2.since9AM = since9AM;
	group2.last60Minutes = last60Minutes;
	group2.last60MinutesReported = last60MinutesReported;
	EXPECT_EQ(group1, group2);
}

//Confirm that if 60-minute rainfall is not reported, it does not affect the 
//comparison result
TEST(RainfallGroup, comparison60minRainfallNotReported) {
	metaf::RainfallGroup group;
	group.last10Minutes = 0.2;
	group.since9AM = 7.0;
	group.last60Minutes = 2.5;
	metaf::RainfallGroup group1(group);
	group.last60Minutes = 2.4;
	EXPECT_EQ(group, group1);
}

//Confirm that groups holding the different values are not equal
TEST(RainfallGroup, differentDataNotEqual) {
	metaf::RainfallGroup group;
	group.last10Minutes = 0.2;
	group.since9AM = 7.0;
	group.last60Minutes = 2.5;
	group.last60MinutesReported = true;
	metaf::RainfallGroup group1(group);
	group1.last10Minutes = 0.3;
	metaf::RainfallGroup group2(group);
	group2.since9AM = 7.1;
	metaf::RainfallGroup group3(group);
	group3.last60Minutes = 2.6;
	metaf::RainfallGroup group4(group);
	group4.last60MinutesReported = false;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
}

//Confirm that short group constructor initialises the group with correct data
TEST(RainfallGroup, constructorShortGroup) {
	static const auto last10Minutes = 0.2;
	static const auto since9AM = 7.0;
	metaf::RainfallGroup group(last10Minutes, since9AM);
	metaf::RainfallGroup group1;
	group1.last10Minutes = last10Minutes;
	group1.since9AM = since9AM;
	EXPECT_EQ(group, group1);
}

//Confirm that long group constructor initialises the group with correct data
TEST(RainfallGroup, constructorLongGroup) {
	static const auto last10Minutes = 0.2;
	static const auto since9AM = 7.0;
	static const auto last60Minutes = 2.5;
	metaf::RainfallGroup group(last10Minutes, since9AM, last60Minutes);
	metaf::RainfallGroup group1;
	group1.last10Minutes = last10Minutes;
	group1.since9AM = since9AM;
	group1.last60Minutes = last60Minutes;
	group1.last60MinutesReported = true;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(SeaSurfaceGroup, defaultDataEqual) {
	const metaf::SeaSurfaceGroup group1;
	const metaf::SeaSurfaceGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(SeaSurfaceGroup, sameDataEqual) {
	static const auto temperature = 7;
	static const auto temperatureReported = true;
	static const auto stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
	metaf::SeaSurfaceGroup group1;
	group1.temperature = temperature;
	group1.temperatureReported = temperatureReported;
	group1.stateOfSurface = stateOfSurface;
	metaf::SeaSurfaceGroup group2;
	group2.temperature = temperature;
	group2.temperatureReported = temperatureReported;
	group2.stateOfSurface = stateOfSurface;
	EXPECT_EQ(group1, group2);
}

//Confirm that if the surface temperature is not reported, it does not affect 
//the comparison result
TEST(SeaSurfaceGroup, comparisonTemperatureNotReported) {
	metaf::SeaSurfaceGroup group;
	group.temperature = 7;
	group.stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
	metaf::SeaSurfaceGroup group1(group);
	group1.temperature = 6;
	EXPECT_EQ(group, group1);
}

//Confirm that groups holding the different values are not equal
TEST(SeaSurfaceGroup, differentDataNotEqual) {
	metaf::SeaSurfaceGroup group;
	group.temperature = 7;
	group.temperatureReported = true;
	group.stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
	metaf::SeaSurfaceGroup group1(group);
	group1.temperature = 6;
	metaf::SeaSurfaceGroup group2(group);
	group2.temperatureReported = false;
	metaf::SeaSurfaceGroup group3(group);
	group3.stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::VERY_ROUGH;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
}

//Confirm that temperature & stateOfSurface constructor initialises the group 
//with correct data
TEST(SeaSurfaceGroup, constructorTemeperatureStateofsurface) {
	static const auto temperature = 7;
	static const auto stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
	metaf::SeaSurfaceGroup group(temperature, stateOfSurface);
	metaf::SeaSurfaceGroup group1;
	group1.temperature = temperature;
	group1.temperatureReported = true;
	group1.stateOfSurface = stateOfSurface;
	EXPECT_EQ(group, group1);
}

//Confirm that stateOfSurface constructor initialises the group with correct 
//data
TEST(SeaSurfaceGroup, constructorStateofsurface) {
	static const auto stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
	metaf::SeaSurfaceGroup group(stateOfSurface);
	metaf::SeaSurfaceGroup group1;
	group1.temperatureReported = false;
	group1.stateOfSurface = stateOfSurface;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(SeaWavesGroup, defaultDataEqual) {
	const metaf::SeaWavesGroup group1;
	const metaf::SeaWavesGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(SeaWavesGroup, sameDataEqual) {
	static const auto temperature = 7;
	static const auto temperatureReported = true;
	static const auto waveHeight = 28;
	static const auto waveHeightReported = true;
	metaf::SeaWavesGroup group1;
	metaf::SeaWavesGroup group2;
	group1.temperature = temperature;
	group1.temperatureReported = temperatureReported;
	group1.waveHeight = waveHeight;
	group1.waveHeightReported = waveHeightReported;
	group2.temperature = temperature;
	group2.temperatureReported = temperatureReported;
	group2.waveHeight = waveHeight;
	group2.waveHeightReported = waveHeightReported;
	EXPECT_EQ(group1, group2);
}

//Confirm that if the surface temperature or wave height are not reported, 
//they does not affect the comparison result
TEST(SeaWavesGroup, comparisonTemperatureWaveHeightNotReported) {
	metaf::SeaWavesGroup group;
	group.temperature = 7;
	group.waveHeight = 28;
	metaf::SeaWavesGroup group1(group);
	group.temperature = 8;
	metaf::SeaWavesGroup group2(group);
	group.waveHeight = 29;
	EXPECT_EQ(group, group1);
	EXPECT_EQ(group, group2);
}

//Confirm that groups holding the different values are not equal
TEST(SeaWavesGroup, differentDataNotEqual) {
	metaf::SeaWavesGroup group;
	group.temperature = 7;
	group.temperatureReported = true;
	group.waveHeight = 28;
	group.waveHeightReported = true;
	metaf::SeaWavesGroup group1(group);
	group.temperature = 8;
	metaf::SeaWavesGroup group2(group);
	group.temperatureReported = false;
	metaf::SeaWavesGroup group3(group);
	group.waveHeight = 29;
	metaf::SeaWavesGroup group4(group);
	group.waveHeightReported = false;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
}

//Confirm that constructor initialises the group with correct data
TEST(SeaWavesGroup, constructor) {
	static const auto temperature = 5;
	static const auto waveHeight = 12;
	const metaf::SeaWavesGroup group(temperature, waveHeight);
	metaf::SeaWavesGroup group1;
	group1.temperature = temperature;
	group1.temperatureReported = true;
	group1.waveHeight = waveHeight;
	group1.waveHeightReported = true;
	EXPECT_EQ(group, group1);
}

//Confirm that makeTemperature initialises the group with correct data
TEST(SeaWavesGroup, makeTemperature) {
	static const auto temperature = 5;
	const metaf::SeaWavesGroup group = metaf::SeaWavesGroup::makeTemperature(temperature);
	metaf::SeaWavesGroup group1;
	group1.temperature = temperature;
	group1.temperatureReported = true;
	EXPECT_EQ(group, group1);
}

//Confirm that makeWaveHeight initialises the group with correct data
TEST(SeaWavesGroup, makeWaveHeight) {
	static const auto waveHeight = 12;
	const metaf::SeaWavesGroup group = metaf::SeaWavesGroup::makeWaveHeight(waveHeight);
	metaf::SeaWavesGroup group1;
	group1.waveHeight = waveHeight;
	group1.waveHeightReported = true;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

//Confirm that default groups are equal
TEST(ColourCodeGroup, defaultDataEqual) {
	const metaf::ColourCodeGroup group1;
	const metaf::ColourCodeGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same value are equal
TEST(ColourCodeGroup, sameDataEqual) {
	static const auto colourCode = metaf::ColourCodeGroup::Code::RED;
	static const auto codeBlack = true;
	metaf::ColourCodeGroup group1;
	metaf::ColourCodeGroup group2;
	group1.code = colourCode;
	group1.codeBlack = codeBlack;
	group2.code = colourCode;
	group2.codeBlack = codeBlack;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different values are not equal
TEST(ColourCodeGroup, differentDataNotEqual) {
	metaf::ColourCodeGroup group;
	group.code = metaf::ColourCodeGroup::Code::RED;
	group.codeBlack = true;
	metaf::ColourCodeGroup group1(group);
	group1.code = metaf::ColourCodeGroup::Code::BLUE;
	metaf::ColourCodeGroup group2(group);
	group2.codeBlack = false;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
}

//Confirm that constructor initialises the group with correct data
TEST(ColourCodeGroup, constructor) {
	static const auto colourCode = metaf::ColourCodeGroup::Code::YELLOW1;
	static const auto codeBlack = true;
	const metaf::ColourCodeGroup group(colourCode, codeBlack);
	metaf::ColourCodeGroup group1;
	group1.code = colourCode;
	group1.codeBlack = codeBlack;
	EXPECT_EQ(group, group1);
}

///////////////////////////////////////////////////////////////////////////////

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
	const metaf::Group group = metaf::ReportTimeGroup(8, 12, 30);
	EXPECT_EQ(metaf::getSyntaxGroup(group), metaf::SyntaxGroup::REPORT_TIME);
}

//Confirm that passing TimeSpanGroup to getSyntaxGroup() results in 
//SyntaxGroup::TIME_SPAN
TEST(getSyntaxGroup, TIME_SPAN) {
	const metaf::Group group = metaf::TimeSpanGroup(8, 12, 8, 24);
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
		metaf::TrendTimeGroup(metaf::TrendTimeGroup::Type::FROM, 11, 14, 00);
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
		metaf::WindGroup(240, metaf::SpeedUnit::KNOTS, 2, 7);
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
		metaf::WindShearGroup(2200, 30, 45, metaf::SpeedUnit::KNOTS);
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
	const metaf::Group group = metaf::MinMaxTemperatureGroup::makeMax(7, 15, 45);
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