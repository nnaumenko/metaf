/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "testdata_real.h"


TEST(Parser, RealDataParsingMETAR) {
	metaf::Parser parser;
	for (const auto & data : testdata::realDataSet) {
		if (strlen(data.metar)) {
			bool parseResult;
			EXPECT_NO_THROW({ parseResult = parser.parse(data.metar); });
			EXPECT_TRUE(parseResult) <<
				"Report: " << data.metar;
			EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR) << 
				"Report: " << data.metar;
		}
	}
}

TEST(Parser, RealDataParsingTAF) {
	metaf::Parser parser;
	for (const auto & data : testdata::realDataSet) {
		if (strlen(data.taf)) {
			bool parseResult;
			EXPECT_NO_THROW({ parseResult = parser.parse(data.taf); });
			EXPECT_TRUE(parseResult) <<
				"Report: " << data.taf;
			EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF) << 
				"Report: " << data.taf;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(Parser, resetResult) {
	metaf::Parser parser;
	ASSERT_TRUE(parser.parse("METAR EGYP 092050Z 06007KT CAVOK 08/02 Q1024 BLU", true));
	ASSERT_NE(parser.getReportType(), metaf::ReportType::UNKNOWN);
	ASSERT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	ASSERT_NE(parser.getResult().size(), 0u);
	ASSERT_NE(parser.getSourceGroups().size(), 0u);
	parser.resetResult();
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 0u);
	EXPECT_EQ(parser.getResult().capacity(), 0u); //Can be implementation-defined?
	EXPECT_EQ(parser.getSourceGroups().size(), 0u);
	EXPECT_EQ(parser.getSourceGroups().capacity(), 0u); //Can be implementation-defined?
}

TEST(Parser, resetResultError) {
	metaf::Parser parser;
	ASSERT_FALSE(parser.parse("METAR EGYP", true));
	ASSERT_NE(parser.getReportType(), metaf::ReportType::UNKNOWN);
	ASSERT_NE(parser.getError(), metaf::Parser::Error::NONE);
	ASSERT_NE(parser.getResult().size(), 0u);
	ASSERT_NE(parser.getSourceGroups().size(), 0u);
	parser.resetResult();
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 0u);
	EXPECT_EQ(parser.getResult().capacity(), 0u); //Can be implementation-defined?
	EXPECT_EQ(parser.getSourceGroups().size(), 0u);
	EXPECT_EQ(parser.getSourceGroups().capacity(), 0u); //Can be implementation-defined?
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
	metaf::Parser parser1;
	EXPECT_FALSE(parser1.parse(""));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::EMPTY_REPORT);
	EXPECT_EQ(parser1.getResult().size(), 0u);

	metaf::Parser parser2;
	EXPECT_FALSE(parser2.parse("="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::EMPTY_REPORT);
	EXPECT_EQ(parser2.getResult().size(), 0u);
}

TEST(ParserSyntaxMalformedReports, location) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse("ZZZZ="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(parser.getResult().size(), 1u);
	EXPECT_TRUE(isLocation(parser.getResult().at(0)));
}

