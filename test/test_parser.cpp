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
	EXPECT_TRUE(isLocation(result.groups.at(0)));
}

TEST(ParserSyntaxMalformedReports, typeLocation) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result1.groups.size(), 2u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));

	const auto result2 = metaf::Parser::parse("SPECI ZZZZ=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result2.groups.size(), 2u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));

	const auto result3 = metaf::Parser::parse("TAF ZZZZ=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result3.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result3.groups.size(), 2u);
	EXPECT_TRUE(isTaf(result3.groups.at(0)));
	EXPECT_TRUE(isLocation(result3.groups.at(1)));
}

TEST(ParserSyntaxMalformedReports, locationTime) {
	const auto result = metaf::Parser::parse("ZZZZ 041115Z=");
	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result.groups.size(), 2u);
	EXPECT_TRUE(isLocation(result.groups.at(0)));
	EXPECT_TRUE(isReportTime(result.groups.at(1)));
}

TEST(ParserSyntaxMalformedReports, typeLocationTime) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result1.groups.size(), 3u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));

	const auto result2 = metaf::Parser::parse("SPECI ZZZZ 041115Z=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result2.groups.size(), 3u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));

	const auto result3 = metaf::Parser::parse("TAF ZZZZ 041115Z=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result3.error, metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(result3.groups.size(), 3u);
	EXPECT_TRUE(isTaf(result3.groups.at(0)));
	EXPECT_TRUE(isLocation(result3.groups.at(1)));
	EXPECT_TRUE(isReportTime(result3.groups.at(2)));
}

TEST(ParserSyntaxMalformedReports, strayGroupInPlaceOfReportTime) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ ZZZZ=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result1.groups.size(), 3u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isLocation(result1.groups.at(2)));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ ZZZZ=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result2.groups.size(), 3u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isLocation(result2.groups.at(2)));

	const auto result3 = metaf::Parser::parse("ZZZZ ZZZZ=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result3.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result3.groups.size(), 2u);
	EXPECT_TRUE(isLocation(result3.groups.at(0)));
	EXPECT_TRUE(isLocation(result3.groups.at(1)));
}

TEST(ParserSyntaxMalformedReports, strayGroupInPlaceOfTimeSpan) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z ZZZZ=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::EXPECTED_TIME_SPAN);
	EXPECT_EQ(result.groups.size(), 4u);
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isLocation(result.groups.at(3)));
}

TEST(ParserSyntaxMalformedReports, tafReportTypeAndTimeNotSpecified) {
	const auto result = metaf::Parser::parse("ZZZZ 0412/0512 TX07/0416Z TN03/0505Z=");
	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result.groups.size(), 2u);
	EXPECT_TRUE(isLocation(result.groups.at(0)));
	EXPECT_TRUE(isTimeSpan(result.groups.at(1)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxNilReports, locationNil) {
	const auto result = metaf::Parser::parse("ZZZZ NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 2u);
	EXPECT_TRUE(isLocation(result.groups.at(0)));
	EXPECT_TRUE(isNil(result.groups.at(1)));
}

TEST(ParserSyntaxNilReports, typeLocationNil) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ NIL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 3u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isNil(result1.groups.at(2)));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ NIL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 3u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isNil(result2.groups.at(2)));
}

TEST(ParserSyntaxNilReports, corAmdLocationNil) {
	const auto result1 = metaf::Parser::parse("COR ZZZZ NIL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result1.error, metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION);
	EXPECT_EQ(result1.groups.size(), 1u);
	EXPECT_TRUE(isCor(result1.groups.at(0)));

	const auto result2 = metaf::Parser::parse("AMD ZZZZ NIL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION);
	EXPECT_EQ(result2.groups.size(), 1u);
	EXPECT_TRUE(isAmd(result2.groups.at(0)));
}

TEST(ParserSyntaxNilReports, metarCorLocationNil) {
	const auto result = metaf::Parser::parse("METAR COR ZZZZ NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 4u);
	EXPECT_TRUE(isMetar(result.groups.at(0)));
	EXPECT_TRUE(isCor(result.groups.at(1)));
	EXPECT_TRUE(isLocation(result.groups.at(2)));
	EXPECT_TRUE(isNil(result.groups.at(3)));
}

TEST(ParserSyntaxNilReports, tafCorLocationNil) {
	const auto result = metaf::Parser::parse("TAF COR ZZZZ NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 4u);
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isCor(result.groups.at(1)));
	EXPECT_TRUE(isLocation(result.groups.at(2)));
	EXPECT_TRUE(isNil(result.groups.at(3)));
}

TEST(ParserSyntaxNilReports, metarAmdLocationNil) {
	const auto result = metaf::Parser::parse("METAR AMD ZZZZ NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::AMD_ALLOWED_IN_TAF_ONLY);
	EXPECT_EQ(result.groups.size(), 2u);
	EXPECT_TRUE(isMetar(result.groups.at(0)));
	EXPECT_TRUE(isAmd(result.groups.at(1)));
}

TEST(ParserSyntaxNilReports, tafAmdLocationNil) {
	const auto result = metaf::Parser::parse("TAF AMD ZZZZ NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 4u);
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isAmd(result.groups.at(1)));
	EXPECT_TRUE(isLocation(result.groups.at(2)));
	EXPECT_TRUE(isNil(result.groups.at(3)));
}

TEST(ParserSyntaxNilReports, locationTimeNil) {
	const auto result = metaf::Parser::parse("ZZZZ 041115Z NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 3u);
	EXPECT_TRUE(isLocation(result.groups.at(0)));
	EXPECT_TRUE(isReportTime(result.groups.at(1)));
	EXPECT_TRUE(isNil(result.groups.at(2)));
}

TEST(ParserSyntaxNilReports, typeLocationTimeNil) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z NIL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 4u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));
	EXPECT_TRUE(isNil(result1.groups.at(3)));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z NIL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 4u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result2.groups.at(2)));
	EXPECT_TRUE(isNil(result2.groups.at(3)));
}

