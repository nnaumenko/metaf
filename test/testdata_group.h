/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef TESTDATA_GROUP_H
#define TESTDATA_GROUP_H

#include <vector>
#include "metaf.h"

namespace testdata {

	struct GroupTestData {
		using ReportParts = std::vector<metaf::ReportPart>;
		static const ReportParts allReportParts;
		std::string source;
		ReportParts reportParts;
		metaf::Group expectedResult;
	};

	extern const std::vector<GroupTestData> groupDataSet;

}; // namespace testData

#endif // #ifndef TESTDATA_GROUP_H