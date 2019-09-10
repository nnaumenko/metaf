/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "testdata_real.h"

TEST(Parser, RealDataParsingMETAR) {
	for (const auto & data : testdata::realDataSet) {
		if (strlen(data.metar)) {
			const std::string report = data.metar;
			metaf::Parser::Result parseResult;
			EXPECT_NO_THROW({ parseResult = metaf::Parser::parse(report); });
			EXPECT_EQ(parseResult.error, metaf::Parser::Error::NONE) <<
				"Report: " << data.metar;
			EXPECT_EQ(parseResult.reportType, metaf::ReportType::METAR) << 
				"Report: " << data.metar;
		}
	}
}

TEST(Parser, RealDataParsingTAF) {
	for (const auto & data : testdata::realDataSet) {
		if (strlen(data.taf)) {
			const std::string report = data.taf;
			metaf::Parser::Result parseResult;
			EXPECT_NO_THROW({ parseResult = metaf::Parser::parse(report); });
			EXPECT_EQ(parseResult.error, metaf::Parser::Error::NONE) <<
				"Report: " << data.taf;
			EXPECT_EQ(parseResult.reportType, metaf::ReportType::TAF) << 
				"Report: " << data.taf;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

static bool isPlainText(const metaf::Group & group, const std::string & str) {
	if (!std::holds_alternative<metaf::PlainTextGroup>(group)) return(false);
	return(std::get<metaf::PlainTextGroup>(group).toString() == str);
}

static bool isMetar(const metaf::Group & group) {
	if (!std::holds_alternative<metaf::FixedGroup>(group)) return(false);
	return (std::get<metaf::FixedGroup>(group).type() == metaf::FixedGroup::Type::METAR);
}

static bool isSpeci(const metaf::Group & group) {
	if (!std::holds_alternative<metaf::FixedGroup>(group)) return(false);
	return (std::get<metaf::FixedGroup>(group).type() == metaf::FixedGroup::Type::SPECI);
}

static bool isTaf(const metaf::Group & group) {
	if (!std::holds_alternative<metaf::FixedGroup>(group)) return(false);
	return (std::get<metaf::FixedGroup>(group).type() == metaf::FixedGroup::Type::TAF);
}

static bool isAmd(const metaf::Group & group) {
	if (!std::holds_alternative<metaf::FixedGroup>(group)) return(false);
	return (std::get<metaf::FixedGroup>(group).type() == metaf::FixedGroup::Type::AMD);
}

static bool isNil(const metaf::Group & group) {
	if (!std::holds_alternative<metaf::FixedGroup>(group)) return(false);
	return (std::get<metaf::FixedGroup>(group).type() == metaf::FixedGroup::Type::NIL);
}

static bool isCnl(const metaf::Group & group) {
	if (!std::holds_alternative<metaf::FixedGroup>(group)) return(false);
	return (std::get<metaf::FixedGroup>(group).type() == metaf::FixedGroup::Type::CNL);
}

static bool isCor(const metaf::Group & group) {
	if (!std::holds_alternative<metaf::FixedGroup>(group)) return(false);
	return (std::get<metaf::FixedGroup>(group).type() == metaf::FixedGroup::Type::COR);
}

static bool isRmk(const metaf::Group & group) {
	if (!std::holds_alternative<metaf::FixedGroup>(group)) return(false);
	return (std::get<metaf::FixedGroup>(group).type() == metaf::FixedGroup::Type::RMK);
}

static bool isMaintenanceIndicator(const metaf::Group & group) {
	if (!std::holds_alternative<metaf::FixedGroup>(group)) return(false);
	return (std::get<metaf::FixedGroup>(group).type() == 
		metaf::FixedGroup::Type::MAINTENANCE_INDICATOR);
}

static bool isFixedGroup(const metaf::Group & group) {
	return (std::holds_alternative<metaf::FixedGroup>(group));
}

static bool isLocation(const metaf::Group & group) {
	return (std::holds_alternative<metaf::LocationGroup>(group));
}

static bool isReportTime(const metaf::Group & group) {
	return (std::holds_alternative<metaf::ReportTimeGroup>(group));
}

static bool isTrend(const metaf::Group & group) {
	return(std::holds_alternative<metaf::TrendGroup>(group));
}

static bool isTimeSpan(const metaf::Group & group) {
	if(!std::holds_alternative<metaf::TrendGroup>(group)) return(false);
	return (std::get<metaf::TrendGroup>(group).isTimeSpanGroup());
}

static bool isWind(const metaf::Group & group) {
	return(std::holds_alternative<metaf::WindGroup>(group));
}

static bool isVisibility(const metaf::Group & group) {
	return(std::holds_alternative<metaf::VisibilityGroup>(group));
}

static bool isCloud(const metaf::Group & group) {
	return(std::holds_alternative<metaf::CloudGroup>(group));
}

static bool isWeather(const metaf::Group & group) {
	return(std::holds_alternative<metaf::WeatherGroup>(group));
}

static bool isTemperature(const metaf::Group & group) {
	return(std::holds_alternative<metaf::TemperatureGroup>(group));
}

static bool isTempForecast(const metaf::Group & group) {
	return(std::holds_alternative<metaf::TemperatureForecastGroup>(group));
}

static bool isPressure(const metaf::Group & group) {
	return(std::holds_alternative<metaf::PressureGroup>(group));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxMalformedReports, emptyReport) {
	const auto result1 = metaf::Parser::parse("");
	EXPECT_EQ(result1.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result1.error, metaf::Parser::Error::EMPTY_REPORT);
	EXPECT_EQ(result1.groups.size(), 0u);

	const auto result2 = metaf::Parser::parse("=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EMPTY_REPORT);
	EXPECT_EQ(result2.groups.size(), 0u);
}

TEST(ParserSyntaxMalformedReports, location) {
	const auto result = metaf::Parser::parse("ZZZZ=");
	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result.groups.size(), 1u);
	EXPECT_TRUE(isLocation(result.groups.at(0).group));
}

TEST(ParserSyntaxMalformedReports, typeLocation) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result1.groups.size(), 2u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));

	const auto result2 = metaf::Parser::parse("SPECI ZZZZ=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result2.groups.size(), 2u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));

	const auto result3 = metaf::Parser::parse("TAF ZZZZ=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result3.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result3.groups.size(), 2u);
	EXPECT_TRUE(isTaf(result3.groups.at(0).group));
	EXPECT_TRUE(isLocation(result3.groups.at(1).group));
}

TEST(ParserSyntaxMalformedReports, strayGroupInPlaceOfLocation) {
	const auto result1 = metaf::Parser::parse("METAR 9999=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::EXPECTED_LOCATION);
	EXPECT_EQ(result1.groups.size(), 2u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isPlainText(result1.groups.at(1).group, "9999"));

	const auto result2 = metaf::Parser::parse("SPECI 9999=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EXPECTED_LOCATION);
	EXPECT_EQ(result2.groups.size(), 2u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0).group));
	EXPECT_TRUE(isPlainText(result2.groups.at(1).group, "9999"));

	const auto result3 = metaf::Parser::parse("TAF 9999=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result3.error, metaf::Parser::Error::EXPECTED_LOCATION);
	EXPECT_EQ(result3.groups.size(), 2u);
	EXPECT_TRUE(isTaf(result3.groups.at(0).group));
	EXPECT_TRUE(isPlainText(result3.groups.at(1).group, "9999"));
}

TEST(ParserSyntaxMalformedReports, locationTime) {
	const auto result = metaf::Parser::parse("ZZZZ 041115Z=");
	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result.groups.size(), 2u);
	EXPECT_TRUE(isLocation(result.groups.at(0).group));
	EXPECT_TRUE(isReportTime(result.groups.at(1).group));
}

TEST(ParserSyntaxMalformedReports, typeLocationTime) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result1.groups.size(), 3u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));

	const auto result2 = metaf::Parser::parse("SPECI ZZZZ 041115Z=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result2.groups.size(), 3u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));

	const auto result3 = metaf::Parser::parse("TAF ZZZZ 041115Z=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result3.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result3.groups.size(), 3u);
	EXPECT_TRUE(isTaf(result3.groups.at(0).group));
	EXPECT_TRUE(isLocation(result3.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result3.groups.at(2).group));
}

TEST(ParserSyntaxMalformedReports, strayGroupInPlaceOfReportTime) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ ZZZZ=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result1.groups.size(), 3u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isLocation(result1.groups.at(2).group));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ ZZZZ=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result2.groups.size(), 3u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isLocation(result2.groups.at(2).group));

	const auto result3 = metaf::Parser::parse("ZZZZ ZZZZ=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result3.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result3.groups.size(), 2u);
	EXPECT_TRUE(isLocation(result3.groups.at(0).group));
	EXPECT_TRUE(isLocation(result3.groups.at(1).group));
}

