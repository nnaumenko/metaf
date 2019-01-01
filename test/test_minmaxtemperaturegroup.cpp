/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

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
	static const auto temperature = metaf::Temperature(-1);
	static const auto time = metaf::MetafTime(12, 17, 0);
	metaf::MinMaxTemperatureGroup group1;
	metaf::MinMaxTemperatureGroup group2;
	group1.point = point;
	group1.temperature = temperature;
	group1.time = time;
	group2.point = point;
	group2.temperature = temperature;
	group2.time = time;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different value are not equal
TEST(MinMaxTemperatureGroup, differentDataNotEqual) {
	metaf::MinMaxTemperatureGroup group;
	group.point = metaf::MinMaxTemperatureGroup::Point::MINIMUM;
	group.temperature = metaf::Temperature(-1);
	group.time = metaf::MetafTime(12, 17);
	metaf::MinMaxTemperatureGroup group1(group);
	group1.point = metaf::MinMaxTemperatureGroup::Point::MAXIMUM;
	metaf::MinMaxTemperatureGroup group2(group);
	group2.temperature = metaf::Temperature(-2);
	metaf::MinMaxTemperatureGroup group3(group);
	group3.time = metaf::MetafTime(13, 17);
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
}

//Confirm that makeMin initialises the group with correct data
TEST(MinMaxTemperatureGroup, makeMin) {
	static const auto temperature = metaf::Temperature(1);
	static const auto time = metaf::MetafTime(25, 12, 0);
	const metaf::MinMaxTemperatureGroup group = 
		metaf::MinMaxTemperatureGroup::makeMin(temperature, time);
	EXPECT_EQ(group.point, metaf::MinMaxTemperatureGroup::Point::MINIMUM);
	EXPECT_EQ(group.temperature, temperature);
	EXPECT_EQ(group.time, time);
}

//Confirm that makeMax initialises the group with correct data
TEST(MinMaxTemperatureGroup, makeMax) {
	static const auto temperature = metaf::Temperature(1);
	static const auto time = metaf::MetafTime(25, 12, 0);
	const metaf::MinMaxTemperatureGroup group = 
		metaf::MinMaxTemperatureGroup::makeMax(temperature, time);
	EXPECT_EQ(group.point, metaf::MinMaxTemperatureGroup::Point::MAXIMUM);
	EXPECT_EQ(group.temperature, temperature);
	EXPECT_EQ(group.time, time);
}

//Confirm that isValid returns true if all the follwing conditions are met:
// - Temperature point is other than Unknown
// - Temperature and time values are valid
// - Time minute value is zero
TEST(MinMaxTemperatureGroup, isValidCorrect) {
	metaf::MinMaxTemperatureGroup g;
	g.point = metaf::MinMaxTemperatureGroup::Point::MAXIMUM;
	ASSERT_NE(g.point, metaf::MinMaxTemperatureGroup::Point::UNKNOWN);
	g.temperature = metaf::Temperature(3);
	ASSERT_TRUE(g.temperature.isValid());
	g.time = metaf::MetafTime(6, 20, 00);
	ASSERT_TRUE(g.time.isValid());
	ASSERT_EQ(g.time.minute, 0u);
	EXPECT_TRUE(g.isValid());
}

