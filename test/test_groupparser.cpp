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
	const metaf::Group keywordGroup = 
		metaf::GroupParser::parse("RMK", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::KeywordGroup>(keywordGroup));

	const metaf::Group locationGroup = 
		metaf::GroupParser::parse("UKLL", metaf::ReportPart::HEADER, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::LocationGroup>(locationGroup));

	const metaf::Group reportTimeGroup = 
		metaf::GroupParser::parse("041758Z", metaf::ReportPart::HEADER, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::ReportTimeGroup>(reportTimeGroup));

	const metaf::Group trendGroup = 
		metaf::GroupParser::parse("NOSIG", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::TrendGroup>(trendGroup));

	const metaf::Group windGroup = 
		metaf::GroupParser::parse("27018G22KT", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::WindGroup>(windGroup));

	const metaf::Group visGroup = 
		metaf::GroupParser::parse("9999", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::VisibilityGroup>(visGroup));

	const metaf::Group cloudGroup = 
		metaf::GroupParser::parse("BKN017", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::CloudGroup>(cloudGroup));

	const metaf::Group weatherGroup = 
		metaf::GroupParser::parse("BLSN", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::WeatherGroup>(weatherGroup));

	const metaf::Group temperatureGroup = 
		metaf::GroupParser::parse("01/M02", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::TemperatureGroup>(temperatureGroup));

	const metaf::Group pressureGroup = 
		metaf::GroupParser::parse("Q0998", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::PressureGroup>(pressureGroup));

	const metaf::Group runwayStateGroup = 
		metaf::GroupParser::parse("R19/190064", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::RunwayStateGroup>(runwayStateGroup));

	const metaf::Group seaSurfaceGroup = 
		metaf::GroupParser::parse("W06/S8", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::SeaSurfaceGroup>(seaSurfaceGroup));

	const metaf::Group minMaxTemperatureGroup = 
		metaf::GroupParser::parse("TX03/1200Z", metaf::ReportPart::TAF, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::MinMaxTemperatureGroup>(minMaxTemperatureGroup));

	const metaf::Group precipitationGroup = 
		metaf::GroupParser::parse("933021", metaf::ReportPart::RMK, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::PrecipitationGroup>(precipitationGroup));

	const metaf::Group layerForecastGroup = 
		metaf::GroupParser::parse("620304", metaf::ReportPart::TAF, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::LayerForecastGroup>(layerForecastGroup));

	const metaf::Group pressureTendencyGroup = 
		metaf::GroupParser::parse("56005", metaf::ReportPart::RMK, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::PressureTendencyGroup>(pressureTendencyGroup));

	const metaf::Group cloudTypesGroup = 
		metaf::GroupParser::parse("SC1SC1AC1CI1", metaf::ReportPart::RMK, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::CloudTypesGroup>(cloudTypesGroup));

	const metaf::Group lowMidHighCloudGroup = 
		metaf::GroupParser::parse("8/52/", metaf::ReportPart::RMK, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::LowMidHighCloudGroup>(lowMidHighCloudGroup));

	const metaf::Group lightningGroup = 
		metaf::GroupParser::parse("CONS", metaf::ReportPart::RMK, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::LightningGroup>(lightningGroup));

	const metaf::Group vicinityGroup = 
		metaf::GroupParser::parse("CBMAM", metaf::ReportPart::RMK, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::VicinityGroup>(vicinityGroup));

	const metaf::Group miscGroup = 
		metaf::GroupParser::parse("YLO2", metaf::ReportPart::METAR, metaf::missingMetadata);
	EXPECT_TRUE(std::holds_alternative<metaf::MiscGroup>(miscGroup));
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