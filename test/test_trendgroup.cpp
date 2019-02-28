/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(TrendGroup, parseProb30) {
	const auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::NONE);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::PROB_30);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeTill().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, parseProb40) {
	const auto tg = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::NONE);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::PROB_40);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeTill().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, parseProbWrongReportPart) {
	static const std::string gs("PROB40");
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(TrendGroup, parseProbWrongFormat) {
	EXPECT_FALSE(metaf::TrendGroup::parse("PROB", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse("PROB//", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse("PROB50", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse("PROB20", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse("PROB35", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse("PROB100", metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse("PROB5", metaf::ReportPart::TAF));
}

TEST(TrendGroup, parseNosig) {
	const auto tg = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::NOSIG);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeTill().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, parseNosigWrongReportPart) {
	static const std::string gs("NOSIG");
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(TrendGroup, parseBecmg) {
	static const std::string gs("BECMG");

	const auto tg1 = metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->type(), metaf::TrendGroup::Type::BECMG);
	EXPECT_EQ(tg1->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg1->timeFrom().has_value());
	EXPECT_FALSE(tg1->timeTill().has_value());
	EXPECT_FALSE(tg1->timeAt().has_value());

	const auto tg2 = metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->type(), metaf::TrendGroup::Type::BECMG);
	EXPECT_EQ(tg2->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg2->timeFrom().has_value());
	EXPECT_FALSE(tg2->timeTill().has_value());
	EXPECT_FALSE(tg2->timeAt().has_value());
}

TEST(TrendGroup, parseBecmgWrongReportPart) {
	static const std::string gs("BECMG");
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(TrendGroup, parseTempo) {
	static const std::string gs("TEMPO");

	const auto tg1 = metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->type(), metaf::TrendGroup::Type::TEMPO);
	EXPECT_EQ(tg1->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg1->timeFrom().has_value());
	EXPECT_FALSE(tg1->timeTill().has_value());
	EXPECT_FALSE(tg1->timeAt().has_value());

	const auto tg2 = metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->type(), metaf::TrendGroup::Type::TEMPO);
	EXPECT_EQ(tg2->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg2->timeFrom().has_value());
	EXPECT_FALSE(tg2->timeTill().has_value());
	EXPECT_FALSE(tg2->timeAt().has_value());
}

TEST(TrendGroup, parseTempoWrongReportPart) {
	static const std::string gs("TEMPO");
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(TrendGroup, parseInter) {
	static const std::string gs("INTER");

	const auto tg1 = metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->type(), metaf::TrendGroup::Type::INTER);
	EXPECT_EQ(tg1->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg1->timeFrom().has_value());
	EXPECT_FALSE(tg1->timeTill().has_value());
	EXPECT_FALSE(tg1->timeAt().has_value());

	const auto tg2 = metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->type(), metaf::TrendGroup::Type::INTER);
	EXPECT_EQ(tg2->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg2->timeFrom().has_value());
	EXPECT_FALSE(tg2->timeTill().has_value());
	EXPECT_FALSE(tg2->timeAt().has_value());
}

TEST(TrendGroup, parseInterWrongReportPart) {
	static const std::string gs("INTER");
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(TrendGroup, parseWrongFixedGroup) {
	EXPECT_FALSE(metaf::TrendGroup::parse("ZZZZZ", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("NOSIG1", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("TEMP", metaf::ReportPart::METAR));
}

TEST(TrendGroup, parseFm) {
	static const std::string gs("FM191445");

	const auto tg = metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::FROM);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tg->timeFrom().has_value());
	ASSERT_TRUE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->day().value(), 19u);
	EXPECT_EQ(tg->timeFrom()->hour(), 14u);
	EXPECT_EQ(tg->timeFrom()->minute(), 45u);
	EXPECT_FALSE(tg->timeTill().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, parseFmWrongReportPart) {
	static const std::string gs("FM191445");
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(TrendGroup, parseFmWrongFormat) {
	EXPECT_FALSE(metaf::TrendGroup::parse("FM", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("FM91445", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("FM0191445", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("FM//////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("FM19144A", metaf::ReportPart::METAR));
}

TEST(TrendGroup, parseTimeSpan) {
	static const std::string gs("1221/1303");

	const auto tg1 = metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_EQ(tg1->type(), metaf::TrendGroup::Type::TIME_SPAN);
	EXPECT_EQ(tg1->probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tg1->timeFrom().has_value());
	ASSERT_TRUE(tg1->timeFrom()->day().has_value());
	EXPECT_EQ(tg1->timeFrom()->day(), 12u);
	EXPECT_EQ(tg1->timeFrom()->hour(), 21u);
	EXPECT_EQ(tg1->timeFrom()->minute(), 00u);
	ASSERT_TRUE(tg1->timeTill().has_value());
	ASSERT_TRUE(tg1->timeTill()->day().has_value());
	EXPECT_EQ(tg1->timeTill()->day().value(), 13u);
	EXPECT_EQ(tg1->timeTill()->hour(), 3u);
	EXPECT_EQ(tg1->timeTill()->minute(), 00u);
	EXPECT_FALSE(tg1->timeAt().has_value());

	const auto tg2 = metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->type(), metaf::TrendGroup::Type::TIME_SPAN);
	EXPECT_EQ(tg2->probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tg2->timeFrom().has_value());
	ASSERT_TRUE(tg2->timeFrom()->day().has_value());
	EXPECT_EQ(tg2->timeFrom()->day().value(), 12u);
	EXPECT_EQ(tg2->timeFrom()->hour(), 21u);
	EXPECT_EQ(tg2->timeFrom()->minute(), 00u);
	ASSERT_TRUE(tg2->timeTill().has_value());
	ASSERT_TRUE(tg2->timeTill()->day().has_value());
	EXPECT_EQ(tg2->timeTill()->day().value(), 13u);
	EXPECT_EQ(tg2->timeTill()->hour(), 3u);
	EXPECT_EQ(tg2->timeTill()->minute(), 00u);
	EXPECT_FALSE(tg2->timeAt().has_value());
}

TEST(TrendGroup, parseTimeSpanWrongReportPart) {
	static const std::string gs("1221/1303");
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(TrendGroup, parseTimeSpanWrongFormat) {
	EXPECT_FALSE(metaf::TrendGroup::parse("12211303", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("12:21/13:03", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("//21/1303", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("1221///03", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("01221/1303", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("221/1303", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("1221/01303", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("1221/303", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("122A/1303", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("1221/130A", metaf::ReportPart::METAR));
}

TEST(TrendGroup, parseTrendTimeFm) {
	static const std::string gs("FM1445");
	const auto tg = metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::NONE);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->hour(), 14u);
	EXPECT_EQ(tg->timeFrom()->minute(), 45u);
	EXPECT_FALSE(tg->timeTill().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, parseTrendTimeFmWrongReportPart) {
	static const std::string gs("FM1445");
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(TrendGroup, parseTrendTimeTl) {
	static const std::string gs("TL0300");
	const auto tg = metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::NONE);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg->timeFrom().has_value());
	ASSERT_TRUE(tg->timeTill().has_value());
	EXPECT_FALSE(tg->timeTill()->day().has_value());
	EXPECT_EQ(tg->timeTill()->hour(), 3u);
	EXPECT_EQ(tg->timeTill()->minute(), 0u);
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, parseTrendTimeTlWrongReportPart) {
	static const std::string gs("TL0300");
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(TrendGroup, parseTrendTimeAt) {
	static const std::string gs("AT1530");
	const auto tg = metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::NONE);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeTill().has_value());
	ASSERT_TRUE(tg->timeAt().has_value());
	EXPECT_FALSE(tg->timeAt()->day().has_value());
	EXPECT_EQ(tg->timeAt()->hour(), 15u);
	EXPECT_EQ(tg->timeAt()->minute(), 30u);
}

TEST(TrendGroup, parseTrendTimeAtWrongReportPart) {
	static const std::string gs("AT1530");
	ASSERT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::RMK));
}

TEST(TrendGroup, parseTrendTimeWrongFormat) {
	EXPECT_FALSE(metaf::TrendGroup::parse("XT1530", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT01530", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT530", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT/15:30", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT15:30", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT153A", metaf::ReportPart::METAR));
}

TEST(TrendGroup, parseEmptyString) {
	EXPECT_FALSE(metaf::TrendGroup::parse("", metaf::ReportPart::METAR));
}

TEST(TrendGroup, combineProbabilityFollowedByTempo) {
	const auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	const auto tgNext = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgNext.has_value());

	const auto combined = tg->combine(tgNext.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());
	EXPECT_EQ(tgCombined.type(), metaf::TrendGroup::Type::TEMPO);
	EXPECT_EQ(tgCombined.probability(), metaf::TrendGroup::Probability::PROB_30);
	EXPECT_FALSE(tgCombined.timeFrom().has_value());
	EXPECT_FALSE(tgCombined.timeTill().has_value());
	EXPECT_FALSE(tgCombined.timeAt().has_value());
}

TEST(TrendGroup, combineProbabilityFollowedByInter) {
	const auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	const auto tgNext = metaf::TrendGroup::parse("INTER", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgNext.has_value());

	const auto combined = tg->combine(tgNext.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));
	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());

	EXPECT_EQ(tgCombined.type(), metaf::TrendGroup::Type::INTER);
	EXPECT_EQ(tgCombined.probability(), metaf::TrendGroup::Probability::PROB_30);
	EXPECT_FALSE(tgCombined.timeFrom().has_value());
	EXPECT_FALSE(tgCombined.timeTill().has_value());
	EXPECT_FALSE(tgCombined.timeAt().has_value());
}

TEST(TrendGroup, combineProbabilityFollowedByTimeSpan) {
	const auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	const auto tgNext = metaf::TrendGroup::parse("2523/2601", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgNext.has_value());

	const auto combined = tg->combine(tgNext.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));
	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());

	EXPECT_EQ(tgCombined.type(), metaf::TrendGroup::Type::TIME_SPAN);
	EXPECT_EQ(tgCombined.probability(), metaf::TrendGroup::Probability::PROB_30);
	ASSERT_TRUE(tgCombined.timeFrom().has_value());
	ASSERT_TRUE(tgCombined.timeFrom()->day().has_value());
	EXPECT_EQ(tgCombined.timeFrom()->day().value(), 25u);
	EXPECT_EQ(tgCombined.timeFrom()->hour(), 23u);
	EXPECT_EQ(tgCombined.timeFrom()->minute(), 00u);
	ASSERT_TRUE(tgCombined.timeTill().has_value());
	ASSERT_TRUE(tgCombined.timeTill()->day().has_value());
	EXPECT_EQ(tgCombined.timeTill()->day().value(), 26u);
	EXPECT_EQ(tgCombined.timeTill()->hour(), 01u);
	EXPECT_EQ(tgCombined.timeTill()->minute(), 00u);
	EXPECT_FALSE(tgCombined.timeAt().has_value());
}

TEST(TrendGroup, combineProbabilityFollowedByOther) {
	const auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	const auto tgFrom = metaf::TrendGroup::parse("FM191445", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgFrom.has_value());
	EXPECT_FALSE(tg->combine(tgFrom.value()).has_value());

	const auto tgTrendTime = metaf::TrendGroup::parse("FM1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTrendTime.has_value());
	EXPECT_FALSE(tg->combine(tgTrendTime.value()).has_value());

	const auto tgNosig = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgNosig.has_value());
	EXPECT_FALSE(tg->combine(tgNosig.value()).has_value());

	const auto tgBecmg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgBecmg.has_value());
	EXPECT_FALSE(tg->combine(tgBecmg.value()).has_value());

	const auto tgProb = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	EXPECT_FALSE(tg->combine(tgProb.value()).has_value());
}

TEST(TrendGroup, combineProbabilityFollowedByFixedGroup) {
	const auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	const auto fg = metaf::FixedGroup::parse("CAVOK", metaf::ReportPart::TAF);
	ASSERT_TRUE(fg.has_value());
	EXPECT_FALSE(tg->combine(fg.value()).has_value());
}

TEST(TrendGroup, combineTrendFollowedByTimeSpan) {
	const auto tgBecmg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgBecmg.has_value());
	const auto tgTimeSpan = metaf::TrendGroup::parse("1221/1303", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());

	const auto combined = tgBecmg->combine(tgTimeSpan.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));
	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());

	EXPECT_EQ(tgCombined.type(), metaf::TrendGroup::Type::BECMG);
	EXPECT_EQ(tgCombined.probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tgCombined.timeFrom().has_value());
	ASSERT_TRUE(tgCombined.timeFrom()->day().has_value());
	EXPECT_EQ(tgCombined.timeFrom()->day().value(), 12u);
	EXPECT_EQ(tgCombined.timeFrom()->hour(), 21u);
	EXPECT_EQ(tgCombined.timeFrom()->minute(), 00u);
	ASSERT_TRUE(tgCombined.timeTill().has_value());
	ASSERT_TRUE(tgCombined.timeTill()->day().has_value());
	EXPECT_EQ(tgCombined.timeTill()->day().value(), 13u);
	EXPECT_EQ(tgCombined.timeTill()->hour(), 03u);
	EXPECT_EQ(tgCombined.timeTill()->minute(), 00u);
	EXPECT_FALSE(tgCombined.timeAt().has_value());
}

TEST(TrendGroup, combineTrendFollowedByTrendTimeSingle) {
	const auto tgBecmg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgBecmg.has_value());
	const auto tgTime = metaf::TrendGroup::parse("TL1303", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTime.has_value());

	const auto combined = tgBecmg->combine(tgTime.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));
	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());

	EXPECT_EQ(tgCombined.type(), metaf::TrendGroup::Type::BECMG);
	EXPECT_EQ(tgCombined.probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tgCombined.timeFrom().has_value());
	ASSERT_TRUE(tgCombined.timeTill().has_value());
	EXPECT_FALSE(tgCombined.timeTill()->day().has_value());
	EXPECT_EQ(tgCombined.timeTill()->hour(), 13u);
	EXPECT_EQ(tgCombined.timeTill()->minute(), 03u);
	EXPECT_FALSE(tgCombined.timeAt().has_value());
}

TEST(TrendGroup, combineTrendFollowedByTrendTimeCombined) {
	const auto tgBecmg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgBecmg.has_value());
	const auto tgTimeFrom = metaf::TrendGroup::parse("FM1221", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTimeFrom.has_value());
	const auto tgTimeTill = metaf::TrendGroup::parse("TL1303", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTimeFrom.has_value());
	
	const auto combined1 = tgBecmg->combine(tgTimeFrom.value());
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined1.value()));

	const auto combined2 = 
		std::get<metaf::TrendGroup>(combined1.value()).combine(tgTimeTill.value());
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined2.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined2.value());
	EXPECT_EQ(tgCombined.type(), metaf::TrendGroup::Type::BECMG);
	EXPECT_EQ(tgCombined.probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tgCombined.timeFrom().has_value());
	EXPECT_FALSE(tgCombined.timeFrom()->day().has_value());
	EXPECT_EQ(tgCombined.timeFrom()->hour(), 12u);
	EXPECT_EQ(tgCombined.timeFrom()->minute(), 21u);
	ASSERT_TRUE(tgCombined.timeTill().has_value());
	EXPECT_FALSE(tgCombined.timeTill()->day().has_value());
	EXPECT_EQ(tgCombined.timeTill()->hour(), 13u);
	EXPECT_EQ(tgCombined.timeTill()->minute(), 03u);
	EXPECT_FALSE(tgCombined.timeAt().has_value());
}

