# Modern C++ library for parsing METAR and TAF with focus on Webassembly

[![pipeline status](https://gitlab.com/nnaumenko/metaf/badges/master/pipeline.svg)](https://gitlab.com/nnaumenko/metaf/commits/master)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE.md)
[![Codecov](https://codecov.io/gl/nnaumenko/metaf/branch/master/graph/badge.svg)](https://codecov.io/gl/nnaumenko/metaf)

## Introduction

Metaf is a header-only, dependency-free modern C++ library for parsing [METAR weather reports](https://en.wikipedia.org/wiki/METAR) and [TAF weather forecasts](https://en.wikipedia.org/wiki/Terminal_aerodrome_forecast) used in aviation.

[Live demo: Short weather summary for aerodromes near your location based on METAR/TAF weather data](https://nnaumenko.gitlab.io/metaf/examples/summary.html).

[Live demo: Convert METAR/TAF report to JSON](https://nnaumenko.gitlab.io/metaf/examples/to_json.html).

[Live demo: Decode METAR/TAF report and explain in English language](https://nnaumenko.gitlab.io/metaf/examples/explain.html).

This project focuses on using METAR and TAF parsing with Webassembly, however the library is has no dependencies and can be used in other environments.

Metaf can do the following:

 * Parse METAR or TAF report and autodetect its type.
 * Check the validity of the report syntax, detect malformed reports and report errors.
 * Convert METAR or TAF report into the vector of classes which represent individual bits of info encoded in the weather report or forecast.


## Documentation

Please refer to [documentation](https://nnaumenko.gitlab.io/metaf/docs/index.html) for details.

## Limitations

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

### Which groups Metaf is able to recognize?

* Report type METAR, SPECI, and TAF
* Various amended or correctional report indicators
* Missing report indicator NIL and cancelled TAF report indicator CNL
* Automated report indicator, automated station type remark, and maintenance indicator
* ICAO location
* Report issue time
* Wind direction, speed and gust speed
* Wind shear information
* Prevailing or directional visibility in meters or statute miles
* Cloud layer information, clear sky conditions, 'no significant cloud' / 'no cloud detected information', and detailed cloud layers information specified in remarks
* Indicator of no significant cloud and good visibility CAVOK
* Indicatiors for certain secondary locations (e.g. wind shear in the lower levels at path of runway approach)
* Current and recent weather information, and indicator or weather phenomena end NSW
* Temperature and dew point, including more precise values given in remarks
* Temperature forecast from TAF reports
* 6-hourly and 24-hourly minimum and maximum temperature
* Current atmospheric pressure, including QNH, QFE, SLP remarks, atmospheric pressure tendency remark, and groups indicating rapid pressure rise or fall
* Forecast lowest atmospheric pressure
* Runway visual range with trend
* State of runway, type and amount of deposits, extent of runway contamination, surface friction and braking action
* Groups indicating that runway or airport is closed due to snow accumulation
* Groups indicating that deposits on runway were cleared or ceased to exist
* Rainfall groups used in Australia
* Various precipitation, snowfall, and ice buildup groups reported in remarks section in North America
* Temperature and state of sea surface or wave height
* Colour codes used by NATO militaries to quickly assess visibility and cloud conditions
* Trend groups NOSIG, BECMG, TEMPO, INTER, FMxxxxxx and various time span groups
* Indicator of no unscheduled reports being issued by station
* Groups indicating non-operational sensors
* Icing and turbulence forecast used by NATO militaries
* Miscellaneous rare groups (e.g. sunshine duration)

## License

This project is available under MIT license.

## Acknowledgements

This project uses [Emscripten](https://emscripten.org/) to compile examples into Webassembly.

Unit tests included with the project use [Google Test](https://github.com/abseil/googletest) framework.

[Gitlab CI/CD](https://docs.gitlab.com/ee/ci/) is used for deployment of the [project website](https://nnaumenko.gitlab.io/metaf/).

METAR, TAF, and weather station data used in [weather summary example](https://nnaumenko.gitlab.io/metaf/examples/summary.html) are from [Aviation Weather Center Text Data Server](https://www.aviationweather.gov/dataserver). 

Weather widget uses the following external content: [Weather Icons by Erik Flowers](http://weathericons.io) distributed under [SIL OFL 1.1 license](http://scripts.sil.org/OFL), [Source Sans Pro Font](https://fonts.google.com/specimen/Source+Sans+Pro) by Paul D. Hunt distributed under [Open Font License](http://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL_web), ['eye-regular' icon](https://fontawesome.com/icons/eye?style=regular) by Font Awesome distributed under [Creative Commons Attribution 4.0 International license](https://fontawesome.com/license).

The project repository is hosted on [Gitlab](https://about.gitlab.com/) and mirrored to [Github](https://github.com/).