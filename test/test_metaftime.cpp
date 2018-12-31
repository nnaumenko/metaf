/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

//Confirm that default MetafTime structs are equal
TEST(MetafTime, defaultEqual) {
	const metaf::MetafTime t1;
	const metaf::MetafTime t2;
	EXPECT_TRUE(t1 == t2);
	EXPECT_FALSE(t1 != t2);
}

//Confirm that MetafTime structs initialised with the same data are equal
TEST(MetafTime, sameDataEqual) {
	metaf::MetafTime t1;
	t1.day = 28;
	t1.hour = 15;
	t1.minute = 45;
	metaf::MetafTime t2;
	t2.day = 28;
	t2.hour = 15;
	t2.minute = 45;
	EXPECT_EQ(t1, t2);
}

//Confirm that MetafTime structs holding the different data are not equal
TEST(MetafTime, differentDataNotEqual) {
	metaf::MetafTime t;
	t.day = 28;
	t.hour = 15;
	t.minute = 45;
	metaf::MetafTime t1 (t);
	t1.day = 29;
	metaf::MetafTime t2 (t);
	t2.hour = 16;
	metaf::MetafTime t3 (t);
	t3.minute = 46;
	EXPECT_NE(t, t1);
	EXPECT_NE(t, t2);
	EXPECT_NE(t, t3);
}

//Confirm that constructor for reported day initialises the struct with 
//correct data
TEST(MetafTime, constructorDayReported) {
	static const auto day = 29u;
	static const auto hour = 23u;
	static const auto minute = 55u;
	const metaf::MetafTime t(day, hour, minute);
	EXPECT_EQ(t.day, day);	
	EXPECT_EQ(t.hour, hour);	
	EXPECT_EQ(t.minute, minute);	
}

//Confirm that constructor for non-reported day initialises the struct with 
//correct data
TEST(MetafTime, constructorDayNotReported) {
	static const auto hour = 23u;
	static const auto minute = 55u;
	const metaf::MetafTime t(hour, minute);
	EXPECT_EQ(t.hour, hour);	
	EXPECT_EQ(t.minute, minute);	
}

//Confirm that isDayReported() returns true if day was initialised and false 
//otherwise
TEST(MetafTime, isDayReported) {
	static const auto day = 29u;
	static const auto hour = 23u;
	static const auto minute = 55u;
	const metaf::MetafTime t1(day, hour, minute);
	const metaf::MetafTime t2(hour, minute);
	EXPECT_TRUE(t1.isDayReported());
	EXPECT_FALSE(t2.isDayReported());
}

//Confirm that isValid() returns true if day and time are within limits
TEST(MetafTime, isValidCorrect) {
	static const auto dayMin = 1u;
	static const auto dayMax = 31u;
	static const auto hourMin = 0u;
	static const auto hourMax = 24u;
	static const auto minuteMin = 0u;
	static const auto minuteMax = 59u;
	const metaf::MetafTime t1(dayMin, hourMin, minuteMin);
	const metaf::MetafTime t2(hourMin, minuteMin);
	const metaf::MetafTime t3(dayMax, hourMax, minuteMax);
	const metaf::MetafTime t4(hourMax, minuteMax);
	EXPECT_TRUE(t1.isValid());
	EXPECT_TRUE(t2.isValid());
	EXPECT_TRUE(t3.isValid());
	EXPECT_TRUE(t4.isValid());
}

//Confirm that isValid() returns false if day and time are outside limits
TEST(MetafTime, isValidIncorrect) {
	static const auto dayMax = 31u;
	static const auto hourMax = 24u;
	static const auto minuteMax = 59u;
	const metaf::MetafTime t1(dayMax + 1, hourMax, minuteMax);
	const metaf::MetafTime t2(dayMax, hourMax + 1, minuteMax);
	const metaf::MetafTime t3(dayMax, hourMax, minuteMax + 1);
	const metaf::MetafTime t4(hourMax + 1, minuteMax);
	const metaf::MetafTime t5(hourMax, minuteMax + 1);
	EXPECT_FALSE(t1.isValid());
	EXPECT_FALSE(t2.isValid());
	EXPECT_FALSE(t3.isValid());
	EXPECT_FALSE(t4.isValid());
	EXPECT_FALSE(t5.isValid());
}