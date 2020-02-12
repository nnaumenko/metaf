/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include <emscripten/bind.h>

using namespace emscripten;

#include "metaf.hpp"

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
	std::vector<int> weather; //actually std::vector<WeatherPhenomena::Weather>
	int airTemperature = valueNotSpecified;
	int perceivedTemperature = valueNotSpecified;
	int airTemperatureHigh = valueNotSpecified;
	int airTemperatureLow = valueNotSpecified;
	int relativeHumidity = valueNotSpecified;
	int atmosphericPressure = valueNotSpecified;
	int updateYear = valueNotSpecified;
	int updateMonth = valueNotSpecified;
	int updateDay = valueNotSpecified;
	int updateHour = valueNotSpecified;
	int updateMinute = valueNotSpecified;
};

using GroupVector = std::vector<metaf::GroupInfo>;

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
		case metaf::CloudGroup::Amount::VARIABLE_FEW_SCATTERED:
		if (prev == Cloud::NOT_SPECIFIED) return Cloud::CLEAR;
		return prev;

		case metaf::CloudGroup::Amount::NSC:
		case metaf::CloudGroup::Amount::FEW:
		case metaf::CloudGroup::Amount::SCATTERED:
		case metaf::CloudGroup::Amount::VARIABLE_SCATTERED_BROKEN:
		case metaf::CloudGroup::Amount::VARIABLE_BROKEN_OVERCAST:
		if (prev != Cloud::OVERCAST || prev != Cloud::MOSTLY_CLOUDY) return Cloud::MOSTLY_CLEAR;
		return prev;

		case metaf::CloudGroup::Amount::BROKEN:
		if (prev != Cloud::OVERCAST) return Cloud::MOSTLY_CLOUDY;
		return prev;

		case metaf::CloudGroup::Amount::OVERCAST:
		case metaf::CloudGroup::Amount::OBSCURED:
		return Cloud::OVERCAST;

		default: return prev;
	}
}

Weather phenomena(metaf::WeatherPhenomena::Qualifier qualifier,
	Weather light,
	Weather moderate,
	Weather heavy)
{
	switch (qualifier) {
		case metaf::WeatherPhenomena::Qualifier::LIGHT:		return light;
		case metaf::WeatherPhenomena::Qualifier::MODERATE:	return moderate;
		case metaf::WeatherPhenomena::Qualifier::HEAVY:		return heavy;
		default: 										return moderate;
	}
}

Weather weatherRain(metaf::WeatherPhenomena::Qualifier qualifier,
	metaf::WeatherPhenomena::Descriptor descriptor)
{
	switch (descriptor) {
		case metaf::WeatherPhenomena::Descriptor::FREEZING:
		return phenomena(qualifier,
						Weather::RAIN_FREEZING_LIGHT, 
						Weather::RAIN_FREEZING_MODERATE, 
						Weather::RAIN_FREEZING_HEAVY);

		case metaf::WeatherPhenomena::Descriptor::SHOWERS:
		return phenomena(qualifier,
						Weather::RAIN_SHOWERS_LIGHT, 
						Weather::RAIN_SHOWERS_MODERATE, 
						Weather::RAIN_SHOWERS_HEAVY);

		case metaf::WeatherPhenomena::Descriptor::NONE:
		return phenomena(qualifier, 
						Weather::RAIN_LIGHT, 
						Weather::RAIN_MODERATE, 
						Weather::RAIN_HEAVY);

		default:
		return phenomena(qualifier, 
						Weather::RAIN_LIGHT, 
						Weather::RAIN_MODERATE, 
						Weather::RAIN_HEAVY);
	}
}

Weather weatherSnow(metaf::WeatherPhenomena::Qualifier qualifier,
	metaf::WeatherPhenomena::Descriptor descriptor)
{
	switch (descriptor) {
		case metaf::WeatherPhenomena::Descriptor::SHOWERS:
		return phenomena(qualifier,
						Weather::SNOW_SHOWERS_LIGHT, 
						Weather::SNOW_SHOWERS_MODERATE, 
						Weather::SNOW_SHOWERS_HEAVY);

		case metaf::WeatherPhenomena::Descriptor::NONE:
		return phenomena(qualifier, 
			Weather::SNOW_LIGHT, 
			Weather::SNOW_MODERATE, 
			Weather::SNOW_HEAVY);

		case metaf::WeatherPhenomena::Descriptor::LOW_DRIFTING:
		return Weather::DRIFTING_SNOW;

		case metaf::WeatherPhenomena::Descriptor::BLOWING:
		return Weather::BLOWING_SNOW;

		default:
		return phenomena(qualifier, 
						Weather::SNOW_LIGHT, 
						Weather::SNOW_MODERATE, 
						Weather::SNOW_HEAVY);
	}
}

