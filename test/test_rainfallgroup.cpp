/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 0.1 / 2;

TEST(RainfallGroup, parseTwoValues) {
	const auto rg = metaf::RainfallGroup::parse("RF02.7/010.5", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg.has_value());

	const auto rf10m = rg->rainfallLast10Minutes();
	EXPECT_EQ(rf10m.unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(rf10m.precipitation().has_value());
	EXPECT_NEAR(rf10m.precipitation().value(), 2.7, margin);

	const auto rf9am = rg->rainfallSince9AM();
	EXPECT_EQ(rf9am.unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(rf9am.precipitation().has_value());
	EXPECT_NEAR(rf9am.precipitation().value(), 10.5, margin);

	EXPECT_FALSE(rg->rainfallLast60Minutes().isReported());
}

TEST(RainfallGroup, parseTwoValuesNotReported) {
	const auto rg1 = metaf::RainfallGroup::parse("RF//.//010.5", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg1.has_value());
	EXPECT_FALSE(rg1->rainfallLast10Minutes().isReported());
	EXPECT_TRUE(rg1->rainfallSince9AM().isReported());
	EXPECT_FALSE(rg1->rainfallLast60Minutes().isReported());

	const auto rg2 = metaf::RainfallGroup::parse("RF02.7////./", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg2.has_value());
	EXPECT_TRUE(rg2->rainfallLast10Minutes().isReported());
	EXPECT_FALSE(rg2->rainfallSince9AM().isReported());
	EXPECT_FALSE(rg2->rainfallLast60Minutes().isReported());

	const auto rg3 = metaf::RainfallGroup::parse("RF//./////./", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg3.has_value());
	EXPECT_FALSE(rg3->rainfallLast10Minutes().isReported());
	EXPECT_FALSE(rg3->rainfallSince9AM().isReported());
	EXPECT_FALSE(rg3->rainfallLast60Minutes().isReported());
}

TEST(RainfallGroup, twoValuesWrongReportPart) {
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/010.5", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/010.5", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/010.5", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/010.5", metaf::ReportPart::RMK).has_value());
}

