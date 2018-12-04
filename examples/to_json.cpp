/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

/// @file 
/// @brief This example parses METAR or TAF reports and converts them to JSON 
/// format.

#include "metaf.h"
#include <sstream>
//#include <iostream>
#include <stack>
#include <emscripten/emscripten.h>

/// @brief JSON maker class, with a VERY basic functionality. It lacks 
/// safeguards against producing malformed JSON and makes end-user watch out 
/// for object/array boundary. However it will do for demonstrating the 
/// usage of GroupVisitor without adding a JSON framefork dependency.
class MakeJson {
public:
	MakeJson(std::ostream & out);
	~MakeJson();
	void startObject(const std::string & name = std::string());
	void startArray(const std::string & name);
	void finish();
	void finishAll();
	void valueStr(const std::string & name, const std::string & value);
	void valueStr(const std::string & value);
	void valueBool(const std::string & name, bool value);
	void valueInt(const std::string & name, int value);
	void valueFloat(const std::string & name, float value);
	void valueNull(const std::string & name);
	void valueNull();
private:
	std::ostream * output = nullptr;
	bool commaRequired = false; 
	std::stack<char> closingBraces;
};

MakeJson::MakeJson(std::ostream & out) {
		output = &out;
		(*output) << '{';
		closingBraces.push('}');
		commaRequired = false;
	}

MakeJson::~MakeJson() {
	finishAll();
}

void MakeJson::startObject(const std::string & name) {
	if (commaRequired) (*output) << ',';
	if (!name.empty()) (*output) << '"' << name << '"' << ':';
	(*output) << '{';
	closingBraces.push('}');
	commaRequired = false; 
}

void MakeJson::startArray(const std::string & name) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << name << '"' << ':' << '[';
	closingBraces.push(']');
	commaRequired = false;
}

void MakeJson::finish() {
	(*output) << closingBraces.top();
	closingBraces.pop();
	commaRequired = true;
	if (closingBraces.empty()) (*output) << '\n';
}

void MakeJson::finishAll() {
	while (!closingBraces.empty()) {
		finish();
	}
}

void MakeJson::valueStr(const std::string & name, const std::string & value) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << name << '"' << ':' << '"' << value << '"';
	commaRequired = true;
}

void MakeJson::valueStr(const std::string & value) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << value << '"';
	commaRequired = true;
}

void MakeJson::valueBool(const std::string & name, bool value) {
	if (commaRequired) (*output) << ',';
	const std::string valueString = value ? std::string("true") : std::string("false");
	(*output) << '"' << name << '"' << ':' << valueString;
	commaRequired = true;
}

void MakeJson::valueInt(const std::string & name, int value) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << name << '"' << ':' << value;
	commaRequired = true;
}

void MakeJson::valueFloat(const std::string & name, float value) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << name << '"' << ':' << value;
	commaRequired = true;
}

void MakeJson::valueNull(const std::string & name) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << name << '"' << ':' << "null";
	commaRequired = true;
}

void MakeJson::valueNull() {
	if (commaRequired) (*output) << ',';
	(*output) << "null";
	commaRequired = true;
}

