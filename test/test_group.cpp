/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////

//Confirm that Group occupies a contiguous memory area
TEST(Group, is_standard_layout) {
    EXPECT_TRUE(std::is_standard_layout<metaf::Group>::value);
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
	EXPECT_FALSE(strcmp(group.text, testStr));
}

///////////////////////////////////////////////////////////////////////////////

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
	EXPECT_EQ(group.type, type);
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
	EXPECT_FALSE(strcmp(group.location, testLocation));
}

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
	static const auto probability = 30u;
	const metaf::ProbabilityGroup group(probability);
	EXPECT_EQ(group.probability, probability);
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
	static const auto directionFrom = 60u;
	static const auto directionTo = 120u;
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
	static const auto directionFrom = 120u;
	static const auto directionTo = 150u;
	const metaf::VarWindGroup group(directionFrom, directionTo);
	EXPECT_EQ(group.directionFrom, directionFrom);
	EXPECT_EQ(group.directionTo, directionTo);
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
	EXPECT_EQ(group.unit, metaf::DistanceUnit::METERS);
	EXPECT_FALSE(group.reported);
	EXPECT_EQ(group.direction, direction);
}

//Confirm that 'reported' version of makeVisibilityMeters initialises the group 
//with correct data
TEST(VisibilityGroup, makeVisibilityMeters2) {
	static const auto direction = metaf::VisibilityGroup::Direction::NW;
	static const auto visibility = 2000u;
	static const auto modifier = metaf::ValueModifier::MORE_THAN;
	const metaf::VisibilityGroup group1 = 
		metaf::VisibilityGroup::makeVisibilityMeters(visibility, direction);
	EXPECT_EQ(group1.unit, metaf::DistanceUnit::METERS);
	EXPECT_TRUE(group1.reported);
	EXPECT_EQ(group1.integer, visibility);
	EXPECT_EQ(group1.numerator, 0u);
	EXPECT_EQ(group1.denominator, 0u);
	EXPECT_EQ(group1.direction, direction);
	EXPECT_EQ(group1.modifier, metaf::ValueModifier::NONE);
	EXPECT_FALSE(group1.incompleteInteger);
	const metaf::VisibilityGroup group2 = 
		metaf::VisibilityGroup::makeVisibilityMeters(visibility, direction, modifier);
	EXPECT_EQ(group2.unit, metaf::DistanceUnit::METERS);
	EXPECT_TRUE(group2.reported);
	EXPECT_EQ(group2.integer, visibility);
	EXPECT_EQ(group2.numerator, 0u);
	EXPECT_EQ(group2.denominator, 0u);
	EXPECT_EQ(group2.direction, direction);
	EXPECT_EQ(group2.modifier, metaf::ValueModifier::MORE_THAN);
	EXPECT_FALSE(group2.incompleteInteger);
}

//Confirm that 'not reported' version of makeVisibilityMiles initialises the 
//group with correct data
TEST(VisibilityGroup, makeVisibilityMiles1) {
	const metaf::VisibilityGroup group = metaf::VisibilityGroup::makeVisibilityMiles();
	EXPECT_FALSE(group.reported);
	EXPECT_EQ(group.direction, metaf::VisibilityGroup::Direction::NONE);
	EXPECT_EQ(group.unit, metaf::DistanceUnit::STATUTE_MILES);
}

//Confirm that 'reported' version of makeVisiblityMiles initialises the group 
//with correct data
TEST(VisibilityGroup, makeVisibilityMiles2) {
	static const auto integer = 2u;
	static const auto numerator = 1u;
	static const auto denominator = 4u;
	static const auto modifier = metaf::ValueModifier::LESS_THAN;
	const metaf::VisibilityGroup group = metaf::VisibilityGroup::makeVisibilityMiles(
		integer, 
		numerator, 
		denominator, 
		modifier);
	EXPECT_EQ(group.direction, metaf::VisibilityGroup::Direction::NONE);
	EXPECT_EQ(group.unit, metaf::DistanceUnit::STATUTE_MILES);
	EXPECT_TRUE(group.reported);
	EXPECT_EQ(group.integer, integer);
	EXPECT_EQ(group.numerator, numerator);
	EXPECT_EQ(group.denominator, denominator);
	EXPECT_EQ(group.modifier, metaf::ValueModifier::LESS_THAN);
	EXPECT_FALSE(group.incompleteInteger);
}

//Confirm that makeVisiblityMilesIncomplete initialises the group with correct 
//data
TEST(VisibilityGroup, makeVisibilityMilesIncomplete) {
	static const auto integer = 1u;
	const metaf::VisibilityGroup group = 
		metaf::VisibilityGroup::makeVisibilityMilesIncomplete(integer);
	EXPECT_EQ(group.direction, metaf::VisibilityGroup::Direction::NONE);
	EXPECT_EQ(group.unit, metaf::DistanceUnit::STATUTE_MILES);
	EXPECT_TRUE(group.reported);
	EXPECT_EQ(group.integer, integer);
	EXPECT_EQ(group.numerator, 0u);
	EXPECT_EQ(group.denominator, 0u);
	EXPECT_EQ(group.modifier, metaf::ValueModifier::NONE);
	EXPECT_TRUE(group.incompleteInteger);
}