//Confirm that isValid returns false if any of the follwing conditions is met:
// - Temperature point is Unknown
// - Temperature or time value is invalid
// - Time minute value is not zero
TEST(MinMaxTemperatureGroup, isValidIncorrect) {
	metaf::MinMaxTemperatureGroup g1;
	g1.point = metaf::MinMaxTemperatureGroup::Point::UNKNOWN;
	ASSERT_EQ(g1.point, metaf::MinMaxTemperatureGroup::Point::UNKNOWN);
	g1.temperature = metaf::Temperature(3);
	ASSERT_TRUE(g1.temperature.isValid());
	g1.time = metaf::MetafTime(6, 20, 00);
	ASSERT_TRUE(g1.time.isValid());
	ASSERT_EQ(g1.time.minute, 0u);
	EXPECT_FALSE(g1.isValid());

	metaf::MinMaxTemperatureGroup g2;
	g2.point = metaf::MinMaxTemperatureGroup::Point::MAXIMUM;
	ASSERT_NE(g2.point, metaf::MinMaxTemperatureGroup::Point::UNKNOWN);
	g2.temperature = metaf::Temperature(3);
	g2.temperature.modifier = metaf::ValueModifier::UNKNOWN;
	ASSERT_FALSE(g2.temperature.isValid());
	g2.time = metaf::MetafTime(6, 20, 00);
	ASSERT_TRUE(g2.time.isValid());
	ASSERT_EQ(g2.time.minute, 0u);
	EXPECT_FALSE(g2.isValid());

	metaf::MinMaxTemperatureGroup g3;
	g3.point = metaf::MinMaxTemperatureGroup::Point::MAXIMUM;
	ASSERT_NE(g3.point, metaf::MinMaxTemperatureGroup::Point::UNKNOWN);
	g3.temperature = metaf::Temperature(3);
	ASSERT_TRUE(g3.temperature.isValid());
	g3.time = metaf::MetafTime(6, 25, 00);
	ASSERT_FALSE(g3.time.isValid());
	ASSERT_EQ(g3.time.minute, 0u);
	EXPECT_FALSE(g3.isValid());

	metaf::MinMaxTemperatureGroup g4;
	g4.point = metaf::MinMaxTemperatureGroup::Point::MAXIMUM;
	ASSERT_NE(g4.point, metaf::MinMaxTemperatureGroup::Point::UNKNOWN);
	g4.temperature = metaf::Temperature(3);
	ASSERT_TRUE(g4.temperature.isValid());
	g4.time = metaf::MetafTime(6, 20, 01);
	ASSERT_TRUE(g4.time.isValid());
	ASSERT_NE(g4.time.minute, 0u);
	EXPECT_FALSE(g4.isValid());
}

//Confirm that parse() method correctly parses Min/Max Temperature Group
TEST(MinMaxTemperatureGroup, parseMinMaxTemperature) {
	metaf::MinMaxTemperatureGroup g1;
	//Max temperature 3 centigrade on day 6, 20:00 GMT
	EXPECT_TRUE(g1.parse("TX03/0620Z", metaf::ReportPart::TAF)); 
	EXPECT_EQ(g1.point, metaf::MinMaxTemperatureGroup::Point::MAXIMUM);
	EXPECT_EQ(g1.temperature, metaf::Temperature(3));
	EXPECT_EQ(g1.time, metaf::MetafTime(6, 20, 00));

	metaf::MinMaxTemperatureGroup g2;
	//Max temperature -1 centigrade on day 4, 2:00 GMT
	EXPECT_TRUE(g2.parse("TXM01/1402Z", metaf::ReportPart::TAF)); 
	EXPECT_EQ(g2.point, metaf::MinMaxTemperatureGroup::Point::MAXIMUM);
	EXPECT_EQ(g2.temperature, metaf::Temperature(-1));
	EXPECT_EQ(g2.time, metaf::MetafTime(14, 02, 00));

	metaf::MinMaxTemperatureGroup g3;
	//Min temperature 13 centigrade on day 12, 4:00 GMT
	EXPECT_TRUE(g3.parse("TN13/1204Z", metaf::ReportPart::TAF)); 
	EXPECT_EQ(g3.point, metaf::MinMaxTemperatureGroup::Point::MINIMUM);
	EXPECT_EQ(g3.temperature, metaf::Temperature(13));
	EXPECT_EQ(g3.time, metaf::MetafTime(12, 04, 00));

	metaf::MinMaxTemperatureGroup g4;
	//Min temperature -2 centigrade on day 7, 9:00 GMT
	EXPECT_TRUE(g4.parse("TNM02/0709Z", metaf::ReportPart::TAF)); 
	EXPECT_EQ(g4.point, metaf::MinMaxTemperatureGroup::Point::MINIMUM);
	EXPECT_EQ(g4.temperature, metaf::Temperature(-2));
	EXPECT_EQ(g4.time, metaf::MetafTime(7, 9, 00));

	metaf::MinMaxTemperatureGroup g5;
	//Max temperature 0 to 0.5 centigrade on day 7, 9:00 GMT
	EXPECT_TRUE(g5.parse("TX00/0709Z", metaf::ReportPart::TAF)); 
	EXPECT_EQ(g5.point, metaf::MinMaxTemperatureGroup::Point::MAXIMUM);
	EXPECT_EQ(g5.temperature, metaf::Temperature(0, metaf::ValueModifier::MORE_THAN));
	EXPECT_EQ(g5.time, metaf::MetafTime(7, 9, 00));

	metaf::MinMaxTemperatureGroup g6;
	//Max temperature -0.5 to 0 centigrade on day 7, 9:00 GMT
	EXPECT_TRUE(g6.parse("TXM00/0709Z", metaf::ReportPart::TAF)); 
	EXPECT_EQ(g6.point, metaf::MinMaxTemperatureGroup::Point::MAXIMUM);
	EXPECT_EQ(g6.temperature, metaf::Temperature(0, metaf::ValueModifier::LESS_THAN));
	EXPECT_EQ(g5.time, metaf::MetafTime(7, 9, 00));
}