TEST(ParserSyntaxMalformedReports, typeLocation) {
	metaf::Parser parser1;

	EXPECT_FALSE(parser1.parse("METAR ZZZZ="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(parser1.getResult().size(), 2u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));

	metaf::Parser parser2;
	EXPECT_FALSE(parser2.parse("SPECI ZZZZ="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(parser2.getResult().size(), 2u);
	EXPECT_TRUE(isSpeci(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));

	metaf::Parser parser3;
	EXPECT_FALSE(parser3.parse("TAF ZZZZ="));
	EXPECT_EQ(parser3.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser3.getError(), metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(parser3.getResult().size(), 2u);
	EXPECT_TRUE(isTaf(parser3.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser3.getResult().at(1)));
}

TEST(ParserSyntaxMalformedReports, locationTime) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse("ZZZZ 041115Z="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(parser.getResult().size(), 2u);
	EXPECT_TRUE(isLocation(parser.getResult().at(0)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(1)));
}

TEST(ParserSyntaxMalformedReports, typeLocationTime) {
	metaf::Parser parser1;

	EXPECT_FALSE(parser1.parse("METAR ZZZZ 041115Z="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(parser1.getResult().size(), 3u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));

	metaf::Parser parser2;
	EXPECT_FALSE(parser2.parse("SPECI ZZZZ 041115Z="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(parser2.getResult().size(), 3u);
	EXPECT_TRUE(isSpeci(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));

	metaf::Parser parser3;
	EXPECT_FALSE(parser3.parse("TAF ZZZZ 041115Z="));
	EXPECT_EQ(parser3.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser3.getError(), metaf::Parser::Error::UNEXPECTED_REPORT_END);
	EXPECT_EQ(parser3.getResult().size(), 3u);
	EXPECT_TRUE(isTaf(parser3.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser3.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser3.getResult().at(2)));
}

TEST(ParserSyntaxMalformedReports, strayGroupInPlaceOfReportTime) {
	metaf::Parser parser1;

	EXPECT_FALSE(parser1.parse("METAR ZZZZ ZZZZ="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(parser1.getResult().size(), 3u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(2)));

	metaf::Parser parser2;
	EXPECT_FALSE(parser2.parse("TAF ZZZZ ZZZZ="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(parser2.getResult().size(), 3u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(2)));

	metaf::Parser parser3;
	EXPECT_FALSE(parser3.parse("ZZZZ ZZZZ="));
	EXPECT_EQ(parser3.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser3.getError(), metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(parser3.getResult().size(), 2u);
	EXPECT_TRUE(isLocation(parser3.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser3.getResult().at(1)));
}

TEST(ParserSyntaxMalformedReports, strayGroupInPlaceOfTimeSpan) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse("TAF ZZZZ 041115Z ZZZZ="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::EXPECTED_TIME_SPAN);
	EXPECT_EQ(parser.getResult().size(), 4u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_TRUE(isLocation(parser.getResult().at(3)));
}

TEST(ParserSyntaxMalformedReports, tafReportTypeAndTimeNotSpecified) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse("ZZZZ 0412/0512 TX07/0416Z TN03/0505Z="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(parser.getResult().size(), 2u);
	EXPECT_TRUE(isLocation(parser.getResult().at(0)));
	EXPECT_TRUE(isTimeSpan(parser.getResult().at(1)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxNilReports, locationNil) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("ZZZZ NIL="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 2u);
	EXPECT_TRUE(isLocation(parser.getResult().at(0)));
	EXPECT_TRUE(isNil(parser.getResult().at(1)));
}

TEST(ParserSyntaxNilReports, typeLocationNil) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("METAR ZZZZ NIL="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 3u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isNil(parser1.getResult().at(2)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("TAF ZZZZ NIL="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 3u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isNil(parser2.getResult().at(2)));
}

TEST(ParserSyntaxNilReports, corAmdLocationNil) {
	metaf::Parser parser1;
	EXPECT_FALSE(parser1.parse("COR ZZZZ NIL="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION);
	EXPECT_EQ(parser1.getResult().size(), 1u);
	EXPECT_TRUE(isCor(parser1.getResult().at(0)));

	metaf::Parser parser2;
	EXPECT_FALSE(parser2.parse("AMD ZZZZ NIL="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION);
	EXPECT_EQ(parser2.getResult().size(), 1u);
	EXPECT_TRUE(isAmd(parser2.getResult().at(0)));
}

TEST(ParserSyntaxNilReports, metarCorLocationNil) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("METAR COR ZZZZ NIL="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 4u);
	EXPECT_TRUE(isMetar(parser.getResult().at(0)));
	EXPECT_TRUE(isCor(parser.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser.getResult().at(2)));
	EXPECT_TRUE(isNil(parser.getResult().at(3)));
}

TEST(ParserSyntaxNilReports, tafCorLocationNil) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("TAF COR ZZZZ NIL="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 4u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isCor(parser.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser.getResult().at(2)));
	EXPECT_TRUE(isNil(parser.getResult().at(3)));
}

TEST(ParserSyntaxNilReports, metarAmdLocationNil) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse("METAR AMD ZZZZ NIL="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::AMD_ALLOWED_IN_TAF_ONLY);
	EXPECT_EQ(parser.getResult().size(), 2u);
	EXPECT_TRUE(isMetar(parser.getResult().at(0)));
	EXPECT_TRUE(isAmd(parser.getResult().at(1)));
}

TEST(ParserSyntaxNilReports, tafAmdLocationNil) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("TAF AMD ZZZZ NIL="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 4u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isAmd(parser.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser.getResult().at(2)));
	EXPECT_TRUE(isNil(parser.getResult().at(3)));
}

TEST(ParserSyntaxNilReports, locationTimeNil) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("ZZZZ 041115Z NIL="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 3u);
	EXPECT_TRUE(isLocation(parser.getResult().at(0)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(1)));
	EXPECT_TRUE(isNil(parser.getResult().at(2)));
}

TEST(ParserSyntaxNilReports, typeLocationTimeNil) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("METAR ZZZZ 041115Z NIL="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 4u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));
	EXPECT_TRUE(isNil(parser1.getResult().at(3)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("TAF ZZZZ 041115Z NIL="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 4u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(2)));
	EXPECT_TRUE(isNil(parser2.getResult().at(3)));
}

TEST(ParserSyntaxNilReports, typeCorAmdLocationTimeNil) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("METAR COR ZZZZ 041115Z NIL="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 5u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isCor(parser1.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(2)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(3)));
	EXPECT_TRUE(isNil(parser1.getResult().at(4)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("TAF AMD ZZZZ 041115Z NIL="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 5u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isAmd(parser2.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(2)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(3)));
	EXPECT_TRUE(isNil(parser2.getResult().at(4)));
}

TEST(ParserSyntaxNilReports, strayGroupAfterNil) {
	metaf::Parser parser1;
	EXPECT_FALSE(parser1.parse("METAR ZZZZ 041115Z NIL 9999="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
	EXPECT_EQ(parser1.getResult().size(), 5u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));
	EXPECT_TRUE(isNil(parser1.getResult().at(3)));
	EXPECT_TRUE(isPlainText(parser1.getResult().at(4), "9999"));

	metaf::Parser parser2;
	EXPECT_FALSE(parser2.parse("TAF ZZZZ 041115Z NIL 9999="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
	EXPECT_EQ(parser2.getResult().size(), 5u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(2)));
	EXPECT_TRUE(isNil(parser2.getResult().at(3)));
	EXPECT_TRUE(isPlainText(parser2.getResult().at(4), "9999"));

	metaf::Parser parser3;
	EXPECT_FALSE(parser3.parse("ZZZZ 041115Z NIL 9999="));
	EXPECT_EQ(parser3.getReportType(), metaf::ReportType::UNKNOWN);
	EXPECT_EQ(parser3.getError(), metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
	EXPECT_EQ(parser3.getResult().size(), 4u);
	EXPECT_TRUE(isLocation(parser3.getResult().at(0)));
	EXPECT_TRUE(isReportTime(parser3.getResult().at(1)));
	EXPECT_TRUE(isNil(parser3.getResult().at(2)));
	EXPECT_TRUE(isPlainText(parser3.getResult().at(3), "9999"));
}

TEST(ParserSyntaxNilReports, strayNilInReportBody) {
	metaf::Parser parser1;
	EXPECT_FALSE(parser1.parse("METAR ZZZZ 041115Z 24005KT 9999 NIL FEW040 25/18 Q1011 NOSIG="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(parser1.getResult().size(), 6u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));
	EXPECT_TRUE(isWind(parser1.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser1.getResult().at(4)));
	EXPECT_TRUE(isNil(parser1.getResult().at(5)));

	metaf::Parser parser2;
	EXPECT_FALSE(parser2.parse("TAF ZZZZ 041115Z 0412/0512 24005KT NIL 10SM FEW250="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(parser2.getResult().size(), 6u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser2.getResult().at(3)));
	EXPECT_TRUE(isWind(parser2.getResult().at(4)));
	EXPECT_TRUE(isNil(parser2.getResult().at(5)));

	metaf::Parser parser3;
	EXPECT_FALSE(parser3.parse("ZZZZ 041115Z 24005KT 9999 NIL FEW040 25/18 Q1011 NOSIG="));
	EXPECT_EQ(parser3.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser3.getError(), metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(parser3.getResult().size(), 5u);
	EXPECT_TRUE(isLocation(parser3.getResult().at(0)));
	EXPECT_TRUE(isReportTime(parser3.getResult().at(1)));
	EXPECT_TRUE(isWind(parser3.getResult().at(2)));
	EXPECT_TRUE(isVisibility(parser3.getResult().at(3)));
	EXPECT_TRUE(isNil(parser3.getResult().at(4)));
}

TEST(ParserSyntaxNilReports, typeLocationTimeTimespanNil) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("TAF ZZZZ 041115Z 0415/0424 NIL="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 5u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser.getResult().at(3)));
	EXPECT_TRUE(isNil(parser.getResult().at(4)));
}

TEST(ParserSyntaxNilReports, typeCorAmdLocationTimeTimespanNil) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("TAF COR ZZZZ 041115Z 0415/0424 NIL="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 6u);
	EXPECT_TRUE(isTaf(parser1.getResult().at(0)));
	EXPECT_TRUE(isCor(parser1.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(2)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(3)));
	EXPECT_TRUE(isTimeSpan(parser1.getResult().at(4)));
	EXPECT_TRUE(isNil(parser1.getResult().at(5)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("TAF AMD ZZZZ 041115Z 0415/0424 NIL="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 6u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isAmd(parser2.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(2)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(3)));
	EXPECT_TRUE(isTimeSpan(parser2.getResult().at(4)));
	EXPECT_TRUE(isNil(parser2.getResult().at(5)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxCancelledReports, typeLocationCnl) {
	metaf::Parser parser1;
	EXPECT_FALSE(parser1.parse("METAR ZZZZ CNL="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(parser1.getResult().size(), 3u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isCnl(parser1.getResult().at(2)));

	metaf::Parser parser2;
	EXPECT_FALSE(parser2.parse("TAF ZZZZ CNL="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::EXPECTED_REPORT_TIME);
	EXPECT_EQ(parser2.getResult().size(), 3u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isCnl(parser2.getResult().at(2)));
}

TEST(ParserSyntaxCancelledReports, typeLocationTimeCnl) {
	metaf::Parser parser1;
	EXPECT_FALSE(parser1.parse("METAR ZZZZ 041115Z CNL="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::CNL_ALLOWED_IN_TAF_ONLY);
	EXPECT_EQ(parser1.getResult().size(), 4u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));
	EXPECT_TRUE(isCnl(parser1.getResult().at(3)));

	metaf::Parser parser2;
	EXPECT_FALSE(parser2.parse("TAF ZZZZ 041115Z CNL="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::EXPECTED_TIME_SPAN);
	EXPECT_EQ(parser2.getResult().size(), 4u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(2)));
	EXPECT_TRUE(isCnl(parser2.getResult().at(3)));
}

TEST(ParserSyntaxCancelledReports, strayGroupAfterCnl) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse("TAF ZZZZ 041115Z 0415/0424 CNL 9999="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_CNL);
	EXPECT_EQ(parser.getResult().size(), 6u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser.getResult().at(3)));
	EXPECT_TRUE(isCnl(parser.getResult().at(4)));
	EXPECT_TRUE(isPlainText(parser.getResult().at(5), "9999"));
}

TEST(ParserSyntaxCancelledReports, strayCnlInReportBody) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse("TAF ZZZZ 041115Z 0412/0512 24005KT CNL 10SM FEW250="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(parser.getResult().size(), 6u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser.getResult().at(3)));
	EXPECT_TRUE(isWind(parser.getResult().at(4)));
	EXPECT_TRUE(isCnl(parser.getResult().at(5)));
}

TEST(ParserSyntaxCancelledReports, strayCnlInMetar) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse("METAR ZZZZ 041115Z 24005KT 9999 CNL FEW040 25/18 Q1011 NOSIG="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
	EXPECT_EQ(parser.getResult().size(), 6u);
	EXPECT_TRUE(isMetar(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_TRUE(isWind(parser.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser.getResult().at(4)));
	EXPECT_TRUE(isCnl(parser.getResult().at(5)));
}

TEST(ParserSyntaxCancelledReports, typeLocationTimeTimespanCnl) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("TAF ZZZZ 041115Z 0415/0424 CNL="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 5u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser.getResult().at(3)));
	EXPECT_TRUE(isCnl(parser.getResult().at(4)));
}

TEST(ParserSyntaxCancelledReports, typeCorAmdLocationTimeTimespanCnl) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("TAF COR ZZZZ 041115Z 0415/0424 CNL="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 6u);
	EXPECT_TRUE(isTaf(parser1.getResult().at(0)));
	EXPECT_TRUE(isCor(parser1.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(2)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(3)));
	EXPECT_TRUE(isTimeSpan(parser1.getResult().at(4)));
	EXPECT_TRUE(isCnl(parser1.getResult().at(5)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("TAF AMD ZZZZ 041115Z 0415/0424 CNL="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 6u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isAmd(parser2.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(2)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(3)));
	EXPECT_TRUE(isTimeSpan(parser2.getResult().at(4)));
	EXPECT_TRUE(isCnl(parser2.getResult().at(5)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxValidMetarReports, plainReports) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 9u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));
	EXPECT_TRUE(isWind(parser1.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser1.getResult().at(4)));
	EXPECT_TRUE(isCloud(parser1.getResult().at(5)));
	EXPECT_TRUE(isTemperature(parser1.getResult().at(6)));
	EXPECT_TRUE(isPressure(parser1.getResult().at(7)));
	EXPECT_TRUE(isTrend(parser1.getResult().at(8)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("SPECI ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 9u);
	EXPECT_TRUE(isSpeci(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(2)));
	EXPECT_TRUE(isWind(parser2.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser2.getResult().at(4)));
	EXPECT_TRUE(isCloud(parser2.getResult().at(5)));
	EXPECT_TRUE(isTemperature(parser2.getResult().at(6)));
	EXPECT_TRUE(isPressure(parser2.getResult().at(7)));
	EXPECT_TRUE(isTrend(parser2.getResult().at(8)));

	metaf::Parser parser3;
	EXPECT_TRUE(parser3.parse("METAR ZZZZ 041115Z 24005KT 5000 RASN 01/01 Q0980="));
	EXPECT_EQ(parser3.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser3.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser3.getResult().size(), 8u);
	EXPECT_TRUE(isMetar(parser3.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser3.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser3.getResult().at(2)));
	EXPECT_TRUE(isWind(parser3.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser3.getResult().at(4)));
	EXPECT_TRUE(isWeather(parser3.getResult().at(5)));
	EXPECT_TRUE(isTemperature(parser3.getResult().at(6)));
	EXPECT_TRUE(isPressure(parser3.getResult().at(7)));
}

TEST(ParserSyntaxValidMetarReports, correctionalReports) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("METAR COR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 10u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isCor(parser1.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(2)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(3)));
	EXPECT_TRUE(isWind(parser1.getResult().at(4)));
	EXPECT_TRUE(isVisibility(parser1.getResult().at(5)));
	EXPECT_TRUE(isCloud(parser1.getResult().at(6)));
	EXPECT_TRUE(isTemperature(parser1.getResult().at(7)));
	EXPECT_TRUE(isPressure(parser1.getResult().at(8)));
	EXPECT_TRUE(isTrend(parser1.getResult().at(9)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("SPECI COR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 10u);
	EXPECT_TRUE(isSpeci(parser2.getResult().at(0)));
	EXPECT_TRUE(isCor(parser2.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(2)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(3)));
	EXPECT_TRUE(isWind(parser2.getResult().at(4)));
	EXPECT_TRUE(isVisibility(parser2.getResult().at(5)));
	EXPECT_TRUE(isCloud(parser2.getResult().at(6)));
	EXPECT_TRUE(isTemperature(parser2.getResult().at(7)));
	EXPECT_TRUE(isPressure(parser2.getResult().at(8)));
	EXPECT_TRUE(isTrend(parser2.getResult().at(9)));

	metaf::Parser parser3;
	EXPECT_TRUE(parser3.parse("METAR ZZZZ 041115Z COR 24005KT 9999 FEW040 25/18 Q1011 NOSIG="));
	EXPECT_EQ(parser3.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser3.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser3.getResult().size(), 10u);
	EXPECT_TRUE(isMetar(parser3.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser3.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser3.getResult().at(2)));
	EXPECT_TRUE(isCor(parser3.getResult().at(3)));
	EXPECT_TRUE(isWind(parser3.getResult().at(4)));
	EXPECT_TRUE(isVisibility(parser3.getResult().at(5)));
	EXPECT_TRUE(isCloud(parser3.getResult().at(6)));
	EXPECT_TRUE(isTemperature(parser3.getResult().at(7)));
	EXPECT_TRUE(isPressure(parser3.getResult().at(8)));
	EXPECT_TRUE(isTrend(parser3.getResult().at(9)));
}

TEST(ParserSyntaxValidMetarReports, reportsWithRemarks) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 13u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));
	EXPECT_TRUE(isWind(parser1.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser1.getResult().at(4)));
	EXPECT_TRUE(isCloud(parser1.getResult().at(5)));
	EXPECT_TRUE(isTemperature(parser1.getResult().at(6)));
	EXPECT_TRUE(isPressure(parser1.getResult().at(7)));
	EXPECT_TRUE(isTrend(parser1.getResult().at(8)));
	EXPECT_TRUE(isRmk(parser1.getResult().at(9)));
	EXPECT_TRUE(isPlainText(parser1.getResult().at(10), "TEST"));
	EXPECT_TRUE(isPlainText(parser1.getResult().at(11), "9999"));
	EXPECT_TRUE(isPlainText(parser1.getResult().at(12), "RMK"));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("SPECI ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 13u);
	EXPECT_TRUE(isSpeci(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(2)));
	EXPECT_TRUE(isWind(parser2.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser2.getResult().at(4)));
	EXPECT_TRUE(isCloud(parser2.getResult().at(5)));
	EXPECT_TRUE(isTemperature(parser2.getResult().at(6)));
	EXPECT_TRUE(isPressure(parser2.getResult().at(7)));
	EXPECT_TRUE(isTrend(parser2.getResult().at(8)));
	EXPECT_TRUE(isRmk(parser2.getResult().at(9)));
	EXPECT_TRUE(isPlainText(parser2.getResult().at(10), "TEST"));
	EXPECT_TRUE(isPlainText(parser2.getResult().at(11), "9999"));
	EXPECT_TRUE(isPlainText(parser2.getResult().at(12), "RMK"));
}

TEST(ParserSyntaxValidMetarReports, reportTypeNotSpecified) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 8u);
	EXPECT_TRUE(isLocation(parser1.getResult().at(0)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(1)));
	EXPECT_TRUE(isWind(parser1.getResult().at(2)));
	EXPECT_TRUE(isVisibility(parser1.getResult().at(3)));
	EXPECT_TRUE(isCloud(parser1.getResult().at(4)));
	EXPECT_TRUE(isTemperature(parser1.getResult().at(5)));
	EXPECT_TRUE(isPressure(parser1.getResult().at(6)));
	EXPECT_TRUE(isTrend(parser1.getResult().at(7)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 12u);
	EXPECT_TRUE(isLocation(parser2.getResult().at(0)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(1)));
	EXPECT_TRUE(isWind(parser2.getResult().at(2)));
	EXPECT_TRUE(isVisibility(parser2.getResult().at(3)));
	EXPECT_TRUE(isCloud(parser2.getResult().at(4)));
	EXPECT_TRUE(isTemperature(parser2.getResult().at(5)));
	EXPECT_TRUE(isPressure(parser2.getResult().at(6)));
	EXPECT_TRUE(isTrend(parser2.getResult().at(7)));
	EXPECT_TRUE(isRmk(parser2.getResult().at(8)));
	EXPECT_TRUE(isPlainText(parser2.getResult().at(9), "TEST"));
	EXPECT_TRUE(isPlainText(parser2.getResult().at(10), "9999"));
	EXPECT_TRUE(isPlainText(parser2.getResult().at(11), "RMK"));

	metaf::Parser parser3;
	EXPECT_TRUE(parser3.parse("ZZZZ 041115Z RMK TEST="));
	EXPECT_EQ(parser3.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser3.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser3.getResult().size(), 4u);
	EXPECT_TRUE(isLocation(parser3.getResult().at(0)));
	EXPECT_TRUE(isReportTime(parser3.getResult().at(1)));
	EXPECT_TRUE(isRmk(parser3.getResult().at(2)));
	EXPECT_TRUE(isPlainText(parser3.getResult().at(3), "TEST"));
}

TEST(ParserSyntaxValidMetarReports, reportWithTrend) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 "
		"TEMPO 3500 BKN010 +RA="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 12u);
	EXPECT_TRUE(isMetar(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_TRUE(isWind(parser.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser.getResult().at(4)));
	EXPECT_TRUE(isCloud(parser.getResult().at(5)));
	EXPECT_TRUE(isTemperature(parser.getResult().at(6)));
	EXPECT_TRUE(isPressure(parser.getResult().at(7)));
	EXPECT_TRUE(isTrend(parser.getResult().at(8)));
	EXPECT_TRUE(isVisibility(parser.getResult().at(9)));
	EXPECT_TRUE(isCloud(parser.getResult().at(10)));
	EXPECT_TRUE(isWeather(parser.getResult().at(11)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxValidTafReports, plainReport) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 7u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser.getResult().at(3)));
	EXPECT_TRUE(isWind(parser.getResult().at(4)));
	EXPECT_TRUE(isVisibility(parser.getResult().at(5)));
	EXPECT_TRUE(isCloud(parser.getResult().at(6)));
}

TEST(ParserSyntaxValidTafReports, amendedAndCorrectionalReports) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("TAF AMD ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 8u);
	EXPECT_TRUE(isTaf(parser1.getResult().at(0)));
	EXPECT_TRUE(isAmd(parser1.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(2)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(3)));
	EXPECT_TRUE(isTimeSpan(parser1.getResult().at(4)));
	EXPECT_TRUE(isWind(parser1.getResult().at(5)));
	EXPECT_TRUE(isVisibility(parser1.getResult().at(6)));
	EXPECT_TRUE(isCloud(parser1.getResult().at(7)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("TAF COR ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 8u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isCor(parser2.getResult().at(1)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(2)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(3)));
	EXPECT_TRUE(isTimeSpan(parser2.getResult().at(4)));
	EXPECT_TRUE(isWind(parser2.getResult().at(5)));
	EXPECT_TRUE(isVisibility(parser2.getResult().at(6)));
	EXPECT_TRUE(isCloud(parser2.getResult().at(7)));
}

TEST(ParserSyntaxValidTafReports, reportsWithTemperatureForecasts) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("TAF ZZZZ 041115Z 0412/0512 TX07/0416Z="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 5u);
	EXPECT_TRUE(isTaf(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser1.getResult().at(3)));
	EXPECT_TRUE(isTempForecast(parser1.getResult().at(4)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("TAF ZZZZ 041115Z 0412/0512 TX07/0416Z TN03/0505Z="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 6u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser2.getResult().at(3)));
	EXPECT_TRUE(isTempForecast(parser2.getResult().at(4)));
	EXPECT_TRUE(isTempForecast(parser2.getResult().at(5)));
}

TEST(ParserSyntaxValidTafReports, reportWithRemarks) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250 RMK TEST 9999 RMK="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 11u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser.getResult().at(3)));
	EXPECT_TRUE(isWind(parser.getResult().at(4)));
	EXPECT_TRUE(isVisibility(parser.getResult().at(5)));
	EXPECT_TRUE(isCloud(parser.getResult().at(6)));
	EXPECT_TRUE(isRmk(parser.getResult().at(7)));
	EXPECT_TRUE(isPlainText(parser.getResult().at(8), "TEST"));
	EXPECT_TRUE(isPlainText(parser.getResult().at(9), "9999"));
	EXPECT_TRUE(isPlainText(parser.getResult().at(10), "RMK"));
}

TEST(ParserSyntaxValidTafReports, reportWithTrends) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse(
		"TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250"
		" PROB40"
		" TEMPO 0420/0424 24010G15KT"
		" FM050300 BKN100 3SM RA"
		" BECMG 0506/0510 OVC050"
		"="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 15u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser.getResult().at(3)));
	EXPECT_TRUE(isWind(parser.getResult().at(4)));
	EXPECT_TRUE(isVisibility(parser.getResult().at(5)));
	EXPECT_TRUE(isCloud(parser.getResult().at(6)));
	EXPECT_TRUE(isTrend(parser.getResult().at(7)));
	EXPECT_TRUE(isWind(parser.getResult().at(8)));
	EXPECT_TRUE(isTrend(parser.getResult().at(9)));
	EXPECT_TRUE(isCloud(parser.getResult().at(10)));
	EXPECT_TRUE(isVisibility(parser.getResult().at(11)));
	EXPECT_TRUE(isWeather(parser.getResult().at(12)));
	EXPECT_TRUE(isTrend(parser.getResult().at(13)));
	EXPECT_TRUE(isCloud(parser.getResult().at(14)));
}

TEST(ParserSyntaxValidTafReports, reportTypeNotSpecified) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("ZZZZ 041115Z 0412/0512 TX07/0416Z="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 4u);
	EXPECT_TRUE(isLocation(parser1.getResult().at(0)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(1)));
	EXPECT_TRUE(isTimeSpan(parser1.getResult().at(2)));
	EXPECT_TRUE(isTempForecast(parser1.getResult().at(3)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("ZZZZ 041115Z 0412/0512 RMK TEST="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 5u);
	EXPECT_TRUE(isLocation(parser2.getResult().at(0)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(1)));
	EXPECT_TRUE(isTimeSpan(parser2.getResult().at(2)));
	EXPECT_TRUE(isRmk(parser2.getResult().at(3)));
	EXPECT_TRUE(isPlainText(parser2.getResult().at(4), "TEST"));
}

TEST(ParserSyntaxValidTafReports, reportTimeNotSpecified) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse("TAF ZZZZ 0412/0512 TX07/0416Z TN03/0505Z="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser.getResult().size(), 5u);
	EXPECT_TRUE(isTaf(parser.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser.getResult().at(1)));
	EXPECT_TRUE(isTimeSpan(parser.getResult().at(2)));
	EXPECT_TRUE(isTempForecast(parser.getResult().at(3)));
	EXPECT_TRUE(isTempForecast(parser.getResult().at(4)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxDesignatorsAndSeparators, noReportEndDesignator) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse("METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG"));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 9u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));
	EXPECT_TRUE(isWind(parser1.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser1.getResult().at(4)));
	EXPECT_TRUE(isCloud(parser1.getResult().at(5)));
	EXPECT_TRUE(isTemperature(parser1.getResult().at(6)));
	EXPECT_TRUE(isPressure(parser1.getResult().at(7)));
	EXPECT_TRUE(isTrend(parser1.getResult().at(8)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse("TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250"));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 7u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser2.getResult().at(3)));
	EXPECT_TRUE(isWind(parser2.getResult().at(4)));
	EXPECT_TRUE(isVisibility(parser2.getResult().at(5)));
	EXPECT_TRUE(isCloud(parser2.getResult().at(6)));
}

TEST(ParserSyntaxDesignatorsAndSeparators, variousGroupSeparators) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse(
		"METAR  ZZZZ    041115Z \r\n 24005KT\r9999\nFEW040\r\n25/18 \nQ1011\r NOSIG="));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 9u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));
	EXPECT_TRUE(isWind(parser1.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser1.getResult().at(4)));
	EXPECT_TRUE(isCloud(parser1.getResult().at(5)));
	EXPECT_TRUE(isTemperature(parser1.getResult().at(6)));
	EXPECT_TRUE(isPressure(parser1.getResult().at(7)));
	EXPECT_TRUE(isTrend(parser1.getResult().at(8)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse(
		"TAF  ZZZZ      041115Z \r\n 0412/0512\r24005KT\n10SM\r\nFEW250="));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 7u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser2.getResult().at(3)));
	EXPECT_TRUE(isWind(parser2.getResult().at(4)));
	EXPECT_TRUE(isVisibility(parser2.getResult().at(5)));
	EXPECT_TRUE(isCloud(parser2.getResult().at(6)));
}

TEST(ParserSyntaxDesignatorsAndSeparators, strayTextAfterReportEndDesignator) {
	metaf::Parser parser1;
	EXPECT_TRUE(parser1.parse(
		"METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG= STRAY TEXT 1000"));
	EXPECT_EQ(parser1.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser1.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser1.getResult().size(), 9u);
	EXPECT_TRUE(isMetar(parser1.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser1.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser1.getResult().at(2)));
	EXPECT_TRUE(isWind(parser1.getResult().at(3)));
	EXPECT_TRUE(isVisibility(parser1.getResult().at(4)));
	EXPECT_TRUE(isCloud(parser1.getResult().at(5)));
	EXPECT_TRUE(isTemperature(parser1.getResult().at(6)));
	EXPECT_TRUE(isPressure(parser1.getResult().at(7)));
	EXPECT_TRUE(isTrend(parser1.getResult().at(8)));

	metaf::Parser parser2;
	EXPECT_TRUE(parser2.parse(
		"TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250= STRAY TEXT 1000"));
	EXPECT_EQ(parser2.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser2.getError(), metaf::Parser::Error::NONE);
	EXPECT_EQ(parser2.getResult().size(), 7u);
	EXPECT_TRUE(isTaf(parser2.getResult().at(0)));
	EXPECT_TRUE(isLocation(parser2.getResult().at(1)));
	EXPECT_TRUE(isReportTime(parser2.getResult().at(2)));
	EXPECT_TRUE(isTimeSpan(parser2.getResult().at(3)));
	EXPECT_TRUE(isWind(parser2.getResult().at(4)));
	EXPECT_TRUE(isVisibility(parser2.getResult().at(5)));
	EXPECT_TRUE(isCloud(parser2.getResult().at(6)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfMetar) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO $"));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_TRUE(isMaintenanceIndicator(parser.getResult().back()));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfMetarRemark) {
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO $"));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_TRUE(isMaintenanceIndicator(parser.getResult().back()));
}

TEST(ParserSyntaxMaintenanceIndicator, 
	maintenanceIndicatorAtTheEndOfMetarRemarkWithReportEndDesignator)
{
	metaf::Parser parser;
	EXPECT_TRUE(parser.parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO $="));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	EXPECT_TRUE(isMaintenanceIndicator(parser.getResult().back()));
}


TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfTaf) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 FEW020TCU BKN035 $"));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(parser.getResult().back()));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAtTheEndOfTafRemark) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 FEW020TCU BKN035 RMK TEST $"));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(parser.getResult().back()));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorInTheMiddleOfMetar) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR $ 02/00 A2954"
		" RMK AO2 SLP989 T00150003 58001 PNO TSNO"));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser.getError(), 
		metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_MAINTENANCE_INDICATOR);
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorInTheMiddleOfMetarRemark) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse(
		"METAR BGTL 092056Z AUTO 30007KT 9999 CLR 02/00 A2954"
		" RMK AO2 SLP989 T00150003 $ 58001 PNO TSNO"));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser.getError(), 
		metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_MAINTENANCE_INDICATOR);
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorInTheMiddleOfTaf) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 $ FEW020TCU BKN035"));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(parser.getResult().back()));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorInTheMiddleOfTafRemark) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse(
		"TAF ENLE 101100Z 1012/1021 24028KT 9999 FEW020TCU BKN035 RMK TEST $ TEST"));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::TAF);
	EXPECT_EQ(parser.getError(), 
		metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
	EXPECT_TRUE(isMaintenanceIndicator(parser.getResult().back()));
}

