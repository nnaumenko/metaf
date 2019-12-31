/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

TEST(GroupParser, parse) {
	const metaf::Group visGroup = 
		metaf::GroupParser::parse("9999", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::VisibilityGroup>(visGroup));

	const metaf::Group tempForecastGroup = 
		metaf::GroupParser::parse("TX03/1200Z", metaf::ReportPart::TAF, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::TemperatureForecastGroup>(tempForecastGroup));

	const metaf::Group fixedGroup = 
		metaf::GroupParser::parse("SPECI", metaf::ReportPart::HEADER, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::FixedGroup>(fixedGroup));
}

TEST(GroupParser, parseUnrecognised) {
	const metaf::Group group = 
		metaf::GroupParser::parse("AAAAAAAAAA", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::UnknownGroup>(group));
}