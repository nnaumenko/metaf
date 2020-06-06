/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/
#ifndef METAF_HPP
#define METAF_HPP

#if defined(__clang__)
	// No issues with clang
	// But clang defines both __clang__ and __GNUC__
#elif defined(__GNUC__)
	// GCC gives false positives in GroupParser::parse
	#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
	// GCC gives numerous false positives in switch/return methods
	#pragma GCC diagnostic ignored "-Wreturn-type"
	// GCC gives false positives in Distance
	#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <regex>
#include <cmath>

namespace metaf {

// Metaf library version
struct Version {
	inline static const int major = 5;
	inline static const int minor = 3;
	inline static const int patch = 0;
	inline static const char tag [] = "";
};

class KeywordGroup;
class LocationGroup;
class ReportTimeGroup;
class TrendGroup;
class WindGroup;
class VisibilityGroup;
class CloudGroup;
class WeatherGroup;
class TemperatureGroup;
class PressureGroup;
class RunwayStateGroup;
class SeaSurfaceGroup;
class MinMaxTemperatureGroup;
class PrecipitationGroup;
class LayerForecastGroup;
class PressureTendencyGroup;
class CloudTypesGroup;
class LowMidHighCloudGroup;
class LightningGroup;
class VicinityGroup;
class MiscGroup;
class UnknownGroup;

using Group = std::variant<
	KeywordGroup,
	LocationGroup,
	ReportTimeGroup,
	TrendGroup,
	WindGroup,
	VisibilityGroup,
	CloudGroup,
	WeatherGroup,
	TemperatureGroup,
	PressureGroup,
	RunwayStateGroup,
	SeaSurfaceGroup,
	MinMaxTemperatureGroup,
	PrecipitationGroup,
	LayerForecastGroup,
	PressureTendencyGroup,
	CloudTypesGroup,
	LowMidHighCloudGroup,
	LightningGroup,
	VicinityGroup,
	MiscGroup,
	UnknownGroup
>;

///////////////////////////////////////////////////////////////////////////

class Runway {
public:
	enum class Designator {
		NONE,
		LEFT,
		CENTER,
		RIGHT,
	};
	unsigned int number() const { return rNumber; }
	Designator designator() const { return rDesignator; }
	bool isValid() const {
		return (rNumber <= maxRunwayNumber ||
			(rNumber == allRunwaysNumber && rDesignator == Designator::NONE) ||
			(rNumber == messageRepetitionNumber && rDesignator == Designator::NONE));
	}
	bool isAllRunways() const {
		return (rNumber == allRunwaysNumber && rDesignator == Designator::NONE);
	}
	bool isMessageRepetition() const {
		return (rNumber == messageRepetitionNumber && rDesignator == Designator::NONE);
	}

	Runway() = default;
	static inline std::optional<Runway> fromString(const std::string & s, bool enableRwy = false);
	static Runway makeAllRunways() {
		Runway rw;
		rw.rNumber = allRunwaysNumber;
		return rw;
	}

private:
	static inline std::optional<Designator> designatorFromChar(char c);

	unsigned int rNumber = 0;
	Designator rDesignator = Designator::NONE;
	static const unsigned int allRunwaysNumber = 88;
	static const unsigned int messageRepetitionNumber = 99;
	static const auto maxRunwayNumber = 360 / 10;
};

class MetafTime {
public:
	struct Date {
		Date(unsigned int y, unsigned int m, unsigned int d) :
			year(y), month(m), day(d) {}
		unsigned int year;
		unsigned int month;
		unsigned int day;
	};
	std::optional<unsigned int> day() const { return dayValue; }
	unsigned int hour() const { return hourValue; }
	unsigned int minute() const { return minuteValue; }
	inline bool isValid() const;
	inline bool is3hourlyReportTime() const;
	inline bool is6hourlyReportTime() const;
	inline Date dateBeforeRef(const Date & refDate) const;

	MetafTime() = default;
	MetafTime(unsigned int hour, unsigned int minute) :
		hourValue(hour), minuteValue(minute) {}
	static inline std::optional<MetafTime> fromStringDDHHMM(const std::string & s);
	static inline std::optional<MetafTime> fromStringDDHH(const std::string & s);

private:
	std::optional<unsigned int> dayValue;
	unsigned int hourValue = 0;
	unsigned int minuteValue = 0;

	static const inline unsigned int dayNotReported = 0;
	static const inline unsigned int maxDay = 31;
	static const inline unsigned int maxHour = 24;
	static const inline unsigned int maxMinute = 59;
};

class Speed;

class Temperature {
public:
	enum class Unit {
		C,
		F,
	};
	std::optional<float> temperature() const {
		if (!tempValue.has_value()) return tempValue;
		return (precise ? 
			(*tempValue * preciseValuePrecision) : *tempValue);
	}
	Unit unit() const { return tempUnit; }
	std::optional<float> inline toUnit(Unit unit) const;
	bool isFreezing() const { return freezing; }
	bool isPrecise() const { return precise; }
	bool isReported() const { return tempValue.has_value(); }
	inline static std::optional<float> relativeHumidity(
		const Temperature & airTemperature,
		const Temperature & dewPoint);
	inline static Temperature heatIndex(const Temperature & airTemperature,
		float relativeHumidity);
	inline static Temperature heatIndex(const Temperature & airTemperature,
		const Temperature & dewPoint);
	inline static Temperature windChill(const Temperature & airTemperature,
		const Speed & windSpeed);

	Temperature () = default;
	static inline std::optional<Temperature> fromString(const std::string & s);
	static inline std::optional<Temperature> fromRemarkString(const std::string & s);
private:
	inline Temperature (float value);

	std::optional<int> tempValue;
	bool freezing = false;
	static const Unit tempUnit = Unit::C;
	bool precise = false; //True = tenth of degrees C, false = degrees C
	static inline const float preciseValuePrecision = 0.1;
};

class Speed {
public:
	enum class Unit {
		KNOTS,
		METERS_PER_SECOND,
		KILOMETERS_PER_HOUR,
		MILES_PER_HOUR
	};
	std::optional<unsigned int> speed() const { return speedValue; }
	Unit unit() const { return speedUnit; }
	std::optional<float> inline toUnit(Unit unit) const;
	bool isReported() const { return speedValue.has_value(); }

	Speed() = default;
	static inline std::optional<Speed> fromString(const std::string & s, Unit unit);
	static inline std::optional<Unit> unitFromString(const std::string & s);

private:
	std::optional<unsigned int> speedValue;
	Unit speedUnit = Unit::KNOTS;

	static inline std::optional<float> knotsToUnit(float valueKnots, Unit otherUnit);
	static inline std::optional<float> mpsToUnit(float valueMps, Unit otherUnit);
	static inline std::optional<float> kmhToUnit(float valueKmh, Unit otherUnit);
	static inline std::optional<float> mphToUnit(float valueMph, Unit otherUnit);
};

class Distance {
public:
	enum class Unit {
		METERS,
		STATUTE_MILES,
		FEET
	};
	enum class Modifier {
		NONE,
		LESS_THAN,
		MORE_THAN,
		DISTANT,
		VICINITY
	};
	enum class MilesFraction {
		NONE,
		F_1_16,
		F_1_8,
		F_3_16,
		F_1_4,
		F_5_16,
		F_3_8,
		F_1_2,
		F_5_8,
		F_3_4,
		F_7_8
	};
	inline std::optional<float> distance() const;
	Unit unit() const { return distUnit; }
	Modifier modifier() const { return distModifier; }
	bool isValue() const { return (dist.has_value()); }
	bool isReported() const {
		return (isValue() || 
			modifier() == Modifier::DISTANT || 
			modifier() == Modifier::VICINITY);
	}
	inline std::optional<float> toUnit(Unit unit) const;
	inline std::optional<std::pair<unsigned int, MilesFraction>> miles() const;

	bool isValid() const { return true; }

	Distance() = default;
	Distance(unsigned int d, Unit u) : 
		dist (d * (u == Unit::STATUTE_MILES ? statuteMileFactor : 1)), distUnit(u) {}
	Distance(unsigned int numerator, unsigned int denominator) {
		if (!denominator) return;
		dist = numerator * statuteMileFactor / denominator;
		distUnit = Unit::STATUTE_MILES;
	}
	Distance(Unit u) : distUnit(u) {}
	static inline std::optional<Distance> fromIntegerAndFraction(const Distance & integer,
		const Distance & fraction);
	static inline std::optional<Distance> fromMeterString(const std::string & s);
	static inline std::optional<Distance> fromMileString(const std::string & s,
		bool remarkFormat = false);
	static inline std::optional<Distance> fromHeightString(const std::string & s);
	static inline std::optional<Distance> fromRvrString(const std::string & s, bool unitFeet);
	static inline std::optional< std::pair<Distance,Distance> > fromLayerString(
		const std::string & s);
	static inline Distance cavokVisibility(bool unitMiles = false);
	static inline std::optional<Distance> fromKmString(const std::string & s);
	static inline Distance makeDistant();
	static inline Distance makeVicinity();
private:
	Modifier distModifier = Modifier::NONE;
	std::optional<unsigned int> dist;
	Unit distUnit = Unit::METERS;

	// If distance unit is statute mile, d stores value in 1/10000ths of mile
	// This allows precision down to 1/16th (0.0625) mile without digit loss
	static const unsigned int statuteMileFactor = 10000;

	static const unsigned int heightFactor = 100; //height unit is 100s of feet

	static const inline unsigned int cavokVisibilityMiles = 6;
	static const inline unsigned int cavokVisibilityMeters = 10000;

	// Icing or turbulence layer depth is given in 1000s of feet
	static const unsigned int layerDepthFactor = 1000;

	static inline std::optional<Modifier> modifierFromChar(char c);
	static inline std::optional<float> metersToUnit(float value, Unit unit);
	static inline std::optional<float> milesToUnit(float value, Unit unit);
	static inline std::optional<float> feetToUnit(float value, Unit unit);
};

class Direction {
public:
	enum class Type {
		NOT_REPORTED,	// Direction is specified as not reported
		VARIABLE,		// Direction is reported as variable
		NDV,			// Direction is reported as No Directional Variation
		VALUE_DEGREES, 	// Direction is reported as value in degrees
		VALUE_CARDINAL,	// Direction is reported as cardinal value
		OVERHEAD,		// Phenomena occurring directly over the location
		ALQDS,			// Direction is reported as all quadrants (in all directions)
		UNKNOWN 		// Direction is reported as unknown explicitly
	};
	enum class Cardinal {
		NOT_REPORTED, // Not reported or no corresponding cardinal direction
		VRB,	// Direction is variable
		NDV,	// No Directional Variation
		N,
		S,
		W,
		E,
		NW,
		NE,
		SW,
		SE,
		TRUE_N,
		TRUE_W,
		TRUE_S,
		TRUE_E,
		OHD,	// Overhead
		ALQDS,	// All quadrants
		UNKNOWN // Unknown
	};
	Type type() const { return dirType; }
	inline Cardinal cardinal(bool trueDirections = false) const;
	std::optional<unsigned int> degrees() const {
		if (!isValue())	return std::optional<unsigned int>();
		return dirDegrees;
	}
	bool isValue() const {
		return (dirType == Type::VALUE_DEGREES || dirType == Type::VALUE_CARDINAL);
	}
	bool isReported() const { return (dirType != Type::NOT_REPORTED); }
	static inline std::vector<Direction> sectorCardinalDirToVector(
		const Direction & dirFrom, 
		const Direction & dirTo);
	bool isValid() const {
		if (isValue() && dirDegrees > maxDegrees) return false;
		return true;
	}
	static inline Cardinal rotateOctantClockwise(Cardinal cardinal);

	Direction() = default;
	static inline std::optional<Direction> fromCardinalString(const std::string & s,
		bool enableOhdAlqds = false,
		bool enableUnknown = false);
	static inline std::optional<Direction> fromDegreesString(const std::string & s);
	static inline std::optional<std::pair<Direction, Direction>> fromSectorString(
		const std::string & s);

private:
	unsigned int dirDegrees = 0;
	Type dirType = Type::NOT_REPORTED;
private:
	static const inline unsigned int maxDegrees = 360;
	static const inline unsigned int octantSize = 45u;
	static const inline unsigned int degreesTrueNorth = 360;
	static const inline unsigned int degreesTrueWest = 270;
	static const inline unsigned int degreesTrueSouth = 180;
	static const inline unsigned int degreesTrueEast = 90;
	static const inline unsigned int degreesNorthWest = 315;
	static const inline unsigned int degreesNorthEast = 45;
	static const inline unsigned int degreesSouthWest = 225;
	static const inline unsigned int degreesSouthEast = 135;

	inline Direction(Cardinal c);
};

class Pressure {
public:
	enum class Unit {
		HECTOPASCAL,
		INCHES_HG,
		MM_HG
	};
	std::optional<float> pressure() const { return pressureValue; }
	Unit unit() const { return pressureUnit; }
	inline std::optional<float> toUnit(Unit unit) const;
	bool isReported() const { return pressureValue.has_value(); }

	Pressure() = default;
	static inline std::optional<Pressure> fromString(const std::string & s);
	static inline std::optional<Pressure> fromForecastString(const std::string & s);
	static inline std::optional<Pressure> fromSlpString(const std::string & s);
	static inline std::optional<Pressure> fromQfeString(const std::string & s);
	static inline std::optional<Pressure> fromTendencyString(const std::string & s);

private:
	std::optional<float> pressureValue;
	Unit pressureUnit = Unit::HECTOPASCAL;

	static inline const float inHgDecimalPointShift = 0.01;
	static inline const float tendencyDecimalPointShift = 0.1;
};

class Precipitation {
public:
	enum class Unit {
		MM,
		INCHES,
	};
	std::optional<float> amount() const { return precipValue; }
	Unit unit() const { return precipUnit; }
	inline std::optional<float> toUnit(Unit unit) const;
	bool isReported() const { return precipValue.has_value(); }

	Precipitation() = default;
	static inline std::optional<Precipitation> fromRainfallString(const std::string & s);
	static inline std::optional<Precipitation> fromRunwayDeposits(const std::string & s);
	static inline std::optional<Precipitation> fromRemarkString(const std::string & s,
		float factor = 1,
		Unit unit = Unit::INCHES, 
		bool allowNotReported = false);
	static inline std::optional<std::pair<Precipitation, Precipitation> >
		fromSnincrString(const std::string & s);

private:
	std::optional<float> precipValue;
	Unit precipUnit = Unit::MM;
private:
	// Special value for runway deposits depth, see Table 1079 in Manual on Codes (WMO No. 306).
	enum Reserved {
		RESERVED = 91,
		DEPTH_10CM = 92,
		DEPTH_15CM = 93,
		DEPTH_20CM = 94,
		DEPTH_25CM = 95,
		DEPTH_30CM = 96,
		DEPTH_35CM = 97,
		DEPTH_40CM = 98,
		RUNWAY_NOT_OPERATIONAL = 99
	};
};

class SurfaceFriction {
public:
	enum class Type {
		NOT_REPORTED,
		SURFACE_FRICTION_REPORTED,
		BRAKING_ACTION_REPORTED,
		UNRELIABLE 		// Value unreliable or unmeasurable.
	};
	enum class BrakingAction {
		NONE,			// Not reported or unreliable
		POOR,	 		// Friction coef <0.26
		MEDIUM_POOR,	// Friction coef 0.26 to 0.29
		MEDIUM,			// Friction coef 0.30 to 0.35
		MEDIUM_GOOD,	// Friction coef 0.36 to 0.39
		GOOD,			// Friction coef >0.39
	};
	Type type() const { return sfType; }
	std::optional<float> coefficient() const {
		if (sfType == Type::NOT_REPORTED ||
			sfType == Type::UNRELIABLE) return std::optional<float>();
		return (sfCoefficient * coefficientDecimalPointShift);
	}
	inline BrakingAction brakingAction() const;
	bool isReported() const { return (type() != Type::NOT_REPORTED); }
	bool isUnreliable() const { return (type() == Type::UNRELIABLE); }

	SurfaceFriction() = default;
	static inline std::optional<SurfaceFriction> fromString(const std::string & s);

private:
	Type sfType = Type::NOT_REPORTED;
	unsigned int sfCoefficient = 0; //0 to 100, multiply by 0.01 to get actual value
	static const inline auto coefficientDecimalPointShift = 0.01;
private:
	// Special values for braking action, see Table 0366 in Manual on Codes (WMO No. 306).
	enum Reserved {
		BRAKING_ACTION_POOR = 91,
		BRAKING_ACTION_MEDIUM_POOR = 92,
		BRAKING_ACTION_MEDIUM = 93,
		BRAKING_ACTION_MEDIUM_GOOD = 94,
		BRAKING_ACTION_GOOD = 95,
		RESERVED_96 = 96,
		RESERVED_97 = 97,
		RESERVED_98 = 98,
		UNRELIABLE = 99,
	};
	static const inline auto baPoorLowLimit = 0u;
	static const inline auto baMediumPoorLowLimit = 26u;
	static const inline auto baMediumLowLimit = 30u;
	static const inline auto baMediumGoodLowLimit = 36u;
	static const inline auto baGoodLowLimit = 40u;
};

class WaveHeight {
public:
	enum class Type {
		STATE_OF_SURFACE,	// Descriptive state of surface is specified
		WAVE_HEIGHT,		// Actual wave height is specified
	};
	enum class Unit {
		METERS,
		FEET,
	};
	// State of sea surface, see Table 3700 in Manual on Codes (WMO No. 306).
	enum class StateOfSurface {
		NOT_REPORTED,
		CALM_GLASSY,
		CALM_RIPPLED,
		SMOOTH,
		SLIGHT,
		MODERATE,
		ROUGH,
		VERY_ROUGH,
		HIGH,
		VERY_HIGH,
		PHENOMENAL,
	};
	Type type() const { return whType; }
	inline StateOfSurface stateOfSurface() const;
	Unit unit() const { return whUnit; }
	std::optional<float> waveHeight() const {
		if (!whValue.has_value()) return std::optional<float>();
		return (*whValue * waveHeightDecimalPointShift);
	}
	bool isReported() const { return whValue.has_value(); }
	inline std::optional<float> toUnit(Unit unit) const;

	WaveHeight() = default;
	static inline std::optional<WaveHeight> fromString(const std::string & s);

private:
	Type whType = Type::STATE_OF_SURFACE;
	std::optional<unsigned int> whValue; //in decimeters, muliply by 0.1 to get value in meters
	static const inline auto waveHeightDecimalPointShift = 0.1;
	static const Unit whUnit = Unit::METERS;
private:
	static inline std::optional<unsigned int> waveHeightFromStateOfSurfaceChar(char c);
private:
	//Values below are in decimeters, muliply by 0.1 to get value in meters
	static const inline auto maxWaveHeightCalmGlassy = 0;
	static const inline auto maxWaveHeightCalmRippled = 1;
	static const inline auto maxWaveHeightSmooth = 5;
	static const inline auto maxWaveHeightSlight = 12;
	static const inline auto maxWaveHeightModerate = 25;
	static const inline auto maxWaveHeightRough = 40;
	static const inline auto maxWaveHeightVeryRough = 60;
	static const inline auto maxWaveHeightHigh = 90;
	static const inline auto maxWaveHeightVeryHigh = 140;
	static const inline auto minWaveHeightPhenomenal = 141;
};

// Describes recent, current, or forecast weather phenomena
class WeatherPhenomena {
public:
	enum class Qualifier {
		NONE,
		RECENT,
		VICINITY,
		LIGHT,
		MODERATE,
		HEAVY
	};
	enum class Descriptor {
		NONE,
		SHALLOW,
		PARTIAL,
		PATCHES,
		LOW_DRIFTING,
		BLOWING,
		SHOWERS,
		THUNDERSTORM,
		FREEZING
	};	
	enum class Weather {
		NOT_REPORTED,
		DRIZZLE,
		RAIN,
		SNOW,
		SNOW_GRAINS,
		ICE_CRYSTALS,
		ICE_PELLETS,
		HAIL,
		SMALL_HAIL,
		UNDETERMINED,
		MIST,
		FOG,
		SMOKE,
		VOLCANIC_ASH,
		DUST,
		SAND,
		HAZE,
		SPRAY,
		DUST_WHIRLS,
		SQUALLS,
		FUNNEL_CLOUD,
		SANDSTORM,
		DUSTSTORM
	};
	enum class Event {
		NONE,
		BEGINNING,
		ENDING
	};

	Qualifier qualifier() const { return q; }
	Descriptor descriptor() const { return d; }
	inline std::vector<Weather> weather() const;
	Event event() const { return ev; }
	std::optional<MetafTime> time() const { return tm; }
	inline bool isValid() const;

	WeatherPhenomena() = default;
	static inline std::optional <WeatherPhenomena> fromString(const std::string & s,
		bool enableQualifiers = false);
	static inline std::optional <WeatherPhenomena> fromWeatherBeginEndString(
		const std::string & s,
		const MetafTime & reportTime,
		const WeatherPhenomena & previous);
	static WeatherPhenomena notReported(bool recent) {
		const auto q = recent ? Qualifier::RECENT : Qualifier::NONE;
		return WeatherPhenomena(Weather::NOT_REPORTED, Descriptor::NONE, q);
	}

private:
	Qualifier q = Qualifier::NONE;
	Descriptor d = Descriptor::NONE;
	static const inline size_t wSize = 3;
	size_t wsz = 0;
	Weather w[wSize] = {Weather::NOT_REPORTED, Weather::NOT_REPORTED, Weather::NOT_REPORTED};
	Event ev = Event::NONE;
	std::optional<MetafTime> tm;

	WeatherPhenomena(Weather wthr1, 
		Weather wthr2, 
		Descriptor dscr = Descriptor::NONE, 
		Qualifier qlf = Qualifier::NONE) : q(qlf), d(dscr), wsz(2), w{wthr1, wthr2} {}
	WeatherPhenomena(Weather wthr, 
		Descriptor dscr = Descriptor::NONE, 
		Qualifier qlf = Qualifier::NONE) : q(qlf), d(dscr), wsz(1), w{wthr} {}
	WeatherPhenomena(Descriptor dscr, Qualifier qlf = Qualifier::NONE) : q(qlf), d(dscr) {}

	static inline bool isDescriptorShAllowed (Weather w);
	static inline bool isDescriptorTsAllowed (Weather w);
	static inline bool isDescriptorFzAllowed (Weather w);
};

class CloudType {
public:
	enum class Type {
		NOT_REPORTED,
		//Low clouds
		CUMULONIMBUS,
		TOWERING_CUMULUS,
		CUMULUS,
		CUMULUS_FRACTUS,
		STRATOCUMULUS,
		NIMBOSTRATUS,
		STRATUS,
		STRATUS_FRACTUS,
		//Med clouds
		ALTOSTRATUS,
		ALTOCUMULUS,
		ALTOCUMULUS_CASTELLANUS,
		//High clouds
		CIRRUS,
		CIRROSTRATUS,
		CIRROCUMULUS,
		//Obscurations
		BLOWING_SNOW,
		BLOWING_DUST,
		BLOWING_SAND,
		ICE_CRYSTALS,
		RAIN,
		DRIZZLE,
		SNOW,
		ICE_PELLETS,
		SMOKE,
		FOG,
		MIST,
		HAZE,
		VOLCANIC_ASH
	};
	Type type() const { return tp; }
	Distance height() const { return ht; }
	unsigned int okta() const { return okt; }
	bool isValid() const { return (okt >= 1u && okt <= 8u); }

	CloudType() = default;
	CloudType(Type t, Distance h, unsigned int o) : tp(t), ht(h), okt(o) {}
	static inline std::optional<CloudType> fromString(const std::string & s);
	static inline std::optional<CloudType> fromStringObscuration(const std::string & s);
private:
	Type tp = Type::NOT_REPORTED;
	Distance ht;
	unsigned int okt = 0u;

	static inline Type cloudTypeFromString(const std::string & s);
	static inline Type cloudTypeOrObscurationFromString(const std::string & s);
};

///////////////////////////////////////////////////////////////////////////

enum class ReportType {
	UNKNOWN,
	METAR,
	TAF
};

enum class ReportPart {
	UNKNOWN,
	HEADER,
	METAR,
	TAF,
	RMK
};

enum class ReportError {
	NONE,
	EMPTY_REPORT,
	EXPECTED_REPORT_TYPE_OR_LOCATION,
	EXPECTED_LOCATION,
	EXPECTED_REPORT_TIME,
	EXPECTED_TIME_SPAN,
	UNEXPECTED_REPORT_END,
	UNEXPECTED_GROUP_AFTER_NIL,
	UNEXPECTED_GROUP_AFTER_CNL,
	UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY,
	AMD_ALLOWED_IN_TAF_ONLY,
	CNL_ALLOWED_IN_TAF_ONLY,
	MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY,
	REPORT_TOO_LARGE
};

struct ReportMetadata {
	ReportType type = ReportType::UNKNOWN;
	ReportError error = ReportError::NONE;
	std::optional<MetafTime> reportTime;
	std::string icaoLocation;
	bool isSpeci = false;
	bool isNospeci = false;
	bool isAutomated = false;
	bool isAo1 = false;
	bool isAo1a = false;
	bool isAo2 = false;
	bool isAo2a = false;
	bool isNil = false;
	bool isCancelled = false;
	bool isAmended = false;
	bool isCorrectional = false;
	std::optional<unsigned int> correctionNumber = 0;
	bool maintenanceIndicator = false;
	std::optional<MetafTime> timeSpanFrom, timeSpanUntil;
};

static const inline ReportMetadata missingMetadata;

///////////////////////////////////////////////////////////////////////////

// Result of appending string to an existing group
enum class AppendResult {
	// String was appended to the group (i.e. the string is a continuation of
	// the previous group); all info from the string is already absorbed into
	// the group; no need to parse the string.
	// E.g. visibility group "1" will append string "1/2SM" since together
	// they form visibility group "1 1/2SM".
	APPENDED,
	// String could not be appended to this group (i.e. the string is a
	// separate group independent from last group); the parser must attempt
	// to parse the string as a group
	// E.g. group "BKN040" will not append string "22/20" since these two
	// groups represent unrelated data (clouds and temperature).
	NOT_APPENDED,
	// The group expected particular string to follow and this expectation
	// was not met; the group is not valid without the expected string and
	// must be converted to raw string; the following string must still be
	// attempted to be parsed.
	// E.g. group "PK" expects next group string "WND", and if instead
	// string "AO2" follows, it means that the original group "PK" is not
	// valid and must be converted to raw string, whereas "AO2" may still
	// represent a valid group and must be parsed separately.
	GROUP_INVALIDATED
};

///////////////////////////////////////////////////////////////////////////

// Default delimiter between groups
// Note: only used to append raw strings, see also groupDelimiterRegex
static const inline char groupDelimiterChar = ' ';

// Everything after this char is ignored by parser
static const inline char reportEndChar = '=';

// Fallback group is the group which stores raw string in
// case when no other group is able to recognise the content
// (i.e. if anything else fails)
using FallbackGroup = UnknownGroup;

///////////////////////////////////////////////////////////////////////////

class KeywordGroup {
public:
	enum class Type {
		METAR,
		SPECI,
		TAF,
		AMD,
		NIL,
		CNL,
		COR,
		AUTO,
		CAVOK,
		RMK,
		MAINTENANCE_INDICATOR,
		AO1,
		AO2,
		AO1A,
		AO2A,
		NOSPECI
	};
	Type type() const { return t; }
	bool isValid() const { return (true); }

	KeywordGroup() = default;
	static inline std::optional<KeywordGroup> parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	Type t;
	KeywordGroup(Type type) :t (type) {}
};

class LocationGroup {
public:
	std::string toString() const {return std::string(location);}
	inline bool isValid() const { return true; }

	LocationGroup() = default;
	static inline std::optional<LocationGroup> parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	static const inline auto locationLength = 4;
	char location [locationLength + 1] = "\0";
};

class ReportTimeGroup {
public:
	MetafTime time() const { return t; };
	bool isValid() const { return (t.isValid() && t.day().has_value()); }

	ReportTimeGroup() = default;
	static inline std::optional<ReportTimeGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	MetafTime t;
};

class TrendGroup {
public:
	enum class Type {
		NOSIG,
		BECMG,
		TEMPO,
		INTER,
		FROM,
		UNTIL,
		AT,
		TIME_SPAN,
		PROB
	};
	enum class Probability {
		NONE,		// Probability not specified.
		PROB_30,
		PROB_40,
	};
	Type type() const { return t; }
	Probability probability() const { return prob; }
	std::optional<MetafTime> timeFrom() const { return tFrom; }
	std::optional<MetafTime> timeUntil() const { return tTill; }
	std::optional<MetafTime> timeAt() const { return tAt; }
	bool isValid() const {
		if (type() == Type::PROB) return false; //PROBxx without time span, BECMG, TEMPO, INTER
		if (tFrom.has_value() && !tFrom->isValid()) return false;
		if (tTill.has_value() && !tTill->isValid()) return false;
		if (tAt.has_value() && !tAt->isValid()) return false;
		return true;
	}
	inline bool isTimeSpanGroup() const;

	TrendGroup() = default;
	static inline std::optional<TrendGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	TrendGroup(Type type) : t(type) {}
	TrendGroup(Probability p) : t(Type::PROB), prob(p) {}

	static inline std::optional<TrendGroup> fromTimeSpan(const std::string & s);
	static inline std::optional<TrendGroup> fromTimeSpanHHMM(const std::string & s);
	static inline std::optional<TrendGroup> fromFm(const std::string & s);
	static inline std::optional<TrendGroup> fromTrendTime(const std::string & s);

	inline bool combineProbAndTrendTypeGroups(const TrendGroup & nextTrendGroup);
	inline bool combineTrendTypeAndTimeGroup(const TrendGroup & nextTrendGroup);
	inline bool combineProbAndTimeSpanGroups(const TrendGroup & nextTrendGroup);
	inline bool combineIncompleteGroups(const TrendGroup & nextTrendGroup);

	static inline bool canCombineTime(const TrendGroup & g1, const TrendGroup & g2);
	inline void combineTime(const TrendGroup & nextTrendGroup);

	inline bool isProbabilityGroup() const;
	inline bool isTrendTypeGroup() const;
	inline bool isTrendTimeGroup() const;

	Type t = Type::NOSIG;
	Probability prob = Probability::NONE;
	bool isTafTimeSpanGroup = false;
	std::optional<MetafTime> tFrom; // Time span beginning.
	std::optional<MetafTime> tTill;	// Time span end time.
	std::optional<MetafTime> tAt;	// Precise time.
};

class WindGroup {
public:
	enum class Type {
		SURFACE_WIND,
		SURFACE_WIND_CALM,
		VARIABLE_WIND_SECTOR,
		SURFACE_WIND_WITH_VARIABLE_SECTOR,
		WIND_SHEAR,
		WIND_SHEAR_IN_LOWER_LAYERS,
		WIND_SHIFT,
		WIND_SHIFT_FROPA,
		PEAK_WIND,
		WSCONDS,
		WND_MISG
	};
	Type type() const { return windType; }
	Direction direction() const { return windDir; }
	Speed windSpeed() const { return wSpeed; }
	Speed gustSpeed() const { return gSpeed; }
	Distance height() const { return wShHeight; }
	Direction varSectorBegin() const { return vsecBegin; }
	Direction varSectorEnd() const { return vsecEnd; }
	std::optional<MetafTime> eventTime() const { return evTime; }
	std::optional<Runway> runway() const { return rw; }
	inline bool isValid() const;

	WindGroup() = default;
	static inline std::optional<WindGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);
private:
	enum class IncompleteText {
		NONE,
		PK,
		PK_WND,
		WND,
		WS,
		WS_ALL
	};
	WindGroup(Type type, IncompleteText incomplete = IncompleteText::NONE) :
		windType(type), incompleteText(incomplete) {}
	static inline std::optional<WindGroup> parseVariableSector(
		const std::string & group);
	inline AppendResult appendPeakWind(const std::string & group,
		const ReportMetadata & reportMetadata);
	inline AppendResult appendWindShift(const std::string & group,
		const ReportMetadata & reportMetadata);
	inline AppendResult appendVariableSector(const std::string & group);

