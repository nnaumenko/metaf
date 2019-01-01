/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

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
	static const auto time = metaf::MetafTime(17, 30);
	metaf::TrendTimeGroup group1;
	metaf::TrendTimeGroup group2;
	group1.type = type;
	group1.time = time;
	group2.type = type;
	group2.time = time;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding different data are not equal
TEST(TrendTimeGroup, differentDataNotEqual) {
	metaf::TrendTimeGroup group;
	group.type = metaf::TrendTimeGroup::Type::FROM;
	group.time = metaf::MetafTime(28, 17, 30);
	metaf::TrendTimeGroup group1(group);
	group1.type = metaf::TrendTimeGroup::Type::AT;
	metaf::TrendTimeGroup group2(group);
	group2.time = metaf::MetafTime(28, 17, 31);
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
}

//Confirm that constructor initialises the group with correct data
TEST(TrendTimeGroup, constructor) {
	static const auto type = metaf::TrendTimeGroup::Type::FROM;
	static const auto time = metaf::MetafTime(28, 17, 30);
	const metaf::TrendTimeGroup group(type, time);
	EXPECT_EQ(group.type, type);
	EXPECT_EQ(group.time, time);
}

//Confirm that isValid() returns true when trend time group is correctly initialised
TEST(TrendTimeGroup, isValidCorrect) {
	const metaf::TrendTimeGroup group1(metaf::TrendTimeGroup::Type::FROM, 
		metaf::MetafTime(28, 17, 30));
	ASSERT_TRUE(group1.time.isValid());

	const metaf::TrendTimeGroup group2(metaf::TrendTimeGroup::Type::FROM, 
		metaf::MetafTime(17, 30));
	ASSERT_TRUE(group2.time.isValid());

	EXPECT_TRUE(group1.isValid());
	EXPECT_TRUE(group1.isValid());
}

//Confirm that isValid() returns false when trend time group is invalid or 
//trend type is unknown
TEST(TrendTimeGroup, isValidIncorrect) {
	const metaf::TrendTimeGroup group1(metaf::TrendTimeGroup::Type::FROM, 
		metaf::MetafTime(32, 17, 30));
	ASSERT_FALSE(group1.time.isValid());

	const metaf::TrendTimeGroup group2(metaf::TrendTimeGroup::Type::UNKNOWN, 
		metaf::MetafTime(28, 17, 30));
	ASSERT_TRUE(group2.time.isValid());

	EXPECT_FALSE(group1.isValid());
	EXPECT_FALSE(group1.isValid());
}