TEST(ParserSyntaxNilReports, typeCorAmdLocationTimeNil) {
	const auto result1 = metaf::Parser::parse("METAR COR ZZZZ 041115Z NIL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 5u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isCor(result1.groups.at(1)));
	EXPECT_TRUE(isLocation(result1.groups.at(2)));
	EXPECT_TRUE(isReportTime(result1.groups.at(3)));
	EXPECT_TRUE(isNil(result1.groups.at(4)));

	const auto result2 = metaf::Parser::parse("TAF AMD ZZZZ 041115Z NIL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isAmd(result2.groups.at(1)));
	EXPECT_TRUE(isLocation(result2.groups.at(2)));
	EXPECT_TRUE(isReportTime(result2.groups.at(3)));
	EXPECT_TRUE(isNil(result2.groups.at(4)));
}

TEST(ParserSyntaxNilReports, strayGroupAfterNil) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z NIL 9999=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
	EXPECT_EQ(result1.groups.size(), 5u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));
	EXPECT_TRUE(isNil(result1.groups.at(3)));
	EXPECT_TRUE(isPlainText(result1.groups.at(4), "9999"));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z NIL 9999=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
	EXPECT_EQ(result2.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result2.groups.at(2)));
	EXPECT_TRUE(isNil(result2.groups.at(3)));
	EXPECT_TRUE(isPlainText(result2.groups.at(4), "9999"));

	const auto result3 = metaf::Parser::parse("ZZZZ 041115Z NIL 9999=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result3.error, metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
	EXPECT_EQ(result3.groups.size(), 4u);
	EXPECT_TRUE(isLocation(result3.groups.at(0)));
	EXPECT_TRUE(isReportTime(result3.groups.at(1)));
	EXPECT_TRUE(isNil(result3.groups.at(2)));
	EXPECT_TRUE(isPlainText(result3.groups.at(3), "9999"));
}

TEST(ParserSyntaxNilReports, strayNilInReportBody) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 NIL FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(result1.groups.size(), 6u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));
	EXPECT_TRUE(isWind(result1.groups.at(3)));
	EXPECT_TRUE(isVisibility(result1.groups.at(4)));
	EXPECT_TRUE(isNil(result1.groups.at(5)));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT NIL 10SM FEW250=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(result2.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result2.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3)));
	EXPECT_TRUE(isWind(result2.groups.at(4)));
	EXPECT_TRUE(isNil(result2.groups.at(5)));

	const auto result3 = metaf::Parser::parse("ZZZZ 041115Z 24005KT 9999 NIL FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result3.error, metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(result3.groups.size(), 5u);
	EXPECT_TRUE(isLocation(result3.groups.at(0)));
	EXPECT_TRUE(isReportTime(result3.groups.at(1)));
	EXPECT_TRUE(isWind(result3.groups.at(2)));
	EXPECT_TRUE(isVisibility(result3.groups.at(3)));
	EXPECT_TRUE(isNil(result3.groups.at(4)));
}

TEST(ParserSyntaxNilReports, typeLocationTimeTimespanNil) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0415/0424 NIL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3)));
	EXPECT_TRUE(isNil(result.groups.at(4)));
}

TEST(ParserSyntaxNilReports, typeCorAmdLocationTimeTimespanNil) {
	const auto result1 = metaf::Parser::parse("TAF COR ZZZZ 041115Z 0415/0424 NIL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result1.groups.at(0)));
	EXPECT_TRUE(isCor(result1.groups.at(1)));
	EXPECT_TRUE(isLocation(result1.groups.at(2)));
	EXPECT_TRUE(isReportTime(result1.groups.at(3)));
	EXPECT_TRUE(isTimeSpan(result1.groups.at(4)));
	EXPECT_TRUE(isNil(result1.groups.at(5)));

	const auto result2 = metaf::Parser::parse("TAF AMD ZZZZ 041115Z 0415/0424 NIL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isAmd(result2.groups.at(1)));
	EXPECT_TRUE(isLocation(result2.groups.at(2)));
	EXPECT_TRUE(isReportTime(result2.groups.at(3)));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(4)));
	EXPECT_TRUE(isNil(result2.groups.at(5)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxCancelledReports, typeLocationCnl) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ CNL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result1.groups.size(), 3u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isCnl(result1.groups.at(2)));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ CNL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(result2.groups.size(), 3u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isCnl(result2.groups.at(2)));
}