	Type windType;
	Direction windDir;
	Speed wSpeed;
	Speed gSpeed;
	Distance wShHeight;
	Direction vsecBegin;
	Direction vsecEnd;
	std::optional<MetafTime> evTime;
	std::optional<Runway> rw;

	IncompleteText incompleteText = IncompleteText::NONE;

};

class VisibilityGroup {
public:
	enum class Type {
		PREVAILING,
		PREVAILING_NDV,
		DIRECTIONAL,
		RUNWAY,
		RVR,
		SURFACE,
		TOWER,
		SECTOR,
		VARIABLE_PREVAILING,
		VARIABLE_DIRECTIONAL,
		VARIABLE_RUNWAY,
		VARIABLE_RVR,
		VARIABLE_SECTOR,
		VIS_MISG,
		RVR_MISG,
		RVRNO,
		VISNO
	};
	enum class Trend {
		NONE,
		NOT_REPORTED,
		UPWARD,
		NEUTRAL,
		DOWNWARD
	};
	Type type() const { return visType; }
	Distance visibility() const { 
		if (isVariable()) return Distance();
		return vis;
	}
	Distance minVisibility() const { 
		if (!isVariable()) return Distance();
		return vis;
	}
	Distance maxVisibility() const { return visMax; }
	std::optional<Direction> direction() const { return dir; }
	std::optional<Runway> runway() const { return rw; }
	std::vector<Direction> sectorDirections() const {
		return Direction::sectorCardinalDirToVector(
			dirSecFrom.value_or(Direction()), 
			dirSecTo.value_or(Direction()));
	}
	Trend trend() const { return rvrTrend; }
	inline bool isValid() const;

	VisibilityGroup() = default;
	inline static std::optional<VisibilityGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);
private:
	enum class IncompleteText {
		NONE,
		INTEGER,
		VIS,
		VIS_INTEGER,
		VIS_FRACTION_OR_METERS,
		VIS_VAR_MAX_INT,
		VIS_VAR,
		VIS_DIR,
		VIS_DIR_INTEGER,
		VIS_DIR_VAR_MAX_INT,
		SFC_OR_TWR,
		SFC_OR_TWR_VIS,
		SFC_OR_TWR_VIS_INTEGER,
		RVR,
		VISNO
	};

	inline VisibilityGroup(Type t, IncompleteText i) : visType(t), incompleteText(i) {}

	bool isVariable() const {
		switch (type()) {
			case Type::VARIABLE_PREVAILING:
			case Type::VARIABLE_DIRECTIONAL:
			case Type::VARIABLE_RUNWAY:
			case Type::VARIABLE_RVR:
			case Type::VARIABLE_SECTOR:
			return true;

			default: return false;
		}
	}

	void makeVariable() {
		switch(visType) {
			case Type::PREVAILING: visType = Type::VARIABLE_PREVAILING; break;
			case Type::DIRECTIONAL: visType = Type::VARIABLE_DIRECTIONAL; break;
			case Type::SECTOR: visType = Type::VARIABLE_SECTOR; break;
			default: break;
		}
	}

	static inline std::optional<VisibilityGroup> fromIncompleteInteger(const std::string & group);
	static inline std::optional<VisibilityGroup> fromMeters(const std::string & group);
	static inline std::optional<VisibilityGroup> fromRvr(const std::string & group);

	inline bool appendFractionToIncompleteInteger(const std::string & group, 
		IncompleteText next = IncompleteText::NONE);
	inline bool appendDirection(const std::string & group, 
		IncompleteText next = IncompleteText::NONE);
	inline bool appendRunway(const std::string & group, 
		IncompleteText next = IncompleteText::NONE);
	inline bool appendInteger(const std::string & group, 
		IncompleteText next = IncompleteText::NONE);
	inline bool appendFraction(const std::string & group, 
		IncompleteText next = IncompleteText::NONE);
	inline bool appendVariable(const std::string & group, 
		IncompleteText nextIfMaxIsInteger = IncompleteText::NONE,
		IncompleteText nextIfMaxIsFraction = IncompleteText::NONE);
	inline bool appendVariableMaxFraction(const std::string & group,
		IncompleteText next = IncompleteText::NONE);
	inline bool appendMeters(const std::string & group, 
		IncompleteText next = IncompleteText::NONE);
	inline bool appendVariableMeters(const std::string & group, 
		IncompleteText next = IncompleteText::NONE);

	static inline Trend trendFromString(const std::string & s);

	Type visType = Type::PREVAILING;
	Distance vis;
	Distance visMax;
	Trend rvrTrend = Trend::NONE;
	std::optional<Direction> dir;
	std::optional<Runway> rw;
	std::optional<Direction> dirSecFrom;
	std::optional<Direction> dirSecTo;

	IncompleteText incompleteText = IncompleteText::NONE;
};

class CloudGroup {
public:
	enum class Type {
		NO_CLOUDS,
		CLOUD_LAYER,
		VERTICAL_VISIBILITY,
		CEILING,
		VARIABLE_CEILING,
		CHINO,
		CLD_MISG,
		OBSCURATION
	};
	enum class Amount {
		NOT_REPORTED,
		NCD,
		NSC,
		NONE_CLR,
		NONE_SKC,
		FEW,
		SCATTERED,
		BROKEN,
		OVERCAST,
		OBSCURED,
		VARIABLE_FEW_SCATTERED,
		VARIABLE_SCATTERED_BROKEN,
		VARIABLE_BROKEN_OVERCAST
	};
	enum class ConvectiveType {
		NONE,
		NOT_REPORTED,
		TOWERING_CUMULUS,
		CUMULONIMBUS
	};
	Type type() const { return tp; }
	Amount amount() const { return amnt; }
	ConvectiveType convectiveType() const { return convtype; }
	inline Distance height() const {
		if (type() != Type::CLOUD_LAYER && 
			type() != Type::CEILING &&
			type() != Type::OBSCURATION) return heightNotReported;
		return heightOrVertVis;
	}
	inline Distance minHeight() const {
		if (type() != Type::VARIABLE_CEILING) return heightNotReported;
		return heightOrVertVis;
	}
	inline Distance maxHeight() const {
		if (type() != Type::VARIABLE_CEILING) return heightNotReported;
		return maxHt;
	}
	Distance verticalVisibility() const {
		if (type() != Type::VERTICAL_VISIBILITY) return heightNotReported;
		return heightOrVertVis;
	}
	inline std::optional<CloudType> cloudType() const;
	std::optional<Runway> runway() const { return rw; }
	std::optional<Direction> direction() const { return dir; }
	bool isValid() const { 
		if (rw.has_value() && !rw->isValid()) return false;
		if (dir.has_value() && !dir->isValid()) return false;
		return (heightOrVertVis.isValid() && maxHt.isValid());
	}

	CloudGroup () = default;
	static inline std::optional<CloudGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);
private:
	Type tp = Type::CLOUD_LAYER;
	Amount amnt = Amount::NOT_REPORTED;
	Distance heightOrVertVis;
	Distance maxHt;
	ConvectiveType convtype = ConvectiveType::NONE;
	WeatherPhenomena w;
	std::optional<Runway> rw;
	std::optional<Direction> dir;
	CloudType cldTp;
	static const inline auto heightNotReported = Distance(Distance::Unit::FEET);

	enum class IncompleteText {
		NONE,
		RMK_AMOUNT,
		RMK_AMOUNT_V,
		CIG,
		CIG_NUM,
		CHINO,
		CLD,
		OBSCURATION
	};
	IncompleteText incompleteText = IncompleteText::NONE;

	CloudGroup(Type t, IncompleteText it) : tp(t), incompleteText(it) {}
	CloudGroup(Type t, Amount a = Amount::NOT_REPORTED) : tp(t), amnt(a) {}
	static inline std::optional<CloudGroup> parseCloudLayerOrVertVis(const std::string & s);
	static inline std::optional<CloudGroup> parseVariableCloudLayer(const std::string & s);
	static inline std::optional<Amount> amountFromString(const std::string & s);
	static inline std::optional<ConvectiveType> convectiveTypeFromString(const std::string & s);
	inline AppendResult appendVariableCloudAmount(const std::string & group);
	inline AppendResult appendCeiling(const std::string & group);
	inline AppendResult appendRunwayOrCardinalDirection(const std::string & group);
	inline AppendResult appendObscuration(const std::string & group);
	static inline unsigned int amountToMaxOkta(Amount a);
	static inline CloudType::Type convectiveTypeToCloudTypeType(ConvectiveType t);
};

class WeatherGroup {
public:
	enum class Type {
		CURRENT,
		RECENT,
		EVENT,
		NSW,
		PWINO,
		WX_MISG,
		TSNO,
		TS_LTNG_TEMPO_UNAVBL
	};
	Type type() const { return t; }
	inline std::vector<WeatherPhenomena> weatherPhenomena() const;
	bool isValid() const {
		if (incompleteText != IncompleteText::NONE) return false;
		for (auto i=0u; i < wsz; i++) 
			if (!w[i].isValid()) return false;
		return true;
	}

	WeatherGroup() = default;
	static inline std::optional<WeatherGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	enum class IncompleteText {
		NONE,
		WX,
		TSLTNG,
		TSLTNG_TEMPO
	};

	inline bool addWeatherPhenomena(const WeatherPhenomena & wp);

	Type t = Type::CURRENT;
	static const inline size_t wSize = 10;
	size_t wsz = 0;
	WeatherPhenomena w[wSize];
	IncompleteText incompleteText = IncompleteText::NONE;

	WeatherGroup(Type tp, IncompleteText i = IncompleteText::NONE) : t(tp), incompleteText(i) {}
	static inline WeatherGroup notReported();
	static inline WeatherGroup notReportedRecent();

	static inline std::optional<WeatherPhenomena> parseWeatherWithoutEvent(
		const std::string & group, 
		ReportPart reportPart);
	static inline std::optional<WeatherGroup> parseWeatherEvent(
		const std::string & group, 
		const MetafTime & reportTime);
};

class TemperatureGroup {
public:
	enum class Type {
		TEMPERATURE_AND_DEW_POINT,
		T_MISG,
		TD_MISG
	};
	Type type() const { return tp; }
	Temperature airTemperature() const { return t; }
	Temperature dewPoint() const { return dp; }
	std::optional<float> relativeHumidity() const {
		return Temperature::relativeHumidity(airTemperature(), dewPoint());
	}
	inline bool isValid() const;

	TemperatureGroup() = default;
	static inline std::optional<TemperatureGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	Type tp = Type::TEMPERATURE_AND_DEW_POINT;
	Temperature t;
	Temperature dp;
	bool isIncomplete = false;

	TemperatureGroup(Type t, bool i = false) : tp(t), isIncomplete(i) {}
};

class PressureGroup {
public:
	enum class Type {
		OBSERVED_QNH,			//Observed mean sea level pressure (METAR)
		FORECAST_LOWEST_QNH,	//Forecast lowest sea level pressure
		OBSERVED_QFE,			//Observed actual (non-normalised) pressure
		SLPNO,					//Mean sea-level pressure information is not available
		PRES_MISG,				//Atmospheric pressure (altimeter) data is missing
	};
	Type type() const { return t; }
	Pressure atmosphericPressure() const { return p; }
	bool isValid() const { return (!isIncomplete); }

	PressureGroup() = default;
	static inline std::optional<PressureGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	Type t = Type::OBSERVED_QNH;
	Pressure p;
	bool isIncomplete = false;

	PressureGroup (Type tp, bool i = false) : t(tp), isIncomplete(i) {} 
};

class RunwayStateGroup {
public:
	enum class Type {
		RUNWAY_STATE,
		RUNWAY_CLRD,
		RUNWAY_SNOCLO,
		RUNWAY_NOT_OPERATIONAL,
		AERODROME_SNOCLO
	};
	enum class Deposits {// Deposits type, see Table 0919 in Manual on Codes (WMO No. 306).
		CLEAR_AND_DRY,
		DAMP,
		WET_AND_WATER_PATCHES,
		RIME_AND_FROST_COVERED,
		DRY_SNOW,
		WET_SNOW,
		SLUSH,
		ICE,
		COMPACTED_OR_ROLLED_SNOW,
		FROZEN_RUTS_OR_RIDGES,
		NOT_REPORTED
	};

	enum class Extent {// Extent of runway contamination, see Table 0519 in Manual on Codes (WMO No. 306).
		NONE,
		LESS_THAN_10_PERCENT,
		FROM_11_TO_25_PERCENT,
		RESERVED_3,
		RESERVED_4,
		FROM_26_TO_50_PERCENT,
		RESERVED_6,
		RESERVED_7,
		RESERVED_8,
		MORE_THAN_51_PERCENT,
		NOT_REPORTED
	};
	Runway runway() const { return rw; }
	Type type() const { return tp; }
	Deposits deposits() const { return dp; }
	Extent contaminationExtent() const { return ext; }
	Precipitation depositDepth() const { return dDepth; }
	SurfaceFriction surfaceFriction() const { return sf; }
	bool isValid() const {
		return (rw.isValid() &&
			ext != Extent::RESERVED_3 && 
			ext != Extent::RESERVED_4 &&
			ext != Extent::RESERVED_6 && 
			ext != Extent::RESERVED_7 && 
			ext != Extent::RESERVED_8);
	}

	RunwayStateGroup() = default;
	static inline std::optional<RunwayStateGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	Runway rw;
	Type tp = Type::RUNWAY_STATE;
	Deposits dp = Deposits::NOT_REPORTED;
	Extent ext = Extent::NOT_REPORTED;
	Precipitation dDepth;
	SurfaceFriction sf;

	RunwayStateGroup(Type t, Runway r, SurfaceFriction s = SurfaceFriction()) : 
		rw(r), tp(t), sf(s) {}

	static inline std::optional<Deposits> depositsFromString(const std::string & s);
	static inline std::optional<Extent> extentFromString(const std::string & s);
};

class SeaSurfaceGroup {
public:
	Temperature surfaceTemperature() const { return t; }
	WaveHeight waves() const { return wh; }
	bool isValid() const { return true; }

	SeaSurfaceGroup() = default;
	static inline std::optional<SeaSurfaceGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	Temperature t;
	WaveHeight wh;
};

class MinMaxTemperatureGroup {
public:
	enum class Type {
		OBSERVED_6_HOURLY,
		OBSERVED_24_HOURLY,
		FORECAST
	};
	Type type() const { return t; }
	Temperature minimum() const { return minTemp; }
	std::optional<MetafTime> minimumTime() const { return minTime; }
	Temperature maximum() const { return maxTemp; }
	std::optional<MetafTime> maximumTime() const { return maxTime; }
	inline bool isValid() const;

	MinMaxTemperatureGroup() = default;
	static inline std::optional<MinMaxTemperatureGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);
private:
	Type t = Type::FORECAST;
	Temperature minTemp;
	Temperature maxTemp;
	std::optional<MetafTime> minTime;
	std::optional<MetafTime> maxTime;
	bool isIncomplete = false;

	static inline std::optional<MinMaxTemperatureGroup> from6hourly(
		const std::string & group);
	static inline std::optional<MinMaxTemperatureGroup> from24hourly(
		const std::string & group);
	static inline std::optional<MinMaxTemperatureGroup> fromForecast(
		const std::string & group);

	inline AppendResult append6hourly(const std::string & group);
	inline AppendResult appendForecast(const std::string & group);
};

class PrecipitationGroup {
public:
	enum class Type {
		TOTAL_PRECIPITATION_HOURLY,
		SNOW_DEPTH_ON_GROUND,
		FROZEN_PRECIP_3_OR_6_HOURLY,
		FROZEN_PRECIP_3_HOURLY,
		FROZEN_PRECIP_6_HOURLY,
		FROZEN_PRECIP_24_HOURLY,
		SNOW_6_HOURLY,
		WATER_EQUIV_OF_SNOW_ON_GROUND,
		ICE_ACCRETION_FOR_LAST_HOUR,
		ICE_ACCRETION_FOR_LAST_3_HOURS,
		ICE_ACCRETION_FOR_LAST_6_HOURS,
		SNOW_INCREASING_RAPIDLY,
		PRECIPITATION_ACCUMULATION_SINCE_LAST_REPORT,
		RAINFALL_9AM_10MIN,
		PNO,
		FZRANO,
		ICG_MISG,
		PCPN_MISG,
	};
	Type type() const { return precType; }
	Precipitation total() const { return precAmount; }
	Precipitation recent() const { return precChange; }
	bool isValid() const { return !isIncomplete; }

	PrecipitationGroup() = default;
	static inline std::optional<PrecipitationGroup> parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);
private:
	Type precType = Type::TOTAL_PRECIPITATION_HOURLY;
	Precipitation precAmount;
	Precipitation precChange;
	bool isIncomplete = false;

	PrecipitationGroup(Type t, bool i = false) : precType(t), isIncomplete(i) {}
	static inline float factorFromType(Type type);
	static inline Precipitation::Unit unitFromType(Type type);
};

class LayerForecastGroup {
public:
	enum class Type {
		ICING_TRACE_OR_NONE,
		ICING_LIGHT_MIXED,
		ICING_LIGHT_RIME_IN_CLOUD,
		ICING_LIGHT_CLEAR_IN_PRECIPITATION,
		ICING_MODERATE_MIXED,
		ICING_MODERATE_RIME_IN_CLOUD,
		ICING_MODERATE_CLEAR_IN_PRECIPITATION,
		ICING_SEVERE_MIXED,
		ICING_SEVERE_RIME_IN_CLOUD,
		ICING_SEVERE_CLEAR_IN_PRECIPITATION,
		TURBULENCE_NONE,
		TURBULENCE_LIGHT,
		TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASIONAL,
		TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT,
		TURBULENCE_MODERATE_IN_CLOUD_OCCASIONAL,
		TURBULENCE_MODERATE_IN_CLOUD_FREQUENT,
		TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASIONAL,
		TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT,
		TURBULENCE_SEVERE_IN_CLOUD_OCCASIONAL,
		TURBULENCE_SEVERE_IN_CLOUD_FREQUENT,
		TURBULENCE_EXTREME,
	};
	Type type() const { return layerType; }
	Distance baseHeight() const { return layerBaseHeight; }
	Distance topHeight() const { return layerTopHeight; }
	bool isValid() const { return true; }

	LayerForecastGroup() = default;
	static inline std::optional<LayerForecastGroup> parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	Type layerType;
	Distance layerBaseHeight;
	Distance layerTopHeight;

	static inline std::optional<Type> typeFromStr(const std::string & s);
};

class PressureTendencyGroup {
public:
	enum class Type {
		NOT_REPORTED,
		INCREASING_THEN_DECREASING,
		INCREASING_MORE_SLOWLY,
		INCREASING,
		INCREASING_MORE_RAPIDLY,
		STEADY,
		DECREASING_THEN_INCREASING,
		DECREASING_MORE_SLOWLY,
		DECREASING,
		DECREASING_MORE_RAPIDLY,
		RISING_RAPIDLY,
		FALLING_RAPIDLY
	};
	enum class Trend {
		NOT_REPORTED,
		HIGHER,
		HIGHER_OR_SAME,
		SAME,
		LOWER_OR_SAME,
		LOWER
	};
	Type type() const { return tendencyType; }
	Pressure difference() const { return pressureDifference; }
	static inline Trend trend(Type type);
	bool isValid() const { return true; }

	PressureTendencyGroup() = default;
	static inline std::optional<PressureTendencyGroup> parse(
		const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	Type tendencyType;
	Pressure pressureDifference;

	static inline std::optional<Type> typeFromChar(char type);
};

class CloudTypesGroup {
public:
	inline std::vector<CloudType> cloudTypes() const;
	inline bool isValid() const;

	CloudTypesGroup() = default;
	static inline std::optional<CloudTypesGroup> parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	size_t cldTpSize = 0;
	inline static const size_t cldTpMaxSize = 8;
	CloudType cldTp[cldTpMaxSize];
};

class LowMidHighCloudGroup {
public:
	enum class LowLayer {
		NONE,
		CU_HU_CU_FR,
		CU_MED_CU_CON,
		CB_CAL,
		SC_CUGEN,
		SC_NON_CUGEN,
		ST_NEB_ST_FR,
		ST_FR_CU_FR_PANNUS,
		CU_SC_NON_CUGEN_DIFFERENT_LEVELS,
		CB_CAP,
		NOT_OBSERVABLE
	};
	enum class MidLayer {
		NONE,
		AS_TR,
		AS_OP_NS,
		AC_TR,
		AC_TR_LEN_PATCHES,
		AC_TR_AC_OP_SPREADING,
		AC_CUGEN_AC_CBGEN,
		AC_DU_AC_OP_AC_WITH_AS_OR_NS,
		AC_CAS_AC_FLO,
		AC_OF_CHAOTIC_SKY,
		NOT_OBSERVABLE
	};
	enum class HighLayer {
		NONE,
		CI_FIB_CI_UNC,
		CI_SPI_CI_CAS_CI_FLO,
		CI_SPI_CBGEN,
		CI_FIB_CI_UNC_SPREADING,
		CI_CS_LOW_ABOVE_HORIZON,
		CI_CS_HIGH_ABOVE_HORIZON,
		CS_NEB_CS_FIB_COVERING_ENTIRE_SKY,
		CS,
		CC,
		NOT_OBSERVABLE
	};
	LowLayer lowLayer() const { return cloudLowLayer; }
	MidLayer midLayer() const { return cloudMidLayer; }
	HighLayer highLayer() const { return cloudHighLayer; }
	inline bool isValid() const;

	LowMidHighCloudGroup() = default;
	static inline std::optional<LowMidHighCloudGroup> parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	LowLayer cloudLowLayer = LowLayer::NONE;
	MidLayer cloudMidLayer = MidLayer::NONE;
	HighLayer cloudHighLayer = HighLayer::NONE;

	static inline LowLayer lowLayerFromChar(char c);
	static inline MidLayer midLayerFromChar(char c);
	static inline HighLayer highLayerFromChar(char c);
};

class LightningGroup {
public:
	enum class Frequency {
		NONE,			// Not specified
		OCCASIONAL,		// Less than 1 flash/minute
		FREQUENT,		// 1 to 6 flashes/minute
		CONSTANT		// More than 6 flashes/minute
	};
	Frequency frequency() const { return freq; }
	Distance distance() const { return dist; }
	bool isCloudGround() const { return typeCloudGround; }
	bool isInCloud() const { return typeInCloud; }
	bool isCloudCloud() const { return typeCloudCloud; }
	bool isCloudAir() const { return typeCloudAir; }
	bool isUnknownType() const { return typeUnknown; }
	inline std::vector<Direction> directions() const;
	inline bool isValid() const { return !typeUnknown; }

	LightningGroup() = default;
	static inline std::optional<LightningGroup> parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	inline AppendResult append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	Frequency freq = Frequency::NONE;
	Distance dist;
	bool typeCloudGround = false;
	bool typeInCloud = false;
	bool typeCloudCloud = false;
	bool typeCloudAir = false;
	bool typeUnknown = false;
	std::optional<Direction> dir1from;
	std::optional<Direction> dir1to;
	std::optional<Direction> dir2from;
	std::optional<Direction> dir2to;
	bool incomplete = false;

	LightningGroup(Frequency frequency) {
		freq = frequency;
		incomplete = true;
	}
	static inline std::optional<LightningGroup> fromLtgGroup(
		const std::string & group);
	bool isOmittedDir1() const { return (!dir1from.has_value() && !dir1to.has_value()); } 
	bool isOmittedDir2() const { return (!dir2from.has_value() && !dir2to.has_value()); }
};

class VicinityGroup {
public:
	VicinityGroup() = default;
	enum class Type {
		THUNDERSTORM,
		CUMULONIMBUS,
		CUMULONIMBUS_MAMMATUS,
		TOWERING_CUMULUS,
		ALTOCUMULUS_CASTELLANUS,
		STRATOCUMULUS_STANDING_LENTICULAR,
		ALTOCUMULUS_STANDING_LENTICULAR,
		CIRROCUMULUS_STANDING_LENTICULAR,
		ROTOR_CLOUD,
		VIRGA,
		PRECIPITATION_IN_VICINITY,
		FOG,
		FOG_SHALLOW,
		FOG_PATCHES,
		HAZE,
		SMOKE,
		BLOWING_SNOW,
		BLOWING_SAND,
		BLOWING_DUST
	};
	Type type() const { return t; }
	Distance distance() const { return dist; }
	inline std::vector<Direction> directions() const;
	inline Direction movingDirection() const { return movDir; }
	inline bool isValid() const {
		return (incompleteType == IncompleteType::NONE);
	}

	static inline std::optional<VicinityGroup> parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	inline AppendResult append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	Type t;
	Distance dist;
	std::optional<Direction> dir1from;
	std::optional<Direction> dir1to;
	std::optional<Direction> dir2from;
	std::optional<Direction> dir2to;
	Direction movDir;

	enum class IncompleteType {
		NONE,				// Group complete
		EXPECT_CLD, 		// ROTOR previously specified, now expecting CLD
		EXPECT_DIST_DIR1,	// Expect distance or first direction sector
		EXPECT_DIR1,		// Expect first direction sector
		EXPECT_DIR2_MOV,	// Expect second direction sector
		EXPECT_DIR2,		// Expect second direction sector
		EXPECT_MOV,			// Expect MOV
		EXPECT_MOVDIR		// Expect cardinal direction of movement
	};
	IncompleteType incompleteType = IncompleteType::NONE;
	AppendResult expectNext(IncompleteType newType) {
		incompleteType = newType; return AppendResult::APPENDED;
	}
	AppendResult finalise() {
		incompleteType = IncompleteType::NONE; 
		return AppendResult::APPENDED;
	}
	AppendResult rejectGroup() {
		incompleteType = IncompleteType::NONE; 
		return AppendResult::NOT_APPENDED;
	}

	VicinityGroup(Type tp) : t(tp) {
		incompleteType = IncompleteType::EXPECT_DIST_DIR1;
		if (type() == Type::ROTOR_CLOUD) incompleteType = IncompleteType::EXPECT_CLD;
	}

	inline bool appendDir1(const std::string & str);
	inline bool appendDir2(const std::string & str);
	inline bool appendDistance(const std::string & str);
};

class MiscGroup {
public:
	enum class Type {
		SUNSHINE_DURATION_MINUTES,
		CORRECTED_WEATHER_OBSERVATION,
		DENSITY_ALTITUDE,
		HAILSTONE_SIZE,
		COLOUR_CODE_BLUE,
		COLOUR_CODE_WHITE,
		COLOUR_CODE_GREEN,
		COLOUR_CODE_YELLOW1,
		COLOUR_CODE_YELLOW2,
		COLOUR_CODE_AMBER,
		COLOUR_CODE_RED,
		COLOUR_CODE_BLACKBLUE,
		COLOUR_CODE_BLACKWHITE,
		COLOUR_CODE_BLACKGREEN,
		COLOUR_CODE_BLACKYELLOW1,
		COLOUR_CODE_BLACKYELLOW2,
		COLOUR_CODE_BLACKAMBER,
		COLOUR_CODE_BLACKRED,
		FROIN
	};
	Type type() const { return groupType; }
	std::optional<float> data() const { return groupData; }
	inline bool isValid() const;

	MiscGroup() = default;
	static inline std::optional<MiscGroup> parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata);
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata);

private:
	enum class IncompleteText {
		NONE,
		DENSITY,
		DENSITY_ALT,
		GR,
		GR_INT
	};

	Type groupType;
	std::optional<float> groupData;
	IncompleteText incompleteText = IncompleteText::NONE;

	inline static std::optional<Type> parseColourCode(const std::string & group);
	inline bool appendHailstoneFraction (const std::string & group);
	inline bool appendDensityAltitude (const std::string & group);
};

class UnknownGroup {
public:
	UnknownGroup() = default;
	static std::optional<UnknownGroup> parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata = missingMetadata)
	{
		(void)group; (void)reportPart; (void)reportMetadata;
		return UnknownGroup();
	}
	AppendResult inline append(const std::string & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const ReportMetadata & reportMetadata = missingMetadata)
	{
		(void)group; (void)reportPart; (void)reportMetadata;
		return AppendResult::APPENDED;
	}
	bool isValid() const { return true; }
};

///////////////////////////////////////////////////////////////////////////////

struct GroupInfo {
	GroupInfo(Group g, ReportPart rp, std::string rawstr) :
		group(std::move(g)), reportPart(rp), rawString(std::move(rawstr)) {}
	Group group;
	ReportPart reportPart;
	std::string rawString;
};

///////////////////////////////////////////////////////////////////////////////

// Syntax Group is a delimiter of structural part of METAR/TAF report
enum class SyntaxGroup {
	OTHER,			// Group is not important for report syntax
	METAR,			// Keyword METAR (beginning of METAR report)
	SPECI,			// Keyword SPECI (beginning of METAR report)
	TAF,			// Keyword TAF (beginning of TAF report)
	COR,			// Keyword COR (correctional report)
	AMD,			// Keyword AMD (amended report)
	LOCATION,		// ICAO location, 4 alphanumeric chars, 1st char must be alpha
	REPORT_TIME,	// Report release time, 6 digits and letter Z (xxxxxxZ)
	TIME_SPAN,		// TAF report validity time (xxxx/xxxx)
	CNL,			// Keyword CNL (cancelled report)
	NIL,			// Keyword NIL (missing report)
	RMK,			// Keyword RMK (remarks to follow)
	MAINTENANCE_INDICATOR //Keyword $ added at the end of automated METARs
};

// Determines groups important for report syntax
inline SyntaxGroup getSyntaxGroup(const Group & group);

///////////////////////////////////////////////////////////////////////////////

class GroupParser {
public:
	static Group parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata)
	{
		return parseAlternative<0>(group, reportPart, reportMetadata);
	}

	static Group reparse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata,
		const Group & previous)
	{
		return reparseAlternative<0>(group, reportPart, reportMetadata, previous.index());
	}
private:
	template <size_t I>
	static Group parseAlternative(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata)
	{
		using Alternative = std::variant_alternative_t<I, Group>;
		if constexpr (!std::is_same<Alternative, FallbackGroup>::value) {
			const auto parsed = Alternative::parse(group, reportPart, reportMetadata);
			if (parsed.has_value()) return *parsed;
		}
		if constexpr (I >= (std::variant_size_v<Group> - 1)) {
			return FallbackGroup();
		} else {
			return parseAlternative<I+1>(group, reportPart, reportMetadata);
		}
	}

	template <size_t I>
	static Group reparseAlternative(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata,
		size_t ignoreIndex)
	{
		using Alternative = std::variant_alternative_t<I, Group>;
		if constexpr (!std::is_same<Alternative, FallbackGroup>::value) {
			if (I != ignoreIndex) {
				const auto parsed = Alternative::parse(group, reportPart, reportMetadata);
				if (parsed.has_value()) return *parsed;
			}
		}
		if constexpr (I >= (std::variant_size_v<Group> - 1)) {
			return FallbackGroup();
		} else {
			return reparseAlternative<I+1>(group, reportPart, reportMetadata, ignoreIndex);
		}
	}
};

struct ParseResult {
	ReportMetadata reportMetadata;
	std::vector<GroupInfo> groups;
};

class Parser {
public:
	static inline ParseResult parse (const std::string & report, size_t groupLimit = 100); 

private:
	static inline bool appendToLastResultGroup(ParseResult & result,
		const std::string & groupStr,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata,
		bool allowReparse = true);
	static inline void addGroupToResult(ParseResult & result,
		Group group,
		ReportPart reportPart,
		std::string groupString);
	static inline void updateMetadata(const Group & group,
		ReportMetadata & reportMetadata);


