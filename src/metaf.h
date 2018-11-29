/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

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

	/// A variant type for all possible METAR and TAF groups; used by 
	/// parser to return decoded report information.
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
	/// @retrn Corresponding SyntaxGroup or SyntaxGroup::OTHER if 
	/// group is not important for report syntax.
	SyntaxGroup getSyntaxGroup(const Group & group);

	/// @brief Visitor is intended for further processing of METAR or TAF 
	/// report information decoded by parser.
	/// @details To reduce amount of boilerplate, inherit from GroupVisitor
	/// and implement all individual virtual methods for each group type.
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
		UNKNOWN = 0,
		METERS = 1,
		STATUTE_MILES = 2,
		FEET = 3
	};

	/// Speed measurement units, used across groups
	enum class SpeedUnit {
		UNKNOWN = 0,
		KNOTS = 1,
		METERS_PER_SECOND = 2,
		KILOMETERS_PER_HOUR = 3
	};
	
	/// Temperature measurement units, used across groups
	enum class TemperatureUnit {
		UNKNOWN = 0,
		DEGREES_C = 1
	};

	/// Runway identification
	struct Runway {
		/// Runway designator
		enum class Designator {
			UNKNOWN,
			NONE,
			LEFT,
			CENTER,
			RIGHT
		};
		Runway() = default;
		/// Initialise Runway based on its number and designator
		/// @param number Runway number
		/// @param designator Runway designator, default is NONE
		Runway(unsigned int number, Designator designator = Designator::NONE);
		/// @bried Decode runway designator from char
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

	/// @brief METAR or TAF group: Plain Text
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
		bool parse(const std::string & group, ReportPart reportPart);
		/// Maximum number of chars which can be stored in this struct.
		static const auto textMaxLength = 15;
		/// Stored plain text.
		char text [textMaxLength + 1] = "\0";
	};

	bool operator ==(const PlainTextGroup & lhs, const PlainTextGroup & rhs);

	/// @brief METAR or TAF group: 
	struct FixedGroup : public GroupBase {
		enum class Type {
			UNKNOWN,
			METAR,			///< Report type: METAR
			SPECI,			///< Report type: SPECI
			TAF,			///< Report type: TAF
			AMD,			///< Amended TAF report
			NIL,			///< Missing report
			CNL,			///< Cancelled TAF report
			COR,			///< Correctional report
			AUTO,			///< Automatic report without human supervision
			CLR,			///< Clear sky
			NCD,			///< No clouds detected
			NOSIG,			///< No significant weather changes expected
			AIRPORT_SNOCLO,	///< Airport closed due to snow accumulation
			CAVOK,			///< Ceiling and visibility OK
			NSC,			///< No significant cloud
			NSW,			///< No significant weather
			TEMPO,			///< Temporary weather phenomena
			BECMG,			///< Weather conditions are changing
			INTER,			///< Intermittent weather changes (for <30 min), Australia only?
			RMK,			///< Remarks
			SKC,			///< Sky clear
		};
		FixedGroup() = default;
		FixedGroup(Type type);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Type type = Type::UNKNOWN;
	private:
		struct GroupData;
		static const std::vector<struct GroupData> group_data;
	};

	bool operator ==(const FixedGroup & lhs, const FixedGroup & rhs);

	struct LocationGroup : public GroupBase {
		LocationGroup() = default;
		LocationGroup(std::string location);
		bool parse(const std::string & group, ReportPart reportPart);
		static const auto locationLength = 4;
		char location [locationLength + 1] = "\0";
	};

	bool operator ==(const LocationGroup & lhs, const LocationGroup & rhs);

	struct ReportTimeGroup : public GroupBase {
		ReportTimeGroup() = default;
		ReportTimeGroup(unsigned int day, unsigned int hour, unsigned int minute);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int day = 0;
		unsigned int hour = 0;
		unsigned int minute = 0;
	};

	bool operator ==(const ReportTimeGroup & lhs, const ReportTimeGroup & rhs);

	struct TimeSpanGroup : public GroupBase {
		TimeSpanGroup() = default;
		TimeSpanGroup(unsigned int dayFrom,	
			unsigned int hourFrom, 
			unsigned int dayTill, 
			unsigned int hourTill);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int dayFrom = 0;
		unsigned int hourFrom = 0;
		unsigned int dayTill = 0;
		unsigned int hourTill = 0;
	};

	bool operator ==(const TimeSpanGroup & lhs, const TimeSpanGroup & rhs);

	struct TrendTimeGroup : public GroupBase {
		enum class Type {
			UNKNOWN,
			FROM,
			TILL,
			AT
		};
		TrendTimeGroup () = default;
		TrendTimeGroup (Type type,
			unsigned int day, 
			unsigned int hour, 
			unsigned int minute);
		TrendTimeGroup (Type type, unsigned int hour, unsigned int minute);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Type type = Type::UNKNOWN;
		unsigned int day = 0;
		unsigned int hour = 0;
		unsigned int minute = 0;
		bool dayReported = false;
	};

	bool operator ==(const TrendTimeGroup & lhs, const TrendTimeGroup & rhs);

	struct ProbabilityGroup : public GroupBase {
		ProbabilityGroup() = default;
		ProbabilityGroup(unsigned int probability);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int probability = 0;
	};

	bool operator ==(const ProbabilityGroup & lhs, const ProbabilityGroup & rhs);

	struct WindGroup : public GroupBase {
		WindGroup() = default;
		WindGroup(SpeedUnit unit);
		WindGroup(unsigned int direction, 
			SpeedUnit unit, 
			unsigned int speed, 
			unsigned int gustSpeed = 0);
		static WindGroup makeVariableDirection (SpeedUnit unit, 
			unsigned int speed, 
			unsigned int gustSpeed = 0);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int direction = 0;
		bool directionReported = false;
		bool directionVariable = false;
		unsigned int speed = 0;
		bool speedReported = false;
		unsigned int gustSpeed = 0;
		bool gustSpeedReported = false;
		SpeedUnit unit = SpeedUnit::UNKNOWN;
	};

	bool operator ==(const WindGroup & lhs, const WindGroup & rhs);

	struct VarWindGroup : public GroupBase {
		VarWindGroup() = default;
		VarWindGroup(unsigned int directionFrom, unsigned int directionTo);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int directionFrom = 0;
		unsigned int directionTo = 0;
	};

	bool operator ==(const VarWindGroup & lhs, const VarWindGroup & rhs);

	struct WindShearGroup : public GroupBase {
		WindShearGroup() = default; 
		WindShearGroup(unsigned int height,
			unsigned int direction, 
			unsigned int speed,	
			SpeedUnit speedUnit);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int height = 0;
		unsigned int direction = 0;
		unsigned int speed = 0;
		SpeedUnit speedUnit = SpeedUnit::UNKNOWN;
		static const DistanceUnit heightUnit = DistanceUnit::FEET;
	};

	bool operator ==(const WindShearGroup & lhs, const WindShearGroup & rhs);

	struct VisibilityGroup : public GroupBase {
		enum class Direction {
			UNKNOWN,
			NONE,
			NDV,
			N,
			S,
			W,
			E,
			NW,
			NE,
			SW,
			SE		
		};
		VisibilityGroup() = default;
		static VisibilityGroup makeVisibilityMeters(Direction direction = Direction::NONE);
		static VisibilityGroup makeVisibilityMeters(unsigned int visibility, 
			Direction direction = Direction::NONE);
		static VisibilityGroup makeVisibilityMiles();
		static VisibilityGroup makeVisibilityMiles(unsigned int integer, 
			unsigned int numerator = 0, 
			unsigned int denominator = 0,
			bool lessThan = false);
		static VisibilityGroup makeVisibilityMilesIncomplete(unsigned int integer);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		/// Check consistency and validity of struct fields. See GroupBase::isValid 
		/// for details.
		bool isValid() const;
		/// Attempt to combine this group with the following group. See 
		/// GroupBase::nextGroup for details.
		bool nextGroup(const Group & nextGroup);
		unsigned int integer = 0;
		unsigned int numerator = 0;
		unsigned int denominator = 0;
		DistanceUnit unit = DistanceUnit::UNKNOWN;
		Direction direction = Direction::UNKNOWN;
		bool reported = false;
		bool lessThan = false;
		bool incompleteInteger = false;
	private:
		bool isIncompleteInteger() const;
		bool isIncompleteFraction() const;

	};

	bool operator ==(const VisibilityGroup & lhs, const VisibilityGroup & rhs);

	struct CloudGroup : public GroupBase {
		enum class Amount {
			UNKNOWN,
			NOT_REPORTED,
			FEW,
			SCATTERED,
			BROKEN,
			OVERCAST
		};
		enum class Type {
			UNKNOWN,
			NONE,
			NOT_REPORTED,
			TOWERING_CUMULUS,
			CUMULONIMBUS
		};
		CloudGroup () = default;
		CloudGroup (Amount amount, unsigned int height, Type type = Type::NONE);
		CloudGroup (Amount amount, Type type = Type::NONE);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Amount amount = Amount::UNKNOWN;
		unsigned int height = 0;
		Type type = Type::UNKNOWN;
		bool heightReported = false;
		static const DistanceUnit heightUnit = DistanceUnit::FEET;
	};

	bool operator ==(const CloudGroup & lhs, const CloudGroup & rhs);

	struct VerticalVisibilityGroup : public GroupBase {
		VerticalVisibilityGroup() = default;
		VerticalVisibilityGroup(unsigned int verticalVisibility);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int vertVisibility = 0;
		bool reported = false;
		static const DistanceUnit unit = DistanceUnit::FEET;
	};

	bool operator ==(const VerticalVisibilityGroup & lhs, const VerticalVisibilityGroup & rhs);

	struct WeatherGroup : public GroupBase {
		enum class Modifier {
			UNKNOWN,
			NONE,
			RECENT,
			VICINITY,
			LIGHT,
			HEAVY
		};
		enum class Weather {
			UNKNOWN,
			NOT_REPORTED,
			SHALLOW,
			PARTIAL,
			PATCHES,
			LOW_DRIFTING,
			BLOWING,
			SHOWERS,
			THUNDERSTORM,
			FREEZING,
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
		WeatherGroup() = default;
		WeatherGroup(Modifier modifier, const std::vector<Weather> & weather);
		WeatherGroup(Modifier modifier, 
			Weather weather1, 
			Weather weather2 = Weather::UNKNOWN, 
			Weather weather3 = Weather::UNKNOWN);
		static WeatherGroup makeNotReported(bool recent = false);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		std::vector<Weather> weatherToVector() const;
		Modifier modifier = Modifier::UNKNOWN;
		static const auto maxWeatherSize = 8;
		Weather weather[maxWeatherSize] = {Weather::UNKNOWN};
		size_t weatherSize = 0;
	private:
		static Modifier modifierFromString(const std::string & str);
		static Weather weatherFromString(const std::string & str);
	};

	bool operator ==(const WeatherGroup & lhs, const WeatherGroup & rhs);

	struct TemperatureGroup : public GroupBase {
		TemperatureGroup() = default;
		TemperatureGroup(int temperature);
		TemperatureGroup(int temperature, int dewPoint);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		int temperature = 0;
		bool temperatureReported = false;
		int dewPoint = 0; 
		bool dewPointReported = false;
		static const TemperatureUnit unit = TemperatureUnit::DEGREES_C;
	};

	bool operator ==(const TemperatureGroup & lhs, const TemperatureGroup & rhs);

	struct MinMaxTemperatureGroup : public GroupBase {
		enum class Point {
			UNKNOWN,
			MINIMUM,
			MAXIMUM
		};
		MinMaxTemperatureGroup() = default;
		static MinMaxTemperatureGroup makeMin(int temperature,
			unsigned int day, 
			unsigned int hour);
		static MinMaxTemperatureGroup makeMax(int temperature, 
			unsigned int day, 
			unsigned int hour);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Point point = Point::UNKNOWN;
		int temperature = 0;
		unsigned int day = 0;
		unsigned int hour = 0;
		static const TemperatureUnit unit = TemperatureUnit::DEGREES_C;
	};

	bool operator ==(const MinMaxTemperatureGroup & lhs, const MinMaxTemperatureGroup & rhs);

	struct PressureGroup : public GroupBase {
		enum class Unit{
			UNKNOWN,
			HECTOPASCAL,
			INCHES_HG
		};
		PressureGroup() = default;
		PressureGroup(PressureGroup::Unit unit);
		PressureGroup(float pressure, PressureGroup::Unit unit);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		float pressure = 0.0;
		bool reported = false;
		Unit unit = Unit::UNKNOWN;
	};

	bool operator ==(const PressureGroup & lhs, const PressureGroup & rhs);

	struct RunwayVisualRangeGroup : public GroupBase {
		enum class Modifier {
			UNKNOWN,
			NONE,
			LESS_THAN,
			MORE_THAN
		};
		enum class Trend {
			UNKNOWN,
			NONE,
			UPWARD,
			NEUTRAL,
			DOWNWARD
		};
		RunwayVisualRangeGroup() = default;
		RunwayVisualRangeGroup(const Runway & runway,
			DistanceUnit unit,
			Trend trend = Trend::NONE);
		RunwayVisualRangeGroup(const Runway & runway,
			unsigned int visRange, 
			DistanceUnit unit,
			Modifier modifier = Modifier::NONE,
			Trend trend = Trend::NONE);
		RunwayVisualRangeGroup(const Runway & runway,
			unsigned int minVisRange,
			unsigned int maxVisRange,
			DistanceUnit unit,
			Modifier minVisModifier = Modifier::NONE,
			Modifier maxVisModifier = Modifier::NONE,
			Trend trend = Trend::NONE);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Runway runway;
		unsigned int visRange = 0;
		Modifier visModifier = Modifier::UNKNOWN;
		unsigned int varVisRange = 0;
		Modifier varVisModifier = Modifier::UNKNOWN;
		bool visRangeReported = false;
		bool varVisRangeReported = false;
		DistanceUnit unit = DistanceUnit::UNKNOWN;
		Trend trend = Trend::UNKNOWN;
	private:
		static Modifier modifierFromString(const std::string & str);
		static DistanceUnit unitFromString(const std::string & str);
		static Trend trendFromString(const std::string & str);
	};

	bool operator ==(const RunwayVisualRangeGroup & lhs, const RunwayVisualRangeGroup & rhs);

	struct RunwayStateGroup : public GroupBase {
		enum class Status {
			UNKNOWN,
			NONE,
			CLRD,
			SNOCLO
		};
		enum class Deposits {// Table 0919
			UNKNOWN = -2,
			NOT_REPORTED = -1,
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
		enum class Extent { //Table 0519
			UNKNOWN = -2,
			NOT_REPORTED = -1,
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
		struct DepositDepth {
			enum class Status {
				UNKNOWN,
				REPORTED,
				NOT_REPORTED,
				RUNWAY_NOT_OPERATIONAL
			};
			enum class Unit {
				MM
			};
			DepositDepth() = default;
			DepositDepth(const std::string & str);
			DepositDepth (unsigned int depth);
			static DepositDepth makeNotReported();
			static DepositDepth makeRunwayNotOperational();
			Status status = Status::UNKNOWN;
			unsigned int depth = 0;
			static const Unit unit = Unit::MM;
		private:
			enum class Reserved { //Table 1079
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
		struct SurfaceFriction {
			enum class Status {
				UNKNOWN,
				SURFACE_FRICTION_REPORTED,
				BRAKING_ACTION_REPORTED,
				NOT_REPORTED,
				UNRELIABLE
			};
			enum class BrakingAction {
				UNKNOWN,
				POOR,
				MEDIUM_POOR,
				MEDIUM,
				MEDIUM_GOOD,
				GOOD,
			};
			SurfaceFriction() = default;
			SurfaceFriction(const std::string & str);
			SurfaceFriction(BrakingAction brakingAction);
			SurfaceFriction(float coefficient);
			static SurfaceFriction makeNotReported();
			static SurfaceFriction makeUnreliable();
			Status status = Status::UNKNOWN;
			float coefficient = 0.0;
			BrakingAction brakingAction = BrakingAction::UNKNOWN;
		private:
			enum class Reserved { //Table 0366
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
		RunwayStateGroup(const Runway & runway,
			Status status,
			Deposits deposits,
			Extent extent,
			DepositDepth depositDepth,
			SurfaceFriction surfaceFriction);
		static RunwayStateGroup makeSnoclo(const Runway & runway);
		static RunwayStateGroup makeClrd(const Runway & runway,
			SurfaceFriction sf);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Runway runway;
		Status status = Status::UNKNOWN;
		Deposits deposits = Deposits::UNKNOWN;
		Extent contaminationExtent = Extent::UNKNOWN;
		struct DepositDepth depositDepth;
		struct SurfaceFriction surfaceFriction;
	private:
		static Deposits depositsFromString(const std::string & str);
		static Extent extentFromString(const std::string & str);
	};

	bool operator ==(const RunwayStateGroup & lhs, const RunwayStateGroup & rhs);
	bool operator ==(const RunwayStateGroup::DepositDepth & lhs, 
		const RunwayStateGroup::DepositDepth & rhs);
	bool operator ==(const RunwayStateGroup::SurfaceFriction & lhs, 
		const RunwayStateGroup::SurfaceFriction & rhs);

	struct RainfallGroup : public GroupBase {
		enum class Unit {
			MM
		};
		RainfallGroup() = default;
		RainfallGroup(float last10Minutes, float since9AM);
		RainfallGroup(float last10Minutes, float since9AM, float last60Minutes);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		float last10Minutes = 0.0;
		float since9AM = 0.0;
		float last60Minutes = 0.0;
		bool last60MinutesReported = false;
		static const Unit unit = Unit::MM;
	};

	bool operator ==(const RainfallGroup & lhs, const RainfallGroup & rhs);

	struct SeaSurfaceGroup : public GroupBase {
		enum class StateOfSurface { //table 3700
			UNKNOWN = -2,
			NOT_REPORTED = -1,
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
		SeaSurfaceGroup(int temperature, StateOfSurface stateOfSurface = StateOfSurface::NOT_REPORTED);
		SeaSurfaceGroup(StateOfSurface stateOfSurface);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		int temperature = 0;
		bool temperatureReported = false;
		static const TemperatureUnit temperatureUnit = TemperatureUnit::DEGREES_C;
		StateOfSurface stateOfSurface = StateOfSurface::UNKNOWN;
	private:
		StateOfSurface stateOfSurfaceFromString(const std::string & str);
	};

	bool operator ==(const SeaSurfaceGroup & lhs, const SeaSurfaceGroup & rhs);

	struct SeaWavesGroup : public GroupBase {
		enum class WaveHeightUnit {
			DECIMETERS
		};
		SeaWavesGroup() = default;
		SeaWavesGroup(int temperature, unsigned int waveHeight);
		static SeaWavesGroup makeTemperature(int temperature);
		static SeaWavesGroup makeWaveHeight(unsigned int waveHeight);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		int temperature = 0;
		bool temperatureReported = false;
		unsigned int waveHeight = 0;
		bool waveHeightReported = false;
		static const TemperatureUnit temperatureUnit = TemperatureUnit::DEGREES_C;
		static const WaveHeightUnit waveHeightUnit = WaveHeightUnit::DECIMETERS;
	};

	bool operator ==(const SeaWavesGroup & lhs, const SeaWavesGroup & rhs);

	struct ColourCodeGroup : public GroupBase {
		enum class Code {
			UNKNOWN,
			BLUE,
			WHITE,
			GREEN,
			YELLOW1,
			YELLOW2,
			AMBER,
			RED
		};
		ColourCodeGroup() = default;
		ColourCodeGroup(Code сode, bool codeBlack = false);
		/// Attempt to parse group. See GroupBase::parse for details.
		bool parse(const std::string & group, ReportPart reportPart);
		Code code = Code::UNKNOWN;
		bool codeBlack = false;
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
		/// Total number of groups in Group variant
		static const auto groups_total = std::variant_size_v<Group>;
		/// Call parse() method from all types within Group variant
		/// @param group METAR or TAF group to be parsed
		/// @param reportPart Current syntaxic part of the report
		static Group parse(const std::string & group, ReportPart reportPart);

	private:
		/// 
		template <size_t I, class T> 
		struct ParseGroupHelper {
			static Group parse(const std::string & group, ReportPart reportPart);
		};

		template <class T> 
		struct ParseGroupHelper<groups_total-1, T> {
			static Group parse(const std::string & group, ReportPart reportPart);
		};
	};

	/// Report type detected by parser
	enum class ReportType {
		UNKNOWN,		///< Report type not detected
		METAR,			///< Report type detected as METAR
		TAF				///< Report type detected as TAF
	};

	///Parses METAR and 
	class Parser {
	public:
		/// Errors which might occur when parsing METAR or TAF report
		enum class Error {
			NONE,								///< No error
			EMPTY_REPORT,						///< Empty report supplied
			EXPECTED_REPORT_TYPE_OR_LOCATION,	///< Report must start with type or location
			EXPECTED_LOCATION,					///< Expected location here
			EXPECTED_REPORT_TIME,				///< Expected report time here
			EXPECTED_TIME_SPAN,					///< Expected time span here
			UNEXPECTED_REPORT_END,				///< Report ends unexpectedly
			UNEXPECTED_GROUP_AFTER_NIL,			///< NIL report must be empty
			UNEXPECTED_GROUP_AFTER_CNL,			///< CNL report must be empty
			UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY,///< NIL or CNL not allowed in report body
			AMD_ALLOWED_IN_TAF_ONLY,			///< AMD only used in TAF reports
			CNL_ALLOWED_IN_TAF_ONLY,			///< CNL only used in TAF reports
			INTERNAL_PARSER_STATE				///< Internal error: invalid parser state
		};
		/// Parse the report
		bool parse(const std::string & report);
		/// Get parsing result
		const std::vector<Group> & getResult() { return(result); }
		/// Reset parsing result
		void resetResult();
		/// Get detected report type
		ReportType getReportType() { return(reportType); }
		/// Get parsing error
		Error getError() { return(error); }
	private:
		std::vector<Group> result;

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

		ReportType reportType = ReportType::UNKNOWN;
		Error error = Error::NONE;

		State transition(State state, SyntaxGroup group);
		State finalTransition(State state);
		State parseError(Error e) { error = e; return(State::ERROR); }
		static ReportPart reportPartFromState(State state);
	};

}; //namespace metaf

#endif //#ifndef METAR_H

