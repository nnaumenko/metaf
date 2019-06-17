/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(SecondaryLocationGroup, parseWs) {
	const auto slg= metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slg.has_value());
	EXPECT_EQ(slg->type(), metaf::SecondaryLocationGroup::Type::INCOMPLETE);
	EXPECT_EQ(slg->incompleteText(), "WS");
	EXPECT_FALSE(slg->runway().has_value());
	EXPECT_FALSE(slg->direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsAll) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto all = metaf::PlainTextGroup::parse("ALL", metaf::ReportPart::METAR);
	ASSERT_TRUE(all.has_value());

	const auto combined = ws->combine(all.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined.value()));
	const auto wsAll = std::get<metaf::SecondaryLocationGroup>(combined.value());
	EXPECT_EQ(wsAll.type(), metaf::SecondaryLocationGroup::Type::INCOMPLETE);
	EXPECT_EQ(wsAll.incompleteText(), "WS ALL");
	EXPECT_FALSE(wsAll.runway().has_value());
	EXPECT_FALSE(wsAll.direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsAllRwy) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto all = metaf::PlainTextGroup::parse("ALL", metaf::ReportPart::METAR);
	ASSERT_TRUE(all.has_value());

	const auto rwy = metaf::PlainTextGroup::parse("RWY", metaf::ReportPart::METAR);
	ASSERT_TRUE(rwy.has_value());

	const auto combined1 = ws->combine(all.value());
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined1.value()));
	const auto wsAll = std::get<metaf::SecondaryLocationGroup>(combined1.value());

	const auto combined2 = wsAll.combine(rwy.value());
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined2.value()));
	const auto wsAllRwy = std::get<metaf::SecondaryLocationGroup>(combined2.value());
	EXPECT_EQ(wsAllRwy.type(), metaf::SecondaryLocationGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	ASSERT_TRUE(wsAllRwy.runway().has_value());
	EXPECT_TRUE(wsAllRwy.runway()->isAllRunways());
	EXPECT_TRUE(wsAllRwy.incompleteText().empty());
	EXPECT_FALSE(wsAllRwy.direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsR32) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto r32 = metaf::PlainTextGroup::parse("R32", metaf::ReportPart::METAR);
	ASSERT_TRUE(r32.has_value());

	const auto combined = ws->combine(r32.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined.value()));
	const auto wsR32 = std::get<metaf::SecondaryLocationGroup>(combined.value());
	EXPECT_EQ(wsR32.type(), metaf::SecondaryLocationGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS);
	ASSERT_TRUE(wsR32.runway().has_value());
	EXPECT_EQ(wsR32.runway()->number(), 32u);
	EXPECT_EQ(wsR32.runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(wsR32.incompleteText().empty());
	EXPECT_FALSE(wsR32.direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsR27C) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto r27c = metaf::PlainTextGroup::parse("R27C", metaf::ReportPart::METAR);
	ASSERT_TRUE(r27c.has_value());

	const auto combined = ws->combine(r27c.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined.value()));
	const auto wsR27c = std::get<metaf::SecondaryLocationGroup>(combined.value());
	ASSERT_TRUE(wsR27c.runway().has_value());
	EXPECT_EQ(wsR27c.runway()->number(), 27u);
	EXPECT_EQ(wsR27c.runway()->designator(), metaf::Runway::Designator::CENTER);
	EXPECT_TRUE(wsR27c.incompleteText().empty());
	EXPECT_FALSE(wsR27c.direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsRWY32) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto r32 = metaf::PlainTextGroup::parse("RWY32", metaf::ReportPart::METAR);
	ASSERT_TRUE(r32.has_value());

	const auto combined = ws->combine(r32.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined.value()));
	const auto wsR32 = std::get<metaf::SecondaryLocationGroup>(combined.value());
	ASSERT_TRUE(wsR32.runway().has_value());
	EXPECT_EQ(wsR32.runway()->number(), 32u);
	EXPECT_EQ(wsR32.runway()->designator(), metaf::Runway::Designator::NONE);
	EXPECT_TRUE(wsR32.incompleteText().empty());
	EXPECT_FALSE(wsR32.direction().has_value());
}

TEST(SecondaryLocationGroup, parseWsRWY27C) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto r27c = metaf::PlainTextGroup::parse("RWY27C", metaf::ReportPart::METAR);
	ASSERT_TRUE(r27c.has_value());

	const auto combined = ws->combine(r27c.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined.value()));
	const auto wsR27c = std::get<metaf::SecondaryLocationGroup>(combined.value());
	ASSERT_TRUE(wsR27c.runway().has_value());
	EXPECT_EQ(wsR27c.runway()->number(), 27u);
	EXPECT_EQ(wsR27c.runway()->designator(), metaf::Runway::Designator::CENTER);
	EXPECT_TRUE(wsR27c.incompleteText().empty());
	EXPECT_FALSE(wsR27c.direction().has_value());
}

