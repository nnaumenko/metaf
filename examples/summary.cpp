/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

/// @file 
/// @brief This example parses pair of METAR and TAF report and produces a 
/// summary of current weather

#include <emscripten/bind.h>

using namespace emscripten;

#include "metaf.h"

static const int valueNotSpecified = -250;

enum Cloud {
	NOT_SPECIFIED = -1,
	CLEAR = 0,
	MOSTLY_CLEAR = 1,
	MOSTLY_CLOUDY = 2,
	OVERCAST = 3
};

enum class Weather {
	NOT_SPECIFIED = 0,
	//Precipitation in vicinity
	PRECIPITATION_IN_VICINITY = 1,
	THUNDERSTORM_IN_VICINITY = 2,
	FOG_IN_VICINITY = 3,
	VOLCANIC_ASH_IN_VICINITY = 4,
	DUST_WHIRLS_IN_VICINITY = 5,
	BLOWING_SNOW_IN_VICINITY = 6,
	BLOWING_DUST_IN_VICINITY = 7,
	BLOWING_SAND_IN_VICINITY = 8,
	SANDSTORM_IN_VICINITY = 9,
	DUSTSTORM_IN_VICINITY = 10,
	//Obscuration, drifting/blowing
	DRIFTING_SAND = 11,
	DRIFTING_DUST = 12,
	DRIFTING_SNOW = 13,
	BLOWING_SAND = 14,
	BLOWING_DUST = 15,
	BLOWING_SNOW = 16,
	BLOWING_SPRAY = 17,
	//Obscuration, fog
	FOG = 18,
	FOG_SHALLOW = 19,
	FOG_PARTIAL = 20,
	FOG_PATCHES = 21,
	FOG_FREEZING = 22,
	//Obscuration, other
	ICE_CRYSTALS = 23,
	MIST = 24,
	SMOKE = 25,
	VOLCANIC_ASH = 26,
	WIDESPREAD_DUST = 27,
	SAND = 28,
	HAZE = 29,
	//Precipitation
	DRIZZLE_LIGHT = 30,
	DRIZZLE_MODERATE = 31,
	DRIZZLE_HEAVY = 32,
	RAIN_LIGHT = 33,
	RAIN_MODERATE = 34,
	RAIN_HEAVY = 35,
	SNOW_LIGHT = 36,
	SNOW_MODERATE = 37,
	SNOW_HEAVY = 38,
	ICE_PELLETS_LIGHT = 39,
	ICE_PELLETS_MODERATE = 40,
	ICE_PELLETS_HEAVY = 41,
	SNOW_GRAINS_LIGHT = 42,
	SNOW_GRAINS_MODERATE = 43,
	SNOW_GRAINS_HEAVY = 44,
	GRAUPEL = 45,
	HAIL = 46,
	UNDETERMINED_LIGHT = 47,
	UNDETERMINED_MODERATE = 48,
	UNDETERMINED_HEAVY = 49,
	//Showery precipitation
	RAIN_SHOWERS_LIGHT = 50,
	RAIN_SHOWERS_MODERATE = 51,
	RAIN_SHOWERS_HEAVY = 52,
	SNOW_SHOWERS_LIGHT = 53,
	SNOW_SHOWERS_MODERATE = 54,
	SNOW_SHOWERS_HEAVY = 55,
	ICE_PELLETS_SHOWERS_LIGHT = 56,
	ICE_PELLETS_SHOWERS_MODERATE = 57,
	ICE_PELLETS_SHOWERS_HEAVY = 58,
	//Freezing precipitation
	DRIZZLE_FREEZING_LIGHT = 59,
	DRIZZLE_FREEZING_MODERATE = 60,
	DRIZZLE_FREEZING_HEAVY = 61,
	RAIN_FREEZING_LIGHT = 62,
	RAIN_FREEZING_MODERATE = 63,
	RAIN_FREEZING_HEAVY = 64,
	//Thunderstorms
	THUNDERSTORM = 65,
	//Miscellaneous
	DUST_WHIRLS = 66,
	SQUALLS = 67,
	FUNNEL_CLOUD = 68,
	TORNADO = 69,
	SANDSTORM = 70,
	SANDSTORM_HEAVY = 71,
	DUSTSTORM = 72,
	DUSTSTORM_HEAVY = 73
};

