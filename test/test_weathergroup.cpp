/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Parsing current, recent or forecast weather 
// Purpose: to confirm that current weather specified in METAR, forecast
// weather specified in METAR or TAF trend, and recent weather specified in
// METAR are parsed correctly and that malformed groups are not parsed
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

TEST(WeatherGroup, parseVicinityMetar) {
	const auto wg = metaf::WeatherGroup::parse("VCSH", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_TRUE(wg->isValid());

	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::CURRENT);
	ASSERT_EQ(wg->weatherPhenomena().size(), 1u);

	const auto wp = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp.qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp.descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp.weather().size(), 0u);
	EXPECT_EQ(wp.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp.time().has_value());
}

TEST(WeatherGroup, parseVicinityTaf) {
	const auto wg = metaf::WeatherGroup::parse("VCSH", metaf::ReportPart::TAF);
	ASSERT_TRUE(wg.has_value());

	EXPECT_TRUE(wg->isValid());

	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::CURRENT);
	ASSERT_EQ(wg->weatherPhenomena().size(), 1u);

	const auto wp = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp.qualifier(), metaf::WeatherPhenomena::Qualifier::VICINITY);
	EXPECT_EQ(wp.descriptor(), metaf::WeatherPhenomena::Descriptor::SHOWERS);
	EXPECT_EQ(wp.weather().size(), 0u);
	EXPECT_EQ(wp.event(), metaf::WeatherPhenomena::Event::NONE);
	EXPECT_FALSE(wp.time().has_value());
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

///////////////////////////////////////////////////////////////////////////////
// Recent weather events with time 
// Purpose: to confirm that recent weather events specified in remarks are 
// parsed and appended correctly, and that malformed groups are not parsed or
// appended
///////////////////////////////////////////////////////////////////////////////

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

TEST(WeatherGroup, parseRecentWeatherEventMultipleShortGroupInTheMiddle) {
	// Make sure that E22 is parsed correctly when it is not the last event in group
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("262356");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	const auto wg = metaf::WeatherGroup::parse("DZB16E22RAB37", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->isValid());
	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::EVENT);
	ASSERT_EQ(wg->weatherPhenomena().size(), 3u);

	const auto wp1 = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp1.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp1.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp1.weather().size(), 1u);
	EXPECT_EQ(wp1.weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp1.event(), metaf::WeatherPhenomena::Event::BEGINNING);
	ASSERT_TRUE(wp1.time().has_value());
	EXPECT_FALSE(wp1.time()->day().has_value());
	EXPECT_EQ(wp1.time()->hour(), 23u);
	EXPECT_EQ(wp1.time()->minute(), 16u);

	const auto wp2 = wg->weatherPhenomena().at(1);
	EXPECT_EQ(wp2.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp2.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp2.weather().size(), 1u);
	EXPECT_EQ(wp2.weather().at(0), metaf::WeatherPhenomena::Weather::DRIZZLE);
	EXPECT_EQ(wp2.event(), metaf::WeatherPhenomena::Event::ENDING);
	ASSERT_TRUE(wp2.time().has_value());
	EXPECT_FALSE(wp2.time()->day().has_value());
	EXPECT_EQ(wp2.time()->hour(), 23u);
	EXPECT_EQ(wp2.time()->minute(), 22u);

	const auto wp3 = wg->weatherPhenomena().at(2);
	EXPECT_EQ(wp3.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp3.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp3.weather().size(), 1u);
	EXPECT_EQ(wp3.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp3.event(), metaf::WeatherPhenomena::Event::BEGINNING);
	ASSERT_TRUE(wp3.time().has_value());
	EXPECT_FALSE(wp3.time()->day().has_value());
	EXPECT_EQ(wp3.time()->hour(), 23u);
	EXPECT_EQ(wp3.time()->minute(), 37u);
}