//Confirm that incomplete integer group correctly merges following fraction group
TEST(VisibilityGroup, mergeVisibilityGroups1) {
	static const auto integer = 1u;
	static const auto numerator = 3u;
	static const auto denominator = 4u;
	const metaf::VisibilityGroup completeGroup = 
		metaf::VisibilityGroup::makeVisibilityMiles(integer, numerator, denominator);
	const metaf::VisibilityGroup incompleteIntegerGroup = 
		metaf::VisibilityGroup::makeVisibilityMilesIncomplete(integer);
	const metaf::VisibilityGroup fractionGroup = 
		metaf::VisibilityGroup::makeVisibilityMiles(0u, numerator, denominator);
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

//Confirm that isValid would not validate groups with unknown distance units
TEST(VisibilityGroup, isValidVisibilityUnknownUnits) {
	metaf::VisibilityGroup invalidGroup1; //The unit is unknown
	invalidGroup1.reported = false;
	invalidGroup1.unit = metaf::DistanceUnit::UNKNOWN;
	EXPECT_FALSE(invalidGroup1.isValid());
}

//Confirm that isValid correctly checks validity of valid struct when the 
//visibility measurement unit is meters
TEST(VisibilityGroup, isValidVisibilityMetersValid) {
	//Not reported, direction unknown
	metaf::VisibilityGroup validGroup1; 
	validGroup1.unit = metaf::DistanceUnit::METERS;
	validGroup1.direction = metaf::VisibilityGroup::Direction::UNKNOWN;
	validGroup1.reported = false;
	EXPECT_TRUE(validGroup1.isValid());
	//Not reported, no direction
	metaf::VisibilityGroup validGroup2; 
	validGroup2.unit = metaf::DistanceUnit::METERS;
	validGroup2.direction = metaf::VisibilityGroup::Direction::NONE;
	EXPECT_TRUE(validGroup2.isValid());
	//Not reported, no directional visibility
	metaf::VisibilityGroup validGroup3; 
	validGroup3.unit = metaf::DistanceUnit::METERS;
	validGroup3.direction = metaf::VisibilityGroup::Direction::NDV;
	EXPECT_TRUE(validGroup3.isValid());
	//Not reported, no direction, modifier is NONE
	metaf::VisibilityGroup validGroup4; 
	validGroup4.unit = metaf::DistanceUnit::METERS;
	validGroup4.direction = metaf::VisibilityGroup::Direction::NONE;
	validGroup4.modifier = metaf::ValueModifier::NONE;
	EXPECT_TRUE(validGroup4.isValid());
	//Not reported, no direction, modifier is not NONE
	metaf::VisibilityGroup validGroup5; 
	validGroup5.unit = metaf::DistanceUnit::METERS;
	validGroup5.direction = metaf::VisibilityGroup::Direction::NONE;
	validGroup5.reported = false;
	validGroup5.modifier = metaf::ValueModifier::UNKNOWN;
	EXPECT_TRUE(validGroup5.isValid());
	//Reported, no direction
	metaf::VisibilityGroup validGroup6;
	validGroup6.unit = metaf::DistanceUnit::METERS;
	validGroup6.direction = metaf::VisibilityGroup::Direction::NONE;
	validGroup6.reported = true;
	validGroup6.integer = 4000;
	EXPECT_TRUE(validGroup6.isValid());
	//Reported, direction specified
	metaf::VisibilityGroup validGroup7; 
	validGroup7.unit = metaf::DistanceUnit::METERS;
	validGroup7.direction = metaf::VisibilityGroup::Direction::NW;
	validGroup7.reported = true;
	validGroup7.integer = 4000;
	EXPECT_TRUE(validGroup7.isValid());
	//Visibility more than 10 km
	metaf::VisibilityGroup validGroup8; 
	validGroup8.unit = metaf::DistanceUnit::METERS;
	validGroup8.direction = metaf::VisibilityGroup::Direction::NONE;
	validGroup8.modifier = metaf::ValueModifier::MORE_THAN;
	validGroup8.integer = 10000;
	validGroup8.reported = true;
	EXPECT_TRUE(validGroup8.isValid());
	//Visibility more than 10 km, no directional visibility
	metaf::VisibilityGroup validGroup9; 
	validGroup9.unit = metaf::DistanceUnit::METERS;
	validGroup9.direction = metaf::VisibilityGroup::Direction::NDV;
	validGroup9.modifier = metaf::ValueModifier::MORE_THAN;
	validGroup9.integer = 10000;
	validGroup9.reported = true;
	EXPECT_TRUE(validGroup9.isValid());
}

//Confirm that isValid correctly checks validity of valid struct when the 
//visibility measurement unit is meters
TEST(VisibilityGroup, isValidVisibilityMetersInvalid) {
	//Reported, no direction, modifier is LESS_THAN
	metaf::VisibilityGroup invalidGroup1; 
	invalidGroup1.unit = metaf::DistanceUnit::METERS;
	invalidGroup1.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup1.reported = true;
	invalidGroup1.integer = 6000;
	invalidGroup1.modifier = metaf::ValueModifier::LESS_THAN;
	EXPECT_FALSE(invalidGroup1.isValid());
	//Reported, incomplete integer
	metaf::VisibilityGroup invalidGroup2; 
	invalidGroup2.unit = metaf::DistanceUnit::METERS;
	invalidGroup2.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup2.reported = true;
	invalidGroup2.incompleteInteger = true;
	EXPECT_FALSE(invalidGroup2.isValid());
	//Reported, integer part is zero
	metaf::VisibilityGroup invalidGroup3; 
	invalidGroup3.unit = metaf::DistanceUnit::METERS;
	invalidGroup3.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup3.reported = true;
	invalidGroup3.integer = 0;
	EXPECT_FALSE(invalidGroup3.isValid());
	//Reported, numerator is non-zero
	metaf::VisibilityGroup invalidGroup4;
	invalidGroup4.unit = metaf::DistanceUnit::METERS;
	invalidGroup4.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup4.reported = true;
	invalidGroup4.integer = 6000;
	invalidGroup4.numerator = 1;
	EXPECT_FALSE(invalidGroup4.isValid());
	//Reported, denominator is non-zero
	metaf::VisibilityGroup invalidGroup5;
	invalidGroup5.unit = metaf::DistanceUnit::METERS;
	invalidGroup5.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup5.reported = true;
	invalidGroup5.integer = 6000;
	invalidGroup5.denominator = 1;
	EXPECT_FALSE(invalidGroup5.isValid());
}

//Confirm that isValid correctly checks validity of invalid struct when the 
//visibility measurement unit is statute miles
TEST(VisibilityGroup, isValidVisibilityMilesValid) {
	//Not reported, no direction
	metaf::VisibilityGroup validGroup1;
	validGroup1.unit = metaf::DistanceUnit::STATUTE_MILES;
	validGroup1.direction = metaf::VisibilityGroup::Direction::NONE;
	validGroup1.reported = false;
	EXPECT_TRUE(validGroup1.isValid());
	//Reported, integer only
	metaf::VisibilityGroup validGroup2;
	validGroup2.unit = metaf::DistanceUnit::STATUTE_MILES;
	validGroup2.direction = metaf::VisibilityGroup::Direction::NONE;
	validGroup2.reported = true;
	validGroup2.integer = 2;
	validGroup2.numerator = 0;
	validGroup2.denominator = 0;
	validGroup2.modifier = metaf::ValueModifier::NONE;
	EXPECT_TRUE(validGroup2.isValid());
	//Reported, numerator and denominator only
	metaf::VisibilityGroup validGroup3;
	validGroup3.unit = metaf::DistanceUnit::STATUTE_MILES;
	validGroup3.direction = metaf::VisibilityGroup::Direction::NONE;
	validGroup3.reported = true;
	validGroup3.integer = 0;
	validGroup3.numerator = 1;
	validGroup3.denominator = 8;
	validGroup3.modifier = metaf::ValueModifier::NONE;
	EXPECT_TRUE(validGroup3.isValid());
	//Reported integer, numerator and denominator, no modifier
	metaf::VisibilityGroup validGroup4;
	validGroup4.unit = metaf::DistanceUnit::STATUTE_MILES;
	validGroup4.direction = metaf::VisibilityGroup::Direction::NONE;
	validGroup4.reported = true;
	validGroup4.integer = 2;
	validGroup4.numerator = 1;
	validGroup4.denominator = 2;
	validGroup4.modifier = metaf::ValueModifier::NONE;
	EXPECT_TRUE(validGroup4.isValid());
	//Reported integer, numerator and denominator, modifier LESS_THAN
	metaf::VisibilityGroup validGroup5;
	validGroup5.unit = metaf::DistanceUnit::STATUTE_MILES;
	validGroup5.direction = metaf::VisibilityGroup::Direction::NONE;
	validGroup5.reported = true;
	validGroup5.integer = 2;
	validGroup5.numerator = 1;
	validGroup5.denominator = 2;
	validGroup5.modifier = metaf::ValueModifier::LESS_THAN;
	EXPECT_TRUE(validGroup5.isValid());
	//Reported integer, numerator and denominator, modifier MORE_THAN
	metaf::VisibilityGroup validGroup6;
	validGroup6.unit = metaf::DistanceUnit::STATUTE_MILES;
	validGroup6.direction = metaf::VisibilityGroup::Direction::NONE;
	validGroup6.reported = true;
	validGroup6.integer = 2;
	validGroup6.numerator = 1;
	validGroup6.denominator = 2;
	validGroup6.modifier = metaf::ValueModifier::LESS_THAN;
	EXPECT_TRUE(validGroup6.isValid());
}

//Confirm that isValid correctly checks validity of valid struct when the 
//visibility measurement unit is statute miles
TEST(VisibilityGroup, isValidVisibilityMilesInvalid) {
	//Not reported, direction unknown
	metaf::VisibilityGroup invalidGroup1;
	invalidGroup1.unit = metaf::DistanceUnit::STATUTE_MILES;
	invalidGroup1.direction = metaf::VisibilityGroup::Direction::UNKNOWN;
	invalidGroup1.reported = false;
	EXPECT_FALSE(invalidGroup1.isValid());
	//Not reported, direction specified
	metaf::VisibilityGroup invalidGroup2;
	invalidGroup2.unit = metaf::DistanceUnit::STATUTE_MILES;
	invalidGroup2.direction = metaf::VisibilityGroup::Direction::NW;
	invalidGroup2.reported = false;
	EXPECT_FALSE(invalidGroup2.isValid());
	//Not reported, direction NDV
	metaf::VisibilityGroup invalidGroup3;
	invalidGroup3.unit = metaf::DistanceUnit::STATUTE_MILES;
	invalidGroup3.direction = metaf::VisibilityGroup::Direction::NDV;
	invalidGroup3.reported = false;
	EXPECT_FALSE(invalidGroup3.isValid());
	//Reported, modifier unknown
	metaf::VisibilityGroup invalidGroup4;
	invalidGroup4.unit = metaf::DistanceUnit::STATUTE_MILES;
	invalidGroup4.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup4.reported = true;
	invalidGroup4.integer = 1;
	invalidGroup4.numerator = 1;
	invalidGroup4.denominator = 4;
	invalidGroup4.modifier = metaf::ValueModifier::UNKNOWN;
	EXPECT_FALSE(invalidGroup4.isValid());
	//Reported, numerator zero, denominator non-zero
	metaf::VisibilityGroup invalidGroup5;
	invalidGroup5.unit = metaf::DistanceUnit::STATUTE_MILES;
	invalidGroup5.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup5.reported = true;
	invalidGroup5.integer = 1;
	invalidGroup5.numerator = 0;
	invalidGroup5.denominator = 4;
	invalidGroup5.modifier = metaf::ValueModifier::NONE;
	EXPECT_FALSE(invalidGroup5.isValid());
	//Reported, numerator non-zero, denominator zero
	metaf::VisibilityGroup invalidGroup6;
	invalidGroup6.unit = metaf::DistanceUnit::STATUTE_MILES;
	invalidGroup6.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup6.reported = true;
	invalidGroup6.integer = 1;
	invalidGroup6.numerator = 1;
	invalidGroup6.denominator = 0;
	invalidGroup6.modifier = metaf::ValueModifier::NONE;
	EXPECT_FALSE(invalidGroup6.isValid());
	//Incomplete integer, integer zero
	metaf::VisibilityGroup invalidGroup7;
	invalidGroup7.unit = metaf::DistanceUnit::STATUTE_MILES;
	invalidGroup7.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup7.reported = true;
	invalidGroup7.integer = 0;
	invalidGroup7.numerator = 0;
	invalidGroup7.denominator = 0;
	invalidGroup7.incompleteInteger = true;
	invalidGroup7.modifier = metaf::ValueModifier::NONE;
	EXPECT_FALSE(invalidGroup7.isValid());
	//Incomplete integer, numerator non-zero
	metaf::VisibilityGroup invalidGroup8;
	invalidGroup8.unit = metaf::DistanceUnit::STATUTE_MILES;
	invalidGroup8.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup8.reported = true;
	invalidGroup8.integer = 2;
	invalidGroup8.numerator = 1;
	invalidGroup8.denominator = 0;
	invalidGroup8.incompleteInteger = true;
	invalidGroup8.modifier = metaf::ValueModifier::NONE;
	EXPECT_FALSE(invalidGroup8.isValid());
	//Incomplete integer, denominator non-zero
	metaf::VisibilityGroup invalidGroup9;
	invalidGroup9.unit = metaf::DistanceUnit::STATUTE_MILES;
	invalidGroup9.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroup9.reported = true;
	invalidGroup9.integer = 2;
	invalidGroup9.numerator = 0;
	invalidGroup9.denominator = 1;
	invalidGroup9.incompleteInteger = true;
	invalidGroup9.modifier = metaf::ValueModifier::NONE;
	EXPECT_FALSE(invalidGroup9.isValid());
	//Incomplete integer, modifier not NONE
	metaf::VisibilityGroup invalidGroupA;
	invalidGroupA.unit = metaf::DistanceUnit::STATUTE_MILES;
	invalidGroupA.direction = metaf::VisibilityGroup::Direction::NONE;
	invalidGroupA.reported = true;
	invalidGroupA.integer = 0;
	invalidGroupA.numerator = 1;
	invalidGroupA.denominator = 0;
	invalidGroupA.incompleteInteger = true;
	invalidGroupA.modifier = metaf::ValueModifier::LESS_THAN;
	EXPECT_FALSE(invalidGroupA.isValid());
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
	static const auto height = 1000u;
	static const auto type = metaf::CloudGroup::Type::NONE;
	const metaf::CloudGroup group(amount, height, type);
	EXPECT_EQ(group.amount, amount);
	EXPECT_EQ(group.height, height);
	EXPECT_TRUE(group.heightReported);
	EXPECT_EQ(group.type, type);
}

//Confirm that constructor without height initialises the group with correct data
TEST(CloudGroup, constructorHeightNotReported) {
	static const auto amount = metaf::CloudGroup::Amount::OVERCAST;
	static const auto type = metaf::CloudGroup::Type::NONE;
	const metaf::CloudGroup group(amount, type);
	EXPECT_EQ(group.amount, amount);
	EXPECT_FALSE(group.heightReported);
	EXPECT_EQ(group.type, type);
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
	static const auto vertVisibility = 700u;
	const metaf::VerticalVisibilityGroup group(vertVisibility);
	EXPECT_EQ(group.vertVisibility, vertVisibility);
	EXPECT_TRUE(group.reported);
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
	ASSERT_LT(weather.size(), metaf::WeatherGroup::maxWeatherSize);
	const metaf::WeatherGroup group(prefix, weather);
	EXPECT_EQ(group.prefix, prefix);
	EXPECT_EQ(group.weatherSize, weather.size());
	for (auto i=0u; i<weather.size(); i++) {
		EXPECT_EQ(group.weather[i], weather.at(i)) << "Index: " << i;
	}
}

//Confirm that constructor from fixed list initialises the group with the 
//correct data
TEST(WeatherGroup, constructorFixedList) {
	static const auto prefix = metaf::WeatherGroup::Prefix::VICINITY;
	static const auto weather1 = metaf::WeatherGroup::Weather::SHOWERS;
	static const auto weather2 = metaf::WeatherGroup::Weather::RAIN;
	const metaf::WeatherGroup group(prefix, weather1, weather2);
	EXPECT_EQ(group.prefix, prefix);
	EXPECT_EQ(group.weatherSize, 2u);
	EXPECT_EQ(group.weather[0], weather1);
	EXPECT_EQ(group.weather[1], weather2);
}

//Confirm that makeNotReported initialises the group with correct data
TEST(WeatherGroup, makeNotReported) {
	const metaf::WeatherGroup group1 = metaf::WeatherGroup::makeNotReported(true);
	EXPECT_EQ(group1.prefix, metaf::WeatherGroup::Prefix::RECENT);
	EXPECT_EQ(group1.weatherSize, 1u);
	EXPECT_EQ(group1.weather[0], metaf::WeatherGroup::Weather::NOT_REPORTED);

	const metaf::WeatherGroup group2 = metaf::WeatherGroup::makeNotReported(false);
	EXPECT_EQ(group2.prefix, metaf::WeatherGroup::Prefix::NONE);
	EXPECT_EQ(group2.weatherSize, 1u);
	EXPECT_EQ(group2.weather[0], metaf::WeatherGroup::Weather::NOT_REPORTED);
}

//Confirm that toVector() returns the correct data
TEST(WeatherGroup, toVectorNonEmpty) {
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
TEST(WeatherGroup, toVectorEmpty) {
	const metaf::WeatherGroup group;
	EXPECT_FALSE(group.weatherToVector().size());
}

class WeatherGroupIsAttribute : public ::testing::Test {
protected:
	//Simple precipitation groups
	const metaf::WeatherGroup precDrizzle =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
					metaf::WeatherGroup::Weather::DRIZZLE);
	const metaf::WeatherGroup precRain =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
					metaf::WeatherGroup::Weather::RAIN);
	const metaf::WeatherGroup precSnow =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
					metaf::WeatherGroup::Weather::SNOW);
	const metaf::WeatherGroup precSnowGrains =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
					metaf::WeatherGroup::Weather::SNOW_GRAINS);
	const metaf::WeatherGroup precIceCrystals =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
					metaf::WeatherGroup::Weather::ICE_CRYSTALS);
	const metaf::WeatherGroup precIcePellets =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
					metaf::WeatherGroup::Weather::ICE_PELLETS);
	const metaf::WeatherGroup precHail =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
					metaf::WeatherGroup::Weather::HAIL);
	const metaf::WeatherGroup precSmallHail =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
					metaf::WeatherGroup::Weather::SMALL_HAIL);
	const metaf::WeatherGroup precUndetermined =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
					metaf::WeatherGroup::Weather::UNDETERMINED);
	//Precipitation groups with prefix or descriptor
	const metaf::WeatherGroup precShowersInVicinity =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
					metaf::WeatherGroup::Weather::SHOWERS);
	const metaf::WeatherGroup precHeavySnowShowers =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
					metaf::WeatherGroup::Weather::SHOWERS,
					metaf::WeatherGroup::Weather::SNOW);
	//Mixed precipitation
	const metaf::WeatherGroup precRainAndSnowMix =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE,
					metaf::WeatherGroup::Weather::RAIN,
					metaf::WeatherGroup::Weather::SNOW);
	const metaf::WeatherGroup precThunderstormShowersRainSmallHail =
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE,
					std::vector<metaf::WeatherGroup::Weather> {
						metaf::WeatherGroup::Weather::THUNDERSTORM,
						metaf::WeatherGroup::Weather::SHOWERS,
						metaf::WeatherGroup::Weather::SMALL_HAIL,
						metaf::WeatherGroup::Weather::RAIN
					});
	//Simple obscuration groups
	const metaf::WeatherGroup obscMist = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::MIST);
	const metaf::WeatherGroup obscFog = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::FOG);
	const metaf::WeatherGroup obscSmoke = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SMOKE);
	const metaf::WeatherGroup obscVolcanicAsh = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::VOLCANIC_ASH);
	const metaf::WeatherGroup obscDust = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::DUST);
	const metaf::WeatherGroup obscSand = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SAND);
	const metaf::WeatherGroup obscHaze = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::HAZE);
	//Obscuration groups with descriptors and prefix
	const metaf::WeatherGroup obscBlowingSpray = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::BLOWING,
			metaf::WeatherGroup::Weather::SPRAY);
	const metaf::WeatherGroup obscFreezingFog = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::FREEZING,
			metaf::WeatherGroup::Weather::FOG);
	const metaf::WeatherGroup obscFogInVicinity = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::FOG);
	const metaf::WeatherGroup obscBlowingDust = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::BLOWING,
			metaf::WeatherGroup::Weather::DUST);
	const metaf::WeatherGroup obscDriftingDustInVicinity = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::LOW_DRIFTING,
			metaf::WeatherGroup::Weather::DUST);
	//Blowing snow and low drifting snow are obscuration rather than precipitation
	const metaf::WeatherGroup obscDriftingSnow = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::LOW_DRIFTING,
			metaf::WeatherGroup::Weather::SNOW);
	const metaf::WeatherGroup obscBlowingSnow = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::BLOWING,
			metaf::WeatherGroup::Weather::SNOW);
	//Simple other weather groups
	const metaf::WeatherGroup otherDustWhirls = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::DUST_WHIRLS);
	const metaf::WeatherGroup otherSqualls = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SQUALLS);
	const metaf::WeatherGroup otherFunnelCloud = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::FUNNEL_CLOUD);
	//Other weather groups with prefix
	const metaf::WeatherGroup otherDustWhirlsInVicinity = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::DUST_WHIRLS);
	const metaf::WeatherGroup otherTornado = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY, 
			metaf::WeatherGroup::Weather::FUNNEL_CLOUD);
	//Thunderstorm without associated precipitation is other weather
	const metaf::WeatherGroup otherThunderstorm = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::THUNDERSTORM);
	const metaf::WeatherGroup otherThunderstormInVicinity = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY,
			metaf::WeatherGroup::Weather::THUNDERSTORM);
	//Sand storm and dust storm are both obscuration and other weather
	const metaf::WeatherGroup obscOtherSandStortInVicinity = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::SANDSTORM);
	const metaf::WeatherGroup obscOtherHeavyDustStort = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY, 
			metaf::WeatherGroup::Weather::DUSTSTORM);
	const metaf::WeatherGroup obscOtherSandStorm = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SANDSTORM);
	const metaf::WeatherGroup obscOtherDustStorm = 
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::DUSTSTORM);
};

