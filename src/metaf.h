/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

/// @file 
/// @brief METAR / TAF report parser, helpers and structs representing 
/// parsed METAR or TAF groups.

#ifndef METAF_H
#define METAF_H

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <regex>
#include <cctype>

namespace metaf {

	/// Metaf library version, see https://semver.org/ for details.
	struct Version {
		static const int major = 1;	///< Version major
		static const int minor = 0; ///< Version minor
		static const int patch = 0; ///< Version patch
	};

	// Structs of individual METAR or TAF groups used with Group variant
	class PlainTextGroup;
	class FixedGroup;
	class LocationGroup;
	class ReportTimeGroup;
	class TrendGroup;
	class WindGroup;
	class VisibilityGroup;
	class CloudGroup;
	class WeatherGroup;
	class TemperatureGroup;
	class TemperatureForecastGroup;
	class PressureGroup;
	class RunwayVisualRangeGroup;
	class RunwayStateGroup;
	class RainfallGroup;
	class SeaSurfaceGroup;
	class ColourCodeGroup;

	/// @brief A variant type for all possible METAR and TAF groups.
	/// @details Used by parser to return parsed group information.
	/// @par Group is expected to include PlainTextGroup, otherwise GroupParser
	/// will not compile.
	/// @par The requirements for classes included in Group are as follows.
	/// @par parse() method is required by GroupParser to recognise and parse 
	/// strings as individual groups. This method has two parameters: raw 
	/// string which contains METAR or TAF group, and Report Part where the 
	/// string was encountered. It returns either initialised Group or empty 
	/// optional if group format was not recognised.
	/// @par nextGroup() is used to combine sequential groups which represent 
	/// the same data (e.g. 1 1/2SM, 1 and 1/2SM are technically different 
	/// groups but represent different parts of the same value). Method 
	/// nextGroup() is required; if it is absent then Parser will not compile. 
	/// Method nextGroup() has a single parameters: a Group value which 
	/// contains parsed next group. It returns a Group with combined data from
	/// both current and next groups or empty optional if groups cannot be
	/// combined.
	using Group = std::variant<
		PlainTextGroup,
		FixedGroup,
		LocationGroup,
		ReportTimeGroup,
		TrendGroup,
		WindGroup,
		VisibilityGroup,
		CloudGroup,
		WeatherGroup,
		TemperatureGroup,
		TemperatureForecastGroup,
		PressureGroup,
		RunwayVisualRangeGroup,
		RunwayStateGroup,
		RainfallGroup,
		SeaSurfaceGroup,
		ColourCodeGroup
	>;

	/// Major structural part of the report 
	enum class ReportPart {
		UNKNOWN,	///< Unknown part of the report
		HEADER,		///< Report header (both METAR and TAF)
		METAR,		///< Report body (METAR)
		TAF,		///< Report body (TAF)
		RMK			///< Remark (both METAR and TAF)
	};

	/// METAR or TAF group important for report's syntax structure
	enum class SyntaxGroup {
		OTHER,			///< Any group other than specified below
		METAR,			///< METAR report
		SPECI,			///< Special METAR report
		TAF,			///< TAF report
		COR,			///< Report is correctional
		AMD,			///< Report is amended (TAF only)
		LOCATION,		///< ICAO location
		REPORT_TIME,	///< Date and time of report origin
		TIME_SPAN,		///< Time span covered by report
		CNL,			///< Cancelled forecast (TAF only)
		NIL,			///< Missing report
		RMK				///< Remark
	};

	///////////////////////////////////////////////////////////////////////////

	/// Runway identification
	class Runway {
	public:
		/// Runway designator (left / right / center).
		enum class Designator {
			NONE,		///< No designator specified.
			LEFT,		///< Left runway (parallel runways).
			CENTER,		///< Center runway (parallel runways).
			RIGHT,		///< Right runway (parallel runways).
		};
		Runway() = default;
		/// @brief Decode runway number/designator from string.
		/// @details The format is as follows: letter 'R' followed by two 
		/// digits followed by optional single letter ('R' or 'L' or 'C' 
		/// allowed).
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Runway> fromString(const std::string & s);
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const {
			return (rNumber <= maxRunwayNumber || 
				(rNumber == allRunwaysNumber && rDesignator == Designator::NONE) || 
				(rNumber == messageRepetitionNumber && rDesignator == Designator::NONE));
		}
		/// @return True when all runways encoded (runway number 88), false otherwise
		bool isAllRunways() const {
			return(rNumber == allRunwaysNumber && rDesignator == Designator::NONE);
		}
		/// @return True when all runways encoded (runway number 99), false otherwise
		bool isMessageRepetition() const {
			return(rNumber == messageRepetitionNumber && rDesignator == Designator::NONE);
		}
		/// @return Runway number.
		unsigned int number() const { return(rNumber); }
		/// @return Runway designator.
		Designator designator() const { return(rDesignator); }
	private:
		static inline std::optional<Designator> designatorFromChar(char c);
	private:
		unsigned int rNumber = 0;
		Designator rDesignator = Designator::NONE;
		static const unsigned int allRunwaysNumber = 88;
		static const unsigned int messageRepetitionNumber = 99;	
		static const auto maxRunwayNumber = 360 / 10;
	};

	/// @brief Time used in METAR/TAF. Includes day-of-month (optional), hour 
	/// and minute. 
	/// @details Day is optional and can be not included. Hour is in range 
	/// from 0 to 24; 0 hours means 'midnight between previous and this day', 
	/// 24 hours means 'midnight between this and next day'.
	class MetafTime {
	public:
		MetafTime() = default;
		/// @brief Decode metaf time in format DDHHMM from string 
		/// @details DD = two digits for day-of-month (optional, can be missing), 
		/// HH = two digits for hour in 24-hour format, MM = two digits for minute.
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<MetafTime> fromStringDDHHMM(const std::string & s);
		/// @brief Decode metaf time in format DDHH from string 
		/// @details DD = two digits for day-of-month, HH = two digits for hour
		/// in 24-hour format. Minutes are assumed to be zero.
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<MetafTime> fromStringDDHH(const std::string & s);
		/// @return True if stored data are consistent, false otherwise.
		inline bool isValid() const;
		/// @return Day value or empty optional if day or entire time value is 
		/// not repoted.
		std::optional<unsigned int> day() const { return(dayValue); }
		/// @return Hours in range 0 to 24.
		unsigned int hour() const { return(hourValue); }
		/// @return Minutes in range 0 to 59.
		unsigned int minute() const { return(minuteValue); }
	private:
		std::optional<unsigned int> dayValue;
		unsigned int hourValue = 0;
		unsigned int minuteValue = 0;
	private:
		static const inline unsigned int dayNotReported = 0;
		static const inline unsigned int maxDay = 31;
		static const inline unsigned int maxHour = 24;
		static const inline unsigned int maxMinute = 59;
	};

	/// @brief Group: Temperature value.
	/// @details Temperature value is optional (can be not reported). 
	/// Flag 'freezing' additionally specifies freezing temperatures (i.e. 
	/// below 0 C / 32 F). When temperature is zero degrees C, 'freezing' flag 
	/// additionally specifies ranges either (-0.5 to 0) or [0 to 0.5), as 
	/// encoded in METAF/TAF temperature format as M00 and 00 respectively.
	class Temperature {
	public:
		/// Temperature measurement units.
		enum class Unit {
			C,	///< Centigrade / Degrees Celsius.
			F,	///< Degrees Fahrenheit.
		};
		Temperature () = default;
		/// @brief Decode temperature value from string.
		/// @details The format is as follows: optional letter 'M' followed by 
		/// two digits.
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Temperature> fromString(const std::string & s);
		/// @brief Return value converted to certain measurment units.
		/// @details Conversion factors from: 
		/// https://en.wikipedia.org/wiki/Conversion_of_units_of_temperature#Celsius_(centigrade)
		/// @param unit Units to convert value to.
		/// @return Value converted to specified units (or empty optional if 
		/// value is not reported or conversion error occurred).
		std::optional<float> inline toUnit(Unit unit) const;
		/// Whether temperature related to freezing (below or equal zero 
		/// centigrate). If temperature is zero centigrade, it can be either 
		/// freezing (corresponds to METAR/TAF value M00) or non-freezing
		/// (corresponds to METAR/TAF value 00).
		/// @return True if freezing temperature, false if non-freezing 
		/// temperature or if temperature is not reported.
		bool isFreezing() const { return(freezing); }
		/// @return Temperature value.
		std::optional<int> temperature() const { return(tempValue); }
		/// @return Currently used measurement units.
		Unit unit() const { return(tempUnit); }
	private:
		std::optional<int> tempValue;
		bool freezing = false;//Combined with zero represents difference between 00 & M00
		static const Unit tempUnit = Unit::C; //METAR & TAF always use centigrade
	};

	/// @brief Speed value.
	/// @details Speed value is optional (can be not reported).
	class Speed {
	public:
		/// Speed measurement units.
		enum class Unit {
			KNOTS,				///< Knots.
			METERS_PER_SECOND,	///< Meters per second.
			KILOMETERS_PER_HOUR,///< Kilometers per hour.
			MILES_PER_HOUR		///< Miles per hour.
		};
		Speed() = default;
		/// @brief Decode speed value from string.
		/// @details The format is two-digit or three-digit speed value.
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @param unit Speed unit (e.g. decoded with unitFromString()).
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Speed> fromString(const std::string & s, Unit unit);
		/// @brief Decode speed unit from string.
		/// @details The following options are allowed: KT (knots), MPS(m/s), 
		/// KMH (km/h).
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded unit or empty optional if format error occurred.
		static inline std::optional<Unit> unitFromString(const std::string & s);
		/// @brief Return value converted to certain measurment units.
		/// @details Conversion factors from: 
		/// https://en.wikipedia.org/wiki/Speed#Units
		/// @param unit Units to convert value to.
		/// @return Value converted to specified units (or empty optional if 
		/// value is not reported or conversion error occurred).
		std::optional<float> inline toUnit(Unit unit) const;
		/// @return Speed value or empty optional if not reported.
		std::optional<unsigned int> speed() const { return(speedValue); }
		/// @return Currently used measurement units.
		Unit unit() const { return(speedUnit); }
	private:
		std::optional<unsigned int> speedValue;
		Unit speedUnit = Unit::KNOTS;
	private:
		static inline std::optional<float> knotsToUnit(float valueKnots, Unit otherUnit);
		static inline std::optional<float> mpsToUnit(float valueMps, Unit otherUnit);
		static inline std::optional<float> kmhToUnit(float valueKmh, Unit otherUnit);
		static inline std::optional<float> mphToUnit(float valueMph, Unit otherUnit);
	};

	/// @brief Distance or height value.
	/// @details Distance value is optional (can be not reported). The value 
	/// consists of integer, numerator and denominator parts (e.g. 1 1/2); if 
	/// numerator or denominator are zero, the value is assumed to be 
	/// integer-only.
	class Distance {
	public:
		/// Distance measurement units, used across groups
		enum class Unit {
			METERS,			///< Distance in meters.
			STATUTE_MILES,	///< Distance in statute miles.
			FEET			///< Distance/altitude in feet.
		};
		/// Modifier: none (exact value) / more than / less than.
		enum class Modifier {
			NONE,		///< The distance is exactly as repoted.
			LESS_THAN,	///< The distance is less than reported value.
			MORE_THAN,	///< The distance is more than reported value.
		};
		Distance() = default;
		/// Init integer-value distance.
		/// @param d Distance integer component.
		/// @param u Distance units.
		Distance(int d, Unit u) : distValueInt(d), distUnit(u) {}
		/// Init non-reported distance.
		/// @param u Distance units.
		Distance(Unit u) : distUnit(u) {}
		/// @brief Decode distance value from visibility string in meters.
		/// @details The recognised format is four digits or //// (not reported).
		/// @par Value "9999": interpreted as 'more than 10000 Meters'.
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Distance> fromMeterString(const std::string & s);
		/// @brief Decode distance value from visibility string in statute 
		/// miles.
		/// @details The recognised format is an optional letter P or M which 
		/// means 'more than' or 'less than' respectively, followed by one or 
		/// two digits, followed by optional slash with one or two digits, 
		/// followed by "SM", (e.g. 1/4SM, M1/8SM, 3SM, 10SM, P6SM are valid 
		/// examples).
		/// @par If fraction numerator is greater than denominator then first 
		/// digit of numerator is interpreted as integer part (e.g. 11/2SM is 
		/// intepreted as '1 and 1/2 of statute mile' or 21/4SM is 
		/// interpreted as '2 and 1/4 of statute mile').
		/// @par Alternatively value "////SM" is interpreted as 'not reported'.
		static inline std::optional<Distance> fromMileString(const std::string & s);
		/// @brief Decode distance value from height string.
		/// @details The format is three digits or "///" (not reported). The 
		/// unit is assumed to be hundreds of feet (the value is multiplied by
		/// 100 and unit set to Feet).
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Distance> fromHeightString(const std::string & s);
		/// @brief Decode distance value from Runway Visual Range string.
		/// @details The format is optional letter P or M which means 'more 
		/// than' or 'less than' respectively followed by four digits. The unit
		/// is provided in a separate string which can be empty (unit assumed 
		/// to be Meters) or take value of "FT" (unit assumed to be Feet).
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @param unitFeet True if distance unit is Feet, false if distance 
		/// unit is Meters.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Distance> fromRvrString(const std::string & s, bool unitFeet);
		/// Combine integer and fraction parts of the distance.
		/// @param integer Distance value which contains integer component.
		/// @param integer Distance value which contains fraction component.
		/// @return Distance value combined from integer and fraction 
		/// components, or empty optional if error occurred.
		static inline std::optional<Distance> fromIntegerAndFraction(const Distance & integer,
			const Distance & fraction);
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const {
			if (distValueDen.has_value() && !distValueDen.value()) return(false);
			if (distValueDen.has_value() && !distValueNum.value()) return(false);
			return(true);
		}
		/// @return Integer component or empty optional if value is not 
		/// reported or has no integer component.
		inline std::optional<unsigned int> integer() const { return(distValueInt); }
		/// @return Numerator of fraction or empty optional if value is not 
		/// reported or has no fraction component.
		inline std::optional<unsigned int> numerator() const { return(distValueNum); }
		/// @return Numerator of fraction or empty optional if value is not 
		/// reported or has no fraction component.
		inline std::optional<unsigned int> denominator() const { return(distValueDen); }
		/// @return True if value is integer (i.e. no fraction component), 
		/// false otherwise.
		bool isInteger() const {
			return(integer().has_value() && 
				!numerator().has_value() && !denominator().has_value());
		}
		/// @return True if value is integer (i.e. no fraction component), 
		/// false otherwise.
		bool isFraction() const {
			return(!integer().has_value() && 
				numerator().has_value() && denominator().has_value());
		}
		/// @return True if value is reported (i.e. either integer or fraction
		/// components are non empty), false otherwise.
		bool isReported() const {
			return(integer().has_value() || 
					(numerator().has_value() && denominator().has_value()));
		}
		/// @return Distance modifier.
		Modifier modifier() const { return(distModifier); }
		/// @return Current measurement units.
		Unit unit() const { return(distUnit); }
		/// Return value converted to certain measurment units.
		/// @param unit Units to convert value to.
		/// @return Value converted to specified units (or empty optional if 
		/// value is not reported or conversion error occurred).
		inline std::optional<float> toUnit(Unit unit) const;
	private:
		Modifier distModifier = Modifier::NONE;
		std::optional<unsigned int> distValueInt;
		std::optional<unsigned int> distValueNum;
		std::optional<unsigned int> distValueDen;
		Unit distUnit = Unit::METERS;

		static const unsigned int heightFactor = 100; //height unit is 100s of feet

		static inline std::optional<Modifier> modifierFromString(const std::string & s);
		static inline std::optional<float> metersToUnit(float value, Unit unit);
		static inline std::optional<float> milesToUnit(float value, Unit unit);
		static inline std::optional<float> feetToUnit(float value, Unit unit);
	};

	/// @brief Direction value.
	/// @details Direction relative to north, 0 to 360 degrees.
	class Direction {
	public:
		/// Cardinal directions
		enum class Cardinal {
			NONE,	///< No corresponding cardinal direction
			NDV,	///< No Directional Variation.
			N,		///< North.
			S,		///< South.
			W,		///< West.
			E,		///< East.
			NW,		///< Northwest.
			NE,		///< Northeast.
			SW,		///< Southwest.
			SE,		///< Southeast.
			TRUE_N, ///< True North.
			TRUE_W, ///< True West.
			TRUE_S, ///< True South.
			TRUE_E, ///< True East.
		};
		enum class Status {
			OMMITTED, 		  ///< Direction is ommitted (not specified at all).
			NOT_REPORTED,	  ///< Direction is specified as not reported.
			VARIABLE,		  ///< Direction is reported as variable.
			NDV,			  ///< Direction is reported as No Directional Variation.
			VALUE_DEGREES, 	  ///< Direction value in degrees is reported.
			VALUE_CARDINAL,	  ///< Direction value as cardinal direction is reported.
		};
		Direction() = default;
		/// @brief Decode direction in degrees from cardinal direction string.
		/// @details The following options are allowed: "N", "S", "W", "E", 
		/// "NW", "SW", "NE", "SE". The special value "NDV" is also allowed, 
		/// value of degreesNDV is returned instead of degrees value.
		/// @par If the input string is empty, degreesNotSpecified is used.
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Direction> fromCardinalString(const std::string & s);
		/// @brief Decode direction in degrees from numeric string. 
		/// @details The format is three digits, alternatively special values 
		/// are allowed: "///" (NOT_REPORTED) or "VRB" (VARIABLE). 
		/// @par If the input string is empty, degreesNotSpecified is used.
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Direction> fromDegreesString(const std::string & s);
		/// @return True if direction value is provided, false if direciton is 
		/// ommitted, not reported, variable or NDV.
		bool isValue() const {
			return(dirStatus == Status::VALUE_DEGREES || dirStatus == Status::VALUE_CARDINAL); 
		}
		/// @return Value in degrees units (or empty optional if the status is 
		/// OMMITTED, NOT_REPORTED, VARIABLE or NDV).
		std::optional<unsigned int> degrees() const {
			if (!isValue())	return(std::optional<unsigned int>());
			return(dirDegrees);
		}
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const {
			if (isValue() && dirDegrees > maxDegrees) return(false);
			return(true);
		}
		/// @return Closest cardinal direction to degrees value (or empty 
		/// optional if the value is not reported or conversion error 
		/// occurred).
		inline Cardinal cardinal(bool trueDirections = false) const;
		/// @return Status value.
		Status status() const { return (dirStatus); }
	private:
		unsigned int dirDegrees = 0;
		Status dirStatus = Status::OMMITTED;
	private:
		static const inline unsigned int maxDegrees = 360;
		static const inline unsigned int degreesTrueNorth = 360;
		static const inline unsigned int degreesTrueWest = 270;
		static const inline unsigned int degreesTrueSouth = 180;
		static const inline unsigned int degreesTrueEast = 90;
		static const inline unsigned int degreesNorthWest = 315;
		static const inline unsigned int degreesNorthEast = 45;
		static const inline unsigned int degreesSouthWest = 225;
		static const inline unsigned int degreesSouthEast = 135;
	};

