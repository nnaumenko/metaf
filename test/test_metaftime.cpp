/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

TEST(MetafTime, fromStringDDHHMMwithDay) {
	const auto t = metaf::MetafTime::fromStringDDHHMM("120830");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 12u);
	EXPECT_EQ(t->hour(), 8u);
	EXPECT_EQ(t->minute(), 30u);
}

TEST(MetafTime, fromStringDDHHMMwithoutDay) {
	const auto t = metaf::MetafTime::fromStringDDHHMM("0830");
	ASSERT_TRUE(t.has_value());
	EXPECT_FALSE(t->day().has_value());
	EXPECT_EQ(t->hour(), 8u);
	EXPECT_EQ(t->minute(), 30u);
}

TEST(MetafTime, fromStringDDHHMM_wrongFormat) {
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("12/08:30").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("A20830").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("//////").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("20830").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("1208030").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("120830Z").has_value());
}

TEST(MetafTime, fromStringDDHH) {
	const auto t = metaf::MetafTime::fromStringDDHH("1208");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 12u);
	EXPECT_EQ(t->hour(), 8u);
	EXPECT_EQ(t->minute(), 0u);
}

TEST(MetafTime, fromStringDDHH_wrongFormat) {
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("12:08").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("12/08").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("A208").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("208").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("////").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("12080").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHH("1208Z").has_value());
}

TEST(MetafTime, isValidCorrect) {
	const auto t1 = metaf::MetafTime::fromStringDDHHMM("312459");
	ASSERT_TRUE(t1.has_value());
	EXPECT_TRUE(t1->isValid());

	const auto t2 = metaf::MetafTime::fromStringDDHHMM("010000");
	ASSERT_TRUE(t2.has_value());
	EXPECT_TRUE(t2->isValid());

	const auto t3 = metaf::MetafTime::fromStringDDHHMM("2459");
	ASSERT_TRUE(t3.has_value());
	EXPECT_TRUE(t3->isValid());

	const auto t4 = metaf::MetafTime::fromStringDDHHMM("0000");
	ASSERT_TRUE(t4.has_value());
	EXPECT_TRUE(t4->isValid());

	const auto t5 = metaf::MetafTime::fromStringDDHH("3124");
	ASSERT_TRUE(t5.has_value());
	EXPECT_TRUE(t5->isValid());

	const auto t6 = metaf::MetafTime::fromStringDDHH("0100");
	ASSERT_TRUE(t6.has_value());
	EXPECT_TRUE(t6->isValid());

}

TEST(MetafTime, isValidIncorrect) {
	const auto t1 = metaf::MetafTime::fromStringDDHHMM("322459");
	ASSERT_TRUE(t1.has_value());
	EXPECT_FALSE(t1->isValid());

	const auto t2 = metaf::MetafTime::fromStringDDHHMM("312559");
	ASSERT_TRUE(t2.has_value());
	EXPECT_FALSE(t2->isValid());

	const auto t3 = metaf::MetafTime::fromStringDDHHMM("312460");
	ASSERT_TRUE(t3.has_value());
	EXPECT_FALSE(t3->isValid());

	const auto t4 = metaf::MetafTime::fromStringDDHHMM("002459");
	ASSERT_TRUE(t4.has_value());
	EXPECT_FALSE(t4->isValid());

	const auto t5 = metaf::MetafTime::fromStringDDHH("3224");
	ASSERT_TRUE(t5.has_value());
	EXPECT_FALSE(t5->isValid());

	const auto t6 = metaf::MetafTime::fromStringDDHH("0024");
	ASSERT_TRUE(t6.has_value());
	EXPECT_FALSE(t6->isValid());

	const auto t7 = metaf::MetafTime::fromStringDDHH("3125");
	ASSERT_TRUE(t7.has_value());
	EXPECT_FALSE(t7->isValid());
}