TEST(RainfallGroup, wrongFormatTwoValues) {
	EXPECT_FALSE(metaf::RainfallGroup::parse("0F02.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RA02.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/010.5/", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF/02.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7-010.5", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF010.5", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF002.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.70/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/0010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF2.70/010.50", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF2.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02./010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02/010.5", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/10.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/010.", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/010", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02-7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/010-5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02-7/010-5", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF0207/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/01005", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF0207/01005", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF0A.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.A/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/01A.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/010.A", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF/.//010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF//./010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF///.//010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF//.///010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7///./", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7////.", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7/////./", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF02.7////.//", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF//-/////./", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF//./////-/", metaf::ReportPart::METAR).has_value());
}

TEST(RainfallGroup, parseThreeValues) {
	const auto rg = metaf::RainfallGroup::parse("RF21.5/112.4/031.8", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg.has_value());

	const auto rf10m = rg->rainfallLast10Minutes();
	EXPECT_EQ(rf10m.unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(rf10m.precipitation().has_value());
	EXPECT_NEAR(rf10m.precipitation().value(), 21.5, margin);

	const auto rf9am = rg->rainfallSince9AM();
	EXPECT_EQ(rf9am.unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(rf9am.precipitation().has_value());
	EXPECT_NEAR(rf9am.precipitation().value(), 112.4, margin);

	const auto rf60m = rg->rainfallLast60Minutes();
	EXPECT_EQ(rf60m.unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(rf60m.precipitation().has_value());
	EXPECT_NEAR(rf60m.precipitation().value(), 031.8, margin);
}

TEST(RainfallGroup, parseThreeValuesNotReported) {
	const auto rg1 = metaf::RainfallGroup::parse("RF//.//112.4/031.8", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg1.has_value());
	EXPECT_FALSE(rg1->rainfallLast10Minutes().isReported());
	EXPECT_TRUE(rg1->rainfallSince9AM().isReported());
	EXPECT_TRUE(rg1->rainfallLast60Minutes().isReported());

	const auto rg2 = metaf::RainfallGroup::parse("RF21.5////.//031.8", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg2.has_value());
	EXPECT_TRUE(rg2->rainfallLast10Minutes().isReported());
	EXPECT_FALSE(rg2->rainfallSince9AM().isReported());
	EXPECT_TRUE(rg2->rainfallLast60Minutes().isReported());

	const auto rg3 = metaf::RainfallGroup::parse("RF//./////.//031.8", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg3.has_value());
	EXPECT_FALSE(rg3->rainfallLast10Minutes().isReported());
	EXPECT_FALSE(rg3->rainfallSince9AM().isReported());
	EXPECT_TRUE(rg3->rainfallLast60Minutes().isReported());
}

TEST(RainfallGroup, threeValuesWrongReportPart) {
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/031.8", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/031.8", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/031.8", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/031.8", metaf::ReportPart::RMK).has_value());
}

TEST(RainfallGroup, wrongFormatThreeValues) {
	EXPECT_FALSE(metaf::RainfallGroup::parse("0F21.5/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RA21.5/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/031.8/", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF/21.5/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5-112.4-031.8", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF112.4/031.8", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF021.5/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/0112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.50/112.4/0031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.40/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/031.80", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF1.5/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/12.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/31.8", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF.5/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21./112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112./031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/031.", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/031", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21-5/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112-4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/031-8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21-5/112-4/031-8", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF2105/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/11204/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/03108", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF2105/11204/03108", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF2A.5/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.A/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/11A.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.A/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/03A.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/112.4/031.A", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF//./////./////./", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF///.//112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF//.///112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF/.//112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF//./112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5/////.//031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5////.///031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5///.//031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF21.5////./031.8", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::RainfallGroup::parse("RF//-/////.//031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::RainfallGroup::parse("RF//./////-//031.8", metaf::ReportPart::METAR).has_value());
}

TEST(RainfallGroup, append) {
	const std::string rgStr1 = "RF02.7/010.5";
	const std::string rgStr2 = "RF//./////./";
	const std::string rgStr3 = "RF21.5/112.4/031.8";
	const std::string rgStr4 = "RF//./////.//031.8";
	const std::string tStr1("RMK");
	const std::string tStr2("TEST");

	auto rg1 = metaf::RainfallGroup::parse("RF02.7/010.5", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg1.has_value());

	auto rg2 = metaf::RainfallGroup::parse("RF//./////./", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg2.has_value());

	auto rg3 = metaf::RainfallGroup::parse("RF21.5/112.4/031.8", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg3.has_value());

	auto rg4 = metaf::RainfallGroup::parse("RF//./////.//031.8", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg4.has_value());

	EXPECT_EQ(rg1->append(rgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg1->append(rgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg1->append(rgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg1->append(rgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg1->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg1->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(rg2->append(rgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg2->append(rgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg2->append(rgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg2->append(rgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg2->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg2->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(rg3->append(rgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg3->append(rgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg3->append(rgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg3->append(rgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg3->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg3->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(rg4->append(rgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg4->append(rgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg4->append(rgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg4->append(rgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg4->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(rg4->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
}

TEST(RainfallGroup, isValid) {
	const auto rg1 = metaf::RainfallGroup::parse("RF02.7/010.5", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg1.has_value());
	EXPECT_TRUE(rg1->isValid());

	const auto rg2 = metaf::RainfallGroup::parse("RF//./////./", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg2.has_value());
	EXPECT_TRUE(rg2->isValid());

	const auto rg3 = metaf::RainfallGroup::parse("RF21.5/112.4/031.8", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg3.has_value());
	EXPECT_TRUE(rg3->isValid());

	const auto rg4 = metaf::RainfallGroup::parse("RF//./////.//031.8", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg4.has_value());
	EXPECT_TRUE(rg4->isValid());
}