struct CurrentWeather {
	CurrentWeather() = default;
	int windDirection = valueNotSpecified;
	bool isWindVariable = false;
	int windSpeed = valueNotSpecified;
	int gustSpeed = valueNotSpecified;
	int visibility = valueNotSpecified;
	int cloud = Cloud::NOT_SPECIFIED;
	bool isStormClouds = false;
	std::vector<int> weather; //actually std::vector<Weather>
	int airTemperature = valueNotSpecified;
	int perceivedTemperature = valueNotSpecified;
	int airTemperatureHigh = valueNotSpecified;
	int airTemperatureLow = valueNotSpecified;
	int relativeHumidity = valueNotSpecified;
	int atmosphericPressure = valueNotSpecified;
};

using GroupVector = std::vector<metaf::Group>;

metaf::Temperature::Unit temperatureUnit(bool isImperialUnit) {
	return (isImperialUnit ? metaf::Temperature::Unit::F : metaf::Temperature::Unit::C);
}

metaf::Pressure::Unit pressureUnit(bool isImperialUnit) {
	return (isImperialUnit ? 
		metaf::Pressure::Unit::INCHES_HG : metaf::Pressure::Unit::HECTOPASCAL);
}

metaf::Distance::Unit visibilityUnit(bool isImperialUnit) {
	return (isImperialUnit ? 
		metaf::Distance::Unit::STATUTE_MILES : metaf::Distance::Unit::METERS);
}

metaf::Speed::Unit speedUnit(bool isImperialUnit) {
	return (isImperialUnit ? 
		metaf::Speed::Unit::MILES_PER_HOUR : metaf::Speed::Unit::KILOMETERS_PER_HOUR);
}

Cloud cloudFromCloudGroupAmount(Cloud prev, metaf::CloudGroup::Amount newAmount) {
	switch(newAmount) {
		case metaf::CloudGroup::Amount::NCD:
		case metaf::CloudGroup::Amount::NONE_CLR:
		case metaf::CloudGroup::Amount::NONE_SKC:
		if (prev == Cloud::NOT_SPECIFIED) return(Cloud::CLEAR);
		return(prev);

		case metaf::CloudGroup::Amount::NSC:
		case metaf::CloudGroup::Amount::FEW:
		case metaf::CloudGroup::Amount::SCATTERED:
		if (prev != Cloud::OVERCAST || prev != Cloud::MOSTLY_CLOUDY) return(Cloud::MOSTLY_CLEAR);
		return(prev);

		case metaf::CloudGroup::Amount::BROKEN:
		if (prev != Cloud::OVERCAST) return(Cloud::MOSTLY_CLOUDY);
		return(prev);

		case metaf::CloudGroup::Amount::OVERCAST:
		case metaf::CloudGroup::Amount::OBSCURED:
		return(Cloud::OVERCAST);

		default: return(prev);
	}
}

Weather phenomena(metaf::WeatherGroup::Qualifier qualifier,
	Weather light,
	Weather moderate,
	Weather heavy)
{
	switch (qualifier) {
		case metaf::WeatherGroup::Qualifier::LIGHT:		return(light);
		case metaf::WeatherGroup::Qualifier::MODERATE:	return(moderate);
		case metaf::WeatherGroup::Qualifier::HEAVY:		return(heavy);
		default: 										return(moderate);
	}
}

Weather weatherRain(metaf::WeatherGroup::Qualifier qualifier,
	metaf::WeatherGroup::Descriptor descriptor)
{
	switch (descriptor) {
		case metaf::WeatherGroup::Descriptor::FREEZING:
		return(phenomena(qualifier,
			Weather::RAIN_FREEZING_LIGHT, 
			Weather::RAIN_FREEZING_MODERATE, 
			Weather::RAIN_FREEZING_HEAVY));

		case metaf::WeatherGroup::Descriptor::SHOWERS:
		return(phenomena(qualifier,
			Weather::RAIN_SHOWERS_LIGHT, 
			Weather::RAIN_SHOWERS_MODERATE, 
			Weather::RAIN_SHOWERS_HEAVY));

		case metaf::WeatherGroup::Descriptor::NONE:
		return(phenomena(qualifier, 
			Weather::RAIN_LIGHT, 
			Weather::RAIN_MODERATE, 
			Weather::RAIN_HEAVY));

		default:
		return(phenomena(qualifier, 
			Weather::RAIN_LIGHT, 
			Weather::RAIN_MODERATE, 
			Weather::RAIN_HEAVY));
	}
}

