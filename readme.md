# Modern c++ library for parsing METAR and TAF with focus on Webassembly

[![pipeline status](https://gitlab.com/nnaumenko/metaf/badges/master/pipeline.svg)](https://gitlab.com/nnaumenko/metaf/commits/master)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE.md)

## Introduction

Metaf is a header-only, dependency-free modern C++ library for parsing [METAR weather reports](https://en.wikipedia.org/wiki/METAR) and [TAF weather forecasts](https://en.wikipedia.org/wiki/Terminal_aerodrome_forecast) used in aviation.

This project focuses on using METAR and TAF parsing with Webassembly, however the library is has no dependencies and can be used in other environments.

Metaf can do the following:

 * Parse METAR or TAF report and autodetect its type.
 * Check the validity of the report syntax, detect malformed reports and report errors.
 * Convert METAR or TAF report into the vector of classes which represent individual bits of info encoded in the weather report or forecast.

[Example: Convert METAR/TAF report to JSON](https://nnaumenko.gitlab.io/metaf/examples/to_json.html).

[Example: Decode METAR/TAF report and explain in English language](https://nnaumenko.gitlab.io/metaf/examples/explain.html).

## License

This project is available under MIT license.

## Documentation

Please refer to [documentation](https://nnaumenko.gitlab.io/metaf/docs/index.html) for details.

## Limitations

Remarks (everything after RMK group) are currently decoded as plain text only.

Old TAF format (before November 2008) uses different format (time without date) for time spans and trends; the current version does not decode this old format.

## Prerequisites and dependencies

Metaf requires C++17.

No external dependencies for parser itself, only standard C++ libraries used. 

Unit tests included with the project use [Google Test](https://github.com/abseil/googletest) framework.

Since this project focuses on metaf library usage with Webassembly, [Emscripten](emscripten.org) is required to build tests and examples. Running the tests and examples requires a www browser with Webassembly support.

## METAR and TAF

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