class GroupVisitorJson : private metaf::GroupVisitor {
public:
	GroupVisitorJson(std::ostream & output) : json(output) {}
	void toJson(metaf::ReportType reportType, 
		metaf::Parser::Error error, 
		const std::vector<metaf::Group> & content);
private:
	virtual void visitPlainTextGroup(const metaf::PlainTextGroup & group);
	virtual void visitFixedGroup(const metaf::FixedGroup & group);
	virtual void visitLocationGroup(const metaf::LocationGroup & group);
	virtual void visitReportTimeGroup(const metaf::ReportTimeGroup & group);
	virtual void visitTimeSpanGroup(const metaf::TimeSpanGroup & group);
	virtual void visitTrendTimeGroup(const metaf::TrendTimeGroup & group);
	virtual void visitProbabilityGroup(const metaf::ProbabilityGroup & group);
	virtual void visitWindGroup(const metaf::WindGroup & group);
	virtual void visitVarWindGroup(const metaf::VarWindGroup & group);
	virtual void visitWindShearGroup(const metaf::WindShearGroup & group);
	virtual void visitVisibilityGroup(const metaf::VisibilityGroup & group);
	virtual void visitCloudGroup(const metaf::CloudGroup & group);
	virtual void visitVerticalVisibilityGroup(const metaf::VerticalVisibilityGroup & group);
	virtual void visitWeatherGroup(const metaf::WeatherGroup & group);
	virtual void visitTemperatureGroup(const metaf::TemperatureGroup & group);
	virtual void visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group);
	virtual void visitPressureGroup(const metaf::PressureGroup & group);
	virtual void visitRunwayVisualRangeGroup(const metaf::RunwayVisualRangeGroup & group);
	virtual void visitRunwayStateGroup(const metaf::RunwayStateGroup & group);
	virtual void visitRainfallGroup(const metaf::RainfallGroup & group);
	virtual void visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group);
	virtual void visitSeaWavesGroup(const metaf::SeaWavesGroup & group);
	virtual void visitColourCodeGroup(const metaf::ColourCodeGroup & group);
	virtual void visitOther(const metaf::Group & group);
	MakeJson json;
	bool isTrend = false;
	bool isRemark = false;
	int plainTextCounter = 0;
	metaf::Group lastGroup;
	void similarGroupsToArrays(const metaf::Group & group);
	void trendsToArray(const metaf::Group & group);
	bool isTrendGroup(const metaf::Group & group);
	static std::string reportTypeToString(metaf::ReportType reportType);
	static std::string parserErrorToString(metaf::Parser::Error error);
	static std::string distanceUnitToString(metaf::DistanceUnit unit);
	static std::string speedUnitToString(metaf::SpeedUnit unit);
	static std::string temperatureUnitToString(metaf::TemperatureUnit unit);
	static std::string runwayToString(metaf::Runway runway);
	static std::string runwayDesignatorToString(metaf::Runway::Designator designator);
	static std::string timeOfDayToString(unsigned int hour, unsigned int minute);
	static std::string trendTypeToString(metaf::TrendTimeGroup::Type type);
	static std::string visibilityDirectionToString(metaf::VisibilityGroup::Direction direction);
	static std::string cloudAmountToString(metaf::CloudGroup::Amount amount);
	static std::string cloudTypeToString(metaf::CloudGroup::Type type);
	static std::string weatherToString(metaf::WeatherGroup::Weather weather);
	static bool hasPrecipitation(const metaf::WeatherGroup & group);
	static std::string temperaturePointToString(metaf::MinMaxTemperatureGroup::Point point);
	static std::string pressureUnitToString(metaf::PressureGroup::Unit unit);
	static std::string rvrModifierToString(metaf::RunwayVisualRangeGroup::Modifier modifier);
	static std::string rvrTrendToString(metaf::RunwayVisualRangeGroup::Trend trend);
	static std::string runwayStateDepositsToString(metaf::RunwayStateGroup::Deposits deposits);
	static std::string runwayStateExtentToString(metaf::RunwayStateGroup::Extent extent);
	static std::string runwayStateDepositDepthUnitToString(
		metaf::RunwayStateGroup::DepositDepth::Unit unit);
	static std::string runwayStateBrakingActionToString(
		metaf::RunwayStateGroup::SurfaceFriction::BrakingAction brakingAction);
	static std::string rainfallUnitsToString(metaf::RainfallGroup::Unit unit);
	static std::string stateOfSeaSurfaceToString(metaf::SeaSurfaceGroup::StateOfSurface state);
	static std::string waveHeightUnitsToString(metaf::SeaWavesGroup::WaveHeightUnit unit);
	static std::string colourCodeToString(metaf::ColourCodeGroup::Code code);
};

void GroupVisitorJson::toJson(metaf::ReportType reportType, 
	metaf::Parser::Error error, 
	const std::vector<metaf::Group> & content)
{
	plainTextCounter = 0;
	isTrend = false;
	isRemark = false;
	lastGroup = metaf::PlainTextGroup("");
	json.valueStr("kind", "metaf");
	if (error != metaf::Parser::Error::NONE) {
		json.startObject("error");
		json.valueStr("message", parserErrorToString(error));
		json.finish();
	}
	json.startObject("report");
	json.valueStr("type", reportTypeToString(reportType));
	json.valueBool("partial", (error != metaf::Parser::Error::NONE));
	for (const auto & group : content) {
		similarGroupsToArrays(group);
		trendsToArray(group);
		visit(group);
		lastGroup = group;
	}
	json.finishAll();
}

void GroupVisitorJson::visitPlainTextGroup(const metaf::PlainTextGroup & group) {
	json.valueStr(std::string("plainText") + std::to_string(plainTextCounter++), 
		std::string(group.text));
}

void GroupVisitorJson::visitFixedGroup(const metaf::FixedGroup & group) {
	switch (group.type) {
		case metaf::FixedGroup::Type::UNKNOWN:
		json.startObject("fixedGroup");
		json.valueStr("type","unknown");
		json.finish();
		break;

		case metaf::FixedGroup::Type::METAR:
		case metaf::FixedGroup::Type::SPECI:
		case metaf::FixedGroup::Type::TAF:
		// Report type was made part of JSON (from autodetected type)
		break;

		case metaf::FixedGroup::Type::AMD:
		json.valueBool("amendedReport", true);
		break;

		case metaf::FixedGroup::Type::NIL:
		json.valueBool("missingReport", true);
		break;

		case metaf::FixedGroup::Type::CNL:
		json.valueBool("cancelledReport", true);
		break;

		case metaf::FixedGroup::Type::COR:
		json.valueBool("correctionalReport", true);
		break;

		case metaf::FixedGroup::Type::AUTO:
		json.valueBool("automatedReport", true);
		break;

		case metaf::FixedGroup::Type::CLR:
		case metaf::FixedGroup::Type::NCD:
		case metaf::FixedGroup::Type::SKC:
		json.startArray("cloudLayers");
		json.valueNull();
		json.finish();
		break;

		case metaf::FixedGroup::Type::NSC:
		json.startArray("cloudLayers");
		json.startObject();
		json.valueStr("amount", "insignificant");
		json.finish(); //object
		json.finish(); //cloudLayers array
		break;

		case metaf::FixedGroup::Type::NSW:
		json.startArray("weather");
		json.startObject();
		json.valueBool("recent", false);
		json.valueBool("inVicinity", false);
		json.startArray("weatherPhenomena");
		json.valueStr("insignificant");
		json.finish(); //weatherPhenomena array
		json.finish(); //object
		json.finish(); //weather array
		break;

		case metaf::FixedGroup::Type::AIRPORT_SNOCLO:
		json.valueBool("airportClosedDueToSnow", true);
		break;

		case metaf::FixedGroup::Type::CAVOK:
		json.valueBool("ceilingAndVisibilityOK", true);
		break;

		case metaf::FixedGroup::Type::NOSIG:
		json.valueStr("trend", "no significant changes");
		break;

		case metaf::FixedGroup::Type::TEMPO:
		json.valueStr("trend", "temporary");
		break;

		case metaf::FixedGroup::Type::BECMG:
		json.valueStr("trend", "temporary");
		break;

		case metaf::FixedGroup::Type::INTER:
		json.valueStr("trend", "intermediate");
		break;

		case metaf::FixedGroup::Type::RMK:
		if (isTrend) {
			json.finish(); //Current trend object
			json.finish(); //Trends array
		}
		isTrend = false;
		json.startObject("remarks");
		break;

		default:
		json.startObject("fixedGroup");
		json.valueStr("type",
			std::string("undefined: ") + std::to_string(static_cast<int>(group.type)));
		json.finish();
		break;
	}
}

