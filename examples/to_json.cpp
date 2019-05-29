/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

/// @file 
/// @brief This example parses METAR or TAF reports and converts them to JSON 
/// format.

#include "metaf.h"
#include <sstream>
#include <stack>
#include <emscripten/emscripten.h>

using namespace std::literals;

/// JSON maker class, with a VERY basic functionality. It lacks safeguards 
/// against producing malformed JSON and makes end-user watch out for 
/// object/array boundary. However it will do for demonstrating the usage of 
/// GroupVisitor without adding a JSON framefork dependency.
class MakeJson {
public:
	MakeJson(std::ostream & out);
	~MakeJson();
	void startObject(std::string_view name = std::string());
	void startArray(std::string_view name);
	void finish();
	void finishAll();
	void valueStr(std::string_view name, std::string_view value);
	void valueStr(std::string_view value);
	void valueBool(std::string_view name, bool value);
	void valueInt(std::string_view name, int value);
	void valueFloat(std::string_view name, float value);
	void valueNull(std::string_view name);
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

void MakeJson::startObject(std::string_view name) {
	if (commaRequired) (*output) << ',';
	if (!name.empty()) (*output) << '"' << name << '"' << ':';
	(*output) << '{';
	closingBraces.push('}');
	commaRequired = false; 
}

void MakeJson::startArray(std::string_view name) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << name << '"' << ':' << '[';
	closingBraces.push(']');
	commaRequired = false;
}

void MakeJson::finish() {
	(*output) << closingBraces.top();
	closingBraces.pop();
	commaRequired = true;
	if (closingBraces.empty()) (*output) << std::endl;
}

void MakeJson::finishAll() {
	while (!closingBraces.empty()) {
		finish();
	}
}

void MakeJson::valueStr(std::string_view name, const std::string_view value) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << name << '"' << ':' << '"' << value << '"';
	commaRequired = true;
}

void MakeJson::valueStr(std::string_view value) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << value << '"';
	commaRequired = true;
}

void MakeJson::valueBool(std::string_view name, bool value) {
	if (commaRequired) (*output) << ',';
	const std::string valueString = value ? std::string("true") : std::string("false");
	(*output) << '"' << name << '"' << ':' << valueString;
	commaRequired = true;
}

void MakeJson::valueInt(std::string_view name, int value) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << name << '"' << ':' << value;
	commaRequired = true;
}

void MakeJson::valueFloat(std::string_view name, float value) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << name << '"' << ':' << value;
	commaRequired = true;
}

void MakeJson::valueNull(std::string_view name) {
	if (commaRequired) (*output) << ',';
	(*output) << '"' << name << '"' << ':' << "null";
	commaRequired = true;
}

void MakeJson::valueNull() {
	if (commaRequired) (*output) << ',';
	(*output) << "null";
	commaRequired = true;
}



class GroupVisitorJson : private metaf::GroupVisitor<void> {
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
	virtual void visitTrendGroup(const metaf::TrendGroup & group);
	virtual void visitWindGroup(const metaf::WindGroup & group);
	virtual void visitVisibilityGroup(const metaf::VisibilityGroup & group);
	virtual void visitCloudGroup(const metaf::CloudGroup & group);
	virtual void visitWeatherGroup(const metaf::WeatherGroup & group);
	virtual void visitTemperatureGroup(const metaf::TemperatureGroup & group);
	virtual void visitTemperatureForecastGroup(const metaf::TemperatureForecastGroup & group);
	virtual void visitPressureGroup(const metaf::PressureGroup & group);
	virtual void visitRunwayVisualRangeGroup(const metaf::RunwayVisualRangeGroup & group);
	virtual void visitRunwayStateGroup(const metaf::RunwayStateGroup & group);
	virtual void visitWindShearLowLayerGroup(const metaf::WindShearLowLayerGroup & group);
	virtual void visitRainfallGroup(const metaf::RainfallGroup & group);
	virtual void visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group);
	virtual void visitColourCodeGroup(const metaf::ColourCodeGroup & group);
	virtual void visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group);
	virtual void visitPrecipitationGroup(const metaf::PrecipitationGroup & group);
	virtual void visitLayerForecastGroup(const metaf::LayerForecastGroup & group);
	virtual void visitPressureTendencyGroup(const metaf::PressureTendencyGroup & group);
	virtual void visitCloudTypesGroup(const metaf::CloudTypesGroup & group);
	virtual void visitCloudLayersGroup(const metaf::CloudLayersGroup & group);
	virtual void visitMiscGroup(const metaf::MiscGroup & group);
	virtual void visitOther(const metaf::Group & group);

	MakeJson json;
	bool isTrend = false;
	bool isRemark = false;
	int plainTextCounter = 0;
	int layerForecastCounter = 0;
	int notSpecifiedTemperatureForecastCounter = 0;
	metaf::Group lastGroup;
	void similarGroupsToArrays(const metaf::Group & group);

	void undefinedToJson(int value, 
		std::string_view name);
	void runwayToJson(const metaf::Runway & runway,
		std::string_view valueName,
		std::string_view designatorName,
		std::string_view lastMessageRepetitionName);
	void metafTimeToJson(const metaf::MetafTime & metafTime,
		std::string_view dayName,
		std::string_view timeName);
	void temperatureToJson(const metaf::Temperature & temperature,
		std::string_view valueName,
		std::string_view unitName,
		std::string_view freezingName);
	void speedToJson(const metaf::Speed & speed,
		std::string_view valueName,
		std::string_view unitName);
	void distanceToJson(const metaf::Distance & distance,
		std::string_view valueName,
		std::string_view unitName,
		std::string_view modifierName);
	void directionToJson(const metaf::Direction & direction,
		std::string_view degreesValueName,
		std::string_view cardinalValueName,
		std::string_view unitName,
		std::string_view undefinedName,
		bool trueCardinalDirections = false);
	void pressureToJson(const metaf::Pressure & pressure,
		std::string_view valueName,
		std::string_view unitName);
	void precipitationToJson(const metaf::Precipitation & precipitation,
		std::string_view valueName,
		std::string_view unitName,
		std::string_view runwayNotOperationalName,
		std::string_view undefinedStatusName,
		bool allowTrace = false);
	void surfaceFrictionToJson(const metaf::SurfaceFriction & surfaceFriction,
		std::string_view coefficientName,
		std::string_view brakingActionName,
		std::string_view unreliableName,
		std::string_view undefinedStatusName);
	void waveHeightToJson(const metaf::WaveHeight waveHeight,
		std::string_view stateOfSurfaceName,
		std::string_view waveHeightName,
		std::string_view waveHeightUnitName,
		std::string_view undefinedTypeName);
	void atmosphericLayerToJson(const metaf::LayerForecastGroup::Type type,
		std::string_view typeName,
		std::string_view descriptionName,
		std::string_view severityName,
		std::string_view frequencyName);

	static std::string undefinedToString(int value);
	static std::string reportTypeToString(metaf::ReportType reportType);
	static std::string parserErrorToString(metaf::Parser::Error error);
	static std::string cardinalDirectionToString(metaf::Direction::Cardinal dir);
	static std::string stateOfSeaSurfaceToString(metaf::WaveHeight::StateOfSurface state);
	static std::string waveHeightUnitToString(metaf::WaveHeight::Unit unit);
	static std::string trendTypeToString(metaf::TrendGroup::Type type);
	static std::string cloudAmountToString(metaf::CloudGroup::Amount amount);
	static std::pair<int, int> cloudAmountToOcta(metaf::CloudGroup::Amount amount);
	static std::string weatherDescriptorToString(metaf::WeatherGroup::Descriptor descriptor);
	static std::string weatherPhenomenaToString(metaf::WeatherGroup::Weather weather);
	static std::string temperatureForecastPointToString(
		metaf::TemperatureForecastGroup::Point point);
	static std::string rvrTrendToString(metaf::RunwayVisualRangeGroup::Trend trend);
	static std::string runwayStateDepositsToString(metaf::RunwayStateGroup::Deposits deposits);
	static std::string runwayStateExtentErrorToString(metaf::RunwayStateGroup::Extent extent);
	static std::pair<int, int> runwayStateExtentToPercent(
		metaf::RunwayStateGroup::Extent extent);
	static std::string colourCodeToString(metaf::ColourCodeGroup::Code code);
	static std::string precipitationGroupTypeToString(metaf::PrecipitationGroup::Type type);
	static std::string pressureTendencyTypeToString(metaf::PressureTendencyGroup::Type type);
	static std::string pressureTendencyTrendToString(metaf::PressureTendencyGroup::Trend type);
	static std::string cloudTypeToString(metaf::CloudTypesGroup::Type type);
	static std::string cloudLowLayerToString(metaf::CloudLayersGroup::LowLayer lowLayer);
	static std::string cloudMidLayerToString(metaf::CloudLayersGroup::MidLayer midLayer);
	static std::string cloudHighLayerToString(metaf::CloudLayersGroup::HighLayer highLayer);
};