TEST(ParserSyntaxMalformedReports, strayGroupInPlaceOfTimeSpan) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z ZZZZ=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::EXPECTED_TIME_SPAN);
	EXPECT_EQ(result.groups.size(), 4u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isLocation(result.groups.at(3).group));
}

TEST(ParserSyntaxMalformedReports, tafReportTypeAndTimeNotSpecified) {
	const auto result = metaf::Parser::parse("ZZZZ 0412/0512 TX07/0416Z TN03/0505Z=");
	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result.groups.size(), 2u);
	EXPECT_TRUE(isLocation(result.groups.at(0).group));
	EXPECT_TRUE(isTimeSpan(result.groups.at(1).group));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxNilReports, locationNil) {
	const auto result = metaf::Parser::parse("ZZZZ NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 2u);
	EXPECT_TRUE(isLocation(result.groups.at(0).group));
	EXPECT_TRUE(isNil(result.groups.at(1).group));
}

TEST(ParserSyntaxNilReports, typeLocationNil) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ NIL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 3u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isNil(result1.groups.at(2).group));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ NIL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 3u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isNil(result2.groups.at(2).group));
}

TEST(ParserSyntaxNilReports, corAmdLocationNil) {
	const auto result1 = metaf::Parser::parse("COR ZZZZ NIL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result1.error, metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION);
	EXPECT_EQ(result1.groups.size(), 1u);
	EXPECT_TRUE(isCor(result1.groups.at(0).group));

	const auto result2 = metaf::Parser::parse("AMD ZZZZ NIL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION);
	EXPECT_EQ(result2.groups.size(), 1u);
	EXPECT_TRUE(isAmd(result2.groups.at(0).group));
}

TEST(ParserSyntaxNilReports, metarCorLocationNil) {
	const auto result = metaf::Parser::parse("METAR COR ZZZZ NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 4u);
	EXPECT_TRUE(isMetar(result.groups.at(0).group));
	EXPECT_TRUE(isCor(result.groups.at(1).group));
	EXPECT_TRUE(isLocation(result.groups.at(2).group));
	EXPECT_TRUE(isNil(result.groups.at(3).group));
}

TEST(ParserSyntaxNilReports, tafCorLocationNil) {
	const auto result = metaf::Parser::parse("TAF COR ZZZZ NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 4u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isCor(result.groups.at(1).group));
	EXPECT_TRUE(isLocation(result.groups.at(2).group));
	EXPECT_TRUE(isNil(result.groups.at(3).group));
}

TEST(ParserSyntaxNilReports, metarAmdLocationNil) {
	const auto result = metaf::Parser::parse("METAR AMD ZZZZ NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::AMD_ALLOWED_IN_TAF_ONLY);
	EXPECT_EQ(result.groups.size(), 2u);
	EXPECT_TRUE(isMetar(result.groups.at(0).group));
	EXPECT_TRUE(isAmd(result.groups.at(1).group));
}

TEST(ParserSyntaxNilReports, tafAmdLocationNil) {
	const auto result = metaf::Parser::parse("TAF AMD ZZZZ NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 4u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isAmd(result.groups.at(1).group));
	EXPECT_TRUE(isLocation(result.groups.at(2).group));
	EXPECT_TRUE(isNil(result.groups.at(3).group));
}

TEST(ParserSyntaxMalformedReports, corStrayGroupInPlaceOfLocation) {
	const auto result1 = metaf::Parser::parse("METAR COR 9999=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::EXPECTED_LOCATION);
	EXPECT_EQ(result1.groups.size(), 3u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isCor(result1.groups.at(1).group));
	EXPECT_TRUE(isPlainText(result1.groups.at(2).group, "9999"));

	const auto result2 = metaf::Parser::parse("TAF COR 9999=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EXPECTED_LOCATION);
	EXPECT_EQ(result2.groups.size(), 3u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isCor(result2.groups.at(1).group));
	EXPECT_TRUE(isPlainText(result2.groups.at(2).group, "9999"));
}

TEST(ParserSyntaxMalformedReports, amdStrayGroupInPlaceOfLocation) {
	const auto result = metaf::Parser::parse("TAF AMD 9999=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::EXPECTED_LOCATION);
	EXPECT_EQ(result.groups.size(), 3u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isAmd(result.groups.at(1).group));
	EXPECT_TRUE(isPlainText(result.groups.at(2).group, "9999"));
}

TEST(ParserSyntaxNilReports, locationTimeNil) {
	const auto result = metaf::Parser::parse("ZZZZ 041115Z NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 3u);
	EXPECT_TRUE(isLocation(result.groups.at(0).group));
	EXPECT_TRUE(isReportTime(result.groups.at(1).group));
	EXPECT_TRUE(isNil(result.groups.at(2).group));
}

TEST(ParserSyntaxNilReports, typeLocationTimeNil) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z NIL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 4u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isNil(result1.groups.at(3).group));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z NIL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 4u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isNil(result2.groups.at(3).group));
}

TEST(ParserSyntaxNilReports, typeCorAmdLocationTimeNil) {
	const auto result1 = metaf::Parser::parse("METAR COR ZZZZ 041115Z NIL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 5u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isCor(result1.groups.at(1).group));
	EXPECT_TRUE(isLocation(result1.groups.at(2).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(3).group));
	EXPECT_TRUE(isNil(result1.groups.at(4).group));

	const auto result2 = metaf::Parser::parse("TAF AMD ZZZZ 041115Z NIL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isAmd(result2.groups.at(1).group));
	EXPECT_TRUE(isLocation(result2.groups.at(2).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(3).group));
	EXPECT_TRUE(isNil(result2.groups.at(4).group));
}

TEST(ParserSyntaxNilReports, strayGroupAfterNil) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z NIL 9999=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
	EXPECT_EQ(result1.groups.size(), 5u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isNil(result1.groups.at(3).group));
	EXPECT_TRUE(isPlainText(result1.groups.at(4).group, "9999"));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z NIL 9999=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
	EXPECT_EQ(result2.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isNil(result2.groups.at(3).group));
	EXPECT_TRUE(isPlainText(result2.groups.at(4).group, "9999"));

	const auto result3 = metaf::Parser::parse("ZZZZ 041115Z NIL 9999=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result3.error, metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
	EXPECT_EQ(result3.groups.size(), 4u);
	EXPECT_TRUE(isLocation(result3.groups.at(0).group));
	EXPECT_TRUE(isReportTime(result3.groups.at(1).group));
	EXPECT_TRUE(isNil(result3.groups.at(2).group));
	EXPECT_TRUE(isPlainText(result3.groups.at(3).group, "9999"));
}

TEST(ParserSyntaxNilReports, strayNilInReportBody) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 NIL FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(result1.groups.size(), 6u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isWind(result1.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(4).group));
	EXPECT_TRUE(isNil(result1.groups.at(5).group));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT NIL 10SM FEW250=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(result2.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3).group));
	EXPECT_TRUE(isWind(result2.groups.at(4).group));
	EXPECT_TRUE(isNil(result2.groups.at(5).group));

	const auto result3 = metaf::Parser::parse("ZZZZ 041115Z 24005KT 9999 NIL FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result3.error, metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(result3.groups.size(), 5u);
	EXPECT_TRUE(isLocation(result3.groups.at(0).group));
	EXPECT_TRUE(isReportTime(result3.groups.at(1).group));
	EXPECT_TRUE(isWind(result3.groups.at(2).group));
	EXPECT_TRUE(isVisibility(result3.groups.at(3).group));
	EXPECT_TRUE(isNil(result3.groups.at(4).group));
}

TEST(ParserSyntaxNilReports, typeLocationTimeTimespanNil) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0415/0424 NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3).group));
	EXPECT_TRUE(isNil(result.groups.at(4).group));
}

TEST(ParserSyntaxNilReports, typeCorAmdLocationTimeTimespanNil) {
	const auto result1 = metaf::Parser::parse("TAF COR ZZZZ 041115Z 0415/0424 NIL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result1.groups.at(0).group));
	EXPECT_TRUE(isCor(result1.groups.at(1).group));
	EXPECT_TRUE(isLocation(result1.groups.at(2).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(3).group));
	EXPECT_TRUE(isTimeSpan(result1.groups.at(4).group));
	EXPECT_TRUE(isNil(result1.groups.at(5).group));

	const auto result2 = metaf::Parser::parse("TAF AMD ZZZZ 041115Z 0415/0424 NIL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isAmd(result2.groups.at(1).group));
	EXPECT_TRUE(isLocation(result2.groups.at(2).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(3).group));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(4).group));
	EXPECT_TRUE(isNil(result2.groups.at(5).group));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxCancelledReports, typeLocationCnl) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ CNL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result1.groups.size(), 3u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isCnl(result1.groups.at(2).group));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ CNL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result2.groups.size(), 3u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isCnl(result2.groups.at(2).group));
}

