/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"


///////////////////////////////////////////////////////////////////////////////
// Parse probability groups
// Purpose: to confirm that probability groups are parsed correctly and 
// malformed groups are not parsed 
///////////////////////////////////////////////////////////////////////////////

TEST(TrendGroup, parseProb30) {
	const auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::PROB);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::PROB_30);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, parseProb40) {
	const auto tg = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::PROB);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::PROB_40);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeUntil().has_value());
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

///////////////////////////////////////////////////////////////////////////////
// Parse trend type groups
// Purpose: to confirm that NOSIG, BECMG, TEMPO, and INTER groups are parsed 
// correctly
///////////////////////////////////////////////////////////////////////////////

TEST(TrendGroup, parseNosig) {
	const auto tg = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::NOSIG);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeUntil().has_value());
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
	EXPECT_FALSE(tg1->timeUntil().has_value());
	EXPECT_FALSE(tg1->timeAt().has_value());

	const auto tg2 = metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->type(), metaf::TrendGroup::Type::BECMG);
	EXPECT_EQ(tg2->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg2->timeFrom().has_value());
	EXPECT_FALSE(tg2->timeUntil().has_value());
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
	EXPECT_FALSE(tg1->timeUntil().has_value());
	EXPECT_FALSE(tg1->timeAt().has_value());

	const auto tg2 = metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->type(), metaf::TrendGroup::Type::TEMPO);
	EXPECT_EQ(tg2->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg2->timeFrom().has_value());
	EXPECT_FALSE(tg2->timeUntil().has_value());
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
	EXPECT_FALSE(tg1->timeUntil().has_value());
	EXPECT_FALSE(tg1->timeAt().has_value());

	const auto tg2 = metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_EQ(tg2->type(), metaf::TrendGroup::Type::INTER);
	EXPECT_EQ(tg2->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg2->timeFrom().has_value());
	EXPECT_FALSE(tg2->timeUntil().has_value());
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

///////////////////////////////////////////////////////////////////////////////
// Parse FMxxxxxx groups
// Purpose: to confirm that FM groups (6 digit time) are parsed correctly and 
// malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

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
	EXPECT_FALSE(tg->timeUntil().has_value());
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

///////////////////////////////////////////////////////////////////////////////
// Parse time span groups
// Purpose: to confirm that time span groups xxxx/xxxx are parsed 
// correctly and malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(TrendGroup, parseTafTimeSpan) {
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
	ASSERT_TRUE(tg1->timeUntil().has_value());
	ASSERT_TRUE(tg1->timeUntil()->day().has_value());
	EXPECT_EQ(tg1->timeUntil()->day().value(), 13u);
	EXPECT_EQ(tg1->timeUntil()->hour(), 3u);
	EXPECT_EQ(tg1->timeUntil()->minute(), 00u);
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
	ASSERT_TRUE(tg2->timeUntil().has_value());
	ASSERT_TRUE(tg2->timeUntil()->day().has_value());
	EXPECT_EQ(tg2->timeUntil()->day().value(), 13u);
	EXPECT_EQ(tg2->timeUntil()->hour(), 3u);
	EXPECT_EQ(tg2->timeUntil()->minute(), 00u);
	EXPECT_FALSE(tg2->timeAt().has_value());
}

TEST(TrendGroup, parseMetarTimeSpan) {
	const auto tg = metaf::TrendGroup::parse("1221/1303", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::TIME_SPAN);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->hour(), 12u);
	EXPECT_EQ(tg->timeFrom()->minute(), 21u);
	ASSERT_TRUE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeUntil()->day().has_value());
	EXPECT_EQ(tg->timeUntil()->hour(), 13u);
	EXPECT_EQ(tg->timeUntil()->minute(), 03u);
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, parseTimeSpanWrongReportPart) {
	static const std::string gs("1221/1303");
	EXPECT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::TAF));
	EXPECT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::HEADER));
	EXPECT_TRUE(metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse(gs, metaf::ReportPart::UNKNOWN));
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

///////////////////////////////////////////////////////////////////////////////
// Parse FM, TL, and AT groups
// Purpose: to confirm that FM, TL, and AT groups (4 digit time) are parsed 
// correctly and malformed groups are not parsed
///////////////////////////////////////////////////////////////////////////////