Weather weatherSnow(metaf::WeatherGroup::Qualifier qualifier,
	metaf::WeatherGroup::Descriptor descriptor)
{
	switch (descriptor) {
		case metaf::WeatherGroup::Descriptor::SHOWERS:
		return(phenomena(qualifier,
			Weather::SNOW_SHOWERS_LIGHT, 
			Weather::SNOW_SHOWERS_MODERATE, 
			Weather::SNOW_SHOWERS_HEAVY));

		case metaf::WeatherGroup::Descriptor::NONE:
		return(phenomena(qualifier, 
			Weather::SNOW_LIGHT, 
			Weather::SNOW_MODERATE, 
			Weather::SNOW_HEAVY));

		case metaf::WeatherGroup::Descriptor::LOW_DRIFTING:
		return(Weather::DRIFTING_SNOW);

		case metaf::WeatherGroup::Descriptor::BLOWING:
		return(Weather::BLOWING_SNOW);

		default:
		return(phenomena(qualifier, 
			Weather::SNOW_LIGHT, 
			Weather::SNOW_MODERATE, 
			Weather::SNOW_HEAVY));
	}
}

Weather weatherFog(metaf::WeatherGroup::Descriptor descriptor) {
	switch (descriptor) {
		case metaf::WeatherGroup::Descriptor::SHALLOW: return(Weather::FOG_SHALLOW);
		case metaf::WeatherGroup::Descriptor::PARTIAL: return(Weather::FOG_PARTIAL);
		case metaf::WeatherGroup::Descriptor::PATCHES: return(Weather::FOG_PATCHES);
		case metaf::WeatherGroup::Descriptor::FREEZING: return(Weather::FOG_FREEZING);
		default: return(Weather::FOG);
	}
}

Weather phenomenaFromWeatherGroup(
	metaf::WeatherGroup::Qualifier qualifier,
	metaf::WeatherGroup::Descriptor descriptor,
	metaf::WeatherGroup::Weather weather) 
{
	switch (weather) {
		case metaf::WeatherGroup::Weather::NOT_REPORTED:
		return(Weather::NOT_SPECIFIED);

		case metaf::WeatherGroup::Weather::DRIZZLE:
		if (descriptor == metaf::WeatherGroup::Descriptor::FREEZING) return(phenomena(qualifier, 
			Weather::DRIZZLE_FREEZING_LIGHT, 
			Weather::DRIZZLE_FREEZING_MODERATE, 
			Weather::DRIZZLE_FREEZING_HEAVY));
		return(phenomena(qualifier, 
			Weather::DRIZZLE_LIGHT, 
			Weather::DRIZZLE_MODERATE, 
			Weather::DRIZZLE_HEAVY));

		case metaf::WeatherGroup::Weather::RAIN:
		return(weatherRain(qualifier, descriptor));

		case metaf::WeatherGroup::Weather::SNOW:
		return(weatherSnow(qualifier, descriptor));

		case metaf::WeatherGroup::Weather::SNOW_GRAINS:
		return(phenomena(qualifier, 
			Weather::SNOW_GRAINS_LIGHT, 
			Weather::SNOW_GRAINS_MODERATE, 
			Weather::SNOW_GRAINS_HEAVY));

		case metaf::WeatherGroup::Weather::ICE_CRYSTALS:
		return(Weather::ICE_CRYSTALS);

		case metaf::WeatherGroup::Weather::ICE_PELLETS:
		if (descriptor == metaf::WeatherGroup::Descriptor::SHOWERS) return(phenomena(qualifier, 
			Weather::ICE_PELLETS_SHOWERS_LIGHT,
			Weather::ICE_PELLETS_SHOWERS_MODERATE, 
			Weather::ICE_PELLETS_SHOWERS_HEAVY));
		return(phenomena(qualifier,
			Weather::ICE_PELLETS_SHOWERS_LIGHT, 
			Weather::ICE_PELLETS_SHOWERS_MODERATE,
			Weather::ICE_PELLETS_SHOWERS_HEAVY));

		case metaf::WeatherGroup::Weather::HAIL:
		return(Weather::HAIL);

		case metaf::WeatherGroup::Weather::SMALL_HAIL:
		return(Weather::GRAUPEL);

		case metaf::WeatherGroup::Weather::UNDETERMINED:
		return(phenomena(qualifier, 
			Weather::UNDETERMINED_LIGHT, 
			Weather::UNDETERMINED_MODERATE, 
			Weather::UNDETERMINED_HEAVY));

		case metaf::WeatherGroup::Weather::MIST:
		return(Weather::MIST);

		case metaf::WeatherGroup::Weather::FOG:
		return(weatherFog(descriptor));

		case metaf::WeatherGroup::Weather::SMOKE:
		return(Weather::SMOKE);

		case metaf::WeatherGroup::Weather::VOLCANIC_ASH:
		return(Weather::VOLCANIC_ASH);

		case metaf::WeatherGroup::Weather::DUST:
		if (descriptor == metaf::WeatherGroup::Descriptor::LOW_DRIFTING) 
			return(Weather::DRIFTING_DUST);
		if (descriptor == metaf::WeatherGroup::Descriptor::BLOWING)
			return(Weather::BLOWING_DUST);
		return(Weather::WIDESPREAD_DUST);

		case metaf::WeatherGroup::Weather::SAND:
		if (descriptor == metaf::WeatherGroup::Descriptor::LOW_DRIFTING) 
			return(Weather::DRIFTING_SAND);
		if (descriptor == metaf::WeatherGroup::Descriptor::BLOWING)
			return(Weather::BLOWING_SAND);
		return(Weather::SAND);

		case metaf::WeatherGroup::Weather::HAZE:
		return(Weather::HAZE);

		case metaf::WeatherGroup::Weather::SPRAY:
		return(Weather::BLOWING_SPRAY);

		case metaf::WeatherGroup::Weather::DUST_WHIRLS:
		return(Weather::DUST_WHIRLS);

		case metaf::WeatherGroup::Weather::SQUALLS:
		return(Weather::SQUALLS);

		case metaf::WeatherGroup::Weather::FUNNEL_CLOUD:
		if (qualifier == metaf::WeatherGroup::Qualifier::HEAVY) return(Weather::TORNADO);
		return(Weather::FUNNEL_CLOUD);

		case metaf::WeatherGroup::Weather::SANDSTORM:
		if (qualifier == metaf::WeatherGroup::Qualifier::HEAVY) return(Weather::SANDSTORM_HEAVY);
		return(Weather::SANDSTORM);

		case metaf::WeatherGroup::Weather::DUSTSTORM:
		if (qualifier == metaf::WeatherGroup::Qualifier::HEAVY) return(Weather::DUSTSTORM_HEAVY);
		return(Weather::DUSTSTORM);
	}
}

