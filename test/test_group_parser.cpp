 /*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "test.h"
#include "test_group_parser.h"
#include <algorithm>

void test::TestGroupParser::run(const std::vector<test::GroupTestData> & testParseData) {
	TEST_BEGIN();
	for (const auto & data : testParseData){
		parseSingleGroup(data);
	}
	TEST_END();
}

void test::TestGroupParser::parseSingleGroup(const test::GroupTestData & data) {
	static const std::vector<metaf::ReportPart> reportParts = {
		metaf::ReportPart::UNKNOWN,
		metaf::ReportPart::HEADER,
		metaf::ReportPart::METAR,
		metaf::ReportPart::TAF,
		metaf::ReportPart::RMK
	};
	for (const auto reportPart : reportParts) {
		const metaf::Group group = metaf::GroupParser::parse(data.source, reportPart);
		const auto i = std::find(data.reportParts.begin(), data.reportParts.end(), reportPart);
		if (i != data.reportParts.end()) {
			TEST_ASSERT(group == data.expectedResult);
			if (!(group == data.expectedResult)) failedTestNote(data.source, reportPart);
		}
		else {
			TEST_ASSERT(!(group == data.expectedResult));
			if (group == data.expectedResult) failedTestNote(data.source, reportPart);
		}
	};
}

void test::TestGroupParser::failedTestNote(const std::string source, metaf::ReportPart reportPart)
{
	std::string reportPartNote("Report part: ");
	switch (reportPart) {
		case metaf::ReportPart::UNKNOWN:
		reportPartNote += std::string("Unknown");
		break;
		case metaf::ReportPart::HEADER:
		reportPartNote += std::string("Header");
		break;
		case metaf::ReportPart::METAR:
		reportPartNote += std::string("METAR");
		break;
		case metaf::ReportPart::TAF:
		reportPartNote += std::string("TAF");
		break;
		case metaf::ReportPart::RMK:
		reportPartNote += std::string("Remark");
		break;
		default:
		reportPartNote += std::string("Undefined");
		break;
	}
	const std::string sourceNote = std::string("Source string: ") + source;
	TEST_NOTE(sourceNote.c_str());
	TEST_NOTE(reportPartNote.c_str());

}
