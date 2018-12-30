/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "testdata_syntax.h"

const std::vector<testdata::MetarTafSyntaxTestData> testdata::syntaxDataSet = {

	///////////////////////////////////////////////////////////////////////////////
	// malformed reports with only type/location specified
	///////////////////////////////////////////////////////////////////////////////

	{
		"ZZZZ=",
		metaf::ReportType::UNKNOWN,
		metaf::Parser::Error::UNEXPECTED_REPORT_END,
		{
			metaf::LocationGroup("ZZZZ"),
		}
	},

	{
		"METAR ZZZZ=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::UNEXPECTED_REPORT_END,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
		}
	},

	{
		"SPECI ZZZZ=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::UNEXPECTED_REPORT_END,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::SPECI),
			metaf::LocationGroup("ZZZZ"),
		}
	},

	{
		"TAF ZZZZ=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::UNEXPECTED_REPORT_END,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// NIL reports with no report time specified
	///////////////////////////////////////////////////////////////////////////////

	{
		"ZZZZ NIL=",
		metaf::ReportType::UNKNOWN,
		metaf::Parser::Error::NONE,
		{
			metaf::LocationGroup("ZZZZ"),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	{
		"METAR ZZZZ NIL=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	{
		"TAF ZZZZ NIL=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	{
		"TAF ZZZZ CNL=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::EXPECTED_REPORT_TIME,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::FixedGroup(metaf::FixedGroup::Type::CNL),
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// correctional NIL reports
	///////////////////////////////////////////////////////////////////////////////

	{
		"COR ZZZZ NIL=",
		metaf::ReportType::UNKNOWN,
		metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::COR),
		}
	},

	{
		"METAR COR ZZZZ NIL=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::FixedGroup(metaf::FixedGroup::Type::COR),
			metaf::LocationGroup("ZZZZ"),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	{
		"TAF COR ZZZZ NIL=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::FixedGroup(metaf::FixedGroup::Type::COR),
			metaf::LocationGroup("ZZZZ"),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	{
		"AMD ZZZZ NIL=",
		metaf::ReportType::UNKNOWN,
		metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::AMD),
		}
	},

	{
		"METAR AMD ZZZZ NIL=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::AMD_ALLOWED_IN_TAF_ONLY,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::FixedGroup(metaf::FixedGroup::Type::AMD),
		}
	},

	{
		"TAF AMD ZZZZ NIL=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::FixedGroup(metaf::FixedGroup::Type::AMD),
			metaf::LocationGroup("ZZZZ"),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// NIL reports with report time specified
	///////////////////////////////////////////////////////////////////////////////

	{
		"METAR ZZZZ 041115Z NIL=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	{
		"METAR COR ZZZZ 041115Z NIL=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::FixedGroup(metaf::FixedGroup::Type::COR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	{
		"METAR ZZZZ 041115Z CNL=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::CNL_ALLOWED_IN_TAF_ONLY,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::CNL),
		}
	},

	{
		"TAF ZZZZ 041115Z NIL=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	{
		"TAF AMD ZZZZ 041115Z NIL=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::FixedGroup(metaf::FixedGroup::Type::AMD),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	{
		"ZZZZ 041115Z NIL=",
		metaf::ReportType::UNKNOWN,
		metaf::Parser::Error::NONE,
		{
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// malformed NIL reports with report time specified but NIL ommitted
	///////////////////////////////////////////////////////////////////////////////

	{
		"METAR ZZZZ 041115Z=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::UNEXPECTED_REPORT_END,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15)
		}
	},

	{
		"TAF ZZZZ 041115Z=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::UNEXPECTED_REPORT_END,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15)
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// malformed reports with stray group in place of report time
	///////////////////////////////////////////////////////////////////////////////

	{
		"METAR ZZZZ ZZZZ=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::EXPECTED_REPORT_TIME,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::LocationGroup("ZZZZ"),
		}
	},


	{
		"TAF ZZZZ ZZZZ=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::EXPECTED_REPORT_TIME,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::LocationGroup("ZZZZ"),
		}
	},


	///////////////////////////////////////////////////////////////////////////////
	// malformed reports with stray group after NIL
	///////////////////////////////////////////////////////////////////////////////

	{
		"METAR ZZZZ 041115Z NIL 9999=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
			metaf::PlainTextGroup("9999")
			//visibility group is not recognised since groups after NIL 
			//qualify as metaf::ReportPart::HEADER, not as metaf::ReportPart::METAR
		}

	},

	{
		"TAF ZZZZ 041115Z NIL 9999=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
			metaf::PlainTextGroup("9999") 
			//visibility group is not recognised since groups after NIL 
			//qualify as metaf::ReportPart::HEADER, not as metaf::ReportPart::TAF
		}
	},

	{
		"ZZZZ 041115Z NIL 9999=",
		metaf::ReportType::UNKNOWN,
		metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL,
		{
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
			metaf::PlainTextGroup("9999") 
			//visibility group is not recognised since groups after NIL 
			//qualify as metaf::ReportPart::HEADER, not as metaf::ReportPart::METAR 
			//or metaf::ReportPart::TAF
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// complete METAR reports
	///////////////////////////////////////////////////////////////////////////////

	{
		"METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
		}
	},

	{
		"SPECI ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::SPECI),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
		}
	},

	{
		"METAR ZZZZ 041115Z 24005KT 5000 RASN 01/01 Q0980=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(5000),
			metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
				metaf::WeatherGroup::Weather::RAIN,
				metaf::WeatherGroup::Weather::SNOW),
			metaf::TemperatureGroup(01, 01),
			metaf::PressureGroup(980, metaf::PressureGroup::Unit::HECTOPASCAL)
		}
	},


	///////////////////////////////////////////////////////////////////////////////
	// complete correctional METAR reports
	///////////////////////////////////////////////////////////////////////////////

	{
		"METAR COR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::FixedGroup(metaf::FixedGroup::Type::COR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
		}
	},

	{
		"SPECI COR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::SPECI),
			metaf::FixedGroup(metaf::FixedGroup::Type::COR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
		}
	},

	{
		"METAR ZZZZ 041115Z COR 24005KT 9999 FEW040 25/18 Q1011 NOSIG=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::COR),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
		}
	},

	{
		"SPECI ZZZZ 041115Z COR 24005KT 9999 FEW040 25/18 Q1011 NOSIG=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::SPECI),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::COR),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// complete METAR reports with remarks
	///////////////////////////////////////////////////////////////////////////////

	{
		"METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG),
			metaf::FixedGroup(metaf::FixedGroup::Type::RMK),
			//remarks are parsed as plain text in current version
			metaf::PlainTextGroup("TEST"),
			metaf::PlainTextGroup("9999"),
			metaf::PlainTextGroup("RMK"),
		}
	},

	{
		"SPECI ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::SPECI),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG),
			metaf::FixedGroup(metaf::FixedGroup::Type::RMK),
			//remarks are parsed as plain text in current version
			metaf::PlainTextGroup("TEST"),
			metaf::PlainTextGroup("9999"),
			metaf::PlainTextGroup("RMK"),
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// complete metar reports with no METAR or SPECI type specified 
	// (autodetected as metaf::ReportType::METAR)
	///////////////////////////////////////////////////////////////////////////////

	{
		"ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
		}
	},

	{
		"ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG RMK TEST 9999 RMK=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG),
			metaf::FixedGroup(metaf::FixedGroup::Type::RMK),
			//remarks are parsed as plain text in current version
			metaf::PlainTextGroup("TEST"),
			metaf::PlainTextGroup("9999"),
			metaf::PlainTextGroup("RMK"),
		}
	},

	{
		"ZZZZ 041115Z RMK TEST=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::FixedGroup(metaf::FixedGroup::Type::RMK),
			metaf::PlainTextGroup("TEST"),
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// TAF NIL and CNL reports with time span specified
	///////////////////////////////////////////////////////////////////////////////

	{
		"TAF ZZZZ 041115Z 0415/0424 CNL=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 15, 4, 24),
			metaf::FixedGroup(metaf::FixedGroup::Type::CNL),
		}
	},

	{
		"TAF ZZZZ 041115Z 0415/0424 NIL=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 15, 4, 24),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	{
		"TAF AMD ZZZZ 041115Z 0415/0424 CNL=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::FixedGroup(metaf::FixedGroup::Type::AMD),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 15, 4, 24),
			metaf::FixedGroup(metaf::FixedGroup::Type::CNL),
		}
	},

	{
		"TAF AMD ZZZZ 041115Z 0415/0424 NIL=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::FixedGroup(metaf::FixedGroup::Type::AMD),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 15, 4, 24),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL),
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// malformed TAF reports with stray group in place of time span or after CNL
	///////////////////////////////////////////////////////////////////////////////

	{
		"TAF ZZZZ 041115Z ZZZZ=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::EXPECTED_TIME_SPAN,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::LocationGroup("ZZZZ"),
		}
	},

	{
		"TAF ZZZZ 041115Z 0415/0424 CNL 9999=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_CNL,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 15, 4, 24),
			metaf::FixedGroup(metaf::FixedGroup::Type::CNL),
			//visibility group not recognised since groups after CNL are 
			//considered as metaf::ReportPart::HEADER
			metaf::PlainTextGroup("9999"),
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// complete normal, correctional, remarked TAF reports & TAF report with trends
	///////////////////////////////////////////////////////////////////////////////

	{
		"TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMiles(10),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 25000),
		}
	},

	{
		"TAF AMD ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::FixedGroup(metaf::FixedGroup::Type::AMD),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMiles(10),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 25000),
		}
	},

	{
		"TAF COR ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::FixedGroup(metaf::FixedGroup::Type::COR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMiles(10),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 25000),
		}
	},

	{
		"TAF ZZZZ 041115Z 0412/0512 TX07/0416Z=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::MinMaxTemperatureGroup::makeMax(7, 4, 16)
		}
	},

	{
		"TAF ZZZZ 041115Z 0412/0512 TX07/0416Z TN03/0505Z=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::MinMaxTemperatureGroup::makeMax(7, 4, 16),
			metaf::MinMaxTemperatureGroup::makeMin(3, 5, 5)
		}
	},

	{
		"TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250 RMK TEST 9999 RMK=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMiles(10),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 25000),
			metaf::FixedGroup(metaf::FixedGroup::Type::RMK),
			//remarks are parsed as plain text in current version
			metaf::PlainTextGroup("TEST"),
			metaf::PlainTextGroup("9999"),
			metaf::PlainTextGroup("RMK"),
		}
	},

	{
		"TAF AMD ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250"
		" PROB40"
		" TEMPO 0420/0424 240010G15KT"
		" FM050300 BKN100 3SM RA"
		" BECMG 0506/0510 OVC050"
		"=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::FixedGroup(metaf::FixedGroup::Type::AMD),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMiles(10),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 25000),
			
			metaf::ProbabilityGroup(40),

			metaf::FixedGroup(metaf::FixedGroup::Type::TEMPO),
			metaf::TimeSpanGroup(4, 20, 4, 24),
			metaf::WindGroup(240, 
				metaf::Speed(10, metaf::Speed::Unit::KNOTS),
				metaf::Speed(15, metaf::Speed::Unit::KNOTS)),

			metaf::TrendTimeGroup(metaf::TrendTimeGroup::Type::FROM, 5, 3, 0),
			metaf::CloudGroup(metaf::CloudGroup::Amount::BROKEN, 10000),
			metaf::VisibilityGroup::makeVisibilityMiles(3),
			metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE, 
				metaf::WeatherGroup::Weather::RAIN),

			metaf::FixedGroup(metaf::FixedGroup::Type::BECMG),
			metaf::TimeSpanGroup(5, 6, 5, 10),
			metaf::CloudGroup(metaf::CloudGroup::Amount::OVERCAST, 5000),
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// complete TAF reports with no type specified 
	// (autodetected as metaf::ReportType::TAF)
	///////////////////////////////////////////////////////////////////////////////

	{
		"ZZZZ 041115Z 0412/0512 TX07/0416Z=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::MinMaxTemperatureGroup::makeMax(7, 4, 16)
		}
	},

	{
		"ZZZZ 041115Z 0412/0512 TX07/0416Z RMK TEST=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::MinMaxTemperatureGroup::makeMax(7, 4, 16),
			metaf::FixedGroup(metaf::FixedGroup::Type::RMK),
			metaf::PlainTextGroup("TEST")
		}
	},

	{
		"ZZZZ 041115Z 0412/0512 RMK TEST=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::FixedGroup(metaf::FixedGroup::Type::RMK),
			metaf::PlainTextGroup("TEST")
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// complete METAR and TAF reports without report end designator (=),
	// with multiple spaces or line breaks between groups 
	// and with text after report end designator
	///////////////////////////////////////////////////////////////////////////////

	{
		"METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
		}
	},

	{
		"TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMiles(10),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 25000),
		}
	},

	{
		"METAR  ZZZZ    041115Z \r\n 24005KT\r9999\nFEW040\r\n25/18 \nQ1011\r NOSIG=",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
		}
	},

	{
		"TAF  ZZZZ      041115Z \r\n 0412/0512\r24005KT\n10SM\r\nFEW250=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMiles(10),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 25000),
		}
	},

	{
		"METAR ZZZZ 041115Z 24005KT 9999 FEW040 25/18 Q1011 NOSIG= STRAY TEXT 1000",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 4000),
			metaf::TemperatureGroup(25, 18),
			metaf::PressureGroup(1011, metaf::PressureGroup::Unit::HECTOPASCAL),
			metaf::FixedGroup(metaf::FixedGroup::Type::NOSIG)
		}
	},

	{
		"TAF ZZZZ 041115Z 0412/0512 24005KT 10SM FEW250= STRAY TEXT 1000",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMiles(10),
			metaf::CloudGroup(metaf::CloudGroup::Amount::FEW, 25000),
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// complete METAR and TAF with stray NIL and CNL groups in report body
	///////////////////////////////////////////////////////////////////////////////

	{
		"METAR ZZZZ 041115Z 24005KT 9999 NIL FEW040 25/18 Q1011 NOSIG",
		metaf::ReportType::METAR,
		metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL)
		}
	},

	{
		"TAF ZZZZ 041115Z 0412/0512 24005KT NIL 10SM FEW250",
		metaf::ReportType::TAF,
		metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::FixedGroup(metaf::FixedGroup::Type::NIL)
		}
	},

	{
		"METAR ZZZZ 041115Z 24005KT 9999 CNL FEW040 25/18 Q1011 NOSIG",
		metaf::ReportType::METAR,
		metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::VisibilityGroup::makeVisibilityMeters(10000,
				metaf::VisibilityGroup::Direction::NONE,
				metaf::ValueModifier::MORE_THAN),
			metaf::FixedGroup(metaf::FixedGroup::Type::CNL)
		}
	},

	{
		"TAF ZZZZ 041115Z 0412/0512 24005KT CNL 10SM FEW250",
		metaf::ReportType::TAF,
		metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::WindGroup(240, metaf::Speed(5, metaf::Speed::Unit::KNOTS)),
			metaf::FixedGroup(metaf::FixedGroup::Type::CNL)
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// TAF reports without report time specified (time span only)
	///////////////////////////////////////////////////////////////////////////////

	{
		"TAF ZZZZ 0412/0512 TX07/0416Z TN03/0505Z=",
		metaf::ReportType::TAF,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::TAF),
			metaf::LocationGroup("ZZZZ"),
			metaf::TimeSpanGroup(4, 12, 5, 12),
			metaf::MinMaxTemperatureGroup::makeMax(7, 4, 16),
			metaf::MinMaxTemperatureGroup::makeMin(3, 5, 5)
		}
	},

	///////////////////////////////////////////////////////////////////////////////
	// Reports with correct and incorrect use of incomplete visibility groups
	///////////////////////////////////////////////////////////////////////////////

	{
		"METAR ZZZZ 041115Z 1 1/2SM",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::VisibilityGroup::makeVisibilityMiles(1, 1, 2),
		}
	},

	{
		"METAR ZZZZ 041115Z 1/2 1SM",
		metaf::ReportType::METAR,
		metaf::Parser::Error::NONE,
		{
			metaf::FixedGroup(metaf::FixedGroup::Type::METAR),
			metaf::LocationGroup("ZZZZ"),
			metaf::ReportTimeGroup(4, 11, 15),
			metaf::PlainTextGroup("1/2"),
			metaf::VisibilityGroup::makeVisibilityMiles(1),
		}
	},

};

