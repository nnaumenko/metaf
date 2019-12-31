/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

//Format 1, e.g. BLSN1SC1SC1CI3

TEST(CloudTypesGroup, format1parseCumulonimbus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 5u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseToweringCumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("TCU3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::TOWERING_CUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseCumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CU2", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 2u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseCumulusFractus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CF1", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CUMULUS_FRACTUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 1u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseStratocumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("SC7", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 7u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseNimbostratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("NS8", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::NIMBOSTRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseStratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("ST8", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::STRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseStratusFractus) {
	const auto ctg = metaf::CloudTypesGroup::parse("SF4", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::STRATUS_FRACTUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 4u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseAltostratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("AS8", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ALTOSTRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseAltocumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("AC6", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ALTOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 6u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseAltocumulusCastellanus) {
	const auto ctg = metaf::CloudTypesGroup::parse("ACC6", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ALTOCUMULUS_CASTELLANUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 6u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseCirrus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CI3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CIRRUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseCirrostratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CS8", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CIRROSTRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseCirrocumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("CC5", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CIRROCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 5u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseBlowingSnow) {
	const auto ctg = metaf::CloudTypesGroup::parse("BLSN4", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::BLOWING_SNOW);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 4u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseBlowingDust) {
	const auto ctg = metaf::CloudTypesGroup::parse("BLDU3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::BLOWING_DUST);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseBlowingSand) {
	const auto ctg = metaf::CloudTypesGroup::parse("BLSA3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::BLOWING_SAND);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseIceCrystals) {
	const auto ctg = metaf::CloudTypesGroup::parse("IC2", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ICE_CRYSTALS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 2u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseRain) {
	const auto ctg = metaf::CloudTypesGroup::parse("RA3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::RAIN);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseDrizzle) {
	const auto ctg = metaf::CloudTypesGroup::parse("DZ1", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::DRIZZLE);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 1u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseSnow) {
	const auto ctg = metaf::CloudTypesGroup::parse("SN6", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::SNOW);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 6u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseIcePellets) {
	const auto ctg = metaf::CloudTypesGroup::parse("PL5", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ICE_PELLETS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 5u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseSmoke) {
	const auto ctg = metaf::CloudTypesGroup::parse("FU8", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::SMOKE);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseFog) {
	const auto ctg = metaf::CloudTypesGroup::parse("FG3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::FOG);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseMist) {
	const auto ctg = metaf::CloudTypesGroup::parse("BR3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::MIST);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseHaze) {
	const auto ctg = metaf::CloudTypesGroup::parse("HZ7", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::HAZE);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 7u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseSequenceTwoItems) {
	const auto ctg = metaf::CloudTypesGroup::parse("IC1SC3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 2u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ICE_CRYSTALS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(1)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(1)), 3u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseSequenceThreeItems) {
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
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseSequenceFourItems) {
	const auto ctg = metaf::CloudTypesGroup::parse("BLSN1SC1SC1CI3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 4u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::BLOWING_SNOW);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(1)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(1)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(2)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(2)), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(3)), 
		metaf::CloudTypesGroup::Type::CIRRUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(3)), 3u);
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseSequenceFiveItems) {
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
	EXPECT_FALSE(ctg->baseHeight().isReported());
}

TEST(CloudTypesGroup, format1parseSequenceTooLong) {
	const auto ctg = 
		metaf::CloudTypesGroup::parse("SC1SC1SC1SC1SC1SC1SC1SC1SC1", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 8u);
}

TEST(CloudTypesGroup, format1parseWrongReportPart) {
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("CB5", metaf::ReportPart::TAF).has_value());
}

TEST(CloudTypesGroup, format1parseWrongFormat) {
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

//Format 2, e.g. 8NS070

TEST(CloudTypesGroup, format2parseCumulonimbus) {
	const auto ctg = metaf::CloudTypesGroup::parse("1CB020", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CUMULONIMBUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 1u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 2000u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseToweringCumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("2TCU020", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::TOWERING_CUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 2u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 2000u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseCumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("4CU010", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 4u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 1000u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseStratocumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("5SC060", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::STRATOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 5u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 6000u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseNimbostratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("8NS070", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::NIMBOSTRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 7000u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseStratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("5ST007", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::STRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 5u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 700u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseAltostratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("8AS100", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ALTOSTRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 8u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 10000u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseAltocumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("3AC070", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::ALTOCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 7000u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseCirrus) {
	const auto ctg = metaf::CloudTypesGroup::parse("5CI230", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CIRRUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 5u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 23000u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseCirrostratus) {
	const auto ctg = metaf::CloudTypesGroup::parse("3CS220", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CIRROSTRATUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 22000u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseCirrocumulus) {
	const auto ctg = metaf::CloudTypesGroup::parse("3CC210", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg.has_value());
	const auto ctv = ctg->toVector();
	ASSERT_EQ(ctv.size(), 1u);
	EXPECT_EQ(std::get<metaf::CloudTypesGroup::Type>(ctv.at(0)), 
		metaf::CloudTypesGroup::Type::CIRROCUMULUS);
	EXPECT_EQ(std::get<unsigned int>(ctv.at(0)), 3u);
	EXPECT_TRUE(ctg->baseHeight().isInteger());
	ASSERT_TRUE(ctg->baseHeight().integer().has_value());
	EXPECT_EQ(ctg->baseHeight().integer().value(), 21000u);
	EXPECT_EQ(ctg->baseHeight().unit(), metaf::Distance::Unit::FEET);
}

TEST(CloudTypesGroup, format2parseWrongReportPart) {
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("1CB020", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("1CB020", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("1CB020", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(metaf::CloudTypesGroup::parse("1CB020", metaf::ReportPart::TAF).has_value());
}

TEST(CloudTypesGroup, format2parseWrongFormat) {
	//TODO
}

// Miscellaneous

TEST(CloudTypesGroup, append) {
	auto ctg1 = metaf::CloudTypesGroup::parse("AC4CI6", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg1.has_value());

	auto ctg2 = metaf::CloudTypesGroup::parse("SC3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg2.has_value());

	auto ctg3 = metaf::CloudTypesGroup::parse("3CU012", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg3.has_value());

	EXPECT_EQ(ctg1->append("AC1CI6", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg1->append("SC3", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg1->append("6SC009", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg1->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg1->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ctg2->append("AC1CI6", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg2->append("SC3", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg2->append("6SC009", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg2->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg2->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);

	EXPECT_EQ(ctg3->append("AC1CI6", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg3->append("SC3", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg3->append("6SC009", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg3->append("RMK", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
	EXPECT_EQ(ctg3->append("TEST", metaf::ReportPart::RMK), 
		metaf::AppendResult::NOT_APPENDED);
}

TEST(CloudTypesGroup, isValid) {
	const auto ctg1 = metaf::CloudTypesGroup::parse("AC4CI6", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg1.has_value());
	EXPECT_TRUE(ctg1->isValid());

	const auto ctg2 = metaf::CloudTypesGroup::parse("SC3", metaf::ReportPart::RMK);
	ASSERT_TRUE(ctg2.has_value());
	EXPECT_TRUE(ctg2->isValid());
}