Weather phenomenaInVicinity(metaf::WeatherGroup::Descriptor descriptor,
	metaf::WeatherGroup::Weather weather)
{
	switch (descriptor) {
		case metaf::WeatherGroup::Descriptor::SHOWERS:
		return(Weather::PRECIPITATION_IN_VICINITY);

		case metaf::WeatherGroup::Descriptor::THUNDERSTORM:
		return(Weather::THUNDERSTORM_IN_VICINITY);
		
		case metaf::WeatherGroup::Descriptor::BLOWING:
		switch (weather) {
			case metaf::WeatherGroup::Weather::DUST: 
			return(Weather::BLOWING_DUST_IN_VICINITY);

			case metaf::WeatherGroup::Weather::SAND:
			return(Weather::BLOWING_SAND_IN_VICINITY);

			case metaf::WeatherGroup::Weather::SNOW:
			return(Weather::BLOWING_SNOW_IN_VICINITY);

			default: break;
		}
		break;

		default:
		break;
	}

	switch (weather) {
		case metaf::WeatherGroup::Weather::FOG: 
		return(Weather::FOG_IN_VICINITY);

		case metaf::WeatherGroup::Weather::VOLCANIC_ASH:
		return(Weather::VOLCANIC_ASH_IN_VICINITY);

		case metaf::WeatherGroup::Weather::DUST_WHIRLS:
		return(Weather::DUST_WHIRLS_IN_VICINITY);

		case metaf::WeatherGroup::Weather::DUSTSTORM:
		return(Weather::DUSTSTORM_IN_VICINITY);

		case metaf::WeatherGroup::Weather::SANDSTORM:
		return(Weather::SANDSTORM_IN_VICINITY);

		default:
		return(Weather::PRECIPITATION_IN_VICINITY);
	}
}

std::vector<int> weatherFromWeatherGroup(
	metaf::WeatherGroup::Qualifier qualifier,
	metaf::WeatherGroup::Descriptor descriptor,
	std::vector<metaf::WeatherGroup::Weather> weather)
{
	std::vector<int> result;

	if (qualifier == metaf::WeatherGroup::Qualifier::RECENT) return(result);

	if (qualifier == metaf::WeatherGroup::Qualifier::VICINITY) {
		const auto w = 
			weather.empty() ? metaf::WeatherGroup::Weather::NOT_REPORTED : weather[0];
		result.push_back(static_cast<int>(phenomenaInVicinity(descriptor, w)));
		return(result);
	}

	if (descriptor == metaf::WeatherGroup::Descriptor::THUNDERSTORM) {
		result.push_back(static_cast<int>(Weather::THUNDERSTORM));
		descriptor = metaf::WeatherGroup::Descriptor::NONE;
	}

	for (const auto w : weather) {
		result.push_back(
			static_cast<int>(phenomenaFromWeatherGroup(qualifier, descriptor, w)));
	}

	return(result);
}