void GroupVisitorJson::visitLocationGroup(const metaf::LocationGroup & group) {
	json.valueStr("icaoLocation", std::string(group.location));
}

void GroupVisitorJson::visitReportTimeGroup(const metaf::ReportTimeGroup & group) {
	json.valueInt("reportReleaseDay", group.day);
	json.valueStr("reportReleaseTime", timeOfDayToString(group.hour, group.minute));
}

void GroupVisitorJson::visitTimeSpanGroup(const metaf::TimeSpanGroup & group) {
	if (std::holds_alternative<metaf::ProbabilityGroup>(lastGroup)) {
		json.valueStr("trend", "time span");
	}
	json.valueInt("applicableFromDay", group.dayFrom);
	json.valueStr("applicableFromTime", timeOfDayToString(group.hourFrom, 0));
	json.valueInt("applicableUntilDay", group.dayTill);
	json.valueStr("applicableUntilTime", timeOfDayToString(group.hourTill, 0));
}

void GroupVisitorJson::visitTrendTimeGroup(const metaf::TrendTimeGroup & group) {
	if (isTrend && !isTrendGroup(lastGroup)) {
		json.valueStr("trend", trendTypeToString(group.type));
	}
	json.startObject(trendTypeToString(group.type));
	if (group.dayReported) json.valueInt("day", group.day);
	json.valueStr("time", timeOfDayToString(group.hour, group.minute));
	json.finish();
}

void GroupVisitorJson::visitProbabilityGroup(const metaf::ProbabilityGroup & group) {
	json.valueInt("probability", group.probability);
}

void GroupVisitorJson::visitWindGroup(const metaf::WindGroup & group) {
	if (group.directionReported &&
		!group.directionVariable &&
		!group.direction &&
		group.speedReported && 
		!group.speed &&
		!group.gustSpeedReported) { 
			//Calm, no wind
			json.startObject("wind");
			json.valueInt("speed", group.speed);
			json.valueStr("speedUnit", speedUnitToString(group.unit));
			json.finish();
			return;
	}
	json.startObject("wind");
	if (group.directionReported && group.directionVariable) {
		json.valueStr("direction", "variable");
	}
	if (group.directionReported && !group.directionVariable) {
		json.valueInt("direction", group.direction);
	}
	if (group.speedReported) json.valueInt("speed", group.speed);
	if (group.gustSpeedReported) json.valueInt("gustSpeed", group.speed);
	json.valueStr("speedUnit", speedUnitToString(group.unit));
	json.finish();
}

void GroupVisitorJson::visitVarWindGroup(const metaf::VarWindGroup & group) {
	json.startObject("variableWindSector");
	json.valueInt("directionLowRange", group.directionFrom);
	json.valueInt("directionHighRange", group.directionTo);
	json.finish();
}

void GroupVisitorJson::visitWindShearGroup(const metaf::WindShearGroup & group) {
	json.startObject("windShear");
	json.valueInt("windShearHeight", group.height);
	json.valueStr("heightUnit", distanceUnitToString(group.heightUnit));
	json.valueInt("windDirection", group.direction);
	json.valueInt("windSpeed", group.speed);
	json.valueStr("speedUnit", speedUnitToString(group.speedUnit));
	json.finish();
}

void GroupVisitorJson::visitVisibilityGroup(const metaf::VisibilityGroup & group) {
	json.startObject();
	if (group.reported) {
		if (group.lessThan) json.valueStr("visibilityModifier", "lessThan");
		if (group.incompleteInteger) json.valueBool("valueIncomplete", true);
		if (!group.denominator || !group.numerator) {
			json.valueInt("visibility", group.integer);
		}
		if (group.denominator && group.numerator) {
			float fraction = static_cast<float>(group.numerator) / group.denominator;
			json.valueFloat("visibility", group.integer + fraction);
		}
	}
	json.valueStr("visibilityUnit", distanceUnitToString(group.unit));
	if (group.direction != metaf::VisibilityGroup::Direction::NONE) {
		json.valueStr("direction", visibilityDirectionToString(group.direction));
	}
	json.finish();
}

