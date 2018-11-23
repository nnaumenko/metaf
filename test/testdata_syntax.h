/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef TESTDATA_SYNTAX_H
#define TESTDATA_SYNTAX_H

#include <vector>
#include <string>
#include "metaf.h"

namespace test {

	struct MetarTafSyntaxTestData {
		const char * report = "";
		metaf::ReportType expectedType = metaf::ReportType::UNKNOWN;
		metaf::Parser::Error expectedError = metaf::Parser::Error::NONE;
		std::vector<metaf::Group> expectedResult = {};
	};

	extern const std::vector<MetarTafSyntaxTestData> syntaxDataSet;

}; // namespace test

#endif // #ifndef TESTDATA_SYNTAX_H