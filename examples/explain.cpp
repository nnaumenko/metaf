/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

/// @file 
/// @brief This example parses METAR or TAF reports and displays them in 
/// human-readable format.

#include "metaf.h"
#include <vector>
#include <sstream>
#include <cmath>
#include <tuple>
#include <emscripten/emscripten.h>

using namespace std::literals;

class GroupVisitorExplain : public metaf::GroupVisitor<std::string> {
public:
	static std::string_view reportTypeToString(metaf::ReportType reportType);
	static std::string_view reportErrorToString(metaf::Parser::Error error);
private:
	virtual std::string visitPlainTextGroup(const metaf::PlainTextGroup & group);
	virtual std::string visitFixedGroup(const metaf::FixedGroup & group);
	virtual std::string visitLocationGroup(const metaf::LocationGroup & group);
	virtual std::string visitReportTimeGroup(const metaf::ReportTimeGroup & group);
	virtual std::string visitTrendGroup(const metaf::TrendGroup & group);
	virtual std::string visitWindGroup(const metaf::WindGroup & group);
	virtual std::string visitVisibilityGroup(const metaf::VisibilityGroup & group);
	virtual std::string visitCloudGroup(const metaf::CloudGroup & group);
	virtual std::string visitWeatherGroup(const metaf::WeatherGroup & group);
	virtual std::string visitTemperatureGroup(const metaf::TemperatureGroup & group);
	virtual std::string visitTemperatureForecastGroup(
		const metaf::TemperatureForecastGroup & group);
	virtual std::string visitPressureGroup(const metaf::PressureGroup & group);
	virtual std::string visitRunwayVisualRangeGroup(const metaf::RunwayVisualRangeGroup & group);
	virtual std::string visitRunwayStateGroup(const metaf::RunwayStateGroup & group);
	virtual std::string visitSecondaryLocationGroup(const metaf::SecondaryLocationGroup & group);
	virtual std::string visitRainfallGroup(const metaf::RainfallGroup & group);
	virtual std::string visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group);
	virtual std::string visitColourCodeGroup(const metaf::ColourCodeGroup & group);
	virtual std::string visitMinMaxTemperatureGroup(
		const metaf::MinMaxTemperatureGroup & group);
	virtual std::string visitPrecipitationGroup(const metaf::PrecipitationGroup & group);
	virtual std::string visitLayerForecastGroup(const metaf::LayerForecastGroup & group);
	virtual std::string visitPressureTendencyGroup(
		const metaf::PressureTendencyGroup & group);
	virtual std::string visitCloudTypesGroup(const metaf::CloudTypesGroup & group);
	virtual std::string visitCloudLayersGroup(const metaf::CloudLayersGroup & group);
	virtual std::string visitMiscGroup(const metaf::MiscGroup & group);
	virtual std::string visitOther(const metaf::Group & group);

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

	static std::string_view speedUnitToString(metaf::Speed::Unit unit);
	static std::string_view distanceUnitToString(metaf::Distance::Unit unit);
	static std::string_view cardinalDirectionToString(metaf::Direction::Cardinal cardinal);
	static std::string_view brakingActionToString(
		metaf::SurfaceFriction::BrakingAction brakingAction);
	static std::string_view stateOfSeaSurfaceToString(
		metaf::WaveHeight::StateOfSurface stateOfSurface);
	static std::string_view trendTypeToString(metaf::TrendGroup::Type type);
	static std::string_view cloudAmountToString(metaf::CloudGroup::Amount amount);
	static std::string_view cloudTypeToString(metaf::CloudGroup::Type type);
	static std::string_view weatherQualifierToString(metaf::WeatherGroup::Qualifier qualifier);
	static std::string_view weatherDescriptorToString(metaf::WeatherGroup::Descriptor descriptor);
	static std::string_view weatherPhenomenaToString(metaf::WeatherGroup::Weather weather);
	static std::string_view specialWeatherPhenomenaToString(const metaf::WeatherGroup & group);
	static std::string_view rvrTrendToString(metaf::RunwayVisualRangeGroup::Trend trend);
	static std::string_view runwayStateDepositsToString(metaf::RunwayStateGroup::Deposits deposits);
	static std::string_view runwayStateExtentToString(metaf::RunwayStateGroup::Extent extent);
	static std::string_view colourCodeToString(metaf::ColourCodeGroup::Code code);
	static unsigned int colourCodeVisibility(metaf::ColourCodeGroup::Code code);
	static unsigned int colourCodeCeiling(metaf::ColourCodeGroup::Code code);
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

	static const inline std::string lineBreak = std::string("<br>");
	static const inline std::string groupNotValidMessage = 
		std::string("Data in this group may be errorneous, incomplete or inconsistent");
};

std::string GroupVisitorExplain::visitPlainTextGroup(const metaf::PlainTextGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "These groups are not recognised by parser: " << group.toString();
	return(result.str());
}