TEST(ParserSyntaxCancelledReports, typeLocationTimeCnl) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z CNL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::CNL_ALLOWED_IN_TAF_ONLY);
	EXPECT_EQ(result1.groups.size(), 4u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isCnl(result1.groups.at(3).group));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z CNL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EXPECTED_TIME_SPAN);
	EXPECT_EQ(result2.groups.size(), 4u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isCnl(result2.groups.at(3).group));
}

TEST(ParserSyntaxCancelledReports, strayGroupAfterCnl) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0415/0424 CNL 9999=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_CNL);
	EXPECT_EQ(result.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3).group));
	EXPECT_TRUE(isCnl(result.groups.at(4).group));
	EXPECT_TRUE(isPlainText(result.groups.at(5).group, "9999"));
}

TEST(ParserSyntaxCancelledReports, strayCnlInReportBody) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT CNL 10SM FEW250=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(result.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3).group));
	EXPECT_TRUE(isWind(result.groups.at(4).group));
	EXPECT_TRUE(isCnl(result.groups.at(5).group));
}

TEST(ParserSyntaxCancelledReports, strayCnlInMetar) {
	const auto result = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 CNL FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(result.groups.size(), 6u);
	EXPECT_TRUE(isMetar(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isWind(result.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result.groups.at(4).group));
	EXPECT_TRUE(isCnl(result.groups.at(5).group));
}

TEST(ParserSyntaxCancelledReports, typeLocationTimeTimespanCnl) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0415/0424 CNL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3).group));
	EXPECT_TRUE(isCnl(result.groups.at(4).group));
}