Weather weatherFog(metaf::WeatherPhenomena::Descriptor descriptor) {
	switch (descriptor) {
		case metaf::WeatherPhenomena::Descriptor::SHALLOW: return Weather::FOG_SHALLOW;
		case metaf::WeatherPhenomena::Descriptor::PARTIAL: return Weather::FOG_PARTIAL;
		case metaf::WeatherPhenomena::Descriptor::PATCHES: return Weather::FOG_PATCHES;
		case metaf::WeatherPhenomena::Descriptor::FREEZING: return Weather::FOG_FREEZING;
		default: return Weather::FOG;
	}
}

Weather phenomenaFromWeatherGroup(
	metaf::WeatherPhenomena::Qualifier qualifier,
	metaf::WeatherPhenomena::Descriptor descriptor,
	metaf::WeatherPhenomena::Weather weather) 
{
	switch (weather) {
		case metaf::WeatherPhenomena::Weather::NOT_REPORTED:
		return Weather::NOT_SPECIFIED;

		case metaf::WeatherPhenomena::Weather::DRIZZLE:
		if (descriptor == metaf::WeatherPhenomena::Descriptor::FREEZING) 
		{
			return phenomena(qualifier, 
							Weather::DRIZZLE_FREEZING_LIGHT, 
							Weather::DRIZZLE_FREEZING_MODERATE, 
							Weather::DRIZZLE_FREEZING_HEAVY);
		}
		return phenomena(qualifier, 
							Weather::DRIZZLE_LIGHT, 
							Weather::DRIZZLE_MODERATE, 
							Weather::DRIZZLE_HEAVY);

		case metaf::WeatherPhenomena::Weather::RAIN:
		return weatherRain(qualifier, descriptor);

		case metaf::WeatherPhenomena::Weather::SNOW:
		return weatherSnow(qualifier, descriptor);

		case metaf::WeatherPhenomena::Weather::SNOW_GRAINS:
		return phenomena(qualifier, 
						Weather::SNOW_GRAINS_LIGHT, 
						Weather::SNOW_GRAINS_MODERATE, 
						Weather::SNOW_GRAINS_HEAVY);

		case metaf::WeatherPhenomena::Weather::ICE_CRYSTALS:
		return Weather::ICE_CRYSTALS;

		case metaf::WeatherPhenomena::Weather::ICE_PELLETS:
		if (descriptor == metaf::WeatherPhenomena::Descriptor::SHOWERS) 
		{
			return phenomena(qualifier, 
							Weather::ICE_PELLETS_SHOWERS_LIGHT,
							Weather::ICE_PELLETS_SHOWERS_MODERATE, 
							Weather::ICE_PELLETS_SHOWERS_HEAVY);
		}
		return phenomena(qualifier,
							Weather::ICE_PELLETS_SHOWERS_LIGHT, 
							Weather::ICE_PELLETS_SHOWERS_MODERATE,
							Weather::ICE_PELLETS_SHOWERS_HEAVY);

		case metaf::WeatherPhenomena::Weather::HAIL:
		return Weather::HAIL;

		case metaf::WeatherPhenomena::Weather::SMALL_HAIL:
		return Weather::GRAUPEL;

		case metaf::WeatherPhenomena::Weather::UNDETERMINED:
		return phenomena(qualifier, 
						Weather::UNDETERMINED_LIGHT, 
						Weather::UNDETERMINED_MODERATE, 
						Weather::UNDETERMINED_HEAVY);

		case metaf::WeatherPhenomena::Weather::MIST:
		return Weather::MIST;

		case metaf::WeatherPhenomena::Weather::FOG:
		return weatherFog(descriptor);

		case metaf::WeatherPhenomena::Weather::SMOKE:
		return Weather::SMOKE;

		case metaf::WeatherPhenomena::Weather::VOLCANIC_ASH:
		return Weather::VOLCANIC_ASH;

		case metaf::WeatherPhenomena::Weather::DUST:
		if (descriptor == metaf::WeatherPhenomena::Descriptor::LOW_DRIFTING) 
			return Weather::DRIFTING_DUST;
		if (descriptor == metaf::WeatherPhenomena::Descriptor::BLOWING)
			return Weather::BLOWING_DUST;
		return Weather::WIDESPREAD_DUST;

		case metaf::WeatherPhenomena::Weather::SAND:
		if (descriptor == metaf::WeatherPhenomena::Descriptor::LOW_DRIFTING) 
			return Weather::DRIFTING_SAND;
		if (descriptor == metaf::WeatherPhenomena::Descriptor::BLOWING)
			return Weather::BLOWING_SAND;
		return Weather::SAND;

		case metaf::WeatherPhenomena::Weather::HAZE:
		return Weather::HAZE;

		case metaf::WeatherPhenomena::Weather::SPRAY:
		return Weather::BLOWING_SPRAY;

		case metaf::WeatherPhenomena::Weather::DUST_WHIRLS:
		return Weather::DUST_WHIRLS;

		case metaf::WeatherPhenomena::Weather::SQUALLS:
		return Weather::SQUALLS;

		case metaf::WeatherPhenomena::Weather::FUNNEL_CLOUD:
		if (qualifier == metaf::WeatherPhenomena::Qualifier::HEAVY) return Weather::TORNADO;
		return Weather::FUNNEL_CLOUD;

		case metaf::WeatherPhenomena::Weather::SANDSTORM:
		if (qualifier == metaf::WeatherPhenomena::Qualifier::HEAVY) return Weather::SANDSTORM_HEAVY;
		return Weather::SANDSTORM;

		case metaf::WeatherPhenomena::Weather::DUSTSTORM:
		if (qualifier == metaf::WeatherPhenomena::Qualifier::HEAVY) return Weather::DUSTSTORM_HEAVY;
		return Weather::DUSTSTORM;
	}
}