	/// Pressure value.
	class Pressure {
	public:
		/// Pressure measurement units.
		enum class Unit {
			HECTOPASCAL,	///< Hectopascal (millibar).
			INCHES_HG		///< Inches mercury.
		};
		Pressure() = default;
		/// @brief Decode pressure value from string.
		/// @details The format is letter A or Q (means unit is inches mercury
		/// or hectopascal respectively) followed by four-digit value or 
		/// value "////" which means 'not reported'. Value in inches mercury 
		/// is interpreted as fixed-point value with two integer digits and
		/// two decimal fraction digits (i.e. divided by 100.0).
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Pressure> fromString(const std::string & s);
		/// Return value converted to certain measurment units.
		/// @param unit Units to convert value to.
		/// @return Value converted to specified units (or empty optional if 
		/// value is not reported or conversion error occurred).
		inline std::optional<float> toUnit(Unit unit) const;
		/// @return Currently used measurement units.
		Unit unit() const { return(pressureUnit); }
		/// @return Pressure value or empty optional if value is not reported.
		std::optional<float> pressure() const { return(pressureValue); }
	private:
		std::optional<float> pressureValue;
		Unit pressureUnit = Unit::HECTOPASCAL;
	};

	/// Precipitation amount or deposit accumulation depth.
	class Precipitation {
	public:
		/// Precipitation value status.
		enum class Status {
			NOT_REPORTED,			///< Precipitation is not reported.
			REPORTED,				///< Precipitation is reported.
			RUNWAY_NOT_OPERATIONAL,	///< Runway is not operational; no precipitation reported.
		};
		/// Precipitation (deposit accumulation) measurement units.
		enum class Unit {
			MM,		///< Millimeters
			INCHES,	///< Inches
		};
		Precipitation() = default;
		/// @brief Decode precipitation amount from rainfall string.
		/// @details The format is three digits of integer value followed
		/// by dot ('.') and one digit of decimal fraction value. If the 
		/// string is empty, a 'not reported' value is returned.
		/// @par Flag runwayNotOperational is always set to false.
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Precipitation> fromRainfallString(const std::string & s);
		/// @brief Decode precipitation amount from runway deposit depth.
		/// @details The format is two digits or "//" which means 'not 
		/// reported'. 
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @par If input string contains reserved value 91, an empty optional 
		/// is returned. If input string contains special value 99, the value
		/// is interpreted as 'runway not operational'.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<Precipitation> fromRunwayDeposits(const std::string & s);
		/// @brief Return value converted to certain measurment units.
		/// @details Conversion factor from https://en.wikipedia.org/wiki/Inch
		/// @param unit Units to convert value to.
		/// @return Value converted to specified units (or empty optional if 
		/// value is not reported or conversion error occurred).
		inline std::optional<float> toUnit(Unit unit) const;
		/// @return Currently used measurement units.
		Unit unit() const { return(precipUnit); }
		/// @return Status value.
		Status status() const { return (precipStatus); }
		/// @return Precipitation value or empty optional if not reported or
		/// runway is not operational.
		std::optional<float> precipitation() const { 
			if (precipStatus != Status::REPORTED) return(std::optional<float>());
			return(precipValue);
		}
	private:
		Status precipStatus = Status::NOT_REPORTED;
		float precipValue = 0.0;
		static const Unit precipUnit = Unit::MM;
	private:
		/// Special value for runway deposits depth, see Table 1079 in Manual 
		/// on Codes (WMO No. 306).
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

	/// Surface friction of the runway.
	class SurfaceFriction {
	public:
		/// Value status (which value was reported).
		enum class Status {
			NOT_REPORTED,				///< Surface friction not reported.
			SURFACE_FRICTION_REPORTED,	///< Surface friction coefficient is reported.
			BRAKING_ACTION_REPORTED,	///< Braking actions is reported.
			UNRELIABLE 					///< Value unreliable or unmeasurable.
		};
		/// Braking action values
		enum class BrakingAction {
			NONE,			///< Value not reported or not specified.
			POOR,	 		///< Braking action poor (friction coef 0 to 0.25).
			MEDIUM_POOR,	///< Braking action medium/poor (friction coef 0.26 to 0.29).
			MEDIUM,			///< Braking action medium (friction coef 0.30 to 0.35).
			MEDIUM_GOOD,	///< Braking action medium/good (friction coef 0.36 to 0.39).
			GOOD,			///< Braking action good (friction coef >0.40).
		};
		SurfaceFriction() = default;
		/// @brief Decode surface friction value from runway state string.
		/// @details The format is two digits or "//" which means 'not 
		/// reported'.
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @par Either Friction Coefficient or Braking Action can be 
		/// reported, in both cases friction coefficient is initialised (if 
		/// Braking Action is reported, then the lowest possible friction 
		/// coefficient value for given Braking Action is selected).
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<SurfaceFriction> fromString(const std::string & s);
		/// @return Status value.
		Status status() const { return (sfStatus); }
		/// @return Friction coefficient or empty optional if not reported or 
		/// unreliable.
		std::optional<float> coefficient() const {
			if (sfStatus == Status::NOT_REPORTED || 
				sfStatus == Status::UNRELIABLE) return(std::optional<float>());
			return(sfCoefficient * coefficientDecimalPointShift);
		}
		/// @return Braking action or empty optional if not reported or 
		/// unreliable.
		inline BrakingAction brakingAction() const;
	private:
		Status sfStatus = Status::NOT_REPORTED;	
		unsigned int sfCoefficient = 0; //0 to 100, multiply by 0.01 to get actual value
		static const inline auto coefficientDecimalPointShift = 0.01;
	private:
		/// Special values for braking action, see Table 0366 in Manual on
		/// Codes (WMO No. 306).
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

	/// Wave height or descriptive sea surface state.
	struct WaveHeight {
	public:
		/// Value status (which value was reported).
		enum class Type {
			STATE_OF_SURFACE,	///< Descriptive state of surface is reported.
			WAVE_HEIGHT,		///< Actual wave height is reported.
		};
		/// Wave height measurement units.
		enum class Unit {
			METERS,	///< Wave height in meters.
			FEET,	///< Wave height in feet.
		};
		/// State of sea surface, see Table 3700 in Manual on Codes (WMO No. 
		/// 306).
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
		WaveHeight() = default;
		/// @brief Decode sea wave height from string.
		/// @details The allowed formats are as follows.
		/// @par Letter S followed by single digit which means state of sea 
		/// surface or '/' which means 'not reported'. 
		/// @par Letter H followed by one to three digits which mean wave 
		/// height in decimeters or '///' which means 'not reported'.
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @par If descriptive State of Surface is reported then waveHeight 
		/// is initialised with maximum wave height for this state (except 
		/// Phenomenal where there's no maximum wave height and minimum value
		/// is used).
		/// @par The wave height unit is set to Meters; wave height is recalculated 
		/// from Decimeters to Meters if needed.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<WaveHeight> fromString(const std::string & s);
		/// Return value converted to certain measurment units if reported.
		/// @param unit Units to convert value to.
		/// @return Value converted to specified units (or empty optional if 
		/// value is not reported or conversion error occurred).
		inline std::optional<float> toUnit(Unit unit) const;
		/// @return Descriptive state of sea surface (or empty optional if 
		/// value is not reported) corresponding to wave height.
		inline StateOfSurface stateOfSurface() const;
		/// @return Wave height
		std::optional<float> waveHeight() const {
			if (!whValue.has_value()) return(std::optional<float>());
			return(whValue.value() * waveHeightDecimalPointShift);
		}
		/// @return Currently used measurement units.
		Unit unit() const { return(whUnit); }
		/// @return Status value.
		Type type() const { return (whType); }
	private:
		Type whType = Type::STATE_OF_SURFACE;
		std::optional<int> whValue; //in decimeters, muliply by 0.1 to get value in meters
		static const inline auto waveHeightDecimalPointShift = 0.1;
		static const Unit whUnit = Unit::METERS;
	private:
		static inline std::optional<float> waveHeightFromStateOfSurfaceString(
			const std::string & s);
	private:
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

	///////////////////////////////////////////////////////////////////////////

	/// @brief Group: Plain Text
	/// @details When the group could not be recognised to conform any of the 
	/// other group formats, it is saved as Plain Text.
	/// @par In this implementation maximum length of stored plain text is 
	/// limited to textMaxLength chars. All extra chars will be discarded.
	class PlainTextGroup {
	public:
		PlainTextGroup() = default;
		/// Initialise Plain Text group from string.
		/// @param s String to initialise from, if longer than textMaxLength,
		/// then only first textMaxLength characters stored.
		PlainTextGroup(const std::string & s) {
			strncpy(text, s.c_str(), textMaxLength); text[textMaxLength] = '\0';
		}
		/// @return Stored text value as a std::string.
		std::string toString() const { return(std::string(text)); }
		/// Maximum number of chars to be stored
		static const size_t textMaxLength = 15; 
		/// @return False only if empty string is stored, true otherwise.
		bool isValid() const { return(text[0]); }
		/// Attempt to parse group. See Group variant for details.
		/// @par This method is expected to always parse successfully and
		/// always returns a non-empty value.
		static std::optional<PlainTextGroup> parse(const std::string & group, 
			ReportPart reportPart)
		{
			(void)reportPart; return(PlainTextGroup(group));
		}
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		char text [textMaxLength + 1] = "\0";
	};

	/// @brief Group: fixed text.
	/// @details This group represents a fixed text which never varies in 
	/// reports.
	class FixedGroup {
	public:
		/// Group options for fixed texts.
		enum class Type {
			METAR = 0,	///< Report type: METAR.
			SPECI = 1,	///< Report type: SPECI.
			TAF = 2,	///< Report type: TAF.
			AMD = 3,	///< Amended TAF report.
			NIL = 4,	///< Missing report.
			CNL = 5,	///< Cancelled TAF report.
			COR = 6,	///< Correctional report.
			AUTO = 7,	///< Fully automatic report without human supervision or interaction.
			R_SNOCLO = 8,///< Aerodrome closed due to snow accumulation.
			CAVOK = 9,	///< Ceiling and visibility OK.
			NSW = 10,	///< No significant weather / significant weather phenomena ended.
			RMK = 11	///< Remarks to follow.
		};
		FixedGroup() = default;
		/// @return Type of fixed text in group.
		Type type() const { return(t); }
		/// @return Always returns true for this group.
		bool isValid() const { return(true); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<FixedGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		FixedGroup(Type type) :t (type) {}
		Type t; 
	};

	/// @brief Group: ICAO location.
	/// @details ICAO location where report applies.
	class LocationGroup {
	public:
		LocationGroup() = default;
		/// @return Stored location value as a std::string.
		std::string toString() const {return(std::string(location));}
		/// @return Always true; no way to init an invalid group currently.
		inline bool isValid() const { return(true); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<LocationGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		static const inline auto locationLength = 4; 
		char location [locationLength + 1] = "\0"; 
	};

	/// @brief Group: Report time.
	/// @details: Day and time when report was issued.
	class ReportTimeGroup {
	public:
		ReportTimeGroup() = default;
		const MetafTime & time() const { return(t); };
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { return(t.isValid() && t.day().has_value()); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<ReportTimeGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		MetafTime t;	
	};

	/// @brief Group: trend or forecast.
	/// @details Stores trend type and time if required.
	class TrendGroup {
	public:
		/// Trend type.
		enum class Type {
			NONE = 0,		///< Incomplete groups or their combination.
			NOSIG = 1,		///< No significant changes expected.
			BECMG = 2,		///< Weather conditions are expected to gradually change.
			TEMPO = 3,		///< Weather phenomena may arise for less than 60 minutes.
			INTER = 4,    	///< Weather phenomena may arise for less than 30 minutes.
			FROM = 5,		///< All previous weather conditions are superseded by the following.
			TIME_SPAN = 6,	///< Time span (TAF vailidity time or incomplete).
		};
		/// Trend or forecast probability.
		enum class Probability {
			NONE = 0,		///< Probability not specified.
			PROB_30 = 1,	///< Probability reported as 30%.
			PROB_40 = 2,	///< Probability reported as 40%.
		};
		TrendGroup() = default;
		/// @return Trend/forecast type.
		Type type() const { return(t); }
		/// @return Trend/forecast probablity.
		Probability probability() const { return(prob); }
		/// @return Time when trend/forecast expected to begin.
		const std::optional<MetafTime> & timeFrom() const { return(tFrom); }
		/// @return Time when trend/forecast expected to end.
		const std::optional<MetafTime> & timeTill() const { return(tTill); }
		/// @return Precise time when weather phenomena is expected.
		const std::optional<MetafTime> & timeAt() const { return(tAt); }
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const {
			if (tFrom.has_value() && !tFrom->isValid()) return(false);
			if (tTill.has_value() && !tTill->isValid()) return(false);
			if (tAt.has_value() && !tAt->isValid()) return(false);
			return (type() != Type::NONE); // Incomplete groups are considered invalid
		}
		/// @return True if the stored data represent a single time span group, 
		/// not combined with any other group; false otherwise.
		inline bool isTimeSpanGroup() const;
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<TrendGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// @brief Used to combine sequence of METAR or TAF groups which 
		/// represent the same data (e.g. BECMG 1108/1110, BECMG and 1108/1110 
		/// are technically different METAR/TAF groups but represent different 
		/// parts of the same trend information). See Group variant for details.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		TrendGroup(Type type) : t(type) {}
		TrendGroup(Probability p) : t(Type::NONE), prob(p) {}

		static inline std::optional<TrendGroup> fromTimeSpan(const std::string & s);
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

		Type t = Type::NONE;
		Probability prob = Probability::NONE;
		std::optional<MetafTime> tFrom; // Time span beginning.
		std::optional<MetafTime> tTill;	// Time span end time.
		std::optional<MetafTime> tAt;	// Precise time.
	};

	/// @brief Group: wind at ground level or wind shear.
	/// @details Specifies wind speed, direction and gust speed. For wind 
	/// additionall height where wind shear occurs. If variable wind direction 
	/// sector is given, sector 'begin' and 'end' directions are specified in 
	/// clockwise order.
	class WindGroup {
	public:
		WindGroup() = default;
		/// Checks if there's calm wind (i.e. no wind). Calm wind is encoded 
		/// as 00000KT or 00000MPS or 00000KMH.
		/// @return True if calm wind, false if the wind is observed. If wind 
		/// shear or variable wind sector are specified, always returns false.
		inline bool isCalm() const;
		/// @return True if stored data are consistent, false otherwise.
		inline bool isValid() const;
		/// @return Wind direction / where the wind is coming from (or not 
		/// reported value if the direction is not reported).
		const Direction & direction() const { return(windDir); }
		/// @return Wind speed (or not reported value).
		const Speed & windSpeed() const { return(wSpeed); }
		/// @return Wind gust speed (or not reported value).
		const Speed & gustSpeed() const { return(gSpeed); }
		/// @return Wind shear height (or not reported value for surface wind).
		const Distance & windShearHeight() const { return(wShHeight); }
		/// @return Variable wind sector 'begin' direction (or ommitted value 
		/// if no variable wind sector is reported).
		const Direction & varSectorBegin() const { return(vsecBegin); }
		/// @return Variable wind sector 'end' direction (or ommitted value if 
		/// no variable wind sector is reported).
		const Direction & varSectorEnd() const { return(vsecEnd); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<WindGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// @brief Used to combine sequence of METAR or TAF groups which 
		/// represent the same data (e.g. 2701012KT 260V280, 2701012KT and 
		/// 260V280 are technically different METAR/TAF groups but represent 
		/// different parts of the wind information). See Group variant for 
		/// details.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Direction windDir;
		Speed wSpeed;
		Speed gSpeed;
		Distance wShHeight;
		Direction vsecBegin;
		Direction vsecEnd;

		inline bool isSurfaceWindGroup() const;
		inline bool isWindSectorGroup() const;
	};

	/// @brief Group: prevailing or directional visibility.
	/// @details Specifies prevailing visibility or visibility toward certain 
	/// cardinal direction.
	class VisibilityGroup {
	public:
		VisibilityGroup() = default;
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { 
			return(!incompleteInteger && vis.isValid() && dir.isValid());
		}
		/// @return Visibility value.
		const Distance & visibility() const { return(vis); }
		/// @return Direction value or Ommitted value if not specified.
		const Direction & direction() const { return(dir); }
		/// Attempt to parse group. See Group variant for details.
		inline static std::optional<VisibilityGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// @brief Used to combine sequence of METAR or TAF groups which 
		/// represent the same data (e.g. 1 1/2SM, 1 and 1/2SM are technically 
		/// different METAR/TAF groups but represent different parts of the 
		/// same visibility value). See Group variant for details.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Distance vis;
		Direction dir;
		bool incompleteInteger = false; 
	};