TEST(MetafTime, is3hourlyReportTimeTrue) {
	const auto t2 = metaf::MetafTime::fromStringDDHHMM("310258");
	ASSERT_TRUE(t2.has_value());
	EXPECT_TRUE(t2->is3hourlyReportTime());

	const auto t3 = metaf::MetafTime::fromStringDDHHMM("310304");
	ASSERT_TRUE(t3.has_value());
	EXPECT_TRUE(t3->is3hourlyReportTime());

	const auto t8 = metaf::MetafTime::fromStringDDHHMM("310858");
	ASSERT_TRUE(t8.has_value());
	EXPECT_TRUE(t8->is3hourlyReportTime());

	const auto t9 = metaf::MetafTime::fromStringDDHHMM("310904");
	ASSERT_TRUE(t9.has_value());
	EXPECT_TRUE(t9->is3hourlyReportTime());

	const auto t14 = metaf::MetafTime::fromStringDDHHMM("311458");
	ASSERT_TRUE(t14.has_value());
	EXPECT_TRUE(t14->is3hourlyReportTime());

	const auto t15 = metaf::MetafTime::fromStringDDHHMM("311504");
	ASSERT_TRUE(t15.has_value());
	EXPECT_TRUE(t15->is3hourlyReportTime());

	const auto t20 = metaf::MetafTime::fromStringDDHHMM("312058");
	ASSERT_TRUE(t20.has_value());
	EXPECT_TRUE(t20->is3hourlyReportTime());

	const auto t21 = metaf::MetafTime::fromStringDDHHMM("312104");
	ASSERT_TRUE(t21.has_value());
	EXPECT_TRUE(t21->is3hourlyReportTime());
}

TEST(MetafTime, is3hourlyReportTimeFalse) {
	const auto t0 = metaf::MetafTime::fromStringDDHHMM("310005");
	ASSERT_TRUE(t0.has_value());
	EXPECT_FALSE(t0->is3hourlyReportTime());

	const auto t1 = metaf::MetafTime::fromStringDDHHMM("310105");
	ASSERT_TRUE(t1.has_value());
	EXPECT_FALSE(t1->is3hourlyReportTime());

	const auto t4 = metaf::MetafTime::fromStringDDHHMM("310405");
	ASSERT_TRUE(t4.has_value());
	EXPECT_FALSE(t4->is3hourlyReportTime());

	const auto t5 = metaf::MetafTime::fromStringDDHHMM("310505");
	ASSERT_TRUE(t5.has_value());
	EXPECT_FALSE(t5->is3hourlyReportTime());

	const auto t6 = metaf::MetafTime::fromStringDDHHMM("310605");
	ASSERT_TRUE(t6.has_value());
	EXPECT_FALSE(t6->is3hourlyReportTime());

	const auto t7 = metaf::MetafTime::fromStringDDHHMM("310705");
	ASSERT_TRUE(t7.has_value());
	EXPECT_FALSE(t7->is3hourlyReportTime());

	const auto t10 = metaf::MetafTime::fromStringDDHHMM("311005");
	ASSERT_TRUE(t10.has_value());
	EXPECT_FALSE(t10->is3hourlyReportTime());

	const auto t11 = metaf::MetafTime::fromStringDDHHMM("311105");
	ASSERT_TRUE(t11.has_value());
	EXPECT_FALSE(t11->is3hourlyReportTime());

	const auto t12 = metaf::MetafTime::fromStringDDHHMM("311205");
	ASSERT_TRUE(t12.has_value());
	EXPECT_FALSE(t12->is3hourlyReportTime());

	const auto t13 = metaf::MetafTime::fromStringDDHHMM("311305");
	ASSERT_TRUE(t13.has_value());
	EXPECT_FALSE(t13->is3hourlyReportTime());

	const auto t16 = metaf::MetafTime::fromStringDDHHMM("311605");
	ASSERT_TRUE(t16.has_value());
	EXPECT_FALSE(t16->is3hourlyReportTime());

	const auto t17 = metaf::MetafTime::fromStringDDHHMM("311705");
	ASSERT_TRUE(t17.has_value());
	EXPECT_FALSE(t17->is3hourlyReportTime());

	const auto t18 = metaf::MetafTime::fromStringDDHHMM("311805");
	ASSERT_TRUE(t18.has_value());
	EXPECT_FALSE(t18->is3hourlyReportTime());

	const auto t19 = metaf::MetafTime::fromStringDDHHMM("311905");
	ASSERT_TRUE(t19.has_value());
	EXPECT_FALSE(t19->is3hourlyReportTime());

	const auto t22 = metaf::MetafTime::fromStringDDHHMM("312205");
	ASSERT_TRUE(t22.has_value());
	EXPECT_FALSE(t22->is3hourlyReportTime());

	const auto t23 = metaf::MetafTime::fromStringDDHHMM("312305");
	ASSERT_TRUE(t23.has_value());
	EXPECT_FALSE(t23->is3hourlyReportTime());
}

