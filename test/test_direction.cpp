/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Test parsing of cardinal direction string, NDV, OHD, ALQDS, UNKNOWN
// Purpose: to confirm that fromCardinalString() method correctly parses 
// strings which contain cardinal directions (N, S, E, W, NE, NW, SE, SW) or
// special strings NDV, OHD, ALQDS, UNKNOWN, and that other strings cannot be
// parsed
///////////////////////////////////////////////////////////////////////////////

TEST(Direction, fromCardinalStringN) {
	const auto d = metaf::Direction::fromCardinalString("N");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 360u);
	EXPECT_EQ(d->type(), metaf::Direction::Type::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringNW) {
	const auto d = metaf::Direction::fromCardinalString("NW");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 315u);
	EXPECT_EQ(d->type(), metaf::Direction::Type::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringW) {
	const auto d = metaf::Direction::fromCardinalString("W");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 270u);
	EXPECT_EQ(d->type(), metaf::Direction::Type::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringSW) {
	const auto d = metaf::Direction::fromCardinalString("SW");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 225u);
	EXPECT_EQ(d->type(), metaf::Direction::Type::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringS) {
	const auto d = metaf::Direction::fromCardinalString("S");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 180u);
	EXPECT_EQ(d->type(), metaf::Direction::Type::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringSE) {
	const auto d = metaf::Direction::fromCardinalString("SE");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 135u);
	EXPECT_EQ(d->type(), metaf::Direction::Type::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringE) {
	const auto d = metaf::Direction::fromCardinalString("E");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 90u);
	EXPECT_EQ(d->type(), metaf::Direction::Type::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringNE) {
	const auto d = metaf::Direction::fromCardinalString("NE");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->degrees(), 45u);
	EXPECT_EQ(d->type(), metaf::Direction::Type::VALUE_CARDINAL);
}

TEST(Direction, fromCardinalStringNDV) {
	const auto d = metaf::Direction::fromCardinalString("NDV");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->type(), metaf::Direction::Type::NDV);
}

TEST(Direction, fromCardinalStringOhd) {
	const auto d = metaf::Direction::fromCardinalString("OHD", true);
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->type(), metaf::Direction::Type::OVERHEAD);

	EXPECT_FALSE(metaf::Direction::fromCardinalString("OHD").has_value());
}

TEST(Direction, fromCardinalStringAlqds) {
	const auto d = metaf::Direction::fromCardinalString("ALQDS", true);
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->type(), metaf::Direction::Type::ALQDS);

	EXPECT_FALSE(metaf::Direction::fromCardinalString("ALQDS").has_value());
}

TEST(Direction, fromCardinalStringUnknown) {
	const auto d = metaf::Direction::fromCardinalString("UNKNOWN", false, true);
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->type(), metaf::Direction::Type::UNKNOWN);

	EXPECT_FALSE(metaf::Direction::fromCardinalString("UNKNOWN", false, false).has_value());
}

TEST(Direction, fromCardinalStringSpecialOther) {
	EXPECT_FALSE(metaf::Direction::fromCardinalString("///").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("VRB").has_value());
}

TEST(Direction, fromCardinalStringOther) {
	EXPECT_FALSE(metaf::Direction::fromCardinalString("").has_value());

	EXPECT_FALSE(metaf::Direction::fromCardinalString("AAA").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("212").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("SDV").has_value());

	EXPECT_FALSE(metaf::Direction::fromCardinalString("WS").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("ES").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("WN").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("EN").has_value());

	EXPECT_FALSE(metaf::Direction::fromCardinalString("EW").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("WE").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("NS").has_value());
	EXPECT_FALSE(metaf::Direction::fromCardinalString("SN").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of degrees string, non-reported, VRB 
// Purpose: to confirm that fromDegreesString() method correctly parses 
// strings which contain direction in degrees, non-reported value, variable
// direction indicator, and that other strings cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(Direction, fromDegreesString) {
	const auto d1 = metaf::Direction::fromDegreesString("010");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->degrees(), 10u);
	EXPECT_EQ(d1->type(), metaf::Direction::Type::VALUE_DEGREES);

	const auto d2 = metaf::Direction::fromDegreesString("270");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->degrees(), 270u);
	EXPECT_EQ(d2->type(), metaf::Direction::Type::VALUE_DEGREES);
}

