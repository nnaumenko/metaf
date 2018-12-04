/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

/// @file 
/// @brief METAR or TAF report parser, helpers and structs representing 
/// parsed METAR or TAF groups.

#ifndef METAR_H
#define METAR_H

#include <string>
#include <vector>
#include <variant>

namespace metaf {

	//Structs of individual METAR or TAF groups used with Group variant

	struct PlainTextGroup;
	struct FixedGroup;
	struct LocationGroup;
	struct ReportTimeGroup;
	struct TimeSpanGroup;
	struct TrendTimeGroup;
	struct ProbabilityGroup;
	struct WindGroup;
	struct VarWindGroup;
	struct WindShearGroup;
	struct VisibilityGroup;
	struct CloudGroup;
	struct VerticalVisibilityGroup;
	struct WeatherGroup;
	struct TemperatureGroup;
	struct MinMaxTemperatureGroup;
	struct PressureGroup;
	struct RunwayVisualRangeGroup;
	struct RunwayStateGroup;
	struct RainfallGroup;
	struct SeaSurfaceGroup;
	struct SeaWavesGroup;
	struct ColourCodeGroup;

	/// @brief A variant type for all possible METAR and TAF groups.
	/// @details Used by parser to return parsed group information.
	/// @par Group is expected to include PlainTextGroup, otherwise 
	/// GroupParser will not compile.
	using Group = std::variant<
		PlainTextGroup,
		FixedGroup,
		LocationGroup,
		ReportTimeGroup,
		TimeSpanGroup,
		TrendTimeGroup,
		ProbabilityGroup,
		WindGroup,
		VarWindGroup,
		WindShearGroup,
		VisibilityGroup,
		CloudGroup,
		VerticalVisibilityGroup,
		WeatherGroup,
		TemperatureGroup,
		MinMaxTemperatureGroup,
		PressureGroup,
		RunwayVisualRangeGroup,
		RunwayStateGroup,
		RainfallGroup,
		SeaSurfaceGroup,
		SeaWavesGroup,
		ColourCodeGroup
	>;

	/// Major structural part of the report
	enum class ReportPart {
		UNKNOWN,	///< Unknown part
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

	/// Returns SyntaxGroup classification of METAR or TAF group.
	/// @param group A parsed METAR or TAF group to classify.
	/// @return Corresponding SyntaxGroup or SyntaxGroup::OTHER if 
	/// group is not important for report syntax.
	SyntaxGroup getSyntaxGroup(const Group & group);

	/// @brief Base class for structs within Group variant.
	/// @details GroupBase provides infrastructure for structs which
	/// hold information decoded by parser.
	/// @par parse() is required by GroupParser to recognise and parse
	/// strings as individual groups.
	/// @par isValid() method is currently not required and used for 
	/// validating the data.
	/// @par nextGroup() is used to combine sequential groups which 
	/// represent the same data (e.g. 1 1/2SM, 1 and 1/2SM are technically
	/// different groups but represent different parts of the same value). 
	/// nextGroup() is required by parser.
	/// @par The individual types might either inherit from GroupBase or 
	/// implement these methods independently.
	struct GroupBase {
	public:
		GroupBase() = default;
		/// @brief Attempt to parse a string as a METAR or TAF group.
		/// @param group Part of METAR or TAF report to be parsed.
		/// @param reportPart Major structural part of the report where
		/// string belongs.
		/// @return True if parsing was successful and struct fields are
		/// initialised with parsed data, or false if string cannot be
		/// recognised as this type of METAR or TAF group.
		bool parse(const std::string & group, ReportPart reportPart) {
			(void)group; (void)reportPart; return(false);
		}
		/// Checks consistency and validity of data in struct fields.
		/// @return True if the data in struct fields are valid and
		/// consistent, false otherwise.
		bool isValid() const { return(false); }
		/// @brief Used to combine sequence of METAR or TAF groups 
		/// which represent the same data (e.g. 1 1/2SM, 1 and 1/2SM 
		/// are technically different METAR/TAF groups but represent 
		/// different parts of the same visibility value). 
		/// @param nextGroup The METAR or TAF group which follows this
		/// METAR or TAF group.
		/// @return True if the METAR/TAF groups were combined and 
		/// fields of this struct already contain all new data from 
		/// the following group and thus there is no need to save the 
		/// following group. False if groups cannot be combined and 
		/// the following group must be treated separately.
		bool nextGroup(const Group & nextGroup) { return(false); }
	};

	/// Distance measurement units, used across groups
	enum class DistanceUnit {
		UNKNOWN = 30,
		METERS = 0,
		STATUTE_MILES = 1,
		FEET = 2
	};

	/// Speed measurement units, used across groups
	enum class SpeedUnit {
		UNKNOWN = 30,
		KNOTS = 0,
		METERS_PER_SECOND = 1,
		KILOMETERS_PER_HOUR = 2
	};
	
	/// Temperature measurement units, used across groups
	enum class TemperatureUnit {
		UNKNOWN = 30,
		DEGREES_C = 0
	};

	/// Runway identification
	struct Runway {
		/// Runway designator
		enum class Designator {
			UNKNOWN = 30,
			NONE = 0,
			LEFT = 1,
			CENTER = 2,
			RIGHT = 3
		};
		Runway() = default;
		/// Initialise Runway based on its number and designator
		/// @param number Runway number
		/// @param designator Runway designator, default is NONE
		Runway(unsigned int number, Designator designator = Designator::NONE);
		/// @brief Decode runway designator from char
		/// @details R/L/C are treates as Right/Left/Center, space or 
		/// null-terminator are treated as None, every other char is treated as
		/// Unknown
		/// @par c Designator as char
		/// @return Decoded designator or UNKNOWN if designator is not recognised
		static Designator designatorFromChar(char c);
		/// @brief Decode runway designator from string
		/// @details If input string has zero length, the designator is None. If 
		/// input string has length greater than one, the designator is Unknown.
		/// If input string has length of exactly one, the designator is as per
		/// designatorFromChar.
		/// @par s Designator as string
		/// @return Decoded designator or UNKNOWN if designator is not recognised
		static Designator designatorFromString(const std::string & s);
		/// Runway number
		unsigned int number = 0;
		/// Runway designator
		Designator designator = Designator::UNKNOWN;
	};

	bool operator ==(const Runway & lhs, const Runway & rhs);

