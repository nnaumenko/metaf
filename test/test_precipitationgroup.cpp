/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

const auto margin = 0.01 / 2;

TEST(PrecipitationGroup, parse4digitGroup) {
	const auto pg1 = metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::TOTAL_PRECIPITATION_HOURLY);
	EXPECT_EQ(pg1->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg1->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg1->amount().precipitation().has_value());
	EXPECT_NEAR(pg1->amount().precipitation().value(), 21.68, margin);
	EXPECT_FALSE(pg1->tendency().isReported());	

	const auto pg2 = metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_OR_6_HOURLY);
	EXPECT_EQ(pg2->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg2->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg2->amount().precipitation().has_value());
	EXPECT_NEAR(pg2->amount().precipitation().value(), 2.17, margin);
	EXPECT_FALSE(pg2->tendency().isReported());	

	const auto pg3 = metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_24_HOURLY);
	EXPECT_EQ(pg3->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg3->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg3->amount().precipitation().has_value());
	EXPECT_NEAR(pg3->amount().precipitation().value(), 1.25, margin);
	EXPECT_FALSE(pg3->tendency().isReported());	
}

TEST(PrecipitationGroup, parse4digitGroupNotReported) {
	const auto pg1 = metaf::PrecipitationGroup::parse("P////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::TOTAL_PRECIPITATION_HOURLY);
	EXPECT_EQ(pg1->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg1->amount().precipitation().has_value());
	EXPECT_FALSE(pg1->tendency().isReported());	

	const auto pg2 = metaf::PrecipitationGroup::parse("6////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_OR_6_HOURLY);
	EXPECT_EQ(pg2->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg2->amount().precipitation().has_value());
	EXPECT_FALSE(pg2->tendency().isReported());	

	const auto pg3 = metaf::PrecipitationGroup::parse("7////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_24_HOURLY);
	EXPECT_EQ(pg3->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg3->amount().precipitation().has_value());
	EXPECT_FALSE(pg3->tendency().isReported());	
}

TEST(PrecipitationGroup, parse3digitGroup) {
	const auto pg1 = metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::SNOW_DEPTH_ON_GROUND);
	EXPECT_EQ(pg1->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg1->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg1->amount().precipitation().has_value());
	EXPECT_NEAR(pg1->amount().precipitation().value(), 21, margin);
	EXPECT_FALSE(pg1->tendency().isReported());	

	const auto pg2 = metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::SNOW_6_HOURLY);
	EXPECT_EQ(pg2->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg2->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg2->amount().precipitation().has_value());
	EXPECT_NEAR(pg2->amount().precipitation().value(), 1.1, margin);
	EXPECT_FALSE(pg2->tendency().isReported());	

	const auto pg3 = metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::WATER_EQUIV_OF_SNOW_ON_GROUND);
	EXPECT_EQ(pg3->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg3->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg3->amount().precipitation().has_value());
	EXPECT_NEAR(pg3->amount().precipitation().value(), 12.5, margin);	
	EXPECT_FALSE(pg3->tendency().isReported());	

	const auto pg4 = metaf::PrecipitationGroup::parse("I1010", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg4.has_value());
	EXPECT_EQ(pg4->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_HOUR);
	EXPECT_EQ(pg4->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg4->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg4->amount().precipitation().has_value());
	EXPECT_NEAR(pg4->amount().precipitation().value(), 0.10, margin);	
	EXPECT_FALSE(pg4->tendency().isReported());	

	const auto pg5 = metaf::PrecipitationGroup::parse("I3015", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg5.has_value());
	EXPECT_EQ(pg5->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_3_HOURS);
	EXPECT_EQ(pg5->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg5->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg5->amount().precipitation().has_value());
	EXPECT_NEAR(pg5->amount().precipitation().value(), 0.15, margin);	
	EXPECT_FALSE(pg5->tendency().isReported());	

	const auto pg6 = metaf::PrecipitationGroup::parse("I6022", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg6.has_value());
	EXPECT_EQ(pg6->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_6_HOURS);
	EXPECT_EQ(pg6->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg6->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg6->amount().precipitation().has_value());
	EXPECT_NEAR(pg6->amount().precipitation().value(), 0.22, margin);
	EXPECT_FALSE(pg6->tendency().isReported());	
}

