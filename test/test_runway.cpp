/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

//Confirm that default Runway structs are equal
TEST(Runway, defaultEqual) {
	const metaf::Runway r1;
	const metaf::Runway r2;
	EXPECT_TRUE(r1 == r2);
	EXPECT_FALSE(r1 != r2);
}

//Confirm that Runway structs initialised with the same data are equal
TEST(Runway, sameDataEqual) {
	metaf::Runway r1;
	r1.number = 21;
	r1.designator = metaf::Runway::Designator::LEFT;
	metaf::Runway r2;
	r2.number = 21;
	r2.designator = metaf::Runway::Designator::LEFT;
	EXPECT_EQ(r1, r2);
}

//Confirm that Runway structs initialised with the different data are not equal
TEST(Runway, differentDataNotEqual) {
	metaf::Runway r;
	r.number = 21;
	r.designator = metaf::Runway::Designator::LEFT;
	metaf::Runway r1(r);
	r1.number = 22;
	metaf::Runway r2(r);
	r2.designator = metaf::Runway::Designator::NONE;
	EXPECT_NE(r, r1);
	EXPECT_NE(r, r2);
}

//Confirm that constructor initialises the Runway struct with correct data 
//when runway number is from 0 to 35
TEST(Runway, constructorNumber0to35) {
	const metaf::Runway r1(25, metaf::Runway::Designator::LEFT);
	EXPECT_EQ(r1.number, 25u);
	EXPECT_EQ(r1.designator, metaf::Runway::Designator::LEFT);
	const metaf::Runway r2(0, metaf::Runway::Designator::CENTER);
	EXPECT_EQ(r2.number, 0u);
	EXPECT_EQ(r2.designator, metaf::Runway::Designator::CENTER);
	const metaf::Runway r3(35, metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(r3.number, 35u);
	EXPECT_EQ(r3.designator, metaf::Runway::Designator::RIGHT);
	const metaf::Runway r4(17);
	EXPECT_EQ(r4.number, 17u);
	EXPECT_EQ(r4.designator, metaf::Runway::Designator::NONE);
}

//Confirm that constructor initialises the Runway struct with correct data 
//when runway number is from 50 to 85 and that designator is forced to RIGHT
TEST(Runway, constructor50to85) {
	const metaf::Runway r1(50);
	EXPECT_EQ(r1.number, 0u);
	EXPECT_EQ(r1.designator, metaf::Runway::Designator::RIGHT);
	const metaf::Runway r2(85);
	EXPECT_EQ(r2.number, 35u);
	EXPECT_EQ(r2.designator, metaf::Runway::Designator::RIGHT);
	const metaf::Runway r3(75, metaf::Runway::Designator::CENTER);
	EXPECT_EQ(r3.number, 25u);
	EXPECT_EQ(r3.designator, metaf::Runway::Designator::RIGHT);
}

//Confirm that makeAllRunways initialises the Runway struct with correct data
TEST(Runway, makeAllRunways) {
	static const unsigned int allRunwaysNumber = 88;
	const metaf::Runway r = metaf::Runway::makeAllRunways();
	EXPECT_EQ(r.number, allRunwaysNumber);
	EXPECT_EQ(r.designator, metaf::Runway::Designator::NONE);
}

//Confirm that makeMessageRepetition initialises the Runway struct with correct data
TEST(Runway, makeMessageRepetition) {
	static const unsigned int messageRepetitionNumber = 99;	
	const metaf::Runway r = metaf::Runway::makeMessageRepetition();
	EXPECT_EQ(r.number, messageRepetitionNumber);
	EXPECT_EQ(r.designator, metaf::Runway::Designator::NONE);
}

//Confirm that designatorFromChar correctly decodes a runway designator
TEST(Runway, designatorFromChar) {
	EXPECT_EQ(metaf::Runway::designatorFromChar('R'), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(metaf::Runway::designatorFromChar('C'), metaf::Runway::Designator::CENTER);
	EXPECT_EQ(metaf::Runway::designatorFromChar('L'), metaf::Runway::Designator::LEFT);
	EXPECT_EQ(metaf::Runway::designatorFromChar(' '), metaf::Runway::Designator::NONE);
	EXPECT_EQ(metaf::Runway::designatorFromChar('\0'), metaf::Runway::Designator::NONE);
	EXPECT_EQ(metaf::Runway::designatorFromChar('M'), metaf::Runway::Designator::UNKNOWN);
	EXPECT_EQ(metaf::Runway::designatorFromChar('5'), metaf::Runway::Designator::UNKNOWN);
}

//Confirm that designatorFromString correctly decodes a runway designator
TEST(Runway, designatorFromString) {
	EXPECT_EQ(metaf::Runway::designatorFromString("R"), metaf::Runway::Designator::RIGHT);
	EXPECT_EQ(metaf::Runway::designatorFromString("C"),	metaf::Runway::Designator::CENTER);
	EXPECT_EQ(metaf::Runway::designatorFromString("L"),	metaf::Runway::Designator::LEFT);
	EXPECT_EQ(metaf::Runway::designatorFromString(" "),	metaf::Runway::Designator::NONE);
	EXPECT_EQ(metaf::Runway::designatorFromString("\0"), metaf::Runway::Designator::NONE);
	EXPECT_EQ(metaf::Runway::designatorFromString(""), metaf::Runway::Designator::NONE);
	EXPECT_EQ(metaf::Runway::designatorFromString("X"), metaf::Runway::Designator::UNKNOWN);
	EXPECT_EQ(metaf::Runway::designatorFromString("RR"), metaf::Runway::Designator::UNKNOWN);
}

//Confirm that isAllRunways correctly indicates "all runways" code present
TEST(Runway, isAllRunwaysTrue) {
	const metaf::Runway r1 = metaf::Runway::makeAllRunways();
	const metaf::Runway r2(88, metaf::Runway::Designator::NONE);
	EXPECT_TRUE(r1.isAllRunways());
	EXPECT_TRUE(r2.isAllRunways());
}

//Confirm that isAllRunways correctly indicates when "all runways" code is 
//absent or malformed
TEST(Runway, isAllRunwaysFalse) {
	const metaf::Runway r1(25, metaf::Runway::Designator::LEFT);
	const metaf::Runway r2(25);
	const metaf::Runway r3(88, metaf::Runway::Designator::UNKNOWN);
	const metaf::Runway r4(88, metaf::Runway::Designator::LEFT);
	const metaf::Runway r5 = metaf::Runway::makeMessageRepetition();
	EXPECT_FALSE(r1.isAllRunways());
	EXPECT_FALSE(r2.isAllRunways());
	EXPECT_FALSE(r3.isAllRunways());
	EXPECT_FALSE(r4.isAllRunways());
	EXPECT_FALSE(r5.isAllRunways());
}

//Confirm that isMessageRepetition correctly indicates "last message repetition" code 
//present
TEST(Runway, isMessageRepetitionTrue) {
	const metaf::Runway r1 = metaf::Runway::makeMessageRepetition();
	const metaf::Runway r2(99, metaf::Runway::Designator::NONE);
	EXPECT_TRUE(r1.isMessageRepetition());
	EXPECT_TRUE(r2.isMessageRepetition());
}

//Confirm that isMessageRepetition correctly indicates when "all runways" code is 
//absent or malformed
TEST(Runway, isMessageRepetitionFalse) {
	const metaf::Runway r1(25, metaf::Runway::Designator::LEFT);
	const metaf::Runway r2(25);
	const metaf::Runway r3(99, metaf::Runway::Designator::UNKNOWN);
	const metaf::Runway r4(99, metaf::Runway::Designator::LEFT);
	const metaf::Runway r5 = metaf::Runway::makeAllRunways();
	EXPECT_FALSE(r1.isMessageRepetition());
	EXPECT_FALSE(r2.isMessageRepetition());
	EXPECT_FALSE(r3.isMessageRepetition());
	EXPECT_FALSE(r4.isMessageRepetition());
	EXPECT_FALSE(r5.isMessageRepetition());
}

//Confirm that isValid returns true if runway number is correct
TEST(Runway, isValidRunwayNumberCorrect) {
	const metaf::Runway r1(0);
	const metaf::Runway r2(22);
	const metaf::Runway r3(35);
	const metaf::Runway r4(50);
	const metaf::Runway r5(72);
	const metaf::Runway r6(85);
	const metaf::Runway r7 = metaf::Runway::makeAllRunways();
	const metaf::Runway r8 = metaf::Runway::makeMessageRepetition();
	EXPECT_TRUE(r1.isValid());
	EXPECT_TRUE(r2.isValid());
	EXPECT_TRUE(r3.isValid());
	EXPECT_TRUE(r4.isValid());
	EXPECT_TRUE(r5.isValid());
	EXPECT_TRUE(r6.isValid());
	EXPECT_TRUE(r7.isValid());
	EXPECT_TRUE(r8.isValid());
}

//Confirm that isValid returns false if runway number is incorrect
TEST(Runway, isValidRunwayNumberIncorrect) {
	const metaf::Runway r1(36);
	const metaf::Runway r2(49);
	const metaf::Runway r3(86);
	const metaf::Runway r4(87);
	const metaf::Runway r5(89);
	const metaf::Runway r6(98);
	EXPECT_FALSE(r1.isValid());
	EXPECT_FALSE(r2.isValid());
	EXPECT_FALSE(r3.isValid());
	EXPECT_FALSE(r4.isValid());
	EXPECT_FALSE(r5.isValid());
	EXPECT_FALSE(r6.isValid());
}

//Confirm that isValid returns true if runway designator is either LEFT, 
//CENTER, RIGHT or NONE
TEST(Runway, isValidRunwayDesignatorCorrect) {
	const metaf::Runway r1(25, metaf::Runway::Designator::LEFT);
	const metaf::Runway r2(25, metaf::Runway::Designator::CENTER);
	const metaf::Runway r3(25, metaf::Runway::Designator::RIGHT);
	const metaf::Runway r4(25, metaf::Runway::Designator::NONE);
	EXPECT_TRUE(r1.isValid());
	EXPECT_TRUE(r2.isValid());
	EXPECT_TRUE(r3.isValid());
	EXPECT_TRUE(r4.isValid());
}

//Confirm that isValid returns false if runway designator is unknown
TEST(Runway, isValidRunwayDesignatorIncorrect) {
	const metaf::Runway r1(25, metaf::Runway::Designator::UNKNOWN);
	EXPECT_FALSE(r1.isValid());

	metaf::Runway r2 = metaf::Runway::makeAllRunways();
	r2.designator = metaf::Runway::Designator::UNKNOWN;
	EXPECT_FALSE(r2.isValid());

	metaf::Runway r3 = metaf::Runway::makeMessageRepetition();
	r3.designator = metaf::Runway::Designator::UNKNOWN;
	EXPECT_FALSE(r3.isValid());
}