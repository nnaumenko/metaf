/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

//Confirm that default Speed structs are equal
TEST(Speed, defaultEqual) {
	const metaf::Speed s1;
	const metaf::Speed s2;
	EXPECT_TRUE(s1 == s2);
	EXPECT_FALSE(s1 != s2);
}

//Confirm that Speed structs initialised with the same data are equal
TEST(Speed, sameDataEqual) {
	metaf::Speed s1;
	s1.value = 4;
	s1.reported = true;
	s1.unit = metaf::Speed::Unit::KNOTS;
	metaf::Speed s2;
	s2.value = 4;
	s2.reported = true;
	s2.unit = metaf::Speed::Unit::KNOTS;
	EXPECT_EQ(s1, s2);
}

//Confirm that if the speed is not reported, then both value and unit do NOT 
//affect the comparison, and unit DOES affect the comparison
TEST(Speed, comparisonSpeedNotReported) {
	metaf::Speed s;
	s.reported = false;
	s.value = 4;
	s.unit = metaf::Speed::Unit::KNOTS;
	metaf::Speed s1(s);
	s1.value = 7;
	metaf::Speed s2(s);
	s2.unit = metaf::Speed::Unit::METERS_PER_SECOND;
	EXPECT_EQ(s, s1);
	EXPECT_EQ(s, s2);
}

//Confirm that if the speed is reported, then both value and unit DO affect 
//the comparison
TEST(Speed, comparisonSpeedReported) {
	metaf::Speed s;
	s.reported = true;
	s.value = 4;
	s.unit = metaf::Speed::Unit::KNOTS;
	metaf::Speed s1(s);
	s1.value = 7;
	metaf::Speed s2(s);
	s2.unit = metaf::Speed::Unit::METERS_PER_SECOND;
	EXPECT_NE(s, s1);
	EXPECT_NE(s, s2);
}

//Confirm that 'not reported' constructor initialises the Speed struct with 
//the correct data
TEST(Speed, constructorNotReported) {
	const metaf::Speed s(metaf::Speed::Unit::KNOTS);
	EXPECT_FALSE(s.reported);
	EXPECT_EQ(s.unit, metaf::Speed::Unit::KNOTS);
}

//Confirm that 'reported' constructor initialises the Speed struct with the 
//correct data
TEST(Speed, constructorReported) {
	const metaf::Speed s(8, metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(s.value, 8u);
	EXPECT_TRUE(s.reported);
	EXPECT_EQ(s.unit, metaf::Speed::Unit::KNOTS);
}

//Confirm that valueAs returns the same value when converted to the same unit
TEST(Speed, valueAsSameUnit) {
	const metaf::Speed s1(10, metaf::Speed::Unit::KNOTS);
	const metaf::Speed s2(10, metaf::Speed::Unit::METERS_PER_SECOND);
	const metaf::Speed s3(10, metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	const metaf::Speed s4(10, metaf::Speed::Unit::MILES_PER_HOUR);
	const auto margin = 0.001;
	EXPECT_NEAR(s1.valueAs(s1.unit), 10, margin);
	EXPECT_NEAR(s2.valueAs(s2.unit), 10, margin);
	EXPECT_NEAR(s3.valueAs(s3.unit), 10, margin);
	EXPECT_NEAR(s4.valueAs(s4.unit), 10, margin);
}

//Confirm that valueAs returns zero when speed value is not reported
TEST(Speed, valueAsNotReported) {
	const metaf::Speed s;
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::KNOTS), 0, 0.01);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::METERS_PER_SECOND), 0, 0.01);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::KILOMETERS_PER_HOUR), 0, 0.01);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::MILES_PER_HOUR), 0, 0.01);
}

//Confirm that valueAs returns zero when unit is unknown
TEST(Speed, valueAsUnitUnknown) {
	const metaf::Speed s(15, metaf::Speed::Unit::UNKNOWN);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::KNOTS), 0, 0.01);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::METERS_PER_SECOND), 0, 0.01);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::KILOMETERS_PER_HOUR), 0, 0.01);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::MILES_PER_HOUR), 0, 0.01);
}