TEST(ParserSyntaxCancelledReports, typeCorAmdLocationTimeTimespanCnl) {
	const auto result1 = metaf::Parser::parse("TAF COR ZZZZ 041115Z 0415/0424 CNL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result1.groups.at(0).group));
	EXPECT_TRUE(isCor(result1.groups.at(1).group));
	EXPECT_TRUE(isLocation(result1.groups.at(2).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(3).group));
	EXPECT_TRUE(isTimeSpan(result1.groups.at(4).group));
	EXPECT_TRUE(isCnl(result1.groups.at(5).group));

	const auto result2 = metaf::Parser::parse("TAF AMD ZZZZ 041115Z 0415/0424 CNL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isAmd(result2.groups.at(1).group));
	EXPECT_TRUE(isLocation(result2.groups.at(2).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(3).group));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(4).group));
	EXPECT_TRUE(isCnl(result2.groups.at(5).group));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxValidMetarReports, plainReports) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 9u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isWind(result1.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(4).group));
	EXPECT_TRUE(isCloud(result1.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result1.groups.at(6).group));
	EXPECT_TRUE(isPressure(result1.groups.at(7).group));
	EXPECT_TRUE(isTrend(result1.groups.at(8).group));

	const auto result2 = metaf::Parser::parse("SPECI ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 9u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isWind(result2.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result2.groups.at(4).group));
	EXPECT_TRUE(isCloud(result2.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result2.groups.at(6).group));
	EXPECT_TRUE(isPressure(result2.groups.at(7).group));
	EXPECT_TRUE(isTrend(result2.groups.at(8).group));

	const auto result3 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 5000 RASN 01/01 Q0980=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result3.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result3.groups.size(), 8u);
	EXPECT_TRUE(isMetar(result3.groups.at(0).group));
	EXPECT_TRUE(isLocation(result3.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result3.groups.at(2).group));
	EXPECT_TRUE(isWind(result3.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result3.groups.at(4).group));
	EXPECT_TRUE(isWeather(result3.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result3.groups.at(6).group));
	EXPECT_TRUE(isPressure(result3.groups.at(7).group));
}

TEST(ParserSyntaxValidMetarReports, correctionalReports) {
	const auto result1 = metaf::Parser::parse("METAR COR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 10u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isCor(result1.groups.at(1).group));
	EXPECT_TRUE(isLocation(result1.groups.at(2).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(3).group));
	EXPECT_TRUE(isWind(result1.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(5).group));
	EXPECT_TRUE(isCloud(result1.groups.at(6).group));
	EXPECT_TRUE(isTemperature(result1.groups.at(7).group));
	EXPECT_TRUE(isPressure(result1.groups.at(8).group));
	EXPECT_TRUE(isTrend(result1.groups.at(9).group));

	const auto result2 = metaf::Parser::parse("SPECI COR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 10u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0).group));
	EXPECT_TRUE(isCor(result2.groups.at(1).group));
	EXPECT_TRUE(isLocation(result2.groups.at(2).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(3).group));
	EXPECT_TRUE(isWind(result2.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result2.groups.at(5).group));
	EXPECT_TRUE(isCloud(result2.groups.at(6).group));
	EXPECT_TRUE(isTemperature(result2.groups.at(7).group));
	EXPECT_TRUE(isPressure(result2.groups.at(8).group));
	EXPECT_TRUE(isTrend(result2.groups.at(9).group));

	const auto result3 = metaf::Parser::parse("METAR ZZZZ 041115Z COR 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result3.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result3.groups.size(), 10u);
	EXPECT_TRUE(isMetar(result3.groups.at(0).group));
	EXPECT_TRUE(isLocation(result3.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result3.groups.at(2).group));
	EXPECT_TRUE(isCor(result3.groups.at(3).group));
	EXPECT_TRUE(isWind(result3.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result3.groups.at(5).group));
	EXPECT_TRUE(isCloud(result3.groups.at(6).group));
	EXPECT_TRUE(isTemperature(result3.groups.at(7).group));
	EXPECT_TRUE(isPressure(result3.groups.at(8).group));
	EXPECT_TRUE(isTrend(result3.groups.at(9).group));
}

TEST(ParserSyntaxValidMetarReports, reportsWithRemarks) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 11u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isWind(result1.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(4).group));
	EXPECT_TRUE(isCloud(result1.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result1.groups.at(6).group));
	EXPECT_TRUE(isPressure(result1.groups.at(7).group));
	EXPECT_TRUE(isTrend(result1.groups.at(8).group));
	EXPECT_TRUE(isRmk(result1.groups.at(9).group));
	EXPECT_TRUE(isPlainText(result1.groups.at(10).group, "TEST"));

	const auto result2 = metaf::Parser::parse("SPECI ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 11u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isWind(result2.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result2.groups.at(4).group));
	EXPECT_TRUE(isCloud(result2.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result2.groups.at(6).group));
	EXPECT_TRUE(isPressure(result2.groups.at(7).group));
	EXPECT_TRUE(isTrend(result2.groups.at(8).group));
	EXPECT_TRUE(isRmk(result2.groups.at(9).group));
	EXPECT_TRUE(isPlainText(result2.groups.at(10).group, "TEST"));
}

TEST(ParserSyntaxValidMetarReports, reportsWithAppendedRemarks) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 11u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isWind(result1.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(4).group));
	EXPECT_TRUE(isCloud(result1.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result1.groups.at(6).group));
	EXPECT_TRUE(isPressure(result1.groups.at(7).group));
	EXPECT_TRUE(isTrend(result1.groups.at(8).group));
	EXPECT_TRUE(isRmk(result1.groups.at(9).group));
	EXPECT_TRUE(isPlainText(result1.groups.at(10).group, "TEST 9999 RMK"));

	const auto result2 = metaf::Parser::parse("SPECI ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 11u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isWind(result2.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result2.groups.at(4).group));
	EXPECT_TRUE(isCloud(result2.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result2.groups.at(6).group));
	EXPECT_TRUE(isPressure(result2.groups.at(7).group));
	EXPECT_TRUE(isTrend(result2.groups.at(8).group));
	EXPECT_TRUE(isRmk(result2.groups.at(9).group));
	EXPECT_TRUE(isPlainText(result2.groups.at(10).group, "TEST 9999 RMK"));
}

TEST(ParserSyntaxValidMetarReports, reportTypeNotSpecified) {
	const auto result1 = metaf::Parser::parse("ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 8u);
	EXPECT_TRUE(isLocation(result1.groups.at(0).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(1).group));
	EXPECT_TRUE(isWind(result1.groups.at(2).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(3).group));
	EXPECT_TRUE(isCloud(result1.groups.at(4).group));
	EXPECT_TRUE(isTemperature(result1.groups.at(5).group));
	EXPECT_TRUE(isPressure(result1.groups.at(6).group));
	EXPECT_TRUE(isTrend(result1.groups.at(7).group));

	const auto result2 = metaf::Parser::parse("ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 10u);
	EXPECT_TRUE(isLocation(result2.groups.at(0).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(1).group));
	EXPECT_TRUE(isWind(result2.groups.at(2).group));
	EXPECT_TRUE(isVisibility(result2.groups.at(3).group));
	EXPECT_TRUE(isCloud(result2.groups.at(4).group));
	EXPECT_TRUE(isTemperature(result2.groups.at(5).group));
	EXPECT_TRUE(isPressure(result2.groups.at(6).group));
	EXPECT_TRUE(isTrend(result2.groups.at(7).group));
	EXPECT_TRUE(isRmk(result2.groups.at(8).group));
	EXPECT_TRUE(isPlainText(result2.groups.at(9).group, "TEST"));

	const auto result3 = metaf::Parser::parse("ZZZZ 041115Z RMK TEST=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result3.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result3.groups.size(), 4u);
	EXPECT_TRUE(isLocation(result3.groups.at(0).group));
	EXPECT_TRUE(isReportTime(result3.groups.at(1).group));
	EXPECT_TRUE(isRmk(result3.groups.at(2).group));
	EXPECT_TRUE(isPlainText(result3.groups.at(3).group, "TEST"));
}

TEST(ParserSyntaxValidMetarReports, reportWithTrend) {
	const auto result = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 "
		"TEMPO 3500 BKN010 +RA=");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 12u);
	EXPECT_TRUE(isMetar(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isWind(result.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result.groups.at(4).group));
	EXPECT_TRUE(isCloud(result.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result.groups.at(6).group));
	EXPECT_TRUE(isPressure(result.groups.at(7).group));
	EXPECT_TRUE(isTrend(result.groups.at(8).group));
	EXPECT_TRUE(isVisibility(result.groups.at(9).group));
	EXPECT_TRUE(isCloud(result.groups.at(10).group));
	EXPECT_TRUE(isWeather(result.groups.at(11).group));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxValidTafReports, plainReport) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 7u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3).group));
	EXPECT_TRUE(isWind(result.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result.groups.at(5).group));
	EXPECT_TRUE(isCloud(result.groups.at(6).group));
}