TEST(Direction, fromDegreesStringVariable) {
	const auto d = metaf::Direction::fromDegreesString("VRB");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->type(), metaf::Direction::Type::VARIABLE);
}

TEST(Direction, fromDegreesStringNotReported) {
	const auto d = metaf::Direction::fromDegreesString("///");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->degrees().has_value());
	EXPECT_EQ(d->type(), metaf::Direction::Type::NOT_REPORTED);
}

TEST(Direction, fromDegreesStringLastDigitNonZero) {
	EXPECT_FALSE(metaf::Direction::fromDegreesString("272").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("001").has_value());
}

TEST(Direction, fromDegreesStringWrongFormat) {
	EXPECT_FALSE(metaf::Direction::fromDegreesString("").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("120A").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("A12").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("AAA").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("12").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("0120").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("//").has_value());
	EXPECT_FALSE(metaf::Direction::fromDegreesString("////").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Test parsing of cardinal direction sector string,  
// Purpose: to confirm that fromSectorString() method correctly parses 
// strings which contain cardinal direction sectors (e.g. W-NW or S-SW-W), and 
// that other strings cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(Direction, fromSectorString_N_NE) {
	const auto d = metaf::Direction::fromSectorString("N-NE");
	ASSERT_TRUE(d.has_value());
	const auto d0 = std::get<0>(d.value());
	EXPECT_EQ(d0.type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(d0.cardinal(), metaf::Direction::Cardinal::N);

	const auto d1 = std::get<1>(d.value());
	EXPECT_EQ(d1.type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(d1.cardinal(), metaf::Direction::Cardinal::NE);
}

TEST(Direction, fromSectorString_N_E_S) {
	const auto d = metaf::Direction::fromSectorString("N-E-S");
	ASSERT_TRUE(d.has_value());
	const auto d0 = std::get<0>(d.value());
	EXPECT_EQ(d0.type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(d0.cardinal(), metaf::Direction::Cardinal::N);

	const auto d1 = std::get<1>(d.value());
	EXPECT_EQ(d1.type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(d1.cardinal(), metaf::Direction::Cardinal::S);
}

TEST(Direction, fromSectorString_N_S) {
	const auto d = metaf::Direction::fromSectorString("N-S");
	ASSERT_TRUE(d.has_value());
	const auto d0 = std::get<0>(d.value());
	EXPECT_EQ(d0.type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(d0.cardinal(), metaf::Direction::Cardinal::N);

	const auto d1 = std::get<1>(d.value());
	EXPECT_EQ(d1.type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(d1.cardinal(), metaf::Direction::Cardinal::S);
}

TEST(Direction, fromSectorString_N_NE_E_SW_S) {
	const auto d = metaf::Direction::fromSectorString("N-NE-E-SW-S");
	ASSERT_TRUE(d.has_value());
	const auto d0 = std::get<0>(d.value());
	EXPECT_EQ(d0.type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(d0.cardinal(), metaf::Direction::Cardinal::N);

	const auto d1 = std::get<1>(d.value());
	EXPECT_EQ(d1.type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(d1.cardinal(), metaf::Direction::Cardinal::S);
}

TEST(Direction, fromSectorString_N_W_S) {
	//Note: all middle values are ignored
	const auto d = metaf::Direction::fromSectorString("N-W-S");
	ASSERT_TRUE(d.has_value());
	const auto d0 = std::get<0>(d.value());
	EXPECT_EQ(d0.type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(d0.cardinal(), metaf::Direction::Cardinal::N);

	const auto d1 = std::get<1>(d.value());
	EXPECT_EQ(d1.type(), metaf::Direction::Type::VALUE_CARDINAL);
	EXPECT_EQ(d1.cardinal(), metaf::Direction::Cardinal::S);
}

TEST(Direction, fromSectorString_WrongCardinal) {
	EXPECT_FALSE(metaf::Direction::fromSectorString("NN-S").has_value());
	EXPECT_FALSE(metaf::Direction::fromSectorString("EN-S").has_value());
	EXPECT_FALSE(metaf::Direction::fromSectorString("WN-S").has_value());
	EXPECT_FALSE(metaf::Direction::fromSectorString("EW-S").has_value());
	EXPECT_FALSE(metaf::Direction::fromSectorString("WE-S").has_value());
	EXPECT_FALSE(metaf::Direction::fromSectorString("N-WE-S").has_value());
	EXPECT_FALSE(metaf::Direction::fromSectorString("N-EW-S").has_value());
	EXPECT_FALSE(metaf::Direction::fromSectorString("N-EN-S").has_value());
	EXPECT_FALSE(metaf::Direction::fromSectorString("N-WN-S").has_value());
	EXPECT_FALSE(metaf::Direction::fromSectorString("N-NN-S").has_value());
	EXPECT_FALSE(metaf::Direction::fromSectorString("N-WW-S").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Test isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(Direction, isValidCorrectDegrees) {
	const auto d1 = metaf::Direction::fromDegreesString("000");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isValid());

	const auto d2 = metaf::Direction::fromDegreesString("040");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isValid());

	const auto d3 = metaf::Direction::fromDegreesString("360");
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isValid());

	const auto d4 = metaf::Direction::fromDegreesString("///");
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->isValid());

	const auto d5 = metaf::Direction::fromDegreesString("VRB");
	ASSERT_TRUE(d5.has_value());
	EXPECT_TRUE(d5->isValid());
}

TEST(Direction, isValidCorrectCardinal) {
	const auto d1 = metaf::Direction::fromCardinalString("NDV");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isValid());

	const auto d2 = metaf::Direction::fromCardinalString("NW");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isValid());

	const auto d3 = metaf::Direction::fromCardinalString("ALQDS", true);
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isValid());

	const auto d4 = metaf::Direction::fromCardinalString("OHD", true);
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->isValid());

	const auto d5 = metaf::Direction::fromCardinalString("UNKNOWN", false, true);
	ASSERT_TRUE(d5.has_value());
	EXPECT_TRUE(d5->isValid());
}

TEST(Direction, isValidOutOfRange) {
	const auto d = metaf::Direction::fromDegreesString("370");
	ASSERT_TRUE(d.has_value());
	EXPECT_FALSE(d->isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Test cardinal()
// Purpose: to confirm that cardinal() method produces correct value of 
// type Direction::Cardinal for all possible types of Direction objects
///////////////////////////////////////////////////////////////////////////////

TEST(Direction, cardinalNorth) {
	const auto d1 = metaf::Direction::fromDegreesString("020");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::N);

	const auto d2 = metaf::Direction::fromDegreesString("340");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::N);
}

TEST(Direction, cardinalNorthEast) {
	const auto d1 = metaf::Direction::fromDegreesString("030");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::NE);

	const auto d2 = metaf::Direction::fromDegreesString("060");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::NE);
}

TEST(Direction, cardinalEast) {
	const auto d1 = metaf::Direction::fromDegreesString("070");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::E);

	const auto d2 = metaf::Direction::fromDegreesString("110");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::E);
}

