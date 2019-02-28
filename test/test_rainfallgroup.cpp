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

	const auto rf60m = rg->rainfallLast60Minutes();
	EXPECT_EQ(rf60m.status(), metaf::Precipitation::Status::NOT_REPORTED);
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

// TODO: rainfall not reported
// TODO: wrong report part
// TODO: wrong format
