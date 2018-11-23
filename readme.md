# Webassembly METAR and TAF parser

## Introduction

### Purpose

This program is a Webassembly parser for [METAR](https://en.wikipedia.org/wiki/METAR) weather reports and [TAF](https://en.wikipedia.org/wiki/Terminal_aerodrome_forecast) weather forecasts used in aviation.

Parser detects weather report type (METAR or TAF), loosely verifies report syntax to detect malformed reports, reports syntax error (if any), and produces report data in form useable for practical applications (via visitor class).

### Project status

Work in progress, still in a very early stage; more info is to follow.

### Dependencies

The current version compiles with C++17.

The only dependencies are standard C++ libraries. Variant is used for a typesafe union. The current version of lexer relies extensively on regular expressions (std::regex).

## Compiling and running

This program is compiled to Webassembly using [Emscripten](http://emscripten.org/).

### Main program

Main program is compiled as follows (no practical use yet in current version):
    emmake make release

### Unit tests

The unit tests are compiled as follows:

    emmake make tests

The compiled version of tests can be opened with a browser:

    bin/tests/main.html

### Cleanup

The cleanup is performed as follows:

    emmake make clean

This will delete object files both for tests and main program.
