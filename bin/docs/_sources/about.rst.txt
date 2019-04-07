About Metaf
===========

.. index:: single: Metaf

Metaf is a header-only, dependency-free modern c++ library for parsing METAR weather reports and TAF weather forecasts used in aviation. 

.. index:: single: Metaf;License

.. index:: single: License

Metaf is released under MIT license.

This project focuses on using METAR and TAF parsing with Webassembly, however the library is has no external dependencies and can be used in other environments.

Metaf library requires C++17.

.. index:: single: METAR

What is METAR?
--------------

METAR is current weather report format used in aviation. Typical METAR report contains information such as location, report issue time, wind, visibility, clouds, weather phenomena, temperature, dewpoint and atmospheric pressure. 

METAR in raw form is human-readable though it might look cryptic for untrained person.

Example of a simple METAR report is as follows: :: 

	METAR EICK 092100Z 23007KT 9999 FEW038 BKN180 11/08 Q1019 NOSIG=

This report includes type (METAR), location (Cork Airport / Ireland), report issue time (day-of-month 9 at 21:00 GMT, actually issued on 9th September 2018 but month and year are not included), wind (southwest wind, wind speed 7 knots), visibility (more than 10KM), cloud layers (few clouds at 3800 feet and broken clouds at 18000 feet), temperature and dew point (temperature 11 centigrade, dewpoint 8 centrigrade), atmospheric pressure (1019 hPa) and weather trend (no significant changes expected).

METAR may also contain additional information such as runway visual range, state of the runway, remarks, trends of weather change, etc. Some locations include METAR reports include specific information, e.g.: oil platforms typically report sea surface temperature and wave height, Australian aeroports often report recent rainfall, NATO militaries report so called colour codes for quick assessment of visibility and ceiling conditions.

.. index:: single: TAF

What is TAF?
------------

TAF (Terminal Aerodrome Forecast) is a weather forecast report format used in aviation. TAF report format is quite similar to METAR and reports trends and changes in visibility, wind, clouds, weather, etc over periods of time.

TAF in raw form is also human-readable but requires training to decode.

Example of a TAF report is as follows: ::

	TAF EICK 091700Z 0918/1018 27012KT 9999 BKN025 BECMG 0920/0923 24007KT BECMG 1002/1005 21007KT BECMG 1009/1012 21015KT TEMPO 1010/1013 -RA BKN012 TEMPO 1010/1018 21018G28KT BECMG 1013/1016 6000 -RA SCT003 BKN010 TEMPO 1014/1018 3000 -RADZ BKN003 PROB40 TEMPO 1015/1018 1200 BR BKN002=

Information included in this report is similar to METAR but focuses on forecast and weather trends rathern than present conditions.
