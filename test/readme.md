# Project tests

This directory contains the project tests.

[Tests at Gitlab Pages](https://nnaumenko.gitlab.io/metaf/test/main.html).

## Test framework

The tests use Goole Test framework.

## Tests description

### Unit tests

The following test cases are included:

* Group: tests of Group variant functionality.
* Runway: tests of struct Runway methods.
* PlainTextGroup, FixedGroup, LocationGroup, ReportTimeGroup, TimeSpanGroup, TrendTimeGroup, ProbabilityGroup, WindGroup, VarWindGroup, WindShearGroup, VisibilityGroup, CloudGroup, VerticalVisibilityGroup, WeatherGroup, TemperatureGroup, MinMaxTemperatureGroup, PressureGroup, RunwayVisualRangeGroup, RunwayStateGroup, RainfallGroup, SeaSurfaceGroup, SeaWavesGroup, ColourCodeGroup: tests of comparison operators, constructors, etc. for the structs representing individual METAR/TAF groups. The only exception is parse() method which is tested in GroupParser.
* getSyntaxGroup: tests of function getSyntaxGroup which checks which syntax-critical group is represented by specified Group variant.
* GroupParser tests parse() method of structs representing individual METAR/TAF groups. This is done by parsing mulptiple groups and simulating different report parts where they may and may not be encountered.
* Parser test tests Parser class. It contains tests for METAR and TAF report syntax parsing, 

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