/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef TESTDATA_REAL_H
#define TESTDATA_REAL_H

#include <vector>
#include "metaf.h"

namespace testdata {

	struct MetarTafRealData {
		const char * airportICAO = "";
		const char * airportName = "";
		struct {
			int year = 0;
			int month = 0;
			int day = 0;
		} date;
		const char * metar = "";
		const char * taf = "";
	};

	/// Data from real METAR/TAF reports
	/// @details METAR & TAF data to cover a maximum number of of METAR/TAF message 
	/// structures and group variants
	extern const std::vector<MetarTafRealData> realDataSet;

}; // namespace test

#endif // #ifndef TESTDATA_REAL_H 