	class ReportInput {
	public:
		ReportInput(const std::string & s) : report(s) {}
		friend ReportInput & operator >> (ReportInput & input, std::string & output) {
			output = input.getNextGroup();
			return input;
		}
	private:
		inline std::string getNextGroup();
		const std::string & report;
		bool finished = false;
		size_t pos = 0;
	};


	class Status {
	public:
		Status() :
			state(State::REPORT_TYPE_OR_LOCATION),
			reportType(ReportType::UNKNOWN),
			reportError(ReportError::NONE) {}
		ReportType getReportType() { return reportType; }
		ReportError getError() { return reportError; }
		bool isError() { return (reportError != ReportError::NONE); }
		inline ReportPart getReportPart();
		inline void transition(SyntaxGroup group);
		inline void finalTransition();
		bool isReparseRequired() {
			return (state == State::REPORT_BODY_BEGIN_METAR_REPEAT_PARSE);
		}
		void setError(ReportError e) { state = State::ERROR; reportError = e; }
	private:
		enum class State {
			// States of state machine used to check syntax of METAR/TAF reports
			REPORT_TYPE_OR_LOCATION,
			CORRECTION,
			LOCATION,
			REPORT_TIME,
			TIME_SPAN,
			REPORT_BODY_BEGIN_METAR,
			REPORT_BODY_BEGIN_METAR_REPEAT_PARSE,
			REPORT_BODY_METAR,
			REPORT_BODY_BEGIN_TAF,
			REPORT_BODY_TAF,
			REMARK_METAR,
			REMARK_TAF,
			NIL,
			CNL,
			ERROR
		};

		void setState(State s) { state = s; }
		void setReportType(ReportType rt) { reportType = rt; }

