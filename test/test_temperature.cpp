/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

//Confirm that default Temperature structs are equal
TEST(Temperature, defaultEqual) {
	const metaf::Temperature t1;
	const metaf::Temperature t2;
	EXPECT_TRUE(t1 == t2);
	EXPECT_FALSE(t1 != t2);
}

//Confirm that Temperature structs initialised with the same data are equal
TEST(Temperature, sameDataEqual) {
	metaf::Temperature t1;
	t1.value = 4;
	t1.reported = true;
	metaf::Temperature t2;
	t2.value = 4;
	t2.reported = true;
	EXPECT_EQ(t1, t2);
}

//Confirm that if the temperature is not reported, then value and modifier 
//DO NOT affect the comparison
TEST(Temperature, comparisonTemperatureNotReported) {
	metaf::Temperature t;
	t.reported = false;
	t.value = 4;
	t.modifier = metaf::ValueModifier::NONE;
	metaf::Temperature t1(t);
	t1.value = -5;
	t1.modifier = metaf::ValueModifier::LESS_THAN;
	EXPECT_EQ(t, t1);
}

//Confirm that if the temperature is reported, then value DOES affect the 
//comparison
TEST(Temperature, comparisonTemperatureReported) {
	metaf::Temperature t;
	t.reported = true;
	t.value = 4;
	metaf::Temperature t1(t);
	t1.value = 5;
	EXPECT_NE(t, t1);
}

//Confirm that if the temperature is reported and non-zero, then modifier 
//does NOT affect the comparison
TEST(Temperature, comparisonTemperatureNonZero) {
	metaf::Temperature t;
	t.reported = true;
	t.value = 4;
	metaf::Temperature t1(t);
	t1.modifier = metaf::ValueModifier::LESS_THAN;
	EXPECT_EQ(t, t1);
}

//Confirm that if the temperature is reported and zero, then modifier DOES 
//affect the comparison
TEST(Temperature, comparisonTemperatureZero) {
	metaf::Temperature t;
	t.reported = true;
	t.value = 0;
	t.modifier = metaf::ValueModifier::MORE_THAN;
	metaf::Temperature t1(t);
	t1.modifier = metaf::ValueModifier::LESS_THAN;
	EXPECT_NE(t, t1);
}

//Confirm that value&modifier constructor initialises the Temperature struct 
//with correct data when value is non-zero
TEST(Temperature, constructorValueModifierNonZero) {
	const metaf::Temperature t1(-2);
	const metaf::Temperature t2(-2, metaf::ValueModifier::LESS_THAN);
	EXPECT_EQ(t1.value, -2);
	EXPECT_TRUE(t1.reported);
	EXPECT_EQ(t1.modifier, metaf::ValueModifier::NONE);
	EXPECT_EQ(t2.value, -2);
	EXPECT_TRUE(t2.reported);
	EXPECT_EQ(t2.modifier, metaf::ValueModifier::NONE);
}

//Confirm that value&modifier constructor initialises the Temperature struct 
//with correct data when value is zero
TEST(Temperature, constructorValueModifierZero) {
	const metaf::Temperature t1(0);
	const metaf::Temperature t2(0, metaf::ValueModifier::LESS_THAN);
	const metaf::Temperature t3(0, metaf::ValueModifier::MORE_THAN);
	EXPECT_EQ(t1.value, 0);
	EXPECT_TRUE(t1.reported);
	EXPECT_EQ(t1.modifier, metaf::ValueModifier::NONE);
	EXPECT_EQ(t2.value, 0);
	EXPECT_TRUE(t2.reported);
	EXPECT_EQ(t2.modifier, metaf::ValueModifier::LESS_THAN);
	EXPECT_EQ(t3.value, 0);
	EXPECT_TRUE(t3.reported);
	EXPECT_EQ(t3.modifier, metaf::ValueModifier::MORE_THAN);
}

//Confirm that abs&minus constructor initialises the Temperature struct with 
//correct data when value is non-zero
TEST(Temperature, constructorAbsMinusNonZero) {
	const metaf::Temperature t1(2, false);
	const metaf::Temperature t2(2, true);
	EXPECT_EQ(t1.value, 2);
	EXPECT_TRUE(t1.reported);
	EXPECT_EQ(t1.modifier, metaf::ValueModifier::NONE);
	EXPECT_EQ(t2.value, -2);
	EXPECT_TRUE(t2.reported);
	EXPECT_EQ(t2.modifier, metaf::ValueModifier::NONE);
}

//Confirm that abs&minus constructor initialises the Temperature struct with 
//correct data when value is zero
TEST(Temperature, constructorAbsMinusZero) {
	const metaf::Temperature t1(0, true);
	const metaf::Temperature t2(0, false);
	EXPECT_EQ(t1.value, 0);
	EXPECT_TRUE(t1.reported);
	EXPECT_EQ(t1.modifier, metaf::ValueModifier::LESS_THAN);
	EXPECT_EQ(t2.value, 0);
	EXPECT_TRUE(t2.reported);
	EXPECT_EQ(t2.modifier, metaf::ValueModifier::MORE_THAN);
}

