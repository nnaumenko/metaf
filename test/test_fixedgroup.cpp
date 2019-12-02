/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

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

TEST(FixedGroup, parseAo1a) {
	static const char gs[] = "AO1A";
	static const auto type = metaf::FixedGroup::Type::AO1A;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseAo2a) {
	static const char gs[] = "AO2A";
	static const auto type = metaf::FixedGroup::Type::AO2A;

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

TEST(FixedGroup, parseCldMisg) {
	static const char gs[] = "CLD";
	static const auto type = metaf::FixedGroup::Type::CLD_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parseCldAndOther) {
	static const char gs[] = "CLD";

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parseIcgMisg) {
	static const char gs[] = "ICG";
	static const auto type = metaf::FixedGroup::Type::ICG_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parseIcgAndOther) {
	static const char gs[] = "ICG";

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parsePcpnMisg) {
	static const char gs[] = "PCPN";
	static const auto type = metaf::FixedGroup::Type::PCPN_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parsePcpnAndOther) {
	static const char gs[] = "PCPN";

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parsePresMisg) {
	static const char gs[] = "PRES";
	static const auto type = metaf::FixedGroup::Type::PRES_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parsePresAndOther) {
	static const char gs[] = "PRES";

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parseRvrMisg) {
	static const char gs[] = "RVR";
	static const auto type = metaf::FixedGroup::Type::RVR_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parseRvrAndOther) {
	static const char gs[] = "RVR";

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parseTMisg) {
	static const char gs[] = "T";
	static const auto type = metaf::FixedGroup::Type::T_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parseTAndOther) {
	static const char gs[] = "T";

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parseTdMisg) {
	static const char gs[] = "TD";
	static const auto type = metaf::FixedGroup::Type::TD_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parseTdAndOther) {
	static const char gs[] = "TD";

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parseVisMisg) {
	static const char gs[] = "VIS";
	static const auto type = metaf::FixedGroup::Type::VIS_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parseVisAndOther) {
	static const char gs[] = "VIS";

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parseWndMisg) {
	static const char gs[] = "WND";
	static const auto type = metaf::FixedGroup::Type::WND_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parseWndAndOther) {
	static const char gs[] = "WND";

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parseWxMisg) {
	static const char gs[] = "WX";
	static const auto type = metaf::FixedGroup::Type::WX_MISG;

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->append("MISG", metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parseWxAndOther) {
	static const char gs[] = "WX";

	auto fg1 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("MSSG", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs, metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parseTsLtng) {
	static const char gs[] = "TS/LTNG";

	auto fg = metaf::FixedGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(FixedGroup, parseTsLtngTempo) {
	static const char gs1[] = "TS/LTNG";
	static const char gs2[] = "TEMPO";
	static const char gs3[] = "UNAVBL";
	static const auto type = metaf::FixedGroup::Type::TS_LTNG_TEMPO_UNAVBL;

	auto fg = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());

	EXPECT_EQ(fg->append(gs2, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);

	EXPECT_EQ(fg->append(gs3, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg->type(), type);
}

TEST(FixedGroup, parseTsLtngAndOther) {
	static const char gs1[] = "TS/LTNG";

	auto fg1 = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append("TEMP"), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg4 = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->append(gs1), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parseTsLtngTempoAndOther) {
	static const char gs1[] = "TS/LTNG";
	static const char gs2[] = "TEMPO";

	auto fg1 = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->append(gs2, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg1->append("TEMPO"), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg2 = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->append(gs2, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg2->append("4000", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);

	auto fg3 = metaf::FixedGroup::parse(gs1, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->append(gs2, metaf::ReportPart::RMK), 
		metaf::AppendResult::APPENDED);
	EXPECT_EQ(fg3->append("////", metaf::ReportPart::RMK), 
		metaf::AppendResult::GROUP_INVALIDATED);
}

TEST(FixedGroup, parseOther) {
	EXPECT_FALSE(metaf::FixedGroup::parse("", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("METAF", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("META", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::FixedGroup::parse("MMETAR", metaf::ReportPart::HEADER).has_value());
}

TEST(FixedGroup, isValidTrue) {
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

TEST(FixedGroup, isValidFalse) {
	const auto fg1 = metaf::FixedGroup::parse("CLD", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_FALSE(fg1->isValid());
	
	const auto fg2 = metaf::FixedGroup::parse("ICG", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_FALSE(fg2->isValid());
	
	const auto fg3 = metaf::FixedGroup::parse("PCPN", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_FALSE(fg3->isValid());

	const auto fg4 = metaf::FixedGroup::parse("PRES", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_FALSE(fg4->isValid());

	const auto fg5 = metaf::FixedGroup::parse("RVR", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg5.has_value());
	EXPECT_FALSE(fg5->isValid());

	const auto fg6 = metaf::FixedGroup::parse("T", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg6.has_value());
	EXPECT_FALSE(fg6->isValid());

	const auto fg7 = metaf::FixedGroup::parse("TD", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg7.has_value());
	EXPECT_FALSE(fg7->isValid());
	
	const auto fg8 = metaf::FixedGroup::parse("VIS", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg8.has_value());
	EXPECT_FALSE(fg8->isValid());
	
	const auto fg9 = metaf::FixedGroup::parse("WND", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg9.has_value());
	EXPECT_FALSE(fg9->isValid());

	const auto fg10 = metaf::FixedGroup::parse("WX", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg10.has_value());
	EXPECT_FALSE(fg10->isValid());

	const auto fg11 = metaf::FixedGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg11.has_value());
	EXPECT_FALSE(fg11->isValid());

	auto fg12 = metaf::FixedGroup::parse("TS/LTNG", metaf::ReportPart::RMK);
	EXPECT_EQ(fg12->append("TEMPO", metaf::ReportPart::RMK), metaf::AppendResult::APPENDED);
	ASSERT_TRUE(fg12.has_value());
	EXPECT_FALSE(fg12->isValid());
}
