/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Group parsing tests
// Purpose: to confirm that all Group types are correctly parsed by GroupParser
///////////////////////////////////////////////////////////////////////////////

TEST(GroupParser, parse) {
	const metaf::Group visGroup = 
		metaf::GroupParser::parse("9999", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::VisibilityGroup>(visGroup));

	const metaf::Group minMaxTempGroup = 
		metaf::GroupParser::parse("TX03/1200Z", metaf::ReportPart::TAF, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::MinMaxTemperatureGroup>(minMaxTempGroup));

	const metaf::Group keywordGroup = 
		metaf::GroupParser::parse("SPECI", metaf::ReportPart::HEADER, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::KeywordGroup>(keywordGroup));

	//TODO: add all group types
}

///////////////////////////////////////////////////////////////////////////////
// Unrecognised group test
// Purpose: to confirm that the group which is not recognised by any group type
// is parsed as UnknownGroup
///////////////////////////////////////////////////////////////////////////////

TEST(GroupParser, parseUnrecognised) {
	const metaf::Group group = 
		metaf::GroupParser::parse("AAAAAAAAAA", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::UnknownGroup>(group));
}