		inline void transitionFromReportTypeOrLocation(SyntaxGroup group);
		inline void transitionFromCorrecton(SyntaxGroup group);
		inline void transitionFromReportTime(SyntaxGroup group);
		inline void transitionFromTimeSpan(SyntaxGroup group);
		inline void transitionFromReportBodyBeginMetar(SyntaxGroup group);
		inline void transitionFromReportBodyMetar(SyntaxGroup group);
		inline void transitionFromReportBodyBeginTaf(SyntaxGroup group);
		inline void transitionFromReportBodyTaf(SyntaxGroup group);
		State state;
		ReportType reportType;
		ReportError reportError;
	};
};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
class Visitor {
public:
	inline T visit(const Group & group,
		ReportPart reportPart = ReportPart::UNKNOWN,
		const std::string & rawString = std::string());
	inline T visit(const GroupInfo & groupInfo) {
		return visit(groupInfo.group, groupInfo.reportPart, groupInfo.rawString);
	}
protected:
	virtual T visitKeywordGroup(
		const KeywordGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitLocationGroup(const LocationGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitReportTimeGroup(const ReportTimeGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitTrendGroup(const TrendGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitWindGroup(const WindGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitVisibilityGroup(const VisibilityGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitCloudGroup(const CloudGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitWeatherGroup(const WeatherGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitTemperatureGroup(const TemperatureGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitPressureGroup(const PressureGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitRunwayStateGroup(const RunwayStateGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitSeaSurfaceGroup(const SeaSurfaceGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitMinMaxTemperatureGroup(const MinMaxTemperatureGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitPrecipitationGroup(const PrecipitationGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitLayerForecastGroup(const LayerForecastGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitPressureTendencyGroup(const PressureTendencyGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitCloudTypesGroup(const CloudTypesGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitLowMidHighCloudGroup(const LowMidHighCloudGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitLightningGroup(const LightningGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitVicinityGroup(const VicinityGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitMiscGroup(const MiscGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
	virtual T visitUnknownGroup(const UnknownGroup & group,
		ReportPart reportPart,
		const std::string & rawString) = 0;
};

template <typename T>
inline T Visitor<T>::visit(const Group & group,
	ReportPart reportPart,
	const std::string & rawString)
{
	if (const auto gr = std::get_if<KeywordGroup>(&group); gr) {
		return this->visitKeywordGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<LocationGroup>(&group); gr) {
		return this->visitLocationGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<ReportTimeGroup>(&group); gr) {
		return this->visitReportTimeGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<TrendGroup>(&group); gr) {
		return this->visitTrendGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<WindGroup>(&group); gr) {
		return this->visitWindGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<VisibilityGroup>(&group); gr) {
		return this->visitVisibilityGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<CloudGroup>(&group); gr) {
		return this->visitCloudGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<WeatherGroup>(&group); gr) {
		return this->visitWeatherGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<TemperatureGroup>(&group); gr) {
		return this->visitTemperatureGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<PressureGroup>(&group); gr) {
		return this->visitPressureGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<RunwayStateGroup>(&group); gr) {
		return this->visitRunwayStateGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<SeaSurfaceGroup>(&group); gr) {
		return this->visitSeaSurfaceGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<MinMaxTemperatureGroup>(&group); gr) {
		return this->visitMinMaxTemperatureGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<PrecipitationGroup>(&group); gr) {
		return this->visitPrecipitationGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<LayerForecastGroup>(&group); gr) {
		return this->visitLayerForecastGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<PressureTendencyGroup>(&group); gr) {
		return this->visitPressureTendencyGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<CloudTypesGroup>(&group); gr) {
		return this->visitCloudTypesGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<LowMidHighCloudGroup>(&group); gr) {
		return this->visitLowMidHighCloudGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<LightningGroup>(&group); gr) {
		return this->visitLightningGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<VicinityGroup>(&group); gr) {
		return this->visitVicinityGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<MiscGroup>(&group); gr) {
		return this->visitMiscGroup(*gr, reportPart, rawString);
	}
	if (const auto gr = std::get_if<UnknownGroup>(&group); gr) {
		return this->visitUnknownGroup(*gr, reportPart, rawString);
	}
	return T();
}

template<>
inline void Visitor<void>::visit(const Group & group,
	ReportPart reportPart,
	const std::string & rawString)
{
	if (const auto gr = std::get_if<KeywordGroup>(&group); gr) {
		this->visitKeywordGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<LocationGroup>(&group); gr) {
		this->visitLocationGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<ReportTimeGroup>(&group); gr) {
		this->visitReportTimeGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<TrendGroup>(&group); gr) {
		this->visitTrendGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<WindGroup>(&group); gr) {
		this->visitWindGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<VisibilityGroup>(&group); gr) {
		this->visitVisibilityGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<CloudGroup>(&group); gr) {
		this->visitCloudGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<WeatherGroup>(&group); gr) {
		this->visitWeatherGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<TemperatureGroup>(&group); gr) {
		this->visitTemperatureGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<PressureGroup>(&group); gr) {
		this->visitPressureGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<RunwayStateGroup>(&group); gr) {
		this->visitRunwayStateGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<SeaSurfaceGroup>(&group); gr) {
		this->visitSeaSurfaceGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<MinMaxTemperatureGroup>(&group); gr) {
		this->visitMinMaxTemperatureGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<PrecipitationGroup>(&group); gr) {
		this->visitPrecipitationGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<LayerForecastGroup>(&group); gr) {
		this->visitLayerForecastGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<PressureTendencyGroup>(&group); gr) {
		this->visitPressureTendencyGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<CloudTypesGroup>(&group); gr) {
		this->visitCloudTypesGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<LowMidHighCloudGroup>(&group); gr) {
		this->visitLowMidHighCloudGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<LightningGroup>(&group); gr) {
		this->visitLightningGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<VicinityGroup>(&group); gr) {
		this->visitVicinityGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<MiscGroup>(&group); gr) {
		this->visitMiscGroup(*gr, reportPart, rawString);
		return;
	}
	if (const auto gr = std::get_if<UnknownGroup>(&group); gr) {
		this->visitUnknownGroup(*gr, reportPart, rawString);
		return;
	}
}

template<>
inline void Visitor<void>::visit(const GroupInfo & groupInfo) {
	visit(groupInfo.group, groupInfo.reportPart, groupInfo.rawString);
}

///////////////////////////////////////////////////////////////////////////////

inline std::optional<unsigned int> strToUint(const std::string & str,
	std::size_t startPos,
	std::size_t digits);

inline std::optional<std::pair<unsigned int, unsigned int> > fractionStrToUint(
	const std::string & str,
	std::size_t startPos,
	std::size_t length);

} //namespace metaf

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

namespace metaf {

std::optional<unsigned int> strToUint(const std::string & str,
	std::size_t startPos,
	std::size_t digits)
{
	std::optional<unsigned int> error;
	if (str.empty() || !digits || startPos + digits > str.length()) return error;
	unsigned int value = 0;
	for (auto [i,c] = std::pair(0u, str.c_str() + startPos); i < digits; i++, c++) {
		if (*c < '0' || *c > '9') return error;
		static const auto decimalRadix = 10u;
		value = value * decimalRadix + (*c - '0');
	}
	return value;
}

std::optional<std::pair<unsigned int, unsigned int> >
	fractionStrToUint(const std::string & str,
		std::size_t startPos,
		std::size_t length)
{
	//Equivalent regex "(\\d\\d?)/(\\d\\d?)"
	std::optional<std::pair<unsigned int, unsigned int> > error;
	if (length + startPos > str.length()) length = str.length() - startPos;
	const int endPos = startPos + length;

	const auto slashPos = str.find('/', startPos);
	if (slashPos == std::string::npos) return error;

	static const int minDigits = 1, maxDigits = 2;

	const int numeratorPos = startPos;
	const int numeratorLength = slashPos - startPos;
	if (numeratorLength < minDigits || numeratorLength > maxDigits) return error;
	const int denominatorPos = slashPos + 1;
	const int denominatorLength = endPos - denominatorPos;
	if (denominatorLength < minDigits || denominatorLength > maxDigits) return error;

	const auto numerator = strToUint(str, numeratorPos, numeratorLength);
	const auto denominator = strToUint(str, denominatorPos, denominatorLength);
	if (!numerator.has_value() || !denominator.has_value()) return error;
	return std::pair(*numerator, *denominator);
}

std::optional<Runway> Runway::fromString(const std::string & s, bool enableRwy) {
	//static const std::regex rgx("R(?:WY)?(\\d\\d)([RLC])?");
	static const std::optional<Runway> error;
	if (s.length() < 3) return error;
	if (s[0] != 'R') return error;
	auto numPos = 1u;
	if (enableRwy && s[1] == 'W' && s[2] == 'Y') numPos += 2;
	const auto rwyNum = strToUint(s, numPos, 2);
	if (!rwyNum.has_value()) return error;
	const auto dsgPos = numPos + 2;
	if (s.length() > dsgPos + 1) return error;
	Runway runway;
	runway.rNumber = *rwyNum;
	if ( s.length() > dsgPos) {
		const auto designator = designatorFromChar(s[dsgPos]);
		if (!designator.has_value()) return error;
		runway.rDesignator = *designator;
	}
	return runway;
}

std::optional<Runway::Designator> Runway::designatorFromChar(char c) {
	switch (c) {
		case 'R': return Designator::RIGHT;
		case 'C': return Designator::CENTER;
		case 'L': return Designator::LEFT;
		default: return std::optional<Designator>();
	}
}

///////////////////////////////////////////////////////////////////////////////

bool MetafTime::is3hourlyReportTime() const {
	switch(hour()) {
		case 2: case 3: case 8: case 9: case 14: case 15: case 20: case 21: return true;
		default: return false;
	}
}

bool MetafTime::is6hourlyReportTime() const {
	switch(hour()) {
		case 0: case 5: case 6: case 11: case 12: case 17: case 18: case 23: return true;
		default: return false;
	}
}

MetafTime::Date MetafTime::dateBeforeRef(const Date & refDate) const {
	if (!day().has_value()) return refDate;
	Date result = refDate;
	result.day = *day();
	if (result.day > refDate.day) { //previous month
		static const auto firstMonth = 1;
		static const auto lastMonth = 12;
		if (result.month == firstMonth) {
			result.year--;
			result.month = lastMonth;
		} else {
			result.month--;
		}
	}
	return result;
}

std::optional<MetafTime> MetafTime::fromStringDDHHMM(const std::string & s) {
	//static const std::regex rgx ("(\\d\\d)?(\\d\\d)(\\d\\d)");
	static const std::optional<MetafTime> error;
	if (s.length() == 4) {
		const auto hour = strToUint(s, 0, 2);
		const auto minute = strToUint(s, 2, 2);
		if (!hour.has_value() || !minute.has_value()) return error;
		MetafTime metafTime;
		metafTime.hourValue = *hour;
		metafTime.minuteValue = *minute;
		return metafTime;
	}
	if (s.length() == 6) {
		const auto day = strToUint(s, 0, 2);
		const auto hour = strToUint(s, 2, 2);
		const auto minute = strToUint(s, 4, 2);
		if (!day.has_value() || !hour.has_value() || !minute.has_value()) return error;
		MetafTime metafTime;
		metafTime.dayValue = day;
		metafTime.hourValue = *hour;
		metafTime.minuteValue = *minute;
		return metafTime;
	}
	return error;
}

std::optional<MetafTime> MetafTime::fromStringDDHH(const std::string & s) {
	//static const std::regex rgx ("(\\d\\d)(\\d\\d)");
	static const std::optional<MetafTime> error;
	if (s.length() != 4) return error;
	const auto day = strToUint(s, 0, 2);
	const auto hour = strToUint(s, 2, 2);
	if (!day.has_value() || !hour.has_value()) return error;
	MetafTime metafTime;
	metafTime.dayValue = day;
	metafTime.hourValue = *hour;
	return metafTime;
}

bool MetafTime::isValid() const {
	if (auto d = dayValue.value_or(maxDay); d > maxDay || !d) return false;
	if (hourValue > maxHour) return false;
	if (minuteValue > maxMinute) return false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////

Temperature::Temperature(float value) :
	tempValue(std::round(value * 10.0)), freezing(value < 0), precise(true)  {}

std::optional<float> Temperature::relativeHumidity(
	const Temperature & airTemperature,
	const Temperature & dewPoint)
{
	const auto temperatureC = airTemperature.toUnit(Temperature::Unit::C);
	const auto dewPointC = dewPoint.toUnit(Temperature::Unit::C);
	if (!temperatureC.has_value() || !dewPointC.has_value()) {
		return std::optional<float>();
	}
	if (*temperatureC < *dewPointC) return 100.0;
	const auto saturationVapourPressure =
		6.11 * pow(10, 7.5 * *temperatureC / (237.7 + *temperatureC));
	const auto actualVapourPressure =
		6.11 * pow(10, 7.5 * *dewPointC / (237.7 + *dewPointC));
	return (100.0 * actualVapourPressure / saturationVapourPressure);
}

Temperature Temperature::heatIndex(
	const Temperature & airTemperature,
	float relativeHumidity)
{
	// Using formula from https://en.wikipedia.org/wiki/Heat_index
	// (see formula for degrees Celsius)
	// The formula is valid for temperature > 27 C and RH > 40%
	const auto temperatureC = airTemperature.toUnit(Temperature::Unit::C);
	if (!temperatureC.has_value() || temperatureC <27.0) return Temperature();

	if (relativeHumidity > 100.0 || relativeHumidity < 40.0) return Temperature();

	const auto c1 = -8.78469475556;
	const auto c2 = 1.61139411;
	const auto c3 = 2.33854883889;
    const auto c4 = -0.14611605;
    const auto c5 = -0.012308094;
    const auto c6 = -0.0164248277778;
    const auto c7 = 0.002211732;
    const auto c8 = 0.00072546;
    const auto c9 = -0.000003582;
    const auto t = *temperatureC, r = relativeHumidity;

    const auto heatIndexC =
    	c1 + c2 * t + c3 * r + c4 * t * r +
    	c5 * t * t + c6 * r * r +
    	c7 * t * t * r + c8 * t * r * r + c9 * t * t * r * r;

    return Temperature(heatIndexC);
}

Temperature Temperature::heatIndex(
	const Temperature & airTemperature,
	const Temperature & dewPoint)
{
	const auto rh = relativeHumidity(airTemperature, dewPoint);
	if (!rh.has_value()) return Temperature();
	return heatIndex(airTemperature, *rh);
}

Temperature Temperature::windChill(
	const Temperature & airTemperature,
	const Speed & windSpeed)
{
	const auto temperatureC = airTemperature.toUnit(Temperature::Unit::C);
	if (!temperatureC.has_value() || *temperatureC > 10.0) return Temperature();

	const auto windKmh = windSpeed.toUnit(Speed::Unit::KILOMETERS_PER_HOUR);
	if (!windKmh.has_value() || *windKmh < 4.8) return Temperature();

	const auto windChillC =
		13.12 +
		0.6215 * *temperatureC -
		11.37 * pow(*windKmh, 0.16) +
		0.3965 * *temperatureC * pow(*windKmh, 0.16);

	return Temperature(windChillC);
}

std::optional<Temperature> Temperature::fromString(const std::string & s) {
	//static const std::regex rgx ("(?:(M)?(\\d\\d))|//");
	std::optional<Temperature> error;
	if (s == "//") return Temperature();
	if (s.length() == 3) {
		if (s[0] != 'M') return error;
		const auto t = strToUint(s, 1, 2);
		if (!t.has_value()) return error;
		Temperature temperature;
		temperature.tempValue = - *t;
		temperature.freezing = true;
		return temperature;
	}
	if (s.length() == 2) {
		const auto t = strToUint(s, 0, 2);
		if (!t.has_value()) return error;
		Temperature temperature;
		temperature.tempValue = *t;
		return temperature;
	}
	return error;
}

std::optional<Temperature> Temperature::fromRemarkString(const std::string & s) {
	//static const std::regex ("([01])(\\d\\d\\d)");
	std::optional<Temperature> error;
	if (s.length() != 4) return error;
	if (s[0] != '0' && s[0] != '1') return error;
	const auto t = strToUint(s, 1, 3);
	if (!t.has_value()) return error;
	int tValueSigned = *t;
	if (s[0] == '1') tValueSigned = -tValueSigned;
	return Temperature(tValueSigned / 10.0);
}

std::optional<float> Temperature::toUnit(Unit unit) const {
	std::optional<float> error;
	auto v = temperature();
	if (!v.has_value() || tempUnit != Unit::C) return error;
	switch (unit) {
		case Unit::C: return *v;
		case Unit::F: return (*v * 1.8 + 32);
	}
}

///////////////////////////////////////////////////////////////////////////////

std::optional<Speed> Speed::fromString(const std::string & s, Unit unit) {
	//static const std::regex rgx ("([1-9]?\\d\\d)|//");
	static const std::optional<Speed> error;
	if (s.empty() || s == "//") return Speed();
	if (s.length() != 2 && s.length() != 3) return error;
	if (s.length() == 3 && s[0] == '0') return error;
	const auto spd = strToUint(s, 0, s.length());
	if (!spd.has_value()) return error;
	Speed speed;
	speed.speedUnit = unit;
	speed.speedValue = *spd;
	return speed;
}

std::optional<float> Speed::toUnit(Unit unit) const {
	if (!speedValue.has_value()) return std::optional<float>();
	switch (speedUnit) {
		case Unit::KNOTS:				return knotsToUnit(*speedValue, unit);
		case Unit::METERS_PER_SECOND:	return mpsToUnit(*speedValue, unit);
		case Unit::KILOMETERS_PER_HOUR:	return kmhToUnit(*speedValue, unit);
		case Unit::MILES_PER_HOUR:		return mphToUnit(*speedValue, unit);
		default:						return std::optional<float>();
	}
}

std::optional<Speed::Unit> Speed::unitFromString(const std::string & s) {
	if (s == "KT") return Speed::Unit::KNOTS;
	if (s == "MPS") return Speed::Unit::METERS_PER_SECOND;
	if (s == "KMH") return Speed::Unit::KILOMETERS_PER_HOUR;
	return std::optional<Speed::Unit>();
}

std::optional<float> Speed::knotsToUnit(float valueKnots, Unit otherUnit) {
	switch(otherUnit) {
		case Unit::KNOTS: 				return valueKnots;
		case Unit::METERS_PER_SECOND: 	return (valueKnots * 0.514444);
		case Unit::KILOMETERS_PER_HOUR:	return (valueKnots * 1.852);
		case Unit::MILES_PER_HOUR:		return (valueKnots * 1.150779);
		default:						return std::optional<float>();
	}
}

std::optional<float> Speed::mpsToUnit(float valueMps, Unit otherUnit) {
	switch(otherUnit) {
		case Unit::KNOTS: 				return (valueMps * 1.943844);
		case Unit::METERS_PER_SECOND: 	return valueMps;
		case Unit::KILOMETERS_PER_HOUR:	return (valueMps * 3.6);
		case Unit::MILES_PER_HOUR:		return (valueMps * 2.236936);
		default:						return std::optional<float>();
	}
}

std::optional<float> Speed::kmhToUnit(float valueKmh, Unit otherUnit) {
	switch(otherUnit) {
		case Unit::KNOTS: 				return (valueKmh / 1.852);
		case Unit::METERS_PER_SECOND:	return (valueKmh / 3.6);
		case Unit::KILOMETERS_PER_HOUR:	return valueKmh;
		case Unit::MILES_PER_HOUR:		return (valueKmh * 0.621371);
		default:						return std::optional<float>();
	}
}

std::optional<float> Speed::mphToUnit(float valueMph, Unit otherUnit) {
	switch(otherUnit) {
		case Unit::KNOTS: 				return (valueMph * 0.868976);
		case Unit::METERS_PER_SECOND: 	return (valueMph * 0.44704);
		case Unit::KILOMETERS_PER_HOUR:	return (valueMph * 1.609344);
		case Unit::MILES_PER_HOUR:		return valueMph;
		default:						return std::optional<float>();
	}
}

////////////////////////////////////////////////////////////////////////////////

std::optional<Distance> Distance::fromMeterString(const std::string & s) {
	//static const std::regex rgx ("(\\d\\d\\d\\d)|////");
	static const std::optional<Distance> error;
	if (s.length() != 4) return error;
	Distance distance;
	distance.distUnit = Unit::METERS;
	if (s == "////") return distance;

	const auto dist = strToUint(s, 0, 4);
	if (!dist.has_value()) return error;
	distance.dist = dist;

	if (const auto valueMoreThan10km = 9999u; distance.dist == valueMoreThan10km) {
		distance.distModifier = Modifier::MORE_THAN;
		distance.dist = 10000u;
	}
	return distance;
}

std::optional<Distance> Distance::fromMileString(const std::string & s, 
	bool remarkFormat)
{
	// static const std::regex rgx ("([PM])?(\\d?\\d)(?:/(\\d?\\d))?SM|////SM");
	// static const std::regex rgx ("([PM])?(\\d?\\d)(?:/(\\d?\\d))?");
	static const std::optional<Distance> error;
	static const auto unitStr = std::string ("SM");
	static const auto unitLength = unitStr.length();

	Distance distance;
	distance.distUnit = Unit::STATUTE_MILES;
	if (!remarkFormat) {
		if (s.length() < unitLength + 1) return error; //1 digit minimum
		if (s == "////SM") return distance;
		if (s.substr(s.length() - unitLength, unitLength) != unitStr) return error; //s.endsWith(unitStr)
	} else {
		if (s.length() < 1) return error; //1 digit minimum
	}
	const auto modifier = modifierFromChar(s[0]);
	if (modifier.has_value()) {
		distance.distModifier = *modifier;
	}

	if (s.find('/') == std::string::npos) {
		//The value is integer, e.g. 3SM or 15SM
		const int intPos = static_cast<int>(modifier.has_value());
		const int intLength = s.length() - 
			(remarkFormat ? 0 : unitLength) - 
			static_cast<int>(modifier.has_value());
		static const int minDigits = 1, maxDigits = 2;
		if (intLength < minDigits || intLength > maxDigits) return error;
		const auto dist = strToUint(s, intPos, intLength);
		if (!dist.has_value()) return error;
		distance.dist = *dist * statuteMileFactor;
	} else {
		//Fraction value, e.g. 1/2SM, 11/2SM, 5/16SM, 11/16SM
		const int fracLength = s.length() - 
			(remarkFormat ? 0: unitLength) - 
			static_cast<int>(modifier.has_value());
		const auto fraction = fractionStrToUint(s,
			static_cast<int>(modifier.has_value()),
			fracLength);
		if (!fraction.has_value()) return error;
		auto integer = 0u;
		auto numerator = std::get<0>(*fraction);
		auto denominator = std::get<1>(*fraction);
		if (!numerator || !denominator) return error;
		if (numerator >= denominator) { //e.g. 11/2SM = 1 1/2SM
			static const auto decimalRadix = 10u;
			integer = numerator / decimalRadix;
			numerator = numerator % decimalRadix;
		}
		if (!numerator) return error;
		distance.dist = 
			integer * statuteMileFactor + 
			numerator * statuteMileFactor / denominator;
	}
	return distance;
}

std::optional<Distance> Distance::fromHeightString(const std::string & s) {
	//static const std::regex rgx ("(\\d\\d\\d)|///");
	static const std::optional<Distance> error;
	if (s.length() != 3) return error;
	Distance distance;
	distance.distUnit = Unit::FEET;
	if (s == "///") return distance;
	const auto h = strToUint(s, 0, 3);
	if (!h.has_value())	return error;
	distance.dist = *h * heightFactor;
	return distance;
}

std::optional<Distance> Distance::fromRvrString(const std::string & s, bool unitFeet) {
	//static const std::regex rgx ("([PM])?(\\d\\d\\d\\d)|////");
	static const std::optional<Distance> error;
	Distance distance;
	distance.distUnit = unitFeet ? Unit::FEET : Unit::METERS;
	if (s.length() == 4) {
		if (s == "////") return distance;
		const auto dist = strToUint(s, 0, 4);
		if (!dist.has_value()) return error;
		distance.dist = *dist;
		return distance;
	}
	if (s.length() == 5) {
		auto modifier = modifierFromChar(s[0]);
		if (!modifier.has_value()) return error;
		distance.distModifier = *modifier;
		const auto dist = strToUint(s, 1, 4);
		if (!dist.has_value()) return error;
		distance.dist = *dist;
		return distance;
	}
	return error;
}

std::optional<std::pair<Distance, Distance>> Distance::fromLayerString(
	const std::string & s)
{
	//static const std::regex rgx ("(\\d\\d\\d)(\\d)");
	static const std::optional<std::pair<Distance, Distance>> error;
	if (s.length() != 4) return error;
	const auto h = strToUint(s, 0, 3);
	if (!h.has_value())	return error;
	const auto d = strToUint(s, 3, 1);
	if (!d.has_value())	return error;
	Distance baseHeight, topHeight;
	baseHeight.distUnit = Unit::FEET;
	topHeight.distUnit = Unit::FEET;
	baseHeight.dist = *h * heightFactor;
	topHeight.dist = *h * heightFactor + *d * layerDepthFactor;
	return std::pair(baseHeight, topHeight);
}

std::optional<Distance> Distance::fromKmString(const std::string & s) {
	//static const std::regex rgx ("(\\d\\d?)KM");
	static const std::optional<Distance> error;
	static const auto metersPerKm = 1000u;

	if (s.length() != 3 && s.length() != 4) return error;

	static const char kmStr[] = "KM"; 
	static const size_t kmStrLen = std::strlen(kmStr);
	const auto numLen = s.length() - kmStrLen;
	if (s.substr(numLen) != kmStr) return error;

	const auto dist = strToUint(s, 0, numLen);
	if (!dist.has_value()) return error;

	Distance distance;
	distance.distUnit = Unit::METERS;
	distance.dist = *dist * metersPerKm;
	return distance;
}

std::optional<Distance> Distance::fromIntegerAndFraction(const Distance & integer,
	const Distance & fraction)
{
	static const std::optional<Distance> error; 
	if (integer.modifier() != Modifier::NONE &&
		integer.modifier() != Modifier::LESS_THAN &&
		integer.modifier() != Modifier::MORE_THAN) return error;
	if (!integer.isValid() ||
		!fraction.isValid() ||
		fraction.modifier() != Modifier::NONE ||
		integer.unit() != Unit::STATUTE_MILES || 
		fraction.unit() != Unit::STATUTE_MILES ||
		!integer.dist.has_value() ||
		!fraction.dist.has_value() ||
		(*integer.dist % statuteMileFactor) ||
		!(*fraction.dist % statuteMileFactor)) return error; 
	Distance result;
	result.distModifier = integer.modifier();
	result.dist = *integer.dist + *fraction.dist;
	result.distUnit = Unit::STATUTE_MILES;
	return result;
}

std::optional<float> Distance::toUnit(Unit unit) const {
	const auto d = distance();
	if (!d.has_value()) return std::optional<float>();
	switch (distUnit) {
		case Unit::METERS: 			return metersToUnit(*d, unit);
		case Unit::STATUTE_MILES:	return milesToUnit(*d, unit);
		case Unit::FEET:			return feetToUnit(*d, unit);
	}
}

std::optional<float> Distance::distance() const {
	if (!dist.has_value()) return std::optional<float>();
	if (distUnit == Unit::STATUTE_MILES) 
		return (static_cast<float>(*dist) / statuteMileFactor);
	return *dist;
}

std::optional<std::pair<unsigned int, Distance::MilesFraction>> Distance::miles() const
{
	const auto milesDecimal = toUnit(Unit::STATUTE_MILES);
	if (!milesDecimal.has_value()) return std::optional<std::pair<unsigned int, Distance::MilesFraction>>();
	static const unsigned int denominator = 16u;
	const unsigned int topHeavyNumerator = std::round(*milesDecimal * denominator);
	switch (topHeavyNumerator) { 
		case 0:  return std::pair(0u, MilesFraction::NONE);
		case 1:  return std::pair(0u, MilesFraction::F_1_16);
		case 2:  return std::pair(0u, MilesFraction::F_1_8);
		case 3:  return std::pair(0u, MilesFraction::F_3_16);
		case 4:  return std::pair(0u, MilesFraction::F_1_4);
		case 5:  return std::pair(0u, MilesFraction::F_5_16);
		case 6: 
		case 7:  return std::pair(0u, MilesFraction::F_3_8);
		case 8:
		case 9:  return std::pair(0u, MilesFraction::F_1_2);
		case 10: 
		case 11: return std::pair(0u, MilesFraction::F_5_8);
		case 12: 
		case 13: return std::pair(0u, MilesFraction::F_3_4);
		case 14: 
		case 15: return std::pair(0u, MilesFraction::F_7_8);
		case 16:
		case 17: return std::pair(1u, MilesFraction::NONE);
		case 18:
		case 19: return std::pair(1u, MilesFraction::F_1_8);
		case 20:
		case 21: return std::pair(1u, MilesFraction::F_1_4);
		case 22:
		case 23: return std::pair(1u, MilesFraction::F_3_8);
		case 24:
		case 25: return std::pair(1u, MilesFraction::F_1_2);
		case 26:
		case 27: return std::pair(1u, MilesFraction::F_5_8);
		case 28:
		case 29: return std::pair(1u, MilesFraction::F_3_4);
		case 30:
		case 31: return std::pair(1u, MilesFraction::F_7_8);
		case 32:
		case 33: 
		case 34:
		case 35: return std::pair(2u, MilesFraction::NONE);
		case 36:
		case 37: 
		case 38: 
		case 39: return std::pair(2u, MilesFraction::F_1_4);
		case 40: 
		case 41: 
		case 42: 
		case 43: return std::pair(2u, MilesFraction::F_1_2);
		case 44:
		case 45: 
		case 46:
		case 47: return std::pair(2u, MilesFraction::F_3_4);
		default: break;
	}
	unsigned int integer = topHeavyNumerator / denominator;
	if (integer > 15) integer = 5 * (integer / 5);
	return std::pair (integer, MilesFraction::NONE);
}

std::optional<Distance::Modifier> Distance::modifierFromChar(char c) {
	if (c == 'M') return Modifier::LESS_THAN;
	if (c == 'P') return Modifier::MORE_THAN;
	return std::optional<Modifier>();
}

std::optional<float> Distance::metersToUnit(float value, Unit unit) {
	switch(unit) {
		case Unit::METERS: 			return value;
		case Unit::STATUTE_MILES:	return (value / 1609.347);
		case Unit::FEET:			return (value / 0.3048);
	}
}

std::optional<float> Distance::milesToUnit(float value, Unit unit) {
	switch(unit) {
		case Unit::METERS: 			return (value * 1609.347);
		case Unit::STATUTE_MILES:	return value;
		case Unit::FEET:			return (value * 5280.0);
	}
}

std::optional<float> Distance::feetToUnit(float value, Unit unit) {
	switch(unit) {
		case Unit::METERS: 			return (value * 0.3048);
		case Unit::STATUTE_MILES:	return (value / 5280.0);
		case Unit::FEET:			return value;
	}
}

Distance Distance::cavokVisibility(bool unitMiles) {
	Distance result;
	result.distModifier = Modifier::MORE_THAN;
	result.dist = cavokVisibilityMeters;
	result.distUnit = Unit::METERS;
	if (unitMiles) {
		result.dist = cavokVisibilityMiles * statuteMileFactor;
		result.distUnit = Unit::STATUTE_MILES;
	}
	return result;
}

Distance Distance::makeDistant() {
	Distance result;
	result.distModifier = Modifier::DISTANT;
	return result;
}

Distance Distance::makeVicinity() {
	Distance result;
	result.distModifier = Modifier::VICINITY;
	return result;
}

////////////////////////////////////////////////////////////////////////////////

std::optional<Direction> Direction::fromCardinalString(
	const std::string & s, 
	bool enableOhdAlqds,
	bool enableUnknown)
{
	std::optional<Direction> error;
	if (s.empty()) return error;
	if (s == "NDV") {
		Direction dir;
		dir.dirType = Type::NDV;
		return dir;
	}
	if (enableOhdAlqds && s == "OHD") {
		Direction dir;
		dir.dirType = Type::OVERHEAD;
		return dir;
	}
	if (enableOhdAlqds && s == "ALQDS") {
		Direction dir;
		dir.dirType = Type::ALQDS;
		return dir;
	}	
	if (enableUnknown && s == "UNKNOWN") {
		Direction dir;
		dir.dirType = Type::UNKNOWN;
		return dir;
	}	
	int cardinalDegrees = 0;
	if (s == "N") cardinalDegrees = degreesTrueNorth;
	if (s == "W") cardinalDegrees = degreesTrueWest;
	if (s == "S") cardinalDegrees = degreesTrueSouth;
	if (s == "E") cardinalDegrees = degreesTrueEast;
	if (s == "NW") cardinalDegrees = degreesNorthWest;
	if (s == "NE") cardinalDegrees = degreesNorthEast;
	if (s == "SW") cardinalDegrees = degreesSouthWest;
	if (s == "SE") cardinalDegrees = degreesSouthEast;
	if (!cardinalDegrees) return error;
	Direction dir;
	dir.dirType = Type::VALUE_CARDINAL;
	dir.dirDegrees = cardinalDegrees;
	return dir;
}

std::optional<Direction> Direction::fromDegreesString(const std::string & s) {
	std::optional<Direction> error;
	Direction direction;
	if (s.length() != 3) return error;
	if (s == "///") {
		direction.dirType = Type::NOT_REPORTED;
		return direction;
	}
	if (s == "VRB") {
		direction.dirType = Type::VARIABLE;
		return direction;
	}
	//static const std::regex rgx("\\d\\d0");
	if (s[2] != '0') return error;
	const auto dir = strToUint(s, 0, 3);
	if (!dir.has_value()) return error;
	direction.dirType = Type::VALUE_DEGREES;
	direction.dirDegrees = *dir;
	return direction;
}

Direction::Cardinal Direction::cardinal(bool trueDirections) const {
	switch (type()) {
		case Type::NOT_REPORTED: return Cardinal::NOT_REPORTED;
		case Type::VARIABLE: return Cardinal::VRB;
		case Type::NDV: return Cardinal::NDV;
		case Type::OVERHEAD: return Cardinal::OHD;
		case Type::ALQDS: return Cardinal::ALQDS;
		case Type::UNKNOWN: return Cardinal::UNKNOWN;
		case Type::VALUE_DEGREES: break;
		case Type::VALUE_CARDINAL: break;
	}
	if (trueDirections) {
		if (dirDegrees == degreesTrueNorth) return Cardinal::TRUE_N;
		if (dirDegrees == degreesTrueSouth) return Cardinal::TRUE_S;
		if (dirDegrees == degreesTrueWest) return Cardinal::TRUE_W;
		if (dirDegrees == degreesTrueEast) return Cardinal::TRUE_E;
	}
	//Degree values specifying cardinal direction sectors must be sorted.
	if (dirDegrees <= octantSize/2) 						return Cardinal::N;
	if (dirDegrees <= (degreesNorthEast + octantSize/2)) 	return Cardinal::NE;
	if (dirDegrees <= (degreesTrueEast + octantSize/2))		return Cardinal::E;
	if (dirDegrees <= (degreesSouthEast + octantSize/2)) 	return Cardinal::SE;
	if (dirDegrees <= (degreesTrueSouth + octantSize/2))	return Cardinal::S;
	if (dirDegrees <= (degreesSouthWest + octantSize/2)) 	return Cardinal::SW;
	if (dirDegrees <= (degreesTrueWest + octantSize/2))		return Cardinal::W;
	if (dirDegrees <= (degreesNorthWest + octantSize/2)) 	return Cardinal::NW;
	if (dirDegrees <= maxDegrees) 							return Cardinal::N;
	return Cardinal::NOT_REPORTED;
}

std::optional<std::pair<Direction, Direction>> Direction::fromSectorString(
	const std::string & s)
{
	static const std::optional<std::pair<Direction, Direction>> notRecognised;
	static const std::regex rgx 
		("([NSWE][WE]?)(?:-[NSWE]|-[NS][WE])*-([NSWE][WE]?)");
	static const auto matchBegin = 1, matchEnd = 2;
	std::smatch match;
	if (!regex_match(s, match, rgx)) return notRecognised;
	const auto dirBegin = fromCardinalString(match.str(matchBegin));
	if (!dirBegin.has_value()) return(notRecognised);
	const auto dirEnd = fromCardinalString(match.str(matchEnd));
	if (!dirEnd.has_value()) return(notRecognised);
	return std::pair(*dirBegin, *dirEnd);
}

std::vector<Direction> Direction::sectorCardinalDirToVector(
	const Direction & dirFrom, 
	const Direction & dirTo)
{
	std::vector<Direction> result;
	const auto cardinalFrom = dirFrom.cardinal();
	const auto cardinalTo = dirTo.cardinal();
	if (cardinalFrom == Cardinal::NOT_REPORTED ||
		cardinalFrom == Cardinal::VRB || 
		cardinalFrom == Cardinal::NDV ||
		cardinalFrom == Cardinal::OHD ||
		cardinalFrom == Cardinal::ALQDS ||
		cardinalTo == Cardinal::NOT_REPORTED || 
		cardinalTo == Cardinal::VRB || 
		cardinalTo == Cardinal::NDV || 
		cardinalTo == Cardinal::OHD ||
		cardinalTo == Cardinal::ALQDS)
	{
		// Not a valid direction sector, one or both begin and end directions are 
		// not cardinal directions
		if (cardinalFrom != Cardinal::NOT_REPORTED) result.push_back(cardinalFrom);
		if (cardinalTo != Cardinal::NOT_REPORTED) result.push_back(cardinalTo);
		return result;
	}
	// Both sector begin and end are cardinal directions
	auto cardinalCurr = cardinalFrom;
	while (cardinalCurr != cardinalTo) 
	{
		result.push_back(cardinalCurr);
		cardinalCurr = rotateOctantClockwise(cardinalCurr);
	}
	result.push_back(cardinalTo);
	return result;
}

Direction::Cardinal Direction::rotateOctantClockwise(Cardinal cardinal) {
	switch(cardinal) {
		case Cardinal::TRUE_N:	return Cardinal::NE;
		case Cardinal::TRUE_E:	return Cardinal::SE;
		case Cardinal::TRUE_S:	return Cardinal::SW;
		case Cardinal::TRUE_W:	return Cardinal::NW;
		case Cardinal::N: 		return Cardinal::NE;
		case Cardinal::NE: 		return Cardinal::E;
		case Cardinal::E:		return Cardinal::SE;
		case Cardinal::SE: 		return Cardinal::S;
		case Cardinal::S:		return Cardinal::SW;
		case Cardinal::SW: 		return Cardinal::W;
		case Cardinal::W:		return Cardinal::NW;
		case Cardinal::NW: 		return Cardinal::N;
		default: 				return Cardinal::NOT_REPORTED;
	}
}

Direction::Direction(Cardinal c) {
	dirType = Type::VALUE_CARDINAL;
	switch (c) {
		case Cardinal::NOT_REPORTED: 	dirType = Type::NOT_REPORTED; break;
		case Cardinal::VRB:				dirType = Type::VARIABLE; break;
		case Cardinal::NDV:				dirType = Type::NDV; break;
		case Cardinal::N:				dirDegrees = degreesTrueNorth; break;			
		case Cardinal::S:				dirDegrees = degreesTrueSouth; break;
		case Cardinal::W:				dirDegrees = degreesTrueWest; break;
		case Cardinal::E:				dirDegrees = degreesTrueEast; break;
		case Cardinal::NW:				dirDegrees = degreesNorthWest; break;
		case Cardinal::NE:				dirDegrees = degreesNorthEast; break;
		case Cardinal::SW:				dirDegrees = degreesSouthWest; break;
		case Cardinal::SE:				dirDegrees = degreesSouthEast; break;
		case Cardinal::TRUE_N:			dirDegrees = degreesTrueNorth; break;
		case Cardinal::TRUE_W:			dirDegrees = degreesTrueSouth; break;
		case Cardinal::TRUE_S:			dirDegrees = degreesTrueWest; break;
		case Cardinal::TRUE_E:			dirDegrees = degreesTrueEast; break;
		case Cardinal::OHD:				dirType = Type::OVERHEAD; break;
		case Cardinal::ALQDS:			dirType = Type::ALQDS; break;
		case Cardinal::UNKNOWN:			dirType = Type::UNKNOWN; break;
	}
}


///////////////////////////////////////////////////////////////////////////////

std::optional<Pressure> Pressure::fromString(const std::string & s) {
	//static const std::regex rgx("([QA])(?:(\\d\\d\\d\\d)|////)");
	static const std::optional<Pressure> error;
	if (s.length() != 5) return error;
	Pressure pressure;
	if (s == "A////") {
		pressure.pressureUnit = Unit::INCHES_HG;
		return pressure;
	}
	if (s == "Q////") {
		pressure.pressureUnit = Unit::HECTOPASCAL;
		return pressure;
	}
	const auto pr = strToUint(s, 1, 4);
	if (!pr.has_value()) return error;
	switch (s[0]) {
		case 'A':
			pressure.pressureUnit = Unit::INCHES_HG;
			pressure.pressureValue = *pr * inHgDecimalPointShift;
			break;
		case 'Q':
			pressure.pressureUnit = Unit::HECTOPASCAL;
			pressure.pressureValue = *pr;
			break;
		default:  return error;
	}
	return pressure;
}

std::optional<Pressure> Pressure::fromForecastString(const std::string & s) {
	//static const std::regex rgx("QNH(\\d\\d\\d\\d)INS");
	static const std::optional<Pressure> error;
	if (s.length() != 10) return error;
	if (s[0] != 'Q' || s[1] != 'N' || s[2] != 'H') return error;
	if (s[7] != 'I' || s[8] != 'N' || s[9] != 'S') return error;
	const auto pr = strToUint(s, 3, 4);
	if (!pr.has_value()) return error;
	Pressure pressure;
	pressure.pressureUnit = Unit::INCHES_HG;
	pressure.pressureValue = *pr * inHgDecimalPointShift;
	return pressure;
}

std::optional<Pressure> Pressure::fromSlpString(const std::string & s) {
	//SLP982 = 998.2 hPa, SLP015 = 1001.5 hPa, SLP221 = 1022.1 hPa
	//static const std::regex rgx("SLP(\\d\\d\\d)");
	static const std::optional<Pressure> error;
	if (s.length() != 6) return error;
	if (s[0] != 'S' || s[1] != 'L' || s[2] != 'P') return error;
	const auto pr = strToUint(s, 3, 3);
	if (!pr.has_value()) return error;
	static const auto slpDecimalPointShift = 0.1;
	const auto base = (*pr < 500) ? 1000 : 900;
	Pressure pressure;
	pressure.pressureUnit = Unit::HECTOPASCAL;
	pressure.pressureValue = *pr * slpDecimalPointShift + base;
	return pressure;
}

std::optional<Pressure> Pressure::fromQfeString(const std::string & s) {
	//static const std::regex rgx("QFE(\\d\\d\\d)(/\\d\\d\\d\\d)?");
	static const std::optional<Pressure> error;
	if (s.length() != 6 && s.length() != 11) return error;
	if (s[0] != 'Q' || s[1] != 'F' || s[2] != 'E') return error;
	const auto mmHg = strToUint(s, 3, 3);
	if (!mmHg.has_value()) return error;
	if (s.length() == 11) {
		const auto hPa = strToUint(s, 7, 4);
		if (!hPa.has_value() || s[6] != '/') return error;
		//Value in hPa is ignored (parsed only for group syntax check)
	}
	Pressure pressure;
	pressure.pressureUnit = Unit::MM_HG;
	pressure.pressureValue = *mmHg;
	return pressure;
}

std::optional<Pressure> Pressure::fromTendencyString(const std::string & s) {
	//static const std::regex rgx("(\\d\\d\\d)|///");
	static const std::optional<Pressure> error;
	if (s.length() != 3) return error;
	if (s == "///") return Pressure();
	const auto hPa = strToUint(s, 0, 3);
	if (!hPa.has_value()) return error;
	Pressure pressure;
	pressure.pressureUnit = Unit::HECTOPASCAL;
	pressure.pressureValue = *hPa * tendencyDecimalPointShift;
	return pressure;
}

std::optional<float> Pressure::toUnit(Unit unit) const {
	if (!pressureValue.has_value()) return std::optional<float>();
	auto v = *pressureValue;
	static const auto hpaPerInHg = 33.8639;
	static const auto hpaPerMmHg = 1.3332;
	static const auto mmPerInch = 25.4;
	switch (pressureUnit) {
		case Unit::HECTOPASCAL:
		switch (unit) {
			case Unit::HECTOPASCAL: return v;
			case Unit::INCHES_HG: return (v / hpaPerInHg);
			case Unit::MM_HG: return (v / hpaPerMmHg);
		}

		case Unit::INCHES_HG:
		switch (unit) {
			case Unit::HECTOPASCAL: return (v * hpaPerInHg);
			case Unit::INCHES_HG: return v;
			case Unit::MM_HG: return (v * mmPerInch);
		}

		case Unit::MM_HG:
		switch (unit) {
			case Unit::HECTOPASCAL: return (v * hpaPerMmHg);
			case Unit::INCHES_HG: return (v / mmPerInch);
			case Unit::MM_HG: return v;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

std::optional<Precipitation> Precipitation::fromRainfallString(const std::string & s) {
	//static const std::regex rgx("\\d?\\d\\d\\.\\d");
	static const std::optional<Precipitation> error;
	if (s == "///./" || s == "//./") return Precipitation();
	if (s.length() != 4 && s.length() != 5) return error;
	if (s[s.length()-2] != '.') return error;
	const auto fractPart = strToUint(s, s.length() - 1, 1);
	if (!fractPart.has_value()) return error;
	const auto intPart = strToUint(s, 0, s.length() - 2);
	if (!intPart.has_value()) return error;
	Precipitation precipitation;
	precipitation.precipValue = *intPart + 0.1 * *fractPart;
	return precipitation;
}

std::optional<Precipitation> Precipitation::fromRunwayDeposits(const std::string & s) {
	//static const std::regex rgx("\\d\\d");
	std::optional<Precipitation> error;
	if (s.length() != 2) return error;
	if (s == "//") return Precipitation();
	const auto depth = strToUint(s, 0, 2);
	if (!depth.has_value()) return error;

	Precipitation precipitation;
	switch (*depth) {
		case Reserved::RESERVED: return error;

		case Reserved::DEPTH_10CM: precipitation.precipValue = 100; break;
		case Reserved::DEPTH_15CM: precipitation.precipValue = 150; break;
		case Reserved::DEPTH_20CM: precipitation.precipValue = 200; break;
		case Reserved::DEPTH_25CM: precipitation.precipValue = 250; break;
		case Reserved::DEPTH_30CM: precipitation.precipValue = 300; break;
		case Reserved::DEPTH_35CM: precipitation.precipValue = 350; break;
		case Reserved::DEPTH_40CM: precipitation.precipValue = 400; break;

		case Reserved::RUNWAY_NOT_OPERATIONAL: 
		precipitation.precipValue = std::optional<float>();
		break;

		default: 
		precipitation.precipValue = *depth;
		break;
	}
	return precipitation;
}

std::optional<Precipitation> Precipitation::fromRemarkString(const std::string & s,
		float factor,
		Precipitation::Unit unit,
		bool allowNotReported)
{
	//static const std::regex rgx("\\d\\d\\d\\d?");
	std::optional<Precipitation> error;
	Precipitation precipitation;
	precipitation.precipUnit = unit;
	if (s.length() != 3 && s.length() != 4) return error;
	if (s == "///" || s == "////") {
		if (!allowNotReported) return error;
		return precipitation;
	}
	const auto pr = strToUint(s, 0, s.length());
	if (!pr.has_value()) return error;
	precipitation.precipValue = *pr * factor;
	return precipitation;
}

std::optional<std::pair<Precipitation, Precipitation>>
	Precipitation::fromSnincrString(const std::string & s)
{
	//static const std::regex rgx("\\d?\\d)/(\\d?\\d");
	static const std::optional<std::pair<Precipitation, Precipitation>> error;
	const auto fraction = fractionStrToUint(s, 0, s.length());
	if (!fraction.has_value()) return error;

	Precipitation total;
	total.precipUnit = Precipitation::Unit::INCHES;
	total.precipValue = std::get<0>(*fraction);

	Precipitation change;
	change.precipUnit = Precipitation::Unit::INCHES;
	change.precipValue = std::get<1>(*fraction);

	return std::pair(total, change);
}

std::optional<float> Precipitation::toUnit(Unit unit) const {
	if (precipValue.has_value()) {
		if (precipUnit == unit) return precipValue;
		static const auto mmPerInch = 25.4;
		if (precipUnit == Unit::MM && unit == Unit::INCHES) {
			return (*precipValue / mmPerInch);
		}
		if (precipUnit == Unit::INCHES && unit == Unit::MM) {
			return (*precipValue * mmPerInch);
		}
	}
	return std::optional<float>();
}

///////////////////////////////////////////////////////////////////////////////

std::optional<SurfaceFriction> SurfaceFriction::fromString(const std::string & s) {
	//static const std::regex rgx("\\d\\d");
	static const std::optional<SurfaceFriction> error;
	if (s.length() != 2) return error;
	if (s == "//") return SurfaceFriction();
	const auto sfVal = strToUint(s, 0, 2);
	if (!sfVal.has_value()) return error;

	SurfaceFriction sf;
	auto coefficient = *sfVal;
	switch (coefficient) {
		case Reserved::BRAKING_ACTION_POOR:
		sf.sfType = Type::BRAKING_ACTION_REPORTED;
		sf.sfCoefficient = baPoorLowLimit;
		break;

		case Reserved::BRAKING_ACTION_MEDIUM_POOR:
		sf.sfType = Type::BRAKING_ACTION_REPORTED;
		sf.sfCoefficient = baMediumPoorLowLimit;
		break;

		case Reserved::BRAKING_ACTION_MEDIUM:
		sf.sfType = Type::BRAKING_ACTION_REPORTED;
		sf.sfCoefficient = baMediumLowLimit;
		break;

		case Reserved::BRAKING_ACTION_MEDIUM_GOOD:
		sf.sfType = Type::BRAKING_ACTION_REPORTED;
		sf.sfCoefficient = baMediumGoodLowLimit;
		break;

		case Reserved::BRAKING_ACTION_GOOD:
		sf.sfType = Type::BRAKING_ACTION_REPORTED;
		sf.sfCoefficient = baGoodLowLimit;
		break;

		case Reserved::RESERVED_96:
		case Reserved::RESERVED_97:
		case Reserved::RESERVED_98:
		return std::optional<SurfaceFriction>();

		case Reserved::UNRELIABLE:
		sf.sfType = Type::UNRELIABLE;
		break;

		default:
		sf.sfCoefficient = coefficient;
		sf.sfType = Type::SURFACE_FRICTION_REPORTED;
		break;
	}
	return sf;
}

SurfaceFriction::BrakingAction SurfaceFriction::brakingAction() const {
	if (sfType == Type::NOT_REPORTED ||
		sfType == Type::UNRELIABLE) 			return BrakingAction::NONE;
	if (sfCoefficient < baMediumPoorLowLimit) 	return BrakingAction::POOR;
	if (sfCoefficient < baMediumLowLimit) 		return BrakingAction::MEDIUM_POOR;
	if (sfCoefficient < baMediumGoodLowLimit) 	return BrakingAction::MEDIUM;
	if (sfCoefficient < baGoodLowLimit)			return BrakingAction::MEDIUM_GOOD;
	return BrakingAction::GOOD;
}

///////////////////////////////////////////////////////////////////////////////

std::optional<WaveHeight> WaveHeight::fromString(const std::string & s) {
	//static const std::regex rgx("S(\\d)|H(\\d?\\d?\\d)");
	static const std::optional<WaveHeight> error;
	if (s.length() < 2 || s.length() > 4) return error;
	WaveHeight wh;
	if (s == "H///") {
		wh.whType = Type::WAVE_HEIGHT;
		return wh;
	}
	if (s == "S/") {
		wh.whType = Type::STATE_OF_SURFACE;
		return wh;
	}
	if (s[0] == 'S') {
		if (s.length() != 2) return error;
		auto h = waveHeightFromStateOfSurfaceChar(s[1]);
		if (!h.has_value()) return error;
		wh.whType = Type::STATE_OF_SURFACE;
		wh.whValue = *h;
		return wh;
	}
	if (s[0] == 'H') {
		auto h = strToUint(s, 1, s.length() - 1);
		if (!h.has_value()) return error;
		wh.whType = Type::WAVE_HEIGHT;
		wh.whValue = *h;
		return wh;
	}
	return error;
}

std::optional<float> WaveHeight::toUnit(Unit unit) const {
	const auto wh = waveHeight();
	if (!wh.has_value() || whUnit != Unit::METERS) return std::optional<float>();
	switch (unit) {
		case Unit::METERS: return *wh;
		case Unit::FEET:   return *wh / 0.3048;
	}
}

WaveHeight::StateOfSurface WaveHeight::stateOfSurface() const {
	if (!whValue.has_value()) return StateOfSurface::NOT_REPORTED;
	const auto h = *whValue;
	// Wave heights must be sorted
	if (!h) 							return StateOfSurface::CALM_GLASSY;
	if (h <= maxWaveHeightCalmRippled) 	return StateOfSurface::CALM_RIPPLED;
	if (h <= maxWaveHeightSmooth) 		return StateOfSurface::SMOOTH;
	if (h <= maxWaveHeightSlight) 		return StateOfSurface::SLIGHT;
	if (h <= maxWaveHeightModerate) 	return StateOfSurface::MODERATE;
	if (h <= maxWaveHeightRough) 		return StateOfSurface::ROUGH;
	if (h <= maxWaveHeightVeryRough) 	return StateOfSurface::VERY_ROUGH;
	if (h <= maxWaveHeightHigh) 		return StateOfSurface::HIGH;
	if (h <= maxWaveHeightVeryHigh) 	return StateOfSurface::VERY_HIGH;
	return StateOfSurface::PHENOMENAL;
}

std::optional<unsigned int> WaveHeight::waveHeightFromStateOfSurfaceChar(char c) {
	switch (c) {
		case '0': return maxWaveHeightCalmGlassy;
		case '1': return maxWaveHeightCalmRippled;
		case '2': return maxWaveHeightSmooth;
		case '3': return maxWaveHeightSlight;
		case '4': return maxWaveHeightModerate;
		case '5': return maxWaveHeightRough;
		case '6': return maxWaveHeightVeryRough;
		case '7': return maxWaveHeightHigh;
		case '8': return maxWaveHeightVeryHigh;
		case '9': return minWaveHeightPhenomenal;
		default:  return std::optional<unsigned int>();
	}
}

///////////////////////////////////////////////////////////////////////////////

std::vector<WeatherPhenomena::Weather> WeatherPhenomena::weather() const 
{
	std::vector<Weather> result;
	for (auto i=0u; i < wsz; i++)
		result.push_back(w[i]);
	return result;
}


std::optional <WeatherPhenomena> WeatherPhenomena::fromString(const std::string & s,
		bool enableQualifiers)
{
	// Descriptors MI, PR, BC are allowed only with FG
	if (s == "MIFG") return WeatherPhenomena(Weather::FOG, Descriptor::SHALLOW);
	if (s == "PRFG") return WeatherPhenomena(Weather::FOG, Descriptor::PARTIAL);
	if (s == "BCFG") return WeatherPhenomena(Weather::FOG, Descriptor::PATCHES);
	// Descriptor DR is allowed only with DU SA SN
	if (s == "DRDU") return WeatherPhenomena(Weather::DUST, Descriptor::LOW_DRIFTING);
	if (s == "DRSA") return WeatherPhenomena(Weather::SAND, Descriptor::LOW_DRIFTING);
	if (s == "DRSN") return WeatherPhenomena(Weather::SNOW, Descriptor::LOW_DRIFTING);
	// Descriptor BL is allowed only with DU SA SN PY
	if (s == "BLDU") return WeatherPhenomena(Weather::DUST, Descriptor::BLOWING);
	if (s == "BLSA") return WeatherPhenomena(Weather::SAND, Descriptor::BLOWING);
	if (s == "BLSN") return WeatherPhenomena(Weather::SNOW, Descriptor::BLOWING);
	if (s == "BLPY") return WeatherPhenomena(Weather::SPRAY, Descriptor::BLOWING);
	// Descriptor TS is allowed alone (or with precipitation)
	if (s == "TS") return WeatherPhenomena(Descriptor::THUNDERSTORM);
	// Descriptor FZ is allowed only with FG (or with precipitation)
	if (s == "FZFG") return WeatherPhenomena(Weather::FOG, Descriptor::FREEZING);
	// Phenomena IC BR FG FU VA DU SA HZ PO SQ FC are allowed only when alone 
	// in the group, with no other phenomena present
	if (s == "IC") return WeatherPhenomena(Weather::ICE_CRYSTALS);
	if (s == "BR") return WeatherPhenomena(Weather::MIST);
	if (s == "FG") return WeatherPhenomena(Weather::FOG);
	if (s == "FU") return WeatherPhenomena(Weather::SMOKE);
	if (s == "VA") return WeatherPhenomena(Weather::VOLCANIC_ASH);
	if (s == "DU") return WeatherPhenomena(Weather::DUST);
	if (s == "SA") return WeatherPhenomena(Weather::SAND);
	if (s == "HZ") return WeatherPhenomena(Weather::HAZE);
	if (s == "PO") return WeatherPhenomena(Weather::DUST_WHIRLS);
	if (s == "SQ") return WeatherPhenomena(Weather::SQUALLS);
	if (s == "FC") return WeatherPhenomena(Weather::FUNNEL_CLOUD);
	// Phenomena SS DS are only allowed alone or in combination with each other
	if (s == "DS") return WeatherPhenomena(Weather::DUSTSTORM);
	if (s == "SS") return WeatherPhenomena(Weather::SANDSTORM);
	if (s == "DSSS") return WeatherPhenomena(Weather::DUSTSTORM, Weather::SANDSTORM);
	if (s == "SSDS") return WeatherPhenomena(Weather::SANDSTORM, Weather::DUSTSTORM);
	if (enableQualifiers) {
		// Qualifier VC is allowed only with VCTS VCFG VCSH VCPO VCFC VCVA VCBLDU 
		// VCBLSA VCBLSN VCDS VCSS
		if (s == "VCTS") 
			return WeatherPhenomena(Descriptor::THUNDERSTORM, Qualifier::VICINITY);
		if (s == "VCFG") 
			return WeatherPhenomena(Weather::FOG, Descriptor::NONE, Qualifier::VICINITY);
		if (s == "VCSH") 
			return WeatherPhenomena(Descriptor::SHOWERS, Qualifier::VICINITY);
		if (s == "VCPO") 
			return WeatherPhenomena(Weather::DUST_WHIRLS, Descriptor::NONE, Qualifier::VICINITY);
		if (s == "VCVA") 
			return WeatherPhenomena(Weather::VOLCANIC_ASH, Descriptor::NONE, Qualifier::VICINITY);
		if (s == "VCFC")
			return WeatherPhenomena(Weather::FUNNEL_CLOUD, Descriptor::NONE, Qualifier::VICINITY);
		if (s == "VCBLDU") 
			return WeatherPhenomena(Weather::DUST, Descriptor::BLOWING, Qualifier::VICINITY);
		if (s == "VCBLSA") 
			return WeatherPhenomena(Weather::SAND, Descriptor::BLOWING, Qualifier::VICINITY);
		if (s == "VCBLSN") 
			return WeatherPhenomena(Weather::SNOW, Descriptor::BLOWING, Qualifier::VICINITY);
		if (s == "VCDS") 
			return WeatherPhenomena(Weather::DUSTSTORM, Descriptor::NONE, Qualifier::VICINITY);
		if (s == "VCSS") 
			return WeatherPhenomena(Weather::SANDSTORM, Descriptor::NONE, Qualifier::VICINITY);
		// Qualifier + are allowed with FC DS SS (or with precipitation)
		if (s == "+FC")
			return WeatherPhenomena(Weather::FUNNEL_CLOUD, Descriptor::NONE, Qualifier::HEAVY);
		if (s == "+DS")
			return WeatherPhenomena(Weather::DUSTSTORM, Descriptor::NONE, Qualifier::HEAVY);
		if (s == "+SS")
			return WeatherPhenomena(Weather::SANDSTORM, Descriptor::NONE, Qualifier::HEAVY);
		if (s == "+DSSS") 
			return WeatherPhenomena(Weather::DUSTSTORM, Weather::SANDSTORM, 
				Descriptor::NONE, Qualifier::HEAVY);
		if (s == "+SSDS") 
			return WeatherPhenomena(Weather::SANDSTORM, Weather::DUSTSTORM, 
				Descriptor::NONE, Qualifier::HEAVY);
		// Qualifier RE is allowed with TS descriptor without any phenomena
		if (s == "RETS") 
			return WeatherPhenomena(Descriptor::THUNDERSTORM, Qualifier::RECENT);
	}
	// Precipitation
	WeatherPhenomena result;
	std::string precipStr = s;
	static const std::optional <WeatherPhenomena> error;
	if (precipStr.length() < 2) return(error);
	// Only + - RE qualifiers are allowed; no qualifier equals moderate intensity
	if (enableQualifiers) {
		result.q = Qualifier::MODERATE;
		switch (precipStr[0]) {
			case '+':
			result.q = Qualifier::HEAVY;
			precipStr = precipStr.substr(1);
			break;

			case '-':
			result.q = Qualifier::LIGHT;
			precipStr = precipStr.substr(1);
			break;

			case 'R': // qualifier "RE"
			if (precipStr[1] == 'E') { 
				result.q = Qualifier::RECENT;
				precipStr = precipStr.substr(2);
			}
			break;

			default:
			break;
		}
	}
	// Descriptors SH TS and FZ are allowed
	if (precipStr.length() < 2) return(error);
	if (precipStr.substr(0, 2) == "SH") result.d = Descriptor::SHOWERS;
	if (precipStr.substr(0, 2) == "TS") result.d = Descriptor::THUNDERSTORM;
	if (precipStr.substr(0, 2) == "FZ") result.d = Descriptor::FREEZING;
	if (result.d != Descriptor::NONE) precipStr = precipStr.substr(2);
	// Phenomena DZ RA SN SG PL GR GS UP are allowed in any combinations if no 
	// duplicate phenomena is specified
	// Descriptors without weather phenomena are not allowed at this point 
	// Descriptors SH TS are allowed only if at least one of the following #
	// phenomena is present RA SN PL GR GS UP
	// Descriptor FZ is allowed only if at least one of the following phenomena 
	// is present DZ RA UP
	if (precipStr.length() < 2 || precipStr.length() % 2) return(error);
	bool allowShDecriptor = false;
	bool allowFzDecriptor = false;
	for (auto i = 0u; i < wSize; i++) {
		if (precipStr.empty()) break; 
		const auto ws = precipStr.substr(0,2);
		std::optional<Weather> w;
		if (ws == "DZ") w = Weather::DRIZZLE;
		if (ws == "RA") w = Weather::RAIN;
		if (ws == "SN") w = Weather::SNOW;
		if (ws == "SG") w = Weather::SNOW_GRAINS;
		if (ws == "PL") w = Weather::ICE_PELLETS;
		if (ws == "GR") w = Weather::HAIL;
		if (ws == "GS") w = Weather::SMALL_HAIL;
		if (ws == "UP") w = Weather::UNDETERMINED;
		if (!w.has_value()) return error;
		if (isDescriptorShAllowed(*w)) allowShDecriptor = true;
		if (isDescriptorFzAllowed(*w)) allowFzDecriptor = true;
		for (auto j = 0u; j < i ; j++)
			if (result.w[j] == *w) return error;
		result.w[i] = *w;
		result.wsz = i + 1;
		precipStr = precipStr.substr(2);
	}
	if (!precipStr.empty()) return error;
	if (!allowShDecriptor && result.descriptor() == Descriptor::SHOWERS) return error;
	if (!allowFzDecriptor && result.descriptor() == Descriptor::FREEZING) return error;
	return result;
}

inline bool WeatherPhenomena::isDescriptorShAllowed (Weather w) {
	switch (w) {
		case Weather::RAIN:
		case Weather::SNOW:
		case Weather::ICE_PELLETS:
		case Weather::HAIL:
		case Weather::SMALL_HAIL:
		case Weather::UNDETERMINED:
		return true;

		default:
		return false;
	}
}

inline bool WeatherPhenomena::isDescriptorFzAllowed (Weather w) {
	switch (w) {
		case Weather::DRIZZLE:
		case Weather::RAIN:
		case Weather::UNDETERMINED:
		return true;

		default:
		return false;
	}
}

std::optional <WeatherPhenomena> WeatherPhenomena::fromWeatherBeginEndString(
	const std::string & s,
	const MetafTime & reportTime,
	const WeatherPhenomena & previous)
{
	std::optional <WeatherPhenomena> error;
	static const std::regex rgx("((?:[A-Z][A-Z]){0,4})([BE])(\\d\\d)?(\\d\\d)");
	static const auto matchPhenomena = 1, matchEvent = 2;
	static const auto matchHour = 3, matchMinute = 4;

	std::smatch match;
	if (!regex_match(s, match, rgx)) return error;

	WeatherPhenomena result;

	if (const auto phstr = match.str(matchPhenomena); !phstr.empty()) {
		const auto ph = fromString(phstr);
		if (!ph.has_value()) return error;
		result = *ph;
	} else {
		if (!previous.isValid()) return error;
		result = previous;
	}

	if (match.str(matchEvent) == "B") result.ev = Event::BEGINNING;
	if (match.str(matchEvent) == "E") result.ev = Event::ENDING;

	unsigned int hour = reportTime.hour();
	unsigned int minute = static_cast<unsigned int>(std::stoi(match.str(matchMinute)));
	if (const auto h = match.str(matchHour); !h.empty())
		hour = static_cast<unsigned int>(std::stoi(h));
	result.tm = MetafTime(hour, minute);

	return result;
}

bool WeatherPhenomena::isValid() const { 
	// Empty weather phenomena is not valid 
	if (qualifier() == Qualifier::NONE && descriptor() == Descriptor::NONE && !wsz) 
		return false;
	// Event time must be valid if present
	if (tm.has_value() && !tm->isValid()) return false;
	// Descriptor FZ only makes sense with precipitation which
	// can potentially freeze, i.e. DZ RA, or with UP, or with FG
	if (descriptor() == Descriptor::FREEZING) {
		bool dzRaUpFg = false;
		for (auto i = 0u; i < wSize; i++) {
			if (w[i] == Weather::DRIZZLE || w[i] == Weather::RAIN ||
				w[i] == Weather::UNDETERMINED || w[i] == Weather::FOG) 
			{
				dzRaUpFg = true; break;
			}
		}
		if (!dzRaUpFg) return false;
	}
	// Everything else is valid
	return true;
}

///////////////////////////////////////////////////////////////////////////////

std::optional<CloudType> CloudType::fromString(const std::string & s) {
	static const std::optional<CloudType> error;
	if (s.empty()) return error;
	if (s[0] >= '0' && s[0] <= '9') {
		// Format with height 8NS070 or 3TCU022 (6 or 7 chars)
		// std::regex("(\d)([A-Z][A-Z][A-Z]?)(\d\d\d)")
		if (s.length() != 6 && s.length() != 7) return error;
		static const auto heightDigits = 3u, oktaDigits = 1u;
		const auto typeStrLen = s.length() - heightDigits - oktaDigits;
		const auto heightValue = 
			Distance::fromHeightString(s.substr(s.length() - heightDigits));
		if (!heightValue.has_value()) return error;
		if (!heightValue->isReported()) return error;
		const auto typeValue = cloudTypeFromString(s.substr(oktaDigits, typeStrLen));
		if (typeValue == Type::NOT_REPORTED) return error;
		return CloudType(typeValue, *heightValue, s[0] - '0');
	} else {
		// Format without height BLSN1 or SC1 (3 or more chars)
		// std::regex("([A-Z][A-Z]+)(\d)")
		if (s.length() < 3) return error;
		const auto oktaCh = s[s.length() - 1];
		if (oktaCh < '0' || oktaCh > '9') return error;
		const auto typeValue = 
			cloudTypeOrObscurationFromString(s.substr(0, s.length() - 1));
		if (typeValue == Type::NOT_REPORTED) return error;
		return CloudType(typeValue, Distance(), oktaCh - '0');
	}
}

std::optional<CloudType> CloudType::fromStringObscuration(const std::string & s) {
	auto type = Type::NOT_REPORTED;
	if (s == "BLSN") type = Type::BLOWING_SNOW;
	if (s == "BLDU") type = Type::BLOWING_DUST;
	if (s == "BLSA") type = Type::BLOWING_SAND;
	if (s == "VA") type = Type::VOLCANIC_ASH;
	if (s == "FU") type = Type::SMOKE;
	if (s == "FG") type = Type::FOG;
	if (type == Type::NOT_REPORTED) return std::optional<CloudType>();
	return CloudType(type, Distance(), 0);
}


CloudType::Type CloudType::cloudTypeFromString(const std::string & s) {
	if (s == "CB")    return Type::CUMULONIMBUS;
	if (s == "TCU")   return Type::TOWERING_CUMULUS;
	if (s == "CU")    return Type::CUMULUS;
	if (s == "CF")    return Type::CUMULUS_FRACTUS;
	if (s == "SC")    return Type::STRATOCUMULUS;
	if (s == "NS")    return Type::NIMBOSTRATUS;
	if (s == "ST")    return Type::STRATUS;
	if (s == "SF")    return Type::STRATUS_FRACTUS;
	if (s == "AS")    return Type::ALTOSTRATUS;
	if (s == "AC")    return Type::ALTOCUMULUS;
	if (s == "ACC")   return Type::ALTOCUMULUS_CASTELLANUS;
	if (s == "CI")    return Type::CIRRUS;
	if (s == "CS")    return Type::CIRROSTRATUS;
	if (s == "CC")    return Type::CIRROCUMULUS;
	return Type::NOT_REPORTED;
}

CloudType::Type CloudType::cloudTypeOrObscurationFromString(const std::string & s) {
	if (const auto t = cloudTypeFromString(s); t != Type::NOT_REPORTED) return t;
	if (s == "BLSN")  return Type::BLOWING_SNOW;
	if (s == "BLDU")  return Type::BLOWING_DUST;
	if (s == "BLSA")  return Type::BLOWING_SAND;
	if (s == "IC")    return Type::ICE_CRYSTALS;
	if (s == "RA")    return Type::RAIN;
	if (s == "DZ")    return Type::DRIZZLE;
	if (s == "SN")    return Type::SNOW;
	if (s == "PL")    return Type::ICE_PELLETS;
	if (s == "FU")    return Type::SMOKE;
	if (s == "FG")    return Type::FOG;
	if (s == "BR")    return Type::MIST;
	if (s == "HZ")    return Type::HAZE;
	if (s == "VA") 	  return Type::VOLCANIC_ASH;
	return Type::NOT_REPORTED;
}

///////////////////////////////////////////////////////////////////////////////

std::optional<KeywordGroup> KeywordGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	if (reportPart == ReportPart::HEADER) {
		if (group == "METAR") return KeywordGroup(Type::METAR);
		if (group == "SPECI") return KeywordGroup(Type::SPECI);
		if (group == "TAF") return KeywordGroup(Type::TAF);
		if (group == "AMD") return KeywordGroup(Type::AMD);
	}
	if (reportPart == ReportPart::HEADER || reportPart == ReportPart::METAR) {
		if (group == "COR") return KeywordGroup(Type::COR);
	}
	if (reportPart == ReportPart::HEADER ||
		reportPart == ReportPart::METAR ||
		reportPart == ReportPart::TAF) {
			if (group == "NIL") return KeywordGroup(Type::NIL);
			if (group == "CNL") return KeywordGroup(Type::CNL);
	}
	if (reportPart == ReportPart::METAR) {
		if (group == "AUTO") return KeywordGroup(Type::AUTO);
	}
	if (reportPart == ReportPart::METAR || reportPart == ReportPart::TAF) {
		if (group == "CAVOK") return KeywordGroup(Type::CAVOK);
		if (group == "RMK") return KeywordGroup(Type::RMK);
	}
	if (reportPart == ReportPart::RMK) {
		if (group == "AO1") return KeywordGroup(Type::AO1);
		if (group == "AO2") return KeywordGroup(Type::AO2);
		if (group == "AO1A") return KeywordGroup(Type::AO1A);
		if (group == "AO2A") return KeywordGroup(Type::AO2A);
		if (group == "NOSPECI") return KeywordGroup(Type::NOSPECI);
	}
	if (group == "$") return KeywordGroup(Type::MAINTENANCE_INDICATOR);
	return std::optional<KeywordGroup>();
}

AppendResult KeywordGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)group; (void)reportMetadata; (void)reportPart;
	return AppendResult::NOT_APPENDED;
}

///////////////////////////////////////////////////////////////////////////////

std::optional<LocationGroup> LocationGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	static const std::optional<LocationGroup> notRecognised;
	if (reportPart != ReportPart::HEADER) return notRecognised;
	static const std::regex rgx = std::regex("[A-Z][A-Z0-9]{3}");
	if (!regex_match(group, rgx)) return notRecognised;
	LocationGroup result;
	strncpy(result.location, group.c_str(), locationLength);
	result.location[locationLength] = '\0';
	return result;
}

AppendResult LocationGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)group; (void)reportPart;
	return AppendResult::NOT_APPENDED;
}

///////////////////////////////////////////////////////////////////////////////

std::optional<ReportTimeGroup> ReportTimeGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	static const std::optional<ReportTimeGroup> notRecognised;
	static const std::regex rgx ("\\d\\d\\d\\d\\d\\dZ");
	static const auto posTime = 0, lenTime = 6;
	if (reportPart != ReportPart::HEADER) return notRecognised;
	if (!regex_match(group, rgx)) return notRecognised;
	const auto tm = MetafTime::fromStringDDHHMM(group.substr(posTime, lenTime));
	if (!tm.has_value()) return notRecognised;
	if (!tm->day().has_value()) return notRecognised;
	ReportTimeGroup g;
	g.t = *tm;
	return g;
}

AppendResult ReportTimeGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)group; (void)reportPart;
	return AppendResult::NOT_APPENDED;
}

///////////////////////////////////////////////////////////////////////////////

std::optional<TrendGroup> TrendGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	if (reportPart == ReportPart::METAR || reportPart == ReportPart::TAF) {
		// Detect trend type groups
		if (group == "BECMG") return TrendGroup(Type::BECMG);
		if (group == "TEMPO") return TrendGroup(Type::TEMPO);
		if (group == "INTER") return TrendGroup(Type::INTER);
	}
	if (reportPart == ReportPart::TAF) {
		// Detect probability groups
		if (group == "PROB30") return TrendGroup(Probability::PROB_30);
		if (group == "PROB40") return TrendGroup(Probability::PROB_40);
		// Detect time span groups
		if (auto timeSpan = fromTimeSpan(group); timeSpan.has_value()) return timeSpan;
		// Detect FMxxxxxx groups
		if (auto longFm = fromFm(group); longFm.has_value()) return longFm;
	}
	if (reportPart == ReportPart::METAR){
		// Detect NOSIG trend type
		if (group == "NOSIG") return TrendGroup(Type::NOSIG);
		// Detect FMxxxx /TLxxxx /ATxxxx
		if (auto trendTime = fromTrendTime(group); trendTime.has_value()) return trendTime;
		// Detect time span in format HHMM/HHMM used in Australia
		if (auto timeSpan = fromTimeSpanHHMM(group); timeSpan.has_value()) return timeSpan;
	}
	if (reportPart == ReportPart::HEADER || reportPart == ReportPart::TAF) {
		// Detect time span
		if (auto ts = fromTimeSpan(group); ts.has_value()) return ts;
	}
	// Cannot detect group
	return std::optional<TrendGroup>();
}

AppendResult TrendGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	if (type() == Type::NOSIG) return AppendResult::NOT_APPENDED;
	const auto nextGroup = parse(group, reportPart, reportMetadata);
	if (!nextGroup.has_value()) return AppendResult::NOT_APPENDED;
	if (combineProbAndTrendTypeGroups(*nextGroup)) return AppendResult::APPENDED;
	if (combineTrendTypeAndTimeGroup(*nextGroup)) return AppendResult::APPENDED;
	if (combineProbAndTimeSpanGroups(*nextGroup)) return AppendResult::APPENDED;
	if (combineIncompleteGroups(*nextGroup)) return AppendResult::APPENDED;
	return AppendResult::NOT_APPENDED;
}

std::optional<TrendGroup> TrendGroup::fromTimeSpan(const std::string & s) {
	static const std::optional<TrendGroup> notRecognised;
	static const std::regex rgx("(\\d\\d\\d\\d)/(\\d\\d\\d\\d)");
	static const auto matchFrom = 1, matchTill = 2;
	std::smatch match;
	if (!regex_match(s, match, rgx)) return notRecognised;
	const auto from = MetafTime::fromStringDDHH(match.str(matchFrom));
	const auto till = MetafTime::fromStringDDHH(match.str(matchTill));
	if (!from.has_value() || !till.has_value()) return notRecognised;

	TrendGroup result;
	result.t = Type::TIME_SPAN;
	result.isTafTimeSpanGroup = true;
	result.tFrom = from;
	result.tTill = till;
	return result;
}

std::optional<TrendGroup> TrendGroup::fromTimeSpanHHMM(const std::string & s) {
	static const std::optional<TrendGroup> notRecognised;
	static const std::regex rgx("(\\d\\d\\d\\d)/(\\d\\d\\d\\d)");
	static const auto matchFrom = 1, matchTill = 2;
	std::smatch match;
	if (!regex_match(s, match, rgx)) return notRecognised;
	const auto from = MetafTime::fromStringDDHHMM(match.str(matchFrom));
	const auto till = MetafTime::fromStringDDHHMM(match.str(matchTill));
	if (!from.has_value() || !till.has_value()) return notRecognised;

	TrendGroup result;
	result.t = Type::TIME_SPAN;
	result.isTafTimeSpanGroup = true;
	result.tFrom = from;
	result.tTill = till;
	return result;
}

std::optional<TrendGroup> TrendGroup::fromFm(const std::string & s) {
	static const std::optional<TrendGroup> notRecognised;
	static const std::regex rgx("FM\\d\\d\\d\\d\\d\\d");
	static const auto posTime = 2, lenTime = 6;
	if (!regex_match(s, rgx)) return notRecognised;
	const auto time = MetafTime::fromStringDDHHMM(s.substr(posTime, lenTime));
	if (!time.has_value()) return notRecognised;

	TrendGroup result;
	result.t = Type::FROM;
	result.tFrom = time;
	return result;
}

std::optional<TrendGroup> TrendGroup::fromTrendTime(const std::string & s) {
	static const std::optional<TrendGroup> notRecognised;
	static const std::regex rgx("([FTA][MLT])(\\d\\d\\d\\d)");
	static const auto matchType = 1, matchTime = 2;
	std::smatch match;
	if (!regex_match(s, match, rgx)) return notRecognised;
	const auto time = MetafTime::fromStringDDHHMM(match.str(matchTime));
	if (!time.has_value()) return notRecognised;
	TrendGroup result;
	if (match.str(matchType) == "FM") {
		result.t = Type::FROM;
		result.tFrom = time;
		return result;
	}
	if (match.str(matchType) == "TL") {
		result.t = Type::UNTIL;
		result.tTill = time;
		return result;
	}
	if (match.str(matchType) == "AT") {
		result.t = Type::AT;
		result.tAt = time;
		return result;
	}
	return notRecognised;
}

bool TrendGroup::combineProbAndTrendTypeGroups(const TrendGroup & nextTrendGroup) {
	if (!isProbabilityGroup()) return false;
	if (!nextTrendGroup.isTrendTypeGroup()) return false;
	if (nextTrendGroup.type() != Type::TEMPO &&
		nextTrendGroup.type() != Type::INTER) return false;
	t = nextTrendGroup.type();
	return true;
}

bool TrendGroup::combineTrendTypeAndTimeGroup(const TrendGroup & nextTrendGroup) {
	if (type() != Type::BECMG &&
		type() != Type::TEMPO &&
		type() != Type::INTER) return false;
	if (!nextTrendGroup.isTimeSpanGroup() &&
		!nextTrendGroup.isTrendTimeGroup()) return false;
	if (!canCombineTime(*this, nextTrendGroup)) return false;
	combineTime(nextTrendGroup);
	return true;
}

bool TrendGroup::combineProbAndTimeSpanGroups(const TrendGroup & nextTrendGroup) {
	if (!isProbabilityGroup()) return false;
	if (!nextTrendGroup.isTimeSpanGroup()) return false;
	combineTime(nextTrendGroup);
	t = Type::TIME_SPAN;
	return true;
}

bool TrendGroup::combineIncompleteGroups(const TrendGroup & nextTrendGroup) {
	if (probability() != Probability::NONE ||
		type() != Type::FROM ||
		nextTrendGroup.type() != Type::UNTIL) return false;
	t = Type::TIME_SPAN;
	tTill = nextTrendGroup.tTill;
	return true;
}

bool TrendGroup::isProbabilityGroup() const {
	// Probability group has format PROB30 or PROB40
	// Probability must be reported and no time allowed
	if (type() != Type::PROB) return false;
	if (probability() == Probability::NONE) return false;
	if (timeFrom().has_value() || timeUntil().has_value()) return false;
	if (timeAt().has_value()) return false;
	return true;
}

bool TrendGroup::isTrendTypeGroup() const {
	// Trend type group is a group BECMG / TEMPO / INTER
	// No probability or time allowed
	if (type() != Type::BECMG &&
		type() != Type::TEMPO &&
		type() != Type::INTER) return false;
	if (probability() != Probability::NONE) return false;
	if (timeFrom().has_value() || timeUntil().has_value()) return false;
	if (timeAt().has_value()) return false;
	return true;
}

bool TrendGroup::isTrendTimeGroup() const {
	// Trend time group has format FMxxxx, TLxxxx, ATxxxx
	// Only one time from timeFrom, timeUntil or timeAt can be reported
	if (type() != Type::FROM && type() != Type::UNTIL && type() != Type::AT) 
		return false;
	if (probability() != Probability::NONE) return false;
	if (!timeFrom() && !timeUntil() && !timeAt()) return false;
	if (timeFrom() && timeUntil()) return false;
	if (timeFrom() && timeAt()) return false;
	if (timeUntil() && timeAt()) return false;
	return true;
}

bool TrendGroup::isTimeSpanGroup() const {
	// Time span group has format DDHH/DDHH,
	// only time 'from' and 'till' must be reported
	// isTafTimeSpanGroup guarantees that DDHH/DDHH group is 
	// not confused with appended groups FMxxxx TLxxxx and HHMM/HHMM
	if (type() != Type::TIME_SPAN ||
		!isTafTimeSpanGroup ||
		probability() != Probability::NONE ||
		!timeFrom().has_value() || 
		!timeUntil().has_value()) return false;
	if (timeAt().has_value()) return false;
	return true;
}

bool TrendGroup::canCombineTime(const TrendGroup & g1, const TrendGroup & g2) {
	// Cannot combine time 'from' with 'from', 'till' with 'till', 'at' with 'at'
	if (g1.timeFrom().has_value() && g2.timeFrom().has_value()) return false;
	if (g1.timeUntil().has_value() && g2.timeUntil().has_value()) return false;
	if (g1.timeAt().has_value() && g2.timeAt().has_value()) return false;
	// Cannot combine time 'from' or 'till' with 'at'
	if (g1.timeAt().has_value() &&
	       (g2.timeFrom().has_value() || g2.timeUntil().has_value())) return false;
	if (g2.timeAt().has_value() &&
	       (g1.timeFrom().has_value() || g1.timeUntil().has_value())) return false;
	return true;	
}

void TrendGroup::combineTime(const TrendGroup & nextTrendGroup) {
	if (!timeFrom().has_value()) tFrom = nextTrendGroup.timeFrom();
	if (!timeUntil().has_value()) tTill = nextTrendGroup.timeUntil();
	if (!timeAt().has_value()) tAt = nextTrendGroup.timeAt();
}

///////////////////////////////////////////////////////////////////////////////

std::optional<WindGroup> WindGroup::parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	static const std::optional<WindGroup> notRecognised;

	if (reportPart == ReportPart::METAR) {
		if (group == "WS") return WindGroup(Type::WIND_SHEAR_IN_LOWER_LAYERS, IncompleteText::WS);
	}
	if (reportPart == ReportPart::TAF) {
		if (group == "WSCONDS") return WindGroup(Type::WSCONDS);
	}
	if (reportPart == ReportPart::RMK) {
		if (group == "WSHFT") return WindGroup(Type::WIND_SHIFT);
		if (group == "PK") return WindGroup(Type::PEAK_WIND, IncompleteText::PK);
		if (group == "WND") return WindGroup(Type::WND_MISG, IncompleteText::WND);
	}

	if (reportPart != ReportPart::METAR &&
		reportPart != ReportPart::TAF) return notRecognised;

	if (const auto result = parseVariableSector(group); result.has_value())
		return *result;

	static const std::regex windRgx("(?:WS(\\d\\d\\d)/)?"
		"(\\d\\d0|VRB|///)([1-9]?\\d\\d|//)(?:G([1-9]?\\d\\d))?([KM][TMP][HS]?)");
	static const auto matchWindShearHeight = 1, matchWindDir = 2;
	static const auto matchWindSpeed = 3, matchWindGust = 4, matchWindUnit = 5;

	// Surface wind or wind shear, e.g. dd0ssKT or dd0ssGggMPS or WShhhdd0ssGggKT
	if (std::smatch match; std::regex_match(group, match, windRgx)) {
		const auto speedUnit = Speed::unitFromString(match.str(matchWindUnit));
		if (!speedUnit.has_value()) return notRecognised;
		const auto speed = Speed::fromString(match.str(matchWindSpeed), *speedUnit);
		if (!speed.has_value()) return notRecognised;

		WindGroup result;

		if (!match.length(matchWindShearHeight) && 
			!match.length(matchWindGust) &&
			match.str(matchWindDir) == "000" &&
			match.str(matchWindSpeed) == "00")
		{
			//00000KT or 00000MPS or 00000KMH: calm wind
			result.windType = Type::SURFACE_WIND_CALM;
			result.wSpeed = *speed;
			return result;
		}

		const auto dir = Direction::fromDegreesString(match.str(matchWindDir));
		if (!dir.has_value()) return notRecognised;
		result.windDir = *dir;
		result.wSpeed = *speed;
		const auto gust = Speed::fromString(match.str(matchWindGust), *speedUnit);
		if (gust.has_value()) result.gSpeed = *gust;
		const auto wsHeight = Distance::fromHeightString(match.str(matchWindShearHeight));
		result.windType = Type::SURFACE_WIND;
		if (wsHeight.has_value()) {
			result.windType = Type::WIND_SHEAR;
			result.wShHeight = *wsHeight;
		}
		return result;
	}

	return notRecognised;
}

AppendResult WindGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportPart; (void)reportMetadata;
	//Append variable wind sector group to surface wind group
	switch (incompleteText) {
		case IncompleteText::NONE:
		switch (type()) {
			case Type::SURFACE_WIND: return appendVariableSector(group);
			case Type::WIND_SHIFT: return appendWindShift(group, reportMetadata);
			default: return AppendResult::NOT_APPENDED;
		}
		
		case IncompleteText::PK:
		if (group != "WND") return AppendResult::GROUP_INVALIDATED;
		incompleteText = IncompleteText::PK_WND;
		return AppendResult::APPENDED;		

		case IncompleteText::PK_WND:
		return appendPeakWind(group, reportMetadata);

		case IncompleteText::WND:
		if (group != "MISG") return AppendResult::GROUP_INVALIDATED;
		incompleteText = IncompleteText::NONE;
		return AppendResult::APPENDED;

		case IncompleteText::WS_ALL:
		if (group != "RWY") return AppendResult::GROUP_INVALIDATED;
		incompleteText = IncompleteText::NONE;
		rw = Runway::makeAllRunways();
		return AppendResult::APPENDED;

		case IncompleteText::WS:
		if (group == "ALL") {
			incompleteText =  IncompleteText::WS_ALL;
			return AppendResult::APPENDED;
		}
		if (const auto r = Runway::fromString(group, true); r.has_value()) {
			incompleteText = IncompleteText::NONE;
			rw = r;
			return AppendResult::APPENDED;
		}
		return AppendResult::GROUP_INVALIDATED;
	}
}

std::optional<WindGroup> WindGroup::parseVariableSector(const std::string & group) {
	static const std::optional<WindGroup> notRecognised;
	static const std::regex varWindRgx("(\\d\\d0)V(\\d\\d0)");
	static const auto matchVarWindBegin = 1, matchVarWindEnd = 2;

	std::smatch match; 
	if (!std::regex_match(group, match, varWindRgx)) return notRecognised; 
	WindGroup result;
	const auto begin = Direction::fromDegreesString(match.str(matchVarWindBegin));
	if (!begin.has_value()) return notRecognised;
	result.vsecBegin = *begin;
	const auto end = Direction::fromDegreesString(match.str(matchVarWindEnd));
	if (!end.has_value()) return notRecognised;
	result.vsecEnd = *end;
	result.windType = Type::VARIABLE_WIND_SECTOR;
	return result;
}

AppendResult WindGroup::appendVariableSector(const std::string & group) {
	if (const auto vs = parseVariableSector(group); vs.has_value()) {
		windType = Type::SURFACE_WIND_WITH_VARIABLE_SECTOR;
		vsecBegin = vs->vsecBegin;
		vsecEnd = vs->vsecEnd;
		return AppendResult::APPENDED;
	}
	return AppendResult::NOT_APPENDED;
}

AppendResult WindGroup::appendPeakWind(const std::string & group,
	const ReportMetadata & reportMetadata)
{

	static const std::regex pkWndRgx("(\\d\\d0)([1-9]?\\d\\d)/(\\d\\d)?(\\d\\d)");
	static const auto matchDir = 1, matchSpeed = 2;
	static const auto matchHour = 3, matchMinute = 4;

	std::smatch match;
	if (!std::regex_match(group, match, pkWndRgx)) return AppendResult::GROUP_INVALIDATED;

	windType = Type::PEAK_WIND;
	const auto dir = Direction::fromDegreesString(match.str(matchDir));
	if (!dir.has_value()) return AppendResult::GROUP_INVALIDATED;
	windDir = *dir;

	const auto speed =
		Speed::fromString(match.str(matchSpeed), Speed::Unit::KNOTS);
	if (!speed.has_value()) return AppendResult::GROUP_INVALIDATED;
	wSpeed = *speed;

	if (!reportMetadata.reportTime.has_value() && match.str(matchHour).empty()) {
		return AppendResult::GROUP_INVALIDATED;
	}
	const auto hour = match.str(matchHour).empty() ?
		reportMetadata.reportTime->hour() : stoi(match.str(matchHour));
	const auto minute = stoi(match.str(matchMinute));
	evTime = MetafTime(hour, minute);
	incompleteText = IncompleteText::NONE;

	return AppendResult::APPENDED;
}

AppendResult WindGroup::appendWindShift(const std::string & group,
	const ReportMetadata & reportMetadata)
{
	//Append FROPA to wind shift group with or without time
	if (group == "FROPA") {
		windType = Type::WIND_SHIFT_FROPA;
		return AppendResult::APPENDED;
	}
	if (eventTime().has_value()) return AppendResult::NOT_APPENDED;
	//Append 2-digit time to WSHFT
	if (!eventTime().has_value() &&
		reportMetadata.reportTime.has_value() &&
		group.length() == 2)
	{
		const auto minuteVal = strToUint(group, 0, 2);
		if (!minuteVal.has_value()) return AppendResult::NOT_APPENDED;
		evTime = MetafTime(reportMetadata.reportTime->hour(), *minuteVal);
		return AppendResult::APPENDED;
	}
	//Append 4-digit time to WSHFT
	if (!eventTime().has_value() && group.length() == 4)
	{
		const auto hourMinuteVal = strToUint(group, 0, 4);
		if (!hourMinuteVal.has_value()) return AppendResult::NOT_APPENDED;
		// hourMinuteVal.value() has 4 digits, format hhmm
		const auto hour = *hourMinuteVal / 100;
		const auto minute = *hourMinuteVal % 100;
		evTime = MetafTime (hour, minute);
		return AppendResult::APPENDED;
	}
	return AppendResult::NOT_APPENDED;
}

bool WindGroup::isValid() const {
	if (incompleteText != IncompleteText::NONE) return false;
	if (!gustSpeed().speed().value_or(1)) return false;
	if (!height().distance().value_or(1)) return false;
	if (!runway().value_or(Runway()).isValid()) return false;
	if (!eventTime().value_or(MetafTime()).isValid()) return false;
	if (windSpeed().speed().value_or(0) >= gustSpeed().speed().value_or(999)) 
		return false; // if wind speed cannot be greater than gust speed
	return (direction().isValid() &&
			height().isValid() &&
			varSectorBegin().isValid() &&
			varSectorEnd().isValid());
}

///////////////////////////////////////////////////////////////////////////

std::optional<VisibilityGroup> VisibilityGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	if (reportPart == ReportPart::RMK) {
		if (group == "VIS") 
			return VisibilityGroup(Type::PREVAILING, IncompleteText::VIS);
		if (group == "SFC")
			return VisibilityGroup(Type::SURFACE, IncompleteText::SFC_OR_TWR);
		if (group == "TWR")
			return VisibilityGroup(Type::TOWER, IncompleteText::SFC_OR_TWR);
		if (group == "RVRNO") 
			return VisibilityGroup(Type::RVRNO, IncompleteText::NONE);
		if (group == "VISNO") 
			return VisibilityGroup(Type::VISNO, IncompleteText::VISNO);
		if (group == "RVR") 
			return VisibilityGroup(Type::RVR_MISG, IncompleteText::RVR);
	}
	if (reportPart == ReportPart::METAR || reportPart == ReportPart::TAF) {
		if (const auto v = fromIncompleteInteger(group); v.has_value()) return v;
		if (const auto v = fromMeters(group); v.has_value()) return v;
		if (const auto v = Distance::fromMileString(group); v.has_value()) {
			VisibilityGroup result;
			result.vis = *v;
			return result;
		}
	}
	if (reportPart == ReportPart::METAR) {
		if (const auto v = fromRvr(group); v.has_value()) return v;
	}
	return std::optional<VisibilityGroup>();
}

AppendResult VisibilityGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportPart;(void)reportMetadata;

	switch (incompleteText) {
		case IncompleteText::NONE:
		return AppendResult::NOT_APPENDED;

		case IncompleteText::INTEGER:
		if (appendFractionToIncompleteInteger(group)) return AppendResult::APPENDED;
		return AppendResult::GROUP_INVALIDATED;
		
		case IncompleteText::VIS:
		if (group == "MISG") {
			visType = Type::VIS_MISG;
			incompleteText = IncompleteText::NONE;
			return AppendResult::APPENDED; 
		}
		if (appendDirection(group, IncompleteText::VIS_DIR)) return AppendResult::APPENDED;
		if (appendInteger(group, IncompleteText::VIS_INTEGER)) return AppendResult::APPENDED;
		if (appendFraction(group, IncompleteText::VIS_FRACTION_OR_METERS)) return AppendResult::APPENDED;
		if (appendVariable(group, IncompleteText::VIS_VAR_MAX_INT, IncompleteText::VIS_VAR)) return AppendResult::APPENDED;
		if (appendMeters(group, IncompleteText::VIS_FRACTION_OR_METERS)) return AppendResult::APPENDED;
		if (appendVariableMeters(group, IncompleteText::VIS_VAR)) return AppendResult::APPENDED;
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteText::VIS_INTEGER:
		if (appendVariable(group, IncompleteText::VIS_VAR_MAX_INT, IncompleteText::VIS_VAR)) return AppendResult::APPENDED;
		if (appendFraction(group, IncompleteText::VIS_FRACTION_OR_METERS)) return AppendResult::APPENDED;
		if (appendRunway(group)) return AppendResult::APPENDED;
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteText::VIS_VAR_MAX_INT:
		if (appendVariableMaxFraction(group, IncompleteText::VIS_VAR)) return AppendResult::APPENDED;
		if (appendRunway(group)) return AppendResult::APPENDED;
		incompleteText = IncompleteText::NONE;
		return AppendResult::NOT_APPENDED;

		case IncompleteText::VIS_VAR:
		if (appendRunway(group)) return AppendResult::APPENDED;
		incompleteText = IncompleteText::NONE;
		return AppendResult::NOT_APPENDED;

		case IncompleteText::VIS_FRACTION_OR_METERS:
		if (appendRunway(group)) return AppendResult::APPENDED;
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteText::VIS_DIR:
		if (appendInteger(group, IncompleteText::VIS_DIR_INTEGER)) return AppendResult::APPENDED;
		if (appendVariable(group, IncompleteText::VIS_DIR_VAR_MAX_INT)) return AppendResult::APPENDED;
		if (appendFraction(group)) return AppendResult::APPENDED;
		if (appendVariableMeters(group)) return AppendResult::APPENDED;
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteText::VIS_DIR_INTEGER:
		if (appendFraction(group)) return AppendResult::APPENDED;
		if (appendVariable(group, IncompleteText::VIS_DIR_VAR_MAX_INT)) return AppendResult::APPENDED;
		incompleteText = IncompleteText::NONE;
		return AppendResult::NOT_APPENDED;

		case IncompleteText::VIS_DIR_VAR_MAX_INT:
		if (appendVariableMaxFraction(group)) return AppendResult::APPENDED;
		incompleteText = IncompleteText::NONE;
		return AppendResult::NOT_APPENDED;

		case IncompleteText::SFC_OR_TWR:
		if (group == "VIS") {
			incompleteText = IncompleteText::SFC_OR_TWR_VIS;
			return AppendResult::APPENDED;
		}
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteText::SFC_OR_TWR_VIS:
		if (appendInteger(group, IncompleteText::SFC_OR_TWR_VIS_INTEGER)) return AppendResult::APPENDED;
		if (appendFraction(group)) return AppendResult::APPENDED;
		if (appendMeters(group)) return AppendResult::APPENDED;
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteText::SFC_OR_TWR_VIS_INTEGER:
		if (appendFraction(group)) return AppendResult::APPENDED;
		if (appendVariable(group)) return AppendResult::GROUP_INVALIDATED;
		incompleteText = IncompleteText::NONE;
		return AppendResult::NOT_APPENDED;

		case IncompleteText::RVR:
		if (group == "MISG") {
			incompleteText = IncompleteText::NONE;
			return AppendResult::APPENDED;
		}
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteText::VISNO:
		if (appendRunway(group)) return AppendResult::APPENDED;
		if (appendDirection(group)) return AppendResult::APPENDED;
		incompleteText = IncompleteText::NONE;
		return AppendResult::NOT_APPENDED;
	}
}

std::optional<VisibilityGroup> VisibilityGroup::fromIncompleteInteger(
	const std::string & group)
{
	static const std::optional<VisibilityGroup> error;
	VisibilityGroup	result;
	if (!result.appendInteger(group)) return error;
	if (result.visibility().modifier() != Distance::Modifier::NONE) return error;
	result.incompleteText = IncompleteText::INTEGER;
	return result;
}

std::optional<VisibilityGroup> VisibilityGroup::fromMeters(
	const std::string & group)
{
	static const std::optional<VisibilityGroup> notRecognised;
	static const std::regex rgx("(\\d\\d\\d\\d|////)([NSWE][WED]?[V]?)?");
	static const auto matchVis = 1, matchDir = 2;
	std::smatch match;
	if (std::regex_match(group, match, rgx)) {
		const auto v = Distance::fromMeterString(match.str(matchVis));
		if (!v.has_value()) return notRecognised;
		const auto d = Direction::fromCardinalString(match.str(matchDir));
		VisibilityGroup result;
		result.vis = *v;
		result.dir = d;
		if (result.dir.has_value()) {
			if (result.dir->isValue()) result.visType = Type::DIRECTIONAL;
			if (result.dir->type() == Direction::Type::NDV) result.visType = Type::PREVAILING_NDV;
		}
		return result;
	}
	return notRecognised;
}

std::optional<VisibilityGroup> VisibilityGroup::fromRvr(const std::string & group) {
	static const std::optional<VisibilityGroup> notRecognised;
	static const std::regex rgx("(R\\d\\d[RCL]?)/(////|[PM]?\\d\\d\\d\\d)"
		"(?:V([PM]?\\d\\d\\d\\d))?(FT/?)?([UND/])?");
	static const auto matchRunway = 1, matchRvr = 2, matchVarRvr = 3, matchUnit = 4;
	static const auto matchTrend = 5;
	std::smatch match;
	if (!regex_match(group, match, rgx)) return notRecognised;
	const bool unitFeet = match.length(matchUnit);
	const auto runway = Runway::fromString(match.str(matchRunway));
	if (!runway.has_value()) return notRecognised;
	const auto rvr = Distance::fromRvrString(match.str(matchRvr), unitFeet);
	if (!rvr.has_value()) return notRecognised;
	VisibilityGroup result;
	result.visType = Type::RVR;
	result.rw = *runway;
	result.vis = *rvr;
	result.rvrTrend = trendFromString(match.str(matchTrend));
	if (match.length(matchVarRvr)) {
		const auto varRvr = Distance::fromRvrString(match.str(matchVarRvr), unitFeet);
		if (!varRvr.has_value()) return notRecognised;
		result.visType = Type::VARIABLE_RVR;
		result.visMax = *varRvr;
	}
	return result;
}

bool VisibilityGroup::appendFractionToIncompleteInteger(const std::string & group, 
	IncompleteText next)
{
	const auto v = Distance::fromMileString(group); 
	if (!v.has_value()) return false;
	const auto vAppended = Distance::fromIntegerAndFraction(visibility(), *v);
	if (!vAppended.has_value()) return false;
	vis = *vAppended;
	incompleteText = next;
	return true;
}


bool VisibilityGroup::appendDirection(const std::string & group, IncompleteText next) {
	if (const auto d = Direction::fromCardinalString(group); d.has_value()) {
		dir = d;
		if (visType == Type::PREVAILING) visType = Type::DIRECTIONAL;
		incompleteText = next;
		return true;
	}
	if (const auto d = Direction::fromSectorString(group); d.has_value()) {
		dirSecFrom = std::get<0>(*d);
		dirSecTo = std::get<1>(*d);
		if (visType == Type::PREVAILING) visType = Type::SECTOR;
		incompleteText = next;
		return true;
	}
	return false;
}

bool VisibilityGroup::appendRunway(const std::string & group, IncompleteText next) {
	const auto r = Runway::fromString(group, true);
	if (!r.has_value()) return false;
	rw = r;
	if (type() == Type::PREVAILING) visType = Type::RUNWAY;
	if (type() == Type::VARIABLE_PREVAILING) visType = Type::VARIABLE_RUNWAY;
	incompleteText = next;
	return true;
}

bool VisibilityGroup::appendInteger(const std::string & group, IncompleteText next) {
	if (group.empty() || group.length() > 3) return false;
	if (group.find('/') != std::string::npos) return false;
	if (vis.isReported()) return false;
	const auto v = Distance::fromMileString(group, true);
	if (!v.has_value()) return false;
	vis = *v;
	incompleteText = next;
	return true;
}

bool VisibilityGroup::appendFraction(const std::string & group, IncompleteText next) {
	if (group.find('/') == std::string::npos) return false;
	const auto v = Distance::fromMileString(group, true);
	if (!v.has_value()) return false;
	if (vis.isReported()) {
		const auto d = Distance::fromIntegerAndFraction(vis, std::move(*v));
		if (!d.has_value()) return false;
		vis = std::move(*d);
	} else {
		vis = std::move(*v);
	}
	incompleteText = next;
	return true;
}

bool VisibilityGroup::appendVariableMaxFraction(const std::string & group, IncompleteText next) {
	const auto fraction = fractionStrToUint(group, 0, group.length());
	if (!fraction.has_value()) return false;
	const auto numerator = std::get<0>(*fraction);
	const auto denominator = std::get<1>(*fraction);
	const auto v = Distance(numerator, denominator);
	if (!v.isValue()) return false;
	const auto t = Distance::fromIntegerAndFraction(visMax, v);
	if (!t.has_value()) return false;
	visMax = *t;
	incompleteText = next;
	return true;
}

bool VisibilityGroup::appendVariable(const std::string & group,
	IncompleteText nextIfMaxIsInteger,
	IncompleteText nextIfMaxIsFraction)
{
	const auto vPos = group.find('V');
	if (vPos == std::string::npos || 
		!vPos ||
		vPos == group.length()) return false;
	const bool maxInteger = (group.find('/', vPos) == std::string::npos);
	const auto min = 
		Distance::fromMileString(group.substr(0, vPos), true);
	if (!min.has_value()) return false;
	const auto max = 
		Distance::fromMileString(group.substr(vPos + 1), true);
	if (!max.has_value()) return false;

	if (vis.isReported()) {
		const auto v = Distance::fromIntegerAndFraction(vis, std::move(*min));
		if (!v.has_value()) return false;
		vis = std::move(*v);
	} else {
		vis = std::move(*min);
	}
	visMax = std::move(*max);
	makeVariable();
	incompleteText = maxInteger ? nextIfMaxIsInteger : nextIfMaxIsFraction;
	return true;
}

bool VisibilityGroup::appendMeters(const std::string & group, IncompleteText next) {
	const auto v = Distance::fromMeterString(group);
	if (!v.has_value()) return false;
	if (!v->isReported()) return false;
	vis = *v;
	incompleteText = next;
	return true;
}

bool VisibilityGroup::appendVariableMeters(const std::string & group, IncompleteText next) {
	static const std::regex rgx("(\\d\\d\\d\\d)V(\\d\\d\\d\\d)");
	static const auto matchMin = 1, matchMax = 2;
	std::smatch match;
	if (!std::regex_match(group, match, rgx)) return false;
	const auto min = Distance::fromMeterString(match.str(matchMin));
	if (!min.has_value()) return false;
	const auto max = Distance::fromMeterString(match.str(matchMax));
	if (!max.has_value()) return false;
	if (!min->isReported() || !max->isReported()) return false;
	vis = *min;
	visMax = *max;
	makeVariable();
	incompleteText = next;
	return true;
}

VisibilityGroup::Trend VisibilityGroup::trendFromString(const std::string & s) {
	if (s == "/") return Trend::NOT_REPORTED;
	if (s == "U") return Trend::UPWARD;
	if (s == "N") return Trend::NEUTRAL;
	if (s == "D") return Trend::DOWNWARD;
	return Trend::NONE;
}


bool VisibilityGroup::isValid() const {
	if (const auto max = visMax.toUnit(vis.unit()); max.has_value()) {
		const auto min = vis.toUnit(vis.unit());
		if (!min.has_value()) return false;
		if (*min > *max) return false;
	}
	if (incompleteText != IncompleteText::NONE) return false;
	if (dir.has_value() && !dir->isValid()) return false;
	if (rw.has_value() && !rw->isValid()) return false;
	return (vis.isValid() && visMax.isValid());
}

///////////////////////////////////////////////////////////////////////////

std::optional<CloudGroup> CloudGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	if (reportPart == ReportPart::METAR || reportPart == ReportPart::TAF)
		return parseCloudLayerOrVertVis(group);

	if (reportPart == ReportPart::RMK) {
		if (group == "CLD") return CloudGroup(Type::CLD_MISG, IncompleteText::CLD);
		if (group == "CIG") return CloudGroup(Type::CEILING, IncompleteText::CIG);
		if (group == "CHINO") return CloudGroup(Type::CHINO, IncompleteText::CHINO);
		if (const auto ct = CloudType::fromStringObscuration(group); ct.has_value()) {
			CloudGroup result = CloudGroup(Type::OBSCURATION, IncompleteText::OBSCURATION);
			result.cldTp = *ct;
			return result;
		}
		return parseVariableCloudLayer(group); 
	}
	return std::optional<CloudGroup>();
}

AppendResult CloudGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)reportPart;

	switch (incompleteText) {
		case IncompleteText::NONE:
		return AppendResult::NOT_APPENDED;

		case IncompleteText::RMK_AMOUNT:
		if (group != "V") return AppendResult::GROUP_INVALIDATED;
		incompleteText = IncompleteText::RMK_AMOUNT_V;
		return AppendResult::APPENDED;

		case IncompleteText::RMK_AMOUNT_V:
		return appendVariableCloudAmount(group);

		case IncompleteText::CIG:
		return appendCeiling(group);

		case IncompleteText::CIG_NUM:
		case IncompleteText::CHINO:
		return appendRunwayOrCardinalDirection(group);
		
		case IncompleteText::CLD:
		if (group != "MISG") return AppendResult::GROUP_INVALIDATED;
		incompleteText = IncompleteText::NONE;
		return AppendResult::APPENDED;

		case IncompleteText::OBSCURATION:
		return appendObscuration(group);
	}
}

std::optional<CloudGroup> CloudGroup::parseCloudLayerOrVertVis(const std::string & s) {
	static const std::optional<CloudGroup> notRecognised;
	// Attempt to parse 'no cloud' groups
	if (s == "NCD") return CloudGroup(Type::NO_CLOUDS, Amount::NCD);
	if (s == "NSC") return CloudGroup(Type::NO_CLOUDS, Amount::NSC);
	if (s == "CLR") return CloudGroup(Type::NO_CLOUDS, Amount::NONE_CLR);
	if (s == "SKC") return CloudGroup(Type::NO_CLOUDS, Amount::NONE_SKC);
	//Attempt to parse cloud layer or vertical visibility
	std::smatch match;
	static const std::regex rgx(
		"([A-Z][A-Z][A-Z]?|///)(\\d\\d\\d|///)([CT][BC][U]?|///)?");
	static const auto matchAmount = 1, matchHeight = 2, matchCnvType = 3;
	if (!std::regex_match(s, match, rgx)) return notRecognised;

	const auto amount = amountFromString(match.str(matchAmount));
	if (!amount.has_value()) return notRecognised;
	const auto height = Distance::fromHeightString(match.str(matchHeight));
	if (!height.has_value()) return notRecognised;
	const auto cnvtype = convectiveTypeFromString(match.str(matchCnvType));
	if (!cnvtype.has_value()) return notRecognised;

	// If vertical visibility is given, convective cloud type must not be specified
	if (*amount == Amount::OBSCURED && *cnvtype != ConvectiveType::NONE) 
		return notRecognised;

	CloudGroup result;
	result.tp = Type::CLOUD_LAYER;
	if (amount == Amount::OBSCURED) result.tp = Type::VERTICAL_VISIBILITY;
	result.amnt = *amount;
	result.heightOrVertVis = *height;
	result.convtype = *cnvtype;
	return result;
}

std::optional<CloudGroup> CloudGroup::parseVariableCloudLayer(const std::string & s) {
	static const std::optional<CloudGroup> notRecognised;

	std::smatch match;
	static const std::regex rgx("([A-Z][A-Z][A-Z])(\\d\\d\\d)?");
	static const auto matchAmount = 1, matchHeight = 2;
	if (!std::regex_match(s, match, rgx)) return notRecognised;

	CloudGroup result;
	result.tp = Type::CLOUD_LAYER;
	result.incompleteText = IncompleteText::RMK_AMOUNT;

	const auto amount = amountFromString(match.str(matchAmount));
	// Not checking for VV here because 3-char amount length guaranteed by regex
	if (!amount.has_value()) return notRecognised;
	result.amnt = *amount;

	if (const std::string heightStr = match.str(matchHeight); !heightStr.empty()) {
		const auto height = Distance::fromHeightString(heightStr);
		if (!height.has_value()) return notRecognised;
		result.heightOrVertVis = *height;
	}

	return result;
}

unsigned int CloudGroup::amountToMaxOkta(Amount a) {
	switch (a) {
		case Amount::FEW:
		return 2;

		case Amount::SCATTERED:
		case Amount::VARIABLE_FEW_SCATTERED:
		return 4;
		
		case Amount::VARIABLE_SCATTERED_BROKEN:
		case Amount::BROKEN:
		return 7;
		
		case Amount::VARIABLE_BROKEN_OVERCAST:
		case Amount::OVERCAST:
		case Amount::OBSCURED:
		return 8;

		default:
		return 0;
	}
}

CloudType::Type CloudGroup::convectiveTypeToCloudTypeType(ConvectiveType t) {
	switch (t) {
		case ConvectiveType::TOWERING_CUMULUS:
		return CloudType::Type::TOWERING_CUMULUS;
		
		case ConvectiveType::CUMULONIMBUS:
		return CloudType::Type::CUMULONIMBUS;

		default:
		return CloudType::Type::NOT_REPORTED;
	}
}

std::optional<CloudType> CloudGroup::cloudType() const {
	const auto cldConvType = convectiveTypeToCloudTypeType(convectiveType());
	const auto cldOkta = amountToMaxOkta(amount());
	switch (type()) {
		case Type::CLOUD_LAYER:
		case Type::CEILING:
		return CloudType(cldConvType, height(), cldOkta);

		case Type::VARIABLE_CEILING:
		return CloudType(cldConvType, minHeight(), cldOkta);

		case Type::OBSCURATION:
		return cldTp;

		default:
		return std::optional<CloudType>();
	}
}

std::optional<CloudGroup::Amount> CloudGroup::amountFromString(const std::string & s) {
	if (s == "FEW") return CloudGroup::Amount::FEW;
	if (s == "SCT") return CloudGroup::Amount::SCATTERED;
	if (s == "BKN") return CloudGroup::Amount::BROKEN;
	if (s == "OVC") return CloudGroup::Amount::OVERCAST;
	if (s == "VV") return CloudGroup::Amount::OBSCURED;
	if (s == "///") return CloudGroup::Amount::NOT_REPORTED;
	return std::optional<Amount>();
}

std::optional<CloudGroup::ConvectiveType> CloudGroup::convectiveTypeFromString(
	const std::string & s)
{
	if (s.empty()) return ConvectiveType::NONE;
	if (s == "TCU") return ConvectiveType::TOWERING_CUMULUS;
	if (s == "CB") return ConvectiveType::CUMULONIMBUS;
	if (s == "///") return ConvectiveType::NOT_REPORTED;
	return std::optional<ConvectiveType>();
}

AppendResult CloudGroup::appendVariableCloudAmount(const std::string & group) {
	const auto newAmount = amountFromString(group); 
	if (!newAmount.has_value()) return AppendResult::GROUP_INVALIDATED;
	const auto a1 = amount();
	const auto a2 = *newAmount;
	auto result = Amount::NOT_REPORTED;
	if (a1 == Amount::FEW && a2 == Amount::SCATTERED) result = Amount::VARIABLE_FEW_SCATTERED;
	if (a1 == Amount::SCATTERED && a2 == Amount::BROKEN) result = Amount::VARIABLE_SCATTERED_BROKEN;
	if (a1 == Amount::BROKEN && a2 == Amount::OVERCAST) result = Amount::VARIABLE_BROKEN_OVERCAST;
	if (result == Amount::NOT_REPORTED) return AppendResult::GROUP_INVALIDATED;
	amnt = result;
	incompleteText = IncompleteText::NONE;
	return AppendResult::APPENDED;
}

AppendResult CloudGroup::appendCeiling(const std::string & group) {
	if (const auto d = Distance::fromHeightString(group); d.has_value()) {
		if (!d->isReported()) return AppendResult::GROUP_INVALIDATED;
		heightOrVertVis = *d;
		incompleteText = IncompleteText::CIG_NUM;
		return AppendResult::APPENDED;
	}
	static const std::regex rgx ("(\\d\\d\\d)V(\\d\\d\\d)");
	static const auto matchMinHeight = 1, matchMaxHeight = 2;
	std::smatch match;
	if (!std::regex_match(group, match, rgx)) return AppendResult::GROUP_INVALIDATED;
	const auto minH = Distance::fromHeightString(match.str(matchMinHeight));
	if (!minH.has_value()) return AppendResult::GROUP_INVALIDATED;
	const auto maxH = Distance::fromHeightString(match.str(matchMaxHeight));
	if (!maxH.has_value()) return AppendResult::GROUP_INVALIDATED;
	heightOrVertVis = *minH;
	maxHt = *maxH;
	tp = Type::VARIABLE_CEILING;
	incompleteText = IncompleteText::CIG_NUM;
	return AppendResult::APPENDED;
}

AppendResult CloudGroup::appendRunwayOrCardinalDirection(const std::string & group) {
	incompleteText = IncompleteText::NONE;
	rw = Runway::fromString(group, true);
	if (rw.has_value()) return AppendResult::APPENDED;
	dir = Direction::fromCardinalString(group);
	if (dir.has_value()) return AppendResult::APPENDED;
	return AppendResult::NOT_APPENDED;
}

AppendResult CloudGroup::appendObscuration(const std::string & group) {
	static const std::regex rgx("([A-Z][A-Z][A-Z])(\\d\\d\\d)");
	static const auto matchAmount = 1, matchHeight = 2;
	std::smatch match;
	if (!std::regex_match(group, match, rgx)) return AppendResult::GROUP_INVALIDATED;

	const auto h = Distance::fromHeightString(match.str(matchHeight));
	if (!h.has_value()) return AppendResult::GROUP_INVALIDATED;

	const auto a = amountFromString(match.str(matchAmount));
	if (!a.has_value()) return AppendResult::GROUP_INVALIDATED;

	amnt = *a;
	heightOrVertVis = *h;
	cldTp = CloudType(cldTp.type(), *h, amountToMaxOkta(*a));
	incompleteText = IncompleteText::NONE;
	return AppendResult::APPENDED;
}

///////////////////////////////////////////////////////////////////////////

std::optional<WeatherGroup> WeatherGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	std::optional<WeatherGroup> notRecognised;
	if (reportPart == ReportPart::METAR || reportPart == ReportPart::TAF) {
		if (group == "NSW") return WeatherGroup(Type::NSW);
		if (const auto wp = parseWeatherWithoutEvent(group, reportPart); wp.has_value()) {
			WeatherGroup result;
			result.w[0] = *wp;
			result.wsz = 1;
			if (wp->qualifier() == WeatherPhenomena::Qualifier::RECENT) result.t = Type::RECENT;
			return result;
		}
	}
	if (reportPart == ReportPart::RMK) {
		if (group == "PWINO") return WeatherGroup(Type::PWINO);
		if (group == "TSNO") return WeatherGroup(Type::TSNO);
		if (group == "WX") return WeatherGroup(Type::WX_MISG, IncompleteText::WX);
		if (group == "TS/LTNG") return WeatherGroup(Type::TS_LTNG_TEMPO_UNAVBL, IncompleteText::TSLTNG);
		if (!reportMetadata.reportTime.has_value()) return notRecognised;
		return parseWeatherEvent(group, *reportMetadata.reportTime);
	}
	return notRecognised;
}

AppendResult WeatherGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	switch (incompleteText) {
		case IncompleteText::NONE:
		break;

		case IncompleteText::WX:
		if (group != "MISG") return AppendResult::GROUP_INVALIDATED;
		incompleteText = IncompleteText::NONE; 
		return AppendResult::APPENDED;

		case IncompleteText::TSLTNG:
		if (group != "TEMPO") return AppendResult::GROUP_INVALIDATED;
		incompleteText = IncompleteText::TSLTNG_TEMPO; 
		return AppendResult::APPENDED;

		case IncompleteText::TSLTNG_TEMPO:
		if (group != "UNAVBL") return AppendResult::GROUP_INVALIDATED;
		incompleteText = IncompleteText::NONE;
		return AppendResult::APPENDED;
	}

	if (type() != Type::CURRENT && type() != Type::RECENT)
		return AppendResult::NOT_APPENDED;

	const auto wp = parseWeatherWithoutEvent(group, reportPart);
	if (wp.has_value()) {
		// Recent weather cannot be appended to current weather and vice versa
		if (type() == Type::CURRENT && 
			wp->qualifier() == WeatherPhenomena::Qualifier::RECENT) return AppendResult::NOT_APPENDED;
		if (type() == Type::RECENT && 
			wp->qualifier() != WeatherPhenomena::Qualifier::RECENT) return AppendResult::NOT_APPENDED;

		if (!addWeatherPhenomena(*wp)) return AppendResult::NOT_APPENDED;
		return AppendResult::APPENDED;
	}
	return AppendResult::NOT_APPENDED;
}

inline std::vector<WeatherPhenomena> WeatherGroup::weatherPhenomena() const {
	std::vector<WeatherPhenomena> result;
	for (auto i=0u; i < wsz; i++) 
		result.push_back(w[i]);
	return result;
}

std::optional<WeatherPhenomena> WeatherGroup::parseWeatherWithoutEvent(
	const std::string & group, 
	ReportPart reportPart)
{
	std::optional<WeatherPhenomena> notRecognised;
	if (reportPart != ReportPart::METAR && reportPart != ReportPart::TAF) return notRecognised;
	// Not reported weather or recent weather is only allowed in METAR
	if (reportPart == ReportPart::METAR) {
		if (group == "RE//") return WeatherPhenomena::notReported(true);
		if (group == "//") return WeatherPhenomena::notReported(false);
	}
	const auto wp = WeatherPhenomena::fromString(group, true);
	if (!wp.has_value()) return notRecognised;
	// RECENT and VICINITY qualifiers are not allowed in TAF
	if (reportPart == ReportPart::TAF &&
		(wp->qualifier() == WeatherPhenomena::Qualifier::RECENT || 
		 wp->qualifier() == WeatherPhenomena::Qualifier::VICINITY)) return notRecognised; 
	return wp;
}

std::optional<WeatherGroup> WeatherGroup::parseWeatherEvent(const std::string & group, 
	const MetafTime & reportTime) 
{
	std::optional<WeatherGroup> notRecognised;
	int eventStartPos = 0;
	bool lastDigit = false;
	WeatherPhenomena previousWeather;
	WeatherGroup result;
	result.t = Type::EVENT;
	for (auto i = 0u; i < group.length(); i++) {
		// Split group into weather event strings
		// Assuming that weather event always has digits at the end
		const bool currDigit = (group[i] >= '0' && group[i] <= '9');
		if (!currDigit && lastDigit) {
			// i is the position after the last digit from eventStartPos
			const auto eventLen = i - eventStartPos;
			if (const auto minEventLen = 3; eventLen < minEventLen) return notRecognised;
			const std::string s = group.substr(eventStartPos, eventLen);
			const auto w = 
				WeatherPhenomena::fromWeatherBeginEndString(s, reportTime, previousWeather);
			if (!w.has_value()) return notRecognised;
			result.addWeatherPhenomena(*w);
			eventStartPos = i;
			previousWeather = *w;
		}
		lastDigit = currDigit;
	}
	// Last weather event in the string ends with a digit is not be detected in the loop  
	const std::string s = group.substr(eventStartPos);
	const auto w = 
		WeatherPhenomena::fromWeatherBeginEndString(s, reportTime, previousWeather);
	if (!w.has_value()) return notRecognised;
	result.addWeatherPhenomena(*w);
	return result;
}


bool WeatherGroup::addWeatherPhenomena(const WeatherPhenomena & wp) {
	if (wsz >= wSize) return false;
	w[wsz++] = wp;
	return true;
}

///////////////////////////////////////////////////////////////////////////

bool TemperatureGroup::isValid() const {
	if (isIncomplete) return false;
	// Either temperature or dew point not reported: always valid
	if (!airTemperature().temperature().has_value() ||
		!dewPoint().temperature().has_value()) return true;
	// If temperature reported M00 then dew point cannot be 00
	if (!*airTemperature().temperature() &&
		!*dewPoint().temperature() &&
		airTemperature().isFreezing() &&
		!dewPoint().isFreezing()) return false;
	// Generally dew point must be less or equal to temperature
	return (*airTemperature().temperature() >= *dewPoint().temperature());
}

std::optional<TemperatureGroup> TemperatureGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	static const std::optional<TemperatureGroup> notRecognised;
	static const std::regex rgx("(M?\\d\\d|//)/(M?\\d\\d|//)?");
	static const auto matchTemperature = 1, matchDewPoint = 2;
	static const std::regex rmkRgx("T([01]\\d\\d\\d)([01]\\d\\d\\d)?");
	static const auto rmkMatchTemperature = 1, rmkMatchDewPoint = 2;
	std::smatch match;
	if (reportPart == ReportPart::METAR) {
		if (regex_match(group, match, rgx)) {
			const auto t = Temperature::fromString(match.str(matchTemperature));
			if (!t.has_value()) return notRecognised;
			TemperatureGroup result(Type::TEMPERATURE_AND_DEW_POINT);
			result.t = *t;
			if (match.length(matchDewPoint)) {
				const auto dp = Temperature::fromString(match.str(matchDewPoint));
				if (!dp.has_value()) return notRecognised;
				result.dp = *dp;
			}
			return result;
		}
	}
	if (reportPart == ReportPart::RMK) {
		if (group == "T") return TemperatureGroup(Type::T_MISG, true);
		if (group == "TD") return TemperatureGroup(Type::TD_MISG, true);
		if (regex_match(group, match, rmkRgx)) {
			const auto t = Temperature::fromRemarkString(match.str(rmkMatchTemperature));
			if (!t.has_value()) return notRecognised;
			TemperatureGroup result;
			result.t = *t;
			if (match.length(matchDewPoint)) {
				const auto dp = Temperature::fromRemarkString(match.str(rmkMatchDewPoint));
				if (!dp.has_value()) return notRecognised;
				result.dp = *dp;
			}
			return result;
		}
	}
	return notRecognised;
}

AppendResult TemperatureGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)group; (void)reportPart;
	if (isIncomplete && (type() == Type::T_MISG || type() == Type::TD_MISG)) {
		if (group != "MISG") return AppendResult::GROUP_INVALIDATED;
		isIncomplete = false;
		return AppendResult::APPENDED;
	}
	return AppendResult::NOT_APPENDED;
}

///////////////////////////////////////////////////////////////////////////

std::optional<PressureGroup> PressureGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	static const std::optional<PressureGroup> notRecognised;
	if (reportPart == ReportPart::METAR || reportPart == ReportPart::RMK) {
		const auto pressure = Pressure::fromString(group);
		if (pressure.has_value()) {
			PressureGroup result;
			result.p = *pressure;
			result.t = Type::OBSERVED_QNH;
			return result;			
		}
		if (reportPart == ReportPart::METAR) return notRecognised;
	}
	if (reportPart == ReportPart::TAF) {
		const auto pressure = Pressure::fromForecastString(group);
		if (!pressure.has_value()) return notRecognised;
		PressureGroup result;
		result.p = *pressure;
		result.t = Type::FORECAST_LOWEST_QNH;
		return result;
	}
	if (reportPart == ReportPart::RMK) {
		if (group == "SLPNO") return PressureGroup(Type::SLPNO);
		if (group == "PRES") return PressureGroup(Type::PRES_MISG, true);
		if (const auto pr = Pressure::fromSlpString(group); pr.has_value()) {
			PressureGroup result;
			result.p = *pr;
			result.t = Type::OBSERVED_QNH;
			return result;
		}
		if (const auto pr = Pressure::fromQfeString(group); pr.has_value()) {
			PressureGroup result;
			result.p = *pr;
			result.t = Type::OBSERVED_QFE;
			return result;
		}
		return notRecognised;
	}
	return notRecognised;
}

AppendResult PressureGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)reportPart;
	if (isIncomplete && type() == Type::PRES_MISG) {
		if (group != "MISG") return AppendResult::GROUP_INVALIDATED;
		isIncomplete = false;
		return AppendResult::APPENDED;
	}
	return AppendResult::NOT_APPENDED;
}

