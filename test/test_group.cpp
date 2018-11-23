/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "test.h"
#include "test_group.h"
#include "metaf.h"
#include <type_traits>

void test::TestGroup::run() {
	testGroup();
	testPlainTextGroup();
	testImmutableGroup();
	testLocationGroup();
	testReportTimeGroup();
	testTimeSpanGroup();
	testTrendTimeGroup();
	testProbabilityGroup();
	testWindGroup();
	testVarWindGroup();
	testWindShearGroup();
	testVisibilityGroup();
	testCloudGroup();
	testVerticalVisibilityGroup();
	testWeatherGroup();
	testTemperatureGroup();
	testMinMaxTemperatureGroup();
	testPressureGroup();
	testRunwayVisualRangeGroup();
	testRunwayStateGroup();
	testRunwayStateGroupDepositDepth();
	testRunwayStateGroupSurfaceFriction();
	testRainfallGroup();
	testSeaSurfaceGroup();
	testSeaWavesGroup();
	testColourCodeGroup();
	testGetSyntaxGroup();
}

void test::TestGroup::testGroup() {
	TEST_BEGIN();
	{
		//Confirm that the group occupies a continuous memory region
		//The point is to simplify passing Group between wasm and js
		TEST_ASSERT(std::is_standard_layout<metaf::Group>::value);
	}
	TEST_END();
}