void GroupVisitorJson::visitCloudGroup(const metaf::CloudGroup & group) {
	json.startObject();
	if (group.amount != metaf::CloudGroup::Amount::NOT_REPORTED) {
		json.valueStr("amount", cloudAmountToString(group.amount));
	}
	if (group.type != metaf::CloudGroup::Type::NOT_REPORTED &&
		group.type != metaf::CloudGroup::Type::NONE) {
			json.valueStr("convectiveType", cloudTypeToString(group.type));
	}
	if (group.type != metaf::CloudGroup::Type::NOT_REPORTED &&
		group.type == metaf::CloudGroup::Type::NONE) {
			json.valueNull("convectiveType");
	}
	if (group.heightReported) {
		json.valueInt("baseHeight", group.height);
		json.valueStr("heightUnit", distanceUnitToString(group.heightUnit));
	}
	json.finish();
}

void GroupVisitorJson::visitVerticalVisibilityGroup(const metaf::VerticalVisibilityGroup & group) {
	json.startObject("verticalVisibility");
	if (group.reported) {
		json.valueInt("verticalVisibility", group.vertVisibility);
		json.valueStr("unit", distanceUnitToString(group.unit));
	}
	json.finish();
}

void GroupVisitorJson::visitWeatherGroup(const metaf::WeatherGroup & group) {
	json.startObject();
	switch (group.modifier) {
		case metaf::WeatherGroup::Modifier::UNKNOWN:
		json.valueStr("modifier", "unknown");
		break;

		case metaf::WeatherGroup::Modifier::RECENT:
		case metaf::WeatherGroup::Modifier::VICINITY:	
		case metaf::WeatherGroup::Modifier::LIGHT:
		case metaf::WeatherGroup::Modifier::NONE:
		case metaf::WeatherGroup::Modifier::HEAVY:
		break;

		default: 
		json.valueStr("modifier", 
			std::string("undefined: ") + std::to_string(static_cast<int>(group.modifier)));
		break;
	}
	const bool modifierRecent = (group.modifier == metaf::WeatherGroup::Modifier::RECENT);
	json.valueBool("recent", modifierRecent);
	const bool modifierVicinity = (group.modifier == metaf::WeatherGroup::Modifier::VICINITY);
	json.valueBool("inVicinity", modifierVicinity);
	json.startArray("weatherPhenomena");
	switch (group.modifier) {
		case metaf::WeatherGroup::Modifier::LIGHT:
		json.valueStr("light");
		break;

		case metaf::WeatherGroup::Modifier::NONE:
		if (hasPrecipitation(group)) json.valueStr("moderate");
		break;

		case metaf::WeatherGroup::Modifier::HEAVY:
		json.valueStr("heavy");
		break;

		default:
		break;
	}
	int weatherArraySize = 0;
	for (const auto w : group.weatherToVector()) {
		if (w != metaf::WeatherGroup::Weather::NOT_REPORTED) {
			json.valueStr(weatherToString(w));
			weatherArraySize++;
		}
	}
	if (!weatherArraySize) json.valueNull();
	json.finish(); //array
	json.finish(); //object
}

void GroupVisitorJson::visitTemperatureGroup(const metaf::TemperatureGroup & group) {
	if (group.temperatureReported) json.valueInt("airTemperature", group.temperature);
	if (group.dewPointReported) json.valueInt("dewPoint", group.dewPoint);
	if (group.temperatureReported || group.dewPointReported) {
		json.valueStr("temperatureUnit", temperatureUnitToString(group.unit));
	}
}

void GroupVisitorJson::visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group) {
	json.startObject(temperaturePointToString(group.point));
	json.valueInt("temperatureValue", group.temperature);
	json.valueStr("temperatureUnit", temperatureUnitToString(group.unit));
	json.valueInt("expectedOnDay", group.day);
	json.valueStr("expectedOnTime", timeOfDayToString(group.hour, 0));
	json.finish();
}

void GroupVisitorJson::visitPressureGroup(const metaf::PressureGroup & group) {
	if (group.reported) json.valueInt("atmosphericPressure", group.pressure);
	json.valueStr("atmosphericPressureUnit", pressureUnitToString(group.unit));
}

void GroupVisitorJson::visitRunwayVisualRangeGroup(const metaf::RunwayVisualRangeGroup & group) {
	json.startObject();
	json.valueStr("runway", runwayToString(group.runway));
	if (group.visRangeReported && group.varVisRangeReported) {
		json.valueStr("lowLimitModifier", rvrModifierToString(group.visModifier));
		json.valueInt("lowLimitRange", group.visRange);
		json.valueStr("highLimitModifier", rvrModifierToString(group.visModifier));
		json.valueInt("highLimitRange", group.visRange);
	}
	if (group.visRangeReported && !group.varVisRangeReported) {
		json.valueStr("modifier", rvrModifierToString(group.visModifier));
		json.valueInt("range", group.visRange);
	}
	json.valueStr("unit", distanceUnitToString(group.unit));
	if (group.trend != metaf::RunwayVisualRangeGroup::Trend::NONE) {
		json.valueStr("rvrTrend", rvrTrendToString(group.trend));
	}
	json.finish();
}