std::string GroupVisitorExplain::visitFixedGroup(const metaf::FixedGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch (group.type()) {
		case metaf::FixedGroup::Type::INCOMPLETE:
		result << "One or more groups were recognised by parser as ";
		result << "a fixed group but some of the text appears missing.";
		break;

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

		case metaf::FixedGroup::Type::R_SNOCLO:
		result << "Aerodrome is closed due to snow accumulation";
		break;

		case metaf::FixedGroup::Type::CAVOK:
		result << "Ceiling and visibility OK" << lineBreak;
		result << "Visibility 10 km or more in all directions, ";
		result << "no cloud below 5000 feet (1500 meters), ";
		result << "no cumulonimbus or towering cumulus clouds, ";
		result << "no significant weather phenomena";
		break;

		case metaf::FixedGroup::Type::WSCONDS:
		result << "Potential wind shear conditions are present ";
		result << "but there's not enough information to reliably forecast ";
		result << "height, direction and speed of wind shear";
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

		case metaf::FixedGroup::Type::NOSPECI:
		result << "This manual station does not issue SPECI (unscheduled) reports";
		break;

		case metaf::FixedGroup::Type::RVRNO:
		result << "Runway visual range should be reported but is missing";
		break;

		case metaf::FixedGroup::Type::PWINO:
		result << "This automated station is equipped with present weather identifier ";
		result << "and this sensor is not operating";
		break;

		case metaf::FixedGroup::Type::PNO:
		result << "This automated station is equipped with tipping bucket rain gauge ";
		result << "and this sensor is not operating";
		break;

		case metaf::FixedGroup::Type::FZRANO:
		result << "This automated station is equipped with freezing rain sensor ";
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

		case metaf::FixedGroup::Type::CLD_MISG:
		result << "Sky condition data (cloud data) is missing";
		break;

		case metaf::FixedGroup::Type::ICG_MISG:
		result << "Icing data is missing";
		break;

		case metaf::FixedGroup::Type::PCPN_MISG:
		result << "Precipitation data is missing";
		break;

		case metaf::FixedGroup::Type::PRES_MISG:
		result << "Atmospheric pressure (altimeter) data is missing";
		break;

		case metaf::FixedGroup::Type::RVR_MISG:
		result << "Runway visual range data is missing";
		break;

		case metaf::FixedGroup::Type::T_MISG:
		result << "Temperature data is missing";
		break;

		case metaf::FixedGroup::Type::TD_MISG:
		result << "Dew point data is missing";
		break;

		case metaf::FixedGroup::Type::VIS_MISG:
		result << "Visibility data is missing";
		break;

		case metaf::FixedGroup::Type::WND_MISG:
		result << "Wind data is missing";
		break;

		case metaf::FixedGroup::Type::WX_MISG:
		result << "Weather phenomena data is missing";
		break;

		case metaf::FixedGroup::Type::TS_LTNG_TEMPO_UNAVBL:
		result << "Thunderstorm / lightning data is missing";
		break;

		default:
		result << "Unknown fixed group";
		break;
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitLocationGroup(const metaf::LocationGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "ICAO code for location: " << group.toString();
	return(result.str());
}

std::string GroupVisitorExplain::visitReportTimeGroup(
	const metaf::ReportTimeGroup & group)
{
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Day and time of report issue: " << explainMetafTime(group.time());
	return(result.str());
}

std::string GroupVisitorExplain::visitTrendGroup(const metaf::TrendGroup & group) {
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
		if (group.type() == metaf::TrendGroup::Type::BECMG ||
			group.type() == metaf::TrendGroup::Type::TEMPO ||
			group.type() == metaf::TrendGroup::Type::INTER)
		{
			result << "Trend probability is 50 percent or more" << lineBreak;;
		}
		break;

		default:
		result << "[Unknown probability value]" << lineBreak;;
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
	return(result.str());
}

std::string GroupVisitorExplain::visitWindGroup(const metaf::WindGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch (group.type()) {
		case metaf::WindGroup::Type::INCOMPLETE:
		result << "One or more groups were recognised by parser as ";
		result << "a wind group but some of the text appears missing.";
		break;

		case metaf::WindGroup::Type::SURFACE_WIND:
		case metaf::WindGroup::Type::VARIABLE_WIND_SECTOR:
		case metaf::WindGroup::Type::SURFACE_WIND_WITH_VARIABLE_SECTOR:
		result << "Surface wind:";
		break;

		case metaf::WindGroup::Type::WIND_SHEAR:
		result << "Wind shear at height ";
		result << explainDistance(group.height()) << ':';
		break;

		case metaf::WindGroup::Type::WIND_SHIFT:		
		case metaf::WindGroup::Type::WIND_SHIFT_FROPA:
		result << "Wind direction changed 45&deg; or more in less than 15 minutes ";
		result << "with sustained wind speed of 10 knots (5.1 m/s / 18.5 km/h / 11.5 mph)";
		if (group.eventTime().has_value()) {
			result << lineBreak << "Wind shift began at ";
			result << explainMetafTime(group.eventTime().value());
		}
		if (group.type() == metaf::WindGroup::Type::WIND_SHIFT_FROPA) {
			result << lineBreak << "This wind shift is associated with weather front passage";
		}
		return(result.str());

		case metaf::WindGroup::Type::PEAK_WIND:
		result << "Peak wind: ";
		break;

		default:
		result << "[unknown wind group]:";
		break;
	}
	result << lineBreak;

	if (group.isCalm()) {
		// Calm wind
		result << "No wind / Calm";
		return(result.str());
	}

	if (group.direction().status() != metaf::Direction::Status::OMMITTED || 
		group.windSpeed().speed().has_value())
	{
		// General wind information present in this group
		result << "Wind direction: " << explainDirection(group.direction(), true) << lineBreak;
		result << "Wind speed: " << explainSpeed(group.windSpeed()) << lineBreak;
		if (group.gustSpeed().speed().has_value()) {
			result  << "Gust speed: " << explainSpeed(group.gustSpeed()) << lineBreak;
		}
	}
	if (group.type() == metaf::WindGroup::Type::VARIABLE_WIND_SECTOR ||
		group.type() == metaf::WindGroup::Type::SURFACE_WIND_WITH_VARIABLE_SECTOR) {
			result << "Variable wind direction sector from ";
			result << explainDirection(group.varSectorBegin()) << " clockwise to ";
			result << explainDirection(group.varSectorEnd());
	}
	if (group.type() == metaf::WindGroup::Type::PEAK_WIND && 
		group.eventTime().has_value()) 
	{
		result << "Peak wind was observed at ";
		result << explainMetafTime(group.eventTime().value());
	}

	return(result.str());
}

std::string GroupVisitorExplain::visitVisibilityGroup(const metaf::VisibilityGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Visibility (";
	switch (group.type()) {
		case metaf::VisibilityGroup::Type::PREVAILING:
		case metaf::VisibilityGroup::Type::PREVAILING_NDV:
		result << "prevailing";
		break;

		case metaf::VisibilityGroup::Type::DIRECTIONAL:
		result << explainDirection(group.direction());
		break;

		default:
		result << "[unknown visibility group]:";
		break;
	}
	result << ") ";
	result << explainDistance(group.visibility());
	if (group.type() == metaf::VisibilityGroup::Type::PREVAILING_NDV) {
		result << lineBreak;
		result << "This station does not distinguish directional variation of visibility";
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitCloudGroup(const metaf::CloudGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << cloudAmountToString(group.amount());
	if (const auto typeStr = cloudTypeToString(group.type()); !typeStr.empty()) {
		result << ", type: " << typeStr;
	}
	result << lineBreak;
	if (group.height().isReported()) {
		result << "Base height " << explainDistance(group.height()) << lineBreak;
	}
	if (group.verticalVisibility().isReported()) {
		result << "Vertical visibility " << explainDistance(group.verticalVisibility()) << lineBreak;
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitWeatherGroup(const metaf::WeatherGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	if (group.qualifier() != metaf::WeatherGroup::Qualifier::RECENT) {
		result << "Weather phenomena: ";
	} else {
		result << "Recent weather: ";
	}

	if (const auto weatherStr = specialWeatherPhenomenaToString(group); !weatherStr.empty()) {
		result << weatherStr;
		return(result.str());
	}

	const bool vicinity = (group.qualifier() == metaf::WeatherGroup::Qualifier::VICINITY);
	const bool showers = (group.descriptor() == metaf::WeatherGroup::Descriptor::SHOWERS);
	const bool thunderstorm = (group.descriptor() == metaf::WeatherGroup::Descriptor::THUNDERSTORM);
	
	if (const auto q = weatherQualifierToString(group.qualifier()); !q.empty()) result << q << " ";
	if (const auto d = weatherDescriptorToString(group.descriptor()); !d.empty()) result << d << " ";

	metaf::WeatherGroup::Weather previous = metaf::WeatherGroup::Weather::NOT_REPORTED;
	const auto weatherPhenomena = group.weather();
	for (const auto w : weatherPhenomena) {
		std::string weatherStr(weatherPhenomenaToString(w));
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
		if (weatherPhenomena.size()) result << "with ";
		result << "thunderstorm ";
	}
	if (vicinity) result << "in vicinity (5 to 10 miles away)";
	return(result.str());
}

std::string GroupVisitorExplain::visitTemperatureGroup(const metaf::TemperatureGroup & group) {
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
	return(result.str());
}

std::string GroupVisitorExplain::visitTemperatureForecastGroup(
	const metaf::TemperatureForecastGroup & group)
{
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

		default:
		result << "[unknown temperature point]";
		break;
	}
	result << " " << explainTemperature(group.airTemperature()) << ", ";
	result << "expected on " << explainMetafTime(group.time());
	return(result.str());
}

std::string GroupVisitorExplain::visitPressureGroup(const metaf::PressureGroup & group) {
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

		default:
		result << "Unknown pressure value";
		break;
	}
	result << ": " << explainPressure(group.atmosphericPressure());
	return(result.str());
}

std::string GroupVisitorExplain::visitRunwayVisualRangeGroup(
	const metaf::RunwayVisualRangeGroup & group)
{
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Visual range of " << explainRunway(group.runway()) << " is ";
	if (group.isVariableVisualRange()) {
		result << explainDistance(group.maxVisualRange());
		result << " to " << explainDistance(group.maxVisualRange());
	} else {
		result << explainDistance(group.visualRange());
	}
	if (group.trend() != metaf::RunwayVisualRangeGroup::Trend::NONE) {
		result << ", and the trend is " << rvrTrendToString(group.trend());
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitRunwayStateGroup(const metaf::RunwayStateGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "State of " << explainRunway(group.runway()) << ": ";
	switch (group.status()) {
		case metaf::RunwayStateGroup::Status::NORMAL:
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

		case metaf::RunwayStateGroup::Status::CLRD:
		result << "deposits on runway were cleared or ceased to exist" << lineBreak;
		result << "Surface friction: ";
		result << explainSurfaceFriction(group.surfaceFriction());
		break;

		case metaf::RunwayStateGroup::Status::SNOCLO:
		result << "closed due to snow accumulation";
		break;

		default:
		result << "[unknown runway status]";
		break;
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitSecondaryLocationGroup(
	const metaf::SecondaryLocationGroup & group)
{
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch (group.type()) {
		case metaf::SecondaryLocationGroup::Type::INCOMPLETE:
		result << "These groups were recognised by parser as ";
		result << "an information about secondary location but ";
		result << "some of the text appears missing.";
		break;

		case metaf::SecondaryLocationGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS:
		result << "Wind shear significant to aircraft operations ";
		result << "is present along the take-off path or approach path ";
		result << "between runway level and 500 metres (1 600 ft)";
		if (const auto rw = group.runway(); rw.has_value()) {
			result << " at " << explainRunway(rw.value());
		}
		break;

		default:
		result << "[unknown secondary location info]";
	}
	return(result.str());
}


std::string GroupVisitorExplain::visitRainfallGroup(const metaf::RainfallGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Rainfall for last 10 minutes ";
	result << explainPrecipitation(group.rainfallLast10Minutes()) << ", ";
	if (group.rainfallLast60Minutes().status() == metaf::Precipitation::Status::REPORTED) {
		result << "for last 60 minutes ";
		result << explainPrecipitation(group.rainfallLast60Minutes()) << ", ";
	}
	result << "total rainfall since 9:00 AM ";
	result << explainPrecipitation(group.rainfallLast60Minutes());
	return(result.str());
}

std::string GroupVisitorExplain::visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Sea surface temperature " << explainTemperature(group.surfaceTemperature()) << ", ";
	result << explainWaveHeight(group.waves());
	return(result.str());
}

std::string GroupVisitorExplain::visitColourCodeGroup(const metaf::ColourCodeGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	if (group.isCodeBlack()) {
		result << "Colour code BLACK: ";
		result << "aerodrome closed due to snow accumulation or non-weather reasons" << lineBreak;
	}
	result << "Colour code " << colourCodeToString(group.code()) << ": ";
	if (const auto [vis, ceiling] = 
			std::pair(colourCodeVisibility(group.code()), colourCodeCeiling(group.code()));
		vis && ceiling)
	{
		if (group.code() == metaf::ColourCodeGroup::Code::RED) {
			result << "either visibility &lt; " << vis;
			result << " or lowest cloud base height &lt; " << ceiling;
		} else {
			result << "visibility &gt;" << vis;
			result << " and lowest cloud base height &gt;" << ceiling;
		}
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group) {
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

		default:
		periodStr = "Unknown period";
		break; 
	}
	result << periodStr << " " << " minimum / maximum temperature";
	if (!group.minimum().temperature().has_value() && 
		!group.maximum().temperature().has_value()) {
			result << " is not reported";
			return(result.str());
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
	return(result.str());
}

std::string GroupVisitorExplain::visitPrecipitationGroup(
	const metaf::PrecipitationGroup & group)
{
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	if (group.type() == metaf::PrecipitationGroup::Type::SNOW_INCREASING_RAPIDLY) {
		result << "Snow increasing rapidly" << lineBreak;
		result << "Snow increased by ";
		result << explainPrecipitation(group.tendency());
		result << " for the last hour, total snowfall is ";
		result << explainPrecipitation(group.amount());
		return(result.str());
	};
	if (group.amount().status() == metaf::Precipitation::Status::REPORTED &&
		!group.amount().precipitation().value())
	{
		result << "Trace amount of ";
		result << precipitationGroupTypeToString(group.type());
	} else {
		result << "Amount of ";
		result << precipitationGroupTypeToString(group.type()) << " is ";
		result << explainPrecipitation(group.amount());
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitLayerForecastGroup(
	const metaf::LayerForecastGroup & group)
{
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << layerForecastGroupTypeToString(group.type());
	result << " at heights from " << explainDistance(group.baseHeight());
	result << " to " << explainDistance(group.topHeight());
	return(result.str());	
}

std::string GroupVisitorExplain::visitPressureTendencyGroup(
		const metaf::PressureTendencyGroup & group)
{
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	if (group.type() == metaf::PressureTendencyGroup::Type::FALLING_RAPIDLY) {
		return ("Pressure is rapidly falling at a rate of at least 0.06 inch of mercury "
			"(2.03 hectopascal) per hour and the pressure change totals "
			"0.02 inch of mercury (0.68 hectopascal) or more "
			"at the time of the observation");
	}
	if (group.type() == metaf::PressureTendencyGroup::Type::RISING_RAPIDLY) {
		return ("Pressure is rapidly rising at a rate of at least 0.06 inch of mercury "
			"(2.03 hectopascal) per hour and the pressure change totals "
			"0.02 inch of mercury (0.68 hectopascal) or more "
			"at the time of the observation");
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
	return(result.str());
}

std::string GroupVisitorExplain::visitCloudTypesGroup(
	const metaf::CloudTypesGroup & group)
{
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	const auto clouds = group.toVector();
	for (auto i = 0u; i < clouds.size(); i++) {
		if (i == 0u) {
			result << "Lowest cloud layer: ";
		} else {
			result << "Next cloud layer: ";
		}
		result << cloudTypeToString(std::get<metaf::CloudTypesGroup::Type>(clouds.at(i)));
		result << ", covering " << std::get<unsigned int>(clouds.at(i)) << "/8 of sky";
		result << lineBreak;
	}
	return(result.str());
}

std::string GroupVisitorExplain::visitCloudLayersGroup(
	const metaf::CloudLayersGroup & group)
{
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	result << "Low cloud layer: " << lineBreak;
	result << cloudLowLayerToString(group.lowLayer()) << lineBreak;
	result << "Mid cloud layer: " << lineBreak;
	result << cloudMidLayerToString(group.midLayer()) << lineBreak;
	result << "High cloud layer: " << lineBreak;
	result << cloudHighLayerToString(group.highLayer()) << lineBreak;
	return(result.str());
}

std::string GroupVisitorExplain::visitMiscGroup(const metaf::MiscGroup & group) {
	std::ostringstream result;
	if (!group.isValid()) result << groupNotValidMessage << lineBreak;
	switch (group.type()) {
		case metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES:
		if (const auto duration = group.value(); duration.has_value()) {
			if (!duration.value()) {
				result << "No sunshine occurred the previous calendar day";
			} else {
				result << "Duration of sunshine ";
				result << "that occurred the previous calendar day is ";
				result << duration.value() << " minutes";
			}
		}
		break;

		case metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION:
		if (const auto correctionNo = group.value(); correctionNo.has_value()) {
			result << "This report is the corrected weather observation, ";
			result << "correction number is " << static_cast<int>(correctionNo.value());
		}
		break;

		default:
		result << "[unknown miscellaneous group type]";
		break;
	}
	return(result.str());
}


std::string GroupVisitorExplain::visitOther(const metaf::Group & group) {
	(void)group;
	return("This group is recognised by parser but not listed");
}

std::string_view GroupVisitorExplain::reportTypeToString(metaf::ReportType reportType) {
	switch (reportType) {
		case metaf::ReportType::UNKNOWN:	return("unable to detect");
		case metaf::ReportType::METAR:		return("METAR (weather observation)");
		case metaf::ReportType::TAF:		return("TAF (weather forecast)");
		default: return("unknown report type");
	}
}

std::string_view GroupVisitorExplain::reportErrorToString(metaf::Parser::Error error) {
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
				
		default: 
		return("unknown error");
	}
}

std::string GroupVisitorExplain::explainRunway(const metaf::Runway & runway) {
	if (runway.isAllRunways()) return("all runways");
	if (runway.isMessageRepetition()) return("same runway (repetition of last message)"); 
	std::ostringstream result;
	result << "runway ";
	result << (runway.number() < 10 ? "0"s : ""s) << std::to_string(runway.number());
	std::string designatorStr;
	switch(runway.designator()) {
		case metaf::Runway::Designator::NONE: 	break;
		case metaf::Runway::Designator::LEFT: 	designatorStr = "LEFT"; break;
		case metaf::Runway::Designator::CENTER:	designatorStr = "CENTER"; break;
		case metaf::Runway::Designator::RIGHT:  designatorStr = "RIGHT"; break;
		default: designatorStr = "(unknown runway designator)"; break;
	}
	if (!designatorStr.empty()) result << " " << designatorStr;
	return(result.str());
}

std::string GroupVisitorExplain::explainMetafTime(const metaf::MetafTime & metafTime) {
	static const std::string hourMinuteSeparator(":");
	std::ostringstream result;
	if (const auto day = metafTime.day(); day.has_value()) {
		result << "day " << *day << ", ";
	}
	result << (metafTime.hour() < 10 ? "0"s : ""s) << metafTime.hour();
	result << hourMinuteSeparator;
	result << (metafTime.minute() < 10 ? "0"s : ""s) << metafTime.minute();
	result << " GMT";
	return(result.str());
}

std::string GroupVisitorExplain::explainTemperature(const metaf::Temperature & temperature) {
	if (!temperature.temperature().has_value()) return("not reported");
	std::ostringstream result;
	if (!temperature.temperature().value() && !temperature.isPrecise()) {
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
	return(result.str());
}

std::string GroupVisitorExplain::explainSpeed(const metaf::Speed & speed) {
	std::ostringstream result;
	if (const auto s = speed.speed(); s.has_value()) {
		result << *speed.speed() << " " << speedUnitToString(speed.unit());
	} else {
		return("not reported");
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
	return(result.str());
}

std::string GroupVisitorExplain::explainDistance(const metaf::Distance & distance) {
	if (!distance.isReported()) return("not reported");
	std::ostringstream result;
	switch (distance.modifier()) {
		case metaf::Distance::Modifier::NONE:			break;
		case metaf::Distance::Modifier::LESS_THAN:		result << "&lt;"; break;
		case metaf::Distance::Modifier::MORE_THAN:		result << "&gt;"; break;
		default: result << "unknown modifier, "; break;
	}
	const auto d = distance.toUnit(distance.unit());
	if (!d.has_value()) return("[unable to get distance's floating-point value]");
	if (distance.unit() == metaf::Distance::Unit::STATUTE_MILES) {
		result << roundTo(*d, 3);
	} else {
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
		if (const auto d = distance.toUnit(metaf::Distance::Unit::STATUTE_MILES); d.has_value()) {
			result << roundTo(*d, 3);
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
	return(result.str());
}

std::string GroupVisitorExplain::explainDirection(const metaf::Direction & direction,
	bool trueCardinalDirections)
{
	std::ostringstream result;
	switch (direction.status()) {
		case metaf::Direction::Status::OMMITTED:
		return(std::string());

		case metaf::Direction::Status::NOT_REPORTED:
		return("not reported");

		case metaf::Direction::Status::VARIABLE:
		return("variable");

		case metaf::Direction::Status::NDV:
		return("no directional variation");

		case metaf::Direction::Status::VALUE_DEGREES:
		if (const auto d = direction.degrees(); d.has_value()) {
			result << *d << " degrees";
		} else {
			result << "[unable to produce value in &deg;]";
		}
		
		case metaf::Direction::Status::VALUE_CARDINAL:
		if (const auto c = cardinalDirectionToString(direction.cardinal(trueCardinalDirections)); 
			!c.empty()) {
				if (direction.status() == metaf::Direction::Status::VALUE_DEGREES) result << "(";
				result << c; 
				if (direction.status() == metaf::Direction::Status::VALUE_DEGREES) result << ")";
		}
		break;

		default:
		return("unknown direction status");
	}
	return(result.str());
}

std::string GroupVisitorExplain::explainPressure(const metaf::Pressure & pressure) {
	if (!pressure.pressure().has_value()) return("not reported");
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

	return(result.str());
}

std::string GroupVisitorExplain::explainPrecipitation(const metaf::Precipitation & precipitation) {
	std::ostringstream result;
	switch (precipitation.status()) {
		case metaf::Precipitation::Status::NOT_REPORTED:
		return("not reported");

		case metaf::Precipitation::Status::RUNWAY_NOT_OPERATIONAL:
		return("not measured because runway is not operational");

		case metaf::Precipitation::Status::REPORTED:
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
		break;

		default:
		return("unknown precipitation status");
	}
	return(result.str());
}

std::string GroupVisitorExplain::explainSurfaceFriction(
	const metaf::SurfaceFriction & surfaceFriction)
{
	switch (surfaceFriction.status()) {
		case metaf::SurfaceFriction::Status::NOT_REPORTED:
		return("not reported");

		case metaf::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED:
		if (const auto c = surfaceFriction.coefficient(); c.has_value()) {
			return ("friction coefficient "s + roundTo(*c, 2));
		}
		return("[unable to produce a friction coefficient]");
		
		case metaf::SurfaceFriction::Status::BRAKING_ACTION_REPORTED:
		return ("braking action "s + 
			std::string(brakingActionToString(surfaceFriction.brakingAction())));

		case metaf::SurfaceFriction::Status::UNRELIABLE:
		return("unreliable or unmeasurable");

		default:
		return("[unknown surface friction status]");
	}
}

std::string GroupVisitorExplain::explainWaveHeight(const metaf::WaveHeight & waveHeight) {
	switch (waveHeight.type()) {
		case metaf::WaveHeight::Type::STATE_OF_SURFACE:
		return("state of sea surface: "s + 
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
			return(result.str());
		}
		return("wave height not reported");

		default:
		return("[unknown wave height status]");
	}
}

std::string_view GroupVisitorExplain::speedUnitToString(metaf::Speed::Unit unit) {
	switch (unit) {	
		case metaf::Speed::Unit::KNOTS: 				return("knots");
		case metaf::Speed::Unit::METERS_PER_SECOND:		return("m/s");
		case metaf::Speed::Unit::KILOMETERS_PER_HOUR:	return("km/h");
		case metaf::Speed::Unit::MILES_PER_HOUR:		return("mph");
		default: 										return("[unknown speed unit]");
	}
}

std::string_view GroupVisitorExplain::distanceUnitToString(metaf::Distance::Unit unit) {
	switch (unit) {
		case metaf::Distance::Unit::METERS:			return("meters");
		case metaf::Distance::Unit::STATUTE_MILES:	return("statute miles");
		case metaf::Distance::Unit::FEET:			return("feet");
		default: 									return("[unknown distance unit]");
	}
}

std::string_view GroupVisitorExplain::cardinalDirectionToString(metaf::Direction::Cardinal cardinal) {
	switch(cardinal) {
		case metaf::Direction::Cardinal::NONE:		return(std::string());
		case metaf::Direction::Cardinal::N:			return("north");
		case metaf::Direction::Cardinal::S: 		return("south");
		case metaf::Direction::Cardinal::W: 		return("west");
		case metaf::Direction::Cardinal::E: 		return("east");
		case metaf::Direction::Cardinal::NW: 		return("northwest");
		case metaf::Direction::Cardinal::NE: 		return("northeast");
		case metaf::Direction::Cardinal::SW: 		return("southwest");
		case metaf::Direction::Cardinal::SE: 		return("southeast");
		case metaf::Direction::Cardinal::TRUE_N: 	return("true north");
		case metaf::Direction::Cardinal::TRUE_W: 	return("true west");
		case metaf::Direction::Cardinal::TRUE_S: 	return("true south");
		case metaf::Direction::Cardinal::TRUE_E: 	return("true east");
		default: 									return("[unknown cardinal direction]");
	}
}

std::string_view GroupVisitorExplain::brakingActionToString(
		metaf::SurfaceFriction::BrakingAction brakingAction)
{
	switch(brakingAction) {
		case metaf::SurfaceFriction::BrakingAction::NONE:
		return("not reported");

		case metaf::SurfaceFriction::BrakingAction::POOR:
		return("poor (friction coefficient 0.0 to 0.25)");

		case metaf::SurfaceFriction::BrakingAction::MEDIUM_POOR:
		return("medium/poor (friction coefficient 0.26 to 0.29)");

		case metaf::SurfaceFriction::BrakingAction::MEDIUM:
		return("medium (friction coefficient 0.30 to 0.35)");

		case metaf::SurfaceFriction::BrakingAction::MEDIUM_GOOD:
		return("medium/good (friction coefficient 0.36 to 0.40)");

		case metaf::SurfaceFriction::BrakingAction::GOOD:
		return("good (friction coefficient 0.40 to 1.00)");

		default:
		return("unknown braking action");
	}
}

std::string_view GroupVisitorExplain::stateOfSeaSurfaceToString(
	metaf::WaveHeight::StateOfSurface stateOfSurface)
{
	switch(stateOfSurface) {
		case metaf::WaveHeight::StateOfSurface::NOT_REPORTED:
		return("not reported");

		case metaf::WaveHeight::StateOfSurface::CALM_GLASSY:
		return("calm (glassy), no waves");

		case metaf::WaveHeight::StateOfSurface::CALM_RIPPLED:
		return("calm (rippled), wave height &lt;0.1 meters / &lt;1/3 feet");

		case metaf::WaveHeight::StateOfSurface::SMOOTH:
		return("smooth, wave height 0.1 to 0.5 meters / 1/3 to 1 1/2 feet");

		case metaf::WaveHeight::StateOfSurface::SLIGHT:
		return("slight, wave height 0.5 to 1.25 meters / 1 1/2 to 4 feet");

		case metaf::WaveHeight::StateOfSurface::MODERATE:
		return("moderate, wave height 1.25 to 2.5 meters / 4 to 8 feet");

		case metaf::WaveHeight::StateOfSurface::ROUGH:
		return("rough, wave height 2.5 to 4 meters / 8 to 13 feet");

		case metaf::WaveHeight::StateOfSurface::VERY_ROUGH:
		return("very rough, wave height 4 to 6 meters / 13 to 20 feet");

		case metaf::WaveHeight::StateOfSurface::HIGH:
		return("high, wave height 6 to 9 meters / 20 to 30 feet");

		case metaf::WaveHeight::StateOfSurface::VERY_HIGH:
		return("very high, wave height 9 to 14 meters / 30 to 46 feet");

		case metaf::WaveHeight::StateOfSurface::PHENOMENAL:
		return("phenomenal, wave height >14 meters / &gt;46 feet");

		default:
		return("unknown state of sea surface");
	}
}

std::string_view GroupVisitorExplain::trendTypeToString(metaf::TrendGroup::Type type) {
	switch (type) {
		case metaf::TrendGroup::Type::NONE:
		return("This group contains incomplete information");

		case metaf::TrendGroup::Type::NOSIG:
		return("No significant weather changes expected");

		case metaf::TrendGroup::Type::BECMG:
		return("Weather conditions are expected to gradually change as follows");

		case metaf::TrendGroup::Type::TEMPO:
		return("The following temporary weather conditions may arise for less than 60 minutes");

		case metaf::TrendGroup::Type::INTER:
		return("The following temporary weather conditions may arise for less than 30 minutes");

		case metaf::TrendGroup::Type::FROM:
		return("All previous weather conditions are superseded by the following conditions");

		case metaf::TrendGroup::Type::TIME_SPAN:
		return("The following weather condition are expected within time span");

		default:
		return("Unknown trend type");
	}
}

std::string_view GroupVisitorExplain::cloudAmountToString(metaf::CloudGroup::Amount amount) {
	switch (amount) {
		case metaf::CloudGroup::Amount::NOT_REPORTED:
		return("Cloud amount not reported");

		case metaf::CloudGroup::Amount::NSC:
		return("No significant cloud: "
			"no cloud below 5000 feet (1500 meters), no cumulonimbus or towering "
			"cumulus clouds, no vertical visibility restriction");

		case metaf::CloudGroup::Amount::NCD:
		return("No cloud detected: automated weather station did not detect any clouds; "
			"this can happen due to either no clouds present or sensor error");

		case metaf::CloudGroup::Amount::NONE_CLR:
		return("Clear sky: "
			"no cloud layers are detected at or below 12000 feet (3700 meters) (US) "
			"or 25000 feet (7600 meters) (Canada); "
			"indicates that station is at least partly automated");

		case metaf::CloudGroup::Amount::NONE_SKC:
		return("Clear sky: "
			"In North America indicates report producted by human rather than "
			"automatic weather station");

		case metaf::CloudGroup::Amount::FEW:
		return("Few clouds (1/8 to 2/8 sky covered)");

		case metaf::CloudGroup::Amount::SCATTERED:
		return("Scattered clouds (3/8 to 4/8 sky covered)");

		case metaf::CloudGroup::Amount::BROKEN:
		return("Broken clouds (5/8 to 7/8 sky covered)");

		case metaf::CloudGroup::Amount::OVERCAST:
		return("Overcast (8/8 sky covered)");

		case metaf::CloudGroup::Amount::OBSCURED:
		return("Sky obscured");

		default: 
		return("Unknown cloud amound");
	}
}

std::string_view GroupVisitorExplain::cloudTypeToString(metaf::CloudGroup::Type type) {
	switch (type) {
		case metaf::CloudGroup::Type::NONE:
		return(std::string_view());

		case metaf::CloudGroup::Type::NOT_REPORTED:
		return("not reported");

		case metaf::CloudGroup::Type::TOWERING_CUMULUS:
		return("towering cumulus");

		case metaf::CloudGroup::Type::CUMULONIMBUS:
		return("cumulonimbus");

		default: 
		return("undefined");
	}
}

std::string_view GroupVisitorExplain::weatherQualifierToString(
	metaf::WeatherGroup::Qualifier qualifier)
{
	switch (qualifier) {
		case metaf::WeatherGroup::Qualifier::NONE:	  	return(std::string_view());
		case metaf::WeatherGroup::Qualifier::RECENT: 	return(std::string_view());
		case metaf::WeatherGroup::Qualifier::VICINITY: 	return(std::string_view());
		case metaf::WeatherGroup::Qualifier::LIGHT: 	return("light");
		case metaf::WeatherGroup::Qualifier::MODERATE:	return("moderate");
		case metaf::WeatherGroup::Qualifier::HEAVY:		return("heavy");
		default: return("[unknown intensity/proximity qualifier];");
	}
}

std::string_view GroupVisitorExplain::weatherDescriptorToString(
	metaf::WeatherGroup::Descriptor descriptor)
{
	switch(descriptor) {
		case metaf::WeatherGroup::Descriptor::NONE:			return(std::string());
		case metaf::WeatherGroup::Descriptor::SHALLOW:		return("shallow");
		case metaf::WeatherGroup::Descriptor::PARTIAL:		return("partial");
		case metaf::WeatherGroup::Descriptor::PATCHES:		return("patches of ");
		case metaf::WeatherGroup::Descriptor::LOW_DRIFTING: return("low drifting");
		case metaf::WeatherGroup::Descriptor::BLOWING:		return("blowing");
		case metaf::WeatherGroup::Descriptor::SHOWERS:		return(std::string());
		case metaf::WeatherGroup::Descriptor::THUNDERSTORM:	return(std::string());
		case metaf::WeatherGroup::Descriptor::FREEZING:		return("freezing");
		default: return("[uknown weather descriptor];");
	}
}

std::string_view GroupVisitorExplain::weatherPhenomenaToString(
	metaf::WeatherGroup::Weather weather)
{
	switch (weather) {
			case metaf::WeatherGroup::Weather::NOT_REPORTED:	return("not reported");
			case metaf::WeatherGroup::Weather::DRIZZLE:			return("drizzle");
			case metaf::WeatherGroup::Weather::RAIN:			return("rain");
			case metaf::WeatherGroup::Weather::SNOW:			return("snow");
			case metaf::WeatherGroup::Weather::SNOW_GRAINS:		return("snow grains");
			case metaf::WeatherGroup::Weather::ICE_CRYSTALS:	return("ice crystals");
			case metaf::WeatherGroup::Weather::ICE_PELLETS:		return("ice pellets");
			case metaf::WeatherGroup::Weather::HAIL:			return("hail");
			case metaf::WeatherGroup::Weather::SMALL_HAIL:		return("small hail (graupel)");
			case metaf::WeatherGroup::Weather::UNDETERMINED:	return("undetermined precipitation");
			case metaf::WeatherGroup::Weather::MIST:			return("mist");
			case metaf::WeatherGroup::Weather::FOG:				return("fog");
			case metaf::WeatherGroup::Weather::SMOKE:			return("smoke");
			case metaf::WeatherGroup::Weather::VOLCANIC_ASH:	return("volcanic ash");
			case metaf::WeatherGroup::Weather::DUST:			return("dust");
			case metaf::WeatherGroup::Weather::SAND:			return("sand");
			case metaf::WeatherGroup::Weather::HAZE:			return("haze");
			case metaf::WeatherGroup::Weather::SPRAY:			return("spray");
			case metaf::WeatherGroup::Weather::DUST_WHIRLS:		return("dust or sand whirls");
			case metaf::WeatherGroup::Weather::SQUALLS:			return("squalls");
			case metaf::WeatherGroup::Weather::FUNNEL_CLOUD:	return("funnel cloud");
			case metaf::WeatherGroup::Weather::SANDSTORM:		return("sand storm");
			case metaf::WeatherGroup::Weather::DUSTSTORM:		return("dust storm");
			default: return("[unknown weather phenomena]");
	}

}

std::string_view GroupVisitorExplain::specialWeatherPhenomenaToString(
	const metaf::WeatherGroup & group)
{
	using WeatherVector = std::vector<metaf::WeatherGroup::Weather>;
	using SpecialWeatherPhenomena = std::tuple<	
		metaf::WeatherGroup::Qualifier, 
		metaf::WeatherGroup::Descriptor, 
		WeatherVector,
		std::string_view
		>;

	static const std::vector <SpecialWeatherPhenomena> 
		specialWeatherPhenomena = {
			SpecialWeatherPhenomena(
				metaf::WeatherGroup::Qualifier::VICINITY,
				metaf::WeatherGroup::Descriptor::SHOWERS,
				{},
				"precipitation in vicinity (up to 10 miles away)"),
			SpecialWeatherPhenomena(
				metaf::WeatherGroup::Qualifier::NONE,
				metaf::WeatherGroup::Descriptor::NONE,
				{ metaf::WeatherGroup::Weather::ICE_CRYSTALS },
				"ice crystals (diamond dust)"),
			SpecialWeatherPhenomena(
				metaf::WeatherGroup::Qualifier::NONE,
				metaf::WeatherGroup::Descriptor::NONE,
				{ metaf::WeatherGroup::Weather::DUST },
				"widespread dust"),
			SpecialWeatherPhenomena(
				metaf::WeatherGroup::Qualifier::NONE,
				metaf::WeatherGroup::Descriptor::NONE,
				{ metaf::WeatherGroup::Weather::UNDETERMINED },
				"undetermined precipitation (automated system cannot identify precipitation)"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherGroup::Qualifier::NONE,
				metaf::WeatherGroup::Descriptor::SHALLOW,
				{ metaf::WeatherGroup::Weather::FOG },
				"shallow fog (ground fog)"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherGroup::Qualifier::NONE,
				metaf::WeatherGroup::Descriptor::PARTIAL,
				{ metaf::WeatherGroup::Weather::FOG },
				"fog covering part of the location"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherGroup::Qualifier::NONE,
				metaf::WeatherGroup::Descriptor::PATCHES,
				{ metaf::WeatherGroup::Weather::FOG },
				"patches of fog randomly covering the location"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherGroup::Qualifier::NONE,
				metaf::WeatherGroup::Descriptor::FREEZING,
				{ metaf::WeatherGroup::Weather::FOG },
				"fog at freezing temperture (may or may not deposit rime)"
			),
			SpecialWeatherPhenomena(
				metaf::WeatherGroup::Qualifier::HEAVY,
				metaf::WeatherGroup::Descriptor::NONE,
				{ metaf::WeatherGroup::Weather::FUNNEL_CLOUD },
				"tornado or waterspout"
			)
	};

	for (const auto w : specialWeatherPhenomena) {
		if (group.qualifier() == std::get<metaf::WeatherGroup::Qualifier>(w) &&
			group.descriptor() == std::get<metaf::WeatherGroup::Descriptor>(w) &&
			group.weather() == std::get<WeatherVector>(w))
		{
				return(std::get<std::string_view>(w));
		}
	}
	return(std::string());
}

std::string_view GroupVisitorExplain::rvrTrendToString(metaf::RunwayVisualRangeGroup::Trend trend) {
	switch(trend) {
		case metaf::RunwayVisualRangeGroup::Trend::NONE:		return(std::string_view());
		case metaf::RunwayVisualRangeGroup::Trend::NOT_REPORTED:return("not reported");
		case metaf::RunwayVisualRangeGroup::Trend::UPWARD:		return("upward");
		case metaf::RunwayVisualRangeGroup::Trend::NEUTRAL:		return("neutral");
		case metaf::RunwayVisualRangeGroup::Trend::DOWNWARD:	return("downward");
		default: 												return("[unknown trend]");
	}
}

std::string_view GroupVisitorExplain::runwayStateDepositsToString(
	metaf::RunwayStateGroup::Deposits deposits)
{
	switch(deposits) {
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
		return("[unknown runway deposits]");
	}
}

std::string_view GroupVisitorExplain::runwayStateExtentToString(
	metaf::RunwayStateGroup::Extent extent)
{
	switch(extent) {
		case metaf::RunwayStateGroup::Extent::NOT_REPORTED:
		return("not reported");

		case metaf::RunwayStateGroup::Extent::NONE:
		return("none");

		case metaf::RunwayStateGroup::Extent::LESS_THAN_10_PERCENT:
		return("&lt;10 percent");

		case metaf::RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT:
		return("11 to 25 percent");

		case metaf::RunwayStateGroup::Extent::FROM_26_TO_50_PERCENT:
		return("26 to 50 percent");

		case metaf::RunwayStateGroup::Extent::MORE_THAN_51_PERCENT:
		return("&gt;51 percent");

		case metaf::RunwayStateGroup::Extent::RESERVED_3:
		return("[reserved_extent_value 3]");

		case metaf::RunwayStateGroup::Extent::RESERVED_4:
		return("[reserved_extent_value 4]");

		case metaf::RunwayStateGroup::Extent::RESERVED_6:
		return("[reserved_extent_value 6]");

		case metaf::RunwayStateGroup::Extent::RESERVED_7:
		return("[reserved_extent_value 7]");

		case metaf::RunwayStateGroup::Extent::RESERVED_8:
		return("[reserved_extent_value 8]");

		default:
		return("[unknown extent value]");
	}
}

std::string_view GroupVisitorExplain::colourCodeToString(metaf::ColourCodeGroup::Code code) {
	switch(code) {
		case metaf::ColourCodeGroup::Code::BLUE:		return("BLUE");
		case metaf::ColourCodeGroup::Code::WHITE:		return("WHITE");
		case metaf::ColourCodeGroup::Code::GREEN:		return("GREEN");
		case metaf::ColourCodeGroup::Code::YELLOW1:		return("YELLOW1");
		case metaf::ColourCodeGroup::Code::YELLOW2:		return("YELLOW2");
		case metaf::ColourCodeGroup::Code::AMBER:		return("AMBER");
		case metaf::ColourCodeGroup::Code::RED:			return("RED");
		default:										return("[unknown colour code]");
	}
}

unsigned int GroupVisitorExplain::colourCodeVisibility(metaf::ColourCodeGroup::Code code) {
	switch(code) {
		case metaf::ColourCodeGroup::Code::BLUE:	return(8000);
		case metaf::ColourCodeGroup::Code::WHITE:	return(5000);
		case metaf::ColourCodeGroup::Code::GREEN:	return(3700);
		case metaf::ColourCodeGroup::Code::YELLOW1: return(2500);
		case metaf::ColourCodeGroup::Code::YELLOW2:	return(1600);
		case metaf::ColourCodeGroup::Code::AMBER:	return(800);
		case metaf::ColourCodeGroup::Code::RED:		return(800);
		default:									return(0);
	}
}

unsigned int GroupVisitorExplain::colourCodeCeiling(metaf::ColourCodeGroup::Code code) {
	switch(code) {
		case metaf::ColourCodeGroup::Code::BLUE:	return(2500);
		case metaf::ColourCodeGroup::Code::WHITE:	return(1500);
		case metaf::ColourCodeGroup::Code::GREEN:	return(700);
		case metaf::ColourCodeGroup::Code::YELLOW1:	return(500);
		case metaf::ColourCodeGroup::Code::YELLOW2:	return(300);
		case metaf::ColourCodeGroup::Code::AMBER:	return(200);
		case metaf::ColourCodeGroup::Code::RED:		return(200);
		default:									return(0);
	}
}

std::string_view GroupVisitorExplain::precipitationGroupTypeToString(
	metaf::PrecipitationGroup::Type type)
{
	switch (type) {
		case metaf::PrecipitationGroup::Type::TOTAL_PRECIPITATION_HOURLY:
		return("total precipitation for the past hour");

		case metaf::PrecipitationGroup::Type::SNOW_DEPTH_ON_GROUND:
		return("snow depth on ground");

		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_OR_6_HOURLY:
		return("water equivalent of frozen precipitation for the last 3 or 6 hours");

		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_HOURLY:
		return("water equivalent of frozen precipitation for the last 3 hours");

		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_6_HOURLY:
		return("water equivalent of frozen precipitation for the last 6 hours");
	
		case metaf::PrecipitationGroup::Type::FROZEN_PRECIP_24_HOURLY:
		return("water equivalent of frozen precipitation for the last 24 hours");

		case metaf::PrecipitationGroup::Type::SNOW_6_HOURLY:
		return("snowfall for the last 6 hours");

		case metaf::PrecipitationGroup::Type::WATER_EQUIV_OF_SNOW_ON_GROUND:
		return("water equivalent of snow on ground");

		case metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_HOUR:
		return("ice accretion for the last hour");

		case metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_3_HOURS:
		return("ice accretion for the last 3 hours");

		case metaf::PrecipitationGroup::Type::ICE_ACCRETION_FOR_LAST_6_HOURS:
		return("ice accretion for the last 6 hours");

		default:
		return("[unknown precipitation type]");
	}
}

std::string_view GroupVisitorExplain::layerForecastGroupTypeToString(
	metaf::LayerForecastGroup::Type type)
{
	switch(type) {
		case metaf::LayerForecastGroup::Type::ICING_TRACE_OR_NONE:
		return("Trace icing or no icing");

		case metaf::LayerForecastGroup::Type::ICING_LIGHT_MIXED:
		return("Light mixed icing");
		
		case metaf::LayerForecastGroup::Type::ICING_LIGHT_RIME_IN_CLOUD:
		return("Light rime icing in cloud");
		
		case metaf::LayerForecastGroup::Type::ICING_LIGHT_CLEAR_IN_PRECIPITATION:
		return("Light clear icing in precipitation");
		
		case metaf::LayerForecastGroup::Type::ICING_MODERATE_MIXED:
		return("Moderate mixed icing");
		
		case metaf::LayerForecastGroup::Type::ICING_MODERATE_RIME_IN_CLOUD:
		return("Moderate rime icing in cloud");
		
		case metaf::LayerForecastGroup::Type::ICING_MODERATE_CLEAR_IN_PRECIPITATION:
		return("Moderate clear icing in precipitation");
		
		case metaf::LayerForecastGroup::Type::ICING_SEVERE_MIXED:
		return("Severe mixed icing");
		
		case metaf::LayerForecastGroup::Type::ICING_SEVERE_RIME_IN_CLOUD:
		return("Severe rime icing in cloud");
		
		case metaf::LayerForecastGroup::Type::ICING_SEVERE_CLEAR_IN_PRECIPITATION:
		return("Severe clear icing in precipitation");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_NONE:
		return("No turbulence");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_LIGHT:
		return("Light turbulence");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASSIONAL:
		return("Occasional moderate turbulence in clear air");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT:
		return("Frequent moderate turbulence in clear air");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLOUD_OCCASSIONAL:
		return("Occasional moderate turbulence in cloud");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLOUD_FREQUENT:
		return("Frequent moderate turbulence in cloud");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASSIONAL:
		return("Occasional severe turbulence in clear air");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT:
		return("Frequent severe turbulence in clear air");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLOUD_OCCASSIONAL:
		return("Occasional severe turbulence in cloud");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLOUD_FREQUENT:
		return("Frequent severe turbulence in cloud");
		
		case metaf::LayerForecastGroup::Type::TURBULENCE_EXTREME:
		return("Extreme turbulence");
		
		default:
		return("[unknown atmospheric layer type]");
	}
}


std::string_view GroupVisitorExplain::pressureTendencyTypeToString(
	metaf::PressureTendencyGroup::Type type)
{
	switch(type) {
		case metaf::PressureTendencyGroup::Type::INCREASING_THEN_DECREASING:
		return("increasing, then decreasing");
		
		case metaf::PressureTendencyGroup::Type::INCREASING_MORE_SLOWLY:
		return("increasing, then steady, or increasing then increasing more slowly");

		case metaf::PressureTendencyGroup::Type::INCREASING:
		return("increasing steadily or unsteadily");

		case metaf::PressureTendencyGroup::Type::INCREASING_MORE_RAPIDLY:
		return("decreasing or steady, then increasing; or increasing then increasing more rapidly");
		
		case metaf::PressureTendencyGroup::Type::STEADY:
		return("steady");
		
		case metaf::PressureTendencyGroup::Type::DECREASING_THEN_INCREASING:
		return("decreasing, then increasing");
		
		case metaf::PressureTendencyGroup::Type::DECREASING_MORE_SLOWLY:
		return("decreasing then steady; or decreasing then decreasing more slowly");
		
		case metaf::PressureTendencyGroup::Type::DECREASING:
		return("decreasing steadily or unsteadily");
		
		case metaf::PressureTendencyGroup::Type::DECREASING_MORE_RAPIDLY:
		return("steady or increasing, then decreasing; or decreasing then decreasing more rapidly");
		
		default:
		return("[unknown pressure tendency]");
	}

}

std::string_view GroupVisitorExplain::pressureTendencyTrendToString(
	metaf::PressureTendencyGroup::Trend trend)
{
	switch(trend) {
		case metaf::PressureTendencyGroup::Trend::HIGHER:
		return("higher than");

		case metaf::PressureTendencyGroup::Trend::HIGHER_OR_SAME:
		return("higher or the same as");

		case metaf::PressureTendencyGroup::Trend::SAME:
		return("same as");

		case metaf::PressureTendencyGroup::Trend::LOWER_OR_SAME:
		return("lower or the same as");

		case metaf::PressureTendencyGroup::Trend::LOWER:
		return("lower than");

		default:
		return("[unknown pressure trend]");
	}	
}

std::string_view GroupVisitorExplain::cloudTypeToString(metaf::CloudTypesGroup::Type type) {
	switch(type) {
		case metaf::CloudTypesGroup::Type::CUMULONIMBUS:
		return("cumulonimbus");

		case metaf::CloudTypesGroup::Type::TOWERING_CUMULUS:
		return("towering cumulus");

		case metaf::CloudTypesGroup::Type::CUMULUS:
		return("cumulus");

		case metaf::CloudTypesGroup::Type::CUMULUS_FRACTUS:
		return("cumulus fractus");

		case metaf::CloudTypesGroup::Type::STRATOCUMULUS:
		return("stratocumulus");

		case metaf::CloudTypesGroup::Type::NIMBOSTRATUS:
		return("nimbostratus");

		case metaf::CloudTypesGroup::Type::STRATUS:
		return("stratus");

		case metaf::CloudTypesGroup::Type::STRATUS_FRACTUS:
		return("stratus fractus");

		case metaf::CloudTypesGroup::Type::ALTOSTRATUS:
		return("altostratus");
		
		case metaf::CloudTypesGroup::Type::ALTOCUMULUS:
		return("altocumulus");

		case metaf::CloudTypesGroup::Type::ALTOCUMULUS_CASTELLANUS:
		return("altocumulus castellanus");
		
		case metaf::CloudTypesGroup::Type::CIRRUS:
		return("cirrus");
		
		case metaf::CloudTypesGroup::Type::CIRROSTRATUS:
		return("cirrostratus");
		
		case metaf::CloudTypesGroup::Type::CIRROCUMULUS:
		return("cirrocumulus");
		
		default:
		return("[unknown cloud type]");
	}
}

std::string_view GroupVisitorExplain::cloudLowLayerToString(metaf::CloudLayersGroup::LowLayer lowLayer) {
	switch(lowLayer) {
		case metaf::CloudLayersGroup::LowLayer::NONE:
		return("No low layer clouds");

		case metaf::CloudLayersGroup::LowLayer::CU_HU_CU_FR:
		return("Cumulus clouds showing little vertical extent "
			"(Cumulus humilis or Cumulus fractus of dry weather or both)");

		case metaf::CloudLayersGroup::LowLayer::CU_MED_CU_CON:
		return("Cumulus clouds showing moderate or significant vertical extent "
			"(Cumulus mediocris or Cumulus congestus, with or without Cumulus "
			"humilis or Cumulus fractus or stratocumulus, all having their bases on "
			"the same level)");

		case metaf::CloudLayersGroup::LowLayer::CB_CAL:
		return("Cumulonimbus clouds without fibrous or striated parts at summit "
			"(Cumulonimbus calvus with or without Cumulus, Stratocumulus or Stratus)");

		case metaf::CloudLayersGroup::LowLayer::SC_CUGEN:
		return("Stratocumulus clouds resulting from the spreading out of Cumulus "
			"(Stratocumulus cumulogenitus; Cumulus may also be present)");

		case metaf::CloudLayersGroup::LowLayer::SC_NON_CUGEN:
		return("Stratocumulus clouds not resulting from the spreading out of Cumulus "
			"(Stratocumulus non-cumulogenitus)");

		case metaf::CloudLayersGroup::LowLayer::ST_NEB_ST_FR:
		return("Stratus clouds which consist of a continuous single sheet or layer "
			"with a fairly uniform base, or transitory stage during the formation "
			"or the dissipation of such clouds "
			"(Stratus nebulosus or Stratus fractus of dry weather, or both)");

		case metaf::CloudLayersGroup::LowLayer::ST_FR_CU_FR_PANNUS:
		return("Ragged grey clouds which form below precipitating clouds "
			"(Stratus fractus or Cumulus fractus of wet weather, or both (pannus) )");

		case metaf::CloudLayersGroup::LowLayer::CU_SC_NON_CUGEN_DIFFERENT_LEVELS:
		return("Cumulus and Stratocumulus not formed by spreading of Cumulus "
			"with bases at different levels");

		case metaf::CloudLayersGroup::LowLayer::CB_CAP:
		return("Cumulonimbus clouds with fibrous or striated summits, often with an anvil "
			"(Cumulonimbus capillatus or Cumulonimbus capillatus incus)");

		case metaf::CloudLayersGroup::LowLayer::NOT_OBSERVABLE:
		return("Clouds are not observable due to fog, blowing dust or sand, "
			"or other similar phenomena");

		default:
		return("[unknown low-layer cloud type]");
	}
}

std::string_view GroupVisitorExplain::cloudMidLayerToString(metaf::CloudLayersGroup::MidLayer midLayer) {
	switch(midLayer) {
		case metaf::CloudLayersGroup::MidLayer::NONE:
		return("No mid-layer clouds");

		case metaf::CloudLayersGroup::MidLayer::AS_TR:
		return("A veil of greyish or bluish colour translucent enough "
			"to reveal the position of the Sun or Moon "
			"(Altostratus translucidus)");
		
		case metaf::CloudLayersGroup::MidLayer::AS_OP_NS:
		return("A veil of a darker grey or a darker bluish grey dense enough to "
			"completely mask the Sun or Moon "
			"(Altostratus opacus or Nimbostratus)");
		
		case metaf::CloudLayersGroup::MidLayer::AC_TR:
		return("Altocumulus (mackerel sky) clouds in patches or sheets at the same "
			"level or in a single layer "
			"(Altocumulus translucidus at a single level)");
		
		case metaf::CloudLayersGroup::MidLayer::AC_TR_LEN_PATCHES:
		return("Patches, often lenticular (lens or almond-shaped), of Altocumulus "
			"translucidus, continually changing and occurring at one or more levels");
		
		case metaf::CloudLayersGroup::MidLayer::AC_TR_AC_OP_SPREADING:
		return("Altocumulus translucidus in bands, or one or more layers of "
			"Altocumulus translucidus or Altocumulus opacus, "
			"progressively invading the sky");
		
		case metaf::CloudLayersGroup::MidLayer::AC_CUGEN_AC_CBGEN:
		return("Altocumulus resulting generally from the spreading "
			"out of the summits of Cumulus; or Alcocumulus clouds acompanying Cumulonimbus "
			"(Altocumulus cumulogenitus or Altocumulus cumulonimbogenitus)");
		
		case metaf::CloudLayersGroup::MidLayer::AC_DU_AC_OP_AC_WITH_AS_OR_NS:
		return("Altocumulus duplicatus, or Altocumulus opacus in a single layer, not "
			"progressively invading the sky, or Altocumulus with Altostratus or "
			"Nimbostratus.");
		
		case metaf::CloudLayersGroup::MidLayer::AC_CAS_AC_FLO:
		return("Turrets at appear to be arranged in lines with a common horizontal "
			"base or scattered tufts with rounded and slightly bulging upper parts"
			"(Altocumulus castellanus or Altocumulus floccus)");
		
		case metaf::CloudLayersGroup::MidLayer::AC_OF_CHAOTIC_SKY:
		return("Sky of chaotic, heavy and stagnant appearance, which consist of "
			"superposed, more or less broken cloud sheets of ill-defined species "
			"or varieties");

		case metaf::CloudLayersGroup::MidLayer::NOT_OBSERVABLE:
		return("Clouds are not observable due to fog, blowing dust or sand, "
			"or other similar phenomena or because of a continuous layer of lower clouds");

		default:
		return("[unknown mid-layer cloud type]");
	}
}

std::string_view GroupVisitorExplain::cloudHighLayerToString(metaf::CloudLayersGroup::HighLayer highLayer) {
	switch(highLayer) {
		case metaf::CloudLayersGroup::HighLayer::NONE:
		return("No high-layer clouds");

		case metaf::CloudLayersGroup::HighLayer::CI_FIB_CI_UNC:
		return("Nearly straight or more or less curved filaments; more rarely, "
			"they are shaped like commas topped with either a hook or a tuft "
			"that is not rounded "
			"(Cirrus fibratus and sometimes Cirrus uncinus, not "
			"progressively invading the sky)");

		case metaf::CloudLayersGroup::HighLayer::CI_SPI_CI_CAS_CI_FLO:
		return("Cirrus spissatus, in patches or entangled sheaves, that usually "
			"do not increase and sometimes appear to be the remains of the upper "
			"part of a Cumulonimbus; or Cirrus clouds with small fibrous turrets "
			"rising from common base; or more or less isolated tufts, often with trails "
			"(Cirrus spissatus or Cirrus castellanus or Cirrus floccus)");

		case metaf::CloudLayersGroup::HighLayer::CI_SPI_CBGEN:
		return("Cirrus clouds originated from a Cumulonimbus cloud(s) "
			"(Cirrus spissatus cumulonimbogenitus)");

		case metaf::CloudLayersGroup::HighLayer::CI_FIB_CI_UNC_SPREADING:
		return("Cirrus uncinus, Cirrus fibratus or both, progressively invading the sky; "
			"they generally thicken as a whole");

		case metaf::CloudLayersGroup::HighLayer::CI_CS_LOW_ABOVE_HORIZON:
		return("Cirrus (often in bands) and Cirrostratus, or Cirrostratus alone, "
			"progressively invading the sky; they generally thicken as a whole, "
			"but the continuous veil does not reach 45° above the horizon");

		case metaf::CloudLayersGroup::HighLayer::CI_CS_HIGH_ABOVE_HORIZON:
		return("Cirrus (often in bands) and Cirrostratus, or Cirrostratus alone, "
			"progressively invading the sky; they generally thicken as a whole; "
			"the continuous veil extends more than 45° above the horizon, without "
			"the sky being totally covered");

		case metaf::CloudLayersGroup::HighLayer::CS_NEB_CS_FIB_COVERING_ENTIRE_SKY:
		return("Light, uniform and nebulous veil showing no distinct details"
			"or a white and fibrous veil with more or less clear-cut striations, "
			"covering the whole sky"
			"(Cirrostratus nebulosus or Cirrostratus fibratus covering the whole sky)");

		case metaf::CloudLayersGroup::HighLayer::CS:
		return("A veil of Cirrostratus that is not (or no longer) invading the sky "
			"progressively and that does not completely cover the whole sky");

		case metaf::CloudLayersGroup::HighLayer::CC:
		return("Cirrocumulus alone, or predominant among the high-layer clouds; when "
			"alone, its elements are frequently grouped into more or less extensive "
			"patches with very characteristic small wavelets");

		case metaf::CloudLayersGroup::HighLayer::NOT_OBSERVABLE:
		return("Clouds are not observable due to fog, blowing dust or sand, "
			"or other similar phenomena or because of a continuous layer of lower clouds");

		default:
		return("[unknown high-layer cloud type]");
	}
}

std::string GroupVisitorExplain::roundTo(float number, size_t digitsAfterDecimalPoint) {
	static const char decimalPoint = '.';
	std::string numStr = std::to_string(number);
	const auto decimalPointPos = numStr.find(decimalPoint);
	if (decimalPointPos == std::string::npos) return(numStr);
	const auto decimalsAfterPoint = numStr.length() - decimalPointPos;
	if (decimalsAfterPoint > digitsAfterDecimalPoint) {
		return(numStr.substr(0, decimalPointPos + digitsAfterDecimalPoint + 1));
	}
	return(numStr);
}

///////////////////////////////////////////////////////////////////////////////

static std::string result;

void addResult(const std::string & group, const std::string & explanation) {
	static const char delimiter = '|';
	result += group;
	result.push_back(delimiter);
	result += explanation;
	result.push_back(delimiter);
}

extern "C" const char * EMSCRIPTEN_KEEPALIVE explain(const char * input) {
	const auto parseResult = metaf::Parser::parse(std::string(input));
	addResult("", "Detected report type: "s + 
		std::string(GroupVisitorExplain::reportTypeToString(parseResult.reportType)));
	if (parseResult.error != metaf::Parser::Error::NONE) {
		addResult("", "Parsing error: "s + 
			std::string(GroupVisitorExplain::reportErrorToString(parseResult.error)));
	}
	GroupVisitorExplain visitor;
	for (const auto groupInfo : parseResult.groups) {
		addResult(groupInfo.rawString, 
			visitor.visit(groupInfo.group));
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
