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

	struct PlainTextGroup;
	struct ImmutableGroup;
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

	using Group = std::variant<
		PlainTextGroup,
		ImmutableGroup,
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

	enum class ReportPart {
		UNKNOWN,
		HEADER,
		METAR,
		TAF,
		RMK
	};

	enum class SyntaxGroup {
		OTHER,
		METAR,
		SPECI,
		TAF,
		COR,
		AMD,
		LOCATION,
		REPORT_TIME,
		TIME_SPAN,
		CNL,
		NIL,
		RMK
	};

	SyntaxGroup getSyntaxGroup(const Group & group);

	class GroupVisitor {
	public:
		void visit(const Group & group);
		void visit(const std::vector<Group> & groups);
	protected:
		virtual void visitPlainTextGroup(const PlainTextGroup & group) = 0;
		virtual void visitImmutableGroup(const ImmutableGroup & group) = 0;
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

	struct GroupBase {
	protected:
		GroupBase() = default;
		bool parse(const std::string & group, ReportPart reportPart) {
			(void)group; (void)reportPart; return(false);
		}
		bool isValid() const { return(false); }
		bool nextGroup(const Group & nextGroup) { return(false); }
	};

	struct PlainTextGroup : public GroupBase {
	public:
		PlainTextGroup() = default;
		PlainTextGroup(std::string text);
		bool parse(const std::string & group, ReportPart reportPart);
		static const auto textMaxLength = 15;
		char text [textMaxLength + 1] = "\0";
	};

	bool operator ==(const PlainTextGroup & lhs, const PlainTextGroup & rhs);

	struct ImmutableGroup : public GroupBase {
		enum class Type {
			UNKNOWN,
			METAR,			///< Report type: METAR
			SPECI,			///< Report type: SPECI
			TAF,			///< Report type: TAF
			AMD,			///< TAF report amendment
			NIL,			///< Missing report
			CNL,			///< Missing TAF report
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
			RMK				///< Remarks
		};
		ImmutableGroup() = default;
		ImmutableGroup(Type type);
		bool parse(const std::string & group, ReportPart reportPart);
		Type type = Type::UNKNOWN;
	private:
		struct GroupData;
		static const std::vector<struct GroupData> group_data;
	};

	bool operator ==(const ImmutableGroup & lhs, const ImmutableGroup & rhs);

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
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int probability = 0;
	};

	bool operator ==(const ProbabilityGroup & lhs, const ProbabilityGroup & rhs);

	struct WindGroup : public GroupBase {
		enum class Unit {
			UNKNOWN,
			KNOTS,
			METERS_PER_SECOND,
			KILOMETERS_PER_HOUR
		};
		WindGroup() = default;
		WindGroup(Unit unit);
		WindGroup(unsigned int direction, 
			Unit unit, 
			unsigned int speed, 
			unsigned int gustSpeed = 0);
		static WindGroup makeVariableDirection (Unit unit, 
			unsigned int speed, 
			unsigned int gustSpeed = 0);
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int direction = 0;
		bool directionReported = false;
		bool directionVariable = false;
		unsigned int speed = 0;
		bool speedReported = false;
		unsigned int gustSpeed = 0;
		bool gustSpeedReported = false;
		Unit unit = Unit::UNKNOWN;
	};

	bool operator ==(const WindGroup & lhs, const WindGroup & rhs);

	struct VarWindGroup : public GroupBase {
		VarWindGroup() = default;
		VarWindGroup(unsigned int directionFrom, unsigned int directionTo);
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int directionFrom = 0;
		unsigned int directionTo = 0;
	};

	bool operator ==(const VarWindGroup & lhs, const VarWindGroup & rhs);

	struct WindShearGroup : public GroupBase {
		enum class SpeedUnit {
			UNKNOWN,
			KNOTS,
			METERS_PER_SECOND,
			KILOMETERS_PER_HOUR
		};
		enum class HeightUnit {
			FEET
		};
		WindShearGroup() = default; 
		WindShearGroup(unsigned int height,
			unsigned int direction, 
			unsigned int speed,	
			SpeedUnit speedUnit);
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int height = 0;
		unsigned int direction = 0;
		unsigned int speed = 0;
		SpeedUnit speedUnit = SpeedUnit::UNKNOWN;
		static const HeightUnit heightUnit = HeightUnit::FEET;
	};

	bool operator ==(const WindShearGroup & lhs, const WindShearGroup & rhs);

	struct VisibilityGroup : public GroupBase {
		enum class Unit {
			UNKNOWN,
			METERS,
			STATUTE_MILES
		};
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
		bool parse(const std::string & group, ReportPart reportPart);
		bool isValid() const;
		bool nextGroup(const Group & nextGroup);
		unsigned int integer = 0;
		unsigned int numerator = 0;
		unsigned int denominator = 0;
		Unit unit = Unit::UNKNOWN;
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
		enum class Unit {
			FEET
		};
		CloudGroup () = default;
		CloudGroup (Amount amount, unsigned int height, Type type = Type::NONE);
		CloudGroup (Amount amount, Type type = Type::NONE);
		bool parse(const std::string & group, ReportPart reportPart);
		Amount amount = Amount::UNKNOWN;
		unsigned int height = 0;
		Type type = Type::UNKNOWN;
		bool heightReported = false;
		static const Unit unit = Unit::FEET;
	};

	bool operator ==(const CloudGroup & lhs, const CloudGroup & rhs);

	struct VerticalVisibilityGroup : public GroupBase {
		enum class Unit {
			FEET
		};
		VerticalVisibilityGroup() = default;
		VerticalVisibilityGroup(unsigned int verticalVisibility);
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int vertVisibility = 0;
		bool reported = false;
		static const Unit unit = Unit::FEET;
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
		bool parse(const std::string & group, ReportPart reportPart);
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
		enum class Unit {
			DEGREES_C
		};
		TemperatureGroup() = default;
		TemperatureGroup(int temperature);
		TemperatureGroup(int temperature, int dewPoint);
		bool parse(const std::string & group, ReportPart reportPart);
		int temperature = 0;
		bool temperatureReported = false;
		int dewPoint = 0; 
		bool dewPointReported = false;
		static const Unit unit = Unit::DEGREES_C;
	};

	bool operator ==(const TemperatureGroup & lhs, const TemperatureGroup & rhs);

	struct MinMaxTemperatureGroup : public GroupBase {
		enum class Unit {
			DEGREES_C
		};
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
		bool parse(const std::string & group, ReportPart reportPart);
		Point point = Point::UNKNOWN;
		int temperature = 0;
		unsigned int day = 0;
		unsigned int hour = 0;
		static const Unit unit = Unit::DEGREES_C;
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
		bool parse(const std::string & group, ReportPart reportPart);
		float pressure = 0.0;
		bool reported = false;
		Unit unit = Unit::UNKNOWN;
	};

	bool operator ==(const PressureGroup & lhs, const PressureGroup & rhs);

	struct RunwayVisualRangeGroup : public GroupBase {
		enum class Designator {
			UNKNOWN,
			NONE,
			LEFT,
			CENTER,
			RIGHT
		};
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
		enum class Unit {
			UNKNOWN,
			FEET,
			METERS
		};
		RunwayVisualRangeGroup() = default;
		RunwayVisualRangeGroup(unsigned int runwayNo, 
			Designator designator,
			Unit unit,
			Trend trend = Trend::NONE);
		RunwayVisualRangeGroup(unsigned int runwayNo, 
			Designator designator,
			unsigned int visibility, 
			Unit unit,
			Modifier modifier = Modifier::NONE,
			Trend trend = Trend::NONE);
		RunwayVisualRangeGroup(unsigned int runwayNo,
			Designator designator,
			unsigned int minVisibility,
			unsigned int maxVisibility,
			Unit unit,
			Modifier minVisModifier = Modifier::NONE,
			Modifier maxVisModifier = Modifier::NONE,
			Trend trend = Trend::NONE);
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int runwayNumber = 0;
		Designator runwayDesignator = Designator::UNKNOWN;
		unsigned int visibility = 0;
		Modifier visModifier = Modifier::UNKNOWN;
		unsigned int varVisibility = 0;
		Modifier varVisModifier = Modifier::UNKNOWN;
		bool visibilityReported = false;
		bool varVisibilityReported = false;
		Unit unit = Unit::UNKNOWN;
		Trend trend = Trend::UNKNOWN;
	private:
		static Designator runwayDesignatorFromString(const std::string & str);
		static Modifier modifierFromString(const std::string & str);
		static Unit unitFromString(const std::string & str);
		static Trend trendFromString(const std::string & str);
	};

	bool operator ==(const RunwayVisualRangeGroup & lhs, const RunwayVisualRangeGroup & rhs);

	struct RunwayStateGroup : public GroupBase {
		enum class Designator {
			UNKNOWN,
			NONE,
			LEFT,
			CENTER,
			RIGHT
		};
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
		RunwayStateGroup(unsigned int runwayNo,
			Designator designator,
			Status status,
			Deposits deposits,
			Extent extent,
			DepositDepth depositDepth,
			SurfaceFriction surfaceFriction);
		static RunwayStateGroup makeSnoclo(unsigned int runwayNo,
			Designator designator);
		static RunwayStateGroup makeClrd(unsigned int runwayNo,
			Designator designator,
			SurfaceFriction sf);
		bool parse(const std::string & group, ReportPart reportPart);
		unsigned int runwayNumber = 0;
		Designator runwayDesignator = Designator::UNKNOWN;
		Status status = Status::UNKNOWN;
		Deposits deposits = Deposits::UNKNOWN;
		Extent contaminationExtent = Extent::UNKNOWN;
		struct DepositDepth depositDepth;
		struct SurfaceFriction surfaceFriction;
	private:
		static Designator runwayDesignatorFromString(const std::string & str);
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
		bool parse(const std::string & group, ReportPart reportPart);
		float last10Minutes = 0.0;
		float since9AM = 0.0;
		float last60Minutes = 0.0;
		bool last60MinutesReported = false;
		static const Unit unit = Unit::MM;
	};

	bool operator ==(const RainfallGroup & lhs, const RainfallGroup & rhs);

	struct SeaSurfaceGroup : public GroupBase {
		enum class TemperatureUnit {
			DEGREES_C
		};
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
		enum class TemperatureUnit {
			DEGREES_C
		};
		enum class WaveHeightUnit {
			DECIMETERS
		};
		SeaWavesGroup() = default;
		SeaWavesGroup(int temperature, unsigned int waveHeight);
		static SeaWavesGroup makeTemperature(int temperature);
		static SeaWavesGroup makeWaveHeight(unsigned int waveHeight);
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
		bool parse(const std::string & group, ReportPart reportPart);
		Code code = Code::UNKNOWN;
		bool codeBlack = false;
	};

	bool operator ==(const ColourCodeGroup & lhs, const ColourCodeGroup & rhs);

	class GroupParser {
	public:
		static const auto groups_total = std::variant_size_v<Group>;
		static Group parse(const std::string & group, ReportPart reportPart);

	private:
		template <size_t I, class T> 
		struct ParseGroupHelper {
			static Group parse(const std::string & group, ReportPart reportPart);
		};

		template <class T> 
		struct ParseGroupHelper<groups_total-1, T> {
			static Group parse(const std::string & group, ReportPart reportPart);
		};
	};

	enum class ReportType {
		UNKNOWN,
		METAR,
		TAF
	};

	class Parser {
	public:
		enum class Error {
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
			INTERNAL_PARSER_STATE
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

