/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

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
	group1.time = metaf::MetafTime(day, hour, minute);
	group2.time = metaf::MetafTime(day, hour, minute);
	EXPECT_EQ(group1, group2);
}

//Confirm that groups with different day/time are not equal
TEST(ReportTimeGroup, differentDataNotEqual) {
	metaf::ReportTimeGroup group;
	group.time = metaf::MetafTime(29, 23, 55);
	metaf::ReportTimeGroup group1(group);
	group1.time = metaf::MetafTime(29, 23, 56);
	EXPECT_NE(group, group1);
}

//Confirm that constructor initialises the group with correct data
TEST(ReportTimeGroup, constructor) {
	static const auto time = metaf::MetafTime (29, 23, 55);
	const metaf::ReportTimeGroup group(time);
	EXPECT_EQ(group.time, time);	
}

//Confirm that isValid returns true if day/time is valid and day is reported
TEST(ReportTimeGroup, isValidCorrect) {
	static const auto time = metaf::MetafTime (29, 23, 55);
	ASSERT_TRUE(time.isValid());
	ASSERT_TRUE(time.isDayReported());
	const metaf::ReportTimeGroup group(time);
	EXPECT_TRUE(group.isValid());
}

//Confirm that isValid returns false if day/time is not valid or day is not 
//reported
TEST(ReportTimeGroup, isValidIncorrect) {
	static const auto time1 = metaf::MetafTime (32, 23, 55);
	ASSERT_FALSE(time1.isValid());
	const metaf::ReportTimeGroup group1(time1);

	static const auto time2 = metaf::MetafTime (23, 55);
	ASSERT_FALSE(time2.isDayReported());
	const metaf::ReportTimeGroup group2(time2);

	EXPECT_FALSE(group1.isValid());
	EXPECT_FALSE(group2.isValid());
}

//Confirm that parse() method correctly parses Report Time Group
TEST(ReportTimeGroup, parseReportTime) {
	metaf::ReportTimeGroup g;
	EXPECT_TRUE(g.parse("201730Z", metaf::ReportPart::HEADER)); //day-of-month 20, time 17:30
	EXPECT_EQ(g.time, metaf::MetafTime(20, 17, 30));
}

//Confirm that parse() method does not parse Report Time Group in report parts 
//other than Header
TEST(ReportTimeGroup, parseWrongReportPart) {
	const std::string gs("201730Z"); //day-of-month 20, time 17:30

	metaf::ReportTimeGroup g1;
	EXPECT_FALSE(g1.parse(gs, metaf::ReportPart::UNKNOWN));

	metaf::ReportTimeGroup g2;
	EXPECT_FALSE(g2.parse(gs, metaf::ReportPart::METAR));

	metaf::ReportTimeGroup g3;
	EXPECT_FALSE(g3.parse(gs, metaf::ReportPart::TAF));

	metaf::ReportTimeGroup g4;
	EXPECT_FALSE(g4.parse(gs, metaf::ReportPart::RMK));
}

//Confirm that parse() method does not parse malformed Report Time Groups
TEST(ReportTimeGroup, parseWrongMalformedGroups) {
	metaf::ReportTimeGroup g1;
	EXPECT_FALSE(g1.parse("20/1730Z", metaf::ReportPart::HEADER));

	metaf::ReportTimeGroup g2;
	EXPECT_FALSE(g2.parse("20:17:30Z", metaf::ReportPart::HEADER));

	metaf::ReportTimeGroup g3;
	EXPECT_FALSE(g3.parse("201730", metaf::ReportPart::HEADER));
}

//Confirm that parse() method does not parse other similar groups
TEST(ReportTimeGroup, parseWrongSimilarGroups) {
	metaf::ReportTimeGroup g1;
	EXPECT_FALSE(g1.parse("05025KT", metaf::ReportPart::HEADER));
}