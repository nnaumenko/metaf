/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef TEST_PARSER_H
#define TEST_PARSER_H

#include <vector>
#include <chrono>
#include "testdata_syntax.h"
#include "testdata_real.h"
#include "metaf.h"

namespace test {

	class TestParser {
	public:
		void run(const std::vector<test::MetarTafSyntaxTestData> &syntaxTestData, 
			const std::vector<MetarTafRealData> &realTestData);
	private:
		void testSyntax(const std::vector<test::MetarTafSyntaxTestData> &syntaxTestData);
		void testRealData(const std::vector<test::MetarTafRealData> &realTestData, bool taf);
		void testResetResult();
	};

	class CheckParserPerformance {
	public:
		 void run(const std::vector<MetarTafRealData> &testData);
	};

};//namespace test

#endif //#ifndef TEST_PARSER_H