TEST(ParserSyntaxMaintenanceIndicator, maintenanceIndicatorAfterNil) {
	metaf::Parser parser;
	EXPECT_FALSE(parser.parse("METAR ZZZZ 041115Z NIL $"));
	EXPECT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	EXPECT_EQ(parser.getError(), metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Parser, doNotKeepSourceGroup) {
	metaf::Parser parser;
	ASSERT_TRUE(parser.parse("METAR EGYP 092050Z 06007KT CAVOK 08/02 Q1024 BLU"));
	ASSERT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	ASSERT_EQ(parser.getError(), metaf::Parser::Error::NONE);
	ASSERT_NE(parser.getResult().size(), 0u);
	EXPECT_EQ(parser.getSourceGroups().size(), 0u);
}

TEST(Parser, keepSourceGroups) {
	metaf::Parser parser;
	ASSERT_TRUE(parser.parse(
		"METAR KLGA 111951Z 04004KT 2 1/2SM HZ BKN008 OVC015 23/21 A3012"
		" RMK AO2 SFC VIS 4 SLP198 T02330206=", 
		true));
	ASSERT_EQ(parser.getReportType(), metaf::ReportType::METAR);
	ASSERT_EQ(parser.getError(), metaf::Parser::Error::NONE);

	EXPECT_EQ(parser.getResult().size(), 17u);
	EXPECT_EQ(parser.getSourceGroups().size(), 17u);

	EXPECT_TRUE(isMetar(parser.getResult().at(0)));
	EXPECT_EQ(parser.getSourceGroups().at(0), "METAR");

	EXPECT_TRUE(isLocation(parser.getResult().at(1))); 
	EXPECT_EQ(parser.getSourceGroups().at(1), "KLGA");

	EXPECT_TRUE(isReportTime(parser.getResult().at(2)));
	EXPECT_EQ(parser.getSourceGroups().at(2), "111951Z");

	EXPECT_TRUE(isWind(parser.getResult().at(3)));
	EXPECT_EQ(parser.getSourceGroups().at(3), "04004KT");

	EXPECT_TRUE(isVisibility(parser.getResult().at(4)));
	EXPECT_EQ(parser.getSourceGroups().at(4), "2 1/2SM");

	EXPECT_TRUE(isWeather(parser.getResult().at(5)));
	EXPECT_EQ(parser.getSourceGroups().at(5), "HZ");

	EXPECT_TRUE(isCloud(parser.getResult().at(6)));
	EXPECT_EQ(parser.getSourceGroups().at(6), "BKN008");

	EXPECT_TRUE(isCloud(parser.getResult().at(7)));
	EXPECT_EQ(parser.getSourceGroups().at(7), "OVC015");

	EXPECT_TRUE(isTemperature(parser.getResult().at(8)));
	EXPECT_EQ(parser.getSourceGroups().at(8), "23/21");

	EXPECT_TRUE(isPressure(parser.getResult().at(9)));
	EXPECT_EQ(parser.getSourceGroups().at(9), "A3012");

	EXPECT_TRUE(isRmk(parser.getResult().at(10)));
	EXPECT_EQ(parser.getSourceGroups().at(10), "RMK");

	EXPECT_TRUE(isFixedGroup(parser.getResult().at(11)));
	EXPECT_EQ(parser.getSourceGroups().at(11), "AO2");

	EXPECT_TRUE(isPlainText(parser.getResult().at(12), "SFC"));
	EXPECT_EQ(parser.getSourceGroups().at(12), "SFC");

	EXPECT_TRUE(isPlainText(parser.getResult().at(13), "VIS"));
	EXPECT_EQ(parser.getSourceGroups().at(13), "VIS");

	EXPECT_TRUE(isPlainText(parser.getResult().at(14), "4"));
	EXPECT_EQ(parser.getSourceGroups().at(14), "4");

	EXPECT_TRUE(isPlainText(parser.getResult().at(15), "SLP198"));
	EXPECT_EQ(parser.getSourceGroups().at(15), "SLP198");

	EXPECT_TRUE(isPlainText(parser.getResult().at(16), "T02330206"));
	EXPECT_EQ(parser.getSourceGroups().at(16), "T02330206");
}