Weather phenomenaInVicinity(metaf::WeatherPhenomena::Descriptor descriptor,
	metaf::WeatherPhenomena::Weather weather)
{
	switch (descriptor) {
		case metaf::WeatherPhenomena::Descriptor::SHOWERS:
		return Weather::PRECIPITATION_IN_VICINITY;

		case metaf::WeatherPhenomena::Descriptor::THUNDERSTORM:
		return Weather::THUNDERSTORM_IN_VICINITY;
		
		case metaf::WeatherPhenomena::Descriptor::BLOWING:
		switch (weather) {
			case metaf::WeatherPhenomena::Weather::DUST: 
			return Weather::BLOWING_DUST_IN_VICINITY;

			case metaf::WeatherPhenomena::Weather::SAND:
			return Weather::BLOWING_SAND_IN_VICINITY;

			case metaf::WeatherPhenomena::Weather::SNOW:
			return Weather::BLOWING_SNOW_IN_VICINITY;

			default: break;
		}
		break;

		default:
		break;
	}

	switch (weather) {
		case metaf::WeatherPhenomena::Weather::FOG: 
		return Weather::FOG_IN_VICINITY;

		case metaf::WeatherPhenomena::Weather::VOLCANIC_ASH:
		return Weather::VOLCANIC_ASH_IN_VICINITY;

		case metaf::WeatherPhenomena::Weather::DUST_WHIRLS:
		return Weather::DUST_WHIRLS_IN_VICINITY;

		case metaf::WeatherPhenomena::Weather::DUSTSTORM:
		return Weather::DUSTSTORM_IN_VICINITY;

		case metaf::WeatherPhenomena::Weather::SANDSTORM:
		return Weather::SANDSTORM_IN_VICINITY;

		default:
		return Weather::PRECIPITATION_IN_VICINITY;
	}
}

