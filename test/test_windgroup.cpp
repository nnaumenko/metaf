/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(WindGroup, parseKnotsMetar) {
	const auto wg = metaf::WindGroup::parse("18005KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->windShearHeight().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(WindGroup, parseKnotsTaf) {
	const auto wg = metaf::WindGroup::parse("18005KT", metaf::ReportPart::TAF);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->windShearHeight().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(WindGroup, parseMps) {
	const auto wg = metaf::WindGroup::parse("18005MPS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::METERS_PER_SECOND);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
}

TEST(WindGroup, parseKmh) {
	const auto wg = metaf::WindGroup::parse("18005KMH", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KILOMETERS_PER_HOUR);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
}

TEST(WindGroup, parseGusts) {
	const auto wg = metaf::WindGroup::parse("18005G10KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	ASSERT_TRUE(wg->gustSpeed().speed().has_value());
	EXPECT_EQ(wg->gustSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->gustSpeed().speed().value(), 10u);
	EXPECT_FALSE(wg->windShearHeight().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(WindGroup, parseWindThreeDigits) {
	const auto wg = metaf::WindGroup::parse("180102KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 102u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->windShearHeight().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(WindGroup, parseGustsThreeDigits) {
	const auto wg = metaf::WindGroup::parse("18099G105KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 99u);
	ASSERT_TRUE(wg->gustSpeed().speed().has_value());
	EXPECT_EQ(wg->gustSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->gustSpeed().speed().value(), 105u);
	EXPECT_FALSE(wg->windShearHeight().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(WindGroup, parseWindAndGustsThreeDigits) {
	const auto wg = metaf::WindGroup::parse("180101G105KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 180u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 101u);
	ASSERT_TRUE(wg->gustSpeed().speed().has_value());
	EXPECT_EQ(wg->gustSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->gustSpeed().speed().value(), 105u);
	EXPECT_FALSE(wg->windShearHeight().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(WindGroup, parseVariableDirection) {
	const auto wg = metaf::WindGroup::parse("VRB05KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VARIABLE);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 5u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->windShearHeight().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(WindGroup, parseNotReported) {
	const auto wg = metaf::WindGroup::parse("/////KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::NOT_REPORTED);
	EXPECT_FALSE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->windShearHeight().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(WindGroup, parseWindShear) {
	const auto wg = metaf::WindGroup::parse("WS020/05035KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->direction().degrees().has_value());
	EXPECT_EQ(wg->direction().degrees().value(), 50u);
	ASSERT_TRUE(wg->windSpeed().speed().has_value());
	EXPECT_EQ(wg->windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wg->windSpeed().speed().value(), 35u);
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_TRUE(wg->windShearHeight().isReported());
	EXPECT_TRUE(wg->windShearHeight().isInteger());
	ASSERT_TRUE(wg->windShearHeight().integer().has_value());
	EXPECT_EQ(wg->windShearHeight().integer().value(), 2000u);
	EXPECT_EQ(wg->windShearHeight().unit(), metaf::Distance::Unit::FEET);
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::OMMITTED);
}

TEST(WindGroup, parseVariableWindSectorMetar) {
	const auto wg = metaf::WindGroup::parse("180V240", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->windSpeed().speed().has_value());
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->windShearHeight().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorBegin().degrees().has_value());
	EXPECT_EQ(wg->varSectorBegin().degrees().value(), 180u);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorEnd().degrees().has_value());
	EXPECT_EQ(wg->varSectorEnd().degrees().value(), 240u);
}

TEST(WindGroup, parseVariableWindSectorTaf) {
	const auto wg = metaf::WindGroup::parse("180V240", metaf::ReportPart::TAF);
	ASSERT_TRUE(wg.has_value());
	EXPECT_EQ(wg->direction().status(), metaf::Direction::Status::OMMITTED);
	EXPECT_FALSE(wg->windSpeed().speed().has_value());
	EXPECT_FALSE(wg->gustSpeed().speed().has_value());
	EXPECT_FALSE(wg->windShearHeight().isReported());
	EXPECT_EQ(wg->varSectorBegin().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorBegin().degrees().has_value());
	EXPECT_EQ(wg->varSectorBegin().degrees().value(), 180u);
	EXPECT_EQ(wg->varSectorEnd().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wg->varSectorEnd().degrees().has_value());
	EXPECT_EQ(wg->varSectorEnd().degrees().value(), 240u);
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

TEST(WindGroup, isCalmTrueKnots) {
	const auto wg = metaf::WindGroup::parse("00000KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->isCalm());
}

TEST(WindGroup, isCalmTrueMps) {
	const auto wg = metaf::WindGroup::parse("00000MPS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->isCalm());
}

TEST(WindGroup, isCalmTrueKmh) {
	const auto wg = metaf::WindGroup::parse("00000KMH", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg.has_value());
	EXPECT_TRUE(wg->isCalm());
}

TEST(WindGroup, isCalmFalse) {
	const auto wg1 = metaf::WindGroup::parse("36000KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg1.has_value());
	EXPECT_FALSE(wg1->isCalm());

	const auto wg2 = metaf::WindGroup::parse("01000KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg2.has_value());
	EXPECT_FALSE(wg2->isCalm());

	const auto wg3 = metaf::WindGroup::parse("00001KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg3.has_value());
	EXPECT_FALSE(wg3->isCalm());

	const auto wg4 = metaf::WindGroup::parse("00000G00KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg4.has_value());
	EXPECT_FALSE(wg4->isCalm());

	const auto wg5 = metaf::WindGroup::parse("000V000", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg5.has_value());
	EXPECT_FALSE(wg5->isCalm());

	const auto wg6 = metaf::WindGroup::parse("WS000/00000KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg6.has_value());
	EXPECT_FALSE(wg6->isCalm());

	const auto wg7 = metaf::WindGroup::parse("WS001/00000KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wg7.has_value());
	EXPECT_FALSE(wg7->isCalm());
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

TEST(WindGroup, combineWindGroupAndWindSectorGroup) {
	const auto wgWind = metaf::WindGroup::parse("18005G10KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgWind.has_value());
	const auto wgSector = metaf::WindGroup::parse("170V190", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgSector.has_value());

	const auto combined = wgWind->combine(wgSector.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::WindGroup>(combined.value()));

	const auto wgCombined = std::get<metaf::WindGroup>(combined.value());
	ASSERT_TRUE(wgCombined.direction().degrees().has_value());
	EXPECT_EQ(wgCombined.direction().degrees().value(), 180u);
	ASSERT_TRUE(wgCombined.windSpeed().speed().has_value());
	EXPECT_EQ(wgCombined.windSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wgCombined.windSpeed().speed().value(), 5u);
	ASSERT_TRUE(wgCombined.gustSpeed().speed().has_value());
	EXPECT_EQ(wgCombined.gustSpeed().unit(), metaf::Speed::Unit::KNOTS);
	EXPECT_EQ(wgCombined.gustSpeed().speed().value(), 10u);
	EXPECT_FALSE(wgCombined.windShearHeight().isReported());
	EXPECT_EQ(wgCombined.varSectorBegin().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wgCombined.varSectorBegin().degrees().has_value());
	EXPECT_EQ(wgCombined.varSectorBegin().degrees().value(), 170u);
	EXPECT_EQ(wgCombined.varSectorEnd().status(), metaf::Direction::Status::VALUE_DEGREES);
	ASSERT_TRUE(wgCombined.varSectorEnd().degrees().has_value());
	EXPECT_EQ(wgCombined.varSectorEnd().degrees().value(), 190u);
}

TEST(WindGroup, combineWindGroupAndOther) {
	const auto wgWind = metaf::WindGroup::parse("18005G10KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgWind.has_value());
	const auto wgWindShear = metaf::WindGroup::parse("WS020/18025KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgWindShear.has_value());

	EXPECT_FALSE(wgWind->combine(wgWindShear.value()));
	EXPECT_FALSE(wgWind->combine(wgWind.value()));
}

TEST(WindGroup, combineWindShearGroupAndOther) {
	const auto wgWind = metaf::WindGroup::parse("18005G10KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgWind.has_value());
	const auto wgWindShear = metaf::WindGroup::parse("WS020/18025KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgWindShear.has_value());
	const auto wgSector = metaf::WindGroup::parse("170V190", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgSector.has_value());

	EXPECT_FALSE(wgWindShear->combine(wgWind.value()).has_value());
	EXPECT_FALSE(wgWindShear->combine(wgWindShear.value()).has_value());
	EXPECT_FALSE(wgSector->combine(wgSector.value()).has_value());
}

TEST(WindGroup, combineWindSectorGroupAndOther) {
	const auto wgWind = metaf::WindGroup::parse("18005G10KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgWind.has_value());
	const auto wgWindShear = metaf::WindGroup::parse("WS020/18025KT", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgWindShear.has_value());
	const auto wgSector = metaf::WindGroup::parse("170V190", metaf::ReportPart::METAR);
	ASSERT_TRUE(wgSector.has_value());

	EXPECT_FALSE(wgSector->combine(wgWind.value()).has_value());
	EXPECT_FALSE(wgSector->combine(wgWindShear.value()).has_value());
	EXPECT_FALSE(wgSector->combine(wgSector.value()).has_value());
}
