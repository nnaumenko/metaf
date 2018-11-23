/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "test.h"
#include "testdata_group.h"
#include "testdata_syntax.h"
#include "testdata_real.h"
#include "test_group.h"
#include "test_group_parser.h"
#include "test_parser.h"

TEST_GLOBALS();

int main(int argc, char ** argv) {
	TESTS_INIT();
	{
		test::TestGroup testGroup;
		testGroup.run();
	}
	{
		test::TestGroupParser testGroupParser;
		testGroupParser.run(test::groupDataSet);
	}
	{
		test::TestParser testParser;
		testParser.run(test::syntaxDataSet, test::realDataSet);
	}
	TESTS_SUMMARY();
	{
		test::CheckParserPerformance parserPerformanceTester;
		parserPerformanceTester.run(test::realDataSet);
	}
}