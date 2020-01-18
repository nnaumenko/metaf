/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Surface wind (with or without gusts, in differnt units, etc) and variable
// wind sector
///////////////////////////////////////////////////////////////////////////////

TEST(WindGroup, parseSurfaceWindKnotsMetar) {
	const auto wg = metaf::WindGroup::parse("18005KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseSurfaceWindKnotsTaf) {
	const auto wg = metaf::WindGroup::parse("18005KT", metaf::ReportPart::TAF);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseSurfaceWindMps) {
	const auto wg = metaf::WindGroup::parse("18005MPS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);	
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::METERS_PER_SECOND);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseSurfaceWindKmh) {
	const auto wg = metaf::WindGroup::parse("18005KMH", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);	
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseGusts) {
	const auto wg = metaf::WindGroup::parse("18005G10KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	ASSERT_TRUE(wg->gustSpeed().speed().has_value());
	EXPECT_EQ(wg->gustSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->gustSpeed().speed().value(), 10u);
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseSurfaceWindThreeDigits) {
	const auto wg = metaf::WindGroup::parse("180102KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 102u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseSurfaceWindGustsThreeDigits) {
	const auto wg = metaf::WindGroup::parse("18099G105KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 99u);
	ASSERT_TRUE(wg->gustSpeed().speed().has_value());
	EXPECT_EQ(wg->gustSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->gustSpeed().speed().value(), 105u);
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseSurfaceWindWindAndGustsThreeDigits) {
	const auto wg = metaf::WindGroup::parse("180101G105KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 101u);
	ASSERT_TRUE(wg->gustSpeed().speed().has_value());
	EXPECT_EQ(wg->gustSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->gustSpeed().speed().value(), 105u);
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseSurfaceWindVariableDirection) {
	const auto wg = metaf::WindGroup::parse("VRB05KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VARIABLE);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseSurfaceWindNotReported) {
	const auto wg = metaf::WindGroup::parse("/////KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::NOT_REPORTED);
	EXPECT_FALSE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseSurfaceWindWrongReportPart) {
	EXPECT_FALSE(metaf::WindGroup::parse("18005KT", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18005KT", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18005KT", metaf::ReportPart::RMK).has_value());
}

TEST(WindGroup, parseSurfaceWindWrongFormat) {
	EXPECT_FALSE(metaf::WindGroup::parse("", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18105KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("180005KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("1805KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18005G5KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18005G005KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18005KTS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18005", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18005Z", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("A8005KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("1800AKT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("VRB5KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("VRB015KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("VRB5KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("VRB05G8KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("VRB05G025KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("//////KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("////KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("////", metaf::ReportPart::METAR).has_value());
}

TEST(WindGroup, parseVariableWindSectorMetar) {
	const auto wg = metaf::WindGroup::parse("180V240", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::VARIABLE_WIND_SECTOR);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().speed().has_value());
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorBegin().degrees().has_value());
	EXPECT_EQ(wg->varSectorBegin().degrees().value(), 180u);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorEnd().degrees().has_value());
	EXPECT_EQ(wg->varSectorEnd().degrees().value(), 240u);
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseVariableWindSectorTaf) {
	const auto wg = metaf::WindGroup::parse("180V240", metaf::ReportPart::TAF);
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::VARIABLE_WIND_SECTOR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().speed().has_value());
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorBegin().degrees().has_value());
	EXPECT_EQ(wg->varSectorBegin().degrees().value(), 180u);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorEnd().degrees().has_value());
	EXPECT_EQ(wg->varSectorEnd().degrees().value(), 240u);
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseVariableWindSectorWrongReportPart) {
	EXPECT_FALSE(metaf::WindGroup::parse("180V240", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("180V240", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("180V240", metaf::ReportPart::RMK).has_value());
}

TEST(WindGroup, parseVariableWindSectorWrongFormat) {
	EXPECT_FALSE(metaf::WindGroup::parse("180A240", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18AV240", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("180V24A", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("181V240", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("180V241", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("0180V240", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("180V0240", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("///V240", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("180V///", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("///V///", metaf::ReportPart::METAR).has_value());
}

TEST(WindGroup, appendSurfaceWindAndVariableWindSector) {
	auto wg = metaf::WindGroup::parse("18005G10KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("170V190", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), 
		metaf::WindGroup::Type::SURFACE_WIND_WITH_VARIABLE_SECTOR);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	ASSERT_TRUE(wg->gustSpeed().speed().has_value());
	EXPECT_EQ(wg->gustSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->gustSpeed().speed().value(), 10u);
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorBegin().degrees().has_value());
	EXPECT_EQ(wg->varSectorBegin().degrees().value(), 170u);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorEnd().degrees().has_value());
	EXPECT_EQ(wg->varSectorEnd().degrees().value(), 190u);
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, appendSurfaceWindAndOther) {
	auto wg = metaf::WindGroup::parse("18005G10KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("WS020/18025KT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSHIFT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("2119", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("19", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("PK", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WND", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/2119", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/19", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////KT"), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSCONDS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	ASSERT_TRUE(wg->gustSpeed().speed().has_value());
	EXPECT_EQ(wg->gustSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->gustSpeed().speed().value(), 10u);
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, appendVariableWindSectorAndOther) {
	auto wg = metaf::WindGroup::parse("170V190", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("18005G10KT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS020/18025KT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("170V190", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSHIFT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("2119", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("19", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("PK", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WND", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/2119", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/19", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////KT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSCONDS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::VARIABLE_WIND_SECTOR);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().speed().has_value());
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorBegin().degrees().has_value());
	EXPECT_EQ(wg->varSectorBegin().degrees().value(), 170u);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorEnd().degrees().has_value());
	EXPECT_EQ(wg->varSectorEnd().degrees().value(), 190u);
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Wind shear
///////////////////////////////////////////////////////////////////////////////

TEST(WindGroup, parseWindShear) {
	const auto wg = metaf::WindGroup::parse("WS020/05035KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 50u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 35u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_TRUE(wg->height().isReported());
	EXPECT_TRUE(wg->height().isInteger());
	ASSERT_TRUE(wg->height().integer().has_value());
	EXPECT_EQ(wg->height().integer().value(), 2000u);
	EXPECT_EQ(wg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseWindShearWrongReportPart) {
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05035KT", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05035KT", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05035KT", metaf::ReportPart::RMK).has_value());
}

TEST(WindGroup, parseWindShearWrongFormat) {
	EXPECT_FALSE(metaf::WindGroup::parse("WS0020/05035KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS20/05035KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/5035KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/050035KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05135KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05135KTS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05A35KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/0500AKT", metaf::ReportPart::METAR).has_value());
}

TEST(WindGroup, appendWindShearGroupAndOther) {
	auto wg = metaf::WindGroup::parse("WS020/18025KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("18005G10KT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("170V190", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSHIFT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("2119", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("19", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("PK", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WND", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/2119", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/19", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////KT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSCONDS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 25u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_TRUE(wg->height().isReported());
	EXPECT_TRUE(wg->height().isInteger());
	ASSERT_TRUE(wg->height().integer().has_value());
	EXPECT_EQ(wg->height().integer().value(), 2000u);
	EXPECT_EQ(wg->height().unit(), metaf::Distance::Unit::FEET);
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Calm wind
///////////////////////////////////////////////////////////////////////////////

TEST(WindGroup, calmWindKnots) {
	const auto wg = metaf::WindGroup::parse("00000KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND_CALM);
	EXPECT_TRUE(wg->direction().isOmitted());
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 0u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, calmWindMps) {
	const auto wg = metaf::WindGroup::parse("00000MPS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND_CALM);
	EXPECT_TRUE(wg->direction().isOmitted());
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::METERS_PER_SECOND);
	EXPECT_EQ(wg->windSpeed().speed().value(), 0u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, calmWindKmh) {
	const auto wg = metaf::WindGroup::parse("00000KMH", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND_CALM);
	EXPECT_TRUE(wg->direction().isOmitted());
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	EXPECT_EQ(wg->windSpeed().speed().value(), 0u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Wind shear conditions
///////////////////////////////////////////////////////////////////////////////

TEST(WindGroup, parseWsconds) {
	auto wg = metaf::WindGroup::parse("WSCONDS", metaf::ReportPart::TAF);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WSCONDS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseWscondsWrongReportPart) {
	EXPECT_FALSE(metaf::WindGroup::parse("WSCONDS", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WSCONDS", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WSCONDS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WSCONDS", metaf::ReportPart::RMK).has_value());
}

TEST(WindGroup, appendWscondsAndOther) {
	auto wg = metaf::WindGroup::parse("WSCONDS", metaf::ReportPart::TAF);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("18005G10KT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS020/18025KT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("170V190", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSHIFT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("2119", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("19", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("PK", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WND", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/2119", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/19", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////KT", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSCONDS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WSCONDS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Wind data missing
///////////////////////////////////////////////////////////////////////////////

TEST(WindGroup, parseWndMisg) {
	auto wg = metaf::WindGroup::parse("WND", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WND_MISG);
}

TEST(WindGroup, appendOtherToWnd) {
	static const char gs[] = "WND";

	auto fg1 = metaf::WindGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::WindGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::WindGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::WindGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

///////////////////////////////////////////////////////////////////////////////
// Wind shift
///////////////////////////////////////////////////////////////////////////////

TEST(WindGroup, parseWshft) {
	const auto wg = metaf::WindGroup::parse("WSHFT", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHIFT);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseWshftWrongReportPart) {
	EXPECT_FALSE(metaf::WindGroup::parse("WSHFT", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WSHFT", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WSHFT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WSHFT", metaf::ReportPart::TAF).has_value());
}

TEST(WindGroup, parseWshftWrongFormat) {
	EXPECT_FALSE(metaf::WindGroup::parse("WSHIFT", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("SHFT", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WSH", metaf::ReportPart::RMK).has_value());
}

TEST(WindGroup, appendWshftAndTimeMin) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("210530");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WindGroup::parse("WSHFT", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("18", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHIFT);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());

	ASSERT_TRUE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->eventTime()->day().has_value());
	EXPECT_EQ(wg->eventTime()->hour(), reportTime->hour());
	EXPECT_EQ(wg->eventTime()->minute(), 18u);

	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, appendWshftAndTimeMinNoReportTime) {
	const metaf::ReportMetadata reportMetadata;

	auto wg = metaf::WindGroup::parse("WSHFT", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("18", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
}

TEST(WindGroup, appendWshftAndTimeHourMin) {
	auto wg = metaf::WindGroup::parse("WSHFT", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("0415", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHIFT);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());

	ASSERT_TRUE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->eventTime()->day().has_value());
	EXPECT_EQ(wg->eventTime()->hour(), 04u);
	EXPECT_EQ(wg->eventTime()->minute(), 15u);

	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, appendWshftAndTimeAndFropa) {
	auto wg = metaf::WindGroup::parse("WSHFT", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("2330", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHIFT_FROPA);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());

	ASSERT_TRUE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->eventTime()->day().has_value());
	EXPECT_EQ(wg->eventTime()->hour(), 23u);
	EXPECT_EQ(wg->eventTime()->minute(), 30u);

	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, appendWshftAndFropa) {
	auto wg = metaf::WindGroup::parse("WSHFT", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHIFT_FROPA);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());

	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, appendWshftAndOther) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("210530");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WindGroup::parse("WSHFT", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->append("WSHFT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("030", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("3", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("01035", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("135", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("18005KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("18005G10KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("320V340", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("PK", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/2119", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/19", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS020/18025KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSCONDS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
}

///////////////////////////////////////////////////////////////////////////////
// Peak wind
///////////////////////////////////////////////////////////////////////////////

TEST(WindGroup, parsePk) {
	const auto wg = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parsePkWrongReportPart) {
	EXPECT_FALSE(metaf::WindGroup::parse("PK", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("PK", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("PK", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("PK", metaf::ReportPart::TAF).has_value());
}

TEST(WindGroup, appendPkAndWnd) {
	auto wg = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->append("WND", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, appendPkAndOther) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("210530");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg1 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	EXPECT_EQ(wg1->append("WSHFT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg2 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->append("30", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg3 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->append("2345", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg4 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->append("18005KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg5 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->append("18005G10KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg6 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->append("320V340", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg7 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg7.has_value());
	EXPECT_EQ(wg7->append("PK", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg8 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg8.has_value());
	EXPECT_EQ(wg8->append("WSHFT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg9 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg9.has_value());
	EXPECT_EQ(wg9->append("FROPA", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg10 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg10.has_value());
	EXPECT_EQ(wg10->append("31027/2119", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg11 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg11.has_value());
	EXPECT_EQ(wg11->append("31027/19", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg12 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg12.has_value());
	EXPECT_EQ(wg12->append("WS020/18025KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg13 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg13.has_value());
	EXPECT_EQ(wg13->append("/////KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg14 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg14.has_value());
	EXPECT_EQ(wg14->append("WSCONDS", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg15 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg15.has_value());
	EXPECT_EQ(wg15->append("WS", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg16 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg16.has_value());
	EXPECT_EQ(wg16->append("ALL", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg17 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg17.has_value());
	EXPECT_EQ(wg17->append("RWY", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg18 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg18.has_value());
	EXPECT_EQ(wg18->append("R32", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg19 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg19.has_value());
	EXPECT_EQ(wg19->append("RWY32", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(WindGroup, appendPkWndAndOther) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("210530");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg1 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg1->append("WSHFT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg2 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg2->append("30", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg3 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg3->append("2345", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg4 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg4->append("18005KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg5 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg5->append("18005G10KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg6 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg6->append("320V340", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg7 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg7.has_value());
	EXPECT_EQ(wg7->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg7->append("PK", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg8 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg8.has_value());
	EXPECT_EQ(wg8->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg8->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg9 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg9.has_value());
	EXPECT_EQ(wg9->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg9->append("WSHFT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg10 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg10.has_value());
	EXPECT_EQ(wg10->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg10->append("FROPA", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg11 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg11.has_value());
	EXPECT_EQ(wg11->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg11->append("3127/2119", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg12 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg12.has_value());
	EXPECT_EQ(wg12->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg12->append("031027/2119", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg13 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg13.has_value());
	EXPECT_EQ(wg13->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg13->append("31027/02119", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg14 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg14.has_value());
	EXPECT_EQ(wg14->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg14->append("31027/219", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg15 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg15.has_value());
	EXPECT_EQ(wg15->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg15->append("31027/2", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg16 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg16.has_value());
	EXPECT_EQ(wg16->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg16->append("A1027/2119", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg17 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg17.has_value());
	EXPECT_EQ(wg17->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg17->append("31027/A119", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg18 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg18.has_value());
	EXPECT_EQ(wg18->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg18->append("WS020/18025KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg19 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg19.has_value());
	EXPECT_EQ(wg19->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg19->append("/////KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg20 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg20.has_value());
	EXPECT_EQ(wg20->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg20->append("WSCONDS", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg21 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg21.has_value());
	EXPECT_EQ(wg21->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg21->append("WS", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg22 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg22.has_value());
	EXPECT_EQ(wg22->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg22->append("ALL", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg23 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg23.has_value());
	EXPECT_EQ(wg23->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg23->append("RWY", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg24 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg24.has_value());
	EXPECT_EQ(wg24->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg24->append("R32", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg25 = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg25.has_value());
	EXPECT_EQ(wg25->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg25->append("RWY32", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

}

TEST(WindGroup, appendPkWndDirSpeedHourMinute) {
	auto wg = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->append("WND", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append("31076/1547", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 310u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 76u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());

	ASSERT_TRUE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->eventTime()->day().has_value());
	EXPECT_EQ(wg->eventTime()->hour(), 15u);
	EXPECT_EQ(wg->eventTime()->minute(), 47u);

	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, appendPkWndDirSpeedMinute) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("211156");
	ASSERT_TRUE(reportTime.has_value());
	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append("24029/06", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::PEAK_WIND);

	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 240u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 29u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());

	ASSERT_TRUE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->eventTime()->day().has_value());
	EXPECT_EQ(wg->eventTime()->hour(), reportTime->hour());
	EXPECT_EQ(wg->eventTime()->minute(), 06u);

	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, appendPkWndDirSpeedMinuteNoReportData) {
	const metaf::ReportMetadata reportMetadata;

	auto wg = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK, reportMetadata);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append("24029/06", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

///////////////////////////////////////////////////////////////////////////////
// Wind shear at lower levels (all runways)
///////////////////////////////////////////////////////////////////////////////

TEST(WindGroup, parseWs) {
	const auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseWsAll) {
	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->runway().has_value());
}

TEST(WindGroup, parseWsAllRwy) {
	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append("RWY", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_TRUE(wg->runway()->isAllRunways());
}

TEST(WindGroup, appendWsAndOther) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("210530");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg1 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_EQ(wg1->append("WSHFT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg2 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->append("30", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg3 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->append("2345", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg4 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->append("18005KT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg5 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->append("18005G10KT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg6 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->append("320V340", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg7 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg7.has_value());
	EXPECT_EQ(wg7->append("PK", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg8 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg8.has_value());
	EXPECT_EQ(wg8->append("WSHFT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg9 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg9.has_value());
	EXPECT_EQ(wg9->append("FROPA", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg10 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg10.has_value());
	EXPECT_EQ(wg10->append("TEST", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg11 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg11.has_value());
	EXPECT_EQ(wg11->append("LAST", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg12 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg12.has_value());
	EXPECT_EQ(wg12->append("WS020/18025KT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg13 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg13.has_value());
	EXPECT_EQ(wg13->append("/////KT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg14 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg14.has_value());
	EXPECT_EQ(wg14->append("WSCONDS", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg15 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg15.has_value());
	EXPECT_EQ(wg15->append("WS", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg16 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg16.has_value());
	EXPECT_EQ(wg16->append("WND", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg17 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg17.has_value());
	EXPECT_EQ(wg17->append("RWY", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg18 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg18.has_value());
	EXPECT_EQ(wg18->append("MISG", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(WindGroup, appendWsAllAndOther) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("210530");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg1 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	EXPECT_EQ(wg1->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg1->append("WSHFT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg2 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_EQ(wg2->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg2->append("30", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg3 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_EQ(wg3->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg3->append("2345", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg4 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_EQ(wg4->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg4->append("18005KT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg5 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_EQ(wg5->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg5->append("18005G10KT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg6 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_EQ(wg6->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg6->append("320V340", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg7 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg7.has_value());
	EXPECT_EQ(wg7->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg7->append("PK", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg8 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg8.has_value());
	EXPECT_EQ(wg8->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg8->append("WSHFT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg9 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg9.has_value());
	EXPECT_EQ(wg9->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg9->append("FROPA", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg10 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg10.has_value());
	EXPECT_EQ(wg10->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg10->append("TEST", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg11 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg11.has_value());
	EXPECT_EQ(wg11->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg11->append("LAST", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg12 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg12.has_value());
	EXPECT_EQ(wg12->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg12->append("WS020/18025KT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg13 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg13.has_value());
	EXPECT_EQ(wg13->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg13->append("/////KT", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg14 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg14.has_value());
	EXPECT_EQ(wg14->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg14->append("WSCONDS", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg15 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg15.has_value());
	EXPECT_EQ(wg15->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg15->append("WS", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg16 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg16.has_value());
	EXPECT_EQ(wg16->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg16->append("WND", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg17 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg17.has_value());
	EXPECT_EQ(wg17->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg17->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg18 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg18.has_value());
	EXPECT_EQ(wg18->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg18->append("R32", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto wg19 = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR, reportMetadata);
	ASSERT_TRUE(wg19.has_value());
	EXPECT_EQ(wg19->append("ALL", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg19->append("RWY32", metaf::ReportPart::METAR, reportMetadata), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(WindGroup, windShearAppendOtherToWsAll) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("210530");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->append("WSHFT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("030", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("3", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("01035", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("135", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("18005KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("18005G10KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("320V340", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("PK", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/2119", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/19", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS020/18025KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSCONDS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("X32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_TRUE(wg->runway()->isAllRunways());
}

///////////////////////////////////////////////////////////////////////////////
// Wind shear at lower levels (for a runway)
///////////////////////////////////////////////////////////////////////////////

TEST(WindGroup, parseWsR32) {
	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("R32", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());

	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_EQ(wg->runway()->number(), 32u);
	EXPECT_EQ(wg->runway()->designator(), metaf::Runway::Designator::NONE);
}

TEST(WindGroup, parseWsR27C) {
	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("R27C", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_EQ(wg->runway()->number(), 27u);
	EXPECT_EQ(wg->runway()->designator(), metaf::Runway::Designator::CENTER);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_EQ(wg->runway()->number(), 27u);
	EXPECT_EQ(wg->runway()->designator(), metaf::Runway::Designator::CENTER);
}

TEST(WindGroup, parseWsRwy32) {
	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("RWY32", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);

	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_EQ(wg->runway()->number(), 32u);
	EXPECT_EQ(wg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_EQ(wg->runway()->number(), 32u);
	EXPECT_EQ(wg->runway()->designator(), metaf::Runway::Designator::NONE);
}

TEST(WindGroup, parseWsRwy27C) {
	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("RWY27C", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_EQ(wg->runway()->number(), 27u);
	EXPECT_EQ(wg->runway()->designator(), metaf::Runway::Designator::CENTER);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());
	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_EQ(wg->runway()->number(), 27u);
	EXPECT_EQ(wg->runway()->designator(), metaf::Runway::Designator::CENTER);
}

TEST(WindGroup, windShearAppendToWsRwy) {
	const auto reportTime = metaf::MetafTime::fromStringDDHHMM("210530");
	ASSERT_TRUE(reportTime.has_value());

	metaf::ReportMetadata reportMetadata;
	reportMetadata.reportTime =reportTime.value();

	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->append("WSHFT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("030", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("3", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("01035", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("135", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("18005KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("18005G10KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("320V340", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("PK", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WND", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/2119", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("31027/19", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS020/18025KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("/////KT", metaf::ReportPart::RMK, reportMetadata), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WSCONDS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("WS", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("RWY", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("R32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(wg->append("X32", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_EQ(wg->runway()->number(), 32u);
	EXPECT_EQ(wg->runway()->designator(), metaf::Runway::Designator::NONE);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	EXPECT_TRUE(wg->direction().isOmitted());
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_TRUE(wg->varSectorBegin().isOmitted());
	EXPECT_TRUE(wg->varSectorEnd().isOmitted());
	EXPECT_FALSE(wg->eventTime().has_value());

	ASSERT_TRUE(wg->runway().has_value());
	EXPECT_EQ(wg->runway()->number(), 32u);
	EXPECT_EQ(wg->runway()->designator(), metaf::Runway::Designator::NONE);
}

TEST(WindGroup, parseWsWrongReportPart) {
	EXPECT_FALSE(metaf::WindGroup::parse("WS", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS", metaf::ReportPart::RMK).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Misc: isValid()
///////////////////////////////////////////////////////////////////////////////

TEST(WindGroup, isValidTrueSurfaceWind) {
	const auto wg1 = metaf::WindGroup::parse("18005KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_TRUE(wg1->isValid());

	const auto wg2 = metaf::WindGroup::parse("18005G10KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_TRUE(wg2->isValid());

	const auto wg3 = metaf::WindGroup::parse("180105G110KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_TRUE(wg3->isValid());

	const auto wg4 = metaf::WindGroup::parse("/////KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_TRUE(wg4->isValid());

	const auto wg5 = metaf::WindGroup::parse("VRB05KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_TRUE(wg5->isValid());
}

TEST(WindGroup, isValidFalseSurfaceWind) {
	const auto wg1 = metaf::WindGroup::parse("37005KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_FALSE(wg1->isValid());

	const auto wg2 = metaf::WindGroup::parse("37015G05KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_FALSE(wg2->isValid());

	const auto wg3 = metaf::WindGroup::parse("18000G00KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_FALSE(wg3->isValid());
}

TEST(WindGroup, isValidTrueWindShear) {
	const auto wg = metaf::WindGroup::parse("WS020/18005KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->isValid());
}

TEST(WindGroup, isValidFalseWindShear) {
	const auto wg = metaf::WindGroup::parse("WS000/18005KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_FALSE(wg->isValid());
}

TEST(WindGroup, isValidTrueVariableWindSector) {
	const auto wg = metaf::WindGroup::parse("18005KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->isValid());
}

TEST(WindGroup, isValidFalseVariableWindSector) {
	const auto wg1 = metaf::WindGroup::parse("370V010", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_FALSE(wg1->isValid());

	const auto wg2 = metaf::WindGroup::parse("300V370", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_FALSE(wg2->isValid());
}

TEST(WindGroup, isValidTruePeakWind) {
	//TODO
}

TEST(WindGroup, isValidFalsePeakWind) {
	//TODO
}

TEST(WindGroup, isValidTrueWindShift) {
	//TODO
}

TEST(WindGroup, isValidFalseWindShift) {
	//TODO
}

TEST(WindGroup, isValidTrueWsconds) {
	//TODO
}

TEST(WindGroup, isValidWindShearLowerLayersValidRunway) {
	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("RWY32", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(wg->isValid());
}

TEST(WindGroup, isValidWindShearLowerLayersWrongRunway) {
	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("RWY37", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(wg->isValid());
}

TEST(WindGroup, isValidWindShearLowerLayersAllRunways) {
	auto wg = metaf::WindGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("ALL", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append("RWY", metaf::ReportPart::METAR), metaf::AppendResult::APPENDED);
	EXPECT_TRUE(wg->isValid());
}

TEST(WindGroup, isValidFalseWs) {
	//TODO
}

TEST(WindGroup, isValidFalseWsAll) {
	//TODO
}

TEST(WindGroup, isValidFalseWnd) {
	//TODO
}
