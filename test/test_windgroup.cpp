/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

TEST(WindGroup, parseKnotsMetar) {
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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, parseKnotsTaf) {
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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, parseMps) {
	const auto wg = metaf::WindGroup::parse("18005MPS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);	
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::METERS_PER_SECOND);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, parseKmh) {
	const auto wg = metaf::WindGroup::parse("18005KMH", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);	
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->eventTime().has_value());
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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, parseWindThreeDigits) {
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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, parseGustsThreeDigits) {
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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, parseWindAndGustsThreeDigits) {
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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, parseVariableDirection) {
	const auto wg = metaf::WindGroup::parse("VRB05KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VARIABLE);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, parseNotReported) {
	const auto wg = metaf::WindGroup::parse("/////KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::NOT_REPORTED);
	EXPECT_FALSE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, parseVariableWindSectorMetar) {
	const auto wg = metaf::WindGroup::parse("180V240", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::VARIABLE_WIND_SECTOR);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
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
}

TEST(WindGroup, parseVariableWindSectorTaf) {
	const auto wg = metaf::WindGroup::parse("180V240", metaf::ReportPart::TAF);
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::VARIABLE_WIND_SECTOR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
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
}

TEST(WindGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::WindGroup::parse("18005KT", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18005KT", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("18005KT", metaf::ReportPart::RMK).has_value());
	
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05035KT", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05035KT", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05035KT", metaf::ReportPart::RMK).has_value());
	
	EXPECT_FALSE(metaf::WindGroup::parse("180V240", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("180V240", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("180V240", metaf::ReportPart::RMK).has_value());
}

TEST(WindGroup, parseWrongFormat) {
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
	EXPECT_FALSE(metaf::WindGroup::parse("WS0020/05035KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS20/05035KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/5035KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/050035KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05135KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05135KTS", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/05A35KT", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::WindGroup::parse("WS020/0500AKT", metaf::ReportPart::METAR).has_value());
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

TEST(WindGroup, calmWindKnots) {
	const auto wg = metaf::WindGroup::parse("00000KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND_CALM);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 0u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, calmWindMps) {
	const auto wg = metaf::WindGroup::parse("00000MPS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND_CALM);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::METERS_PER_SECOND);
	EXPECT_EQ(wg->windSpeed().speed().value(), 0u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, calmWindKmh) {
	const auto wg = metaf::WindGroup::parse("00000KMH", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::SURFACE_WIND_CALM);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	EXPECT_EQ(wg->windSpeed().speed().value(), 0u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

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

TEST(WindGroup, appendWindGroupAndWindSectorGroup) {
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
}

TEST(WindGroup, appendWindGroupAndOther) {
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
	EXPECT_EQ(wg->append("/////KT"), metaf::AppendResult::NOT_APPENDED);

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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
}

TEST(WindGroup, appendWindSectorGroupAndOther) {
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

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::VARIABLE_WIND_SECTOR);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
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
}

TEST(WindGroup, parseWshft) {
	const auto wg = metaf::WindGroup::parse("WSHFT", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHIFT);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
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
	EXPECT_FALSE(metaf::WindGroup::parse("WS", metaf::ReportPart::RMK).has_value());
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
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);

	ASSERT_TRUE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->eventTime()->day().has_value());
	EXPECT_EQ(wg->eventTime()->hour(), reportTime->hour());
	EXPECT_EQ(wg->eventTime()->minute(), 18u);
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
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);

	ASSERT_TRUE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->eventTime()->day().has_value());
	EXPECT_EQ(wg->eventTime()->hour(), 04u);
	EXPECT_EQ(wg->eventTime()->minute(), 15u);
}

TEST(WindGroup, appendWshftAndTimeAndFropa) {
	auto wg = metaf::WindGroup::parse("WSHFT", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("2330", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHIFT_FROPA);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);

	ASSERT_TRUE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->eventTime()->day().has_value());
	EXPECT_EQ(wg->eventTime()->hour(), 23u);
	EXPECT_EQ(wg->eventTime()->minute(), 30u);
}

TEST(WindGroup, appendWshftAndFropa) {
	auto wg = metaf::WindGroup::parse("WSHFT", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());

	EXPECT_EQ(wg->append("FROPA", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::WIND_SHIFT_FROPA);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
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
}

TEST(WindGroup, parsePk) {
	const auto wg = metaf::WindGroup::parse("PK", metaf::ReportPart::RMK);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::INCOMPLETE);
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
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

	EXPECT_EQ(wg->type(), metaf::WindGroup::Type::INCOMPLETE);

	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->windSpeed().isReported());
	EXPECT_FALSE(wg->gustSpeed().isReported());
	EXPECT_FALSE(wg->height().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->eventTime().has_value());
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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);

	ASSERT_TRUE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->eventTime()->day().has_value());
	EXPECT_EQ(wg->eventTime()->hour(), 15u);
	EXPECT_EQ(wg->eventTime()->minute(), 47u);
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
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);

	ASSERT_TRUE(wg->eventTime().has_value());
	EXPECT_FALSE(wg->eventTime()->day().has_value());
	EXPECT_EQ(wg->eventTime()->hour(), reportTime->hour());
	EXPECT_EQ(wg->eventTime()->minute(), 06u);
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