//Confirm that parse() method does not parse Min/Max Temperature Group in 
//report parts other than TAF
TEST(MinMaxTemperatureGroup, parseWrongReportPart) {
	metaf::MinMaxTemperatureGroup g1;
	EXPECT_FALSE(g1.parse("TX03/0620Z", metaf::ReportPart::UNKNOWN)); 

	metaf::MinMaxTemperatureGroup g2;
	EXPECT_FALSE(g2.parse("TX03/0620Z", metaf::ReportPart::HEADER)); 

	metaf::MinMaxTemperatureGroup g3;
	EXPECT_FALSE(g3.parse("TX03/0620Z", metaf::ReportPart::METAR)); 

	metaf::MinMaxTemperatureGroup g4;
	EXPECT_FALSE(g4.parse("TX03/0620Z", metaf::ReportPart::RMK)); 
}

//Confirm that parse() method does not parse Min/Max Temperature Group if 
//number of digits is wrong
TEST(MinMaxTemperatureGroup, parseWrongDigitNumber) {
	metaf::MinMaxTemperatureGroup g1;
	EXPECT_FALSE(g1.parse("TX3/0620Z", metaf::ReportPart::TAF));

	metaf::MinMaxTemperatureGroup g2;
	EXPECT_FALSE(g2.parse("TX003/0620Z", metaf::ReportPart::TAF));

	metaf::MinMaxTemperatureGroup g3;
	EXPECT_FALSE(g3.parse("TXM3/0620Z", metaf::ReportPart::TAF));

	metaf::MinMaxTemperatureGroup g4;
	EXPECT_FALSE(g4.parse("TXM003/0620Z", metaf::ReportPart::TAF));

	metaf::MinMaxTemperatureGroup g5;
	EXPECT_FALSE(g5.parse("TX03/620Z", metaf::ReportPart::TAF));

	metaf::MinMaxTemperatureGroup g6;
	EXPECT_FALSE(g6.parse("TX03/00620Z", metaf::ReportPart::TAF));
}


//Confirm that parse() method does not parse malformed Min/Max Temperature 
//groups
TEST(MinMaxTemperatureGroup, parseWrongMalformedGroups) {
	metaf::MinMaxTemperatureGroup g1;
	EXPECT_FALSE(g1.parse("T03/0620Z", metaf::ReportPart::TAF)); 

	metaf::MinMaxTemperatureGroup g2;
	EXPECT_FALSE(g2.parse("TM03/0620Z", metaf::ReportPart::TAF)); 

	metaf::MinMaxTemperatureGroup g3;
	EXPECT_FALSE(g3.parse("TN03/0620", metaf::ReportPart::TAF)); 

	metaf::MinMaxTemperatureGroup g4;
	EXPECT_FALSE(g4.parse("TN03-0620Z", metaf::ReportPart::TAF)); 
}

//Confirm that parse() method does not parse other similar groups
TEST(MinMaxTemperatureGroup, parseWrongSimilarGroups) {
	metaf::MinMaxTemperatureGroup g1;
	EXPECT_FALSE(g1.parse("060230Z", metaf::ReportPart::TAF)); 

	metaf::MinMaxTemperatureGroup g2;
	EXPECT_FALSE(g2.parse("M00/M04", metaf::ReportPart::TAF)); 

	metaf::MinMaxTemperatureGroup g3;
	EXPECT_FALSE(g3.parse("1221/1303", metaf::ReportPart::TAF)); 
}