	/// @brief Group: Plain Text
	/// @details When the group could not be recognised to conform
	/// any of the other group formats, it is saved as Plain Text
	/// @par In this implementation maximum length of plain text 
	/// stored within this struct is limited to textMaxLength chars. 
	/// All extra chars will be ommitted.
	struct PlainTextGroup : public GroupBase {
	public:
		PlainTextGroup() = default;
		/// Initialise Plain Text group with a string. If string length
		/// exceeds textMaxLength, only textMaxLength chars are stored.
		/// @param text Plain Text string to be stored
		PlainTextGroup(std::string text);
		/// Attempt to parse group. See GroupBase::parse for details.
		/// This method is expected to always parse successfully and
		/// always return true.
		bool parse(const std::string & group, ReportPart reportPart);
		/// Maximum number of chars which can be stored in this struct.
		static const auto textMaxLength = 15;
		char text [textMaxLength + 1] = "\0";	///< Stored plain text.
	};

	bool operator ==(const PlainTextGroup & lhs, const PlainTextGroup & rhs);

	/// @brief Group: fixed text.
	/// @details This group represents a fixed text which never varies in 
	/// reports.
	struct FixedGroup : public GroupBase {
		/// Group options for fixed texts.
		enum class Type {
			UNKNOWN = 30,
			METAR = 0,		///< Report type: METAR
			SPECI = 1,		///< Report type: SPECI
			TAF = 2,		///< Report type: TAF
			AMD = 3,		///< Amended TAF report
			NIL = 4,		///< Missing report
			CNL = 5,		///< Cancelled TAF report
			COR = 6,		///< Correctional report
			AUTO = 7,		///< Automatic report without human supervision
			CLR = 8,		///< Clear sky
			NCD = 9,		///< No clouds detected
			NOSIG = 10,		///< No significant weather changes expected
			AIRPORT_SNOCLO = 11,	///< Airport closed due to snow accumulation
			CAVOK = 12,		///< Ceiling and visibility OK
			NSC = 13,		///< No significant cloud
			NSW = 14,		///< No significant weather
			TEMPO = 15,		///< Temporary weather phenomena
			BECMG = 16,		///< Weather conditions are changing
			INTER = 17,		///< Intermittent weather changes (for <30 min), Australia only?
			RMK = 18,		///< Remarks
			SKC = 19,		///< Sky clear
		};
		FixedGroup() = default;
		/// Initialise group with a specified type.
		/// @param type Type to specify.
		FixedGroup(Type type);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Type type = Type::UNKNOWN; ///< Type of fixed text in group.
	private:
		struct GroupData;
		static const std::vector<struct GroupData> group_data;
	};

	bool operator ==(const FixedGroup & lhs, const FixedGroup & rhs);

	/// @brief Group: ICAO location.
	/// @details ICAO location where report applies.
	struct LocationGroup : public GroupBase {
		LocationGroup() = default;
		/// Initialise group with ICAO location string.
		/// @param location Location string; if string size is greater than 4
		/// then only first 4 characters will be stored.
		LocationGroup(std::string location);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		static const auto locationLength = 4; ///< ICAO location length.
		char location [locationLength + 1] = "\0"; ///< ICAO location.
	};

	bool operator ==(const LocationGroup & lhs, const LocationGroup & rhs);

	/// @brief Group: Report time.
	/// @details: Day and time when report was issued.
	struct ReportTimeGroup : public GroupBase {
		ReportTimeGroup() = default;
		/// Initialise report with day and time.
		/// @param day Day when report was issued.
		/// @param hour Hour when report was issued.
		/// @param minute Minute when report was issued.
		ReportTimeGroup(unsigned int day, unsigned int hour, unsigned int minute);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int day = 0; ///< Day when report was issued.
		unsigned int hour = 0; ///< Hour when report was issued.
		unsigned int minute = 0; ///< Minute when report was issued.
	};

	bool operator ==(const ReportTimeGroup & lhs, const ReportTimeGroup & rhs);

	/// @brief Group: Time span.
	/// @details: Time span when report or trend is applicable.
	struct TimeSpanGroup : public GroupBase {
		TimeSpanGroup() = default;
		/// Initialise group with a day/hour from and until which report is
		/// applicable.
		/// @param dayFrom Day from which the report is applicable.
		/// @param hourFrom Hour from which the report is applicable.
		/// @param dayTill Day until which the report is applicable.
		/// @param hourTill Hour until which the report is applicable.
		TimeSpanGroup(unsigned int dayFrom,	
			unsigned int hourFrom, 
			unsigned int dayTill, 
			unsigned int hourTill);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int dayFrom = 0;  ///< Day from which the report is applicable.
		unsigned int hourFrom = 0; ///< Hour from which the report is applicable.
		unsigned int dayTill = 0;  ///< Day until which the report is applicable.
		unsigned int hourTill = 0; ///< Hour until which the report is applicable.
	};

	bool operator ==(const TimeSpanGroup & lhs, const TimeSpanGroup & rhs);

	/// @brief Group: trend time.
	/// @details Time when trend is applicable.
	struct TrendTimeGroup : public GroupBase {
		enum class Type {
			UNKNOWN = 30,
			FROM = 0,		///< Trend is applicable FROM specified time.
			TILL = 1,		///< Trend is applicable UNTIL specified time.
			AT = 2			///< Trend is applicable AT specified time.
		};
		TrendTimeGroup () = default;
		/// Initialised group with a day and time (long trend time group).
		/// @param type Trend time type (FROM, UNTIL, AT).
		/// @param day Day of the trend time.
		/// @param hour Hour of the trend time.
		/// @param minute Minute of the trend time.
		TrendTimeGroup (Type type,
			unsigned int day, 
			unsigned int hour, 
			unsigned int minute);
		/// Initialised group with time only (short trend time group)
		/// @param type Trend time type (FROM, UNTIL, AT)
		/// @param hour Hour of the trend time
		/// @param minute Minute of the trend time
		TrendTimeGroup (Type type, unsigned int hour, unsigned int minute);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Type type = Type::UNKNOWN;	///< Trend time type (FROM, UNTIL, AT)
		unsigned int day = 0;		///< Day of the trend time (if included)
		unsigned int hour = 0;		///< Hour of the trend time
		unsigned int minute = 0;	///< Minute of the trend time
		bool dayReported = false;	///< Is day included in the trend time
	};

	bool operator ==(const TrendTimeGroup & lhs, const TrendTimeGroup & rhs);

	/// @brief Group: forecast probability.
	/// @details Specifies forecast probability (30% or 40% only).
	struct ProbabilityGroup : public GroupBase {
		ProbabilityGroup() = default;
		/// Initialises the group with probability value.
		/// @param probability Probability value.
		ProbabilityGroup(unsigned int probability);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int probability = 0;	///< Probability value.
	};

