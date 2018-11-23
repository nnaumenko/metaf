/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef TEST_GROUP_H
#define TEST_GROUP_H

namespace test {

	class TestGroup {
	public:
		void run();
	private:
		void testGroup();
		void testPlainTextGroup();
		void testImmutableGroup();
		void testLocationGroup();
		void testReportTimeGroup();
		void testTimeSpanGroup();
		void testTrendTimeGroup();
		void testProbabilityGroup();
		void testWindGroup();
		void testVarWindGroup();
		void testWindShearGroup();
		void testVisibilityGroup();
		void testCloudGroup();
		void testVerticalVisibilityGroup();
		void testWeatherGroup();
		void testTemperatureGroup();
		void testMinMaxTemperatureGroup();
		void testPressureGroup();
		void testRunwayVisualRangeGroup();
		void testRunwayStateGroup();
		void testRunwayStateGroupDepositDepth();
		void testRunwayStateGroupSurfaceFriction();
		void testRainfallGroup();
		void testSeaSurfaceGroup();
		void testSeaWavesGroup();
		void testColourCodeGroup();
		void testGetSyntaxGroup();
	};

};//namespace test

#endif //#ifndef TEST_GROUP_H