TEST(ParserSyntaxCancelledReports, typeLocationTimeCnl) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z CNL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::CNL_ALLOWED_IN_TAF_ONLY);
	EXPECT_EQ(result1.groups.size(), 4u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));
	EXPECT_TRUE(isCnl(result1.groups.at(3)));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z CNL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::EXPECTED_TIME_SPAN);
	EXPECT_EQ(result2.groups.size(), 4u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result2.groups.at(2)));
	EXPECT_TRUE(isCnl(result2.groups.at(3)));
}

TEST(ParserSyntaxCancelledReports, strayGroupAfterCnl) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0415/0424 CNL 9999=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_CNL);
	EXPECT_EQ(result.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3)));
	EXPECT_TRUE(isCnl(result.groups.at(4)));
	EXPECT_TRUE(isPlainText(result.groups.at(5), "9999"));
}

TEST(ParserSyntaxCancelledReports, strayCnlInReportBody) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT CNL 10SM FEW250=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(result.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3)));
	EXPECT_TRUE(isWind(result.groups.at(4)));
	EXPECT_TRUE(isCnl(result.groups.at(5)));
}

TEST(ParserSyntaxCancelledReports, strayCnlInMetar) {
	const auto result = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 CNL FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(result.groups.size(), 6u);
	EXPECT_TRUE(isMetar(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isWind(result.groups.at(3)));
	EXPECT_TRUE(isVisibility(result.groups.at(4)));
	EXPECT_TRUE(isCnl(result.groups.at(5)));
}

TEST(ParserSyntaxCancelledReports, typeLocationTimeTimespanCnl) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0415/0424 CNL=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3)));
	EXPECT_TRUE(isCnl(result.groups.at(4)));
}

TEST(ParserSyntaxCancelledReports, typeCorAmdLocationTimeTimespanCnl) {
	const auto result1 = metaf::Parser::parse("TAF COR ZZZZ 041115Z 0415/0424 CNL=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result1.groups.at(0)));
	EXPECT_TRUE(isCor(result1.groups.at(1)));
	EXPECT_TRUE(isLocation(result1.groups.at(2)));
	EXPECT_TRUE(isReportTime(result1.groups.at(3)));
	EXPECT_TRUE(isTimeSpan(result1.groups.at(4)));
	EXPECT_TRUE(isCnl(result1.groups.at(5)));

	const auto result2 = metaf::Parser::parse("TAF AMD ZZZZ 041115Z 0415/0424 CNL=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isAmd(result2.groups.at(1)));
	EXPECT_TRUE(isLocation(result2.groups.at(2)));
	EXPECT_TRUE(isReportTime(result2.groups.at(3)));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(4)));
	EXPECT_TRUE(isCnl(result2.groups.at(5)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxValidMetarReports, plainReports) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 9u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));
	EXPECT_TRUE(isWind(result1.groups.at(3)));
	EXPECT_TRUE(isVisibility(result1.groups.at(4)));
	EXPECT_TRUE(isCloud(result1.groups.at(5)));
	EXPECT_TRUE(isTemperature(result1.groups.at(6)));
	EXPECT_TRUE(isPressure(result1.groups.at(7)));
	EXPECT_TRUE(isTrend(result1.groups.at(8)));

	const auto result2 = metaf::Parser::parse("SPECI ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 9u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result2.groups.at(2)));
	EXPECT_TRUE(isWind(result2.groups.at(3)));
	EXPECT_TRUE(isVisibility(result2.groups.at(4)));
	EXPECT_TRUE(isCloud(result2.groups.at(5)));
	EXPECT_TRUE(isTemperature(result2.groups.at(6)));
	EXPECT_TRUE(isPressure(result2.groups.at(7)));
	EXPECT_TRUE(isTrend(result2.groups.at(8)));

	const auto result3 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 5000 RASN 01/01 Q0980=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result3.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result3.groups.size(), 8u);
	EXPECT_TRUE(isMetar(result3.groups.at(0)));
	EXPECT_TRUE(isLocation(result3.groups.at(1)));
	EXPECT_TRUE(isReportTime(result3.groups.at(2)));
	EXPECT_TRUE(isWind(result3.groups.at(3)));
	EXPECT_TRUE(isVisibility(result3.groups.at(4)));
	EXPECT_TRUE(isWeather(result3.groups.at(5)));
	EXPECT_TRUE(isTemperature(result3.groups.at(6)));
	EXPECT_TRUE(isPressure(result3.groups.at(7)));
}