	bool operator ==(const ProbabilityGroup & lhs, const ProbabilityGroup & rhs);

	/// @brief Group: wind.
	/// @details Specifies wind speed, direction and gust speed.
	struct WindGroup : public GroupBase {
		WindGroup() = default;
		/// Initialises group where no wind direction or speed is reported.
		/// @param unit Wind speed unit.
		WindGroup(SpeedUnit unit);
		/// Initialises group where wind direction, speed and optionally gust
		/// speed is reported.
		/// @param direction Wind direction ("from") in degrees. Must be rounded 
		/// to 10 degrees. True north is 360 degrees. When calm, direction 0 is 
		/// reported.
		/// @param unit Wind speed unit.
		/// @param speed Wind speed in specified units.
		/// @param gustSpeed Wind gust speed in specified units (optional).
		WindGroup(unsigned int direction, 
			SpeedUnit unit, 
			unsigned int speed, 
			unsigned int gustSpeed = 0);
		/// Initialises group where wind direction is variable
		/// @param unit Wind speed unit.
		/// @param speed Wind speed in specified units.
		/// @param gustSpeed Wind gust speed in specified units (optional).
		/// @return Initialised wind group.
		static WindGroup makeVariableDirection (SpeedUnit unit, 
			unsigned int speed, 
			unsigned int gustSpeed = 0);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		/// Wind direction "from" in degrees (if reported and not variable).
		unsigned int direction = 0;		
		bool directionReported = false;	///< Is wind direction is reported.
		bool directionVariable = false; ///< Is wind direction is variable.
		unsigned int speed = 0;			///< Wind speed.
		bool speedReported = false;		///< Is wind speed reported.
		unsigned int gustSpeed = 0;		///< Wind gust speed.
		bool gustSpeedReported = false; ///< Is wind gust speed reported.
		SpeedUnit unit = SpeedUnit::UNKNOWN; ///< Wind speed measurement units.
	};

	bool operator ==(const WindGroup & lhs, const WindGroup & rhs);

	/// @brief Group: variable wind direction.
	/// @details Specifies sector of direction when the wind is variable.
	/// @par Wind directions "from" and "to" are specified in clockwise order.
	struct VarWindGroup : public GroupBase {
		VarWindGroup() = default;
		/// Initialises group with variable wind directions
		/// @param directionFrom Wind direction "from"
		/// @param directionTo Wind direction "to"
		VarWindGroup(unsigned int directionFrom, unsigned int directionTo);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int directionFrom = 0;	///< Wind direction "from"
		unsigned int directionTo = 0;	///< Wind direction "to"
	};

	bool operator ==(const VarWindGroup & lhs, const VarWindGroup & rhs);

	/// @brief Group: wind shear group.
	/// @details Reports wind shear at specified height.
	struct WindShearGroup : public GroupBase {
		WindShearGroup() = default; 
		/// Initialises group with wind shear height, and wind direction & 
		/// speed
		/// @param height Height where wind shear occurs.
		/// @param direction Wind direction ("from") in degrees. Must be rounded 
		/// to 10 degrees. True north is reported as 360 degrees.
		/// @param speed Wind speed in specified units.
		/// @param speedUnit Wind speed unit.
		WindShearGroup(unsigned int height,
			unsigned int direction, 
			unsigned int speed,	
			SpeedUnit speedUnit);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int height = 0;	///> Height where wind shear occurs.
		unsigned int direction = 0;	///> Wind direction in degrees
		unsigned int speed = 0;		///> Wind speed
		/// Wind speed measurement units
		SpeedUnit speedUnit = SpeedUnit::UNKNOWN;	
		/// Height measurement units
		static const DistanceUnit heightUnit = DistanceUnit::FEET;
	};

	bool operator ==(const WindShearGroup & lhs, const WindShearGroup & rhs);

	/// @brief Group: prevailing visibility.
	/// @details Reports prevailing or directional visibility.
	struct VisibilityGroup : public GroupBase {
		///
		enum class Direction {
			UNKNOWN = 30,
			NONE = 0,	///< No direction included in group
			NDV = 1,	///< No directional variation in visibility (AUTO reports)
			N = 2,		///< North
			S = 3,		///< South
			W = 4,		///< West
			E = 5,		///< East
			NW = 6,		///< Northwest
			NE = 7,		///< Northeast
			SW = 8,		///< Southwest
			SE = 9		///< Southeast
		};
		VisibilityGroup() = default;
		/// Initialise group with non-reported visibility (but units are 
		/// meters), with optional direction.
		/// @param direction Direction with which visibility value is 
		/// applicable (optional).
		/// @return Initialised visibility group.
		static VisibilityGroup makeVisibilityMeters(Direction direction = Direction::NONE);
		/// Initialise group with visibility value in meters, with optional 
		/// direction.
		/// @param visibility Visibility in meters.
		/// @param direction Direction with which visibility is not reported 
		/// (optional).
		/// @return Initialised visibility group.
		static VisibilityGroup makeVisibilityMeters(unsigned int visibility, 
			Direction direction = Direction::NONE);
		/// Initialise group with non-reported visibility (but units are 
		/// statute miles).
		/// @return Initialised visibility group.
		static VisibilityGroup makeVisibilityMiles();
		/// Initialise group with visibility value in statute miles, with integer, 
		/// and fraction part. Optionally "less than" modifier can be specified.
		/// @param integer Integer part of visibility in statute miles.
		/// @param numerator Numerator of visibility in statute miles.
		/// @param denominator Denominator of visibility in statute miles.
		/// @param lessThan True if visibility is less than specified value.
		/// @return Initialised visibility group.
		static VisibilityGroup makeVisibilityMiles(unsigned int integer, 
			unsigned int numerator = 0, 
			unsigned int denominator = 0,
			bool lessThan = false);
		/// @brief Initialise group with incomplete visibility value in statute 
		/// miles.
		/// @details When visibility is specified with integer and fraction parts
		/// separated by space (e.g. 1 1/2SM), then technically 1 and 1/2SM are 
		/// independent groups, though they represent the same data. When first part
		/// of such group is detected, it is marked as "incomplete" in order to 
		/// allow combining with second part of group.
		/// @param integer Integer part of visibility in statute miles.
		static VisibilityGroup makeVisibilityMilesIncomplete(unsigned int integer);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		/// Check consistency and validity of struct fields. See GroupBase::isValid 
		/// for details.
		bool isValid() const;
		/// Attempt to combine this group with the following group. See 
		/// GroupBase::nextGroup for details.
		bool nextGroup(const Group & nextGroup);
		unsigned int integer = 0;		///< Integer part of visibility
		unsigned int numerator = 0;		///< Numerator of fraction part of visibility
		unsigned int denominator = 0;	///< Denominator of fraction part of visibility
		DistanceUnit unit = DistanceUnit::UNKNOWN;///< Visibility measurement units
		Direction direction = Direction::UNKNOWN;///< Direction with which visibility is reported
		bool reported = false;	///< Is visibility value reported
		bool lessThan = false;	///< Is visibility reported as "less than specified value"
		bool incompleteInteger = false; ///< See makeVisibilityMilesIncomplete() for details
	private:
		bool isIncompleteInteger() const;
		bool isIncompleteFraction() const;

	};

