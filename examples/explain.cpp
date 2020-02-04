/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
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
#include <emscripten/emscripten.h>

using namespace std::literals;

class VisitorExplain : public metaf::Visitor<std::string> {
public:
	static std::string_view reportTypeToString(metaf::ReportType reportType);
	static std::string_view reportErrorToString(metaf::ReportError reportError);

private:
	static const inline std::string lineBreak = "<br>"s;
	static const inline std::string groupNotValidMessage = 
		"Data in this group may be errorneous, incomplete or inconsistent"s;

	virtual std::string visitFixedGroup(const metaf::FixedGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitLocationGroup(const metaf::LocationGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitReportTimeGroup(const metaf::ReportTimeGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitTrendGroup(const metaf::TrendGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitWindGroup(const metaf::WindGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitVisibilityGroup(const metaf::VisibilityGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitCloudGroup(const metaf::CloudGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitWeatherGroup(const metaf::WeatherGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitTemperatureGroup(const metaf::TemperatureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitTemperatureForecastGroup(
		const metaf::TemperatureForecastGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitPressureGroup(const metaf::PressureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitRunwayStateGroup(const metaf::RunwayStateGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString);
	virtual std::string visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group,
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
	virtual std::string visitCloudLayersGroup(
		const metaf::CloudLayersGroup & group,
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

	static std::string explainRunway(const metaf::Runway & runway);
	static std::string explainMetafTime(const metaf::MetafTime & metafTime);
	static std::string explainTemperature(const metaf::Temperature & temperature);
	static std::string explainSpeed(const metaf::Speed & speed);
	static std::string explainDistance(const metaf::Distance & distance);
	static std::string explainDirection(const metaf::Direction & direction,
		bool trueCardinalDirections = false);
	static std::string explainPressure(const metaf::Pressure & pressure);
	static std::string explainPrecipitation(const metaf::Precipitation & precipitation);
	static std::string explainSurfaceFriction(const metaf::SurfaceFriction & surfaceFriction);
	static std::string explainWaveHeight(const metaf::WaveHeight & waveHeight);
	static std::string explainWeatherPhenomena(const metaf::WeatherPhenomena & weatherPhenomena);
	static std::string explainDirectionSector(const std::vector<metaf::Direction> dir);

	static std::string_view speedUnitToString(metaf::Speed::Unit unit);
	static std::string_view distanceUnitToString(metaf::Distance::Unit unit);
	static std::string_view distanceMilesFractionToString(metaf::Distance::MilesFraction f);
	static std::string_view cardinalDirectionToString(metaf::Direction::Cardinal cardinal);
	static std::string_view brakingActionToString(
		metaf::SurfaceFriction::BrakingAction brakingAction);
	static std::string_view stateOfSeaSurfaceToString(
		metaf::WaveHeight::StateOfSurface stateOfSurface);
	static std::string_view trendTypeToString(metaf::TrendGroup::Type type);
	static std::string_view cloudAmountToString(metaf::CloudGroup::Amount amount);
	static std::string_view cloudTypeToString(metaf::CloudGroup::CloudType type);
	static std::string_view weatherPhenomenaQualifierToString(metaf::WeatherPhenomena::Qualifier qualifier);
	static std::string_view weatherPhenomenaDescriptorToString(metaf::WeatherPhenomena::Descriptor descriptor);
	static std::string_view weatherPhenomenaWeatherToString(metaf::WeatherPhenomena::Weather weather);
	static std::string_view specialWeatherPhenomenaToString(const metaf::WeatherPhenomena & wp);
	static std::string_view visTrendToString(metaf::VisibilityGroup::Trend trend);
	static std::string_view runwayStateDepositsToString(metaf::RunwayStateGroup::Deposits deposits);
	static std::string_view runwayStateExtentToString(metaf::RunwayStateGroup::Extent extent);
	static std::string_view precipitationGroupTypeToString(
		metaf::PrecipitationGroup::Type type);
	static std::string_view layerForecastGroupTypeToString(
		metaf::LayerForecastGroup::Type type);
	static std::string_view pressureTendencyTypeToString(
		metaf::PressureTendencyGroup::Type type);
	static std::string_view pressureTendencyTrendToString(
		metaf::PressureTendencyGroup::Trend trend);
	static std::string_view cloudTypeToString(
		metaf::CloudTypesGroup::Type type);
	static std::string_view cloudLowLayerToString(
		metaf::CloudLayersGroup::LowLayer lowLayer);
	static std::string_view cloudMidLayerToString(
		metaf::CloudLayersGroup::MidLayer midLayer);
	static std::string_view cloudHighLayerToString(
		metaf::CloudLayersGroup::HighLayer highLayer);

	static std::string roundTo(float number, size_t digitsAfterDecimalPoint);
};

std::string VisitorExplain::visitFixedGroup(const metaf::FixedGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch (group.type()) {
		case metaf::FixedGroup::Type::METAR:
		result << "Report type: METAR (weather observation report)";
		break;

		case metaf::FixedGroup::Type::SPECI:
		result << "Report type: unscheduled METAR (weather observation report)" << lineBreak;
		result << "Unscheduled report is issued dut to sudden changes in weather ";
		result << "conditions: wind shift, visibility decrease, severe weather, ";
		result << "clouds formed or dissipated, etc.";
		break;

		case metaf::FixedGroup::Type::TAF:
		result << "Report type: TAF (terminal aerodrome forecast)";
		break;

		case metaf::FixedGroup::Type::AMD:
		result << "Amended report";
		break;

		case metaf::FixedGroup::Type::NIL:
		result << "Missing report";
		break;

		case metaf::FixedGroup::Type::CNL:
		result << "Cancelled report";
		break;

		case metaf::FixedGroup::Type::COR:
		result << "Correctional report";
		break;

		case metaf::FixedGroup::Type::AUTO:
		result << "Fully automated report with no human intervention or oversight";
		break;

		case metaf::FixedGroup::Type::NSW:
		result << "No significant weather" << lineBreak;
		result << "Indicates end of significant weather phenomena";
		break;

		case metaf::FixedGroup::Type::CAVOK:
		result << "Ceiling and visibility OK" << lineBreak;
		result << "Visibility 10 km or more in all directions, ";
		result << "no cloud below 5000 feet (1500 meters), ";
		result << "no cumulonimbus or towering cumulus clouds, ";
		result << "no significant weather phenomena";
		break;

		case metaf::FixedGroup::Type::RMK:
		result << "The remarks are as follows" << lineBreak;
		break;

		case metaf::FixedGroup::Type::MAINTENANCE_INDICATOR:
		result << "Automated station requires maintenance";
		break;

		case metaf::FixedGroup::Type::AO1:
		result << "This automated station is not equipped with a precipitation discriminator";
		break;

		case metaf::FixedGroup::Type::AO2:
		result << "This automated station is equipped with a precipitation discriminator";
		break;

		case metaf::FixedGroup::Type::AO1A:
		result << "This automated station is not equipped with a precipitation discriminator";
		result << lineBreak;
		result << "Automated observation is augmented by a human observer";
		break;

		case metaf::FixedGroup::Type::AO2A:
		result << "This automated station is equipped with a precipitation discriminator";
		result << lineBreak;
		result << "Automated observation is augmented by a human observer";
		break;

		case metaf::FixedGroup::Type::NOSPECI:
		result << "This manual station does not issue SPECI (unscheduled) reports";
		break;

		case metaf::FixedGroup::Type::PWINO:
		result << "This automated station is equipped with present weather identifier ";
		result << "and this sensor is not operating";
		break;

		case metaf::FixedGroup::Type::TSNO:
		result << "This automated station is equipped with lightning detector ";
		result << "and this sensor is not operating";
		break;

		case metaf::FixedGroup::Type::SLPNO:
		result << "Mean sea-level pressure information is not available";
		break;

		case metaf::FixedGroup::Type::FROIN:
		result << "Frost on the instrument (e.g. due to freezing fog depositing rime).";
		break;

		case metaf::FixedGroup::Type::PRES_MISG:
		result << "Atmospheric pressure (altimeter) data is missing";
		break;

		case metaf::FixedGroup::Type::T_MISG:
		result << "Temperature data is missing";
		break;

		case metaf::FixedGroup::Type::TD_MISG:
		result << "Dew point data is missing";
		break;

		case metaf::FixedGroup::Type::WX_MISG:
		result << "Weather phenomena data is missing";
		break;

		case metaf::FixedGroup::Type::TS_LTNG_TEMPO_UNAVBL:
		result << "Thunderstorm / lightning data is missing";
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitLocationGroup(const metaf::LocationGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "ICAO code for location: " << group.toString();
	return result.str();
}

std::string VisitorExplain::visitReportTimeGroup(
	const metaf::ReportTimeGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Day and time of report issue: " << explainMetafTime(group.time());
	return result.str();
}

std::string VisitorExplain::visitTrendGroup(const metaf::TrendGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << trendTypeToString(group.type()) << lineBreak;

	switch (group.probability()) {
		case metaf::TrendGroup::Probability::PROB_30:
		result << "Trend probability is 30 percent" << lineBreak;;
		break;

		case metaf::TrendGroup::Probability::PROB_40:
		result << "Trend probability is 40 percent" << lineBreak;;
		break;

		case metaf::TrendGroup::Probability::NONE:
		break;
	}
	if (const auto timeFrom = group.timeFrom(); timeFrom) {
		result << "From " << explainMetafTime(*timeFrom) << lineBreak;
	}
	if (const auto timeTill = group.timeTill(); timeTill) {
		result << "Until " << explainMetafTime(*timeTill) << lineBreak;
	}
	if (const auto timeAt = group.timeAt(); timeAt) {
		result << "At " << explainMetafTime(*timeAt) << lineBreak;
	}
	return result.str();
}

std::string VisitorExplain::visitWindGroup(const metaf::WindGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	
	const auto varSectorEndStr = explainDirection(group.varSectorEnd());

	switch (group.type()) {
		case metaf::WindGroup::Type::SURFACE_WIND_CALM:
		result << "No wind";
		return result.str();

		case metaf::WindGroup::Type::SURFACE_WIND:
		result << "Surface wind" << lineBreak;
		result << "Wind direction: " << explainDirection(group.direction(), true);
		result << lineBreak;
		result << "Wind speed: " << explainSpeed(group.windSpeed()) << lineBreak;
		if (group.gustSpeed().isReported()) 
			result << "Gust speed: " << explainSpeed(group.gustSpeed()) << lineBreak;
		break;

		case metaf::WindGroup::Type::VARIABLE_WIND_SECTOR:
		result << "Variable wind sector" << lineBreak;
		result << "Variable wind direction sector from ";
		result << explainDirection(group.varSectorBegin()); 
		result << " clockwise to ";
		result << explainDirection(group.varSectorEnd());
		break;

		case metaf::WindGroup::Type::SURFACE_WIND_WITH_VARIABLE_SECTOR:
		result << "Surface wind" << lineBreak;
		result << "Wind direction: " << explainDirection(group.direction(), true);
		result << lineBreak;
		result << "Wind speed: " << explainSpeed(group.windSpeed()) << lineBreak;
		if (group.gustSpeed().isReported()) 
			result << "Gust speed: " << explainSpeed(group.gustSpeed()) << lineBreak;
		result << "Variable wind direction sector from ";
		result << explainDirection(group.varSectorBegin()); 
		result << " clockwise to ";
		result << explainDirection(group.varSectorEnd());
		break;

		case metaf::WindGroup::Type::WIND_SHEAR:
		result << "Wind shear at height ";
		result << explainDistance(group.height()) << ':';
		result << "Wind direction: " << explainDirection(group.direction(), true) << lineBreak;
		result << "Wind speed: " << explainSpeed(group.windSpeed());
		if (group.gustSpeed().isReported()) 
			result  << lineBreak << "Gust speed: " << explainSpeed(group.gustSpeed());
		break;

		case metaf::WindGroup::Type::WIND_SHIFT:		
		case metaf::WindGroup::Type::WIND_SHIFT_FROPA:
		result << "Wind direction changed 45&deg; or more in less than 15 minutes ";
		result << "with sustained wind speed of 10 knots";
		result << " (5.1 m/s / 18.5 km/h / 11.5 mph)";
		if (group.eventTime().has_value()) {
			result << lineBreak << "Wind shift began at ";
			result << explainMetafTime(*group.eventTime());
		}
		if (group.type() == metaf::WindGroup::Type::WIND_SHIFT_FROPA) {
			result << lineBreak;
			result << "This wind shift is associated with weather front passage";
		}
		break;

		case metaf::WindGroup::Type::PEAK_WIND:
		result << "Peak wind ";
		if (group.eventTime().has_value()) {
			result << "was observed at ";
			result << explainMetafTime(*group.eventTime());
		}
		break;

		case metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS:
		result << "Wind shear significant to aircraft operations ";
		result << "is present along the take-off path or approach path ";
		result << "between runway level and 500 metres (1 600 ft)";
		if (const auto rw = group.runway(); rw.has_value()) {
		result << " at " << explainRunway(*rw);
		}
		break;

		case metaf::WindGroup::Type::WSCONDS:
		result << "Potential wind shear conditions are present ";
		result << "but there's not enough information to reliably forecast ";
		result << "height, direction and speed of wind shear";
		break;

		case metaf::WindGroup::Type::WND_MISG:
		result << "Wind data is missing";
		break;
	}
	return result.str();
}

std::string VisitorExplain::visitVisibilityGroup(const metaf::VisibilityGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch (group.type()) {
		case metaf::VisibilityGroup::Type::PREVAILING:
		result << "Prevailing visibility is ";
		result << explainDistance(group.visibility());
		break;

		case metaf::VisibilityGroup::Type::PREVAILING_NDV:
		result << "Prevailing visibility is "; 
		result << explainDistance(group.visibility());
		result << lineBreak;
		result << "This station cannot differentiate the directional ";
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
		result << explainDistance(group.visibility()) << lineBreak;
		result << "In the following directions: ";
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
		result << explainDistance(group.maxVisibility()) << lineBreak;
		result << "In the following directions: ";
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
	}
	return result.str();
}

std::string VisitorExplain::visitCloudGroup(const metaf::CloudGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;

	switch (group.type()) {
		case metaf::CloudGroup::Type::NO_CLOUDS:
		result << cloudAmountToString(group.amount());
		break;

		case metaf::CloudGroup::Type::CLOUD_LAYER:
		result << "Cloud layer" << lineBreak;
		result << cloudAmountToString(group.amount()) << lineBreak;
		if (group.cloudType() != metaf::CloudGroup::CloudType::NONE) {
			result << "Convective type: " << cloudTypeToString(group.cloudType());
			result << lineBreak;
		}
		result << "Base height " << explainDistance(group.height());
		break;

		case metaf::CloudGroup::Type::VERTICAL_VISIBILITY:
		result << "Sky is obscured, vertical visibility ";
		result << explainDistance(group.verticalVisibility());
		break;

		case metaf::CloudGroup::Type::CEILING:
		result << "Ceiling height " << explainDistance(group.height());
		break;

		case metaf::CloudGroup::Type::VARIABLE_CEILING:
		result << "Ceiling height is variable between ";
		result << explainDistance(group.minHeight());
		result << " and ";
		result << explainDistance(group.maxHeight());
		break;

		case metaf::CloudGroup::Type::CHINO:
		result << "Ceiling data not awailable";
		break;

		case metaf::CloudGroup::Type::CLD_MISG:
		result << "Sky condition data (cloud data) is missing";
		break;
	}

	switch (group.type()) {
		case metaf::CloudGroup::Type::CEILING:
		case metaf::CloudGroup::Type::VARIABLE_CEILING:
		case metaf::CloudGroup::Type::CHINO:
		if (const auto rw = group.runway(); rw.has_value()) {
			result << " at " << explainRunway(*rw);
		}
		if (const auto d = group.direction(); d.has_value()) {
			result << " towards " << explainDirection(*d);
		}
		break;

		default: break;
	}

	return result.str();
}

std::string VisitorExplain::visitWeatherGroup(const metaf::WeatherGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch (group.type()) {
		case metaf::WeatherGroup::Type::CURRENT: 
		result << "Weather phenomena:";
		break;
		
		case metaf::WeatherGroup::Type::RECENT: 
		result << "Recent weather:"; break;
		
		case metaf::WeatherGroup::Type::EVENT: 
		result << "Precipitation beginning/ending time:"; break;
	}
	result << lineBreak;

	const auto phenomena = group.weatherPhenomena();
	for (const auto p : phenomena) {
		result << explainWeatherPhenomena(p) << lineBreak;
	}
	return result.str();
}

std::string VisitorExplain::visitTemperatureGroup(const metaf::TemperatureGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Air temperature: " << explainTemperature(group.airTemperature()) << lineBreak;
	result << "Dew point: " << explainTemperature(group.dewPoint()) << lineBreak;
	if (const auto rh = group.relativeHumidity(); rh.has_value()) {
			result << "Relative humidity: " << static_cast<int>(*rh) << " percent";
			result << lineBreak; 
	}
	if (const auto heatIndex = metaf::Temperature::heatIndex(
		group.airTemperature(), 
		group.dewPoint());
		heatIndex.temperature().has_value()) {
			result << "Heat index: " << explainTemperature(heatIndex) << lineBreak;
	}
	return result.str();
}

std::string VisitorExplain::visitTemperatureForecastGroup(
	const metaf::TemperatureForecastGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch(group.point()) {
		case metaf::TemperatureForecastGroup::Point::NOT_SPECIFIED:
		result << "Temperature";
		break;

		case metaf::TemperatureForecastGroup::Point::MINIMUM: 
		result << "Minimum temperature";
		break;

		case metaf::TemperatureForecastGroup::Point::MAXIMUM:
		result << "Maximum temperature";
		break;
	}
	result << " " << explainTemperature(group.airTemperature()) << ", ";
	result << "expected on " << explainMetafTime(group.time());
	return result.str();
}

std::string VisitorExplain::visitPressureGroup(const metaf::PressureGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch(group.type()) {
		case metaf::PressureGroup::Type::OBSERVED_QNH:
		result << "Observed mean atmospheric pressure (normalised to sea level)";
		break;

		case metaf::PressureGroup::Type::FORECAST_LOWEST_QNH:
		result << "Forecast lowest sea level pressure";
		break;

		case metaf::PressureGroup::Type::OBSERVED_QFE:
		result << "Observed actual atmospheric pressure";
		break;
	}
	result << ": " << explainPressure(group.atmosphericPressure());
	return result.str();
}

std::string VisitorExplain::visitRunwayStateGroup(const metaf::RunwayStateGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "State of " << explainRunway(group.runway()) << ": ";
	switch (group.type()) {
		case metaf::RunwayStateGroup::Type::RUNWAY_STATE:
		result << runwayStateDepositsToString(group.deposits()) << lineBreak;
		if (group.deposits() != metaf::RunwayStateGroup::Deposits::CLEAR_AND_DRY) {
		    result << "Depth of deposits on runway: ";
		    result << explainPrecipitation(group.depositDepth()) << lineBreak;
			result << "Runway contamination extent: ";
			result << runwayStateExtentToString(group.contaminationExtent()) << lineBreak;
		}
		result << "Surface friction: ";
		result << explainSurfaceFriction(group.surfaceFriction());
		break;

		case metaf::RunwayStateGroup::Type::RUNWAY_CLRD:
		result << "deposits on runway were cleared or ceased to exist" << lineBreak;
		result << "Surface friction: ";
		result << explainSurfaceFriction(group.surfaceFriction());
		break;

		case metaf::RunwayStateGroup::Type::RUNWAY_SNOCLO:
		result << "closed due to snow accumulation";
		break;

		case metaf::RunwayStateGroup::Type::AERODROME_SNOCLO:
		result << "aerodrome closed due to snow accumulation";
		break;

		case metaf::RunwayStateGroup::Type::RUNWAY_NOT_OPERATIONAL:
		result << "runway is not operational";
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
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Sea surface temperature " << explainTemperature(group.surfaceTemperature()) << ", ";
	result << explainWaveHeight(group.waves());
	return result.str();
}

std::string VisitorExplain::visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	std::string periodStr;
	switch(group.observationPeriod()) {
		case metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS6:
		periodStr = "6-hourly";
		break;

		case metaf::MinMaxTemperatureGroup::ObservationPeriod::HOURS24:
		periodStr = "24-hourly";
		break;
	}
	result << periodStr << " " << " minimum / maximum temperature";
	if (!group.minimum().temperature().has_value() && 
		!group.maximum().temperature().has_value()) {
			result << " is not reported";
			return result.str();
	}
	result << lineBreak;
	if (group.minimum().temperature().has_value()) {
		result << "Minimum temperature ";
		result << explainTemperature(group.minimum()) << lineBreak;
	}
	if (group.maximum().temperature().has_value()) {
		result << "Maximum temperature ";
		result << explainTemperature(group.maximum()) << lineBreak;
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
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
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
		result << "Water equivalent of frozen precipitation for the last 3 hours: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_6_HOURLY:
		result << "Water equivalent of frozen precipitation for the last 6 hours: ";
		result << explainPrecipitation(group.total());
		break;
	
		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_24_HOURLY:
		result << "Water equivalent of frozen precipitation for the last 24 hours: ";
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

		case metaf::PrecipitationGroup::Type::PRECIPITATION_ACCUMULATION_SINCE_LAST_REPORT:
		result << "Precipitation accumulation since last report: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::SNOW_INCREASING_RAPIDLY:
		result << "Snow increasing rapidly" << lineBreak;
		result << "For the last hour snow increased by ";
		result << explainPrecipitation(group.recent()) << lineBreak;
		result << "Total snowfall: ";
		result << explainPrecipitation(group.total());
		break;

		case metaf::PrecipitationGroup::Type::RAINFALL_9AM_10MIN:
		result << "Rainfall for the last 10 minutes before report release time: ";
		result << explainPrecipitation(group.recent()) << lineBreak;
		result << "Rainfall since 9AM (9:00) local time: ";
		result << explainPrecipitation(group.total()) << lineBreak;
		break;

		case metaf::PrecipitationGroup::Type::PNO:
		result << "This automated station is equipped with tipping bucket rain gauge ";
		result << "and this sensor is not operating";
		break;

		case metaf::PrecipitationGroup::Type::FZRANO:
		result << "This automated station is equipped with freezing rain sensor ";
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
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << layerForecastGroupTypeToString(group.type());
	result << " at heights from " << explainDistance(group.baseHeight());
	result << " to " << explainDistance(group.topHeight());
	return result.str();	
}

std::string VisitorExplain::visitPressureTendencyGroup(
	const metaf::PressureTendencyGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	if (group.type() == metaf::PressureTendencyGroup::Type::FALLING_RAPIDLY) {
		return "Pressure is rapidly falling at a rate of at least 0.06 inch of mercury "
			"(2.03 hectopascal) per hour and the pressure change totals "
			"0.02 inch of mercury (0.68 hectopascal) or more "
			"at the time of the observation";
	}
	if (group.type() == metaf::PressureTendencyGroup::Type::RISING_RAPIDLY) {
		return "Pressure is rapidly rising at a rate of at least 0.06 inch of mercury "
			"(2.03 hectopascal) per hour and the pressure change totals "
			"0.02 inch of mercury (0.68 hectopascal) or more "
			"at the time of the observation";
	}
	if (group.type() != metaf::PressureTendencyGroup::Type::NOT_REPORTED) {
		result << "During last 3 hours the atmospheric pressure was ";
		result << pressureTendencyTypeToString (group.type()) << lineBreak;
		result << "Now pressure is ";
		result << pressureTendencyTrendToString(
			metaf::PressureTendencyGroup::trend(group.type()));
		result << " 3 hours ago";
	} else {
		result << "3-hourly pressure tendency is not reported";
	}
	result << ", absolute pressure change is ";
	result << explainPressure(group.difference());
	return result.str();
}

std::string VisitorExplain::visitCloudTypesGroup(
	const metaf::CloudTypesGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	const auto clouds = group.toVector();
	for (auto i = 0u; i < clouds.size(); i++) {
		if (i == 0u) {
			result << "Lowest cloud layer or obscuration: ";
		} else {
			result << "Next cloud layer: ";
		}
		result << cloudTypeToString(std::get<metaf::CloudTypesGroup::Type>(clouds.at(i)));
		result << ", covering " << std::get<unsigned int>(clouds.at(i)) << "/8 of sky";
		result << lineBreak;
	}
	if (group.baseHeight().isReported()) {
		result << "Base height is ";
		result << explainDistance(group.baseHeight());
	}
	return result.str();
}

std::string VisitorExplain::visitCloudLayersGroup(
	const metaf::CloudLayersGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Low cloud layer: " << lineBreak;
	result << cloudLowLayerToString(group.lowLayer()) << lineBreak;
	result << "Mid cloud layer: " << lineBreak;
	result << cloudMidLayerToString(group.midLayer()) << lineBreak;
	result << "High cloud layer: " << lineBreak;
	result << cloudHighLayerToString(group.highLayer()) << lineBreak;
	return result.str();
}

std::string VisitorExplain::visitLightningGroup(const metaf::LightningGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;

	result << "Lightning strikes observed";
	if (group.distance().isReported())
	{
		result << " at distance " << explainDistance(group.distance());
	}
	result << lineBreak;

	switch(group.frequency()) {
		case metaf::LightningGroup::Frequency::NONE:
		break;

		case metaf::LightningGroup::Frequency::OCCASSIONAL:
		result << "Less than 1 strike per minute" << lineBreak;
		break;

		case metaf::LightningGroup::Frequency::FREQUENT:
		result << "1 to 6 strikes per minute" << lineBreak;
		break;

		case metaf::LightningGroup::Frequency::CONSTANT:
		result << "More than 6 strikes per minute" << lineBreak;
		break;
	}

	if (group.isCloudGround() || 
	 	group.isInCloud() ||
		group.isCloudCloud() ||
		group.isCloudAir()) 
	{
		result << "The following lightning types are observed: ";
	}
	if (group.isCloudGround()) {
		result << "cloud-to-ground";
		if (group.isInCloud() || group.isCloudCloud() || group.isCloudAir()) result << ", ";
	}
	if (group.isInCloud()) {
		result << "in-cloud";
		if (group.isCloudCloud() || group.isCloudAir()) result << ", ";
	}
	if (group.isCloudCloud()) {
		result << "cloud-to-cloud";
		if (group.isCloudAir()) result << ", ";
	}
	if (group.isCloudAir()) {
		result << "cloud-to-air (without strike to ground)";
	}
	if (group.isCloudGround() || 
	 	group.isInCloud() ||
		group.isCloudCloud() ||
		group.isCloudAir()) 
	{
		result << lineBreak;
	}

	if (group.isUnknownType()) 
	{
		result << "Some lightning types specified in this group ";
		result << "were not recognised by parser" << lineBreak; 
	}

	const auto directions = group.directions();
	if (directions.size()) {
		result << "Lightning strikes observed in the following directions: ";
		result << explainDirectionSector(group.directions());
	}
	return result.str();
}

std::string VisitorExplain::visitVicinityGroup(const metaf::VicinityGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch (group.type()) {
		case metaf::VicinityGroup::Type::THUNDERSTORM:
		result << "Thunderstorm"; break;

		case metaf::VicinityGroup::Type::CUMULONIMBUS:
		result << "Cumulonimbus cloud(s)"; break;

		case metaf::VicinityGroup::Type::CUMULONIMBUS_MAMMATUS:
		result << "Cumulonimbus cloud(s) with mammatus"; break;

		case metaf::VicinityGroup::Type::TOWERING_CUMULUS:
		result << "Towering cumulus cloud(s)"; break;

		case metaf::VicinityGroup::Type::ALTOCUMULUS_CASTELLANUS:
		result << "Altocumulus cloud(s)"; break;

		case metaf::VicinityGroup::Type::STRATOCUMULUS_STANDING_LENTICULAR:
		result << "Stratocumulus standing lenticular cloud(s)"; break;

		case metaf::VicinityGroup::Type::ALTOCUMULUS_STANDING_LENTICULAR:
		result << "Altocumulus standing lenticular cloud(s)"; break;

		case metaf::VicinityGroup::Type::CIRROCUMULUS_STANDING_LENTICULAR:
		result << "Cirrocumulus standing lenticular cloud(s)"; break;

		case metaf::VicinityGroup::Type::ROTOR_CLOUD:
		result << "Rotor cloud(s)"; break;

		case metaf::VicinityGroup::Type::VIRGA:
		result << "Virga"; break;

		case metaf::VicinityGroup::Type::PRECIPITATION_IN_VICINITY:
		result << "Precipitation"; break;

		case metaf::VicinityGroup::Type::FOG:
		result << "Fog"; break;

		case metaf::VicinityGroup::Type::FOG_SHALLOW:
		result << "Shallow fog"; break;

		case metaf::VicinityGroup::Type::FOG_PATCHES:
		result << "Patches of fog"; break;

		case metaf::VicinityGroup::Type::HAZE:
		result << "Haze"; break;

		case metaf::VicinityGroup::Type::SMOKE:
		result << "Smoke"; break;

		case metaf::VicinityGroup::Type::BLOWING_SNOW:
		result << "Blowing snow"; break;

		case metaf::VicinityGroup::Type::BLOWING_SAND:
		result << "Blowing sand"; break;

		case metaf::VicinityGroup::Type::BLOWING_DUST:
		result << "Blowing dust"; break;

	}
	result << " observed";
	if (group.distance().isReported())
	{
		result << " at distance " << explainDistance(group.distance());
	}
	result << lineBreak;

	
	if (const auto directions = group.directions(); directions.size()) {
		result << "Observed in the following directions: ";
		result << explainDirectionSector(directions);
		result << lineBreak;
	}

	if (group.movingDirection().isReported()) {
		result << "Moving towards ";
		result << cardinalDirectionToString(group.movingDirection().cardinal());
		result << lineBreak;
	}

	return result.str();

}

std::string VisitorExplain::visitMiscGroup(const metaf::MiscGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart; (void)rawString;
	std::ostringstream result;
	static const std::string colourCodeBlack = "Colour code BLACK: "
		"aerodrome closed due to snow accumulation or non-weather reasons";

	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch (group.type()) {
		case metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES:
		if (const auto duration = group.data(); duration.has_value()) {
			if (! *duration) {
				result << "No sunshine occurred the previous calendar day";
			} else {
				result << "Duration of sunshine ";
				result << "that occurred the previous calendar day is ";
				result << *duration << " minutes";
			}
		}
		break;

		case metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION:
		if (const auto correctionNo = group.data(); correctionNo.has_value()) {
			result << "This report is the corrected weather observation, ";
			result << "correction number is " << static_cast<int>(*correctionNo);
		}
		break;

		case metaf::MiscGroup::Type::DENSITY_ALTITUDE:
		result << "Density altitude ";
		if (!group.data().has_value()) { result << "data missing"; break; }
		result << " is " << *group.data() << " feet";
		break;

		case metaf::MiscGroup::Type::HAILSTONE_SIZE:
		result << "Largest hailstone size ";
		if (!group.data().has_value()) { result << "data missing"; break; }
		result << " is " << *group.data() << " inches";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKBLUE:
		result << colourCodeBlack << lineBreak;
		case metaf::MiscGroup::Type::COLOUR_CODE_BLUE:
		result << "Colour code BLUE: ";
		result << "visibility &gt;8000 m and lowest cloud base height &gt;2500 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKWHITE:
		result << colourCodeBlack << lineBreak;
		case metaf::MiscGroup::Type::COLOUR_CODE_WHITE:
		result << "Colour code WHITE: ";
		result << "visibility &gt;5000 m and lowest cloud base height &gt;1500 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKGREEN:
		result << colourCodeBlack << lineBreak;
		case metaf::MiscGroup::Type::COLOUR_CODE_GREEN:
		result << "Colour code GREEN: ";
		result << "visibility &gt;3700 m and lowest cloud base height &gt;700 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW1:
		result << colourCodeBlack << lineBreak;
		case metaf::MiscGroup::Type::COLOUR_CODE_YELLOW1:
		result << "Colour code YELLOW 1: ";
		result << "visibility &gt;2500 m and lowest cloud base height &gt;500 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW2:
		result << colourCodeBlack << lineBreak;
		case metaf::MiscGroup::Type::COLOUR_CODE_YELLOW2:
		result << "Colour code YELLOW 2: ";
		result << "visibility &gt;1600 m and lowest cloud base height &gt;300 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKAMBER:
		result << colourCodeBlack << lineBreak;
		case metaf::MiscGroup::Type::COLOUR_CODE_AMBER:
		result << "Colour code AMBER: ";
		result << "visibility &gt;800 m and lowest cloud base height &gt;200 ft";
		break;

		case metaf::MiscGroup::Type::COLOUR_CODE_BLACKRED:
		result << colourCodeBlack << lineBreak;
		case metaf::MiscGroup::Type::COLOUR_CODE_RED:
		result << "Colour code RED: ";
		result << "either visibility &lt;800 m or ";
		result << "lowest cloud base height &lt;200 ft ";
		result << "or both";
		break;
	}
	return result.str();
}


std::string VisitorExplain::visitUnknownGroup(const metaf::UnknownGroup & group,
	metaf::ReportPart reportPart,
	const std::string & rawString)
{
	(void)reportPart;
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Not recognised by parser: " << rawString;
	return result.str();
}

std::string_view VisitorExplain::reportTypeToString(metaf::ReportType reportType) {
	switch (reportType) {
		case metaf::ReportType::UNKNOWN:	return "unable to detect";
		case metaf::ReportType::METAR:		return "METAR (weather observation)";
		case metaf::ReportType::TAF:		return "TAF (weather forecast)";
	}
}

std::string_view VisitorExplain::reportErrorToString(metaf::ReportError reportError) {
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
	if (runway.isMessageRepetition()) return "same runway (repetition of last message)"; 
	std::ostringstream result;
	result << "runway ";
	result << (runway.number() < 10 ? "0"s : ""s) << std::to_string(runway.number());
	std::string designatorStr;
	switch(runway.designator()) {
		case metaf::Runway::Designator::NONE: 	break;
		case metaf::Runway::Designator::LEFT: 	designatorStr = "LEFT"; break;
		case metaf::Runway::Designator::CENTER:	designatorStr = "CENTER"; break;
		case metaf::Runway::Designator::RIGHT:  designatorStr = "RIGHT"; break;
	}
	if (!designatorStr.empty()) result << " " << designatorStr;
	return result.str();
}

std::string VisitorExplain::explainMetafTime(const metaf::MetafTime & metafTime) {
	static const std::string hourMinuteSeparator(":");
	std::ostringstream result;
	if (const auto day = metafTime.day(); day.has_value()) {
		result << "day " << *day << ", ";
	}
	result << (metafTime.hour() < 10 ? "0"s : ""s) << metafTime.hour();
	result << hourMinuteSeparator;
	result << (metafTime.minute() < 10 ? "0"s : ""s) << metafTime.minute();
	result << " GMT";
	return result.str();
}

std::string VisitorExplain::explainTemperature(const metaf::Temperature & temperature) {
	if (!temperature.temperature().has_value()) return "not reported";
	std::ostringstream result;
	if (!(*temperature.temperature()) && !temperature.isPrecise()) {
		if (temperature.isFreezing()) result << "slightly less than ";
		if (!temperature.isFreezing()) result << "slightly more than ";
	}
	if (const auto t = temperature.toUnit(metaf::Temperature::Unit::C); t.has_value()) {
		result << roundTo(*t, 1) << " &deg;C";
	} else {
		result << "[unable to convert temperature to &deg;C]";
	}
	result << " / ";
	if (const auto t = temperature.toUnit(metaf::Temperature::Unit::F); t.has_value()) {
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
		if (const auto s = speed.toUnit(metaf::Speed::Unit::KNOTS); s.has_value()) {
			result << roundTo(*s, 1);
			result << " " << speedUnitToString(metaf::Speed::Unit::KNOTS);
		} else {
			result << "[unable to convert speed to knots]";
		}
		result << " / ";
	}
	if (speed.unit() != metaf::Speed::Unit::METERS_PER_SECOND) {
		if (const auto s = speed.toUnit(metaf::Speed::Unit::METERS_PER_SECOND); s.has_value()) {
			result << roundTo(*s, 1);
			result << " " << speedUnitToString(metaf::Speed::Unit::METERS_PER_SECOND);
		} else {
			result << "[unable to convert speed to m/s]";
		}
		result << " / ";
	}
	if (speed.unit() != metaf::Speed::Unit::KILOMETERS_PER_HOUR) {
		if (const auto s = speed.toUnit(metaf::Speed::Unit::KILOMETERS_PER_HOUR); s.has_value()) {
			result << roundTo(*s, 1);
			result << " " << speedUnitToString(metaf::Speed::Unit::KILOMETERS_PER_HOUR);
		} else {
			result << "[unable to convert speed to km/h]";
		}
		if (speed.unit() != metaf::Speed::Unit::MILES_PER_HOUR) result << " / ";
	}
	if (speed.unit() != metaf::Speed::Unit::MILES_PER_HOUR) {
		if (const auto s = speed.toUnit(metaf::Speed::Unit::MILES_PER_HOUR); s.has_value()) {
			result << roundTo(*s, 1);
			result << " " << speedUnitToString(metaf::Speed::Unit::MILES_PER_HOUR);
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
		const auto fraction = std::get<metaf::Distance::MilesFraction>(d.value());
		if (integer || fraction == metaf::Distance::MilesFraction::NONE)
			result << integer;
		if (integer && fraction != metaf::Distance::MilesFraction::NONE)
			result << " ";
		if (fraction != metaf::Distance::MilesFraction::NONE)
			result << distanceMilesFractionToString(fraction);
	} else {
		const auto d = distance.toUnit(distance.unit());
		if (!d.has_value()) return "[unable to get distance's floating-point value]";
		result << static_cast<int>(*d);
	}
	result << " " << distanceUnitToString(distance.unit());
	result << " (";
	if (distance.unit() != metaf::Distance::Unit::METERS) {
		if (const auto d = distance.toUnit(metaf::Distance::Unit::METERS); d.has_value()) {
			result << static_cast<int>(*d);
			result << " " << distanceUnitToString(metaf::Distance::Unit::METERS);
		} else {
			result << "[unable to convert distance to meters]";
		}
		result << " / ";
	}
	if (distance.unit() != metaf::Distance::Unit::STATUTE_MILES) {
		if (const auto d = distance.miles(); d.has_value()) {
			if (!d.has_value()) return "[unable to get distance value in miles]";
			const auto integer = std::get<unsigned int>(d.value());
			const auto fraction = std::get<metaf::Distance::MilesFraction>(d.value());
			if (integer || fraction == metaf::Distance::MilesFraction::NONE)
				result << integer;
			if (integer && fraction != metaf::Distance::MilesFraction::NONE)
				result << " ";
			if (fraction != metaf::Distance::MilesFraction::NONE)
				result << distanceMilesFractionToString(fraction);
			result << " " << distanceUnitToString(metaf::Distance::Unit::STATUTE_MILES);
		} else {
			result << "[unable to convert distance to statute miles]";
		}
		if (distance.unit() != metaf::Distance::Unit::FEET) result << " / ";
	}
	if (distance.unit() != metaf::Distance::Unit::FEET) {
		if (const auto d = distance.toUnit(metaf::Distance::Unit::FEET); d.has_value()) {
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
		return "not reported";

		case metaf::Direction::Type::VARIABLE:
		return "variable";

		case metaf::Direction::Type::NDV:
		return "no directional variation";

		case metaf::Direction::Type::VALUE_DEGREES:
		if (const auto d = direction.degrees(); d.has_value()) {
			result << *d << " degrees";
		} else {
			result << "[unable to produce value in &deg;]";
		}
		
		case metaf::Direction::Type::VALUE_CARDINAL:
		if (const auto c = cardinalDirectionToString(direction.cardinal(trueCardinalDirections)); 
			!c.empty()) {
				if (direction.type() == metaf::Direction::Type::VALUE_DEGREES) result << "(";
				result << c; 
				if (direction.type() == metaf::Direction::Type::VALUE_DEGREES) result << ")";
		}
		break;

		case metaf::Direction::Type::OVERHEAD:
		return "overhead";

		case metaf::Direction::Type::ALQDS:
		return "all quadrants (all directions)";

		case metaf::Direction::Type::UNKNOWN:
		return "unknown direction";
	}
	return result.str();
}

std::string VisitorExplain::explainPressure(const metaf::Pressure & pressure) {
	if (!pressure.pressure().has_value()) return "not reported";
	std::ostringstream result;
	if (const auto phpa = pressure.toUnit(metaf::Pressure::Unit::HECTOPASCAL); phpa.has_value()) {
		result << roundTo(*phpa, 1) << " hPa";
	} else {
		result << "[unable to convert pressure to hPa]";
	}
	result << " / ";
	if (const auto pinhg = pressure.toUnit(metaf::Pressure::Unit::INCHES_HG); pinhg.has_value()) {
		result << roundTo(*pinhg, 2) << " inHg";
	} else {
		result << "[unable to convert pressure to inHg]";
	}
	result << " / ";
	if (const auto pmmhg = pressure.toUnit(metaf::Pressure::Unit::MM_HG); pmmhg.has_value()) {
		result << roundTo(*pmmhg, 1) << " mmHg";
	} else {
		result << "[unable to convert pressure to mmHg]";
	}

	return result.str();
}

std::string VisitorExplain::explainPrecipitation(const metaf::Precipitation & precipitation) {
	std::ostringstream result;
	if (!precipitation.isReported()) return "not reported";
	if (const auto p = precipitation.amount(); p.has_value() && !*p) return "trace amount";
	if (const auto p = precipitation.toUnit(metaf::Precipitation::Unit::MM); p.has_value()) {
		result << roundTo(*p, 1) << " mm";
	} else {
		result << "[unable to convert precipitation to mm]";
	}
	result << " / ";
	if (const auto p = precipitation.toUnit(metaf::Precipitation::Unit::INCHES); p.has_value()) {
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

std::string VisitorExplain::explainWaveHeight(const metaf::WaveHeight & waveHeight) {
	switch (waveHeight.type()) {
		case metaf::WaveHeight::Type::STATE_OF_SURFACE:
		return ("state of sea surface: "s + 
			std::string(stateOfSeaSurfaceToString(waveHeight.stateOfSurface())));

		case metaf::WaveHeight::Type::WAVE_HEIGHT:
		if (waveHeight.isReported()) {
			std::ostringstream result;
			result << "wave height: ";
			if (const auto h = waveHeight.toUnit(metaf::WaveHeight::Unit::METERS); h.has_value()) {
				result << roundTo(*h, 1) << " meters";
			} else {
				result << "[unable to convert wave height to meters]";
			}
			result << " / ";
			if (const auto h = waveHeight.toUnit(metaf::WaveHeight::Unit::FEET); h.has_value()) {
				result << roundTo(*h, 1) << " feet";
			} else {
				result << "[unable to convert wave height to feet]";
			}
			return result.str();
		}
		return "wave height not reported";
	}
}

std::string VisitorExplain::explainWeatherPhenomena(const metaf::WeatherPhenomena & wp) {
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
	
	if (const auto q = weatherPhenomenaQualifierToString(wp.qualifier()); !q.empty()) result << q << " ";
	if (const auto d = weatherPhenomenaDescriptorToString(wp.descriptor()); !d.empty()) result << d << " ";

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

std::string VisitorExplain::explainDirectionSector(const std::vector<metaf::Direction> dir) {
	std::string result;
	for (auto i=0u; i<dir.size(); i++) {
		if (i) result += ", ";
		result += cardinalDirectionToString(dir[i].cardinal());
	}
	return result;
}

std::string_view VisitorExplain::speedUnitToString(metaf::Speed::Unit unit) {
	switch (unit) {	
		case metaf::Speed::Unit::KNOTS: 				return "knots";
		case metaf::Speed::Unit::METERS_PER_SECOND:		return "m/s";
		case metaf::Speed::Unit::KILOMETERS_PER_HOUR:	return "km/h";
		case metaf::Speed::Unit::MILES_PER_HOUR:		return "mph";
	}
}

std::string_view VisitorExplain::distanceUnitToString(metaf::Distance::Unit unit) {
	switch (unit) {
		case metaf::Distance::Unit::METERS:			return "meters";
		case metaf::Distance::Unit::STATUTE_MILES:	return "statute miles";
		case metaf::Distance::Unit::FEET:			return "feet";
	}
}

std::string_view VisitorExplain::distanceMilesFractionToString(metaf::Distance::MilesFraction f) {
	switch (f) {
		case metaf::Distance::MilesFraction::NONE: 		return "";
		case metaf::Distance::MilesFraction::F_1_16:	return "1/16";
		case metaf::Distance::MilesFraction::F_1_8:		return "1/8";
		case metaf::Distance::MilesFraction::F_3_16:	return "3/16";
		case metaf::Distance::MilesFraction::F_1_4:		return "1/4";
		case metaf::Distance::MilesFraction::F_5_16:	return "5/16";
		case metaf::Distance::MilesFraction::F_3_8:		return "3/8";
		case metaf::Distance::MilesFraction::F_1_2:		return "1/2";
		case metaf::Distance::MilesFraction::F_5_8:		return "5/8";
		case metaf::Distance::MilesFraction::F_3_4:		return "3/4";
		case metaf::Distance::MilesFraction::F_7_8:		return "7/8";
	}
}


std::string_view VisitorExplain::cardinalDirectionToString(metaf::Direction::Cardinal cardinal) {
	switch(cardinal) {
		case metaf::Direction::Cardinal::NOT_REPORTED:	return "not reported";
		case metaf::Direction::Cardinal::N:			return "north";
		case metaf::Direction::Cardinal::S: 		return "south";
		case metaf::Direction::Cardinal::W: 		return "west";
		case metaf::Direction::Cardinal::E: 		return "east";
		case metaf::Direction::Cardinal::NW: 		return "northwest";
		case metaf::Direction::Cardinal::NE: 		return "northeast";
		case metaf::Direction::Cardinal::SW: 		return "southwest";
		case metaf::Direction::Cardinal::SE: 		return "southeast";
		case metaf::Direction::Cardinal::TRUE_N: 	return "true north";
		case metaf::Direction::Cardinal::TRUE_W: 	return "true west";
		case metaf::Direction::Cardinal::TRUE_S: 	return "true south";
		case metaf::Direction::Cardinal::TRUE_E: 	return "true east";
		case metaf::Direction::Cardinal::NDV:		return "no directional variations";
		case metaf::Direction::Cardinal::VRB:		return "variable";
		case metaf::Direction::Cardinal::OHD:		return "overhead";
		case metaf::Direction::Cardinal::ALQDS:		return "all quadrants (in all directions)";
		case metaf::Direction::Cardinal::UNKNOWN:	return "unknown direction";
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

std::string_view VisitorExplain::trendTypeToString(metaf::TrendGroup::Type type) {
	switch (type) {
		case metaf::TrendGroup::Type::NONE:
		return "This group contains incomplete information";

		case metaf::TrendGroup::Type::NOSIG:
		return "No significant weather changes expected";

		case metaf::TrendGroup::Type::BECMG:
		return "Weather conditions are expected to gradually change as follows";

		case metaf::TrendGroup::Type::TEMPO:
		return "The following temporary weather conditions may arise for less than 60 minutes";

		case metaf::TrendGroup::Type::INTER:
		return "The following temporary weather conditions may arise for less than 30 minutes";

		case metaf::TrendGroup::Type::FROM:
		return "All previous weather conditions are superseded by the following conditions";

		case metaf::TrendGroup::Type::TIME_SPAN:
		return "The following weather condition are expected within time span";
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

		case metaf::CloudGroup::Amount::VARIABLE_FEW_SCATTERED:
		return "Sky cover variable between few and scattered clouds (sky coverage variable between 1/8 and 4/8)";

		case metaf::CloudGroup::Amount::VARIABLE_SCATTERED_BROKEN:
		return "Sky cover variable between scattered and broken clouds (sky coverage variable between 3/8 and 7/8)";

		case metaf::CloudGroup::Amount::VARIABLE_BROKEN_OVERCAST:
		return "Sky cover variable between broken clouds and overcast (sky coverage variable between 5/8 and 8/8)";
	}
}

std::string_view VisitorExplain::cloudTypeToString(metaf::CloudGroup::CloudType type) {
	switch (type) {
		case metaf::CloudGroup::CloudType::NONE:
		return std::string_view();

		case metaf::CloudGroup::CloudType::NOT_REPORTED:
		return "not reported";

		case metaf::CloudGroup::CloudType::TOWERING_CUMULUS:
		return "towering cumulus";

		case metaf::CloudGroup::CloudType::CUMULONIMBUS:
		return "cumulonimbus";
	}
}

std::string_view VisitorExplain::weatherPhenomenaQualifierToString(
	metaf::WeatherPhenomena::Qualifier qualifier)
{
	switch (qualifier) {
		case metaf::WeatherPhenomena::Qualifier::NONE:	  	return std::string_view();
		case metaf::WeatherPhenomena::Qualifier::RECENT: 	return std::string_view();
		case metaf::WeatherPhenomena::Qualifier::VICINITY: 	return std::string_view();
		case metaf::WeatherPhenomena::Qualifier::LIGHT: 	return "light";
		case metaf::WeatherPhenomena::Qualifier::MODERATE:	return "moderate";
		case metaf::WeatherPhenomena::Qualifier::HEAVY:		return "heavy";
	}
}

std::string_view VisitorExplain::weatherPhenomenaDescriptorToString(
	metaf::WeatherPhenomena::Descriptor descriptor)
{
	switch(descriptor) {
		case metaf::WeatherPhenomena::Descriptor::NONE:			return std::string();
		case metaf::WeatherPhenomena::Descriptor::SHALLOW:		return "shallow";
		case metaf::WeatherPhenomena::Descriptor::PARTIAL:		return "partial";
		case metaf::WeatherPhenomena::Descriptor::PATCHES:		return "patches of ";
		case metaf::WeatherPhenomena::Descriptor::LOW_DRIFTING:	return "low drifting";
		case metaf::WeatherPhenomena::Descriptor::BLOWING:		return "blowing";
		case metaf::WeatherPhenomena::Descriptor::SHOWERS:		return std::string();
		case metaf::WeatherPhenomena::Descriptor::THUNDERSTORM:	return std::string();
		case metaf::WeatherPhenomena::Descriptor::FREEZING:		return "freezing";
	}
}

std::string_view VisitorExplain::weatherPhenomenaWeatherToString(
	metaf::WeatherPhenomena::Weather weather)
{
	switch (weather) {
			case metaf::WeatherPhenomena::Weather::OMITTED:
			return "not specified";

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
				"undetermined precipitation (automated system cannot identify precipitation)"
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

	for (const auto w : specialWeatherPhenomena) {
		if (wp.qualifier() == std::get<metaf::WeatherPhenomena::Qualifier>(w) &&
			wp.descriptor() == std::get<metaf::WeatherPhenomena::Descriptor>(w) &&
			wp.weather() == std::get<WeatherVector>(w))
		{
			return std::get<std::string_view>(w);
		}
	}
	return std::string();
}

std::string_view VisitorExplain::visTrendToString(metaf::VisibilityGroup::Trend trend) {
	switch(trend) {
		case metaf::VisibilityGroup::Trend::NONE:			return std::string_view();
		case metaf::VisibilityGroup::Trend::NOT_REPORTED:	return "not reported";
		case metaf::VisibilityGroup::Trend::UPWARD:			return "upward";
		case metaf::VisibilityGroup::Trend::NEUTRAL:		return "neutral";
		case metaf::VisibilityGroup::Trend::DOWNWARD:		return "downward";
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

		case metaf::LayerForecastGroup::Type::ICING_LIGHT_CLEAR_IN_PRECIPITATION:
		return "Light clear icing in precipitation";

		case metaf::LayerForecastGroup::Type::ICING_MODERATE_MIXED:
		return "Moderate mixed icing";

		case metaf::LayerForecastGroup::Type::ICING_MODERATE_RIME_IN_CLOUD:
		return "Moderate rime icing in cloud";

		case metaf::LayerForecastGroup::Type::ICING_MODERATE_CLEAR_IN_PRECIPITATION:
		return "Moderate clear icing in precipitation";

		case metaf::LayerForecastGroup::Type::ICING_SEVERE_MIXED:
		return "Severe mixed icing";

		case metaf::LayerForecastGroup::Type::ICING_SEVERE_RIME_IN_CLOUD:
		return "Severe rime icing in cloud";

		case metaf::LayerForecastGroup::Type::ICING_SEVERE_CLEAR_IN_PRECIPITATION:
		return "Severe clear icing in precipitation";

		case metaf::LayerForecastGroup::Type::TURBULENCE_NONE:
		return "No turbulence";

		case metaf::LayerForecastGroup::Type::TURBULENCE_LIGHT:
		return "Light turbulence";

		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASSIONAL:
		return "Occasional moderate turbulence in clear air";

		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT:
		return "Frequent moderate turbulence in clear air";

		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLOUD_OCCASSIONAL:
		return "Occasional moderate turbulence in cloud";

		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLOUD_FREQUENT:
		return "Frequent moderate turbulence in cloud";

		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASSIONAL:
		return "Occasional severe turbulence in clear air";

		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT:
		return "Frequent severe turbulence in clear air";

		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLOUD_OCCASSIONAL:
		return "Occasional severe turbulence in cloud";

		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLOUD_FREQUENT:
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
		return "increasing, then steady, or increasing then increasing more slowly";

		case metaf::PressureTendencyGroup::Type::INCREASING:
		return "increasing steadily or unsteadily";

		case metaf::PressureTendencyGroup::Type::INCREASING_MORE_RAPIDLY:
		return "decreasing or steady, then increasing; or increasing then increasing more rapidly";

		case metaf::PressureTendencyGroup::Type::STEADY:
		return "steady";

		case metaf::PressureTendencyGroup::Type::DECREASING_THEN_INCREASING:
		return "decreasing, then increasing";

		case metaf::PressureTendencyGroup::Type::DECREASING_MORE_SLOWLY:
		return "decreasing then steady; or decreasing then decreasing more slowly";

		case metaf::PressureTendencyGroup::Type::DECREASING:
		return "decreasing steadily or unsteadily";

		case metaf::PressureTendencyGroup::Type::DECREASING_MORE_RAPIDLY:
		return "steady or increasing, then decreasing; or decreasing then decreasing more rapidly";

		case metaf::PressureTendencyGroup::Type::NOT_REPORTED:
		return "not reported";

		case metaf::PressureTendencyGroup::Type::RISING_RAPIDLY:
		return "rising rapidly";

		case metaf::PressureTendencyGroup::Type::FALLING_RAPIDLY:
		return "falling rapidly";
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

std::string_view VisitorExplain::cloudTypeToString(metaf::CloudTypesGroup::Type type) {
	switch(type) {
		case metaf::CloudTypesGroup::Type::CUMULONIMBUS:
		return "cumulonimbus";

		case metaf::CloudTypesGroup::Type::TOWERING_CUMULUS:
		return "towering cumulus";

		case metaf::CloudTypesGroup::Type::CUMULUS:
		return "cumulus";

		case metaf::CloudTypesGroup::Type::CUMULUS_FRACTUS:
		return "cumulus fractus";

		case metaf::CloudTypesGroup::Type::STRATOCUMULUS:
		return "stratocumulus";

		case metaf::CloudTypesGroup::Type::NIMBOSTRATUS:
		return "nimbostratus";

		case metaf::CloudTypesGroup::Type::STRATUS:
		return "stratus";

		case metaf::CloudTypesGroup::Type::STRATUS_FRACTUS:
		return "stratus fractus";

		case metaf::CloudTypesGroup::Type::ALTOSTRATUS:
		return "altostratus";
		
		case metaf::CloudTypesGroup::Type::ALTOCUMULUS:
		return "altocumulus";

		case metaf::CloudTypesGroup::Type::ALTOCUMULUS_CASTELLANUS:
		return "altocumulus castellanus";
		
		case metaf::CloudTypesGroup::Type::CIRRUS:
		return "cirrus";
		
		case metaf::CloudTypesGroup::Type::CIRROSTRATUS:
		return "cirrostratus";
		
		case metaf::CloudTypesGroup::Type::CIRROCUMULUS:
		return "cirrocumulus";
		
		case metaf::CloudTypesGroup::Type::BLOWING_SNOW:
		return "blowing snow";

		case metaf::CloudTypesGroup::Type::BLOWING_DUST:
		return "blowing dust";

		case metaf::CloudTypesGroup::Type::BLOWING_SAND:
		return "blowing sand";

		case metaf::CloudTypesGroup::Type::ICE_CRYSTALS:
		return "ice crystals";

		case metaf::CloudTypesGroup::Type::RAIN:
		return "rain";

		case metaf::CloudTypesGroup::Type::DRIZZLE:
		return "drizzle";

		case metaf::CloudTypesGroup::Type::SNOW:
		return "snow";

		case metaf::CloudTypesGroup::Type::ICE_PELLETS:
		return "ice pellets";

		case metaf::CloudTypesGroup::Type::SMOKE:
		return "smoke";

		case metaf::CloudTypesGroup::Type::FOG:
		return "fog";

		case metaf::CloudTypesGroup::Type::MIST:
		return "mist";

		case metaf::CloudTypesGroup::Type::HAZE:
		return "haze";
	}
}

std::string_view VisitorExplain::cloudLowLayerToString(metaf::CloudLayersGroup::LowLayer lowLayer) {
	switch(lowLayer) {
		case metaf::CloudLayersGroup::LowLayer::NONE:
		return "No low layer clouds";

		case metaf::CloudLayersGroup::LowLayer::CU_HU_CU_FR:
		return "Cumulus clouds showing little vertical extent "
			"(Cumulus humilis or Cumulus fractus of dry weather or both)";

		case metaf::CloudLayersGroup::LowLayer::CU_MED_CU_CON:
		return "Cumulus clouds showing moderate or significant vertical extent "
			"(Cumulus mediocris or Cumulus congestus, with or without Cumulus "
			"humilis or Cumulus fractus or stratocumulus, all having their bases on "
			"the same level)";

		case metaf::CloudLayersGroup::LowLayer::CB_CAL:
		return "Cumulonimbus clouds without fibrous or striated parts at summit "
			"(Cumulonimbus calvus with or without Cumulus, Stratocumulus or Stratus)";

		case metaf::CloudLayersGroup::LowLayer::SC_CUGEN:
		return "Stratocumulus clouds resulting from the spreading out of Cumulus "
			"(Stratocumulus cumulogenitus; Cumulus may also be present)";

		case metaf::CloudLayersGroup::LowLayer::SC_NON_CUGEN:
		return "Stratocumulus clouds not resulting from the spreading out of Cumulus "
			"(Stratocumulus non-cumulogenitus)";

		case metaf::CloudLayersGroup::LowLayer::ST_NEB_ST_FR:
		return "Stratus clouds which consist of a continuous single sheet or layer "
			"with a fairly uniform base, or transitory stage during the formation "
			"or the dissipation of such clouds "
			"(Stratus nebulosus or Stratus fractus of dry weather, or both)";

		case metaf::CloudLayersGroup::LowLayer::ST_FR_CU_FR_PANNUS:
		return "Ragged grey clouds which form below precipitating clouds "
			"(Stratus fractus or Cumulus fractus of wet weather, or both (pannus) )";

		case metaf::CloudLayersGroup::LowLayer::CU_SC_NON_CUGEN_DIFFERENT_LEVELS:
		return "Cumulus and Stratocumulus not formed by spreading of Cumulus "
			"with bases at different levels";

		case metaf::CloudLayersGroup::LowLayer::CB_CAP:
		return "Cumulonimbus clouds with fibrous or striated summits, often with an anvil "
			"(Cumulonimbus capillatus or Cumulonimbus capillatus incus)";

		case metaf::CloudLayersGroup::LowLayer::NOT_OBSERVABLE:
		return "Clouds are not observable due to fog, blowing dust or sand, "
			"or other similar phenomena";
	}
}

std::string_view VisitorExplain::cloudMidLayerToString(metaf::CloudLayersGroup::MidLayer midLayer) {
	switch(midLayer) {
		case metaf::CloudLayersGroup::MidLayer::NONE:
		return "No mid-layer clouds";

		case metaf::CloudLayersGroup::MidLayer::AS_TR:
		return "A veil of greyish or bluish colour translucent enough "
			"to reveal the position of the Sun or Moon "
			"(Altostratus translucidus)";
		
		case metaf::CloudLayersGroup::MidLayer::AS_OP_NS:
		return "A veil of a darker grey or a darker bluish grey dense enough to "
			"completely mask the Sun or Moon "
			"(Altostratus opacus or Nimbostratus)";
		
		case metaf::CloudLayersGroup::MidLayer::AC_TR:
		return "Altocumulus (mackerel sky) clouds in patches or sheets at the same "
			"level or in a single layer "
			"(Altocumulus translucidus at a single level)";
		
		case metaf::CloudLayersGroup::MidLayer::AC_TR_LEN_PATCHES:
		return "Patches, often lenticular (lens or almond-shaped), of Altocumulus "
			"translucidus, continually changing and occurring at one or more levels";
		
		case metaf::CloudLayersGroup::MidLayer::AC_TR_AC_OP_SPREADING:
		return "Altocumulus translucidus in bands, or one or more layers of "
			"Altocumulus translucidus or Altocumulus opacus, "
			"progressively invading the sky";
		
		case metaf::CloudLayersGroup::MidLayer::AC_CUGEN_AC_CBGEN:
		return "Altocumulus resulting generally from the spreading "
			"out of the summits of Cumulus; or Alcocumulus clouds acompanying Cumulonimbus "
			"(Altocumulus cumulogenitus or Altocumulus cumulonimbogenitus)";
		
		case metaf::CloudLayersGroup::MidLayer::AC_DU_AC_OP_AC_WITH_AS_OR_NS:
		return "Altocumulus duplicatus, or Altocumulus opacus in a single layer, not "
			"progressively invading the sky, or Altocumulus with Altostratus or "
			"Nimbostratus.";
		
		case metaf::CloudLayersGroup::MidLayer::AC_CAS_AC_FLO:
		return "Turrets at appear to be arranged in lines with a common horizontal "
			"base or scattered tufts with rounded and slightly bulging upper parts"
			"(Altocumulus castellanus or Altocumulus floccus)";
		
		case metaf::CloudLayersGroup::MidLayer::AC_OF_CHAOTIC_SKY:
		return "Sky of chaotic, heavy and stagnant appearance, which consist of "
			"superposed, more or less broken cloud sheets of ill-defined species "
			"or varieties";

		case metaf::CloudLayersGroup::MidLayer::NOT_OBSERVABLE:
		return "Clouds are not observable due to fog, blowing dust or sand, "
			"or other similar phenomena or because of a continuous layer of lower clouds";
	}
}

std::string_view VisitorExplain::cloudHighLayerToString(metaf::CloudLayersGroup::HighLayer highLayer) {
	switch(highLayer) {
		case metaf::CloudLayersGroup::HighLayer::NONE:
		return "No high-layer clouds";

		case metaf::CloudLayersGroup::HighLayer::CI_FIB_CI_UNC:
		return "Nearly straight or more or less curved filaments; more rarely, "
			"they are shaped like commas topped with either a hook or a tuft "
			"that is not rounded "
			"(Cirrus fibratus and sometimes Cirrus uncinus, not "
			"progressively invading the sky)";

		case metaf::CloudLayersGroup::HighLayer::CI_SPI_CI_CAS_CI_FLO:
		return "Cirrus spissatus, in patches or entangled sheaves, that usually "
			"do not increase and sometimes appear to be the remains of the upper "
			"part of a Cumulonimbus; or Cirrus clouds with small fibrous turrets "
			"rising from common base; or more or less isolated tufts, often with trails "
			"(Cirrus spissatus or Cirrus castellanus or Cirrus floccus)";

		case metaf::CloudLayersGroup::HighLayer::CI_SPI_CBGEN:
		return "Cirrus clouds originated from a Cumulonimbus cloud(s) "
			"(Cirrus spissatus cumulonimbogenitus)";

		case metaf::CloudLayersGroup::HighLayer::CI_FIB_CI_UNC_SPREADING:
		return "Cirrus uncinus, Cirrus fibratus or both, progressively invading the sky; "
			"they generally thicken as a whole";

		case metaf::CloudLayersGroup::HighLayer::CI_CS_LOW_ABOVE_HORIZON:
		return "Cirrus (often in bands) and Cirrostratus, or Cirrostratus alone, "
			"progressively invading the sky; they generally thicken as a whole, "
			"but the continuous veil does not reach 45&deg; above the horizon";

		case metaf::CloudLayersGroup::HighLayer::CI_CS_HIGH_ABOVE_HORIZON:
		return "Cirrus (often in bands) and Cirrostratus, or Cirrostratus alone, "
			"progressively invading the sky; they generally thicken as a whole; "
			"the continuous veil extends more than 45&deg; above the horizon, without "
			"the sky being totally covered";

		case metaf::CloudLayersGroup::HighLayer::CS_NEB_CS_FIB_COVERING_ENTIRE_SKY:
		return "Light, uniform and nebulous veil showing no distinct details"
			"or a white and fibrous veil with more or less clear-cut striations, "
			"covering the whole sky"
			"(Cirrostratus nebulosus or Cirrostratus fibratus covering the whole sky)";

		case metaf::CloudLayersGroup::HighLayer::CS:
		return "A veil of Cirrostratus that is not (or no longer) invading the sky "
			"progressively and that does not completely cover the whole sky";

		case metaf::CloudLayersGroup::HighLayer::CC:
		return "Cirrocumulus alone, or predominant among the high-layer clouds; when "
			"alone, its elements are frequently grouped into more or less extensive "
			"patches with very characteristic small wavelets";

		case metaf::CloudLayersGroup::HighLayer::NOT_OBSERVABLE:
		return "Clouds are not observable due to fog, blowing dust or sand, "
			"or other similar phenomena or because of a continuous layer of lower clouds";
	}
}

std::string VisitorExplain::roundTo(float number, size_t digitsAfterDecimalPoint) {
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

static std::string result;

extern "C" void EMSCRIPTEN_KEEPALIVE freeMemory(){
	std::string().swap(result);
}

extern "C" const char * EMSCRIPTEN_KEEPALIVE explain(const char * input) {
	freeMemory();
	const auto parseResult = metaf::Parser::parse(std::string(input));
	VisitorExplain visitor;
	//Table header
	result += "<thead><tr><th>Group</th><th>Explanation</th></tr></thead><tbody>";
	//Detected report type
	result += "<tr><td>&nbsp;</td><td>";
	result += "Detected report type: ";
	result += VisitorExplain::reportTypeToString(parseResult.reportMetadata.type);
	result += "</td></tr>";
	//Error (if present)
	if (parseResult.reportMetadata.error != metaf::ReportError::NONE) {
		result += "<tr><td>&nbsp;</td><td>";
		result += "Parsing error: ";
		result += VisitorExplain::reportErrorToString(parseResult.reportMetadata.error);
		result += "</td></tr>";
	}
	//Report groups
	for (const auto groupInfo : parseResult.groups) {
		result += "<tr><td>";
		result += groupInfo.rawString;
		result += "</td><td>";
		result += visitor.visit(groupInfo.group, groupInfo.reportPart, groupInfo.rawString);
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