///////////////////////////////////////////////////////////////////////////

std::optional<RunwayStateGroup> RunwayStateGroup::parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	static const std::optional<RunwayStateGroup> notRecognised;
	RunwayStateGroup result;
	if (reportPart != ReportPart::METAR) return notRecognised;
	if (group == "SNOCLO" || group == "R/SNOCLO") 
		return RunwayStateGroup(Type::AERODROME_SNOCLO, Runway::makeAllRunways());
	static const std::regex rgx("(R\\d\\d[RCL]?)/(?:"
		"(SNOCLO)|"
		"((\\d\\d)?D)|"
		"(?:([0-9/])([0-9/])(\\d\\d|//)|(CLRD))(\\d\\d|//))|");
	static const auto matchRunway = 1, matchSnoclo = 2;
	static const auto matchClrdAvc = 3, matchFrictionAvc = 4;
	static const auto matchDeposits = 5, matchExtent = 6, matchDepth = 7;
	static const auto matchClrd = 8, matchFriction = 9;
	static const std::string depthRunwayNotOperational = "99";
	std::smatch match;
	if (!regex_match(group, match, rgx)) return notRecognised;
	const auto runway = Runway::fromString(match.str(matchRunway));
	if (!runway.has_value()) return notRecognised;
	if (match.length(matchSnoclo)) 
		return RunwayStateGroup(Type::RUNWAY_SNOCLO, *runway);
	if (match.length(matchClrdAvc)) {
		auto fr = SurfaceFriction();
		if (match.length(matchFrictionAvc)) {
			const auto f = SurfaceFriction::fromString(match.str(matchFrictionAvc));
			if (!f.has_value()) return notRecognised;
			fr = *f;
		}
		return RunwayStateGroup(Type::RUNWAY_CLRD, *runway, fr);
	}
	const auto friction = SurfaceFriction::fromString(match.str(matchFriction));
	if (!friction.has_value()) return notRecognised;
	if (match.length(matchClrd)) 
		return RunwayStateGroup(Type::RUNWAY_CLRD, *runway, *friction);
	const auto deposits = depositsFromString(match.str(matchDeposits));
	if (!deposits.has_value()) return notRecognised;
	const auto extent = extentFromString(match.str(matchExtent));
	if (!extent.has_value()) return notRecognised;
	const auto depth = Precipitation::fromRunwayDeposits(match.str(matchDepth));
	if (!depth.has_value()) return notRecognised;
	result.tp = Type::RUNWAY_STATE;
	if (match.str(matchDepth) == depthRunwayNotOperational) {
		result.tp = Type::RUNWAY_NOT_OPERATIONAL;
	}
	result.rw = runway.value();
	result.dp = deposits.value();
	result.ext = extent.value();
	result.dDepth = depth.value();
	result.sf = friction.value();
	return result;
}