	bool operator ==(const VisibilityGroup & lhs, const VisibilityGroup & rhs);

	/// @brief Group: cloud layer.
	/// @details Reports height, amount (cover) and type of a cloud layer.
	struct CloudGroup : public GroupBase {
		/// Amount (cover) of clouds.
		enum class Amount {
			UNKNOWN = 30,
			NOT_REPORTED = 31,
			FEW = 0,		///< Few clouds (1/8 to 2/8 cover).
			SCATTERED = 1,	///< Scattered clouds (3/8 to 4/8 cover).
			BROKEN = 2,		///< Broken clouds (5/8 to 7/8 cover).
			OVERCAST = 3	///< Overcast (8/8 cover).
		};
		/// Type of clouds.
		enum class Type {
			UNKNOWN = 30,
			NONE = 31,				///< Not towering cumulus or cumulonimbus.
			NOT_REPORTED = 0,
			TOWERING_CUMULUS = 1,	///< Towering cumulus clouds.
			CUMULONIMBUS = 2		///< Cumulonimbus clouds.
		};
		CloudGroup () = default;
		/// Initialises group with amount, height and type of cloud cover.
		/// @param amount Amount (cover) of clouds.
		/// @param height Height of cloud layer base (in feet).
		/// @param type Type of clouds.
		CloudGroup (Amount amount, unsigned int height, Type type = Type::NONE);
		/// Initialises group with amount, type of cloud cover but height is not reported.
		/// @param amount Amount (cover) of clouds.
		/// @param type Type of clouds.
		CloudGroup (Amount amount, Type type = Type::NONE);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Amount amount = Amount::UNKNOWN;	///< Amount (cover) of clouds.
		unsigned int height = 0;			///< Height of cloud layer base.
		Type type = Type::UNKNOWN;			///< Type of clouds.
		bool heightReported = false;		///< Is height of cloud layer base reported.
		static const DistanceUnit heightUnit = DistanceUnit::FEET; ///< Height measurement units
	};

	bool operator ==(const CloudGroup & lhs, const CloudGroup & rhs);

	/// @brief Group: vertical visibility.
	/// @details When clouds are impossible to determine, e.g. due to fog or 
	/// precipitation, vertical visibility is reported instead.
	struct VerticalVisibilityGroup : public GroupBase {
		VerticalVisibilityGroup() = default;
		/// Initialise group with vertical visibility value.
		/// @param verticalVisibility Vertical visibility in feet.
		VerticalVisibilityGroup(unsigned int verticalVisibility);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int vertVisibility = 0;	///< Vertical visibility in feet.
		bool reported = false;				///< Is vertical visibility reported.
		static const DistanceUnit unit = DistanceUnit::FEET; ///Vert.visibility measurement units.
	};

	bool operator ==(const VerticalVisibilityGroup & lhs, const VerticalVisibilityGroup & rhs);

	/// @brief Group: weather conditions.
	/// @details Reports obscuration, precipitation or other weather phenomena.
	struct WeatherGroup : public GroupBase {
		/// Intensity or proximity
		enum class Modifier {
			UNKNOWN = 30,
			NONE = 0,		///< No modifier with precipitation means "moderate".
			RECENT = 1,		///< Recent weather is reported.
			VICINITY = 2,	///< Weather phenomena in vicinity (up to 10 miles) is reported.
			LIGHT = 3,		///< Light (e.g. precipitation).
			HEAVY = 4		///< Heavy (e.g. precipitation).
		};
		/// Obscuration, precipitation or other weather phenomena.
		enum class Weather {
			UNKNOWN = 30,
			NOT_REPORTED = 31,
			/// Shallow, little vertical extent (less than 6 feet).
			SHALLOW = 0,
			/// Partial (covers part of airfield).
			PARTIAL = 1,
			/// Patches.
			PATCHES = 2,		
			/// Dust, snow or sand raised by wind to less than 6 feet.
			LOW_DRIFTING = 3,
			/// Dust, snow or sand raised by wind to 6 feet or more.
			BLOWING = 4,
			/// Precipitation characterised by sudden start or stop or change 
			/// of intensity.
			SHOWERS = 5,		
			/// Thunderstorm (accompanied by lightning or thunder).
			THUNDERSTORM = 6,
			/// Precipitation forming ice glase on ground or fog at freezing 
			/// temperatures (regardless of whether it actually deposits rime).
			FREEZING = 7,
			/// Precipitation of very fine water droplets (falls to the ground 
			/// unlike fog and mist).
			DRIZZLE = 8,
			/// Precipitation in form of water drops or droplets.
			RAIN = 9,
			/// Precipitation in form of branched (unlike ice crystals) ice 
			/// crystals.
			SNOW = 10,
			/// Very small, opaque, white grains of ice (2 to 5 mm).
			SNOW_GRAINS = 11,	
			/// Non-branched (unlike snow) ice crystals in form of needles, 
			/// columns or plates (a.k.a. diamond dust).
			ICE_CRYSTALS = 12,
			/// Translucent pellets of ice (up to 5 mm, in form of frozen 
			/// raindrops or melted and refrozen snowflakes) or snow pellets 
			/// encased in thin ice layer.
			ICE_PELLETS = 13,
			/// Small balls or other pieces of ice, can be frozen together in
			/// irregular lumps.
			HAIL = 14,
			/// White, small, opaque grains of ice (2 to 5 mm).
			SMALL_HAIL = 15,	
			/// Precipitation detected by automated station but precipitation 
			/// type cannot be determined.
			UNDETERMINED = 16,
			/// Water particles suspended in atmosphere, visibility from 5/8 
			/// to 7 statute miles (unlike drizzle does not fall on ground).
			MIST = 17,
			/// Water particles suspended in atmosphere, visibility less than
			/// 5/8 statute miles (unlike drizzle does not fall on ground).
			FOG = 18,
			/// Smoke
			SMOKE = 19,
			/// Fine particles of rock powder suspended in atmosphere.
			VOLCANIC_ASH = 20,
			/// Dust (widespread dust). Dust raised by wind.
			DUST = 21,
			/// Sand raised by wind.
			SAND = 22,
			/// Suspension of very small dry particles invisible to naked eye.
			HAZE = 23,
			/// Water droplets torn by wind from surface of body of water.
			SPRAY = 24,
			/// Well developen dust or sand whirls (a.k.a. dust devils).
			DUST_WHIRLS = 25,
			/// Suddenly increasing strong wind (increases by at least 16 
			/// knots and sustains at 22 knots for at least 1 minute).
			SQUALLS = 26,
			/// Tornado, funnel cloud or waterspout (tornadic activity).
			FUNNEL_CLOUD = 27,
			/// Particles of sand carried aloft by a strong wind.
			SANDSTORM = 28,
			/// Dust-filled air and strong winds.
			DUSTSTORM = 29
		};
		WeatherGroup() = default;
		/// Initialises weather group with modifier and vector of weather 
		/// phenomena.
		/// @param modifier Intensity or proximity of weather phenomena.
		/// @param weather Vector of obscurations, precipitation or weather 
		/// phenomena. If more than maxWeatherSize are specified, only first
		/// maxWeatherSize number of item are stored.
		WeatherGroup(Modifier modifier, const std::vector<Weather> & weather);
		/// Initialises weather group with modifier and up to three weather 
		/// phenomena.
		/// @param modifier Intensity or proximity of weather phenomena.
		/// @param weather1 Obscurations, precipitation or weather phenomenon.
		/// @param weather2 Optional weather phenomenon.
		/// @param weather3 Optional weather phenomenon.
		WeatherGroup(Modifier modifier, 
			Weather weather1, 
			Weather weather2 = Weather::UNKNOWN, 
			Weather weather3 = Weather::UNKNOWN);
		/// Initialises weather group with not reported present or recent 
		/// weather.
		static WeatherGroup makeNotReported(bool recent = false);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		/// Convert weather phenomena stored in group to vector.
		std::vector<Weather> weatherToVector() const;
		Modifier modifier = Modifier::UNKNOWN; ///< Intensity or proximity of weather phenomena.
		static const auto maxWeatherSize = 8; ///< Maximum number of stored weather phenomena.
		Weather weather[maxWeatherSize] = {Weather::UNKNOWN}; ///< Stored weather phenomena.
		size_t weatherSize = 0; ///< Number of stored weather phenomena.
	private:
		static Modifier modifierFromString(const std::string & str);
		static Weather weatherFromString(const std::string & str);
	};