GroupVector parseReport(const std::string & report, metaf::ReportType type) {
	auto parsed = metaf::Parser::parse(report);
	if (parsed.reportType != type || parsed.error != metaf::Parser::Error::NONE) {	
		return (GroupVector());
	}
	return(std::move(parsed.groups));
}

CurrentWeather currentWeatherFromMetar(const GroupVector & metarGroups, bool isImperialUnit) {
	CurrentWeather result;
	const auto visUnit = visibilityUnit(isImperialUnit);
	const auto presUnit = pressureUnit(isImperialUnit);
	const auto tempUnit = temperatureUnit(isImperialUnit);
	const auto spdUnit = speedUnit(isImperialUnit);
	metaf::Speed windSpeed;
	for (const auto & metarGroup : metarGroups) {
		if (const auto gr = std::get_if<metaf::FixedGroup>(&metarGroup); gr) {
			if (gr->type() == metaf::FixedGroup::Type::CAVOK) {
				const auto v = metaf::Distance::cavokVisibility().toUnit(visUnit); 
				if (v.has_value()) result.visibility = v.value();	
				result.cloud = Cloud::MOSTLY_CLEAR;
			}
		}
		if (const auto gr = std::get_if<metaf::TrendGroup>(&metarGroup); gr) {
			break; //this version ignores trends
		}
		if (const auto gr = std::get_if<metaf::WindGroup>(&metarGroup); gr) {
			if (gr->status() == metaf::WindGroup::Status::SURFACE_WIND ||
				gr->status() == metaf::WindGroup::Status::SURFACE_WIND_WITH_VARIABLE_SECTOR)
			{
				if (const auto dir = gr->direction().degrees(); dir.has_value()) {
					result.windDirection = dir.value();
				}
				if (gr->direction().status() == metaf::Direction::Status::VARIABLE) {
					result.isWindVariable = true;
				}
				windSpeed = gr->windSpeed();
				if (const auto ws = gr->windSpeed().toUnit(spdUnit); ws.has_value()) {
					result.windSpeed = ws.value();
				}
				if (const auto gs = gr->gustSpeed().toUnit(spdUnit); gs.has_value()) {
					result.gustSpeed = gs.value();
				}				
			}
		}
		if (const auto gr = std::get_if<metaf::VisibilityGroup>(&metarGroup); gr) {
			if (const auto v = gr->visibility().toUnit(visUnit); v.has_value()) {
				result.visibility = v.value();
			}
		}
		if (const auto gr = std::get_if<metaf::CloudGroup>(&metarGroup); gr) {
			result.cloud = 
				cloudFromCloudGroupAmount(static_cast<Cloud>(result.cloud), gr->amount());
			if (gr->type() == metaf::CloudGroup::Type::CUMULONIMBUS) {
				result.isStormClouds = true;
			}
		}
		if (const auto gr = std::get_if<metaf::WeatherGroup>(&metarGroup); gr) {
			const auto weather = weatherFromWeatherGroup(
				gr->qualifier(), 
				gr->descriptor(),
				gr->weather());
			for (const auto w : weather) {
				result.weather.push_back(w);	
			}
		}
		if (const auto gr = std::get_if<metaf::TemperatureGroup>(&metarGroup); gr) {
			if (const auto t = gr->airTemperature().toUnit(tempUnit); t.has_value()) {
				result.airTemperature = t.value();
				result.perceivedTemperature = t.value();
			}
			if (const auto rh = gr->relativeHumidity(); rh.has_value()) {
				result.relativeHumidity = rh.value();
			}
			// Assuming that the wind group always comes before temperature group
			// and result.windSpeed is already initialised
			const auto windChill = 
				metaf::Temperature::windChill(gr->airTemperature(), windSpeed);
			if (const auto wch = windChill.toUnit(tempUnit); wch.has_value()) {
				result.perceivedTemperature = wch.value();
			} 
			const auto heatIndex = 
				metaf::Temperature::heatIndex(gr->airTemperature(), gr->dewPoint());
			if (const auto hidx = heatIndex.toUnit(tempUnit); hidx.has_value()) {
				result.perceivedTemperature = hidx.value();
			}
		}
		if (const auto gr = std::get_if<metaf::PressureGroup>(&metarGroup); gr) {
			if (gr->type() == metaf::PressureGroup::Type::OBSERVED_QNH) {
				if (const auto p = gr->atmosphericPressure().toUnit(presUnit); p.has_value()) {
					auto pressure = p.value();
					if (presUnit == metaf::Pressure::Unit::INCHES_HG) pressure *= 100;
					result.atmosphericPressure = pressure;
				}
			}
		}
	}
	return(result);
}

