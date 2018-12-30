/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

//Confirm that default groups are equal
TEST(WindGroup, defaultDataEqual) {
	const metaf::WindGroup group1;
	const metaf::WindGroup group2;
	EXPECT_TRUE(group1 == group2);
	EXPECT_FALSE(group1 != group2);
}

//Confirm that groups holding the same data (direction, speed, gust speed, 
//unit) are equal
TEST(WindGroup, sameDataEqual) {
	static const auto direction = 120;
	static const auto directionReported = true;
	static const auto windSpeed = metaf::Speed(12, metaf::Speed::Unit::METERS_PER_SECOND);
	static const auto gustSpeed = metaf::Speed(20, metaf::Speed::Unit::METERS_PER_SECOND);
	metaf::WindGroup group1;
	metaf::WindGroup group2;
	group1.direction = direction;
	group1.directionReported = directionReported;
	group1.windSpeed = windSpeed;
	group1.gustSpeed = gustSpeed;
	group2.direction = direction;
	group2.directionReported = directionReported;
	group2.windSpeed = windSpeed;
	group2.gustSpeed = gustSpeed;
	EXPECT_EQ(group1, group2);
}

//Confirm that if the wind is variable, direction does not affect the 
//comparison
TEST(WindGroup, comparisonVariableWind) {
	metaf::WindGroup group;
	group.direction = 0;
	group.directionReported = true;
	group.directionVariable = true;
	group.windSpeed = metaf::Speed(12, metaf::Speed::Unit::METERS_PER_SECOND);
	group.gustSpeed = metaf::Speed(20, metaf::Speed::Unit::METERS_PER_SECOND);
	metaf::WindGroup group1(group);
	group1.direction = 180;
	EXPECT_EQ(group, group1);
}

//Confirm that if the direction is not reported, its value does not affect the 
//comparison
TEST(WindGroup, comparisonDirectionNotReported) {
	metaf::WindGroup group;
	group.direction = 0;
	group.directionReported = false;
	group.windSpeed = metaf::Speed(12, metaf::Speed::Unit::METERS_PER_SECOND);
	group.gustSpeed = metaf::Speed(20, metaf::Speed::Unit::METERS_PER_SECOND);
	metaf::WindGroup group1(group);
	group1.direction = 180;
	EXPECT_EQ(group, group1);
}

//Confirm that groups holding the different data (direction, speed, gust 
//speed, unit) are not equal
TEST(WindGroup, differentDataNotEqual) {
	metaf::WindGroup group;
	group.direction = 120;
	group.directionReported = true;
	group.directionVariable = false;
	group.windSpeed = metaf::Speed(12, metaf::Speed::Unit::METERS_PER_SECOND);
	group.gustSpeed = metaf::Speed(20, metaf::Speed::Unit::METERS_PER_SECOND);
	metaf::WindGroup group1(group);
	group1.direction = 240;
	metaf::WindGroup group2(group);
	group2.directionReported = false;
	metaf::WindGroup group3(group);
	group3.directionVariable = true;
	metaf::WindGroup group4(group);
	group4.windSpeed = metaf::Speed(10, metaf::Speed::Unit::METERS_PER_SECOND);
	metaf::WindGroup group5(group);
	group5.gustSpeed = metaf::Speed(25, metaf::Speed::Unit::METERS_PER_SECOND);
	EXPECT_NE(group, group1);
	EXPECT_NE(group, group2);
	EXPECT_NE(group, group3);
	EXPECT_NE(group, group4);
	EXPECT_NE(group, group5);
}

//Confirm that 'not reported' constructor initialises the group with correct 
//data
TEST(WindGroup, constructorNotReported) {
	static const auto unit = metaf::Speed::Unit::KNOTS;
	const metaf::WindGroup group(unit);
	EXPECT_FALSE(group.directionReported);	
	EXPECT_EQ(group.windSpeed, metaf::Speed(unit));
	EXPECT_EQ(group.windSpeed.unit, unit);
	EXPECT_EQ(group.gustSpeed, metaf::Speed(unit));
	EXPECT_EQ(group.gustSpeed.unit, unit);
}