TEST(TrendGroup, parseTrendTimeFm) {
	static const std::string gs("FM1445");
	const auto tg = metaf::TrendGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::FROM);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->hour(), 14u);
	EXPECT_EQ(tg->timeFrom()->minute(), 45u);
	EXPECT_FALSE(tg->timeUntil().has_value());
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
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::UNTIL);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg->timeFrom().has_value());
	ASSERT_TRUE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeUntil()->day().has_value());
	EXPECT_EQ(tg->timeUntil()->hour(), 3u);
	EXPECT_EQ(tg->timeUntil()->minute(), 0u);
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
	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::AT);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeUntil().has_value());
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
	EXPECT_FALSE(metaf::TrendGroup::parse("AM1530", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT01530", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT530", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT/15:30", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT15:30", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT////", metaf::ReportPart::METAR));
	EXPECT_FALSE(metaf::TrendGroup::parse("AT153A", metaf::ReportPart::METAR));
}

///////////////////////////////////////////////////////////////////////////////
// Empty string
// Purpose: to confirm that empty string is not parsed as TrendGroup
///////////////////////////////////////////////////////////////////////////////

TEST(TrendGroup, parseEmptyString) {
	EXPECT_FALSE(metaf::TrendGroup::parse("", metaf::ReportPart::METAR));
}

///////////////////////////////////////////////////////////////////////////////
// Appending tests: probability
// Purpose: to confirm that groups are appended correctly to the probability
// groups
///////////////////////////////////////////////////////////////////////////////

TEST(TrendGroup, appendProbabilityFollowedByTempo) {
	auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("TEMPO", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::TEMPO);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::PROB_30);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, appendProbabilityFollowedByInter) {
	auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("INTER", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::INTER);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::PROB_30);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, appendProbabilityFollowedByTimeSpan) {
	auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("2523/2601", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::TIME_SPAN);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::PROB_30);
	ASSERT_TRUE(tg->timeFrom().has_value());
	ASSERT_TRUE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->day().value(), 25u);
	EXPECT_EQ(tg->timeFrom()->hour(), 23u);
	EXPECT_EQ(tg->timeFrom()->minute(), 00u);
	ASSERT_TRUE(tg->timeUntil().has_value());
	ASSERT_TRUE(tg->timeUntil()->day().has_value());
	EXPECT_EQ(tg->timeUntil()->day().value(), 26u);
	EXPECT_EQ(tg->timeUntil()->hour(), 01u);
	EXPECT_EQ(tg->timeUntil()->minute(), 00u);
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, appendProbabilityFollowedByOther) {
	auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("FM191445", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("FM1445", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("NOSIG", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("BECMG", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("PROB40", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("FM191445", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::PROB);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::PROB_30);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, appendProbabilityFollowedByKeywordGroup) {
	auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("NSW", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::PROB);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::PROB_30);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Appending tests: trend type
// Purpose: to confirm that groups are appended correctly to the trend type
// groups such as BECMG, TEMPO and INTER
///////////////////////////////////////////////////////////////////////////////

TEST(TrendGroup, appendTrendFollowedByTimeSpan) {
	auto tg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("1221/1303", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::BECMG);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tg->timeFrom().has_value());
	ASSERT_TRUE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->day().value(), 12u);
	EXPECT_EQ(tg->timeFrom()->hour(), 21u);
	EXPECT_EQ(tg->timeFrom()->minute(), 00u);
	ASSERT_TRUE(tg->timeUntil().has_value());
	ASSERT_TRUE(tg->timeUntil()->day().has_value());
	EXPECT_EQ(tg->timeUntil()->day().value(), 13u);
	EXPECT_EQ(tg->timeUntil()->hour(), 03u);
	EXPECT_EQ(tg->timeUntil()->minute(), 00u);
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, appendTrendFollowedByTrendTimeSingle) {
	auto tg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("TL1303", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::BECMG);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg->timeFrom().has_value());
	ASSERT_TRUE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeUntil()->day().has_value());
	EXPECT_EQ(tg->timeUntil()->hour(), 13u);
	EXPECT_EQ(tg->timeUntil()->minute(), 03u);
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, appendTrendFollowedByTrendTimeCompound) {
	auto tg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("FM1221", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(tg->append("TL1303", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::BECMG);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->hour(), 12u);
	EXPECT_EQ(tg->timeFrom()->minute(), 21u);
	ASSERT_TRUE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeUntil()->day().has_value());
	EXPECT_EQ(tg->timeUntil()->hour(), 13u);
	EXPECT_EQ(tg->timeUntil()->minute(), 03u);
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, appendTrendFollowedByOther) {
	auto tg = metaf::TrendGroup::parse("BECMG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("FM191445", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("NOSIG", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("TEMPO", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("PROB40", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::BECMG);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, appendProbTrendFollowedByTimeSpan) {
	auto tg = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("TEMPO", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(tg->append("1221/1303", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::TEMPO);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::PROB_40);
	ASSERT_TRUE(tg->timeFrom().has_value());
	ASSERT_TRUE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->day().value(), 12u);
	EXPECT_EQ(tg->timeFrom()->hour(), 21u);
	EXPECT_EQ(tg->timeFrom()->minute(), 00u);
	ASSERT_TRUE(tg->timeUntil().has_value());
	ASSERT_TRUE(tg->timeUntil()->day().has_value());
	EXPECT_EQ(tg->timeUntil()->day().value(), 13u);
	EXPECT_EQ(tg->timeUntil()->hour(), 03u);
	EXPECT_EQ(tg->timeUntil()->minute(), 00u);
	EXPECT_FALSE(tg->timeAt().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Appending tests: FMxxxxxx and NOSIG
// Purpose: to confirm that no group can be appended do FMxxxxxx and NOSIG
// groups
///////////////////////////////////////////////////////////////////////////////

TEST(TrendGroup, appendFrom) {
	auto tg = metaf::TrendGroup::parse("FM191445", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("FM251959", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("TL1445", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("NOSIG", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("BECMG", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("PROB40", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("2520/2522", metaf::ReportPart::TAF), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::FROM);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tg->timeFrom().has_value());
	ASSERT_TRUE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->day().value(), 19u);
	EXPECT_EQ(tg->timeFrom()->hour(), 14u);
	EXPECT_EQ(tg->timeFrom()->minute(), 45u);
	EXPECT_FALSE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

TEST(TrendGroup, appendNosig) {
	auto tg = metaf::TrendGroup::parse("NOSIG", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("FM191445", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("TL1445", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("NOSIG", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("BECMG", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("PROB40", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append("2520/2522", metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::NOSIG);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Appending tests: time FMxxxx and TLxxxx
// Purpose: to confirm that TLxxxx group can be appended to FMxxxx group, and
// that any other combination of FMxxxx, TLxxxx and ATxxxx groups cannot be 
// appended
///////////////////////////////////////////////////////////////////////////////

TEST(TrendGroup, appendTrendTimeFromUntil) {
	auto tg = metaf::TrendGroup::parse("FM1230", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("TL1445", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::TIME_SPAN);
	ASSERT_TRUE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->hour(), 12u);
	EXPECT_EQ(tg->timeFrom()->minute(), 30u);
	ASSERT_TRUE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeUntil()->day().has_value());
	EXPECT_EQ(tg->timeUntil()->hour(), 14u);
	EXPECT_EQ(tg->timeUntil()->minute(), 45u);
}

TEST(TrendGroup, appendTrendTimeWrongCombinations) {
	const std::string fromStr ("FM1230");
	const std::string untilStr ("TL1445");
	const std::string atStr ("AT1315");

	auto tg = metaf::TrendGroup::parse(fromStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append(atStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tg->append(fromStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tg->type(), metaf::TrendGroup::Type::FROM);
	EXPECT_EQ(tg->probability(), metaf::TrendGroup::Probability::NONE);
	ASSERT_TRUE(tg->timeFrom().has_value());
	EXPECT_FALSE(tg->timeFrom()->day().has_value());
	EXPECT_EQ(tg->timeFrom()->hour(), 12u);
	EXPECT_EQ(tg->timeFrom()->minute(), 30u);
	EXPECT_FALSE(tg->timeUntil().has_value());
	EXPECT_FALSE(tg->timeAt().has_value());

	auto tgUntil = metaf::TrendGroup::parse(untilStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(tgUntil.has_value());

	EXPECT_EQ(tgUntil->append(fromStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tgUntil->append(atStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tgUntil->append(untilStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tgUntil->type(), metaf::TrendGroup::Type::UNTIL);
	EXPECT_EQ(tgUntil->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tgUntil->timeFrom().has_value());
	ASSERT_TRUE(tgUntil->timeUntil().has_value());
	EXPECT_FALSE(tgUntil->timeUntil()->day().has_value());
	EXPECT_EQ(tgUntil->timeUntil()->hour(), 14u);
	EXPECT_EQ(tgUntil->timeUntil()->minute(), 45u);
	EXPECT_FALSE(tgUntil->timeAt().has_value());

	auto tgAt = metaf::TrendGroup::parse(atStr, metaf::ReportPart::METAR);
	ASSERT_TRUE(tgAt.has_value());

	EXPECT_EQ(tgAt->append(untilStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tgAt->append(atStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(tgAt->append(fromStr, metaf::ReportPart::METAR), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(tgAt->type(), metaf::TrendGroup::Type::AT);
	EXPECT_EQ(tgAt->probability(), metaf::TrendGroup::Probability::NONE);
	EXPECT_FALSE(tgAt->timeFrom().has_value());
	EXPECT_FALSE(tgAt->timeUntil().has_value());
	ASSERT_TRUE(tgAt->timeAt().has_value());
	EXPECT_FALSE(tgAt->timeAt()->day().has_value());
	EXPECT_EQ(tgAt->timeAt()->hour(), 13u);
	EXPECT_EQ(tgAt->timeAt()->minute(), 15u);
}

///////////////////////////////////////////////////////////////////////////////
// isTimeSpanGroup() tests
// Purpose: to confirm that groups isTimeSpanGroup() method correctly 
// identifies time span groups xxxx/xxxx.
///////////////////////////////////////////////////////////////////////////////

TEST(TrendGroup, isTimeSpanGroupTrue) {
	const auto tg = metaf::TrendGroup::parse("1221/1303", metaf::ReportPart::HEADER);
	ASSERT_TRUE(tg.has_value());
	EXPECT_TRUE(tg->isTimeSpanGroup());
}

TEST(TrendGroup, isTimeSpanGroupFalseCompoundTrendTime) {
	auto tg = metaf::TrendGroup::parse("FM1230", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("TL1445", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);

	EXPECT_FALSE(tg->isTimeSpanGroup());
}

TEST(TrendGroup, isTimeSpanGroupFalseTrendTypeAndAppendedTimeSpan) {
	const std::string timeSpanStr("2520/2522");

	const auto tgTimeSpan = metaf::TrendGroup::parse(timeSpanStr, metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());
	ASSERT_TRUE(tgTimeSpan->isTimeSpanGroup());

	auto tg = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append(timeSpanStr, metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(tg->isTimeSpanGroup());
}

TEST(TrendGroup, isTimeSpanFalseProbabilityAndAppendedTimeSpan) {
	const std::string timeSpanStr("2520/2522");

	const auto tgTimeSpan = metaf::TrendGroup::parse(timeSpanStr, metaf::ReportPart::TAF);
	ASSERT_TRUE(tgTimeSpan.has_value());
	ASSERT_TRUE(tgTimeSpan->isTimeSpanGroup());

	auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	
	EXPECT_EQ(tg->append(timeSpanStr, metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);
	EXPECT_FALSE(tg->isTimeSpanGroup());
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

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

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

TEST(TrendGroup, isValidTrueProbAndAppendedTrendType) {
	auto tg = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("TEMPO", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(tg->isValid());
}

TEST(TrendGroup, isValidTrueProbAndAppendedTimeSpan) {
	auto tg = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("2520/2522", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(tg->isValid());
}

TEST(TrendGroup, isValidTrueProbAndAppendedTrendTypeAndTimeSpan) {
	auto tg = metaf::TrendGroup::parse("PROB40", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("TEMPO", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(tg->append("2520/2522", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(tg->isValid());
}

TEST(TrendGroup, isValidTrueTrendTypeAndAppendedTimeSpan) {
	auto tg = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());

	EXPECT_EQ(tg->append("2520/2522", metaf::ReportPart::TAF), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(tg->isValid());
}

TEST(TrendGroup, isValidTrueTrendTypeAndAppendedTrendTime) {
	auto tg1 = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());

	EXPECT_EQ(tg1->append("FM1445", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(tg1->isValid());

	auto tg2 = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());

	EXPECT_EQ(tg2->append("TL1530", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(tg2->isValid());

	auto tg3 = metaf::TrendGroup::parse("TEMPO", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());

	EXPECT_EQ(tg3->append("AT1921", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(tg3->isValid());
}

TEST(TrendGroup, isValidFalseInvalidTimeSpan) {
	const auto tg = metaf::TrendGroup::parse("2520/2525", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	ASSERT_TRUE(tg->timeUntil().has_value());
	ASSERT_FALSE(tg->timeUntil()->isValid());
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

TEST(TrendGroup, isValidTrueTrendTimeSingle) {
	const auto tg1 = metaf::TrendGroup::parse("FM1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg1.has_value());
	EXPECT_TRUE(tg1->isValid());
	
	const auto tg2 = metaf::TrendGroup::parse("TL1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg2.has_value());
	EXPECT_TRUE(tg2->isValid());

	const auto tg3 = metaf::TrendGroup::parse("AT1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg3.has_value());
	EXPECT_TRUE(tg3->isValid());
}

TEST(TrendGroup, isValidTrueTrendTimeAppended) {
	auto tg = metaf::TrendGroup::parse("FM1445", metaf::ReportPart::METAR);
	ASSERT_TRUE(tg.has_value());
	EXPECT_EQ(tg->append("TL1530", metaf::ReportPart::METAR), 
		metaf::AppendResult::APPENDED);
	EXPECT_TRUE(tg->isValid());
}

TEST(TrendGroup, isValidFalseProbabilityGroup) {
	const auto tg = metaf::TrendGroup::parse("PROB30", metaf::ReportPart::TAF);
	ASSERT_TRUE(tg.has_value());
	EXPECT_FALSE(tg->isValid());
}