TEST(MetafTime, is6hourlyReportTimeTrue) {
	const auto t0 = metaf::MetafTime::fromStringDDHHMM("310004");
	ASSERT_TRUE(t0.has_value());
	EXPECT_TRUE(t0->is6hourlyReportTime());

	const auto t5 = metaf::MetafTime::fromStringDDHHMM("310558");
	ASSERT_TRUE(t5.has_value());
	EXPECT_TRUE(t5->is6hourlyReportTime());

	const auto t6 = metaf::MetafTime::fromStringDDHHMM("310604");
	ASSERT_TRUE(t6.has_value());
	EXPECT_TRUE(t6->is6hourlyReportTime());

	const auto t11 = metaf::MetafTime::fromStringDDHHMM("311158");
	ASSERT_TRUE(t11.has_value());
	EXPECT_TRUE(t11->is6hourlyReportTime());

	const auto t12 = metaf::MetafTime::fromStringDDHHMM("311204");
	ASSERT_TRUE(t12.has_value());
	EXPECT_TRUE(t12->is6hourlyReportTime());

	const auto t17 = metaf::MetafTime::fromStringDDHHMM("311758");
	ASSERT_TRUE(t17.has_value());
	EXPECT_TRUE(t17->is6hourlyReportTime());

	const auto t18 = metaf::MetafTime::fromStringDDHHMM("311804");
	ASSERT_TRUE(t18.has_value());
	EXPECT_TRUE(t18->is6hourlyReportTime());

	const auto t23 = metaf::MetafTime::fromStringDDHHMM("312358");
	ASSERT_TRUE(t23.has_value());
	EXPECT_TRUE(t23->is6hourlyReportTime());
}

