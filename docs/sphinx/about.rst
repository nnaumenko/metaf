About Metaf
===========

Metaf is a header-only, dependency-free modern C++ library for parsing METAR weather reports and TAF weather forecasts used in aviation. 


What Metaf can and cannot do?
-----------------------------

Metaf can:

- Parse METAR or TAF report and autodetect its type.
- Check the validity of the report syntax, detect malformed reports and report errors.
- Convert METAR or TAF report into the vector of classes; each of these classes represents individual chunk of information encoded in the weather report or forecast.

Metaf cannot:

- Parse old TAFs used before November 2008 since it uses obsolete format for time spans and trends.


Compatible compilers
--------------------

The following compiler are compatible (with C++17 standard enabled) and routinely tested using Gitlab CI after each commit.

- emscripten emcc 1.38.28 (based on clang)
- gcc 7.4.0
- clang 8.0.0


Prerequisites and dependencies
------------------------------

Metaf requires C++17.

Metaf library itself is dependency-free and requires only standard C++ library. Boost is not used.

Unit tests included with the project use `Google Test <https://github.com/abseil/googletest>`_ framework.


License
-------

Metaf is released under MIT license.