	/// @brief Group: Сloud layer or cloud conditions or vertical visibility.
	/// @details Reports height, amount (cover) and type of a cloud layer, or
	/// vertical visibility if sky is obscured (e.g. due to fog). Alternatively
	/// specifies conditions such as Clear Sky, No Significant Clouds, etc.
	class CloudGroup {
	public:
		/// Amount (cover) of clouds.
		enum class Amount {
			NOT_REPORTED = 0,///< Amount not reported.
			NCD = 1,		///< No cloud detected (no cloud or sensor malfunction).
			NSC = 2,		///< No significant clouds.
			NONE_CLR = 3,	///< No clouds / sky clear (reported as CLR).
			NONE_SKC = 4,	///< No clouds / sky clear (reported as SKC).
			FEW = 5,		/// Few clouds (1/8 to 2/8 cover).
			SCATTERED = 6,	/// Scattered clouds (3/8 to 4/8 cover).
			BROKEN = 7,		/// Broken clouds (5/8 to 7/8 cover).
			OVERCAST = 8,	/// Overcast (8/8 cover).
			OBSCURED = 9	/// Sky obscured, vertical visibility given instead.
		};
		/// Significant cloud type.
		enum class Type {
			NOT_REPORTED = 0,		///< Cloud type not reported.
			NONE = 1,				///< No significant (convective) cloud observed.
			TOWERING_CUMULUS = 2,	///< Towering cumulus.
			CUMULONIMBUS = 3		///< Cumulonimbus.
		};
		CloudGroup () = default;
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { return(heightOrVertVis.isValid()); }
		/// @return Cloud amount (cover).
		Amount amount() const { return(amnt); }
		/// @return Convective type.
		Type type() const { return(tp); }
		/// @return Cloud cover height. If height is not reported or not 
		/// applicable to current group (e.g. sky clear or vertical visibility), 
		/// a non-reported distance is returned.
		inline const Distance & height() const;
		/// @return Vertical visibility value. If vertical visibility is not 
		/// reported or amount is other than OBSCURED, a non-reported distance 
		/// is returned.
		const Distance & verticalVisibility() const {
			if (amount() != Amount::OBSCURED) return(heightNotReported);
			return(heightOrVertVis);
		}
		/// Attempt to parse group. See Group variant for details.
		/// @details The allowed formats are as follows.
		/// @par Cloud cover type FEW(few) or SCT(scattered) or BKN(broken) or 
		/// OVC (overcase) or /// (cloud cover not reported), followed by 
		/// three digits of cloud base height in hundreds of feet (or /// if 
		/// height is not reported) followed by optional TCU (towering 
		/// cumulus) or CB (cumulonimbus) or /// (convective cloud type not 
		/// reported).
		/// @par VV followed by three digits of vertical visibility in 
		/// hundreds of feet or /// if vertical visibility is not reported.
		/// @par One of the following strings: CLR (sky clear, automated 
		/// reports) or SKC (sky clear, human-produced report) or NSC (no 
		/// significant clouds) or NCD (no clouds detected).
		/// @par If the input string does not follow this format, an empty 
		/// optional is returned.
		/// @param s Input string to decode.
		/// @return Decoded value or empty optional if format error occurred.
		static inline std::optional<CloudGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Amount amnt = Amount::NOT_REPORTED;
		Distance heightOrVertVis;
		Type tp = Type::NONE;
		static const inline auto heightNotReported = Distance(Distance::Unit::FEET);

		CloudGroup(Amount a) : amnt(a) {}
		static inline std::optional<Amount> amountFromString(const std::string & s);
		static inline std::optional<Type> typeFromString(const std::string & s);
	};

	/// @brief Group: weather conditions or their combination.
	/// @details Reports obscuration, precipitation or other weather phenomena.
	class WeatherGroup {
	public:
		/// Intensity or proximity
		enum class Qualifier {
			/// No qualifier (for precipitation MODERATE is used instead).
			NONE,	
			/// Recent weather is reported.
			RECENT,
			/// Weather phenomena in vicinity (up to 10 miles) is reported.
			VICINITY,	
			/// Light (precipitation).
			LIGHT,		
			/// Moderate (used with precipitation instead of NONE). Not used 
			/// with Hail/Small Hail (unless they are accompanied by rain or 
			/// other precipitation where Moderate qualifier applies), Ice 
			/// Crystals. Not used with Low Drifting Snow and Blowing Snow.
			MODERATE,	
			/// Heavy (precipitation). Also used with Funnel Cloud and means
			/// Tornado or Waterspout. Also used with Sandstorm/Duststorm and  
			/// means a heavy storm obscuring visibility to <5/16 miles.
			HEAVY		
		};
		/// Descriptor
		enum class Descriptor {
			/// No descriptor
			NONE,
			/// Shallow (fog), having little vertical extent (less than 6 feet).
			SHALLOW,
			/// Partial (fog), covering part of the aerodrome.
			PARTIAL,
			/// Patches (of fog) randomly covering the aerodrome.
			PATCHES,
			/// Dust, snow or sand raised by wind to height less than 6 feet.
			LOW_DRIFTING,
			/// Dust, snow or sand raised by wind to height 6 feet or more.
			BLOWING,
			/// Precipitation characterised by sudden start or stop or change 
			/// of intensity.
			SHOWERS,
			/// Thunderstorm (accompanied by lightning or thunder).
			THUNDERSTORM,
			/// Precipitation forming ice glase on ground or fog at freezing 
			/// temperatures (regardless of whether it actually deposits rime).
			FREEZING
		};
		/// Obscuration, precipitation or other weather phenomena.
		enum class Weather {
			NOT_REPORTED,
			/// Precipitation of very fine water droplets (falls to the ground 
			/// unlike fog and mist).
			DRIZZLE,
			/// Precipitation in form of water drops or droplets.
			RAIN,
			/// Precipitation in form of branched (unlike 'diamond dust') ice 
			/// crystals.
			SNOW,
			/// Similar to graupel (small hail) but smaller in size. Very 
			/// small, opaque, white grains of ice. Snow grains normally neither 
			/// bounce off the ground nor break when falling to the ground.
			SNOW_GRAINS,
			/// 'Diamond Dust'. Non-branched (unlike snow) ice crystals in form of needles, 
			/// columns or plates.
			ICE_CRYSTALS,
			/// Translucent pellets of ice (in form of frozen raindrops or 
			/// melted and refrozen snowflakes) or snow pellets encased in 
			/// thin ice layer. Diameter up to 5 mm.
			ICE_PELLETS,
			/// Small balls or other pieces of ice, can be frozen together in
			/// irregular lumps. Diameter more than 5 mm.
			HAIL,
			/// Graupel. White, small, opaque grains of ice. Diameter 2 to 5 mm.
			SMALL_HAIL,
			/// Precipitation detected by automated station but precipitation 
			/// type cannot be determined.
			UNDETERMINED,
			/// Water particles suspended in atmosphere, visibility from 5/8 
			/// to 7 statute miles (unlike drizzle does not fall on ground).
			MIST,
			/// Water particles suspended in atmosphere, visibility less than
			/// 5/8 statute miles (unlike drizzle does not fall on ground).
			FOG,
			/// Smoke. Suspension of combustion-produced particles in the air.
			SMOKE,
			/// Fine particles of rock powder originating from volcano 
			/// suspended in atmosphere. May remain suspended for long periods.
			VOLCANIC_ASH,
			/// Widespread dust. Dust particles suspended in the air by the wind.
			DUST,
			/// Sand particles raised by wind.
			SAND,
			/// Suspension of very small dry particles invisible to naked eye.
			HAZE,
			/// Ensemble of water droplets torn by wind from surface of body 
			/// of water (generally from crests of waves).
			SPRAY,
			/// Dust devils. Well developen dust or sand whirls.
			DUST_WHIRLS,
			/// Suddenly increasing strong wind (increases by at least 16 
			/// knots and sustains at 22 knots for at least 1 minute).
			SQUALLS,
			/// Tornadic activity. Tornado, funnel cloud or waterspout.
			FUNNEL_CLOUD,
			/// Particles of sand carried aloft by a strong wind.
			SANDSTORM,
			/// Dust-filled air and strong winds.
			DUSTSTORM
		};
		WeatherGroup() = default;
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { return(true); } //TODO
		/// @return Weather phenomena qualifier (intensity or proximity).
		Qualifier qualifier() const { return(q); }
		/// @return Weather phenomena descriptor.
		Descriptor descriptor() const { return(d); }
		/// @return A vector of weather phenomena stored in group.
		std::vector<Weather> weather() const {
			std::vector<Weather> result;
			for (auto i = 0u; i < wSize; i++) result.push_back(w[i]);
			return(result);
		}
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<WeatherGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Qualifier q = Qualifier::NONE;
		Descriptor d = Descriptor::NONE;
		inline static const size_t maxwSize = 8;
		Weather w[maxwSize];
		size_t wSize = 0;

		inline bool isModerateQualifier() const;

		static inline std::optional<Qualifier> qualifierFromString(const std::string & s);
		static inline std::optional<Descriptor> descriptorFromString(const std::string & s);
		static inline std::optional<Weather> weatherFromString(const std::string & s);

		static inline WeatherGroup notReported();
		static inline WeatherGroup notReportedRecent();
	};

	/// @brief Group: temperature and dew point.
	/// @details Reports ambient air temperature and dew point.
	class TemperatureGroup {
	public:
		TemperatureGroup() = default;
		/// @return True if stored data are consistent, false otherwise.
		inline bool isValid() const;
		/// @return Temperature value (if reported).
		const Temperature & airTemperature() const { return(t); }
		/// @return Dewpoint value (if reported).
		const Temperature & dewPoint() const { return(dp); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<TemperatureGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Temperature t;
		Temperature dp;
	};

	/// @brief Group: forecast minimum or maximum temperature.
	/// @details Minimum or maximum temperature forecast with specified time 
	/// when minimum or maximum temperatures are expected.
	class TemperatureForecastGroup {
	public:
		/// Temperature point: minimum or maximum
		enum class Point {
			MINIMUM = 0,
			MAXIMUM = 1
		};
		TemperatureForecastGroup() = default;
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { return(tm.isValid()); }
		/// @return Temperature point.
		Point point() const { return(p); }
		/// @return Temperature value.
		const Temperature & airTemperature() const { return(t); }
		/// @return Time when the temperature value is expected.
		const MetafTime & time() const { return(tm); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<TemperatureForecastGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Point p = Point::MINIMUM;
		Temperature t;
		MetafTime tm;

		static std::optional<Point> pointFromString (const std::string & s) {
			if (s == "TX") return(Point::MAXIMUM);
			if (s == "TN") return(Point::MINIMUM);
			return(std::optional<Point>());
		}
	};

	/// @brief Group: atmospheric pressure.
	/// @details Reports current atmospheric pressure.
	class PressureGroup {
	public:
		PressureGroup() = default;
		/// @return Pressure value.
		const Pressure & atmosphericPressure() const { return(p); }
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { return(true); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<PressureGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Pressure p;
	};

	/// @brief Group: runway visual range.
	/// @details Runway visual range is an instrumentally-derived value 
	/// that represents the horizontal distance a pilot may see down the
	/// runway from the approach end.
	class RunwayVisualRangeGroup {
	public:
		/// Runway Visual Range trend.
		enum class Trend {
			NONE,			///< Trend not specified.
			NOT_REPORTED, 	///< Trend is specified as not reported.
			UPWARD,			///< Trend is upward.
			NEUTRAL,		///< Trend is neutral.
			DOWNWARD		///< Trend is downward.
		};
		RunwayVisualRangeGroup() = default;
		/// @return Runway the visual range is reported for.
		const Runway & runway() const { return(rw); }
		/// @return Visual range value.
		const Distance & visualRange() const { return(visRange); }
		/// @return Variable visual range value (if reported).
		const Distance & variableVisualRange() const { return(varVisRange); }
		/// @return Visual range trend.
		Trend trend() const { return(trnd); }
		/// @return True if variable visual range is reported, false otherwise.
		bool isVariableVisualRange() const { return(varVisRange.isReported()); }
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { return(visRange.isValid() && varVisRange.isValid()); };
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<RunwayVisualRangeGroup> parse(const std::string & group,
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Runway rw;
		Distance visRange;
		Distance varVisRange;
		Trend trnd;
	private:
		static inline std::optional<Trend> trendFromString(const std::string & s);
	};

	/// @brief Group: runway state.
	/// @details Runway state, deposits, extent of contamination, deposit 
	/// depth and surface friction.
	class RunwayStateGroup {
	public:
		/// Runway status: Normal, Contamination ceased to exist, Closed due 
		/// to snow accumulation.
		enum class Status {
			NORMAL,	///< Normal runway state reported.
			CLRD, 	///< Runway contaminations ceased to exist.
			SNOCLO  ///< Runway is closed due to snow accumulation
		};
		/// Deposits type, see Table 0919 in Manual on Codes (WMO No. 306).
		enum class Deposits {
			CLEAR_AND_DRY, 				// 0
			DAMP, 						// 1
			WET_AND_WATER_PATCHES,		// 2
			RIME_AND_FROST_COVERED,		// 3
			DRY_SNOW,					// 4
			WET_SNOW,					// 5
			SLUSH,						// 6
			ICE,						// 7
			COMPACTED_OR_ROLLED_SNOW,   // 8
			FROZEN_RUTS_OR_RIDGES,      // 9
			NOT_REPORTED                // /
		};
		/// Extent of runway contamination, see Table 0519 in Manual on Codes 
		/// (WMO No. 306).
		enum class Extent {
			NONE,					// 0
			LESS_THAN_10_PERCENT,	// 1
			FROM_11_TO_25_PERCENT,	// 2
			RESERVED_3,				// 3
			RESERVED_4,				// 4
			FROM_26_TO_50_PERCENT,	// 5
			RESERVED_6,				// 6
			RESERVED_7,				// 7
			RESERVED_8,				// 8
			MORE_THAN_51_PERCENT,	// 9
			NOT_REPORTED			// /
		};
		RunwayStateGroup() = default;
		/// @return Runway for which the state is reported.
		const Runway & runway() const { return(rw); }
		/// @return Status of the runway.
		Status status() const { return(st); }
		/// @return Deposits type.
		Deposits deposits() const { return(dp); }
		/// @return Extent of runway contamination.
		Extent contaminationExtent() const { return(ext); }
		/// @return Depth of deposits.
		const Precipitation & depositDepth() const { return(dDepth); }
		/// @return Surface friction of runway.
		const SurfaceFriction & surfaceFriction() const { return(sf); }
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { return(true); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<RunwayStateGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Runway rw;
		Status st = Status::NORMAL;
		Deposits dp = Deposits::NOT_REPORTED;
		Extent ext = Extent::NOT_REPORTED;
		Precipitation dDepth;
		SurfaceFriction sf;

		static inline RunwayStateGroup runwaySnoclo(Runway runway);
		static inline RunwayStateGroup runwayClrd(Runway runway, 
			SurfaceFriction surfaceFriction);

		static inline std::optional<Deposits> depositsFromString(const std::string & s);
		static inline std::optional<Extent> extentFromString(const std::string & s);
	};

	/// @brief Group: rainfall.
	/// @details Actually measured rainfall for various periods (Australia only?).
	class RainfallGroup {
	public:
		RainfallGroup() = default;
		/// @return Rainfall for last 10 minutes.
		const Precipitation & rainfallLast10Minutes() const { return(last10m); }
		/// @return Rainfall for last 60 minutes.
		const Precipitation & rainfallLast60Minutes() const { return(last60m); }
		/// @return Rainfall since 09:00 (9AM)
		const Precipitation & rainfallSince9AM() const { return(since0900); }
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { return(true); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<RainfallGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Precipitation last10m;
		Precipitation since0900;
		Precipitation last60m;
	};

	/// @brief Group: state of sea surface.
	/// @details Temperature of sea surface and state of surface / wave height 
	/// (oil platforms only?).
	class SeaSurfaceGroup {
	public:
		SeaSurfaceGroup() = default;
		/// @return Sea surface temperature.
		const Temperature & surfaceTemperature() const { return(t); }
		/// @return State of sea surface / wave height.
		const WaveHeight & waves() const { return(wh); }
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { return(true); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<SeaSurfaceGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Temperature t;
		WaveHeight wh;
	};

	/// @brief Group: Airfield colour state.
	/// @details Encoded visibility and cloud height (UK navy & RAF only?). 
	/// Code BLACK may indicate that airfield is closed due to reasons not 
	/// related to visibility and ceiling (e.g. snow accumulation).
	class ColourCodeGroup {
	public:
		enum class Code {
			/// Visibility >8000 m AND no cloud obscuring 3/8 or more below 2500 feet.
			BLUE = 0,	 
			/// Visibility >5000 m AND no cloud obscuring 3/8 or more below 1500 feet.
			WHITE = 1,
			/// Visibility >3700 m AND no cloud obscuring 3/8 or more below 700 feet.
			GREEN = 2,
			/// Visibility >2500 m AND no cloud obscuring 3/8 or more below 500 feet.
			YELLOW1 = 3,
			/// Visibility >1600 m AND no cloud obscuring 3/8 or more below 300 feet.
			YELLOW2 = 4,
			/// Visibility >800 m AND no cloud obscuring 3/8 or more below 200 feet.
			AMBER = 5,
			/// Visibility <800 m OR clouds obscuring 3/8 or more below 200 feet.
			RED = 6
		};
		ColourCodeGroup() = default;
		/// @return Colour code.
		Code code() const { return(c); }
		/// @return True if code Black reported additionally to main code.
		bool isCodeBlack() const { return(cBlack); }
		/// @return True if stored data are consistent, false otherwise.
		bool isValid() const { return(true); }
		/// Attempt to parse group. See Group variant for details.
		static inline std::optional<ColourCodeGroup> parse(const std::string & group, 
			ReportPart reportPart);
		/// This method is required by parser but not used for this group.
		inline std::optional<Group> combine(const Group & nextGroup) const;
	private:
		Code c = Code::BLUE;
		bool cBlack = false; //Is colour code BLACK additionally reported.
	};

	///////////////////////////////////////////////////////////////////////////////

	/// Returns SyntaxGroup classification of METAR or TAF group.
	/// @param group A parsed METAR or TAF group to classify.
	/// @return Corresponding SyntaxGroup or SyntaxGroup::OTHER if 
	/// group is not important for report syntax.
	inline SyntaxGroup getSyntaxGroup(const Group & group);