//Confirm that 'reported' constructor initialises the group with correct data
TEST(WindGroup, constructorReported) {
	static const auto direction = 240u;
	static const auto unit = metaf::Speed::Unit::KNOTS;
	static const auto speed = metaf::Speed(5, unit);
	static const auto gustSpeed = metaf::Speed(12, unit);

	const metaf::WindGroup group1(direction, speed);
	EXPECT_TRUE(group1.directionReported);	
	EXPECT_FALSE(group1.directionVariable);	
	EXPECT_EQ(group1.direction, direction);
	EXPECT_EQ(group1.windSpeed, speed);
	EXPECT_EQ(group1.gustSpeed, metaf::Speed(unit));

	const metaf::WindGroup group2(direction, speed, gustSpeed);
	EXPECT_TRUE(group2.directionReported);	
	EXPECT_FALSE(group2.directionVariable);	
	EXPECT_EQ(group2.direction, direction);
	EXPECT_EQ(group2.windSpeed, speed);
	EXPECT_EQ(group2.gustSpeed, gustSpeed);
}

//Confirm that 'makeVariableDirection' named constructor initialises the group 
//with correct data
TEST(WindGroup, makeVariableDirection) {
	static const auto unit = metaf::Speed::Unit::KNOTS;
	static const auto speed = metaf::Speed(5, unit);
	static const auto gustSpeed = metaf::Speed(12, unit);

	const metaf::WindGroup group1 = 
		metaf::WindGroup::makeVariableDirection(speed);
	EXPECT_TRUE(group1.directionReported);	
	EXPECT_TRUE(group1.directionVariable);
	EXPECT_EQ(group1.windSpeed, speed);
	EXPECT_EQ(group1.gustSpeed, metaf::Speed(unit));

	const metaf::WindGroup group2 = 
		metaf::WindGroup::makeVariableDirection(speed, gustSpeed);
	EXPECT_TRUE(group2.directionReported);	
	EXPECT_TRUE(group2.directionVariable);
	EXPECT_EQ(group2.windSpeed, speed);
	EXPECT_EQ(group2.gustSpeed, gustSpeed);
}

//Confirm that 'makeCalm' named constructor initialises the group with correct 
//data
TEST(WindGroup, makeCalm) {
	static const auto unit = metaf::Speed::Unit::METERS_PER_SECOND;
	const metaf::WindGroup group = metaf::WindGroup::makeCalm(unit);
	EXPECT_TRUE(group.directionReported);	
	EXPECT_FALSE(group.directionVariable);
	EXPECT_EQ(group.direction, 0u);
	EXPECT_EQ(group.windSpeed, metaf::Speed(0, unit));
	EXPECT_EQ(group.gustSpeed, metaf::Speed(unit));
}