TEST(WeatherGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("RA", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(metaf::WeatherGroup::parse("VCSH", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCSH", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("VCSH", metaf::ReportPart::RMK).has_value());

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

///////////////////////////////////////////////////////////////////////////////
// Current, recent, and forecast weather phenomena appending tests 
// Purpose: to confirm that current or forecast weather phenomena specified in 
// METAR or TAF report body or trend are appended correctly, and that groups
// or different types cannot be appended
///////////////////////////////////////////////////////////////////////////////

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
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
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
	EXPECT_EQ(wg->append(wgStr, metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
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

TEST(WeatherGroup, appendWeatherEvent) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("281232");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WeatherGroup::parse("TSRA", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("SNB1211", metaf::ReportPart::METAR, reportMetadata), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RAE1212", metaf::ReportPart::METAR, reportMetadata), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("E1213", metaf::ReportPart::METAR, reportMetadata), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("E14", metaf::ReportPart::METAR, reportMetadata), metaf::AppendResult::NOT_APPENDED);

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

TEST(WeatherGroup, appendToWeatherEvent) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("281232");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WeatherGroup::parse("RAB1200", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("SNB1211", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RAE1212", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("E1213", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("E14", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("FG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RA", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RERA", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("VCSH", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::EVENT);
	ASSERT_EQ(wg->weatherPhenomena().size(), 1u);
	const auto wp = wg->weatherPhenomena().at(0);
	EXPECT_EQ(wp.qualifier(), metaf::WeatherPhenomena::Qualifier::NONE);
	EXPECT_EQ(wp.descriptor(), metaf::WeatherPhenomena::Descriptor::NONE);
	EXPECT_EQ(wp.weather().size(), 1u);
	EXPECT_EQ(wp.weather().at(0), metaf::WeatherPhenomena::Weather::RAIN);
	EXPECT_EQ(wp.event(), metaf::WeatherPhenomena::Event::BEGINNING);
	ASSERT_TRUE(wp.time().has_value());
	EXPECT_FALSE(wp.time()->day().has_value());
	EXPECT_EQ(wp.time()->hour(), 12u);
	EXPECT_EQ(wp.time()->minute(), 0u);
}

TEST(WeatherGroup, appendOther) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("281232");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

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

	auto wg3 = metaf::WeatherGroup::parse("RAB1200", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg3.has_value());

	EXPECT_EQ(wg3->append("RMK", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append("12/12", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append("NOSIG", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append("TEMPO", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append("BECMG", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg3->append("/////", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// NSW, PWINO, and TSNO
// Purpose: to confirm that NSW, PWINO, and TSNO groups are parsed correctly,
// and that no other group can be appended to them
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherGroup, parseNsw) {
	const auto wg1 = metaf::WeatherGroup::parse("NSW", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->type(), metaf::WeatherGroup::Type::NSW);
	ASSERT_EQ(wg1->weatherPhenomena().size(), 0u);
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WeatherGroup::parse("NSW", metaf::ReportPart::TAF);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->type(), metaf::WeatherGroup::Type::NSW);
	ASSERT_EQ(wg2->weatherPhenomena().size(), 0u);
	EXPECT_TRUE(wg2->isValid());
}

TEST(WeatherGroup, parseNswWrongReportPart) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("NSW", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("NSW", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("NSW", metaf::ReportPart::RMK).has_value());
}

TEST(WeatherGroup, appendToNsw) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("281232");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WeatherGroup::parse("NSW", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("RMK", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("12/12", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("NOSIG", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("TEMPO", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("BECMG", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RAB1211", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("B1211", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("B11", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RA", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RERA", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("VCSH", metaf::ReportPart::METAR), metaf::AppendResult::NOT_APPENDED);
}

TEST(WeatherGroup, parsePwino) {
	const auto wg1 = metaf::WeatherGroup::parse("PWINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->type(), metaf::WeatherGroup::Type::PWINO);
	ASSERT_EQ(wg1->weatherPhenomena().size(), 0u);
	EXPECT_TRUE(wg1->isValid());
}

TEST(WeatherGroup, parsePwinoWrongReportPart) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("PWINO", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PWINO", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PWINO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("PWINO", metaf::ReportPart::TAF).has_value());
}

TEST(WeatherGroup, appendToPwino) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("281232");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WeatherGroup::parse("PWINO", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("RMK", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("12/12", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("NOSIG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("BECMG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RAB1211", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("B1211", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("B11", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RA", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RERA", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("VCSH", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(WeatherGroup, parseTsno) {
	const auto wg = metaf::WeatherGroup::parse("TSNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::TSNO);
	ASSERT_EQ(wg->weatherPhenomena().size(), 0u);
	EXPECT_TRUE(wg->isValid());
}

TEST(WeatherGroup, parseTsnoWrongReportPart) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSNO", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSNO", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSNO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TSNO", metaf::ReportPart::TAF).has_value());
}

TEST(WeatherGroup, appendToTsno) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("281232");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WeatherGroup::parse("TSNO", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("RMK", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("12/12", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("NOSIG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("BECMG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RAB1211", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("B1211", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("B11", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RA", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RERA", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("VCSH", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// WX MISG and TS/LTNG TEMPO UNAVBL groups
// Purpose: to confirm that WX MISG and TS/LTNG TEMPO UNAVBL groups are parsed
// and appended correctly
///////////////////////////////////////////////////////////////////////////////

TEST(WeatherGroup, parseWxMisg) {
	auto wg = metaf::WeatherGroup::parse("WX", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());
	EXPECT_FALSE(wg->isValid());
	EXPECT_EQ(wg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(wg->isValid());
	EXPECT_EQ(wg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::WX_MISG);
	ASSERT_EQ(wg->weatherPhenomena().size(), 0u);
}

TEST(WeatherGroup, parseWxWrongReportPart) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("WX", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("WX", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("WX", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("WX", metaf::ReportPart::TAF).has_value());
}

TEST(WeatherGroup, appendToWxMisg) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("281232");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WeatherGroup::parse("WX", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->append("RMK", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("12/12", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("NOSIG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("BECMG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RAB1211", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("B1211", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("B11", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RA", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RERA", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("VCSH", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(WeatherGroup, appendOtherToWx) {
	auto wg1 = metaf::WeatherGroup::parse("WX", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg2 = metaf::WeatherGroup::parse("WX", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->append("WX", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg3 = metaf::WeatherGroup::parse("WX", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->append("TSNO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg4 = metaf::WeatherGroup::parse("WX", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->append("RA", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg5 = metaf::WeatherGroup::parse("WX", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->append("RAB1031", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg6 = metaf::WeatherGroup::parse("WX", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg7 = metaf::WeatherGroup::parse("WX", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg7.has_value());
	EXPECT_EQ(wg7->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(WeatherGroup, parseMisg) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("MISG", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MISG", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MISG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MISG", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("MISG", metaf::ReportPart::RMK).has_value());
}

TEST(WeatherGroup, parseTsLtngTempoUnavbl) {
	auto wg = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());
	EXPECT_FALSE(wg->isValid());
	EXPECT_EQ(wg->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_FALSE(wg->isValid());
	EXPECT_EQ(wg->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(wg->isValid());
	EXPECT_EQ(wg->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->type(), metaf::WeatherGroup::Type::TS_LTNG_TEMPO_UNAVBL);
	ASSERT_EQ(wg->weatherPhenomena().size(), 0u);
}

TEST(WeatherGroup, parseTsLtngWrongReportPart) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::TAF).has_value());
}

TEST(WeatherGroup, appendOtherToTsLtng) {
	auto wg1 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg2 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->append("WX", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg3 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->append("TSNO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg4 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->append("RA", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg5 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->append("RAB1031", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg6 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->append("TS/LTNG", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg7 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg7.has_value());
	EXPECT_EQ(wg7->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg8 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg8.has_value());
	EXPECT_EQ(wg8->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(WeatherGroup, appendOtherToTsLtngTempo) {
	auto wg1 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg1.has_value());
	ASSERT_EQ(wg1->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg2 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg2.has_value());
	ASSERT_EQ(wg2->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg2->append("WX", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg3 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg3.has_value());
	ASSERT_EQ(wg3->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg3->append("TSNO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg4 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg4.has_value());
	ASSERT_EQ(wg4->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg4->append("RA", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg5 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg5.has_value());
	ASSERT_EQ(wg5->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg5->append("RAB1031", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg6 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg6.has_value());
	ASSERT_EQ(wg6->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg6->append("TS/LTNG", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg7 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg7.has_value());
	ASSERT_EQ(wg7->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg7->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);

	auto wg8 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg8.has_value());
	ASSERT_EQ(wg8->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg8->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(WeatherGroup, appendToTsLtngTempoUnavbl) {
	auto wg1 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg1.has_value());
	ASSERT_EQ(wg1->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_EQ(wg1->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg1->append("", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto wg2 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg2.has_value());
	ASSERT_EQ(wg2->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_EQ(wg2->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg2->append("WX", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto wg3 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg3.has_value());
	ASSERT_EQ(wg3->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_EQ(wg3->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg3->append("TSNO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto wg4 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg4.has_value());
	ASSERT_EQ(wg4->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_EQ(wg4->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg4->append("RA", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto wg5 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg5.has_value());
	ASSERT_EQ(wg5->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_EQ(wg5->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg5->append("RAB1031", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto wg6 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg6.has_value());
	ASSERT_EQ(wg6->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_EQ(wg6->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg6->append("TS/LTNG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto wg7 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg7.has_value());
	ASSERT_EQ(wg7->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_EQ(wg7->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg7->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto wg8 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg8.has_value());
	ASSERT_EQ(wg8->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_EQ(wg8->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg8->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);

	auto wg9 = metaf::WeatherGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg9.has_value());
	ASSERT_EQ(wg9->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_EQ(wg9->append("UNAVBL", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg9->append("MISG", metaf::ReportPart::RMK), metaf::AppendResult::NOT_APPENDED);
}

TEST(WeatherGroup, parseTempo) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("TEMPO", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TEMPO", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TEMPO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TEMPO", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("TEMPO", metaf::ReportPart::RMK).has_value());
}

TEST(WeatherGroup, parseUnavbl) {
	EXPECT_FALSE(metaf::WeatherGroup::parse("UNAVBL", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("UNAVBL", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("UNAVBL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("UNAVBL", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::WeatherGroup::parse("UNAVBL", metaf::ReportPart::RMK).has_value());
}
