/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef TESTDATA_REAL_H
#define TESTDATA_REAL_H

#include <vector>
#include <string>

namespace testdata {

struct MetarTafRealData {
	std::string airportICAO;
	std::string airportName;
	struct {
		int year = 0;
		int month = 0;
		int day = 0;
	} date;
	std::string metar;
	std::string taf;
};

///////////////////////////////////////////////////////////////////////////////
// Data from real-life METAR/TAF reports produced by actual weather stations
// This set has been put together to cover a maximum number of of METAR/TAF 
// report structures and group formats.
///////////////////////////////////////////////////////////////////////////////
extern const std::vector<MetarTafRealData> realDataSet;

} // namespace test

#endif // #ifndef TESTDATA_REAL_H 