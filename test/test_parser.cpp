/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "testdata_syntax.h"
#include "testdata_real.h"
#include <chrono>
#include <sstream>

// Confirm that parser is able to parse and identify valid METAR and TAF 
// reports, and that parser is able to detect errors in malformed reports.
TEST(Parser, SyntaxParsing) {
	metaf::Parser parser;
	for (const auto & data : testdata::syntaxDataSet) {
		EXPECT_NO_THROW({ parser.parse(data.report); });
		EXPECT_EQ(parser.getReportType(), data.expectedType) 
			<< "Report: " << data.report;
		EXPECT_EQ(parser.getError(), data.expectedError) 
			<< "Report: " << data.report;
		EXPECT_EQ(parser.getResult(), data.expectedResult) 
			<< "Report: " << data.report;
	}
}

// Confirm that parsing empty report with or without final = symbol results in 
// EMPTY_REPORT error and empty result vector
TEST(Parser, parseEmptyReport) {
	metaf::Parser parser1;
	EXPECT_FALSE(parser1.parse(""));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::EMPTY_REPORT);
	EXPECT_EQ(parser1.getResult().size(), 0u);

	metaf::Parser parser2;
	EXPECT_FALSE(parser2.parse("="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::EMPTY_REPORT);
	EXPECT_EQ(parser2.getResult().size(), 0u);
}


// Confirm that parser is able to parse real-life METAR reports and that 
// real-life METAR reports' syntax is recognised by parser. 
// If this test fails, a METAR report which failed the test is printed. This 
// allows to identify individual report in testdata_real.cpp
TEST(Parser, RealDataParsingMETAR) {
	metaf::Parser parser;
	for (const auto & data : testdata::realDataSet) {
		if (strlen(data.metar)) {
			bool parseResult;
			EXPECT_NO_THROW({ parseResult = parser.parse(data.metar); });
			EXPECT_TRUE(parseResult) <<
				"Report: " << data.metar;
			EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR) << 
				"Report: " << data.metar;
		}
	}
}

// Confirm that parser is able to parse real-life TAF reports and that 
// real-life TAF reports' syntax is recognised by parser. 
// If this test fails, a TAF report which failed the test is printed. This 
// allows to identify individual report in testdata_real.cpp
TEST(Parser, RealDataParsingTAF) {
	metaf::Parser parser;
	for (const auto & data : testdata::realDataSet) {
		if (strlen(data.taf)) {
			bool parseResult;
			EXPECT_NO_THROW({ parseResult = parser.parse(data.taf); });
			EXPECT_TRUE(parseResult) <<
				"Report: " << data.taf;
			EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF) << 
				"Report: " << data.taf;
		}
	}
}

// Confirm that resetResult() resets parsing result vector, source group 
// vector, report type and error after successful parsing
TEST(Parser, resetResult) {
	metaf::Parser parser;
	ASSERT_TRUE(parser.parse("METAR EGYP 092050Z 06007KT CAVOK 08/02 Q1024 BLU", true));
	ASSERT_NE(parser.getReportType(), metaf::ReportType::UNKNOWN);
	ASSERT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	ASSERT_NE(parser.getResult().size(), 0u);
	ASSERT_NE(parser.getSourceGroups().size(), 0u);
	parser.resetResult();
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 0u);
	EXPECT_EQ(parser.getResult().capacity(), 0u); //Can be implementation-defined?
	EXPECT_EQ(parser.getSourceGroups().size(), 0u);
	EXPECT_EQ(parser.getSourceGroups().capacity(), 0u); //Can be implementation-defined?
}

// Confirm that resetResult() resets parsing result vector, report type and 
// error after parsing error was generated
TEST(Parser, resetResultError) {
	metaf::Parser parser;
	ASSERT_FALSE(parser.parse("METAR EGYP", true));
	ASSERT_NE(parser.getReportType(), metaf::ReportType::UNKNOWN);
	ASSERT_NE(parser.getError(), metaf::Parser::Error::NONE);
	ASSERT_NE(parser.getResult().size(), 0u);
	ASSERT_NE(parser.getSourceGroups().size(), 0u);
	parser.resetResult();
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 0u);
	EXPECT_EQ(parser.getResult().capacity(), 0u); //Can be implementation-defined?
	EXPECT_EQ(parser.getSourceGroups().size(), 0u);
	EXPECT_EQ(parser.getSourceGroups().capacity(), 0u); //Can be implementation-defined?
}

TEST(Parser, doNotKeepSourceGroup) {
	metaf::Parser parser;
	ASSERT_TRUE(parser.parse("METAR EGYP 092050Z 06007KT CAVOK 08/02 Q1024 BLU"));
	ASSERT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	ASSERT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	ASSERT_NE(parser.getResult().size(), 0u);
	EXPECT_EQ(parser.getSourceGroups().size(), 0u);
}