void GroupVisitorJson::visitRunwayStateGroup(const metaf::RunwayStateGroup & group) {
	json.startObject();
	json.valueStr("runway", runwayToString(group.runway));
	switch (group.status) {
		case metaf::RunwayStateGroup::Status::UNKNOWN:
		json.valueStr("status", "unknown");
		return;

		case metaf::RunwayStateGroup::Status::NONE:
		break;

		case metaf::RunwayStateGroup::Status::CLRD:
		json.valueBool("depositsCeasedToExist", true);
		json.valueBool("runwayOperational", true);
		break;

		case metaf::RunwayStateGroup::Status::SNOCLO:
		json.valueBool("closedSnowAccumulation", true);
		json.valueBool("runwayOperational", false);
		return;

		default:
		json.valueStr("status", 
			std::string("undefined: ") + std::to_string(static_cast<int>(group.status)));
		return;
	}
	if (group.status != metaf::RunwayStateGroup::Status::CLRD) {
		if (group.deposits != metaf::RunwayStateGroup::Deposits::NOT_REPORTED) {
			json.valueStr("deposits", runwayStateDepositsToString(group.deposits));
		} 
		if (group.contaminationExtent != metaf::RunwayStateGroup::Extent::NOT_REPORTED) {
			json.valueStr("contaminationExtent", 
				runwayStateExtentToString(group.contaminationExtent));
		}
		switch (group.depositDepth.status) {
			case metaf::RunwayStateGroup::DepositDepth::Status::UNKNOWN:
			json.valueStr("depositDepthStatus", "unknown");
			break;

			case metaf::RunwayStateGroup::DepositDepth::Status::REPORTED:
			json.valueInt("depositDepth", group.depositDepth.depth);
			json.valueStr("depositDepthUnit", 
				runwayStateDepositDepthUnitToString(group.depositDepth.unit));
			json.valueBool("runwayOperational", true);
			break;
			
			case metaf::RunwayStateGroup::DepositDepth::Status::NOT_REPORTED:
			break;

			case metaf::RunwayStateGroup::DepositDepth::Status::RUNWAY_NOT_OPERATIONAL:
			json.valueBool("runwayOperational", false);
			break;

			default:
			json.valueStr("depositDepthStatus", 
				std::string("undefined: ") + 
				std::to_string(static_cast<int>(group.depositDepth.status)));
			break;
		}
	} // if (group.status != metaf::RunwayStateGroup::Status::CLRD)
	json.startObject("surfaceFriction");
	switch (group.surfaceFriction.status) {
		case metaf::RunwayStateGroup::SurfaceFriction::Status::UNKNOWN:
		json.valueStr("status", "unknown");
		break;

		case metaf::RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED:
		json.valueFloat("coefficient", group.surfaceFriction.coefficient);
		break;

		case metaf::RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED:
		json.valueStr("brakingAction", 
			runwayStateBrakingActionToString(group.surfaceFriction.brakingAction));
		break;

		case metaf::RunwayStateGroup::SurfaceFriction::Status::NOT_REPORTED:
		break;

		case metaf::RunwayStateGroup::SurfaceFriction::Status::UNRELIABLE:
		json.valueBool("unreliable", true);
		break;

		default:
		json.valueStr("status", 
			std::string("undefined: ") + 
			std::to_string(static_cast<int>(group.surfaceFriction.status)));
		break;
	}
	json.finish();//surfaceFriction object
	json.finish();//main object
}

void GroupVisitorJson::visitRainfallGroup(const metaf::RainfallGroup & group) {
	json.valueFloat("rainfallForLastTenMinutes", group.last10Minutes);
	json.valueFloat("rainfallSince9AM", group.last10Minutes);
	if (group.last60MinutesReported) {
		json.valueFloat("rainfallForLastHour", group.last60Minutes);
	}
	json.valueStr("rainfallUnit", rainfallUnitsToString(group.unit));
}

void GroupVisitorJson::visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group) {
	if (group.temperatureReported) {
		json.valueInt("seaSurfaceTemperature", group.temperature);
		json.valueStr("seaSurfaceTemperatureUnit", 
			temperatureUnitToString(group.temperatureUnit));
	}
	if (group.stateOfSurface != metaf::SeaSurfaceGroup::StateOfSurface::NOT_REPORTED) {
		json.valueStr("stateOfSeaSurface", stateOfSeaSurfaceToString(group.stateOfSurface));
	}
}

void GroupVisitorJson::visitSeaWavesGroup(const metaf::SeaWavesGroup & group) {
	if (group.temperatureReported) {
		json.valueInt("seaSurfaceTemperature", group.temperature);
		json.valueStr("seaSurfaceTemperatureUnit", 
			temperatureUnitToString(group.temperatureUnit));
	}
	if (group.waveHeightReported) {
		json.valueInt("seaWaveHeight", group.waveHeight);
		json.valueStr("seaWaveHeightUnit", waveHeightUnitsToString(group.waveHeightUnit));
	}
}

void GroupVisitorJson::visitColourCodeGroup(const metaf::ColourCodeGroup & group) {
	json.valueStr("colourCode", colourCodeToString(group.code));
	json.valueBool("colourCodeBlack", group.codeBlack);
}

void GroupVisitorJson::visitOther(const metaf::Group & group) {
	json.valueNull("unknownGroup");
}

