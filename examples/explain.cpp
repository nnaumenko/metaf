/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

/// @file 
/// @brief This example parses METAR or TAF reports displays them in human-readable format.

#include "metaf.h"
#include <vector>
#include <sstream>
#include <cmath>
#include <emscripten/emscripten.h>

static std::string result;

void addResult(const std::string & group, const std::string & explanation) {
	static const char delimiter = '|';
	result += group;
	result.push_back(delimiter);
	result += explanation;
	result.push_back(delimiter);
}

class GroupVisitorExplain : public metaf::GroupVisitor<std::string> {
public:
	static std::string explainReportType(metaf::ReportType reportType);
	static std::string explainReportError(metaf::Parser::Error error);
private:
	virtual std::string visitPlainTextGroup(const metaf::PlainTextGroup & group);
	virtual std::string visitFixedGroup(const metaf::FixedGroup & group);
	virtual std::string visitLocationGroup(const metaf::LocationGroup & group);
	virtual std::string visitReportTimeGroup(const metaf::ReportTimeGroup & group);
	virtual std::string visitTimeSpanGroup(const metaf::TimeSpanGroup & group);
	virtual std::string visitTrendTimeGroup(const metaf::TrendTimeGroup & group);
	virtual std::string visitProbabilityGroup(const metaf::ProbabilityGroup & group);
	virtual std::string visitWindGroup(const metaf::WindGroup & group);
	virtual std::string visitVarWindGroup(const metaf::VarWindGroup & group);
	virtual std::string visitWindShearGroup(const metaf::WindShearGroup & group);
	virtual std::string visitVisibilityGroup(const metaf::VisibilityGroup & group);
	virtual std::string visitCloudGroup(const metaf::CloudGroup & group);
	virtual std::string visitVerticalVisibilityGroup(const metaf::VerticalVisibilityGroup & group);
	virtual std::string visitWeatherGroup(const metaf::WeatherGroup & group);
	virtual std::string visitTemperatureGroup(const metaf::TemperatureGroup & group);
	virtual std::string visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group);
	virtual std::string visitPressureGroup(const metaf::PressureGroup & group);
	virtual std::string visitRunwayVisualRangeGroup(const metaf::RunwayVisualRangeGroup & group);
	virtual std::string visitRunwayStateGroup(const metaf::RunwayStateGroup & group);
	virtual std::string visitRainfallGroup(const metaf::RainfallGroup & group);
	virtual std::string visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group);
	virtual std::string visitSeaWavesGroup(const metaf::SeaWavesGroup & group);
	virtual std::string visitColourCodeGroup(const metaf::ColourCodeGroup & group);
	virtual std::string visitOther(const metaf::Group & group);
	static std::string metafTimeToString(const metaf::MetafTime & time);
	static std::string trendTypeToString(metaf::TrendTimeGroup::Type type);
	static float roundTo(float number, int digitsAfterDecimalPoint);
	static std::string speedToString(const metaf::Speed & speed);
	static std::string windDirectionToString(unsigned int direction, 
		bool reported = true, 
		bool variable = false);
	static std::string valueModifierToStringShort(metaf::ValueModifier modifier);
	static std::string distanceToString(float distance, 
		metaf::DistanceUnit unit,
		bool reported = true,
		metaf::ValueModifier modifier = metaf::ValueModifier::NONE);
	static std::string visibilityDirectionToString(
		metaf::VisibilityGroup::Direction direction);
	static std::string cloudAmountToString(metaf::CloudGroup::Amount amount);
	static std::string cloudTypeToString(metaf::CloudGroup::Type type);
	static std::string specialWeatherPhenomenaToString(const metaf::WeatherGroup &group);
	static std::string weatherToString(metaf::WeatherGroup::Weather weather);
	static std::string temperatureToString(const metaf::Temperature & temperature);
	static float calculateRelHumidity(float temperatureC, float dewPointC);
	static std::string temperaturePointToString(metaf::MinMaxTemperatureGroup::Point point);
	static std::string runwayToString(const metaf::Runway & runway);
	static std::string rvrTrendToString(metaf::RunwayVisualRangeGroup::Trend trend);
	static std::string runwayStateDepositsToString(metaf::RunwayStateGroup::Deposits deposits);
	static std::string runwayStateExtentToString(metaf::RunwayStateGroup::Extent extent);
	static std::string runwayStateDepositDepthToString(
		const metaf::RunwayStateGroup::DepositDepth & depth);
	static std::string runwayStateSurfaceFrictionToString(
		const metaf::RunwayStateGroup::SurfaceFriction & sf);
	static std::string runwayStateBrakingActionToString(
		metaf::RunwayStateGroup::SurfaceFriction::BrakingAction brakingAction);
	static std::string depthToString(float depthMm);
	static std::string stateOfSeaSurfaceToString(
		metaf::SeaSurfaceGroup::StateOfSurface state);
	static std::string waveHeightToString(int waveHeightDm, bool reported = true);
	static std::string colourCodeToString(metaf::ColourCodeGroup::Code code);
	static std::string colourCodeVisibilityToString(metaf::ColourCodeGroup::Code code);
	static std::string colourCodeCeilingToString(metaf::ColourCodeGroup::Code code);
};

std::string GroupVisitorExplain::explainReportType(metaf::ReportType reportType) {
	switch (reportType) {
		case metaf::ReportType::UNKNOWN:
		return("unable to detect");

		case metaf::ReportType::METAR:
		return("METAR (weather observation)");

		case metaf::ReportType::TAF:
		return("TAF (weather forecast)");

		default: 
		return(std::string("report type known by parser but not by visitor: ") + 
			std::to_string(static_cast<int>(reportType)));
	}
}

std::string GroupVisitorExplain::explainReportError(metaf::Parser::Error error) {
	switch (error) {
		case metaf::Parser::Error::NONE:
		return("no error");

		case metaf::Parser::Error::EMPTY_REPORT:
		return("report is empty");
				
		case metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION:
		return("expected report type or ICAO location at report start");
				
		case metaf::Parser::Error::EXPECTED_LOCATION:
		return("expected ICAO location");
				
		case metaf::Parser::Error::EXPECTED_REPORT_TIME:
		return("expected report time");
				
		case metaf::Parser::Error::EXPECTED_TIME_SPAN:
		return("expected time span");
				
		case metaf::Parser::Error::UNEXPECTED_REPORT_END:
		return("unexpected report end");
				
		case metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL:
		return("unexpected group after NIL");
				
		case metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_CNL:
		return("unexpected group after CNL");
				
		case metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY:
		return("unexpected NIL or CNL in report body");
				
		case metaf::Parser::Error::AMD_ALLOWED_IN_TAF_ONLY:
		return("AMD is allowed only in TAF reports");
				
		case metaf::Parser::Error::CNL_ALLOWED_IN_TAF_ONLY:
		return("CNL is allowed only in TAF reports");
				
		case metaf::Parser::Error::INTERNAL_PARSER_STATE:
		return("internal error, unknown parser state");
				
		default: 
		return(std::string("error known by parser but not by visitor: ") + 
			std::to_string(static_cast<int>(error)));
	}
}

