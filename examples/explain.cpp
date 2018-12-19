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
#include <string>
#include <sstream>
#include <cmath>
#include <emscripten/emscripten.h>

static const std::string delimiter("|");
static std::string result;

void addResult(const std::string & group, const std::string & explanation) {
	result += group;
	result += delimiter;
	result += explanation;
	result += delimiter;
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
	static std::string timeOfDayToString(unsigned int hour, 
		unsigned int minute);
	static std::string trendTypeToString(metaf::TrendTimeGroup::Type type);
	static float roundTo(float number, int digitsAfterDecimalPoint);
	static std::string speedToString(unsigned int speed, 
		metaf::SpeedUnit unit,
		bool reported = true);
	static std::string windDirectionToString(unsigned int direction, 
		bool reported = true, 
		bool variable = false);
	static std::string valueModifierToStringShort(metaf::ValueModifier modifier);
	static std::string distanceToString(float distance, 
		metaf::DistanceUnit unit,
		bool reported = true);
	static std::string visibilityDirectionToString(
		metaf::VisibilityGroup::Direction direction);
	static std::string cloudAmountToString(metaf::CloudGroup::Amount amount);
	static std::string cloudTypeToString(metaf::CloudGroup::Type type);
	static std::string specialWeatherPhenomenaToString(const metaf::WeatherGroup &group);
	static std::string weatherToString(metaf::WeatherGroup::Weather weather);
	static std::string temperatureToString(metaf::Temperature temperature);
	static float calculateRelHumidity(float temperatureC, float dewPointC);
	static std::string temperaturePointToString(metaf::MinMaxTemperatureGroup::Point point);
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
		return("Report type: unscheduled METAR (weather observation report)<br>"
			"Unscheduled report is issued dut to sudden changes in weather conditions: "
			"wind shift, visibility decrease, severe weather, "
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
		return("Clear sky<br>"
			"No cloud layers are detected at or below 12000 feet (3700 meters) (US) "
			"or 25000 feet (7600 meters) (Canada)<br>"
			"Indicates that station is at least partly automated");

		case metaf::FixedGroup::Type::NCD:
		return("No cloud detected<br>"
			"Automated weather station did not detect any clouds; "
			"this can happen due to either no clouds present or sensor error");

		case metaf::FixedGroup::Type::SKC:
		return("Clear sky<br>"
			"In North America indicates report producted by human rather than "
			"automatic weather station");

		case metaf::FixedGroup::Type::NSC:
		return("No significant cloud<br>"
			"No cloud below 5000 feet (1500 meters), no cumulonimbus or towering "
			"cumulus clouds, no vertical visibility restriction");

		case metaf::FixedGroup::Type::NSW:
		return("No significant weather<br>"
			"Indicates end of significant weather phenomena");

		case metaf::FixedGroup::Type::AIRPORT_SNOCLO:
		return("Aerodrome is closed due to snow accumulation");
		break;

		case metaf::FixedGroup::Type::CAVOK:
		return("Ceiling and visibility OK<br>"
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
		return("The remarks are as follows<br>"
			"This version does not recognise or decode remarks");

		default:
		return (std::string("Fixed group type recognised by parser but not by visitor: ") + 
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
	result << "Day and time of report issue: day " << group.day << ", time ";
	result << timeOfDayToString(group.hour, group.minute);
	return(result.str());
}

std::string GroupVisitorExplain::visitTimeSpanGroup(const metaf::TimeSpanGroup & group) {
	std::ostringstream result;
	result << "From day " << group.dayFrom << ", time ";
	result << timeOfDayToString(group.hourFrom, 0);
	result << " until day " << group.dayTill << ", time ";
	result << timeOfDayToString(group.hourTill, 0);
	return(result.str());
}

std::string GroupVisitorExplain::visitTrendTimeGroup(const metaf::TrendTimeGroup & group) {
	std::ostringstream result;
	result << trendTypeToString(group.type) << " ";
	if (group.dayReported) {
		result << std::string("day ") << std::to_string(group.day) << ", time ";
	}
	result << timeOfDayToString(group.hour, group.minute);
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
	if (group.directionReported &&
		!group.directionVariable &&
		!group.direction &&
		group.speedReported && 
		!group.speed &&
		!group.gustSpeedReported) { 
			return("No wind / Calm");
	}
	std::ostringstream result;
	result << "Wind direction: ";
	result << windDirectionToString(group.direction, 
		group.directionReported, 
		group.directionVariable);
	result << "<br>";
	result << "Wind speed: ";
	result << speedToString(group.speed, group.unit, group.speedReported);
	result << "<br>";
	if (group.gustSpeedReported) {
		result << "Gust speed: ";
		result << speedToString(group.gustSpeed, group.unit);
		result << "<br>";
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
	result << ", speed " << speedToString(group.speed, group.speedUnit);
	return(result.str());
}

std::string GroupVisitorExplain::visitVisibilityGroup(const metaf::VisibilityGroup & group) {
	std::ostringstream result;
	result << "Visibility (" << visibilityDirectionToString(group.direction) << ") ";
	if (group.modifier != metaf::ValueModifier::NONE && group.reported) {
		result << valueModifierToStringShort(group.modifier);
	}
	float visibility = group.integer;
	if (group.denominator) {
		visibility += static_cast<float>(group.numerator) / group.denominator;
	}
	result << distanceToString(visibility, group.unit, group.reported);
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
		return(std::string("weather prefix recognised by parser but not by visitor: ") +
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
	result << "dew point: " << temperatureToString(group.dewPoint) << "<br>";
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
	result << "expected on day " << group.day;
	result << ", time " << timeOfDayToString(group.hour, 0);
	return(result.str());
}

std::string GroupVisitorExplain::visitPressureGroup(const metaf::PressureGroup & group) {
	return("Pressure group");
}

std::string GroupVisitorExplain::visitRunwayVisualRangeGroup(const metaf::RunwayVisualRangeGroup & group) {
	return("Runway visual range group");
}

std::string GroupVisitorExplain::visitRunwayStateGroup(const metaf::RunwayStateGroup & group) {
	return("Runway state group");
}

std::string GroupVisitorExplain::visitRainfallGroup(const metaf::RainfallGroup & group) {
	return("Rainfall group");
}

std::string GroupVisitorExplain::visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group) {
	return("Sea surface group");
}

std::string GroupVisitorExplain::visitSeaWavesGroup(const metaf::SeaWavesGroup & group) {
	return("Sea waves group");
}

std::string GroupVisitorExplain::visitColourCodeGroup(const metaf::ColourCodeGroup & group) {
	return("Colour code group");
}

std::string GroupVisitorExplain::visitOther(const metaf::Group & group) {
	return("This group is recognised by parser but not by visitor");
}

std::string GroupVisitorExplain::timeOfDayToString(unsigned int hour, 
	unsigned int minute)
{
	static const std::string hourMinuteSeparator(":");
	static const size_t numberStrSize = 3; //Actually 2 digits + \0
	char hourStr[numberStrSize];
	char minuteStr[numberStrSize];
	if (hour>24) hour = 24;
	std::snprintf(hourStr, numberStrSize, "%02u", hour);
	if (minute>59) minute = 59;
	std::snprintf(minuteStr, numberStrSize, "%02u", minute);
	return(std::string(hourStr) + 
		hourMinuteSeparator + 
		std::string(minuteStr) + 
		std::string(" GMT"));
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
		return(std::string("Trend type recognised by parser but not by visitor: ") +
			std::to_string(static_cast<int>(type)));
	}
}

float GroupVisitorExplain::roundTo(float number, int digitsAfterDecimalPoint) {
	const auto factor = powf(10, digitsAfterDecimalPoint);
	return(static_cast<long int>(number * factor) / factor);
}

std::string GroupVisitorExplain::speedToString(unsigned int speed, 
	metaf::SpeedUnit unit,
	bool reported)
{
	//Conversion factors from https://en.wikipedia.org/wiki/Knot_(unit)
	if (!reported) return("not reported");
	std::ostringstream result;
	switch (unit) {
		case metaf::SpeedUnit::UNKNOWN:
		result << "value " << speed << ", unknown speed units";
		break;

		case metaf::SpeedUnit::KNOTS:
		result << speed << " knots (";
		result << roundTo(speed * 0.514444, 1) << " m/s, ";
		result << roundTo(speed * 1.852, 1) << " km/h, ";
		result << roundTo(speed * 1.150779, 1) << " mph)";
		break;

		case metaf::SpeedUnit::METERS_PER_SECOND:
		result << speed << " m/s (";
		result << roundTo(speed * 1.943844, 1) << " knots, ";
		result << roundTo(speed * 3.6, 1) << " km/h, ";
		result << roundTo(speed * 2.236936, 1) << " mph)";
		break;
		
		case metaf::SpeedUnit::KILOMETERS_PER_HOUR:
		result << speed << " km/h (";
		result << roundTo(speed * 0.621371, 1) << " knots, ";
		result << roundTo(speed * 0.277778, 1) << " m/s, ";
		result << roundTo(speed * 0.621371, 1) << " mph)";
		break;

		default: 
		result << "value " << speed;
		result << ", speed unit recognised by parser but not by visitor: ";
		result << static_cast<int>(unit);
		break;
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
	bool reported) 
{
	if (!reported) return("not reported");
	std::ostringstream result;
	switch (unit) {
		case metaf::DistanceUnit::UNKNOWN:
		result << "value " << distance << ", unknown distance units";
		break;

		case metaf::DistanceUnit::METERS:
		result << static_cast<long int>(distance) << " meters (";
		result << roundTo(distance / 1609.347, 2) << " statute miles, ";
		result << static_cast<long int>(distance / 0.3048) << " feet)";
		break;

		case metaf::DistanceUnit::STATUTE_MILES:
		result << roundTo(distance, 2) << " statute miles (";
		result << static_cast<long int>(distance * 1609.347) << " meters, ";
		result << static_cast<long int>(distance * 5280) << " feet)";
		break;

		case metaf::DistanceUnit::FEET:
		result << static_cast<long int>(distance) << " feet (";
		result << static_cast<long int>(distance * 0.3048) << " meters, ";
		result << roundTo(distance / 5280, 2) << " statute miles)";
		break;

		default: 
		result << "value " << distance;
		result << ", distance unit recognised by parser but not by visitor: ";
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
		return(std::string("direction recognised by parser but not by visitor: ") + 
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
			return("small hail");

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

std::string GroupVisitorExplain::temperatureToString(metaf::Temperature temperature) {
	if (!temperature.reported) return("not reported");
	std::ostringstream result;
	if (!temperature.value) result << valueModifierToStringShort(temperature.modifier);
	result << temperature.value << " &deg;C (";
	if (!temperature.value) result << valueModifierToStringShort(temperature.modifier);
	result << static_cast<int>(temperature.value * 9.0 / 5 + 32) << " &deg;F";
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
		return(std::string("Temperature point recognised by parser but not by visitor: ") + 
			std::to_string(static_cast<int>(point)));
	}
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
	for (auto i=0; i<parser.getResult().size(); i++) {
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