void GroupVisitorJson::similarGroupsToArrays(const metaf::Group & group) {

	if (!std::holds_alternative<metaf::VisibilityGroup>(group) && 
		std::holds_alternative<metaf::VisibilityGroup>(lastGroup))	{
			json.finish();
	}
	if (!std::holds_alternative<metaf::CloudGroup>(group) && 
		std::holds_alternative<metaf::CloudGroup>(lastGroup))	{
			json.finish();
	}
	if (!std::holds_alternative<metaf::WeatherGroup>(group) && 
		std::holds_alternative<metaf::WeatherGroup>(lastGroup))	{
			json.finish();
	}
	if (!std::holds_alternative<metaf::RunwayVisualRangeGroup>(group) && 
		std::holds_alternative<metaf::RunwayVisualRangeGroup>(lastGroup))	{
			json.finish();
	}
	if (!std::holds_alternative<metaf::RunwayStateGroup>(group) && 
		std::holds_alternative<metaf::RunwayStateGroup>(lastGroup))	{
			json.finish();
	}

	if (std::holds_alternative<metaf::VisibilityGroup>(group) && 
		!std::holds_alternative<metaf::VisibilityGroup>(lastGroup)) {
			json.startArray("visibility");
	}
	if (std::holds_alternative<metaf::CloudGroup>(group) && 
		!std::holds_alternative<metaf::CloudGroup>(lastGroup)) {
			json.startArray("cloudLayers");
	}
	if (std::holds_alternative<metaf::WeatherGroup>(group) && 
		!std::holds_alternative<metaf::WeatherGroup>(lastGroup)) {
			json.startArray("weather");
	}
	if (std::holds_alternative<metaf::RunwayVisualRangeGroup>(group) && 
		!std::holds_alternative<metaf::RunwayVisualRangeGroup>(lastGroup)) {
			json.startArray("runwayVisualRange");
	}
	if (std::holds_alternative<metaf::RunwayStateGroup>(group) && 
		!std::holds_alternative<metaf::RunwayStateGroup>(lastGroup)) {
			json.startArray("runwayState");
	}
}

void GroupVisitorJson::trendsToArray(const metaf::Group & group) {
	if (isTrendGroup(group) && !isTrendGroup(lastGroup)) {
		if (!isTrend) json.startArray("trends");
		if (isTrend) json.finish(); //finish previous trend object
		json.startObject();
		isTrend = true;
	}
}

bool GroupVisitorJson::isTrendGroup(const metaf::Group & group) {
	if (std::holds_alternative<metaf::TrendTimeGroup>(group)) return(true);
	if (std::holds_alternative<metaf::ProbabilityGroup>(group)) return(true);
	const metaf::FixedGroup * fixedGroup = std::get_if<metaf::FixedGroup>(&group);
	if (fixedGroup) {
		if (fixedGroup->type == metaf::FixedGroup::Type::NOSIG ||
			fixedGroup->type == metaf::FixedGroup::Type::TEMPO ||
			fixedGroup->type == metaf::FixedGroup::Type::BECMG ||
			fixedGroup->type == metaf::FixedGroup::Type::INTER) return(true);
	}
	return(false);
}

std::string GroupVisitorJson::reportTypeToString(metaf::ReportType reportType) {
	switch (reportType) {
		case metaf::ReportType::UNKNOWN:
		return("unknown");

		case metaf::ReportType::METAR:
		return("METAR");

		case metaf::ReportType::TAF:
		return("TAF");

		default: 
		return(std::string("undefined: ") + std::to_string(static_cast<int>(reportType)));
	}
}

std::string GroupVisitorJson::parserErrorToString(metaf::Parser::Error error) {
	switch (error) {
		case metaf::Parser::Error::NONE:
		return("none");

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
		return("internal error: unknown parser state");
				
		default: 
		return(std::string("undefined: ") + std::to_string(static_cast<int>(error)));
	}
}

std::string GroupVisitorJson::distanceUnitToString(metaf::DistanceUnit unit) {
	switch (unit) {
		case metaf::DistanceUnit::UNKNOWN:
		return("unknown");

		case metaf::DistanceUnit::METERS:
		return("meters");

		case metaf::DistanceUnit::STATUTE_MILES:
		return("statute miles");

		case metaf::DistanceUnit::FEET:
		return("feet");

		default: 
		return(std::string("undefined: ") + std::to_string(static_cast<int>(unit)));
	}
}

std::string GroupVisitorJson::speedUnitToString(metaf::SpeedUnit unit) {
	switch (unit) {
		case metaf::SpeedUnit::UNKNOWN:
		return("unknown");

		case metaf::SpeedUnit::KNOTS:
		return("knots");

		case metaf::SpeedUnit::METERS_PER_SECOND:
		return("m/s");

		case metaf::SpeedUnit::KILOMETERS_PER_HOUR:
		return("km/h");

		default: 
		return(std::string("undefined: ") + std::to_string(static_cast<int>(unit)));
	}
}

std::string GroupVisitorJson::temperatureUnitToString(metaf::TemperatureUnit unit) {
	switch (unit) {
		case metaf::TemperatureUnit::UNKNOWN:
		return("unknown");

		case metaf::TemperatureUnit::DEGREES_C:
		return("centigrade");

		default: 
		return(std::string("undefined: ") + std::to_string(static_cast<int>(unit)));
	}
}

std::string GroupVisitorJson::runwayToString(metaf::Runway runway) {
	static const char strSize = 3; //2 digits + \0
	char runwayNumberStr[strSize];
	std::snprintf(runwayNumberStr, strSize, "%02u", runway.number);
	const std::string designatorString = 
		(runway.designator != metaf::Runway::Designator::NONE) ?
		(std::string(" ") + runwayDesignatorToString(runway.designator)) : 
		std::string();
	return(std::string(runwayNumberStr) + designatorString);
}

std::string GroupVisitorJson::runwayDesignatorToString(metaf::Runway::Designator designator) {
	switch(designator) {
		case metaf::Runway::Designator::UNKNOWN:
		return("unknown");

		case metaf::Runway::Designator::NONE:
		return("none");

		case metaf::Runway::Designator::LEFT:
		return("left");

		case metaf::Runway::Designator::CENTER:
		return("center");

		case metaf::Runway::Designator::RIGHT:
		return("right");

		default:
		return(std::string("undefined: ") + std::to_string(static_cast<int>(designator)));
	}	
}

