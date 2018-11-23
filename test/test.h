/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef TEST_H
#define TEST_H

#include <stdio.h>

/// Display verbose test results (true by default)
#ifndef TEST_VERBOSE
#define TEST_VERBOSE true
#endif

/// Global variables declarations. 
/// Must be added to main program at global scope
#define TEST_GLOBALS() \
int testsPassed = 0; \
int testsFailed = 0; \
bool testResult = false

extern int testsPassed;
extern int testsFailed;
extern bool testResult;

/// Prints a failed test message. This macro is intended to be used internally only.
#define TEST_FAILED \
do { \
	printf("FAILED %s, line %i, file %s\n", __func__, __LINE__, __FILE__); \
	testResult = false; \
} while(0)

/// Assert a condition. If assertion fails, function, file and line is reported; if verbose reporting is enabled 
/// the failed condition is also reported

#define TEST_ASSERT(condition) \
do { \
	if (!(condition)) { \
		TEST_FAILED; \
		if (TEST_VERBOSE) printf("Condition: " #condition "\n"); \
	} \
} while(0)

/// Generic note to be added to test console
#define TEST_NOTE(note) \
do { \
	if (TEST_VERBOSE) printf("Note: %s\n\n", note); \
} while(0)

/// Must be called before beginning of each individual test
#define TEST_BEGIN() \
do{ \
	testResult = true; \
	if (TEST_VERBOSE) printf("Test: %s, file %s\n", __func__, __FILE__); \
} while(0)

/// Must be called after each individual test
#define TEST_END() \
do{ \
	if (testResult) { \
		if (TEST_VERBOSE) printf("Passed\n\n"); \
		testsPassed++; \
	} \
	else { \
		if (TEST_VERBOSE) printf("\n"); \
		testsFailed++; \
	} \
} while (0)

/// Must be called before first test is performed
#define TESTS_INIT() \
do{ \
	printf("------- TESTS STARTED -------\n\n"); \
} while(0)

/// Must be called after all tests are completed
#define TESTS_SUMMARY() \
do{ \
	printf("------- TESTS FINISHED -------\n\n"); \
	if (TEST_VERBOSE) printf("Tests passed: %i, tests failed: %i\n", testsPassed, testsFailed); \
} while(0)

 #endif