TEST(TrendGroup, combineTrendFollowedByOther) {
	const auto tg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());

	const auto tgFrom = metaf::TrendGroup::parse("FM191445", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgFrom.has_value());
	EXPECT_FALSE(tg->combine(tgFrom.value()));

	const auto tgNosig = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgNosig.has_value());
	EXPECT_FALSE(tg->combine(tgNosig.value()));

	const auto tgBecmg = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgBecmg.has_value());
	EXPECT_FALSE(tg->combine(tgBecmg.value()));

	const auto tgProb = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	EXPECT_FALSE(tg->combine(tgProb.value()));
}

TEST(TrendGroup, combineProbTrendFollowedByTimeSpan) {
	const auto tgProb = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	const auto tgTempo = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTempo.has_value());
	const auto tgTimeSpan = metaf::TrendGroup::parse("1221/1303", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());

	const auto combined1 = tgProb->combine(tgTempo.value());
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined1.value()));

	const auto combined2 = 
		std::get<metaf::TrendGroup>(combined1.value()).combine(tgTimeSpan.value());
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined2.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined2.value());
	EXPECT_EQ(tgCombined.type(), metaf::TrendGroup::Type::TEMPO);
	EXPECT_EQ(tgCombined.probability(), metaf::TrendGroup::Probability::PROB_40);
	ASSERT_TRUE(tgCombined.timeFrom().has_value());
	ASSERT_TRUE(tgCombined.timeFrom()->day().has_value());
	EXPECT_EQ(tgCombined.timeFrom()->day().value(), 12u);
	EXPECT_EQ(tgCombined.timeFrom()->hour(), 21u);
	EXPECT_EQ(tgCombined.timeFrom()->minute(), 00u);
	ASSERT_TRUE(tgCombined.timeTill().has_value());
	ASSERT_TRUE(tgCombined.timeTill()->day().has_value());
	EXPECT_EQ(tgCombined.timeTill()->day().value(), 13u);
	EXPECT_EQ(tgCombined.timeTill()->hour(), 03u);
	EXPECT_EQ(tgCombined.timeTill()->minute(), 00u);
	EXPECT_FALSE(tgCombined.timeAt().has_value());
}

