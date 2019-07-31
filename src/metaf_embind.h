/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

/// @file 
/// @brief Bindings for using Metaf classes in JavaScript outside Webassembly environment.

#ifndef METAF_EMBIND_H
#define METAF_EMBIND_H

#ifdef __EMSCRIPTEN__

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(metaf) {

///////////////////////////////////////////////////////////////////////////

emscripten::class_<std::optional<unsigned int>>("MetafOptUInt")
	.property("has_value", &std::optional<unsigned int>::has_value)
	.property("value", &std::optional<unsigned int>::value)

emscripten::class_<std::optional<int>>("MetafOptInt")
	.property("has_value", &std::optional<int>::has_value)
	.property("value", &std::optional<int>::value)

emscripten::class_<std::optional<float>>("MetafOptFloat")
	.property("has_value", &std::optional<float>::has_value)
	.property("value", &std::optional<float>::value)

///////////////////////////////////////////////////////////////////////////

emscripten::enum_<metaf::Runway::Designator>("MetafRunwayDesignator")
	.value("NONE", metaf::Runway::Designator::NONE)
	.value("LEFT", metaf::Runway::Designator::LEFT)
	.value("CENTER", metaf::Runway::Designator::CENTER)
	.value("RIGHT", metaf::Runway::Designator::RIGHT)
	;

emscripten::class_<metaf::Runway>("MetafRunway")
	.constructor()
	.property("number", &metaf::Runway::number)
	.property("designator", &metaf::Runway::designator)
	.property("isValid", &metaf::Runway::isValid)
	.property("isAllRunways", &metaf::Runway::isAllRunways)
	.property("isMessageRepetition", &metaf::Runway::isMessageRepetition)
	;

///////////////////////////////////////////////////////////////////////////

emscripten::class_<metaf::MetafTime::Date>("MetafTimeDate")
	.constructor<unsigned int, unsigned int, unsigned int>()
	.field(&metaf::MetafTime::Date::year)
	.field(&metaf::MetafTime::Date::month)
	.field(&metaf::MetafTime::Date::day)
	;

emscripten::class_<metaf::MetafTime>("MetafTime")
	.constructor()
	.property("day", &metaf::MetafTime::day)
	.property("hour", &metaf::MetafTime::hour)
	.property("minute", &metaf::MetafTime::minute)
	.property("isValid", &metaf::MetafTime::isValid)
	.property("is3hourlyReportTime", &metaf::MetafTime::is3hourlyReportTime)
	.property("is6hourlyReportTime", &metaf::MetafTime::is6hourlyReportTime)
	.property("dateBeforeRef", &metaf::MetafTime::dateBeforeRef)
	;

///////////////////////////////////////////////////////////////////////////

emscripten::enum_<metaf::Temperature::Unit>("MetafTemperatureUnit")
	.value("C", metaf::Temperature::Unit::C)
	.value("F", metaf::Temperature::Unit::F)
	;

emscripten::class_<metaf::Temperature>("MetafTemperature")
	.constructor()
	.property("temperature", &metaf::Temperature::temperature)
	.property("unit", &metaf::Temperature::unit)
	.property("isFreezing", &metaf::Temperature::isFreezing)
	.property("isPrecise", &metaf::Temperature::isPrecise)
	.property("isReported", &metaf::Temperature::isReported)
	.function("toUnit", &metaf::Temperature::toUnit)
	.class_function("relativeHumidity", &metaf::Temperature::relativeHumidity)
	.class_function("heatIndexFromHumidity", 
		select_overload<metaf::Temperature(const Temperature &, float)>(&metaf::Temperature::heatIndex))
	.class_function("heatIndexFromDewPoint", 
		select_overload<metaf::Temperature(const Temperature &, const Temperature &)>(&metaf::Temperature::heatIndex))
	.class_function("windChill", &metaf::Temperature::windChill)
	;

///////////////////////////////////////////////////////////////////////////

emscripten::enum_<metaf::Speed::Unit>("MetafSpeedUnit")
	.value("KNOTS", metaf::Speed::Unit::KNOTS)
	.value("METERS_PER_SECOND", metaf::Speed::Unit::METERS_PER_SECOND)
	.value("KILOMETERS_PER_HOUR", metaf::Speed::Unit::KILOMETERS_PER_HOUR)
	.value("MILES_PER_HOUR", metaf::Speed::Unit::MILES_PER_HOUR)
	;

emscripten::class_<metaf::Speed>("MetafSpeed")
	.constructor()
	.property("speed", &metaf::Speed::speed)
	.property("unit", &metaf::Speed::unit)
	.property("speed", &metaf::Speed::speed)
	.property("isReported", &metaf::Speed::isReported)
	.function("toUnit", &metaf::Speed::toUnit)

///////////////////////////////////////////////////////////////////////////

emscripten::enum_<metaf::Distance::Unit>("MetafDistanceUnit")
	.value("METERS", metaf::Distance::Unit::METERS)
	.value("STATUTE_MILES", metaf::Distance::Unit::STATUTE_MILES)
	.value("FEET", metaf::Distance::Unit::FEET)
	;

emscripten::enum_<metaf::Distance::Modifier>("MetafDistanceModifier")
	.value("NONE", metaf::Distance::Modifier::NONE)
	.value("LESS_THAN", metaf::Distance::Modifier::LESS_THAN)
	.value("MORE_THAN", metaf::Distance::Modifier::MORE_THAN)
	;

emscripten::class_<metaf::Distance>("MetafDistance")
	.constructor()
	.property("integer", &metaf::Distance::integer)
	.property("numerator", &metaf::Distance::numerator)
	.property("denominator", &metaf::Distance::denominator)
	.property("modifier", &metaf::Distance::modifier)
	.property("unit", &metaf::Distance::unit)
	.property("isInteger", &metaf::Distance::isInteger)
	.property("isFraction", &metaf::Distance::isFraction)
	.property("hasInteger", &metaf::Distance::hasInteger)
	.property("hasFraction", &metaf::Distance::hasFraction)
	.property("isReported", &metaf::Distance::isReported)
	.property("isValid", &metaf::Distance::isValid)
	.function("toUnit", &metaf::Distance::toUnit)

///////////////////////////////////////////////////////////////////////////

emscripten::enum_<metaf::Direction::Cardinal>("MetafCardinalDirection")
	.value("NONE", metaf::Direction::Cardinal::NONE)
	.value("NDV", metaf::Direction::Cardinal::NDV)
	.value("N", metaf::Direction::Cardinal::N)
	.value("S", metaf::Direction::Cardinal::S)
	.value("W", metaf::Direction::Cardinal::W)
	.value("E", metaf::Direction::Cardinal::E)
	.value("NW", metaf::Direction::Cardinal::NW)
	.value("NE", metaf::Direction::Cardinal::NE)
	.value("SW", metaf::Direction::Cardinal::SW)
	.value("SE", metaf::Direction::Cardinal::SE)
	.value("TRUE_N", metaf::Direction::Cardinal::TRUE_N)
	.value("TRUE_S", metaf::Direction::Cardinal::TRUE_S)
	.value("TRUE_W", metaf::Direction::Cardinal::TRUE_W)
	.value("TRUE_E", metaf::Direction::Cardinal::TRUE_E)
	;

emscripten::enum_<metaf::Direction::Status>("MetafDirectionStatus")
	.value("OMMITTED", metaf::Direction::Status::OMMITTED)
	.value("NOT_REPORTED", metaf::Direction::Status::NOT_REPORTED)
	.value("VARIABLE", metaf::Direction::Status::VARIABLE)
	.value("NDV", metaf::Direction::Status::NDV)
	.value("VALUE_DEGREES", metaf::Direction::Status::VALUE_DEGREES)
	.value("VALUE_CARDINAL", metaf::Direction::Status::VALUE_CARDINAL)
	;

emscripten::class_<metaf::Direction>("MetafDirection")
	.constructor()
	.property("status", &metaf::Direction::status)
	.property("cardinal", &metaf::Direction::cardinal)
	.property("degrees", &metaf::Direction::degrees)
	.property("isValue", &metaf::Direction::isValue)
	.property("isValid", &metaf::Direction::isValid)
	;

///////////////////////////////////////////////////////////////////////////

emscripten::enum_<metaf::Pressure::Unit>("MetafPressureUnit")
	.value("HECTOPASCAL", metaf::Direction::Status::HECTOPASCAL)
	.value("INCHES_HG", metaf::Direction::Status::INCHES_HG)
	.value("MM_HG", metaf::Direction::Status::MM_HG)
	;

emscripten::class_<metaf::Pressure>("MetafPressure")
	.constructor()
	.property("pressure", &metaf::Pressure::pressure)
	.property("unit", &metaf::Pressure::unit)
	.property("isReported", &metaf::Pressure::isReported)
	.function("toUnit", &metaf::Pressure::toUnit)
	;

///////////////////////////////////////////////////////////////////////////

emscripten::enum_<metaf::Precipitation::Status>("MetafPrecipitationStatus")


} //EMSCRIPTEN_BINDINGS(metaf)

#endif //#ifdef __EMSCRIPTEN__

#endif //#ifndef METAF_EMBIND_H