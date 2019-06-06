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

TEST(PlainTextGroup, combine) {
	const auto ptg1 = metaf::PlainTextGroup("PLAIN");
	const auto ptg2 = metaf::PlainTextGroup("TEXT");

	const auto combined = ptg1.combine(ptg2);
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined.value()));

	const auto ptgCombined = std::get<metaf::PlainTextGroup>(combined.value());
	EXPECT_EQ(ptgCombined.toString(), "PLAIN TEXT");

}

TEST(PlainTextGroup, combineMaxLength) {
	static const std::string strMaxLengthMinus2 (metaf::PlainTextGroup::textMaxLength - 2, 'X');
	static const std::string strSingleChar("A");

	const auto ptgMaxLengthMinus2 = metaf::PlainTextGroup(strMaxLengthMinus2);
	const auto ptg = metaf::PlainTextGroup(strSingleChar);

	const auto combined = ptgMaxLengthMinus2.combine(ptg);
	ASSERT_TRUE(combined.has_value());
	ASSERT_TRUE(std::holds_alternative<metaf::PlainTextGroup>(combined.value()));

	const auto ptgCombined = std::get<metaf::PlainTextGroup>(combined.value());
	EXPECT_EQ(ptgCombined.toString(), strMaxLengthMinus2 + " " + strSingleChar);
}

TEST(PlainTextGroup, combineTooLongText) {
	static const std::string strMaxLengthMinus2 (metaf::PlainTextGroup::textMaxLength - 2, 'X');
	static const std::string strTwoChars = ("AA");

	const auto ptgMaxLengthMinus2 = metaf::PlainTextGroup(strMaxLengthMinus2);
	const auto ptg = metaf::PlainTextGroup(strTwoChars);

	EXPECT_FALSE(ptgMaxLengthMinus2.combine(ptg).has_value());
}