	bool operator ==(const WeatherGroup & lhs, const WeatherGroup & rhs);

	/// @brief Group: temperature and dew point.
	/// @details Reports ambient air temperature and dew point.
	struct TemperatureGroup : public GroupBase {
		TemperatureGroup() = default;
		/// Initialise group where only temperature is reported.
		/// @param temperature Ambient air temperature.
		TemperatureGroup(int temperature);
		/// Initialise group where both temperature and dew point are reported.
		/// @param temperature Ambient air temperature.
		/// @param dewPoint Dew point.
		TemperatureGroup(int temperature, int dewPoint);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		int temperature = 0;				///< Ambient air temperature.
		bool temperatureReported = false;	///< Is ambient air temperature reported.
		int dewPoint = 0; 					///< Dew point.
		bool dewPointReported = false;		///< Is dew point reported.
		/// Temperture measurement units.
		static const TemperatureUnit unit = TemperatureUnit::DEGREES_C; 
	};

	bool operator ==(const TemperatureGroup & lhs, const TemperatureGroup & rhs);

	/// @brief Group: minimum or maximum temperature.
	/// @details Minimum or maximum temperature forecast with specified time 
	/// when minimum or maximum temperatures are expected.
	struct MinMaxTemperatureGroup : public GroupBase {
		/// Temperature point: minimum or maximum
		enum class Point {
			UNKNOWN = 30,
			MINIMUM = 0,
			MAXIMUM = 1
		};
		MinMaxTemperatureGroup() = default;
		/// Initialise group with minimum temperature forecast.
		/// @param temperature Minimum temperature expected.
		/// @param day Day when specified temperature is expected.
		/// @param hour Hour when specified temperature is expected.
		static MinMaxTemperatureGroup makeMin(int temperature,
			unsigned int day, 
			unsigned int hour);
		/// Initialise group with maximum temperature forecast.
		/// @param temperature Maximum temperature expected.
		/// @param day Day when specified temperature is expected.
		/// @param hour Hour when specified temperature is expected.
		static MinMaxTemperatureGroup makeMax(int temperature, 
			unsigned int day, 
			unsigned int hour);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Point point = Point::UNKNOWN;	///< Temperature point (minimum or maximum)
		int temperature = 0;			///< Temperature value (minimum or maximum)
		unsigned int day = 0;		///< Day when specified temperature is expected.
		unsigned int hour = 0;		///< Hour when specified temperature is expected.
		/// Temperture measurement units.
		static const TemperatureUnit unit = TemperatureUnit::DEGREES_C;
	};

	bool operator ==(const MinMaxTemperatureGroup & lhs, const MinMaxTemperatureGroup & rhs);

	/// @brief Group: atmospheric pressure.
	/// @details Reports current atmospheric pressure.
	struct PressureGroup : public GroupBase {
		/// Pressure measurement units.
		enum class Unit{
			UNKNOWN = 30,
			HECTOPASCAL = 0,	///< Hectopascal (millibar)
			INCHES_HG = 1		///< Inches mercury
		};
		PressureGroup() = default;
		/// Initialises group where atmospheric pressure is not reported.
		/// @param unit Pressure measurement units.
		PressureGroup(PressureGroup::Unit unit);
		/// Initialises group with atmospheric pressure value
		/// @param pressure Current atmospheric pressure.
		/// @param unit Pressure measurement units.
		PressureGroup(float pressure, PressureGroup::Unit unit);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		float pressure = 0.0; 		///< Current atmospheric pressure value.
		bool reported = false;		///< Is pressure value reported.
		Unit unit = Unit::UNKNOWN; 	///< Pressure measurement units.

	};

	bool operator ==(const PressureGroup & lhs, const PressureGroup & rhs);