//Confirm that 'isCalm' correctly identifies calm wind coded in wind group
TEST(WindGroup, isCalmTrue) {
	const metaf::WindGroup group1 = 
		metaf::WindGroup::makeCalm(metaf::Speed::Unit::KNOTS);
	const metaf::WindGroup group2 = 
		metaf::WindGroup::makeCalm(metaf::Speed::Unit::METERS_PER_SECOND);
	const metaf::WindGroup group3 = 
		metaf::WindGroup::makeCalm(metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	EXPECT_TRUE(group1.isCalm());	
	EXPECT_TRUE(group2.isCalm());	
	EXPECT_TRUE(group3.isCalm());	
}

//Confirm that 'isCalm' correctly identifies that no calm wind is coded in 
//wind group
TEST(WindGroup, isCalmFalse) {
	static const auto unit = metaf::Speed::Unit::KNOTS;
	static const auto direction = 80u;
	static const auto speed = metaf::Speed(5, unit);
	static const auto gustSpeed = metaf::Speed(12, unit);
	const metaf::WindGroup group1(unit);
	const metaf::WindGroup group2(direction, speed, gustSpeed);
	const metaf::WindGroup group3(direction, speed);
	const metaf::WindGroup group4(0, speed);
	const metaf::WindGroup group5 = metaf::WindGroup::makeVariableDirection(speed, gustSpeed);
	const metaf::WindGroup group6 = metaf::WindGroup::makeVariableDirection(speed);
	const metaf::WindGroup group7 = metaf::WindGroup::makeVariableDirection(metaf::Speed(0, unit));
	EXPECT_FALSE(group1.isCalm());
	EXPECT_FALSE(group2.isCalm());
	EXPECT_FALSE(group3.isCalm());
	EXPECT_FALSE(group4.isCalm());
	EXPECT_FALSE(group5.isCalm());
	EXPECT_FALSE(group6.isCalm());
	EXPECT_FALSE(group7.isCalm());
}

//Confirm that parse() method correctly parses Wind Groups with wind speed and 
//gust speed reported in knots
TEST(WindGroup, parseUnitKnots) {
	const std::string gs("18005G10KT"); //direction 180 (South), speed 5 knots, gusts 10 knots
	const auto unit = metaf::Speed::Unit::KNOTS;

	metaf::WindGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_TRUE(g1.directionReported);
	EXPECT_FALSE(g1.directionVariable);
	EXPECT_EQ(g1.direction, 180u);
	EXPECT_EQ(g1.windSpeed, metaf::Speed(5, unit));
	EXPECT_EQ(g1.gustSpeed, metaf::Speed(10, unit));

	metaf::WindGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));

	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method correctly parses Wind Groups with wind speed and 
