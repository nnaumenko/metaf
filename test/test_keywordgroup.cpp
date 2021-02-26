/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Parsing tests
// Purpose: to confirm that each keyword is parsed correctly
///////////////////////////////////////////////////////////////////////////////

TEST(KeywordGroup, parseMetar) {
	static const char gs[] = "METAR";

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::KeywordGroup::Type::METAR);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(KeywordGroup, parseSpeci) {
	static const char gs[] = "SPECI";

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::KeywordGroup::Type::SPECI);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(KeywordGroup, parseTaf) {
	static const char gs[] = "TAF";

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::KeywordGroup::Type::TAF);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(KeywordGroup, parseAmd) {
	static const char gs[] = "AMD";
	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::KeywordGroup::Type::AMD);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(KeywordGroup, parseNil) {
	static const char gs[] = "NIL";
	static const auto type = metaf::KeywordGroup::Type::NIL;

	auto fg1 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	auto fg3 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(KeywordGroup, parseCnl) {
	static const char gs[] = "CNL";
	static const auto type = metaf::KeywordGroup::Type::CNL;

	auto fg1 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	auto fg3 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(KeywordGroup, parseCor) {
	static const char gs[] = "COR";
	static const auto type = metaf::KeywordGroup::Type::COR;

	auto fg1 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(KeywordGroup, parseAuto) {
	static const char gs[] = "AUTO";

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), metaf::KeywordGroup::Type::AUTO);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(KeywordGroup, parseCavok) {
	static const char gs[] = "CAVOK";
	static const auto type = metaf::KeywordGroup::Type::CAVOK;

	auto fg1 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(KeywordGroup, parseRmk) {
	static const char gs[] = "RMK";
	static const auto type = metaf::KeywordGroup::Type::RMK;

	auto fg1 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK).has_value());
}

TEST(KeywordGroup, parseMaintenanceIndicator) {
	static const char gs[] = "$";
	static const auto type = metaf::KeywordGroup::Type::MAINTENANCE_INDICATOR;

	auto fg1 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_EQ(fg1->type(), type);

	auto fg2 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_EQ(fg2->type(), type);

	auto fg3 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_EQ(fg3->type(), type);

	auto fg4 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_EQ(fg4->type(), type);

	auto fg5 = metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg5.has_value());
	EXPECT_EQ(fg5->type(), type);

}

TEST(KeywordGroup, parseAo1) {
	static const char gs[] = "AO1";
	static const auto type = metaf::KeywordGroup::Type::AO1;

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(KeywordGroup, parseA01) {
	static const char gs[] = "A01";
	static const auto type = metaf::KeywordGroup::Type::AO1;

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(KeywordGroup, parseAo2) {
	static const char gs[] = "AO2";
	static const auto type = metaf::KeywordGroup::Type::AO2;

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(KeywordGroup, parseA02) {
	static const char gs[] = "A02";
	static const auto type = metaf::KeywordGroup::Type::AO2;

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(KeywordGroup, parseAo1a) {
	static const char gs[] = "AO1A";
	static const auto type = metaf::KeywordGroup::Type::AO1A;

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(KeywordGroup, parseA01a) {
	static const char gs[] = "A01A";
	static const auto type = metaf::KeywordGroup::Type::AO1A;

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(KeywordGroup, parseAo2a) {
	static const char gs[] = "AO2A";
	static const auto type = metaf::KeywordGroup::Type::AO2A;

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(KeywordGroup, parseA02a) {
	static const char gs[] = "A02A";
	static const auto type = metaf::KeywordGroup::Type::AO2A;

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

TEST(KeywordGroup, parseNospeci) {
	static const char gs[] = "NOSPECI";
	static const auto type = metaf::KeywordGroup::Type::NOSPECI;

	auto fg = metaf::KeywordGroup::parse(gs, metaf::ReportPart::RMK);
	ASSERT_TRUE(fg.has_value());
	EXPECT_EQ(fg->type(), type);

	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse(gs, metaf::ReportPart::TAF).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Non-keywords
// Purpose: to confirm that non-keywords are not parsed 
///////////////////////////////////////////////////////////////////////////////

TEST(KeywordGroup, parseOther) {
	EXPECT_FALSE(metaf::KeywordGroup::parse("", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse("METAF", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse("META", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::KeywordGroup::parse("MMETAR", metaf::ReportPart::HEADER).has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method returns true for all keywords
///////////////////////////////////////////////////////////////////////////////

TEST(KeywordGroup, isValidTrue) {
	const auto fg1 = metaf::KeywordGroup::parse("METAR", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg1.has_value());
	EXPECT_TRUE(fg1->isValid());
	
	const auto fg2 = metaf::KeywordGroup::parse("SPECI", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg2.has_value());
	EXPECT_TRUE(fg2->isValid());
	
	const auto fg3 = metaf::KeywordGroup::parse("TAF", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg3.has_value());
	EXPECT_TRUE(fg3->isValid());

	const auto fg4 = metaf::KeywordGroup::parse("AMD", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg4.has_value());
	EXPECT_TRUE(fg4->isValid());

	const auto fg5 = metaf::KeywordGroup::parse("NIL", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg5.has_value());
	EXPECT_TRUE(fg5->isValid());

	const auto fg6 = metaf::KeywordGroup::parse("CNL", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg6.has_value());
	EXPECT_TRUE(fg6->isValid());

	const auto fg7 = metaf::KeywordGroup::parse("COR", metaf::ReportPart::HEADER);
	ASSERT_TRUE(fg7.has_value());
	EXPECT_TRUE(fg7->isValid());
	
	const auto fg8 = metaf::KeywordGroup::parse("AUTO", metaf::ReportPart::METAR);
	ASSERT_TRUE(fg8.has_value());
	EXPECT_TRUE(fg8->isValid());
	
	const auto fg9 = metaf::KeywordGroup::parse("CAVOK", metaf::ReportPart::METAR);
	ASSERT_TRUE(fg9.has_value());
	EXPECT_TRUE(fg9->isValid());

	const auto fg10 = metaf::KeywordGroup::parse("RMK", metaf::ReportPart::METAR);
	ASSERT_TRUE(fg10.has_value());
	EXPECT_TRUE(fg10->isValid());

	const auto fg11 = metaf::KeywordGroup::parse("AO1", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg11.has_value());
	EXPECT_TRUE(fg11->isValid());

	const auto fg12 = metaf::KeywordGroup::parse("AO2", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg12.has_value());
	EXPECT_TRUE(fg12->isValid());

	const auto fg13 = metaf::KeywordGroup::parse("AO1A", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg13.has_value());
	EXPECT_TRUE(fg13->isValid());

	const auto fg14 = metaf::KeywordGroup::parse("AO2A", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg14.has_value());
	EXPECT_TRUE(fg14->isValid());

	const auto fg15 = metaf::KeywordGroup::parse("NOSPECI", metaf::ReportPart::RMK);
	ASSERT_TRUE(fg15.has_value());
	EXPECT_TRUE(fg15->isValid());
}