AppendResult RunwayStateGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)group; (void)reportPart;
	return AppendResult::NOT_APPENDED;
}

std::optional<RunwayStateGroup::Deposits> RunwayStateGroup::depositsFromString(
	const std::string & s)
{
	std::optional<Deposits> error;
	if (s.length() != 1) return error;
	switch (s[0]) {
		case '0': return Deposits::CLEAR_AND_DRY;
		case '1': return Deposits::DAMP;
		case '2': return Deposits::WET_AND_WATER_PATCHES;
		case '3': return Deposits::RIME_AND_FROST_COVERED;
		case '4': return Deposits::DRY_SNOW;
		case '5': return Deposits::WET_SNOW;
		case '6': return Deposits::SLUSH;
		case '7': return Deposits::ICE;
		case '8': return Deposits::COMPACTED_OR_ROLLED_SNOW;
		case '9': return Deposits::FROZEN_RUTS_OR_RIDGES;
		case '/': return Deposits::NOT_REPORTED;
		default:  return error;
	}
}

std::optional<RunwayStateGroup::Extent> RunwayStateGroup::extentFromString(
	const std::string & s)
{
	std::optional<Extent> error;
	if (s.length() != 1) return error;
	switch (s[0]) {
		case '0': return Extent::NONE;
		case '1': return Extent::LESS_THAN_10_PERCENT;
		case '2': return Extent::FROM_11_TO_25_PERCENT;
		case '3': return Extent::RESERVED_3;
		case '4': return Extent::RESERVED_4;
		case '5': return Extent::FROM_26_TO_50_PERCENT;
		case '6': return Extent::RESERVED_6;
		case '7': return Extent::RESERVED_7;
		case '8': return Extent::RESERVED_8;
		case '9': return Extent::MORE_THAN_51_PERCENT;
		case '/': return Extent::NOT_REPORTED;
		default:  return error;
	}
}