TEST(SecondaryLocationGroup, windShearCombineWithWrongGroupWs) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto combined1 = ws->combine(metaf::PlainTextGroup("X37"));
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(combined1.value()).toString(), "WS X37");

	const auto combined2 = ws->combine(metaf::PlainTextGroup("TEST"));
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(combined2.value()).toString(), "WS TEST");
}

TEST(SecondaryLocationGroup, windShearCombineWithWrongGroupWsAll) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto all = metaf::PlainTextGroup::parse("ALL", metaf::ReportPart::METAR);
	ASSERT_TRUE(all.has_value());

	const auto combined1 = ws->combine(all.value());
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined1.value()));
	const auto wsAll = std::get<metaf::SecondaryLocationGroup>(combined1.value());

	const auto combined2 = wsAll.combine(metaf::PlainTextGroup("R22"));
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(combined2.value()).toString(), "WS ALL R22");

}

TEST(SecondaryLocationGroup, windShearCombineCompleteGroups) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto combined1 = ws->combine(metaf::PlainTextGroup("ALL"));
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined1.value()));
	const auto wsAll = std::get<metaf::SecondaryLocationGroup>(combined1.value());

	const auto combined2 = wsAll.combine(metaf::PlainTextGroup("RWY"));
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined2.value()));
	const auto wsAllRwy = std::get<metaf::SecondaryLocationGroup>(combined2.value());

	EXPECT_FALSE(wsAllRwy.combine(metaf::PlainTextGroup("R32")));
	EXPECT_FALSE(wsAllRwy.combine(metaf::PlainTextGroup("RWY32")));
	EXPECT_FALSE(wsAllRwy.combine(metaf::PlainTextGroup("RWY")));

	const auto combined3 = ws->combine(metaf::PlainTextGroup("R32"));
	ASSERT_TRUE(combined3.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined3.value()));
	const auto wsR32 = std::get<metaf::SecondaryLocationGroup>(combined3.value());

	EXPECT_FALSE(wsR32.combine(metaf::PlainTextGroup("R32")));
	EXPECT_FALSE(wsR32.combine(metaf::PlainTextGroup("RWY32")));
	EXPECT_FALSE(wsR32.combine(metaf::PlainTextGroup("RWY")));
}

TEST(SecondaryLocationGroup, parseWindShearWrongReportPart) {
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::RMK).has_value());
}

TEST(SecondaryLocationGroup, parseWindShearWrongFormat) {
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("W0", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("ALL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("RWY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("R32", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("R27C", metaf::ReportPart::METAR).has_value());
}

TEST(SecondaryLocationGroup, isValidValidRunway) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto combined = ws->combine(metaf::PlainTextGroup("RWY32"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined.value()));
	const auto wsR32 = std::get<metaf::SecondaryLocationGroup>(combined.value());

	EXPECT_TRUE(wsR32.isValid());
}

TEST(SecondaryLocationGroup, isValidWindShearWrongRunway) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto combined = ws->combine(metaf::PlainTextGroup("R37"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined.value()));
	const auto wsR37 = std::get<metaf::SecondaryLocationGroup>(combined.value());
	EXPECT_FALSE(wsR37.isValid());
	EXPECT_FALSE(wsR37.runway()->isValid());
	EXPECT_EQ(wsR37.runway()->number(), 37u);
	EXPECT_EQ(wsR37.runway()->designator(), metaf::Runway::Designator::NONE);
}



TEST(SecondaryLocationGroup, isValidWindShearAllRunways) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto combined1 = ws->combine(metaf::PlainTextGroup("ALL"));
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined1.value()));
	const auto wsAll = std::get<metaf::SecondaryLocationGroup>(combined1.value());

	const auto combined2 = wsAll.combine(metaf::PlainTextGroup("RWY"));
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined2.value()));
	const auto wsAllRwy = std::get<metaf::SecondaryLocationGroup>(combined2.value());

	EXPECT_TRUE(wsAllRwy.isValid());
}

TEST(SecondaryLocationGroup, isValidWindShearIncomplete) {
	const auto slgWs= metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(slgWs.has_value());
	ASSERT_EQ(slgWs->type(), metaf::SecondaryLocationGroup::Type::INCOMPLETE);
	EXPECT_FALSE(slgWs->isValid());

	const auto combined = slgWs->combine(metaf::PlainTextGroup("ALL"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined.value()));
	const auto slgWsAll = std::get<metaf::SecondaryLocationGroup>(combined.value());
	ASSERT_EQ(slgWsAll.type(), metaf::SecondaryLocationGroup::Type::INCOMPLETE);
	EXPECT_FALSE(slgWsAll.isValid());
}
