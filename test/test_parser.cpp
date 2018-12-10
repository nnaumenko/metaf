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

// Confirm that resetResult() resets parsing result vector, report type and 
// error after successful parsing
TEST(Parser, resetResult) {
	metaf::Parser parser;
	ASSERT_TRUE(parser.parse("METAR EGYP 092050Z 06007KT CAVOK 08/02 Q1024 BLU"));
	ASSERT_NE(parser.getReportType(), metaf::ReportType::UNKNOWN);
	ASSERT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	ASSERT_NE(parser.getResult().size(), 0);
	parser.resetResult();
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 0);
	EXPECT_EQ(parser.getResult().capacity(), 0); //Can be implementation-defined?
}

// Confirm that resetResult() resets parsing result vector, report type and 
// error after parsing error was generated
TEST(Parser, resetResultError) {
	metaf::Parser parser;
	ASSERT_FALSE(parser.parse("METAR EGYP"));
	ASSERT_NE(parser.getReportType(), metaf::ReportType::UNKNOWN);
	ASSERT_NE(parser.getError(), metaf::Parser::Error::NONE);
	ASSERT_NE(parser.getResult().size(), 0);
	parser.resetResult();
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 0);
	EXPECT_EQ(parser.getResult().capacity(), 0); //Can be implementation-defined?
}