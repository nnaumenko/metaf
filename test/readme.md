# Project tests

This directory contains the project tests.

[Tests on Gitlab Pages](https://nnaumenko.gitlab.io/metaf/test/main.html).

## Building

Build tests only:

    emmake make tests

Build entire project:

    emmake make

Pre-built binary files are also included with this repo.

## Running

The compiled version of tests can be run by opening the file `bin/test/main.html` in a www browser.

[Or try it on Gitlab Pages](https://nnaumenko.gitlab.io/metaf/test/main.html).

## Description

### Test framework

The tests use [Google Test](https://github.com/abseil/googletest/) framework.

### Unit tests

The following test cases are included:

* Group: tests of Group variant functionality.
* Runway: tests of struct Runway methods.
* Temperature: tests of struct Temperature methods.
* PlainTextGroup, FixedGroup, LocationGroup, ReportTimeGroup, TimeSpanGroup, TrendTimeGroup, ProbabilityGroup, WindGroup, VarWindGroup, WindShearGroup, VisibilityGroup, CloudGroup, VerticalVisibilityGroup, WeatherGroup, TemperatureGroup, MinMaxTemperatureGroup, PressureGroup, RunwayVisualRangeGroup, RunwayStateGroup, RainfallGroup, SeaSurfaceGroup, SeaWavesGroup, ColourCodeGroup: tests of comparison operators, constructors, etc. for the structs representing individual METAR/TAF groups. The only exception is parse() method which is tested in GroupParser.
* getSyntaxGroup: tests of function getSyntaxGroup which checks which syntax-critical group is represented by specified Group variant.
* GroupParser tests parse() method of structs representing all individual METAR/TAF groups. This is done by parsing mulptiple groups and simulating different report parts where they may and may not be encountered.
* Parser test tests Parser class. It contains tests for METAR and TAF report syntax parsing, 

### Test data sets

The following test data are used for the testing. Each set of data is stored in a separate vector.

* Individual METAR and TAF groups of different types (see `testdata_group.h` and `testdata_group.cpp`).
* Correct and malformed METAR and TAF reports along with expected autodetected report type and expected error in case of malformed report (see `testdata_syntax.h` and `testdata_syntax.cpp`).
* Real life examples of METAR and TAF reports from different location, demonstrating a wide range of group formats and regional diffferences (see `testdata_real.h` and `testdata_real.cpp`).

### Test results

The tests results are reported by Google Test framework.

Some tests batch-process large amount of data, in these cases when tests fails an additional relevant information is reported in order to identify which data caused tests to fail.