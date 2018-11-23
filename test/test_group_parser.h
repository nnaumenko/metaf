/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef TEST_GROUP_PARSER_H
#define TEST_GROUP_PARSER_H

#include <string>
#include <vector>
#include "testdata_group.h"

namespace test {

	class TestGroupParser {
	public:
		void run(const std::vector<GroupTestData> & testParseData);
	private:
		void parseSingleGroup(const test::GroupTestData & data);
		void failedTestNote(const std::string source, metaf::ReportPart reportPart);
	};

}; //namespace test

#endif //#ifndef TEST_GROUP_PARSER_H