// Confirm that parse() does not keep the source group unless specified
TEST(Parser, keepSourceGroups) {
	metaf::Parser parser;
	ASSERT_TRUE(parser.parse(
		"METAR KLGA 111951Z 04004KT 2 1/2SM HZ BKN008 OVC015 23/21 A3012"
		" RMK AO2 SFC VIS 4 SLP198 T02330206=", 
		true));
	ASSERT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	ASSERT_EQ(parser.getError(), metaf::Parser::Error::NONE);

	EXPECT_EQ(parser.getResult().size(), 17u);
	EXPECT_EQ(parser.getSourceGroups().size(), 17u);

	EXPECT_EQ(parser.getResult().at(0), 
		metaf::Group(metaf::FixedGroup(metaf::FixedGroup::Type::METAR)));
	EXPECT_EQ(parser.getSourceGroups().at(0), "METAR");

	EXPECT_EQ(parser.getResult().at(1), 
		metaf::Group(metaf::LocationGroup("KLGA")));
	EXPECT_EQ(parser.getSourceGroups().at(1), "KLGA");

	EXPECT_EQ(parser.getResult().at(2), 
		metaf::Group(metaf::ReportTimeGroup(metaf::MetafTime(11, 19, 51))));
	EXPECT_EQ(parser.getSourceGroups().at(2), "111951Z");

	EXPECT_EQ(parser.getResult().at(3), 
		metaf::Group(metaf::WindGroup(40, metaf::Speed(4, metaf::Speed::Unit::KNOTS))));
	EXPECT_EQ(parser.getSourceGroups().at(3), "04004KT");

	EXPECT_EQ(parser.getResult().at(4), 
		metaf::Group(metaf::VisibilityGroup::makeVisibilityMiles(2, 1, 2)));
	EXPECT_EQ(parser.getSourceGroups().at(4), "2 1/2SM");

	EXPECT_EQ(parser.getResult().at(5), 
		metaf::Group(metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::HAZE)));
	EXPECT_EQ(parser.getSourceGroups().at(5), "HZ");

	EXPECT_EQ(parser.getResult().at(6), 
		metaf::Group(metaf::CloudGroup(metaf::CloudGroup::Amount::BROKEN, 800)));
	EXPECT_EQ(parser.getSourceGroups().at(6), "BKN008");

	EXPECT_EQ(parser.getResult().at(7), 
		metaf::Group(metaf::CloudGroup(metaf::CloudGroup::Amount::OVERCAST, 1500)));
	EXPECT_EQ(parser.getSourceGroups().at(7), "OVC015");

	EXPECT_EQ(parser.getResult().at(8), 
		metaf::Group(metaf::TemperatureGroup(23, 21)));
	EXPECT_EQ(parser.getSourceGroups().at(8), "23/21");

	EXPECT_EQ(parser.getResult().at(9), 
		metaf::Group(metaf::PressureGroup(30.12, metaf::PressureGroup::Unit::INCHES_HG)));
	EXPECT_EQ(parser.getSourceGroups().at(9), "A3012");

	EXPECT_EQ(parser.getResult().at(10), 
		metaf::Group(metaf::FixedGroup(metaf::FixedGroup::Type::RMK)));
	EXPECT_EQ(parser.getSourceGroups().at(10), "RMK");

	EXPECT_EQ(parser.getResult().at(11), metaf::Group(metaf::PlainTextGroup("AO2")));
	EXPECT_EQ(parser.getSourceGroups().at(11), "AO2");

	EXPECT_EQ(parser.getResult().at(12), metaf::Group(metaf::PlainTextGroup("SFC")));
	EXPECT_EQ(parser.getSourceGroups().at(12), "SFC");

	EXPECT_EQ(parser.getResult().at(13), metaf::Group(metaf::PlainTextGroup("VIS")));
	EXPECT_EQ(parser.getSourceGroups().at(13), "VIS");

	EXPECT_EQ(parser.getResult().at(14), metaf::Group(metaf::PlainTextGroup("4")));
	EXPECT_EQ(parser.getSourceGroups().at(14), "4");

	EXPECT_EQ(parser.getResult().at(15), metaf::Group(metaf::PlainTextGroup("SLP198")));
	EXPECT_EQ(parser.getSourceGroups().at(15), "SLP198");

	EXPECT_EQ(parser.getResult().at(16), metaf::Group(metaf::PlainTextGroup("T02330206")));
	EXPECT_EQ(parser.getSourceGroups().at(16), "T02330206");
}