TEST(ParserSyntaxValidMetarReports, correctionalReports) {
	const auto result1 = metaf::Parser::parse("METAR COR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 10u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isCor(result1.groups.at(1)));
	EXPECT_TRUE(isLocation(result1.groups.at(2)));
	EXPECT_TRUE(isReportTime(result1.groups.at(3)));
	EXPECT_TRUE(isWind(result1.groups.at(4)));
	EXPECT_TRUE(isVisibility(result1.groups.at(5)));
	EXPECT_TRUE(isCloud(result1.groups.at(6)));
	EXPECT_TRUE(isTemperature(result1.groups.at(7)));
	EXPECT_TRUE(isPressure(result1.groups.at(8)));
	EXPECT_TRUE(isTrend(result1.groups.at(9)));

	const auto result2 = metaf::Parser::parse("SPECI COR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 10u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0)));
	EXPECT_TRUE(isCor(result2.groups.at(1)));
	EXPECT_TRUE(isLocation(result2.groups.at(2)));
	EXPECT_TRUE(isReportTime(result2.groups.at(3)));
	EXPECT_TRUE(isWind(result2.groups.at(4)));
	EXPECT_TRUE(isVisibility(result2.groups.at(5)));
	EXPECT_TRUE(isCloud(result2.groups.at(6)));
	EXPECT_TRUE(isTemperature(result2.groups.at(7)));
	EXPECT_TRUE(isPressure(result2.groups.at(8)));
	EXPECT_TRUE(isTrend(result2.groups.at(9)));

	const auto result3 = metaf::Parser::parse("METAR ZZZZ 041115Z COR 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result3.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result3.groups.size(), 10u);
	EXPECT_TRUE(isMetar(result3.groups.at(0)));
	EXPECT_TRUE(isLocation(result3.groups.at(1)));
	EXPECT_TRUE(isReportTime(result3.groups.at(2)));
	EXPECT_TRUE(isCor(result3.groups.at(3)));
	EXPECT_TRUE(isWind(result3.groups.at(4)));
	EXPECT_TRUE(isVisibility(result3.groups.at(5)));
	EXPECT_TRUE(isCloud(result3.groups.at(6)));
	EXPECT_TRUE(isTemperature(result3.groups.at(7)));
	EXPECT_TRUE(isPressure(result3.groups.at(8)));
	EXPECT_TRUE(isTrend(result3.groups.at(9)));
}

TEST(ParserSyntaxValidMetarReports, reportsWithRemarks) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 13u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));
	EXPECT_TRUE(isWind(result1.groups.at(3)));
	EXPECT_TRUE(isVisibility(result1.groups.at(4)));
	EXPECT_TRUE(isCloud(result1.groups.at(5)));
	EXPECT_TRUE(isTemperature(result1.groups.at(6)));
	EXPECT_TRUE(isPressure(result1.groups.at(7)));
	EXPECT_TRUE(isTrend(result1.groups.at(8)));
	EXPECT_TRUE(isRmk(result1.groups.at(9)));
	EXPECT_TRUE(isPlainText(result1.groups.at(10), "TEST"));
	EXPECT_TRUE(isPlainText(result1.groups.at(11), "9999"));
	EXPECT_TRUE(isPlainText(result1.groups.at(12), "RMK"));

	const auto result2 = metaf::Parser::parse("SPECI ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 13u);
	EXPECT_TRUE(isSpeci(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result2.groups.at(2)));
	EXPECT_TRUE(isWind(result2.groups.at(3)));
	EXPECT_TRUE(isVisibility(result2.groups.at(4)));
	EXPECT_TRUE(isCloud(result2.groups.at(5)));
	EXPECT_TRUE(isTemperature(result2.groups.at(6)));
	EXPECT_TRUE(isPressure(result2.groups.at(7)));
	EXPECT_TRUE(isTrend(result2.groups.at(8)));
	EXPECT_TRUE(isRmk(result2.groups.at(9)));
	EXPECT_TRUE(isPlainText(result2.groups.at(10), "TEST"));
	EXPECT_TRUE(isPlainText(result2.groups.at(11), "9999"));
	EXPECT_TRUE(isPlainText(result2.groups.at(12), "RMK"));
}

TEST(ParserSyntaxValidMetarReports, reportTypeNotSpecified) {
	const auto result1 = metaf::Parser::parse("ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 8u);
	EXPECT_TRUE(isLocation(result1.groups.at(0)));
	EXPECT_TRUE(isReportTime(result1.groups.at(1)));
	EXPECT_TRUE(isWind(result1.groups.at(2)));
	EXPECT_TRUE(isVisibility(result1.groups.at(3)));
	EXPECT_TRUE(isCloud(result1.groups.at(4)));
	EXPECT_TRUE(isTemperature(result1.groups.at(5)));
	EXPECT_TRUE(isPressure(result1.groups.at(6)));
	EXPECT_TRUE(isTrend(result1.groups.at(7)));

	const auto result2 = metaf::Parser::parse("ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 12u);
	EXPECT_TRUE(isLocation(result2.groups.at(0)));
	EXPECT_TRUE(isReportTime(result2.groups.at(1)));
	EXPECT_TRUE(isWind(result2.groups.at(2)));
	EXPECT_TRUE(isVisibility(result2.groups.at(3)));
	EXPECT_TRUE(isCloud(result2.groups.at(4)));
	EXPECT_TRUE(isTemperature(result2.groups.at(5)));
	EXPECT_TRUE(isPressure(result2.groups.at(6)));
	EXPECT_TRUE(isTrend(result2.groups.at(7)));
	EXPECT_TRUE(isRmk(result2.groups.at(8)));
	EXPECT_TRUE(isPlainText(result2.groups.at(9), "TEST"));
	EXPECT_TRUE(isPlainText(result2.groups.at(10), "9999"));
	EXPECT_TRUE(isPlainText(result2.groups.at(11), "RMK"));

	const auto result3 = metaf::Parser::parse("ZZZZ 041115Z RMK TEST=");
	EXPECT_EQ(result3.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result3.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result3.groups.size(), 4u);
	EXPECT_TRUE(isLocation(result3.groups.at(0)));
	EXPECT_TRUE(isReportTime(result3.groups.at(1)));
	EXPECT_TRUE(isRmk(result3.groups.at(2)));
	EXPECT_TRUE(isPlainText(result3.groups.at(3), "TEST"));
}

TEST(ParserSyntaxValidMetarReports, reportWithTrend) {
	const auto result = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 "
		"TEMPO 3500 BKN010 +RA=");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 12u);
	EXPECT_TRUE(isMetar(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isWind(result.groups.at(3)));
	EXPECT_TRUE(isVisibility(result.groups.at(4)));
	EXPECT_TRUE(isCloud(result.groups.at(5)));
	EXPECT_TRUE(isTemperature(result.groups.at(6)));
	EXPECT_TRUE(isPressure(result.groups.at(7)));
	EXPECT_TRUE(isTrend(result.groups.at(8)));
	EXPECT_TRUE(isVisibility(result.groups.at(9)));
	EXPECT_TRUE(isCloud(result.groups.at(10)));
	EXPECT_TRUE(isWeather(result.groups.at(11)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxValidTafReports, plainReport) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 7u);
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3)));
	EXPECT_TRUE(isWind(result.groups.at(4)));
	EXPECT_TRUE(isVisibility(result.groups.at(5)));
	EXPECT_TRUE(isCloud(result.groups.at(6)));
}

