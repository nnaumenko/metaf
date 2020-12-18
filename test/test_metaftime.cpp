/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

///////////////////////////////////////////////////////////////////////////////
// Constructor tests
// Purpose: to confirm that constructor correctly initialises the instances of 
// MetafTime, even if constructor's arguments are not valid/consistent.
///////////////////////////////////////////////////////////////////////////////

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

TEST(MetafTime, constructorDayHourMinute) {
	const auto t = metaf::MetafTime(4, 11, 25);
	ASSERT_TRUE(t.day().has_value());
	EXPECT_EQ(t.day().value(), 4u);
	EXPECT_EQ(t.hour(), 11u);
	EXPECT_EQ(t.minute(), 25u);
	EXPECT_TRUE(t.isValid());
}

TEST(MetafTime, constructorDayHourMinuteDayNotReported) {
	const auto t = metaf::MetafTime(std::optional<unsigned int>(), 11, 25);
	EXPECT_FALSE(t.day().has_value());
	EXPECT_EQ(t.hour(), 11u);
	EXPECT_EQ(t.minute(), 25u);
	EXPECT_TRUE(t.isValid());
}

TEST(MetafTime, constructorDayHourMinuteInvalidDay) {
	const auto t = metaf::MetafTime(32, 11, 25);
	ASSERT_TRUE(t.day().has_value());
	EXPECT_EQ(t.day().value(), 32u);
	EXPECT_EQ(t.hour(), 11u);
	EXPECT_EQ(t.minute(), 25u);
	EXPECT_FALSE(t.isValid());
}

///////////////////////////////////////////////////////////////////////////////
// Parsing tests
// Purpose: to confirm that various date/time formats used in METAR and TAF 
// reports are parsed correctly and other data formats are not parsed
///////////////////////////////////////////////////////////////////////////////

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
	EXPECT_FALSE(metaf::MetafTime::fromStringDDHHMM("12/0830").has_value());
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

TEST(MetafTime, fromStringDDSlashHHMM) {
	const auto t = metaf::MetafTime::fromStringDDSlashHHMM("12/0830");
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 12u);
	EXPECT_EQ(t->hour(), 8u);
	EXPECT_EQ(t->minute(), 30u);
}

TEST(MetafTime, fromStringDDSlashHHMM_wrongFormat) {
	EXPECT_FALSE(metaf::MetafTime::fromStringDDSlashHHMM("12/08:30").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDSlashHHMM("120830").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDSlashHHMM("A20830").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDSlashHHMM("//////").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDSlashHHMM("20830").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDSlashHHMM("1208030").has_value());
	EXPECT_FALSE(metaf::MetafTime::fromStringDDSlashHHMM("120830Z").has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Remark formats test
// Purpose: to confirm that all formats used in schedule remarks (NEXT, NO 
// AMDS AFT, etc) are parsed correctly.
// Note: 
// Note: tests for the detection of DDHH and HHMM format are performed in the
// next section 
///////////////////////////////////////////////////////////////////////////////

TEST(MetafTime, fromRemarkString) {
	const auto t1 = metaf::MetafTime::fromRemarkString("0830",
		metaf::MetafTime(12, 3, 0),
		metaf::MetafTime(13, 3, 0),
		true);
	ASSERT_TRUE(t1.has_value());
	EXPECT_FALSE(t1->day().has_value());
	EXPECT_EQ(t1->hour(), 8u);
	EXPECT_EQ(t1->minute(), 30u);

	const auto t2 = metaf::MetafTime::fromRemarkString("2611",
		metaf::MetafTime(25, 17, 0),
		metaf::MetafTime(26, 17, 0));
	ASSERT_TRUE(t2.has_value());
	ASSERT_TRUE(t2->day().has_value());
	EXPECT_EQ(t2->day().value(), 26u);
	EXPECT_EQ(t2->hour(), 11u);
	EXPECT_EQ(t2->minute(), 0u);

	const auto t3 = metaf::MetafTime::fromRemarkString("120830",
		metaf::MetafTime(12, 3, 0),
		metaf::MetafTime(13, 3, 0));
	ASSERT_TRUE(t3.has_value());
	ASSERT_TRUE(t3->day().has_value());
	EXPECT_EQ(t3->day().value(), 12u);
	EXPECT_EQ(t3->hour(), 8u);
	EXPECT_EQ(t3->minute(), 30u);

	const auto t4 = metaf::MetafTime::fromRemarkString("120830Z",
		metaf::MetafTime(12, 3, 0),
		metaf::MetafTime(13, 3, 0));
	ASSERT_TRUE(t4.has_value());
	ASSERT_TRUE(t4->day().has_value());
	EXPECT_EQ(t4->day().value(), 12u);
	EXPECT_EQ(t4->hour(), 8u);
	EXPECT_EQ(t4->minute(), 30u);

	const auto t5 = metaf::MetafTime::fromRemarkString("12/0830",
		metaf::MetafTime(12, 3, 0),
		metaf::MetafTime(13, 3, 0));
	ASSERT_TRUE(t5.has_value());
	ASSERT_TRUE(t5->day().has_value());
	EXPECT_EQ(t5->day().value(), 12u);
	EXPECT_EQ(t5->hour(), 8u);
	EXPECT_EQ(t5->minute(), 30u);

	const auto t6 = metaf::MetafTime::fromRemarkString("1208Z",
		metaf::MetafTime(12, 3, 0),
		metaf::MetafTime(13, 3, 0));
	ASSERT_TRUE(t6.has_value());
	ASSERT_TRUE(t6->day().has_value());
	EXPECT_EQ(t6->day().value(), 12u);
	EXPECT_EQ(t6->hour(), 8u);
	EXPECT_EQ(t6->minute(), 0u);
}


TEST(MetafTime, fromRemarkString_HHMMvsDDHH) {
	const auto timeStr = "0815";
	const auto begin = metaf::MetafTime(12, 3, 0);
	const auto end = metaf::MetafTime(13, 3, 0);

	const auto t1 = metaf::MetafTime::fromRemarkString(timeStr, begin, end, true);
	ASSERT_TRUE(t1.has_value());
	EXPECT_FALSE(t1->day().has_value());
	EXPECT_EQ(t1->hour(), 8u);
	EXPECT_EQ(t1->minute(), 15u);

	const auto t2 = metaf::MetafTime::fromRemarkString(timeStr, begin, end, false);
	ASSERT_TRUE(t2.has_value());
	ASSERT_TRUE(t2->day().has_value());
	EXPECT_EQ(t2->day().value(), 8u);
	EXPECT_EQ(t2->hour(), 15u);
	EXPECT_EQ(t2->minute(), 0u);
}

///////////////////////////////////////////////////////////////////////////////
// Format detection tests
// Purpose: to confirm that formats DDHH and HHMM are correctly distinguisted
// based on range vaildity for day, hour and minute, on date of the report, 
// and on forecast applicability time 
///////////////////////////////////////////////////////////////////////////////

TEST(MetafTime, formatDetectionDDHH_dayValue) {
	const auto t = metaf::MetafTime::fromStringDDHHorHHMM("2508", // hour cannot be 25
		metaf::MetafTime(0, 0),
		metaf::MetafTime(0, 0));
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 25u);
	EXPECT_EQ(t->hour(), 8u);
	EXPECT_EQ(t->minute(), 0u);
}

TEST(MetafTime, formatDetectionHHMM_hourValue) {
	const auto t = metaf::MetafTime::fromStringDDHHorHHMM("0008", // day-of-month cannot be 0
		metaf::MetafTime(0, 0),
		metaf::MetafTime(0, 0));
	ASSERT_TRUE(t.has_value());
	EXPECT_FALSE(t->day().has_value());
	EXPECT_EQ(t->hour(), 0u);
	EXPECT_EQ(t->minute(), 8u);
}

TEST(MetafTime, formatDetectionHHMM_minuteValue) {
	const auto t = metaf::MetafTime::fromStringDDHHorHHMM("0125", // hour cannot be 25
		metaf::MetafTime(0, 0),
		metaf::MetafTime(0, 0));
	ASSERT_TRUE(t.has_value());
	EXPECT_FALSE(t->day().has_value());
	EXPECT_EQ(t->hour(), 1u);
	EXPECT_EQ(t->minute(), 25u);
}

TEST(MetafTime, formatDetectionDDHH_beginDay) {
	const auto t = metaf::MetafTime::fromStringDDHHorHHMM("1507", 
		metaf::MetafTime(15, 11, 0),
		metaf::MetafTime(0, 0));
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 15u);
	EXPECT_EQ(t->hour(), 7u);
	EXPECT_EQ(t->minute(), 0u);
}