std::string GroupVisitorJson::timeOfDayToString(unsigned int hour, unsigned int minute) {
	static const std::string hourMinuteSeparator(":");
	static const size_t numberStrSize = 3; //Actually 2 digits + \0
	char hourStr[numberStrSize];
	char minuteStr[numberStrSize];
	if (hour>24) hour = 24;
	std::snprintf(hourStr, numberStrSize, "%02u", hour);
	if (minute>59) minute = 59;
	std::snprintf(minuteStr, numberStrSize, "%02u", minute);
	return(std::string(hourStr) + hourMinuteSeparator + std::string(minuteStr));
}

std::string GroupVisitorJson::trendTypeToString(metaf::TrendTimeGroup::Type type){
	switch (type) {
		case metaf::TrendTimeGroup::Type::UNKNOWN:
		return("unknown");

		case metaf::TrendTimeGroup::Type::FROM:
		return("from");

		case metaf::TrendTimeGroup::Type::TILL:
		return("until");

		case metaf::TrendTimeGroup::Type::AT:
		return("at");

		default: 
		return(std::string("undefined: ") + std::to_string(static_cast<int>(type)));
	}
}

std::string GroupVisitorJson::visibilityDirectionToString(
	metaf::VisibilityGroup::Direction direction)
{
	switch (direction) {
		case metaf::VisibilityGroup::Direction::UNKNOWN:
		return("unknown");

		case metaf::VisibilityGroup::Direction::NONE:
		return("none");

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
		return(std::string("undefined: ") + std::to_string(static_cast<int>(direction)));
	}
}

std::string GroupVisitorJson::cloudAmountToString(metaf::CloudGroup::Amount amount) {
	switch (amount) {
		case metaf::CloudGroup::Amount::UNKNOWN:
		return("unknown");

		case metaf::CloudGroup::Amount::NOT_REPORTED:
		return("not reported");

		case metaf::CloudGroup::Amount::FEW:
		return("few");

		case metaf::CloudGroup::Amount::SCATTERED:
		return("scattered");

		case metaf::CloudGroup::Amount::BROKEN:
		return("broken");

		case metaf::CloudGroup::Amount::OVERCAST:
		return("overcast");

		default: 
		return(std::string("undefined: ") + std::to_string(static_cast<int>(amount)));
	}
}

std::string GroupVisitorJson::cloudTypeToString(metaf::CloudGroup::Type type) {
	switch (type) {
		case metaf::CloudGroup::Type::UNKNOWN:
		return("unknown");

		case metaf::CloudGroup::Type::NONE:
		return("none");

		case metaf::CloudGroup::Type::NOT_REPORTED:
		return("not reported");

		case metaf::CloudGroup::Type::TOWERING_CUMULUS:
		return("towering cumulus");

		case metaf::CloudGroup::Type::CUMULONIMBUS:
		return("cumulonimbus");

		default: 
		return(std::string("undefined: ") + std::to_string(static_cast<int>(type)));
	}
}

std::string GroupVisitorJson::weatherToString(metaf::WeatherGroup::Weather weather) {
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

bool GroupVisitorJson::hasPrecipitation(const metaf::WeatherGroup & group){
	bool precipitationFound = false;
	for (const auto w : group.weatherToVector()) {
		switch (w) {
			case metaf::WeatherGroup::Weather::DRIZZLE:
			case metaf::WeatherGroup::Weather::RAIN:
			case metaf::WeatherGroup::Weather::SNOW:
			case metaf::WeatherGroup::Weather::SNOW_GRAINS:
			case metaf::WeatherGroup::Weather::ICE_CRYSTALS:
			case metaf::WeatherGroup::Weather::ICE_PELLETS:
			case metaf::WeatherGroup::Weather::HAIL:
			case metaf::WeatherGroup::Weather::SMALL_HAIL:
			case metaf::WeatherGroup::Weather::UNDETERMINED:
			precipitationFound = true;
			break;

			default:
			break;
		}
	}
	return(precipitationFound);
}	


std::string GroupVisitorJson::temperaturePointToString(
	metaf::MinMaxTemperatureGroup::Point point)
{
	switch(point) {
		case metaf::MinMaxTemperatureGroup::Point::UNKNOWN:
		return("unknownTemperaturePoint");

		case metaf::MinMaxTemperatureGroup::Point::MINIMUM:
		return("minimumTemperature");

		case metaf::MinMaxTemperatureGroup::Point::MAXIMUM:
		return("maximumTemperature");

		default:
		return(std::string("undefinedTemperaturePoint_") + 
			std::to_string(static_cast<int>(point)));
	}
}

std::string GroupVisitorJson::pressureUnitToString(metaf::PressureGroup::Unit unit) {
	switch(unit) {
		case metaf::PressureGroup::Unit::UNKNOWN:
		return("unknown");

		case metaf::PressureGroup::Unit::HECTOPASCAL:
		return("hPa");

		case metaf::PressureGroup::Unit::INCHES_HG:
		return("inHg");

		default:
		return(std::string("undefined: ") + std::to_string(static_cast<int>(unit)));
	}
}

std::string GroupVisitorJson::rvrModifierToString(
	metaf::RunwayVisualRangeGroup::Modifier modifier)
{
	switch(modifier) {
		case metaf::RunwayVisualRangeGroup::Modifier::UNKNOWN:
		return("unknown");

		case metaf::RunwayVisualRangeGroup::Modifier::NONE:
		return("none");

		case metaf::RunwayVisualRangeGroup::Modifier::LESS_THAN:
		return("lessThan");

		case metaf::RunwayVisualRangeGroup::Modifier::MORE_THAN:
		return("moreThan");

		default:
		return(std::string("undefined: ") + std::to_string(static_cast<int>(modifier)));
	}
}

std::string GroupVisitorJson::rvrTrendToString(
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
		return(std::string("undefined: ") + std::to_string(static_cast<int>(trend)));
	}
}