TEST(Direction, cardinalSouthEast) {
	const auto d1 = metaf::Direction::fromDegreesString("120");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::SE);

	const auto d2 = metaf::Direction::fromDegreesString("150");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::SE);
}

TEST(Direction, cardinalSouth) {
	const auto d1 = metaf::Direction::fromDegreesString("160");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::S);

	const auto d2 = metaf::Direction::fromDegreesString("200");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::S);
}

TEST(Direction, cardinalSouthWest) {
	const auto d1 = metaf::Direction::fromDegreesString("210");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::SW);

	const auto d2 = metaf::Direction::fromDegreesString("240");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::SW);
}

TEST(Direction, cardinalWest) {
	const auto d1 = metaf::Direction::fromDegreesString("250");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::W);

	const auto d2 = metaf::Direction::fromDegreesString("290");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::W);
}

TEST(Direction, cardinalNorthWest) {
	const auto d1 = metaf::Direction::fromDegreesString("300");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::NW);

	const auto d2 = metaf::Direction::fromDegreesString("330");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::NW);
}

TEST(Direction, cardinalTrueDirectionsDisabled) {
	const auto d1 = metaf::Direction::fromDegreesString("000");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::N);

	const auto d2 = metaf::Direction::fromDegreesString("360");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::N);

	const auto d3 = metaf::Direction::fromDegreesString("090");
	ASSERT_TRUE(d3.has_value());
	EXPECT_EQ(d3->cardinal(), metaf::Direction::Cardinal::E);

	const auto d4 = metaf::Direction::fromDegreesString("180");
	ASSERT_TRUE(d4.has_value());
	EXPECT_EQ(d4->cardinal(), metaf::Direction::Cardinal::S);

	const auto d5 = metaf::Direction::fromDegreesString("270");
	ASSERT_TRUE(d5.has_value());
	EXPECT_EQ(d5->cardinal(), metaf::Direction::Cardinal::W);
}

