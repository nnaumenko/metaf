/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

const auto margin = 0.01 / 2;

///////////////////////////////////////////////////////////////////////////////
// Single remark groups
// Purpose: to confirm that groups which consists of single remark are parsed 
// correctly
///////////////////////////////////////////////////////////////////////////////

TEST(PrecipitationGroup, parse4digitGroup) {
	const auto pg1 = metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::TOTAL_PRECIPITATION_HOURLY);
	EXPECT_EQ(pg1->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg1->total().amount().has_value());
	EXPECT_NEAR(pg1->total().amount().value(), 21.68, margin);
	EXPECT_FALSE(pg1->recent().isReported());	

	const auto pg2 = metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_OR_6_HOURLY);
	EXPECT_EQ(pg2->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg2->total().amount().has_value());
	EXPECT_NEAR(pg2->total().amount().value(), 2.17, margin);
	EXPECT_FALSE(pg2->recent().isReported());	

	const auto pg3 = metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_24_HOURLY);
	EXPECT_EQ(pg3->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg3->total().amount().has_value());
	EXPECT_NEAR(pg3->total().amount().value(), 1.25, margin);
	EXPECT_FALSE(pg3->recent().isReported());	
}

TEST(PrecipitationGroup, parse4digitGroupNotReported) {
	const auto pg1 = metaf::PrecipitationGroup::parse("P////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::TOTAL_PRECIPITATION_HOURLY);
	EXPECT_FALSE(pg1->total().isReported());
	EXPECT_FALSE(pg1->recent().isReported());	

	const auto pg2 = metaf::PrecipitationGroup::parse("6////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_OR_6_HOURLY);
	EXPECT_FALSE(pg2->total().isReported());
	EXPECT_FALSE(pg2->recent().isReported());	

	const auto pg3 = metaf::PrecipitationGroup::parse("7////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_24_HOURLY);
	EXPECT_FALSE(pg3->total().isReported());
	EXPECT_FALSE(pg3->recent().isReported());	
}

TEST(PrecipitationGroup, parse3digitGroup) {
	const auto pg1 = metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::SNOW_DEPTH_ON_GROUND);
	EXPECT_EQ(pg1->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg1->total().amount().has_value());
	EXPECT_NEAR(pg1->total().amount().value(), 21, margin);
	EXPECT_FALSE(pg1->recent().isReported());	

	const auto pg2 = metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::SNOW_6_HOURLY);
	EXPECT_EQ(pg2->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg2->total().amount().has_value());
	EXPECT_NEAR(pg2->total().amount().value(), 1.1, margin);
	EXPECT_FALSE(pg2->recent().isReported());	

	const auto pg3 = metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::WATER_EQUIV_OF_SNOW_ON_GROUND);
	EXPECT_EQ(pg3->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg3->total().amount().has_value());
	EXPECT_NEAR(pg3->total().amount().value(), 12.5, margin);	
	EXPECT_FALSE(pg3->recent().isReported());	

	const auto pg4 = metaf::PrecipitationGroup::parse("I1010", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg4.has_value());
	EXPECT_EQ(pg4->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_HOUR);
	EXPECT_EQ(pg4->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg4->total().amount().has_value());
	EXPECT_NEAR(pg4->total().amount().value(), 0.10, margin);	
	EXPECT_FALSE(pg4->recent().isReported());	

	const auto pg5 = metaf::PrecipitationGroup::parse("I3015", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg5.has_value());
	EXPECT_EQ(pg5->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_3_HOURS);
	EXPECT_EQ(pg5->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg5->total().amount().has_value());
	EXPECT_NEAR(pg5->total().amount().value(), 0.15, margin);	
	EXPECT_FALSE(pg5->recent().isReported());	

	const auto pg6 = metaf::PrecipitationGroup::parse("I6022", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg6.has_value());
	EXPECT_EQ(pg6->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_6_HOURS);
	EXPECT_EQ(pg6->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg6->total().amount().has_value());
	EXPECT_NEAR(pg6->total().amount().value(), 0.22, margin);
	EXPECT_FALSE(pg6->recent().isReported());	

	const auto pg7 = metaf::PrecipitationGroup::parse("PP134", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg7.has_value());
	EXPECT_EQ(pg7->type(), metaf::PrecipitationGroup::Type::PRECIPITATION_ACCUMULATION_SINCE_LAST_REPORT);
	EXPECT_EQ(pg7->total().unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(pg7->total().amount().has_value());
	EXPECT_NEAR(pg7->total().amount().value(), 13.4, margin);
	EXPECT_FALSE(pg7->recent().isReported());	
}

TEST(PrecipitationGroup, parse3digitGroupNotReported) {
	const auto pg1 = metaf::PrecipitationGroup::parse("4////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::SNOW_DEPTH_ON_GROUND);
	EXPECT_FALSE(pg1->total().isReported());
	EXPECT_FALSE(pg1->recent().isReported());	

	const auto pg2 = metaf::PrecipitationGroup::parse("931///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::SNOW_6_HOURLY);
	EXPECT_FALSE(pg2->total().isReported());
	EXPECT_FALSE(pg2->recent().isReported());	

	const auto pg3 = metaf::PrecipitationGroup::parse("933///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::WATER_EQUIV_OF_SNOW_ON_GROUND);
	EXPECT_FALSE(pg3->total().isReported());
	EXPECT_FALSE(pg3->recent().isReported());	

	const auto pg4 = metaf::PrecipitationGroup::parse("I1///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg4.has_value());
	EXPECT_EQ(pg4->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_HOUR);
	EXPECT_FALSE(pg4->total().isReported());
	EXPECT_FALSE(pg4->recent().isReported());	

	const auto pg5 = metaf::PrecipitationGroup::parse("I3///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg5.has_value());
	EXPECT_EQ(pg5->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_3_HOURS);
	EXPECT_FALSE(pg5->total().isReported());
	EXPECT_FALSE(pg5->recent().isReported());	

	const auto pg6 = metaf::PrecipitationGroup::parse("I6///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg6.has_value());
	EXPECT_EQ(pg6->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_6_HOURS);
	EXPECT_FALSE(pg6->total().isReported());
	EXPECT_FALSE(pg6->recent().isReported());	

	const auto pg7 = metaf::PrecipitationGroup::parse("PP///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg7.has_value());
	EXPECT_EQ(pg7->type(), metaf::PrecipitationGroup::Type::PRECIPITATION_ACCUMULATION_SINCE_LAST_REPORT);
	EXPECT_FALSE(pg7->total().isReported());
	EXPECT_FALSE(pg7->recent().isReported());	
}

///////////////////////////////////////////////////////////////////////////////
// Rainfall (RFxx.x/xxx.x) group
// Purpose: to confirm that rainfall groups included in METAR body are parsed 
// correctly, and that malformed groups of this type cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(PrecipitationGroup, parseRainfall) {
	const auto pg = 
		metaf::PrecipitationGroup::parse("RF02.7/010.5", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg.has_value());

	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::RAINFALL_9AM_10MIN);

	EXPECT_EQ(pg->total().unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(pg->total().amount().has_value());
	EXPECT_NEAR(pg->total().amount().value(), 10.5, margin);

	EXPECT_EQ(pg->recent().unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(pg->recent().amount().has_value());
	EXPECT_NEAR(pg->recent().amount().value(), 2.7, margin);
}

TEST(PrecipitationGroup, parseRainfallNotReported) {
	const auto pg1 = metaf::PrecipitationGroup::parse("RF//.//010.5", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::RAINFALL_9AM_10MIN);
	EXPECT_EQ(pg1->total().unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(pg1->total().amount().has_value());
	EXPECT_NEAR(pg1->total().amount().value(), 10.5, margin);
	EXPECT_FALSE(pg1->recent().isReported());

	const auto pg2 = metaf::PrecipitationGroup::parse("RF02.7////./", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::RAINFALL_9AM_10MIN);
	EXPECT_FALSE(pg2->total().isReported());
	EXPECT_EQ(pg2->recent().unit(), metaf::Precipitation::Unit::MM);
	ASSERT_TRUE(pg2->recent().amount().has_value());
	EXPECT_NEAR(pg2->recent().amount().value(), 2.7, margin);

	const auto pg3 = metaf::PrecipitationGroup::parse("RF//./////./", metaf::ReportPart::METAR);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::RAINFALL_9AM_10MIN);
	EXPECT_FALSE(pg3->total().isReported());
	EXPECT_FALSE(pg3->recent().isReported());
}

TEST(PrecipitationGroup, rainfallWrongReportPart) {
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/010.5", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/010.5", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/010.5", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/010.5", metaf::ReportPart::RMK).has_value());
}

TEST(PrecipitationGroup, rainfallWrongFormat) {
	// Note: three-value group is not parsed by Metaf anymore since version 5.0.0
	// because only two-value group appears to be included in METARs
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF21.5/112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF//.//112.4/031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF21.5////.//031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF21.5////.//031.8", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF21.5////./////./", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::PrecipitationGroup::parse("0F02.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RA02.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/010.5/", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF/02.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7-010.5", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF010.5", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF002.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.70/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/0010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF2.70/010.50", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF2.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02./010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02/010.5", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/10.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/010.", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/010", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02-7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/010-5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02-7/010-5", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF0207/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/01005", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF0207/01005", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF0A.7/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.A/010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/01A.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/010.A", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF/.//010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF//./010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF///.//010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF//.///010.5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7///./", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7////.", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7/////./", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF02.7////.//", metaf::ReportPart::METAR).has_value());

	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF//-/////./", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("RF//./////-/", metaf::ReportPart::METAR).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// SNINCR group
// Purpose: to confirm that SNINCR groups included in remarks are parsed and 
// appended correctly, and malformed groups of this type cannot be parsed or 
// appended 
///////////////////////////////////////////////////////////////////////////////

TEST(PrecipitationGroup, parseSnincrGroup) {
	const auto pg = metaf::PrecipitationGroup::parse("SNINCR", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::SNOW_INCREASING_RAPIDLY);
	EXPECT_FALSE(pg->total().isReported());
	EXPECT_FALSE(pg->recent().isReported());	
}

TEST(PrecipitationGroup, appendFractionToSnincr) {
	const std::string snincrStr("SNINCR");
	const std::string fractionStr("4/12");

	auto pg = metaf::PrecipitationGroup::parse(snincrStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());

	EXPECT_EQ(pg->append(fractionStr, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::SNOW_INCREASING_RAPIDLY);

	EXPECT_EQ(pg->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg->total().amount().has_value());
	EXPECT_NEAR(pg->total().amount().value(), 12, margin);

	EXPECT_EQ(pg->recent().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg->recent().amount().has_value());
	EXPECT_NEAR(pg->recent().amount().value(), 4, margin);
}

TEST(PrecipitationGroup, appendOtherToSnincr) {
	auto pg = metaf::PrecipitationGroup::parse("SNINCR", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());

	EXPECT_EQ(pg->append("SNINCR", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("A/3", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("1/A", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("1/2SM", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("4", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("ABCD", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::SNOW_INCREASING_RAPIDLY);
	EXPECT_FALSE(pg->total().isReported());
	EXPECT_FALSE(pg->recent().isReported());	
}

///////////////////////////////////////////////////////////////////////////////
// Incorrect groups
// Purpose: to confirm that malformed groups which consist of single remark 
// cannot be parsed
///////////////////////////////////////////////////////////////////////////////

TEST(PrecipitationGroup, parseWrongReportPart) {
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("SNINCR", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("SNINCR", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("SNINCR", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("SNINCR", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PP000", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PP000", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PP000", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PP000", metaf::ReportPart::TAF).has_value());
}

TEST(PrecipitationGroup, parseWrongFormat4digitGroup) {
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("X2168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("52168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("02168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("12168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("22168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("32168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("42168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("52168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("82168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("92168", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P02168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PA168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P216A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P/////", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("602168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6A168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6216A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6/////", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("702168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7A168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7216A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7/////", metaf::ReportPart::RMK).has_value());
}

TEST(PrecipitationGroup, parseWrongFormatSnowDepthGroup) {
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("0/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("1/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("2/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("3/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("5/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("8/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("9/021", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/0021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/A21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/21A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/////", metaf::ReportPart::RMK).has_value());
}

TEST(PrecipitationGroup, parseWrongFormat93Group) {
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("93A125", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("930/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("932/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("935/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("936/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("937/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("938/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("939/021", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931/0021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931/21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931/A21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931/21A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931//", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931////", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933/0021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933/21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933/A21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933/21A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933//", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933////", metaf::ReportPart::RMK).has_value());
}

TEST(PrecipitationGroup, parseWrongFormatPPGroup) {
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PA000", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("ZZ000", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PPA00", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PP0A0", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PP00A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PP00", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PP0000", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PP//", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PP////", metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// 3-hourly vs 6-hourly observations
// Purpose: to confirm that groups which may represent either 3-hourly or 
// 6-hourly observation are correctly distinguished based on report time.
///////////////////////////////////////////////////////////////////////////////

TEST(PrecipitationGroup, parse3hourly) {
	const auto time3hourly = metaf::MetafTime::fromStringDDHHMM("310300");
	ASSERT_TRUE(time3hourly.has_value());
    ASSERT_TRUE(time3hourly->is3hourlyReportTime());

    metaf::ReportMetadata reportMetadata;
    reportMetadata.reportTime = time3hourly;

	const auto pg = metaf::PrecipitationGroup::parse("60217", 
		metaf::ReportPart::RMK, 
		reportMetadata);
	ASSERT_TRUE(pg.has_value());
	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_HOURLY);
	EXPECT_EQ(pg->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg->total().amount().has_value());
	EXPECT_NEAR(pg->total().amount().value(), 2.17, margin);
}

TEST(PrecipitationGroup, parse6hourly) {
	const auto time6hourly = metaf::MetafTime::fromStringDDHHMM("310600");
	ASSERT_TRUE(time6hourly.has_value());
    ASSERT_TRUE(time6hourly->is6hourlyReportTime());

    metaf::ReportMetadata reportMetadata;
    reportMetadata.reportTime = time6hourly;
    
	const auto pg = metaf::PrecipitationGroup::parse("60217", 
		metaf::ReportPart::RMK,
		reportMetadata);
	ASSERT_TRUE(pg.has_value());
	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_6_HOURLY);
	EXPECT_EQ(pg->total().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg->total().amount().has_value());
	EXPECT_NEAR(pg->total().amount().value(), 2.17, margin);
}

///////////////////////////////////////////////////////////////////////////////
// Tests for missing data and non-operational devices
// Purpose: to confirm that groups PNO, FZRANO, PCPN MISC, ICG MISG are parsed
// and appended correctly
///////////////////////////////////////////////////////////////////////////////

TEST(PrecipitationGroup, parsePno) {
	auto pg = metaf::PrecipitationGroup::parse("PNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::PNO);
	EXPECT_FALSE(pg->total().isReported());
	EXPECT_FALSE(pg->recent().isReported());
}

TEST(PrecipitationGroup, parsePnoWrongReportPart) {
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("PNO", 
		metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("PNO", 
		metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("PNO", 
		metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("PNO", 
		metaf::ReportPart::TAF).has_value());
}

TEST(PrecipitationGroup, parseFzrano) {
	auto pg = metaf::PrecipitationGroup::parse("FZRANO", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::FZRANO);
	EXPECT_FALSE(pg->total().isReported());
	EXPECT_FALSE(pg->recent().isReported());
}

TEST(PrecipitationGroup, parseFzranoWrongReportPart) {
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("FZRANO", 
		metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("FZRANO", 
		metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("FZRANO", 
		metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("FZRANO", 
		metaf::ReportPart::TAF).has_value());
}

TEST(PrecipitationGroup, parseIcgMisg) {
	auto pg = metaf::PrecipitationGroup::parse("ICG", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	ASSERT_EQ(pg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::ICG_MISG);
	EXPECT_FALSE(pg->total().isReported());
	EXPECT_FALSE(pg->recent().isReported());
}

TEST(PrecipitationGroup, parseIcgWrongReportPart) {
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("ICG", 
		metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("ICG", 
		metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("ICG", 
		metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("ICG", 
		metaf::ReportPart::TAF).has_value());
}

TEST(PrecipitationGroup, parsePcpnMisg) {
	auto pg = metaf::PrecipitationGroup::parse("PCPN", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	ASSERT_EQ(pg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::PCPN_MISG);
	EXPECT_FALSE(pg->total().isReported());
	EXPECT_FALSE(pg->recent().isReported());
}

TEST(PrecipitationGroup, parsePcpnWrongReportPart) {
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("PCPN", 
		metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("PCPN", 
		metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("PCPN", 
		metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::PrecipitationGroup::parse("PCPN", 
		metaf::ReportPart::TAF).has_value());
}

TEST(PrecipitationGroup, parseIcgAppendOther) {
	auto pg1 = metaf::PrecipitationGroup::parse("ICG", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->append("ICG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto pg2 = metaf::PrecipitationGroup::parse("ICG", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->append("MSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto pg3 = metaf::PrecipitationGroup::parse("ICG", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto pg4 = metaf::PrecipitationGroup::parse("ICG", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg4.has_value());
	EXPECT_EQ(pg4->append("PNO", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto pg5 = metaf::PrecipitationGroup::parse("ICG", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg5.has_value());
	EXPECT_EQ(pg5->append("P////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(PrecipitationGroup, parseIcgMisgAppendOther) {
	auto pg = metaf::PrecipitationGroup::parse("ICG", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	ASSERT_EQ(pg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(pg->append("ICG", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("PNO", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("P////", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::ICG_MISG);
	EXPECT_FALSE(pg->total().isReported());
	EXPECT_FALSE(pg->recent().isReported());
}

TEST(PrecipitationGroup, parsePcpnAppendOther) {
	auto pg1 = metaf::PrecipitationGroup::parse("PCPN", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->append("ICG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto pg2 = metaf::PrecipitationGroup::parse("PCPN", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->append("MSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto pg3 = metaf::PrecipitationGroup::parse("PCPN", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto pg4 = metaf::PrecipitationGroup::parse("PCPN", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg4.has_value());
	EXPECT_EQ(pg4->append("PNO", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto pg5 = metaf::PrecipitationGroup::parse("PCPN", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg5.has_value());
	EXPECT_EQ(pg5->append("P////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(PrecipitationGroup, parsePcpnMisgAppendOther) {
	auto pg = metaf::PrecipitationGroup::parse("PCPN", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	ASSERT_EQ(pg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(pg->append("ICG", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("PNO", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(pg->append("P////", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::PCPN_MISG);
	EXPECT_FALSE(pg->total().isReported());
	EXPECT_FALSE(pg->recent().isReported());
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

TEST(PrecipitationGroup, isValid4digit) {
	const auto pg1 = metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_TRUE(pg1->isValid());

	const auto pg2 = metaf::PrecipitationGroup::parse("P////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_TRUE(pg2->isValid());

	const auto pg3 = metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_TRUE(pg3->isValid());

	const auto pg4 = metaf::PrecipitationGroup::parse("6////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg4.has_value());
	EXPECT_TRUE(pg4->isValid());

	const auto pg5 = metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg5.has_value());
	EXPECT_TRUE(pg5->isValid());

	const auto pg6 = metaf::PrecipitationGroup::parse("7////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg6.has_value());
	EXPECT_TRUE(pg6->isValid());
}

TEST(PrecipitationGroup, isValid3digit) {
	const auto pg1 = metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_TRUE(pg1->isValid());

	const auto pg2 = metaf::PrecipitationGroup::parse("4////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_TRUE(pg2->isValid());

	const auto pg3 = metaf::PrecipitationGroup::parse("I1010", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_TRUE(pg3->isValid());

	const auto pg4 = metaf::PrecipitationGroup::parse("I1///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg4.has_value());
	EXPECT_TRUE(pg4->isValid());

	const auto pg5 = metaf::PrecipitationGroup::parse("I3015", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg5.has_value());
	EXPECT_TRUE(pg5->isValid());

	const auto pg6 = metaf::PrecipitationGroup::parse("I3///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg6.has_value());
	EXPECT_TRUE(pg6->isValid());

	const auto pg7 = metaf::PrecipitationGroup::parse("I6022", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg7.has_value());
	EXPECT_TRUE(pg7->isValid());

	const auto pg8 = metaf::PrecipitationGroup::parse("I6///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg8.has_value());
	EXPECT_TRUE(pg8->isValid());

	const auto pg9 = metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg9.has_value());
	EXPECT_TRUE(pg9->isValid());

	const auto pg10 = metaf::PrecipitationGroup::parse("931///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg10.has_value());
	EXPECT_TRUE(pg10->isValid());

	const auto pg11 = metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg11.has_value());
	EXPECT_TRUE(pg11->isValid());

	const auto pg12 = metaf::PrecipitationGroup::parse("933///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg12.has_value());
	EXPECT_TRUE(pg12->isValid());

	const auto pg13 = metaf::PrecipitationGroup::parse("PP012", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg13.has_value());
	EXPECT_TRUE(pg13->isValid());

	const auto pg14 = metaf::PrecipitationGroup::parse("PP///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg14.has_value());
	EXPECT_TRUE(pg14->isValid());
}

TEST(PrecipitationGroup, isValidSnincr) {
	auto pg = metaf::PrecipitationGroup::parse("SNINCR", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_TRUE(pg->isValid());
	ASSERT_EQ(pg->append("4/12", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(pg->isValid());
}

TEST(PrecipitationGroup, isValidRainfall) {
	const auto rg1 = 
		metaf::PrecipitationGroup::parse("RF02.7/010.5", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg1.has_value());
	EXPECT_TRUE(rg1->isValid());

	const auto rg2 = 
		metaf::PrecipitationGroup::parse("RF02.7////./", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg2.has_value());
	EXPECT_TRUE(rg2->isValid());

	const auto rg3 = 
		metaf::PrecipitationGroup::parse("RF//.//010.5", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg3.has_value());
	EXPECT_TRUE(rg3->isValid());

	const auto rg4 = 
		metaf::PrecipitationGroup::parse("RF//./////./", metaf::ReportPart::METAR);
	ASSERT_TRUE(rg4.has_value());
	EXPECT_TRUE(rg4->isValid());
}

TEST(PrecipitationGroup, isValidPnoFzrano) {
	const auto pg1 = metaf::PrecipitationGroup::parse("PNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_TRUE(pg1->isValid());

	const auto pg2 = metaf::PrecipitationGroup::parse("FZRANO", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_TRUE(pg2->isValid());
}

TEST(PrecipitationGroup, isValidIcgPcpn) {
	auto pg1 = metaf::PrecipitationGroup::parse("ICG", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_FALSE(pg1->isValid());
	ASSERT_EQ(pg1->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(pg1->isValid());

	auto pg2 = metaf::PrecipitationGroup::parse("PCPN", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_FALSE(pg2->isValid());
	ASSERT_EQ(pg2->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(pg2->isValid());
}
