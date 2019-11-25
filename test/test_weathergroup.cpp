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

	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::CURRENT);
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

	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::CURRENT);
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

	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::RECENT);
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

	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::CURRENT);
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

	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::RECENT);
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

TEST(WeatherGroup, parseRecentWeatherEventSingle) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("281232");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	const auto wg1 = metaf::WeatherGroup::parse("RAB1200", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_TRUE(wg1->isValid());
	EXPECT_EQ(wg1->type(), metaf::WeatherGroup::Type::EVENT);
	ASSERT_EQ(wg1->weatherPhenomena().size(), 1u);

	const auto wp1 = wg1->weatherPhenomena().at(0);
	EXPECT_EQ(wp1.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1.weather().size(), 1u);
	EXPECT_EQ(wp1.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1.event(), metaf::WeatherPhenomena::Event::BEGINNING);
	ASSERT_TRUE(wp1.time().has_value());
	EXPECT_FALSE(wp1.time()->day().has_value());
	EXPECT_EQ(wp1.time()->hour(), 12u);
	EXPECT_EQ(wp1.time()->minute(), 0u);

	const auto wg2 = metaf::WeatherGroup::parse("SNE00", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_TRUE(wg2->isValid());
	EXPECT_EQ(wg2->type(), metaf::WeatherGroup::Type::EVENT);
	ASSERT_EQ(wg2->weatherPhenomena().size(), 1u);
	
	const auto wp2 = wg2->weatherPhenomena().at(0);
	EXPECT_EQ(wp2.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2.weather().size(), 1u);
	EXPECT_EQ(wp2.weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp2.event(), metaf::WeatherPhenomena::Event::ENDING);
	ASSERT_TRUE(wp2.time().has_value());
	EXPECT_FALSE(wp2.time()->day().has_value());
	EXPECT_EQ(wp2.time()->hour(), reportTime->hour());
	EXPECT_EQ(wp2.time()->minute(), 0u);
}

TEST(WeatherGroup, parseRecentWeatherEventMultiple) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("281123");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	const auto wg = metaf::WeatherGroup::parse("RAE1031SNB1053SHPLB08E15", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->isValid());
	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::EVENT);
	ASSERT_EQ(wg->weatherPhenomena().size(), 4u);

	const auto wp1 = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp1.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1.weather().size(), 1u);
	EXPECT_EQ(wp1.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp1.event(), metaf::WeatherPhenomena::Event::ENDING);
	ASSERT_TRUE(wp1.time().has_value());
	EXPECT_FALSE(wp1.time()->day().has_value());
	EXPECT_EQ(wp1.time()->hour(), 10u);
	EXPECT_EQ(wp1.time()->minute(), 31u);

	const auto wp2 = wg->weatherPhenomena().at(1);
	EXPECT_EQ(wp2.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2.weather().size(), 1u);
	EXPECT_EQ(wp2.weather().at(0), metaf::WeatherPhenomena::Weather::SNOW);
	EXPECT_EQ(wp2.event(), metaf::WeatherPhenomena::Event::BEGINNING);
	ASSERT_TRUE(wp2.time().has_value());
	EXPECT_FALSE(wp2.time()->day().has_value());
	EXPECT_EQ(wp2.time()->hour(), 10u);
	EXPECT_EQ(wp2.time()->minute(), 53u);

	const auto wp3 = wg->weatherPhenomena().at(2);
	EXPECT_EQ(wp3.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp3.descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp3.weather().size(), 1u);
	EXPECT_EQ(wp3.weather().at(0), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp3.event(), metaf::WeatherPhenomena::Event::BEGINNING);
	ASSERT_TRUE(wp3.time().has_value());
	EXPECT_FALSE(wp3.time()->day().has_value());
	EXPECT_EQ(wp3.time()->hour(), 11u);
	EXPECT_EQ(wp3.time()->minute(), 8u);

	const auto wp4 = wg->weatherPhenomena().at(3);
	EXPECT_EQ(wp4.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp4.descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp4.weather().size(), 1u);
	EXPECT_EQ(wp4.weather().at(0), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp4.event(), metaf::WeatherPhenomena::Event::ENDING);
	ASSERT_TRUE(wp4.time().has_value());
	EXPECT_FALSE(wp4.time()->day().has_value());
	EXPECT_EQ(wp4.time()->hour(), 11u);
	EXPECT_EQ(wp4.time()->minute(), 15u);
}