TEST(Direction, cardinalTrueDirectionsEnabled) {
	const auto d1 = metaf::Direction::fromDegreesString("360");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(true), metaf::Direction::Cardinal::TRUE_N);

	const auto d2 = metaf::Direction::fromDegreesString("090");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(true), metaf::Direction::Cardinal::TRUE_E);

	const auto d3 = metaf::Direction::fromDegreesString("180");
	ASSERT_TRUE(d3.has_value());
	EXPECT_EQ(d3->cardinal(true), metaf::Direction::Cardinal::TRUE_S);

	const auto d4 = metaf::Direction::fromDegreesString("270");
	ASSERT_TRUE(d4.has_value());
	EXPECT_EQ(d4->cardinal(true), metaf::Direction::Cardinal::TRUE_W);
}

TEST(Direction, cardinalInvalid) {
	const auto d1 = metaf::Direction::fromDegreesString("370");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);

	const auto d2 = metaf::Direction::fromDegreesString("720");
	ASSERT_TRUE(d2.has_value());
	EXPECT_EQ(d2->cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
}

TEST(Direction, cardinalNotReported) {
	const auto d1 = metaf::Direction::fromDegreesString("///");
	ASSERT_TRUE(d1.has_value());
	EXPECT_EQ(d1->cardinal(), metaf::Direction::Cardinal::NOT_REPORTED);
}

TEST(Direction, cardinalVrb) {
	const auto d = metaf::Direction::fromDegreesString("VRB");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->cardinal(), metaf::Direction::Cardinal::VRB);
}

TEST(Direction, cardinalNdv) {
	auto d = metaf::Direction::fromCardinalString("NDV");
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->cardinal(), metaf::Direction::Cardinal::NDV);
}

TEST(Direction, cardinalOhd) {
	auto d = metaf::Direction::fromCardinalString("OHD", true);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->cardinal(), metaf::Direction::Cardinal::OHD);
}

TEST(Direction, cardinalAlqds) {
	auto d = metaf::Direction::fromCardinalString("ALQDS", true);
	ASSERT_TRUE(d.has_value());
	EXPECT_EQ(d->cardinal(), metaf::Direction::Cardinal::ALQDS);
}

///////////////////////////////////////////////////////////////////////////////
// Test rotateOctantClockwise()
// To confirm that rotateOctantClockwise() method produces correct value for
// all possible types of Direction objects
///////////////////////////////////////////////////////////////////////////////

TEST(Direction, rotateOctantClockwiseDirections) {
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::N),
		metaf::Direction::Cardinal::NE);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::NE),
		metaf::Direction::Cardinal::E);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::E),
		metaf::Direction::Cardinal::SE);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::SE),
		metaf::Direction::Cardinal::S);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::S),
		metaf::Direction::Cardinal::SW);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::SW),
		metaf::Direction::Cardinal::W);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::W),
		metaf::Direction::Cardinal::NW);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::NW),
		metaf::Direction::Cardinal::N);
}

TEST(Direction, rotateOctantClockwiseTrueDirections) {
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::TRUE_N),
		metaf::Direction::Cardinal::NE);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::TRUE_E),
		metaf::Direction::Cardinal::SE);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::TRUE_S),
		metaf::Direction::Cardinal::SW);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::TRUE_W),
		metaf::Direction::Cardinal::NW);
}