std::vector<int> weatherFromWeatherPhenomena(
	metaf::WeatherPhenomena::Qualifier qualifier,
	metaf::WeatherPhenomena::Descriptor descriptor,
	std::vector<metaf::WeatherPhenomena::Weather> weather)
{
	std::vector<int> result;

	if (qualifier == metaf::WeatherPhenomena::Qualifier::VICINITY) {
		const auto w = 
			weather.empty() ? metaf::WeatherPhenomena::Weather::NOT_REPORTED : weather[0];
		result.push_back(static_cast<int>(phenomenaInVicinity(descriptor, w)));
		return result;
	}

	if (descriptor == metaf::WeatherPhenomena::Descriptor::THUNDERSTORM) {
		result.push_back(static_cast<int>(Weather::THUNDERSTORM));
		descriptor = metaf::WeatherPhenomena::Descriptor::NONE;
	}

	for (const auto w : weather) {
		result.push_back(
			static_cast<int>(phenomenaFromWeatherGroup(qualifier, descriptor, w)));
	}
	return result;
}

GroupVector parseReport(const std::string & report, metaf::ReportType type) {
	auto parsed = metaf::Parser::parse(report);
	if (parsed.reportMetadata.type != type || 
		parsed.reportMetadata.error != metaf::ReportError::NONE) return GroupVector();
	return std::move(parsed.groups);
}

