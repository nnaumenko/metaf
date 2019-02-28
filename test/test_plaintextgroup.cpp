/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(PlainTextGroup, constructorFromString) {
	static const std::string str = "AAAAAAAA";
	EXPECT_EQ(metaf::PlainTextGroup(str).toString(), str);
}

TEST(PlainTextGroup, parse) {
	static const std::string str = "AAAAAAAA";
	const auto ptg1 = metaf::PlainTextGroup::parse(str, metaf::ReportPart::UNKNOWN);
	ASSERT_TRUE(ptg1.has_value());
	EXPECT_EQ(ptg1->toString(), str);

	const auto ptg2 = metaf::PlainTextGroup::parse(str, metaf::ReportPart::HEADER);
	ASSERT_TRUE(ptg2.has_value());
	EXPECT_EQ(ptg2->toString(), str);

	const auto ptg3 = metaf::PlainTextGroup::parse(str, metaf::ReportPart::METAR);
	ASSERT_TRUE(ptg3.has_value());
	EXPECT_EQ(ptg3->toString(), str);

	const auto ptg4 = metaf::PlainTextGroup::parse(str, metaf::ReportPart::TAF);
	ASSERT_TRUE(ptg4.has_value());
	EXPECT_EQ(ptg4->toString(), str);

	const auto ptg5 = metaf::PlainTextGroup::parse(str, metaf::ReportPart::RMK);
	ASSERT_TRUE(ptg5.has_value());
	EXPECT_EQ(ptg5->toString(), str);
}

TEST(PlainTextGroup, parseTruncate) {
	static const std::string strMaxLength (metaf::PlainTextGroup::textMaxLength, 'X');
	static const std::string strTooBig (metaf::PlainTextGroup::textMaxLength + 1, 'X');
	EXPECT_EQ(metaf::PlainTextGroup(strMaxLength).toString(), strMaxLength);
	EXPECT_EQ(metaf::PlainTextGroup(strTooBig).toString(), strMaxLength);
}

TEST(PlainTextGroup, isValidTrue) {
	EXPECT_TRUE(metaf::PlainTextGroup("ZZZZZZ").isValid());
}

TEST(PlainTextGroup, isValidFalse) {
	EXPECT_FALSE(metaf::PlainTextGroup("").isValid());
}