TEST(ParserSyntaxValidTafReports, amendedAndCorrectionalReports) {
	const auto result1 = metaf::Parser::parse("TAF AMD ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 8u);
	EXPECT_TRUE(isTaf(result1.groups.at(0).group));
	EXPECT_TRUE(isAmd(result1.groups.at(1).group));
	EXPECT_TRUE(isLocation(result1.groups.at(2).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(3).group));
	EXPECT_TRUE(isTimeSpan(result1.groups.at(4).group));
	EXPECT_TRUE(isWind(result1.groups.at(5).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(6).group));
	EXPECT_TRUE(isCloud(result1.groups.at(7).group));

	const auto result2 = metaf::Parser::parse("TAF COR ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 8u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isCor(result2.groups.at(1).group));
	EXPECT_TRUE(isLocation(result2.groups.at(2).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(3).group));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(4).group));
	EXPECT_TRUE(isWind(result2.groups.at(5).group));
	EXPECT_TRUE(isVisibility(result2.groups.at(6).group));
	EXPECT_TRUE(isCloud(result2.groups.at(7).group));
}

TEST(ParserSyntaxValidTafReports, reportsWithTemperatureForecasts) {
	const auto result1 = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 TX07/0416Z=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result1.groups.at(3).group));
	EXPECT_TRUE(isTempForecast(result1.groups.at(4).group));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 TX07/0416Z TN03/0505Z=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3).group));
	EXPECT_TRUE(isTempForecast(result2.groups.at(4).group));
	EXPECT_TRUE(isTempForecast(result2.groups.at(5).group));
}

TEST(ParserSyntaxValidTafReports, reportWithRemarks) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250 RMK TEST=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 9u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3).group));
	EXPECT_TRUE(isWind(result.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result.groups.at(5).group));
	EXPECT_TRUE(isCloud(result.groups.at(6).group));
	EXPECT_TRUE(isRmk(result.groups.at(7).group));
	EXPECT_TRUE(isPlainText(result.groups.at(8).group, "TEST"));
}

TEST(ParserSyntaxValidTafReports, reportWithAppendedRemarks) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250 RMK TEST 9999 RMK=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 9u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3).group));
	EXPECT_TRUE(isWind(result.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result.groups.at(5).group));
	EXPECT_TRUE(isCloud(result.groups.at(6).group));
	EXPECT_TRUE(isRmk(result.groups.at(7).group));
	EXPECT_TRUE(isPlainText(result.groups.at(8).group, "TEST 9999 RMK"));
}

TEST(ParserSyntaxValidTafReports, reportWithTrends) {
	const auto result = metaf::Parser::parse(
		"TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250"
		" PROB40"
		" TEMPO 0420/0424 24010G15KT"
		" FM050300 BKN100 3SM RA"
		" BECMG 0506/0510 OVC050"
		"=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 15u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3).group));
	EXPECT_TRUE(isWind(result.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result.groups.at(5).group));
	EXPECT_TRUE(isCloud(result.groups.at(6).group));
	EXPECT_TRUE(isTrend(result.groups.at(7).group));
	EXPECT_TRUE(isWind(result.groups.at(8).group));
	EXPECT_TRUE(isTrend(result.groups.at(9).group));
	EXPECT_TRUE(isCloud(result.groups.at(10).group));
	EXPECT_TRUE(isVisibility(result.groups.at(11).group));
	EXPECT_TRUE(isWeather(result.groups.at(12).group));
	EXPECT_TRUE(isTrend(result.groups.at(13).group));
	EXPECT_TRUE(isCloud(result.groups.at(14).group));
}

TEST(ParserSyntaxValidTafReports, reportTypeNotSpecified) {
	const auto result1 = metaf::Parser::parse("ZZZZ 041115Z 0412/0512 TX07/0416Z=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 4u);
	EXPECT_TRUE(isLocation(result1.groups.at(0).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(1).group));
	EXPECT_TRUE(isTimeSpan(result1.groups.at(2).group));
	EXPECT_TRUE(isTempForecast(result1.groups.at(3).group));

	const auto result2 = metaf::Parser::parse("ZZZZ 041115Z 0412/0512 RMK TEST=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 5u);
	EXPECT_TRUE(isLocation(result2.groups.at(0).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(1).group));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(2).group));
	EXPECT_TRUE(isRmk(result2.groups.at(3).group));
	EXPECT_TRUE(isPlainText(result2.groups.at(4).group, "TEST"));
}

TEST(ParserSyntaxValidTafReports, reportTimeNotSpecified) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 0412/0512 TX07/0416Z TN03/0505Z=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group));
	EXPECT_TRUE(isTimeSpan(result.groups.at(2).group));
	EXPECT_TRUE(isTempForecast(result.groups.at(3).group));
	EXPECT_TRUE(isTempForecast(result.groups.at(4).group));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxDesignatorsAndSeparators, noReportEndDesignator) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 9u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isWind(result1.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(4).group));
	EXPECT_TRUE(isCloud(result1.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result1.groups.at(6).group));
	EXPECT_TRUE(isPressure(result1.groups.at(7).group));
	EXPECT_TRUE(isTrend(result1.groups.at(8).group));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 7u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3).group));
	EXPECT_TRUE(isWind(result2.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result2.groups.at(5).group));
	EXPECT_TRUE(isCloud(result2.groups.at(6).group));
}