///////////////////////////////////////////////////////////////////////////////

std::optional<SeaSurfaceGroup> SeaSurfaceGroup::parse(const std::string & group,
		ReportPart reportPart,
		const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	static const std::optional<SeaSurfaceGroup> notRecognised;
	if (reportPart != ReportPart::METAR) return notRecognised;
	static const std::regex rgx ("W(\\d\\d|//)/([HS](?:\\d\\d?\\d?|///|/))");
	static const auto matchTemp = 1, matchWaveHeight = 2;
	std::smatch match;
	if (!std::regex_match(group, match, rgx)) return notRecognised;
	const auto temp = Temperature::fromString(match.str(matchTemp));
	if (!temp.has_value()) return notRecognised;
	const auto waveHeight = WaveHeight::fromString(match.str(matchWaveHeight));
	if (!waveHeight.has_value()) return notRecognised;
	SeaSurfaceGroup result;
	result.t = *temp;
	result.wh = *waveHeight;
	return result;
}

AppendResult SeaSurfaceGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)group; (void)reportPart;
	return AppendResult::NOT_APPENDED;
}

///////////////////////////////////////////////////////////////////////////////

std::optional<MinMaxTemperatureGroup> MinMaxTemperatureGroup::parse(
	const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	if (reportPart == ReportPart::TAF) return fromForecast(group);
	if (reportPart == ReportPart::RMK) {
		if (const auto h6 = from6hourly(group); h6.has_value()) return h6;
		if (const auto h24 = from24hourly(group); h24.has_value()) return h24;
	}
	return std::optional<MinMaxTemperatureGroup>();
}

AppendResult MinMaxTemperatureGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportPart; (void)reportMetadata;
	switch (type()) {
		case Type::OBSERVED_6_HOURLY: return append6hourly(group);
		case Type::OBSERVED_24_HOURLY: return AppendResult::NOT_APPENDED;
		case Type::FORECAST: return appendForecast(group);
	}
}

std::optional<MinMaxTemperatureGroup> MinMaxTemperatureGroup::from6hourly(
	const std::string & group)
{
	std::optional<MinMaxTemperatureGroup> notRecognised;
	static const std::regex rgx("([12])([01]\\d\\d\\d|////)");
	static const auto matchType = 1, matchValue = 2;
	std::smatch match;
	if (!std::regex_match(group, match, rgx)) return notRecognised;
	MinMaxTemperatureGroup result;
	result.t = Type::OBSERVED_6_HOURLY;
	if (match.str(matchValue) == "////") return result;
	const auto temp = Temperature::fromRemarkString(match.str(matchValue));
	if (!temp.has_value()) return notRecognised;
	const auto typeStr = match.str(matchType);
	if (typeStr == "1") { result.maxTemp = *temp; }
	if (typeStr == "2") { result.minTemp = *temp; }
	return result;
}

std::optional<MinMaxTemperatureGroup> MinMaxTemperatureGroup::from24hourly(
	const std::string & group)
{
	std::optional<MinMaxTemperatureGroup> notRecognised;
	static const std::regex rgx("4([01]\\d\\d\\d)([01]\\d\\d\\d)");
	static const auto matchMax = 1, matchMin = 2;
	std::smatch match;
	if (!std::regex_match(group, match, rgx)) return notRecognised;
	const auto max = Temperature::fromRemarkString(match.str(matchMax));
	if (!max.has_value()) return notRecognised;
	const auto min = Temperature::fromRemarkString(match.str(matchMin));
	if (!min.has_value()) return notRecognised;
	MinMaxTemperatureGroup result;
	result.t = Type::OBSERVED_24_HOURLY;
	result.minTemp = *min;
	result.maxTemp = *max;
	return result;
}

std::optional<MinMaxTemperatureGroup> MinMaxTemperatureGroup::fromForecast(
	const std::string & group)
{
	static const std::optional<MinMaxTemperatureGroup> notRecognised;
	static const std::regex rgx ("T([XN])?(M?\\d\\d)/(\\d\\d\\d\\d)Z");
	static const auto matchPoint = 1, matchTemperature = 2, matchTime = 3;
	std::smatch match;
	if (!std::regex_match(group, match, rgx)) return notRecognised;
	auto temp = Temperature::fromString(match.str(matchTemperature));
	if (!temp.has_value()) return notRecognised;
	auto time = MetafTime::fromStringDDHH(match.str(matchTime));
	if (!time.has_value()) return notRecognised;
	MinMaxTemperatureGroup result;
	result.t = Type::FORECAST;
	if (match.str(matchPoint) == "N") {
		result.minTemp = *temp;
		result.minTime = time;
	} 
	if (match.str(matchPoint) == "X") {
		result.maxTemp = *temp;
		result.maxTime = time;
	}
	if (!match.length(matchPoint)) {
		result.minTemp = *temp;
		result.minTime = time;
		result.maxTemp = *temp;
		result.maxTime = time;
		result.isIncomplete = true;
	}
	return result;
}

AppendResult MinMaxTemperatureGroup::append6hourly(const std::string & group) {
	static const auto error = AppendResult::NOT_APPENDED;
	if (minTemp.isReported() && maxTemp.isReported()) return error;
	const auto nextGroup = from6hourly(group);
	if (!nextGroup.has_value()) return error;
	if (minTemp.isReported() && nextGroup->minTemp.isReported()) return error;
	if (maxTemp.isReported() && nextGroup->maxTemp.isReported()) return error;
	if (!minTemp.isReported() && nextGroup->minTemp.isReported()) minTemp = nextGroup->minTemp;
	if (!maxTemp.isReported() && nextGroup->maxTemp.isReported()) maxTemp = nextGroup->maxTemp;
	return AppendResult::APPENDED;
}

AppendResult MinMaxTemperatureGroup::appendForecast(const std::string & group) {
	static const auto error = AppendResult::NOT_APPENDED;
	if (minTemp.isReported() && maxTemp.isReported() && !isIncomplete) return error;
	const auto nextGroup = fromForecast(group);
	if (!nextGroup.has_value()) return error;
	if (isIncomplete != nextGroup->isIncomplete) return error;
	if (!isIncomplete) {
		// Minimum or maximum temperature point specified explicitly
		// Get minimum either from *this or from nextGroup, and maximum from other group
		// Make sure appending two minimums or two maximums will result in error
		if (minTemp.isReported() && nextGroup->minTemp.isReported()) return error;
		if (maxTemp.isReported() && nextGroup->maxTemp.isReported()) return error;
		if (!minTemp.isReported() && nextGroup->minTemp.isReported()) {
			minTemp = nextGroup->minTemp;
			minTime = nextGroup->minTime;
		}
		if (!maxTemp.isReported() && nextGroup->maxTemp.isReported()) {
			maxTemp = nextGroup->maxTemp;
			maxTime = nextGroup->maxTime;
		}
		if (!minTemp.isReported() || !maxTemp.isReported()) return error;
	} else {
		maxTemp = nextGroup->maxTemp;
		maxTime = nextGroup->maxTime;
		if ((!minTemp.isFreezing() && maxTemp.isFreezing()) || 
			*minTemp.temperature() > *maxTemp.temperature())
		{
			std::swap(minTemp, maxTemp);
			std::swap(minTime, maxTime);
		}
		isIncomplete = false;
	}
	return AppendResult::APPENDED;
}

bool MinMaxTemperatureGroup::isValid() const {
	if (isIncomplete) return false;
	if (minimum().isReported() && 
		maximum().isReported() &&
		!minimum().isFreezing() && 
		maximum().isFreezing()) return false;
	if (!minimumTime().value_or(MetafTime()).isValid()) return false;
	if (!maximumTime().value_or(MetafTime()).isValid()) return false;
	return ((minimum().temperature().value_or(-100) < maximum().temperature().value_or(100)));
}

///////////////////////////////////////////////////////////////////////////////

std::optional<PrecipitationGroup> PrecipitationGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	std::optional<PrecipitationGroup> notRecognised;
	static const std::regex rgx(
		"([P67])(\\d\\d\\d\\d|////)|(4/|93[13]|I[136]|PP)(\\d\\d\\d|///)");
	static const auto matchType1 = 1, matchType2 = 3;
	static const auto matchValue1 = 2, matchValue2 = 4;

	static const std::regex rfRgx (
		"RF(\\d\\d\\.\\d|//\\./)/(\\d\\d\\d\\.\\d|///\\./)");
	static const auto rfMatchLast10Minutes = 1, rfMatchSince9AM = 2;

	std::smatch match;
	PrecipitationGroup result;

	if (reportPart == ReportPart::METAR) {
		if (!std::regex_match(group, match, rfRgx)) return notRecognised;
		const auto last10min = 
			Precipitation::fromRainfallString(match.str(rfMatchLast10Minutes));
		if (!last10min.has_value()) return notRecognised;
		const auto since9AM = 
			Precipitation::fromRainfallString(match.str(rfMatchSince9AM));
		if (!since9AM.has_value()) return notRecognised;
		result.precType = Type::RAINFALL_9AM_10MIN;
		result.precAmount = *since9AM;
		result.precChange = *last10min;
		return result;
	}

	if (reportPart != ReportPart::RMK) return notRecognised;

	if (group == "PNO") return PrecipitationGroup(Type::PNO);
	if (group == "FZRANO") return PrecipitationGroup(Type::FZRANO);
	if (group == "ICG") return PrecipitationGroup(Type::ICG_MISG, true);
	if (group == "PCPN") return PrecipitationGroup(Type::PCPN_MISG, true);
	if (group == "SNINCR") return PrecipitationGroup(Type::SNOW_INCREASING_RAPIDLY);

	if (!std::regex_match(group, match, rgx)) return notRecognised;

	// assuming only one pair matchType and matchValue will be non-empty
	std::string typeStr = match.str(matchType1) + match.str(matchType2);
	std::string valueStr = match.str(matchValue1) + match.str(matchValue2);

	const bool is3hourly =
		reportMetadata.reportTime.has_value() ?
		reportMetadata.reportTime->is3hourlyReportTime() :
		false;
	const bool is6hourly =
		reportMetadata.reportTime.has_value() ?
		reportMetadata.reportTime->is6hourlyReportTime() :
		false;

	if (typeStr == "P") result.precType = Type::TOTAL_PRECIPITATION_HOURLY;
	if (typeStr == "4/") result.precType = Type::SNOW_DEPTH_ON_GROUND;
	if (typeStr == "6") {
		result.precType = Type::FROZEN_PRECIP_3_OR_6_HOURLY;
		if (is3hourly) result.precType = Type::FROZEN_PRECIP_3_HOURLY;
		if (is6hourly) result.precType = Type::FROZEN_PRECIP_6_HOURLY;
	}
	if (typeStr == "7") result.precType = Type::FROZEN_PRECIP_24_HOURLY;
	if (typeStr == "931") result.precType = Type::SNOW_6_HOURLY;
	if (typeStr == "933") result.precType = Type::WATER_EQUIV_OF_SNOW_ON_GROUND;
	if (typeStr == "I1") result.precType = Type::ICE_ACCRETION_FOR_LAST_HOUR;
	if (typeStr == "I3") result.precType = Type::ICE_ACCRETION_FOR_LAST_3_HOURS;
	if (typeStr == "I6") result.precType = Type::ICE_ACCRETION_FOR_LAST_6_HOURS;
	if (typeStr == "PP") result.precType = Type::PRECIPITATION_ACCUMULATION_SINCE_LAST_REPORT;

	const auto amount = Precipitation::fromRemarkString(valueStr,
			factorFromType(result.precType),
			unitFromType(result.precType),
			true);
	if (!amount.has_value()) return notRecognised;

	result.precAmount = *amount;
	return result;
}

AppendResult PrecipitationGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)reportPart;
	if (type() == Type::PCPN_MISG || type() == Type::ICG_MISG) {
		if (!isIncomplete) return AppendResult::NOT_APPENDED;
		if (group != "MISG") return AppendResult::GROUP_INVALIDATED;
		isIncomplete = false;
		return AppendResult::APPENDED;
	}
	if (type() != Type::SNOW_INCREASING_RAPIDLY) return AppendResult::NOT_APPENDED;
	if (total().isReported() || recent().isReported())
		return AppendResult::NOT_APPENDED;
	const auto precip = Precipitation::fromSnincrString(group);
	if (!precip.has_value()) return AppendResult::NOT_APPENDED;
	precChange = std::get<0>(*precip);
	precAmount = std::get<1>(*precip);
	return AppendResult::APPENDED;
}

Precipitation::Unit PrecipitationGroup::unitFromType(Type type) {
	switch(type) {
		case Type::PRECIPITATION_ACCUMULATION_SINCE_LAST_REPORT:
		return Precipitation::Unit::MM;

		default:
		return Precipitation::Unit::INCHES;		
	}	
}

float PrecipitationGroup::factorFromType(Type type) {
	switch(type) {
		case Type::WATER_EQUIV_OF_SNOW_ON_GROUND:
		case Type::SNOW_6_HOURLY:
		case Type::PRECIPITATION_ACCUMULATION_SINCE_LAST_REPORT:
		return 0.1;

		case Type::TOTAL_PRECIPITATION_HOURLY:
		case Type::FROZEN_PRECIP_3_OR_6_HOURLY:
		case Type::FROZEN_PRECIP_3_HOURLY:
		case Type::FROZEN_PRECIP_6_HOURLY:
		case Type::FROZEN_PRECIP_24_HOURLY:
		case Type::ICE_ACCRETION_FOR_LAST_HOUR:
		case Type::ICE_ACCRETION_FOR_LAST_3_HOURS:
		case Type::ICE_ACCRETION_FOR_LAST_6_HOURS:
		return 0.01;

		default:
		return 1;
	}
}

///////////////////////////////////////////////////////////////////////////////

std::optional<LayerForecastGroup::Type> LayerForecastGroup::typeFromStr(
	const std::string & s)
{
	if (s == "60") return Type::ICING_TRACE_OR_NONE;
	if (s == "61") return Type::ICING_LIGHT_MIXED;
	if (s == "62") return Type::ICING_LIGHT_RIME_IN_CLOUD;
	if (s == "63") return Type::ICING_LIGHT_CLEAR_IN_PRECIPITATION;
	if (s == "64") return Type::ICING_MODERATE_MIXED;
	if (s == "65") return Type::ICING_MODERATE_RIME_IN_CLOUD;
	if (s == "66") return Type::ICING_MODERATE_CLEAR_IN_PRECIPITATION;
	if (s == "67") return Type::ICING_SEVERE_MIXED;
	if (s == "68") return Type::ICING_SEVERE_RIME_IN_CLOUD;
	if (s == "69") return Type::ICING_SEVERE_CLEAR_IN_PRECIPITATION;
	if (s == "50") return Type::TURBULENCE_NONE;
	if (s == "51") return Type::TURBULENCE_LIGHT;
	if (s == "52") return Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASIONAL;
	if (s == "53") return Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT;
	if (s == "54") return Type::TURBULENCE_MODERATE_IN_CLOUD_OCCASIONAL;
	if (s == "55") return Type::TURBULENCE_MODERATE_IN_CLOUD_FREQUENT;
	if (s == "56") return Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASIONAL;
	if (s == "57") return Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT;
	if (s == "58") return Type::TURBULENCE_SEVERE_IN_CLOUD_OCCASIONAL;
	if (s == "59") return Type::TURBULENCE_SEVERE_IN_CLOUD_FREQUENT;
	if (s == "5X") return Type::TURBULENCE_EXTREME;
	return std::optional<Type>();
}

std::optional<LayerForecastGroup> LayerForecastGroup::parse(
	const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	std::optional<LayerForecastGroup> notRecognised;
	static const std::regex rgx("([65][\\dX])(\\d\\d\\d\\d)");
	static const auto matchType = 1, matchHeight = 2;

	if (reportPart != ReportPart::TAF) return notRecognised;
	std::smatch match;
	if (!std::regex_match(group, match, rgx)) return notRecognised;
	const auto type = typeFromStr(match.str(matchType));
	if (!type.has_value()) return notRecognised;
	const auto heights = Distance::fromLayerString(match.str(matchHeight));
	if (!heights.has_value()) return notRecognised;

	LayerForecastGroup result;
	result.layerType = *type;
	result.layerBaseHeight = std::get<0>(*heights);
	result.layerTopHeight = std::get<1>(*heights);
	return result;
}

AppendResult LayerForecastGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)group; (void)reportPart;
	return AppendResult::NOT_APPENDED;
}

///////////////////////////////////////////////////////////////////////////////

PressureTendencyGroup::Trend PressureTendencyGroup::trend(Type type) {
	switch(type) {
		case Type::INCREASING_MORE_SLOWLY:
		case Type::INCREASING:
		case Type::INCREASING_MORE_RAPIDLY:
		return Trend::HIGHER;

		case Type::INCREASING_THEN_DECREASING:
		return Trend::HIGHER_OR_SAME;

		case Type::STEADY:
		return Trend::SAME;

		case Type::DECREASING_THEN_INCREASING:
		return Trend::LOWER_OR_SAME;

		case Type::DECREASING_MORE_SLOWLY:
		case Type::DECREASING:
		case Type::DECREASING_MORE_RAPIDLY:
		return Trend::LOWER;

		case Type::NOT_REPORTED:
		case Type::RISING_RAPIDLY:
		case Type::FALLING_RAPIDLY:
		return Trend::NOT_REPORTED;
	}
}

std::optional<PressureTendencyGroup::Type> PressureTendencyGroup::typeFromChar(
	char type)
{
	switch (type) {
		case '0': return Type::INCREASING_THEN_DECREASING;
		case '1': return Type::INCREASING_MORE_SLOWLY;
		case '2': return Type::INCREASING;
		case '3': return Type::INCREASING_MORE_RAPIDLY;
		case '4': return Type::STEADY;
		case '5': return Type::DECREASING_THEN_INCREASING;
		case '6': return Type::DECREASING_MORE_SLOWLY;
		case '7': return Type::DECREASING;
		case '8': return Type::DECREASING_MORE_RAPIDLY;
		case '/': return Type::NOT_REPORTED;
		default: return std::optional<Type>();
	}
}

std::optional<PressureTendencyGroup> PressureTendencyGroup::parse(
	const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	std::optional<PressureTendencyGroup> notRecognised;

	if (reportPart != ReportPart::RMK) return notRecognised;

	if (group == "PRESRR") {
		PressureTendencyGroup result;
		result.tendencyType = Type::RISING_RAPIDLY;
		return result;
	}
	if (group == "PRESFR") {
		PressureTendencyGroup result;
		result.tendencyType = Type::FALLING_RAPIDLY;
		return result;
	}

	static const std::regex rgx("5([\\d/])(\\d\\d\\d|///)");
	static const auto matchType = 1, matchPressure = 2;

	std::smatch match;
	if (!std::regex_match(group, match, rgx)) return notRecognised;
	const auto type = typeFromChar(match.str(matchType)[0]);
	if (!type.has_value()) return notRecognised;
	const auto pressure = Pressure::fromTendencyString(match.str(matchPressure));
	if (!pressure.has_value()) return notRecognised;

	PressureTendencyGroup result;
	result.tendencyType = *type;
	result.pressureDifference = *pressure;
	return result;
}

AppendResult PressureTendencyGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)group; (void)reportPart;
	return AppendResult::NOT_APPENDED;
}

///////////////////////////////////////////////////////////////////////////////

std::vector<CloudType> CloudTypesGroup::cloudTypes() const {
	std::vector<CloudType> result;
	for (auto i=0u; i < cldTpSize; i++)
		result.push_back(cldTp[i]);
	return result;
}

std::optional<CloudTypesGroup> CloudTypesGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	std::optional<CloudTypesGroup> notRecognised;
	//"(CB|TCU|CU|CF|SC|NS|ST|SF|AS|AC|ACC|CI|CS|CC|BLSN|BLDU|BLSA|IC|)(\\d)"
	static const std::regex matchRgx("(?:(?:[A-Z]{2,4})[\\d])+");
	static const std::regex searchRgx("[A-Z]{2,4}[\\d]");

	if (reportPart != ReportPart::RMK) return notRecognised;
	std::smatch match;
	CloudTypesGroup result;
	
	if (const auto ctp = CloudType::fromString(group); ctp.has_value()) {
		result.cldTp[0] = *ctp;
		result.cldTpSize = 1;
		return result;
	}
	if (!std::regex_match(group, match, matchRgx)) return notRecognised;
	auto iter = std::sregex_iterator(group.begin(), group.end(), searchRgx);
	while (iter != std::sregex_iterator()) {
		if (result.cldTpSize >= result.cldTpMaxSize) return result;
		match = *iter++;
		const auto ctp = CloudType::fromString(match.str(0));
		if (!ctp.has_value()) return notRecognised;
		result.cldTp[result.cldTpSize++] = *ctp;
	}
	return result;
}

AppendResult CloudTypesGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)group; (void)reportPart;
	if (cldTpSize >= cldTpMaxSize) return AppendResult::NOT_APPENDED;
	if (!cldTp[cldTpSize - 1].height().isReported()) return AppendResult::NOT_APPENDED;
	const auto ctp = CloudType::fromString(group);
	if (!ctp.has_value()) return AppendResult::NOT_APPENDED;
	if (!ctp->height().isReported()) return AppendResult::NOT_APPENDED;
	cldTp[cldTpSize++] = *ctp;
	return AppendResult::APPENDED;
}

bool CloudTypesGroup::isValid() const {
	for (auto i=0u; i < cldTpSize; i++)
		if (!cldTp[i].isValid()) return false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////

bool LowMidHighCloudGroup::isValid() const {
	if (lowLayer() == LowLayer::NOT_OBSERVABLE &&
		midLayer() != MidLayer::NOT_OBSERVABLE) return false;
	if (midLayer() == MidLayer::NOT_OBSERVABLE &&
		highLayer() != HighLayer::NOT_OBSERVABLE) return false;
	return true;
}


std::optional<LowMidHighCloudGroup> LowMidHighCloudGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	std::optional<LowMidHighCloudGroup> notRecognised;
	static const std::regex rgx("8/([0-9/])([0-9/])([0-9/])");
	static const auto matchLowLayer = 1, matchMidLayer = 2, matchHighLayer = 3;

	if (reportPart != ReportPart::RMK) return notRecognised;
	std::smatch match;
	if (!std::regex_match(group, match, rgx)) return notRecognised;

	const auto lowLayer = lowLayerFromChar(match.str(matchLowLayer)[0]);
	const auto midLayer = midLayerFromChar(match.str(matchMidLayer)[0]);
	const auto highLayer = highLayerFromChar(match.str(matchHighLayer)[0]);
	LowMidHighCloudGroup result;
	result.cloudLowLayer = lowLayer;
	result.cloudMidLayer = midLayer;
	result.cloudHighLayer = highLayer;
	return result;
}

AppendResult LowMidHighCloudGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)group; (void)reportPart;
	return AppendResult::NOT_APPENDED;
}

LowMidHighCloudGroup::LowLayer LowMidHighCloudGroup::lowLayerFromChar(char c) {
	switch (c) {
		case '0': return LowLayer::NONE;
		case '1': return LowLayer::CU_HU_CU_FR;
		case '2': return LowLayer::CU_MED_CU_CON;
		case '3': return LowLayer::CB_CAL;
		case '4': return LowLayer::SC_CUGEN;
		case '5': return LowLayer::SC_NON_CUGEN;
		case '6': return LowLayer::ST_NEB_ST_FR;
		case '7': return LowLayer::ST_FR_CU_FR_PANNUS;
		case '8': return LowLayer::CU_SC_NON_CUGEN_DIFFERENT_LEVELS;
		case '9': return LowLayer::CB_CAP;
		default: return LowLayer::NOT_OBSERVABLE;
	}
}

LowMidHighCloudGroup::MidLayer LowMidHighCloudGroup::midLayerFromChar(char c) {
	switch (c) {
		case '0': return MidLayer::NONE;
		case '1': return MidLayer::AS_TR;
		case '2': return MidLayer::AS_OP_NS;
		case '3': return MidLayer::AC_TR;
		case '4': return MidLayer::AC_TR_LEN_PATCHES;
		case '5': return MidLayer::AC_TR_AC_OP_SPREADING;
		case '6': return MidLayer::AC_CUGEN_AC_CBGEN;
		case '7': return MidLayer::AC_DU_AC_OP_AC_WITH_AS_OR_NS;
		case '8': return MidLayer::AC_CAS_AC_FLO;
		case '9': return MidLayer::AC_OF_CHAOTIC_SKY;
		default: return MidLayer::NOT_OBSERVABLE;
	}
}

LowMidHighCloudGroup::HighLayer LowMidHighCloudGroup::highLayerFromChar(char c) {
	switch (c) {
		case '0': return HighLayer::NONE;
		case '1': return HighLayer::CI_FIB_CI_UNC;
		case '2': return HighLayer::CI_SPI_CI_CAS_CI_FLO;
		case '3': return HighLayer::CI_SPI_CBGEN;
		case '4': return HighLayer::CI_FIB_CI_UNC_SPREADING;
		case '5': return HighLayer::CI_CS_LOW_ABOVE_HORIZON;
		case '6': return HighLayer::CI_CS_HIGH_ABOVE_HORIZON;
		case '7': return HighLayer::CS_NEB_CS_FIB_COVERING_ENTIRE_SKY;
		case '8': return HighLayer::CS;
		case '9': return HighLayer::CC;
		default: return HighLayer::NOT_OBSERVABLE;
	}
}