TEST(PrecipitationGroup, parse3digitGroupNotReported) {
	const auto pg1 = metaf::PrecipitationGroup::parse("4////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::SNOW_DEPTH_ON_GROUND);
	EXPECT_EQ(pg1->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg1->amount().precipitation().has_value());
	EXPECT_FALSE(pg1->tendency().isReported());	

	const auto pg2 = metaf::PrecipitationGroup::parse("931///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::SNOW_6_HOURLY);
	EXPECT_EQ(pg2->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg2->amount().precipitation().has_value());
	EXPECT_FALSE(pg2->tendency().isReported());	

	const auto pg3 = metaf::PrecipitationGroup::parse("933///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::WATER_EQUIV_OF_SNOW_ON_GROUND);
	EXPECT_EQ(pg3->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg3->amount().precipitation().has_value());	
	EXPECT_FALSE(pg3->tendency().isReported());	

	const auto pg4 = metaf::PrecipitationGroup::parse("I1///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg4.has_value());
	EXPECT_EQ(pg4->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_HOUR);
	EXPECT_EQ(pg4->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg4->amount().precipitation().has_value());	
	EXPECT_FALSE(pg4->tendency().isReported());	

	const auto pg5 = metaf::PrecipitationGroup::parse("I3///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg5.has_value());
	EXPECT_EQ(pg5->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_3_HOURS);
	EXPECT_EQ(pg5->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg5->amount().precipitation().has_value());	
	EXPECT_FALSE(pg5->tendency().isReported());	

	const auto pg6 = metaf::PrecipitationGroup::parse("I6///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg6.has_value());
	EXPECT_EQ(pg6->type(), metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_6_HOURS);
	EXPECT_EQ(pg6->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg6->amount().precipitation().has_value());	
	EXPECT_FALSE(pg6->tendency().isReported());	

}

TEST(PrecipitationGroup, parseSnincrGroup) {
	const auto pg = metaf::PrecipitationGroup::parse("SNINCR", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::SNOW_INCREASING_RAPIDLY);
	EXPECT_FALSE(pg->amount().isReported());
	EXPECT_FALSE(pg->tendency().isReported());	
}

TEST(PrecipitationGroup, appendFractionToSnincr) {

	const std::string snincrStr("SNINCR");
	const std::string fractionStr("4/12");

	auto pg = metaf::PrecipitationGroup::parse(snincrStr, metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());

	EXPECT_EQ(pg->append(fractionStr, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(pg->type(), metaf::PrecipitationGroup::Type::SNOW_INCREASING_RAPIDLY);

	EXPECT_EQ(pg->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg->amount().precipitation().has_value());
	EXPECT_NEAR(pg->amount().precipitation().value(), 12, margin);

	EXPECT_EQ(pg->tendency().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg->tendency().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg->tendency().precipitation().has_value());
	EXPECT_NEAR(pg->tendency().precipitation().value(), 4, margin);
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
	EXPECT_FALSE(pg->amount().isReported());
	EXPECT_FALSE(pg->tendency().isReported());	
}

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
	EXPECT_EQ(pg->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg->amount().precipitation().has_value());
	EXPECT_NEAR(pg->amount().precipitation().value(), 2.17, margin);
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
	EXPECT_EQ(pg->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg->amount().precipitation().has_value());
	EXPECT_NEAR(pg->amount().precipitation().value(), 2.17, margin);
}

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

	const auto pg13 = metaf::PrecipitationGroup::parse("933///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg13.has_value());
	EXPECT_TRUE(pg13->isValid());
}

TEST(PrecipitationGroup, isValidSnincr) {
	auto pg = metaf::PrecipitationGroup::parse("SNINCR", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg.has_value());
	EXPECT_TRUE(pg->isValid());

	ASSERT_EQ(pg->append("4/12", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(pg->isValid());
}