TEST(TrendGroup, combineProbTrendFollowedByTrendTimeSingle) {
	const auto tgProb = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	const auto tgTempo = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTempo.has_value());
	const auto tgTime = metaf::TrendGroup::parse("TL1303", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTime.has_value());

	const auto combined1 = tgProb->combine(tgTempo.value());
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined1.value()));

	const auto combined2 = 
		std::get<metaf::TrendGroup>(combined1.value()).combine(tgTime.value());
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined2.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined2.value());
	EXPECT_EQ(tgCombined.type(), metaf::TrendGroup::Type::TEMPO);
	EXPECT_EQ(tgCombined.probability(), metaf::TrendGroup::Probability::PROB_40);
	EXPECT_FALSE(tgCombined.timeFrom().has_value());
	ASSERT_TRUE(tgCombined.timeTill().has_value());
	EXPECT_FALSE(tgCombined.timeTill()->day().has_value());
	EXPECT_EQ(tgCombined.timeTill()->hour(), 13u);
	EXPECT_EQ(tgCombined.timeTill()->minute(), 03u);
	EXPECT_FALSE(tgCombined.timeAt().has_value());
}

TEST(TrendGroup, combineProbTrendFollowedByTrendTimeCombined) {
	const auto tgProb = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	const auto tgTempo = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTempo.has_value());
	const auto tgTimeFrom = metaf::TrendGroup::parse("FM1221", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTimeFrom.has_value());
	const auto tgTimeTill = metaf::TrendGroup::parse("TL1303", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTimeFrom.has_value());
	
	const auto combined1 = tgProb->combine(tgTempo.value());
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined1.value()));

	const auto combined2 = 
		std::get<metaf::TrendGroup>(combined1.value()).combine(tgTimeFrom.value());
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined2.value()));

	const auto combined3 = 
		std::get<metaf::TrendGroup>(combined2.value()).combine(tgTimeTill.value());
	ASSERT_TRUE(combined3.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined3.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined3.value());
	EXPECT_EQ(tgCombined.type(), metaf::TrendGroup::Type::TEMPO);
	EXPECT_EQ(tgCombined.probability(), metaf::TrendGroup::Probability::PROB_40);
	ASSERT_TRUE(tgCombined.timeFrom().has_value());
	EXPECT_FALSE(tgCombined.timeFrom()->day().has_value());
	EXPECT_EQ(tgCombined.timeFrom()->hour(), 12u);
	EXPECT_EQ(tgCombined.timeFrom()->minute(), 21u);
	ASSERT_TRUE(tgCombined.timeTill().has_value());
	EXPECT_FALSE(tgCombined.timeTill()->day().has_value());
	EXPECT_EQ(tgCombined.timeTill()->hour(), 13u);
	EXPECT_EQ(tgCombined.timeTill()->minute(), 03u);
	EXPECT_FALSE(tgCombined.timeAt().has_value());
}