TEST_F(WeatherGroupIsAttribute, isPrecipitation){
	EXPECT_TRUE(precDrizzle.isPrecipitation());
	EXPECT_TRUE(precRain.isPrecipitation());
	EXPECT_TRUE(precSnow.isPrecipitation());
	EXPECT_TRUE(precSnowGrains.isPrecipitation());
	EXPECT_TRUE(precIceCrystals.isPrecipitation());
	EXPECT_TRUE(precIcePellets.isPrecipitation());
	EXPECT_TRUE(precHail.isPrecipitation());
	EXPECT_TRUE(precIcePellets.isPrecipitation());
	EXPECT_TRUE(precHail.isPrecipitation());
	EXPECT_TRUE(precSmallHail.isPrecipitation());
	EXPECT_TRUE(precUndetermined.isPrecipitation());
	EXPECT_TRUE(precShowersInVicinity.isPrecipitation());
	EXPECT_TRUE(precHeavySnowShowers.isPrecipitation());
	EXPECT_TRUE(precRainAndSnowMix.isPrecipitation());
	EXPECT_TRUE(precThunderstormShowersRainSmallHail.isPrecipitation());
	EXPECT_FALSE(obscMist.isPrecipitation());
	EXPECT_FALSE(obscFog.isPrecipitation());
	EXPECT_FALSE(obscSmoke.isPrecipitation());
	EXPECT_FALSE(obscVolcanicAsh.isPrecipitation());
	EXPECT_FALSE(obscDust.isPrecipitation());
	EXPECT_FALSE(obscSand.isPrecipitation());
	EXPECT_FALSE(obscHaze.isPrecipitation());
	EXPECT_FALSE(obscBlowingSpray.isPrecipitation());
	EXPECT_FALSE(obscFreezingFog.isPrecipitation());
	EXPECT_FALSE(obscFogInVicinity.isPrecipitation());
	EXPECT_FALSE(obscBlowingDust.isPrecipitation());
	EXPECT_FALSE(obscDriftingDustInVicinity.isPrecipitation());
	EXPECT_FALSE(obscDriftingSnow.isPrecipitation());
	EXPECT_FALSE(obscBlowingSnow.isPrecipitation());
	EXPECT_FALSE(otherDustWhirls.isPrecipitation());
	EXPECT_FALSE(otherSqualls.isPrecipitation());
	EXPECT_FALSE(otherFunnelCloud.isPrecipitation());
	EXPECT_FALSE(otherDustWhirlsInVicinity.isPrecipitation());
	EXPECT_FALSE(otherTornado.isPrecipitation());
	EXPECT_FALSE(otherThunderstorm.isPrecipitation());
	EXPECT_FALSE(otherThunderstormInVicinity.isPrecipitation());
	EXPECT_FALSE(obscOtherSandStortInVicinity.isPrecipitation());
	EXPECT_FALSE(obscOtherHeavyDustStort.isPrecipitation());
	EXPECT_FALSE(obscOtherSandStorm.isPrecipitation());
	EXPECT_FALSE(obscOtherDustStorm.isPrecipitation());
}