TEST(ParserSyntaxDesignatorsAndSeparators, separatorInLeadingPosition) {
	const auto result1 = metaf::Parser::parse(" METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 9u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isWind(result1.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(4).group));
	EXPECT_TRUE(isCloud(result1.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result1.groups.at(6).group));
	EXPECT_TRUE(isPressure(result1.groups.at(7).group));
	EXPECT_TRUE(isTrend(result1.groups.at(8).group));

	const auto result2 = metaf::Parser::parse(" TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 7u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3).group));
	EXPECT_TRUE(isWind(result2.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result2.groups.at(5).group));
	EXPECT_TRUE(isCloud(result2.groups.at(6).group));
}

TEST(ParserSyntaxDesignatorsAndSeparators, variousGroupSeparators) {
	const auto result1 = metaf::Parser::parse(
		"METAR  ZZZZ    041115Z \r\n 24005KT\r9999\nFEW040\r\n25/18 \nQ1011\r NOSIG=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 9u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isWind(result1.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(4).group));
	EXPECT_TRUE(isCloud(result1.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result1.groups.at(6).group));
	EXPECT_TRUE(isPressure(result1.groups.at(7).group));
	EXPECT_TRUE(isTrend(result1.groups.at(8).group));

	const auto result2 = metaf::Parser::parse(
		"TAF  ZZZZ      041115Z \r\n 0412/0512\r24005KT\n10SM\r\nFEW250=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 7u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3).group));
	EXPECT_TRUE(isWind(result2.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result2.groups.at(5).group));
	EXPECT_TRUE(isCloud(result2.groups.at(6).group));
}

TEST(ParserSyntaxDesignatorsAndSeparators, strayTextAfterReportEndDesignator) {
	const auto result1 = metaf::Parser::parse(
		"METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG= STRAY TEXT 1000");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 9u);
	EXPECT_TRUE(isMetar(result1.groups.at(0).group));
	EXPECT_TRUE(isLocation(result1.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result1.groups.at(2).group));
	EXPECT_TRUE(isWind(result1.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result1.groups.at(4).group));
	EXPECT_TRUE(isCloud(result1.groups.at(5).group));
	EXPECT_TRUE(isTemperature(result1.groups.at(6).group));
	EXPECT_TRUE(isPressure(result1.groups.at(7).group));
	EXPECT_TRUE(isTrend(result1.groups.at(8).group));

	const auto result2 = metaf::Parser::parse(
		"TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250= STRAY TEXT 1000");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 7u);
	EXPECT_TRUE(isTaf(result2.groups.at(0).group));
	EXPECT_TRUE(isLocation(result2.groups.at(1).group));
	EXPECT_TRUE(isReportTime(result2.groups.at(2).group));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3).group));
	EXPECT_TRUE(isWind(result2.groups.at(4).group));
	EXPECT_TRUE(isVisibility(result2.groups.at(5).group));
	EXPECT_TRUE(isCloud(result2.groups.at(6).group));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtMetarBodyBegin) {
	const auto result = metaf::Parser::parse("METAR ZZZZ 041115Z $");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back().group));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTafBodyBegin) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 180712Z 1807/1821 $");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back().group));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfMetar) {
	const auto result = metaf::Parser::parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO $");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back().group));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfMetarRemark) {
	const auto result = metaf::Parser::parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO $");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back().group));
}

TEST(ParserSyntaxMaintenanceIndicator, 
	maintenanceIndicatorAtTheEndOfMetarRemarkWithReportEndDesignator)
{
	const auto result = metaf::Parser::parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO $=");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back().group));
}


TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfTaf) {
	const auto result = metaf::Parser::parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 FEW020TCU BKN035 $");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back().group));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfTafRemark) {
	const auto result = metaf::Parser::parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 FEW020TCU BKN035 RMK TEST $");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back().group));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorInTheMiddleOfMetar) {
	const auto result = metaf::Parser::parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR $ 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, 
		metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_MAINTENANCE_INDICATOR);
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorInTheMiddleOfMetarRemark) {
	const auto result = metaf::Parser::parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 $ 58001 PNO TSNO");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, 
		metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_MAINTENANCE_INDICATOR);
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorInTheMiddleOfTaf) {
	const auto result = metaf::Parser::parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 $ FEW020TCU BKN035");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back().group));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorInTheMiddleOfTafRemark) {
	const auto result = metaf::Parser::parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 FEW020TCU BKN035 RMK TEST $ TEST");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back().group));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAfterNil) {
	const auto result = metaf::Parser::parse("METAR ZZZZ 041115Z NIL $");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserAppended, appendedGroups) {
	const auto result = metaf::Parser::parse(
		"METAR KLGA 111951Z 04004KT 2 1/2SM HZ BKN008 OVC015 23/21 A3012"
		" RMK AO2 SFC VIS 4 SLP198 T02330206=");
	ASSERT_EQ(result.reportType, metaf::ReportType::METAR);
	ASSERT_EQ(result.error, metaf::Parser::Error::NONE);

	EXPECT_EQ(result.groups.size(), 15u);

	EXPECT_TRUE(isMetar(result.groups.at(0).group));
	EXPECT_TRUE(isLocation(result.groups.at(1).group)); 
	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_TRUE(isWind(result.groups.at(3).group));
	EXPECT_TRUE(isVisibility(result.groups.at(4).group));
	EXPECT_TRUE(isWeather(result.groups.at(5).group));
	EXPECT_TRUE(isCloud(result.groups.at(6).group));
	EXPECT_TRUE(isCloud(result.groups.at(7).group));
	EXPECT_TRUE(isTemperature(result.groups.at(8).group));
	EXPECT_TRUE(isPressure(result.groups.at(9).group));
	EXPECT_TRUE(isRmk(result.groups.at(10).group));
	EXPECT_TRUE(isFixedGroup(result.groups.at(11).group));
	EXPECT_TRUE(isPlainText(result.groups.at(12).group, "SFC VIS 4"));
	EXPECT_TRUE(isPressure(result.groups.at(13).group));
	EXPECT_TRUE(isTemperature(result.groups.at(14).group));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserResultReportPartAndRawString, metar) {
	const auto result = metaf::Parser::parse(
		"METAR LMML 092045Z 14004KT 9999 FEW020 25/21 Q1020 NOSIG RMK SMOKE");

	EXPECT_EQ(result.groups.size(), 11u);

	EXPECT_TRUE(isMetar(result.groups.at(0).group));
	EXPECT_EQ(result.groups.at(0).rawString, "METAR");
	EXPECT_EQ(result.groups.at(0).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isLocation(result.groups.at(1).group)); 
	EXPECT_EQ(result.groups.at(1).rawString, "LMML");
	EXPECT_EQ(result.groups.at(1).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_EQ(result.groups.at(2).rawString, "092045Z");
	EXPECT_EQ(result.groups.at(2).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isWind(result.groups.at(3).group));
	EXPECT_EQ(result.groups.at(3).rawString, "14004KT");
	EXPECT_EQ(result.groups.at(3).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isVisibility(result.groups.at(4).group));
	EXPECT_EQ(result.groups.at(4).rawString, "9999");
	EXPECT_EQ(result.groups.at(4).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isCloud(result.groups.at(5).group));
	EXPECT_EQ(result.groups.at(5).rawString, "FEW020");
	EXPECT_EQ(result.groups.at(5).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isTemperature(result.groups.at(6).group));
	EXPECT_EQ(result.groups.at(6).rawString, "25/21");
	EXPECT_EQ(result.groups.at(6).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isPressure(result.groups.at(7).group));
	EXPECT_EQ(result.groups.at(7).rawString, "Q1020");
	EXPECT_EQ(result.groups.at(7).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isTrend(result.groups.at(8).group));
	EXPECT_EQ(result.groups.at(8).rawString, "NOSIG");
	EXPECT_EQ(result.groups.at(8).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isRmk(result.groups.at(9).group));
	EXPECT_EQ(result.groups.at(9).rawString, "RMK");
	EXPECT_EQ(result.groups.at(9).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isPlainText(result.groups.at(10).group, "SMOKE"));
	EXPECT_EQ(result.groups.at(10).rawString, "SMOKE");
	EXPECT_EQ(result.groups.at(10).reportPart, metaf::ReportPart::RMK);
}

TEST(ParserResultReportPartAndRawString, taf) {
	const auto result = metaf::Parser::parse(
		"TAF CYVQ 091738Z 0918/1006 VRB03KT P6SM FEW003 SCT120 BKN210"
		" FM092000 25006KT P6SM FEW080 BKN120 BKN210"
		" RMK LAST");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);

	EXPECT_EQ(result.groups.size(), 17u);

	EXPECT_TRUE(isTaf(result.groups.at(0).group));
	EXPECT_EQ(result.groups.at(0).rawString, "TAF");
	EXPECT_EQ(result.groups.at(0).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isLocation(result.groups.at(1).group)); 
	EXPECT_EQ(result.groups.at(1).rawString, "CYVQ");
	EXPECT_EQ(result.groups.at(1).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_EQ(result.groups.at(2).rawString, "091738Z");
	EXPECT_EQ(result.groups.at(2).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isTimeSpan(result.groups.at(3).group));
	EXPECT_EQ(result.groups.at(3).rawString, "0918/1006");
	EXPECT_EQ(result.groups.at(3).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isWind(result.groups.at(4).group));
	EXPECT_EQ(result.groups.at(4).rawString, "VRB03KT");
	EXPECT_EQ(result.groups.at(4).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isVisibility(result.groups.at(5).group));
	EXPECT_EQ(result.groups.at(5).rawString, "P6SM");
	EXPECT_EQ(result.groups.at(5).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(result.groups.at(6).group));
	EXPECT_EQ(result.groups.at(6).rawString, "FEW003");
	EXPECT_EQ(result.groups.at(6).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(result.groups.at(7).group));
	EXPECT_EQ(result.groups.at(7).rawString, "SCT120");
	EXPECT_EQ(result.groups.at(7).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(result.groups.at(8).group));
	EXPECT_EQ(result.groups.at(8).rawString, "BKN210");
	EXPECT_EQ(result.groups.at(8).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isTrend(result.groups.at(9).group));
	EXPECT_EQ(result.groups.at(9).rawString, "FM092000");
	EXPECT_EQ(result.groups.at(9).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isWind(result.groups.at(10).group));
	EXPECT_EQ(result.groups.at(10).rawString, "25006KT");
	EXPECT_EQ(result.groups.at(10).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isVisibility(result.groups.at(11).group));
	EXPECT_EQ(result.groups.at(11).rawString, "P6SM");
	EXPECT_EQ(result.groups.at(11).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(result.groups.at(12).group));
	EXPECT_EQ(result.groups.at(12).rawString, "FEW080");
	EXPECT_EQ(result.groups.at(12).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(result.groups.at(13).group));
	EXPECT_EQ(result.groups.at(13).rawString, "BKN120");
	EXPECT_EQ(result.groups.at(13).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(result.groups.at(14).group));
	EXPECT_EQ(result.groups.at(14).rawString, "BKN210");
	EXPECT_EQ(result.groups.at(14).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isRmk(result.groups.at(15).group));
	EXPECT_EQ(result.groups.at(15).rawString, "RMK");
	EXPECT_EQ(result.groups.at(15).reportPart, metaf::ReportPart::TAF);

	EXPECT_TRUE(isPlainText(result.groups.at(16).group, "LAST"));
	EXPECT_EQ(result.groups.at(16).rawString, "LAST");
	EXPECT_EQ(result.groups.at(16).reportPart, metaf::ReportPart::RMK);
}

TEST(ParserResultReportPartAndRawString, autodetectMetar){
	const auto result = metaf::Parser::parse(
		"CYEV 092000Z 25007KT 15SM FEW030 BKN085 OVC250 08/02 A2999 RMK SC1AC6CI1 SLP162");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 12u);
}