TEST(TrendGroup, combineFrom) {
	const auto tg = metaf::TrendGroup::parse("FM191445", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	const auto tgFrom = metaf::TrendGroup::parse("FM251959", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgFrom.has_value());
	EXPECT_FALSE(tg->combine(tgFrom.value()).has_value());

	const auto tgTrendTime = metaf::TrendGroup::parse("TL1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTrendTime.has_value());
	EXPECT_FALSE(tg->combine(tgTrendTime.value()).has_value());

	const auto tgNosig = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgNosig.has_value());
	EXPECT_FALSE(tg->combine(tgNosig.value()));

	const auto tgBecmg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgBecmg.has_value());
	EXPECT_FALSE(tg->combine(tgBecmg.value()));

	const auto tgProb = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	EXPECT_FALSE(tg->combine(tgProb.value()));

	const auto tgTimeSpan = metaf::TrendGroup::parse("2520/2522", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());
	EXPECT_FALSE(tg->combine(tgTimeSpan.value()));
}

TEST(TrendGroup, combineNosig) {
	const auto tg = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());

	const auto tgFrom = metaf::TrendGroup::parse("FM191445", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgFrom.has_value());
	EXPECT_FALSE(tg->combine(tgFrom.value()).has_value());

	const auto tgTrendTime = metaf::TrendGroup::parse("TL1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTrendTime.has_value());
	EXPECT_FALSE(tg->combine(tgTrendTime.value()).has_value());

	const auto tgNosig = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgNosig.has_value());
	EXPECT_FALSE(tg->combine(tgNosig.value()).has_value());

	const auto tgBecmg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgBecmg.has_value());
	EXPECT_FALSE(tg->combine(tgBecmg.value()).has_value());

	const auto tgProb = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	EXPECT_FALSE(tg->combine(tgProb.value()).has_value());

	const auto tgTimeSpan = metaf::TrendGroup::parse("2520/2522", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());
 	EXPECT_FALSE(tg->combine(tgTimeSpan.value()).has_value());
}

