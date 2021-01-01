/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Parsing and validation
// Purpose: to confirm that ICAO locations are parsed correctly, malformed
// location groups cannot be parsed, and isValid() method always returns true
///////////////////////////////////////////////////////////////////////////////

TEST(LocationGroup, parseLocationWithoutNumber) {
	static const char gs[] = "UKLL"; //UKLL = Lviv Danylo Halytskyi International Airport 
	const auto lg = metaf::LocationGroup::parse(gs, metaf::ReportPart::HEADER);
	EXPECT_TRUE(lg.has_value());
	EXPECT_EQ(lg->toString(), gs);
}

TEST(LocationGroup, parseLocationWithNumber) {
	static const char gs[] = "K2J3"; //K2J3 = Louisville Municipal Airport
	const auto lg = metaf::LocationGroup::parse(gs, metaf::ReportPart::HEADER);
	EXPECT_TRUE(lg.has_value());
	EXPECT_EQ(lg->toString(), gs);
}

TEST(LocationGroup, parseWrongReportPart) {
	static const char gs[] = "UKLL"; //UKLL = Lviv Danylo Halytskyi International Airport 
	ASSERT_TRUE(metaf::LocationGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::LocationGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::LocationGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::LocationGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LocationGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(LocationGroup, parseWrongFormat) {
	EXPECT_FALSE(metaf::LocationGroup::parse("2AAA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::LocationGroup::parse("AAA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::LocationGroup::parse("AAAAA", metaf::ReportPart::METAR).has_value());
}

TEST(LocationGroup, isValid) {
	const auto lg1 = metaf::LocationGroup::parse("UKLL", metaf::ReportPart::HEADER);
	ASSERT_TRUE(lg1.has_value());
	EXPECT_TRUE(lg1->isValid());

	const auto lg2 = metaf::LocationGroup::parse("K2J3", metaf::ReportPart::HEADER);
	ASSERT_TRUE(lg2.has_value());
	EXPECT_TRUE(lg2->isValid());
}
