/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "test.h"
#include "test_parser.h"
#include "metaf.h"
#include <chrono>
#include <sstream>

void test::TestParser::run(const std::vector<test::MetarTafSyntaxTestData> &syntaxTestData, 
	const std::vector<test::MetarTafRealData> &realTestData)
{
	bool exceptionThrown = false;
	try {
		testSyntax(syntaxTestData);
		testRealData(realTestData, false);
		testRealData(realTestData, true);
	} catch (...) {
		exceptionThrown = true;
	}
	testResetResult();
	TEST_BEGIN();
	TEST_ASSERT(!exceptionThrown);
	TEST_END();
}

void test::TestParser::testSyntax(const std::vector<test::MetarTafSyntaxTestData> &syntaxTestData){
	TEST_BEGIN();
	metaf::Parser parser;
	for (const auto & data : syntaxTestData) {
		parser.parse(data.report);
		bool parseTypeOK = (parser.getReportType() == data.expectedType);
		bool parseErrorOK = (parser.getError() == data.expectedError);
		bool parseResultOK = (parser.getResult() == data.expectedResult);
		TEST_ASSERT(parseTypeOK);
		TEST_ASSERT(parseErrorOK);
		TEST_ASSERT(parseResultOK);
		if (!parseTypeOK || !parseErrorOK || !parseResultOK) TEST_NOTE(data.report);
	}
	TEST_END();
}

void test::TestParser::testRealData(const std::vector<test::MetarTafRealData> &testData, 
	bool taf)
{
	TEST_BEGIN();
	metaf::Parser parser;
	for (const auto & data : testData) {
		const std::string & testReport = taf ? data.taf : data.metar;
		const auto expectedType = 
			taf ? metaf::ReportType::TAF : metaf::ReportType::METAR;
		const bool parseResult = !testReport.empty() ? 
			parser.parse(testReport) : true;
		const bool parseTypeOK = !testReport.empty() ? 
			(parser.getReportType() == expectedType) : true;
		TEST_ASSERT(parseResult);
		TEST_ASSERT(parseTypeOK);
		if (!parseResult || !parseTypeOK) TEST_NOTE(testReport.c_str());
	}
	TEST_END();
}

void test::TestParser::testResetResult() {
	TEST_BEGIN();
	metaf::Parser parser;
	TEST_ASSERT(parser.parse("METAR EGYP 092050Z 06007KT CAVOK 08/02 Q1024 BLU"));
	TEST_ASSERT(parser.getResult().size());
	parser.resetResult();
	TEST_ASSERT(!parser.getResult().size());
	TEST_ASSERT(!parser.getResult().capacity());//Can be implementation-defined?
	TEST_END();
}


void test::CheckParserPerformance::run(
	const std::vector<test::MetarTafRealData> &testData)
{
	metaf::Parser parser;
	auto beginTime = std::chrono::system_clock::now();
	auto reportCount = 0;
	for (const auto & data : testData) {
		if (data.metar[0]) {
			parser.parse(data.metar);
			reportCount++;
		}
		if (data.taf[0]) {
			parser.parse(data.taf);
			reportCount++;
		}
	}
	auto endTime = std::chrono::system_clock::now();
	auto parseTime = std::chrono::microseconds(endTime - beginTime);
	auto averageReportTime = std::chrono::microseconds(parseTime / reportCount);
	static const auto microsecondsPerSecond = 
		std::chrono::microseconds(std::chrono::seconds(1)).count();
	std::ostringstream result;
	result << "Parser performance: ";
	result << parseTime.count() << " microseconds, ";
	result << reportCount << " reports, ";
	result << averageReportTime.count() << " microseconds per report, ";
	result << microsecondsPerSecond / averageReportTime.count();
	result << " reports per second";
	TEST_NOTE(result.str().c_str());
}