TEST_F(WeatherGroupIsAttribute, isObscuration){
	EXPECT_FALSE(precDrizzle.isObscuration());
	EXPECT_FALSE(precRain.isObscuration());
	EXPECT_FALSE(precSnow.isObscuration());
	EXPECT_FALSE(precSnowGrains.isObscuration());
	EXPECT_FALSE(precIceCrystals.isObscuration());
	EXPECT_FALSE(precIcePellets.isObscuration());
	EXPECT_FALSE(precHail.isObscuration());
	EXPECT_FALSE(precIcePellets.isObscuration());
	EXPECT_FALSE(precHail.isObscuration());
	EXPECT_FALSE(precSmallHail.isObscuration());
	EXPECT_FALSE(precUndetermined.isObscuration());
	EXPECT_FALSE(precShowersInVicinity.isObscuration());
	EXPECT_FALSE(precHeavySnowShowers.isObscuration());
	EXPECT_FALSE(precRainAndSnowMix.isObscuration());
	EXPECT_FALSE(precThunderstormShowersRainSmallHail.isObscuration());
	EXPECT_TRUE(obscMist.isObscuration());
	EXPECT_TRUE(obscFog.isObscuration());
	EXPECT_TRUE(obscSmoke.isObscuration());
	EXPECT_TRUE(obscVolcanicAsh.isObscuration());
	EXPECT_TRUE(obscDust.isObscuration());
	EXPECT_TRUE(obscSand.isObscuration());
	EXPECT_TRUE(obscHaze.isObscuration());
	EXPECT_TRUE(obscBlowingSpray.isObscuration());
	EXPECT_TRUE(obscFreezingFog.isObscuration());
	EXPECT_TRUE(obscFogInVicinity.isObscuration());
	EXPECT_TRUE(obscBlowingDust.isObscuration());
	EXPECT_TRUE(obscDriftingDustInVicinity.isObscuration());
	EXPECT_TRUE(obscDriftingSnow.isObscuration());
	EXPECT_TRUE(obscBlowingSnow.isObscuration());
	EXPECT_FALSE(otherDustWhirls.isObscuration());
	EXPECT_FALSE(otherSqualls.isObscuration());
	EXPECT_FALSE(otherFunnelCloud.isObscuration());
	EXPECT_FALSE(otherDustWhirlsInVicinity.isObscuration());
	EXPECT_FALSE(otherTornado.isObscuration());
	EXPECT_FALSE(otherThunderstorm.isObscuration());
	EXPECT_FALSE(otherThunderstormInVicinity.isObscuration());
	EXPECT_TRUE(obscOtherSandStortInVicinity.isObscuration());
	EXPECT_TRUE(obscOtherHeavyDustStort.isObscuration());
	EXPECT_TRUE(obscOtherSandStorm.isObscuration());
	EXPECT_TRUE(obscOtherDustStorm.isObscuration());
}