TEST(ParserSyntaxValidTafReports, amendedAndCorrectionalReports) {
	const auto result1 = metaf::Parser::parse("TAF AMD ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 8u);
	EXPECT_TRUE(isTaf(result1.groups.at(0)));
	EXPECT_TRUE(isAmd(result1.groups.at(1)));
	EXPECT_TRUE(isLocation(result1.groups.at(2)));
	EXPECT_TRUE(isReportTime(result1.groups.at(3)));
	EXPECT_TRUE(isTimeSpan(result1.groups.at(4)));
	EXPECT_TRUE(isWind(result1.groups.at(5)));
	EXPECT_TRUE(isVisibility(result1.groups.at(6)));
	EXPECT_TRUE(isCloud(result1.groups.at(7)));

	const auto result2 = metaf::Parser::parse("TAF COR ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 8u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isCor(result2.groups.at(1)));
	EXPECT_TRUE(isLocation(result2.groups.at(2)));
	EXPECT_TRUE(isReportTime(result2.groups.at(3)));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(4)));
	EXPECT_TRUE(isWind(result2.groups.at(5)));
	EXPECT_TRUE(isVisibility(result2.groups.at(6)));
	EXPECT_TRUE(isCloud(result2.groups.at(7)));
}

TEST(ParserSyntaxValidTafReports, reportsWithTemperatureForecasts) {
	const auto result1 = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 TX07/0416Z=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result1.groups.at(3)));
	EXPECT_TRUE(isTempForecast(result1.groups.at(4)));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 TX07/0416Z TN03/0505Z=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 6u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result2.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3)));
	EXPECT_TRUE(isTempForecast(result2.groups.at(4)));
	EXPECT_TRUE(isTempForecast(result2.groups.at(5)));
}

TEST(ParserSyntaxValidTafReports, reportWithRemarks) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250 RMK TEST 9999 RMK=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 11u);
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3)));
	EXPECT_TRUE(isWind(result.groups.at(4)));
	EXPECT_TRUE(isVisibility(result.groups.at(5)));
	EXPECT_TRUE(isCloud(result.groups.at(6)));
	EXPECT_TRUE(isRmk(result.groups.at(7)));
	EXPECT_TRUE(isPlainText(result.groups.at(8), "TEST"));
	EXPECT_TRUE(isPlainText(result.groups.at(9), "9999"));
	EXPECT_TRUE(isPlainText(result.groups.at(10), "RMK"));
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
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result.groups.at(3)));
	EXPECT_TRUE(isWind(result.groups.at(4)));
	EXPECT_TRUE(isVisibility(result.groups.at(5)));
	EXPECT_TRUE(isCloud(result.groups.at(6)));
	EXPECT_TRUE(isTrend(result.groups.at(7)));
	EXPECT_TRUE(isWind(result.groups.at(8)));
	EXPECT_TRUE(isTrend(result.groups.at(9)));
	EXPECT_TRUE(isCloud(result.groups.at(10)));
	EXPECT_TRUE(isVisibility(result.groups.at(11)));
	EXPECT_TRUE(isWeather(result.groups.at(12)));
	EXPECT_TRUE(isTrend(result.groups.at(13)));
	EXPECT_TRUE(isCloud(result.groups.at(14)));
}

TEST(ParserSyntaxValidTafReports, reportTypeNotSpecified) {
	const auto result1 = metaf::Parser::parse("ZZZZ 041115Z 0412/0512 TX07/0416Z=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 4u);
	EXPECT_TRUE(isLocation(result1.groups.at(0)));
	EXPECT_TRUE(isReportTime(result1.groups.at(1)));
	EXPECT_TRUE(isTimeSpan(result1.groups.at(2)));
	EXPECT_TRUE(isTempForecast(result1.groups.at(3)));

	const auto result2 = metaf::Parser::parse("ZZZZ 041115Z 0412/0512 RMK TEST=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 5u);
	EXPECT_TRUE(isLocation(result2.groups.at(0)));
	EXPECT_TRUE(isReportTime(result2.groups.at(1)));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(2)));
	EXPECT_TRUE(isRmk(result2.groups.at(3)));
	EXPECT_TRUE(isPlainText(result2.groups.at(4), "TEST"));
}

