# Performance check

This directory contains the performance check for Metaf library.

## Building

Build performance check only:

    emmake make performance

Build entire project:

    emmake make

Pre-built binary files are also included with this repo.

## Running

The compiled version of performance check can be run by opening the file `bin/test/main.html` in a www browser.

[Or try it on Gitlab Pages](https://nnaumenko.gitlab.io/metaf/performance/check.html).

## Test data

This version runs performance check over fixed set of various real-life METAR and TAF reports, see `test/testdata_real.cpp` for details.

## Parsing speed

Parsing speed check is performed by measuring time needed to parse the entire data set.

The results are reported as follows:

    Checking performance on testdata::realDataSet
    Parser performance: 173000 microseconds, 649 reports, 266 microseconds per report, 3759 reports per second
    
Please note that parsing speed data is for reference only. The numbers depend on hardware configuration, OS, browser, etc.

## Unrecognised and successfully parsed METAR and TAF groups

Since at the moment parser does not recognise remarks (yet), the remarks are not included in this check.

All METAR or TAF groups not recognised by parser are summarised as follows:

    Detecting non-recognised groups in testdata::realDataSet
    Total 5242 METAR groups, 26 groups not recognised, 5216 groups parsed successfully
    0.49 % not recognised, 99.5 % parsed successfully
     
    Total 6722 TAF groups, 339 groups not recognised, 6383 groups parsed successfully
    5.04 % not recognised, 94.95 % parsed successfully

After this message, the following data are listed:

* All groups not recognised by parser along with occurence counter (how many times the group was found in the test set).
* All reports which contain groups not recognised by parser along with unrecognised group count per report.