CurrentWeather currentWeatherFromMetar(const GroupVector & metarGroups, bool isImperialUnit) {
	CurrentWeather result;
	const auto visUnit = visibilityUnit(isImperialUnit);
	const auto presUnit = pressureUnit(isImperialUnit);
	const auto tempUnit = temperatureUnit(isImperialUnit);
	const auto spdUnit = speedUnit(isImperialUnit);
	metaf::Speed windSpeed;
	for (const auto & metarGroupInfo : metarGroups) {
		const auto & metarGroup = metarGroupInfo.group;
		if (const auto gr = std::get_if<metaf::KeywordGroup>(&metarGroup); gr) {
			if (gr->type() == metaf::KeywordGroup::Type::CAVOK) {
				const auto v = metaf::Distance::cavokVisibility().toUnit(visUnit); 
				if (v.has_value()) result.visibility = v.value();	
				result.cloud = Cloud::MOSTLY_CLEAR;
			}
		}
		
		if (const auto gr = std::get_if<metaf::TrendGroup>(&metarGroup); gr) {
			break; //this version ignores trends
		}

		if (const auto gr = std::get_if<metaf::WindGroup>(&metarGroup); gr) {
			if (gr->type() == metaf::WindGroup::Type::SURFACE_WIND ||
				gr->type() == metaf::WindGroup::Type::SURFACE_WIND_CALM ||
				gr->type() == metaf::WindGroup::Type::SURFACE_WIND_WITH_VARIABLE_SECTOR)
			{
				if (const auto dir = gr->direction().degrees(); dir.has_value()) {
					result.windDirection = dir.value();
				}
				if (gr->direction().type() == metaf::Direction::Type::VARIABLE) {
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
			const bool isPrevailing = 
				gr->type() == metaf::VisibilityGroup::Type::PREVAILING ||
				gr->type() == metaf::VisibilityGroup::Type::PREVAILING_NDV;
			if (const auto v = gr->visibility().toUnit(visUnit); 
				v.has_value() && isPrevailing)
			{
					result.visibility = v.value();
			}
		}

		if (const auto gr = std::get_if<metaf::CloudGroup>(&metarGroup); gr) {
			result.cloud = 
				cloudFromCloudGroupAmount(static_cast<Cloud>(result.cloud), gr->amount());
			if (gr->convectiveType() == metaf::CloudGroup::ConvectiveType::CUMULONIMBUS) {
				result.isStormClouds = true;
			}
		}

		if (const auto gr = std::get_if<metaf::WeatherGroup>(&metarGroup); 
			gr && gr->type() == metaf::WeatherGroup::Type::CURRENT)
		{
			const auto weatherPhenomenaVector = gr->weatherPhenomena();
			for (const auto wp : weatherPhenomenaVector) {
				const auto weather = weatherFromWeatherPhenomena(
					wp.qualifier(), 
					wp.descriptor(),
					wp.weather());
				for (const auto w : weather) {
					result.weather.push_back(w);	
				}
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
	return result;
}

CurrentWeather currentWeatherFromTaf(const GroupVector & tafGroups, bool isImperialUnit) {
	CurrentWeather result;
	const auto visUnit = visibilityUnit(isImperialUnit);
	const auto spdUnit = speedUnit(isImperialUnit);
	metaf::Speed windSpeed;
	int trendCounter = 0;
	for (const auto & tafGroupInfo : tafGroups) {
		const auto & tafGroup = tafGroupInfo.group;
		if (const auto gr = std::get_if<metaf::KeywordGroup>(&tafGroup); gr) {
			if (gr->type() == metaf::KeywordGroup::Type::CAVOK) {
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
			if (gr->direction().type() == metaf::Direction::Type::VARIABLE) {
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
			if (gr->convectiveType() == metaf::CloudGroup::ConvectiveType::CUMULONIMBUS) {
				result.isStormClouds = true;
			}
		}
		if (const auto gr = std::get_if<metaf::WeatherGroup>(&tafGroup); gr) {
			const auto weatherPhenomenaVector = gr->weatherPhenomena();
			for (const auto wp : weatherPhenomenaVector) {
				const auto weather = weatherFromWeatherPhenomena(
					wp.qualifier(), 
					wp.descriptor(),
					wp.weather());
				for (const auto w : weather) {
					result.weather.push_back(w);	
				}
			}
		}
	}
	return result;
}

void temperatureForecastFromTaf(
	CurrentWeather & currentWeather,
	const GroupVector & tafGroups, 
	bool isImperialUnit)
{
	const auto tempUnit = temperatureUnit(isImperialUnit);
	metaf::Temperature minTemp, maxTemp;
	for (const auto & tafGroupInfo : tafGroups) {
		const auto & tafGroup = tafGroupInfo.group;
		if (const auto gr = std::get_if<metaf::MinMaxTemperatureGroup>(&tafGroup); gr) {
			minTemp = gr->minimum();
			maxTemp = gr->maximum();
		}
	}
	if (const auto t = minTemp.toUnit(tempUnit); t.has_value()) {
		currentWeather.airTemperatureLow = t.value();		
	}
	if (const auto t = maxTemp.toUnit(tempUnit); t.has_value()) {
		currentWeather.airTemperatureHigh = t.value();		
	}
}

void reportReleaseTime(CurrentWeather & currentWeather, 
	const GroupVector & groups,
	int year, 
	int month, 
	int day)
{
	for (const auto & groupInfo : groups) {
		const auto & group = groupInfo.group;
		if (const auto gr = std::get_if<metaf::ReportTimeGroup>(&group); gr) {
			const auto currDate = metaf::MetafTime::Date(year, month, day);
			const auto reportTime = gr->time();
			const auto reportDate = reportTime.dateBeforeRef(currDate);
			currentWeather.updateYear = reportDate.year;
			currentWeather.updateMonth = reportDate.month;
			currentWeather.updateDay = reportDate.day;
			currentWeather.updateHour = reportTime.hour();
			currentWeather.updateMinute = reportTime.minute();	
		}
	}
}

CurrentWeather getCurrentWeather(
	std::string metar, 
	std::string taf,
	int currYear,
	int currMonth,
	int currDay, 
	bool isImperialUnit)
{
	CurrentWeather result;
	const auto metarGroups = parseReport(metar, metaf::ReportType::METAR);
	const auto tafGroups = parseReport(taf, metaf::ReportType::TAF);
	if (!metarGroups.empty()) {
		result = currentWeatherFromMetar(metarGroups, isImperialUnit);
		reportReleaseTime(result, metarGroups, currYear, currMonth, currDay);
	} else {
		result = currentWeatherFromTaf(tafGroups, isImperialUnit);
		reportReleaseTime(result, tafGroups, currYear, currMonth, currDay);
	}
	temperatureForecastFromTaf(result, tafGroups, isImperialUnit);

	return result;
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
		.field("updateYear", &CurrentWeather::updateYear)
		.field("updateMonth", &CurrentWeather::updateMonth)
		.field("updateDay", &CurrentWeather::updateDay)
		.field("updateHour", &CurrentWeather::updateHour)
		.field("updateMinute", &CurrentWeather::updateMinute)
		;

	function("getCurrentWeather", &getCurrentWeather);

	register_vector<int>("vector<int>");

	constant("valueNotSpecified", valueNotSpecified);
};
