# Modern c++ library for parsing METAR and TAF with focus on Webassembly

[![pipeline status](https://gitlab.com/nnaumenko/metaf/badges/master/pipeline.svg)](https://gitlab.com/nnaumenko/metaf/commits/master)

## Introduction

Metaf is a dependency-free modern c++ library for parsing [METAR weather reports](https://en.wikipedia.org/wiki/METAR) and [TAF weather forecasts](https://en.wikipedia.org/wiki/Terminal_aerodrome_forecast) used in aviation.

This project focuses on using METAR and TAF parsing with Webassembly, however the library is has no dependencies and can be used in other environments.

[Live demo on Gitlab Pages](https://nnaumenko.gitlab.io/metaf/).

[Example: convert METAR or TAF to JSON](https://nnaumenko.gitlab.io/metaf/examples/to_json.html).

### Project status

Currently this project is work in progress.

In its current state the parser successfully parses various real-life examples METAR and TAF reports as demonstrated by [tests](blob/master/test/readme.md).

### Description

METAR or TAF reports consist of groups, lexical tokens sparated by spaces. In this project a METAR or TAF group is represented by type Group. Group is a variant type which can hold structs representing various METAR or TAF groups. For example, consider Group that contains struct LocationGroup which represents report's location (in a form of 4-char ICAO code), or Group that contains struct CloudGroup which represents cloud layer information such as cloud cover, base height and convective type.

A string with raw report is passed to parser which produces a vector or type Group. This vector is stored within parser instance and can be accessed via getter method. A GroupVisitor base class is provided to simplify working with Group and vector of Group.

Report syntax based on type is loosely verified by parser, if malformed report is detected the parsing stops and report is only decoded partially. Parser error can be obtained via getter method.

All groups which parser cannot recognise are included in the decoded report as PlainTextGroup. 

From practice, METAR and TAF reports may contain rare kinds of information used in certain regions, or plain text remarks, or errors and typos. Such groups canot be recognised by parser but have minor effect on report overall. Due to this, when unrecognised group does not break report syntax, parser does not stop report processing and does not report an error. If this is undesirable, the decoded report might be checked after parsing and unrecognised groups can be easily detected.

The report type (METAR or TAF) is autodetected by the parser, autodetected type can be obtained via getter method. There is no need to specify report type before parsing.

### Limitations

Remarks (everything after RMK group) are currently decoded as plain text only.

### Prerequisites and dependencies

Metaf is written for C++17.

No external dependencies for parser itself, only standard C++ libraries used. 

* Variant (std::variant) is used for a typesafe union. 
* The current version relies extensively on regular expressions (std::regex).

Unit tests included with the project use [Google Test](https://github.com/abseil/googletest) framework.

Since this project focuses on metaf library usage with Webassembly, [Emscripten](emscripten.org) is required to build tests and examples. Running the tests and examples requires a www browser with Webassembly support.

### License

This project is available under MIT license.

See [LICENSE](blob/master/LICENSE) file for details.

## General

### What is METAR?

METAR is current weather report format used in aviation. Typical METAR report contains information such as location, report issue time, wind, visibility, clouds, weather phenomena, temperature, dewpoint and atmospheric pressure.

METAR in raw form is human-readable though it might look cryptic for untrained person.

Example of a simple METAR report is as follows: 

    METAR EICK 092100Z 23007KT 9999 FEW038 BKN180 11/08 Q1019 NOSIG=

### What is TAF?

TAF (Terminal Aerodrome Forecast) is a weather forecast report format used in aviation. TAF report is quite similar to METAR and reports trends and changes in visibility, wind, clouds, weather, etc over periods of time.

TAF in raw form is also human-readable but requires training to decode.

Example of a TAF report is as follows:

    TAF EICK 091700Z 0918/1018 27012KT 9999 BKN025 BECMG 0920/0923 24007KT BECMG 1002/1005 21007KT BECMG 1009/1012 21015KT TEMPO 1010/1013 -RA BKN012 TEMPO 1010/1018 21018G28KT BECMG 1013/1016 6000 -RA SCT003 BKN010 TEMPO 1014/1018 3000 -RADZ BKN003 PROB40 TEMPO 1015/1018 1200 BR BKN002=

## Setting up and running

### Cloning this repo

This project contains Google Test submodule, make sure to clone recursively to get Google Test as well:

    git clone --recursive https://gitlab.com/nnaumenko/metaf.git

### Building

This project compiles with [Emscripten](emscripten.org), make sure it is installed or follow [Emscripten downloading and installing instructions](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html#sdk-download-and-install).

Build examples and tests:

    emmake make

This will build tests into directory `bin/tests` and examples into directory `bin/examples`.

Pre-built binary files are also included with this repo.

#### Selective building

Build examples only:

    emmake make examples

Build tests only:

    emmake make tests

Build performance check only:

    emmake make performance

### Running

Open file `bin/index.html` in a www browser (Webassembly support required). 

[Or try it on Gitlab Pages](https://nnaumenko.gitlab.io/metaf/).

The page open in browser contains links to tests and examples.

[More information on tests](tree/master/test/readme.md).

[More information on examples](tree/master/examples/readme.md).

[More information on performance check](tree/master/performance/readme.md).

## Usage examples

### Example 1: Converting METAR or TAF report to JSON

[Run this example at Gitlab Pages](https://nnaumenko.gitlab.io/metaf/examples/to_json.html).

To run this example locally, open file `bin/examples/to_json.html` in a browser.

Copy and paste (or manually enter) METAR or TAF report in the "Enter METAR or TAF here" text area, and parsed report in JSON format is displayed in "Conversion result" text area. 

Checking "Formatted result" displays JSON formatted for easier reading. Unchecking "Formatted result" displays minified JSON.

For example entering the following METAR report: 

    NZSP 042250Z 08008KT 9999 FEW005 FEW030 M29/ A2869 RMK FG DSNT GRID S CLN AIR 09009KT ALL WNDS GRID SDG/HDG
 
produces the following JSON (providing that "Formatted result" is checked):

<details>
    <summary>Click to show/hide result</summary>
    <pre>
        <code>
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
    "airTemperatureFreezing": true,
    "airTemperatureUnit": "centigrade",
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
        </code>
    </pre>
</details>