TEST(Direction, rotateOctantClockwiseOther) {
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::NOT_REPORTED),
		metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::VRB),
		metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::NDV),
		metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::OHD),
		metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::ALQDS),
		metaf::Direction::Cardinal::NOT_REPORTED);
	EXPECT_EQ(metaf::Direction::rotateOctantClockwise(metaf::Direction::Cardinal::UNKNOWN),
		metaf::Direction::Cardinal::NOT_REPORTED);
}

///////////////////////////////////////////////////////////////////////////////
// Test sectorCardinalDirToVector()
// To confirm that sectorCardinalDirToVector() method correctly produces vector
// of cardinal directions included in the direction sector.
///////////////////////////////////////////////////////////////////////////////

TEST(Direction, sectorCardinalDirToVector_Sector) {
	const auto d = metaf::Direction::fromSectorString("SW-NW");
	ASSERT_TRUE(d.has_value());
	const auto d0 = std::get<0>(d.value());
	const auto d1 = std::get<1>(d.value());
	const auto dv = metaf::Direction::sectorCardinalDirToVector(d0, d1);
	ASSERT_EQ(dv.size(), 3u);
	EXPECT_EQ(dv.at(0).cardinal(), metaf::Direction::Cardinal::SW);
	EXPECT_EQ(dv.at(1).cardinal(), metaf::Direction::Cardinal::W);
	EXPECT_EQ(dv.at(2).cardinal(), metaf::Direction::Cardinal::NW);
}

TEST(Direction, sectorCardinalDirToVector_NonSector) {
	const auto dn = metaf::Direction::fromCardinalString("N");
	ASSERT_TRUE(dn.has_value());
	const auto dnone = metaf::Direction::fromDegreesString("///");
	ASSERT_TRUE(dnone.has_value());
	const auto dvrb = metaf::Direction::fromDegreesString("VRB");
	ASSERT_TRUE(dvrb.has_value());
	const auto dndv = metaf::Direction::fromCardinalString("NDV");
	ASSERT_TRUE(dndv.has_value());
	const auto dohd = metaf::Direction::fromCardinalString("OHD", true);
	ASSERT_TRUE(dohd.has_value());
	const auto dalqds = metaf::Direction::fromCardinalString("ALQDS", true);
	ASSERT_TRUE(dalqds.has_value());

	const auto dv1 = metaf::Direction::sectorCardinalDirToVector(dn.value(), dndv.value());
	ASSERT_EQ(dv1.size(), 2u);
	EXPECT_EQ(dv1.at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(dv1.at(1).cardinal(), metaf::Direction::Cardinal::NDV);

	const auto dv2 = metaf::Direction::sectorCardinalDirToVector(dn.value(), dnone.value());
	ASSERT_EQ(dv2.size(), 1u);
	EXPECT_EQ(dv2.at(0).cardinal(), metaf::Direction::Cardinal::N);

	const auto dv3 = metaf::Direction::sectorCardinalDirToVector(dn.value(), dohd.value());
	ASSERT_EQ(dv3.size(), 2u);
	EXPECT_EQ(dv3.at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(dv3.at(1).cardinal(), metaf::Direction::Cardinal::OHD);

	const auto dv4 = metaf::Direction::sectorCardinalDirToVector(dn.value(), dalqds.value());
	ASSERT_EQ(dv4.size(), 2u);
	EXPECT_EQ(dv4.at(0).cardinal(), metaf::Direction::Cardinal::N);
	EXPECT_EQ(dv4.at(1).cardinal(), metaf::Direction::Cardinal::ALQDS);

	const auto dv5 = metaf::Direction::sectorCardinalDirToVector(dohd.value(), dnone.value());
	ASSERT_EQ(dv5.size(), 1u);
	EXPECT_EQ(dv5.at(0).cardinal(), metaf::Direction::Cardinal::OHD);

	const auto dv6 = metaf::Direction::sectorCardinalDirToVector(dalqds.value(), dnone.value());
	ASSERT_EQ(dv6.size(), 1u);
	EXPECT_EQ(dv6.at(0).cardinal(), metaf::Direction::Cardinal::ALQDS);

	const auto dv7 = metaf::Direction::sectorCardinalDirToVector(dvrb.value(), dnone.value());
	ASSERT_EQ(dv7.size(), 1u);
	EXPECT_EQ(dv7.at(0).cardinal(), metaf::Direction::Cardinal::VRB);
}

///////////////////////////////////////////////////////////////////////////////
// Test isValue() & isReported()
// Purpose: to confirm that isValue() method correctly identifies cardinal 
// value or value in degrees in the Direction object, and that isReported() 
// correctly identifies reported and non-reported values.
///////////////////////////////////////////////////////////////////////////////

TEST(Direction, isValueTrue) {
	const auto d1 = metaf::Direction::fromDegreesString("000");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isValue());

	const auto d2 = metaf::Direction::fromDegreesString("040");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isValue());

	const auto d3 = metaf::Direction::fromDegreesString("360");
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isValue());

	const auto d4 = metaf::Direction::fromDegreesString("370");
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->isValue());

	const auto d5 = metaf::Direction::fromCardinalString("NW");
	ASSERT_TRUE(d5.has_value());
	EXPECT_TRUE(d5->isValue());
}

