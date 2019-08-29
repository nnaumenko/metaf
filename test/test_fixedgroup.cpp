/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(FixedGroup, parseMetar) {
	static const char gs[] = "METAR";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::FixedGroup::Type::METAR);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseSpeci) {
	static const char gs[] = "SPECI";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::FixedGroup::Type::SPECI);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseTaf) {
	static const char gs[] = "TAF";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::FixedGroup::Type::TAF);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseAmd) {
	static const char gs[] = "AMD";
	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::FixedGroup::Type::AMD);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseNil) {
	static const char gs[] = "NIL";
	static const auto type = metaf::FixedGroup::Type::NIL;

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseCnl) {
	static const char gs[] = "CNL";
	static const auto type = metaf::FixedGroup::Type::CNL;

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseCor) {
	static const char gs[] = "COR";
	static const auto type = metaf::FixedGroup::Type::COR;

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseAuto) {
	static const char gs[] = "AUTO";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::FixedGroup::Type::AUTO);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseSnoclo) {
	static const char gs[] = "SNOCLO";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::FixedGroup::Type::R_SNOCLO);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseRsnoclo) {
	static const char gs[] = "R/SNOCLO";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::FixedGroup::Type::R_SNOCLO);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseCavok) {
	static const char gs[] = "CAVOK";
	static const auto type = metaf::FixedGroup::Type::CAVOK;

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseNsw) {
	static const char gs[] = "NSW";
	static const auto type = metaf::FixedGroup::Type::NSW;

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseRmk) {
	static const char gs[] = "RMK";
	static const auto type = metaf::FixedGroup::Type::RMK;

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseMaintenanceIndicator) {
	static const char gs[] = "$";
	static const auto type = metaf::FixedGroup::Type::MAINTENANCE_INDICATOR;

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->type(), type);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->type(), type);

	auto fg5 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg5.has_value());
	EXPECT_EQ(fg5->type(), type);

}