TEST(ParserResultReportPartAndRawString, autodetectTaf) {
	const auto result = metaf::Parser::parse(
		"MYNN 131000Z 1312/1412 13006KT 9999 SCT018CB SCT050");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 7u);
}


TEST(ParserResultReportPartAndRawStringParse, error) {
	const auto result = metaf::Parser::parse(
		"ZZZZ FEW010 BKN022 12/11 Q1002");

	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);

	EXPECT_EQ(result.groups.size(), 2u);

	EXPECT_TRUE(isLocation(result.groups.at(0).group)); 
	EXPECT_EQ(result.groups.at(0).rawString, "ZZZZ");
	EXPECT_EQ(result.groups.at(0).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isPlainText(result.groups.at(1).group, "FEW010"));
	EXPECT_EQ(result.groups.at(1).rawString, "FEW010");
	EXPECT_EQ(result.groups.at(1).reportPart, metaf::ReportPart::HEADER);
}

TEST(ParserResultReportPartAndRawString, appendedGroups) {
	const auto result = metaf::Parser::parse(
		"METAR KLGA 111951Z 04004KT 2 1/2SM HZ BKN008 OVC015 23/21 A3012"
		" RMK AO2 SLP198 T02330206");
	ASSERT_EQ(result.reportType, metaf::ReportType::METAR);
	ASSERT_EQ(result.error, metaf::Parser::Error::NONE);

	EXPECT_EQ(result.groups.size(), 14u);

	EXPECT_TRUE(isMetar(result.groups.at(0).group));
	EXPECT_EQ(result.groups.at(0).rawString, "METAR");
	EXPECT_EQ(result.groups.at(0).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isLocation(result.groups.at(1).group)); 
	EXPECT_EQ(result.groups.at(1).rawString, "KLGA");
	EXPECT_EQ(result.groups.at(1).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_EQ(result.groups.at(2).rawString, "111951Z");
	EXPECT_EQ(result.groups.at(2).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isWind(result.groups.at(3).group));
	EXPECT_EQ(result.groups.at(3).rawString, "04004KT");
	EXPECT_EQ(result.groups.at(3).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isVisibility(result.groups.at(4).group));
	EXPECT_EQ(result.groups.at(4).rawString, "2 1/2SM");
	EXPECT_EQ(result.groups.at(4).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isWeather(result.groups.at(5).group));
	EXPECT_EQ(result.groups.at(5).rawString, "HZ");
	EXPECT_EQ(result.groups.at(5).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isCloud(result.groups.at(6).group));
	EXPECT_EQ(result.groups.at(6).rawString, "BKN008");
	EXPECT_EQ(result.groups.at(6).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isCloud(result.groups.at(7).group));
	EXPECT_EQ(result.groups.at(7).rawString, "OVC015");
	EXPECT_EQ(result.groups.at(7).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isTemperature(result.groups.at(8).group));
	EXPECT_EQ(result.groups.at(8).rawString, "23/21");
	EXPECT_EQ(result.groups.at(8).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isPressure(result.groups.at(9).group));
	EXPECT_EQ(result.groups.at(9).rawString, "A3012");
	EXPECT_EQ(result.groups.at(9).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isRmk(result.groups.at(10).group));
	EXPECT_EQ(result.groups.at(10).rawString, "RMK");
	EXPECT_EQ(result.groups.at(10).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isFixedGroup(result.groups.at(11).group));
	EXPECT_EQ(result.groups.at(11).rawString, "AO2");
	EXPECT_EQ(result.groups.at(11).reportPart, metaf::ReportPart::RMK);

	EXPECT_TRUE(isPressure(result.groups.at(12).group));
	EXPECT_EQ(result.groups.at(12).rawString, "SLP198");
	EXPECT_EQ(result.groups.at(12).reportPart, metaf::ReportPart::RMK);

	EXPECT_TRUE(isTemperature(result.groups.at(13).group));
	EXPECT_EQ(result.groups.at(13).rawString, "T02330206");
	EXPECT_EQ(result.groups.at(13).reportPart, metaf::ReportPart::RMK);
}

TEST(ParserResultReportPartAndRawString, appendedRemarks) {
	const auto result = metaf::Parser::parse(
		"METAR LMML 092045Z 14004KT 9999 FEW020 25/21 Q1020 NOSIG RMK SMOKE TO NE");

	EXPECT_EQ(result.groups.size(), 11u);

	EXPECT_TRUE(isMetar(result.groups.at(0).group));
	EXPECT_EQ(result.groups.at(0).rawString, "METAR");
	EXPECT_EQ(result.groups.at(0).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isLocation(result.groups.at(1).group)); 
	EXPECT_EQ(result.groups.at(1).rawString, "LMML");
	EXPECT_EQ(result.groups.at(1).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isReportTime(result.groups.at(2).group));
	EXPECT_EQ(result.groups.at(2).rawString, "092045Z");
	EXPECT_EQ(result.groups.at(2).reportPart, metaf::ReportPart::HEADER);

	EXPECT_TRUE(isWind(result.groups.at(3).group));
	EXPECT_EQ(result.groups.at(3).rawString, "14004KT");
	EXPECT_EQ(result.groups.at(3).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isVisibility(result.groups.at(4).group));
	EXPECT_EQ(result.groups.at(4).rawString, "9999");
	EXPECT_EQ(result.groups.at(4).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isCloud(result.groups.at(5).group));
	EXPECT_EQ(result.groups.at(5).rawString, "FEW020");
	EXPECT_EQ(result.groups.at(5).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isTemperature(result.groups.at(6).group));
	EXPECT_EQ(result.groups.at(6).rawString, "25/21");
	EXPECT_EQ(result.groups.at(6).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isPressure(result.groups.at(7).group));
	EXPECT_EQ(result.groups.at(7).rawString, "Q1020");
	EXPECT_EQ(result.groups.at(7).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isTrend(result.groups.at(8).group));
	EXPECT_EQ(result.groups.at(8).rawString, "NOSIG");
	EXPECT_EQ(result.groups.at(8).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isRmk(result.groups.at(9).group));
	EXPECT_EQ(result.groups.at(9).rawString, "RMK");
	EXPECT_EQ(result.groups.at(9).reportPart, metaf::ReportPart::METAR);

	EXPECT_TRUE(isPlainText(result.groups.at(10).group, "SMOKE TO NE"));
	EXPECT_EQ(result.groups.at(10).rawString, "SMOKE TO NE");
	EXPECT_EQ(result.groups.at(10).reportPart, metaf::ReportPart::RMK);
}