//Confirm that valueAs returns zero when target unit is unknown
TEST(Speed, valueAsTargetUnitUnknown) {
	const metaf::Speed s1(15, metaf::Speed::Unit::KNOTS);
	const metaf::Speed s2(15, metaf::Speed::Unit::METERS_PER_SECOND);
	const metaf::Speed s3(15, metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	const metaf::Speed s4(15, metaf::Speed::Unit::MILES_PER_HOUR);
	EXPECT_NEAR(s1.valueAs(metaf::Speed::Unit::UNKNOWN), 0, 0.01);
	EXPECT_NEAR(s2.valueAs(metaf::Speed::Unit::UNKNOWN), 0, 0.01);
	EXPECT_NEAR(s3.valueAs(metaf::Speed::Unit::UNKNOWN), 0, 0.01);
	EXPECT_NEAR(s4.valueAs(metaf::Speed::Unit::UNKNOWN), 0, 0.01);
}

//Confirm that valueAs correctly converts from knots to other units
TEST(Speed, valueAsFromKnots) {
	const metaf::Speed s(10, metaf::Speed::Unit::KNOTS);
	const auto margin = 0.1 / 2;
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::METERS_PER_SECOND), 5.1, margin);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::KILOMETERS_PER_HOUR), 18.5, margin);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::MILES_PER_HOUR), 11.5, margin);
}

//Confirm that valueAs correctly converts from m/s to other units
TEST(Speed, valueAsFromMps) {
	const metaf::Speed s(10, metaf::Speed::Unit::METERS_PER_SECOND);
	const auto margin = 0.1 / 2;
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::KNOTS), 19.4, margin);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::KILOMETERS_PER_HOUR), 36, margin);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::MILES_PER_HOUR), 22.4, margin);
}

//Confirm that valueAs correctly converts from km/h to other units
TEST(Speed, valueAsFromKmh) {
	const metaf::Speed s(10, metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	const auto margin = 0.1 / 2;
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::KNOTS), 5.4, margin);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::METERS_PER_SECOND), 2.8, margin);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::MILES_PER_HOUR), 6.2, margin);
}

//Confirm that valueAs correctly converts from mph to other units
TEST(Speed, valueAsFromMph) {
	const metaf::Speed s(10, metaf::Speed::Unit::MILES_PER_HOUR);
	const auto margin = 0.1 / 2;
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::KNOTS), 8.7, margin);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::METERS_PER_SECOND), 4.5, margin);
	EXPECT_NEAR(s.valueAs(metaf::Speed::Unit::KILOMETERS_PER_HOUR), 16.1, margin);
}

//Confirm that isValid returns true when speed unit is either knots, m/s, km/h 
//or mph and the value is reported
TEST(Speed, isValidCorrectReported) {
	const metaf::Speed s1(15, metaf::Speed::Unit::KNOTS);
	const metaf::Speed s2(15, metaf::Speed::Unit::METERS_PER_SECOND);
	const metaf::Speed s3(15, metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	const metaf::Speed s4(15, metaf::Speed::Unit::MILES_PER_HOUR);
	EXPECT_TRUE(s1.isValid());
	EXPECT_TRUE(s2.isValid());
	EXPECT_TRUE(s3.isValid());
	EXPECT_TRUE(s4.isValid());
}

//Confirm that isValid returns false when speed unit is unknown and the value 
//is reported
TEST(Speed, isValidIncorrect) {
	const metaf::Speed s1(15, metaf::Speed::Unit::UNKNOWN);
	EXPECT_FALSE(s1.isValid());
}

//Confirm that isValid returns true with any speed unit if the value is not reported
TEST (Speed, isValidNotReported) {
	const metaf::Speed s1(metaf::Speed::Unit::KNOTS);
	const metaf::Speed s2(metaf::Speed::Unit::METERS_PER_SECOND);
	const metaf::Speed s3(metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	const metaf::Speed s4(metaf::Speed::Unit::MILES_PER_HOUR);
	const metaf::Speed s5(metaf::Speed::Unit::UNKNOWN);
	EXPECT_TRUE(s1.isValid());
	EXPECT_TRUE(s2.isValid());
	EXPECT_TRUE(s3.isValid());
	EXPECT_TRUE(s4.isValid());
	EXPECT_TRUE(s5.isValid());
}