TEST(TrendGroup, combineTrendTimeFromTill) {
	const auto tgFrom = metaf::TrendGroup::parse("FM1230", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgFrom.has_value());
	const auto tgTill = metaf::TrendGroup::parse("TL1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTill.has_value());

	const auto combined = tgFrom->combine(tgTill.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());
	ASSERT_TRUE(tgCombined.timeFrom().has_value());
	EXPECT_FALSE(tgCombined.timeFrom()->day().has_value());
	EXPECT_EQ(tgCombined.timeFrom()->hour(), 12u);
	EXPECT_EQ(tgCombined.timeFrom()->minute(), 30u);
	ASSERT_TRUE(tgCombined.timeTill().has_value());
	EXPECT_FALSE(tgCombined.timeTill()->day().has_value());
	EXPECT_EQ(tgCombined.timeTill()->hour(), 14u);
	EXPECT_EQ(tgCombined.timeTill()->minute(), 45u);
}

TEST(TrendGroup, combineTrendTimeTillFrom) {
	const auto tgFrom = metaf::TrendGroup::parse("FM1230", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgFrom.has_value());
	const auto tgTill = metaf::TrendGroup::parse("TL1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTill.has_value());

	const auto combined = tgTill->combine(tgFrom.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());
	ASSERT_TRUE(tgCombined.timeFrom().has_value());
	EXPECT_FALSE(tgCombined.timeFrom()->day().has_value());
	EXPECT_EQ(tgCombined.timeFrom()->hour(), 12u);
	EXPECT_EQ(tgCombined.timeFrom()->minute(), 30u);
	ASSERT_TRUE(tgCombined.timeTill().has_value());
	EXPECT_FALSE(tgCombined.timeTill()->day().has_value());
	EXPECT_EQ(tgCombined.timeTill()->hour(), 14u);
	EXPECT_EQ(tgCombined.timeTill()->minute(), 45u);
}

TEST(TrendGroup, combineTrendTimeWrongCombinations) {
	const auto tgFrom = metaf::TrendGroup::parse("FM1230", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgFrom.has_value());
	const auto tgTill = metaf::TrendGroup::parse("TL1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTill.has_value());
	const auto tgAt = metaf::TrendGroup::parse("AT1315", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgAt.has_value());

	EXPECT_FALSE(tgFrom->combine(tgAt.value()).has_value());
	EXPECT_FALSE(tgTill->combine(tgAt.value()).has_value());

	EXPECT_FALSE(tgAt->combine(tgFrom.value()).has_value());
	EXPECT_FALSE(tgAt->combine(tgTill.value()).has_value());
}

TEST(TrendGroup, isTimeSpanGroupTrue) {
	const auto tg = metaf::TrendGroup::parse("1221/1303", metaf::ReportPart::HEADER);
	ASSERT_TRUE(tg.has_value());
	EXPECT_TRUE(tg->isTimeSpanGroup());
}

TEST(TrendGroup, isTimeSpanGroupFalseCombinedTrendTime) {
	const auto tgFrom = metaf::TrendGroup::parse("FM1230", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgFrom.has_value());
	const auto tgTill = metaf::TrendGroup::parse("TL1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTill.has_value());

	const auto combined = tgFrom->combine(tgTill.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());
	EXPECT_FALSE(tgCombined.isTimeSpanGroup());
}

TEST(TrendGroup, isTimeSpanGroupFalseCombinedTrendTypeAndTimeSpan) {
	const auto tgTempo = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTempo.has_value());
	const auto tgTimeSpan = metaf::TrendGroup::parse("2520/2522", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());
	ASSERT_TRUE(tgTimeSpan->isTimeSpanGroup());

	const auto combined = tgTempo->combine(tgTimeSpan.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());
	EXPECT_FALSE(tgCombined.isTimeSpanGroup());
}

TEST(TrendGroup, isTimeSpanFalseCombinedProbabilityAndTimeSpan) {
	const auto tgProb = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	const auto tgTimeSpan = metaf::TrendGroup::parse("2520/2522", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());

	const auto combined = tgProb->combine(tgTimeSpan.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());
	EXPECT_FALSE(tgCombined.isTimeSpanGroup());
}

TEST(TrendGroup, isTimeSpanFalseOther) {
	const auto tg1 = metaf::TrendGroup::parse("FM191445", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_FALSE(tg1->isTimeSpanGroup());

	const auto tg2 = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_FALSE(tg2->isTimeSpanGroup());
	
	const auto tg3 = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_FALSE(tg3->isTimeSpanGroup());
	
	const auto tg4 = metaf::TrendGroup::parse("INTER", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg4.has_value());
	EXPECT_FALSE(tg4->isTimeSpanGroup());
	
	const auto tg5 = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg5.has_value());
	EXPECT_FALSE(tg5->isTimeSpanGroup());
	
	const auto tg6 = metaf::TrendGroup::parse("FM1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg6.has_value());
	EXPECT_FALSE(tg6->isTimeSpanGroup());
	
	const auto tg7 = metaf::TrendGroup::parse("TL1530", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg7.has_value());
	EXPECT_FALSE(tg7->isTimeSpanGroup());
	
	const auto tg8 = metaf::TrendGroup::parse("AT0019", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg8.has_value());
	EXPECT_FALSE(tg8->isTimeSpanGroup());
}

TEST(TrendGroup, isValidTrueNosigBecmgTempoInter) {
	const auto tg1 = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_TRUE(tg1->isValid());

	const auto tg2 = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_TRUE(tg2->isValid());
	
	const auto tg3 = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_TRUE(tg3->isValid());
	
	const auto tg4 = metaf::TrendGroup::parse("INTER", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg4.has_value());
	EXPECT_TRUE(tg4->isValid());
}

TEST(TrendGroup, isValidTrueFrom) {
	const auto tg = metaf::TrendGroup::parse("FM191445", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_TRUE(tg->isValid());
}

TEST(TrendGroup, isValidTrueTimeSpan) {
	const auto tg = metaf::TrendGroup::parse("2520/2522", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_TRUE(tg->isValid());
}

TEST(TrendGroup, isValidTrueCombinedProbAndTrendType) {
	const auto tgProb = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	const auto tgTempo = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTempo.has_value());

	const auto combined = tgProb->combine(tgTempo.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());
	EXPECT_TRUE(tgCombined.isValid());
}

TEST(TrendGroup, isValidTrueCombinedProbAndTimeSpan) {
	const auto tgProb = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	const auto tgTimeSpan = metaf::TrendGroup::parse("2520/2522", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());

	const auto combined = tgProb->combine(tgTimeSpan.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());
	EXPECT_TRUE(tgCombined.isValid());
}

TEST(TrendGroup, isValidTrueCombinedProbAndTrendTypeAndTimeSpan) {
	const auto tgProb = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgProb.has_value());
	const auto tgTempo = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTempo.has_value());
	const auto tgTimeSpan = metaf::TrendGroup::parse("2520/2522", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());

	const auto combined1 = tgProb->combine(tgTempo.value());
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined1.value()));

	const auto combined2 = 
		std::get<metaf::TrendGroup>(combined1.value()).combine(tgTimeSpan.value());
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined2.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined2.value());
	EXPECT_TRUE(tgCombined.isValid());
}