	/// @brief Group: runway visual range.
	/// @details Runway visual range is an instrumentally-derived value 
	/// that represents the horizontal distance a pilot may see down the
	/// runway from the approach end.
	struct RunwayVisualRangeGroup : public GroupBase {
		/// Distance modifier ("none", "less than" or "more than").
		enum class Modifier {
			UNKNOWN = 30,
			NONE = 0,
			LESS_THAN = 1,
			MORE_THAN = 2
		};
		/// RVR trend (upward, neutral or downward).
		enum class Trend {
			UNKNOWN = 30,
			NONE = 0,
			UPWARD = 1,
			NEUTRAL = 2,
			DOWNWARD = 3
		};
		RunwayVisualRangeGroup() = default;
		/// Initialise group with non-reported RVR value.
		/// @param runway Runway RVR is reported for.
		/// @param unit Distance measurement units.
		/// @param trend Optional RVR trend (upward, neutral or downward).
		RunwayVisualRangeGroup(const Runway & runway,
			DistanceUnit unit,
			Trend trend = Trend::NONE);
		/// Initialise group with a single RVR value.
		/// @param runway Runway RVR is reported for.
		/// @param visRange Runway visual range value.
		/// @param unit Distance measurement units.
		/// @param modifier Optional distance modifier (less than / more than).
		/// @param trend Optional RVR trend (upward, neutral or downward).
		RunwayVisualRangeGroup(const Runway & runway,
			unsigned int visRange, 
			DistanceUnit unit,
			Modifier modifier = Modifier::NONE,
			Trend trend = Trend::NONE);
		/// Initialise group with range of two RVR values ("from" and "to").
		/// @param runway Runway RVR is reported for.
		/// @param minVisRange Runway visual range minimum value.
		/// @param minVisRange Runway visual range maximum value.
		/// @param unit Distance measurement units.
		/// @param minVisModifier Optional minimum range modifier (less 
		/// than / more than).
		/// @param maxVisModifier Optional maximum range modifier (less 
		/// than / more than).
		/// @param trend Optional RVR trend (upward, neutral or downward).
		RunwayVisualRangeGroup(const Runway & runway,
			unsigned int minVisRange,
			unsigned int maxVisRange,
			DistanceUnit unit,
			Modifier minVisModifier = Modifier::NONE,
			Modifier maxVisModifier = Modifier::NONE,
			Trend trend = Trend::NONE);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Runway runway;	///> Runway RVR is reported for.
		unsigned int visRange = 0; ///> RVR single or minimum value.
		Modifier visModifier = Modifier::UNKNOWN; ///> RVR single or minimum value range modifier.
		unsigned int varVisRange = 0; ///> RVR maximum value (if reported).
		Modifier varVisModifier = Modifier::UNKNOWN; ///> RVR maximum value range modifier (if reported).
		bool visRangeReported = false; 		///> Is RVR reported.
		bool varVisRangeReported = false;	///> Is RVR min/max range reported.
		DistanceUnit unit = DistanceUnit::UNKNOWN;	///> RVR distance units.
		Trend trend = Trend::UNKNOWN;	///> RVR trend (if reported).
	private:
		static Modifier modifierFromString(const std::string & str);
		static DistanceUnit unitFromString(const std::string & str);
		static Trend trendFromString(const std::string & str);
	};

	bool operator ==(const RunwayVisualRangeGroup & lhs, const RunwayVisualRangeGroup & rhs);

	/// @brief Group: runway state.
	/// @details Runway state, deposits, extent of contamination, deposit 
	/// depth and surface friction.
	struct RunwayStateGroup : public GroupBase {
		/// Which special status of runway state is reported.
		enum class Status {
			UNKNOWN = 30,
			NONE = 0,	///< Normal runway state is reported.
			CLRD = 1,	///< Runway contaminations have ceased to exist.
			SNOCLO = 2	///< Runway is closed due to extreme deposit of snow.
		};
		/// Deposits type, see Table 0919 in Manual on Codes (WMO No. 306).
		enum class Deposits {
			UNKNOWN = 30,
			NOT_REPORTED = 31,
			CLEAR_AND_DRY = 0,
			DAMP = 1,
			WET_AND_WATER_PATCHES = 2,
			RIME_AND_FROST_COVERED = 3,
			DRY_SNOW = 4,
			WET_SNOW = 5,
			SLUSH = 6,
			ICE = 7,
			COMPACTED_OR_ROLLED_SNOW = 8,
			FROZEN_RUTS_OR_RIDGES = 9
		};
		/// Extent of runway contamination, see Table 0519 in Manual on Codes 
		/// (WMO No. 306).
		enum class Extent {
			UNKNOWN = 30,
			NOT_REPORTED = 31,
			NONE = 0,
			LESS_THAN_10_PERCENT = 1,
			FROM_11_TO_25_PERCENT = 2,
			RESERVED_3 = 3,
			RESERVED_4 = 4,
			FROM_26_TO_50_PERCENT = 5,
			RESERVED_6 = 6,
			RESERVED_7 = 7,
			RESERVED_8 = 8,
			MORE_THAN_51_PERCENT = 9
		};
		/// Depth of deposits on the runway.
		struct DepositDepth {
			enum class Status {
				UNKNOWN = 30,
				REPORTED = 0,				///< Deposits depth reported.
				NOT_REPORTED = 31,			///< Deposits depth not reported.
				RUNWAY_NOT_OPERATIONAL = 1	///< Runway not operational.
			};
			/// Depth of deposits measurement units.
			enum class Unit {
				MM
			};
			DepositDepth() = default;
			/// Initialises DepositsDepth struct with two-digit string 
			/// representation.
			DepositDepth(const std::string & str);
			/// Initialises DepositsDepth struct with numeric representation.
			DepositDepth (unsigned int depth);
			/// Initialised DepositsDepth struct with non-reported value.
			static DepositDepth makeNotReported();
			/// Initialised DepositsDepth struct specifying that runway 
			/// is not operational.
			static DepositDepth makeRunwayNotOperational();
			Status status = Status::UNKNOWN;	///< Status of deposit depth reported.
			unsigned int depth = 0;				///< Deposit depth (if reported)
			static const Unit unit = Unit::MM;	///< Deposit depth measurement units.
		private:
			/// Reserved value for deposits depth, see Table 1079 in Manual on
			/// Codes (WMO No. 306).
			enum class Reserved {
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
		/// Surface friction of the runway
		struct SurfaceFriction {
			enum class Status {
				UNKNOWN = 30,
				SURFACE_FRICTION_REPORTED = 0,	///< Surface friction coefficient is reported.
				BRAKING_ACTION_REPORTED = 1,	///< Braking actions reported.
				NOT_REPORTED = 31,				///< Surface friction not reported.
				UNRELIABLE = 2					///< Unreliable
			};
			enum class BrakingAction {
				UNKNOWN = 30,
				POOR = 0,			///< Braking action poor.
				MEDIUM_POOR = 1,	///< Braking action medium-poor.
				MEDIUM = 2,			///< Braking action medium.
				MEDIUM_GOOD = 3,	///< Braking action medium-good.
				GOOD = 4,			///< Braking action good.
			};
			SurfaceFriction() = default;
			/// Initialises SurfaceFriction struct with a two-digit string 
			/// representation.
			SurfaceFriction(const std::string & str);
			/// Initialises SurfaceFriction struct using braking action value.
			SurfaceFriction(BrakingAction brakingAction);
			/// Initialises SurfaceFriction struct using friction coefficient
			/// value.
			SurfaceFriction(float coefficient);
			/// Initialises SurfaceFriction struct with non-reported value.
			static SurfaceFriction makeNotReported();
			/// Initialises SurfaceFriction struct specifying unreliable value.
			static SurfaceFriction makeUnreliable();
			Status status = Status::UNKNOWN;	///< Surface friction reporting status
			float coefficient = 0.0;			///< Surface friction coefficient
			BrakingAction brakingAction = BrakingAction::UNKNOWN; ///< Braking action
		private:
			/// Reserved value for braking action, see Table 0366 in Manual on
			/// Codes (WMO No. 306).
			enum class Reserved {
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
		};
		RunwayStateGroup() = default;
		/// Initialises group with values.
		/// @param runway Runway to report status.
		/// @param status Reporting status.
		/// @param deposits Runway deposits.
		/// @param dextent Runway contamination extent.
		/// @param depositDepth Depth of deposits on runway.
		/// @param sufraceFriction Runway sufrace friction value.
		RunwayStateGroup(const Runway & runway,
			Status status,
			Deposits deposits,
			Extent extent,
			DepositDepth depositDepth,
			SurfaceFriction surfaceFriction);
		/// Initialises group specifying that runway is closed due to snow 
		/// accumulation.
		/// @param runway Runway to report status.
		/// @return Initialised group.
		static RunwayStateGroup makeSnoclo(const Runway & runway);
		/// Initialises group specifying that runway deposits ceased to exist.
		/// @param runway Runway to report status.
		/// @param sufraceFriction Runway sufrace friction value.
		/// @return Initialised group.
		static RunwayStateGroup makeClrd(const Runway & runway,
			SurfaceFriction sf);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Runway runway;								///< Runway to report status.
		Status status = Status::UNKNOWN;			///< Reporting status.
		Deposits deposits = Deposits::UNKNOWN;		///< Deposits type.
		Extent contaminationExtent = Extent::UNKNOWN;///< Extent of runway contamination.
		struct DepositDepth depositDepth;	///< Depth of deposits on runway.
		struct SurfaceFriction surfaceFriction; ///< Runway sufrace friction value.
	private:
		static Deposits depositsFromString(const std::string & str);
		static Extent extentFromString(const std::string & str);
	};

