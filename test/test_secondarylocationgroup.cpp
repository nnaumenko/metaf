/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(SecondaryLocationGroup, parseWs) {
	const auto wsllg = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(wsllg.has_value());
	EXPECT_FALSE(wsllg->isValid());
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
	EXPECT_FALSE(wsAll.isValid());
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
	EXPECT_TRUE(wsAllRwy.isValid());
	EXPECT_TRUE(wsAllRwy.runway().isAllRunways());
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
	EXPECT_TRUE(wsR32.isValid());
	EXPECT_EQ(wsR32.runway().number(), 32u);
	EXPECT_EQ(wsR32.runway().designator(), metaf::Runway::Designator::NONE);
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
	EXPECT_TRUE(wsR27c.isValid());
	EXPECT_EQ(wsR27c.runway().number(), 27u);
	EXPECT_EQ(wsR27c.runway().designator(), metaf::Runway::Designator::CENTER);
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
	EXPECT_TRUE(wsR32.isValid());
	EXPECT_EQ(wsR32.runway().number(), 32u);
	EXPECT_EQ(wsR32.runway().designator(), metaf::Runway::Designator::NONE);
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
	EXPECT_TRUE(wsR27c.isValid());
	EXPECT_EQ(wsR27c.runway().number(), 27u);
	EXPECT_EQ(wsR27c.runway().designator(), metaf::Runway::Designator::CENTER);
}

TEST(SecondaryLocationGroup, parseInvalidRunway) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto r37 = metaf::PlainTextGroup::parse("R37", metaf::ReportPart::METAR);
	ASSERT_TRUE(r37.has_value());

	const auto combined = ws->combine(r37.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::SecondaryLocationGroup>(combined.value()));
	const auto wsR37 = std::get<metaf::SecondaryLocationGroup>(combined.value());
	EXPECT_FALSE(wsR37.isValid());
	EXPECT_FALSE(wsR37.runway().isValid());
	EXPECT_EQ(wsR37.runway().number(), 37u);
	EXPECT_EQ(wsR37.runway().designator(), metaf::Runway::Designator::NONE);
}

TEST(SecondaryLocationGroup, combineOther) {
	const auto ws = metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::METAR);
	ASSERT_TRUE(ws.has_value());

	const auto x37 = metaf::PlainTextGroup::parse("X37", metaf::ReportPart::METAR);
	ASSERT_TRUE(x37.has_value());

	const auto test = metaf::PlainTextGroup::parse("TEST", metaf::ReportPart::METAR);
	ASSERT_TRUE(test.has_value());

	EXPECT_FALSE(ws->combine(x37.value()).has_value());
	EXPECT_FALSE(ws->combine(test.value()).has_value());
}

TEST(SecondaryLocationGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("WS", metaf::ReportPart::RMK).has_value());
}

TEST(SecondaryLocationGroup, parseOtherParts) {
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("W0", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("ALL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("RWY", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("R32", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::SecondaryLocationGroup::parse("R27C", metaf::ReportPart::METAR).has_value());
}