//gust speed reported in meters per second
TEST(WindGroup, parseUnitMps) {
	const std::string gs("18005G10MPS"); //direction 180 (South), speed 5 knots, gusts 10 m/s
	const auto unit = metaf::Speed::Unit::METERS_PER_SECOND;

	metaf::WindGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_TRUE(g1.directionReported);
	EXPECT_FALSE(g1.directionVariable);
	EXPECT_EQ(g1.direction, 180u);
	EXPECT_EQ(g1.windSpeed, metaf::Speed(5, unit));
	EXPECT_EQ(g1.gustSpeed, metaf::Speed(10, unit));

	metaf::WindGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method correctly parses Wind Groups with wind speed and 
//gust speed reported in kilometers per hour
TEST(WindGroup, parseUnitKmh) {
	const std::string gs("18005G10KMH"); //direction 180 (South), speed 5 knots, gusts 10 km/h
	const auto unit = metaf::Speed::Unit::KILOMETERS_PER_HOUR;

	metaf::WindGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_TRUE(g1.directionReported);
	EXPECT_FALSE(g1.directionVariable);
	EXPECT_EQ(g1.direction, 180u);
	EXPECT_EQ(g1.windSpeed, metaf::Speed(5, unit));
	EXPECT_EQ(g1.gustSpeed, metaf::Speed(10, unit));

	metaf::WindGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method correctly parses Wind Groups with 3-digit gust 
//speed
TEST(WindGroup, parse3digitGustSpeed) {
	const std::string gs("08063G112KT"); //direction 80 (East), speed 63 knots, gusts 112 knots
	const auto unit = metaf::Speed::Unit::KNOTS;

	metaf::WindGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_TRUE(g1.directionReported);
	EXPECT_FALSE(g1.directionVariable);
	EXPECT_EQ(g1.direction, 80u);
	EXPECT_EQ(g1.windSpeed, metaf::Speed(63, unit));
	EXPECT_EQ(g1.gustSpeed, metaf::Speed(112, unit));

	metaf::WindGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method correctly parses Wind Groups with 3-digit wind 
//speed and 3-digit gust speed
TEST(WindGroup, parse3digitWindGustSpeed) {
	const std::string gs("080100G150KT"); //direction 80 (East), speed 100 knots, gusts 150 knots
	const auto unit = metaf::Speed::Unit::KNOTS;

	metaf::WindGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_TRUE(g1.directionReported);
	EXPECT_FALSE(g1.directionVariable);
	EXPECT_EQ(g1.direction, 80u);
	EXPECT_EQ(g1.windSpeed, metaf::Speed(100, unit));
	EXPECT_EQ(g1.gustSpeed, metaf::Speed(150, unit));

	metaf::WindGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method correctly parses Wind Groups with gust speed not 
//reported
TEST(WindGroup, parseGustSpeedNotReported) {
	const std::string gs("18005KT"); //direction 180 (South), speed 5 knots, no gusts

	metaf::WindGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_TRUE(g1.directionReported);
	EXPECT_FALSE(g1.directionVariable);
	EXPECT_EQ(g1.direction, 180u);
	EXPECT_EQ(g1.windSpeed, metaf::Speed(5, metaf::Speed::Unit::KNOTS));
	EXPECT_EQ(g1.gustSpeed, metaf::Speed());

	metaf::WindGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method correctly parses Wind Groups with variable direction and 
//gust speed reported
TEST(WindGroup, parseVariableDirectionWithGusts) {
	const std::string gs("VRB15G25KT"); //variable direction, speed 15 knots, gusts 25 knots
	const auto unit = metaf::Speed::Unit::KNOTS;

	metaf::WindGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_TRUE(g1.directionReported);
	EXPECT_TRUE(g1.directionVariable);
	EXPECT_EQ(g1.windSpeed, metaf::Speed(15, unit));
	EXPECT_EQ(g1.gustSpeed, metaf::Speed(25, unit));

	metaf::WindGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method correctly parses Wind Groups with variable direction and 
//gust speed not reported
TEST(WindGroup, parseVariableDirection) {
	const std::string gs("VRB06KT"); //variable direction, speed 15 knots, gusts 25 knots

	metaf::WindGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_TRUE(g1.directionReported);
	EXPECT_TRUE(g1.directionVariable);
	EXPECT_EQ(g1.windSpeed, metaf::Speed(6, metaf::Speed::Unit::KNOTS));
	EXPECT_EQ(g1.gustSpeed, metaf::Speed());

	metaf::WindGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method correctly parses Wind Groups with direction, speed and 
//gust speed not reported
TEST(WindGroup, parseNotReported) {
	const std::string gs("/////KT");	//direction and speed not reported

	metaf::WindGroup g1;
	EXPECT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	EXPECT_FALSE(g1.directionReported);
	EXPECT_EQ(g1.windSpeed, metaf::Speed());
	EXPECT_EQ(g1.gustSpeed, metaf::Speed());

	metaf::WindGroup g2;
	EXPECT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));
	EXPECT_EQ(g2, g1);
}

//Confirm that parse() method correctly parses Wind Groups which means calm wind
TEST(WindGroup, parseCalmWind) {
	const std::string gs1("00000KT");	//calm wind (i.e. no wind), speed unit is knots
	const std::string gs2("00000MPS");	//calm wind (i.e. no wind), speed unit is m/s

	metaf::WindGroup g1;
	EXPECT_TRUE(g1.parse(gs1, metaf::ReportPart::METAR));
	EXPECT_TRUE(g1.isCalm());

	metaf::WindGroup g2;
	EXPECT_TRUE(g2.parse(gs1, metaf::ReportPart::TAF));
	EXPECT_TRUE(g2.isCalm());

	metaf::WindGroup g3;
	EXPECT_TRUE(g3.parse(gs2, metaf::ReportPart::METAR));
	EXPECT_TRUE(g3.isCalm());

	metaf::WindGroup g4;
	EXPECT_TRUE(g4.parse(gs2, metaf::ReportPart::TAF));
	EXPECT_TRUE(g4.isCalm());
}

//Confirm that parse() method does not parse Wind Groups in the following 
//report parts:: unknown, header, remarks
TEST(WindGroup, parseWrongReportPart) {
	const std::string gs("18005G10KT"); //direction 180 (South), speed 5 knots, gusts 10 knots

	metaf::WindGroup g1;
	ASSERT_TRUE(g1.parse(gs, metaf::ReportPart::METAR));
	metaf::WindGroup g2;
	ASSERT_TRUE(g2.parse(gs, metaf::ReportPart::TAF));

	metaf::WindGroup g3;
	EXPECT_FALSE(g3.parse(gs, metaf::ReportPart::UNKNOWN));
	metaf::WindGroup g4;
	EXPECT_FALSE(g4.parse(gs, metaf::ReportPart::HEADER));
	metaf::WindGroup g5;
	EXPECT_FALSE(g5.parse(gs, metaf::ReportPart::RMK));
}

//Confirm that parse() method does not parse Wind Groups if units are other 
//than KT, MPS or KMH
TEST(WindGroup, parseWrongUnit) {
	//Error: unit is KKT
	metaf::WindGroup g;
	EXPECT_FALSE(g.parse("18005G10KKT", metaf::ReportPart::METAR));
}

//Confirm that parse() method does not parse Wind Groups if direction last 
//digit is non-zero
TEST(WindGroup, parseWrongDirection) {
	//Error: direction is 182 but last digit must be zero
	metaf::WindGroup g;
	EXPECT_FALSE(g.parse("18206KT", metaf::ReportPart::METAR));
}

//Confirm that parse() method does not parse Wind Groups if digit number is wrong
TEST(WindGroup, parseWrongDigitNumber) {
	//Error: too few digits in direction / speed
	metaf::WindGroup g1;
	EXPECT_FALSE(g1.parse("1800KT", metaf::ReportPart::METAR)); 

	//Error: too few digits in direction / speed, gust speed reported
	metaf::WindGroup g2;
	EXPECT_FALSE(g2.parse("1800G15KT", metaf::ReportPart::METAR));

	//Error: too many digits in direction / speed
	metaf::WindGroup g3;
	EXPECT_FALSE(g3.parse("1800001KT", metaf::ReportPart::METAR));

	//Error: too many digits in direction / speed, gust speed reported
	metaf::WindGroup g4;
	EXPECT_FALSE(g4.parse("1800001G15KT", metaf::ReportPart::METAR));

	//Error: too few digits in gust speed
	metaf::WindGroup g5;
	EXPECT_FALSE(g5.parse("18005G1KT", metaf::ReportPart::METAR));

	//Error: too many digits in gust speed
	metaf::WindGroup g6;
	EXPECT_FALSE(g6.parse("18005G0011KT", metaf::ReportPart::METAR));
}

//Confirm that parse() method does not parse malformed Wind Groups
TEST(WindGroup, parseWrongMalformedGroups) {
	metaf::WindGroup g1;
	EXPECT_FALSE(g1.parse("180/05KT", metaf::ReportPart::METAR)); 

	metaf::WindGroup g2;
	EXPECT_FALSE(g2.parse("180-05-KT", metaf::ReportPart::METAR)); 

	metaf::WindGroup g3;
	EXPECT_FALSE(g3.parse("VVV00KT", metaf::ReportPart::METAR)); 

	metaf::WindGroup g4;
	EXPECT_FALSE(g4.parse("VRB0QKT", metaf::ReportPart::METAR)); 

	metaf::WindGroup g5;
	EXPECT_FALSE(g5.parse("18005H15KT", metaf::ReportPart::METAR)); 
}

//Confirm that parse() method does not parse other similar groups
TEST(WindGroup, parseWrongSimilarGroups) {
	metaf::WindGroup g1;
	EXPECT_FALSE(g1.parse("1800", metaf::ReportPart::METAR)); 

	metaf::WindGroup g2;
	EXPECT_FALSE(g2.parse("WS020/05025KT", metaf::ReportPart::METAR)); 

	metaf::WindGroup g3;
	EXPECT_FALSE(g3.parse("BKN007CB", metaf::ReportPart::METAR)); 
}