TEST(ParserSyntaxValidTafReports, reportTimeNotSpecified) {
	const auto result = metaf::Parser::parse("TAF ZZZZ 0412/0512 TX07/0416Z TN03/0505Z=");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.groups.size(), 5u);
	EXPECT_TRUE(isTaf(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1)));
	EXPECT_TRUE(isTimeSpan(result.groups.at(2)));
	EXPECT_TRUE(isTempForecast(result.groups.at(3)));
	EXPECT_TRUE(isTempForecast(result.groups.at(4)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxDesignatorsAndSeparators, noReportEndDesignator) {
	const auto result1 = metaf::Parser::parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 9u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));
	EXPECT_TRUE(isWind(result1.groups.at(3)));
	EXPECT_TRUE(isVisibility(result1.groups.at(4)));
	EXPECT_TRUE(isCloud(result1.groups.at(5)));
	EXPECT_TRUE(isTemperature(result1.groups.at(6)));
	EXPECT_TRUE(isPressure(result1.groups.at(7)));
	EXPECT_TRUE(isTrend(result1.groups.at(8)));

	const auto result2 = metaf::Parser::parse("TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 7u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result2.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3)));
	EXPECT_TRUE(isWind(result2.groups.at(4)));
	EXPECT_TRUE(isVisibility(result2.groups.at(5)));
	EXPECT_TRUE(isCloud(result2.groups.at(6)));
}

TEST(ParserSyntaxDesignatorsAndSeparators, variousGroupSeparators) {
	const auto result1 = metaf::Parser::parse(
		"METAR  ZZZZ    041115Z \r\n 24005KT\r9999\nFEW040\r\n25/18 \nQ1011\r NOSIG=");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 9u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));
	EXPECT_TRUE(isWind(result1.groups.at(3)));
	EXPECT_TRUE(isVisibility(result1.groups.at(4)));
	EXPECT_TRUE(isCloud(result1.groups.at(5)));
	EXPECT_TRUE(isTemperature(result1.groups.at(6)));
	EXPECT_TRUE(isPressure(result1.groups.at(7)));
	EXPECT_TRUE(isTrend(result1.groups.at(8)));

	const auto result2 = metaf::Parser::parse(
		"TAF  ZZZZ      041115Z \r\n 0412/0512\r24005KT\n10SM\r\nFEW250=");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 7u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result2.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3)));
	EXPECT_TRUE(isWind(result2.groups.at(4)));
	EXPECT_TRUE(isVisibility(result2.groups.at(5)));
	EXPECT_TRUE(isCloud(result2.groups.at(6)));
}

