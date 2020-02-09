/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// UnknownGroup tests
// Purpose: to confirm that any string can be parsed as UnknownGroup, any
// string can be appended to UnknownGroup, and that isValid() method always 
// returns true for UnknownGroup
///////////////////////////////////////////////////////////////////////////////

TEST(UnknownGroup, parse) {
	static const std::string str = "AAAAAAAA";
	const auto ug1 = metaf::UnknownGroup::parse(str, metaf::ReportPart::UNKNOWN);
	ASSERT_TRUE(ug1.has_value());

	const auto ug2 = metaf::UnknownGroup::parse(str, metaf::ReportPart::HEADER);
	ASSERT_TRUE(ug2.has_value());

	const auto ug3 = metaf::UnknownGroup::parse(str, metaf::ReportPart::METAR);
	ASSERT_TRUE(ug3.has_value());

	const auto ug4 = metaf::UnknownGroup::parse(str, metaf::ReportPart::TAF);
	ASSERT_TRUE(ug4.has_value());

	const auto ug5 = metaf::UnknownGroup::parse(str, metaf::ReportPart::RMK);
	ASSERT_TRUE(ug5.has_value());
}

TEST(UnknownGroup, append) {
	auto ug = metaf::UnknownGroup::parse("ZZZZZZZZ ", metaf::ReportPart::METAR);
	ASSERT_TRUE(ug.has_value());
	EXPECT_EQ(ug->append("AAAAAAAAAAA"), metaf::AppendResult::APPENDED);
}

TEST(UnknownGroup, isValid) {
	auto ug1 = metaf::UnknownGroup::parse("ZZZZZZZZ ", metaf::ReportPart::METAR);
	ASSERT_TRUE(ug1.has_value());
	EXPECT_TRUE(ug1->isValid());

	auto ug2 = metaf::UnknownGroup::parse(" ", metaf::ReportPart::METAR);
	ASSERT_TRUE(ug2.has_value());
	EXPECT_TRUE(ug2->isValid());
}