TEST(FixedGroup, parseWsconds) {
	static const char gs[] = "WSCONDS";
	static const auto type = metaf::FixedGroup::Type::WSCONDS;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseAo1) {
	static const char gs[] = "AO1";
	static const auto type = metaf::FixedGroup::Type::AO1;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseAo2) {
	static const char gs[] = "AO2";
	static const auto type = metaf::FixedGroup::Type::AO2;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseNospeci) {
	static const char gs[] = "NOSPECI";
	static const auto type = metaf::FixedGroup::Type::NOSPECI;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseRvrno) {
	static const char gs[] = "RVRNO";
	static const auto type = metaf::FixedGroup::Type::RVRNO;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parsePwino) {
	static const char gs[] = "PWINO";
	static const auto type = metaf::FixedGroup::Type::PWINO;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parsePno) {
	static const char gs[] = "PNO";
	static const auto type = metaf::FixedGroup::Type::PNO;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseFzrano) {
	static const char gs[] = "FZRANO";
	static const auto type = metaf::FixedGroup::Type::FZRANO;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseTsno) {
	static const char gs[] = "TSNO";
	static const auto type = metaf::FixedGroup::Type::TSNO;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseSlpno) {
	static const char gs[] = "SLPNO";
	static const auto type = metaf::FixedGroup::Type::SLPNO;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseFroin) {
	static const char gs[] = "FROIN";
	static const auto type = metaf::FixedGroup::Type::FROIN;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseMisg) {
	EXPECT_FALSE(metaf::FixedGroup::parse("MISG", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("MISG", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("MISG", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("MISG", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("MISG", metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseTempo) {
	EXPECT_FALSE(metaf::FixedGroup::parse("TEMPO", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("TEMPO", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("TEMPO", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("TEMPO", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("TEMPO", metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseUnavbl) {
	EXPECT_FALSE(metaf::FixedGroup::parse("UNAVBL", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("UNAVBL", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("UNAVBL", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("UNAVBL", metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("UNAVBL", metaf::ReportPart::RMK).has_value());
}

TEST(FixedGroup, parseCld) {
	static const char gs[] = "CLD";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseCldMisg) {
	static const char gs[] = "CLD";
	static const auto type = metaf::FixedGroup::Type::CLD_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup("MISG"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	EXPECT_EQ(fgCombined.type(), type);
}

TEST(FixedGroup, parseCldAndOther) {
	static const char gs[] = "CLD";

	const auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("MSSG"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "CLD MSSG");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "CLD 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "CLD ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parseIcg) {
	static const char gs[] = "ICG";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseIcgMisg) {
	static const char gs[] = "ICG";
	static const auto type = metaf::FixedGroup::Type::ICG_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup("MISG"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	EXPECT_EQ(fgCombined.type(), type);
}

TEST(FixedGroup, parseIcgAndOther) {
	static const char gs[] = "ICG";

	const auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("MSSG"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "ICG MSSG");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "ICG 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "ICG ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parsePcpn) {
	static const char gs[] = "PCPN";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parsePcpnMisg) {
	static const char gs[] = "PCPN";
	static const auto type = metaf::FixedGroup::Type::PCPN_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup("MISG"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	EXPECT_EQ(fgCombined.type(), type);
}

TEST(FixedGroup, parsePcpnAndOther) {
	static const char gs[] = "PCPN";

	const auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("MSSG"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "PCPN MSSG");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "PCPN 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "PCPN ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parsePres) {
	static const char gs[] = "PRES";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parsePresMisg) {
	static const char gs[] = "PRES";
	static const auto type = metaf::FixedGroup::Type::PRES_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup("MISG"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	EXPECT_EQ(fgCombined.type(), type);
}

TEST(FixedGroup, parsePresAndOther) {
	static const char gs[] = "PRES";

	const auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("MSSG"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "PRES MSSG");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "PRES 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "PRES ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parseRvr) {
	static const char gs[] = "RVR";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseRvrMisg) {
	static const char gs[] = "RVR";
	static const auto type = metaf::FixedGroup::Type::RVR_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup("MISG"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	EXPECT_EQ(fgCombined.type(), type);
}

TEST(FixedGroup, parseRvrAndOther) {
	static const char gs[] = "RVR";

	const auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("MSSG"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "RVR MSSG");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "RVR 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "RVR ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parseT) {
	static const char gs[] = "T";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseTMisg) {
	static const char gs[] = "T";
	static const auto type = metaf::FixedGroup::Type::T_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup("MISG"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	EXPECT_EQ(fgCombined.type(), type);
}

TEST(FixedGroup, parseTAndOther) {
	static const char gs[] = "T";

	const auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("MSSG"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "T MSSG");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "T 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "T ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parseTd) {
	static const char gs[] = "TD";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseTdMisg) {
	static const char gs[] = "TD";
	static const auto type = metaf::FixedGroup::Type::TD_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup("MISG"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	EXPECT_EQ(fgCombined.type(), type);
}

TEST(FixedGroup, parseTdAndOther) {
	static const char gs[] = "TD";

	const auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("MSSG"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "TD MSSG");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "TD 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "TD ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parseVis) {
	static const char gs[] = "VIS";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseVisMisg) {
	static const char gs[] = "VIS";
	static const auto type = metaf::FixedGroup::Type::VIS_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup("MISG"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	EXPECT_EQ(fgCombined.type(), type);
}

TEST(FixedGroup, parseVisAndOther) {
	static const char gs[] = "VIS";

	const auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("MSSG"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "VIS MSSG");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "VIS 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "VIS ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parseWnd) {
	static const char gs[] = "WND";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseWndMisg) {
	static const char gs[] = "WND";
	static const auto type = metaf::FixedGroup::Type::WND_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup("MISG"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	EXPECT_EQ(fgCombined.type(), type);
}

TEST(FixedGroup, parseWndAndOther) {
	static const char gs[] = "WND";

	const auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("MSSG"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "WND MSSG");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "WND 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "WND ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parseWx) {
	static const char gs[] = "WX";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseWxMisg) {
	static const char gs[] = "WX";
	static const auto type = metaf::FixedGroup::Type::WX_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup("MISG"));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	EXPECT_EQ(fgCombined.type(), type);
}

TEST(FixedGroup, parseWxAndOther) {
	static const char gs[] = "WX";

	const auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("MSSG"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "WX MSSG");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "WX 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "WX ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parseTsLtng) {
	static const char gs[] = "TS/LTNG";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	ASSERT_EQ(fg->type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fg->incompleteText(), std::string(gs));	

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseTsLtngTempo) {
	static const char gs1[] = "TS/LTNG";
	static const char gs2[] = "TEMPO";

	auto fg = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup(gs2));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	ASSERT_EQ(fgCombined.type(), metaf::FixedGroup::Type::INCOMPLETE);
	ASSERT_EQ(fgCombined.incompleteText(), "TS/LTNG TEMPO"); 
}

TEST(FixedGroup, parseTsLtngAndOther) {
	static const char gs1[] = "TS/LTNG";

	auto fg = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup("TEMP"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "TS/LTNG TEMP");

	const auto combined2 = fg->combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "TS/LTNG 4000");

	const auto combined3 = fg->combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "TS/LTNG ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parseTsLtngTempoUnavbl) {
	static const char gs1[] = "TS/LTNG";
	static const char gs2[] = "TEMPO";
	static const char gs3[] = "UNAVBL";
	static const auto type = metaf::FixedGroup::Type::TS_LTNG_TEMPO_UNAVBL;

	auto fg = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined1 = fg->combine(metaf::PlainTextGroup(gs2));
	ASSERT_TRUE(combined1.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined1.value()));

	const auto fgCombined1 = std::get<metaf::FixedGroup>(combined1.value());

	const auto combined2 = fgCombined1.combine(metaf::PlainTextGroup(gs3));
	ASSERT_TRUE(combined2.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined2.value()));

	const auto fgCombined2 = std::get<metaf::FixedGroup>(combined2.value());

	ASSERT_EQ(fgCombined2.type(), type);
}

TEST(FixedGroup, parseTsLtngTempoAndOther) {
	static const char gs1[] = "TS/LTNG";
	static const char gs2[] = "TEMPO";

	auto fg = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	const auto combined = fg->combine(metaf::PlainTextGroup(gs2));
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::FixedGroup>(combined.value()));

	const auto fgCombined = std::get<metaf::FixedGroup>(combined.value());

	const auto combined1 = fgCombined.combine(metaf::PlainTextGroup("TEMPO"));
    ASSERT_TRUE(combined1.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined1.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined1.value()).toString(), "TS/LTNG TEMPO TEMPO");

	const auto combined2 = fgCombined.combine(metaf::PlainTextGroup("4000"));
    ASSERT_TRUE(combined2.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined2.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined2.value()).toString(), "TS/LTNG TEMPO 4000");

	const auto combined3 = fgCombined.combine(metaf::PlainTextGroup("////"));	
    ASSERT_TRUE(combined3.has_value());
	EXPECT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined3.value()));
	EXPECT_EQ(std::get<metaf::PlainTextGroup>(
		combined3.value()).toString(), "TS/LTNG TEMPO ////");

	EXPECT_FALSE(fg->combine(fg.value()).has_value());
}

TEST(FixedGroup, parseOther) {
	EXPECT_FALSE(metaf::FixedGroup::parse("", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("METAF", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("META", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("MMETAR", metaf::ReportPart::HEADER).has_value());
}

TEST(FixedGroup, isValid) {
	const auto fg1 = metaf::FixedGroup::parse("METAR", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_TRUE(fg1->isValid());
	
	const auto fg2 = metaf::FixedGroup::parse("SPECI", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_TRUE(fg2->isValid());
	
	const auto fg3 = metaf::FixedGroup::parse("TAF", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_TRUE(fg3->isValid());

	const auto fg4 = metaf::FixedGroup::parse("AMD", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_TRUE(fg4->isValid());

	const auto fg5 = metaf::FixedGroup::parse("NIL", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg5.has_value());
	EXPECT_TRUE(fg5->isValid());

	const auto fg6 = metaf::FixedGroup::parse("CNL", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg6.has_value());
	EXPECT_TRUE(fg6->isValid());

	const auto fg7 = metaf::FixedGroup::parse("COR", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg7.has_value());
	EXPECT_TRUE(fg7->isValid());
	
	const auto fg8 = metaf::FixedGroup::parse("AUTO", metaf::ReportPart::METAR);
	ASSERT_TRUE(fg8.has_value());
	EXPECT_TRUE(fg8->isValid());
	
	const auto fg9 = metaf::FixedGroup::parse("R/SNOCLO", metaf::ReportPart::METAR);
	ASSERT_TRUE(fg9.has_value());
	EXPECT_TRUE(fg9->isValid());

	const auto fg10 = metaf::FixedGroup::parse("CAVOK", metaf::ReportPart::METAR);
	ASSERT_TRUE(fg10.has_value());
	EXPECT_TRUE(fg10->isValid());

	const auto fg11 = metaf::FixedGroup::parse("NSW", metaf::ReportPart::METAR);
	ASSERT_TRUE(fg11.has_value());
	EXPECT_TRUE(fg11->isValid());

	const auto fg12 = metaf::FixedGroup::parse("RMK", metaf::ReportPart::METAR);
	ASSERT_TRUE(fg12.has_value());
	EXPECT_TRUE(fg12->isValid());

	const auto fg13 = metaf::FixedGroup::parse("WSCONDS", metaf::ReportPart::TAF);
	ASSERT_TRUE(fg13.has_value());
	EXPECT_TRUE(fg13->isValid());

	const auto fg14 = metaf::FixedGroup::parse("AO1", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg14.has_value());
	EXPECT_TRUE(fg14->isValid());

	const auto fg15 = metaf::FixedGroup::parse("AO2", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg15.has_value());
	EXPECT_TRUE(fg15->isValid());

	const auto fg16 = metaf::FixedGroup::parse("NOSPECI", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg16.has_value());
	EXPECT_TRUE(fg16->isValid());

	const auto fg19 = metaf::FixedGroup::parse("RVRNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg19.has_value());
	EXPECT_TRUE(fg19->isValid());

	const auto fg20 = metaf::FixedGroup::parse("PWINO", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg20.has_value());
	EXPECT_TRUE(fg20->isValid());

	const auto fg21 = metaf::FixedGroup::parse("PNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg21.has_value());
	EXPECT_TRUE(fg21->isValid());

	const auto fg22 = metaf::FixedGroup::parse("FZRANO", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg22.has_value());
	EXPECT_TRUE(fg22->isValid());

	const auto fg23 = metaf::FixedGroup::parse("TSNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg23.has_value());
	EXPECT_TRUE(fg23->isValid());

	const auto fg24 = metaf::FixedGroup::parse("SLPNO", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg24.has_value());
	EXPECT_TRUE(fg24->isValid());

	const auto fg25 = metaf::FixedGroup::parse("FROIN", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg25.has_value());
	EXPECT_TRUE(fg25->isValid());
}