CurrentWeather currentWeatherFromTaf(const GroupVector & tafGroups, bool isImperialUnit) {
	CurrentWeather result;
	const auto visUnit = visibilityUnit(isImperialUnit);
	const auto spdUnit = speedUnit(isImperialUnit);
	metaf::Speed windSpeed;
	int trendCounter = 0;
	for (const auto & tafGroup : tafGroups) {
		if (const auto gr = std::get_if<metaf::FixedGroup>(&tafGroup); gr) {
			if (gr->type() == metaf::FixedGroup::Type::CAVOK) {
				const auto v = metaf::Distance::cavokVisibility().toUnit(visUnit); 
				if (v.has_value()) result.visibility = v.value();	
				result.cloud = Cloud::MOSTLY_CLEAR;
			}
		}
		if (const auto gr = std::get_if<metaf::TrendGroup>(&tafGroup); gr) {
			trendCounter++;
			if (trendCounter > 1) break; //consider only prevailing trend (i.e. first trend)
		}
		if (const auto gr = std::get_if<metaf::WindGroup>(&tafGroup); gr) {
			if (const auto dir = gr->direction().degrees(); dir.has_value()) {
				result.windDirection = dir.value();
			}
			if (gr->direction().status() == metaf::Direction::Status::VARIABLE) {
				result.isWindVariable = true;
			}
			windSpeed = gr->windSpeed();
			if (const auto ws = gr->windSpeed().toUnit(spdUnit); ws.has_value()) {
				result.windSpeed = ws.value();
			}
			if (const auto gs = gr->gustSpeed().toUnit(spdUnit); gs.has_value()) {
				result.gustSpeed = gs.value();
			}				
		}
		if (const auto gr = std::get_if<metaf::VisibilityGroup>(&tafGroup); gr) {
			if (const auto v = gr->visibility().toUnit(visUnit); v.has_value()) {
				result.visibility = v.value();
			}
		}
		if (const auto gr = std::get_if<metaf::CloudGroup>(&tafGroup); gr) {
			result.cloud = 
				cloudFromCloudGroupAmount(static_cast<Cloud>(result.cloud), gr->amount());
			if (gr->type() == metaf::CloudGroup::Type::CUMULONIMBUS) {
				result.isStormClouds = true;
			}
		}
		if (const auto gr = std::get_if<metaf::WeatherGroup>(&tafGroup); gr) {
			const auto weather = weatherFromWeatherGroup(
				gr->qualifier(), 
				gr->descriptor(),
				gr->weather());
			for (const auto w : weather) {
				result.weather.push_back(w);	
			}
		}
	}
	return(result);
}

void temperatureForecastFromTaf(
	CurrentWeather & currentWeather,
	const GroupVector & tafGroups, 
	bool isImperialUnit)
{
	const auto tempUnit = temperatureUnit(isImperialUnit);
	metaf::Temperature minTemp, maxTemp;
	for (const auto & tafGroup : tafGroups) {
		if (const auto gr = std::get_if<metaf::TemperatureForecastGroup>(&tafGroup); gr) {
			switch(gr->point()) {
				case metaf::TemperatureForecastGroup::Point::MINIMUM:
				minTemp = gr->airTemperature();
				break;

				case metaf::TemperatureForecastGroup::Point::MAXIMUM:
				maxTemp = gr->airTemperature();
				break;

				default:
				break;
			}
		}
	}
	if (const auto t = minTemp.toUnit(tempUnit); t.has_value()) {
		currentWeather.airTemperatureLow = t.value();		
	}
	if (const auto t = maxTemp.toUnit(tempUnit); t.has_value()) {
		currentWeather.airTemperatureHigh = t.value();		
	}
}

