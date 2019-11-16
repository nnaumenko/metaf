/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"


///////////////////////////////////////////////////////////////////////////////

TEST(WeatherGroup, parseWeatherMetar) {
	const auto wg = metaf::WeatherGroup::parse("TSRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_TRUE(wg->isValid());

	ASSERT_EQ(wg->weatherPhenomena().size(), 1u);

	const auto wp = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp.qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp.descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp.weather().size(), 1u);
	EXPECT_EQ(wp.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp.time().has_value());
}

TEST(WeatherGroup, parseWeatherTaf) {
	const auto wg = metaf::WeatherGroup::parse("TSRA", metaf::ReportPart::TAF);
	ASSERT_TRUE(wg.has_value());

	EXPECT_TRUE(wg->isValid());

	ASSERT_EQ(wg->weatherPhenomena().size(), 1u);

	const auto wp = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp.qualifier(), metaf::WeatherPhenomena::Qualifier::MODERATE);
	EXPECT_EQ(wp.descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp.weather().size(), 1u);
	EXPECT_EQ(wp.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp.time().has_value());
}

TEST(WeatherGroup, parseRecentWeatherMetar) {
	const auto wg = metaf::WeatherGroup::parse("RESHRA", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_TRUE(wg->isValid());

	ASSERT_EQ(wg->weatherPhenomena().size(), 1u);

	const auto wp = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp.qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp.descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp.weather().size(), 1u);
	EXPECT_EQ(wp.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp.time().has_value());
}

TEST(WeatherGroup, parseRecentWeatherTaf) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("RESHRA", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RERA", metaf::ReportPart::TAF).has_value());
}

TEST(WeatherGroup, parseNonReportedMetar) {
	const auto wg = metaf::WeatherGroup::parse("//", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_TRUE(wg->isValid());

	ASSERT_EQ(wg->weatherPhenomena().size(), 1u);

	const auto wp = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp.weather().size(), 1u);
	EXPECT_EQ(wp.weather().at(0), metaf::WeatherPhenomena::Weather::NOT_REPORTED);
	EXPECT_EQ(wp.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp.time().has_value());
}

TEST(WeatherGroup, parseNonReportedTaf) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("//", metaf::ReportPart::TAF).has_value());
}

TEST(WeatherGroup, parseNonReportedRecentMetar) {
	const auto wg = metaf::WeatherGroup::parse("RE//", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_TRUE(wg->isValid());

	ASSERT_EQ(wg->weatherPhenomena().size(), 1u);

	const auto wp = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp.qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp.weather().size(), 1u);
	EXPECT_EQ(wp.weather().at(0), metaf::WeatherPhenomena::Weather::NOT_REPORTED);
	EXPECT_EQ(wp.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp.time().has_value());
}

TEST(WeatherGroup, parseNonReportedRecentTaf) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("RE//", metaf::ReportPart::TAF).has_value());
}

TEST(WeatherGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::RMK).has_value());
}

TEST(WeatherGroup, parseWrongFormat) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("/", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("///", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("BA", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RES", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RERAS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("R0", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("0R", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSRAR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+TSRAR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCTSRAR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSRZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+TSRZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCTSRZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSRARZ", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("+TSRARZ", metaf::ReportPart::METAR).has_value());
}

TEST(WeatherGroup, append) {
	const std::string wgStr1("RERA");
	const std::string wgStr2("RE//");
	const std::string wgStr3("VCSH");
	const std::string wgStr4("TS");
	const std::string wgStr5("SHRASN");
	const std::string wgStr6("BLDU");
	const std::string wgStr7("FG");
	const std::string wgStr8("//");
	const std::string tStr1("RMK");
	const std::string tStr2("TEST");

	auto wg1 = metaf::WeatherGroup::parse(wgStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());

	auto wg2 = metaf::WeatherGroup::parse(wgStr2, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());

	auto wg3 = metaf::WeatherGroup::parse(wgStr3, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());

	auto wg4 = metaf::WeatherGroup::parse(wgStr4, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());

	auto wg5 = metaf::WeatherGroup::parse(wgStr5, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg5.has_value());

	auto wg6 = metaf::WeatherGroup::parse(wgStr6, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg6.has_value());

	auto wg7 = metaf::WeatherGroup::parse(wgStr7, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg7.has_value());

	auto wg8 = metaf::WeatherGroup::parse(wgStr8, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg8.has_value());

	EXPECT_EQ(wg1->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg2->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg3->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg4->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg4->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg5->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg5->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg6->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg6->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg7->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg7->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg8->append(wgStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr3, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr4, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr5, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr6, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr7, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(wgStr8, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(tStr1, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg8->append(tStr2, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
}