TEST(MetafTime, formatDetectionDDHH_endDay) {
	const auto t = metaf::MetafTime::fromStringDDHHorHHMM("1607", 
		metaf::MetafTime(15, 11, 0),
		metaf::MetafTime(16, 11, 0));
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 16u);
	EXPECT_EQ(t->hour(), 7u);
	EXPECT_EQ(t->minute(), 0u);
}

TEST(MetafTime, formatDetectionDDHH_nextDayAfterBeginDay) {
	const auto t = metaf::MetafTime::fromStringDDHHorHHMM("1607", 
		metaf::MetafTime(15, 11, 0),
		metaf::MetafTime(0, 0));
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 16u);
	EXPECT_EQ(t->hour(), 7u);
	EXPECT_EQ(t->minute(), 0u);
}

TEST(MetafTime, formatDetectionDDHH_endDayEndOfMonth) {
	const auto t = metaf::MetafTime::fromStringDDHHorHHMM("0107", 
		metaf::MetafTime(28, 11, 0),
		metaf::MetafTime(1, 11, 0));
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 1u);
	EXPECT_EQ(t->hour(), 7u);
	EXPECT_EQ(t->minute(), 0u);
}

TEST(MetafTime, formatDetectionDDHH_nextDayAfterBeginEndOfMonth) {
	const auto t = metaf::MetafTime::fromStringDDHHorHHMM("0107", 
		metaf::MetafTime(28, 11, 0),
		metaf::MetafTime(0, 0));
	ASSERT_TRUE(t.has_value());
	ASSERT_TRUE(t->day().has_value());
	EXPECT_EQ(t->day().value(), 1u);
	EXPECT_EQ(t->hour(), 7u);
	EXPECT_EQ(t->minute(), 0u);
}

///////////////////////////////////////////////////////////////////////////////
// Tests for isValid()
// Purpose: to confirm that isValid() method correctly validates the data
///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////
// Tests for is3hourlyReportTime() and is6hourlyReportTime()
// Purpose: to confirm that the methods is3hourlyReportTime() and 
// is6hourlyReportTime() correctly identify the time for 3-hourly and 6-hourly
// reports as per FMH-1, section 12.4.
///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////
// Date tests
// Purpose: to confirm that dateBeforeRef() method correctly calculates the 
// date before provided reference date (e.g. to inher month and year of report
// release time from reference data).
// Note: month and year are not specified in METAR and TAF and must be inherred
// from some reference date, e.g. for recent report use current date.
///////////////////////////////////////////////////////////////////////////////

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