TEST(ParserSyntaxDesignatorsAndSeparators, strayTextAfterReportEndDesignator) {
	const auto result1 = metaf::Parser::parse(
		"METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG= STRAY TEXT 1000");
	EXPECT_EQ(result1.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result1.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result1.groups.size(), 9u);
	EXPECT_TRUE(isMetar(result1.groups.at(0)));
	EXPECT_TRUE(isLocation(result1.groups.at(1)));
	EXPECT_TRUE(isReportTime(result1.groups.at(2)));
	EXPECT_TRUE(isWind(result1.groups.at(3)));
	EXPECT_TRUE(isVisibility(result1.groups.at(4)));
	EXPECT_TRUE(isCloud(result1.groups.at(5)));
	EXPECT_TRUE(isTemperature(result1.groups.at(6)));
	EXPECT_TRUE(isPressure(result1.groups.at(7)));
	EXPECT_TRUE(isTrend(result1.groups.at(8)));

	const auto result2 = metaf::Parser::parse(
		"TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250= STRAY TEXT 1000");
	EXPECT_EQ(result2.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result2.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result2.groups.size(), 7u);
	EXPECT_TRUE(isTaf(result2.groups.at(0)));
	EXPECT_TRUE(isLocation(result2.groups.at(1)));
	EXPECT_TRUE(isReportTime(result2.groups.at(2)));
	EXPECT_TRUE(isTimeSpan(result2.groups.at(3)));
	EXPECT_TRUE(isWind(result2.groups.at(4)));
	EXPECT_TRUE(isVisibility(result2.groups.at(5)));
	EXPECT_TRUE(isCloud(result2.groups.at(6)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfMetar) {
	const auto result = metaf::Parser::parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO $");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back()));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfMetarRemark) {
	const auto result = metaf::Parser::parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO $");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back()));
}

TEST(ParserSyntaxMaintenanceIndicator, 
	maintenanceIndicatorAtTheEndOfMetarRemarkWithReportEndDesignator)
{
	const auto result = metaf::Parser::parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO $=");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back()));
}


TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfTaf) {
	const auto result = metaf::Parser::parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 FEW020TCU BKN035 $");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back()));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfTafRemark) {
	const auto result = metaf::Parser::parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 FEW020TCU BKN035 RMK TEST $");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back()));
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
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back()));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorInTheMiddleOfTafRemark) {
	const auto result = metaf::Parser::parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 FEW020TCU BKN035 RMK TEST $ TEST");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(result.groups.back()));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAfterNil) {
	const auto result = metaf::Parser::parse("METAR ZZZZ 041115Z NIL $");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserCombined, combinedGroups) {
	const auto result = metaf::Parser::parse(
		"METAR KLGA 111951Z 04004KT 2 1/2SM HZ BKN008 OVC015 23/21 A3012"
		" RMK AO2 SFC VIS 4 SLP198 T02330206=");
	ASSERT_EQ(result.reportType, metaf::ReportType::METAR);
	ASSERT_EQ(result.error, metaf::Parser::Error::NONE);

	EXPECT_EQ(result.groups.size(), 17u);

	EXPECT_TRUE(isMetar(result.groups.at(0)));
	EXPECT_TRUE(isLocation(result.groups.at(1))); 
	EXPECT_TRUE(isReportTime(result.groups.at(2)));
	EXPECT_TRUE(isWind(result.groups.at(3)));
	EXPECT_TRUE(isVisibility(result.groups.at(4)));
	EXPECT_TRUE(isWeather(result.groups.at(5)));
	EXPECT_TRUE(isCloud(result.groups.at(6)));
	EXPECT_TRUE(isCloud(result.groups.at(7)));
	EXPECT_TRUE(isTemperature(result.groups.at(8)));
	EXPECT_TRUE(isPressure(result.groups.at(9)));
	EXPECT_TRUE(isRmk(result.groups.at(10)));
	EXPECT_TRUE(isFixedGroup(result.groups.at(11)));
	EXPECT_TRUE(isPlainText(result.groups.at(12), "SFC"));
	EXPECT_TRUE(isPlainText(result.groups.at(13), "VIS"));
	EXPECT_TRUE(isPlainText(result.groups.at(14), "4"));
	EXPECT_TRUE(isPlainText(result.groups.at(15), "SLP198"));
	EXPECT_TRUE(isPlainText(result.groups.at(16), "T02330206"));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserExtendedParse, metar) {
	const auto result = metaf::Parser::extendedParse(
		"METAR LMML 092045Z 14004KT 9999 FEW020 25/21 Q1020 NOSIG RMK SMOKE TO NE");

	EXPECT_EQ(result.extgroups.size(), 13u);

	EXPECT_TRUE(isMetar(std::get<metaf::Group>(result.extgroups.at(0))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(0)), "METAR");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(0)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isLocation(std::get<metaf::Group>(result.extgroups.at(1)))); 
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(1)), "LMML");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(1)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isReportTime(std::get<metaf::Group>(result.extgroups.at(2))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(2)), "092045Z");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(2)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isWind(std::get<metaf::Group>(result.extgroups.at(3))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(3)), "14004KT");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(3)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isVisibility(std::get<metaf::Group>(result.extgroups.at(4))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(4)), "9999");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(4)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isCloud(std::get<metaf::Group>(result.extgroups.at(5))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(5)), "FEW020");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(5)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isTemperature(std::get<metaf::Group>(result.extgroups.at(6))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(6)), "25/21");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(6)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isPressure(std::get<metaf::Group>(result.extgroups.at(7))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(7)), "Q1020");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(7)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isTrend(std::get<metaf::Group>(result.extgroups.at(8))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(8)), "NOSIG");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(8)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isRmk(std::get<metaf::Group>(result.extgroups.at(9))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(9)), "RMK");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(9)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(10)), "SMOKE"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(10)), "SMOKE");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(10)), 
		metaf::ReportPart::RMK);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(11)), "TO"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(11)), "TO");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(11)), 
		metaf::ReportPart::RMK);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(12)), "NE"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(12)), "NE");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(12)), 
		metaf::ReportPart::RMK);
}

TEST(ParserExtendedParse, taf) {
	const auto result = metaf::Parser::extendedParse(
		"TAF CYVQ 091738Z 0918/1006 VRB03KT P6SM FEW003 SCT120 BKN210"
		" FM092000 25006KT P6SM FEW080 BKN120 BKN210"
		" RMK NXT FCST BY 100000Z");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);

	EXPECT_EQ(result.extgroups.size(), 20u);

	EXPECT_TRUE(isTaf(std::get<metaf::Group>(result.extgroups.at(0))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(0)), "TAF");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(0)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isLocation(std::get<metaf::Group>(result.extgroups.at(1)))); 
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(1)), "CYVQ");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(1)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isReportTime(std::get<metaf::Group>(result.extgroups.at(2))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(2)), "091738Z");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(2)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isTimeSpan(std::get<metaf::Group>(result.extgroups.at(3))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(3)), "0918/1006");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(3)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isWind(std::get<metaf::Group>(result.extgroups.at(4))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(4)), "VRB03KT");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(4)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isVisibility(std::get<metaf::Group>(result.extgroups.at(5))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(5)), "P6SM");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(5)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(std::get<metaf::Group>(result.extgroups.at(6))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(6)), "FEW003");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(6)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(std::get<metaf::Group>(result.extgroups.at(7))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(7)), "SCT120");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(7)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(std::get<metaf::Group>(result.extgroups.at(8))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(8)), "BKN210");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(8)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isTrend(std::get<metaf::Group>(result.extgroups.at(9))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(9)), "FM092000");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(9)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isWind(std::get<metaf::Group>(result.extgroups.at(10))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(10)), "25006KT");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(10)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isVisibility(std::get<metaf::Group>(result.extgroups.at(11))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(11)), "P6SM");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(11)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(std::get<metaf::Group>(result.extgroups.at(12))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(12)), "FEW080");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(12)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(std::get<metaf::Group>(result.extgroups.at(13))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(13)), "BKN120");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(13)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isCloud(std::get<metaf::Group>(result.extgroups.at(14))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(14)), "BKN210");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(14)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isRmk(std::get<metaf::Group>(result.extgroups.at(15))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(15)), "RMK");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(15)), 
		metaf::ReportPart::TAF);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(16)), "NXT"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(16)), "NXT");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(16)), 
		metaf::ReportPart::RMK);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(17)), "FCST"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(17)), "FCST");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(17)), 
		metaf::ReportPart::RMK);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(18)), "BY"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(18)), "BY");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(18)), 
		metaf::ReportPart::RMK);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(19)), "100000Z"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(19)), "100000Z");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(19)), 
		metaf::ReportPart::RMK);
}