TEST(TrendGroup, isValidTrueCombinedTrendTypeAndTimeSpan) {
	const auto tgTempo = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTempo.has_value());
	const auto tgTimeSpan = metaf::TrendGroup::parse("2520/2522", metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());

	const auto combined = tgTempo->combine(tgTimeSpan.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));

	const auto tgCombined = std::get<metaf::TrendGroup>(combined.value());
	EXPECT_TRUE(tgCombined.isValid());
}

TEST(TrendGroup, isValidTrueCombinedTrendTypeAndTrendTime) {
	const auto tgTempo = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTempo.has_value());
	const auto tgFrom = metaf::TrendGroup::parse("FM1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgFrom.has_value());
	const auto tgTill = metaf::TrendGroup::parse("TL1530", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgFrom.has_value());
	const auto tgAt = metaf::TrendGroup::parse("FM1921", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgFrom.has_value());

	const auto combined1 = tgTempo->combine(tgFrom.value());
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined1.value()));
	EXPECT_TRUE(std::get<metaf::TrendGroup>(combined1.value()).isValid());

	const auto combined2 = tgTempo->combine(tgTill.value());
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined2.value()));
	EXPECT_TRUE(std::get<metaf::TrendGroup>(combined2.value()).isValid());

	const auto combined3 = tgTempo->combine(tgAt.value());
	ASSERT_TRUE(combined3.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined3.value()));
	EXPECT_TRUE(std::get<metaf::TrendGroup>(combined3.value()).isValid());

	const auto combined4temp = tgTempo->combine(tgFrom.value());
	ASSERT_TRUE(combined4temp.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined4temp.value()));
	ASSERT_TRUE(std::get<metaf::TrendGroup>(combined4temp.value()).isValid());

	const auto combined4 = 
		std::get<metaf::TrendGroup>(combined4temp.value()).combine(tgTill.value());
	ASSERT_TRUE(combined4.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined4.value()));
	EXPECT_TRUE(std::get<metaf::TrendGroup>(combined4.value()).isValid());
}