	/// @brief Attempts to parse all types within Group variant
	/// @details Picks each individual type from Group variant, constructs an 
	/// instance of this type and calls its parse() method. If group instance's
	/// parse() method returns true, the group instance is returned. If parse()
	/// method returns false, next type is picked and the process is repeated.
	/// @par PlainTextGroup::parse() is always called last, with no regard 
	/// of PlainTextGroup index within Group variant. Due to this, if none of
	/// the types within Group variant if able to recognise and parse the 
	/// string, it is saved as a PlainTextGroup.
	/// @par It is expected that all types within Group variant have the 
	/// following method, otherwise this class won't compile
	/// bool parse(const std::string & group, ReportPart reportPart)
	class GroupParser {
	public:
		/// Total number of groups in Group variant.
		static const auto groups_total = std::variant_size_v<Group>;
		/// Call parse() method from all types within Group variant
		/// @param group METAR or TAF group to be parsed
		/// @param reportPart Current syntaxic part of the report
		static Group parse(const std::string & group, ReportPart reportPart) {
			// Start cycling through types within Group variant
			return(ParseGroupHelper<0, std::variant_alternative_t<0, Group>>::parse(
				group, 
				reportPart));
		}
	private:
		/// @brief Helper for cycling through all types within Group variant.
		/// @details Performs a step of recursively cycling through types 
		/// within Group variant.
		/// @par If the current group is PlainTextGroup, it is skipped at this 
		/// moment because PlainTextGroup is only used when no other group type 
		/// is able to recognise and parse group string.
		/// @par A call to current type's parse() method is performed, if parse
		/// is successful (i.e. parse() returned true), the group initialised 
		/// by parse() method is returned and recursion is terminated.
		/// If parse using current type was not successful (i.e. parse() 
		/// returned false) then next type within Group variant is processed
		/// recursively.
		/// @tparam I Index of currently processed type within Group variant.
		/// @tparam T Currently processed type within Group variant.
		template <size_t I, class T> 
		struct ParseGroupHelper {
			static inline Group parse(const std::string & group, ReportPart reportPart) {
				// Skip PlainTextGroup regardless of its position, it will be 
				// used if everything else fails (i.e. no type can recognise a 
				// string).
				if (!std::is_same<T, PlainTextGroup>::value) {
					auto temporaryGroup = T::parse(group, reportPart);
					if (temporaryGroup.has_value()) return(temporaryGroup.value());
				}
				// Pick next type within Group variant and repeat recursively.
				return(ParseGroupHelper<I+1, std::variant_alternative_t<I+1, Group>>::parse(
					group, 
					reportPart));
			}
		};

		/// @brief Helper for cycling through all types within Group variant.
		/// @details Terminates recursive cycling group variant.
		/// @par Attempts to parse group string with last type in Group 
		/// variant, if parsing is successful, the group struct initialised by
		/// parse() is returned; otherwise, group is parsed by PlainTextGroup,
		/// which is guaranteed to succeed and just stores the group string.
		/// In any case, recursion is terminated.
		/// @tparam T Last listed type within Group variant.
		template <class T> 
		struct ParseGroupHelper<groups_total-1, T> {
			static inline Group parse(const std::string & group, ReportPart reportPart) {
				// Attempt to parse last type in Group; if last type is 
				// PlainTextGroup then temporaryGroup is guaranteed to have 
				// value anyway. If last type is not PlainTextGroup it may 
				// return an empty optional, then parsing using PlainTextGroup
				// is attempted.
				auto temporaryGroup = T::parse(group, reportPart);
				if (temporaryGroup.has_value()) return(temporaryGroup.value());
				// At this point no type within Group variant was able to 
				// recognise the group string.
				// In this case the group is parsed as PlainTextGroup.
				return(PlainTextGroup(group));
			}
		};
	};

	/// Report type detected by parser
	enum class ReportType {
		UNKNOWN = 0,	///< Report type not detected
		METAR = 1,		///< Report type detected as METAR
		TAF = 2			///< Report type detected as TAF
	};

	/// @brief Parses METAR and TAF reports.
	/// @details The report is provided as std::string; the parser splits it 
	/// into groups (tokens separated by spaces or line breaks), and parses 
	/// each group individually by passing group string to parse() method of 
	/// each type included Group variant (cycling through Group variant types 
	/// is done using GroupParser). If a call to parse() method returned true,
	/// the relevant group is stored in a vector. If no parse() call returned 
	/// true (that is, no particular group type was able to recognise the 
	/// string), this group is stored as PlainTextGroup.
	/// @par The report type and structure is detected during parsing, using 
	/// a simple state machine. See enum State for list of states, transition()
	/// for algorithm of transition between states, and finalTransition() for
	/// checking whether report end is allowed for current state.
	/// @par Parsing results are obtained via getResult() method which returns
	/// a reference to vector of Group. If it is desirable to reset 
	/// 
	class Parser {
	public:
		/// Errors which might occur when parsing METAR or TAF report
		enum class Error {
			NONE = 0,							///< No error
			EMPTY_REPORT = 1,					///< Empty report supplied
			EXPECTED_REPORT_TYPE_OR_LOCATION = 2,///< Report must start with type or location
			EXPECTED_LOCATION = 3,				///< Expected location here
			EXPECTED_REPORT_TIME = 4,			///< Expected report time here
			EXPECTED_TIME_SPAN = 5,				///< Expected time span here
			UNEXPECTED_REPORT_END = 6,			///< Report ends unexpectedly
			UNEXPECTED_GROUP_AFTER_NIL = 7,		///< NIL report must be empty
			UNEXPECTED_GROUP_AFTER_CNL = 8,		///< CNL report must be empty
			UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY = 9,///< NIL or CNL not allowed in report body
			AMD_ALLOWED_IN_TAF_ONLY = 10,		///< AMD only used in TAF reports
			CNL_ALLOWED_IN_TAF_ONLY = 11,		///< CNL only used in TAF reports
			INTERNAL_PARSER_STATE = 12			///< Internal error: invalid parser state
		};
		/// @brief Parse METAR or TAf report.
		/// @details Parses METAR or TAF report, autodetects its type and 
		/// verifies syntax.
		/// @par Use getResult(), getReportType() and getError() to get parse 
		/// results.
		/// @param report Raw report string.
		/// @param keepSourceGroup When true, each group string being parsed 
		/// is kept in a vector. After parsing this vector can be acquired via 
		/// getSourceGroups(). When false, group string are not kept to 
		/// maximise performance and getSourceGroups() will return an empty 
		/// vector.
		/// @return True if no error occurred during parsing, false if an error
		/// did occurr during parsing.
		inline bool parse(const std::string & report, bool keepSourceGroup = false);
		/// Get parsing result.
		/// @return Vector of Group which contains complete report (if parsed
		/// successfully) or part of the report (to the point where an error 
		/// occurred).
		const std::vector<Group> & getResult() const { return(result); }
		/// Get source group strings memorised during parsing.
		/// @return Vector of string which contains all parsed source groups 
		/// (if parsed successfully) or part of the report (to the point where 
		/// an error occurred).
		const std::vector<std::string> & getSourceGroups() const { return(sourceGroups); }
		/// @brief Reset parsing result and free memory.
		/// @details After calling this method, getResult() and 
		/// getSourceGroups() will return empty vectors, getReportType() will 
		/// return ReportType::UNKNOWN, and getError() will return 
		/// Parser::Error::NONE.
		inline void resetResult();
		/// Get report type autodetected during parsing.
		/// @return Autodetected type of report (METAR or TAF or UNKNOWN if 
		/// it was impossible to determine report type).
		ReportType getReportType() const { return(reportType); }
		/// Get error occurred during parsing.
		/// @return Parsing error or Parser::Error::NONE if report was parsed 
		/// successfully.
		Error getError() const { return(error); }
	private:
		std::vector<Group> result;
		std::vector<std::string> sourceGroups;
		ReportType reportType = ReportType::UNKNOWN;
		Error error = Error::NONE;

		/// States of state machine used to check syntax of METAR/TAF reports.
		enum class State {
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
			REMARK,
			NIL,
			CNL,
			ERROR
		};

		/// Transition between parser state machine states.
		inline State transition(State state, SyntaxGroup group);
		/// Check if report end is allowed in current state.
		/// If report ended unexpectedly, state transitions to State::ERROR.
		inline State finalTransition(State state);
		/// Helper method to transition to Error state during parsing.
		inline State parseError(Error e) { error = e; return(State::ERROR); }
		/// Derive metaf:ReportPart
		static inline ReportPart reportPartFromState(State state);
	};

	///////////////////////////////////////////////////////////////////////////////

	/// @brief Visitor is intended for further processing of METAR or TAF 
	/// report information decoded by parser.
	/// @details To reduce amount of boilerplate, inherit from GroupVisitor
	/// and implement all individual virtual methods for each group type.
	/// @tparam T Type returned by visitor methods or void if visitor methods
	/// do not need to return a value.
	template <typename T>
	class GroupVisitor {
	public:
		inline T visit(const Group & group);
	protected:
		virtual T visitPlainTextGroup(const PlainTextGroup & group) = 0;
		virtual T visitFixedGroup(const FixedGroup & group) = 0;
		virtual T visitLocationGroup(const LocationGroup & group) = 0;
		virtual T visitReportTimeGroup(const ReportTimeGroup & group) = 0;
		virtual T visitTrendGroup(const TrendGroup & group) = 0;
		virtual T visitWindGroup(const WindGroup & group) = 0;
		virtual T visitVisibilityGroup(const VisibilityGroup & group) = 0;
		virtual T visitCloudGroup(const CloudGroup & group) = 0;
		virtual T visitWeatherGroup(const WeatherGroup & group) = 0;
		virtual T visitTemperatureGroup(const TemperatureGroup & group) = 0;
		virtual T visitTemperatureForecastGroup(const TemperatureForecastGroup & group) = 0;
		virtual T visitPressureGroup(const PressureGroup & group) = 0;
		virtual T visitRunwayVisualRangeGroup(const RunwayVisualRangeGroup & group) = 0;
		virtual T visitRunwayStateGroup(const RunwayStateGroup & group) = 0;
		virtual T visitRainfallGroup(const RainfallGroup & group) = 0;
		virtual T visitSeaSurfaceGroup(const SeaSurfaceGroup & group) = 0;
		virtual T visitColourCodeGroup(const ColourCodeGroup & group) = 0;
		virtual T visitOther(const Group & group) = 0;
	};

	/// General value-returning visitor implementation.
	/// @tparam T Type returned by visitor methods.
	template <typename T>
	inline T GroupVisitor<T>::visit(const Group & group) {
		if (std::holds_alternative<PlainTextGroup>(group)) {
			return(this->visitPlainTextGroup(std::get<PlainTextGroup>(group)));
		}
		if (std::holds_alternative<FixedGroup>(group)) {
			return(this->visitFixedGroup(std::get<FixedGroup>(group)));
		}
		if (std::holds_alternative<LocationGroup>(group)) {
			return(this->visitLocationGroup(std::get<LocationGroup>(group)));
		}
		if (std::holds_alternative<ReportTimeGroup>(group)) {
			return(this->visitReportTimeGroup(std::get<ReportTimeGroup>(group)));
		}
		if (std::holds_alternative<TrendGroup>(group)) {
			return(this->visitTrendGroup(std::get<TrendGroup>(group)));
		}
		if (std::holds_alternative<WindGroup>(group)) {
			return(this->visitWindGroup(std::get<WindGroup>(group)));
		}
		if (std::holds_alternative<VisibilityGroup>(group)) {
			return(this->visitVisibilityGroup(std::get<VisibilityGroup>(group)));
		}
		if (std::holds_alternative<CloudGroup>(group)) {
			return(this->visitCloudGroup(std::get<CloudGroup>(group)));
		}
		if (std::holds_alternative<WeatherGroup>(group)) {
			return(this->visitWeatherGroup(std::get<WeatherGroup>(group)));
		}
		if (std::holds_alternative<TemperatureGroup>(group)) {
			return(this->visitTemperatureGroup(std::get<TemperatureGroup>(group)));
		}
		if (std::holds_alternative<TemperatureForecastGroup>(group)) {
			return(this->visitTemperatureForecastGroup(std::get<TemperatureForecastGroup>(group)));
		}
		if (std::holds_alternative<PressureGroup>(group)) {
			return(this->visitPressureGroup(std::get<PressureGroup>(group)));
		}
		if (std::holds_alternative<RunwayVisualRangeGroup>(group)) {
			return(this->visitRunwayVisualRangeGroup(std::get<RunwayVisualRangeGroup>(group)));
		}
		if (std::holds_alternative<RunwayStateGroup>(group)) {
			return(this->visitRunwayStateGroup(std::get<RunwayStateGroup>(group)));
		}
		if (std::holds_alternative<RainfallGroup>(group)) {
			return(this->visitRainfallGroup(std::get<RainfallGroup>(group)));
		}
		if (std::holds_alternative<SeaSurfaceGroup>(group)) {
			return(this->visitSeaSurfaceGroup(std::get<SeaSurfaceGroup>(group)));
		}
		if (std::holds_alternative<ColourCodeGroup>(group)) {
			return(this->visitColourCodeGroup(std::get<ColourCodeGroup>(group)));
		}
		return(this->visitOther(group));
	}

	/// Non-value-returning visitor implementation.
	/// Partial template specialisation for template type void, where visitor 
	/// methods do not return a value.
	template<>
	inline void GroupVisitor<void>::visit(const Group & group) {
		if (std::holds_alternative<PlainTextGroup>(group)) {
			this->visitPlainTextGroup(std::get<PlainTextGroup>(group));
			return;
		}
		if (std::holds_alternative<FixedGroup>(group)) {
			this->visitFixedGroup(std::get<FixedGroup>(group));
			return;
		}
		if (std::holds_alternative<LocationGroup>(group)) {
			this->visitLocationGroup(std::get<LocationGroup>(group));
			return;
		}
		if (std::holds_alternative<ReportTimeGroup>(group)) {
			this->visitReportTimeGroup(std::get<ReportTimeGroup>(group));
			return;
		}
		if (std::holds_alternative<TrendGroup>(group)) {
			this->visitTrendGroup(std::get<TrendGroup>(group));
			return;
		}
		if (std::holds_alternative<WindGroup>(group)) {
			this->visitWindGroup(std::get<WindGroup>(group));
			return;
		}
		if (std::holds_alternative<VisibilityGroup>(group)) {
			this->visitVisibilityGroup(std::get<VisibilityGroup>(group));
			return;
		}
		if (std::holds_alternative<CloudGroup>(group)) {
			this->visitCloudGroup(std::get<CloudGroup>(group));
			return;
		}
		if (std::holds_alternative<WeatherGroup>(group)) {
			this->visitWeatherGroup(std::get<WeatherGroup>(group));
			return;
		}
		if (std::holds_alternative<TemperatureGroup>(group)) {
			this->visitTemperatureGroup(std::get<TemperatureGroup>(group));
			return;
		}
		if (std::holds_alternative<TemperatureForecastGroup>(group)) {
			this->visitTemperatureForecastGroup(std::get<TemperatureForecastGroup>(group));
			return;
		}
		if (std::holds_alternative<PressureGroup>(group)) {
			this->visitPressureGroup(std::get<PressureGroup>(group));
			return;
		}
		if (std::holds_alternative<RunwayVisualRangeGroup>(group)) {
			this->visitRunwayVisualRangeGroup(std::get<RunwayVisualRangeGroup>(group));
			return;
		}
		if (std::holds_alternative<RunwayStateGroup>(group)) {
			this->visitRunwayStateGroup(std::get<RunwayStateGroup>(group));
			return;
		}
		if (std::holds_alternative<RainfallGroup>(group)) {
			this->visitRainfallGroup(std::get<RainfallGroup>(group));
			return;
		}
		if (std::holds_alternative<SeaSurfaceGroup>(group)) {
			this->visitSeaSurfaceGroup(std::get<SeaSurfaceGroup>(group));
			return;
		}
		if (std::holds_alternative<ColourCodeGroup>(group)) {
			this->visitColourCodeGroup(std::get<ColourCodeGroup>(group));
			return;
		}
		this->visitOther(group);
	}

}; //namespace metaf

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

namespace metaf {

	std::optional<Runway> Runway::fromString(const std::string & s) {
		static const std::optional<Runway> error;
		static const std::regex rgx("R(\\d\\d)([RLC])?");
		static const auto matchNumber = 1, matchDesignator = 2;
		std::smatch match;
		if (!std::regex_match(s, match, rgx)) return(error);
		Runway runway;
		runway.rNumber = static_cast<unsigned int>(std::stoi(match.str(matchNumber)));
		if (match.length(matchDesignator)) {
			const auto designator = designatorFromChar(match.str(matchDesignator).at(0));
			if (!designator.has_value()) return(error);
			runway.rDesignator = designator.value();
		}
		return(runway);
	}

