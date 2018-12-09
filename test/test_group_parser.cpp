 /*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "testdata_group.h"
#include <algorithm>

static std::string reportPartToString(metaf::ReportPart reportPart)
{
	switch (reportPart) {
		case metaf::ReportPart::UNKNOWN:
		return("Unknown");
		break;

		case metaf::ReportPart::HEADER:
		return("Header");
		break;

		case metaf::ReportPart::METAR:
		return("METAR");
		break;

		case metaf::ReportPart::TAF:
		return("TAF");
		break;

		case metaf::ReportPart::RMK:
		return("Remark");
		break;

		default:
		return("Undefined" + std::to_string(static_cast<int>(reportPart)));
		break;
	}
}

// If this test fails, check the info reported under "Additional test data":
// 1. Whether group "was not expected to parse" (which means that the group was 
//    not expected to work within the certain report part) or "did not parse 
//    as expected (which means that either group did not work within certain
//    report part and/or the group's parsing mechanism is broken)"
// 2. Group string: the string can be used to find a specific item in the 
//    vector testdata::groupDataSet located in file testdata_group.cpp.
// 3. Report part: the parsing of the group was simulated within this report 
//    part. Used to identify situations where e.g. group should (or should not) 
//    have worked within certain report part.
TEST(GroupParser, ParseAllGroups) {
	for (const auto & data : testdata::groupDataSet){
		for (const auto reportPart : testdata::GroupTestData::allReportParts) {
			const metaf::Group group = metaf::GroupParser::parse(data.source, reportPart);
			const auto i = std::find(data.reportParts.begin(), 
				data.reportParts.end(),
				reportPart);
			const bool groupShouldParseForThisReportPart = (i != data.reportParts.end());

			if (groupShouldParseForThisReportPart) {
				EXPECT_EQ(group, data.expectedResult) << "Additional test data:\n"
					" Group did not parse as expected within this report part\n" <<
					" Group String: " << data.source << "\n" <<
					" Report Part: " << reportPartToString(reportPart);
			}

			if (!groupShouldParseForThisReportPart) {
				EXPECT_NE(group, data.expectedResult) << "Additional test data:\n"
					" Group was not expected to parse within this report part\n" <<
					" Group String: " << data.source << "\n" <<
					" Report part: " << reportPartToString(reportPart);
			}
		}
	}
}