TEST(MetafTime, is6hourlyReportTimeFalse) {
	const auto t1 = metaf::MetafTime::fromStringDDHHMM("310105");
	ASSERT_TRUE(t1.has_value());
	EXPECT_FALSE(t1->is6hourlyReportTime());

	const auto t2 = metaf::MetafTime::fromStringDDHHMM("310205");
	ASSERT_TRUE(t2.has_value());
	EXPECT_FALSE(t2->is6hourlyReportTime());

	const auto t3 = metaf::MetafTime::fromStringDDHHMM("310305");
	ASSERT_TRUE(t3.has_value());
	EXPECT_FALSE(t3->is6hourlyReportTime());

	const auto t4 = metaf::MetafTime::fromStringDDHHMM("310405");
	ASSERT_TRUE(t4.has_value());
	EXPECT_FALSE(t4->is6hourlyReportTime());

	const auto t7 = metaf::MetafTime::fromStringDDHHMM("310705");
	ASSERT_TRUE(t7.has_value());
	EXPECT_FALSE(t7->is6hourlyReportTime());

	const auto t8 = metaf::MetafTime::fromStringDDHHMM("310805");
	ASSERT_TRUE(t8.has_value());
	EXPECT_FALSE(t8->is6hourlyReportTime());

	const auto t9 = metaf::MetafTime::fromStringDDHHMM("310905");
	ASSERT_TRUE(t9.has_value());
	EXPECT_FALSE(t9->is6hourlyReportTime());

	const auto t10 = metaf::MetafTime::fromStringDDHHMM("311005");
	ASSERT_TRUE(t10.has_value());
	EXPECT_FALSE(t10->is6hourlyReportTime());

	const auto t13 = metaf::MetafTime::fromStringDDHHMM("311305");
	ASSERT_TRUE(t13.has_value());
	EXPECT_FALSE(t13->is6hourlyReportTime());

	const auto t14 = metaf::MetafTime::fromStringDDHHMM("311405");
	ASSERT_TRUE(t14.has_value());
	EXPECT_FALSE(t14->is6hourlyReportTime());

	const auto t15 = metaf::MetafTime::fromStringDDHHMM("311505");
	ASSERT_TRUE(t15.has_value());
	EXPECT_FALSE(t15->is6hourlyReportTime());

	const auto t16 = metaf::MetafTime::fromStringDDHHMM("311605");
	ASSERT_TRUE(t16.has_value());
	EXPECT_FALSE(t16->is6hourlyReportTime());

	const auto t19 = metaf::MetafTime::fromStringDDHHMM("311905");
	ASSERT_TRUE(t19.has_value());
	EXPECT_FALSE(t19->is6hourlyReportTime());

	const auto t20 = metaf::MetafTime::fromStringDDHHMM("312005");
	ASSERT_TRUE(t20.has_value());
	EXPECT_FALSE(t20->is6hourlyReportTime());

	const auto t21 = metaf::MetafTime::fromStringDDHHMM("312105");
	ASSERT_TRUE(t21.has_value());
	EXPECT_FALSE(t21->is6hourlyReportTime());

	const auto t22 = metaf::MetafTime::fromStringDDHHMM("312205");
	ASSERT_TRUE(t22.has_value());
	EXPECT_FALSE(t22->is6hourlyReportTime());
}

TEST(MetafTime, dateBeforeRefSameMonthAndYear) {
	const auto t = metaf::MetafTime::fromStringDDHHMM("201730");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->isValid());
	const auto tDate = t->dateBeforeRef(metaf::MetafTime::Date(2019, 6, 21));
	EXPECT_EQ(tDate.year, 2019u);
	EXPECT_EQ(tDate.month, 6u);
	EXPECT_EQ(tDate.day, 20u);
}

TEST(MetafTime, dateBeforeRefDifferentMonth) {
	const auto t = metaf::MetafTime::fromStringDDHHMM("301730");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->isValid());
	const auto tDate = t->dateBeforeRef(metaf::MetafTime::Date(2019, 7, 1));
	EXPECT_EQ(tDate.year, 2019u);
	EXPECT_EQ(tDate.month, 6u);
	EXPECT_EQ(tDate.day, 30u);
}

TEST(MetafTime, dateBeforeRefDifferentMonthAndYear) {
	const auto t = metaf::MetafTime::fromStringDDHHMM("311730");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->isValid());
	const auto tDate = t->dateBeforeRef(metaf::MetafTime::Date(2019, 1, 1));
	EXPECT_EQ(tDate.year, 2018u);
	EXPECT_EQ(tDate.month, 12u);
	EXPECT_EQ(tDate.day, 31u);
}

TEST(MetafTime, constructorHourMinute) {
	const auto t = metaf::MetafTime(11, 25);
	EXPECT_FALSE(t.day().has_value());
	EXPECT_EQ(t.hour(), 11u);
	EXPECT_EQ(t.minute(), 25u);
	EXPECT_TRUE(t.isValid());
}

TEST(MetafTime, constructorHourMinuteInvalidTime) {
	const auto t = metaf::MetafTime(25, 90);
	EXPECT_FALSE(t.day().has_value());
	EXPECT_EQ(t.hour(), 25u);
	EXPECT_EQ(t.minute(), 90u);
	EXPECT_FALSE(t.isValid());
}