TEST(Direction, isValueFalse) {
	const auto d1 = metaf::Direction::fromDegreesString("///");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->isValue());

	const auto d2 = metaf::Direction::fromDegreesString("VRB");
	ASSERT_TRUE(d2.has_value());
	EXPECT_FALSE(d2->isValue());

	const auto d3 = metaf::Direction::fromCardinalString("NDV");
	ASSERT_TRUE(d3.has_value());
	EXPECT_FALSE(d3->isValue());

	const auto d5 = metaf::Direction::fromCardinalString("ALQDS", true);
	ASSERT_TRUE(d5.has_value());
	EXPECT_FALSE(d5->isValue());

	const auto d6 = metaf::Direction::fromCardinalString("OHD", true);
	ASSERT_TRUE(d6.has_value());
	EXPECT_FALSE(d6->isValue());

	const auto d7 = metaf::Direction::fromCardinalString("UNKNOWN", false, true);
	ASSERT_TRUE(d7.has_value());
	EXPECT_FALSE(d7->isValue());
}

TEST(Direction, isReportedTrue) {
	const auto d1 = metaf::Direction::fromDegreesString("000");
	ASSERT_TRUE(d1.has_value());
	EXPECT_TRUE(d1->isReported());

	const auto d2 = metaf::Direction::fromDegreesString("040");
	ASSERT_TRUE(d2.has_value());
	EXPECT_TRUE(d2->isReported());

	const auto d3 = metaf::Direction::fromDegreesString("360");
	ASSERT_TRUE(d3.has_value());
	EXPECT_TRUE(d3->isReported());

	const auto d4 = metaf::Direction::fromDegreesString("370");
	ASSERT_TRUE(d4.has_value());
	EXPECT_TRUE(d4->isReported());

	const auto d5 = metaf::Direction::fromDegreesString("VRB");
	ASSERT_TRUE(d5.has_value());
	EXPECT_TRUE(d5->isReported());

	const auto d6 = metaf::Direction::fromCardinalString("NDV");
	ASSERT_TRUE(d6.has_value());
	EXPECT_TRUE(d6->isReported());

	const auto d7 = metaf::Direction::fromCardinalString("NW");
	ASSERT_TRUE(d7.has_value());
	EXPECT_TRUE(d7->isReported());

	const auto d8 = metaf::Direction::fromCardinalString("ALQDS", true);
	ASSERT_TRUE(d8.has_value());
	EXPECT_TRUE(d8->isReported());

	const auto d9 = metaf::Direction::fromCardinalString("OHD", true);
	ASSERT_TRUE(d9.has_value());
	EXPECT_TRUE(d9->isReported());

	const auto d10 = metaf::Direction::fromCardinalString("UNKNOWN", false, true);
	ASSERT_TRUE(d10.has_value());
	EXPECT_TRUE(d10->isReported());
}

TEST(Direction, isReportedFalse) {
	const auto d1 = metaf::Direction::fromDegreesString("///");
	ASSERT_TRUE(d1.has_value());
	EXPECT_FALSE(d1->isReported());
}
