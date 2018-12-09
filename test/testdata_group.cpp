/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "testdata_group.h"

const testdata::GroupTestData::ReportParts testdata::GroupTestData::allReportParts = {
	metaf::ReportPart::UNKNOWN, 
	metaf::ReportPart::HEADER,
	metaf::ReportPart::METAR,
	metaf::ReportPart::TAF,
	metaf::ReportPart::RMK
};

const std::vector<testdata::GroupTestData> testdata::groupDataSet = {

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("AAAAAAAA"),	//Generic plain text group
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("AAAAAAAA")
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("METAR"),
		{metaf::ReportPart::HEADER},
		metaf::FixedGroup(metaf::FixedGroup::Type::METAR)
	},
	{
		std::string("SPECI"),
		{metaf::ReportPart::HEADER},
		metaf::FixedGroup(metaf::FixedGroup::Type::SPECI)
	},
	{
		std::string("TAF"),
		{metaf::ReportPart::HEADER},
		metaf::FixedGroup(metaf::FixedGroup::Type::TAF)
	},
	{
		std::string("AMD"),
		{metaf::ReportPart::HEADER},
		metaf::FixedGroup(metaf::FixedGroup::Type::AMD)
	},
	{
		std::string("NIL"),
		{metaf::ReportPart::HEADER, metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::FixedGroup(metaf::FixedGroup::Type::NIL)
	},
	{
		std::string("CNL"),
		{metaf::ReportPart::HEADER, metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::FixedGroup(metaf::FixedGroup::Type::CNL)
	},
	{
		std::string("COR"),
		{metaf::ReportPart::HEADER, metaf::ReportPart::METAR},
		metaf::FixedGroup(metaf::FixedGroup::Type::COR)
	},
	{
		std::string("AUTO"),
		{metaf::ReportPart::METAR},
		metaf::FixedGroup(metaf::FixedGroup::Type::AUTO)
	},
	{
		std::string("CLR"),
		{metaf::ReportPart::METAR},
		metaf::FixedGroup(metaf::FixedGroup::Type::CLR)
	},
	{
		std::string("NCD"),
		{metaf::ReportPart::METAR},
		metaf::FixedGroup(metaf::FixedGroup::Type::NCD)
	},
	{
		std::string("NOSIG"),
		{metaf::ReportPart::METAR},
		metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
	},
	{
		std::string("SNOCLO"),
		{metaf::ReportPart::METAR},
		metaf::FixedGroup(metaf::FixedGroup::Type::AIRPORT_SNOCLO)
	},
	{
		std::string("R/SNOCLO"),
		{metaf::ReportPart::METAR},
		metaf::FixedGroup(metaf::FixedGroup::Type::AIRPORT_SNOCLO)
	},
	{
		std::string("CAVOK"),
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::FixedGroup(metaf::FixedGroup::Type::CAVOK)
	},
	{
		std::string("NSC"),
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::FixedGroup(metaf::FixedGroup::Type::NSC)
	},
	{
		std::string("NSW"),
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::FixedGroup(metaf::FixedGroup::Type::NSW)
	},
	{
		std::string("TEMPO"),
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::FixedGroup(metaf::FixedGroup::Type::TEMPO)
	},
	{
		std::string("BECMG"),
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::FixedGroup(metaf::FixedGroup::Type::BECMG)
	},
	{
		std::string("INTER"),
		{metaf::ReportPart::TAF},
		metaf::FixedGroup(metaf::FixedGroup::Type::INTER)
	},
	{
		std::string("RMK"),
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::FixedGroup(metaf::FixedGroup::Type::RMK)
	},
	{
		std::string("SKC"),
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::FixedGroup(metaf::FixedGroup::Type::SKC)
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("EGLL"), //London Heathrow International Airport
		{metaf::ReportPart::HEADER},
		metaf::LocationGroup("EGLL")
	},
	{
		std::string("K2J3"), //K2J3 = Louisville Municipal Airport
		{metaf::ReportPart::HEADER},
		metaf::LocationGroup("K2J3")
	},
	{
		std::string("2AAA"), //ICAO code cannot begin with a number
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("2AAA")
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("201730Z"),	//Day-of-month 20, time 17:30 GMT
		{metaf::ReportPart::HEADER},
		metaf::ReportTimeGroup(20, 17, 30)
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("1221/1303"),	//From day 12 21:00 till day 13 03:00
		{metaf::ReportPart::HEADER, metaf::ReportPart::TAF},
		metaf::TimeSpanGroup(12,21,13,3)
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("FM191445"),	//from day 19 14:45
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::TrendTimeGroup(metaf::TrendTimeGroup::Type::FROM, 19, 14, 45),
	},
	{
		std::string("TL010300"),	//till day 1, 03:00
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::TrendTimeGroup(metaf::TrendTimeGroup::Type::TILL, 1, 3, 0),
	},
	{
		std::string("AT121530"),	//at day 12 15:30
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::TrendTimeGroup(metaf::TrendTimeGroup::Type::AT, 12, 15, 30),
	},
	{
		std::string("FM1445"),	//from 14:45
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::TrendTimeGroup(metaf::TrendTimeGroup::Type::FROM, 14, 45),
	},
	{
		std::string("TL0300"),	//till 03:00
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::TrendTimeGroup(metaf::TrendTimeGroup::Type::TILL, 3, 0),
	},
	{
		std::string("AT1530"),	//at 15:30
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::TrendTimeGroup(metaf::TrendTimeGroup::Type::AT, 15, 30),
	},
	{
		std::string("FF051025"),	//type can only be FM, TL, and AT
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("FF051025"),
	},
	{
		std::string("FF1025"),	//type can only be FM, TL, and AT
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("FF1025"),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("PROB30"),	//probability 30%
		{metaf::ReportPart::TAF},
		metaf::ProbabilityGroup(30),
	},
	{
		std::string("PROB40"),	//probability 40%
		{metaf::ReportPart::TAF},
		metaf::ProbabilityGroup(40),
	},
	{
		std::string("PROB35"),	//probability can be only 30% or 40%
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("PROB35"),
	},
	{
		std::string("PROB20"),	//probability can be only 30% or 40%
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("PROB20"),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("18005G10KT"),	//direction 180 (South), speed 5 knots, gusts 10 knots
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindGroup(180, metaf::SpeedUnit::KNOTS, 5, 10),
	},
	{
		std::string("18005G10MPS"),	//direction 180 (South), speed 5 m/s, gusts 10 m/s
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindGroup(180, metaf::SpeedUnit::METERS_PER_SECOND, 5, 10),
	},
	{
		std::string("18005G10KMH"),	//direction 180 (South), speed 5 km/h, gusts 10 km/h
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindGroup(180, metaf::SpeedUnit::KILOMETERS_PER_HOUR, 5, 10),
	},
	{
		std::string("08063G112KT"),	//direction 80 (East), speed 63 knots, gusts 112 knots
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindGroup(80, metaf::SpeedUnit::KNOTS, 63, 112),
	},
	{
		std::string("080100G150KT"),//direction 80 (East), speed 100 knots, gusts 150 knots
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindGroup(80, metaf::SpeedUnit::KNOTS, 100, 150),
	},
	{
		std::string("18005KT"),	//direction 180 (South), speed 5 knots, no gusts
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindGroup(180, metaf::SpeedUnit::KNOTS, 5),
	},
	{
		std::string("VRB15G25KT"),	//variable direction, speed 15 knots, gusts 25 knots
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindGroup::makeVariableDirection(metaf::SpeedUnit::KNOTS, 15, 25),
	},
	{
		std::string("VRB06KT"),	//variable direction, speed 6 knots no gusts
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindGroup::makeVariableDirection(metaf::SpeedUnit::KNOTS, 6),
	},	{
		std::string("/////KT"),	//direction and speed not reported
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindGroup(metaf::SpeedUnit::KNOTS),
	},
	{
		std::string("18005G10KKT"),	//unit can only be KT, MPS, or KMH
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("18005G10KKT"),
	},
	{
		std::string("18206KT"),	//direction's last digit must be zero
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("18206KT"),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("180V240"),	//variable direction from 180 (south) to 240 (west)
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VarWindGroup(180, 240),
	},
	{
		std::string("182V240"),	//direction's last digit must be zero
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("182V240"),
	},
	{
		std::string("180V242"),	//direction's last digit must be zero
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("180V242"),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("WS020/05065KT"),	//wind shear at 2000 feet: wind 50 degrees & 65 knots
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindShearGroup(2000, 50, 65, metaf::SpeedUnit::KNOTS),
	},
	{
		std::string("WS020/05065MPS"),	//wind shear at 2000 feet: wind 50 degrees & 65 m/s
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindShearGroup(2000, 50, 65, metaf::SpeedUnit::METERS_PER_SECOND),
	},
	{
		std::string("WS020/05065KMH"),	//wind shear at 2000 feet: wind 50 degrees & 65 km/h
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WindShearGroup(2000, 50, 65, metaf::SpeedUnit::KILOMETERS_PER_HOUR),
	},
	{
		std::string("WS020/05165KT"),	//direction's last digit must be zero
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("WS020/05165KT"),
	},
	{
		std::string("WS020/05065KKT"),	//wind speed unit must be KT, MPS or KMH
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("WS020/05065KKT"),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("1600"),	//visibility 1600 meters
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMeters(1600),
	},
	{
		std::string("0100"),	//visibility 100 meters
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMeters(100),
	},
	{
		std::string("0050"),	//visibility 50 meters
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMeters(50),
	},
	{
		std::string("4000W"),	//visibility 4000 meters to the west
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMeters(4000,
			metaf::VisibilityGroup::Direction::W),
	},
	{
		std::string("0000E"),	//zero visibility to the east
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMeters(0,
			metaf::VisibilityGroup::Direction::E),
	},
	{
		std::string("9999NDV"),	//visibility more than 10km, no directional visibility
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMeters(9999, 
			metaf::VisibilityGroup::Direction::NDV),
	},
	{
		std::string("////"),	//visibility not reported (but distance unit is meters)
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMeters(),
	},
	{
		std::string("3SM"),	//visibility 3 statute miles
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMiles(3),
	},
	{
		std::string("15SM"),	//visibility 15 statute miles
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMiles(15),
	},
	{
		std::string("1/16SM"),	//visibility 1/16 of statute mile
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMiles(0, 1, 16),
	},
	{
		std::string("M1/8SM"),	//visibility less than 1/8 of statute mile
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMiles(0, 1, 8, metaf::ValueModifier::LESS_THAN),
	},
	{
		std::string("11/2SM"),	//visibility 1 1/2 statute miles
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMiles(1, 1, 2),
	},
	{
		std::string("////SM"),	//visibility not reported (but distance unit is statute miles)
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMiles(),
	},
	{
		std::string("1"),	//incomplete group, integer part of visibility 1 statute mile
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMilesIncomplete(1),
	},
	{
		std::string("2"),	//incomplete group, integer part of visibility 1 statute mile
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VisibilityGroup::makeVisibilityMilesIncomplete(2),
	},
	{
		std::string("3"),	//only 1 and 2 are accepted as incomplete integer part
		testdata::GroupTestData::allReportParts,
		metaf::PlainTextGroup("3"),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("FEW040"),	//few clouds at 4000 feet
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
	},
	{
		std::string("SCT300"),	//scattered clouds at 30000 feet
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::CloudGroup(metaf::CloudGroup::Amount::SCATTERED, 30000),
	},
	{
		std::string("BKN370"),	//scattered clouds at 37000 feet
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::CloudGroup(metaf::CloudGroup::Amount::BROKEN, 37000),
	},
	{
		std::string("OVC001"),	//overcast, clouds at 100 feet
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::CloudGroup(metaf::CloudGroup::Amount::OVERCAST, 100),
	},
	{
		std::string("FEW020TCU"),	//few clouds at 2000 feet, towering cumulus
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 
			2000, 
			metaf::CloudGroup::Type::TOWERING_CUMULUS),
	},
	{
		std::string("BKN012CB"),	//few clouds at 1200 feet
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::CloudGroup(metaf::CloudGroup::Amount::BROKEN, 
			1200, 
			metaf::CloudGroup::Type::CUMULONIMBUS),
	},
	{
		std::string("SCT016///"),	//scattered clouds of non-reported type at 1600 feet
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::CloudGroup(metaf::CloudGroup::Amount::SCATTERED, 
			1600, 
			metaf::CloudGroup::Type::NOT_REPORTED),
	},
	{
		std::string("///074///"),
		//non-reported amount of clouds of non-reported type at 7400 feet
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::CloudGroup(metaf::CloudGroup::Amount::NOT_REPORTED, 
			7400, 
			metaf::CloudGroup::Type::NOT_REPORTED),
	},
	{
		std::string("//////TCU"), 
		//non-reported amount of towering cumulus clouds, base height not reported
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::CloudGroup(metaf::CloudGroup::Amount::NOT_REPORTED, 
			metaf::CloudGroup::Type::TOWERING_CUMULUS),
	},
	{
		std::string("/////////"), //no cloud information reported
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::CloudGroup(metaf::CloudGroup::Amount::NOT_REPORTED, 
			metaf::CloudGroup::Type::NOT_REPORTED),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("VV016"), //vertical visibility 1600 feet
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VerticalVisibilityGroup(1600),
	},
	{
		std::string("VV001"), //vertical visibility 100 feet
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VerticalVisibilityGroup(100),
	},
	{
		std::string("VV000"), //vertical visibility less than 100 feet
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VerticalVisibilityGroup(0),
	},
	{
		std::string("VV///"), //vertical visibility not reported
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::VerticalVisibilityGroup(),
	},

	///////////////////////////////////////////////////////////////////////////////
	{
		std::string("MIFG"), //shallow fog
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SHALLOW,
			metaf::WeatherGroup::Weather::FOG),
	},
	{
		std::string("PRFG"), //partial fog
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::PARTIAL,
			metaf::WeatherGroup::Weather::FOG),
	},
	{
		std::string("BCFG"), //patches of fog
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::PATCHES,
			metaf::WeatherGroup::Weather::FOG),
	},
	{
		std::string("DRSA"), //low drifting sand
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::LOW_DRIFTING,
			metaf::WeatherGroup::Weather::SAND),
	},
	{
		std::string("DRDU"), //low drifting dust
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::LOW_DRIFTING,
			metaf::WeatherGroup::Weather::DUST),
	},
	{
		std::string("DRSN"), //low drifting snow
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::LOW_DRIFTING,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("BLDU"), //blowing dust
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::BLOWING,
			metaf::WeatherGroup::Weather::DUST),
	},
	{
		std::string("BLSA"), //blowind sand
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::BLOWING,
			metaf::WeatherGroup::Weather::SAND),
	},
	{
		std::string("BLSN"), //blowind snow
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::BLOWING,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("BLPY"), //blowind spray
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::BLOWING,
			metaf::WeatherGroup::Weather::SPRAY),
	},
	{
		std::string("SHRA"), //rain showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("SHSN"), //snow showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("SHPL"), //ice pellets showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::ICE_PELLETS),
	},
	{
		std::string("SHGS"), //small hail showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::SMALL_HAIL),
	},
	{
		std::string("SHGR"), //hail showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::HAIL),
	},
	{
		std::string("SHRAGS"), //hail & rain showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::RAIN,
			metaf::WeatherGroup::Weather::SMALL_HAIL),
	},
	{
		std::string("SHRASN"), //rain & snow showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::RAIN,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("TS"), //thunderstorm
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::THUNDERSTORM),
	},
	{
		std::string("TSRA"), //thunderstorm with rain
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::THUNDERSTORM,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("TSSN"), //thunderstorm with snow
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::THUNDERSTORM,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("TSPL"), //thunderstorm with ice pellets
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::THUNDERSTORM,
			metaf::WeatherGroup::Weather::ICE_PELLETS),
	},
	{
		std::string("TSGS"), //thunderstorm with small hail
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::THUNDERSTORM,
			metaf::WeatherGroup::Weather::SMALL_HAIL),
	},
	{
		std::string("TSGR"), //thunderstorm with hail
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::THUNDERSTORM,
			metaf::WeatherGroup::Weather::HAIL),
	},
	{
		std::string("TSRAGR"), //thunderstorm with rain and hail
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::THUNDERSTORM,
			metaf::WeatherGroup::Weather::RAIN,
			metaf::WeatherGroup::Weather::HAIL),
	},
	{
		std::string("TSSNGS"), //thunderstorm with snow and snow pellets (small hail)
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::THUNDERSTORM,
			metaf::WeatherGroup::Weather::SNOW,
			metaf::WeatherGroup::Weather::SMALL_HAIL),
	},
	{
		std::string("FZFG"), //freezing fog
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::FREEZING,
			metaf::WeatherGroup::Weather::FOG),
	},
	{
		std::string("FZRA"), //freezing rain
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::FREEZING,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("FZRASN"), //freezing rain and snow
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::FREEZING,
			metaf::WeatherGroup::Weather::RAIN,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("FZDZ"), //freezing drizzle
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::FREEZING,
			metaf::WeatherGroup::Weather::DRIZZLE),
	},
	{
		std::string("DZ"), //drizzle
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::DRIZZLE),
	},
	{
		std::string("DZRA"), //drizzle and rain
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::DRIZZLE,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("RA"), //rain
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("SN"), //snow
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("SG"), //snow grains
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SNOW_GRAINS),
	},
	{
		std::string("IC"), //ice crystals
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::ICE_CRYSTALS),
	},
	{
		std::string("UP"), //unknown precipitation
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::UNDETERMINED),
	},
	{
		std::string("BR"), //mist
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::MIST),
	},
	{
		std::string("FU"), //smoke
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SMOKE),
	},
	{
		std::string("VA"), //volcanic ash
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::VOLCANIC_ASH),
	},
	{
		std::string("DU"), //widespread dust
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::DUST),
	},
	{
		std::string("SA"), //widespread sand
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SAND),
	},
	{
		std::string("HZ"), //haze
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::HAZE),
	},
	{
		std::string("PO"), //dust whirls
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::DUST_WHIRLS),
	},
	{
		std::string("SQ"), //squalls
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SQUALLS),
	},
	{
		std::string("FC"), //funnel cloud
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::FUNNEL_CLOUD),
	},
	{
		std::string("DS"), //dust storm
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::DUSTSTORM),
	},
	{
		std::string("SS"), //sand storm
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::SANDSTORM),
	},
	{
		std::string("DSSS"), //dust and sand storm
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::DUSTSTORM,
			metaf::WeatherGroup::Weather::SANDSTORM),
	},
	{
		std::string("-FZDZ"), //light freezing drizzle
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::LIGHT,
			metaf::WeatherGroup::Weather::FREEZING,
			metaf::WeatherGroup::Weather::DRIZZLE),
	},
	{
		std::string("-RA"), //light rain
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::LIGHT,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("-SN"), //light snow
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::LIGHT,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("-SHSG"), //light snow grains showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::LIGHT,
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::SNOW_GRAINS),
	},
	{
		std::string("-SHRA"), //light rain showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::LIGHT,
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("-TSRA"), //thunderstorm with light rain
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::LIGHT,
			metaf::WeatherGroup::Weather::THUNDERSTORM,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("-SHRASN"), //light rain and snow showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::LIGHT,
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::RAIN,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("+DZ"), //heavy drizzle
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
			metaf::WeatherGroup::Weather::DRIZZLE),
	},
	{
		std::string("+RA"), //heavy rain
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("+SN"), //heavy snow
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("+SHRA"), //heavy rain showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("+TSRA"), //thunderstorm with heavy rain
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
			metaf::WeatherGroup::Weather::THUNDERSTORM,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("+SHRASN"), //heavy rain and snow showers
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
			metaf::WeatherGroup::Weather::SHOWERS,
			metaf::WeatherGroup::Weather::RAIN,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("+FC"), //tornado or waterspout
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
			metaf::WeatherGroup::Weather::FUNNEL_CLOUD),
	},
	{
		std::string("+DS"), //heavy dust storm
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
			metaf::WeatherGroup::Weather::DUSTSTORM),
	},
	{
		std::string("+SS"), //heavy sand storm
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
			metaf::WeatherGroup::Weather::SANDSTORM),
	},
	{
		std::string("+DSSS"), //heavy dust and sand storm
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
			metaf::WeatherGroup::Weather::DUSTSTORM,
			metaf::WeatherGroup::Weather::SANDSTORM),
	},
	{
		std::string("VCTS"), //thunderstorm in vicinity
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY,
			metaf::WeatherGroup::Weather::THUNDERSTORM),
	},
	{
		std::string("VCTSRA"), //thunderstorm with rain in vicinity
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::THUNDERSTORM,
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("VCFG"), //fog in vicinity
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::FOG),
	},
	{
		std::string("VCSH"), //showers in vicinity or distant precipitation
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::SHOWERS),
	},
	{
		std::string("VCPO"), //dust whirls in vicinity
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::DUST_WHIRLS),
	},
	{
		std::string("VCBLDU"), //blowind dust in vicinity
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::BLOWING,
			metaf::WeatherGroup::Weather::DUST),
	},
	{
		std::string("VCBLSA"), //blowind sand in vicinity
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::BLOWING,
			metaf::WeatherGroup::Weather::SAND),
	},
	{
		std::string("VCBLSN"), //blowind snow in vicinity
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::BLOWING,
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("VCDS"), //dust storm in vicinity
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::DUSTSTORM),
	},
	{
		std::string("VCSS"), //sand storm in vicinity
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::SANDSTORM),
	},
	{
		std::string("VCVA"), //volcanic ash in vicinity
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY, 
			metaf::WeatherGroup::Weather::VOLCANIC_ASH),
	},
	{
		std::string("RESN"), //recent weather: snow
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::RECENT, 
			metaf::WeatherGroup::Weather::SNOW),
	},
	{
		std::string("RETSRA"), //recent weather: thunderstorm and rain
		{metaf::ReportPart::METAR, metaf::ReportPart::TAF},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::RECENT, 
			metaf::WeatherGroup::Weather::THUNDERSTORM, 
			metaf::WeatherGroup::Weather::RAIN),
	},
	{
		std::string("//"), //weather not reported
		{metaf::ReportPart::METAR},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
			metaf::WeatherGroup::Weather::NOT_REPORTED),
	},
	{
		std::string("RE//"), //recent weather not reported
		{metaf::ReportPart::METAR},
		metaf::WeatherGroup(metaf::WeatherGroup::Prefix::RECENT, 
			metaf::WeatherGroup::Weather::NOT_REPORTED),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("25/18"), //temperature 25 centigrade and dew point 18 centigrade
		{metaf::ReportPart::METAR},
		metaf::TemperatureGroup(25, 18),
	},
	{
		std::string("05/M02"), //temperature 5 centigrade and dew point -2 centigrade
		{metaf::ReportPart::METAR},
		metaf::TemperatureGroup(5, -2),
	},
	{
		std::string("M02/M04"), //temperature -2 centigrade and dew point -4 centigrade
		{metaf::ReportPart::METAR},
		metaf::TemperatureGroup(-2, -4),
	},
	{
		std::string("M70/"), //temperature -70 centigrade and dew point not reported
		{metaf::ReportPart::METAR},
		metaf::TemperatureGroup(-70),
	},
	{
		std::string("25///"), //temperature 25 centigrade and dew point not reported
		{metaf::ReportPart::METAR},
		metaf::TemperatureGroup(25),
	},
	{
		std::string("/////"), //temperature and dew point not reported
		{metaf::ReportPart::METAR},
		metaf::TemperatureGroup(),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("TX03/0620Z"), //Max temperature 03 centigrade on day 6, 20:00 GMT
		{metaf::ReportPart::TAF},
		metaf::MinMaxTemperatureGroup::makeMax(3, 6, 20),
	},
	{
		std::string("TXM01/1402Z"), //Max temperature 01 centigrade on day 4, 2:00 GMT
		{metaf::ReportPart::TAF},
		metaf::MinMaxTemperatureGroup::makeMax(-1, 14, 02),
	},
	{
		std::string("TN13/1204Z"), //Min temperature 13 centigrade on day 12, 4:00 GMT
		{metaf::ReportPart::TAF},
		metaf::MinMaxTemperatureGroup::makeMin(13, 12, 4),
	},
	{
		std::string("TNM02/0709Z"), //Min temperature -2 centigrade on day 7, 9:00 GMT
		{metaf::ReportPart::TAF},
		metaf::MinMaxTemperatureGroup::makeMin(-2, 7, 9),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("A2724"), //Atmospheric pressure 27.24 inHg
		{metaf::ReportPart::METAR},
		metaf::PressureGroup(27.24, metaf::PressureGroup::Unit::INCHES_HG),
	},
	{
		std::string("A////"), //Atmospheric pressure not reported (but unit is inHg)
		{metaf::ReportPart::METAR},
		metaf::PressureGroup(metaf::PressureGroup::Unit::INCHES_HG),
	},
	{
		std::string("Q1033"), //Atmospheric pressure 1033 hectopascal
		{metaf::ReportPart::METAR},
		metaf::PressureGroup(1033, metaf::PressureGroup::Unit::HECTOPASCAL),
	},
	{
		std::string("Q////"), //Atmospheric pressure not reported (but unit is hectopascal)
		{metaf::ReportPart::METAR},
		metaf::PressureGroup(metaf::PressureGroup::Unit::HECTOPASCAL),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("R05/1100"), //Runway 05 visual range 1100 meters
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(5),
			1100, metaf::DistanceUnit::METERS),
	},
	{
		std::string("R05L/1100"), //Runway 05LEFT visual range 1100 meters
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(5, metaf::Runway::Designator::LEFT),
			1100, metaf::DistanceUnit::METERS),
	},
	{
		std::string("R05C/1100"), //Runway 05CENTER visual range 1100 meters
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(5, metaf::Runway::Designator::CENTER),
			1100, metaf::DistanceUnit::METERS),
	},
	{
		std::string("R05R/1100"), //Runway 05RIGHT visual range 1100 meters
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(5, metaf::Runway::Designator::RIGHT),
			1100, metaf::DistanceUnit::METERS),
	},
	{
		std::string("R04/P2000"), //Runway 04 visual range more than 2000 meters
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(4),
			2000, metaf::DistanceUnit::METERS,
			metaf::ValueModifier::MORE_THAN),
	},
	{
		std::string("R21/M0200"), //Runway 04 visual range less than 200 meters
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(21),
			200, metaf::DistanceUnit::METERS,
			metaf::ValueModifier::LESS_THAN),
	},
	{
		std::string("R11/4500FT"), //Runway 11 visual range 4500 feet
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(11),
			4500, metaf::DistanceUnit::FEET),
	},
	{
		std::string("R11/P6000FT"), //Runway 11 visual range more than 6000 feet
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(11),
			6000, metaf::DistanceUnit::FEET,
			metaf::ValueModifier::MORE_THAN),
	},
	{
		std::string("R12/M0400FT"), //Runway 12 visual range less than 400 feet
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(12),
			400, metaf::DistanceUnit::FEET,
			metaf::ValueModifier::LESS_THAN),
	},
	{
		std::string("R35/5500FT/D"), //Runway 35 visual range 5500 feet and trend is downward
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(35),
			5500, metaf::DistanceUnit::FEET,
			metaf::ValueModifier::NONE,
			metaf::RunwayVisualRangeGroup::Trend::DOWNWARD),
	},
	{
		std::string("R02/5000FT/N"), //Runway 02 visual range 5000 feet and trend is neutral
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(2),
			5000, metaf::DistanceUnit::FEET,
			metaf::ValueModifier::NONE,
			metaf::RunwayVisualRangeGroup::Trend::NEUTRAL),
	},
	{
		std::string("R12/P6000FT/U"), //Runway 12 visual range more than 6000 feet 
		//and trend is upward
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(12),
			6000, metaf::DistanceUnit::FEET,
			metaf::ValueModifier::MORE_THAN,
			metaf::RunwayVisualRangeGroup::Trend::UPWARD),
	},
	{
		std::string("R14/P1500N"), //Runway 14 visual range more than 1500 meters 
		//and trend is neutral
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(14),
			1500, metaf::DistanceUnit::METERS,
			metaf::ValueModifier::MORE_THAN,
			metaf::RunwayVisualRangeGroup::Trend::NEUTRAL),
	},
	{
		std::string("R17/0250V0450N"), //Runway 17 visual range from 250 to 450 meters 
		//and trend is neutral
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(17),
			250, 450, metaf::DistanceUnit::METERS,
			metaf::ValueModifier::NONE,
			metaf::ValueModifier::NONE,
			metaf::RunwayVisualRangeGroup::Trend::NEUTRAL),
	},
	{
		std::string("R34L/M0050VP2000U"), //Runway 34LEFT visual range from less than 
		//50 meters to more than 2000 meters and trend is upward
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(34, metaf::Runway::Designator::LEFT),
			50, 2000, metaf::DistanceUnit::METERS,
			metaf::ValueModifier::LESS_THAN,
			metaf::ValueModifier::MORE_THAN,
			metaf::RunwayVisualRangeGroup::Trend::UPWARD),
	},
	{
		std::string("R16L/0450VP2000U"), //Runway 16LEFT visual range from 450 meters to more 
		//than 2000 meters and trend is upward
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(16, metaf::Runway::Designator::LEFT),
			450, 2000, metaf::DistanceUnit::METERS,
			metaf::ValueModifier::NONE,
			metaf::ValueModifier::MORE_THAN,
			metaf::RunwayVisualRangeGroup::Trend::UPWARD),
	},
	{
		std::string("R07/2000V4500FT"), //Runway 07 visual range from 2000 to 4500 feet
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(7),
			2000, 4500, metaf::DistanceUnit::FEET,
			metaf::ValueModifier::NONE,
			metaf::ValueModifier::NONE),
	},
	{
		std::string("R30/5000VP6000FT/U"), //Runway 30 visual range from 5000 feet to more 
		//than 6000 feet and trend is upward
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(30),
			5000, 6000, metaf::DistanceUnit::FEET,
			metaf::ValueModifier::NONE,
			metaf::ValueModifier::MORE_THAN,
			metaf::RunwayVisualRangeGroup::Trend::UPWARD),
	},
	{
		std::string("R08/6000FT/"), //Runway 08 visual range 6000 feet (followed by 
		//trailing slash but no trend specified)
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(8),
			6000, metaf::DistanceUnit::FEET,
			metaf::ValueModifier::NONE),
	},
	{
		std::string("R25/2800VP6000FT/"), //Runway 25 visual range from 2800 feet to more
		//than 6000 feet (followed by trailing slash but no trend specified)
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(25),
			2800, 6000, metaf::DistanceUnit::FEET,
			metaf::ValueModifier::NONE,
			metaf::ValueModifier::MORE_THAN),
	},
	{
		std::string("R06R/////"), //Runway 06RIGHT visual range not reported (but the 
		//distance unit is meters)
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(6, metaf::Runway::Designator::RIGHT),
			metaf::DistanceUnit::METERS),
	},
	{
		std::string("R34/////N"), //Runway 34 visual range not reported (but the distance 
		//unit is meters) and trend is neutral
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(34),
			metaf::DistanceUnit::METERS,
			metaf::RunwayVisualRangeGroup::Trend::NEUTRAL),
	},
	{
		std::string("R34/////FT"), //Runway 34 visual range not reported (but the distance 
		//unit is feet)
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(34),
			metaf::DistanceUnit::FEET),
	},
	{
		std::string("R34/////FT/N"), //Runway 34 visual range not reported (but the distance 
		//unit is feet) and trend is neutral
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(34),
			metaf::DistanceUnit::FEET,
			metaf::RunwayVisualRangeGroup::Trend::NEUTRAL),
	},
	{
		std::string("R06R//////"), //Runway 06RIGHT visual range not reported (but the 
		//distance unit is meters) and trend is not reported
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(6, metaf::Runway::Designator::RIGHT),
			metaf::DistanceUnit::METERS),
	},
	{
		std::string("R34/////FT//"), //Runway 34 visual range not reported (but the distance 
		//unit is feet) and trend is not reported
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(34),
			metaf::DistanceUnit::FEET),
	},
	{
		std::string("R34/6000FTN"), //Runway 34 visual range is 6000 feet and trend is 
		//neutral, with slash before trend being omitted
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(34),
			6000, metaf::DistanceUnit::FEET,
			metaf::ValueModifier::NONE,
			metaf::RunwayVisualRangeGroup::Trend::NEUTRAL),
	},
	{
		std::string("R34/////FTN"), //Runway 34 visual range not reported (but the distance 
		//unit is feet) and trend is neutral, with slash before trend being omitted
		{metaf::ReportPart::METAR},
		metaf::RunwayVisualRangeGroup(metaf::Runway(34),
			metaf::DistanceUnit::FEET,
			metaf::RunwayVisualRangeGroup::Trend::NEUTRAL),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("R16R/290155"), //Runway 16RIGHT state: wet and water patches, more than
		//50% contaminated, deposits depth 1mm, friction coefficient 0.55
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(16, metaf::Runway::Designator::RIGHT),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES, 
			metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT,
			metaf::RunwayStateGroup::DepositDepth(1),
			metaf::RunwayStateGroup::SurfaceFriction(0.55)),
	},
	{
		std::string("R16R//90155"), //Runway 16RIGHT state: not reported, more than
		//50% contaminated, deposits depth 1mm, friction coefficient 0.55
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(16, metaf::Runway::Designator::RIGHT),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT,
			metaf::RunwayStateGroup::DepositDepth(1),
			metaf::RunwayStateGroup::SurfaceFriction(0.55)),
	},
	{
		std::string("R16R/2/0155"), //Runway 16RIGHT state: wet and water patches, 
		//contamination extent not reported, deposits depth 1mm, friction coefficient 0.55
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(16, metaf::Runway::Designator::RIGHT),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth(1),
			metaf::RunwayStateGroup::SurfaceFriction(0.55)),
	},
	{
		std::string("R16R/29//55"), //Runway 16RIGHT state: wet and water patches, more than
		//50% contaminated, deposits depth not reported, friction coefficient 0.55
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(16, metaf::Runway::Designator::RIGHT),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES, 
			metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction(0.55)),
	},
	{
		std::string("R16R/2901//"), //Runway 16RIGHT state: wet and water patches, more than
		//50% contaminated, deposits depth 1mm, friction coefficient not reported
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(16, metaf::Runway::Designator::RIGHT),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES, 
			metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT,
			metaf::RunwayStateGroup::DepositDepth(1),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R33///////"), //Runway 33 state: not reported
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(33),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R21/SNOCLO"), //Runway 16RIGHT state: closed due to snow accumulation
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup::makeSnoclo(metaf::Runway(21)),
	},
	{
		std::string("R88/CLRD70"), //All runways state: cleared, friction coefficient 0.70
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup::makeClrd(metaf::Runway(88),
			metaf::RunwayStateGroup::SurfaceFriction(0.70)),
	},
	{
		std::string("R88/0/////"), //All runways state: clear and dry
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::CLEAR_AND_DRY, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88/1/////"), //All runways state: damp
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::DAMP, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88/2/////"), //All runways state: wet and water patches
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88/3/////"), //All runways state: rime and frost covered
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::RIME_AND_FROST_COVERED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88/4/////"), //All runways state: dry snow
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::DRY_SNOW, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88/5/////"), //All runways state: wet snow
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::WET_SNOW, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88/6/////"), //All runways state: slush
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::SLUSH, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88/7/////"), //All runways state: ice
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::ICE, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88/8/////"), //All runways state: compacted or rolled snow
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::COMPACTED_OR_ROLLED_SNOW, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88/9/////"), //All runways state: frozen ruts or ridges
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::FROZEN_RUTS_OR_RIDGES, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88//1////"), //All runways state: contamination less than 10 
		//percent of runway
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::LESS_THAN_10_PERCENT,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88//2////"), //All runways state: contamination from 11 to 25 
		//percent of runway
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88//5////"), //All runways state: contamination from 26 to 50 
		//percent or runway
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::FROM_26_TO_50_PERCENT,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88//9////"), //All runways state: contamination of more than 51 
		//percent or runway
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88///90//"), //All runways state: deposits depth 90 mm
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth(90),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88///92//"), //All runways state: deposits depth 10 cm
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth(100),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88///93//"), //All runways state: deposits depth 15 cm
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth(150),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88///94//"), //All runways state: deposits depth 20 cm
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth(200),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88///95//"), //All runways state: deposits depth 25 cm
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth(250),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88///96//"), //All runways state: deposits depth 30 cm
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth(300),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88///97//"), //All runways state: deposits depth 10 cm
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth(350),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88///98//"), //All runways state: deposits depth 40 cm
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth(400),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88///99//"), //All runways state: runway not operational
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeRunwayNotOperational(),
			metaf::RunwayStateGroup::SurfaceFriction::makeNotReported()),
	},
	{
		std::string("R88/////91"), //All runways state: braking action poor
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction(
				metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR)),
	},
	{
		std::string("R88/////92"), //All runways state: braking action medium poor
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction(
				metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR)),
	},
	{
		std::string("R88/////93"), //All runways state: braking action medium
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction(
				metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM)),
	},
	{
		std::string("R88/////94"), //All runways state: braking action medium good
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction(
				metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_GOOD)),
	},
	{
		std::string("R88/////95"), //All runways state: braking action good
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction(
				metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::GOOD)),
	},
	{
		std::string("R88/////99"), //All runways state: surface friction unreliable
		{metaf::ReportPart::METAR},
		metaf::RunwayStateGroup(metaf::Runway(88),
			metaf::RunwayStateGroup::Status::NONE, 
			metaf::RunwayStateGroup::Deposits::NOT_REPORTED, 
			metaf::RunwayStateGroup::Extent::NOT_REPORTED,
			metaf::RunwayStateGroup::DepositDepth::makeNotReported(),
			metaf::RunwayStateGroup::SurfaceFriction::makeUnreliable()),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("RF02.7/010.5"), 
		//Rainfall for last 10 minutes 2.7mm, since 9:00AM 10.5mm
		{metaf::ReportPart::METAR},
		metaf::RainfallGroup(2.7, 10.5),
	},
	{
		std::string("RF21.5/112.4/031.8"), 
		//Rainfall for last 10 minutes 21.5mm, since 9:00AM 112.4mm, for last 60 minutes 31.8mm
		{metaf::ReportPart::METAR},
		metaf::RainfallGroup(21.5, 112.4, 31.8),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("W15/S4"), 
		//Sea surface temperature 15 centigrade, state of surface: moderate waves
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(15, metaf::SeaSurfaceGroup::StateOfSurface::MODERATE),
	},
	{
		std::string("W///S4"), 
		//Sea surface temperature not reported, state of surface: moderate waves
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::MODERATE),
	},
	{
		std::string("W///S/"), 
		//Sea surface temperature and state of surface not reported
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::NOT_REPORTED),
	},
	{
		std::string("W///S0"), 
		//Sea surface temperature not reported, state of surface: calm (glassy)
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::CALM_GLASSY),
	},
	{
		std::string("W///S1"), 
		//Sea surface temperature not reported, state of surface: calm (rippled)
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::CALM_RIPPLED),
	},
	{
		std::string("W///S2"), 
		//Sea surface temperature not reported, state of surface: smooth
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::SMOOTH),
	},
	{
		std::string("W///S3"), 
		//Sea surface temperature not reported, state of surface: slight waves
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::SLIGHT),
	},
	{
		std::string("W///S4"),
		//Sea surface temperature not reported, state of surface: moderate waves
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::MODERATE),
	},
	{
		std::string("W///S5"),
		//Sea surface temperature not reported, state of surface: rough
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::ROUGH),
	},
	{
		std::string("W///S6"), 
		//Sea surface temperature not reported, state of surface: very rough
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::VERY_ROUGH),
	},
	{
		std::string("W///S7"), 
		//Sea surface temperature not reported, state of surface: high waves
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::HIGH),
	},
	{
		std::string("W///S8"), 
		//Sea surface temperature not reported, state of surface: very high waves
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::VERY_HIGH),
	},
	{
		std::string("W///S9"), 
		//Sea surface temperature not reported, state of surface: wave height phenomenal
		{metaf::ReportPart::METAR},
		metaf::SeaSurfaceGroup(metaf::SeaSurfaceGroup::StateOfSurface::PHENOMENAL),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("W17/H23"), 
		//Sea surface temperature 17 centigrade, wave height 23 decimeters
		{metaf::ReportPart::METAR},
		metaf::SeaWavesGroup(17, 23),
	},
	{
		std::string("W17/H100"), 
		//Sea surface temperature 17 centigrade, wave height 100 decimeters
		{metaf::ReportPart::METAR},
		metaf::SeaWavesGroup(17, 100),
	},
	{
		std::string("W///H23"), 
		//Sea surface temperature not reported, wave height 23 decimeters
		{metaf::ReportPart::METAR},
		metaf::SeaWavesGroup::makeWaveHeight(23),
	},
	{
		std::string("W17/H//"), 
		//Sea surface temperature 17 centigrade, wave height not reported
		{metaf::ReportPart::METAR},
		metaf::SeaWavesGroup::makeTemperature(17),
	},
	{
		std::string("W17/H///"), 
		//Sea surface temperature 17 centigrade, wave height not reported
		{metaf::ReportPart::METAR},
		metaf::SeaWavesGroup::makeTemperature(17),
	},
	{
		std::string("W///H///"), 
		//Sea surface temperature and wave height not reported
		{metaf::ReportPart::METAR},
		metaf::SeaWavesGroup(),
	},

	///////////////////////////////////////////////////////////////////////////////

	{
		std::string("BLU"), //Colour code blue
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::BLUE),
	},
	{
		std::string("WHT"), //Colour code white
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::WHITE),
	},
	{
		std::string("GRN"), //Colour code green
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::GREEN),
	},
	{
		std::string("YLO1"), //Colour code yellow1
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::YELLOW1),
	},
	{
		std::string("YLO2"), //Colour code yellow2
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::YELLOW2),
	},
	{
		std::string("AMB"), //Colour code amber
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::AMBER),
	},
	{
		std::string("RED"), //Colour code red
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::RED),
	},
	{
		std::string("BLACKBLU"), //Colour code blue and airport is closed (code black)
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::BLUE, true),
	},
	{
		std::string("BLACKWHT"), //Colour code white and airport is closed (code black)
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::WHITE, true),
	},
	{
		std::string("BLACKGRN"), //Colour code green and airport is closed (code black)
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::GREEN, true),
	},
	{
		std::string("BLACKYLO1"), //Colour code yellow1 and airport is closed (code black)
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::YELLOW1, true),
	},
	{
		std::string("BLACKYLO2"), //Colour code yellow2 and airport is closed (code black)
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::YELLOW2, true),
	},
	{
		std::string("BLACKAMB"), //Colour code amber and airport is closed (code black)
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::AMBER, true),
	},
	{
		std::string("BLACKRED"), //Colour code red and airport is closed (code black)
		{metaf::ReportPart::METAR},
		metaf::ColourCodeGroup(metaf::ColourCodeGroup::Code::RED, true),
	},

	///////////////////////////////////////////////////////////////////////////////
};
