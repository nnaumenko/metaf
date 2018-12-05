# Webassembly METAR and TAF parser

## Introduction

### Purpose

This program is a Webassembly parser for [METAR](https://en.wikipedia.org/wiki/METAR) weather reports and [TAF](https://en.wikipedia.org/wiki/Terminal_aerodrome_forecast) weather forecasts used in aviation.

Parser detects weather report type (METAR or TAF), loosely verifies report syntax to detect malformed reports, reports syntax error (if any), and produces a vector of report data which can be used for practical applications (e.g. via visitor class).

### Project status

Work in progress, more info is to follow.

### Dependencies

The current version compiles with C++17.

No dependencies: only standard C++ libraries used. 

Variant (std::variant) is used for a typesafe union. 

The current version of lexer relies extensively on regular expressions (std::regex).

## Usage examples

### Example 1: Converting METAR or TAF report to JSON

To run this example open file `bin/examples/to_json.html` in a browser.

Copy and paste (or manually enter) METAR or TAF report in the "Enter METAR or TAF here" text area, and parsed report in JSON format is displayed in "Conversion result" text area. 

Checking "Formatted result" displays JSON formatted for easier reading. Unchecking "Formatted result" displays minified JSON.

For example entering the following METAR report: 

    NZSP 042250Z 08008KT 9999 FEW005 FEW030 M29/ A2869 RMK FG DSNT GRID S CLN AIR 09009KT ALL WNDS GRID SDG/HDG

produces the following JSON (providing that "Formatted result" is checked):

    {
      "kind": "metaf",
      "report": {
        "type": "METAR",
        "partial": false,
        "icaoLocation": "NZSP",
        "reportReleaseDay": 4,
        "reportReleaseTime": "22:50",
        "wind": {
          "direction": 80,
          "cardinalDirection": "east",
          "speed": 8,
          "speedUnit": "knots"
        },
        "visibility": [
          {
            "visibility": 9999,
            "visibilityUnit": "meters"
          }
        ],
        "cloudLayers": [
          {
            "amount": "few",
            "convectiveType": null,
            "baseHeight": 500,
            "heightUnit": "feet"
          },
          {
            "amount": "few",
            "convectiveType": null,
            "baseHeight": 3000,
            "heightUnit": "feet"
          }
        ],
        "airTemperature": -29,
        "temperatureUnit": "centigrade",
        "atmosphericPressure": 28,
        "atmosphericPressureUnit": "inHg",
        "remarks": {
          "plainText0": "FG",
          "plainText1": "DSNT",
          "plainText2": "GRID",
          "plainText3": "S",
          "plainText4": "CLN",
          "plainText5": "AIR",
          "plainText6": "09009KT",
          "plainText7": "ALL",
          "plainText8": "WNDS",
          "plainText9": "GRID",
          "plainText10": "SDG/HDG"
        }
      }
    } 

## Compiling and running

This program is compiled to Webassembly using [Emscripten](http://emscripten.org/).

### Main program

Main program is compiled as follows (no practical use yet in current version):

    emmake make release

### Unit tests

The unit tests are compiled as follows: `emmake make tests`

The compiled version of tests can be run by opening file `bin/tests/main.html` with a browser.

See `test/readme.md` for details.

### Examples

The examples are compiled as follows: `emmake make examples`

The compiled version of tests can be run by html files located in directory `bin/examples` with a browser.

See `examples/readme.md` for details.

### Cleanup

The cleanup is performed as follows: `emmake make clean`

This will delete object files for examples, tests and main program.