	std::optional<Runway::Designator> Runway::designatorFromChar(char c) {
		switch (c) {
			case 'R': return(Designator::RIGHT);
			case 'C': return(Designator::CENTER);
			case 'L': return(Designator::LEFT);
			default: return(std::optional<Designator>());
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<MetafTime> MetafTime::fromStringDDHHMM(const std::string & s) {
		static const std::regex rgx ("(\\d\\d)?(\\d\\d)(\\d\\d)");
		static const auto matchDay = 1, matchHour = 2, matchMinute = 3;
		std::smatch match;
		if (!std::regex_match(s, match, rgx)) return(std::optional<MetafTime>());
		MetafTime metafTime;
		if (match.length(matchDay)) {
			metafTime.dayValue = static_cast<unsigned int>(std::stoi(match.str(matchDay)));
		}
		metafTime.hourValue = static_cast<unsigned int>(std::stoi(match.str(matchHour)));
		metafTime.minuteValue = static_cast<unsigned int>(std::stoi(match.str(matchMinute)));
		return(metafTime);
	}

	std::optional<MetafTime> MetafTime::fromStringDDHH(const std::string & s) {
		static const std::regex rgx ("(\\d\\d)(\\d\\d)");
		std::smatch match;
		if (!regex_match(s, match, rgx)) return(std::optional<MetafTime>());
		MetafTime metafTime;
		static const auto matchDay = 1;
		metafTime.dayValue = static_cast<unsigned int>(std::stoi(match.str(matchDay)));
		static const auto matchHour = 2;
		metafTime.hourValue = static_cast<unsigned int>(std::stoi(match.str(matchHour)));
		metafTime.minuteValue = 0;
		return(metafTime);
	}

	bool MetafTime::isValid() const {
		if (auto d = dayValue.value_or(maxDay); d > maxDay || !d) return(false);
		if (hourValue > maxHour) return(false);
		if (minuteValue > maxMinute) return(false);
		return(true);
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<Temperature> Temperature::fromString(const std::string & s) {
		static const std::regex rgx ("(?:(M)?(\\d\\d))|//");
		static const auto matchM = 1, matchValue = 2;
		std::smatch match;
		if (!std::regex_match(s, match, rgx)) return(std::optional<Temperature>());
		Temperature temperature;
		if (!match.length(matchValue)) return(temperature);
		auto value = std::stoi(match.str(matchValue));
		if (match.length(matchM)) {
			value = -value;
			temperature.freezing = true;
		}
		temperature.tempValue = value;
		return(temperature);
	}

	std::optional<float> Temperature::toUnit(Unit unit) const {
		std::optional<float> error;
		auto v = temperature();
		if (!v.has_value()) return(error);
		if (tempUnit == unit) return(v);
		if (tempUnit == Unit::C && unit == Unit::F) return(v.value() * 1.8 + 32);
		return(error);
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<Speed> Speed::fromString(const std::string & s, Unit unit) {
		if (s.empty()) return(Speed());
		static const std::regex rgx ("([1-9]?\\d\\d)|//");
		static const auto matchValue = 1;
		std::smatch match;
		if (!std::regex_match(s, match, rgx)) return(std::optional<Speed>());
		Speed speed;
		speed.speedUnit = unit;
		if (match.length(matchValue)) {
			speed.speedValue = static_cast<unsigned int>(std::stoi(match.str(matchValue)));
		}
		return(speed);
	}

	std::optional<float> Speed::toUnit(Unit unit) const {
		if (!speedValue.has_value()) return(std::optional<float>());
		switch (speedUnit) {
			case Unit::KNOTS:				return(knotsToUnit(speedValue.value(), unit));
			case Unit::METERS_PER_SECOND:	return(mpsToUnit(speedValue.value(), unit));
			case Unit::KILOMETERS_PER_HOUR:	return(kmhToUnit(speedValue.value(), unit));
			case Unit::MILES_PER_HOUR:		return(mphToUnit(speedValue.value(), unit));
			default:						return(std::optional<float>());
		}
	}

	std::optional<Speed::Unit> Speed::unitFromString(const std::string & s) {
		if (s == "KT") return(Speed::Unit::KNOTS);
		if (s == "MPS") return(Speed::Unit::METERS_PER_SECOND);
		if (s == "KMH") return(Speed::Unit::KILOMETERS_PER_HOUR);
		return(std::optional<Speed::Unit>());
	}

	std::optional<float> Speed::knotsToUnit(float valueKnots, Unit otherUnit) {
		switch(otherUnit) {
			case Unit::KNOTS: 				return(valueKnots);
			case Unit::METERS_PER_SECOND: 	return(valueKnots * 0.514444);
			case Unit::KILOMETERS_PER_HOUR:	return(valueKnots * 1.852);
			case Unit::MILES_PER_HOUR:		return(valueKnots * 1.150779);
			default:						return(std::optional<float>());
		}
	}

	std::optional<float> Speed::mpsToUnit(float valueMps, Unit otherUnit) {
		switch(otherUnit) {
			case Unit::KNOTS: 				return(valueMps * 1.943844);
			case Unit::METERS_PER_SECOND: 	return(valueMps);
			case Unit::KILOMETERS_PER_HOUR:	return(valueMps * 3.6);
			case Unit::MILES_PER_HOUR:		return(valueMps * 2.236936);
			default:						return(std::optional<float>());
		}
	}

	std::optional<float> Speed::kmhToUnit(float valueKmh, Unit otherUnit) {
		switch(otherUnit) {
			case Unit::KNOTS: 				return(valueKmh / 1.852);
			case Unit::METERS_PER_SECOND:	return(valueKmh / 3.6);
			case Unit::KILOMETERS_PER_HOUR:	return(valueKmh);
			case Unit::MILES_PER_HOUR:		return(valueKmh * 0.621371);
			default:						return(std::optional<float>());
		}
	}

	std::optional<float> Speed::mphToUnit(float valueMph, Unit otherUnit) {
		switch(otherUnit) {
			case Unit::KNOTS: 				return(valueMph * 0.868976);
			case Unit::METERS_PER_SECOND: 	return(valueMph * 0.44704);
			case Unit::KILOMETERS_PER_HOUR:	return(valueMph * 1.609344);
			case Unit::MILES_PER_HOUR:		return(valueMph);
			default:						return(std::optional<float>());
		}
	}

	////////////////////////////////////////////////////////////////////////////////

	std::optional<Distance> Distance::fromMeterString(const std::string & s) {
		static const std::regex rgx ("(\\d\\d\\d\\d)|////");
		static const auto matchValue = 1;
		std::smatch match;
		if (!std::regex_match(s, match, rgx)) return(std::optional<Distance>());
		Distance distance;
		distance.distUnit = Unit::METERS;
		if (match.length(matchValue)) {
			const auto value = static_cast<unsigned int>(stoi(match.str(matchValue)));
			static const auto valueMoreThan10km = 9999;
			static const auto value10km = 10000;
			distance.distValueInt = value;
			if (value == valueMoreThan10km) {
				distance.distModifier = Modifier::MORE_THAN;
				distance.distValueInt = value10km;
			}
		}
		return(distance);
	}

	std::optional<Distance> Distance::fromMileString(const std::string & s) {
		static const std::optional<Distance> error;
		static const std::regex rgx ("([PM])?(\\d?\\d)(?:/(\\d?\\d))?SM|////SM");
		static const auto matchModifier = 1, matchNumerator = 2, matchDenominator = 3;
		std::smatch match;
		if (!std::regex_match(s, match, rgx)) return(error);

		Distance distance;
		distance.distUnit = Unit::STATUTE_MILES;

		if (!match.length(matchNumerator)) return(distance); //not reported

		auto modifier = modifierFromString(match.str(matchModifier));
		if (!modifier.has_value()) return(error);
		distance.distModifier = modifier.value();

		const auto num = static_cast<unsigned int>(stoi(match.str(matchNumerator)));
		if (!match.length(matchDenominator)) distance.distValueInt = num;

		if (match.length(matchDenominator)) {
			const auto den = static_cast<unsigned int>(stoi(match.str(matchDenominator)));
			distance.distValueNum = num;
			distance.distValueDen = den;
			if (den <= num && num >= 10) {
				static const auto decimalRadix = 10u;
				distance.distValueInt = num / decimalRadix;
				distance.distValueNum = num % decimalRadix;
			}
		}
		return(distance);
	}

	std::optional<Distance> Distance::fromHeightString(const std::string & s) {
		static const std::regex rgx ("(\\d\\d\\d)|///");
		static const auto matchValue = 1;
		std::smatch match;
		if (!std::regex_match(s, match, rgx)) return(std::optional<Distance>());
		Distance distance;
		distance.distUnit = Unit::FEET;
		if (match.length(matchValue)) {
			distance.distValueInt = 
				static_cast<unsigned int>(std::stoi(match.str(matchValue))) * heightFactor;
		}
		return(distance);
	}

	std::optional<Distance> Distance::fromRvrString(const std::string & s, bool unitFeet)
	{
		static const std::optional<Distance> error;
		static const std::regex rgx ("([PM])?(\\d\\d\\d\\d)|////");
		static const auto matchModifier = 1, matchValue = 2;
		std::smatch match;
		if (!std::regex_match(s, match, rgx)) return(std::optional<Distance>());
		Distance distance;
		distance.distUnit = unitFeet ? Unit::FEET : Unit::METERS;

		auto modifier = modifierFromString(match.str(matchModifier));
		if (!modifier.has_value()) return(error);
		distance.distModifier = modifier.value();

		if (match.length(matchValue)) {
			auto value = static_cast<unsigned int>(stoi(match.str(matchValue)));
			distance.distValueInt = value;
		}
		return(distance);
	}

	std::optional<Distance> Distance::fromIntegerAndFraction(const Distance & integer, 
		const Distance & fraction)
	{
		if (!integer.isValid() || 
			!fraction.isValid() ||
			integer.modifier() != Modifier::NONE ||
			fraction.modifier() != Modifier::NONE ||
			integer.unit() != fraction.unit() ||
			!integer.isInteger() ||
			!fraction.isFraction()) return(std::optional<Distance>());
		Distance result = integer;
		result.distValueNum = fraction.distValueNum;
		result.distValueDen = fraction.distValueDen;
		return(result);
	}

	std::optional<float> Distance::toUnit(Unit unit) const {
		static const std::optional<float> error;
		if (!isReported()) return(error);
		if (!distValueDen.value_or(1)) return(error);
		const auto value = distValueInt.value_or(0) + 
			static_cast<float>(distValueNum.value_or(0)) / distValueDen.value_or(1);
		if (distUnit == unit) return(value);
		switch (distUnit) {
			case Unit::METERS: 			return(metersToUnit(value, unit));
			case Unit::STATUTE_MILES:	return(milesToUnit(value, unit));
			case Unit::FEET:			return(feetToUnit(value, unit));
			default:					return(error);
		}
	}

	std::optional<Distance::Modifier> Distance::modifierFromString(const std::string & s) {
		if (s.empty()) return(Modifier::NONE);
		if (s == "M") return(Modifier::LESS_THAN);
		if (s == "P") return(Modifier::MORE_THAN);
		return(std::optional<Modifier>());
	}

	std::optional<float> Distance::metersToUnit(float value, Unit unit) {
		switch(unit) {
			case Unit::METERS: 		return(value);
			case Unit::STATUTE_MILES:	return(value / 1609.347);
			case Unit::FEET:			return(value / 0.3048);
			default: return(std::optional<float>());
		}
	}

	std::optional<float> Distance::milesToUnit(float value, Unit unit) {
		switch(unit) {
			case Unit::METERS: 		return(value * 1609.347);
			case Unit::STATUTE_MILES:	return(value);
			case Unit::FEET:			return(value * 5280.0);
			default: return(std::optional<float>());
		}
	}

	std::optional<float> Distance::feetToUnit(float value, Unit unit) {
		switch(unit) {
			case Unit::METERS: 		return(value * 0.3048);
			case Unit::STATUTE_MILES:	return(value / 5280.0);
			case Unit::FEET:			return(value);
			default: return(std::optional<float>());
		}
	}

	////////////////////////////////////////////////////////////////////////////////

	std::optional<Direction> Direction::fromCardinalString(const std::string & s) {
		if (s.empty()) {
			Direction dir;
			dir.dirStatus = Status::OMMITTED;
			return(dir);
		}
		if (s == "NDV") {
			Direction dir;
			dir.dirStatus = Status::NDV;
			return(dir);
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
		if (!cardinalDegrees) return(std::optional<Direction>());
		Direction dir;
		dir.dirStatus = Status::VALUE_CARDINAL;
		dir.dirDegrees = cardinalDegrees;
		return(dir);
	}

	std::optional<Direction> Direction::fromDegreesString(const std::string & s) {
		if (s.empty()) {
			Direction dir;
			dir.dirStatus = Status::OMMITTED;
			return(dir);
		}
		if (s == "///") {
			Direction dir;
			dir.dirStatus = Status::NOT_REPORTED;
			return(dir);
		}
		if (s == "VRB") {
			Direction dir;
			dir.dirStatus = Status::VARIABLE;
			return(dir);
		}
		static const std::regex rgx("\\d\\d0");
		if (!std::regex_match(s, rgx)) return (std::optional<Direction>());
		Direction dir;
		dir.dirStatus = Status::VALUE_DEGREES;
		dir.dirDegrees = static_cast<unsigned int>(stoi(s));
		return(dir);
	}

	Direction::Cardinal Direction::cardinal(bool trueDirections) const {
		if (status() == Status::OMMITTED || 
			status() == Status::NOT_REPORTED ||
			status() == Status::VARIABLE) return(metaf::Direction::Cardinal::NONE);
		if (status() == Status::NDV) return(metaf::Direction::Cardinal::NDV);
		if (trueDirections) {
			if (dirDegrees == degreesTrueNorth) return(Cardinal::TRUE_N);
			if (dirDegrees == degreesTrueSouth) return(Cardinal::TRUE_S);
			if (dirDegrees == degreesTrueWest) return(Cardinal::TRUE_W);
			if (dirDegrees == degreesTrueEast) return(Cardinal::TRUE_E);
		}
		//Degree values specifying cardinal direction sectors must be sorted.
		auto sectorSize = 45u; //cardinal direction sector size
		if (dirDegrees <= sectorSize/2) 						return(Cardinal::N);
		if (dirDegrees <= (degreesNorthEast + sectorSize/2)) 	return(Cardinal::NE);
		if (dirDegrees <= (degreesTrueEast + sectorSize/2))		return(Cardinal::E);
		if (dirDegrees <= (degreesSouthEast + sectorSize/2)) 	return(Cardinal::SE);
		if (dirDegrees <= (degreesTrueSouth + sectorSize/2))	return(Cardinal::S);
		if (dirDegrees <= (degreesSouthWest + sectorSize/2)) 	return(Cardinal::SW);
		if (dirDegrees <= (degreesTrueWest + sectorSize/2))		return(Cardinal::W);
		if (dirDegrees <= (degreesNorthWest + sectorSize/2)) 	return(Cardinal::NW);
		if (dirDegrees <= maxDegrees) 							return(Cardinal::N);
		return(Cardinal::NONE);
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<Pressure> Pressure::fromString(const std::string & s) {
		static const std::optional<Pressure> error;
		static const std::regex rgx("([QA])(?:(\\d\\d\\d\\d)|////)");
		static const auto matchUnit = 1, matchValue = 2;
		std::smatch match;
		if (!std::regex_match(s, match, rgx)) return(std::optional<Pressure>());
		std::optional<float> val;
		if (match.length(matchValue)) val = stof(match.str(matchValue));
		if (match.str(matchUnit) == "Q") {
			Pressure pressure;
			pressure.pressureUnit = Unit::HECTOPASCAL;
			pressure.pressureValue = val;
			return(pressure);
		}
		if (match.str(matchUnit) == "A") {
			Pressure pressure;
			pressure.pressureUnit = Unit::INCHES_HG;
			static const float inHgDecimalPointShift = 0.01;
			if (val.has_value()) val = val.value() * inHgDecimalPointShift;
			pressure.pressureValue = val;
			return(pressure);
		}
		return(error);
	}

	std::optional<float> Pressure::toUnit(Unit unit) const {
		if (!pressureValue.has_value()) return(std::optional<float>());
		auto v = pressureValue.value();
		if (pressureUnit == unit) return(v);
		static const auto hpaPerInHg = 33.8639;
		if (pressureUnit == Unit::HECTOPASCAL && unit == Unit::INCHES_HG) {
			return(v / hpaPerInHg);
		}
		if (pressureUnit == Unit::INCHES_HG && unit == Unit::HECTOPASCAL) {
			return(v * hpaPerInHg);
		}
		return(std::optional<float>());
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<Precipitation> Precipitation::fromRainfallString(const std::string & s) {
		if (s.empty() || s == "///./" || s == "//./") return(Precipitation());
		static const std::regex rgx("\\d?\\d\\d\\.\\d");
		if (!std::regex_match(s, rgx)) return(std::optional<Precipitation>());
		Precipitation precipitation;
		precipitation.precipStatus = Precipitation::Status::REPORTED;
		precipitation.precipValue = std::stof(s);
		return(precipitation);
	}

	std::optional<Precipitation> Precipitation::fromRunwayDeposits(const std::string & s) {
		if (s == "//") return(Precipitation());
		static const std::regex rgx("\\d\\d");
		if (!std::regex_match(s, rgx)) return(std::optional<Precipitation>());
		auto value = static_cast<unsigned int>(stoi(s));
		Precipitation precipitation;
		precipitation.precipStatus = Status::REPORTED;
		switch (value) {
			case Reserved::RESERVED: return(std::optional<Precipitation>());

			case Reserved::DEPTH_10CM: value = 100; break;
			case Reserved::DEPTH_15CM: value = 150; break;
			case Reserved::DEPTH_20CM: value = 200; break;
			case Reserved::DEPTH_25CM: value = 250; break;
			case Reserved::DEPTH_30CM: value = 300; break;
			case Reserved::DEPTH_35CM: value = 350; break;
			case Reserved::DEPTH_40CM: value = 400; break;

			case Reserved::RUNWAY_NOT_OPERATIONAL: 
			value = 0;
			precipitation.precipStatus = Status::RUNWAY_NOT_OPERATIONAL;
			break;

			default: break;
		}
		precipitation.precipValue = value;
		return(precipitation);
	}

	std::optional<float> Precipitation::toUnit(Unit unit) const {
		if (precipStatus != Status::REPORTED) return(std::optional<float>());
		if (precipUnit == unit) return(precipValue);
		static const auto mmPerInch = 25.4;
		if (precipUnit == Unit::MM && unit == Unit::INCHES) return(precipValue / mmPerInch);
		return(std::optional<float>());
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<SurfaceFriction> SurfaceFriction::fromString(const std::string & s) {
		if (s == "//") return(SurfaceFriction());
		static const std::regex rgx("\\d\\d");
		if (!std::regex_match(s, rgx)) return(std::optional<SurfaceFriction>());
		SurfaceFriction sf;
		auto coefficient = static_cast<unsigned int>(stoi(s));
		switch (coefficient) {
			case Reserved::BRAKING_ACTION_POOR:
			sf.sfStatus = Status::BRAKING_ACTION_REPORTED;
			sf.sfCoefficient = baPoorLowLimit;
			break;

			case Reserved::BRAKING_ACTION_MEDIUM_POOR:
			sf.sfStatus = Status::BRAKING_ACTION_REPORTED;
			sf.sfCoefficient = baMediumPoorLowLimit;
			break;

			case Reserved::BRAKING_ACTION_MEDIUM:
			sf.sfStatus = Status::BRAKING_ACTION_REPORTED;
			sf.sfCoefficient = baMediumLowLimit;
			break;

			case Reserved::BRAKING_ACTION_MEDIUM_GOOD:
			sf.sfStatus = Status::BRAKING_ACTION_REPORTED;
			sf.sfCoefficient = baMediumGoodLowLimit;
			break;

			case Reserved::BRAKING_ACTION_GOOD:
			sf.sfStatus = Status::BRAKING_ACTION_REPORTED;
			sf.sfCoefficient = baGoodLowLimit;
			break;

			case Reserved::RESERVED_96:
			case Reserved::RESERVED_97:
			case Reserved::RESERVED_98:
			return(std::optional<SurfaceFriction>());

			case Reserved::UNRELIABLE:
			sf.sfStatus = Status::UNRELIABLE;
			break;

			default:
			sf.sfCoefficient = coefficient;
			sf.sfStatus = Status::SURFACE_FRICTION_REPORTED;
			break;
		}
		return(sf);
	}

	SurfaceFriction::BrakingAction SurfaceFriction::brakingAction() const {
		if (sfStatus == Status::NOT_REPORTED || 
			sfStatus == Status::UNRELIABLE) return(BrakingAction::NONE);
		if (sfCoefficient < baMediumPoorLowLimit) 	return(BrakingAction::POOR);
		if (sfCoefficient < baMediumLowLimit) 		return(BrakingAction::MEDIUM_POOR);
		if (sfCoefficient < baMediumGoodLowLimit) 	return(BrakingAction::MEDIUM);
		if (sfCoefficient < baGoodLowLimit)			return(BrakingAction::MEDIUM_GOOD);
		return(BrakingAction::GOOD);
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<WaveHeight> WaveHeight::fromString(const std::string & s) {
		static const std::optional<WaveHeight> error;
		if (s == "H///") {
			WaveHeight wh;
			wh.whType = Type::WAVE_HEIGHT;
			return(wh);
		}
		if (s == "S/") {
			WaveHeight wh;
			wh.whType = Type::STATE_OF_SURFACE;
			return(wh);
		}
		static const std::regex rgx("S(\\d)|H(\\d?\\d?\\d)");
		static const auto matchStateOfSurface = 1, matchWaveHeight = 2;
		std::smatch match;
		if (!std::regex_match(s, match, rgx)) return(error);
		if (match.length(matchStateOfSurface)) {
			auto h = waveHeightFromStateOfSurfaceString(match.str(matchStateOfSurface));
			if (!h.has_value()) return(error);
			WaveHeight wh;
			wh.whType = Type::STATE_OF_SURFACE;
			wh.whValue = h.value();
			return(wh);
		}
		if (match.length(matchWaveHeight)) {
			WaveHeight wh;
			wh.whType = Type::WAVE_HEIGHT;
			wh.whValue = static_cast<unsigned int>(stoi(match.str(matchWaveHeight)));
			return(wh);
		}
		return(error);
	}

	std::optional<float> WaveHeight::toUnit(Unit unit) const {
		const auto wh = waveHeight();
		if (!wh.has_value()) return(std::optional<float>());
		if (this->unit() == unit) return(wh.value());
		if (static const float metersPerFoot = 0.3048;
			this->unit() == Unit::METERS && unit == Unit::FEET)
		{
			return(wh.value() / metersPerFoot);
		} 
		return(std::optional<float>());

	}

	WaveHeight::StateOfSurface WaveHeight::stateOfSurface() const {
		if (!whValue.has_value()) return(StateOfSurface::NOT_REPORTED);
		const auto h = whValue.value();
		// Wave heights must be sorted
		if (!h) return(StateOfSurface::CALM_GLASSY);
		if (h <= maxWaveHeightCalmRippled) return(StateOfSurface::CALM_RIPPLED);
		if (h <= maxWaveHeightSmooth) return(StateOfSurface::SMOOTH);
		if (h <= maxWaveHeightSlight) return(StateOfSurface::SLIGHT);
		if (h <= maxWaveHeightModerate) return(StateOfSurface::MODERATE);
		if (h <= maxWaveHeightRough) return(StateOfSurface::ROUGH);
		if (h <= maxWaveHeightVeryRough) return(StateOfSurface::VERY_ROUGH);
		if (h <= maxWaveHeightHigh) return(StateOfSurface::HIGH);
		if (h <= maxWaveHeightVeryHigh) return(StateOfSurface::VERY_HIGH);
		return(StateOfSurface::PHENOMENAL);
	}

	std::optional<float> WaveHeight::waveHeightFromStateOfSurfaceString(const std::string & s) {
		if (s == "0") return(maxWaveHeightCalmGlassy);
		if (s == "1") return(maxWaveHeightCalmRippled);
		if (s == "2") return(maxWaveHeightSmooth);
		if (s == "3") return(maxWaveHeightSlight);
		if (s == "4") return(maxWaveHeightModerate);
		if (s == "5") return(maxWaveHeightRough);
		if (s == "6") return(maxWaveHeightVeryRough);
		if (s == "7") return(maxWaveHeightHigh);
		if (s == "8") return(maxWaveHeightVeryHigh);
		if (s == "9") return(minWaveHeightPhenomenal);
		return(std::optional<float>());
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<Group> PlainTextGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<FixedGroup> FixedGroup::parse(const std::string & group, 
		ReportPart reportPart)
	{
		if (reportPart == ReportPart::HEADER) {
			if (group == "METAR") return(FixedGroup(Type::METAR));
			if (group == "SPECI") return(FixedGroup(Type::SPECI));
			if (group == "TAF") return(FixedGroup(Type::TAF));
			if (group == "AMD") return(FixedGroup(Type::AMD));
		}
		if (reportPart == ReportPart::HEADER || reportPart == ReportPart::METAR) {
			if (group == "COR") return(FixedGroup(Type::COR));
		}
		if (reportPart == ReportPart::HEADER || 
			reportPart == ReportPart::METAR || 
			reportPart == ReportPart::TAF) {
				if (group == "NIL") return(FixedGroup(Type::NIL));
				if (group == "CNL") return(FixedGroup(Type::CNL));
		}
		if (reportPart == ReportPart::METAR) {
			if (group == "AUTO") return(FixedGroup(Type::AUTO));
			if (group == "SNOCLO") return(FixedGroup(Type::R_SNOCLO));
			if (group == "R/SNOCLO") return(FixedGroup(Type::R_SNOCLO));
		}
		if (reportPart == ReportPart::METAR || reportPart == ReportPart::TAF) {
			if (group == "CAVOK") return(FixedGroup(Type::CAVOK));
			if (group == "NSW") return(FixedGroup(Type::NSW));
			if (group == "RMK") return(FixedGroup(Type::RMK));
		}
		return(std::optional<FixedGroup>());
	}

	std::optional<Group> FixedGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<LocationGroup> LocationGroup::parse(const std::string & group, 
		ReportPart reportPart)
	{
		static const std::optional<LocationGroup> notRecognised;
		if (reportPart != ReportPart::HEADER) return(notRecognised);
		static const std::regex rgx = std::regex("[A-Z][A-Z0-9]{3}");		
		if (!regex_match(group, rgx)) return(notRecognised);
		LocationGroup result;
		strncpy(result.location, group.c_str(), locationLength);
		result.location[locationLength] = '\0';
		return(result);
	}

	std::optional<Group> LocationGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<ReportTimeGroup> ReportTimeGroup::parse(const std::string & group, 
		ReportPart reportPart) 
	{
		static const std::optional<ReportTimeGroup> notRecognised;
		static const std::regex rgx ("\\d\\d\\d\\d\\d\\dZ");
		static const auto posTime = 0, lenTime = 6;
		if (reportPart != ReportPart::HEADER) return(notRecognised);
		if (!regex_match(group, rgx)) return(notRecognised);
		const auto tm = MetafTime::fromStringDDHHMM(group.substr(posTime, lenTime));
		if (!tm.has_value()) return(notRecognised);
		if (!tm->day().has_value()) return(notRecognised);
		ReportTimeGroup g; 
		g.t = tm.value();
		return(g);
	}

	std::optional<Group> ReportTimeGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<TrendGroup> TrendGroup::parse(const std::string & group, 
		ReportPart reportPart)
	{
		if (reportPart == ReportPart::METAR || reportPart == ReportPart::TAF) {
			// Detect trend type fixed groups
			if (group == "BECMG") return(TrendGroup(Type::BECMG));
			if (group == "TEMPO") return(TrendGroup(Type::TEMPO));
			if (group == "INTER") return(TrendGroup(Type::INTER));
		}
		if (reportPart == ReportPart::TAF) {
			// Detect probability groups
			if (group == "PROB30") return(TrendGroup(Probability::PROB_30));
			if (group == "PROB40") return(TrendGroup(Probability::PROB_40));
			// Detect time span groups
			if (auto timeSpan = fromTimeSpan(group); timeSpan.has_value()) return(timeSpan);
			// Detect FMxxxxxx groups
			if (auto longFm = fromFm(group); longFm.has_value()) return(longFm);
		}
		if (reportPart == ReportPart::METAR){
			// Detect NOSIG trend type
			if ( group == "NOSIG") return(TrendGroup(Type::NOSIG));
			// Detect FMxxxx /TLxxxx /ATxxxx
			if (auto trendTime = fromTrendTime(group); trendTime.has_value()) return(trendTime);
		}
		if (reportPart == ReportPart::HEADER || reportPart == ReportPart::TAF) {
			// Detect time span
			if (auto ts = fromTimeSpan(group); ts.has_value()) return(ts);
		}
		// Cannot detect group
		return(std::optional<TrendGroup>());
	}

	std::optional<Group> TrendGroup::combine(const Group & nextGroup) const {
		if (!std::holds_alternative<TrendGroup>(nextGroup)) return(std::optional<Group>());
		auto nextTrendGroup = std::get<TrendGroup>(nextGroup);
		TrendGroup combinedGroup = *this;
		if (combinedGroup.combineProbAndTrendTypeGroups(nextTrendGroup)) return(combinedGroup);
		if (combinedGroup.combineTrendTypeAndTimeGroup(nextTrendGroup)) return(combinedGroup);
		if (combinedGroup.combineProbAndTimeSpanGroups(nextTrendGroup)) return(combinedGroup);
		if (combinedGroup.combineIncompleteGroups(nextTrendGroup)) return(combinedGroup);
		return(std::optional<Group>());
	}

	std::optional<TrendGroup> TrendGroup::fromTimeSpan(const std::string & s) {
		static const std::optional<TrendGroup> notRecognised;
		static const std::regex rgx("(\\d\\d\\d\\d)/(\\d\\d\\d\\d)");
		static const auto matchFrom = 1, matchTill = 2;
		std::smatch match;
		if (!regex_match(s, match, rgx)) return(notRecognised);
		const auto from = MetafTime::fromStringDDHH(match.str(matchFrom));
		const auto till = MetafTime::fromStringDDHH(match.str(matchTill));
		if (!from.has_value() || !till.has_value()) return(notRecognised);

		TrendGroup result;
		result.t = Type::TIME_SPAN;
		result.tFrom = from;
		result.tTill = till;
		return(result);
	}

	std::optional<TrendGroup> TrendGroup::fromFm(const std::string & s) {
		static const std::optional<TrendGroup> notRecognised;
		static const std::regex rgx("FM\\d\\d\\d\\d\\d\\d");
		static const auto posTime = 2, lenTime = 6;
		if (!regex_match(s, rgx)) return(notRecognised);
		const auto time = MetafTime::fromStringDDHHMM(s.substr(posTime, lenTime));
		if (!time.has_value()) return(notRecognised);

		TrendGroup result;
		result.t = Type::FROM;
		result.tFrom = time;
		return(result);
	}

	std::optional<TrendGroup> TrendGroup::fromTrendTime(const std::string & s) {
		static const std::optional<TrendGroup> notRecognised;
		static const std::regex rgx("([FTA][MLT])(\\d\\d\\d\\d)");
		static const auto matchType = 1, matchTime = 2;
		std::smatch match;
		if (!regex_match(s, match, rgx)) return(notRecognised);
		const auto time = MetafTime::fromStringDDHHMM(match.str(matchTime));
		if (!time.has_value()) return(notRecognised);
		TrendGroup result;
		result.t = Type::NONE;
		if (match.str(matchType) == "FM") {
			result.tFrom = time;
			return(result);
		}
		if (match.str(matchType) == "TL") {
			result.tTill = time;
			return(result);
		}
		if (match.str(matchType) == "AT") {
			result.tAt = time;
			return(result);
		}
		return(notRecognised);
	}

	bool TrendGroup::combineProbAndTrendTypeGroups(const TrendGroup & nextTrendGroup) {
		if (!isProbabilityGroup()) return(false);
		if (!nextTrendGroup.isTrendTypeGroup()) return(false);
		if (nextTrendGroup.type() != Type::TEMPO && 
			nextTrendGroup.type() != Type::INTER) return(false);
		t = nextTrendGroup.type();
		return(true);
	}

	bool TrendGroup::combineTrendTypeAndTimeGroup(const TrendGroup & nextTrendGroup) {
		if (type() != Type::BECMG && 
			type() != Type::TEMPO && 
			type() != Type::INTER) return(false);
		if (!nextTrendGroup.isTimeSpanGroup() && 
			!nextTrendGroup.isTrendTimeGroup()) return(false);
		if (!canCombineTime(*this, nextTrendGroup)) return(false);
		combineTime(nextTrendGroup);
		return(true);
	}

	bool TrendGroup::combineProbAndTimeSpanGroups(const TrendGroup & nextTrendGroup) {
		if (!isProbabilityGroup()) return(false);
		if (!nextTrendGroup.isTimeSpanGroup()) return(false);
		combineTime(nextTrendGroup);
		t = Type::TIME_SPAN;
		return(true);
	}

	bool TrendGroup::combineIncompleteGroups(const TrendGroup & nextTrendGroup) {
		if (type() != Type::NONE) return(false);
		if (probability() != Probability::NONE) return(false);
		if (!nextTrendGroup.isTrendTimeGroup()) return(false);
		if (!canCombineTime(*this, nextTrendGroup)) return(false);
		combineTime(nextTrendGroup);
		return(true);
	}

	bool TrendGroup::isProbabilityGroup() const {
		// Probability group has format PROB30 or PROB40
		// Probability must be reported and no time allowed
		if (type() != Type::NONE) return(false);
		if (probability() == Probability::NONE) return(false);
		if (timeFrom().has_value() || timeTill().has_value()) return(false);
		if (timeAt().has_value()) return(false);
		return(true);
	}

	bool TrendGroup::isTrendTypeGroup() const {
		// Trend type group is a fixed group BECMG / TEMPO / INTER
		// No probability or time allowed
		if (type() != Type::BECMG && 
			type() != Type::TEMPO && 
			type() != Type::INTER) return(false);
		if (probability() != Probability::NONE) return(false);
		if (timeFrom().has_value() || timeTill().has_value()) return(false);
		if (timeAt().has_value()) return(false);
		return(true);
	}

	bool TrendGroup::isTrendTimeGroup() const {
		// Trend time group has format FMxxxx, TLxxxx, ATxxxx
		// Only one time from timeFrom, timeTill or timeAt can be reported
		if (type() != Type::NONE) return(false);
		if (probability() != Probability::NONE) return(false);
		if (!timeFrom() && !timeTill() && !timeAt()) return(false);
		if (timeFrom() && timeTill()) return(false);
		if (timeFrom() && timeAt()) return(false);
		if (timeTill() && timeAt()) return(false);
		return(true);
	}

	bool TrendGroup::isTimeSpanGroup() const {
		// Time span group has format xxxx/xxxx, 
		// only time 'from' and 'till' must be reported
		if (type() != Type::TIME_SPAN) return(false);
		if (probability() != Probability::NONE) return(false);
		if (!timeFrom().has_value() || !timeTill().has_value()) return(false);
		if (timeAt().has_value()) return(false);
		return(true);
	}

	bool TrendGroup::canCombineTime(const TrendGroup & g1, const TrendGroup & g2) {
		// Cannot combine time 'from' with 'from', 'till' with 'till', 'at' with 'at'
		if (g1.timeFrom().has_value() && g2.timeFrom().has_value()) return(false);
		if (g1.timeTill().has_value() && g2.timeTill().has_value()) return(false);
		if (g1.timeAt().has_value() && g2.timeAt().has_value()) return(false);
		// Cannot combine time 'from' or 'till' with 'at'
		if (g1.timeAt().has_value() &&
			(g2.timeFrom().has_value() || g2.timeTill().has_value())) return(false);
		if (g2.timeAt().has_value() &&
			(g1.timeFrom().has_value() || g1.timeTill().has_value())) return(false);
		return(true);
	}

	void TrendGroup::combineTime(const TrendGroup & nextTrendGroup) {
		if (!timeFrom().has_value()) tFrom = nextTrendGroup.timeFrom();
		if (!timeTill().has_value()) tTill = nextTrendGroup.timeTill();
		if (!timeAt().has_value()) tAt = nextTrendGroup.timeAt();
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<WindGroup> WindGroup::parse(const std::string & group, 
			ReportPart reportPart)
	{
		static const std::optional<WindGroup> notRecognised;
		static const std::regex windRgx("(?:WS(\\d\\d\\d)/)?"
			"(\\d\\d0|VRB|///)([1-9]?\\d\\d|//)(?:G([1-9]?\\d\\d))?([KM][TMP][HS]?)");
		static const auto matchWindShearHeight = 1, matchWindDir = 2;
		static const auto matchWindSpeed = 3, matchWindGust = 4, matchWindUnit = 5;
		static const std::regex varWindRgx("(\\d\\d0)V(\\d\\d0)");
		static const auto matchVarWindBegin = 1, matchVarWindEnd = 2;
		if (reportPart != ReportPart::METAR && 
			reportPart != ReportPart::TAF) return(notRecognised);
		if (std::smatch match; std::regex_match(group, match, windRgx)) {
			const auto speedUnit = Speed::unitFromString(match.str(matchWindUnit));
			if (!speedUnit.has_value()) return(notRecognised);
			WindGroup result;
			const auto dir = Direction::fromDegreesString(match.str(matchWindDir));
			if (!dir.has_value()) return(notRecognised);
			result.windDir = dir.value();
			const auto speed = Speed::fromString(match.str(matchWindSpeed), speedUnit.value());
			if (!speed.has_value()) return(notRecognised);
			result.wSpeed = speed.value();
			const auto gust = Speed::fromString(match.str(matchWindGust), speedUnit.value());
			if (gust.has_value()) result.gSpeed = gust.value();
			const auto wsHeight = Distance::fromHeightString(match.str(matchWindShearHeight));
			if (wsHeight.has_value()) result.wShHeight = wsHeight.value();
			return(result);
		}
		if (std::smatch match; std::regex_match(group, match, varWindRgx)) {
			WindGroup result;
			const auto begin = Direction::fromDegreesString(match.str(matchVarWindBegin));
			if (!begin.has_value()) return(notRecognised);
			result.vsecBegin = begin.value();
			const auto end = Direction::fromDegreesString(match.str(matchVarWindEnd));
			if (!end.has_value()) return(notRecognised);
			result.vsecEnd = end.value();
			return(result);
		}
		return(notRecognised);
	}

	std::optional<Group> WindGroup::combine(const Group & nextGroup) const {
		if (!std::holds_alternative<WindGroup>(nextGroup)) return(std::optional<Group>());
		auto nextWindGroup = std::get<WindGroup>(nextGroup);
		if (!isSurfaceWindGroup()) return(std::optional<Group>());
		if (!nextWindGroup.isWindSectorGroup()) return(std::optional<Group>());
		WindGroup combinedGroup = *this;
		combinedGroup.vsecBegin = nextWindGroup.vsecBegin;
		combinedGroup.vsecEnd = nextWindGroup.vsecEnd;
		return(combinedGroup);
	}

	bool WindGroup::isCalm() const {
		return (!direction().degrees().value_or(1) && 
			direction().status() == Direction::Status::VALUE_DEGREES &&
			!windSpeed().speed().value_or(1) && 
			!gustSpeed().speed().has_value() &&
			!windShearHeight().isReported() && 
			varSectorBegin().status() == Direction::Status::OMMITTED &&
			varSectorEnd().status() == Direction::Status::OMMITTED);
	}

	bool WindGroup::isValid() const {
		// If both wind and gust speed reported, wind speed cannot be greater than gust speed
		if (windSpeed().speed().value_or(0) >= gustSpeed().speed().value_or(999)) return(false);
		// Gust speed cannot be zero if reported
		if (!gustSpeed().speed().value_or(1)) return(false);
		// Wind shear height cannot be zero if reported
		if (!windShearHeight().integer().value_or(1)) return(false);
		// All data must be valid
		return(direction().isValid() &&
			windShearHeight().isValid() &&
			varSectorBegin().isValid() &&
			varSectorEnd().isValid());
	}

	bool WindGroup::isSurfaceWindGroup() const {
		return (direction().status() != Direction::Status::OMMITTED &&
			!windShearHeight().isReported() &&
			varSectorBegin().status() == Direction::Status::OMMITTED &&
			varSectorEnd().status() == Direction::Status::OMMITTED);
	}

	bool WindGroup::isWindSectorGroup() const {
		return (direction().status() == Direction::Status::OMMITTED &&
			!windSpeed().speed().has_value() &&
			!gustSpeed().speed().has_value() &&
			!windShearHeight().isReported() &&
			varSectorBegin().status() == Direction::Status::VALUE_DEGREES &&
			varSectorEnd().status() == Direction::Status::VALUE_DEGREES);
	}

	///////////////////////////////////////////////////////////////////////////

	std::optional<VisibilityGroup> VisibilityGroup::parse(const std::string & group, 
		ReportPart reportPart)
	{
		static const std::optional<VisibilityGroup> notRecognised;
		if (reportPart != ReportPart::METAR && reportPart != ReportPart::TAF) return(notRecognised);
		// Attempt to parse string as incomplete integer value
		if (auto incompleteLength = 1u; group.length() == incompleteLength && 
			std::isdigit(group.front()))
		{
			VisibilityGroup result;
			auto v = group.front() - '0';
			result.vis = Distance(v, Distance::Unit::STATUTE_MILES);
			result.incompleteInteger = true;
			return(result);
		}
		// Attempt to parse string as visibility in meters
		static const std::regex rgx("(\\d\\d\\d\\d|////)([NSWE][WED]?[V]?)?");
		static const auto matchVis = 1, matchDir = 2;
		std::smatch match;
		if (std::regex_match(group, match, rgx)) {
			const auto v = Distance::fromMeterString(match.str(matchVis));
			if (!v.has_value()) return(notRecognised);
			const auto d = Direction::fromCardinalString(match.str(matchDir));
			if (!d.has_value()) return(notRecognised);
			VisibilityGroup result;
			result.vis = v.value();
			result.dir = d.value();
			return(result);
		}
		// Attempt to parse as visibility in miles
		const auto v = Distance::fromMileString(group);
		if (!v.has_value()) return(notRecognised);
		VisibilityGroup result;
		result.vis = v.value();
		return(result);
	}

	std::optional<Group> VisibilityGroup::combine(const Group & nextGroup) const {
		if (!std::holds_alternative<VisibilityGroup>(nextGroup)) return(std::optional<Group>());
		auto nextVisGroup = std::get<VisibilityGroup>(nextGroup);
		if (visibility().unit() != Distance::Unit::STATUTE_MILES || 
			nextVisGroup.visibility().unit() != Distance::Unit::STATUTE_MILES) {
				return(std::optional<Group>());
		}
		if (incompleteInteger && nextVisGroup.visibility().isFraction()) {
			auto v = Distance::fromIntegerAndFraction(visibility(), 
				nextVisGroup.visibility());
			if (!v.has_value()) return(std::optional<Group>());
			VisibilityGroup combinedGroup = *this;
			combinedGroup.vis = v.value();
			combinedGroup.incompleteInteger = false;
			return(combinedGroup);
		}
		return (std::optional<Group>());
	}

	///////////////////////////////////////////////////////////////////////////

	std::optional<CloudGroup> CloudGroup::parse(const std::string & group, 
		ReportPart reportPart)
	{
		static const std::optional<CloudGroup> notRecognised;
		if (reportPart != ReportPart::METAR && reportPart != ReportPart::TAF) return(notRecognised);
		// Attempt to parse fixed groups
		if (group == "NCD") return(CloudGroup(Amount::NCD));
		if (group == "NSC") return(CloudGroup(Amount::NSC));
		if (group == "CLR") return(CloudGroup(Amount::NONE_CLR));
		if (group == "SKC") return(CloudGroup(Amount::NONE_SKC));
		if (group == "NCD") return(CloudGroup(Amount::NCD));
		// Attempt to parse vertical visibility (format VVxxx)
		std::smatch match;
		// Attempt to parse 
		static const std::regex rgx(
			"([BFOSV][CEKV][CNTW]?|///)(\\d\\d\\d|///)([CT][BC][U]?|///)?");
		static const auto matchAmount = 1, matchHeight = 2, matchType = 3;
		if (!std::regex_match(group, match, rgx)) return(notRecognised);

		const auto amount = amountFromString(match.str(matchAmount));
		if (!amount.has_value()) return(notRecognised);
		const auto height = Distance::fromHeightString(match.str(matchHeight));
		if (!height.has_value()) return(notRecognised);
		const auto type = typeFromString(match.str(matchType));
		if (!type.has_value()) return(notRecognised);

		// If vertical visibility is given, convective cloud type must not be specified
		if (amount.value() == Amount::OBSCURED && type.value() != Type::NONE) return(notRecognised);

		CloudGroup result;
		result.amnt = amount.value();
		result.heightOrVertVis = height.value();
		result.tp = type.value();
		return(result);
	}

	std::optional<Group> CloudGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	std::optional<CloudGroup::Amount> CloudGroup::amountFromString(const std::string & s) {
		if (s == "FEW") return(CloudGroup::Amount::FEW);
		if (s == "SCT") return(CloudGroup::Amount::SCATTERED);
		if (s == "BKN") return(CloudGroup::Amount::BROKEN);
		if (s == "OVC") return(CloudGroup::Amount::OVERCAST);
		if (s == "VV") return(CloudGroup::Amount::OBSCURED);
		if (s == "///") return(CloudGroup::Amount::NOT_REPORTED);
		return(std::optional<Amount>());
	}

	const Distance & CloudGroup::height() const {
		switch(amount()) {
			case Amount::NOT_REPORTED:
			case Amount::FEW:
			case Amount::SCATTERED:
			case Amount::BROKEN:
			case Amount::OVERCAST:
			return(heightOrVertVis);

			default: 
			return(heightNotReported);
		}
	}

	std::optional<CloudGroup::Type> CloudGroup::typeFromString(const std::string & s){
		if (s.empty()) return(Type::NONE);
		if (s == "TCU") return(Type::TOWERING_CUMULUS);
		if (s == "CB") return(Type::CUMULONIMBUS);
		if (s == "///") return(Type::NOT_REPORTED);
		return(std::optional<Type>());
	}

	///////////////////////////////////////////////////////////////////////////

	std::optional<WeatherGroup> WeatherGroup::parse(const std::string & group, 
		ReportPart reportPart)
	{
		std::optional<WeatherGroup> notRecognised;
		if (reportPart != ReportPart::METAR && reportPart != ReportPart::TAF) return(notRecognised);

		if (reportPart == ReportPart::METAR) {
			if (group == "RE//") return(notReportedRecent());
			if (group == "//") return(notReported());
		}

		if (group.empty()) return(notRecognised);
		static const std::regex rgx("(RE|[\\+-]|VC)?(MI|PR|BC|DR|BL|SH|TS|FZ)?((?:[A-Z][A-Z])*)");
		static const auto matchQualifier = 1, matchDescriptor = 2, matchWeather = 3;
		std::smatch match;
		if (!regex_match(group, match, rgx)) return(notRecognised);
		
		const auto qualifier = qualifierFromString(match.str(matchQualifier));
		if (!qualifier.has_value()) return(notRecognised);
		const auto descriptor = descriptorFromString(match.str(matchDescriptor));
		if (!descriptor.has_value()) return(notRecognised);
		
		WeatherGroup result;
		static const auto wthrTokenSize = 2;
		const auto weatherStr = match.str(matchWeather);
		for (auto i = 0; i < match.length(matchWeather); i += wthrTokenSize) {
			auto weather = weatherFromString(weatherStr.substr(i, wthrTokenSize));
			if (!weather.has_value()) return(notRecognised);
			if (result.wSize >= maxwSize) return(notRecognised);
			result.w[result.wSize++] = weather.value();
		}
		result.d = descriptor.value();
		result.q = qualifier.value();
		if (result.q == Qualifier::NONE && result.isModerateQualifier()) result.q = Qualifier::MODERATE;
		return(result);
	}

	std::optional<Group> WeatherGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	bool WeatherGroup::isModerateQualifier() const {
		for (auto i=0u; i<wSize; i++) {
			switch (w[i]) {
				case Weather::DRIZZLE:
				case Weather::RAIN:
				case Weather::SNOW_GRAINS:
				case Weather::ICE_PELLETS:
				case Weather::UNDETERMINED:
				return(true);

				case Weather::SNOW:
				if (descriptor() != Descriptor::LOW_DRIFTING && 
					descriptor() != Descriptor::BLOWING) return(true);
				break;

				default:
				break;
			}
		}
		return(false);
	}

	std::optional<WeatherGroup::Qualifier> WeatherGroup::qualifierFromString(
		const std::string & s)
	{
		if (s.empty()) return(Qualifier::NONE);
		if (s == "RE") return(Qualifier::RECENT);
		if (s == "-") return(Qualifier::LIGHT);
		if (s == "+") return(Qualifier::HEAVY);
		if (s == "VC") return(Qualifier::VICINITY);
		return(std::optional<Qualifier>());
	}

	std::optional<WeatherGroup::Descriptor> WeatherGroup::descriptorFromString(
		const std::string & s)
	{
		if (s.empty()) return(Descriptor::NONE);
		if (s == "MI") return(Descriptor::SHALLOW);
		if (s == "PR") return(Descriptor::PARTIAL);
		if (s == "BC") return(Descriptor::PATCHES);
		if (s == "DR") return(Descriptor::LOW_DRIFTING);
		if (s == "BL") return(Descriptor::BLOWING);
		if (s == "SH") return(Descriptor::SHOWERS);
		if (s == "TS") return(Descriptor::THUNDERSTORM);
		if (s == "FZ") return(Descriptor::FREEZING);
		return(std::optional<Descriptor>());
	}

	std::optional<WeatherGroup::Weather> WeatherGroup::weatherFromString(
		const std::string & s)
	{
		if (s == "DZ") return(Weather::DRIZZLE);
		if (s == "RA") return(Weather::RAIN);
		if (s == "SN") return(Weather::SNOW);
		if (s == "SG") return(Weather::SNOW_GRAINS);
		if (s == "IC") return(Weather::ICE_CRYSTALS);
		if (s == "PL") return(Weather::ICE_PELLETS);
		if (s == "GR") return(Weather::HAIL);
		if (s == "GS") return(Weather::SMALL_HAIL);
		if (s == "UP") return(Weather::UNDETERMINED);
		if (s == "BR") return(Weather::MIST);
		if (s == "FG") return(Weather::FOG);
		if (s == "FU") return(Weather::SMOKE);
		if (s == "VA") return(Weather::VOLCANIC_ASH);
		if (s == "DU") return(Weather::DUST);
		if (s == "SA") return(Weather::SAND);
		if (s == "HZ") return(Weather::HAZE);
		if (s == "PY") return(Weather::SPRAY);
		if (s == "PO") return(Weather::DUST_WHIRLS);
		if (s == "SQ") return(Weather::SQUALLS);
		if (s == "FC") return(Weather::FUNNEL_CLOUD);
		if (s == "SS") return(Weather::SANDSTORM);
		if (s == "DS") return(Weather::DUSTSTORM);
		return(std::optional<Weather>());
	}

	WeatherGroup WeatherGroup::notReported() {
		WeatherGroup result;
		result.w[0] = Weather::NOT_REPORTED; result.wSize = 1;
		return(result);
	}
	
	WeatherGroup WeatherGroup::notReportedRecent() {
		WeatherGroup result = notReported();
		result.q = Qualifier::RECENT;
		return(result);
	}

	///////////////////////////////////////////////////////////////////////////

	bool TemperatureGroup::isValid() const {
		// Either temperature or dew point not reported: always valid
		if (!airTemperature().temperature().has_value() || 
			!dewPoint().temperature().has_value()) return(true);
		// If temperature reported M00 then dew point cannot be 00
		if (!airTemperature().temperature().value() &&	
			!dewPoint().temperature().value() &&
			airTemperature().isFreezing() &&
			!dewPoint().isFreezing()) return (false);
		// Generally dew point must be less or equal to temperature
		return (airTemperature().temperature().value() >= dewPoint().temperature().value());
	}

	std::optional<TemperatureGroup> TemperatureGroup::parse(const std::string & group, 
		ReportPart reportPart)
	{
		static const std::optional<TemperatureGroup> notRecognised;
		if (reportPart != ReportPart::METAR) return(notRecognised);
		static const std::regex rgx("(M?\\d\\d|//)/(M?\\d\\d|//)?");
		static const auto matchTemperature = 1, matchDewPoint = 2;
		std::smatch match;
		if (!regex_match(group, match, rgx)) return(notRecognised);
		const auto t = Temperature::fromString(match.str(matchTemperature));
		if (!t.has_value()) return(notRecognised);
		TemperatureGroup result;
		result.t = t.value();
		if (match.length(matchDewPoint)) {
			const auto dp = Temperature::fromString(match.str(matchDewPoint));
			if (!dp.has_value()) return(notRecognised);
			result.dp = dp.value();
		}
		return(result);
	}

	std::optional<Group> TemperatureGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	///////////////////////////////////////////////////////////////////////////

	std::optional<TemperatureForecastGroup> TemperatureForecastGroup::parse(
		const std::string & group, 
		ReportPart reportPart)
	{
		static const std::optional<TemperatureForecastGroup> notRecognised;
		if (reportPart != ReportPart::TAF) return (notRecognised);
		static const std::regex rgx ("(TX|TN)(M?\\d\\d)/(\\d\\d\\d\\d)Z");
		static const auto matchPoint = 1, matchTemperature = 2, matchTime = 3;
		std::smatch match;
		if (!std::regex_match(group, match, rgx)) return(notRecognised);
		auto point = pointFromString(match.str(matchPoint));
		if (!point.has_value()) return(notRecognised);
		auto temp = metaf::Temperature::fromString(match.str(matchTemperature));
		if (!temp.has_value()) return(notRecognised);
		auto time = metaf::MetafTime::fromStringDDHH(match.str(matchTime));
		if (!time.has_value()) return(notRecognised);
		TemperatureForecastGroup result;
		result.p = point.value();
		result.t = temp.value();
		result.tm = time.value();
		return(result);
	}

	std::optional<Group> TemperatureForecastGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	///////////////////////////////////////////////////////////////////////////

	std::optional<PressureGroup> PressureGroup::parse(const std::string & group, 
		ReportPart reportPart)
	{
		static const std::optional<PressureGroup> notRecognised;
		if (reportPart != metaf::ReportPart::METAR) return(notRecognised);
		const auto pressure = metaf::Pressure::fromString(group);
		if (!pressure.has_value()) return(notRecognised);
		PressureGroup result;
		result.p = pressure.value();
		return(result);
	}

	std::optional<Group> PressureGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	///////////////////////////////////////////////////////////////////////////

	std::optional<RunwayVisualRangeGroup> RunwayVisualRangeGroup::parse(
		const std::string & group, ReportPart reportPart)
	{
		static const std::optional<RunwayVisualRangeGroup> notRecognised;
		if (reportPart != ReportPart::METAR) return(notRecognised);
		static const std::regex rgx("(R\\d\\d[RCL]?)/(////|[PM]?\\d\\d\\d\\d)"
			"(?:V([PM]?\\d\\d\\d\\d))?(FT/?)?([UND/])?");
		static const auto matchRunway = 1, matchRvr = 2, matchVarRvr = 3, matchUnit = 4;
		static const auto matchTrend = 5;
		std::smatch match;
		if (!regex_match(group, match, rgx)) return(notRecognised);
		const auto tr = trendFromString(match.str(matchTrend));
		if (!tr.has_value()) return(notRecognised);
		const bool unitFeet = match.length(matchUnit);
		const auto runway = Runway::fromString(match.str(matchRunway));
		if (!runway.has_value()) return(notRecognised);
		const auto rvr = Distance::fromRvrString(match.str(matchRvr), unitFeet);
		if (!rvr.has_value()) return(notRecognised);
		RunwayVisualRangeGroup result;
		if (match.length(matchVarRvr)) {
			const auto varRvr = Distance::fromRvrString(match.str(matchVarRvr), unitFeet);
			if (!varRvr.has_value()) return(notRecognised);
			result.varVisRange = varRvr.value();
		}
		result.rw = runway.value();
		result.visRange = rvr.value();
		result.trnd = tr.value();
		return(result);
	}

	std::optional<Group> RunwayVisualRangeGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	std::optional<RunwayVisualRangeGroup::Trend> RunwayVisualRangeGroup::trendFromString(
		const std::string & s)
	{
		if (!s.length()) return(Trend::NONE);
		if (s == "/") return(Trend::NOT_REPORTED);
		if (s == "U") return(Trend::UPWARD);
		if (s == "N") return(Trend::NEUTRAL);
		if (s == "D") return(Trend::DOWNWARD);
		return(std::optional<Trend>());
	}

	///////////////////////////////////////////////////////////////////////////

	std::optional<RunwayStateGroup> RunwayStateGroup::parse(const std::string & group, 
			ReportPart reportPart)
	{
		static const std::optional<RunwayStateGroup> notRecognised;
		if (reportPart != ReportPart::METAR) return(notRecognised);
		static const std::regex rgx("(R\\d\\d[RCL]?)/"
			"(?:(SNOCLO)|(?:(\\d|/)(\\d|/)(\\d\\d|//)|(CLRD))(\\d\\d|//))");
		static const auto matchRunway = 1, matchSnoclo = 2, matchDeposits = 3;
		static const auto matchExtent = 4, matchDepth = 5, matchClrd = 6, matchFriction = 7;
		std::smatch match;
		if (!regex_match(group, match, rgx)) return(notRecognised);
		const auto runway = Runway::fromString(match.str(matchRunway));
		if (!runway.has_value()) return(notRecognised);
		if (match.length(matchSnoclo)) return(runwaySnoclo(runway.value()));
		const auto friction = SurfaceFriction::fromString(match.str(matchFriction));
		if (!friction.has_value()) return(notRecognised);
		if (match.length(matchClrd)) return(runwayClrd(runway.value(), friction.value()));
		const auto deposits = depositsFromString(match.str(matchDeposits));
		if (!deposits.has_value()) return(notRecognised);
		const auto extent = extentFromString(match.str(matchExtent));
		if (!extent.has_value()) return(notRecognised);
		const auto depth = Precipitation::fromRunwayDeposits(match.str(matchDepth));
		if (!depth.has_value()) return(notRecognised);
		RunwayStateGroup result;
		result.st = Status::NORMAL;
		result.rw = runway.value();
		result.dp = deposits.value();
		result.ext = extent.value();
		result.dDepth = depth.value();
		result.sf = friction.value();
		return(result);
	}

	std::optional<Group> RunwayStateGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	RunwayStateGroup RunwayStateGroup::runwaySnoclo(Runway runway) {
		RunwayStateGroup result; 
		result.rw = std::move(runway); result.st = Status::SNOCLO;
		return(result);
	}

	RunwayStateGroup RunwayStateGroup::runwayClrd(Runway runway, 
		SurfaceFriction surfaceFriction)
	{
		RunwayStateGroup result; 
		result.rw = std::move(runway); result.st = Status::CLRD;
		result.sf = std::move(surfaceFriction);
		return(result);
	}

	std::optional<RunwayStateGroup::Deposits> RunwayStateGroup::depositsFromString(
		const std::string & s)
	{
		std::optional<Deposits> error;
		if (s.length() != 1) return(error);
		switch (s[0]) {
			case '0': return(Deposits::CLEAR_AND_DRY);
			case '1': return(Deposits::DAMP);
			case '2': return(Deposits::WET_AND_WATER_PATCHES);
			case '3': return(Deposits::RIME_AND_FROST_COVERED);
			case '4': return(Deposits::DRY_SNOW);
			case '5': return(Deposits::WET_SNOW);
			case '6': return(Deposits::SLUSH);
			case '7': return(Deposits::ICE);
			case '8': return(Deposits::COMPACTED_OR_ROLLED_SNOW);
			case '9': return(Deposits::FROZEN_RUTS_OR_RIDGES);
			case '/': return(Deposits::NOT_REPORTED);
			default:  return(error);
		}
	}

	std::optional<RunwayStateGroup::Extent> RunwayStateGroup::extentFromString(
		const std::string & s)
	{
		std::optional<Extent> error;
		if (s.length() != 1) return(error);
		switch (s[0]) {
			case '0': return(Extent::NONE);
			case '1': return(Extent::LESS_THAN_10_PERCENT);
			case '2': return(Extent::FROM_11_TO_25_PERCENT);
			case '3': return(Extent::RESERVED_3);
			case '4': return(Extent::RESERVED_4);
			case '5': return(Extent::FROM_26_TO_50_PERCENT);
			case '6': return(Extent::RESERVED_6);
			case '7': return(Extent::RESERVED_7);
			case '8': return(Extent::RESERVED_8);
			case '9': return(Extent::MORE_THAN_51_PERCENT);
			case '/': return(Extent::NOT_REPORTED);
			default:  return(error);
		}
	}

	///////////////////////////////////////////////////////////////////////////

	std::optional<RainfallGroup> RainfallGroup::parse(const std::string & group, 
		ReportPart reportPart)
	{
		static const std::optional<RainfallGroup> notRecognised;
		if (reportPart != ReportPart::METAR) return(notRecognised);
		static const std::regex rgx (
			"RF(\\d\\d\\.\\d|//\\./)/(\\d\\d\\d\\.\\d|///\\./)(?:/(\\d\\d\\d\\.\\d))?");
		static const auto matchLast10Minutes = 1, matchSince9AM = 2, matchLast60Minutes = 3;
		std::smatch match;
		if (!std::regex_match(group, match, rgx)) return(notRecognised);
		const auto last10min = metaf::Precipitation::fromRainfallString(match.str(matchLast10Minutes));
		if (!last10min.has_value()) return(notRecognised);
		const auto since9AM = metaf::Precipitation::fromRainfallString(match.str(matchSince9AM));
		if (!since9AM.has_value()) return(notRecognised);
		const auto last60min = metaf::Precipitation::fromRainfallString(match.str(matchLast60Minutes));
		if (!last60min.has_value()) return(notRecognised);
		RainfallGroup result;
		result.last10m = last10min.value();
		result.since0900 = since9AM.value();
		result.last60m = last60min.value();
		return(result);
	}

	std::optional<Group> RainfallGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}
	
	///////////////////////////////////////////////////////////////////////////////

	std::optional<SeaSurfaceGroup> SeaSurfaceGroup::parse(const std::string & group, 
			ReportPart reportPart)
	{
		static const std::optional<SeaSurfaceGroup> notRecognised;
		if (reportPart != metaf::ReportPart::METAR) return(notRecognised);
		static const std::regex rgx ("W(\\d\\d|//)/([HS](?:\\d\\d?\\d?|///|/))");
		static const auto matchTemp = 1, matchWaveHeight = 2;
		std::smatch match;
		if (!std::regex_match(group, match, rgx)) return(notRecognised);
		const auto temp = metaf::Temperature::fromString(match.str(matchTemp));
		if (!temp.has_value()) return(notRecognised);
		const auto waveHeight = metaf::WaveHeight::fromString(match.str(matchWaveHeight));
		if (!waveHeight.has_value()) return(notRecognised);
		SeaSurfaceGroup result;
		result.t = temp.value();
		result.wh = waveHeight.value();
		return(result);
	}

	std::optional<Group> SeaSurfaceGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	///////////////////////////////////////////////////////////////////////////////

	std::optional<ColourCodeGroup> ColourCodeGroup::parse(const std::string & group, 
		ReportPart reportPart)
	{
		std::optional<ColourCodeGroup> notRecognised;
		if (reportPart!=ReportPart::METAR) return(notRecognised);
		ColourCodeGroup result;
		std::string colourCodeStr(group);
		if (std::string codeBlack("BLACK"); !group.find(codeBlack)) {
			result.cBlack = true;
			colourCodeStr = group.substr(codeBlack.length());
		}
		if (colourCodeStr == "BLU") { result.c = Code::BLUE; return(result); }
		if (colourCodeStr == "WHT") { result.c = Code::WHITE; return(result); }
		if (colourCodeStr == "GRN") { result.c = Code::GREEN; return(result); }
		if (colourCodeStr == "YLO1") { result.c = Code::YELLOW1; return(result); }
		if (colourCodeStr == "YLO2") { result.c = Code::YELLOW2; return(result); }
		if (colourCodeStr == "AMB") { result.c = Code::AMBER; return(result); }
		if (colourCodeStr == "RED") { result.c = Code::RED; return(result); }
		return(notRecognised);
	}

	std::optional<Group> ColourCodeGroup::combine(const Group & nextGroup) const { 
		(void)nextGroup; return(std::optional<Group>());
	}

	///////////////////////////////////////////////////////////////////////////////

	SyntaxGroup getSyntaxGroup(const Group & group) {
		if (auto fixedGroup = std::get_if<FixedGroup>(&group)) {
			switch (fixedGroup->type()) {
				case FixedGroup::Type::METAR:
				return(SyntaxGroup::METAR);
				case FixedGroup::Type::SPECI:
				return(SyntaxGroup::SPECI);
				case FixedGroup::Type::TAF:
				return(SyntaxGroup::TAF);
				case FixedGroup::Type::COR:
				return(SyntaxGroup::COR);
				case FixedGroup::Type::AMD:
				return(SyntaxGroup::AMD);
				case FixedGroup::Type::NIL:
				return(SyntaxGroup::NIL);
				case FixedGroup::Type::CNL:
				return(SyntaxGroup::CNL);
				case FixedGroup::Type::RMK:
				return(SyntaxGroup::RMK);
				default:
				return(SyntaxGroup::OTHER);
			}
		}
		if (std::get_if<LocationGroup>(&group)) return(SyntaxGroup::LOCATION);
		if (std::get_if<ReportTimeGroup>(&group)) return(SyntaxGroup::REPORT_TIME);
		if (auto trendGroup = std::get_if<TrendGroup>(&group)) {
			if (trendGroup->isTimeSpanGroup()) return(SyntaxGroup::TIME_SPAN);
			return(SyntaxGroup::OTHER);
		}
		return(SyntaxGroup::OTHER);
	}

	///////////////////////////////////////////////////////////////////////////////

	bool Parser::parse(const std::string & metarTafString, bool keepSourceGroup) {
		static const std::regex delimiterRegex("\\s+");
		std::sregex_token_iterator iter(metarTafString.begin(), 
			metarTafString.end(), 
			delimiterRegex, 
			-1);
		bool reportEnd = false;
		State state = State::REPORT_TYPE_OR_LOCATION;
		resetResult();
		while (iter != std::sregex_token_iterator() && !reportEnd && state != State::ERROR) {
			static const char reportEndChar = '=';
			std::string groupString = *iter;

			if (groupString.back() == reportEndChar) {
				reportEnd = true;
				groupString.pop_back();
			}

			if (groupString.length()) {
				Group group;

				do {
					group = GroupParser::parse(groupString, reportPartFromState(state));
					state = transition(state, getSyntaxGroup(group));
				} while(state == State::REPORT_BODY_BEGIN_METAR_REPEAT_PARSE);
				
				const auto combinedGroup = result.size() ? 
					std::visit([](auto&& previousGroup, auto && currentGroup) -> 
						std::optional<Group> {
							return (previousGroup.combine(currentGroup));
						}, result.back(), group) : 
					std::optional<Group>();

				if (!combinedGroup.has_value()) {
					//Current group is not related to previously saved group.
					result.push_back(group);
					if (keepSourceGroup) sourceGroups.push_back(groupString);
				} else {
					//Current group was combined with previously saved group.
					result.pop_back();
					result.push_back(combinedGroup.value());
					if (keepSourceGroup) {
						sourceGroups.back() += std::string(" ");
						sourceGroups.back() += groupString;
					}
				}
			}
			iter++;
		}

		state = finalTransition(state);
		return(error == Error::NONE);
	}

	void Parser::resetResult() {
		std::vector<Group>().swap(result);
		std::vector<std::string>().swap(sourceGroups);
		reportType = ReportType::UNKNOWN;
		error = Error::NONE;
	}

	Parser::State Parser::transition(State state, SyntaxGroup group) {
		switch (state) {
			case State::REPORT_TYPE_OR_LOCATION:
			reportType = ReportType::UNKNOWN;
			if (group == SyntaxGroup::METAR || group == SyntaxGroup::SPECI) {
				reportType = ReportType::METAR;
				return(State::CORRECTION);
			}
			if (group == SyntaxGroup::TAF) {
				reportType = ReportType::TAF;
				return(State::CORRECTION);
			}
			if (group == SyntaxGroup::LOCATION) {
				return(State::REPORT_TIME);
			}
			return(parseError(Error::EXPECTED_REPORT_TYPE_OR_LOCATION));

			case State::CORRECTION:
			if (group == SyntaxGroup::AMD) {
				if (reportType != ReportType::TAF) {
					return(parseError(Error::AMD_ALLOWED_IN_TAF_ONLY));
				}
				return(State::LOCATION);
			}
			if (group == SyntaxGroup::COR) return(State::LOCATION);
			if (group == SyntaxGroup::LOCATION) {
				return(State::REPORT_TIME);
			}
			return(parseError(Error::EXPECTED_LOCATION));

			case State::LOCATION:
			if (group == SyntaxGroup::LOCATION) return(State::REPORT_TIME);
			return(parseError(Error::EXPECTED_LOCATION));

			case State::REPORT_TIME:
			if (group == SyntaxGroup::REPORT_TIME) {
				if (reportType == ReportType::METAR) return(State::REPORT_BODY_BEGIN_METAR);
				return(State::TIME_SPAN);
			}
			if (group == SyntaxGroup::TIME_SPAN && reportType == ReportType::TAF) {
				return(State::REPORT_BODY_BEGIN_TAF);
			}
			if (group == SyntaxGroup::NIL) return(State::NIL);
			return(parseError(Error::EXPECTED_REPORT_TIME));

			case State::TIME_SPAN:
			if (group == SyntaxGroup::TIME_SPAN) {
				reportType = ReportType::TAF;
				return(State::REPORT_BODY_BEGIN_TAF);
			}
			if (group == SyntaxGroup::NIL) return(State::NIL);
			if (reportType == ReportType::UNKNOWN) {
				reportType = ReportType::METAR;
				return(State::REPORT_BODY_BEGIN_METAR_REPEAT_PARSE);
			}
			return(parseError(Error::EXPECTED_TIME_SPAN));

			case State::REPORT_BODY_BEGIN_METAR:
			case State::REPORT_BODY_BEGIN_METAR_REPEAT_PARSE:
			if (group == SyntaxGroup::NIL) return(State::NIL);
			if (group == SyntaxGroup::CNL) return(parseError(Error::CNL_ALLOWED_IN_TAF_ONLY));
			if (group == SyntaxGroup::RMK) return(State::REMARK);
			return(State::REPORT_BODY_METAR);

			case State::REPORT_BODY_METAR:
			if (group == SyntaxGroup::RMK) return(State::REMARK);
			if (group == SyntaxGroup::NIL || group == SyntaxGroup::CNL) {
				return(parseError(Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY));
			}
			return(State::REPORT_BODY_METAR);

			case State::REPORT_BODY_BEGIN_TAF:
			if (group == SyntaxGroup::NIL) return(State::NIL);
			if (group == SyntaxGroup::CNL) return(State::CNL);
			if (group == SyntaxGroup::RMK) return(State::REMARK);
			return(State::REPORT_BODY_TAF);

			case State::REPORT_BODY_TAF:
			if (group == SyntaxGroup::RMK) return(State::REMARK);
			if (group == SyntaxGroup::NIL || group == SyntaxGroup::CNL) {
				return(parseError(Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY));
			}
			return(State::REPORT_BODY_TAF);

			case State::REMARK:
			return(State::REMARK);

			case State::NIL:
			return(parseError(Error::UNEXPECTED_GROUP_AFTER_NIL));

			case State::CNL:
			return(parseError(Error::UNEXPECTED_GROUP_AFTER_CNL));

			case State::ERROR:
			return(State::ERROR);

			default:
			return(parseError(Error::INTERNAL_PARSER_STATE));
		}
	}

	ReportPart Parser::reportPartFromState(State state) {
		using StateReportPart = std::pair<State, ReportPart>;
		static const std::vector<StateReportPart> stateReportParts = {
			std::make_pair(State::REPORT_TYPE_OR_LOCATION, ReportPart::HEADER),
			std::make_pair(State::CORRECTION, ReportPart::HEADER),
			std::make_pair(State::LOCATION, ReportPart::HEADER),
			std::make_pair(State::REPORT_TIME, ReportPart::HEADER),
			std::make_pair(State::TIME_SPAN, ReportPart::HEADER),
			std::make_pair(State::REPORT_BODY_BEGIN_METAR, ReportPart::METAR),
			std::make_pair(State::REPORT_BODY_BEGIN_METAR_REPEAT_PARSE, ReportPart::METAR),
			std::make_pair(State::REPORT_BODY_METAR, ReportPart::METAR),
			std::make_pair(State::REPORT_BODY_BEGIN_TAF, ReportPart::TAF),
			std::make_pair(State::REPORT_BODY_TAF, ReportPart::TAF),
			std::make_pair(State::REMARK, ReportPart::RMK),
			std::make_pair(State::NIL, ReportPart::UNKNOWN),
			std::make_pair(State::CNL, ReportPart::UNKNOWN),
			std::make_pair(State::ERROR, ReportPart::UNKNOWN)
		};
		for (const auto & srp : stateReportParts) {
			if (std::get<State>(srp) == state) return(std::get<ReportPart>(srp));
		}
		return(ReportPart::UNKNOWN);
	}

	Parser::State Parser::finalTransition(State state) {
		switch (state) {
			case State::REPORT_BODY_METAR:
			case State::REPORT_BODY_TAF:
			case State::REMARK:
			case State::NIL:
			case State::CNL:
			case State::ERROR:
			return(state);

			case State::REPORT_TYPE_OR_LOCATION:
			return(parseError(Error::EMPTY_REPORT));
			
			case State::CORRECTION:
			case State::LOCATION:
			case State::REPORT_TIME:
			case State::TIME_SPAN:
			case State::REPORT_BODY_BEGIN_METAR:
			case State::REPORT_BODY_BEGIN_METAR_REPEAT_PARSE:
			case State::REPORT_BODY_BEGIN_TAF:
			return(parseError(Error::UNEXPECTED_REPORT_END));

			default:
			return(parseError(Error::INTERNAL_PARSER_STATE));
		}
	}


}; //namespace metaf

#endif //#ifndef METAF_H