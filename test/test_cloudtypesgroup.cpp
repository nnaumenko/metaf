/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

TEST(CloudTypesGroup, parseCumulonimbus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 5u);
}

TEST(CloudTypesGroup, parseToweringCumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("TCU3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::TOWERING_CUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
}

TEST(CloudTypesGroup, parseCumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CU2", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 2u);
}

TEST(CloudTypesGroup, parseCumulusFractus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CF1", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CUMULUS_FRACTUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 1u);
}

TEST(CloudTypesGroup, parseStratocumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("SC7", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 7u);
}

TEST(CloudTypesGroup, parseNimbostratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("NS8", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::NIMBOSTRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
}

TEST(CloudTypesGroup, parseStratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("ST8", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::STRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
}

TEST(CloudTypesGroup, parseStratusFractus) {
	const auto ctg = metaf::CloudTypesGroup::parse("SF4", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::STRATUS_FRACTUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 4u);
}

TEST(CloudTypesGroup, parseAltostratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("AS8", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ALTOSTRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
}

TEST(CloudTypesGroup, parseAltocumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("AC6", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ALTOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 6u);
}

TEST(CloudTypesGroup, parseAltocumulusCastellanus) {
	const auto ctg = metaf::CloudTypesGroup::parse("ACC6", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ALTOCUMULUS_CASTELLANUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 6u);
}

TEST(CloudTypesGroup, parseCirrus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CI3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CIRRUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
}

TEST(CloudTypesGroup, parseCirrostratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CS8", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CIRROSTRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
}

TEST(CloudTypesGroup, parseCirrocumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CC5", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CIRROCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 5u);
}

TEST(CloudTypesGroup, parseSequenceTwoItems) {
	const auto ctg = metaf::CloudTypesGroup::parse("AC1CI1", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 2u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ALTOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(1)), 
		metaf::CloudTypesGroup::Type::CIRRUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(1)), 1u);
}

TEST(CloudTypesGroup, parseSequenceThreeItems) {
	const auto ctg = metaf::CloudTypesGroup::parse("SC1AC6CI1", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 3u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(1)), 
		metaf::CloudTypesGroup::Type::ALTOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(1)), 6u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(2)), 
		metaf::CloudTypesGroup::Type::CIRRUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(2)), 1u);
}

TEST(CloudTypesGroup, parseSequenceFourItems) {
	const auto ctg = metaf::CloudTypesGroup::parse("SC1SC1SC3AC2", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 4u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(1)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(1)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(2)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(2)), 3u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(3)), 
		metaf::CloudTypesGroup::Type::ALTOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(3)), 2u);
}

TEST(CloudTypesGroup, parseSequenceFiveItems) {
	const auto ctg = metaf::CloudTypesGroup::parse("CF1CU3SC1CS1CI2", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 5u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CUMULUS_FRACTUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(1)), 
		metaf::CloudTypesGroup::Type::CUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(1)), 3u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(2)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(2)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(3)), 
		metaf::CloudTypesGroup::Type::CIRROSTRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(3)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(4)), 
		metaf::CloudTypesGroup::Type::CIRRUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(4)), 2u);
}

TEST(CloudTypesGroup, parseSequenceTooLong) {
	const auto ctg = 
		metaf::CloudTypesGroup::parse("SC1SC1SC1SC1SC1SC1SC1SC1SC1", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 8u);
}

TEST(CloudTypesGroup, parseWrongReportPart) {
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::TAF).has_value());
}

TEST(CloudTypesGroup, parseWrongFormat) {
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("AB5", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("AC0", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("AC9", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("AC/", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("//7", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("ACX", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("AB5CF2CI3", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CF2AB5CI3", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CF2CI3AB5", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CF2/CI3", metaf::ReportPart::RMK).has_value());
}
