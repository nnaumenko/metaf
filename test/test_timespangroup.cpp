/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

//Confirm that default groups are equal
TEST(TimeSpanGroup, defaultDataEqual) {
	const metaf::TimeSpanGroup group1;
	const metaf::TimeSpanGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups with the same time are equal
TEST(TimeSpanGroup, sameDataEqual) {
	static const auto timeFrom = metaf::MetafTime(29, 23, 00);
	static const auto timeTill = metaf::MetafTime(30, 02, 00);
	metaf::TimeSpanGroup group1;
	metaf::TimeSpanGroup group2;
	group1.from = timeFrom;
	group1.till = timeTill;
	group2.from = timeFrom;
	group2.till = timeTill;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups with different time are not equal
TEST(TimeSpanGroup, differentDataNotEqual) {
	metaf::TimeSpanGroup group;
	group.from = metaf::MetafTime(29, 23, 00);
	group.till = metaf::MetafTime(30, 02, 00);
	metaf::TimeSpanGroup group1(group);
	group1.from = metaf::MetafTime(29, 24, 00);
	metaf::TimeSpanGroup group2(group);
	group2.till = metaf::MetafTime(30, 03, 00);
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
}

//Confirm that constructor initialises the group with correct data
TEST(TimeSpanGroup, constructor) {
	static const auto timeFrom = metaf::MetafTime(29, 23, 00);
	static const auto timeTill = metaf::MetafTime(30, 02, 00);
	const metaf::TimeSpanGroup group(timeFrom, timeTill);
	EXPECT_EQ(group.from, timeFrom);
	EXPECT_EQ(group.till, timeTill);
}

//Confirm that isValid returns true if day is reported and minutes are zero
TEST(TimeSpanGroup, isValidCorrect) {
	static const auto timeFrom = metaf::MetafTime (29, 21, 00);
	ASSERT_TRUE(timeFrom.isValid());
	ASSERT_TRUE(timeFrom.isDayReported());
	ASSERT_EQ(timeFrom.minute, 0u);

	static const auto timeTill = metaf::MetafTime (30, 06, 00);
	ASSERT_TRUE(timeTill.isValid());
	ASSERT_TRUE(timeTill.isDayReported());
	ASSERT_EQ(timeTill.minute, 0u);

	const metaf::TimeSpanGroup group(timeFrom, timeTill);
	EXPECT_TRUE(group.isValid());
}

//Confirm that isValid returns false if time is invalid, day is not reported 
//or minutes are not zero (time 'from')
TEST(TimeSpanGroup, isValidIncorrectTimeFrom) {
	static const auto time1 = metaf::MetafTime (29, 25, 00);
	ASSERT_FALSE(time1.isValid());
	ASSERT_TRUE(time1.isDayReported());
	ASSERT_EQ(time1.minute, 0u);

	static const auto time2 = metaf::MetafTime (21, 00);
	ASSERT_TRUE(time2.isValid());
	ASSERT_FALSE(time2.isDayReported());
	ASSERT_EQ(time2.minute, 0u);

	static const auto time3 = metaf::MetafTime (29, 21, 30);
	ASSERT_TRUE(time3.isValid());
	ASSERT_TRUE(time3.isDayReported());
	ASSERT_NE(time3.minute, 0u);

	static const auto timeTill = metaf::MetafTime (30, 06, 00);
	ASSERT_TRUE(timeTill.isValid());
	ASSERT_TRUE(timeTill.isDayReported());
	ASSERT_EQ(timeTill.minute, 0u);

	const metaf::TimeSpanGroup group1(time1, timeTill);
	EXPECT_FALSE(group1.isValid());

	const metaf::TimeSpanGroup group2(time2, timeTill);
	EXPECT_FALSE(group2.isValid());

	const metaf::TimeSpanGroup group3(time3, timeTill);
	EXPECT_FALSE(group3.isValid());
}

//Confirm that isValid returns false if time is invalid, day is not reported 
//or minutes are not zero (time 'until')
TEST(TimeSpanGroup, isValidIncorrectTimeUntil) {
	static const auto timeFrom = metaf::MetafTime (29, 02, 00);
	ASSERT_TRUE(timeFrom.isValid());
	ASSERT_TRUE(timeFrom.isDayReported());
	ASSERT_EQ(timeFrom.minute, 0u);

	static const auto time1 = metaf::MetafTime (29, 25, 00);
	ASSERT_FALSE(time1.isValid());
	ASSERT_TRUE(time1.isDayReported());
	ASSERT_EQ(time1.minute, 0u);

	static const auto time2 = metaf::MetafTime (21, 00);
	ASSERT_TRUE(time2.isValid());
	ASSERT_FALSE(time2.isDayReported());
	ASSERT_EQ(time2.minute, 0u);

	static const auto time3 = metaf::MetafTime (29, 21, 30);
	ASSERT_TRUE(time3.isValid());
	ASSERT_TRUE(time3.isDayReported());
	ASSERT_NE(time3.minute, 0u);

	const metaf::TimeSpanGroup group1(timeFrom, time1);
	EXPECT_FALSE(group1.isValid());

	const metaf::TimeSpanGroup group2(timeFrom, time2);
	EXPECT_FALSE(group2.isValid());

	const metaf::TimeSpanGroup group3(timeFrom, time3);
	EXPECT_FALSE(group3.isValid());
}

//Confirm that parse() method correctly parses Time Span Group
TEST(TimeSpanGroup, parseTimeSpan) {
	std::string gs("1221/1303");//from day 12 21:00 till day 13 03:00

	metaf::TimeSpanGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::HEADER)); 
	EXPECT_EQ(g1.from, metaf::MetafTime(12, 21, 00));
	EXPECT_EQ(g1.till, metaf::MetafTime(13, 03, 00));

	metaf::TimeSpanGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method does not parse Time Span Group in report parts 
//other than Header or TAF
TEST(TimeSpanGroup, parseWrongReportPart) {
	std::string gs("1221/1303");//from day 12 21:00 till day 13 03:00

	metaf::ReportTimeGroup g1;
	EXPECT_FALSE(g1.parse(gs, metaf::ReportPart::UNKNOWN));

	metaf::ReportTimeGroup g2;
	EXPECT_FALSE(g2.parse(gs, metaf::ReportPart::METAR));

	metaf::ReportTimeGroup g3;
	EXPECT_FALSE(g3.parse(gs, metaf::ReportPart::RMK));

}

//Confirm that parse() method does not parse Time Span Group if number of 
//digits is wrong
TEST(TimeSpanGroup, parseWrongDigitNumber) {
	metaf::TimeSpanGroup g1;
	EXPECT_FALSE(g1.parse("221/1303", metaf::ReportPart::HEADER)); //Too few digits in 'from'

	metaf::TimeSpanGroup g2;
	EXPECT_FALSE(g2.parse("01221/1303", metaf::ReportPart::HEADER)); //Too many digits in 'from'

	metaf::TimeSpanGroup g3;
	EXPECT_FALSE(g3.parse("1221/303", metaf::ReportPart::HEADER)); //Too few digits in 'till'

	metaf::TimeSpanGroup g4;
	EXPECT_FALSE(g4.parse("1221/01303", metaf::ReportPart::HEADER)); //Too many digits in 'till'
}

//Confirm that parse() method does not parse malformed Time Span groups
TEST(TimeSpanGroup, parseWrongMalformedGroups) {
	metaf::TimeSpanGroup g1;
	//Error::extra colon
	EXPECT_FALSE(g1.parse("12:21/13:03", metaf::ReportPart::HEADER));
	
	metaf::TimeSpanGroup g2;
	//Error::wrong separator
	EXPECT_FALSE(g2.parse("1221:1303", metaf::ReportPart::HEADER));

	metaf::TimeSpanGroup g3;
	//Error::no separator
	EXPECT_FALSE(g3.parse("12211303", metaf::ReportPart::HEADER));
}

//Confirm that parse() method does not parse other similar groups
TEST(TimeSpanGroup, parseWrongSimilarGroups) {
	metaf::TimeSpanGroup g1;
	EXPECT_FALSE(g1.parse("05025KT", metaf::ReportPart::HEADER));

	metaf::TimeSpanGroup g2;
	EXPECT_FALSE(g2.parse("201730Z", metaf::ReportPart::HEADER));

	metaf::TimeSpanGroup g3;
	EXPECT_FALSE(g3.parse("FM122230", metaf::ReportPart::HEADER));
}