TEST(ParserExtendedParse, autodetectMetar){
	const auto result = metaf::Parser::extendedParse(
		"CYEV 092000Z 25007KT 15SM FEW030 BKN085 OVC250 08/02 A2999 RMK SC1AC6CI1 SLP162");
	EXPECT_EQ(result.reportType, metaf::ReportType::METAR);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.extgroups.size(), 12u);
}

TEST(ParserExtendedParse, autodetectTaf) {
	const auto result = metaf::Parser::extendedParse(
		"MYNN 131000Z 1312/1412 13006KT 9999 SCT018CB SCT050");
	EXPECT_EQ(result.reportType, metaf::ReportType::TAF);
	EXPECT_EQ(result.error, metaf::Parser::Error::NONE);
	EXPECT_EQ(result.extgroups.size(), 7u);
}


TEST(ParserExtendedParse, error) {
	const auto result = metaf::Parser::extendedParse(
		"ZZZZ FEW010 BKN022 12/11 Q1002");

	EXPECT_EQ(result.reportType, metaf::ReportType::UNKNOWN);
	EXPECT_EQ(result.error, metaf::Parser::Error::EXPECTED_REPORT_TIME);

	EXPECT_EQ(result.extgroups.size(), 2u);

	EXPECT_TRUE(isLocation(std::get<metaf::Group>(result.extgroups.at(0)))); 
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(0)), "ZZZZ");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(0)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(1)), "FEW010"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(1)), "FEW010");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(1)), 
		metaf::ReportPart::HEADER);

}

TEST(ParserExtendedParse, combinedGroups) {
	const auto result = metaf::Parser::extendedParse(
		"METAR KLGA 111951Z 04004KT 2 1/2SM HZ BKN008 OVC015 23/21 A3012"
		" RMK AO2 SFC VIS 4 SLP198 T02330206");
	ASSERT_EQ(result.reportType, metaf::ReportType::METAR);
	ASSERT_EQ(result.error, metaf::Parser::Error::NONE);

	EXPECT_EQ(result.extgroups.size(), 17u);

	EXPECT_TRUE(isMetar(std::get<metaf::Group>(result.extgroups.at(0))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(0)), "METAR");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(0)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isLocation(std::get<metaf::Group>(result.extgroups.at(1)))); 
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(1)), "KLGA");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(1)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isReportTime(std::get<metaf::Group>(result.extgroups.at(2))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(2)), "111951Z");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(2)), 
		metaf::ReportPart::HEADER);

	EXPECT_TRUE(isWind(std::get<metaf::Group>(result.extgroups.at(3))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(3)), "04004KT");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(3)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isVisibility(std::get<metaf::Group>(result.extgroups.at(4))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(4)), "2 1/2SM");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(4)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isWeather(std::get<metaf::Group>(result.extgroups.at(5))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(5)), "HZ");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(5)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isCloud(std::get<metaf::Group>(result.extgroups.at(6))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(6)), "BKN008");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(6)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isCloud(std::get<metaf::Group>(result.extgroups.at(7))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(7)), "OVC015");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(7)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isTemperature(std::get<metaf::Group>(result.extgroups.at(8))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(8)), "23/21");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(8)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isPressure(std::get<metaf::Group>(result.extgroups.at(9))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(9)), "A3012");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(9)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isRmk(std::get<metaf::Group>(result.extgroups.at(10))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(10)), "RMK");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(10)), 
		metaf::ReportPart::METAR);

	EXPECT_TRUE(isFixedGroup(std::get<metaf::Group>(result.extgroups.at(11))));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(11)), "AO2");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(11)), 
		metaf::ReportPart::RMK);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(12)), "SFC"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(12)), "SFC");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(12)), 
		metaf::ReportPart::RMK);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(13)), "VIS"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(13)), "VIS");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(13)), 
		metaf::ReportPart::RMK);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(14)), "4"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(14)), "4");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(14)), 
		metaf::ReportPart::RMK);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(15)), "SLP198"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(15)), "SLP198");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(15)), 
		metaf::ReportPart::RMK);

	EXPECT_TRUE(isPlainText(std::get<metaf::Group>(result.extgroups.at(16)), "T02330206"));
	EXPECT_EQ(std::get<std::string>(result.extgroups.at(16)), "T02330206");
	EXPECT_EQ(std::get<metaf::ReportPart>(result.extgroups.at(16)), 
		metaf::ReportPart::RMK);
}