//Confirm that valueAs() returns zero if value is not reported
TEST(Temperature, valueAsNotReported) {
	const metaf::Temperature t;
	EXPECT_NEAR(t.valueAs(metaf::Temperature::Unit::UNKNOWN), 0, 0.01);
	EXPECT_NEAR(t.valueAs(metaf::Temperature::Unit::DEGREES_C), 0, 0.01);
	EXPECT_NEAR(t.valueAs(metaf::Temperature::Unit::DEGREES_F), 0, 0.01);
}

//Confirm that valueAs() returns zero if target unit is unknown
TEST(Temperature, valueAsUnitUnknown) {
	const metaf::Temperature t(-2);
	EXPECT_NEAR(t.valueAs(metaf::Temperature::Unit::UNKNOWN), 0, 0.01);
}


//Confirm that if no conversion from Celsius to Fahrenheit is required then 
//valueAs() returns the following values:
// - value is zero and modifier 'more than': number between 0 and 0.5 returned
// - value is zero and modifier 'less than': number between -0.5 and 0 returned
// - value is zero and modifier 'none' or 'unknown': zero returned
TEST(Temperature, valueAsMoreLessThanZero) {
	const metaf::Temperature t1(0, metaf::ValueModifier::MORE_THAN);
	const metaf::Temperature t2(0, metaf::ValueModifier::LESS_THAN);
	const metaf::Temperature t3(0);
	EXPECT_GT(t1.valueAs(metaf::Temperature::Unit::DEGREES_C), 0);
	EXPECT_LT(t1.valueAs(metaf::Temperature::Unit::DEGREES_C), 0.5);
	EXPECT_GT(t2.valueAs(metaf::Temperature::Unit::DEGREES_C), -0.5);
	EXPECT_LT(t2.valueAs(metaf::Temperature::Unit::DEGREES_C), 0);
	EXPECT_NEAR(t3.valueAs(metaf::Temperature::Unit::DEGREES_C), 0, 0.01);
}

//Confirm that valueAs() return the same value if target unit is Celsius
TEST(Temperature, valueAsSameUnit) {
	const metaf::Temperature t1(-40);
	const metaf::Temperature t2(0);
	ASSERT_EQ(t1.unit, metaf::Temperature::Unit::DEGREES_C);
	ASSERT_EQ(t2.unit, metaf::Temperature::Unit::DEGREES_C);
	EXPECT_NEAR(t1.valueAs(metaf::Temperature::Unit::DEGREES_C), -40, 0.01);
	EXPECT_NEAR(t2.valueAs(metaf::Temperature::Unit::DEGREES_C), 0, 0.01);
}

//Confirm that valueAs() correctly convert from Celsius to Fahrenheit if target
//unit if Fahernheit
TEST(Temperature, valueAsCelsiusToFahrenheit) {
	const metaf::Temperature t1(-40);
	const metaf::Temperature t2(0);
	EXPECT_NEAR(t1.valueAs(metaf::Temperature::Unit::DEGREES_F), -40, 0.01);
	EXPECT_NEAR(t2.valueAs(metaf::Temperature::Unit::DEGREES_F), 32, 0.01);
}

//Confirm that isValid() returns true if temperature value is not reported 
//regardless of value and modifier
TEST(Temperature, isValidNotReported) {
	const metaf::Temperature t1;
	metaf::Temperature t2;
	t2.value = 7;
	t2.modifier = metaf::ValueModifier::MORE_THAN;
	EXPECT_TRUE(t1.isValid());
	EXPECT_TRUE(t2.isValid());
}

//Confirm that isValid() returns true if temperature value is either non-zero 
//with modifier 'none' or zero with modifier 'more than'/'less than' / 'none'
TEST(Temperature, isValidCorrect) {
	const metaf::Temperature t1(7);
	const metaf::Temperature t2(0);
	const metaf::Temperature t3(0, metaf::ValueModifier::LESS_THAN);
	const metaf::Temperature t4(0, metaf::ValueModifier::MORE_THAN);
	EXPECT_TRUE(t1.isValid());
	EXPECT_TRUE(t2.isValid());
	EXPECT_TRUE(t3.isValid());
	EXPECT_TRUE(t4.isValid());
}	

//Confirm that isValid() returns false if modifier is 'unknown'
TEST(Temperature, isValidIncorrectModifierUnknown) {
	const metaf::Temperature t(0, metaf::ValueModifier::UNKNOWN);
	EXPECT_FALSE(t.isValid());
}

//Confirm that isValid() returns false if value is non-zero with modifier
//'more than' or 'less than'
TEST(Temperature, isValidIncorrectValueNonZeroWithModifier) {
	metaf::Temperature t1(3);
	metaf::Temperature t2(3);
	t1.modifier = metaf::ValueModifier::MORE_THAN;
	t2.modifier = metaf::ValueModifier::LESS_THAN;
	EXPECT_FALSE(t1.isValid());
	EXPECT_FALSE(t2.isValid());
}