//Confirm that parse() method correctly parses a Long Trend Time group
TEST(TrendTimeGroup, parseLong) {
	const std::string gsFrom("FM191445");	//from day 19, 14:45
	const std::string gsTill("TL010300");	//till day 1, 03:00
	const std::string gsAt("AT121530");		//at day 12, 15:30

	metaf::TrendTimeGroup g1;
	EXPECT_TRUE(g1.parse(gsFrom, metaf::ReportPart::METAR)); 
	EXPECT_EQ(g1.type, metaf::TrendTimeGroup::Type::FROM);
	EXPECT_EQ(g1.time, metaf::MetafTime(19, 14, 45));

	metaf::TrendTimeGroup g2;
	EXPECT_TRUE(g2.parse(gsFrom, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);

	metaf::TrendTimeGroup g3;
	EXPECT_TRUE(g3.parse(gsTill, metaf::ReportPart::METAR)); 
	EXPECT_EQ(g3.type, metaf::TrendTimeGroup::Type::TILL);
	EXPECT_EQ(g3.time, metaf::MetafTime(1, 3, 0));

	metaf::TrendTimeGroup g4;
	EXPECT_TRUE(g4.parse(gsTill, metaf::ReportPart::TAF));
	EXPECT_EQ(g4, g3);

	metaf::TrendTimeGroup g5;
	EXPECT_TRUE(g5.parse(gsAt, metaf::ReportPart::METAR)); 
	EXPECT_EQ(g5.type, metaf::TrendTimeGroup::Type::AT);
	EXPECT_EQ(g5.time, metaf::MetafTime(12, 15, 30));

	metaf::TrendTimeGroup g6;
	EXPECT_TRUE(g6.parse(gsAt, metaf::ReportPart::TAF));
	EXPECT_EQ(g6, g5);
}

//Confirm that parse() method correctly parses a Short Trend Time group (no 
//day reported)
TEST(TrendTimeGroup, parseShort) {
	const std::string gsFrom("FM1445");	//from 14:45
	const std::string gsTill("TL0300");	//till 03:00
	const std::string gsAt("AT1530");	//at 15:30

	metaf::TrendTimeGroup g1;
	EXPECT_TRUE(g1.parse(gsFrom, metaf::ReportPart::METAR)); 
	EXPECT_EQ(g1.type, metaf::TrendTimeGroup::Type::FROM);
	EXPECT_EQ(g1.time, metaf::MetafTime(14, 45));

	metaf::TrendTimeGroup g2;
	EXPECT_TRUE(g2.parse(gsFrom, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);

	metaf::TrendTimeGroup g3;
	EXPECT_TRUE(g3.parse(gsTill, metaf::ReportPart::METAR)); 
	EXPECT_EQ(g3.type, metaf::TrendTimeGroup::Type::TILL);
	EXPECT_EQ(g3.time, metaf::MetafTime(3, 0));

	metaf::TrendTimeGroup g4;
	EXPECT_TRUE(g4.parse(gsTill, metaf::ReportPart::TAF));
	EXPECT_EQ(g4, g3);

	metaf::TrendTimeGroup g5;
	EXPECT_TRUE(g5.parse(gsAt, metaf::ReportPart::METAR)); 
	EXPECT_EQ(g5.type, metaf::TrendTimeGroup::Type::AT);
	EXPECT_EQ(g5.time, metaf::MetafTime(15, 30));

	metaf::TrendTimeGroup g6;
	EXPECT_TRUE(g6.parse(gsAt, metaf::ReportPart::TAF));
	EXPECT_EQ(g6, g5);
}

//Confirm that parse() method does not parse Trend Time group in the following 
//report parts:: unknown, header, remarks
TEST(TrendTimeGroup, parseWrongReportPart) {
	const std::string gs("FM1445"); //from 14:45

	metaf::TrendTimeGroup g1;
	ASSERT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	metaf::TrendTimeGroup g2;
	ASSERT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));

	metaf::TrendTimeGroup g3;
	EXPECT_FALSE(g3.parse(gs, metaf::ReportPart::UNKNOWN));
	metaf::TrendTimeGroup g4;
	EXPECT_FALSE(g4.parse(gs, metaf::ReportPart::HEADER));
	metaf::TrendTimeGroup g5;
	EXPECT_FALSE(g5.parse(gs, metaf::ReportPart::RMK));
}

//Confirm that parse() method does not parse Trend Time group if the trend type
//is other than FM, TL, AT
TEST(TrendTimeGroup, parseWrongTrendType) {
	metaf::TrendTimeGroup g1;
	EXPECT_FALSE(g1.parse("FF051025", metaf::ReportPart::METAR)); //Error: type is FF

	metaf::TrendTimeGroup g2;
	EXPECT_FALSE(g2.parse("FF1025", metaf::ReportPart::METAR)); //Error: type is FF
}

//Confirm that parse() method does not parse Report Time Groups if number of 
//digits is wrong
TEST(TrendTimeGroup, parseWrongDigitNumber) {
	metaf::TrendTimeGroup g1;
	EXPECT_FALSE(g1.parse("FM025", metaf::ReportPart::METAR)); //Error: too few digits

	metaf::TrendTimeGroup g2;
	//Error: too many digits for short group and too few digit for long group
	EXPECT_FALSE(g2.parse("FF51025", metaf::ReportPart::METAR)); 

	metaf::TrendTimeGroup g3;
	EXPECT_FALSE(g3.parse("FF0051025", metaf::ReportPart::METAR)); //Error: too many digits
}

//Confirm that parse() method does not parse malformed Trend Time groups
TEST(TrendTimeGroup, parseWrongMalformedGroups) {
	metaf::TrendTimeGroup g1;
	EXPECT_FALSE(g1.parse("FM/05/1025", metaf::ReportPart::METAR)); 

	metaf::TrendTimeGroup g2;
	EXPECT_FALSE(g2.parse("FM05/10:25", metaf::ReportPart::METAR)); 
}

//Confirm that parse() method does not parse other similar groups
TEST(TrendTimeGroup, parseWrongSimilarGroups) {
	metaf::TrendTimeGroup g1;
	EXPECT_FALSE(g1.parse("201730Z", metaf::ReportPart::METAR)); 

	metaf::TrendTimeGroup g2;
	EXPECT_FALSE(g2.parse("TX00/0819", metaf::ReportPart::METAR)); 

	metaf::TrendTimeGroup g3;
	EXPECT_FALSE(g3.parse("0412/0421", metaf::ReportPart::METAR)); 
}