TEST_F(WeatherGroupIsAttribute, isOtherPhenomena){
	EXPECT_FALSE(precDrizzle.isOtherPhenomena());
	EXPECT_FALSE(precRain.isOtherPhenomena());
	EXPECT_FALSE(precSnow.isOtherPhenomena());
	EXPECT_FALSE(precSnowGrains.isOtherPhenomena());
	EXPECT_FALSE(precIceCrystals.isOtherPhenomena());
	EXPECT_FALSE(precIcePellets.isOtherPhenomena());
	EXPECT_FALSE(precHail.isOtherPhenomena());
	EXPECT_FALSE(precIcePellets.isOtherPhenomena());
	EXPECT_FALSE(precHail.isOtherPhenomena());
	EXPECT_FALSE(precSmallHail.isOtherPhenomena());
	EXPECT_FALSE(precUndetermined.isOtherPhenomena());
	EXPECT_FALSE(precShowersInVicinity.isOtherPhenomena());
	EXPECT_FALSE(precHeavySnowShowers.isOtherPhenomena());
	EXPECT_FALSE(precRainAndSnowMix.isOtherPhenomena());
	EXPECT_TRUE(precThunderstormShowersRainSmallHail.isOtherPhenomena());
	EXPECT_FALSE(obscMist.isOtherPhenomena());
	EXPECT_FALSE(obscFog.isOtherPhenomena());
	EXPECT_FALSE(obscSmoke.isOtherPhenomena());
	EXPECT_FALSE(obscVolcanicAsh.isOtherPhenomena());
	EXPECT_FALSE(obscDust.isOtherPhenomena());
	EXPECT_FALSE(obscSand.isOtherPhenomena());
	EXPECT_FALSE(obscHaze.isOtherPhenomena());
	EXPECT_FALSE(obscBlowingSpray.isOtherPhenomena());
	EXPECT_FALSE(obscFreezingFog.isOtherPhenomena());
	EXPECT_FALSE(obscFogInVicinity.isOtherPhenomena());
	EXPECT_FALSE(obscBlowingDust.isOtherPhenomena());
	EXPECT_FALSE(obscDriftingDustInVicinity.isOtherPhenomena());
	EXPECT_FALSE(obscDriftingSnow.isOtherPhenomena());
	EXPECT_FALSE(obscBlowingSnow.isOtherPhenomena());
	EXPECT_TRUE(otherDustWhirls.isOtherPhenomena());
	EXPECT_TRUE(otherSqualls.isOtherPhenomena());
	EXPECT_TRUE(otherFunnelCloud.isOtherPhenomena());
	EXPECT_TRUE(otherDustWhirlsInVicinity.isOtherPhenomena());
	EXPECT_TRUE(otherTornado.isOtherPhenomena());
	EXPECT_TRUE(otherThunderstorm.isOtherPhenomena());
	EXPECT_TRUE(otherThunderstormInVicinity.isOtherPhenomena());
	EXPECT_TRUE(obscOtherSandStortInVicinity.isOtherPhenomena());
	EXPECT_TRUE(obscOtherHeavyDustStort.isOtherPhenomena());
	EXPECT_TRUE(obscOtherSandStorm.isOtherPhenomena());
	EXPECT_TRUE(obscOtherDustStorm.isOtherPhenomena());
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
	static const auto airTemp = metaf::Temperature(2);
	static const auto dewPoint = metaf::Temperature(-1);
	metaf::TemperatureGroup group1;
	group1.airTemp = airTemp;
	group1.dewPoint = dewPoint;
	metaf::TemperatureGroup group2;
	group2.airTemp = airTemp;
	group2.dewPoint = dewPoint;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different values are not equal
TEST(TemperatureGroup, differentDataNotEqual) {
	metaf::TemperatureGroup group;
	group.airTemp = metaf::Temperature(2);
	group.dewPoint = metaf::Temperature(-1);
	metaf::TemperatureGroup group1(group);
	group1.airTemp = metaf::Temperature(1);
	metaf::TemperatureGroup group2(group);
	group2.airTemp = metaf::Temperature();
	metaf::TemperatureGroup group3(group);
	group3.dewPoint = metaf::Temperature(-2);
	metaf::TemperatureGroup group4(group);
	group4.dewPoint = metaf::Temperature();
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
}

//Confirm that temperature-only constructor initialises the group with correct 
//data
TEST(TemperatureGroup, constructorTempertureOnly) {
	static const auto airTemp = metaf::Temperature(5);
	const metaf::TemperatureGroup group(airTemp);
	EXPECT_EQ(group.airTemp, airTemp);
	EXPECT_EQ(group.dewPoint, metaf::Temperature());
}

//Confirm that temperature & dewpoint constructor initialises the group with 
//correct data
TEST(TemperatureGroup, constructorTemperatureDewpoint) {
	static const auto airTemp = metaf::Temperature(5);
	static const auto dewPoint = metaf::Temperature(0);
	const metaf::TemperatureGroup group(airTemp, dewPoint);
	EXPECT_EQ(group.airTemp, airTemp);
	EXPECT_EQ(group.dewPoint, dewPoint);
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
	EXPECT_EQ(group.unit, unit);
	EXPECT_FALSE(group.reported);
}

//Confirm that constructor initialises the group with correct data
TEST(PressureGroup, constructorReported) {
	static const auto unit = metaf::PressureGroup::Unit::INCHES_HG;
	static const auto pressure = 30.31;
	const metaf::PressureGroup group(pressure, unit);
	EXPECT_EQ(group.unit, unit);
	EXPECT_TRUE(group.reported);
	EXPECT_NEAR(group.pressure, pressure, 0.01/2);
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
	EXPECT_EQ(group.runway, runway);
	EXPECT_EQ(group.unit, unit);
	EXPECT_EQ(group.trend, trend);
	EXPECT_FALSE(group.visRangeReported);
}

//Confirm that 'single rvr value' constructor initialises
//the group with correct data
TEST(RunwayVisualRangeGroup, constructorSingleRvrValue) {
	static const auto runway = metaf::Runway(22);
	static const auto visRange = 6500u;
	static const auto modifier = metaf::ValueModifier::NONE;
	static const auto unit = metaf::DistanceUnit::METERS;
	static const auto trend = metaf::RunwayVisualRangeGroup::Trend::NEUTRAL;
	metaf::RunwayVisualRangeGroup group(runway, 
		visRange, 
		unit, 
		modifier,
		trend);
	EXPECT_EQ(group.runway, runway);
	EXPECT_EQ(group.visRange, visRange);
	EXPECT_TRUE(group.visRangeReported);
	EXPECT_FALSE(group.varVisRangeReported);
	EXPECT_EQ(group.visModifier, modifier);
	EXPECT_EQ(group.unit, unit);
	EXPECT_EQ(group.trend, trend);
}

//Confirm that 'min/max visibility' constructor initialises the group with 
//correct data
TEST(RunwayVisualRangeGroup, constructorMinMaxRvrValue) {
	static const auto runway = metaf::Runway(22);
	static const auto visRange = 4500u;
	static const auto modifier = metaf::ValueModifier::LESS_THAN;
	static const auto varVisRange = 6500u;
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
	EXPECT_EQ(group.runway, runway);
	EXPECT_EQ(group.visRange, visRange);
	EXPECT_TRUE(group.visRangeReported);
	EXPECT_TRUE(group.varVisRangeReported);
	EXPECT_EQ(group.visModifier, modifier);
	EXPECT_EQ(group.varVisRange, varVisRange);
	EXPECT_EQ(group.varVisModifier, varVisModifier);
	EXPECT_EQ(group.unit, unit);
	EXPECT_EQ(group.trend, trend);
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

//Confirm that for groups with CLRD status surface friction DOES affect 
//comparion result
TEST(RunwayStateGroup, comparisonClrdSurfaceFriction) {
	metaf::RunwayStateGroup group;
	group.runway = metaf::Runway(22);
	group.status = metaf::RunwayStateGroup::Status::CLRD;
	group.surfaceFriction = metaf::RunwayStateGroup::SurfaceFriction(0.65);
	metaf::RunwayStateGroup group1(group);
	group1.surfaceFriction = metaf::RunwayStateGroup::SurfaceFriction(0.64);
	EXPECT_NE(group, group1);
}

//Confirm that for groups with CLRD status deposits, extent and deposits depth 
//do NOT affect comparison result
TEST(RunwayStateGroup, comparisonClrdOther) {
	metaf::RunwayStateGroup group;
	group.runway = metaf::Runway(22);
	group.status = metaf::RunwayStateGroup::Status::CLRD;
	group.deposits = metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES;
	group.contaminationExtent = metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT;
	group.depositDepth = metaf::RunwayStateGroup::DepositDepth(1);
	group.surfaceFriction = metaf::RunwayStateGroup::SurfaceFriction(0.65);
	metaf::RunwayStateGroup group1(group);
	group1.deposits = metaf::RunwayStateGroup::Deposits::DAMP;
	metaf::RunwayStateGroup group2(group);
	group2.contaminationExtent = metaf::RunwayStateGroup::Extent::LESS_THAN_10_PERCENT;
	metaf::RunwayStateGroup group3(group);
	group3.depositDepth = metaf::RunwayStateGroup::DepositDepth(2);
}

//Confirm that for groups with SNOCLO status, deposits, extent, deposits depth
//and surface friction do NOT affect the comparison
TEST(RunwayStateGroup, comparisonSNOCLO) {
	metaf::RunwayStateGroup group;
	group.runway = metaf::Runway(22);
	group.status = metaf::RunwayStateGroup::Status::SNOCLO;
	group.deposits = metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES;
	group.contaminationExtent = metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT;
	group.depositDepth = metaf::RunwayStateGroup::DepositDepth(1);
	group.surfaceFriction = metaf::RunwayStateGroup::SurfaceFriction(0.65);
	metaf::RunwayStateGroup group1(group);
	group1.deposits = metaf::RunwayStateGroup::Deposits::DRY_SNOW;
	metaf::RunwayStateGroup group2(group);
	group2.contaminationExtent = metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT;
	metaf::RunwayStateGroup group3(group);
	group3.depositDepth = metaf::RunwayStateGroup::DepositDepth(3);
	metaf::RunwayStateGroup group4(group);
	group4.surfaceFriction = metaf::RunwayStateGroup::SurfaceFriction(0.5);
	EXPECT_EQ(group, group1);
	EXPECT_EQ(group, group2);
	EXPECT_EQ(group, group3);
	EXPECT_EQ(group, group4);
}

//Confirm that constructor initialises the group with correct data
TEST(RunwayStateGroup, constructor) {
	static const auto runway = metaf::Runway(21, metaf::Runway::Designator::CENTER);
	static const auto status = metaf::RunwayStateGroup::Status::NONE;
	static const auto deposits = metaf::RunwayStateGroup::Deposits::DRY_SNOW;
	static const auto extent = metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT;
	static const auto ddDepth = 5u;
	static const auto sfBrakingAction = 
		metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR;
	const metaf::RunwayStateGroup group(
		runway,
		status,
		deposits,
		extent,
		metaf::RunwayStateGroup::DepositDepth(ddDepth),
		metaf::RunwayStateGroup::SurfaceFriction(sfBrakingAction));
	EXPECT_EQ(group.runway, runway);
	EXPECT_EQ(group.status, status);
	EXPECT_EQ(group.deposits, deposits);
	EXPECT_EQ(group.contaminationExtent, extent);
	EXPECT_EQ(group.depositDepth, metaf::RunwayStateGroup::DepositDepth(ddDepth));
	EXPECT_EQ(group.depositDepth.depth, ddDepth);
	EXPECT_EQ(group.surfaceFriction, metaf::RunwayStateGroup::SurfaceFriction(sfBrakingAction));
}

//Confirm that makeSnoclo initialises the group with correct data
TEST(RunwayStateGroup, makeSnoclo) {
	static const auto runway = metaf::Runway(21, metaf::Runway::Designator::CENTER);
	const metaf::RunwayStateGroup group = metaf::RunwayStateGroup::makeSnoclo(runway);
	EXPECT_EQ(group.runway, runway);
	EXPECT_EQ(group.status, metaf::RunwayStateGroup::Status::SNOCLO);
}

//Confirm that makeClrd initialises the group with correct data
TEST(RunwayStateGroup, makeClrd) {
	static const auto runway = metaf::Runway(21, metaf::Runway::Designator::CENTER);
	static const auto surfaceFriction = 0.67;
	const metaf::RunwayStateGroup group = metaf::RunwayStateGroup::makeClrd(runway,
		metaf::RunwayStateGroup::SurfaceFriction(surfaceFriction));
	EXPECT_EQ(group.runway, runway);
	EXPECT_EQ(group.status, metaf::RunwayStateGroup::Status::CLRD);
	EXPECT_EQ(group.surfaceFriction, metaf::RunwayStateGroup::SurfaceFriction(surfaceFriction));
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
	static const metaf::Temperature surfaceTemp(7);
	static const auto stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
	metaf::SeaSurfaceGroup group1;
	group1.surfaceTemp = surfaceTemp;
	group1.stateOfSurface = stateOfSurface;
	metaf::SeaSurfaceGroup group2;
	group2.surfaceTemp = surfaceTemp;
	group2.stateOfSurface = stateOfSurface;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different values are not equal
TEST(SeaSurfaceGroup, differentDataNotEqual) {
	metaf::SeaSurfaceGroup group;
	group.surfaceTemp = metaf::Temperature(7);
	group.stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
	metaf::SeaSurfaceGroup group1(group);
	group1.surfaceTemp = metaf::Temperature(6);
	metaf::SeaSurfaceGroup group2(group);
	group2.surfaceTemp = metaf::Temperature();
	metaf::SeaSurfaceGroup group3(group);
	group3.stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::VERY_ROUGH;
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
}

//Confirm that temperature & stateOfSurface constructor initialises the group 
//with correct data
TEST(SeaSurfaceGroup, constructorTemeperatureStateofsurface) {
	static const metaf::Temperature surfaceTemp(7);
	static const auto stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
	const metaf::SeaSurfaceGroup group(surfaceTemp, stateOfSurface);
	EXPECT_EQ(group.surfaceTemp, surfaceTemp);
	EXPECT_EQ(group.stateOfSurface, stateOfSurface);
}

//Confirm that stateOfSurface constructor initialises the group with correct 
//data
TEST(SeaSurfaceGroup, constructorStateofsurface) {
	static const auto stateOfSurface = metaf::SeaSurfaceGroup::StateOfSurface::ROUGH;
	const metaf::SeaSurfaceGroup group(stateOfSurface);
	EXPECT_EQ(group.surfaceTemp, metaf::Temperature());
	EXPECT_EQ(group.stateOfSurface, stateOfSurface);
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
	static const metaf::Temperature surfaceTemp(7);
	static const auto waveHeight = 28;
	static const auto waveHeightReported = true;
	metaf::SeaWavesGroup group1;
	metaf::SeaWavesGroup group2;
	group1.surfaceTemp = surfaceTemp;
	group1.waveHeight = waveHeight;
	group1.waveHeightReported = waveHeightReported;
	group2.surfaceTemp = surfaceTemp;
	group2.waveHeight = waveHeight;
	group2.waveHeightReported = waveHeightReported;
	EXPECT_EQ(group1, group2);
}

//Confirm that if the wave height is not reported, it does NOT affect the 
//comparison result
TEST(SeaWavesGroup, comparisonTemperatureWaveHeightNotReported) {
	metaf::SeaWavesGroup group;
	group.surfaceTemp = metaf::Temperature(7);
	group.waveHeight = 28;
	metaf::SeaWavesGroup group1(group);
	group1.waveHeight = 29;
	EXPECT_EQ(group, group1);
}

//Confirm that groups holding the different values are not equal
TEST(SeaWavesGroup, differentDataNotEqual) {
	metaf::SeaWavesGroup group;
	group.surfaceTemp = metaf::Temperature(7);
	group.waveHeight = 28;
	group.waveHeightReported = true;
	metaf::SeaWavesGroup group1(group);
	group1.surfaceTemp = metaf::Temperature(8);
	metaf::SeaWavesGroup group2(group);
	group2.surfaceTemp = metaf::Temperature();
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
	static const auto surfaceTemp = metaf::Temperature(5);
	static const auto waveHeight = 12u;
	const metaf::SeaWavesGroup group(surfaceTemp, waveHeight);
	EXPECT_EQ(group.surfaceTemp, surfaceTemp);
	EXPECT_TRUE(group.waveHeightReported);
	EXPECT_EQ(group.waveHeight, waveHeight);
}

//Confirm that makeTemperature initialises the group with correct data
TEST(SeaWavesGroup, makeTemperature) {
	static const auto surfaceTemp = metaf::Temperature(5);
	const metaf::SeaWavesGroup group = metaf::SeaWavesGroup::makeTemperature(surfaceTemp);
	EXPECT_EQ(group.surfaceTemp, surfaceTemp);
	EXPECT_FALSE(group.waveHeightReported);
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