std::string GroupVisitorJson::runwayStateDepositsToString(
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
		return("frozen ruts and ridges");

		default:
		return(std::string("undefined: ") + std::to_string(static_cast<int>(deposits)));
	}
}

std::string GroupVisitorJson::runwayStateExtentToString(
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
		return(std::string("reserved: ") + std::to_string(static_cast<int>(extent)));

		default:
		return(std::string("undefined: ") + std::to_string(static_cast<int>(extent)));
	}

}

std::string GroupVisitorJson::runwayStateDepositDepthUnitToString(
		metaf::RunwayStateGroup::DepositDepth::Unit unit)
{
	switch(unit) {
		case metaf::RunwayStateGroup::DepositDepth::Unit::MM:
		return("mm");

		default:
		return(std::string("undefined: ") + std::to_string(static_cast<int>(unit)));
	}
}

std::string GroupVisitorJson::runwayStateBrakingActionToString(
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
		return(std::string("undefined: ") + std::to_string(static_cast<int>(brakingAction)));
	}
}

std::string GroupVisitorJson::rainfallUnitsToString(metaf::RainfallGroup::Unit unit) {
	switch(unit) {
		case metaf::RainfallGroup::Unit::MM:
		return("mm");

		default:
		return(std::string("undefined: ") + std::to_string(static_cast<int>(unit)));
	}
}

std::string GroupVisitorJson::stateOfSeaSurfaceToString(
	metaf::SeaSurfaceGroup::StateOfSurface state)
{
	switch(state) {
		case metaf::SeaSurfaceGroup::StateOfSurface::UNKNOWN:
		return("unknown");

		case metaf::SeaSurfaceGroup::StateOfSurface::NOT_REPORTED:
		return("not reported");

		case metaf::SeaSurfaceGroup::StateOfSurface::CALM_GLASSY:
		return("calm (glassy)");

		case metaf::SeaSurfaceGroup::StateOfSurface::CALM_RIPPLED:
		return("calm (rippled)");

		case metaf::SeaSurfaceGroup::StateOfSurface::SMOOTH:
		return("smooth");

		case metaf::SeaSurfaceGroup::StateOfSurface::SLIGHT:
		return("slight waves");

		case metaf::SeaSurfaceGroup::StateOfSurface::MODERATE:
		return("moderate waves");

		case metaf::SeaSurfaceGroup::StateOfSurface::ROUGH:
		return("rough");

		case metaf::SeaSurfaceGroup::StateOfSurface::VERY_ROUGH:
		return("very rough");

		case metaf::SeaSurfaceGroup::StateOfSurface::HIGH:
		return("high waves");

		case metaf::SeaSurfaceGroup::StateOfSurface::VERY_HIGH:
		return("very high waves");

		case metaf::SeaSurfaceGroup::StateOfSurface::PHENOMENAL:
		return("phenomenal waves");

		default:
		return(std::string("undefined: ") + std::to_string(static_cast<int>(state)));
	}
}


std::string GroupVisitorJson::waveHeightUnitsToString(metaf::SeaWavesGroup::WaveHeightUnit unit) {
	switch(unit) {
		case metaf::SeaWavesGroup::WaveHeightUnit::DECIMETERS:
		return("decimeters");

		default:
		return(std::string("undefined: ") + std::to_string(static_cast<int>(unit)));
	}
}

std::string GroupVisitorJson::colourCodeToString(metaf::ColourCodeGroup::Code code) {
	switch(code) {
		case metaf::ColourCodeGroup::Code::UNKNOWN:
		return("unknown");

		case metaf::ColourCodeGroup::Code::BLUE:
		return("blue");

		case metaf::ColourCodeGroup::Code::WHITE:
		return("white");

		case metaf::ColourCodeGroup::Code::GREEN:
		return("green");

		case metaf::ColourCodeGroup::Code::YELLOW1:
		return("yellow1");

		case metaf::ColourCodeGroup::Code::YELLOW2:
		return("yellow2");

		case metaf::ColourCodeGroup::Code::AMBER:
		return("amber");

		case metaf::ColourCodeGroup::Code::RED:
		return("red");

		default:
		return(std::string("undefined: ") + std::to_string(static_cast<int>(code)));
	}
}

std::string result;

extern "C" const char * EMSCRIPTEN_KEEPALIVE toJson(const char * input) {
	metaf::Parser parser;
	parser.parse(std::string(input));

	std::ostringstream output;
	GroupVisitorJson visitor(output);
	visitor.toJson(parser.getReportType(), parser.getError(), parser.getResult());

	result = output.str(); //involves copying operation but will do for this example
	return(result.c_str());
}

extern "C" void EMSCRIPTEN_KEEPALIVE freeMemory(){
	std::string().swap(result);
}