std::string GroupVisitorExplain::visitPlainTextGroup(const metaf::PlainTextGroup & group) {
	return(std::string("This group is not recognised by parser: ") + 
		std::string(group.text));
}

std::string GroupVisitorExplain::visitFixedGroup(const metaf::FixedGroup & group) {
	switch (group.type) {
		case metaf::FixedGroup::Type::UNKNOWN:
		return("Unknown fixed group");

		case metaf::FixedGroup::Type::METAR:
		return("Report type: METAR (weather observation report)");

		case metaf::FixedGroup::Type::SPECI:
		return("Report type: unscheduled METAR (weather observation report)\n"
			"Unscheduled report is issued dut to sudden changes in weather "
			"conditions: wind shift, visibility decrease, severe weather, "
			"clouds formed or dissipated, etc.");

		case metaf::FixedGroup::Type::TAF:
		return("Report type: TAF (terminal aerodrome forecast)");

		case metaf::FixedGroup::Type::AMD:
		return("Amended report");

		case metaf::FixedGroup::Type::NIL:
		return("Missing report");

		case metaf::FixedGroup::Type::CNL:
		return("Cancelled report");

		case metaf::FixedGroup::Type::COR:
		return("Correctional report");

		case metaf::FixedGroup::Type::AUTO:
		return("Fully automated report with no human intervention or oversight");

		case metaf::FixedGroup::Type::CLR:
		return("Clear sky\n"
			"No cloud layers are detected at or below 12000 feet (3700 meters) (US) "
			"or 25000 feet (7600 meters) (Canada)\n"
			"Indicates that station is at least partly automated");

		case metaf::FixedGroup::Type::NCD:
		return("No cloud detected\n"
			"Automated weather station did not detect any clouds; "
			"this can happen due to either no clouds present or sensor error");

		case metaf::FixedGroup::Type::SKC:
		return("Clear sky\n"
			"In North America indicates report producted by human rather than "
			"automatic weather station");

		case metaf::FixedGroup::Type::NSC:
		return("No significant cloud\n"
			"No cloud below 5000 feet (1500 meters), no cumulonimbus or towering "
			"cumulus clouds, no vertical visibility restriction");

		case metaf::FixedGroup::Type::NSW:
		return("No significant weather\n"
			"Indicates end of significant weather phenomena");

		case metaf::FixedGroup::Type::AIRPORT_SNOCLO:
		return("Aerodrome is closed due to snow accumulation");
		break;

		case metaf::FixedGroup::Type::CAVOK:
		return("Ceiling and visibility OK\n"
			"Visibility 10 km or more in all directions, "
			"no cloud below 5000 feet (1500 meters), "
			"no cumulonimbus or towering cumulus clouds, "
			"no significant weather phenomena");

		case metaf::FixedGroup::Type::NOSIG:
		return("No significant weather changes expected");

		case metaf::FixedGroup::Type::TEMPO:
		return("The following temporary weather conditions may arise "
			"for less than 60 minutes");

		case metaf::FixedGroup::Type::BECMG:
		return("Weather conditions are expected to change as follows");

		case metaf::FixedGroup::Type::INTER:
		return("The following temporary weather conditions may arise "
			"for less than 30 minutes");

		case metaf::FixedGroup::Type::RMK:
		return("The remarks are as follows\n"
			"(this version does not recognise or decode remarks)");

		default:
		return (std::string("Fixed group type not listed: ") + 
			std::to_string(static_cast<int>(group.type)));
		break;
	}
}

std::string GroupVisitorExplain::visitLocationGroup(const metaf::LocationGroup & group) {
	std::ostringstream result;
	result << "ICAO code for location: " << group.location;
	return(result.str());
}

std::string GroupVisitorExplain::visitReportTimeGroup(
	const metaf::ReportTimeGroup & group)
{
	std::ostringstream result;
	result << "Day and time of report issue: " << metafTimeToString(group.time);
	return(result.str());
}

std::string GroupVisitorExplain::visitTimeSpanGroup(const metaf::TimeSpanGroup & group) {
	std::ostringstream result;
	result << "From " << metafTimeToString(metaf::MetafTime(group.dayFrom, group.hourFrom, 0));
	result << " until " << metafTimeToString(metaf::MetafTime(group.dayTill, group.hourTill, 0));
	return(result.str());
}

std::string GroupVisitorExplain::visitTrendTimeGroup(const metaf::TrendTimeGroup & group) {
	std::ostringstream result;
	result << trendTypeToString(group.type) << " ";
	metaf::MetafTime t(group.hour, group.minute);
	if (group.dayReported) t = metaf::MetafTime(group.day, group.hour, group.minute);
	result << metafTimeToString(t);
	return(result.str());
}

std::string GroupVisitorExplain::visitProbabilityGroup(
	const metaf::ProbabilityGroup & group)
{
	std::ostringstream result;
	result << "Probability of the following trend is " << group.probability << " percent";
	return(result.str());
}