TEST(WeatherGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB1200", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB1200", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB1200", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB1200", metaf::ReportPart::TAF).has_value());
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

	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("120830");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	EXPECT_FALSE(metaf::WeatherGroup::parse("+RAB1200", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCSHB1200", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("//B1200", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAZ1200", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RABA100", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB120A", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB////", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA/1200", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB120", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB01200", metaf::ReportPart::METAR, reportMetadata).has_value());

	EXPECT_FALSE(metaf::WeatherGroup::parse("+RAB00", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCSHB00", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("//B00", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAZ00", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RABA0", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB0A", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB//", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA/00", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB0", metaf::ReportPart::METAR, reportMetadata).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RAB000", metaf::ReportPart::METAR, reportMetadata).has_value());
}

TEST(WeatherGroup, appendCurrentWeather) {
	const std::string wgStr1("VCTS");
	const std::string wgStr2("SQ");
	const std::string wgStr3("-SHRAGS");

	auto wg = metaf::WeatherGroup::parse(wgStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::CURRENT);

	EXPECT_EQ(wg->append(wgStr2, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr3, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::CURRENT);
	ASSERT_EQ(wg->weatherPhenomena().size(), 3u);

	const auto wp1 = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp1.qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp1.descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp1.weather().size(), 0u);
	EXPECT_EQ(wp1.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1.time().has_value());

	const auto wp2 = wg->weatherPhenomena().at(1);
	EXPECT_EQ(wp2.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2.weather().size(), 1u);
	EXPECT_EQ(wp2.weather().at(0), metaf::WeatherPhenomena::Weather::SQUALLS);
	EXPECT_EQ(wp2.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2.time().has_value());

	const auto wp3 = wg->weatherPhenomena().at(2);
	EXPECT_EQ(wp3.qualifier(), metaf::WeatherPhenomena::Qualifier::LIGHT);
	EXPECT_EQ(wp3.descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp3.weather().size(), 2u);
	EXPECT_EQ(wp3.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp3.weather().at(1), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp3.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3.time().has_value());
}

TEST(WeatherGroup, appendCurrentWeatherTooManyGroups) {
	const std::string wgStr("RA");
	auto wg = metaf::WeatherGroup::parse(wgStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
}

TEST(WeatherGroup, appendRecentWeather) {
	const std::string wgStr1("REDZ");
	const std::string wgStr2("RETS");
	const std::string wgStr3("RESHRAPLGS");

	auto wg = metaf::WeatherGroup::parse(wgStr1, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::RECENT);

	EXPECT_EQ(wg->append(wgStr2, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr3, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::RECENT);
	ASSERT_EQ(wg->weatherPhenomena().size(), 3u);

	const auto wp1 = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp1.qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp1.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1.weather().size(), 1u);
	EXPECT_EQ(wp1.weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp1.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp1.time().has_value());

	const auto wp2 = wg->weatherPhenomena().at(1);
	EXPECT_EQ(wp2.qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp2.descriptor(), metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	EXPECT_EQ(wp2.weather().size(), 0u);
	EXPECT_EQ(wp2.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp2.time().has_value());

	const auto wp3 = wg->weatherPhenomena().at(2);
	EXPECT_EQ(wp3.qualifier(), metaf::WeatherPhenomena::Qualifier::RECENT);
	EXPECT_EQ(wp3.descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp3.weather().size(), 3u);
	EXPECT_EQ(wp3.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp3.weather().at(1), metaf::WeatherPhenomena::Weather::ICE_PELLETS);
	EXPECT_EQ(wp3.weather().at(2), metaf::WeatherPhenomena::Weather::SMALL_HAIL);
	EXPECT_EQ(wp3.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp3.time().has_value());
}

TEST(WeatherGroup, appendRecentWeatherTooManyGroups) {
	const std::string wgStr("RERA");
	auto wg = metaf::WeatherGroup::parse(wgStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
}

TEST(WeatherGroup, appendCurrentRecentWeather) {
	auto wg1 = metaf::WeatherGroup::parse("SN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->type(), metaf::WeatherGroup::Type::CURRENT);

	EXPECT_EQ(wg1->append("RERA", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);

	auto wg2 = metaf::WeatherGroup::parse("RESN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->type(), metaf::WeatherGroup::Type::RECENT);

	EXPECT_EQ(wg2->append("RA", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
}

TEST(WeatherGroup, appendEvent) {
	//TODO
}

TEST(WeatherGroup, appendToWeatherEvent) {
	//TODO
}

TEST(WeatherGroup, appendOther) {
	auto wg1 = metaf::WeatherGroup::parse("SN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());

	EXPECT_EQ(wg1->append("RMK", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append("12/12", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append("NOSIG", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append("TEMPO", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append("BECMG", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg1->append("/////", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);

	auto wg2 = metaf::WeatherGroup::parse("RESN", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());

	EXPECT_EQ(wg2->append("RMK", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append("12/12", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append("NOSIG", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append("TEMPO", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append("BECMG", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg2->append("/////", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);

	//TODO:append other to event
}