void GroupVisitorJson::toJson(metaf::ReportType reportType, 
	metaf::Parser::Error error, 
	const std::vector<metaf::Group> & content)
{
	plainTextCounter = 0;
	isTrend = false;
	isRemark = false;
	lastGroup = metaf::PlainTextGroup();
	json.valueStr("kind", "metaf");
	if (const auto errorStr = parserErrorToString(error); 
		!errorStr.empty()) {
			json.startObject("error");
			json.valueStr("message", errorStr);
			json.finish();
	}
	json.startObject("report");
	json.valueStr("type", reportTypeToString(reportType));
	json.valueBool("partial", (error != metaf::Parser::Error::NONE));
	json.valueStr("timeZone", "GMT");
	for (const auto & group : content) {
		similarGroupsToArrays(group);
		visit(group);
		lastGroup = group;
	}
	json.finishAll();
}

void GroupVisitorJson::visitPlainTextGroup(const metaf::PlainTextGroup & group) {
	const auto name = "plainText"s + std::to_string(plainTextCounter++);
	json.valueStr(name, group.toString());
	if (!group.isValid()) json.valueBool(name + "valid"s,	false);
}

void GroupVisitorJson::visitFixedGroup(const metaf::FixedGroup & group) {
	switch (group.type()) {
		case metaf::FixedGroup::Type::METAR:
		case metaf::FixedGroup::Type::SPECI:
		case metaf::FixedGroup::Type::TAF:
		// No action required: report type was is a part of JSON (from autodetected type)
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

	case metaf::FixedGroup::Type::NSW:
		json.valueBool("weatherPhenomenaEnded", true);
		break;

		case metaf::FixedGroup::Type::R_SNOCLO:
		json.valueBool("aerodromeClosedDueToSnow", true);
		break;

		case metaf::FixedGroup::Type::CAVOK:
		json.valueBool("ceilingAndVisibilityOK", true);
		break;

		case metaf::FixedGroup::Type::WSCONDS:
		json.valueBool("conditionsForWindShearPresent", true);
		break;

		case metaf::FixedGroup::Type::RMK:
		if (isTrend) {
			json.finish(); //Current trend object
			json.finish(); //Trends array
		}
		isTrend = false;
		json.startObject("remarks");
		break;

		case metaf::FixedGroup::Type::MAINTENANCE_INDICATOR:
		if (isTrend) {
			json.finish(); //Current trend object
			json.finish(); //Trends array
		}
		isTrend = false;
		json.valueBool("stationMaintenanceRequired", true);
		break;

		case metaf::FixedGroup::Type::AO1:
		json.valueBool("automatedStation", true);
		json.valueBool("stationHasPrecipitationDiscriminator", false);
		break;

		case metaf::FixedGroup::Type::AO2:
		json.valueBool("automatedStation", true);
		json.valueBool("stationHasPrecipitationDiscriminator", true);
		break;

		case metaf::FixedGroup::Type::NOSPECI:
		json.valueBool("stationIssuesSpeciReports", false);
		break;

		case metaf::FixedGroup::Type::PRESFR:
		json.valueBool("atmosphericPressureFallingRapidly", true);
		break;

		case metaf::FixedGroup::Type::PRESRR:
		json.valueBool("atmosphericPressureRisingRapidly", true);
		break;

		case metaf::FixedGroup::Type::RVRNO:
		json.valueNull("runwayVisualRange");
		break;

		case metaf::FixedGroup::Type::PWINO:
		json.valueBool("presentWeatherIdentifierOperating", false);
		break;

		case metaf::FixedGroup::Type::PNO:
		json.valueBool("tippingBucketRainGaugeOperating", false);
		break;

		case metaf::FixedGroup::Type::FZRANO:
		json.valueBool("freezingRainSensorOperating", false);
		break;

		case metaf::FixedGroup::Type::TSNO:
		json.valueBool("lightningDetectorOperating", false);
		break;

		case metaf::FixedGroup::Type::SLPNO:
		json.valueNull("atmosphericPressureAdditionalSeaLevelPressure");
		break;

		default:
		json.startObject("fixedGroup");
		undefinedToJson(static_cast<int>(group.type()), "type");
		json.finish();
		break;
	}
	if (!group.isValid()) json.valueBool("lastFixedGroupvalid", false);
}

void GroupVisitorJson::visitLocationGroup(const metaf::LocationGroup & group) {
	json.valueStr("icaoLocation", std::string(group.toString()));
	if (!group.isValid()) json.valueBool("icaoLocationValid", false);
}

void GroupVisitorJson::visitReportTimeGroup(const metaf::ReportTimeGroup & group) {
	metafTimeToJson(group.time(), "reportReleaseDay", "reportReleaseTime");
	if (!group.isValid()) json.valueBool("reportReleaseTimeValid", false);
}

void GroupVisitorJson::visitTrendGroup(const metaf::TrendGroup & group) {
	if (group.isTimeSpanGroup() && !isTrend) {
		if (const auto timeFrom = group.timeFrom(); timeFrom) {
			metafTimeToJson(*timeFrom, "applicableFromDay", "applicableFromTime");
		}
		if (const auto timeTill = group.timeTill(); timeTill) {
			metafTimeToJson(*timeTill, "applicableUntilDay", "applicableUntilTime");
		}
		if (!group.isValid()) json.valueBool("applicableDayAndTimeValid", false);
		return;
	}
	if (isTrend) {
		json.finish(); //Current trend object
	} else {
		json.startArray("trends");
		isTrend = true;
	}
	json.startObject();
	json.valueStr("trend", trendTypeToString(group.type()));
	if (!group.isValid()) json.valueBool("valid", false);
	if (group.probability() == metaf::TrendGroup::Probability::PROB_30) {
		json.valueInt("probability", 30);
	}
	if (group.probability() == metaf::TrendGroup::Probability::PROB_40) {
		json.valueInt("probability", 40);
	}
	if (const auto timeFrom = group.timeFrom(); timeFrom) {
		metafTimeToJson(*timeFrom, "fromDay", "fromTime");
	}
	if (const auto timeTill = group.timeTill(); timeTill) {
		metafTimeToJson(*timeTill, "tillDay", "tillTime");
	}
	if (const auto timeAt = group.timeAt(); timeAt) {
		metafTimeToJson(*timeAt, "atDay", "atTime");
	}
}

void GroupVisitorJson::visitWindGroup(const metaf::WindGroup & group) {
	if (group.isWindShear()) json.startObject("windShear");
	if (group.isSurfaceWind()) json.startObject("wind");
	if (!group.isValid()) json.valueBool("valid", false);
	if (group.isCalm()) json.valueBool("calm", true);
	distanceToJson(group.windShearHeight(), "windShearHeight", "heightUnit", "heightModifier");
	directionToJson(group.direction(), 
		"direction", 
		"directionCardinal",
		"directionUnit", 
		"directionType", 
		true);
	speedToJson(group.windSpeed(), "windSpeed", "windSpeedUnit");
	speedToJson(group.gustSpeed(), "gustSpeed", "gustSpeedUnit");
	directionToJson(group.varSectorBegin(), 
		"varDirectionLowRange", 
		"varDirectionCardinalLowRange",
		"varDirectionLowRangeUnit", 
		"varDirectionLowRangeType", 
		true);
	directionToJson(group.varSectorEnd(), 
		"varDirectionHighRange", 
		"varDirectionCardinalHighRange",
		"varDirectionHighRangeUnit", 
		"varDirectionHighRangeType", 
		true);
	json.finish();
}

void GroupVisitorJson::visitVisibilityGroup(const metaf::VisibilityGroup & group) {
	json.startObject();
	if (!group.isValid()) json.valueBool("valid", false);
	distanceToJson(group.visibility(), "visibility", "visibilityUnit", "visibilityModifier");
	directionToJson(group.direction(), 
		"direction", 
		"directionCardinal", 
		"directionUnit", 
		"directionType");
	json.finish();
}

void GroupVisitorJson::visitCloudGroup(const metaf::CloudGroup & group) {
	json.startObject();
	if (!group.isValid()) json.valueBool("valid", false);

	if (const auto as = cloudAmountToString(group.amount()); !as.empty()) {
		json.valueStr("amount", as);
	}
	if (const auto octa = cloudAmountToOcta(group.amount());
		std::get<0>(octa) || std::get<1>(octa)) {
			json.valueInt("coverMinimumOcta", std::get<0>(octa));
			json.valueInt("coverMaximumOcta", std::get<1>(octa));
	}
	switch (group.type()) {
		case metaf::CloudGroup::Type::NOT_REPORTED:
		break;

		case metaf::CloudGroup::Type::NONE:
		json.valueNull("convectiveType");
		break;

		case metaf::CloudGroup::Type::TOWERING_CUMULUS:
		json.valueStr("convectiveType", "towering cumulus");
		break;

		case metaf::CloudGroup::Type::CUMULONIMBUS:
		json.valueStr("convectiveType", "cumulonimbus");
		break;

		default:
		undefinedToJson(static_cast<int>(group.type()), "convectiveType");
		break;
	}
	if (group.isCloudLayer()) {
		distanceToJson(group.height(), 
			"baseHeight", 
			"heightUnit", 
			"heightModifier");
	}
	if (group.isVerticalVisibility()) {
		distanceToJson(group.verticalVisibility(), 
			"verticalVisibility", 
			"verticalVisibilityUnit", 
			"verticalVisibilityModifier");
	}
	json.finish();
}