void test::TestGroup::testPlainTextGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::PlainTextGroup group1;
		const metaf::PlainTextGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups initialised with the same data are equal 
		static const char testStr[] = "A1B2C3D4";
		metaf::PlainTextGroup group1;
		metaf::PlainTextGroup group2;
		strcpy(group1.text, testStr);
		strcpy(group2.text, testStr);
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups initialised with different data are not equal
		static const char testStr1[] = "A1B2C3D4";
		static const char testStr2[] = "A9B0";
		metaf::PlainTextGroup group1;
		metaf::PlainTextGroup group2;
		strcpy(group1.text, testStr1);
		strcpy(group2.text, testStr2);
		TEST_ASSERT(!(group1 == group2));
	}
	{
		//Confirm that constructor initialises the group with correct string
		static const char testStr[] = "A1B2C3D4";
		const metaf::PlainTextGroup group(testStr);
		metaf::PlainTextGroup group1;
		strcpy(group1.text, testStr);
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testImmutableGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::ImmutableGroup group1;
		const metaf::ImmutableGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same type are equal 
		static const auto type = metaf::ImmutableGroup::Type::SPECI;
		metaf::ImmutableGroup group1;
		metaf::ImmutableGroup group2;
		group1.type = type;
		group2.type = type;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the different type are not equal 
		metaf::ImmutableGroup group;
		group.type = metaf::ImmutableGroup::Type::METAR;
		metaf::ImmutableGroup group1(group);
		group1.type = metaf::ImmutableGroup::Type::SPECI;
		TEST_ASSERT(!(group == group1));
	}
	{
		//Confirm that constructor initialises the group with correct type
		static const auto type = metaf::ImmutableGroup::Type::METAR;
		const metaf::ImmutableGroup group(type);
		metaf::ImmutableGroup group1;
		group1.type = type;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testLocationGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::LocationGroup group1;
		const metaf::LocationGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups initialised with the same data are equal 
		static const char testLocation[] = "ICAO";
		metaf::LocationGroup group1;
		metaf::LocationGroup group2;
		strcpy(group1.location, testLocation);
		strcpy(group2.location, testLocation);
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups initialised with different data are not equal
		static const char testLocation1[] = "ICAO";
		static const char testLocation2[] = "ICA0";
		metaf::LocationGroup group1;
		metaf::LocationGroup group2;
		strcpy(group1.location, testLocation1);
		strcpy(group2.location, testLocation2);
		TEST_ASSERT(!(group1 == group2));
	}
	{
		//Confirm that constructor initialises the group with correct string
		static const char testLocation[] = "ICAO";
		const metaf::LocationGroup group(testLocation);
		metaf::LocationGroup group1;
		strcpy(group1.location, testLocation);
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testReportTimeGroup() {
	TEST_BEGIN();	
	{
		//Confirm that default groups are equal
		const metaf::ReportTimeGroup group1;
		const metaf::ReportTimeGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups with the same day/time are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups with different day/time are not equal
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
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
	}
	{
		//Confirm that constructor initialises the group with correct data
		static const auto day = 29;
		static const auto hour = 23;
		static const auto minute = 55;
		metaf::ReportTimeGroup group(day, hour, minute);
		metaf::ReportTimeGroup group1;
		group1.day = day;
		group1.hour = hour;
		group1.minute = minute;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testTimeSpanGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::TimeSpanGroup group1;
		const metaf::TimeSpanGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups with the same time are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
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
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
	}
	{
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
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testTrendTimeGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::TrendTimeGroup group1;
		const metaf::TrendTimeGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that the groups without day reported and with the same time are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that the groups with the day reported and same day/time are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if day is not reported, its value does not affect comparison
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding different data are not equal
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
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
		TEST_ASSERT(!(group == group5));
	}
	{
		//Confirm that long trend time group constructor initialises the group 
		//with correct data
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
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that short trend time group constructor initialises the group 
		//with correct data
		static const auto type = metaf::TrendTimeGroup::Type::FROM;
		static const auto hour = 17;
		static const auto minute = 30;
		const metaf::TrendTimeGroup group(type, hour, minute);
		metaf::TrendTimeGroup group1;
		group1.type = type;
		group1.hour = hour;
		group1.minute = minute;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testProbabilityGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::ProbabilityGroup group1;
		const metaf::ProbabilityGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal 
		static const auto probability = 30;
		metaf::ProbabilityGroup group1;
		metaf::ProbabilityGroup group2;
		group1.probability = probability;
		group2.probability = probability;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the different value are not equal 
		metaf::ProbabilityGroup group1;
		metaf::ProbabilityGroup group2;
		group1.probability = 30;
		group2.probability = 40;
		TEST_ASSERT(!(group1 == group2));
	}
	{
		//Confirm that constructor initialises the group with correct data
		static const auto probability = 30;
		const metaf::ProbabilityGroup group(probability);
		metaf::ProbabilityGroup group1;
		group1.probability = probability;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testWindGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::WindGroup group1;
		const metaf::WindGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same data (direction, speed, gust speed, unit) are equal
		static const auto direction = 120;
		static const auto directionReported = true;
		static const auto speed = 12;
		static const auto speedReported = true;
		static const auto gustSpeed = 20;
		static const auto gustSpeedReported = true;
		static const auto unit = metaf::WindGroup::Unit::METERS_PER_SECOND;
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if the wind is variable, direction does not affect the comparison
		metaf::WindGroup group;
		group.direction = 0;
		group.directionReported = true;
		group.directionVariable = true;
		group.speed = 12;
		group.speedReported = true;
		group.gustSpeed = 20;
		group.gustSpeedReported = true;
		group.unit = metaf::WindGroup::Unit::METERS_PER_SECOND;
		metaf::WindGroup group1(group);
		group1.direction = 180;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that if the direction is not reported, its value does not affect the comparison
		metaf::WindGroup group;
		group.direction = 0;
		group.directionReported = false;
		group.speed = 12;
		group.speedReported = true;
		group.gustSpeed = 20;
		group.gustSpeedReported = true;
		group.unit = metaf::WindGroup::Unit::METERS_PER_SECOND;
		metaf::WindGroup group1(group);
		group1.direction = 180;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that if wind speed is not reported, its value does not affect the comparison
		metaf::WindGroup group;
		group.direction = 120;
		group.directionReported = true;
		group.speed = 12;
		group.speedReported = false;
		group.gustSpeed = 20;
		group.gustSpeedReported = true;
		group.unit = metaf::WindGroup::Unit::METERS_PER_SECOND;
		metaf::WindGroup group1(group);
		group1.speed = 20;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that if gust speed is not reported, its value does not affect the comparison
		metaf::WindGroup group;
		group.direction = 120;
		group.directionReported = true;
		group.speed = 12;
		group.speedReported = true;
		group.gustSpeed = 20;
		group.gustSpeedReported = false;
		group.unit = metaf::WindGroup::Unit::METERS_PER_SECOND;
		metaf::WindGroup group1(group);
		group1.gustSpeed = 30;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that groups holding the different data (direction, speed, gust 
		//speed, unit) are not equal
		metaf::WindGroup group;
		group.direction = 120;
		group.directionReported = true;
		group.directionVariable = false;
		group.speed = 12;
		group.speedReported = true;
		group.gustSpeed = 20;
		group.gustSpeedReported = true;
		group.unit = metaf::WindGroup::Unit::METERS_PER_SECOND;
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
		group8.unit = metaf::WindGroup::Unit::KNOTS;
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
		TEST_ASSERT(!(group == group5));
		TEST_ASSERT(!(group == group6));
		TEST_ASSERT(!(group == group7));
		TEST_ASSERT(!(group == group8));
	}
	{
		//Confirm that 'not reported' constructor initialises the group with correct data
		static const auto unit = metaf::WindGroup::Unit::KNOTS;
		const metaf::WindGroup group(unit);
		metaf::WindGroup group1;
		group1.unit = unit;
 		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that 'reported' constructor initialises the group with correct data
		static const auto direction = 240;
		static const auto unit = metaf::WindGroup::Unit::KNOTS;
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
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that 'makeVariableDirection' named constructor initialises the group 
		//with correct data
		static const auto unit = metaf::WindGroup::Unit::KNOTS;
		static const auto speed = 5;
		static const auto gustSpeed = 12;
		const metaf::WindGroup group = 
			metaf::WindGroup::makeVariableDirection(unit, speed, gustSpeed);
		metaf::WindGroup group1;
		group1.unit = metaf::WindGroup::Unit::KNOTS;
		group1.speed = speed;
		group1.gustSpeed = gustSpeed;
		group1.directionReported = true;
		group1.directionVariable = true;
		group1.speedReported = true;
		group1.gustSpeedReported = true;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testVarWindGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::VarWindGroup group1;
		const metaf::VarWindGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal 
		static const auto directionFrom = 60;
		static const auto directionTo = 120;
		metaf::VarWindGroup group1;
		metaf::VarWindGroup group2;
		group1.directionFrom = directionFrom;
		group1.directionTo = directionTo;
		group2.directionFrom = directionFrom;
		group2.directionTo = directionTo;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the different values are not equal 
		metaf::VarWindGroup group;
		group.directionFrom = 60;
		group.directionTo = 120;
		metaf::VarWindGroup group1(group);
		group1.directionFrom = 80;
		metaf::VarWindGroup group2(group);
		group2.directionTo = 180;
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
	}
	{
		//Confirm that constructor initialises the group with correct data
		static const auto directionFrom = 120;
		static const auto directionTo = 150;
		const metaf::VarWindGroup group(directionFrom, directionTo);
		metaf::VarWindGroup group1;
		group1.directionFrom = directionFrom;
		group1.directionTo = directionTo;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testWindShearGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::WindShearGroup group1;
		const metaf::WindShearGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal 
		static const auto direction = 240;
		static const auto height = 4000;
		static const auto speed = 20;
		static const auto speedUnit = metaf::WindShearGroup::SpeedUnit::KNOTS;
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the different values are not equal 
		metaf::WindShearGroup group;
		group.direction = 240;
		group.height = 4000;
		group.speed = 20;
		group.speedUnit = metaf::WindShearGroup::SpeedUnit::KNOTS;
		metaf::WindShearGroup group1(group);
		group1.direction = 180;
		metaf::WindShearGroup group2(group);
		group2.height = 6000;
		metaf::WindShearGroup group3(group);
		group3.speed = 22;
		metaf::WindShearGroup group4(group);
		group4.speedUnit = metaf::WindShearGroup::SpeedUnit::METERS_PER_SECOND;
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
	}
	{
		//Confirm that constructor initialises the group with correct data
		static const auto direction = 240;
		static const auto height = 4000;
		static const auto speed = 20;
		static const auto speedUnit = metaf::WindShearGroup::SpeedUnit::KNOTS;
		const metaf::WindShearGroup group(height, direction, speed, speedUnit);
		metaf::WindShearGroup group1;
		group1.height = height;
		group1.direction = direction;
		group1.speed = speed;
		group1.speedUnit = speedUnit;
	}
	TEST_END();
}

void test::TestGroup::testVisibilityGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::VisibilityGroup group1;
		const metaf::VisibilityGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal 
		static const auto unit = metaf::VisibilityGroup::Unit::STATUTE_MILES;
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if visibility is not reported, 
		//the following fields do NOT affect the comparison result:
		//incompleteInteger, lessThan, integer, numerator, denominator
		metaf::VisibilityGroup group;
		group.unit = metaf::VisibilityGroup::Unit::METERS;
		group.direction = metaf::VisibilityGroup::Direction::NDV;
		metaf::VisibilityGroup group1(group);
		group1.incompleteInteger = true;
		metaf::VisibilityGroup group2(group);
		group2.lessThan = true;
		metaf::VisibilityGroup group3(group);
		group3.integer = 1;
		metaf::VisibilityGroup group4(group);
		group4.numerator = 1;
		metaf::VisibilityGroup group5(group);
		group5.denominator = 1;
		TEST_ASSERT(group == group1);
		TEST_ASSERT(group == group2);
		TEST_ASSERT(group == group3);
		TEST_ASSERT(group == group4);
		TEST_ASSERT(group == group5);
	}
	{
		//Confirm that if visibility is not reported, unit and direction DO affect 
		//the comparison result
		metaf::VisibilityGroup group;
		group.unit = metaf::VisibilityGroup::Unit::METERS;
		group.direction = metaf::VisibilityGroup::Direction::NDV;
		metaf::VisibilityGroup group1(group);
		group1.unit = metaf::VisibilityGroup::Unit::STATUTE_MILES;
		metaf::VisibilityGroup group2(group);
		group2.direction = metaf::VisibilityGroup::Direction::NONE;
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
	}
	{
		//Confirm that if visibility is reported but incomplete, lessThan, numerator, 
		//and denominator do NOT affect the comparison results
		metaf::VisibilityGroup group;
		group.unit = metaf::VisibilityGroup::Unit::STATUTE_MILES;
		group.direction = metaf::VisibilityGroup::Direction::NONE;
		group.reported = true;
		group.integer = 1;
		group.incompleteInteger = true;
		metaf::VisibilityGroup group1(group);
		group1.lessThan = true;
		metaf::VisibilityGroup group2(group);
		group2.numerator = 1;
		metaf::VisibilityGroup group3(group);
		group3.denominator = 1;
		TEST_ASSERT(group == group1);
		TEST_ASSERT(group == group2);
		TEST_ASSERT(group == group3);
	}
	{
		//Confirm that if visibility is reported but incomplete, integer DOES affect
		//the comparison result
		metaf::VisibilityGroup group;
		group.unit = metaf::VisibilityGroup::Unit::STATUTE_MILES;
		group.direction = metaf::VisibilityGroup::Direction::NONE;
		group.reported = true;
		group.integer = 1;
		group.incompleteInteger = true;
		metaf::VisibilityGroup group1(group);
		group1.integer = 2;
		TEST_ASSERT(!(group == group1));
	}
	{
		//Confirm that if visibility is reported and complete, all fields affect comparison result
		metaf::VisibilityGroup group;
		group.unit = metaf::VisibilityGroup::Unit::STATUTE_MILES;
		group.direction = metaf::VisibilityGroup::Direction::NONE;
		group.reported = true;
		group.integer = 1;
		group.numerator = 3;
		group.denominator = 4;
		metaf::VisibilityGroup group1(group);
		group1.unit = metaf::VisibilityGroup::Unit::METERS;
		metaf::VisibilityGroup group2(group);
		group2.direction = metaf::VisibilityGroup::Direction::NW;
		metaf::VisibilityGroup group3(group);
		group3.reported = false;
		metaf::VisibilityGroup group4(group);
		group4.lessThan = true;
		metaf::VisibilityGroup group5(group);
		group5.incompleteInteger = true;
		metaf::VisibilityGroup group6(group);
		group6.integer = 2;
		metaf::VisibilityGroup group7(group);
		group7.numerator = 2;
		metaf::VisibilityGroup group8(group);
		group8.denominator = 2;
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
		TEST_ASSERT(!(group == group5));
		TEST_ASSERT(!(group == group6));
		TEST_ASSERT(!(group == group7));
		TEST_ASSERT(!(group == group8));
	}
	{
		//Confirm that 'not reported' version of makeVisiblityMeters 
		//initialises the group with correct data
		static const auto direction = metaf::VisibilityGroup::Direction::NDV;
		const metaf::VisibilityGroup group = 
			metaf::VisibilityGroup::makeVisibilityMeters(direction);
		metaf::VisibilityGroup group1;
		group1.unit = metaf::VisibilityGroup::Unit::METERS;
		group1.reported = false;
		group1.direction = direction;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that 'reported' version of makeVisiblityMeters 
		//initialises the group with correct data
		static const auto direction = metaf::VisibilityGroup::Direction::NW;
		static const auto visibility = 2000;
		const metaf::VisibilityGroup group = 
			metaf::VisibilityGroup::makeVisibilityMeters(visibility, direction);
		metaf::VisibilityGroup group1;
		group1.unit = metaf::VisibilityGroup::Unit::METERS;
		group1.reported = true;
		group1.integer = visibility;
		group1.direction = direction;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that 'not reported' version of makeVisiblityMiles
		//initialises the group with correct data
		const metaf::VisibilityGroup group = metaf::VisibilityGroup::makeVisibilityMiles();
		metaf::VisibilityGroup group1;
		group1.reported = false;
		group1.direction = metaf::VisibilityGroup::Direction::NONE;
		group1.unit = metaf::VisibilityGroup::Unit::STATUTE_MILES;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that 'reported' version of makeVisiblityMiles
		//initialises the group with correct data
		static const auto integer = 0;
		static const auto numerator = 1;
		static const auto denominator = 4;
		static const auto lessThan = true;
		const metaf::VisibilityGroup group = metaf::VisibilityGroup::makeVisibilityMiles(
			integer, 
			numerator, 
			denominator, 
			lessThan);
		metaf::VisibilityGroup group1;
		group1.direction = metaf::VisibilityGroup::Direction::NONE;
		group1.unit = metaf::VisibilityGroup::Unit::STATUTE_MILES;
		group1.reported = true;
		group1.integer = integer;
		group1.numerator = numerator;
		group1.denominator = denominator;
		group1.lessThan = lessThan;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that makeVisiblityMilesIncomplete initialises the group 
		//with correct data
		static const auto integer = 1;
		const metaf::VisibilityGroup group = 
			metaf::VisibilityGroup::makeVisibilityMilesIncomplete(integer);
		metaf::VisibilityGroup group1;
		group1.direction = metaf::VisibilityGroup::Direction::NONE;
		group1.unit = metaf::VisibilityGroup::Unit::STATUTE_MILES;
		group1.reported = true;
		group1.integer = integer;
		group1.incompleteInteger = true;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that incomplete integer group correctly merges following fraction group
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
		TEST_ASSERT(result);
		TEST_ASSERT(mergeGroup == completeGroup);
	}
	{
		//Confirm that fraction group before incomplete integer (wrong order, 
		//e.g 1/2 1 instead of 1 1/2) groups do not merge
		static const auto integer = 1;
		static const auto numerator = 3;
		static const auto denominator = 4;
		const metaf::VisibilityGroup incompleteIntegerGroup = 
			metaf::VisibilityGroup::makeVisibilityMilesIncomplete(integer);
		const metaf::VisibilityGroup fractionGroup = 
			metaf::VisibilityGroup::makeVisibilityMiles(0, numerator, denominator);
		metaf::VisibilityGroup mergeGroup(fractionGroup);
		bool result = mergeGroup.nextGroup(incompleteIntegerGroup);
		TEST_ASSERT(!result);
		TEST_ASSERT(mergeGroup == fractionGroup);
	}
	{
		//Confirm that incomplete integer group does not merge the following groups:
		// - Not reported visibility
		// - Visibility in meters
		// - Visibility in miles with non-zero integer
		// - Visibility in miles with zero numerator
		// - Visibility in miles with zero denominator
		// - Visibility in miles with lessThan flag
		// - Another incomplete visibility group
		const metaf::VisibilityGroup incompleteIntegerGroup = 
			metaf::VisibilityGroup::makeVisibilityMilesIncomplete(1);
		const metaf::VisibilityGroup notReportedVisibility = 
			metaf::VisibilityGroup::makeVisibilityMiles();
		const metaf::VisibilityGroup visibilityMeters = 
			metaf::VisibilityGroup::makeVisibilityMeters(1000);
		const metaf::VisibilityGroup visibilityMilesInteger = 
			metaf::VisibilityGroup::makeVisibilityMiles(1);
		const metaf::VisibilityGroup visibilityMilesZeroNumerator = 
			metaf::VisibilityGroup::makeVisibilityMiles(0, 0, 2);
		const metaf::VisibilityGroup visibilityMilesZeroDenominator = 
			metaf::VisibilityGroup::makeVisibilityMiles(0, 1, 0);
		const metaf::VisibilityGroup visibilityMilesLessThan = 
			metaf::VisibilityGroup::makeVisibilityMiles(0, 1, 4, true);
			{
				metaf::VisibilityGroup mergeGroup(incompleteIntegerGroup);
				TEST_ASSERT(!mergeGroup.nextGroup(notReportedVisibility));
				TEST_ASSERT(mergeGroup == incompleteIntegerGroup);
			}
			{
				metaf::VisibilityGroup mergeGroup(incompleteIntegerGroup);
				TEST_ASSERT(!mergeGroup.nextGroup(visibilityMeters));
				TEST_ASSERT(mergeGroup == incompleteIntegerGroup);

			}
			{
				metaf::VisibilityGroup mergeGroup(incompleteIntegerGroup);
				TEST_ASSERT(!mergeGroup.nextGroup(visibilityMilesInteger));
				TEST_ASSERT(mergeGroup == incompleteIntegerGroup);

			}
			{
				metaf::VisibilityGroup mergeGroup(incompleteIntegerGroup);
				TEST_ASSERT(!mergeGroup.nextGroup(visibilityMilesZeroNumerator));
				TEST_ASSERT(mergeGroup == incompleteIntegerGroup);
			}
			{
				metaf::VisibilityGroup mergeGroup(incompleteIntegerGroup);
				TEST_ASSERT(!mergeGroup.nextGroup(visibilityMilesZeroDenominator));
				TEST_ASSERT(mergeGroup == incompleteIntegerGroup);
			}
			{
				metaf::VisibilityGroup mergeGroup(incompleteIntegerGroup);
				TEST_ASSERT(!mergeGroup.nextGroup(visibilityMilesLessThan));
				TEST_ASSERT(mergeGroup == incompleteIntegerGroup);
			}
			{
				metaf::VisibilityGroup mergeGroup(incompleteIntegerGroup);
				TEST_ASSERT(!mergeGroup.nextGroup(incompleteIntegerGroup));
				TEST_ASSERT(mergeGroup == incompleteIntegerGroup);
			}
	}
	{
		//Confirm that the following groups do not merge with fraction group: 
		// - Not reported visibility
		// - Visibility in meters
		// - Visibility in miles (integer)
		// - Visibility in miles (integer and fraction)
		// - Visibility in miles with lessThan flag
		const metaf::VisibilityGroup incompleteIntegerGroup = 
			metaf::VisibilityGroup::makeVisibilityMilesIncomplete(1);
		const metaf::VisibilityGroup notReportedVisibility = 
			metaf::VisibilityGroup::makeVisibilityMiles();
		const metaf::VisibilityGroup visibilityMeters = 
			metaf::VisibilityGroup::makeVisibilityMeters(1000);
		const metaf::VisibilityGroup visibilityMilesInteger = 
			metaf::VisibilityGroup::makeVisibilityMiles(1);
		const metaf::VisibilityGroup visibilityMilesIntegerFraction = 
			metaf::VisibilityGroup::makeVisibilityMiles(1, 3, 4);
		const metaf::VisibilityGroup visibilityMilesLessThan = 
			metaf::VisibilityGroup::makeVisibilityMiles(0, 1, 4, true);
			{
				metaf::VisibilityGroup mergeGroup(notReportedVisibility);
				TEST_ASSERT(!mergeGroup.nextGroup(incompleteIntegerGroup));
				TEST_ASSERT(mergeGroup == notReportedVisibility);
			}
			{
				metaf::VisibilityGroup mergeGroup(visibilityMeters);
				TEST_ASSERT(!mergeGroup.nextGroup(incompleteIntegerGroup));
				TEST_ASSERT(mergeGroup == visibilityMeters);
			}
			{
				metaf::VisibilityGroup mergeGroup(visibilityMilesInteger);
				TEST_ASSERT(!mergeGroup.nextGroup(incompleteIntegerGroup));
				TEST_ASSERT(mergeGroup == visibilityMilesInteger);
			}
			{
				metaf::VisibilityGroup mergeGroup(visibilityMilesIntegerFraction);
				TEST_ASSERT(!mergeGroup.nextGroup(incompleteIntegerGroup));
				TEST_ASSERT(mergeGroup == visibilityMilesIntegerFraction);
			}
			{
				metaf::VisibilityGroup mergeGroup(visibilityMilesLessThan);
				TEST_ASSERT(!mergeGroup.nextGroup(incompleteIntegerGroup));
				TEST_ASSERT(mergeGroup == visibilityMilesLessThan);
			}
	}
	TEST_END();
}

