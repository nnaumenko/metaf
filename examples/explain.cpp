/*
* Copyright (C) 2018-2021 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "metaf.hpp"
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <cmath>

#ifdef __EMSCRIPTEN__

#include <emscripten/emscripten.h>

#else

#include <iostream>

#endif

using namespace std::literals;

class VisitorExplain : public metaf::Visitor<std::string> {
public:
	static std::string_view reportTypeToString(metaf::ReportType reportType);
	static std::string_view reportErrorToString(metaf::ReportError reportError);

private:
	static const inline std::string groupNotValidMessage = 
		"Data in this group may be errorneous, incomplete or inconsistent"s;

	virtual std::string visitKeywordGroup(
		const metaf::KeywordGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitLocationGroup(
		const metaf::LocationGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitReportTimeGroup(
		const metaf::ReportTimeGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitTrendGroup(
		const metaf::TrendGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitWindGroup(
		const metaf::WindGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitVisibilityGroup(
		const metaf::VisibilityGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitCloudGroup(
		const metaf::CloudGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitWeatherGroup(
		const metaf::WeatherGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitTemperatureGroup(
		const metaf::TemperatureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitPressureGroup(
		const metaf::PressureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitRunwayStateGroup(
		const metaf::RunwayStateGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitSeaSurfaceGroup(
		const metaf::SeaSurfaceGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitMinMaxTemperatureGroup(
		const metaf::MinMaxTemperatureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitPrecipitationGroup(
		const metaf::PrecipitationGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitLayerForecastGroup(
		const metaf::LayerForecastGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitPressureTendencyGroup(
		const metaf::PressureTendencyGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitCloudTypesGroup(
		const metaf::CloudTypesGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitLowMidHighCloudGroup(
		const metaf::LowMidHighCloudGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitLightningGroup(
		const metaf::LightningGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitVicinityGroup(
		const metaf::VicinityGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitMiscGroup(
		const metaf::MiscGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitUnknownGroup(
		const metaf::UnknownGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);

	static std::string explainRunway(
		const metaf::Runway & runway);
	static std::string explainMetafTime(
		const metaf::MetafTime & metafTime);
	static std::string explainTemperature(
		const metaf::Temperature & temperature);
	static std::string explainSpeed(
		const metaf::Speed & speed);
	static std::string explainDistance(
		const metaf::Distance & distance);
	static std::string explainDirection(
		const metaf::Direction & direction,
		bool trueCardinalDirections = false);
	static std::string explainPressure(
		const metaf::Pressure & pressure);
	static std::string explainPrecipitation(
		const metaf::Precipitation & precipitation);
	static std::string explainSurfaceFriction(
		const metaf::SurfaceFriction & surfaceFriction);
	static std::string explainWaveHeight(
		const metaf::WaveHeight & waveHeight);
	static std::string explainWeatherPhenomena(
		const metaf::WeatherPhenomena & weatherPhenomena);
	static std::string explainDirectionSector(
		const std::vector<metaf::Direction> dir);
	static std::string explainCloudType(const metaf::CloudType ct);

	static std::string_view speedUnitToString(
		metaf::Speed::Unit unit);
	static std::string_view distanceUnitToString(
		metaf::Distance::Unit unit);
	static std::string_view distanceMilesFractionToString(
		metaf::Distance::MilesFraction f);
	static std::string_view cardinalDirectionToString(
		metaf::Direction::Cardinal cardinal);
	static std::string_view brakingActionToString(
		metaf::SurfaceFriction::BrakingAction brakingAction);
	static std::string_view stateOfSeaSurfaceToString(
		metaf::WaveHeight::StateOfSurface stateOfSurface);
	static std::string_view cloudAmountToString(
		metaf::CloudGroup::Amount amount);
	static std::string_view convectiveTypeToString(
		metaf::CloudGroup::ConvectiveType type);
	static std::string_view weatherPhenomenaQualifierToString(
		metaf::WeatherPhenomena::Qualifier qualifier);
	static std::string_view weatherPhenomenaDescriptorToString(
		metaf::WeatherPhenomena::Descriptor descriptor);
	static std::string_view weatherPhenomenaWeatherToString(
		metaf::WeatherPhenomena::Weather weather);
	static std::string_view specialWeatherPhenomenaToString(
		const metaf::WeatherPhenomena & wp);

	static std::string_view probabilityToString(
		metaf::TrendGroup::Probability prob);
	static std::string_view visTrendToString(
		metaf::VisibilityGroup::Trend trend);
	static std::string_view runwayStateDepositsToString(
		metaf::RunwayStateGroup::Deposits deposits);
	static std::string_view runwayStateExtentToString(
		metaf::RunwayStateGroup::Extent extent);
	static std::string_view precipitationGroupTypeToString(
		metaf::PrecipitationGroup::Type type);
	static std::string_view layerForecastGroupTypeToString(
		metaf::LayerForecastGroup::Type type);
	static std::string_view pressureTendencyTypeToString(
		metaf::PressureTendencyGroup::Type type);
	static std::string_view pressureTendencyTrendToString(
		metaf::PressureTendencyGroup::Trend trend);
	static std::string_view cloudTypeToString(
		metaf::CloudType::Type type);
	static std::string_view cloudLowLayerToString(
		metaf::LowMidHighCloudGroup::LowLayer lowLayer);
	static std::string_view cloudMidLayerToString(
		metaf::LowMidHighCloudGroup::MidLayer midLayer);
	static std::string_view cloudHighLayerToString(
		metaf::LowMidHighCloudGroup::HighLayer highLayer);

	static std::string roundTo(float number, size_t digitsAfterDecimalPoint);
};

std::string VisitorExplain::visitKeywordGroup(
	const metaf::KeywordGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";

	switch (group.type()) {
		case metaf::KeywordGroup::Type::METAR:
		result << "Report type: METAR (weather observation report)";
		break;

		case metaf::KeywordGroup::Type::SPECI:
		result << "Report type: unscheduled METAR ";
		result << "(weather observation report)";
		result << "\nUnscheduled report is issued due to sudden changes in ";
		result << "weather conditions: wind shift, visibility decrease, ";
		result << "severe weather, clouds formed or dissipated, etc.";
		break;

		case metaf::KeywordGroup::Type::TAF:
		result << "Report type: TAF (terminal aerodrome forecast)";
		break;

		case metaf::KeywordGroup::Type::AMD:
		result << "Amended report";
		break;

		case metaf::KeywordGroup::Type::NIL:
		result << "Missing report";
		break;

		case metaf::KeywordGroup::Type::CNL:
		result << "Cancelled report";
		break;

		case metaf::KeywordGroup::Type::COR:
		result << "Correctional report";
		break;

		case metaf::KeywordGroup::Type::AUTO:
		result << "Fully automated report with no human intervention ";
		result << "or oversight";
		break;

		case metaf::KeywordGroup::Type::CAVOK:
		result << "Ceiling and visibility OK";
		result << "\nVisibility 10 km or more in all directions, ";
		result << "no cloud below 5000 feet (1500 meters), ";
		result << "no cumulonimbus or towering cumulus clouds, ";
		result << "no significant weather phenomena";
		break;

		case metaf::KeywordGroup::Type::RMK:
		result << "The remarks are as follows";
		break;

		case metaf::KeywordGroup::Type::MAINTENANCE_INDICATOR:
		result << "Automated station requires maintenance";
		break;

		case metaf::KeywordGroup::Type::AO1:
		result << "This automated station is not equipped with a ";
		result << "precipitation discriminator";
		break;

		case metaf::KeywordGroup::Type::AO2:
		result << "This automated station is equipped with a ";
		result << "precipitation discriminator";
		break;

		case metaf::KeywordGroup::Type::AO1A:
		result << "This automated station is not equipped with a ";
		result << "precipitation discriminator";
		result << "\nAutomated observation is augmented by a human observer";
		break;

		case metaf::KeywordGroup::Type::AO2A:
		result << "This automated station is equipped with a ";
		result << "precipitation discriminator";
		result << "\nAutomated observation is augmented by a human observer";
		break;

		case metaf::KeywordGroup::Type::NOSPECI:
		result << "This manual station does not issue SPECI (unscheduled) ";
		result << "reports";
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitLocationGroup(
	const metaf::LocationGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	result << "ICAO code of station: " << group.toString();
	return result.str();
}

std::string VisitorExplain::visitReportTimeGroup(
	const metaf::ReportTimeGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	result << "Day and time of report release: ";
	result << explainMetafTime(group.time());
	return result.str();
}

std::string VisitorExplain::visitTrendGroup(
	const metaf::TrendGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	switch (group.type()) {
		case metaf::TrendGroup::Type::NOSIG:
		result << "No significant weather changes expected";
		break;

		case metaf::TrendGroup::Type::BECMG:
		result << "Weather conditions are expected to gradually change ";
		result << "as follows";
		if (const auto timeFrom = group.timeFrom(); timeFrom) {
			result << "\nFrom ";
			result << explainMetafTime(*timeFrom);
		}
		if (const auto timeUntil = group.timeUntil(); timeUntil) {
			result << "\nUntil ";
			result << explainMetafTime(*timeUntil);
		}
		if (const auto timeAt = group.timeAt(); timeAt) {
			result << "\nAt ";
			result << explainMetafTime(*timeAt);
		}
		break;

		case metaf::TrendGroup::Type::TEMPO:
		result << "The following temporary weather conditions may arise ";
		result << " for less than 60 minutes";
		if (const auto timeFrom = group.timeFrom(); timeFrom) {
			result << "\nFrom ";
			result << explainMetafTime(*timeFrom);
		}
		if (const auto timeUntil = group.timeUntil(); timeUntil) {
			result << "\nUntil ";
			result << explainMetafTime(*timeUntil);
		}
		if (const auto timeAt = group.timeAt(); timeAt) {
			result << "\nAt ";
			result << explainMetafTime(*timeAt);
		}
		if (group.probability() != metaf::TrendGroup::Probability::NONE) {
			result << "\n";
			result << probabilityToString(group.probability());
		}
		break;

		case metaf::TrendGroup::Type::INTER:
		result << "The following temporary weather conditions may arise ";
		result << "for less than 30 minutes";
		if (const auto timeFrom = group.timeFrom(); timeFrom) {
			result << "\nFrom ";
			result << explainMetafTime(*timeFrom);
		}
		if (const auto timeUntil = group.timeUntil(); timeUntil) {
			result << "\nUntil ";
			result << explainMetafTime(*timeUntil);
		}
		if (const auto timeAt = group.timeAt(); timeAt) {
			result << "\nAt ";
			result << explainMetafTime(*timeAt);
		}
		if (group.probability() != metaf::TrendGroup::Probability::NONE) {
			result << "\n";
			result << probabilityToString(group.probability());
		}
		break;

		case metaf::TrendGroup::Type::FROM:
		result << "Weather conditions are expected to change rapidly from ";
		result << explainMetafTime(*group.timeFrom());
		result << "\nAll previous weather conditions are superseded by ";
		result << "the following conditions";
		break;

		case metaf::TrendGroup::Type::UNTIL:
		result << "The following weather conditions expected to last until ";
		result << explainMetafTime(*group.timeUntil());
		break;

		case metaf::TrendGroup::Type::AT:
		result << "The following weather conditions expected to occur at ";
		result << explainMetafTime(*group.timeAt());
		break;

		case metaf::TrendGroup::Type::TIME_SPAN:
		result << "The following weather condition are expected within ";
		result << "time span from ";
		result << explainMetafTime(*group.timeFrom());
		result << " until ";
		result << explainMetafTime(*group.timeUntil());
		if (group.probability() != metaf::TrendGroup::Probability::NONE) {
			result << "\n";
			result << probabilityToString(group.probability());
		}
		break;

		case metaf::TrendGroup::Type::PROB:
		result << "The weather conditions probability is as follows\n";
		result << probabilityToString(group.probability());
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitWindGroup(
	const metaf::WindGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	
	switch (group.type()) {
		case metaf::WindGroup::Type::SURFACE_WIND_CALM:
		result << "No wind";
		break;

		case metaf::WindGroup::Type::SURFACE_WIND:
		result << "Surface wind";
		result << "\nWind direction: ";
		result << explainDirection(group.direction(), true);
		result << "\nWind speed: ";
		result << explainSpeed(group.windSpeed());
		if (group.gustSpeed().isReported()) {
			result << "\nGust speed: ";
			result << explainSpeed(group.gustSpeed());
		}
		break;

		case metaf::WindGroup::Type::VARIABLE_WIND_SECTOR:
		result << "Variable wind sector";
		result << "\nVariable wind direction sector from ";
		result << explainDirection(group.varSectorBegin()); 
		result << " clockwise to ";
		result << explainDirection(group.varSectorEnd());
		break;

		case metaf::WindGroup::Type::SURFACE_WIND_WITH_VARIABLE_SECTOR:
		result << "Surface wind with variable with sector";
		result << "\nWind direction: ";
		result << explainDirection(group.direction(), true);
		result << "\nWind speed: ";
		result << explainSpeed(group.windSpeed());
		if (group.gustSpeed().isReported()) {
			result << "\nGust speed: ";
			result << explainSpeed(group.gustSpeed());
		}
		result << "\nVariable wind direction sector from ";
		result << explainDirection(group.varSectorBegin()); 
		result << " clockwise to ";
		result << explainDirection(group.varSectorEnd());
		break;

		case metaf::WindGroup::Type::WIND_SHEAR:
		result << "Wind shear at height ";
		result << explainDistance(group.height());
		result << ":\n";
		result << "Wind direction: ";
		result << explainDirection(group.direction(), true);
		result << "Wind speed: ";
		result << explainSpeed(group.windSpeed());
		if (group.gustSpeed().isReported()) {
			result << "\nGust speed: ";
			result << explainSpeed(group.gustSpeed());
		}
		break;

		case metaf::WindGroup::Type::WIND_SHIFT:		
		result << "Wind direction changed 45&deg; or more in less than ";
		result << "15 minutes with sustained wind speed of 10 knots";
		result << " (5.1 m/s / 18.5 km/h / 11.5 mph)";
		if (group.eventTime()) {
			result << "\nWind shift began at ";
			result << explainMetafTime(*group.eventTime());
		}
		break;

		case metaf::WindGroup::Type::WIND_SHIFT_FROPA:
		result << "Wind direction changed 45&deg; or more in less than ";
		result << "15 minutes with sustained wind speed of 10 knots";
		result << " (5.1 m/s / 18.5 km/h / 11.5 mph)";
		if (group.eventTime()) {
			result << "\nWind shift began at ";
			result << explainMetafTime(*group.eventTime());
		}
		result << "\nThis wind shift is associated with weather front passage";
		break;

		case metaf::WindGroup::Type::PEAK_WIND:
		result << "Peak wind was observed at ";
		result << explainMetafTime(group.eventTime().value());
		result << "\nWind direction: ";
		result << explainDirection(group.direction(), true);
		result << "\nWind speed: ";
		result << explainSpeed(group.windSpeed());
		break;

		case metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS:
		result << "Wind shear significant to aircraft operations ";
		result << "is present along the take-off path or approach path ";
		result << "between runway level and 500 metres (1 600 ft)";
		if (const auto rw = group.runway(); rw.has_value())
			result << " at " << explainRunway(*rw);
		break;

		case metaf::WindGroup::Type::WSCONDS:
		result << "Potential wind shear conditions are present ";
		result << "but there's not enough information to reliably forecast ";
		result << "height, direction and speed of wind shear";
		break;

		case metaf::WindGroup::Type::WND_MISG:
		result << "Wind data are missing";
		break;

		case metaf::WindGroup::Type::WIND_DATA_ESTIMATED:
		result << "Wind data are estimated";
		break;

		case metaf::WindGroup::Type::WIND_AT_HEIGHT:
		result << "Wind at height ";
		result << explainDistance(group.height());
		result << ":\n";
		result << "Wind direction: ";
		result << explainDirection(group.direction(), true);
		result << "Wind speed: ";
		result << explainSpeed(group.windSpeed());
		if (group.gustSpeed().isReported()) {
			result << "\nGust speed: ";
			result << explainSpeed(group.gustSpeed());
		}
		break;

		case metaf::WindGroup::Type::RUNWAY_WIND:
		result << "Wind at " << explainRunway(*group.runway());
		result << "\nWind direction: ";
		result << explainDirection(group.direction(), true);
		result << "\nWind speed: ";
		result << explainSpeed(group.windSpeed());
		if (group.gustSpeed().isReported()) {
			result << "\nGust speed: ";
			result << explainSpeed(group.gustSpeed());
		}
		break;

		case metaf::WindGroup::Type::RUNWAY_WIND_WITH_VARIABLE_SECTOR:
		result << "Wind at " << explainRunway(*group.runway());
		result << "\nWind direction: ";
		result << explainDirection(group.direction(), true);
		result << "\nWind speed: ";
		result << explainSpeed(group.windSpeed());
		if (group.gustSpeed().isReported()) {
			result << "\nGust speed: ";
			result << explainSpeed(group.gustSpeed());
		}
		result << "\nVariable wind direction sector from ";
		result << explainDirection(group.varSectorBegin()); 
		result << " clockwise to ";
		result << explainDirection(group.varSectorEnd());
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitVisibilityGroup(
	const metaf::VisibilityGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	switch (group.type()) {
		case metaf::VisibilityGroup::Type::PREVAILING:
		result << "Prevailing visibility is ";
		result << explainDistance(group.visibility());
		break;

		case metaf::VisibilityGroup::Type::PREVAILING_NDV:
		result << "Prevailing visibility is "; 
		result << explainDistance(group.visibility());
		result << "\nThis station cannot differentiate the directional ";
		result << "variation of visibility";
		break;

		case metaf::VisibilityGroup::Type::DIRECTIONAL:
		result << "Directional visibility toward ";
		result << explainDirection(group.direction().value());
		result << " is " << explainDistance(group.visibility());
		break;
	
		case metaf::VisibilityGroup::Type::RUNWAY:
		result << "Visibility for ";
		result << explainRunway(group.runway().value());
		result << " is " << explainDistance(group.visibility());
		break;

		case metaf::VisibilityGroup::Type::RVR:
		if (!group.runway().has_value()) {
			result << "Runway visual range not reported ";		
			break;
		}
		result << "Runway visual range for ";
		result << explainRunway(group.runway().value()) << " is ";
		result << explainDistance(group.visibility());
		if (group.trend() != metaf::VisibilityGroup::Trend::NONE) {
			result << ", and the trend is " << visTrendToString(group.trend());
		}
		break;

		case metaf::VisibilityGroup::Type::SURFACE:
		result << "Visibility at surface level is ";
		result << explainDistance(group.visibility());
		break;

		case metaf::VisibilityGroup::Type::TOWER:
		result << "Visibility from air traffic control tower is ";
		result << explainDistance(group.visibility());
		break;

		case metaf::VisibilityGroup::Type::SECTOR:
		result << "Sector visibility is ";
		result << explainDistance(group.visibility());
		result << "\nIn the following directions: ";
		result << explainDirectionSector(group.sectorDirections());
		break;

		case metaf::VisibilityGroup::Type::VARIABLE_PREVAILING:
		result << "Prevailing visibility is variable from ";
		result << explainDistance(group.minVisibility());
		result << " to ";
		result << explainDistance(group.maxVisibility());
		break;

		case metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL:
		result << "Directional visibility toward ";
		result << explainDirection(group.direction().value());
		result << " is variable from ";
		result << explainDistance(group.minVisibility());
		result << " to ";
		result << explainDistance(group.maxVisibility());
		break;

		case metaf::VisibilityGroup::Type::VARIABLE_RUNWAY:
		result << "Visibility for ";
		result << explainRunway(group.runway().value());
		result << " is variable from ";
		result << explainDistance(group.minVisibility());
		result << " to ";
		result << explainDistance(group.maxVisibility());
		break;

		case metaf::VisibilityGroup::Type::VARIABLE_RVR:
		result << "Runway visual range for ";
		result << explainRunway(group.runway().value());
		result << " is variable from ";
		result << explainDistance(group.minVisibility());
		result << " to ";
		result << explainDistance(group.maxVisibility());
		if (group.trend() != metaf::VisibilityGroup::Trend::NONE) {
			result << ", and the trend is " << visTrendToString(group.trend());
		}
		break;

		case metaf::VisibilityGroup::Type::VARIABLE_SECTOR:
		result << "Sector visibility is variable from ";
		result << explainDistance(group.minVisibility());
		result << " to ";
		result << explainDistance(group.maxVisibility());
		result << "\nIn the following directions: ";
		result << explainDirectionSector(group.sectorDirections());
		break;

		case metaf::VisibilityGroup::Type::VIS_MISG:
		result << "Visibility data missing";
		break;

		case metaf::VisibilityGroup::Type::RVR_MISG:
		result << "Runway visual range data is missing";
		break;

		case metaf::VisibilityGroup::Type::RVRNO:
		result << "Runway visual range should be reported but is missing";
		break;

		case metaf::VisibilityGroup::Type::VISNO:
		result << "Visibility data not awailable";
		if (const auto r = group.runway(); r.has_value()) {
			result << " for " << explainRunway(*r);
		}
		if (const auto d = group.direction(); d.has_value()) {
			result << " in the direction of " << explainDirection(*d);
		}
		break;

		case metaf::VisibilityGroup::Type::MINIMUM:
		result << "Minimum visibility ";
		if (group.direction().has_value() && group.direction()->isValue()) {
			result << "(towards " << explainDirection(group.direction().value()) << ")";
		}
		result << " is " << explainDistance(group.visibility());
		break;

		case metaf::VisibilityGroup::Type::MAXIMUM:
		result << "Maximum visibility ";
		if (group.direction().has_value() && group.direction()->isValue()) {
			result << "(towards " << explainDirection(group.direction().value()) << ")";
		}
		result << " is " << explainDistance(group.visibility());
		break;

		case metaf::VisibilityGroup::Type::TOWARDS_SEA:
		result << "Visibility towards the sea is ";
		result << explainDistance(group.visibility());
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitCloudGroup(
	const metaf::CloudGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";

	switch (group.type()) {
		case metaf::CloudGroup::Type::NO_CLOUDS:
		result << cloudAmountToString(group.amount());
		break;

		case metaf::CloudGroup::Type::CLOUD_LAYER:
		result << "Cloud layer\n";
		result << cloudAmountToString(group.amount());
		result << "\nBase height ";
		result << explainDistance(group.height());
		if (group.convectiveType() != metaf::CloudGroup::ConvectiveType::NONE) {
			result << "\nConvective type: ";
			result << convectiveTypeToString(group.convectiveType());
		}
		break;

		case metaf::CloudGroup::Type::VERTICAL_VISIBILITY:
		result << "Sky is obscured, vertical visibility is ";
		result << explainDistance(group.verticalVisibility());
		break;

		case metaf::CloudGroup::Type::CEILING:
		result << "Ceiling height " << explainDistance(group.height());
		if (const auto rw = group.runway(); rw.has_value()) {
			result << " at " << explainRunway(*rw);
		}
		if (const auto d = group.direction(); d.has_value()) {
			result << " towards " << explainDirection(*d);
		}
		break;

		case metaf::CloudGroup::Type::VARIABLE_CEILING:
		result << "Ceiling height is variable between ";
		result << explainDistance(group.minHeight());
		result << " and ";
		result << explainDistance(group.maxHeight());
		if (const auto rw = group.runway(); rw.has_value()) {
			result << " at " << explainRunway(*rw);
		}
		if (const auto d = group.direction(); d.has_value()) {
			result << " towards " << explainDirection(*d);
		}
		break;

		case metaf::CloudGroup::Type::CHINO:
		result << "Ceiling data not awailable";
		if (const auto rw = group.runway(); rw.has_value()) {
			result << " at " << explainRunway(*rw);
		}
		if (const auto d = group.direction(); d.has_value()) {
			result << " towards " << explainDirection(*d);
		}
		break;

		case metaf::CloudGroup::Type::CLD_MISG:
		result << "Sky condition data (cloud data) is missing";
		break;

		case metaf::CloudGroup::Type::OBSCURATION:
		if (const auto h = group.height().distance(); h.has_value()) {
			if (!h.value()) {
				result << "Ground-based obscuration\n";
			} else {
				result << "Aloft obscuration\n";
			}
		}
		if (const auto ct = group.cloudType(); ct.has_value()) {
			result << explainCloudType(ct.value());
		}
		break;

		case metaf::CloudGroup::Type::VARIABLE_COVER:
		result << "Cloud cover is variable between ";
		result << cloudAmountToString(group.amount());
		result << " and ";
		result << cloudAmountToString(group.variableAmount());
		if (group.height().isReported()) {
			result << "\nCloud base height ";
			result << explainDistance(group.height());
		}
		break;

		case metaf::CloudGroup::Type::CIG_RAG:
		result << "Ceiling is ragged";
		break;

		case metaf::CloudGroup::Type::CIG_DFUS:
		result << "Ceiling is diffuse";
		break;

		case metaf::CloudGroup::Type::TOTAL_COVER:
		result << "Total cloud cover: ";
		result << cloudAmountToString(group.amount());
		break;

	}
	return result.str();
}

std::string VisitorExplain::visitWeatherGroup(
	const metaf::WeatherGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";

	switch (group.type()) {
		case metaf::WeatherGroup::Type::CURRENT: 
		result << "Weather phenomena:";
		for (const auto p : group.weatherPhenomena()) {
			result << "\n";
			result << explainWeatherPhenomena(p);
		}
		break;
		
		case metaf::WeatherGroup::Type::RECENT: 
		result << "Recent weather:";
		for (const auto p : group.weatherPhenomena()) {
			result << "\n";
			result << explainWeatherPhenomena(p);
		}
		break;
		
		case metaf::WeatherGroup::Type::EVENT: 
		result << "Precipitation beginning/ending time:";
		for (const auto p : group.weatherPhenomena()) {
			result << "\n";
			result << explainWeatherPhenomena(p);
		}
		break;

		case metaf::WeatherGroup::Type::NSW:
		result << "Nil significant weather";
		result << "\nIndicates end of significant weather phenomena";
		break;

		case metaf::WeatherGroup::Type::PWINO:
		result << "This automated station is equipped with ";
		result << "present weather identifier ";
		result << "and this sensor is not operating";
		break;

		case metaf::WeatherGroup::Type::TSNO:
		result << "This automated station is equipped with lightning detector ";
		result << "and this sensor is not operating";
		break;

		case metaf::WeatherGroup::Type::WX_MISG:
		result << "Weather phenomena data is missing";
		break;

		case metaf::WeatherGroup::Type::TS_LTNG_TEMPO_UNAVBL:
		result << "Thunderstorm / lightning data is missing";
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitTemperatureGroup(
	const metaf::TemperatureGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";

	switch (group.type()) {
		case metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT:
		result << "Ambient air temperature: ";
		result << explainTemperature(group.airTemperature());
		result << "\nDew point: ";
		result << explainTemperature(group.dewPoint());
		if (const auto rh = group.relativeHumidity(); rh.has_value()) {
				result << "\nRelative humidity: ";
				result << static_cast<int>(*rh);
				result << " percent"; 
		}
		break;

		case metaf::TemperatureGroup::Type::T_MISG:
		result << "Temperature data is missing";
		break;

		case metaf::TemperatureGroup::Type::TD_MISG:
		result << "Dew point data is missing";
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitPressureGroup(
	const metaf::PressureGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";

	switch(group.type()) {
		case metaf::PressureGroup::Type::OBSERVED_QNH:
		result << "Observed mean atmospheric pressure ";
		result << "(normalised to sea level) / altimeter setting: ";
		result << explainPressure(group.atmosphericPressure());
		break;

		case metaf::PressureGroup::Type::FORECAST_LOWEST_QNH:
		result << "Forecast lowest sea level pressure: ";
		result << explainPressure(group.atmosphericPressure());
		break;

		case metaf::PressureGroup::Type::OBSERVED_QFE:
		result << "Observed actual atmospheric pressure: ";
		result << explainPressure(group.atmosphericPressure());
		break;

		case metaf::PressureGroup::Type::OBSERVED_SLP:
		result << "Observed atmospheric pressure normalised to sea level:";
		result << explainPressure(group.atmosphericPressure());
		result << "\nThis is a value that might be more accurate for meteorological ";
		result << "purposes and attempts to correct for non-standard temperatures.";
		break;

		case metaf::PressureGroup::Type::SLPNO:
		result << "Mean sea-level pressure information is not available";
		break;

		case metaf::PressureGroup::Type::PRES_MISG:
		result << "Atmospheric pressure (altimeter) data is missing";
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitRunwayStateGroup(
	const metaf::RunwayStateGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";

	result << "State of " << explainRunway(group.runway()) << ":\n";

	switch (group.type()) {
		case metaf::RunwayStateGroup::Type::RUNWAY_STATE:
		result << runwayStateDepositsToString(group.deposits());
		if (group.deposits() != 
				metaf::RunwayStateGroup::Deposits::CLEAR_AND_DRY)
		{
		    result << "\nDepth of deposits on runway: ";
		    result << explainPrecipitation(group.depositDepth());
			result << "\nRunway contamination extent: ";
			result << runwayStateExtentToString(group.contaminationExtent());
		}
		result << "\nSurface friction: ";
		result << explainSurfaceFriction(group.surfaceFriction());
		break;

		case metaf::RunwayStateGroup::Type::RUNWAY_CLRD:
		result << "Deposits on runway were cleared or ceased to exist";
		result << "\nSurface friction: ";
		result << explainSurfaceFriction(group.surfaceFriction());
		break;

		case metaf::RunwayStateGroup::Type::RUNWAY_SNOCLO:
		result << "Runway closed due to snow accumulation";
		break;

		case metaf::RunwayStateGroup::Type::AERODROME_SNOCLO:
		result << "Aerodrome closed due to snow accumulation";
		break;

		case metaf::RunwayStateGroup::Type::RUNWAY_NOT_OPERATIONAL:
		result << "Runway is not operational";
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";

	result << "Sea surface temperature ";
	result << explainTemperature(group.surfaceTemperature()) << ", ";
	result << explainWaveHeight(group.waves());
	return result.str();
}

std::string VisitorExplain::visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	switch(group.type()) {
		case metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY:
		result << "Observed 6-hourly minimum/maximum temperature: ";
		result << "\nMinimum ambient air temperature: ";
		result << explainTemperature(group.minimum());
		result << "\nMaximum ambient air temperature: ";
		result << explainTemperature(group.maximum());
		break;

		case metaf::MinMaxTemperatureGroup::Type::OBSERVED_24_HOURLY:
		result << "Observed 24-hourly minimum/maximum temperature: ";
		result << "\nMinimum ambient air temperature: ";
		result << explainTemperature(group.minimum());
		result << "\nMaximum ambient air temperature: ";
		result << explainTemperature(group.maximum());
		break;

		case metaf::MinMaxTemperatureGroup::Type::FORECAST:
		result << "Forecast minimum/maximum temperature";
		if (group.minimum().isReported()) {
			result << "\nMinimum ambient air temperature: ";
			result << explainTemperature(group.minimum());
			result << ", expected at ";
			result << explainMetafTime(group.minimumTime().value());
		}
		if (group.maximum().isReported()) {
			result << "\nMaximum ambient air temperature: ";
			result << explainTemperature(group.maximum());
			result << ", expected at ";
			result << explainMetafTime(group.maximumTime().value());
		}
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitPrecipitationGroup(
	const metaf::PrecipitationGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	switch(group.type()) {
		case metaf::PrecipitationGroup::Type::TOTAL_PRECIPITATION_HOURLY:
		result << "Total precipitation for the past hour: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::SNOW_DEPTH_ON_GROUND:
		result << "Snow depth on ground: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_OR_6_HOURLY:
		result << "Water equivalent of frozen precipitation ";
		result << "for the last 3 or 6 hours: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_HOURLY:
		result << "Water equivalent of frozen precipitation";
		result << " for the last 3 hours: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_6_HOURLY:
		result << "Water equivalent of frozen precipitation"; 
		result << "for the last 6 hours: ";
		result << explainPrecipitation(group.total());
		break;
	
		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_24_HOURLY:
		result << "Water equivalent of frozen precipitation ";
		result << "for the last 24 hours: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::SNOW_6_HOURLY:
		result << "Snowfall for the last 6 hours: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::WATER_EQUIV_OF_SNOW_ON_GROUND:
		result << "Water equivalent of snow on ground: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_HOUR:
		result << "Ice accretion for the last hour: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_3_HOURS:
		result << "Ice accretion for the last 3 hours: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_6_HOURS:
		result << "Ice accretion for the last 6 hours: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::
				PRECIPITATION_ACCUMULATION_SINCE_LAST_REPORT:
		result << "Precipitation accumulation since last report: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::SNOW_INCREASING_RAPIDLY:
		result << "Snow increasing rapidly";
		result << "\nFor the last hour snow increased by ";
		result << explainPrecipitation(group.recent());
		result << "\nTotal snowfall: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::RAINFALL_9AM_10MIN:
		result << "Rainfall for the last 10 minutes before ";
		result << "report release time: ";
		result << explainPrecipitation(group.recent());
		result << "\nRainfall since 9AM (9:00) local time: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::PNO:
		result << "This automated station is equipped with ";
		result << "tipping bucket rain gauge ";
		result << "and this sensor is not operating";
		break;

		case metaf::PrecipitationGroup::Type::FZRANO:
		result << "This automated station is equipped with ";
		result << "freezing rain sensor ";
		result << "and this sensor is not operating";
		break;

		case metaf::PrecipitationGroup::Type::ICG_MISG:
		result << "Icing data is missing";
		break;

		case metaf::PrecipitationGroup::Type::PCPN_MISG:
		result << "Precipitation data is missing";
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitLayerForecastGroup(
	const metaf::LayerForecastGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	result << layerForecastGroupTypeToString(group.type());
	result << " at ";
	if (!group.baseHeight().isReported() && !group.topHeight().isReported()) {
		result << "all heights";
	} else {
		result << "heights from ";
		result << explainDistance(group.baseHeight());
		result << " to ";
		result << explainDistance(group.topHeight());
	}
	return result.str();	
}

std::string VisitorExplain::visitPressureTendencyGroup(
	const metaf::PressureTendencyGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;

	if (!group.isValid()) result << groupNotValidMessage << "\n";

	switch (group.type()) {
		case metaf::PressureTendencyGroup::Type::NOT_REPORTED:
		result << "3-hourly pressure tendency is not reported";
		result << "\nAbsolute pressure change is ";
		result << explainPressure(group.difference());
		break;

		case metaf::PressureTendencyGroup::Type::RISING_RAPIDLY:
		case metaf::PressureTendencyGroup::Type::FALLING_RAPIDLY:
		result << "Atmospheric pressure is ";
		result << pressureTendencyTypeToString (group.type());
		break;

		default:
		result << "During last 3 hours the atmospheric pressure was ";
		result << pressureTendencyTypeToString (group.type());
		result << "\nNow the atmospheric pressure is ";
		result << pressureTendencyTrendToString(
			metaf::PressureTendencyGroup::trend(group.type()));
		result << " 3 hours ago";
		result << "\nAbsolute pressure change is ";
		result << explainPressure(group.difference());
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitCloudTypesGroup(
	const metaf::CloudTypesGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	const auto clouds = group.cloudTypes();
	result << "Obscuration / cloud layers: ";
	for (auto i = 0u; i < clouds.size(); i++) {
		result << "\n";
		result << explainCloudType(clouds.at(i));
	}
	return result.str();
}

std::string VisitorExplain::visitLowMidHighCloudGroup(
	const metaf::LowMidHighCloudGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	result << "Low cloud layer:\n";
	result << cloudLowLayerToString(group.lowLayer());
	result << "\nMid cloud layer:\n";
	result << cloudMidLayerToString(group.midLayer());
	result << "\nHigh cloud layer:\n";
	result << cloudHighLayerToString(group.highLayer());
	return result.str();
}

std::string VisitorExplain::visitLightningGroup(
	const metaf::LightningGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";

	result << "Lightning strikes observed";
	if (group.distance().isReported())
	{
		result << " at distance ";
		result << explainDistance(group.distance());
	}

	switch(group.frequency()) {
		case metaf::LightningGroup::Frequency::NONE:
		break;

		case metaf::LightningGroup::Frequency::OCCASIONAL:
		result << "\nLess than 1 strike per minute";
		break;

		case metaf::LightningGroup::Frequency::FREQUENT:
		result << "\n1 to 6 strikes per minute";
		break;

		case metaf::LightningGroup::Frequency::CONSTANT:
		result << "\nMore than 6 strikes per minute";
		break;
	}

	if (group.isCloudGround() || 
	 	group.isInCloud() ||
		group.isCloudCloud() ||
		group.isCloudAir()) 
	{
		result << "\nThe following lightning types are observed: ";
		if (group.isCloudGround()) result << "\ncloud-to-ground";
		if (group.isInCloud()) result << "\nin-cloud";
		if (group.isCloudCloud()) result << "\ncloud-to-cloud";
		if (group.isCloudAir()) 
			result << "\ncloud-to-air without strike to ground";
	}
	if (group.isUnknownType()) 
	{
		result << "\nSome lightning strike types specified in this group ";
		result << "were not recognised by parser"; 
	}
	if (const auto directions = group.directions(); directions.size()) {
		result << "\nLightning strikes observed in the following directions: ";
		result << explainDirectionSector(directions);
	}
	return result.str();
}

std::string VisitorExplain::visitVicinityGroup(
	const metaf::VicinityGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	switch (group.type()) {
		case metaf::VicinityGroup::Type::THUNDERSTORM:
		result << "Thunderstorm";
		break;

		case metaf::VicinityGroup::Type::CUMULONIMBUS:
		result << "Cumulonimbus cloud(s)";
		break;

		case metaf::VicinityGroup::Type::CUMULONIMBUS_MAMMATUS:
		result << "Cumulonimbus cloud(s) with mammatus";
		break;

		case metaf::VicinityGroup::Type::TOWERING_CUMULUS:
		result << "Towering cumulus cloud(s)";
		break;

		case metaf::VicinityGroup::Type::ALTOCUMULUS_CASTELLANUS:
		result << "Altocumulus cloud(s)";
		break;

		case metaf::VicinityGroup::Type::STRATOCUMULUS_STANDING_LENTICULAR:
		result << "Stratocumulus standing lenticular cloud(s)";
		break;

		case metaf::VicinityGroup::Type::ALTOCUMULUS_STANDING_LENTICULAR:
		result << "Altocumulus standing lenticular cloud(s)";
		break;

		case metaf::VicinityGroup::Type::CIRROCUMULUS_STANDING_LENTICULAR:
		result << "Cirrocumulus standing lenticular cloud(s)";
		break;

		case metaf::VicinityGroup::Type::ROTOR_CLOUD:
		result << "Rotor cloud(s)";
		break;

		case metaf::VicinityGroup::Type::VIRGA:
		result << "Virga";
		break;

		case metaf::VicinityGroup::Type::PRECIPITATION_IN_VICINITY:
		result << "Precipitation";
		break;

		case metaf::VicinityGroup::Type::FOG:
		result << "Fog";
		break;

		case metaf::VicinityGroup::Type::FOG_SHALLOW:
		result << "Shallow fog";
		break;

		case metaf::VicinityGroup::Type::FOG_PATCHES:
		result << "Patches of fog";
		break;

		case metaf::VicinityGroup::Type::HAZE:
		result << "Haze";
		break;

		case metaf::VicinityGroup::Type::SMOKE:
		result << "Smoke";
		break;

		case metaf::VicinityGroup::Type::BLOWING_SNOW:
		result << "Blowing snow";
		break;

		case metaf::VicinityGroup::Type::BLOWING_SAND:
		result << "Blowing sand";
		break;

		case metaf::VicinityGroup::Type::BLOWING_DUST:
		result << "Blowing dust";
		break;

		case metaf::VicinityGroup::Type::LOWER_CEILING:
		result << "Lower ceiling";
		break;
	}
	result << " observed";
	if (group.distance().isReported())
	{
		result << " at distance ";
		result << explainDistance(group.distance());
	}	
	if (const auto directions = group.directions(); directions.size()) {
		result << "\nObserved in the following directions: ";
		result << explainDirectionSector(directions);
	}
	if (group.movingDirection().isReported()) {
		result << "\nMoving towards ";
		result << cardinalDirectionToString(group.movingDirection().cardinal());
	}

	return result.str();

}

std::string VisitorExplain::visitMiscGroup(const metaf::MiscGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";

	static const std::string colourCodeBlack = "Colour code BLACK: "
		"aerodrome closed due to snow accumulation or non-weather reasons";

	switch (group.type()) {
		case metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES:
		if (const auto duration = group.data(); *duration) {
			result << "Duration of sunshine ";
			result << "that occurred the previous calendar day is ";
			result << *duration;
			result << " minutes";
		} else {
			result << "No sunshine occurred the previous calendar day";
		}
		break;

		case metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION:
		result << "This report is the corrected weather observation, ";
		result << "correction number is ";
		result << static_cast<int>(*group.data());
		break;

		case metaf::MiscGroup::Type::DENSITY_ALTITUDE:
		result << "Density altitude ";
		if (!group.data().has_value()) {
			result << "data missing";
		} else {
			result << " is ";
			result << *group.data() << " feet";
		}
		break;

		case metaf::MiscGroup::Type::HAILSTONE_SIZE:
		result << "Largest hailstone size is ";
		result << *group.data();
		result << " inches";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKBLUE_PLUS:
		result << colourCodeBlack << "\n";
		[[fallthrough]];
		case metaf::MiscGroup::Type::COLOUR_CODE_BLUE_PLUS:
		result << "Colour code BLUE+: ";
		result << "visibility &gt;8000 m and ";
		result << "lowest cloud base height &ge;20000 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKBLUE:
		result << colourCodeBlack << "\n";
		[[fallthrough]];
		case metaf::MiscGroup::Type::COLOUR_CODE_BLUE:
		result << "Colour code BLUE: ";
		result << "visibility &gt;8000 m and ";
		result << "lowest cloud base height &gt;2500 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKWHITE:
		result << colourCodeBlack << "\n";
		[[fallthrough]];
		case metaf::MiscGroup::Type::COLOUR_CODE_WHITE:
		result << "Colour code WHITE: ";
		result << "visibility &gt;5000 m and ";
		result << "lowest cloud base height &gt;1500 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKGREEN:
		result << colourCodeBlack << "\n";
		[[fallthrough]];
		case metaf::MiscGroup::Type::COLOUR_CODE_GREEN:
		result << "Colour code GREEN: ";
		result << "visibility &gt;3700 m and ";
		result << "lowest cloud base height &gt;700 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW:
		result << colourCodeBlack << "\n";
		[[fallthrough]];
		case metaf::MiscGroup::Type::COLOUR_CODE_YELLOW:
		result << "Colour code YELLOW: ";
		result << "visibility &gt;2500 m and ";
		result << "lowest cloud base height &gt;500 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW1:
		result << colourCodeBlack << "\n";
		[[fallthrough]];
		case metaf::MiscGroup::Type::COLOUR_CODE_YELLOW1:
		result << "Colour code YELLOW 1: ";
		result << "visibility &gt;2500 m and ";
		result << "lowest cloud base height &gt;500 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW2:
		result << colourCodeBlack << "\n";
		[[fallthrough]];
		case metaf::MiscGroup::Type::COLOUR_CODE_YELLOW2:
		result << "Colour code YELLOW 2: ";
		result << "visibility &gt;1600 m and ";
		result << "lowest cloud base height &gt;300 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKAMBER:
		result << colourCodeBlack << "\n";
		[[fallthrough]];
		case metaf::MiscGroup::Type::COLOUR_CODE_AMBER:
		result << "Colour code AMBER: ";
		result << "visibility &gt;800 m and ";
		result << "lowest cloud base height &gt;200 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKRED:
		result << colourCodeBlack << "\n";
		[[fallthrough]];
		case metaf::MiscGroup::Type::COLOUR_CODE_RED:
		result << "Colour code RED: ";
		result << "either visibility &lt;800 m or ";
		result << "lowest cloud base height &lt;200 ft ";
		result << "or both";
		break;

		case metaf::MiscGroup::Type::FROIN:
		result << "Frost on the instrument ";
		result << "(e.g. due to freezing fog depositing rime).";
		break;

		case metaf::MiscGroup::Type::ISSUER_ID_FN:
		result << "Report issuer identifier is ";
		result << static_cast<int>(*group.data());
		result << "\nThis forecast is issued at The Fleet Weather Center Norfolk, VA.";
		break;

		case metaf::MiscGroup::Type::ISSUER_ID_FS:
		result << "Report issuer identifier is ";
		result << static_cast<int>(*group.data());
		result << "\nThis forecast is issued at The Fleet Weather Center San Diego, CA (FS).";
		break;

		case metaf::MiscGroup::Type::FIRST:
		result << "First manned observation.";
		break;

		case metaf::MiscGroup::Type::LAST:
		result << "Last manned observation or last forecast.";
		if (group.time().has_value()) {
			result << "\nNext report is scheduled for " << explainMetafTime(*group.time());
		}
		break;


		case metaf::MiscGroup::Type::LAST_STAFFED_OBSERVATION:
		result << "Last staffed observation.";
		if (group.time().has_value()) {
			result << "\nNext report is scheduled for " << explainMetafTime(*group.time());
		}
		break;

		case metaf::MiscGroup::Type::NO_AMENDMENTS_AFTER:
		result << "No amendments are scheduled after ";
		if (group.time().has_value()) {
			result << explainMetafTime(*group.time());
		} else {
			result << "[no time provided].";
		}
		break;

		case metaf::MiscGroup::Type::NEXT:
		result << "Next report is scheduled for ";
		if (group.time().has_value()) {
			result << explainMetafTime(*group.time());
		} else {
			result << "[no time provided].";
		}
		break;

		case metaf::MiscGroup::Type::NEXT_FORECAST:
		result << "Next forecast is scheduled for ";
		if (group.time().has_value()) {
			result << explainMetafTime(*group.time());
		} else {
			result << "[no time provided].";
		}
		break;

		case metaf::MiscGroup::Type::AMENDED_AT:
		result << "Report was amended on ";
		if (group.time().has_value()) {
			result << explainMetafTime(*group.time());
		} else {
			result << "[no time provided].";
		}
		break;

		case metaf::MiscGroup::Type::CANCELLED_AT:
		result << "Forecast was cancelled at ";
		if (group.time().has_value()) {
			result << explainMetafTime(*group.time());
		} else {
			result << "[no time provided].";
		}
		break;

		case metaf::MiscGroup::Type::FORECAST_BASED_ON_AUTO_OBSERVATION:
		result << "Forecast is based on automated (rather than manned) observations.";
		break;

		case metaf::MiscGroup::Type::CONTRAILS:
		result << "Middle or high clouds consist in whole or in part of persistent ";
		result << "(&ge; 15 minutes) condensation trails which do not deteriorate rapidly.";
		break;

		case metaf::MiscGroup::Type::SUN_DIMLY_VISIBLE:
		result << "Sun is visible dimly through the cloud layer(s) or obscurations.";
		break;

		case metaf::MiscGroup::Type::MOON_DIMLY_VISIBLE:
		result << "Moon is visible dimly through the cloud layer(s) or obscurations.";
		break;
	}
	return result.str();
}


std::string VisitorExplain::visitUnknownGroup(
	const metaf::UnknownGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)group;(void)reportPart;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << "\n";
	result << "Not recognised by parser: " << rawString;
	return result.str();
}

std::string_view VisitorExplain::reportTypeToString(
	metaf::ReportType reportType)
{
	switch (reportType) {
		case metaf::ReportType::UNKNOWN:	return "unable to detect";
		case metaf::ReportType::METAR:		return "METAR (weather observation)";
		case metaf::ReportType::TAF:		return "TAF (weather forecast)";
	}
}

std::string_view VisitorExplain::reportErrorToString(
	metaf::ReportError reportError)
{
	switch (reportError) {
		case metaf::ReportError::NONE:
		return "no error";

		case metaf::ReportError::EMPTY_REPORT:
		return "report is empty";
				
		case metaf::ReportError::EXPECTED_REPORT_TYPE_OR_LOCATION:
		return "expected report type or ICAO location at report start";

		case metaf::ReportError::EXPECTED_LOCATION:
		return "expected ICAO location";
				
		case metaf::ReportError::EXPECTED_REPORT_TIME:
		return "expected report time";
				
		case metaf::ReportError::EXPECTED_TIME_SPAN:
		return "expected time span";
				
		case metaf::ReportError::UNEXPECTED_REPORT_END:
		return "unexpected report end";
				
		case metaf::ReportError::UNEXPECTED_GROUP_AFTER_NIL:
		return "unexpected group after NIL";
				
		case metaf::ReportError::UNEXPECTED_GROUP_AFTER_CNL:
		return "unexpected group after CNL";
				
		case metaf::ReportError::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY:
		return "unexpected NIL or CNL in report body";

		case metaf::ReportError::AMD_ALLOWED_IN_TAF_ONLY:
		return "AMD is allowed only in TAF reports";

		case metaf::ReportError::CNL_ALLOWED_IN_TAF_ONLY:
		return "CNL is allowed only in TAF reports";

		case metaf::ReportError::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY:
		return "Maintenance indicator is allowed only in METAR reports";

		case metaf::ReportError::REPORT_TOO_LARGE:
		return "Report has too many groups";
	}
}

std::string VisitorExplain::explainRunway(const metaf::Runway & runway) {
	if (runway.isAllRunways()) return "all runways";
	if (runway.isMessageRepetition()) 
		return "same runway (repetition of last message)"; 
	std::ostringstream result;
	result << "runway ";
	result << (runway.number() < 10 ? "0"s : ""s);
	result << runway.number();
	std::string designatorStr;
	switch(runway.designator()) {
		case metaf::Runway::Designator::NONE: 	break;
		case metaf::Runway::Designator::LEFT: 	result << " LEFT"; break;
		case metaf::Runway::Designator::CENTER:	result << " CENTER"; break;
		case metaf::Runway::Designator::RIGHT:  result << " RIGHT"; break;
	}
	return result.str();
}

std::string VisitorExplain::explainMetafTime(
	const metaf::MetafTime & metafTime)
{
	std::ostringstream result;
	if (const auto day = metafTime.day(); day.has_value()) {
		result << "day " << *day << ", ";
	}
	result << (metafTime.hour() < 10 ? "0"s : ""s) << metafTime.hour();
	result << ":";
	result << (metafTime.minute() < 10 ? "0"s : ""s) << metafTime.minute();
	result << " GMT";
	return result.str();
}

std::string VisitorExplain::explainTemperature(
	const metaf::Temperature & temperature)
{
	if (!temperature.temperature().has_value()) return "not reported";
	std::ostringstream result;
	if (!(*temperature.temperature()) && !temperature.isPrecise()) {
		if (temperature.isFreezing()) result << "slightly less than ";
		if (!temperature.isFreezing()) result << "slightly more than ";
	}
	if (const auto t = temperature.toUnit(metaf::Temperature::Unit::C);
			t.has_value())
	{
		result << roundTo(*t, 1) << " &deg;C";
	} else {
		result << "[unable to convert temperature to &deg;C]";
	}
	result << " / ";
	if (const auto t = temperature.toUnit(metaf::Temperature::Unit::F);
			t.has_value())
	{
		result << roundTo(*t, 1) << " &deg;F";
	} else {
		result << "[unable to convert temperature to &deg;F]";
	}
	return result.str();
}

std::string VisitorExplain::explainSpeed(const metaf::Speed & speed) {
	std::ostringstream result;
	if (const auto s = speed.speed(); s.has_value()) {
		result << *speed.speed() << " " << speedUnitToString(speed.unit());
	} else {
		return "not reported";
	}
	result << " (";
	if (speed.unit() != metaf::Speed::Unit::KNOTS) {
		if (const auto s = speed.toUnit(metaf::Speed::Unit::KNOTS); 
				s.has_value())
		{
			result << roundTo(*s, 1) << " ";
			result << speedUnitToString(metaf::Speed::Unit::KNOTS);
		} else {
			result << "[unable to convert speed to knots]";
		}
		result << " / ";
	}
	if (speed.unit() != metaf::Speed::Unit::METERS_PER_SECOND) {
		if (const auto s = speed.toUnit(metaf::Speed::Unit::METERS_PER_SECOND); 
				s.has_value())
		{
			result << roundTo(*s, 1) << " ";
			result << speedUnitToString(metaf::Speed::Unit::METERS_PER_SECOND);
		} else {
			result << "[unable to convert speed to m/s]";
		}
		result << " / ";
	}
	if (speed.unit() != metaf::Speed::Unit::KILOMETERS_PER_HOUR) {
		if (const auto s = 
				speed.toUnit(metaf::Speed::Unit::KILOMETERS_PER_HOUR);
				s.has_value())
		{
			result << roundTo(*s, 1) << " ";
			result << 
				speedUnitToString(metaf::Speed::Unit::KILOMETERS_PER_HOUR);
		} else {
			result << "[unable to convert speed to km/h]";
		}
		if (speed.unit() != metaf::Speed::Unit::MILES_PER_HOUR) result << " / ";
	}
	if (speed.unit() != metaf::Speed::Unit::MILES_PER_HOUR) {
		if (const auto s = speed.toUnit(metaf::Speed::Unit::MILES_PER_HOUR);
				s.has_value())
		{
			result << roundTo(*s, 1) << " ";
			result << speedUnitToString(metaf::Speed::Unit::MILES_PER_HOUR);
		} else {
			result << "[unable to convert speed to mph]";
		}
	}
	result << ")";
	return result.str();
}

std::string VisitorExplain::explainDistance(const metaf::Distance & distance) {
	if (!distance.isReported()) return "not reported";
	std::ostringstream result;
	switch (distance.modifier()) {
		case metaf::Distance::Modifier::NONE:
		break;
		
		case metaf::Distance::Modifier::LESS_THAN:
		result << "&lt;"; 
		break;
		
		case metaf::Distance::Modifier::MORE_THAN:
		result << "&gt;";
		break;

		case metaf::Distance::Modifier::DISTANT:
		result << "10 to 30 nautical miles ";
		result << "(19 to 55 km, 12 to 35 statue miles)";
		break;

		case metaf::Distance::Modifier::VICINITY:
		result << "5 to 10 nautical miles "; 
		result << "(9 to 19 km, 6 to 12 statue miles)";
		break;
	}

	if (!distance.isValue()) return result.str();

	if (distance.unit() == metaf::Distance::Unit::STATUTE_MILES) {
		const auto d = distance.miles();
		if (!d.has_value()) return "[unable to get distance value in miles]";
		const auto integer = std::get<unsigned int>(d.value());
		const auto fraction = 
			std::get<metaf::Distance::MilesFraction>(d.value());
		if (integer || fraction == metaf::Distance::MilesFraction::NONE)
			result << integer;
		if (integer && fraction != metaf::Distance::MilesFraction::NONE)
			result << " ";
		if (fraction != metaf::Distance::MilesFraction::NONE)
			result << distanceMilesFractionToString(fraction);
	} else {
		const auto d = distance.toUnit(distance.unit());
		if (!d.has_value())
			return "[unable to get distance's floating-point value]";
		result << static_cast<int>(*d);
	}
	result << " " << distanceUnitToString(distance.unit());
	result << " (";
	if (distance.unit() != metaf::Distance::Unit::METERS) {
		if (const auto d = distance.toUnit(metaf::Distance::Unit::METERS); 
			d.has_value())
		{
			result << static_cast<int>(*d);
			result << " ";
			result << distanceUnitToString(metaf::Distance::Unit::METERS);
		} else {
			result << "[unable to convert distance to meters]";
		}
		result << " / ";
	}
	if (distance.unit() != metaf::Distance::Unit::STATUTE_MILES) {
		if (const auto d = distance.miles(); d.has_value()) {
			if (!d.has_value())
				return "[unable to get distance value in miles]";
			const auto integer = std::get<unsigned int>(d.value());
			const auto fraction = 
				std::get<metaf::Distance::MilesFraction>(d.value());
			if (integer || fraction == metaf::Distance::MilesFraction::NONE)
				result << integer;
			if (integer && fraction != metaf::Distance::MilesFraction::NONE)
				result << " ";
			if (fraction != metaf::Distance::MilesFraction::NONE)
				result << distanceMilesFractionToString(fraction);
			result << " ";
			result << distanceUnitToString(metaf::Distance::Unit::STATUTE_MILES);
		} else {
			result << "[unable to convert distance to statute miles]";
		}
		if (distance.unit() != metaf::Distance::Unit::FEET) result << " / ";
	}
	if (distance.unit() != metaf::Distance::Unit::FEET) {
		if (const auto d = distance.toUnit(metaf::Distance::Unit::FEET); 
			d.has_value())
		{
			result << static_cast<int>(*d);
			result << " " << distanceUnitToString(metaf::Distance::Unit::FEET);
		} else {
			result << "[unable to convert distance to feet]";
		}
	}
	result << ")";
	return result.str();
}

std::string VisitorExplain::explainDirection(const metaf::Direction & direction,
	bool trueCardinalDirections)
{
	std::ostringstream result;
	switch (direction.type()) {
		case metaf::Direction::Type::NOT_REPORTED:
		result << "not reported";
		break;

		case metaf::Direction::Type::VARIABLE:
		result << "variable";
		break;

		case metaf::Direction::Type::NDV:
		result << "no directional variation";
		break;

		case metaf::Direction::Type::VALUE_DEGREES:
		if (const auto d = direction.degrees(); d.has_value()) {
			result << *d << " degrees";
		} else {
			result << "[unable to produce value in &deg;]";
		}		
		[[fallthrough]];
		case metaf::Direction::Type::VALUE_CARDINAL:
		if (const auto c = 
				cardinalDirectionToString(
					direction.cardinal(trueCardinalDirections)); 
			!c.empty())
		{
			if (direction.type() == metaf::Direction::Type::VALUE_DEGREES) {
				result << "(" << c << ")";
			} else {
				result << c; 
			}
		}
		break;

		case metaf::Direction::Type::OVERHEAD:
		result << "overhead";
		break;

		case metaf::Direction::Type::ALQDS:
		result << "all quadrants (all directions)";
		break;

		case metaf::Direction::Type::UNKNOWN:
		result << "unknown direction";
		break;
	}
	return result.str();
}

std::string VisitorExplain::explainPressure(const metaf::Pressure & pressure) {
	if (!pressure.pressure().has_value()) return "not reported";
	std::ostringstream result;
	if (const auto phpa = pressure.toUnit(metaf::Pressure::Unit::HECTOPASCAL);
		phpa.has_value())
	{
		result << roundTo(*phpa, 1) << " hPa";
	} else {
		result << "[unable to convert pressure to hPa]";
	}
	result << " / ";
	if (const auto pinhg = pressure.toUnit(metaf::Pressure::Unit::INCHES_HG);
		pinhg.has_value())
	{
		result << roundTo(*pinhg, 2) << " inHg";
	} else {
		result << "[unable to convert pressure to inHg]";
	}
	result << " / ";
	if (const auto pmmhg = pressure.toUnit(metaf::Pressure::Unit::MM_HG);
		pmmhg.has_value())
	{
		result << roundTo(*pmmhg, 1) << " mmHg";
	} else {
		result << "[unable to convert pressure to mmHg]";
	}
	return result.str();
}

std::string VisitorExplain::explainPrecipitation(
	const metaf::Precipitation & precipitation)
{
	std::ostringstream result;
	if (!precipitation.isReported()) return "not reported";
	if (const auto p = precipitation.amount(); p.has_value() && !*p) 
		return "trace amount";
	if (const auto p = precipitation.toUnit(metaf::Precipitation::Unit::MM);
		p.has_value())
	{
		result << roundTo(*p, 1) << " mm";
	} else {
		result << "[unable to convert precipitation to mm]";
	}
	result << " / ";
	if (const auto p = precipitation.toUnit(metaf::Precipitation::Unit::INCHES);
		p.has_value())
	{
		result << roundTo(*p, 2) << " inches";
	} else {
		result << "[unable to convert precipitation to inches]";
	}
	return result.str();
}

std::string VisitorExplain::explainSurfaceFriction(
	const metaf::SurfaceFriction & surfaceFriction)
{
	switch (surfaceFriction.type()) {
		case metaf::SurfaceFriction::Type::NOT_REPORTED:
		return "not reported";

		case metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED:
		if (const auto c = surfaceFriction.coefficient(); c.has_value()) {
			return ("friction coefficient "s + roundTo(*c, 2));
		}
		return "[unable to produce a friction coefficient]";
		
		case metaf::SurfaceFriction::Type::BRAKING_ACTION_REPORTED:
		return ("braking action "s + 
		   std::string(brakingActionToString(surfaceFriction.brakingAction())));

		case metaf::SurfaceFriction::Type::UNRELIABLE:
		return "unreliable or unmeasurable";
	}
}

std::string VisitorExplain::explainWaveHeight(
	const metaf::WaveHeight & waveHeight)
{
	switch (waveHeight.type()) {
		case metaf::WaveHeight::Type::STATE_OF_SURFACE:
		return ("state of sea surface: "s + 
		   std::string(stateOfSeaSurfaceToString(waveHeight.stateOfSurface())));

		case metaf::WaveHeight::Type::WAVE_HEIGHT:
		if (waveHeight.isReported()) {
			std::ostringstream result;
			result << "wave height: ";
			if (const auto h = 
					waveHeight.toUnit(metaf::WaveHeight::Unit::METERS); 
					h.has_value())
			{
				result << roundTo(*h, 1) << " meters";
			} else {
				result << "[unable to convert wave height to meters]";
			}
			result << " / ";
			if (const auto h = 
					waveHeight.toUnit(metaf::WaveHeight::Unit::FEET); 
					h.has_value())
			{
				result << roundTo(*h, 1) << " feet";
			} else {
				result << "[unable to convert wave height to feet]";
			}
			return result.str();
		}
		return "wave height not reported";
	}
}

std::string VisitorExplain::explainWeatherPhenomena(
	const metaf::WeatherPhenomena & wp)
{
	std::ostringstream result;

	if (const auto weatherStr = specialWeatherPhenomenaToString(wp); 
		!weatherStr.empty()) {
			result << weatherStr;
			return result.str();
	}

	const bool vicinity = 
		(wp.qualifier() == metaf::WeatherPhenomena::Qualifier::VICINITY);
	const bool showers = 
		(wp.descriptor() == metaf::WeatherPhenomena::Descriptor::SHOWERS);
	const bool thunderstorm = 
		(wp.descriptor() == metaf::WeatherPhenomena::Descriptor::THUNDERSTORM);
	
	if (const auto q = weatherPhenomenaQualifierToString(wp.qualifier());
		!q.empty()) 
	{
		result << q << " ";
	}
	if (const auto d = weatherPhenomenaDescriptorToString(wp.descriptor());
		!d.empty()) 
	{
		result << d << " ";
	}
	metaf::WeatherPhenomena::Weather previous = 
		metaf::WeatherPhenomena::Weather::NOT_REPORTED;
	const auto weather = wp.weather();
	for (const auto w : weather) {
		std::string weatherStr(weatherPhenomenaWeatherToString(w));
		if (previous == metaf::WeatherPhenomena::Weather::RAIN && 
			w == metaf::WeatherPhenomena::Weather::SNOW) {
				weatherStr = "and snow mix";
		}
		if (previous == metaf::WeatherPhenomena::Weather::SNOW && 
			w == metaf::WeatherPhenomena::Weather::RAIN) {
				weatherStr = "and rain mix";
		}
		result << weatherStr << " ";
		previous = w;
	}
	if (showers) result << "showers ";
	if (thunderstorm) {
		if (weather.size()) result << "with ";
		result << "thunderstorm ";
	}
	if (vicinity) result << "in vicinity (5 to 10 miles away)";

	const auto time = wp.time();
	switch (wp.event()){
		case metaf::WeatherPhenomena::Event::BEGINNING:
		if (!time.has_value()) break;
		result << " began: ";
		result << explainMetafTime(*wp.time());
		break;

		case metaf::WeatherPhenomena::Event::ENDING:
		if (!time.has_value()) break;
		result << " ended: ";
		result << explainMetafTime(*wp.time());
		break;

		case metaf::WeatherPhenomena::Event::NONE:
		break;
	}

	return result.str();
}

std::string VisitorExplain::explainDirectionSector(
	const std::vector<metaf::Direction> dir)
{
	std::string result;
	for (auto i=0u; i<dir.size(); i++) {
		if (i) result += ", ";
		result += cardinalDirectionToString(dir[i].cardinal());
	}
	return result;
}

std::string VisitorExplain::explainCloudType(const metaf::CloudType ct) {
	std::ostringstream result;
	if (!ct.okta().has_value()) {
		result << "Unknown amount of ";
		result << cloudTypeToString(ct.type());
	} else {
		result << cloudTypeToString(ct.type());
		result << " covering ";
		if (const auto okta = *ct.okta(); okta) {
			result << okta;
			result << "/8 of the sky";		
		} else {
			result << "&lt;1/8 of the sky";
		}
	}
	if (const auto h = ct.height(); h.isReported()) {
		result << ", base height ";
		result << explainDistance(ct.height());
	}
	return result.str();
}

std::string_view VisitorExplain::speedUnitToString(metaf::Speed::Unit unit) {
	switch (unit) {	
		case metaf::Speed::Unit::KNOTS:
		return "knots";

		case metaf::Speed::Unit::METERS_PER_SECOND:
		return "m/s";
		
		case metaf::Speed::Unit::KILOMETERS_PER_HOUR:
		return "km/h";
		
		case metaf::Speed::Unit::MILES_PER_HOUR:
		return "mph";
	}
}

std::string_view VisitorExplain::distanceUnitToString(
	metaf::Distance::Unit unit)
{
	switch (unit) {
		case metaf::Distance::Unit::METERS:
		return "meters";
		
		case metaf::Distance::Unit::STATUTE_MILES:
		return "statute miles";
		
		case metaf::Distance::Unit::FEET:
		return "feet";
	}
}

std::string_view VisitorExplain::distanceMilesFractionToString(
	metaf::Distance::MilesFraction f)
{
	switch (f) {
		case metaf::Distance::MilesFraction::NONE:
		return "";
		
		case metaf::Distance::MilesFraction::F_1_16:
		return "1/16";
		
		case metaf::Distance::MilesFraction::F_1_8:
		return "1/8";
		
		case metaf::Distance::MilesFraction::F_3_16:
		return "3/16";
		
		case metaf::Distance::MilesFraction::F_1_4:
		return "1/4";
		
		case metaf::Distance::MilesFraction::F_5_16:
		return "5/16";
		
		case metaf::Distance::MilesFraction::F_3_8:
		return "3/8";
		
		case metaf::Distance::MilesFraction::F_1_2:
		return "1/2";
		
		case metaf::Distance::MilesFraction::F_5_8:
		return "5/8";
		
		case metaf::Distance::MilesFraction::F_3_4:
		return "3/4";
		
		case metaf::Distance::MilesFraction::F_7_8:
		return "7/8";
	}
}


std::string_view VisitorExplain::cardinalDirectionToString(
	metaf::Direction::Cardinal cardinal)
{
	switch(cardinal) {
		case metaf::Direction::Cardinal::NOT_REPORTED:
		return "not reported";
		
		case metaf::Direction::Cardinal::N:
		return "north";
		
		case metaf::Direction::Cardinal::S:
		return "south";
		
		case metaf::Direction::Cardinal::W:
		return "west";
		
		case metaf::Direction::Cardinal::E:
		return "east";
		
		case metaf::Direction::Cardinal::NW:
		return "northwest";
		
		case metaf::Direction::Cardinal::NE:
		return "northeast";
		
		case metaf::Direction::Cardinal::SW:
		return "southwest";
		
		case metaf::Direction::Cardinal::SE:
		return "southeast";
		
		case metaf::Direction::Cardinal::TRUE_N:
		return "true north";
		
		case metaf::Direction::Cardinal::TRUE_W:
		return "true west";
		
		case metaf::Direction::Cardinal::TRUE_S:
		return "true south";
		
		case metaf::Direction::Cardinal::TRUE_E:
		return "true east";
		
		case metaf::Direction::Cardinal::NDV:
		return "no directional variations";
		
		case metaf::Direction::Cardinal::VRB:
		return "variable";
		
		case metaf::Direction::Cardinal::OHD:
		return "overhead";
		
		case metaf::Direction::Cardinal::ALQDS:
		return "all quadrants (in all directions)";
		
		case metaf::Direction::Cardinal::UNKNOWN:
		return "unknown direction";
	}
}

std::string_view VisitorExplain::brakingActionToString(
		metaf::SurfaceFriction::BrakingAction brakingAction)
{
	switch(brakingAction) {
		case metaf::SurfaceFriction::BrakingAction::NONE:
		return "not reported";

		case metaf::SurfaceFriction::BrakingAction::POOR:
		return "poor (friction coefficient 0.0 to 0.25)";

		case metaf::SurfaceFriction::BrakingAction::MEDIUM_POOR:
		return "medium/poor (friction coefficient 0.26 to 0.29)";

		case metaf::SurfaceFriction::BrakingAction::MEDIUM:
		return "medium (friction coefficient 0.30 to 0.35)";

		case metaf::SurfaceFriction::BrakingAction::MEDIUM_GOOD:
		return "medium/good (friction coefficient 0.36 to 0.40)";

		case metaf::SurfaceFriction::BrakingAction::GOOD:
		return "good (friction coefficient 0.40 to 1.00)";
	}
}

std::string_view VisitorExplain::stateOfSeaSurfaceToString(
	metaf::WaveHeight::StateOfSurface stateOfSurface)
{
	switch(stateOfSurface) {
		case metaf::WaveHeight::StateOfSurface::NOT_REPORTED:
		return "not reported";

		case metaf::WaveHeight::StateOfSurface::CALM_GLASSY:
		return "calm (glassy), no waves";

		case metaf::WaveHeight::StateOfSurface::CALM_RIPPLED:
		return "calm (rippled), wave height &lt;0.1 meters / &lt;1/3 feet";

		case metaf::WaveHeight::StateOfSurface::SMOOTH:
		return "smooth, wave height 0.1 to 0.5 meters / 1/3 to 1 1/2 feet";

		case metaf::WaveHeight::StateOfSurface::SLIGHT:
		return "slight, wave height 0.5 to 1.25 meters / 1 1/2 to 4 feet";

		case metaf::WaveHeight::StateOfSurface::MODERATE:
		return "moderate, wave height 1.25 to 2.5 meters / 4 to 8 feet";

		case metaf::WaveHeight::StateOfSurface::ROUGH:
		return "rough, wave height 2.5 to 4 meters / 8 to 13 feet";

		case metaf::WaveHeight::StateOfSurface::VERY_ROUGH:
		return "very rough, wave height 4 to 6 meters / 13 to 20 feet";

		case metaf::WaveHeight::StateOfSurface::HIGH:
		return "high, wave height 6 to 9 meters / 20 to 30 feet";

		case metaf::WaveHeight::StateOfSurface::VERY_HIGH:
		return "very high, wave height 9 to 14 meters / 30 to 46 feet";

		case metaf::WaveHeight::StateOfSurface::PHENOMENAL:
		return "phenomenal, wave height >14 meters / &gt;46 feet";
	}
}

std::string_view VisitorExplain::cloudAmountToString(metaf::CloudGroup::Amount amount) {
	switch (amount) {
		case metaf::CloudGroup::Amount::NOT_REPORTED:
		return "Cloud amount not reported";

		case metaf::CloudGroup::Amount::NSC:
		return "No significant cloud: "
			"no cloud below 5000 feet (1500 meters), no cumulonimbus or towering "
			"cumulus clouds, no vertical visibility restriction";

		case metaf::CloudGroup::Amount::NCD:
		return "No cloud detected: automated weather station did not detect any clouds; "
			"this can happen due to either no clouds present or sensor error";

		case metaf::CloudGroup::Amount::NONE_CLR:
		return "Clear sky: "
			"no cloud layers are detected at or below 12000 feet (3700 meters) (US) "
			"or 25000 feet (7600 meters) (Canada); "
			"indicates that station is at least partly automated";

		case metaf::CloudGroup::Amount::NONE_SKC:
		return "Clear sky: "
			"In North America indicates report producted by human rather than "
			"automatic weather station";

		case metaf::CloudGroup::Amount::FEW:
		return "Few clouds (1/8 to 2/8 sky covered)";

		case metaf::CloudGroup::Amount::SCATTERED:
		return "Scattered clouds (3/8 to 4/8 sky covered)";

		case metaf::CloudGroup::Amount::BROKEN:
		return "Broken clouds (5/8 to 7/8 sky covered)";

		case metaf::CloudGroup::Amount::OVERCAST:
		return "Overcast (8/8 sky covered)";

		case metaf::CloudGroup::Amount::OBSCURED:
		return "Sky obscured";

		case metaf::CloudGroup::Amount::TRACE:
		return "Trace amount of clouds";
	}
}

std::string_view VisitorExplain::convectiveTypeToString(
	metaf::CloudGroup::ConvectiveType type)
{
	switch (type) {
		case metaf::CloudGroup::ConvectiveType::NONE:
		return std::string_view();

		case metaf::CloudGroup::ConvectiveType::NOT_REPORTED:
		return "not reported";

		case metaf::CloudGroup::ConvectiveType::TOWERING_CUMULUS:
		return "towering cumulus";

		case metaf::CloudGroup::ConvectiveType::CUMULONIMBUS:
		return "cumulonimbus";
	}
}

std::string_view VisitorExplain::weatherPhenomenaQualifierToString(
	metaf::WeatherPhenomena::Qualifier qualifier)
{
	switch (qualifier) {
		case metaf::WeatherPhenomena::Qualifier::NONE:
		return std::string_view();
		
		case metaf::WeatherPhenomena::Qualifier::RECENT:
		return std::string_view();
		
		case metaf::WeatherPhenomena::Qualifier::VICINITY:
		return std::string_view();
		
		case metaf::WeatherPhenomena::Qualifier::LIGHT:
		return "light";
		
		case metaf::WeatherPhenomena::Qualifier::MODERATE:
		return "moderate";
		
		case metaf::WeatherPhenomena::Qualifier::HEAVY:
		return "heavy";
	}
}

std::string_view VisitorExplain::weatherPhenomenaDescriptorToString(
	metaf::WeatherPhenomena::Descriptor descriptor)
{
	switch(descriptor) {
		case metaf::WeatherPhenomena::Descriptor::NONE:
		return std::string_view();
		
		case metaf::WeatherPhenomena::Descriptor::SHALLOW:
		return "shallow";
		
		case metaf::WeatherPhenomena::Descriptor::PARTIAL:
		return "partial";
		
		case metaf::WeatherPhenomena::Descriptor::PATCHES:
		return "patches of ";
		
		case metaf::WeatherPhenomena::Descriptor::LOW_DRIFTING:
		return "low drifting";
		
		case metaf::WeatherPhenomena::Descriptor::BLOWING:
		return "blowing";
		
		case metaf::WeatherPhenomena::Descriptor::SHOWERS:
		return std::string_view();
		
		case metaf::WeatherPhenomena::Descriptor::THUNDERSTORM:
		return std::string_view();
		
		case metaf::WeatherPhenomena::Descriptor::FREEZING:
		return "freezing";
	}
}

std::string_view VisitorExplain::weatherPhenomenaWeatherToString(
	metaf::WeatherPhenomena::Weather weather)
{
	switch (weather) {
			case metaf::WeatherPhenomena::Weather::NOT_REPORTED:
			return "not reported";

			case metaf::WeatherPhenomena::Weather::DRIZZLE:
			return "drizzle";

			case metaf::WeatherPhenomena::Weather::RAIN:
			return "rain";

			case metaf::WeatherPhenomena::Weather::SNOW:
			return "snow";

			case metaf::WeatherPhenomena::Weather::SNOW_GRAINS:
			return "snow grains";

			case metaf::WeatherPhenomena::Weather::ICE_CRYSTALS:
			return "ice crystals";

			case metaf::WeatherPhenomena::Weather::ICE_PELLETS:
			return "ice pellets";

			case metaf::WeatherPhenomena::Weather::HAIL:
			return "hail";

			case metaf::WeatherPhenomena::Weather::SMALL_HAIL:
			return "small hail (graupel)";

			case metaf::WeatherPhenomena::Weather::UNDETERMINED:
			return "undetermined precipitation";

			case metaf::WeatherPhenomena::Weather::MIST:
			return "mist";

			case metaf::WeatherPhenomena::Weather::FOG:
			return "fog";

			case metaf::WeatherPhenomena::Weather::SMOKE:
			return "smoke";

			case metaf::WeatherPhenomena::Weather::VOLCANIC_ASH:
			return "volcanic ash";

			case metaf::WeatherPhenomena::Weather::DUST:
			return "dust";

			case metaf::WeatherPhenomena::Weather::SAND:
			return "sand";

			case metaf::WeatherPhenomena::Weather::HAZE:
			return "haze";

			case metaf::WeatherPhenomena::Weather::SPRAY:
			return "spray";

			case metaf::WeatherPhenomena::Weather::DUST_WHIRLS:
			return "dust or sand whirls";

			case metaf::WeatherPhenomena::Weather::SQUALLS:
			return "squalls";

			case metaf::WeatherPhenomena::Weather::FUNNEL_CLOUD:
			return "funnel cloud";

			case metaf::WeatherPhenomena::Weather::SANDSTORM:
			return "sand storm";

			case metaf::WeatherPhenomena::Weather::DUSTSTORM:
			return "dust storm";
	}
}

std::string_view VisitorExplain::specialWeatherPhenomenaToString(
	const metaf::WeatherPhenomena & wp)
{
	using WeatherVector = std::vector<metaf::WeatherPhenomena::Weather>;
	using SpecialWeatherPhenomena = std::tuple<	
		metaf::WeatherPhenomena::Qualifier, 
		metaf::WeatherPhenomena::Descriptor, 
		WeatherVector,
		std::string_view
		>;

	static const std::vector <SpecialWeatherPhenomena> 
		specialWeatherPhenomena = {
			SpecialWeatherPhenomena(
				metaf::WeatherPhenomena::Qualifier::VICINITY,
				metaf::WeatherPhenomena::Descriptor::SHOWERS,
				{},
				"precipitation in vicinity (up to 10 miles away)"),
			SpecialWeatherPhenomena(
				metaf::WeatherPhenomena::Qualifier::NONE,
				metaf::WeatherPhenomena::Descriptor::NONE,
				{ metaf::WeatherPhenomena::Weather::ICE_CRYSTALS },
				"ice crystals (diamond dust)"),
			SpecialWeatherPhenomena(
				metaf::WeatherPhenomena::Qualifier::NONE,
				metaf::WeatherPhenomena::Descriptor::NONE,
				{ metaf::WeatherPhenomena::Weather::DUST },
				"widespread dust"),
			SpecialWeatherPhenomena(
				metaf::WeatherPhenomena::Qualifier::NONE,
				metaf::WeatherPhenomena::Descriptor::NONE,
				{ metaf::WeatherPhenomena::Weather::UNDETERMINED },
				"undetermined precipitation "
				"(automated system cannot identify precipitation)"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherPhenomena::Qualifier::NONE,
				metaf::WeatherPhenomena::Descriptor::SHALLOW,
				{ metaf::WeatherPhenomena::Weather::FOG },
				"shallow fog (ground fog)"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherPhenomena::Qualifier::NONE,
				metaf::WeatherPhenomena::Descriptor::PARTIAL,
				{ metaf::WeatherPhenomena::Weather::FOG },
				"fog covering part of the location"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherPhenomena::Qualifier::NONE,
				metaf::WeatherPhenomena::Descriptor::PATCHES,
				{ metaf::WeatherPhenomena::Weather::FOG },
				"patches of fog randomly covering the location"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherPhenomena::Qualifier::NONE,
				metaf::WeatherPhenomena::Descriptor::FREEZING,
				{ metaf::WeatherPhenomena::Weather::FOG },
				"fog at freezing temperture (may or may not deposit rime)"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherPhenomena::Qualifier::HEAVY,
				metaf::WeatherPhenomena::Descriptor::NONE,
				{ metaf::WeatherPhenomena::Weather::FUNNEL_CLOUD },
				"tornado or waterspout"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherPhenomena::Qualifier::VICINITY,
				metaf::WeatherPhenomena::Descriptor::NONE,
				{ metaf::WeatherPhenomena::Weather::FUNNEL_CLOUD },
				"tornadic activity in vicinity"
			)
	};

	for (const auto & w : specialWeatherPhenomena) {
		if (wp.qualifier() == 
				std::get<metaf::WeatherPhenomena::Qualifier>(w) &&
			wp.descriptor() == 
				std::get<metaf::WeatherPhenomena::Descriptor>(w) &&
			wp.weather() == std::get<WeatherVector>(w))
		{
			return std::get<std::string_view>(w);
		}
	}
	return std::string_view();
}

std::string_view VisitorExplain::probabilityToString(
	metaf::TrendGroup::Probability prob)
{
	switch (prob) {
		case metaf::TrendGroup::Probability::PROB_30:
		return "Trend probability is 30 percent";

		case metaf::TrendGroup::Probability::PROB_40:
		return "Trend probability is 40 percent";

		case metaf::TrendGroup::Probability::NONE:
		return "";
	}
}

std::string_view VisitorExplain::visTrendToString(
	metaf::VisibilityGroup::Trend trend)
{
	switch(trend) {
		case metaf::VisibilityGroup::Trend::NONE:
		return std::string_view();
		
		case metaf::VisibilityGroup::Trend::NOT_REPORTED:
		return "not reported";
		
		case metaf::VisibilityGroup::Trend::UPWARD:
		return "upward";
		
		case metaf::VisibilityGroup::Trend::NEUTRAL:
		return "neutral";
		
		case metaf::VisibilityGroup::Trend::DOWNWARD:
		return "downward";
	}
}

std::string_view VisitorExplain::runwayStateDepositsToString(
	metaf::RunwayStateGroup::Deposits deposits)
{
	switch(deposits) {
		case metaf::RunwayStateGroup::Deposits::NOT_REPORTED:
		return "not reported";

		case metaf::RunwayStateGroup::Deposits::CLEAR_AND_DRY:
		return "clear and dry";

		case metaf::RunwayStateGroup::Deposits::DAMP:
		return "damp";

		case metaf::RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES:
		return "wet and water patches";

		case metaf::RunwayStateGroup::Deposits::RIME_AND_FROST_COVERED:
		return "rime and frost covered";

		case metaf::RunwayStateGroup::Deposits::DRY_SNOW:
		return "dry snow";

		case metaf::RunwayStateGroup::Deposits::WET_SNOW:
		return "wet snow";

		case metaf::RunwayStateGroup::Deposits::SLUSH:
		return "slush";

		case metaf::RunwayStateGroup::Deposits::ICE:
		return "ice";

		case metaf::RunwayStateGroup::Deposits::COMPACTED_OR_ROLLED_SNOW:
		return "compacted or rolled snow";

		case metaf::RunwayStateGroup::Deposits::FROZEN_RUTS_OR_RIDGES:
		return "frozen ruts or ridges";
	}
}

std::string_view VisitorExplain::runwayStateExtentToString(
	metaf::RunwayStateGroup::Extent extent)
{
	switch(extent) {
		case metaf::RunwayStateGroup::Extent::NOT_REPORTED:
		return "not reported";

		case metaf::RunwayStateGroup::Extent::NONE:
		return "none";

		case metaf::RunwayStateGroup::Extent::LESS_THAN_10_PERCENT:
		return "&lt;10 percent";

		case metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT:
		return "11 to 25 percent";

		case metaf::RunwayStateGroup::Extent::FROM_26_TO_50_PERCENT:
		return "26 to 50 percent";

		case metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT:
		return "&gt;51 percent";

		case metaf::RunwayStateGroup::Extent::RESERVED_3:
		return "[reserved_extent_value 3]";

		case metaf::RunwayStateGroup::Extent::RESERVED_4:
		return "[reserved_extent_value 4]";

		case metaf::RunwayStateGroup::Extent::RESERVED_6:
		return "[reserved_extent_value 6]";

		case metaf::RunwayStateGroup::Extent::RESERVED_7:
		return "[reserved_extent_value 7]";

		case metaf::RunwayStateGroup::Extent::RESERVED_8:
		return "[reserved_extent_value 8]";
	}
}

std::string_view VisitorExplain::layerForecastGroupTypeToString(
	metaf::LayerForecastGroup::Type type)
{
	switch(type) {
		case metaf::LayerForecastGroup::Type::ICING_TRACE_OR_NONE:
		return "Trace icing or no icing";

		case metaf::LayerForecastGroup::Type::ICING_LIGHT_MIXED:
		return "Light mixed icing";

		case metaf::LayerForecastGroup::Type::ICING_LIGHT_RIME_IN_CLOUD:
		return "Light rime icing in cloud";

		case metaf::LayerForecastGroup::Type::
			ICING_LIGHT_CLEAR_IN_PRECIPITATION:
		return "Light clear icing in precipitation";

		case metaf::LayerForecastGroup::Type::ICING_MODERATE_MIXED:
		return "Moderate mixed icing";

		case metaf::LayerForecastGroup::Type::ICING_MODERATE_RIME_IN_CLOUD:
		return "Moderate rime icing in cloud";

		case metaf::LayerForecastGroup::Type::
			ICING_MODERATE_CLEAR_IN_PRECIPITATION:
		return "Moderate clear icing in precipitation";

		case metaf::LayerForecastGroup::Type::ICING_SEVERE_MIXED:
		return "Severe mixed icing";

		case metaf::LayerForecastGroup::Type::ICING_SEVERE_RIME_IN_CLOUD:
		return "Severe rime icing in cloud";

		case metaf::LayerForecastGroup::Type::
			ICING_SEVERE_CLEAR_IN_PRECIPITATION:
		return "Severe clear icing in precipitation";

		case metaf::LayerForecastGroup::Type::TURBULENCE_NONE:
		return "No turbulence";

		case metaf::LayerForecastGroup::Type::TURBULENCE_LIGHT:
		return "Light turbulence";

		case metaf::LayerForecastGroup::Type::
			TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASIONAL:
		return "Occasional moderate turbulence in clear air";

		case metaf::LayerForecastGroup::Type::
			TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT:
		return "Frequent moderate turbulence in clear air";

		case metaf::LayerForecastGroup::Type::
			TURBULENCE_MODERATE_IN_CLOUD_OCCASIONAL:
		return "Occasional moderate turbulence in cloud";

		case metaf::LayerForecastGroup::Type::
			TURBULENCE_MODERATE_IN_CLOUD_FREQUENT:
		return "Frequent moderate turbulence in cloud";

		case metaf::LayerForecastGroup::Type::
			TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASIONAL:
		return "Occasional severe turbulence in clear air";

		case metaf::LayerForecastGroup::Type::
			TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT:
		return "Frequent severe turbulence in clear air";

		case metaf::LayerForecastGroup::Type::
			TURBULENCE_SEVERE_IN_CLOUD_OCCASIONAL:
		return "Occasional severe turbulence in cloud";

		case metaf::LayerForecastGroup::Type::
			TURBULENCE_SEVERE_IN_CLOUD_FREQUENT:
		return "Frequent severe turbulence in cloud";

		case metaf::LayerForecastGroup::Type::TURBULENCE_EXTREME:
		return "Extreme turbulence";
	}
}


std::string_view VisitorExplain::pressureTendencyTypeToString(
	metaf::PressureTendencyGroup::Type type)
{
	switch(type) {
		case metaf::PressureTendencyGroup::Type::INCREASING_THEN_DECREASING:
		return "increasing, then decreasing";

		case metaf::PressureTendencyGroup::Type::INCREASING_MORE_SLOWLY:
		return "increasing, then steady, "
			"or increasing then increasing more slowly";

		case metaf::PressureTendencyGroup::Type::INCREASING:
		return "increasing steadily or unsteadily";

		case metaf::PressureTendencyGroup::Type::INCREASING_MORE_RAPIDLY:
		return "decreasing or steady, then increasing; "
			"or increasing then increasing more rapidly";

		case metaf::PressureTendencyGroup::Type::STEADY:
		return "steady";

		case metaf::PressureTendencyGroup::Type::DECREASING_THEN_INCREASING:
		return "decreasing, then increasing";

		case metaf::PressureTendencyGroup::Type::DECREASING_MORE_SLOWLY:
		return "decreasing then steady; "
			"or decreasing then decreasing more slowly";

		case metaf::PressureTendencyGroup::Type::DECREASING:
		return "decreasing steadily or unsteadily";

		case metaf::PressureTendencyGroup::Type::DECREASING_MORE_RAPIDLY:
		return "steady or increasing, then decreasing; "
			"or decreasing then decreasing more rapidly";

		case metaf::PressureTendencyGroup::Type::NOT_REPORTED:
		return "not reported";

		case metaf::PressureTendencyGroup::Type::RISING_RAPIDLY:
		return "rising rapidly at a rate of "
			"at least 0.06 inch of mercury (2.03 hectopascal) per hour "
			"and the pressure change totals "
			"0.02 inch of mercury (0.68 hectopascal) or more "
			"at the time of the observation";

		case metaf::PressureTendencyGroup::Type::FALLING_RAPIDLY:
		return "falling rapidly at a rate of "
			"at least 0.06 inch of mercury (2.03 hectopascal) per hour "
			"and the pressure change totals "
			"0.02 inch of mercury (0.68 hectopascal) or more "
			"at the time of the observation";
	}
}

std::string_view VisitorExplain::pressureTendencyTrendToString(
	metaf::PressureTendencyGroup::Trend trend)
{
	switch(trend) {
		case metaf::PressureTendencyGroup::Trend::NOT_REPORTED:
		return "not reported";

		case metaf::PressureTendencyGroup::Trend::HIGHER:
		return "higher than";

		case metaf::PressureTendencyGroup::Trend::HIGHER_OR_SAME:
		return "higher or the same as";

		case metaf::PressureTendencyGroup::Trend::SAME:
		return "same as";

		case metaf::PressureTendencyGroup::Trend::LOWER_OR_SAME:
		return "lower or the same as";

		case metaf::PressureTendencyGroup::Trend::LOWER:
		return "lower than";
	}	
}

std::string_view VisitorExplain::cloudTypeToString(
	metaf::CloudType::Type type)
{
	switch(type) {
		case metaf::CloudType::Type::NOT_REPORTED:
		return "cumulonimbus";
		
		case metaf::CloudType::Type::CUMULONIMBUS:
		return "cumulonimbus";
		
		case metaf::CloudType::Type::TOWERING_CUMULUS:
		return "towering cumulus";
		
		case metaf::CloudType::Type::CUMULUS:
		return "cumulus";
		
		case metaf::CloudType::Type::CUMULUS_FRACTUS:
		return "cumulus fractus";
		
		case metaf::CloudType::Type::STRATOCUMULUS:
		return "stratocumulus";
		
		case metaf::CloudType::Type::NIMBOSTRATUS:
		return "nimbostratus";
		
		case metaf::CloudType::Type::STRATUS:
		return "stratus";
		
		case metaf::CloudType::Type::STRATUS_FRACTUS:
		return "stratus fractus";
		
		case metaf::CloudType::Type::ALTOSTRATUS:
		return "altostratus";
		
		case metaf::CloudType::Type::ALTOCUMULUS:
		return "altocumulus";
		
		case metaf::CloudType::Type::ALTOCUMULUS_CASTELLANUS:
		return "altocumulus castellanus";
		
		case metaf::CloudType::Type::CIRRUS:
		return "cirrus";
		
		case metaf::CloudType::Type::CIRROSTRATUS:
		return "cirrostratus";
		
		case metaf::CloudType::Type::CIRROCUMULUS:
		return "cirrocumulus";
		
		case metaf::CloudType::Type::BLOWING_SNOW:
		return "blowing snow";
		
		case metaf::CloudType::Type::BLOWING_DUST:
		return "blowing dust";
		
		case metaf::CloudType::Type::BLOWING_SAND:
		return "blowing sand";
		
		case metaf::CloudType::Type::ICE_CRYSTALS:
		return "ice crystals";
		
		case metaf::CloudType::Type::RAIN:
		return "rain";
		
		case metaf::CloudType::Type::DRIZZLE:
		return "drizzle";
		
		case metaf::CloudType::Type::SNOW:
		return "snow";
		
		case metaf::CloudType::Type::ICE_PELLETS:
		return "ice pellets";
		
		case metaf::CloudType::Type::SMOKE:
		return "smoke";
		
		case metaf::CloudType::Type::FOG:
		return "fog";
		
		case metaf::CloudType::Type::MIST:
		return "mist";
		
		case metaf::CloudType::Type::HAZE:
		return "haze";

		case metaf::CloudType::Type::VOLCANIC_ASH:
		return "volcanic ash";

		case metaf::CloudType::Type::HAIL:
		return "hail";

		case metaf::CloudType::Type::DUSTSTORM:
		return "dust storm";

		case metaf::CloudType::Type::SANDSTORM:
		return "sand storm";

	}
}

std::string_view VisitorExplain::cloudLowLayerToString(
	metaf::LowMidHighCloudGroup::LowLayer lowLayer)
{
	switch(lowLayer) {
		case metaf::LowMidHighCloudGroup::LowLayer::NONE:
		return "No low layer clouds";

		case metaf::LowMidHighCloudGroup::LowLayer::CU_HU_CU_FR:
		return "Cumulus clouds showing little vertical extent "
			"(Cumulus humilis or Cumulus fractus of dry weather or both)";

		case metaf::LowMidHighCloudGroup::LowLayer::CU_MED_CU_CON:
		return "Cumulus clouds showing moderate or significant vertical extent "
			"(Cumulus mediocris or Cumulus congestus, with or without Cumulus "
			"humilis or Cumulus fractus or stratocumulus, "
			"all having their bases on the same level)";

		case metaf::LowMidHighCloudGroup::LowLayer::CB_CAL:
		return "Cumulonimbus clouds without fibrous or striated parts at "
			"summit (Cumulonimbus calvus with or without Cumulus, "
			"Stratocumulus or Stratus)";

		case metaf::LowMidHighCloudGroup::LowLayer::SC_CUGEN:
		return "Stratocumulus clouds resulting from "
			"the spreading out of Cumulus (Stratocumulus cumulogenitus; "
			"Cumulus may also be present)";

		case metaf::LowMidHighCloudGroup::LowLayer::SC_NON_CUGEN:
		return "Stratocumulus clouds not resulting from "
			"the spreading out of Cumulus (Stratocumulus non-cumulogenitus)";

		case metaf::LowMidHighCloudGroup::LowLayer::ST_NEB_ST_FR:
		return "Stratus clouds which consist of a continuous "
			"single sheet or layer with a fairly uniform base, or "
			"transitory stage during the formation or "
			"the dissipation of such clouds "
			"(Stratus nebulosus or Stratus fractus of dry weather, or both)";

		case metaf::LowMidHighCloudGroup::LowLayer::ST_FR_CU_FR_PANNUS:
		return "Ragged grey clouds which form below precipitating clouds "
			"(Stratus fractus or Cumulus fractus of wet weather, "
			"or both (pannus) )";

		case metaf::LowMidHighCloudGroup::LowLayer::
			CU_SC_NON_CUGEN_DIFFERENT_LEVELS:
		return "Cumulus and Stratocumulus not formed by spreading of Cumulus "
			"with bases at different levels";

		case metaf::LowMidHighCloudGroup::LowLayer::CB_CAP:
		return "Cumulonimbus clouds with fibrous or striated summits, "
			"often with an anvil (Cumulonimbus capillatus or "
			"Cumulonimbus capillatus incus)";

		case metaf::LowMidHighCloudGroup::LowLayer::NOT_OBSERVABLE:
		return "Clouds are not observable due to fog, blowing dust or sand, "
			"or other similar phenomena";
	}
}

std::string_view VisitorExplain::cloudMidLayerToString(
	metaf::LowMidHighCloudGroup::MidLayer midLayer)
{
	switch(midLayer) {
		case metaf::LowMidHighCloudGroup::MidLayer::NONE:
		return "No mid-layer clouds";

		case metaf::LowMidHighCloudGroup::MidLayer::AS_TR:
		return "A veil of greyish or bluish colour translucent enough "
			"to reveal the position of the Sun or Moon "
			"(Altostratus translucidus)";
		
		case metaf::LowMidHighCloudGroup::MidLayer::AS_OP_NS:
		return "A veil of a darker grey or a darker bluish grey dense enough "
			"to completely mask the Sun or Moon "
			"(Altostratus opacus or Nimbostratus)";
		
		case metaf::LowMidHighCloudGroup::MidLayer::AC_TR:
		return "Altocumulus (mackerel sky) clouds in patches or sheets at "
			"the same level or in a single layer "
			"(Altocumulus translucidus at a single level)";
		
		case metaf::LowMidHighCloudGroup::MidLayer::AC_TR_LEN_PATCHES:
		return "Patches, often lenticular (lens or almond-shaped), "
			"of Altocumulus translucidus, continually changing and "
			"occurring at one or more levels";
		
		case metaf::LowMidHighCloudGroup::MidLayer::AC_TR_AC_OP_SPREADING:
		return "Altocumulus translucidus in bands, or one or more layers of "
			"Altocumulus translucidus or Altocumulus opacus, "
			"progressively invading the sky";
		
		case metaf::LowMidHighCloudGroup::MidLayer::AC_CUGEN_AC_CBGEN:
		return "Altocumulus resulting generally from the spreading "
			"out of the summits of Cumulus; or Alcocumulus clouds "
			"acompanying Cumulonimbus (Altocumulus cumulogenitus or "
			"Altocumulus cumulonimbogenitus)";
		
		case metaf::LowMidHighCloudGroup::MidLayer::
			AC_DU_AC_OP_AC_WITH_AS_OR_NS:
		return "Altocumulus duplicatus, or Altocumulus opacus in a single "
			"layer, not progressively invading the sky, or Altocumulus "
			"with Altostratus or Nimbostratus.";
		
		case metaf::LowMidHighCloudGroup::MidLayer::AC_CAS_AC_FLO:
		return "Turrets at appear to be arranged in lines with a "
			"common horizontal base or scattered tufts with rounded and "
			"slightly bulging upper parts (Altocumulus castellanus or "
			"Altocumulus floccus)";
		
		case metaf::LowMidHighCloudGroup::MidLayer::AC_OF_CHAOTIC_SKY:
		return "Sky of chaotic, heavy and stagnant appearance, "
			"which consists of superposed, more or less broken cloud sheets "
			"of ill-defined species or varieties";

		case metaf::LowMidHighCloudGroup::MidLayer::NOT_OBSERVABLE:
		return "Clouds are not observable due to fog, blowing dust or sand, "
			"or other similar phenomena "
			"or because of a continuous layer of lower clouds";
	}
}

std::string_view VisitorExplain::cloudHighLayerToString(
	metaf::LowMidHighCloudGroup::HighLayer highLayer)
{
	switch(highLayer) {
		case metaf::LowMidHighCloudGroup::HighLayer::NONE:
		return "No high-layer clouds";

		case metaf::LowMidHighCloudGroup::HighLayer::CI_FIB_CI_UNC:
		return "Nearly straight or more or less curved filaments; more rarely, "
			"they are shaped like commas topped with either a hook or a tuft "
			"that is not rounded "
			"(Cirrus fibratus and sometimes Cirrus uncinus, not "
			"progressively invading the sky)";

		case metaf::LowMidHighCloudGroup::HighLayer::CI_SPI_CI_CAS_CI_FLO:
		return "Cirrus spissatus, in patches or entangled sheaves, "
			"that usually do not increase and sometimes appear to be "
			"the remains of the upper part of a Cumulonimbus; or "
			"Cirrus clouds with small fibrous turrets rising from common base; "
			"or more or less isolated tufts, often with trails "
			"(Cirrus spissatus or Cirrus castellanus or Cirrus floccus)";

		case metaf::LowMidHighCloudGroup::HighLayer::CI_SPI_CBGEN:
		return "Cirrus clouds originated from a Cumulonimbus cloud(s) "
			"(Cirrus spissatus cumulonimbogenitus)";

		case metaf::LowMidHighCloudGroup::HighLayer::CI_FIB_CI_UNC_SPREADING:
		return "Cirrus uncinus, Cirrus fibratus or both, "
			"progressively invading the sky; they generally thicken as a whole";

		case metaf::LowMidHighCloudGroup::HighLayer::CI_CS_LOW_ABOVE_HORIZON:
		return "Cirrus (often in bands) and Cirrostratus, "
			"or Cirrostratus alone, progressively invading the sky; "
			"they generally thicken as a whole, "
			"but the continuous veil does not reach 45&deg; above the horizon";

		case metaf::LowMidHighCloudGroup::HighLayer::CI_CS_HIGH_ABOVE_HORIZON:
		return "Cirrus (often in bands) and Cirrostratus, "
			"or Cirrostratus alone, progressively invading the sky; "
			"they generally thicken as a whole; "
			"the continuous veil extends more than 45&deg; above the horizon, "
			"without the sky being totally covered";

		case metaf::LowMidHighCloudGroup::HighLayer::
			CS_NEB_CS_FIB_COVERING_ENTIRE_SKY:
		return "Light, uniform and nebulous veil showing no distinct details"
			"or a white and fibrous veil with more "
			"or less clear-cut striations, covering the whole sky"
			"(Cirrostratus nebulosus or "
			"Cirrostratus fibratus covering the whole sky)";

		case metaf::LowMidHighCloudGroup::HighLayer::CS:
		return "A veil of Cirrostratus that is not (or no longer) "
			"invading the sky progressively "
			"and that does not completely cover the whole sky";

		case metaf::LowMidHighCloudGroup::HighLayer::CC:
		return "Cirrocumulus alone, or "
			"predominant among the high-layer clouds; "
			"when alone, its elements are frequently grouped into more or less "
			"extensive patches with very characteristic small wavelets";

		case metaf::LowMidHighCloudGroup::HighLayer::NOT_OBSERVABLE:
		return "Clouds are not observable due to fog, blowing dust or sand, "
			"or other similar phenomena "
			"or because of a continuous layer of lower clouds";
	}
}

std::string VisitorExplain::roundTo(
	float number, 
	size_t digitsAfterDecimalPoint)
{
	static const char decimalPoint = '.';
	std::string numStr = std::to_string(number);
	const auto decimalPointPos = numStr.find(decimalPoint);
	if (decimalPointPos == std::string::npos) return numStr;
	const auto decimalsAfterPoint = numStr.length() - decimalPointPos;
	if (decimalsAfterPoint > digitsAfterDecimalPoint) {
		return numStr.substr(0, decimalPointPos + digitsAfterDecimalPoint + 1);
	}
	return numStr;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef __EMSCRIPTEN__

static std::string result;

extern "C" void EMSCRIPTEN_KEEPALIVE freeMemory(){
	std::string().swap(result);
}

extern "C" const char * EMSCRIPTEN_KEEPALIVE explain(const char * input) {
	freeMemory();
	const auto parseResult = metaf::Parser::parse(std::string(input));
	VisitorExplain visitor;
	//Table header
	result += "<thead><tr><th>Group</th>";
	result += "<th>Explanation</th></tr></thead>";
	result += "<tbody>";
	//Detected report type
	result += "<tr><td>&nbsp;</td><td>";
	result += "Detected report type: ";
	result += VisitorExplain::reportTypeToString(
		parseResult.reportMetadata.type);
	result += "</td></tr>";
	//Error (if present)
	if (parseResult.reportMetadata.error != metaf::ReportError::NONE) {
		result += "<tr><td>&nbsp;</td><td>";
		result += "Parsing error: ";
		result += VisitorExplain::reportErrorToString(
			parseResult.reportMetadata.error);
		result += "</td></tr>";
	}
	//Report groups
	for (const auto & groupInfo : parseResult.groups) {
		result += "<tr><td>";
		result += groupInfo.rawString;
		result += "</td><td>";
		std::string str = visitor.visit(
			groupInfo.group, 
			groupInfo.reportPart, 
			groupInfo.rawString);
		static const std::string lineBreakN("\n");
		static const std::string lineBreakBr("<br>");
		size_t pos = str.find(lineBreakN);
		while(pos != std::string::npos) {
         str.replace(pos, lineBreakN.length(), lineBreakBr);
         pos = str.find(lineBreakN, lineBreakBr.length());
    	}
    	result += str;
		result += "</td></tr>";
	}
	//Terminate table
	result += "</tbody>";
	return result.c_str();
}

int main(int argc, char ** argv) {
	(void) argc; (void) argv;
	//Using EM_ASM_ because EM_ASM(explain()); gives a warning
	EM_ASM_(explain(), 0); 
}

#else

// If compiled without Emscripten / WASM, will decode reports from input pipe,
// one METAR, SPECI or TAF report per line 

static const auto reportDelimiter = std::string(80, '='); 
static const auto groupDelimiter = std::string(80, '-'); 

int main(int argc, char ** argv) {
	(void) argc; (void) argv;
	std::string report;
	while (getline(std::cin, report)) {
		const auto parseResult = metaf::Parser::parse(report);
		std::cout << reportDelimiter << '\n';
		std::cout << report << '\n';
		std::cout << groupDelimiter << '\n';
		std::cout << "Report type\n" << VisitorExplain::reportTypeToString(
			parseResult.reportMetadata.type) << '\n';
		if (parseResult.reportMetadata.error != metaf::ReportError::NONE) {
			std::cout << groupDelimiter << '\n';
			std::cout << "Parsing error\n" << VisitorExplain::reportErrorToString(
				parseResult.reportMetadata.error) << '\n';
		}
		VisitorExplain visitor;
		for (const auto & groupInfo : parseResult.groups) {
			std::cout << groupDelimiter << '\n';
			std::cout << groupInfo.rawString << '\n';
			std::cout << visitor.visit(
				groupInfo.group, 
				groupInfo.reportPart, 
				groupInfo.rawString);
			std::cout << '\n';
		}
	}
	std::cout << reportDelimiter << '\n';
}

#endif

///////////////////////////////////////////////////////////////////////////////
