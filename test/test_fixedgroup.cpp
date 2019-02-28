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
}