TEST(TrendGroup, isValidFalseInvalidTimeSpan) {
	const auto tg = metaf::TrendGroup::parse("2520/2525", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	ASSERT_TRUE(tg->timeTill().has_value());
	ASSERT_FALSE(tg->timeTill()->isValid());
	EXPECT_FALSE(tg->isValid());
}

TEST(TrendGroup, isValidFalseInvalidTimeFrom) {
	const auto tg = metaf::TrendGroup::parse("FM321445", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	ASSERT_TRUE(tg->timeFrom().has_value());
	ASSERT_FALSE(tg->timeFrom()->isValid());
	EXPECT_FALSE(tg->isValid());
}

TEST(TrendGroup, isValidFalseInvalidTrendTime) {
	const auto tg = metaf::TrendGroup::parse("AT1465", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	ASSERT_TRUE(tg->timeAt().has_value());
	ASSERT_FALSE(tg->timeAt()->isValid());
	EXPECT_FALSE(tg->isValid());
}

TEST(TrendGroup, isValidFalseTrendTimeSingle) {
	const auto tg1 = metaf::TrendGroup::parse("FM1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_FALSE(tg1->isValid());
	
	const auto tg2 = metaf::TrendGroup::parse("TL1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_FALSE(tg2->isValid());

	const auto tg3 = metaf::TrendGroup::parse("AT1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_FALSE(tg3->isValid());
}

TEST(TrendGroup, isValidFalseTrendTimeCombined) {
	const auto tgFrom = metaf::TrendGroup::parse("FM1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgFrom.has_value());
	const auto tgTill = metaf::TrendGroup::parse("TL1530", metaf::ReportPart::METAR);
	ASSERT_TRUE(tgTill.has_value());

	const auto combined = tgFrom->combine(tgTill.value());
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::TrendGroup>(combined.value()));
	EXPECT_FALSE(std::get<metaf::TrendGroup>(combined.value()).isValid());
}

TEST(TrendGroup, isValidFalseProbabilityGroup) {
	const auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_FALSE(tg->isValid());
}