void test::TestGroup::testCloudGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::CloudGroup group1;
		const metaf::CloudGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if height is not reported, it does not affect the comparison result
		metaf::CloudGroup group;
		group.amount = metaf::CloudGroup::Amount::FEW;
		group.height = 2000;
		group.type = metaf::CloudGroup::Type::NONE;
		metaf::CloudGroup group1 (group);
		group1.height = 1000;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that groups holding the different values are not equal
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
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
	}
	{
		//Confirm that constructor with height initialises the group with correct data
		static const auto amount = metaf::CloudGroup::Amount::OVERCAST;
		static const auto height = 1000;
		static const auto type = metaf::CloudGroup::Type::NONE;
		const metaf::CloudGroup group(amount, height, type);
		metaf::CloudGroup group1;
		group1.amount = amount;
		group1.height = height;
		group1.heightReported = true;
		group1.type = type;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that constructor without height initialises the group with correct data
		static const auto amount = metaf::CloudGroup::Amount::OVERCAST;
		static const auto type = metaf::CloudGroup::Type::NONE;
		const metaf::CloudGroup group(amount, type);
		metaf::CloudGroup group1;
		group1.amount = amount;
		group1.type = type;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testVerticalVisibilityGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::VerticalVisibilityGroup group1;
		const metaf::VerticalVisibilityGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
		static const auto vertVisibility = 100;
		static const auto reported = true;
		metaf::VerticalVisibilityGroup group1;
		metaf::VerticalVisibilityGroup group2;
		group1.reported = reported;
		group1.vertVisibility = vertVisibility;
		group2.reported = reported;
		group2.vertVisibility = vertVisibility;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if visibility is not reported, it does NOT affect the comparison result
		metaf::VerticalVisibilityGroup group;
		group.vertVisibility = 100;
		metaf::VerticalVisibilityGroup group1(group);
		group1.vertVisibility = 0;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that groups holding the different values are not equal 
		metaf::VerticalVisibilityGroup group;
		group.reported = true;
		group.vertVisibility = 100;
		metaf::VerticalVisibilityGroup group1(group);
		group1.reported = false;
		metaf::VerticalVisibilityGroup group2(group);
		group2.vertVisibility = 0;
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
	}
	{
		//Confirm that constructor initialises the group with correct data
		static const auto vertVisibility = 700;
		const metaf::VerticalVisibilityGroup group(vertVisibility);
		metaf::VerticalVisibilityGroup group1;
		group1.vertVisibility = vertVisibility;
		group1.reported = true;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testWeatherGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::WeatherGroup group1;
		const metaf::WeatherGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
		static const auto modifier = metaf::WeatherGroup::Modifier::VICINITY;
		static const auto weatherSize = 3;
		static const auto weather1 = metaf::WeatherGroup::Weather::SHOWERS;
		static const auto weather2 = metaf::WeatherGroup::Weather::RAIN;
		static const auto weather3 = metaf::WeatherGroup::Weather::SNOW;
		metaf::WeatherGroup group1;
		metaf::WeatherGroup group2;
		group1.modifier = modifier;
		group1.weather[0] = weather1;
		group1.weather[1] = weather2;
		group1.weather[2] = weather3;
		group1.weatherSize = weatherSize;
		group2.modifier = modifier;
		group2.weather[0] = weather1;
		group2.weather[1] = weather2;
		group2.weather[2] = weather3;
		group2.weatherSize = weatherSize;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the different values are not equal
		metaf::WeatherGroup group;
		group.modifier = metaf::WeatherGroup::Modifier::VICINITY;
		group.weather[0] = metaf::WeatherGroup::Weather::SHOWERS;
		group.weather[1] = metaf::WeatherGroup::Weather::RAIN;
		group.weatherSize = 2;
		metaf::WeatherGroup group1(group);
		group1.modifier = metaf::WeatherGroup::Modifier::NONE;
		metaf::WeatherGroup group2(group);
		group2.weather[1] = metaf::WeatherGroup::Weather::SNOW;
		metaf::WeatherGroup group3(group);
		group3.weatherSize = 1;
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
	}
	{
		//Confirm that constructor from vector initialises the group with 
		//the correct data
		static const auto modifier = metaf::WeatherGroup::Modifier::VICINITY;
		const std::vector<metaf::WeatherGroup::Weather> weather = {
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::RAIN,
			metaf::WeatherGroup::Weather::SNOW,
		};
		const metaf::WeatherGroup group(modifier, weather);
		metaf::WeatherGroup group1;
		group1.modifier = modifier;
		TEST_ASSERT(weather.size() < metaf::WeatherGroup::maxWeatherSize);
		group1.weatherSize = weather.size();
		for (auto i=0; i<weather.size(); i++) {
			group1.weather[i] = weather.at(i);
		}
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that constructor from fixed list initialises the group 
		//with the correct data
		static const auto modifier = metaf::WeatherGroup::Modifier::VICINITY;
		static const auto weather1 = metaf::WeatherGroup::Weather::SHOWERS;
		static const auto weather2 = metaf::WeatherGroup::Weather::RAIN;
		const metaf::WeatherGroup group(modifier, weather1, weather2);
		metaf::WeatherGroup group1;
		group1.modifier = modifier;
		group1.weatherSize = 2;
		group1.weather[0] = weather1;
		group1.weather[1] = weather2;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that makeNotReported initialises the group with correct data
		static const auto recent = true;
		const metaf::WeatherGroup group = metaf::WeatherGroup::makeNotReported(true);
		metaf::WeatherGroup group1;
		group1.modifier = metaf::WeatherGroup::Modifier::NONE;
		if (recent) group1.modifier = metaf::WeatherGroup::Modifier::RECENT;
		group1.weatherSize = 1;
		group1.weather[0] = metaf::WeatherGroup::Weather::NOT_REPORTED;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testTemperatureGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::TemperatureGroup group1;
		const metaf::TemperatureGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if temperature or dewpoing are NOT reported, they do NOT affect 
		//the comparison result
		metaf::TemperatureGroup group;
		group.temperature = 2;
		group.dewPoint = -1;
		metaf::TemperatureGroup group1(group);
		group1.temperature = 1;
		group1.dewPoint = -2;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that groups holding the different values are not equal
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
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
	}
	{
		//Confirm that temperature-only constructor initialises the group 
		//with correct data
		static const auto temperature = 5;
		const metaf::TemperatureGroup group(temperature);
		metaf::TemperatureGroup group1;
		group1.temperature = temperature;
		group1.temperatureReported = true;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that temperature & dewpoint constructor initialises the 
		//group with correct data
		static const auto temperature = 5;
		static const auto dewPoint = 0;
		const metaf::TemperatureGroup group(temperature, dewPoint);
		metaf::TemperatureGroup group1;
		group1.temperature = temperature;
		group1.temperatureReported = true;
		group1.dewPoint = dewPoint;
		group1.dewPointReported = true;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testMinMaxTemperatureGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::MinMaxTemperatureGroup group1;
		const metaf::MinMaxTemperatureGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the different value are not equal
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
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
	}
	{
		//Confirm that makeMin initialises the group with correct data
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
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that makeMax initialises the group with correct data
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
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testPressureGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::PressureGroup group1;
		const metaf::PressureGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if pressure value is not reported, it does not affect the comparison result
		metaf::PressureGroup group;
		group.unit = metaf::PressureGroup::Unit::INCHES_HG;
		group.pressure = 30.31;
		metaf::PressureGroup group1(group);
		group1.pressure = 30.30;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that groups holding the different values are not equal
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
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
	}
	{
		//Confirm that 'not reported' constructor initialises the group 
		//with correct data
		static const auto unit = metaf::PressureGroup::Unit::INCHES_HG;
		const metaf::PressureGroup group(unit);
		metaf::PressureGroup group1;
		group1.unit = unit;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that constructor initialises the group with correct data
		static const auto unit = metaf::PressureGroup::Unit::INCHES_HG;
		static const auto pressure = 30.31;
		const metaf::PressureGroup group(pressure, unit);
		metaf::PressureGroup group1;
		group1.unit = unit;
		group1.pressure = pressure;
		group1.reported = true;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testRunwayVisualRangeGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::RunwayVisualRangeGroup group1;
		const metaf::RunwayVisualRangeGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same values are equal
		static const auto runwayNumber = 22;
		static const auto designator = metaf::RunwayVisualRangeGroup::Designator::NONE;
		static const auto visibility = 4500;
		static const auto visModifier = metaf::RunwayVisualRangeGroup::Modifier::NONE;
		static const auto varVisibility = 6500;
		static const auto varVisModifier = metaf::RunwayVisualRangeGroup::Modifier::NONE;
		static const auto visibilityReported = true;
		static const auto varVisibilityReported = true;
		static const auto unit = metaf::RunwayVisualRangeGroup::Unit::METERS;
		static const auto trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
		metaf::RunwayVisualRangeGroup group1;
		metaf::RunwayVisualRangeGroup group2;
		group1.runwayNumber = runwayNumber;
		group1.runwayDesignator = designator;
		group1.visibility = visibility;
		group1.visModifier = visModifier;
		group1.varVisibility = varVisibility;
		group1.varVisModifier = varVisModifier;
		group1.visibilityReported = visibilityReported;
		group1.varVisibilityReported = varVisibilityReported;
		group1.unit = unit;
		group1.trend = trend;
		group2.runwayNumber = runwayNumber;
		group2.runwayDesignator = designator;
		group2.visibility = visibility;
		group2.visModifier = visModifier;
		group2.varVisibility = varVisibility;
		group2.varVisModifier = varVisModifier;
		group2.visibilityReported = visibilityReported;
		group2.varVisibilityReported = varVisibilityReported;
		group2.unit = unit;
		group2.trend = trend;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if runway visibility is not reported, then visibility & modifier,
		//variable visibility & modifier, and variable visibility reported flag do not 
		//affect the comparison result
		metaf::RunwayVisualRangeGroup group;
		group.runwayNumber = 22;
		group.runwayDesignator = metaf::RunwayVisualRangeGroup::Designator::NONE;
		group.unit = metaf::RunwayVisualRangeGroup::Unit::METERS;
		group.trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
		group.visibilityReported = false;
		group.visibility = 4500;
		group.visModifier = metaf::RunwayVisualRangeGroup::Modifier::NONE;
		group.varVisibility = 6500;
		group.varVisModifier = metaf::RunwayVisualRangeGroup::Modifier::NONE;
		group.varVisibilityReported = true;
		metaf::RunwayVisualRangeGroup group1(group);
		group1.visibility = 4600;
		metaf::RunwayVisualRangeGroup group2(group);
		group2.visModifier = metaf::RunwayVisualRangeGroup::Modifier::LESS_THAN;
		metaf::RunwayVisualRangeGroup group3(group);
		group3.varVisibility = 6600;
		metaf::RunwayVisualRangeGroup group4(group);
		group4.varVisModifier = metaf::RunwayVisualRangeGroup::Modifier::MORE_THAN;
		metaf::RunwayVisualRangeGroup group5(group);
		group5.varVisibilityReported = false;
		TEST_ASSERT(group == group1);
		TEST_ASSERT(group == group2);
		TEST_ASSERT(group == group3);
		TEST_ASSERT(group == group4);
		TEST_ASSERT(group == group5);
	}
	{
		//Confirm that if variable visibility is not reported, then variable visibility 
		//value and modifier do not affect the comparison result
		metaf::RunwayVisualRangeGroup group;
		group.runwayNumber = 22;
		group.runwayDesignator = metaf::RunwayVisualRangeGroup::Designator::NONE;
		group.unit = metaf::RunwayVisualRangeGroup::Unit::METERS;
		group.visibilityReported = true;
		group.visibility = 4500;
		group.visModifier = metaf::RunwayVisualRangeGroup::Modifier::NONE;
		group.varVisibilityReported = false;
		group.varVisibility = 6500;
		group.varVisModifier = metaf::RunwayVisualRangeGroup::Modifier::NONE;
		group.unit = metaf::RunwayVisualRangeGroup::Unit::METERS;
		group.trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
		metaf::RunwayVisualRangeGroup group1(group);
		group1.varVisibility = 6500;
		metaf::RunwayVisualRangeGroup group2(group);
		group2.varVisModifier = metaf::RunwayVisualRangeGroup::Modifier::LESS_THAN;
		TEST_ASSERT(group == group1);
		TEST_ASSERT(group == group2);
	}
	{
		//Confirm that groups holding different values are not equal
		metaf::RunwayVisualRangeGroup group;
		group.runwayNumber = 22;
		group.runwayDesignator = metaf::RunwayVisualRangeGroup::Designator::NONE;
		group.visibility = 4500;
		group.visModifier = metaf::RunwayVisualRangeGroup::Modifier::NONE;
		group.varVisibility = 6500;
		group.varVisModifier = metaf::RunwayVisualRangeGroup::Modifier::NONE;
		group.visibilityReported = true;
		group.varVisibilityReported = true;
		group.unit = metaf::RunwayVisualRangeGroup::Unit::METERS;
		group.trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
		metaf::RunwayVisualRangeGroup group1(group);
		group1.runwayNumber = 21;
		metaf::RunwayVisualRangeGroup group2(group);
		group2.runwayDesignator = metaf::RunwayVisualRangeGroup::Designator::CENTER;
		metaf::RunwayVisualRangeGroup group3(group);
		group3.visibility = 4600;
		metaf::RunwayVisualRangeGroup group4(group);
		group4.visModifier = metaf::RunwayVisualRangeGroup::Modifier::LESS_THAN;
		metaf::RunwayVisualRangeGroup group5(group);
		group5.varVisibility = 6600;
		metaf::RunwayVisualRangeGroup group6(group);
		group6.varVisModifier = metaf::RunwayVisualRangeGroup::Modifier::MORE_THAN;
		metaf::RunwayVisualRangeGroup group7(group);
		group7.visibilityReported = false;
		metaf::RunwayVisualRangeGroup group8(group);
		group8.varVisibilityReported = false;
		metaf::RunwayVisualRangeGroup group9(group);
		group9.unit = metaf::RunwayVisualRangeGroup::Unit::FEET;
		metaf::RunwayVisualRangeGroup group10(group);
		group10.trend = metaf::RunwayVisualRangeGroup::Trend::DOWNWARD;
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
		TEST_ASSERT(!(group == group5));
		TEST_ASSERT(!(group == group6));
		TEST_ASSERT(!(group == group7));
		TEST_ASSERT(!(group == group8));
		TEST_ASSERT(!(group == group9));
		TEST_ASSERT(!(group == group10));
	}
	{
		//Confirm that 'not reported' constructor initialises the 
		//group with correct data
		static const auto runwayNumber = 22;
		static const auto designator = metaf::RunwayVisualRangeGroup::Designator::NONE;
		static const auto unit = metaf::RunwayVisualRangeGroup::Unit::METERS;
		static const auto trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
		metaf::RunwayVisualRangeGroup group(runwayNumber, designator, unit, trend);
		metaf::RunwayVisualRangeGroup group1;
		group1.runwayNumber = runwayNumber;
		group1.runwayDesignator = designator;
		group1.unit = unit;
		group1.trend = trend;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that 'single visibility value' constructor initialises
		//the group with correct data
		static const auto runwayNumber = 22;
		static const auto designator = metaf::RunwayVisualRangeGroup::Designator::NONE;
		static const auto visibility = 6500;
		static const auto modifier = metaf::RunwayVisualRangeGroup::Modifier::NONE;
		static const auto unit = metaf::RunwayVisualRangeGroup::Unit::METERS;
		static const auto trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
		metaf::RunwayVisualRangeGroup group(runwayNumber, 
			designator, 
			visibility, 
			unit, 
			modifier,
			trend);
		metaf::RunwayVisualRangeGroup group1;
		group1.runwayNumber = runwayNumber;
		group1.runwayDesignator = designator;
		group1.visibility = visibility;
		group1.visibilityReported = true;
		group1.visModifier = modifier;
		group1.unit = unit;
		group1.trend = trend;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that 'min/max visibility' constructor initialises the 
		//group with correct data
		static const auto runwayNumber = 22;
		static const auto designator = metaf::RunwayVisualRangeGroup::Designator::NONE;
		static const auto visibility = 4500;
		static const auto modifier = metaf::RunwayVisualRangeGroup::Modifier::LESS_THAN;
		static const auto varVisibility = 6500;
		static const auto varVisModifier = metaf::RunwayVisualRangeGroup::Modifier::MORE_THAN;
		static const auto unit = metaf::RunwayVisualRangeGroup::Unit::FEET;
		static const auto trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
		metaf::RunwayVisualRangeGroup group(runwayNumber, 
			designator, 
			visibility, 
			varVisibility,
			unit, 
			modifier,
			varVisModifier,
			trend);
		metaf::RunwayVisualRangeGroup group1;
		group1.runwayNumber = runwayNumber;
		group1.runwayDesignator = designator;
		group1.visibility = visibility;
		group1.visibilityReported = true;
		group1.visModifier = modifier;
		group1.varVisibility = varVisibility;
		group1.varVisibilityReported = true;
		group1.varVisModifier = varVisModifier;
		group1.unit = unit;
		group1.trend = trend;
		TEST_ASSERT(group == group1);
	}


	TEST_END();
}