void GroupVisitorJson::visitWeatherGroup(const metaf::WeatherGroup & group) {
	json.startObject();
	if (!group.isValid()) json.valueBool("valid", false);

	bool recent = false;
	bool inVicinity = false;
	std::string intensityStr;
	switch(group.qualifier()) {
		case metaf::WeatherGroup::Qualifier::NONE:		break;
		case metaf::WeatherGroup::Qualifier::RECENT:	recent = true; break;
		case metaf::WeatherGroup::Qualifier::VICINITY:	inVicinity = true; break;
		case metaf::WeatherGroup::Qualifier::LIGHT:		intensityStr = "light"; break;
		case metaf::WeatherGroup::Qualifier::MODERATE:	intensityStr = "moderate"; break;
		case metaf::WeatherGroup::Qualifier::HEAVY:		intensityStr = "heavy"; break;
		default: undefinedToJson(static_cast<int>(group.qualifier()), "qualifier"); break;
	}
	json.valueStr("weather", recent ? "recent" : "current");
	json.valueStr("observed", inVicinity ? "in vicinity" : "on site");
	if (!intensityStr.empty()) json.valueStr("intensity", intensityStr);
	
	if (const auto ds = weatherDescriptorToString(group.descriptor()); !ds.empty()) {
		json.valueStr("descriptor", ds);
	}

	json.startArray("weatherPhenomena");
	auto weather = group.weather();
	if (!weather.empty()) {
		for (const auto w : weather) {
			if (w != metaf::WeatherGroup::Weather::NOT_REPORTED) {
				json.valueStr(weatherPhenomenaToString(w));
			}
		}
	} else { //if (!weather.empty()) 
		json.valueNull();
	}

	json.finish(); //array
	json.finish(); //object
}

void GroupVisitorJson::visitTemperatureGroup(const metaf::TemperatureGroup & group) {
	temperatureToJson(group.airTemperature(),
		"airTemperature", 
		"airTemperatureUnit", 
		"airTemperatureFreezing");
	temperatureToJson(group.dewPoint(),
		"dewPoint", 
		"dewPointUnit", 
		"dewPointFreezing");
	
	if (auto rh = metaf::Temperature::relativeHumidity(group.airTemperature(), group.dewPoint()); 
		rh.has_value()) {
			json.valueFloat("airRelativeHumidify", *rh);
			json.valueStr("airRelativeHumidifyUnit", "percent");
	}
	if (!group.isValid()) {
		json.valueBool("airTemperatureValid", false);
		json.valueBool("dewPointValid", false);
	}
}

void GroupVisitorJson::visitTemperatureForecastGroup(
	const metaf::TemperatureForecastGroup & group)
{
	std::string objectName = 
		temperatureForecastPointToString(group.point()) + "Temperature"s;
	if (group.point() == metaf::TemperatureForecastGroup::Point::NOT_SPECIFIED) {
		objectName = "temperatureForecast" +	
			std::to_string(notSpecifiedTemperatureForecastCounter++);
	}
	json.startObject(objectName);
	if (!group.isValid()) json.valueBool("valid", false);
	temperatureToJson(group.airTemperature(),
		"airTemperature", 
		"airTemperatureUnit", 
		"airTemperatureFreezing");
	metafTimeToJson(group.time(), "expectedOnDay", "expectedOnTime");
	json.finish();
}

void GroupVisitorJson::visitPressureGroup(const metaf::PressureGroup & group) {
	if (!group.isValid()) json.valueBool("atmosphericPressureValid", false);
	std::string pressureStr = "unknownPressure";
	switch (group.type()) {
		case metaf::PressureGroup::Type::OBSERVED_QNH:
		pressureStr = "observedMeanSeaLevelPressure";
		break;

		case metaf::PressureGroup::Type::FORECAST_LOWEST_QNH:
		pressureStr = "forecastLowestSeaLevelPressure";
		break;

		case metaf::PressureGroup::Type::OBSERVED_QFE:
		pressureStr = "observedActualPressure";
		break;

		default:
		break;
	}
	const std::string unitStr = pressureStr + "Unit";
	pressureToJson(group.atmosphericPressure(), 
		pressureStr, 
		unitStr);
}

void GroupVisitorJson::visitRunwayVisualRangeGroup(const metaf::RunwayVisualRangeGroup & group) {
	json.startObject();
	if (!group.isValid()) json.valueBool("valid", false);
	runwayToJson(group.runway(), "runway", "runwayDesignator", "lastMessageRepetition");
	if (!group.isVariableVisualRange()) {
		distanceToJson(group.visualRange(), 
			"range", 
			"rangeUnit", 
			"rangeModifier");

	} else { //if (!group.isVariableVisualRange())
		distanceToJson(group.minVisualRange(), 
			"lowLimit", 
			"lowLimitUnit", 
			"lowLimitModifier");
		distanceToJson(group.maxVisualRange(), 
			"highLimit", 
			"highLimitUnit", 
			"highLimitModifier");
	}
	if (const auto trendStr = rvrTrendToString(group.trend()); !trendStr.empty()) {
		json.valueStr("trend", trendStr);
	}
	json.finish();
}

void GroupVisitorJson::visitRunwayStateGroup(const metaf::RunwayStateGroup & group) {
	json.startObject();
	if (!group.isValid()) json.valueBool("valid", false);
	runwayToJson(group.runway(), "runway", "runwayDesignator", "lastMessageRepetition");
	if (group.status() == metaf::RunwayStateGroup::Status::NORMAL) {
		if (const auto ds = runwayStateDepositsToString(group.deposits()); !ds.empty()) {
			json.valueStr("deposits", ds);
		}
		if (const auto es = runwayStateExtentErrorToString(group.contaminationExtent()); 
			!es.empty()) {
				json.valueStr("extent", es);
		}
		if (const auto extent = runwayStateExtentToPercent(group.contaminationExtent()); 
			std::get<0>(extent) || std::get<1>(extent)) {
				json.valueInt("extentMinPercent", std::get<0>(extent));
 				json.valueInt("extentMaxPercent", std::get<1>(extent));
		}
		precipitationToJson(group.depositDepth(),
			"depositsDepth",
			"depositsDepthUnit",
			"runwayOperational",
			"depositsDepthStatus");
	}
	if (group.status() == metaf::RunwayStateGroup::Status::CLRD) {
		json.valueBool("depositsCeasedToExist", true);
	}
	if (group.status() == metaf::RunwayStateGroup::Status::SNOCLO) {
		json.valueBool("closedDueToSnowAccumulation", true);
	}
	if (group.status() == metaf::RunwayStateGroup::Status::NORMAL || 
		group.status() == metaf::RunwayStateGroup::Status::CLRD) {
			surfaceFrictionToJson(group.surfaceFriction(),
				"frictionCoefficient",
				"brakingAction",
				"frictionCoefficientUnreliable",
				"surfaceFrictionStatus");
	}
	if (group.status() != metaf::RunwayStateGroup::Status::NORMAL && 
		group.status() != metaf::RunwayStateGroup::Status::CLRD &&
		group.status() != metaf::RunwayStateGroup::Status::SNOCLO) {
			json.valueStr("status", undefinedToString(static_cast<int>(group.status())));
	}
	json.finish();
}

void GroupVisitorJson::visitWindShearLowLayerGroup(const metaf::WindShearLowLayerGroup & group) {
	json.startObject();
	if (!group.isValid()) json.valueBool("windShearInLowerLayersValid", false);
	if (group.isValid()) {
		runwayToJson(group.runway(), "runway", "runwayDesignator", "lastMessageRepetition");
	}
	json.finish();
}

void GroupVisitorJson::visitRainfallGroup(const metaf::RainfallGroup & group) {
	precipitationToJson(group.rainfallLast10Minutes(),
		"rainfallForLast10Minutes",
		"rainfallForLast10MinutesUnit",
		"runwayOperational",
		"rainfallForLast10MinutesStatus");
	precipitationToJson(group.rainfallLast60Minutes(),
		"rainfallForLast60Minutes",
		"rainfallForLast60MinutesUnit",
		"runwayOperational",
		"rainfallForLast60MinutesStatus");
	precipitationToJson(group.rainfallSince9AM(),
		"rainfallSince9AM",
		"rainfallSince9AMUnit",
		"runwayOperational",
		"rainfallSince9AMStatus");
}

void GroupVisitorJson::visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group) {
	temperatureToJson(group.surfaceTemperature(),
		"seaSurfaceTemperature", 
		"seaSurfaceTemperatureUnit", 
		"seaSurfaceTemperatureFreezing");
	waveHeightToJson(group.waves(),
		"stateOfSeaSurface",
		"seaWaveHeight",
		"seaWaveHeightUnit",
		"seaWaveHeightType");
	if (!group.isValid()) json.valueBool("seaSurfaceDataValid", false);
}

void GroupVisitorJson::visitColourCodeGroup(const metaf::ColourCodeGroup & group) {
	json.valueStr("colourCode", colourCodeToString(group.code()));
	json.valueBool("colourCodeBlack", group.isCodeBlack());
	if (!group.isValid()) json.valueBool("colourCodeValid", false);
}

