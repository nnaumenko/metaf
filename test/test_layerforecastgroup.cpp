/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

static const auto margin = 1.0 / 2;

///////////////////////////////////////////////////////////////////////////////
// Atmospheric layer range
// Purpose: to confirm that base and top heights which define the atmospheric
// layer range are parsed and identified correctly
///////////////////////////////////////////////////////////////////////////////

TEST(LayerForecastGroup, baseTopHeights) {
	const auto lfg1 = metaf::LayerForecastGroup::parse("620304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg1.has_value());

	EXPECT_TRUE(lfg1->baseHeight().isReported());	
	EXPECT_EQ(lfg1->baseHeight().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(lfg1->baseHeight().distance().has_value());
	EXPECT_NEAR(lfg1->baseHeight().distance().value(), 3000, margin);
	EXPECT_EQ(lfg1->baseHeight().unit(), metaf::Distance::Unit::FEET);

	EXPECT_TRUE(lfg1->topHeight().isReported());	
	EXPECT_EQ(lfg1->baseHeight().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(lfg1->topHeight().distance().has_value());
	EXPECT_NEAR(lfg1->topHeight().distance().value(), 7000, margin);
	EXPECT_EQ(lfg1->baseHeight().unit(), metaf::Distance::Unit::FEET);

	const auto lfg2 = metaf::LayerForecastGroup::parse("520004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg2.has_value());

	EXPECT_TRUE(lfg2->baseHeight().isReported());	
	EXPECT_EQ(lfg2->baseHeight().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(lfg2->baseHeight().distance().has_value());
	EXPECT_NEAR(lfg2->baseHeight().distance().value(), 0, margin);
	EXPECT_EQ(lfg2->baseHeight().unit(), metaf::Distance::Unit::FEET);

	EXPECT_TRUE(lfg2->topHeight().isReported());	
	EXPECT_EQ(lfg2->baseHeight().modifier(), metaf::Distance::Modifier::NONE);
	ASSERT_TRUE(lfg2->topHeight().distance().has_value());
	EXPECT_NEAR(lfg2->topHeight().distance().value(), 4000, margin);
	EXPECT_EQ(lfg2->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

///////////////////////////////////////////////////////////////////////////////
// Icing groups
// Purpose: to confirm that each type if icing is identified correctly
///////////////////////////////////////////////////////////////////////////////

TEST(LayerForecastGroup, icingTrace) {
	const auto lfg = metaf::LayerForecastGroup::parse("600304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::ICING_TRACE_OR_NONE);
}

TEST(LayerForecastGroup, icingLightMixed) {
	const auto lfg = metaf::LayerForecastGroup::parse("610304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::ICING_LIGHT_MIXED);
}

TEST(LayerForecastGroup, icingLightRimeInCloud) {
	const auto lfg = metaf::LayerForecastGroup::parse("620304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::ICING_LIGHT_RIME_IN_CLOUD);
}

TEST(LayerForecastGroup, icingLightClearInPrecipitation) {
	const auto lfg = metaf::LayerForecastGroup::parse("630304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::ICING_LIGHT_CLEAR_IN_PRECIPITATION);
}

TEST(LayerForecastGroup, icingModerateMixed) {
	const auto lfg = metaf::LayerForecastGroup::parse("640304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::ICING_MODERATE_MIXED);	
}

TEST(LayerForecastGroup, icingModerateRimeInCloud) {
	const auto lfg = metaf::LayerForecastGroup::parse("650304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::ICING_MODERATE_RIME_IN_CLOUD);
}

TEST(LayerForecastGroup, icingModerateClearInPrecipitation) {
	const auto lfg = metaf::LayerForecastGroup::parse("660304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::ICING_MODERATE_CLEAR_IN_PRECIPITATION);
}

TEST(LayerForecastGroup, icingSevereMixed) {
	const auto lfg = metaf::LayerForecastGroup::parse("670304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::ICING_SEVERE_MIXED);
}

TEST(LayerForecastGroup, icingSevereRimeInCloud) {
	const auto lfg = metaf::LayerForecastGroup::parse("680304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::ICING_SEVERE_RIME_IN_CLOUD);
}

TEST(LayerForecastGroup, icingSevereClearInPrecipitation) {
	const auto lfg = metaf::LayerForecastGroup::parse("690304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::ICING_SEVERE_CLEAR_IN_PRECIPITATION);	
}

///////////////////////////////////////////////////////////////////////////////
// Turbulence groups
// Purpose: to confirm that each type if turbulence is identified correctly
///////////////////////////////////////////////////////////////////////////////

TEST(LayerForecastGroup, turbulenceNone) {
	const auto lfg = metaf::LayerForecastGroup::parse("500004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_NONE);
}

TEST(LayerForecastGroup, turbulenceLight) {
	const auto lfg = metaf::LayerForecastGroup::parse("510004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_LIGHT);
}

TEST(LayerForecastGroup, turbulenceModerateInClearAirOccassional) {
	const auto lfg = metaf::LayerForecastGroup::parse("520004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASSIONAL);
}

TEST(LayerForecastGroup, turbulenceModerateInClearAirFrequent) {
	const auto lfg = metaf::LayerForecastGroup::parse("530004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT);
}

TEST(LayerForecastGroup, turbulenceModerateInCloudOccassional) {
	const auto lfg = metaf::LayerForecastGroup::parse("540004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLOUD_OCCASSIONAL);
}

TEST(LayerForecastGroup, turbulenceModerateInCloudFrequent) {
	const auto lfg = metaf::LayerForecastGroup::parse("550004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLOUD_FREQUENT);
}

TEST(LayerForecastGroup, turbulenceSevereInClearAirOccassional) {
	const auto lfg = metaf::LayerForecastGroup::parse("560004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASSIONAL);
}

TEST(LayerForecastGroup, turbulenceSevereInClearAirFrequent) {
	const auto lfg = metaf::LayerForecastGroup::parse("570004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT);
}

TEST(LayerForecastGroup, turbulenceSevereInCloudOccassional) {
	const auto lfg = metaf::LayerForecastGroup::parse("580004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLOUD_OCCASSIONAL);
}

TEST(LayerForecastGroup, turbulenceSevereInCloudFrequent) {
	const auto lfg = metaf::LayerForecastGroup::parse("590004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLOUD_FREQUENT);
}

TEST(LayerForecastGroup, turbulenceExtreme) {
	const auto lfg = metaf::LayerForecastGroup::parse("5X0004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg.has_value());
	EXPECT_EQ(lfg->type(), metaf::LayerForecastGroup::Type::TURBULENCE_EXTREME);
}

///////////////////////////////////////////////////////////////////////////////
// Incorrect groups
// Purpose: to confirm that incorrect groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(LayerForecastGroup, wrongReportPart) {
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("620324", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("620324", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("620324", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("620324", metaf::ReportPart::RMK).has_value());
}

TEST(LayerForecastGroup, wrongFormat) {
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("62", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("6", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("6X0324", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("6/////", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("5/////", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("62////", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("52////", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("62///3", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("62065/", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("6200324", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("62324", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("62032A", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("6203A4", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("620A24", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::LayerForecastGroup::parse("62A324", metaf::ReportPart::TAF).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Appending tests
// Purpose: to confirm that turbulence/icing forecast groups do not append to
// each other and other groups cannot be appended.
///////////////////////////////////////////////////////////////////////////////

TEST(LayerForecastGroup, append) {
	auto lfg1 = metaf::LayerForecastGroup::parse("590004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg1.has_value());

	auto lfg2 = metaf::LayerForecastGroup::parse("620304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg2.has_value());

	EXPECT_EQ(lfg1->append("590004", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(lfg1->append("620304", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(lfg1->append("RMK", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(lfg1->append("TEST", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(lfg2->append("590004", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(lfg2->append("620304", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(lfg2->append("RMK", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(lfg2->append("TEST", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method returns true for all groups
///////////////////////////////////////////////////////////////////////////////

TEST(LayerForecastGroup, isValid) {
	const auto lfg1 = metaf::LayerForecastGroup::parse("590004", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg1.has_value());
	EXPECT_TRUE(lfg1->isValid());

	const auto lfg2 = metaf::LayerForecastGroup::parse("620304", metaf::ReportPart::TAF);
	ASSERT_TRUE(lfg2.has_value());
	EXPECT_TRUE(lfg2->isValid());
}