	bool operator ==(const RunwayStateGroup & lhs, const RunwayStateGroup & rhs);
	bool operator ==(const RunwayStateGroup::DepositDepth & lhs, 
		const RunwayStateGroup::DepositDepth & rhs);
	bool operator ==(const RunwayStateGroup::SurfaceFriction & lhs, 
		const RunwayStateGroup::SurfaceFriction & rhs);

	/// @brief Group: rainfall.
	/// @details Actually measured rainfall for various periods (Australia only?).
	struct RainfallGroup : public GroupBase {
		/// Rainfall measurement units.
		enum class Unit {
			MM = 0
		};
		RainfallGroup() = default;
		/// Initialises group with two rainfall values (short rainfall group).
		/// @param last10Minutes Rainfall for last 10 minutes.
		/// @param since9AM Rainfall since 9AM.
		RainfallGroup(float last10Minutes, float since9AM);
		/// Initialises group with three rainfall values (long rainfall group).
		/// @param last10Minutes Rainfall for last 10 minutes.
		/// @param since9AM Rainfall since 9AM.
		/// @param last60Minutes Rainfall for last 60 minutes.
		RainfallGroup(float last10Minutes, float since9AM, float last60Minutes);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		float last10Minutes = 0.0; 			///< Rainfall for last 10 minutes.
		float since9AM = 0.0;				///< Rainfall since 9AM.
		float last60Minutes = 0.0;			///< Rainfall for last 60 minutes.
		bool last60MinutesReported = false;	///< Is rainfall for last 60 minutes included.
		static const Unit unit = Unit::MM;	///< Rainfall measurement units.
	};

	bool operator ==(const RainfallGroup & lhs, const RainfallGroup & rhs);

	/// @brief Group: state of sea surface.
	/// @details Temperature of sea surface and state of surface (oil platforms only?).
	struct SeaSurfaceGroup : public GroupBase {
		/// State of sea surface, see Table 3700 in Manual on Codes (WMO No. 
		/// 306).
		enum class StateOfSurface {
			UNKNOWN = 30,
			NOT_REPORTED = 31,
			CALM_GLASSY = 0,
			CALM_RIPPLED = 1,
			SMOOTH = 2,
			SLIGHT = 3,
			MODERATE = 4,
			ROUGH = 5,
			VERY_ROUGH = 6,
			HIGH = 7,
			VERY_HIGH = 8,
			PHENOMENAL = 9
		};
		SeaSurfaceGroup() = default;
		/// Initialise group with temperature and optional state of surface value
		/// @param temperature Temperature of sea surface (if reported).
		/// @param stateOfSurface State of sea surface.
		SeaSurfaceGroup(int temperature, StateOfSurface stateOfSurface = StateOfSurface::NOT_REPORTED);
		/// Initialise group with state of surface value only (temperature is 
		/// not reported).
		/// @param stateOfSurface State of sea surface (if reported).
		SeaSurfaceGroup(StateOfSurface stateOfSurface);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		int temperature = 0;					///< Sea surface temperature.
		bool temperatureReported = false;		///< Is sea surface temperature reported.
		/// Temperature measurement units
		static const TemperatureUnit temperatureUnit = TemperatureUnit::DEGREES_C;
		StateOfSurface stateOfSurface = StateOfSurface::UNKNOWN; ///< State of sea surface.
	private:
		StateOfSurface stateOfSurfaceFromString(const std::string & str);
	};

	bool operator ==(const SeaSurfaceGroup & lhs, const SeaSurfaceGroup & rhs);