void GroupVisitorJson::visitMinMaxTemperatureGroup(
	const metaf::MinMaxTemperatureGroup & group)
{
	json.startObject("minMaxTemperature");
	std::string periodStr;
	switch(group.observationPeriod()) {
		case metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6:
		periodStr = "6-hourly";
		break;

		case metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS24:
		periodStr = "24-hourly";
		break;

		default:
		periodStr = "unknown";
		break; 
	}
	json.valueStr("observationPeriod", periodStr);
	temperatureToJson(group.minimum(),
		"minimumTemperature", 
		"mimimumTemperatureUnit", 
		"minumumTemperatureFreezing");
	temperatureToJson(group.maximum(),
		"maximumTemperature", 
		"maximumTemperatureUnit", 
		"maxumumTemperatureFreezing");
	json.finish();
}

void GroupVisitorJson::visitPrecipitationGroup(
	const metaf::PrecipitationGroup & group)
{
	const std::string typeStr = precipitationGroupTypeToString(group.type());
	precipitationToJson(group.amount(),
		typeStr,
		typeStr + "Unit",
		"",
		typeStr + "Status",
		true);	
}

void GroupVisitorJson::visitLayerForecastGroup(
	const metaf::LayerForecastGroup & group)
{
	const auto name = "atmosphericLayerForecast"s + std::to_string(layerForecastCounter++);
	json.startObject(name);
	atmosphericLayerToJson(group.type(), "type", "description", "severity", "frequency");
	distanceToJson(group.baseHeight(), 
		"layerBaseHeight",
		"layerBaseHeightUnit",
		"layerBaseHeightModifier");
	distanceToJson(group.topHeight(),
		"layerTopHeight",
		"layerTopHeightUnit",
		"layerTopHeightModifier");
	json.finish();
}

void GroupVisitorJson::visitPressureTendencyGroup(
	const metaf::PressureTendencyGroup & group)
{
	if (group.type() != metaf::PressureTendencyGroup::Type::NOT_REPORTED ||
		group.difference().pressure().has_value())
	{
		json.startObject("pressureTendency");
		if (group.type() != metaf::PressureTendencyGroup::Type::NOT_REPORTED) {
			json.valueStr("tendency", 
				pressureTendencyTypeToString(group.type()));
			json.valueStr("trend", 
				pressureTendencyTrendToString(
					metaf::PressureTendencyGroup::trend(group.type())));
		}
		json.valueStr("observationPeriod", "3-hourly");
		pressureToJson(group.difference(), 
			"pressureChange",
			"pressureChangeUnit");
		json.finish();
	}
}

void GroupVisitorJson::visitCloudTypesGroup(const metaf::CloudTypesGroup & group) {
	json.startArray("cloudTypes");
	for (const auto clouds : group.toVector()) {
		json.startObject();
		json.valueStr("type", 
			cloudTypeToString(std::get<metaf::CloudTypesGroup::Type>(clouds)));
		json.valueInt("okta", std::get<unsigned int>(clouds));
		json.finish();
	}
	json.finish();
}

void GroupVisitorJson::visitCloudLayersGroup(const metaf::CloudLayersGroup & group) {
	json.valueStr("cloudLowLayer", cloudLowLayerToString(group.lowLayer()));
	json.valueStr("cloudMidLayer", cloudMidLayerToString(group.midLayer()));
	json.valueStr("cloudHighLayer", cloudHighLayerToString(group.highLayer()));
}

void GroupVisitorJson::visitMiscGroup(
	const metaf::MiscGroup & group)
{
	switch (group.type()) {
		case metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES:
		if (!group.value().has_value()) break;
		json.valueInt("sunshineDuration", group.value().value());
		json.valueStr("sunshineDurationUnit", "minutes");
		break;

		default:
		break;
	}	
}



void GroupVisitorJson::visitOther(const metaf::Group & group) {
	(void)group;
	json.valueNull("unknownGroup");
}