std::string GroupVisitorExplain::visitWindGroup(const metaf::WindGroup & group) {
	if (group.isCalm()) return("No wind / Calm");
	std::ostringstream result;
	result << "Wind direction: ";
	result << windDirectionToString(group.direction, 
		group.directionReported, 
		group.directionVariable);
	result << '\n';
	result << "Wind speed: ";
	result << speedToString(group.windSpeed);
	result << '\n';
	if (group.gustSpeed.reported) {
		result << "Gust speed: ";
		result << speedToString(group.gustSpeed);
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitVarWindGroup(const metaf::VarWindGroup & group) {
	std::ostringstream result;
	result << "Variable wind direction sector from "; 
	result << windDirectionToString(group.directionFrom);
	result << " clockwise to ";
	result << windDirectionToString(group.directionTo);
	return(result.str());
}

std::string GroupVisitorExplain::visitWindShearGroup(const metaf::WindShearGroup & group) {
	std::ostringstream result;
	result << "Wind shear at height ";
	result << distanceToString(group.height, group.heightUnit);
	result << ", direction " << windDirectionToString(group.direction);
	result << ", wind speed " << speedToString(group.windSpeed);
	return(result.str());
}

std::string GroupVisitorExplain::visitVisibilityGroup(const metaf::VisibilityGroup & group) {
	std::ostringstream result;
	result << "Visibility (" << visibilityDirectionToString(group.direction) << ") ";
	float visibility = group.integer;
	if (group.denominator) {
		visibility += static_cast<float>(group.numerator) / group.denominator;
	}
	result << distanceToString(visibility, group.unit, group.reported, group.modifier);
	if (group.incompleteInteger) result << " (incomplete value, possibly error in report)";
	return(result.str());
}

std::string GroupVisitorExplain::visitCloudGroup(const metaf::CloudGroup & group) {
	std::ostringstream result;
	result << cloudAmountToString(group.amount) << ", base height ";
	result << distanceToString(group.height, group.heightUnit, group.heightReported);
	if (group.type != metaf::CloudGroup::Type::NONE) {
		result << ", " << cloudTypeToString(group.type);
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitVerticalVisibilityGroup(const metaf::VerticalVisibilityGroup & group) {
	std::ostringstream result;
	result << "Vertical visibility ";
	result << distanceToString(group.vertVisibility, group.unit, group.reported);
	return(result.str());
}

std::string GroupVisitorExplain::visitWeatherGroup(const metaf::WeatherGroup & group) {
	std::ostringstream result;
	if (group.prefix != metaf::WeatherGroup::Prefix::RECENT) {
		result << "Weather phenomena: ";
	} else {
		result << "Recent weather: ";
	}
	std::string specialWeatherPhenomena = specialWeatherPhenomenaToString(group);
	if (!specialWeatherPhenomena.empty()) {
		result << specialWeatherPhenomena;
		return(result.str());
	}
	bool vicinity = false;
	switch (group.prefix) {
		case metaf::WeatherGroup::Prefix::UNKNOWN:
		result << "unknown weather prefix; ";
		break;

		case metaf::WeatherGroup::Prefix::NONE:
		if (group.isPrecipitation()) {
			result << "moderate ";
		}
		break;

		case metaf::WeatherGroup::Prefix::RECENT:
		break;

		case metaf::WeatherGroup::Prefix::VICINITY:
		vicinity = true;
		break;

		case metaf::WeatherGroup::Prefix::LIGHT:
		result << "light ";
		break;

		case metaf::WeatherGroup::Prefix::HEAVY:
		result << "heavy ";
		break;

		default:
		return(std::string("weather prefix not listed: ") +
			std::to_string(static_cast<int>(group.prefix)) +
			std::string("; "));
	}
	bool showers = false;
	bool thunderstorm = false;
	metaf::WeatherGroup::Weather previous = metaf::WeatherGroup::Weather::UNKNOWN;
	for (const auto w : group.weatherToVector()) {
		if (w == metaf::WeatherGroup::Weather::SHOWERS) {
			showers = true;
			previous = w;
			continue;
		}
		if (w == metaf::WeatherGroup::Weather::THUNDERSTORM) {
			thunderstorm = true;
			previous = w;
			continue;
		}
		std::string weatherStr = weatherToString(w);
		if (previous == metaf::WeatherGroup::Weather::RAIN && 
			w == metaf::WeatherGroup::Weather::SNOW) {
				weatherStr = "and snow mix";
		}
		if (previous == metaf::WeatherGroup::Weather::SNOW && 
			w == metaf::WeatherGroup::Weather::RAIN) {
				weatherStr = "and rain mix";
		}
		result << weatherStr << " ";
		previous = w;
	}
	if (showers) result << "showers ";
	if (thunderstorm) {
		if (group.isPrecipitation()) result << "with ";
		result << "thunderstorm ";
	}
	if (vicinity) result << "in vicinity (5 to 10 miles away)";
	return(result.str());
}

std::string GroupVisitorExplain::visitTemperatureGroup(const metaf::TemperatureGroup & group) {
	std::ostringstream result;
	result << "Air temperature: " << temperatureToString(group.airTemp) << ", ";
	result << "dew point: " << temperatureToString(group.dewPoint) << '\n';
	if (group.airTemp.reported && group.dewPoint.reported) {
		auto rh = calculateRelHumidity(group.airTemp.value, group.dewPoint.value);
		result << "Relative humidity: " << static_cast<int>(rh) << " percent";
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitMinMaxTemperatureGroup(
	const metaf::MinMaxTemperatureGroup & group)
{
	std::ostringstream result;
	result << temperaturePointToString(group.point) << " ";
	result << temperatureToString(group.temperature) << ", ";
	result << "expected on " << metafTimeToString(metaf::MetafTime(group.day, group.hour, 0));
	return(result.str());
}

std::string GroupVisitorExplain::visitPressureGroup(const metaf::PressureGroup & group) {
	std::ostringstream result;
	result << "Atmospheric pressure: ";
	if (!group.reported) {
		result << "not reported";
		return(result.str());
	}
	switch(group.unit) {
		case metaf::PressureGroup::Unit::UNKNOWN:
		result << "value " << group.pressure << ", unknown pressure unit";
		break;

		case metaf::PressureGroup::Unit::HECTOPASCAL:
		result << group.pressure << " hPa (";
		result << roundTo(group.pressure / 33.8639, 2) << "inHg)";
		break;

		case metaf::PressureGroup::Unit::INCHES_HG:
		result << group.pressure << " inHg (";
		result << static_cast <int>(group.pressure * 33.8639) << "hPa)";
		break;

		default:
		result << "value " << group.pressure;
		result << ", pressure unit not listed: "; 
		result << static_cast<int>(group.unit);
		break;
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitRunwayVisualRangeGroup(const metaf::RunwayVisualRangeGroup & group) {
	std::ostringstream result;
	result << "Visual range of " << runwayToString(group.runway) << " ";
	if (!group.visRangeReported) {
		result << "not reported";
		return(result.str());
	}
	if (group.varVisRangeReported) result << "from ";
	result << distanceToString(group.visRange, group.unit, true, group.visModifier);
	if (group.varVisRangeReported) {
		result << " to " << distanceToString(group.varVisRange, 
												group.unit, 
												true, 
												group.varVisModifier);
	}
	if (group.trend != metaf::RunwayVisualRangeGroup::Trend::NONE) {
		result << ", and the trend is " << rvrTrendToString(group.trend);
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitRunwayStateGroup(const metaf::RunwayStateGroup & group) {
	std::ostringstream result;
	result << "State of " << runwayToString(group.runway) << ": ";
	switch (group.status) {
		case metaf::RunwayStateGroup::Status::UNKNOWN:
		result << "status unknown";
		break;

		case metaf::RunwayStateGroup::Status::NONE:
		result << runwayStateDepositsToString(group.deposits) << '\n';
		if (group.deposits != metaf::RunwayStateGroup::Deposits::CLEAR_AND_DRY) {
		    result << "Depth of deposits on runway: ";
		    result << runwayStateDepositDepthToString(group.depositDepth) << '\n';
			result << "Runway contamination extent: ";
			result << runwayStateExtentToString(group.contaminationExtent) << '\n';
		}
		result << "Surface friction: ";
		result << runwayStateSurfaceFrictionToString(group.surfaceFriction);
		break;

		case metaf::RunwayStateGroup::Status::CLRD:
		result << "deposits on runway were cleared or ceased to exist" << '\n';
		result << "Surface friction: ";
		result << runwayStateSurfaceFrictionToString(group.surfaceFriction);
		break;

		case metaf::RunwayStateGroup::Status::SNOCLO:
		result << "closed due to snow accumulation";
		break;

		default:
		result << "status not listed: " << static_cast<int>(group.status);
		break;
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitRainfallGroup(const metaf::RainfallGroup & group) {
	std::ostringstream result;
	if (group.unit != metaf::RainfallGroup::Unit::MM) {
		return(std::string("rainfall measurement units expected to be millimeters, unit: ") + 
			std::to_string(static_cast<int>(group.unit)));
	}
	result << "Rainfall for last 10 minutes " << depthToString(group.last10Minutes) << ", ";
	if (group.last60MinutesReported) {
		result << "for last 60 minutes " << depthToString(group.last60Minutes) << ", ";
	}
	result << "total rainfall since 9:00 AM " << depthToString(group.since9AM);
	return(result.str());
}

std::string GroupVisitorExplain::visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group) {
	std::ostringstream result;
	result << "Sea surface temperature ";
	result << temperatureToString(group.surfaceTemp);
	result << '\n';
	result << "State of sea surface: " << stateOfSeaSurfaceToString(group.stateOfSurface);
	return(result.str());
}

std::string GroupVisitorExplain::visitSeaWavesGroup(const metaf::SeaWavesGroup & group) {
	if (group.waveHeightUnit != metaf::SeaWavesGroup::WaveHeightUnit::DECIMETERS) {
		return(std::string("wave height measurement units expected to be decimeters, unit: ") + 
			std::to_string(static_cast<int>(group.waveHeightUnit)));
	}
	std::ostringstream result;
	result << "Sea surface temperature ";
	result << temperatureToString(group.surfaceTemp);
	result << '\n';
	result << "Wave height " << waveHeightToString(group.waveHeight, group.waveHeightReported);
	return(result.str());
}

std::string GroupVisitorExplain::visitColourCodeGroup(const metaf::ColourCodeGroup & group) {
	std::ostringstream result;
	if (group.codeBlack) {
		result << "Colour code BLACK: ";
		result << "aerodrome closed due to snow accumulation or non-weather reasons";
		result << '\n';
	}
	result << "Colour code " << colourCodeToString(group.code) << ": ";
	if (group.code == metaf::ColourCodeGroup::Code::RED) {
		result << "either visibility " << colourCodeVisibilityToString(group.code);
		result << " or lowest cloud base height " << colourCodeCeilingToString(group.code);
		return(result.str());
	}
	result << "visibility " << colourCodeVisibilityToString(group.code);
	result << " and lowest cloud base height " << colourCodeCeilingToString(group.code);
	return(result.str());
}

std::string GroupVisitorExplain::visitOther(const metaf::Group & group) {
	(void)group;
	return("This group is recognised by parser but not listed");
}

std::string GroupVisitorExplain::metafTimeToString(const metaf::MetafTime & time) {
	std::ostringstream result;
	if (time.isDayReported()) result << "day " << time.day << ", ";
	static const std::string hourMinuteSeparator(":");
	static const size_t numberStrSize = 3; //Actually 2 digits + \0
	char numberStr[numberStrSize];
	std::snprintf(numberStr, numberStrSize, "%02u", time.hour<=24 ? time.hour : 24);
	result << numberStr << hourMinuteSeparator;
	std::snprintf(numberStr, numberStrSize, "%02u", time.minute<=59 ? time.minute : 59);
	result << numberStr << " GMT";
	return(result.str());
}

std::string GroupVisitorExplain::trendTypeToString(metaf::TrendTimeGroup::Type type){
	switch (type) {
		case metaf::TrendTimeGroup::Type::UNKNOWN:
		return("Unknown trend");

		case metaf::TrendTimeGroup::Type::FROM:
		return("From");

		case metaf::TrendTimeGroup::Type::TILL:
		return("Until");

		case metaf::TrendTimeGroup::Type::AT:
		return("At");

		default: 
		return(std::string("Trend type not listed: ") +
			std::to_string(static_cast<int>(type)));
	}
}

float GroupVisitorExplain::roundTo(float number, int digitsAfterDecimalPoint) {
	const auto factor = powf(10, digitsAfterDecimalPoint);
	return(static_cast<long int>(number * factor) / factor);
}

std::string GroupVisitorExplain::speedToString(const metaf::Speed & speed) {
	//Conversion factors from https://en.wikipedia.org/wiki/Knot_(unit)
	if (!speed.reported) return("not reported");
	std::ostringstream result;
	bool unknownUnits = false;
	switch (speed.unit) {
		case metaf::Speed::Unit::UNKNOWN:
		result << "value " << speed.value << ", unknown speed units";
		unknownUnits = true;
		break;

		case metaf::Speed::Unit::KNOTS:
		result << speed.valueAs(metaf::Speed::Unit::KNOTS) << " knots";
		break;

		case metaf::Speed::Unit::METERS_PER_SECOND:
		result << speed.valueAs(metaf::Speed::Unit::METERS_PER_SECOND) << " m/s";
		break;
		
		case metaf::Speed::Unit::KILOMETERS_PER_HOUR:
		result << speed.valueAs(metaf::Speed::Unit::KILOMETERS_PER_HOUR) << " km/h";
		break;

		case metaf::Speed::Unit::MILES_PER_HOUR:
		result << speed.valueAs(metaf::Speed::Unit::MILES_PER_HOUR) << " mph";

		default: 
		result << "value " << speed.value;
		result << ", speed unit not listed: ";
		result << static_cast<int>(speed.unit);
		unknownUnits = true;
		break;
	}
	if (!unknownUnits) {
		result << " (";
		if (speed.unit != metaf::Speed::Unit::KNOTS) {
			result << roundTo(speed.valueAs(metaf::Speed::Unit::KNOTS), 1);
			result << " knots, ";
		}
		if (speed.unit != metaf::Speed::Unit::METERS_PER_SECOND) {
			result << roundTo(speed.valueAs(metaf::Speed::Unit::METERS_PER_SECOND), 1);
			result << " m/s, ";
		}
		if (speed.unit != metaf::Speed::Unit::KILOMETERS_PER_HOUR) {
			result << roundTo(speed.valueAs(metaf::Speed::Unit::KILOMETERS_PER_HOUR), 1);
			result << " km/h";
			if (speed.unit != metaf::Speed::Unit::MILES_PER_HOUR) result << ", ";
		}
		if (speed.unit != metaf::Speed::Unit::MILES_PER_HOUR) {
			result << roundTo(speed.valueAs(metaf::Speed::Unit::MILES_PER_HOUR), 1);
			result << " mph";
		}
		result << ")";
	}
	return(result.str());
}

std::string GroupVisitorExplain::windDirectionToString(unsigned int direction, 
	bool reported,
	bool variable)
{
	if (!reported) return("not reported");
	if (variable) return("variable");

	std::ostringstream result;
	result << direction << "&deg; (";
	if (direction == 90 || direction == 180 || direction == 270 || direction == 360) {
		result << "true ";
	}
	if (direction >= 0 && direction <= 20) result << "north";
	if (direction >= 30 && direction <= 60) result << "northeast";
	if (direction >= 70 && direction <= 110) result << "east";
	if (direction >= 120 && direction <= 150) result << "southeast";
	if (direction >= 160 && direction <= 200) result << "south";
	if (direction >= 210 && direction <= 240) result << "southwest";
	if (direction >= 250 && direction <= 290) result << "west";
	if (direction >= 300 && direction <= 330) result << "northwest";
	if (direction >= 340 && direction <= 360) result << "north";
	result << ")";
	return(result.str());
}

std::string GroupVisitorExplain::valueModifierToStringShort(
	metaf::ValueModifier modifier) 
{
	switch (modifier) {
		case metaf::ValueModifier::UNKNOWN:
		return("?");

		case metaf::ValueModifier::NONE:
		return(" ");

		case metaf::ValueModifier::LESS_THAN:
		return("<");

		case metaf::ValueModifier::MORE_THAN:
		return(">");

		default:
		return(std::string("?") + 
			std::to_string(static_cast<int>(modifier)) + 
			std::string("?"));
	}
}

std::string GroupVisitorExplain::distanceToString(float distance, 
	metaf::DistanceUnit unit,
	bool reported,
	metaf::ValueModifier modifier) 
{
	if (!reported) return("not reported");
	std::ostringstream result;
	const std::string modStr = 
		(modifier == metaf::ValueModifier::NONE) ? 
			std::string("") : 
			valueModifierToStringShort(modifier);
	switch (unit) {
		case metaf::DistanceUnit::UNKNOWN:
		result << "value " << distance << ", unknown distance units";
		break;

		case metaf::DistanceUnit::METERS:
		result << modStr << static_cast<long int>(distance) << " meters (";
		result << modStr << roundTo(distance / 1609.347, 2) << " statute miles, ";
		result << modStr << static_cast<long int>(distance / 0.3048) << " feet)";
		break;

		case metaf::DistanceUnit::STATUTE_MILES:
		result << modStr << roundTo(distance, 2) << " statute miles (";
		result << modStr << static_cast<long int>(distance * 1609.347) << " meters, ";
		result << modStr << static_cast<long int>(distance * 5280) << " feet)";
		break;

		case metaf::DistanceUnit::FEET:
		result << modStr << static_cast<long int>(distance) << " feet (";
		result << modStr << static_cast<long int>(distance * 0.3048) << " meters, ";
		result << modStr << roundTo(distance / 5280, 2) << " statute miles)";
		break;

		default: 
		result << modStr << "value " << distance;
		result << ", distance unit not listed: ";
		result << static_cast<int>(unit);
		break;
	}
	return(result.str());
}

std::string GroupVisitorExplain::visibilityDirectionToString(
	metaf::VisibilityGroup::Direction direction)
{
	switch (direction) {
		case metaf::VisibilityGroup::Direction::UNKNOWN:
		return("unknown direction");

		case metaf::VisibilityGroup::Direction::NONE:
		return("prevailing");

		case metaf::VisibilityGroup::Direction::NDV:
		return("no directional variation");

		case metaf::VisibilityGroup::Direction::N:
		return("north");

		case metaf::VisibilityGroup::Direction::S:
		return("south");

		case metaf::VisibilityGroup::Direction::W:
		return("west");

		case metaf::VisibilityGroup::Direction::E:
		return("east");

		case metaf::VisibilityGroup::Direction::NW:
		return("northwest");

		case metaf::VisibilityGroup::Direction::NE:
		return("northeast");

		case metaf::VisibilityGroup::Direction::SW:
		return("southwest");

		case metaf::VisibilityGroup::Direction::SE:
		return("southeast");

		default: 
		return(std::string("direction not listed: ") + 
			std::to_string(static_cast<int>(direction)));
		break;
	}
}

std::string GroupVisitorExplain::cloudAmountToString(metaf::CloudGroup::Amount amount) {
	switch (amount) {
		case metaf::CloudGroup::Amount::UNKNOWN:
		return("Unknown cloud amount");

		case metaf::CloudGroup::Amount::NOT_REPORTED:
		return("Cloud amount not reported");

		case metaf::CloudGroup::Amount::FEW:
		return("Few clouds (1/8 to 2/8 sky covered)");

		case metaf::CloudGroup::Amount::SCATTERED:
		return("Scattered clouds (3/8 to 4/8 sky covered)");

		case metaf::CloudGroup::Amount::BROKEN:
		return("Broken clouds (5/8 to 7/8 sky covered)");

		case metaf::CloudGroup::Amount::OVERCAST:
		return("Overcast (8/8 sky covered)");

		default: 
		return(std::string("Cloud cover amount is recognised by parser"
			" but not by visitor: ") + 
			std::to_string(static_cast<int>(amount)));
	}
}

std::string GroupVisitorExplain::cloudTypeToString(metaf::CloudGroup::Type type) {
	switch (type) {
		case metaf::CloudGroup::Type::UNKNOWN:
		return("unknown cloud type");

		case metaf::CloudGroup::Type::NONE:
		return("no significant convective cloud type");

		case metaf::CloudGroup::Type::NOT_REPORTED:
		return("cloud type not reported");

		case metaf::CloudGroup::Type::TOWERING_CUMULUS:
		return("towering cumulus");

		case metaf::CloudGroup::Type::CUMULONIMBUS:
		return("cumulonimbus");

		default: 
		return(std::string("undefined: ") + std::to_string(static_cast<int>(type)));
	}
}

std::string GroupVisitorExplain::specialWeatherPhenomenaToString(
	const metaf::WeatherGroup &group)
{
	static const std::vector < std::pair<metaf::WeatherGroup, std::string> > 
		weatherPhenomena = {
			make_pair(
				metaf::WeatherGroup(metaf::WeatherGroup::Prefix::VICINITY,
					metaf::WeatherGroup::Weather::SHOWERS),
				std::string("precipitation in vicinity (up to 10 miles away)")),
			make_pair(
				metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE,
					metaf::WeatherGroup::Weather::ICE_CRYSTALS),
				std::string("ice crystals (diamond dust)")),
			make_pair(
				metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE,
					metaf::WeatherGroup::Weather::DUST),
				std::string("widespread dust")),
			make_pair(
				metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE,
					metaf::WeatherGroup::Weather::UNDETERMINED),
				std::string("undetermined precipitation"
					" (automated system cannot identify precipitation)")),
			make_pair(
				metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE,
					metaf::WeatherGroup::Weather::SHALLOW,
					metaf::WeatherGroup::Weather::FOG),
				std::string("shallow fog (ground fog)")),
			make_pair(
				metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE,
					metaf::WeatherGroup::Weather::PARTIAL,
					metaf::WeatherGroup::Weather::FOG),
				std::string("fog covering part of the location")),
			make_pair(
				metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE,
					metaf::WeatherGroup::Weather::PATCHES,
					metaf::WeatherGroup::Weather::FOG),
				std::string("patches of fog randomly covering the location")),
			make_pair(
				metaf::WeatherGroup(metaf::WeatherGroup::Prefix::NONE,
					metaf::WeatherGroup::Weather::FREEZING,
					metaf::WeatherGroup::Weather::FOG),
				std::string("fog at freezing temperture (may or may not deposit rime)")),
			make_pair(
				metaf::WeatherGroup(metaf::WeatherGroup::Prefix::HEAVY,
					metaf::WeatherGroup::Weather::FUNNEL_CLOUD),
				std::string("tornado or waterspout")),
	};

	for (const auto w : weatherPhenomena) {
		if (std::get<metaf::WeatherGroup>(w) == group) return(std::get<std::string>(w));
	}
	return(std::string());
}

std::string GroupVisitorExplain::weatherToString(metaf::WeatherGroup::Weather weather) {
	switch (weather) {
			case metaf::WeatherGroup::Weather::UNKNOWN:
			return("unknown");

			case metaf::WeatherGroup::Weather::NOT_REPORTED:
			return("not reported");

			case metaf::WeatherGroup::Weather::SHALLOW:
			return("shallow");

			case metaf::WeatherGroup::Weather::PARTIAL:
			return("partial");

			case metaf::WeatherGroup::Weather::PATCHES:
			return("patches");

			case metaf::WeatherGroup::Weather::LOW_DRIFTING:
			return("low drifting");
			break;

			case metaf::WeatherGroup::Weather::BLOWING:
			return("blowing");

			case metaf::WeatherGroup::Weather::SHOWERS:
			return("showers");
			
			case metaf::WeatherGroup::Weather::THUNDERSTORM:
			return("thunderstorm");

			case metaf::WeatherGroup::Weather::FREEZING:
			return("freezing");

			case metaf::WeatherGroup::Weather::DRIZZLE:
			return("drizzle");

			case metaf::WeatherGroup::Weather::RAIN:
			return("rain");

			case metaf::WeatherGroup::Weather::SNOW:
			return("snow");

			case metaf::WeatherGroup::Weather::SNOW_GRAINS:
			return("snow grains");

			case metaf::WeatherGroup::Weather::ICE_CRYSTALS:
			return("ice crystals");

			case metaf::WeatherGroup::Weather::ICE_PELLETS:
			return("ice pellets");

			case metaf::WeatherGroup::Weather::HAIL:
			return("hail");

			case metaf::WeatherGroup::Weather::SMALL_HAIL:
			return("small hail (graupel)");

			case metaf::WeatherGroup::Weather::UNDETERMINED:
			return("undetermined precipitation");

			case metaf::WeatherGroup::Weather::MIST:
			return("mist");

			case metaf::WeatherGroup::Weather::FOG:
			return("fog");

			case metaf::WeatherGroup::Weather::SMOKE:
			return("smoke");

			case metaf::WeatherGroup::Weather::VOLCANIC_ASH:
			return("volcanic ash");

			case metaf::WeatherGroup::Weather::DUST:
			return("dust");

			case metaf::WeatherGroup::Weather::SAND:
			return("sand");

			case metaf::WeatherGroup::Weather::HAZE:
			return("haze");

			case metaf::WeatherGroup::Weather::SPRAY:
			return("spray");

			case metaf::WeatherGroup::Weather::DUST_WHIRLS:
			return("dust or sand whirls");

			case metaf::WeatherGroup::Weather::SQUALLS:
			return("squalls");

			case metaf::WeatherGroup::Weather::FUNNEL_CLOUD:
			return("funnel cloud");

			case metaf::WeatherGroup::Weather::SANDSTORM:
			return("sand storm");

			case metaf::WeatherGroup::Weather::DUSTSTORM:
			return("dust storm");

			default:
			return(std::string("undefined: ") + std::to_string(static_cast<int>(weather)));
	}
}

std::string GroupVisitorExplain::temperatureToString(
	const metaf::Temperature & temperature)
{
	if (!temperature.reported) return("not reported");
	if (temperature.unit != metaf::Temperature::Unit::DEGREES_C) {
		return(std::string("temperature measurement units expected to be &deg;C, unit: ") + 
			std::to_string(static_cast<int>(temperature.unit)));
	}
	std::ostringstream result;
	if (!temperature.value) result << valueModifierToStringShort(temperature.modifier);
	result << temperature.valueAs(metaf::Temperature::Unit::DEGREES_C) << " &deg;C (";
	if (!temperature.value) result << valueModifierToStringShort(temperature.modifier);
	result << static_cast<int>(temperature.valueAs(metaf::Temperature::Unit::DEGREES_F));
	result << " &deg;F";
	result << ")";
	return(result.str());
}

float GroupVisitorExplain::calculateRelHumidity(float temperatureC, float dewPointC) {
	if (temperatureC < dewPointC) return(100);
	const auto saturationVapourPressure = 
		6.11 * powf(10, 7.5 * temperatureC / (237.7 + temperatureC));
	const auto actualVapourPressure = 
		6.11 * powf(10, 7.5 * dewPointC / (237.7 + dewPointC));
	return (100 * actualVapourPressure / saturationVapourPressure);
}

std::string GroupVisitorExplain::temperaturePointToString(
	metaf::MinMaxTemperatureGroup::Point point)
{
	switch(point) {
		case metaf::MinMaxTemperatureGroup::Point::UNKNOWN:
		return("Unknown temperature point");

		case metaf::MinMaxTemperatureGroup::Point::MINIMUM:
		return("Minimum temperature");

		case metaf::MinMaxTemperatureGroup::Point::MAXIMUM:
		return("Maximum temperature");

		default:
		return(std::string("Temperature point not listed: ") + 
			std::to_string(static_cast<int>(point)));
	}
}

std::string GroupVisitorExplain::runwayToString(const metaf::Runway & runway) {
	static const char strSize = 3; //2 digits + \0
	char runwayNumberStr[strSize];
	std::snprintf(runwayNumberStr, strSize, "%02u", runway.number);
	if (runway.isAllRunways()) return ("all runways");
	if (runway.isMessageRepetition()) return ("same runway (repetition of last message)");
	std::string runwayDesignatorStr("");
	switch (runway.designator) {
		case metaf::Runway::Designator::UNKNOWN:
		runwayDesignatorStr = std::string("unknown designator");
		break;

		case metaf::Runway::Designator::NONE:
		break;

		case metaf::Runway::Designator::LEFT:
		runwayDesignatorStr = std::string("LEFT");
		break;

		case metaf::Runway::Designator::CENTER:
		runwayDesignatorStr = std::string("CENTER");
		break;

		case metaf::Runway::Designator::RIGHT:
		runwayDesignatorStr = std::string("RIGHT");
		break;

		default:
		runwayDesignatorStr = std::string("designator not listed: ") + 
			std::to_string(static_cast<int>(runway.designator));
	}
	return(std::string("runway ") + 
		std::string(runwayNumberStr) + 
		(runwayDesignatorStr.empty() ? std::string(" ") : std::string("")) + 
		runwayDesignatorStr);
}

std::string GroupVisitorExplain::rvrTrendToString(
	metaf::RunwayVisualRangeGroup::Trend trend)
{
	switch(trend) {
		case metaf::RunwayVisualRangeGroup::Trend::UNKNOWN:
		return("unknown");

		case metaf::RunwayVisualRangeGroup::Trend::NONE:
		return("none");

		case metaf::RunwayVisualRangeGroup::Trend::UPWARD:
		return("upward");

		case metaf::RunwayVisualRangeGroup::Trend::NEUTRAL:
		return("neutral");

		case metaf::RunwayVisualRangeGroup::Trend::DOWNWARD:
		return("downward");

		default:
		return(std::string("not listed: ") + 
			std::to_string(static_cast<int>(trend)));
	}
}

std::string GroupVisitorExplain::runwayStateDepositsToString(
	metaf::RunwayStateGroup::Deposits deposits)
{
	switch(deposits) {
		case metaf::RunwayStateGroup::Deposits::UNKNOWN:
		return("unknown");

		case metaf::RunwayStateGroup::Deposits::NOT_REPORTED:
		return("not reported");

		case metaf::RunwayStateGroup::Deposits::CLEAR_AND_DRY:
		return("clear and dry");

		case metaf::RunwayStateGroup::Deposits::DAMP:
		return("damp");

		case metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES:
		return("wet and water patches");

		case metaf::RunwayStateGroup::Deposits::RIME_AND_FROST_COVERED:
		return("rime and frost covered");

		case metaf::RunwayStateGroup::Deposits::DRY_SNOW:
		return("dry snow");

		case metaf::RunwayStateGroup::Deposits::WET_SNOW:
		return("wet snow");

		case metaf::RunwayStateGroup::Deposits::SLUSH:
		return("slush");

		case metaf::RunwayStateGroup::Deposits::ICE:
		return("ice");

		case metaf::RunwayStateGroup::Deposits::COMPACTED_OR_ROLLED_SNOW:
		return("compacted or rolled snow");

		case metaf::RunwayStateGroup::Deposits::FROZEN_RUTS_OR_RIDGES:
		return("frozen ruts or ridges");

		default:
		return(std::string("not listed: ") + 
			std::to_string(static_cast<int>(deposits)));
	}
}

std::string GroupVisitorExplain::runwayStateExtentToString(
	metaf::RunwayStateGroup::Extent extent)
{
	switch(extent) {
		case metaf::RunwayStateGroup::Extent::UNKNOWN:
		return("unknown");

		case metaf::RunwayStateGroup::Extent::NOT_REPORTED:
		return("not reported");

		case metaf::RunwayStateGroup::Extent::NONE:
		return("none");

		case metaf::RunwayStateGroup::Extent::LESS_THAN_10_PERCENT:
		return("<10%");

		case metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT:
		return("11% to 25%");

		case metaf::RunwayStateGroup::Extent::FROM_26_TO_50_PERCENT:
		return("26% to 50%");

		case metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT:
		return(">51%");

		case metaf::RunwayStateGroup::Extent::RESERVED_3:
		case metaf::RunwayStateGroup::Extent::RESERVED_4:
		case metaf::RunwayStateGroup::Extent::RESERVED_6:
		case metaf::RunwayStateGroup::Extent::RESERVED_7:
		case metaf::RunwayStateGroup::Extent::RESERVED_8:
		return(std::string("reserved value: ") + std::to_string(static_cast<int>(extent)));

		default:
		return(std::string("not listed: ") + 
			std::to_string(static_cast<int>(extent)));
	}
}

std::string GroupVisitorExplain::runwayStateDepositDepthToString(
	const metaf::RunwayStateGroup::DepositDepth & depth) 
{
	if (depth.unit != metaf::RunwayStateGroup::DepositDepth::Unit::MM) {
		return(std::string("depth measurement units expected to be millimeters, unit: ") + 
			std::to_string(static_cast<int>(depth.unit)));
	}
	switch (depth.status) {
		case metaf::RunwayStateGroup::DepositDepth::Status::UNKNOWN:
		return("status unknown");

		case metaf::RunwayStateGroup::DepositDepth::Status::REPORTED:
		return(depthToString(depth.depth));
			
		case metaf::RunwayStateGroup::DepositDepth::Status::NOT_REPORTED:
		return("not reported");

		case metaf::RunwayStateGroup::DepositDepth::Status::RUNWAY_NOT_OPERATIONAL:
		return("not measured because runway is not operational");

		default:
		return(std::string("status not listed: ") + 
			std::to_string(static_cast<int>(depth.status)));
	}
}

std::string GroupVisitorExplain::runwayStateSurfaceFrictionToString(
	const metaf::RunwayStateGroup::SurfaceFriction & sf)
{
	switch (sf.status) {
		case metaf::RunwayStateGroup::SurfaceFriction::Status::UNKNOWN:
		return("status unknown");

		case metaf::RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED:
		return (std::string("friction coefficient ") + std::to_string(sf.coefficient));

		case metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED:
		return (std::string("braking action ") + 
			runwayStateBrakingActionToString(sf.brakingAction));

		case metaf::RunwayStateGroup::SurfaceFriction::Status::NOT_REPORTED:
		return("not reported");

		case metaf::RunwayStateGroup::SurfaceFriction::Status::UNRELIABLE:
		return("unreliable or unmeasurable");

		default:
		return(std::string("status not listed: ") +
			std::to_string(static_cast<int>(sf.status)));
		break;
	}

}

std::string GroupVisitorExplain::runwayStateBrakingActionToString(
		metaf::RunwayStateGroup::SurfaceFriction::BrakingAction brakingAction)
{
	switch(brakingAction) {
		case metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::UNKNOWN:
		return("unknown");

		case metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::POOR:
		return("poor");

		case metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_POOR:
		return("medium/poor");

		case metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM:
		return("medium");

		case metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::MEDIUM_GOOD:
		return("medium/good");

		case metaf::RunwayStateGroup::SurfaceFriction::BrakingAction::GOOD:
		return("good");

		default:
		return(std::string("not listed: ") + 
			std::to_string(static_cast<int>(brakingAction)));
	}
}

std::string GroupVisitorExplain::depthToString(float depthMm) {
	return(std::to_string(depthMm) + std::string(" mm (") + 
			std::to_string(roundTo(depthMm / 25.4, 2)) + std::string(" inches)"));
}

std::string GroupVisitorExplain::stateOfSeaSurfaceToString(
	metaf::SeaSurfaceGroup::StateOfSurface state)
{
	switch(state) {
		case metaf::SeaSurfaceGroup::StateOfSurface::UNKNOWN:
		return("unknown");

		case metaf::SeaSurfaceGroup::StateOfSurface::NOT_REPORTED:
		return("not reported");

		case metaf::SeaSurfaceGroup::StateOfSurface::CALM_GLASSY:
		return("calm (glassy), no waves");

		case metaf::SeaSurfaceGroup::StateOfSurface::CALM_RIPPLED:
		return("calm (rippled), 0 to 0.1 meters (0 to 1/3 feet) wave height");

		case metaf::SeaSurfaceGroup::StateOfSurface::SMOOTH:
		return("smooth, 0.1 to 0.5 meters (1/3 to 1 1/2 feet) wave height");

		case metaf::SeaSurfaceGroup::StateOfSurface::SLIGHT:
		return("slight, 0.5 to 1.25 meters (1 1/2 to 4 feet) wave height");

		case metaf::SeaSurfaceGroup::StateOfSurface::MODERATE:
		return("moderate, 1.25 to 2.5 meters (4 to 8 feet) wave height");

		case metaf::SeaSurfaceGroup::StateOfSurface::ROUGH:
		return("rough, 2.5 to 4 meters (8 to 13 feet) wave height");

		case metaf::SeaSurfaceGroup::StateOfSurface::VERY_ROUGH:
		return("very rough, 4 to 6 meters (13 to 20 feet) wave height");

		case metaf::SeaSurfaceGroup::StateOfSurface::HIGH:
		return("high, 6 to 9 meters (20 to 30 feet) wave height");

		case metaf::SeaSurfaceGroup::StateOfSurface::VERY_HIGH:
		return("very high, 9 to 14 meters (30 to 46 feet) wave height");

		case metaf::SeaSurfaceGroup::StateOfSurface::PHENOMENAL:
		return("phenomenal, >14 meters (>46 feet) wave height");

		default:
		return(std::string("not listed: ") + std::to_string(static_cast<int>(state)));
	}
}

std::string GroupVisitorExplain::waveHeightToString(int waveHeightDm, bool reported) {
	if (!reported) return("not reported");
	return(std::to_string(roundTo(waveHeightDm / 10.0, 1)) + 
		std::string(" meters (") +
		std::to_string(static_cast<int>(waveHeightDm / 3.048)) + 
		std::string(" feet)"));
};

std::string GroupVisitorExplain::colourCodeToString(metaf::ColourCodeGroup::Code code) {
	switch(code) {
		case metaf::ColourCodeGroup::Code::UNKNOWN:
		return("unknown");

		case metaf::ColourCodeGroup::Code::BLUE:
		return("BLUE");

		case metaf::ColourCodeGroup::Code::WHITE:
		return("WHITE");

		case metaf::ColourCodeGroup::Code::GREEN:
		return("GREEN");

		case metaf::ColourCodeGroup::Code::YELLOW1:
		return("YELLOW1");

		case metaf::ColourCodeGroup::Code::YELLOW2:
		return("YELLOW2");

		case metaf::ColourCodeGroup::Code::AMBER:
		return("AMBER");

		case metaf::ColourCodeGroup::Code::RED:
		return("RED");

		default:
		return(std::string("not listed: ") + std::to_string(static_cast<int>(code)));
	}
}

std::string GroupVisitorExplain::colourCodeVisibilityToString(
	metaf::ColourCodeGroup::Code code)
{
	int visibility = 0;
	metaf::ValueModifier modifier = metaf::ValueModifier::MORE_THAN;
	switch(code) {
		case metaf::ColourCodeGroup::Code::UNKNOWN:
		return("unknown code");

		case metaf::ColourCodeGroup::Code::BLUE:
		visibility = 8000;
		break;

		case metaf::ColourCodeGroup::Code::WHITE:
		visibility = 5000;
		break;

		case metaf::ColourCodeGroup::Code::GREEN:
		visibility = 3700;
		break;

		case metaf::ColourCodeGroup::Code::YELLOW1:
		visibility = 2500;
		break;

		case metaf::ColourCodeGroup::Code::YELLOW2:
		visibility = 1600;
		break;

		case metaf::ColourCodeGroup::Code::AMBER:
		visibility = 800;
		break;

		case metaf::ColourCodeGroup::Code::RED:
		visibility = 800;
		modifier = metaf::ValueModifier::LESS_THAN;
		break;

		default:
		return(std::string("not listed code: ") + std::to_string(static_cast<int>(code)));
	}
	return(distanceToString(visibility, metaf::DistanceUnit::METERS, true, modifier));
}

std::string GroupVisitorExplain::colourCodeCeilingToString(
	metaf::ColourCodeGroup::Code code)
{
	int ceiling = 0;
	metaf::ValueModifier modifier = metaf::ValueModifier::MORE_THAN;
	switch(code) {
		case metaf::ColourCodeGroup::Code::UNKNOWN:
		return("unknown code");

		case metaf::ColourCodeGroup::Code::BLUE:
		ceiling = 2500;
		break;

		case metaf::ColourCodeGroup::Code::WHITE:
		ceiling = 1500;
		break;

		case metaf::ColourCodeGroup::Code::GREEN:
		ceiling = 700;
		break;

		case metaf::ColourCodeGroup::Code::YELLOW1:
		ceiling = 500;
		break;

		case metaf::ColourCodeGroup::Code::YELLOW2:
		ceiling = 300;
		break;

		case metaf::ColourCodeGroup::Code::AMBER:
		ceiling = 200;
		break;

		case metaf::ColourCodeGroup::Code::RED:
		ceiling = 200;
		modifier = metaf::ValueModifier::LESS_THAN;
		break;

		default:
		return(std::string("not listed code: ") + std::to_string(static_cast<int>(code)));
	}
	return(distanceToString(ceiling, metaf::DistanceUnit::FEET, true, modifier));
}

extern "C" const char * EMSCRIPTEN_KEEPALIVE explain(const char * input) {
	metaf::Parser parser;
	parser.parse(std::string(input), true);
	addResult("", 
		std::string("Detected report type: ") + 
		GroupVisitorExplain::explainReportType(parser.getReportType()));
	if (parser.getError() != metaf::Parser::Error::NONE) {
		addResult("", 
			std::string("Parsing error: ") + 
			GroupVisitorExplain::explainReportError(parser.getError()));
	}
	for (auto i=0u; i<parser.getResult().size(); i++) {
		std::string source = std::string("");
		if (i < parser.getSourceGroups().size()) {
			source = parser.getSourceGroups().at(i);
		}
		GroupVisitorExplain visitor;
		addResult(source, visitor.visit(parser.getResult().at(i)));
	}
	//remove last delimiter which is always added at the end 
	//of the last explanation string
	result.pop_back(); 
	return(result.c_str());
}

extern "C" void EMSCRIPTEN_KEEPALIVE freeMemory(){
	std::string().swap(result);
}

int main(int argc, char ** argv) {
	(void) argc; (void) argv;
	//Using EM_ASM_ because EM_ASM(explain()); gives a warning
	EM_ASM_(explain(), 0); 
}