	/// @brief Group: sea temperature and wave height.
	/// @details Temperature of sea surface and wave height (oil platforms only?).
	struct SeaWavesGroup : public GroupBase {
		/// Sea wave height measurement units.
		enum class WaveHeightUnit {
			DECIMETERS = 0
		};
		SeaWavesGroup() = default;
		/// Initialises group with both temperature and wave height.
		/// @param temperature Temperature of sea surface.
		/// @param waveHeight Sea waves height.
		SeaWavesGroup(int temperature, unsigned int waveHeight);
		/// Initialises group with temperature only (wave height not reported).
		/// @param temperature Temperature of sea surface.
		/// @return Initialised group.
		static SeaWavesGroup makeTemperature(int temperature);
		/// Initialises group with wave height only (temperature not reported).
		/// @param waveHeight Sea waves height.
		/// @return Initialised group.
		static SeaWavesGroup makeWaveHeight(unsigned int waveHeight);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		int temperature = 0;				///< Sea surface temperature.
		bool temperatureReported = false;	///< Is sea surface temperature reported.
		unsigned int waveHeight = 0;		///< Wave height.
		bool waveHeightReported = false;	///< Is wave height reported.
		/// Temperature measurement units.
		static const TemperatureUnit temperatureUnit = TemperatureUnit::DEGREES_C;
		/// Wave height measurement units.
		static const WaveHeightUnit waveHeightUnit = WaveHeightUnit::DECIMETERS;
	};

	bool operator ==(const SeaWavesGroup & lhs, const SeaWavesGroup & rhs);

	/// @brief Group: Airfield colour state.
	/// @details Encoded visibility and cloud height (UK navy & RAF only?). 
	/// Code BLACK may indicate that airfield is closed due to reasons not 
	/// related to visibility and ceiling (e.g. snow accumulation).
	struct ColourCodeGroup : public GroupBase {
		enum class Code {
			UNKNOWN = 30,
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
		/// Initialises group with colour code and optional code BLACK.
		/// @param code Colour code (BLUE, WHITE, GREEN, YELLOW1, YELLOW2, 
		/// AMBER, RED).
		/// @param codeBlack Optional colour code BLACK in addition to main
		/// colour code to indicate that airfield is closed due to reasons not 
		/// related to visibility and ceiling (e.g. snow accumulation).
		ColourCodeGroup(Code сode, bool codeBlack = false);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Code code = Code::UNKNOWN; 	///< Colour code.
		bool codeBlack = false;		///< Is colour code BLACK specified.
	};

	bool operator ==(const ColourCodeGroup & lhs, const ColourCodeGroup & rhs);

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
		static Group parse(const std::string & group, ReportPart reportPart);

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
			static Group parse(const std::string & group, ReportPart reportPart);
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
			static Group parse(const std::string & group, ReportPart reportPart);
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
		/// @return True if no error occurred during parsing, false if an error
		/// did occurr during parsing.
		bool parse(const std::string & report);
		/// Get parsing result.
		/// @return Vector of Group which contain complete report (if parsed
		/// successfully) or part of the report (to the point where an error 
		/// occurred).
		const std::vector<Group> & getResult() const { return(result); }
		/// @brief Reset parsing result and free memory.
		/// @details After calling this method, getResult() will return an 
		/// empty vector, getReportType() will return 
		/// metaf::ReportType::UNKNOWN, and getError() will return 
		/// metaf::Parser::Error::NONE.
		void resetResult();
		/// Get report type autodetected during parsing.
		/// @return Autodetected type of report (METAR or TAF or UNKNOWN if 
		/// it was impossible to determine report type).
		ReportType getReportType() const { return(reportType); }
		/// Get error occurred during parsing.
		/// @return Parsing error or metaf::Parser::Error::NONE if report was 
		/// parsed successfully.
		Error getError() const { return(error); }
	private:
		std::vector<Group> result;
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
		State transition(State state, SyntaxGroup group);
		/// Check if report end is allowed in current state.
		/// If report ended unexpectedly, state transitions to State::ERROR.
		State finalTransition(State state);
		/// Helper method to transition to Error state during parsing.
		State parseError(Error e) { error = e; return(State::ERROR); }
		/// Derive metaf:ReportPart
		static ReportPart reportPartFromState(State state);
	};

	/// @brief Visitor is intended for further processing of METAR or TAF 
	/// report information decoded by parser.
	/// @details To reduce amount of boilerplate, inherit from GroupVisitor
	/// and implement all individual virtual methods for each group type.
	/// @par 
	class GroupVisitor {
	public:
		void visit(const Group & group);
		void visit(const std::vector<Group> & groups);
	protected:
		virtual void visitPlainTextGroup(const PlainTextGroup & group) = 0;
		virtual void visitFixedGroup(const FixedGroup & group) = 0;
		virtual void visitLocationGroup(const LocationGroup & group) = 0;
		virtual void visitReportTimeGroup(const ReportTimeGroup & group) = 0;
		virtual void visitTimeSpanGroup(const TimeSpanGroup & group) = 0;
		virtual void visitTrendTimeGroup(const TrendTimeGroup & group) = 0;
		virtual void visitProbabilityGroup(const ProbabilityGroup & group) = 0;
		virtual void visitWindGroup(const WindGroup & group) = 0;
		virtual void visitVarWindGroup(const VarWindGroup & group) = 0;
		virtual void visitWindShearGroup(const WindShearGroup & group) = 0;
		virtual void visitVisibilityGroup(const VisibilityGroup & group) = 0;
		virtual void visitCloudGroup(const CloudGroup & group) = 0;
		virtual void visitVerticalVisibilityGroup(const VerticalVisibilityGroup & group) = 0;
		virtual void visitWeatherGroup(const WeatherGroup & group) = 0;
		virtual void visitTemperatureGroup(const TemperatureGroup & group) = 0;
		virtual void visitMinMaxTemperatureGroup(const MinMaxTemperatureGroup & group) = 0;
		virtual void visitPressureGroup(const PressureGroup & group) = 0;
		virtual void visitRunwayVisualRangeGroup(const RunwayVisualRangeGroup & group) = 0;
		virtual void visitRunwayStateGroup(const RunwayStateGroup & group) = 0;
		virtual void visitRainfallGroup(const RainfallGroup & group) = 0;
		virtual void visitSeaSurfaceGroup(const SeaSurfaceGroup & group) = 0;
		virtual void visitSeaWavesGroup(const SeaWavesGroup & group) = 0;
		virtual void visitColourCodeGroup(const ColourCodeGroup & group) = 0;
		virtual void visitOther(const Group & group) = 0;
	};

}; //namespace metaf

#endif //#ifndef METAR_H