CurrentWeather getCurrentWeather(std::string metar, std::string taf, bool isImperialUnit) {
	CurrentWeather result;
	const auto metarGroups = parseReport(metar, metaf::ReportType::METAR);
	const auto tafGroups = parseReport(taf, metaf::ReportType::TAF);
	if (!metarGroups.empty()) {
		result = currentWeatherFromMetar(metarGroups, isImperialUnit);
	} else {
		result = currentWeatherFromTaf(tafGroups, isImperialUnit);
	}
	temperatureForecastFromTaf(result, tafGroups, isImperialUnit);
	return(result);
}

// Embind definitions

EMSCRIPTEN_BINDINGS(WeatherSummary) {

	enum_<Cloud>("Cloud")
		.value("NOT_SPECIFIED", Cloud::NOT_SPECIFIED)
		.value("CLEAR", Cloud::CLEAR)
		.value("MOSTLY_CLEAR", Cloud::MOSTLY_CLEAR)
		.value("MOSTLY_CLOUDY", Cloud::MOSTLY_CLOUDY)
		.value("OVERCAST", Cloud::OVERCAST)
		;

	enum_<Weather>("Weather")
		.value("NOT_SPECIFIED",Weather::NOT_SPECIFIED)
		.value("PRECIPITATION_IN_VICINITY",Weather::PRECIPITATION_IN_VICINITY)
		.value("THUNDERSTORM_IN_VICINITY",Weather::THUNDERSTORM_IN_VICINITY)
		.value("FOG_IN_VICINITY",Weather::FOG_IN_VICINITY)
		.value("VOLCANIC_ASH_IN_VICINITY",Weather::VOLCANIC_ASH_IN_VICINITY)
		.value("DUST_WHIRLS_IN_VICINITY",Weather::DUST_WHIRLS_IN_VICINITY)
		.value("BLOWING_SNOW_IN_VICINITY",Weather::BLOWING_SNOW_IN_VICINITY)
		.value("BLOWING_DUST_IN_VICINITY",Weather::BLOWING_DUST_IN_VICINITY)
		.value("BLOWING_SAND_IN_VICINITY",Weather::BLOWING_SAND_IN_VICINITY)
		.value("SANDSTORM_IN_VICINITY",Weather::SANDSTORM_IN_VICINITY)
		.value("DUSTSTORM_IN_VICINITY",Weather::DUSTSTORM_IN_VICINITY)
		.value("DRIFTING_SAND",Weather::DRIFTING_SAND)
		.value("DRIFTING_DUST",Weather::DRIFTING_DUST)
		.value("DRIFTING_SNOW",Weather::DRIFTING_SNOW)
		.value("BLOWING_SAND",Weather::BLOWING_SAND)
		.value("BLOWING_DUST",Weather::BLOWING_DUST)
		.value("BLOWING_SNOW",Weather::BLOWING_SNOW)
		.value("BLOWING_SPRAY",Weather::BLOWING_SPRAY)
		.value("FOG",Weather::FOG)
		.value("FOG_SHALLOW",Weather::FOG_SHALLOW)
		.value("FOG_PARTIAL",Weather::FOG_PARTIAL)
		.value("FOG_PATCHES",Weather::FOG_PATCHES)
		.value("FOG_FREEZING",Weather::FOG_FREEZING)
		.value("ICE_CRYSTALS",Weather::ICE_CRYSTALS)
		.value("MIST",Weather::MIST)
		.value("SMOKE",Weather::SMOKE)
		.value("VOLCANIC_ASH",Weather::VOLCANIC_ASH)
		.value("WIDESPREAD_DUST",Weather::WIDESPREAD_DUST)
		.value("SAND",Weather::SAND)
		.value("HAZE",Weather::HAZE)
		.value("DRIZZLE_LIGHT",Weather::DRIZZLE_LIGHT)
		.value("DRIZZLE_MODERATE",Weather::DRIZZLE_MODERATE)
		.value("DRIZZLE_HEAVY",Weather::DRIZZLE_HEAVY)
		.value("RAIN_LIGHT",Weather::RAIN_LIGHT)
		.value("RAIN_MODERATE",Weather::RAIN_MODERATE)
		.value("RAIN_HEAVY",Weather::RAIN_HEAVY)
		.value("SNOW_LIGHT",Weather::SNOW_LIGHT)
		.value("SNOW_MODERATE",Weather::SNOW_MODERATE)
		.value("SNOW_HEAVY",Weather::SNOW_HEAVY)
		.value("ICE_PELLETS_LIGHT",Weather::ICE_PELLETS_LIGHT)
		.value("ICE_PELLETS_MODERATE",Weather::ICE_PELLETS_MODERATE)
		.value("ICE_PELLETS_HEAVY",Weather::ICE_PELLETS_HEAVY)
		.value("SNOW_GRAINS_LIGHT",Weather::SNOW_GRAINS_LIGHT)
		.value("SNOW_GRAINS_MODERATE",Weather::SNOW_GRAINS_MODERATE)
		.value("SNOW_GRAINS_HEAVY",Weather::SNOW_GRAINS_HEAVY)
		.value("GRAUPEL",Weather::GRAUPEL)
		.value("HAIL",Weather::HAIL)
		.value("UNDETERMINED_LIGHT",Weather::UNDETERMINED_LIGHT)
		.value("UNDETERMINED_MODERATE",Weather::UNDETERMINED_MODERATE)
		.value("UNDETERMINED_HEAVY",Weather::UNDETERMINED_HEAVY)
		.value("RAIN_SHOWERS_LIGHT",Weather::RAIN_SHOWERS_LIGHT)
		.value("RAIN_SHOWERS_MODERATE",Weather::RAIN_SHOWERS_MODERATE)
		.value("RAIN_SHOWERS_HEAVY",Weather::RAIN_SHOWERS_HEAVY)
		.value("SNOW_SHOWERS_LIGHT",Weather::SNOW_SHOWERS_LIGHT)
		.value("SNOW_SHOWERS_MODERATE",Weather::SNOW_SHOWERS_MODERATE)
		.value("SNOW_SHOWERS_HEAVY",Weather::SNOW_SHOWERS_HEAVY)
		.value("ICE_PELLETS_SHOWERS_LIGHT",Weather::ICE_PELLETS_SHOWERS_LIGHT)
		.value("ICE_PELLETS_SHOWERS_MODERATE",Weather::ICE_PELLETS_SHOWERS_MODERATE)
		.value("ICE_PELLETS_SHOWERS_HEAVY",Weather::ICE_PELLETS_SHOWERS_HEAVY)
		.value("DRIZZLE_FREEZING_LIGHT",Weather::DRIZZLE_FREEZING_LIGHT)
		.value("DRIZZLE_FREEZING_MODERATE",Weather::DRIZZLE_FREEZING_MODERATE)
		.value("DRIZZLE_FREEZING_HEAVY",Weather::DRIZZLE_FREEZING_HEAVY)
		.value("RAIN_FREEZING_LIGHT",Weather::RAIN_FREEZING_LIGHT)
		.value("RAIN_FREEZING_MODERATE",Weather::RAIN_FREEZING_MODERATE)
		.value("RAIN_FREEZING_HEAVY",Weather::RAIN_FREEZING_HEAVY)
		.value("THUNDERSTORM",Weather::THUNDERSTORM)
		.value("DUST_WHIRLS",Weather::DUST_WHIRLS)
		.value("SQUALLS",Weather::SQUALLS)
		.value("FUNNEL_CLOUD",Weather::FUNNEL_CLOUD)
		.value("TORNADO",Weather::TORNADO)
		.value("SANDSTORM",Weather::SANDSTORM)
		.value("SANDSTORM_HEAVY",Weather::SANDSTORM_HEAVY)
		.value("DUSTSTORM",Weather::DUSTSTORM)
		.value("DUSTSTORM_HEAVY",Weather::DUSTSTORM_HEAVY)
		;

	value_object<CurrentWeather>("CurrentWeather")
		.field("windDirection", &CurrentWeather::windDirection)
		.field("isWindVariable", &CurrentWeather::isWindVariable)
		.field("windSpeed", &CurrentWeather::windSpeed)
		.field("gustSpeed", &CurrentWeather::gustSpeed)
		.field("visibility", &CurrentWeather::visibility)
		.field("cloud", &CurrentWeather::cloud)
		.field("isStormClouds", &CurrentWeather::isStormClouds)
		.field("weather", &CurrentWeather::weather)
		.field("airTemperature", &CurrentWeather::airTemperature)
		.field("perceivedTemperature", &CurrentWeather::perceivedTemperature)
		.field("airTemperatureHigh", &CurrentWeather::airTemperatureHigh)
		.field("airTemperatureLow", &CurrentWeather::airTemperatureLow)
		.field("relativeHumidity", &CurrentWeather::relativeHumidity)
		.field("atmosphericPressure", &CurrentWeather::atmosphericPressure)
		;

	function("getCurrentWeather", &getCurrentWeather);

	register_vector<int>("vector<int>");

	constant("valueNotSpecified", valueNotSpecified);
};
