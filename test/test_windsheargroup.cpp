/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

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
	static const auto speed = metaf::Speed(20, metaf::Speed::Unit::KNOTS);
	metaf::WindShearGroup group1;
	metaf::WindShearGroup group2;
	group1.direction = direction;
	group1.height = height;
	group1.windSpeed = speed;
	group2.direction = direction;
	group2.height = height;
	group2.windSpeed = speed;
	EXPECT_EQ(group1, group2);
}

//Confirm that groups holding the different values are not equal 
TEST(WindShearGroup, differentDataNotEqual) {
	metaf::WindShearGroup group;
	group.direction = 240;
	group.height = 4000;
	group.windSpeed = metaf::Speed(20, metaf::Speed::Unit::KNOTS);
	metaf::WindShearGroup group1(group);
	group1.direction = 180;
	metaf::WindShearGroup group2(group);
	group2.height = 6000;
	metaf::WindShearGroup group3(group);
	group3.windSpeed = metaf::Speed(22, metaf::Speed::Unit::KNOTS);
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
}

//Confirm that constructor initialises the group with correct data
TEST(WindShearGroup, constructor) {
	static const auto direction = 240u;
	static const auto height = 4000u;
	static const auto speed = metaf::Speed(20, metaf::Speed::Unit::KNOTS);
	const metaf::WindShearGroup group(height, direction, speed);
	EXPECT_EQ(group.height, height);
	EXPECT_EQ(group.direction, direction);
	EXPECT_EQ(group.windSpeed, speed);
}

//Confirm that parse() method parses wind shear group with wind speed in knots 
TEST(WindShearGroup, parseWindShearKnots) {
	const std::string gs("WS020/05065KT"); //wind shear at 2000 feet: wind 50 degrees & 65 knots

	metaf::WindShearGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_EQ(g1.height, 2000u);
	EXPECT_EQ(g1.direction, 50u);
	EXPECT_EQ(g1.windSpeed, metaf::Speed(65, metaf::Speed::Unit::KNOTS));

	metaf::WindShearGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method parses wind shear group with wind speed in m/s
TEST(WindShearGroup, parseWindShearMps) {
	const std::string gs("WS020/05035MPS"); //wind shear at 2000 feet: wind 50 degrees & 35 knots

	metaf::WindShearGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_EQ(g1.height, 2000u);
	EXPECT_EQ(g1.direction, 50u);
	EXPECT_EQ(g1.windSpeed, metaf::Speed(35, metaf::Speed::Unit::METERS_PER_SECOND));

	metaf::WindShearGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method does not parse Wind Shear Groups in the 
//following report parts:: unknown, header, remarks
TEST(WindShearGroup, parseWrongReportPart) {
	const std::string gs("WS020/05065KT"); //wind shear at 2000 feet: wind 50 degrees & 65 knots

	metaf::WindShearGroup g1;
	ASSERT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	metaf::WindShearGroup g2;
	ASSERT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));

	metaf::WindShearGroup g3;
	EXPECT_FALSE(g3.parse(gs, metaf::ReportPart::UNKNOWN));
	metaf::WindShearGroup g4;
	EXPECT_FALSE(g4.parse(gs, metaf::ReportPart::HEADER));
	metaf::WindShearGroup g5;
	EXPECT_FALSE(g5.parse(gs, metaf::ReportPart::RMK));
}

//Confirm that parse() method does not parse Wind Shear Groups if the units are 
//other than KT, MPS or KMH
TEST(WindShearGroup, parseWrongUnit) {
	//Error: unit is KTT
	metaf::WindShearGroup g1;
	EXPECT_FALSE(g1.parse("WS020/05165KTT", metaf::ReportPart::METAR));
}


//Confirm that parse() method does not parse Wind Shear Groups if direction 
//last digit is not zero
TEST(WindShearGroup, parseWrongDirection) {
	//Error: direction last digit must be zero
	metaf::WindShearGroup g;
	EXPECT_FALSE(g.parse("WS020/05165KT", metaf::ReportPart::METAR));
}

//Confirm that parse() method does not parse Wind Shear Groups if digit number 
//is wrong
TEST(WindShearGroup, parseWrongDigitNumber) {
	//Error: too few digits in height
	metaf::WindShearGroup g1;
	EXPECT_FALSE(g1.parse("WS20/05015KT", metaf::ReportPart::METAR));

	//Error: too many digits in height
	metaf::WindShearGroup g2;
	EXPECT_FALSE(g2.parse("WS0200/05015KT", metaf::ReportPart::METAR));

	//Error: too few digits in direction/speed
	metaf::WindShearGroup g3;
	EXPECT_FALSE(g3.parse("WS020/0505KT", metaf::ReportPart::METAR));

	//Error: too many digits in direction/speed
	metaf::WindShearGroup g4;
	EXPECT_FALSE(g4.parse("WS020/0500005KT", metaf::ReportPart::METAR));
}

//Confirm that parse() method does not parse malformed Wind Shear Groups
TEST(WindShearGroup, parseWrongMalformedGroups) {
	metaf::WindShearGroup g1;
	EXPECT_FALSE(g1.parse("WS020/05015G25KT", metaf::ReportPart::METAR));

	metaf::WindShearGroup g2;
	EXPECT_FALSE(g2.parse("WS/05015KT", metaf::ReportPart::METAR));

	metaf::WindShearGroup g3;
	EXPECT_FALSE(g3.parse("WS02005015KT", metaf::ReportPart::METAR));

	metaf::WindShearGroup g4;
	EXPECT_FALSE(g4.parse("WS020/050/15/KT", metaf::ReportPart::METAR));
}

//Confirm that parse() method does not parse other similar groups
TEST(WindShearGroup, parseWrongSimilarGroups) {
	metaf::WindShearGroup g1;
	EXPECT_FALSE(g1.parse("1800", metaf::ReportPart::METAR)); 

	metaf::WindShearGroup g2;
	EXPECT_FALSE(g2.parse("05025KT", metaf::ReportPart::METAR)); 

	metaf::WindShearGroup g3;
	EXPECT_FALSE(g3.parse("05025G30KT", metaf::ReportPart::METAR)); 

	metaf::WindShearGroup g4;
	EXPECT_FALSE(g4.parse("BKN007CB", metaf::ReportPart::METAR)); 
}