void test::TestGroup::testRunwayStateGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::RunwayStateGroup group1;
		const metaf::RunwayStateGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
		static const auto runwayNumber = 22;
		static const auto designator = metaf::RunwayStateGroup::Designator::NONE;
		static const auto status = metaf::RunwayStateGroup::Status::NONE;
		static const auto deposits = metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES;
		static const auto contaminationExtent = metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT;
		static const metaf::RunwayStateGroup::DepositDepth dd(1);
		static const metaf::RunwayStateGroup::SurfaceFriction sf(0.65);
		metaf::RunwayStateGroup group1;
		metaf::RunwayStateGroup group2;
		group1.runwayNumber = runwayNumber;
		group1.runwayDesignator = designator;
		group1.status = status;
		group1.deposits = deposits;
		group1.contaminationExtent = contaminationExtent;
		group1.depositDepth = dd;
		group1.surfaceFriction = sf;
		group2.runwayNumber = runwayNumber;
		group2.runwayDesignator = designator;
		group2.status = status;
		group2.deposits = deposits;
		group2.contaminationExtent = contaminationExtent;
		group2.depositDepth = dd;
		group2.surfaceFriction = sf;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the different values are not equal
		static const metaf::RunwayStateGroup::DepositDepth dd(1);
		static const metaf::RunwayStateGroup::SurfaceFriction sf(0.65);
		metaf::RunwayStateGroup group;
		group.runwayNumber = 22;
		group.runwayDesignator = metaf::RunwayStateGroup::Designator::NONE;
		group.status = metaf::RunwayStateGroup::Status::NONE;
		group.deposits = metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES;
		group.contaminationExtent = metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT;
		group.depositDepth = dd;
		group.surfaceFriction = sf;
		metaf::RunwayStateGroup group1(group);
		group1.runwayNumber = 21;
		metaf::RunwayStateGroup group2(group);
		group2.runwayDesignator = metaf::RunwayStateGroup::Designator::CENTER;
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
	}
	{
		//Confirm that constructor initialises the group with correct data
		static const auto runwayNumber = 21;
		static const auto designator = metaf::RunwayStateGroup::Designator::CENTER;
		static const auto status = metaf::RunwayStateGroup::Status::NONE;
		static const auto deposits = metaf::RunwayStateGroup::Deposits::DRY_SNOW;
		static const auto extent = metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT;
		static const auto ddDepth = 5;
		static const auto sfBrakingAction = 
			metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
		const metaf::RunwayStateGroup group(
			runwayNumber,
			designator,
			status,
			deposits,
			extent,
			metaf::RunwayStateGroup::DepositDepth(ddDepth),
			metaf::RunwayStateGroup::SurfaceFriction(sfBrakingAction));
		metaf::RunwayStateGroup group1;
		group1.runwayNumber = runwayNumber;
		group1.runwayDesignator = designator;
		group1.status = status;
		group1.deposits = deposits;
		group1.contaminationExtent = extent;
		group1.depositDepth.status = metaf::RunwayStateGroup::DepositDepth::Status::REPORTED;
		group1.depositDepth.depth = ddDepth;
		group1.surfaceFriction.status = 
			metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED;
		group1.surfaceFriction.brakingAction = sfBrakingAction;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that makeSnoclo initialises the group with correct data
		static const auto runwayNumber = 21;
		static const auto designator = metaf::RunwayStateGroup::Designator::CENTER;
		const metaf::RunwayStateGroup group = 
			metaf::RunwayStateGroup::makeSnoclo(runwayNumber, designator);
		metaf::RunwayStateGroup group1;
		group1.runwayNumber = runwayNumber;
		group1.runwayDesignator = designator;
		group1.status = metaf::RunwayStateGroup::Status::SNOCLO;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that makeClrd initialises the group with correct data
		static const auto runwayNumber = 21;
		static const auto designator = metaf::RunwayStateGroup::Designator::CENTER;
		static const auto surfaceFriction = 0.67;
		const metaf::RunwayStateGroup group = metaf::RunwayStateGroup::makeClrd(
			runwayNumber,
			designator,
			metaf::RunwayStateGroup::SurfaceFriction(surfaceFriction));
		metaf::RunwayStateGroup group1;
		group1.runwayNumber = runwayNumber;
		group1.runwayDesignator = designator;
		group1.status = metaf::RunwayStateGroup::Status::CLRD;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testRunwayStateGroupDepositDepth() {
	TEST_BEGIN();
	{
		//Confirm that default-initialised structs are equal
		const metaf::RunwayStateGroup::DepositDepth dd1;
		const metaf::RunwayStateGroup::DepositDepth dd2;
		TEST_ASSERT(dd1 == dd2);
	}
	{
		//Confirm that structs holding the same value are equal
		static const auto status = metaf::RunwayStateGroup::DepositDepth::Status::REPORTED;
		static const auto depth = 10;
		metaf::RunwayStateGroup::DepositDepth dd1;
		metaf::RunwayStateGroup::DepositDepth dd2;
		dd1.status = status;
		dd1.depth = depth;
		dd2.status = status;
		dd2.depth = depth;
		TEST_ASSERT(dd1 == dd2);
	}
	{
		//Confirm that if depth is not reported, it does not affect the comparison result
		metaf::RunwayStateGroup::DepositDepth dd;
		dd.status = metaf::RunwayStateGroup::DepositDepth::Status::NOT_REPORTED;
		dd.depth = 10;
		metaf::RunwayStateGroup::DepositDepth dd1(dd);
		dd1.depth = 20;
		TEST_ASSERT(dd == dd1);
	}
	{
		//Confirm that structs holding the different values are not equal
		metaf::RunwayStateGroup::DepositDepth dd;
		dd.status = metaf::RunwayStateGroup::DepositDepth::Status::REPORTED;
		dd.depth = 10;
		metaf::RunwayStateGroup::DepositDepth dd1(dd);
		dd1.status = metaf::RunwayStateGroup::DepositDepth::Status::NOT_REPORTED;
		metaf::RunwayStateGroup::DepositDepth dd2(dd);
		dd2.depth = 20;
		TEST_ASSERT(!(dd == dd1));
		TEST_ASSERT(!(dd == dd2));
	}
	{
		//Confirm that constructor initialises the struct with correct data
		static const auto depth = 5;
		const metaf::RunwayStateGroup::DepositDepth dd(depth);
		metaf::RunwayStateGroup::DepositDepth dd1;
		dd1.status = metaf::RunwayStateGroup::DepositDepth::Status::REPORTED;
		dd1.depth = depth;
		TEST_ASSERT(dd == dd1);
	}
	{
		//Confirm that makeNotReported initialises the struct with correct data
		const metaf::RunwayStateGroup::DepositDepth dd = 
			metaf::RunwayStateGroup::DepositDepth::makeNotReported();
		metaf::RunwayStateGroup::DepositDepth dd1;
		dd1.status = metaf::RunwayStateGroup::DepositDepth::Status::NOT_REPORTED;
		TEST_ASSERT(dd == dd1);
	}
	{
		//Confirm that makeRunwayNotOperational initialises the struct with correct data
		const metaf::RunwayStateGroup::DepositDepth dd = 
			metaf::RunwayStateGroup::DepositDepth::makeRunwayNotOperational();
		metaf::RunwayStateGroup::DepositDepth dd1;
		dd1.status = metaf::RunwayStateGroup::DepositDepth::Status::RUNWAY_NOT_OPERATIONAL;
		TEST_ASSERT(dd == dd1);
	}
	TEST_END();
}

void test::TestGroup::testRunwayStateGroupSurfaceFriction() {
	TEST_BEGIN();
	{
		//Confirm that default-initialised structs are equal
		const metaf::RunwayStateGroup::SurfaceFriction sf1;
		const metaf::RunwayStateGroup::SurfaceFriction sf2;
		TEST_ASSERT(sf1 == sf2);
	}
	{
		//Confirm that if surface friction is reported, the braking action value 
		//does NOT affect the comparison result
		metaf::RunwayStateGroup::SurfaceFriction sf;
		sf.status = metaf::RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED;
		sf.coefficient = 0.65;
		sf.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
		metaf::RunwayStateGroup::SurfaceFriction sf1(sf);
		sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED;
		sf1.coefficient = 0.65;
		sf1.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
		TEST_ASSERT(sf == sf1);
	}
	{
		//Confirm that if surface friction is reported, the surface friction value 
		//DOES affect the comparison result
		metaf::RunwayStateGroup::SurfaceFriction sf;
		sf.status = metaf::RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED;
		sf.coefficient = 0.65;
		sf.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
		metaf::RunwayStateGroup::SurfaceFriction sf1(sf);
		sf1.coefficient = 0.66;
		TEST_ASSERT(!(sf == sf1));
	}
	{
		//Confirm that if braking action is reported, the surface friction value 
		//does NOT affect the comparison result
		metaf::RunwayStateGroup::SurfaceFriction sf1;
		metaf::RunwayStateGroup::SurfaceFriction sf2;
		sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED;
		sf1.coefficient = 0.64;
		sf1.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
		sf2.status = metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED;
		sf2.coefficient = 0.65;
		sf2.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
		TEST_ASSERT(sf1 == sf2);
	}
	{
		//Confirm that if braking action is reported, the braking action value 
		//DOES affect the comparison result
		metaf::RunwayStateGroup::SurfaceFriction sf;
		sf.status = metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED;
		sf.coefficient = 0.65;
		sf.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
		metaf::RunwayStateGroup::SurfaceFriction sf1(sf);
		sf1.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
		TEST_ASSERT(!(sf == sf1));
	}
	{
		//Confirm that if neither braking action nor surface friction are reported, these
		//values do not affect the comparison result
		metaf::RunwayStateGroup::SurfaceFriction sf;
		sf.status = metaf::RunwayStateGroup::SurfaceFriction::Status::NOT_REPORTED; 
		sf.coefficient = 0.64;
		sf.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
		metaf::RunwayStateGroup::SurfaceFriction sf1(sf);
		sf1.coefficient = 0.65;
		sf1.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
		TEST_ASSERT(sf == sf1);
	}
	{
		//Confirm that if the status is 'unreliable', then neither braking action nor
		//surface friction affect the comparison result
		metaf::RunwayStateGroup::SurfaceFriction sf;
		sf.status = metaf::RunwayStateGroup::SurfaceFriction::Status::UNRELIABLE; 
		sf.coefficient = 0.64;
		sf.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR;
		metaf::RunwayStateGroup::SurfaceFriction sf1(sf);
		sf1.coefficient = 0.65;
		sf1.brakingAction = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
		TEST_ASSERT(sf == sf1);
	}
	{
		//Confirm that braking action constructor initialises the group with correct data
		static const auto ba = metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::GOOD;
		const metaf::RunwayStateGroup::SurfaceFriction sf(ba);
		metaf::RunwayStateGroup::SurfaceFriction sf1;
		sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED;
		sf1.brakingAction = ba;
		TEST_ASSERT(sf == sf1);
	}
	{
		//Confirm that surface friction constructor initialises the group with correct data
		static const auto sfValue = 0.7;
		const metaf::RunwayStateGroup::SurfaceFriction sf(sfValue);
		metaf::RunwayStateGroup::SurfaceFriction sf1;
		sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED;
		sf1.coefficient = sfValue;
		TEST_ASSERT(sf == sf1);
	}
	{
		//Confirm that makeNotReported initialises the group with correct data
		const metaf::RunwayStateGroup::SurfaceFriction sf = 
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported();
		metaf::RunwayStateGroup::SurfaceFriction sf1;
		sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::NOT_REPORTED;
		TEST_ASSERT(sf == sf1);
	}
	{
		//Confirm that makeUnreliable initialises the group with correct data
		const metaf::RunwayStateGroup::SurfaceFriction sf = 
			metaf::RunwayStateGroup::SurfaceFriction::makeUnreliable();
		metaf::RunwayStateGroup::SurfaceFriction sf1;
		sf1.status = metaf::RunwayStateGroup::SurfaceFriction::Status::UNRELIABLE;
		TEST_ASSERT(sf == sf1);
	}

	TEST_END();
}

void test::TestGroup::testRainfallGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::RainfallGroup group1;
		const metaf::RainfallGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if 60-minute rainfall is not reported, it does not affect the comparison result
		metaf::RainfallGroup group;
		group.last10Minutes = 0.2;
		group.since9AM = 7.0;
		group.last60Minutes = 2.5;
		metaf::RainfallGroup group1(group);
		group.last60Minutes = 2.4;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that groups holding the different values are not equal
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
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
	}
	{
		//Confirm that short group constructor initialises the group with 
		//correct data
		static const auto last10Minutes = 0.2;
		static const auto since9AM = 7.0;
		metaf::RainfallGroup group(last10Minutes, since9AM);
		metaf::RainfallGroup group1;
		group1.last10Minutes = last10Minutes;
		group1.since9AM = since9AM;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that long group constructor initialises the group with 
		//correct data
		static const auto last10Minutes = 0.2;
		static const auto since9AM = 7.0;
		static const auto last60Minutes = 2.5;
		metaf::RainfallGroup group(last10Minutes, since9AM, last60Minutes);
		metaf::RainfallGroup group1;
		group1.last10Minutes = last10Minutes;
		group1.since9AM = since9AM;
		group1.last60Minutes = last60Minutes;
		group1.last60MinutesReported = true;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testSeaSurfaceGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::SeaSurfaceGroup group1;
		const metaf::SeaSurfaceGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if the surface temperature is not reported, 
		//it does not affect the comparison result
		metaf::SeaSurfaceGroup group;
		group.temperature = 7;
		group.stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
		metaf::SeaSurfaceGroup group1(group);
		group1.temperature = 6;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that groups holding the different values are not equal
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
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
	}
	{
		//Confirm that temperature & stateOfSurface constructor initialises the group 
		//with correct data
		static const auto temperature = 7;
		static const auto stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
		metaf::SeaSurfaceGroup group(temperature, stateOfSurface);
		metaf::SeaSurfaceGroup group1;
		group1.temperature = temperature;
		group1.temperatureReported = true;
		group1.stateOfSurface = stateOfSurface;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that stateOfSurface constructor initialises the group with correct 
		//data
		static const auto stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
		metaf::SeaSurfaceGroup group(stateOfSurface);
		metaf::SeaSurfaceGroup group1;
		group1.temperatureReported = false;
		group1.stateOfSurface = stateOfSurface;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testSeaWavesGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::SeaWavesGroup group1;
		const metaf::SeaWavesGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
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
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that if the surface temperature or wave height are not reported, 
		//they does not affect the comparison result
		metaf::SeaWavesGroup group;
		group.temperature = 7;
		group.waveHeight = 28;
		metaf::SeaWavesGroup group1(group);
		group.temperature = 8;
		metaf::SeaWavesGroup group2(group);
		group.waveHeight = 29;
		TEST_ASSERT(group == group1);
		TEST_ASSERT(group == group2);
	}
	{
		//Confirm that groups holding the different values are not equal
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
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
		TEST_ASSERT(!(group == group3));
		TEST_ASSERT(!(group == group4));
	}
	{
		//Confirm that constructor initialises the group with correct data
		static const auto temperature = 5;
		static const auto waveHeight = 12;
		const metaf::SeaWavesGroup group(temperature, waveHeight);
		metaf::SeaWavesGroup group1;
		group1.temperature = temperature;
		group1.temperatureReported = true;
		group1.waveHeight = waveHeight;
		group1.waveHeightReported = true;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that makeTemperature initialises the group with correct data
		static const auto temperature = 5;
		const metaf::SeaWavesGroup group = metaf::SeaWavesGroup::makeTemperature(temperature);
		metaf::SeaWavesGroup group1;
		group1.temperature = temperature;
		group1.temperatureReported = true;
		TEST_ASSERT(group == group1);
	}
	{
		//Confirm that makeWaveHeight initialises the group with correct data
		static const auto waveHeight = 12;
		const metaf::SeaWavesGroup group = metaf::SeaWavesGroup::makeWaveHeight(waveHeight);
		metaf::SeaWavesGroup group1;
		group1.waveHeight = waveHeight;
		group1.waveHeightReported = true;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testColourCodeGroup() {
	TEST_BEGIN();
	{
		//Confirm that default groups are equal
		const metaf::ColourCodeGroup group1;
		const metaf::ColourCodeGroup group2;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the same value are equal
		static const auto colourCode = metaf::ColourCodeGroup::Code::RED;
		static const auto codeBlack = true;
		metaf::ColourCodeGroup group1;
		metaf::ColourCodeGroup group2;
		group1.code = colourCode;
		group1.codeBlack = codeBlack;
		group2.code = colourCode;
		group2.codeBlack = codeBlack;
		TEST_ASSERT(group1 == group2);
	}
	{
		//Confirm that groups holding the different values are not equal
		metaf::ColourCodeGroup group;
		group.code = metaf::ColourCodeGroup::Code::RED;
		group.codeBlack = true;
		metaf::ColourCodeGroup group1(group);
		group1.code = metaf::ColourCodeGroup::Code::BLUE;
		metaf::ColourCodeGroup group2(group);
		group2.codeBlack = false;
		TEST_ASSERT(!(group == group1));
		TEST_ASSERT(!(group == group2));
	}
	{
		//Confirm that constructor initialises the group with correct data
		static const auto colourCode = metaf::ColourCodeGroup::Code::YELLOW1;
		static const auto codeBlack = true;
		const metaf::ColourCodeGroup group(colourCode, codeBlack);
		metaf::ColourCodeGroup group1;
		group1.code = colourCode;
		group1.codeBlack = codeBlack;
		TEST_ASSERT(group == group1);
	}
	TEST_END();
}

void test::TestGroup::testGetSyntaxGroup(){
	TEST_BEGIN();
	{
		//Confirm that passing ImmutableGroup of Type::METAF to getSyntaxGroup()
		//results in SyntaxGroup::METAR
		const metaf::Group 
			group = metaf::ImmutableGroup(metaf::ImmutableGroup::Type::METAR);
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::METAR);
	}
	{
		//Confirm that passing ImmutableGroup of Type::SPECI to getSyntaxGroup()
		//results in SyntaxGroup::SPECI
		const metaf::Group 
			group = metaf::ImmutableGroup(metaf::ImmutableGroup::Type::SPECI);
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::SPECI);
	}
	{
		//Confirm that passing ImmutableGroup of Type::TAF to getSyntaxGroup()
		//results in SyntaxGroup::TAF
		const metaf::Group 
			group = metaf::ImmutableGroup(metaf::ImmutableGroup::Type::TAF);
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::TAF);
	}
	{
		//Confirm that passing ImmutableGroup of Type::COR to getSyntaxGroup()
		//results in SyntaxGroup::COR
		const metaf::Group group = 
			metaf::ImmutableGroup(metaf::ImmutableGroup::Type::COR);
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::COR);
	}
	{
		//Confirm that passing ImmutableGroup of Type::AMD to getSyntaxGroup()
		//results in SyntaxGroup::AMD
		const metaf::Group group = 
			metaf::ImmutableGroup(metaf::ImmutableGroup::Type::AMD);
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::AMD);
	}
	{
		//Confirm that passing ImmutableGroup of Type::NIL to getSyntaxGroup()
		//results in SyntaxGroup::NIL
		const metaf::Group group = 
			metaf::ImmutableGroup(metaf::ImmutableGroup::Type::NIL);
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::NIL);
	}
	{
		//Confirm that passing ImmutableGroup of Type::CNL to getSyntaxGroup()
		//results in SyntaxGroup::CNL
		const metaf::Group group = 
			metaf::ImmutableGroup(metaf::ImmutableGroup::Type::CNL);
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::CNL);
	}
	{
		//Confirm that passing ImmutableGroup of Type::RMK to getSyntaxGroup()
		//results in SyntaxGroup::RMK
		const metaf::Group group = 
			metaf::ImmutableGroup(metaf::ImmutableGroup::Type::RMK);
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::RMK);
	}
	{
		//Confirm that passing LocationGroup to getSyntaxGroup() results in 
		//SyntaxGroup::LOCATION
		const metaf::Group group = metaf::LocationGroup("ABCD");
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::LOCATION);
	}
 	{
		//Confirm that passing ReportTimeGroup to getSyntaxGroup() results in 
		//SyntaxGroup::REPORT_TIME
		const metaf::Group group = metaf::ReportTimeGroup(8, 12, 30);
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::REPORT_TIME);
	}
	{
		//Confirm that passing TimeSpanGroup to getSyntaxGroup() results in 
		//SyntaxGroup::TIME_SPAN
		const metaf::Group group = metaf::TimeSpanGroup(8, 12, 8, 24);
		TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::TIME_SPAN);
	}
	{
		//Confirm that passing ImmutableGroup of the following types to getSyntaxGroup()
		//results in SyntaxGroup::OTHER
		// - AUTO
		// - CLR
		// - NCD
		// - NOSIG
		// - AIRPORT_SNOCLO
		// - CAVOK
		// - NSC
		// - NSW
		// - TEMPO
		// - BECMG
		// - INTER
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::AUTO);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::CLR);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::NCD);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::NOSIG);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::AIRPORT_SNOCLO);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::CAVOK);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::NSC);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::NSW);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::TEMPO);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::BECMG);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ImmutableGroup(metaf::ImmutableGroup::Type::INTER);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
	}
	{
		//Confirm that passing the following groups to getSyntaxGroup()
		//results in SyntaxGroup::OTHER
		// - PlainTextGroup
		// - TrendTimeGroup
		// - ProbabilityGroup
		// - WindGroup
		// - VarWindGroup
		// - WindShearGroup
		// - VisibilityGroup
		// - CloudGroup
		// - VerticalVisibilityGroup
		// - WeatherGroup
		// - TemperatureGroup
		// - MinMaxTemperatureGroup
		// - PressureGroup
		// - RunwayVisualRangeGroup
		// - RunwayStateGroup
		// - RainfallGroup
		// - SeaSurfaceGroup
		// - SeaWavesGroup
		// - ColourCodeGroup
		{
			const metaf::Group group = metaf::PlainTextGroup("A1B2C3D4");
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::TrendTimeGroup(metaf::TrendTimeGroup::Type::FROM, 11, 14, 00);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = metaf::ProbabilityGroup(30);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::WindGroup(240, metaf::WindGroup::Unit::KNOTS, 2, 7);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = metaf::VarWindGroup(180, 240);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::WindShearGroup(2200, 30, 45, metaf::WindShearGroup::SpeedUnit::KNOTS);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::VisibilityGroup::makeVisibilityMeters(6500);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::CloudGroup(metaf::CloudGroup::Amount::BROKEN, 2400);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = metaf::VerticalVisibilityGroup(1500);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::WeatherGroup(metaf::WeatherGroup::Modifier::NONE, 
					metaf::WeatherGroup::Weather::SHOWERS, 
					metaf::WeatherGroup::Weather::SNOW);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = metaf::TemperatureGroup(7, 2);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = metaf::MinMaxTemperatureGroup::makeMax(7, 15, 45);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::PressureGroup(1033, metaf::PressureGroup::Unit::HECTOPASCAL);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::RunwayVisualRangeGroup(5, 
					metaf::RunwayVisualRangeGroup::Designator::NONE,
					1500, 
					metaf::RunwayVisualRangeGroup::Unit::METERS);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::RunwayStateGroup(16, metaf::RunwayStateGroup::Designator::RIGHT,
					metaf::RunwayStateGroup::Status::NONE, 
					metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES, 
					metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT,
					metaf::RunwayStateGroup::DepositDepth(1),
					metaf::RunwayStateGroup::SurfaceFriction(0.55));
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = metaf::RainfallGroup(0.0, 0.0);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::SeaSurfaceGroup(15, metaf::SeaSurfaceGroup::StateOfSurface::MODERATE);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = metaf::SeaWavesGroup(9, 15);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
		{
			const metaf::Group group = 
				metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::AMBER);
			TEST_ASSERT(metaf::getSyntaxGroup(group) == metaf::SyntaxGroup::OTHER);
		}
	}
	TEST_END();
}