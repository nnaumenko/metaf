# Project tests

This directory contains the project tests.

[Tests at Gitlab Pages](https://nnaumenko.gitlab.io/metaf/test/main.html).

## Test framework

The tests use a custom minimalistic framework inspired by [MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html).

The test framework is located in file `test.h`.

## Tests description

### Unit tests

The following unit tests are included with the project:

* Tests of individual METAR/TAF groups' parsing (see `class TestGroupParser` in files `test_group_parser.h` and `test_group_parser.cpp`).
* Tests of groups' equality operator and constructors (see `class TestGroup` in files `test_group.h` and `test_group.cpp`).
* Tests of parser's verification of METAR/TAF syntax, error detection, and ability to process real life examples of METAR/TAF without issues (see class TestParser in files `test_parser.h` and `test_parser.cpp`).

### Performance tests

The following performance tests are included with the project.

* Parser performance check (see class CheckParserPerformance in files test_parser.h and test_parser.cpp). It parses real life examples of METAR and TAF reports and displays number of reports, total time, average time per report and number of reports processed per second.

### Test data sets

The following test data are used for the testing. Each set of data is stored in a separate vector.

* Individual METAR and TAF groups of different types (see `testdata_group.h` and `testdata_group.cpp`).
* Correct and malformed METAR and TAF reports along with expected autodetected report type and expected error in case of malformed report (see `testdata_syntax.h` and `testdata_syntax.cpp`).
* Real life examples of METAR and TAF reports from different location, demonstrating a wide range of group formats and regional diffferences (see `testdata_real.h` and `testdata_real.cpp`).

## Running the tests

### Compiling and running

The tests are compiled with command `emmake make tests`.

The compiled version of tests can be run by opening the file `bin/tests/main.html` with in a browser.

### Test results

Each test is reported either 'Passed' or 'FAILED'.

The failed tests are reported along with line, file and condition which caused test to fail.

In some cases, e.g. when a test data set is processed in a loop, notes such as line and file do not provide sufficient information to identify a failure. In these cases additional note with raw input data or group content is also reported.