void GroupVisitorJson::similarGroupsToArrays(const metaf::Group & group) {
	if (!std::holds_alternative<metaf::VisibilityGroup>(group) && 
		std::holds_alternative<metaf::VisibilityGroup>(lastGroup))	{
			json.finish(); //json.startArray("visibility");
	}
	if (!std::holds_alternative<metaf::CloudGroup>(group) && 
		std::holds_alternative<metaf::CloudGroup>(lastGroup))	{
			json.finish(); //json.startArray("cloudLayers");
	}
	if (!std::holds_alternative<metaf::WeatherGroup>(group) && 
		std::holds_alternative<metaf::WeatherGroup>(lastGroup))	{
			json.finish(); //json.startArray("weather");
	}
	if (!std::holds_alternative<metaf::RunwayVisualRangeGroup>(group) && 
		std::holds_alternative<metaf::RunwayVisualRangeGroup>(lastGroup))	{
			json.finish(); //json.startArray("runwayVisualRange");
	}
	if (!std::holds_alternative<metaf::RunwayStateGroup>(group) && 
		std::holds_alternative<metaf::RunwayStateGroup>(lastGroup))	{
			json.finish(); //json.startArray("runwayState");
	}

	if (!std::holds_alternative<metaf::WindShearLowLayerGroup>(group) && 
		std::holds_alternative<metaf::WindShearLowLayerGroup>(lastGroup))	{
			json.finish(); //json.startArray("windShearInLowerLayers");
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
	if (std::holds_alternative<metaf::WindShearLowLayerGroup>(group) && 
		!std::holds_alternative<metaf::WindShearLowLayerGroup>(lastGroup)) {
			json.startArray("windShearInLowerLayers");
	}
}

void GroupVisitorJson::undefinedToJson(int value, std::string_view name) {
	json.valueStr(name, "undefined_"s + std::to_string(value));
}

void GroupVisitorJson::runwayToJson(const metaf::Runway & runway,
	std::string_view valueName,
	std::string_view designatorName,
	std::string_view lastMessageRepetitionName)
{
	if (runway.isAllRunways()) {
		json.valueStr(valueName, "all");
		return;
	}
	if (runway.isMessageRepetition()) {
		json.valueBool(lastMessageRepetitionName, true);
		return;
	}
	json.valueStr(valueName, 
		(runway.number() < 10 ? "0"s : ""s) + std::to_string(runway.number()));
	switch(runway.designator()) {
		case metaf::Runway::Designator::NONE: break;
		case metaf::Runway::Designator::LEFT:   json.valueStr(designatorName, "left"); break;
		case metaf::Runway::Designator::CENTER:	json.valueStr(designatorName, "center"); break;
		case metaf::Runway::Designator::RIGHT:  json.valueStr(designatorName, "right"); break;
		default: undefinedToJson(static_cast<int>(runway.designator()), designatorName); break;
	}	
}

void GroupVisitorJson::metafTimeToJson(const metaf::MetafTime & metafTime,
	std::string_view dayName,
	std::string_view timeName)
{
	if (const auto day = metafTime.day(); day.has_value()) json.valueInt(dayName, *day);
	const auto hourStr = 
		(metafTime.hour() < 10 ? "0"s : ""s) + std::to_string(metafTime.hour());
	const auto minuteStr = 
		(metafTime.minute() < 10 ? "0"s : ""s) + std::to_string(metafTime.minute());
	static const std::string hourMinuteSeparator(":");
	json.valueStr(timeName, hourStr + hourMinuteSeparator + minuteStr);

}

void GroupVisitorJson::temperatureToJson(const metaf::Temperature & temperature,
	std::string_view valueName,
	std::string_view unitName,
	std::string_view freezingName)
{
	if (const auto t = temperature.temperature(); t.has_value()) {
		if (temperature.isPrecise()) {
			json.valueFloat(valueName, *t);
		} else {
			json.valueInt(valueName, *t);
		}
		switch (temperature.unit()) {
			case metaf::Temperature::Unit::C: json.valueStr(unitName, "C"); break;
			case metaf::Temperature::Unit::F: json.valueStr(unitName, "F"); break;
			default: undefinedToJson(static_cast<int>(temperature.unit()), unitName); break;
		}
		json.valueBool(freezingName, temperature.isFreezing());
	}
}

void GroupVisitorJson::speedToJson(const metaf::Speed & speed,
	std::string_view valueName,
	std::string_view unitName)
{
	
	if (const auto s = speed.speed(); s.has_value()) {
		json.valueInt(valueName, *s);
		switch (speed.unit()) {
			case metaf::Speed::Unit::KNOTS:
			json.valueStr(unitName, "knots");
			break;
			
			case metaf::Speed::Unit::METERS_PER_SECOND:
			json.valueStr(unitName, "m/s");
			break;
			
			case metaf::Speed::Unit::KILOMETERS_PER_HOUR:
			json.valueStr(unitName, "km/h");
			break;

			case metaf::Speed::Unit::MILES_PER_HOUR:
			json.valueStr(unitName, "mph");
			break;
			
			default:
			undefinedToJson(static_cast<int>(speed.unit()), unitName);
			break;
		}
	}
}

void GroupVisitorJson::distanceToJson(const metaf::Distance & distance,
	std::string_view valueName,
	std::string_view unitName,
	std::string_view modifierName)
{
	
	if (const auto d = distance.toUnit(distance.unit()); d.has_value()) {
		json.valueFloat(valueName, *d);
		switch (distance.unit()) {
			case metaf::Distance::Unit::METERS:
			json.valueStr(unitName, "meters");
			break;

			case metaf::Distance::Unit::STATUTE_MILES:
			json.valueStr(unitName, "statute miles");
			break;

			case metaf::Distance::Unit::FEET:
			json.valueStr(unitName, "feet");
			break;

			default:
			undefinedToJson(static_cast<int>(distance.unit()), unitName);
			break;
		}
		switch(distance.modifier()) {
			case metaf::Distance::Modifier::NONE:
			break;

			case metaf::Distance::Modifier::LESS_THAN:
			json.valueStr(modifierName, "<");
			break;

			case metaf::Distance::Modifier::MORE_THAN:
			json.valueStr(modifierName, ">");
			break;

			default:
			undefinedToJson(static_cast<int>(distance.modifier()), modifierName);
			break;
		}
	}
}

void GroupVisitorJson::directionToJson(const metaf::Direction & direction,
	std::string_view degreesValueName,
	std::string_view cardinalValueName,
	std::string_view unitName,
	std::string_view undefinedName,
	bool trueCardinalDirections)
{
	switch(direction.status()) {
		case metaf::Direction::Status::OMMITTED:
		case metaf::Direction::Status::NOT_REPORTED:
		return;

		case metaf::Direction::Status::VARIABLE:
		json.valueStr(degreesValueName, "variable");
		return;

		case metaf::Direction::Status::NDV:
		json.valueStr(degreesValueName, "no_directional_variation");
		return;

		case metaf::Direction::Status::VALUE_DEGREES:
		if (const auto d = direction.degrees(); d.has_value()) {
			json.valueInt(degreesValueName, *d);
			json.valueStr(unitName, "degrees");
		}
		json.valueStr(cardinalValueName, 
			cardinalDirectionToString(direction.cardinal(trueCardinalDirections)));
		break;

		case metaf::Direction::Status::VALUE_CARDINAL:
		json.valueStr(cardinalValueName, 
			cardinalDirectionToString(direction.cardinal(trueCardinalDirections)));
		break;

		default:
		undefinedToJson(static_cast<int>(direction.status()), undefinedName);
		break;
	}
}

void GroupVisitorJson::pressureToJson(const metaf::Pressure & pressure,
	std::string_view valueName,
	std::string_view unitName)
{
	
	if (const auto p = pressure.pressure(); p.has_value()) {
		json.valueFloat(valueName, *p);
		switch(pressure.unit()) {
			case metaf::Pressure::Unit::HECTOPASCAL:
			json.valueStr(unitName, "hPa");
			break;

			case metaf::Pressure::Unit::INCHES_HG:
			json.valueStr(unitName, "inHg");
			break;

			case metaf::Pressure::Unit::MM_HG:
			json.valueStr(unitName, "inHg");
			break;

			default: undefinedToJson(static_cast<int>(pressure.unit()), unitName); break;
		}
	}
}

void GroupVisitorJson::precipitationToJson(const metaf::Precipitation & precipitation,
	std::string_view valueName,
	std::string_view unitName,
	std::string_view runwayOperationalName,
	std::string_view undefinedStatusName,
	bool allowTrace)
{
	switch (precipitation.status()) {
		case metaf::Precipitation::Status::NOT_REPORTED:
		break;

		case metaf::Precipitation::Status::RUNWAY_NOT_OPERATIONAL:
		json.valueBool(runwayOperationalName, false);
		break;

		case metaf::Precipitation::Status::REPORTED:
		if (const auto p = precipitation.precipitation(); p.has_value()) {
		if (!*p && allowTrace) {
			json.valueStr(valueName, "traceAmount");
		} else {
			json.valueFloat(valueName, *p);
		}
		switch(precipitation.unit()) {
			case metaf::Precipitation::Unit::MM:
			json.valueStr(unitName, "mm");
			break;

			case metaf::Precipitation::Unit::INCHES:
			json.valueStr(unitName, "inches");
			break;

			default:
			undefinedToJson(static_cast<int>(precipitation.unit()), unitName);
			break;
		}
		}
		break;

		default:
		undefinedToJson(static_cast<int>(precipitation.status()), undefinedStatusName);
		break;
	}
}

void GroupVisitorJson::surfaceFrictionToJson(const metaf::SurfaceFriction & surfaceFriction,
	std::string_view coefficientName,
	std::string_view brakingActionName,
	std::string_view unreliableName,
	std::string_view undefinedStatusName)
{
	switch(surfaceFriction.status()) {
		case metaf::SurfaceFriction::Status::NOT_REPORTED:
		break;

		case metaf::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED:
		if (const auto c = surfaceFriction.coefficient(); c.has_value()) {
			json.valueFloat(coefficientName, *c);
		}
		break;

		case metaf::SurfaceFriction::Status::BRAKING_ACTION_REPORTED:
		switch(surfaceFriction.brakingAction()) {
			case metaf::SurfaceFriction::BrakingAction::NONE:
			break;

			case metaf::SurfaceFriction::BrakingAction::POOR:
			json.valueStr(brakingActionName, "poor");
			break;

			case metaf::SurfaceFriction::BrakingAction::MEDIUM_POOR:
			json.valueStr(brakingActionName, "medium/poor");
			break;

			case metaf::SurfaceFriction::BrakingAction::MEDIUM:
			json.valueStr(brakingActionName, "medium");
			break;

			case metaf::SurfaceFriction::BrakingAction::MEDIUM_GOOD:
			json.valueStr(brakingActionName, "medium/good");
			break;

			case metaf::SurfaceFriction::BrakingAction::GOOD:
			json.valueStr(brakingActionName, "good");
			break;

			default:
			undefinedToJson(static_cast<int>(surfaceFriction.brakingAction()), brakingActionName);
		}
		break;

		case metaf::SurfaceFriction::Status::UNRELIABLE:
		json.valueNull(coefficientName);
		json.valueBool(unreliableName, true);
		break;

		default:
		undefinedToJson(static_cast<int>(surfaceFriction.status()), undefinedStatusName);
		break;
	}
}

void GroupVisitorJson::waveHeightToJson(const metaf::WaveHeight waveHeight,
	std::string_view stateOfSurfaceName,
	std::string_view waveHeightName,
	std::string_view waveHeightUnitName,
	std::string_view undefinedTypeName)
{
	
	const auto h = waveHeight.waveHeight();
	switch (waveHeight.type()) {
		case metaf::WaveHeight::Type::STATE_OF_SURFACE:
		if (waveHeight.stateOfSurface() != metaf::WaveHeight::StateOfSurface::NOT_REPORTED) {
			json.valueStr(stateOfSurfaceName, 
				stateOfSeaSurfaceToString(waveHeight.stateOfSurface()));	
		}
		break;

		case metaf::WaveHeight::Type::WAVE_HEIGHT:
		if (h.has_value()) {
			json.valueFloat(waveHeightName, *h);
			switch(waveHeight.unit()) {
				case metaf::WaveHeight::Unit::METERS:
				json.valueStr(waveHeightUnitName, "meters");
				break;

				case metaf::WaveHeight::Unit::FEET:
				json.valueStr(waveHeightUnitName, "feet");
				break;

				default: 
				undefinedToJson(static_cast<int>(waveHeight.unit()), waveHeightUnitName);
				break;
			}
			
		}
		break;

		default:
		undefinedToJson(static_cast<int>(waveHeight.type()), undefinedTypeName);
		break;
	}
}

void GroupVisitorJson::atmosphericLayerToJson(const metaf::LayerForecastGroup::Type type,
	std::string_view typeName,
	std::string_view descriptionName,
	std::string_view severityName,
	std::string_view frequencyName)
{
	std::string typeStr, descriptionStr, severityStr, frequencyStr;
	switch (type) {
		case metaf::LayerForecastGroup::Type::ICING_TRACE_OR_NONE:
		typeStr = "icing";
		descriptionStr = "traceOrNone";
		severityStr = "none";
		break;

		case metaf::LayerForecastGroup::Type::ICING_LIGHT_MIXED:
		typeStr = "icing";
		descriptionStr = "mixed";
		severityStr = "light";
		break;
		
		case metaf::LayerForecastGroup::Type::ICING_LIGHT_RIME_IN_CLOUD:
		typeStr = "icing";
		descriptionStr = "rimeInCloud";
		severityStr = "light";
		break;
		
		case metaf::LayerForecastGroup::Type::ICING_LIGHT_CLEAR_IN_PRECIPITATION:
		typeStr = "icing";
		descriptionStr = "clearInPrecipitation";
		severityStr = "light";
		break;
		
		case metaf::LayerForecastGroup::Type::ICING_MODERATE_MIXED:
		typeStr = "icing";
		descriptionStr = "mixed";
		severityStr = "moderate";
		break;
		
		case metaf::LayerForecastGroup::Type::ICING_MODERATE_RIME_IN_CLOUD:
		typeStr = "icing";
		descriptionStr = "rimeInCloud";
		severityStr = "moderate";
		break;
		
		case metaf::LayerForecastGroup::Type::ICING_MODERATE_CLEAR_IN_PRECIPITATION:
		typeStr = "icing";
		descriptionStr = "clearInPrecipitation";
		severityStr = "moderate";
		break;
		
		case metaf::LayerForecastGroup::Type::ICING_SEVERE_MIXED:
		typeStr = "icing";
		descriptionStr = "mixed";
		severityStr = "severe";
		break;
		
		case metaf::LayerForecastGroup::Type::ICING_SEVERE_RIME_IN_CLOUD:
		typeStr = "icing";
		descriptionStr = "rimeInCloud";
		severityStr = "severe";
		break;
		
		case metaf::LayerForecastGroup::Type::ICING_SEVERE_CLEAR_IN_PRECIPITATION:
		typeStr = "icing";
		descriptionStr = "clearInPrecipitation";
		severityStr = "severe";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_NONE:
		typeStr = "turbulence";
		severityStr = "none";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_LIGHT:
		typeStr = "turbulence";
		severityStr = "light";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASSIONAL:
		typeStr = "turbulence";
		descriptionStr = "clearAir";
		severityStr = "moderate";
		frequencyStr = "occasional";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT:
		typeStr = "turbulence";
		descriptionStr = "clearAir";
		severityStr = "moderate";
		frequencyStr = "frequent";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLOUD_OCCASSIONAL:
		typeStr = "turbulence";
		descriptionStr = "inCloud";
		severityStr = "moderate";
		frequencyStr = "occasional";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLOUD_FREQUENT:
		typeStr = "turbulence";
		descriptionStr = "inCloud";
		severityStr = "moderate";
		frequencyStr = "frequent";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASSIONAL:
		typeStr = "turbulence";
		descriptionStr = "clearAir";
		severityStr = "severe";
		frequencyStr = "occasional";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT:
		typeStr = "turbulence";
		descriptionStr = "clearAir";
		severityStr = "severe";
		frequencyStr = "frequent";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLOUD_OCCASSIONAL:
		typeStr = "turbulence";
		descriptionStr = "inCloud";
		severityStr = "severe";
		frequencyStr = "occasional";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLOUD_FREQUENT:
		typeStr = "turbulence";
		descriptionStr = "inCloud";
		severityStr = "severe";
		frequencyStr = "frequent";
		break;
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_EXTREME:
		typeStr = "turbulence";
		severityStr = "extreme";
		break;
		
		default:
		undefinedToJson(static_cast<int>(type), typeName);
		return;
	}
	if (!typeStr.empty()) json.valueStr(typeName, typeStr);
	if (!descriptionStr.empty()) json.valueStr(descriptionName, descriptionStr);
	if (!severityStr.empty()) json.valueStr(severityName, severityStr);
	if (!frequencyStr.empty()) json.valueStr(frequencyName, frequencyStr);

}

std::string GroupVisitorJson::undefinedToString(int value) {
	return("undefined_"s + std::to_string(value));
}

std::string GroupVisitorJson::reportTypeToString(metaf::ReportType reportType) {
	switch (reportType) {
		case metaf::ReportType::UNKNOWN: return("unknown"); break;
		case metaf::ReportType::METAR: 	 return("METAR"); break;
		case metaf::ReportType::TAF:	 return("TAF"); break;
		default: return(undefinedToString(static_cast<int>(reportType)));
	}
}

std::string GroupVisitorJson::parserErrorToString(metaf::Parser::Error error) {
	switch (error) {
		case metaf::Parser::Error::NONE:	
		return(std::string());

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

		case metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_MAINTENANCE_INDICATOR:
		return("unexpected group after maintenance indicator");
				
		case metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY:
		return("unexpected NIL or CNL in report body");
				
		case metaf::Parser::Error::AMD_ALLOWED_IN_TAF_ONLY:
		return("AMD is allowed only in TAF reports");
				
		case metaf::Parser::Error::CNL_ALLOWED_IN_TAF_ONLY:
		return("CNL is allowed only in TAF reports");

		case metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY:
		return("Maintenance indicator is allowed only in METAR reports");

		case metaf::Parser::Error::INTERNAL_PARSER_STATE:
		return("internal error: unknown parser state");
				
		default: return(undefinedToString(static_cast<int>(error)));
	}
}

std::string GroupVisitorJson::cardinalDirectionToString(metaf::Direction::Cardinal direction) {
	switch(direction) {
		case metaf::Direction::Cardinal::NONE:	return("");
		case metaf::Direction::Cardinal::N:	return("north");
		case metaf::Direction::Cardinal::S: return("south");
		case metaf::Direction::Cardinal::W: return("west");
		case metaf::Direction::Cardinal::E: return("east");
		case metaf::Direction::Cardinal::NW: return("northwest");
		case metaf::Direction::Cardinal::NE: return("northeast");
		case metaf::Direction::Cardinal::SW: return("southwest");
		case metaf::Direction::Cardinal::SE: return("southeast");
		case metaf::Direction::Cardinal::TRUE_N: return("true north");
		case metaf::Direction::Cardinal::TRUE_W: return("true west");
		case metaf::Direction::Cardinal::TRUE_S: return("true south");
		case metaf::Direction::Cardinal::TRUE_E: return("true east");
		default: return(undefinedToString(static_cast<int>(direction)));
	}
}

std::string GroupVisitorJson::stateOfSeaSurfaceToString(
	metaf::WaveHeight::StateOfSurface state)
{
	switch(state) {
		case metaf::WaveHeight::StateOfSurface::NOT_REPORTED:	return("not reported");
		case metaf::WaveHeight::StateOfSurface::CALM_GLASSY:	return("calm (glassy)");
		case metaf::WaveHeight::StateOfSurface::CALM_RIPPLED:	return("calm (rippled)");
		case metaf::WaveHeight::StateOfSurface::SMOOTH:			return("smooth");
		case metaf::WaveHeight::StateOfSurface::SLIGHT:			return("slight waves");
		case metaf::WaveHeight::StateOfSurface::MODERATE:		return("moderate waves");
		case metaf::WaveHeight::StateOfSurface::ROUGH:			return("rough");
		case metaf::WaveHeight::StateOfSurface::VERY_ROUGH:		return("very rough");
		case metaf::WaveHeight::StateOfSurface::HIGH:			return("high waves");
		case metaf::WaveHeight::StateOfSurface::VERY_HIGH:		return("very high waves");
		case metaf::WaveHeight::StateOfSurface::PHENOMENAL:		return("phenomenal waves");
		default: return(undefinedToString(static_cast<int>(state)));
	}
}

std::string GroupVisitorJson::trendTypeToString(metaf::TrendGroup::Type type) {
	switch (type) {
		case metaf::TrendGroup::Type::NONE:			return("none");
		case metaf::TrendGroup::Type::NOSIG:		return("nosig");
		case metaf::TrendGroup::Type::BECMG:		return("becmg");
		case metaf::TrendGroup::Type::TEMPO:		return("tempo");
		case metaf::TrendGroup::Type::INTER:		return("inter");
		case metaf::TrendGroup::Type::FROM:			return("from");
		case metaf::TrendGroup::Type::TIME_SPAN:	return("time span");
		default: return(undefinedToString(static_cast<int>(type)));
	}
}

std::string GroupVisitorJson::cloudAmountToString(metaf::CloudGroup::Amount amount) {
	switch (amount) {
		case metaf::CloudGroup::Amount::NOT_REPORTED: 	return(std::string());
		case metaf::CloudGroup::Amount::NCD:			return("no clouds detected");
		case metaf::CloudGroup::Amount::NSC:			return("no significant cloud");
		case metaf::CloudGroup::Amount::NONE_CLR:		return("none");
		case metaf::CloudGroup::Amount::NONE_SKC:		return("none");
		case metaf::CloudGroup::Amount::FEW:			return("few");
		case metaf::CloudGroup::Amount::SCATTERED:		return("scattered");
		case metaf::CloudGroup::Amount::BROKEN:			return("broken");
		case metaf::CloudGroup::Amount::OVERCAST:		return("overcast");
		case metaf::CloudGroup::Amount::OBSCURED:		return("sky obscured");
		default: return(undefinedToString(static_cast<int>(amount)));
		break;
	}

}

std::pair<int, int> GroupVisitorJson::cloudAmountToOcta(metaf::CloudGroup::Amount amount) {
	switch (amount) {
		case metaf::CloudGroup::Amount::NOT_REPORTED:
		case metaf::CloudGroup::Amount::NCD:
		case metaf::CloudGroup::Amount::NSC:
		case metaf::CloudGroup::Amount::NONE_CLR:
		case metaf::CloudGroup::Amount::NONE_SKC:
		return(std::pair(0, 0));

		case metaf::CloudGroup::Amount::FEW:		return(std::pair(1, 2));
		case metaf::CloudGroup::Amount::SCATTERED:	return(std::pair(3, 4));
		case metaf::CloudGroup::Amount::BROKEN:		return(std::pair(5, 7));
		case metaf::CloudGroup::Amount::OVERCAST:	return(std::pair(8, 8));

		case metaf::CloudGroup::Amount::OBSCURED:
		default: 
		return(std::pair(0, 0));
	}

}

std::string GroupVisitorJson::weatherDescriptorToString(
	metaf::WeatherGroup::Descriptor descriptor)
{
	switch(descriptor) {
		case metaf::WeatherGroup::Descriptor::NONE:			return(std::string());
		case metaf::WeatherGroup::Descriptor::SHALLOW:		return("shallow");
		case metaf::WeatherGroup::Descriptor::PARTIAL:		return("partial");
		case metaf::WeatherGroup::Descriptor::PATCHES:		return("patches");
		case metaf::WeatherGroup::Descriptor::LOW_DRIFTING: return("low drifting");
		case metaf::WeatherGroup::Descriptor::BLOWING:		return("blowing");
		case metaf::WeatherGroup::Descriptor::SHOWERS:		return("showers");
		case metaf::WeatherGroup::Descriptor::THUNDERSTORM:	return("thunderstorm");
		case metaf::WeatherGroup::Descriptor::FREEZING:		return("freezing");
		default: return(undefinedToString(static_cast<int>(descriptor)));
	}
}

std::string GroupVisitorJson::weatherPhenomenaToString(
	metaf::WeatherGroup::Weather weather)
{
	switch (weather) {
		case metaf::WeatherGroup::Weather::NOT_REPORTED: return(std::string());
		case metaf::WeatherGroup::Weather::DRIZZLE:		 return("drizzle");
		case metaf::WeatherGroup::Weather::RAIN:		 return("rain");
		case metaf::WeatherGroup::Weather::SNOW:		 return("snow");
		case metaf::WeatherGroup::Weather::SNOW_GRAINS:	 return("snow grains");
		case metaf::WeatherGroup::Weather::ICE_CRYSTALS: return("ice crystals");
		case metaf::WeatherGroup::Weather::ICE_PELLETS:	 return("ice pellets");
		case metaf::WeatherGroup::Weather::HAIL:		 return("hail");
		case metaf::WeatherGroup::Weather::SMALL_HAIL:	 return("small hail");
		case metaf::WeatherGroup::Weather::UNDETERMINED: return("undetermined precipitation");
		case metaf::WeatherGroup::Weather::MIST:		 return("mist");
		case metaf::WeatherGroup::Weather::FOG:			 return("fog");
		case metaf::WeatherGroup::Weather::SMOKE:		 return("smoke");
		case metaf::WeatherGroup::Weather::VOLCANIC_ASH: return("volcanic ash");
		case metaf::WeatherGroup::Weather::DUST:		 return("dust");
		case metaf::WeatherGroup::Weather::SAND:		 return("sand");
		case metaf::WeatherGroup::Weather::HAZE:		 return("haze");
		case metaf::WeatherGroup::Weather::SPRAY:		 return("spray");
		case metaf::WeatherGroup::Weather::DUST_WHIRLS:	 return("dust or sand whirls");
		case metaf::WeatherGroup::Weather::SQUALLS:		 return("squalls");
		case metaf::WeatherGroup::Weather::FUNNEL_CLOUD: return("funnel cloud");
		case metaf::WeatherGroup::Weather::SANDSTORM:	 return("sand storm");
		case metaf::WeatherGroup::Weather::DUSTSTORM:	 return("dust storm");
		default: return(undefinedToString(static_cast<int>(weather)));
	}
}

std::string GroupVisitorJson::temperatureForecastPointToString(
		metaf::TemperatureForecastGroup::Point point) 
{
	switch(point) {
		case metaf::TemperatureForecastGroup::Point::NOT_SPECIFIED:	return("notSpecified");
		case metaf::TemperatureForecastGroup::Point::MINIMUM:	return("minimum");
		case metaf::TemperatureForecastGroup::Point::MAXIMUM:	return("maximum");
		default: return(undefinedToString(static_cast<int>(point)));
	}
}

std::string GroupVisitorJson::rvrTrendToString(metaf::RunwayVisualRangeGroup::Trend trend){
	switch (trend) {
		case metaf::RunwayVisualRangeGroup::Trend::NONE:		 return(std::string());
		case metaf::RunwayVisualRangeGroup::Trend::NOT_REPORTED: return(std::string());
		case metaf::RunwayVisualRangeGroup::Trend::UPWARD:		 return("upward");
		case metaf::RunwayVisualRangeGroup::Trend::NEUTRAL:		 return("neutral");
		case metaf::RunwayVisualRangeGroup::Trend::DOWNWARD:	 return("downward");
		default: return(undefinedToString(static_cast<int>(trend)));
	}
}

std::string GroupVisitorJson::runwayStateDepositsToString(
	metaf::RunwayStateGroup::Deposits deposits)
{
	switch(deposits) {
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

		case metaf::RunwayStateGroup::Deposits::NOT_REPORTED:
		return("not reported");

		default:
		return(undefinedToString(static_cast<int>(deposits)));
	}
}

std::string GroupVisitorJson::runwayStateExtentErrorToString(
	metaf::RunwayStateGroup::Extent extent)
{
	switch(extent) {
		case metaf::RunwayStateGroup::Extent::NONE:
		case metaf::RunwayStateGroup::Extent::LESS_THAN_10_PERCENT:
		case metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT:
		case metaf::RunwayStateGroup::Extent::FROM_26_TO_50_PERCENT:
		case metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT:
		case metaf::RunwayStateGroup::Extent::NOT_REPORTED:
		return(std::string());

		case metaf::RunwayStateGroup::Extent::RESERVED_3:	return("reserved_3");
		case metaf::RunwayStateGroup::Extent::RESERVED_4:	return("reserved_4");
		case metaf::RunwayStateGroup::Extent::RESERVED_6:	return("reserved_6");
		case metaf::RunwayStateGroup::Extent::RESERVED_7:	return("reserved_7");
		case metaf::RunwayStateGroup::Extent::RESERVED_8:	return("reserved_8");
		default: undefinedToString(static_cast<int>(extent));
	}
}

std::pair<int, int> GroupVisitorJson::runwayStateExtentToPercent(
	metaf::RunwayStateGroup::Extent extent)
{
	switch(extent) {
		case metaf::RunwayStateGroup::Extent::LESS_THAN_10_PERCENT:
		return(std::pair(0, 10));

		case metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT:
		return(std::pair(11, 25));

		case metaf::RunwayStateGroup::Extent::FROM_26_TO_50_PERCENT:
		return(std::pair(26, 50));

		case metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT:
		return(std::pair(51, 100));

		case metaf::RunwayStateGroup::Extent::NONE:
		case metaf::RunwayStateGroup::Extent::RESERVED_3:
		case metaf::RunwayStateGroup::Extent::RESERVED_4:
		case metaf::RunwayStateGroup::Extent::RESERVED_6:
		case metaf::RunwayStateGroup::Extent::RESERVED_7:
		case metaf::RunwayStateGroup::Extent::RESERVED_8:
		case metaf::RunwayStateGroup::Extent::NOT_REPORTED:
		default: 
		return(std::pair(0, 0));
	}

}

std::string GroupVisitorJson::colourCodeToString(metaf::ColourCodeGroup::Code code)
{
	switch(code) {
		case metaf::ColourCodeGroup::Code::BLUE:	return("blue");
		case metaf::ColourCodeGroup::Code::WHITE:	return("white");
		case metaf::ColourCodeGroup::Code::GREEN:	return("green");
		case metaf::ColourCodeGroup::Code::YELLOW1:	return("yellow1");
		case metaf::ColourCodeGroup::Code::YELLOW2:	return("yellow2");
		case metaf::ColourCodeGroup::Code::AMBER:	return("amber");
		case metaf::ColourCodeGroup::Code::RED:		return("red");
		default: return(undefinedToString(static_cast<int>(code)));
	}
}

std::string GroupVisitorJson::precipitationGroupTypeToString(
	metaf::PrecipitationGroup::Type type)
{
	switch (type) {
		case metaf::PrecipitationGroup::Type::TOTAL_PRECIPITATION_HOURLY:
		return("totalPrecipitationForPastHour");

		case metaf::PrecipitationGroup::Type::SNOW_DEPTH_ON_GROUND:
		return("snowDepthOnGround");

		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_OR_6_HOURLY:
		return("waterEquivalentOfFrozenPrecipitationForLast3or6hours");
	
		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_24_HOURLY:
		return("waterEquivalentOfFrozenPrecipitationForLast24hours");

		case metaf::PrecipitationGroup::Type::SNOW_6_HOURLY:
		return("snowfallForLast6hours");

		case metaf::PrecipitationGroup::Type::WATER_EQUIV_OF_SNOW_ON_GROUND:
		return("waterEquivalentOfSnowOnGround");

		case metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_HOUR:
		return("iceAccretionForLastHour");

		case metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_3_HOURS:
		return("iceAccretionForLast3Hours");

		case metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_6_HOURS:
		return("iceAccretionForLast6Hours");

		default: return(undefinedToString(static_cast<int>(type)));
	}
}

std::string GroupVisitorJson::pressureTendencyTypeToString(
	metaf::PressureTendencyGroup::Type type)
{
	switch(type) {
		case metaf::PressureTendencyGroup::Type::NOT_REPORTED:
		return("notReported");
		
		case metaf::PressureTendencyGroup::Type::INCREASING_THEN_DECREASING:
		return("increasingThenDecreasing");
		
		case metaf::PressureTendencyGroup::Type::INCREASING_MORE_SLOWLY:
		return("increasingMoreSlowly");

		case metaf::PressureTendencyGroup::Type::INCREASING:
		return("increasingSteadilyOrUnsteadily");

		case metaf::PressureTendencyGroup::Type::INCREASING_MORE_RAPIDLY:
		return("increasingMoreRapidly");
		
		case metaf::PressureTendencyGroup::Type::STEADY:
		return("steady");
		
		case metaf::PressureTendencyGroup::Type::DECREASING_THEN_INCREASING:
		return("decreasingThenIncreasing");
		
		case metaf::PressureTendencyGroup::Type::DECREASING_MORE_SLOWLY:
		return("decreasingMoreSlowly");
		
		case metaf::PressureTendencyGroup::Type::DECREASING:
		return("decreasingSteadilyOrUnsteadily");
		
		case metaf::PressureTendencyGroup::Type::DECREASING_MORE_RAPIDLY:
		return("decreasingMoreRapidly");
		
		default:
		return(undefinedToString(static_cast<int>(type)));
	}
}

std::string GroupVisitorJson::pressureTendencyTrendToString(
	metaf::PressureTendencyGroup::Trend trend)
{
	switch(trend) {
		case metaf::PressureTendencyGroup::Trend::NOT_REPORTED:
		return("notReported");

		case metaf::PressureTendencyGroup::Trend::HIGHER:
		return("higherThanBefore");

		case metaf::PressureTendencyGroup::Trend::HIGHER_OR_SAME:
		return("higherOrSameAsBefore");

		case metaf::PressureTendencyGroup::Trend::SAME:
		return("sameAsBefore");

		case metaf::PressureTendencyGroup::Trend::LOWER_OR_SAME:
		return("lowerOrSameAsBefore");

		case metaf::PressureTendencyGroup::Trend::LOWER:
		return("lowerThanBefore");

		default:
		return(undefinedToString(static_cast<int>(trend)));
	}
}

std::string GroupVisitorJson::cloudTypeToString(metaf::CloudTypesGroup::Type type) {
	switch(type) {
		case metaf::CloudTypesGroup::Type::CUMULONIMBUS:
		return("cumulonimbus");

		case metaf::CloudTypesGroup::Type::TOWERING_CUMULUS:
		return("toweringCumulus");

		case metaf::CloudTypesGroup::Type::CUMULUS:
		return("cumulus");

		case metaf::CloudTypesGroup::Type::CUMULUS_FRACTUS:
		return("cumulusFractus");

		case metaf::CloudTypesGroup::Type::STRATOCUMULUS:
		return("stratocumulus");

		case metaf::CloudTypesGroup::Type::NIMBOSTRATUS:
		return("nimbostratus");

		case metaf::CloudTypesGroup::Type::STRATUS:
		return("stratus");

		case metaf::CloudTypesGroup::Type::STRATUS_FRACTUS:
		return("stratusFractus");

		case metaf::CloudTypesGroup::Type::ALTOSTRATUS:
		return("altostratus");
		
		case metaf::CloudTypesGroup::Type::ALTOCUMULUS:
		return("altocumulus");

		case metaf::CloudTypesGroup::Type::ALTOCUMULUS_CASTELLANUS:
		return("altocumulusCastellanus");
		
		case metaf::CloudTypesGroup::Type::CIRRUS:
		return("cirrus");
		
		case metaf::CloudTypesGroup::Type::CIRROSTRATUS:
		return("cirrostratus");
		
		case metaf::CloudTypesGroup::Type::CIRROCUMULUS:
		return("cirrocumulus");
		
		default: 
		return(undefinedToString(static_cast<int>(type)));
	}
}

std::string GroupVisitorJson::cloudLowLayerToString(metaf::CloudLayersGroup::LowLayer lowLayer) {
	switch(lowLayer) {
		case metaf::CloudLayersGroup::LowLayer::NONE:
		return("none");

		case metaf::CloudLayersGroup::LowLayer::CU_HU_CU_FR:
		return("CuHu_CuFr");

		case metaf::CloudLayersGroup::LowLayer::CU_MED_CU_CON:
		return("CuMed_CuCon");

		case metaf::CloudLayersGroup::LowLayer::CB_CAL:
		return("CbCal");

		case metaf::CloudLayersGroup::LowLayer::SC_CUGEN:
		return("ScCugen");

		case metaf::CloudLayersGroup::LowLayer::SC_NON_CUGEN:
		return("ScNotCugen");

		case metaf::CloudLayersGroup::LowLayer::ST_NEB_ST_FR:
		return("StNeb_StFr");

		case metaf::CloudLayersGroup::LowLayer::ST_FR_CU_FR_PANNUS:
		return("StFr_CuFr");

		case metaf::CloudLayersGroup::LowLayer::CU_SC_NON_CUGEN_DIFFERENT_LEVELS:
		return("Cu_ScNotCugen_onDifferentLevels");

		case metaf::CloudLayersGroup::LowLayer::CB_CAP:
		return("CbCap");

		case metaf::CloudLayersGroup::LowLayer::NOT_OBSERVABLE:
		return("notObservable");

		default:
		return(undefinedToString(static_cast<int>(lowLayer)));		
	}
}

std::string GroupVisitorJson::cloudMidLayerToString(metaf::CloudLayersGroup::MidLayer midLayer) {
	switch(midLayer) {
		case metaf::CloudLayersGroup::MidLayer::NONE:
		return("none");

		case metaf::CloudLayersGroup::MidLayer::AS_TR:
		return("AsTr");
		
		case metaf::CloudLayersGroup::MidLayer::AS_OP_NS:
		return("AsOp_Ns");
		
		case metaf::CloudLayersGroup::MidLayer::AC_TR:
		return("AcTr");
		
		case metaf::CloudLayersGroup::MidLayer::AC_TR_LEN_PATCHES:
		return("AcTr_patches");
		
		case metaf::CloudLayersGroup::MidLayer::AC_TR_AC_OP_SPREADING:
		return("AcTr_AcOp_spreading");
		
		case metaf::CloudLayersGroup::MidLayer::AC_CUGEN_AC_CBGEN:
		return("AcCugen_AcCbGen");
		
		case metaf::CloudLayersGroup::MidLayer::AC_DU_AC_OP_AC_WITH_AS_OR_NS:
		return("AcDu_AcOp_As_Ns");
		
		case metaf::CloudLayersGroup::MidLayer::AC_CAS_AC_FLO:
		return("AcCas_AcFlo");
		
		case metaf::CloudLayersGroup::MidLayer::AC_OF_CHAOTIC_SKY:
		return("Ac_chaoticSky");

		case metaf::CloudLayersGroup::MidLayer::NOT_OBSERVABLE:
		return("notObservable");

		default:
		return(undefinedToString(static_cast<int>(midLayer)));
	}
}

std::string GroupVisitorJson::cloudHighLayerToString(metaf::CloudLayersGroup::HighLayer highLayer) {
	switch(highLayer) {
		case metaf::CloudLayersGroup::HighLayer::NONE:
		return("none");

		case metaf::CloudLayersGroup::HighLayer::CI_FIB_CI_UNC:
		return("CiFib_CiUnc");

		case metaf::CloudLayersGroup::HighLayer::CI_SPI_CI_CAS_CI_FLO:
		return("CiSpi_CiCas_CiFlo");

		case metaf::CloudLayersGroup::HighLayer::CI_SPI_CBGEN:
		return("CiSpiCbgen");

		case metaf::CloudLayersGroup::HighLayer::CI_FIB_CI_UNC_SPREADING:
		return("CiFib_CiUnc_spreading");

		case metaf::CloudLayersGroup::HighLayer::CI_CS_LOW_ABOVE_HORIZON:
		return("Ci_Cs_lowAboveHorizon");

		case metaf::CloudLayersGroup::HighLayer::CI_CS_HIGH_ABOVE_HORIZON:
		return("Ci_Cs_highAboveHorizon");

		case metaf::CloudLayersGroup::HighLayer::CS_NEB_CS_FIB_COVERING_ENTIRE_SKY:
		return("CsNeb_CsFib_coveringEntireSky");

		case metaf::CloudLayersGroup::HighLayer::CS:
		return("Cs");

		case metaf::CloudLayersGroup::HighLayer::CC:
		return("Cc");

		case metaf::CloudLayersGroup::HighLayer::NOT_OBSERVABLE:
		return("notObservable");

		default:
		return(undefinedToString(static_cast<int>(highLayer)));
	}
}


std::string result;

extern "C" const char * EMSCRIPTEN_KEEPALIVE toJson(const char * input) {
	const auto parseResult = metaf::Parser::parse(std::string(input));

	std::ostringstream output;
	GroupVisitorJson visitor(output);
	visitor.toJson(parseResult.reportType, parseResult.error, parseResult.groups);

	result = output.str(); //involves copying operation but will do for this example
	return(result.c_str());
}

extern "C" void EMSCRIPTEN_KEEPALIVE freeMemory(){
	std::string().swap(result);
}

int main(int argc, char ** argv) {
	(void)argc; (void)argv;
	//Using EM_ASM_ because EM_ASM(convertToJson()); gives a warning
	EM_ASM_(convertToJson(), 0); 
}