///////////////////////////////////////////////////////////////////////////////

std::optional<LightningGroup> LightningGroup::parse(
	const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void) reportMetadata;
	if (reportPart != ReportPart::RMK) return std::optional<LightningGroup>();
	if (group == "OCNL") return LightningGroup(Frequency::OCCASIONAL);
	if (group == "FRQ") return LightningGroup(Frequency::FREQUENT);
	if (group == "CONS") return LightningGroup(Frequency::CONSTANT);
	return fromLtgGroup(group);
}

AppendResult LightningGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportPart; (void)reportMetadata;
	if (incomplete) {
		//Frequency was specified, now expecting LTG group
		if (const auto ltg = fromLtgGroup(group); ltg.has_value())
		{
			Frequency f = freq;
			*this = std::move(*ltg);
			freq = f;
			return AppendResult::APPENDED;
		}
		return AppendResult::GROUP_INVALIDATED;
	}

	if (!isOmittedDir2()) return AppendResult::NOT_APPENDED;
	if (!isOmittedDir1()) {
		// First direction sector is already appended, try to append second one 
		if (group == "AND") return AppendResult::APPENDED;
		if (const auto dir = Direction::fromCardinalString(group, true); dir.has_value()) {
			//Single direction is specified
			dir2from = *dir;
			return AppendResult::APPENDED;
		}
		if (const auto dirSec = Direction::fromSectorString(group); dirSec.has_value())
		{
			//Direction sector is specified
			dir2from = std::get<0>(*dirSec);
			dir2to = std::get<1>(*dirSec);
			return AppendResult::APPENDED;
		}
	return AppendResult::NOT_APPENDED;
	}

	// No direction sector was previously appended to group
	if (group == "DSNT") { dist = Distance::makeDistant(); return AppendResult::APPENDED; }
	if (group == "VC") { dist = Distance::makeVicinity(); return AppendResult::APPENDED; }
	if (const auto dir = Direction::fromCardinalString(group, true); dir.has_value()) {
		//Single direction is specified
		dir1from = *dir;
		return AppendResult::APPENDED;
	}
	if (const auto dirSec = Direction::fromSectorString(group); dirSec.has_value())
	{
		dir1from = std::get<0>(*dirSec);
		dir1to = std::get<1>(*dirSec);
		return AppendResult::APPENDED;
	}
	return AppendResult::NOT_APPENDED;
}

std::optional<LightningGroup> LightningGroup::fromLtgGroup(const std::string & group) {
	std::optional<LightningGroup> notRecognised;
	static const auto ltgLen = 3u; // length of string LTG
	static const char ltg[ltgLen + 1] = "LTG";

	LightningGroup result;
	if (group == ltg) return (result);

	if (group.length() < ltgLen) return notRecognised;
	if (group.substr(0, ltgLen) != ltg) return notRecognised;

	auto currPos = ltgLen;
	static const auto typeLen = 2;
	static const auto maxLightningTypeCount = 
		6; // arbitrary number greater than 4; well formed group can have max 4 types
	auto lightningTypeCount = 0;
	while(currPos < group.length() && lightningTypeCount < maxLightningTypeCount) {
		const auto currType = group.substr(currPos, typeLen);
		currPos += typeLen;
		lightningTypeCount++;
		if (currType == "IC") { result.typeInCloud = true; continue; }
		if (currType == "CC") { result.typeCloudCloud = true; continue; }
		if (currType == "CG") { result.typeCloudGround = true; continue; }
		if (currType == "CA") { result.typeCloudAir = true; continue; }
		result.typeUnknown = true;
	}
	return result;
}

std::vector<Direction> LightningGroup::directions() const {
	// The result vector is max 10 elements possible, typically up to 
	// 5 elements which does not justify using std::set and std::find
	auto result = Direction::sectorCardinalDirToVector(
		dir1from.value_or(Direction()), 
		dir1to.value_or(Direction()));
	const auto result2 = Direction::sectorCardinalDirToVector(
		dir2from.value_or(Direction()), 
		dir2to.value_or(Direction()));
	for (const auto r2 : result2) {
		// Check if r2 is already present in result
		bool r2_alreadyPresent = false;
		for (const auto r : result)
			if (r.cardinal() == r2.cardinal()) { r2_alreadyPresent = true; break;}
		if (!r2_alreadyPresent) result.push_back(r2);
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////

std::optional<VicinityGroup> VicinityGroup::parse(
	const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void) reportMetadata;
	std::optional<VicinityGroup> notRecognised;
	if (reportPart != ReportPart::RMK) return notRecognised;
	if (group == "TS") return VicinityGroup(Type::THUNDERSTORM);
	if (group == "CB") return VicinityGroup(Type::CUMULONIMBUS);
	if (group == "CBMAM") return VicinityGroup(Type::CUMULONIMBUS_MAMMATUS);
	if (group == "ACC") return VicinityGroup(Type::ALTOCUMULUS_CASTELLANUS);
	if (group == "TCU") return VicinityGroup(Type::TOWERING_CUMULUS);
	if (group == "SCSL") return VicinityGroup(Type::STRATOCUMULUS_STANDING_LENTICULAR);
	if (group == "ACSL") return VicinityGroup(Type::ALTOCUMULUS_STANDING_LENTICULAR);
	if (group == "CCSL") return VicinityGroup(Type::CIRROCUMULUS_STANDING_LENTICULAR);
	if (group == "ROTOR") return VicinityGroup(Type::ROTOR_CLOUD);
	if (group == "VIRGA") return VicinityGroup(Type::VIRGA);
	if (group == "VCSH") return VicinityGroup(Type::PRECIPITATION_IN_VICINITY);
	if (group == "FOG" || group == "FG") return VicinityGroup(Type::FOG);
	if (group == "HAZE" || group == "HZ") return VicinityGroup(Type::HAZE);
	if (group == "SMOKE" || group == "FU") return VicinityGroup(Type::SMOKE);
	if (group == "BLSN") return VicinityGroup(Type::BLOWING_SNOW);
	if (group == "BLDU") return VicinityGroup(Type::BLOWING_DUST);
	if (group == "BLSA") return VicinityGroup(Type::BLOWING_SAND);
	if (group == "MIFG") return VicinityGroup(Type::FOG_SHALLOW);
	if (group == "BCFG") return VicinityGroup(Type::FOG_PATCHES);
	return notRecognised;
}

AppendResult VicinityGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportPart; (void)reportMetadata;
	switch(incompleteType) {
		case IncompleteType::NONE:
		//Group is complete, nothing more to append
		return AppendResult::NOT_APPENDED; 

		case IncompleteType::EXPECT_CLD:
		if (group == "CLD") return expectNext(IncompleteType::EXPECT_DIST_DIR1);
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteType::EXPECT_DIST_DIR1:
		if (appendDistance(group)) return expectNext(IncompleteType::EXPECT_DIR1);
		if (appendDir1(group)) return expectNext(IncompleteType::EXPECT_DIR2_MOV);
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteType::EXPECT_DIR1:
		if (appendDir1(group)) return expectNext(IncompleteType::EXPECT_DIR2_MOV);
		return rejectGroup();

		case IncompleteType::EXPECT_DIR2_MOV:
		if (group == "MOV") return expectNext(IncompleteType::EXPECT_MOVDIR);
		if (group == "AND") return expectNext(IncompleteType::EXPECT_DIR2);
		if (appendDir2(group)) return expectNext(IncompleteType::EXPECT_MOV);
		return rejectGroup();

		case IncompleteType::EXPECT_DIR2:
		if (appendDir2(group)) return expectNext(IncompleteType::EXPECT_MOV);
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteType::EXPECT_MOV:
		if (group == "MOV") return expectNext(IncompleteType::EXPECT_MOVDIR);
		return rejectGroup();

		case IncompleteType::EXPECT_MOVDIR:
		if (const auto dir = Direction::fromCardinalString(group, false, true);
			dir.has_value()) 
		{
			movDir = *dir;
			return finalise();
		}
		return AppendResult::GROUP_INVALIDATED;
	}
}

bool VicinityGroup::appendDir1(const std::string & str) {
	if (const auto dir = Direction::fromCardinalString(str, true); dir.has_value()) {
		//Single direction is specified
		dir1from = *dir;
		return true;
	}
	if (const auto dirSec = Direction::fromSectorString(str); dirSec.has_value())
	{
		dir1from = std::get<0>(*dirSec);
		dir1to = std::get<1>(*dirSec);
		return true;
	}
	return false;
}

bool VicinityGroup::appendDir2(const std::string & str) {
	if (const auto dir = Direction::fromCardinalString(str, true); dir.has_value()) {
		//Single direction is specified
		dir2from = *dir;
		return true;
	}
	if (const auto dirSec = Direction::fromSectorString(str); dirSec.has_value())
	{
		dir2from = std::get<0>(*dirSec);
		dir2to = std::get<1>(*dirSec);
		return true;
	}
	return false;
}

bool VicinityGroup::appendDistance(const std::string & str) {
	if (str == "DSNT") { dist = Distance::makeDistant(); return true; }
	if (str == "VC") { dist = Distance::makeVicinity(); return true; }
	const auto d = Distance::fromKmString(str);
	if (!d.has_value()) return false;
	dist = *d;
	return true;
}

std::vector<Direction> VicinityGroup::directions() const {
	// Copy of LightningGroup::directions
	auto result = Direction::sectorCardinalDirToVector(
		dir1from.value_or(Direction()), 
		dir1to.value_or(Direction()));
	const auto result2 = Direction::sectorCardinalDirToVector(
		dir2from.value_or(Direction()), 
		dir2to.value_or(Direction()));
	for (const auto r2 : result2) {
		// Check if r2 is already present in result
		bool r2_alreadyPresent = false;
		for (const auto r : result)
			if (r.cardinal() == r2.cardinal()) { r2_alreadyPresent = true; break;}
		if (!r2_alreadyPresent) result.push_back(r2);
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////

std::optional<MiscGroup> MiscGroup::parse(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata;
	static const std::regex rgxSunshineDuration("98(\\d\\d\\d)");
	static const std::regex rgxCorrectionObservation("CC([A-Z])");
	static const auto matchValue = 1;

	std::smatch match;
	MiscGroup result;

	if (reportPart == ReportPart::METAR) {
		if (const auto c = parseColourCode(group); c.has_value()) {
			result.groupType = *c;
			return result;
		}

		if (std::regex_match(group, match, rgxCorrectionObservation)) {
			result.groupType = Type::CORRECTED_WEATHER_OBSERVATION;
			result.groupData = match.str(matchValue)[0] - 'A' + 1;
			return result;
		}
	}

	if (reportPart == ReportPart::RMK) {
		if (group == "GR") {
			result.groupType = Type::HAILSTONE_SIZE;
			result.incompleteText = IncompleteText::GR;
			return result;
		}
		if (group == "DENSITY") {
			result.groupType = Type::DENSITY_ALTITUDE;
			result.incompleteText = IncompleteText::DENSITY;
			return result;
		}
		if (std::regex_match(group, match, rgxSunshineDuration)) {
			result.groupType = Type::SUNSHINE_DURATION_MINUTES;
			result.groupData = std::stoi(match.str(matchValue));
			return result;
		}
		if (group == "FROIN") {
			result.groupType = Type::FROIN;
			return result;
		}
	}

	return std::optional<MiscGroup>();
}

AppendResult MiscGroup::append(const std::string & group,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata)
{
	(void)reportMetadata; (void)reportPart;
	switch(incompleteText){
		case IncompleteText::NONE:
		return AppendResult::NOT_APPENDED;

		case IncompleteText::DENSITY:
		if (group == "ALT") {
			incompleteText = IncompleteText::DENSITY_ALT;
			return AppendResult::APPENDED;
		}
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteText::DENSITY_ALT:
		if (group == "MISG") {
			incompleteText = IncompleteText::NONE;
			return AppendResult::APPENDED;
		}
		if (appendDensityAltitude(group)) return AppendResult::APPENDED;
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteText::GR:
		if (group.length() == 1 && group[0] >= '1' && group[0] <= '9') {
			groupData = group[0] - '0';
			incompleteText = IncompleteText::GR_INT;
			return AppendResult::APPENDED;
		}
		if (appendHailstoneFraction(group)) return AppendResult::APPENDED;
		return AppendResult::GROUP_INVALIDATED;

		case IncompleteText::GR_INT:
		if (appendHailstoneFraction(group)) return AppendResult::APPENDED;
		return AppendResult::GROUP_INVALIDATED;

	}
}

std::optional<MiscGroup::Type> MiscGroup::parseColourCode(const std::string & group) {
	if (group == "BLU") return Type::COLOUR_CODE_BLUE;
	if (group == "WHT") return Type::COLOUR_CODE_WHITE;
	if (group == "GRN") return Type::COLOUR_CODE_GREEN;
	if (group == "YLO1") return Type::COLOUR_CODE_YELLOW1;
	if (group == "YLO2") return Type::COLOUR_CODE_YELLOW2;
	if (group == "AMB") return Type::COLOUR_CODE_AMBER;
	if (group == "RED") return Type::COLOUR_CODE_RED;
	if (group == "BLACKBLU") return Type::COLOUR_CODE_BLACKBLUE;
	if (group == "BLACKWHT") return Type::COLOUR_CODE_BLACKWHITE;
	if (group == "BLACKGRN") return Type::COLOUR_CODE_BLACKGREEN;
	if (group == "BLACKYLO1") return Type::COLOUR_CODE_BLACKYELLOW1;
	if (group == "BLACKYLO2") return Type::COLOUR_CODE_BLACKYELLOW2;
	if (group == "BLACKAMB") return Type::COLOUR_CODE_BLACKAMBER;
	if (group == "BLACKRED") return Type::COLOUR_CODE_BLACKRED;
	return std::optional<Type>();
}

bool MiscGroup::appendHailstoneFraction(const std::string & group) {
	// Fraction specified with increment of 1/4
	bool appended = false;
	auto value = groupData.value_or(0.0);
	if (group == "1/4") { value += 0.25; appended = true; }
	if (group == "1/2" || group == "2/4") { value += 0.5; appended = true; }
	if (group == "3/4") { value += 0.75; appended = true; }
	if (!appended) return false; 
	groupData = value;
	incompleteText = IncompleteText::NONE;
	return true;
}

bool MiscGroup::appendDensityAltitude(const std::string & group) {
	static const std::string unitStr ("FT");
	static const auto unitLen = unitStr.length();
	if (group.length() < unitLen + 1) return false; //require at least 1 digit and FT
	const auto groupUnitStr = group.substr(group.length() - unitLen);
	if (groupUnitStr != unitStr) return false;
	const auto val = strToUint(group, 0, group.length() - unitLen);
	if (!val.has_value()) return false;
	groupData = *val;
	incompleteText = IncompleteText::NONE;
	return true;
}

bool MiscGroup::isValid() const {
	return (incompleteText == IncompleteText::NONE);
}

///////////////////////////////////////////////////////////////////////////////

SyntaxGroup getSyntaxGroup(const Group & group) {
	if (auto keywordGroup = std::get_if<KeywordGroup>(&group)) {
		switch (keywordGroup->type()) {
			case KeywordGroup::Type::METAR:	return SyntaxGroup::METAR;
			case KeywordGroup::Type::SPECI:	return SyntaxGroup::SPECI;
			case KeywordGroup::Type::TAF:	return SyntaxGroup::TAF;
			case KeywordGroup::Type::COR:	return SyntaxGroup::COR;
			case KeywordGroup::Type::AMD:	return SyntaxGroup::AMD;
			case KeywordGroup::Type::NIL:	return SyntaxGroup::NIL;
			case KeywordGroup::Type::CNL:	return SyntaxGroup::CNL;
			case KeywordGroup::Type::RMK:	return SyntaxGroup::RMK;

			case KeywordGroup::Type::MAINTENANCE_INDICATOR:
			return SyntaxGroup::MAINTENANCE_INDICATOR;

			default:						return SyntaxGroup::OTHER;
		}
	}
	if (std::get_if<LocationGroup>(&group)) return SyntaxGroup::LOCATION;
	if (std::get_if<ReportTimeGroup>(&group)) return SyntaxGroup::REPORT_TIME;
	if (auto trendGroup = std::get_if<TrendGroup>(&group)) {
		if (trendGroup->isTimeSpanGroup()) return SyntaxGroup::TIME_SPAN;
		return SyntaxGroup::OTHER;
	}
	return SyntaxGroup::OTHER;
}

///////////////////////////////////////////////////////////////////////////////

ParseResult Parser::parse(const std::string & report, size_t groupLimit) {
	ReportInput in(report);

	bool reportEnd = false;
	Status status;
	ReportMetadata reportMetadata;
	ParseResult result;
	size_t groupCount = 0;

	//Iterate through report groups separated by delimiters
	std::string groupStr;
	in >> groupStr;
	while (!groupStr.empty() && !reportEnd && !status.isError()) {

		Group group;
		ReportPart reportPart = status.getReportPart();
		if (!appendToLastResultGroup(result, groupStr, reportPart, reportMetadata)) {
			// Current group was not appended to last group
			do {
				// Group may be parsed multiple times because at this point 
				// parser may not know yet if the report is METAR or TAF
				// and reportPart may change based on report type.
				reportPart = status.getReportPart(); 
				group = GroupParser::parse(groupStr, reportPart, reportMetadata);
				status.transition(getSyntaxGroup(group));
				groupCount++;
				if (groupCount >= groupLimit) status.setError(ReportError::REPORT_TOO_LARGE);
			} while(status.isReparseRequired()  && !status.isError());
			updateMetadata(group, reportMetadata);
			addGroupToResult(result, std::move(group), reportPart, std::move(groupStr));
		} else {
			// Raw string was appended to the group, just increase group count
			groupCount++;
			if (groupCount >= groupLimit) status.setError(ReportError::REPORT_TOO_LARGE);
		}
		
		in >> groupStr;
	}
	if (!result.groups.empty()) {
		// if last group is incomplete, invalidate it by adding an empty string
		appendToLastResultGroup(result, "", status.getReportPart(), reportMetadata);
		// but do not save this empty string if the group just rejects it
		if (result.groups.back().rawString.empty()) result.groups.pop_back();
	}
	status.finalTransition();
	reportMetadata.type = status.getReportType();
	reportMetadata.error = status.getError();
	result.reportMetadata = std::move(reportMetadata);
	return result;
}

bool Parser::appendToLastResultGroup(ParseResult & result,
	const std::string & groupStr,
	ReportPart reportPart,
	const ReportMetadata & reportMetadata,
	bool allowReparse)
{
	// Unable to append if this is the first group
	if (result.groups.empty()) return false;

	// Do not append last group in result is fallback (unknown) group
	// Fallback group is for case 'when everything else fails' and must be
	// used only if all parse attempts by other groups failed
	if (std::holds_alternative<FallbackGroup>(result.groups.back().group)) return false;

	GroupInfo & lastGroupInfo = result.groups.back();
	Group & lastGroup = lastGroupInfo.group;

	const auto appendResult = std::visit(
		[&](auto && gr) -> AppendResult {
			return gr.append(groupStr, reportPart, reportMetadata);
		}, lastGroup);

	switch (appendResult) {
		case AppendResult::APPENDED:
		lastGroupInfo.rawString += groupDelimiterChar;
		lastGroupInfo.rawString += groupStr;
		return true;

		case AppendResult::NOT_APPENDED:
		return false;

		case AppendResult::GROUP_INVALIDATED:
		{
			std::string prevStr = std::move(result.groups.back().rawString);
			const auto prevRp = result.groups.back().reportPart;
			const auto & prevGroup = result.groups.back().group;
			if (!allowReparse) {
				addGroupToResult(result, FallbackGroup(), prevRp, std::move(prevStr));
				return false;
			}
			const auto reparsed = 
				GroupParser::reparse(prevStr, prevRp, reportMetadata, prevGroup);
			const bool reparsedIsOtherGroup = 
				!std::holds_alternative<FallbackGroup>(reparsed);
			result.groups.pop_back();
			addGroupToResult(result, std::move(reparsed), prevRp, std::move(prevStr));
			if (!reparsedIsOtherGroup) return false;
			return appendToLastResultGroup(result, groupStr, reportPart, reportMetadata, false);
		}
	}
}

void Parser::addGroupToResult(ParseResult & result,
	Group group,
	ReportPart reportPart,
	std::string groupString)
{
	if (!result.groups.empty() && std::holds_alternative<FallbackGroup>(group)) {
		// Assumed that two fallback groups can always be appended 
		GroupInfo & lastGroupInfo = result.groups.back();
		if (std::get_if<FallbackGroup>(&lastGroupInfo.group)) {
			lastGroupInfo.rawString += groupDelimiterChar;
			lastGroupInfo.rawString += groupString;
			return;
		}
	}
	GroupInfo groupInfo(std::move(group), reportPart, groupString);
	result.groups.push_back(std::move(groupInfo));
}


std::string Parser::ReportInput::getNextGroup() {
	if (finished) return std::string();

	// ASCII control codes and spaces are concidered delimiters
	while (report[pos] <= ' ') {
		if (pos >= report.length()) {
			finished = true;
			return std::string();
		}
		pos++;
	}

	size_t groupLen = 0;
	while (report[pos + groupLen] > ' ') {
		if (pos >= report.length() || report[pos + groupLen] == reportEndChar) {
			finished = true;
			return report.substr(pos, groupLen);
		}
		groupLen++;
	}
	const auto prevPos = pos;
	pos = pos + groupLen;
	return report.substr(prevPos, groupLen);
}


ReportPart Parser::Status::getReportPart() {
	switch (state) {
		case State::REPORT_TYPE_OR_LOCATION:
		case State::CORRECTION:
		case State::LOCATION:
		case State::REPORT_TIME:
		case State::TIME_SPAN:
		return ReportPart::HEADER;

		case State::REPORT_BODY_BEGIN_METAR:
		case State::REPORT_BODY_BEGIN_METAR_REPEAT_PARSE:
		case State::REPORT_BODY_METAR:
		return ReportPart::METAR;

		case State::REPORT_BODY_BEGIN_TAF:
		case State::REPORT_BODY_TAF:
		return ReportPart::TAF;

		case State::REMARK_METAR:
		case State::REMARK_TAF:
		return ReportPart::RMK;

		case State::NIL:
		case State::CNL:
		case State::ERROR:
		return ReportPart::UNKNOWN;
	}
}

void Parser::Status::transition(SyntaxGroup group) {
	switch (state) {
		case State::REPORT_TYPE_OR_LOCATION:
		transitionFromReportTypeOrLocation(group);
		break;

		case State::CORRECTION:
		transitionFromCorrecton(group);
		break;

		case State::LOCATION:
		if (group == SyntaxGroup::LOCATION) {setState(State::REPORT_TIME); break;}
		setError(ReportError::EXPECTED_LOCATION);
		break;

		case State::REPORT_TIME:
		transitionFromReportTime(group);
		break;

		case State::TIME_SPAN:
		transitionFromTimeSpan(group);
		break;

		case State::REPORT_BODY_BEGIN_METAR:
		case State::REPORT_BODY_BEGIN_METAR_REPEAT_PARSE:
		transitionFromReportBodyBeginMetar(group);
		break;

		case State::REPORT_BODY_METAR:
		transitionFromReportBodyMetar(group);
		break;

		case State::REPORT_BODY_BEGIN_TAF:
		transitionFromReportBodyBeginTaf(group);
		break;

		case State::REPORT_BODY_TAF:
		transitionFromReportBodyTaf(group);
		break;

		case State::REMARK_TAF:
		if (group == SyntaxGroup::MAINTENANCE_INDICATOR) {
			setError(ReportError::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
		}
		break;

		case State::NIL:
		setError(ReportError::UNEXPECTED_GROUP_AFTER_NIL);
		break;

		case State::CNL:
		setError(ReportError::UNEXPECTED_GROUP_AFTER_CNL);
		break;

		case State::REMARK_METAR:
		case State::ERROR:
		break;
	}
}

void Parser::Status::transitionFromReportTypeOrLocation(SyntaxGroup group) {
	switch(group) {
		case SyntaxGroup::METAR:
		case SyntaxGroup::SPECI:
		setReportType(ReportType::METAR);
		setState(State::CORRECTION);
		break;

		case SyntaxGroup::TAF:
		setReportType(ReportType::TAF);
		setState(State::CORRECTION);
		break;

		case SyntaxGroup::LOCATION:
		setState(State::REPORT_TIME);
		break;

		default:
		setError(ReportError::EXPECTED_REPORT_TYPE_OR_LOCATION);
		break;
	}
}

void Parser::Status::transitionFromCorrecton(SyntaxGroup group) {
	switch (group) {
		case SyntaxGroup::AMD:
		setState(State::LOCATION);
		if (getReportType() != ReportType::TAF)
			setError(ReportError::AMD_ALLOWED_IN_TAF_ONLY);
		break;

		case SyntaxGroup::COR:
		setState(State::LOCATION);
		break;

		case SyntaxGroup::LOCATION:
		setState(State::REPORT_TIME);
		break;

		default:
		setError(ReportError::EXPECTED_LOCATION);
		break;
	}
}

void Parser::Status::transitionFromReportTime(SyntaxGroup group) {
	switch (group) {
		case SyntaxGroup::REPORT_TIME:
		if (reportType == ReportType::METAR) {
			setState(State::REPORT_BODY_BEGIN_METAR);
			break;
		}
		setState(State::TIME_SPAN);
		break;

		case SyntaxGroup::TIME_SPAN:
		if (getReportType() == ReportType::TAF) {
			setState(State::REPORT_BODY_BEGIN_TAF);
			break;
		}
		setError(ReportError::EXPECTED_REPORT_TIME);
		break;

		case SyntaxGroup::NIL:
		setState(State::NIL);
		break;

		default:
		setError(ReportError::EXPECTED_REPORT_TIME);
		break;
	}
}

void Parser::Status::transitionFromTimeSpan(SyntaxGroup group) {
	switch(group) {
		case SyntaxGroup::TIME_SPAN:
		setReportType(ReportType::TAF);
		setState(State::REPORT_BODY_BEGIN_TAF);
		break;

		case SyntaxGroup::NIL:
		setState(State::NIL);
		break;

		default:
		if (getReportType() == ReportType::UNKNOWN) {
			setReportType(ReportType::METAR);
			setState(State::REPORT_BODY_BEGIN_METAR_REPEAT_PARSE);
			break;
		}
		setError(ReportError::EXPECTED_TIME_SPAN);
		break;
	}
}

void Parser::Status::transitionFromReportBodyBeginMetar(SyntaxGroup group) {
	switch(group) {
		case SyntaxGroup::NIL:
		setState(State::NIL);
		break;

		case SyntaxGroup::CNL:
		setError(ReportError::CNL_ALLOWED_IN_TAF_ONLY);
		break;

		case SyntaxGroup::RMK:
		setState(State::REMARK_METAR);
		break;

		default:
		setState(State::REPORT_BODY_METAR);
		break;
	}
}

void Parser::Status::transitionFromReportBodyMetar(SyntaxGroup group) {
	switch(group) {
		case SyntaxGroup::RMK:
		setState(State::REMARK_METAR);
		break;

		case SyntaxGroup::NIL:
		case SyntaxGroup::CNL:
		setError(ReportError::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
		break;

		default:
		break;
	}
}

void Parser::Status::transitionFromReportBodyBeginTaf(SyntaxGroup group) {
	switch(group) {
		case SyntaxGroup::NIL:
		setState(State::NIL);
		break;

		case SyntaxGroup::CNL:
		setState(State::CNL);
		break;

		case SyntaxGroup::RMK:
		setState(State::REMARK_TAF);
		break;

		case SyntaxGroup::MAINTENANCE_INDICATOR:
		setError(ReportError::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
		break;

		default:
		setState(State::REPORT_BODY_TAF);
		break;
	}
}

void Parser::Status::transitionFromReportBodyTaf(SyntaxGroup group) {
	switch(group) {
		case SyntaxGroup::RMK:
		setState(State::REMARK_TAF);
		break;

		case SyntaxGroup::NIL:
		case SyntaxGroup::CNL:
		setError(ReportError::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY);
		break;

		case SyntaxGroup::MAINTENANCE_INDICATOR:
		setError(ReportError::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY);
		break;

		default:
		break;
	}
}

void Parser::Status::finalTransition() {
	switch (state) {
		case State::REPORT_BODY_METAR:
		case State::REPORT_BODY_TAF:
		case State::REMARK_METAR:
		case State::REMARK_TAF:
		case State::NIL:
		case State::CNL:
		case State::ERROR:
		break;

		case State::REPORT_TYPE_OR_LOCATION:
		setError(ReportError::EMPTY_REPORT);
		break;

		case State::CORRECTION:
		case State::LOCATION:
		case State::REPORT_TIME:
		case State::TIME_SPAN:
		case State::REPORT_BODY_BEGIN_METAR:
		case State::REPORT_BODY_BEGIN_METAR_REPEAT_PARSE:
		case State::REPORT_BODY_BEGIN_TAF:
		setError(ReportError::UNEXPECTED_REPORT_END);
		break;
	}
}

void Parser::updateMetadata(const Group & group, ReportMetadata & reportMetadata) {
	if (const auto keyword = std::get_if<KeywordGroup>(&group); keyword)
		switch (keyword->type()) {
			case KeywordGroup::Type::SPECI: 
			reportMetadata.isSpeci = true;
			break;
			
			case KeywordGroup::Type::NOSPECI:
			reportMetadata.isNospeci = true;
			break;

			case KeywordGroup::Type::AUTO: reportMetadata.isAutomated = true;
			break;

			case KeywordGroup::Type::AO1:
			reportMetadata.isAo1 = true;
			break;

			case KeywordGroup::Type::AO1A:
			reportMetadata.isAo1a = true;
			break;

			case KeywordGroup::Type::AO2:
			reportMetadata.isAo2 = true;
			break;
			
			case KeywordGroup::Type::AO2A:
			reportMetadata.isAo2a = true;
			break;

			case KeywordGroup::Type::NIL:
			reportMetadata.isNil = true;
			break;

			case KeywordGroup::Type::CNL:
			reportMetadata.isCancelled = true;
			break;

			case KeywordGroup::Type::AMD:
			reportMetadata.isAmended = true;
			break;

			case KeywordGroup::Type::COR:
			reportMetadata.isCorrectional = true;
			break;
			
			case KeywordGroup::Type::MAINTENANCE_INDICATOR:
			reportMetadata.maintenanceIndicator = true;
			break;
			
			default:
			break;
		}
	if (const auto reportTime = std::get_if<ReportTimeGroup>(&group); reportTime) {
		reportMetadata.reportTime = reportTime->time();
	}
	if (const auto location = std::get_if<LocationGroup>(&group); location)
		reportMetadata.icaoLocation = location->toString();
	if (const auto misc = std::get_if<MiscGroup>(&group); 
		misc && 
		misc->type() == MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION &&
		misc->data().has_value()) {
			reportMetadata.isCorrectional = true;
			reportMetadata.correctionNumber = *misc->data();
	}
	if (const auto trend = std::get_if<TrendGroup>(&group); 
		trend && 
		reportMetadata.type != ReportType::METAR &&
		trend->type() == TrendGroup::Type::TIME_SPAN &&
		!reportMetadata.timeSpanFrom.has_value() &&
		!reportMetadata.timeSpanUntil.has_value())
	{
		reportMetadata.timeSpanFrom = trend->timeFrom();
		reportMetadata.timeSpanUntil = trend->timeUntil();
	}
}

} //namespace metaf

#endif //#ifndef METAF_HPP