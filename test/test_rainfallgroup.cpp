/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

static const auto margin = 0.1 / 2;

TEST(RainfallGroup, parseTwoValues) {
	const auto rg = metaf::RainfallGroup::parse("RF02.7/010.5", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg.has_value());

	const auto rf10m = rg->rainfallLast10Minutes();
	EXPECT_EQ(rf10m.status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(rf10m.unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(rf10m.precipitation().has_value());
	EXPECT_NEAR(rf10m.precipitation().value(), 2.7, margin);

	const auto rf9am = rg->rainfallSince9AM();
	EXPECT_EQ(rf9am.status(), metaf::Precipitation::Status::REPORTED);
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
	EXPECT_EQ(rf10m.status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(rf10m.unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(rf10m.precipitation().has_value());
	EXPECT_NEAR(rf10m.precipitation().value(), 21.5, margin);

	const auto rf9am = rg->rainfallSince9AM();
	EXPECT_EQ(rf9am.status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(rf9am.unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(rf9am.precipitation().has_value());
	EXPECT_NEAR(rf9am.precipitation().value(), 112.4, margin);

	const auto rf60m = rg->rainfallLast60Minutes();
	EXPECT_EQ(rf60m.status(), metaf::Precipitation::Status::REPORTED);
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

TEST(RainfallGroup, combine) {
	const auto rg1 = metaf::RainfallGroup::parse("RF02.7/010.5", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg1.has_value());

	const auto rg2 = metaf::RainfallGroup::parse("RF//./////./", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg2.has_value());

	const auto rg3 = metaf::RainfallGroup::parse("RF21.5/112.4/031.8", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg3.has_value());

	const auto rg4 = metaf::RainfallGroup::parse("RF//./////.//031.8", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg4.has_value());

	const auto rmk = metaf::FixedGroup::parse("RMK", metaf::ReportPart::METAR);
	ASSERT_TRUE(rmk.has_value());

	const auto text = metaf::PlainTextGroup::parse("TEST", metaf::ReportPart::METAR);
	ASSERT_TRUE(text.has_value());

	EXPECT_FALSE(rg1->combine(rg1.value()).has_value());
	EXPECT_FALSE(rg1->combine(rg2.value()).has_value());
	EXPECT_FALSE(rg1->combine(rg3.value()).has_value());
	EXPECT_FALSE(rg1->combine(rg4.value()).has_value());
	EXPECT_FALSE(rg1->combine(rmk.value()).has_value());
	EXPECT_FALSE(rg1->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(rg2->combine(rg1.value()).has_value());
	EXPECT_FALSE(rg2->combine(rg2.value()).has_value());
	EXPECT_FALSE(rg2->combine(rg3.value()).has_value());
	EXPECT_FALSE(rg2->combine(rg4.value()).has_value());
	EXPECT_FALSE(rg2->combine(rmk.value()).has_value());
	EXPECT_FALSE(rg2->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(rg3->combine(rg1.value()).has_value());
	EXPECT_FALSE(rg3->combine(rg2.value()).has_value());
	EXPECT_FALSE(rg3->combine(rg3.value()).has_value());
	EXPECT_FALSE(rg3->combine(rg4.value()).has_value());
	EXPECT_FALSE(rg3->combine(rmk.value()).has_value());
	EXPECT_FALSE(rg3->combine(metaf::PlainTextGroup("TEST")).has_value());

	EXPECT_FALSE(rg4->combine(rg1.value()).has_value());
	EXPECT_FALSE(rg4->combine(rg2.value()).has_value());
	EXPECT_FALSE(rg4->combine(rg3.value()).has_value());
	EXPECT_FALSE(rg4->combine(rg4.value()).has_value());
	EXPECT_FALSE(rg4->combine(rmk.value()).has_value());
	EXPECT_FALSE(rg4->combine(metaf::PlainTextGroup("TEST")).has_value());
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