/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "metaf.h"

#include <string>
#include <regex>
#include <variant>
#include <sstream>
#include <cmath>

using namespace std;
using namespace metaf;

///////////////////////////////////////////////////////////////////////////////

SyntaxGroup metaf::getSyntaxGroup(const Group & group) {
	if (auto immutableGroup = std::get_if<ImmutableGroup>(&group)) {
		switch (immutableGroup->type) {
			case ImmutableGroup::Type::METAR:
			return(SyntaxGroup::METAR);
			case ImmutableGroup::Type::SPECI:
			return(SyntaxGroup::SPECI);
			case ImmutableGroup::Type::TAF:
			return(SyntaxGroup::TAF);
			case ImmutableGroup::Type::COR:
			return(SyntaxGroup::COR);
			case ImmutableGroup::Type::AMD:
			return(SyntaxGroup::AMD);
			case ImmutableGroup::Type::NIL:
			return(SyntaxGroup::NIL);
			case ImmutableGroup::Type::CNL:
			return(SyntaxGroup::CNL);
			case ImmutableGroup::Type::RMK:
			return(SyntaxGroup::RMK);
			default:
			return(SyntaxGroup::OTHER);
		}
	}
	if (get_if<LocationGroup>(&group)) return(SyntaxGroup::LOCATION);
	if (get_if<ReportTimeGroup>(&group)) return(SyntaxGroup::REPORT_TIME);
	if (get_if<TimeSpanGroup>(&group)) return(SyntaxGroup::TIME_SPAN);
	return(SyntaxGroup::OTHER);
}

///////////////////////////////////////////////////////////////////////////////

void GroupVisitor::visit(const Group & group) {
	if (holds_alternative<PlainTextGroup>(group)) {
		this->visitPlainTextGroup(get<PlainTextGroup>(group));
		return;
	}
	if (holds_alternative<ImmutableGroup>(group)) {
		this->visitImmutableGroup(get<ImmutableGroup>(group));
		return;
	}
	if (holds_alternative<LocationGroup>(group)) {
		this->visitLocationGroup(get<LocationGroup>(group));
		return;
	}
	if (holds_alternative<ReportTimeGroup>(group)) {
		this->visitReportTimeGroup(get<ReportTimeGroup>(group));
		return;
	}
	if (holds_alternative<TimeSpanGroup>(group)) {
		this->visitTimeSpanGroup(get<TimeSpanGroup>(group));
		return;
	}
	if (holds_alternative<TrendTimeGroup>(group)) {
		this->visitTrendTimeGroup(get<TrendTimeGroup>(group));
		return;
	}
	if (holds_alternative<ProbabilityGroup>(group)) {
		this->visitProbabilityGroup(get<ProbabilityGroup>(group));
		return;
	}
	if (holds_alternative<WindGroup>(group)) {
		this->visitWindGroup(get<WindGroup>(group));
		return;
	}
	if (holds_alternative<VarWindGroup>(group)) {
		this->visitVarWindGroup(get<VarWindGroup>(group));
		return;
	}
	if (holds_alternative<WindShearGroup>(group)) {
		this->visitWindShearGroup(get<WindShearGroup>(group));
		return;
	}
	if (holds_alternative<VisibilityGroup>(group)) {
		this->visitVisibilityGroup(get<VisibilityGroup>(group));
		return;
	}
	if (holds_alternative<CloudGroup>(group)) {
		this->visitCloudGroup(get<CloudGroup>(group));
		return;
	}
	if (holds_alternative<VerticalVisibilityGroup>(group)) {
		this->visitVerticalVisibilityGroup(get<VerticalVisibilityGroup>(group));
		return;
	}
	if (holds_alternative<WeatherGroup>(group)) {
		this->visitWeatherGroup(get<WeatherGroup>(group));
		return;
	}
	if (holds_alternative<TemperatureGroup>(group)) {
		this->visitTemperatureGroup(get<TemperatureGroup>(group));
		return;
	}
	if (holds_alternative<MinMaxTemperatureGroup>(group)) {
		this->visitMinMaxTemperatureGroup(get<MinMaxTemperatureGroup>(group));
		return;
	}
	if (holds_alternative<PressureGroup>(group)) {
		this->visitPressureGroup(get<PressureGroup>(group));
		return;
	}
	if (holds_alternative<RunwayVisualRangeGroup>(group)) {
		this->visitRunwayVisualRangeGroup(get<RunwayVisualRangeGroup>(group));
		return;
	}
	if (holds_alternative<RunwayStateGroup>(group)) {
		this->visitRunwayStateGroup(get<RunwayStateGroup>(group));
		return;
	}
	if (holds_alternative<RainfallGroup>(group)) {
		this->visitRainfallGroup(get<RainfallGroup>(group));
		return;
	}
	if (holds_alternative<SeaSurfaceGroup>(group)) {
		this->visitSeaSurfaceGroup(get<SeaSurfaceGroup>(group));
		return;
	}
	if (holds_alternative<SeaWavesGroup>(group)) {
		this->visitSeaWavesGroup(get<SeaWavesGroup>(group));
		return;
	}
	if (holds_alternative<ColourCodeGroup>(group)) {
		this->visitColourCodeGroup(get<ColourCodeGroup>(group));
		return;
	}
	this->visitOther(group);
}

void GroupVisitor::visit(const vector<Group> & groups) {
	for (const auto & group : groups) {
		visit(group);
	}
}

///////////////////////////////////////////////////////////////////////////////

PlainTextGroup::PlainTextGroup(std::string text) {
	strncpy(this->text, text.c_str(), textMaxLength);
	this->text[PlainTextGroup::textMaxLength] = '\0';
}

bool PlainTextGroup::parse(const string & group, ReportPart reportPart) {
	(void)reportPart;
	strncpy(text, group.c_str(), textMaxLength);
	text[textMaxLength] = '\0';
	return(true);
}

bool metaf::operator ==(const PlainTextGroup & lhs, const PlainTextGroup & rhs) {
	return(!strncmp(lhs.text, rhs.text, PlainTextGroup::textMaxLength));
}

///////////////////////////////////////////////////////////////////////////////

struct ImmutableGroup::GroupData{
	Type type;
	ReportPart reportPart;
	std::string str;
};

const std::vector<struct ImmutableGroup::GroupData> ImmutableGroup::group_data = {
	{Type::METAR, ReportPart::HEADER, std::string("METAR")},

	{Type::SPECI, ReportPart::HEADER, std::string("SPECI")},

	{Type::TAF, ReportPart::HEADER, std::string("TAF")},

	{Type::COR, ReportPart::HEADER, std::string("COR")},
	{Type::COR, ReportPart::METAR, std::string("COR")},

	{Type::AMD, ReportPart::HEADER, std::string("AMD")},

	{Type::NIL, ReportPart::HEADER, std::string("NIL")},
	{Type::NIL, ReportPart::METAR, std::string("NIL")},
	{Type::NIL, ReportPart::TAF, std::string("NIL")},

	{Type::CNL, ReportPart::HEADER, std::string("CNL")},
	{Type::CNL, ReportPart::METAR, std::string("CNL")},
	{Type::CNL, ReportPart::TAF, std::string("CNL")},

	{Type::AUTO, ReportPart::METAR, std::string("AUTO")},

	{Type::CLR, ReportPart::METAR, std::string("CLR")},

	{Type::NCD, ReportPart::METAR, std::string("NCD")},

	{Type::NOSIG, ReportPart::METAR, std::string("NOSIG")},

	{Type::AIRPORT_SNOCLO, ReportPart::METAR, std::string("SNOCLO")},

	{Type::AIRPORT_SNOCLO, ReportPart::METAR, std::string("R/SNOCLO")},

	{Type::CAVOK, ReportPart::METAR, std::string("CAVOK")},
	{Type::CAVOK, ReportPart::TAF, std::string("CAVOK")},

	{Type::NSC, ReportPart::METAR, std::string("NSC")},
	{Type::NSC, ReportPart::TAF, std::string("NSC")},

	{Type::NSW, ReportPart::METAR, std::string("NSW")},
	{Type::NSW, ReportPart::TAF, std::string("NSW")},

	{Type::TEMPO, ReportPart::METAR, std::string("TEMPO")},
	{Type::TEMPO, ReportPart::TAF, std::string("TEMPO")},

	{Type::BECMG, ReportPart::METAR, std::string("BECMG")},
	{Type::BECMG, ReportPart::TAF, std::string("BECMG")},

	{Type::INTER, ReportPart::TAF, std::string("INTER")},

	{Type::RMK, ReportPart::METAR, std::string("RMK")},
	{Type::RMK, ReportPart::TAF, std::string("RMK")}
};


ImmutableGroup::ImmutableGroup(ImmutableGroup::Type t) : type(t) {}

bool ImmutableGroup::parse(const string & group, ReportPart reportPart){
	for (const auto & data : group_data) {
		if (data.reportPart != reportPart) continue;
		if (data.str == group) {
			type = data.type;
			return(true);
		}
	}
	return(false);
}

bool metaf::operator ==(const ImmutableGroup & lhs, const ImmutableGroup & rhs){
	return(lhs.type == rhs.type);
}

///////////////////////////////////////////////////////////////////////////////

LocationGroup::LocationGroup(std::string location){
	strncpy(this->location, location.c_str(), locationLength);
	this->location[locationLength] = '\0';
}

bool LocationGroup::parse(const string & group, ReportPart reportPart){
	static const std::regex location_regex = std::regex ("[A-Z][A-Z0-9]{3}");
	if (reportPart != ReportPart::HEADER) return(false);
	if (!regex_match(group, location_regex)) return(false);
	strncpy(location, group.c_str(), locationLength);
	location[locationLength] = '\0';
	return(true);
}

bool metaf::operator ==(const LocationGroup & lhs, const LocationGroup & rhs){
	return(!strncmp(lhs.location, rhs.location, LocationGroup::locationLength));
}

///////////////////////////////////////////////////////////////////////////////

ReportTimeGroup::ReportTimeGroup(unsigned int d, unsigned int h, unsigned int m) : 
	day(d), hour(h), minute(m) {}

bool ReportTimeGroup::parse(const string & group, ReportPart reportPart) {
	static const regex reportTimeRegex ("(\\d\\d)(\\d\\d)(\\d\\d)Z");
	static const auto expectedMatchGroups = 4;
	static const auto matchDay = 1;
	static const auto matchHour = 2;
	static const auto matchMinute = 3;
	smatch match;
	if (reportPart != ReportPart::HEADER) return(false);
	if (!regex_match(group, match, reportTimeRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	day = static_cast<unsigned int>(stoi(match.str(matchDay)));
	hour = static_cast<unsigned int>(stoi(match.str(matchHour)));
	minute = static_cast<unsigned int>(stoi(match.str(matchMinute)));
	return(true);
}

bool metaf::operator ==(const ReportTimeGroup & lhs, const ReportTimeGroup & rhs){
	return(lhs.day == rhs.day && 
		lhs.hour == rhs.hour && 
		lhs.minute == rhs.minute);
}

///////////////////////////////////////////////////////////////////////////////

TimeSpanGroup::TimeSpanGroup(unsigned int df,	
	unsigned int hf, 
	unsigned int dt, 
	unsigned int ht) : 
		dayFrom(df), hourFrom(hf), dayTill(dt), hourTill(ht) {}


bool TimeSpanGroup::parse(const string & group, ReportPart reportPart){
	static const regex timeSpanRegex("(\\d\\d)(\\d\\d)/(\\d\\d)(\\d\\d)");
	static const auto expectedMatchGroups = 5;
	static const auto matchDayFrom = 1;
	static const auto matchHourFrom = 2;
	static const auto matchDayTill = 3;
	static const auto matchHourTill = 4;
	smatch match;
	if (reportPart != ReportPart::HEADER &&	reportPart != ReportPart::TAF) return(false);
	if (!regex_match(group, match, timeSpanRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	dayFrom = static_cast<unsigned int>(stoi(match.str(matchDayFrom)));
	hourFrom = static_cast<unsigned int>(stoi(match.str(matchHourFrom)));
	dayTill = static_cast<unsigned int>(stoi(match.str(matchDayTill)));
	hourTill = static_cast<unsigned int>(stoi(match.str(matchHourTill)));
	return(true);
}

bool metaf::operator ==(const TimeSpanGroup & lhs, const TimeSpanGroup & rhs){
	return(lhs.dayFrom == rhs.dayFrom &&
		lhs.hourFrom == rhs.hourFrom &&
		lhs.dayTill == rhs.dayTill &&
		lhs.hourTill == rhs.hourTill);
}

///////////////////////////////////////////////////////////////////////////////

TrendTimeGroup::TrendTimeGroup (Type t, unsigned int d, unsigned int h, unsigned int m) :
	type(t), day(d), hour(h), minute(m), dayReported(true) {}

TrendTimeGroup::TrendTimeGroup (Type t, unsigned int h, unsigned int m) : 
	type(t), hour(h), minute(m), dayReported(false) {}


bool TrendTimeGroup::parse(const string & group, ReportPart reportPart) {
	static const regex trendTimeRegex ("(?:(FM)|(TL)|(AT))(\\d\\d)?(\\d\\d)(\\d\\d)");
	static const auto expectedMatchGroups = 7;
	static const auto matchFm = 1;
	static const auto matchTl = 2;
	static const auto matchAt = 3;
	static const auto matchDay = 4;
	static const auto matchHour = 5;
	static const auto matchMinute = 6;
	smatch match;
	if (reportPart != ReportPart::METAR &&
		reportPart != ReportPart::TAF) return(false);
		if (!regex_match(group, match, trendTimeRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	type = Type::UNKNOWN;
	if (match.length(matchFm)) type = Type::FROM;
	if (match.length(matchTl)) type = Type::TILL;
	if (match.length(matchAt)) type = Type::AT;
	dayReported = false; day = 0;
	if (match.length(matchDay)) {
		day = static_cast<unsigned int>(stoi(match.str(matchDay)));
		dayReported = true;
	}
	hour = static_cast<unsigned int>(stoi(match.str(matchHour)));
	minute = static_cast<unsigned int>(stoi(match.str(matchMinute)));
	return(true);
}

bool metaf::operator ==(const TrendTimeGroup & lhs, const TrendTimeGroup & rhs){
	if (lhs.type != rhs.type ||
		lhs.dayReported != rhs.dayReported ||
		lhs.hour != rhs.hour ||
		lhs.minute != rhs.minute) return(false);
		if (lhs.dayReported && (lhs.day != rhs.day)) return(false);
	return(true);		
}

///////////////////////////////////////////////////////////////////////////////

ProbabilityGroup::ProbabilityGroup(unsigned int p) : probability(p) {}

bool ProbabilityGroup::parse(const string & group, ReportPart reportPart) {
	static const regex probRegex("PROB([34]0))");
	static const auto expectedMatchGroups = 2;
	static const auto matchProb = 1;
	smatch match;
	if (reportPart != ReportPart::TAF) return(false);
	if (!regex_match(group, match, probRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	probability = static_cast<unsigned int>(stoi(match.str(matchProb)));
	return(true);
}

bool metaf::operator ==(const ProbabilityGroup & lhs, const ProbabilityGroup & rhs){
	return (lhs.probability == rhs.probability);
}

///////////////////////////////////////////////////////////////////////////////

WindGroup::WindGroup(Unit u) : 
	directionReported(false), speedReported(false), gustSpeedReported(false), unit(u) {}

WindGroup::WindGroup(unsigned int dir, 
	Unit u, 
	unsigned int s, 
	unsigned int gs) :
		direction(dir),
		directionReported(true),
		directionVariable(false),
		speed(s),
		speedReported(true), 
		gustSpeed(gs),
		gustSpeedReported(gs), 
		unit(u) {}

WindGroup WindGroup::makeVariableDirection (Unit unit, 
		unsigned int speed, 
		unsigned int gustSpeed)
{
		WindGroup group;
		group.directionReported = true;
		group.directionVariable = true;
		group.speed = speed;
		group.speedReported = true;
		group.gustSpeed = gustSpeed;
		group.gustSpeedReported = gustSpeed;
		group.unit = unit;
		return(group);
}

bool WindGroup::parse(const string & group, ReportPart reportPart) {
	static const regex windRegex(
		"(?:(\\d\\d0)|(VRB)|(?:///))"
		"(?:(\\d\\d\\d?)|(?://))"
		"(?:G(\\d\\d\\d?))?"
		"(?:(KT)|(KMH)|(MPS))");
	static const auto expectedMatchGroups = 8;
	static const auto matchDirNumeric = 1;
	static const auto matchDirVrb = 2;
	static const auto matchSpeedNumeric = 3;
	static const auto matchGustSpeed = 4;
	static const auto matchKt = 5;
	static const auto matchKmh = 6;
	static const auto matchMps = 7;
	smatch match;
	if (reportPart != ReportPart::METAR &&
		reportPart != ReportPart::TAF) return(false);
	if (!regex_match(group, match, windRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	directionReported = false;
	direction = 0;
	if (match.length(matchDirNumeric)) {
		direction = static_cast<unsigned int>(stoi(match.str(matchDirNumeric)));
		directionReported = true;
	}
	if (match.length(matchDirVrb)) {
		directionVariable = true;
		directionReported = true;
	}
	speed = 0;
	speedReported = false;
	if (match.length(matchSpeedNumeric)) {
		speed = static_cast<unsigned int>(stoi(match.str(matchSpeedNumeric)));
		speedReported = true;
	}
	gustSpeed = 0;
	gustSpeedReported = false;
	if (match.length(matchGustSpeed)) {
		gustSpeed = static_cast<unsigned int>(stoi(match.str(matchGustSpeed)));
		gustSpeedReported = true;
	}
	unit = Unit::UNKNOWN;
	if (match.length(matchKt)) unit = Unit::KNOTS;
	if (match.length(matchKmh)) unit = Unit::KILOMETERS_PER_HOUR;
	if (match.length(matchMps)) unit = Unit::METERS_PER_SECOND;
	return(true);
}

bool metaf::operator ==(const WindGroup & lhs, const WindGroup & rhs){
	if (lhs.directionReported != rhs.directionReported ||
		lhs.speedReported != rhs.speedReported ||
		lhs.gustSpeedReported != rhs.gustSpeedReported ||
		lhs.unit != rhs.unit) return(false);
	if (lhs.directionReported) {
		if (lhs.directionVariable != rhs.directionVariable) return(false);
		if (!lhs.directionVariable && (lhs.direction != rhs.direction)) return(false);
	}
	if (lhs.speedReported && (lhs.speed != rhs.speed)) return(false);
	if (lhs.gustSpeedReported && (lhs.gustSpeed != rhs.gustSpeed)) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

VarWindGroup::VarWindGroup(unsigned int dirFrom, unsigned int dirTo) :
	directionFrom(dirFrom), directionTo(dirTo) {}

bool VarWindGroup::parse(const string & group, ReportPart reportPart){
	static const regex varWindRegex("(\\d\\d0)V(\\d\\d0)");
	static const auto expectedMatchGroups = 3;
	static const auto matchDirFrom = 1;
	static const auto matchDirTo = 2;
	smatch match;
	if (reportPart != ReportPart::METAR &&
		reportPart != ReportPart::TAF) return(false);
	if (!regex_match(group, match, varWindRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	directionFrom = static_cast<unsigned int>(stoi(match.str(matchDirFrom)));
	directionTo = static_cast<unsigned int>(stoi(match.str(matchDirTo)));
	return(true);
}

bool metaf::operator ==(const VarWindGroup & lhs, const VarWindGroup & rhs){
	return(lhs.directionFrom == rhs.directionFrom &&
		lhs.directionTo == rhs.directionTo);
}

///////////////////////////////////////////////////////////////////////////////

WindShearGroup::WindShearGroup(unsigned int h, unsigned int d, unsigned int s, SpeedUnit su) :
	height(h), direction(d), speed(s), speedUnit(su) {}

bool WindShearGroup::parse(const string & group, ReportPart reportPart) {
	static const regex windShearRegex(
		"WS(\\d\\d\\d)/"
		"(\\d\\d0)(\\d\\d\\d?)"
		"(?:(KT)|(KMH)|(MPS))");
	static const auto expectedMatchGroups = 7;
	static const auto matchHeight = 1;
	static const auto matchDir = 2;
	static const auto matchSpeed = 3;
	static const auto matchKt = 4;
	static const auto matchKmh = 5;
	static const auto matchMps = 6;
	static const auto heightFactor = 100;
	smatch match;
	if (reportPart != ReportPart::METAR &&
		reportPart != ReportPart::TAF) return(false);
		if (!regex_match(group, match, windShearRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	height = static_cast<unsigned int>(stoi(match.str(matchHeight))) * heightFactor;
	direction = static_cast<unsigned int>(stoi(match.str(matchDir)));
	speed = static_cast<unsigned int>(stoi(match.str(matchSpeed)));
	if (match.length(matchKt)) speedUnit = SpeedUnit::KNOTS;
	if (match.length(matchKmh)) speedUnit = SpeedUnit::KILOMETERS_PER_HOUR;
	if (match.length(matchMps)) speedUnit = SpeedUnit::METERS_PER_SECOND;
	return(true);
}

bool metaf::operator ==(const WindShearGroup & lhs, const WindShearGroup & rhs){
	return(lhs.direction == rhs.direction &&
		lhs.height == rhs.height &&
		lhs.speed == rhs.speed &&
		lhs.speedUnit == rhs.speedUnit &&
		lhs.heightUnit == lhs.heightUnit);
}

///////////////////////////////////////////////////////////////////////////////

VisibilityGroup VisibilityGroup::makeVisibilityMeters(Direction direction) {
	VisibilityGroup group;
	group.direction = direction;
	group.unit = VisibilityGroup::Unit::METERS;
	return(group);
}

VisibilityGroup VisibilityGroup::makeVisibilityMeters(unsigned int visibility, 
	Direction direction)
{
	VisibilityGroup group;
	group.reported = true;
	group.direction = direction;
	group.unit = VisibilityGroup::Unit::METERS;
	group.integer = visibility;
	return(group);
}

VisibilityGroup VisibilityGroup::makeVisibilityMiles() {
	VisibilityGroup group;
	group.direction = VisibilityGroup::Direction::NONE;
	group.unit = VisibilityGroup::Unit::STATUTE_MILES;
	return(group);
}

VisibilityGroup VisibilityGroup::makeVisibilityMiles(unsigned int integer, 
	unsigned int numerator, 
	unsigned int denominator,
	bool lessThan)
{
	VisibilityGroup group;
	group.direction = VisibilityGroup::Direction::NONE;
	group.unit = VisibilityGroup::Unit::STATUTE_MILES;
	group.reported = true;
	group.integer = integer;
	group.numerator = numerator;
	group.denominator = denominator;
	group.lessThan = lessThan;
	return(group);
}

VisibilityGroup VisibilityGroup::makeVisibilityMilesIncomplete(unsigned int integer) {
	VisibilityGroup group;
	group.direction = VisibilityGroup::Direction::NONE;
	group.unit = VisibilityGroup::Unit::STATUTE_MILES;
	group.reported = true;
	group.incompleteInteger = true;
	group.integer = integer;
	return(group);
}


bool VisibilityGroup::parse(const string & group, ReportPart reportPart) {
	static const regex visibilityRegex(
		"(?:(////)(SM)?)|"
		"(?:(\\d\\d\\d\\d)(?:(NDV)|(N)|(S)|(W)|(E)|(NW)|(NE)|(SW)|(SE))?)|"
		"([12])|"
		"(?:([M])?(\\d\\d?)(?:/(\\d\\d?))?SM)");
	static const auto expectedMatchGroups = 17;
	static const auto matchVisNr = 1;
	static const auto matchVisNrSm = 2;
	static const auto matchVisMeters = 3;
	static const auto matchNDV = 4;
	static const auto matchN = 5;
	static const auto matchS = 6;
	static const auto matchW = 7;
	static const auto matchE = 8;
	static const auto matchNW = 9;
	static const auto matchNE = 10;
	static const auto matchSW = 11;
	static const auto matchSE = 12;
	static const auto matchIncompleteInt = 13;
	static const auto matchFractionM = 14;
	static const auto matchFractionNumOrInt = 15;
	static const auto matchFractionDen = 16;
	smatch match;
	if (reportPart != ReportPart::METAR &&
		reportPart != ReportPart::TAF) return(false);
	if (!regex_match(group, match, visibilityRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	integer = 0;
	numerator = 0;
	denominator = 0;
	unit = Unit::UNKNOWN;
	direction = Direction::UNKNOWN;
	reported = false;
	lessThan = false;
	incompleteInteger = false;
	if (match.length(matchVisNr)) {
		direction = Direction::NONE;
		unit = Unit::METERS;
		if (match.length(matchVisNrSm)) unit = Unit::STATUTE_MILES;
		return(true);
	}
	if (match.length(matchVisMeters)) {
		unit = Unit::METERS;
		integer = static_cast<unsigned int>(stoi(match.str(matchVisMeters)));
		direction = Direction::NONE;
		if (match.length(matchNDV)) direction = Direction::NDV;
		if (match.length(matchN)) direction = Direction::N;
		if (match.length(matchS)) direction = Direction::S;
		if (match.length(matchW)) direction = Direction::W;
		if (match.length(matchE)) direction = Direction::E;
		if (match.length(matchNW)) direction = Direction::NW;
		if (match.length(matchNE)) direction = Direction::NE;
		if (match.length(matchSW)) direction = Direction::SW;
		if (match.length(matchSE)) direction = Direction::SE;
		reported = true;
		return(true);
	}
	if (match.length(matchIncompleteInt)) {
		integer = static_cast<unsigned int>(stoi(match.str(matchIncompleteInt)));
		unit = Unit::STATUTE_MILES;
		direction = Direction::NONE;
		incompleteInteger = true;
		reported = true;
		return(true);
	}
	if (match.length(matchFractionNumOrInt)) {
		if (match.length(matchFractionM)) lessThan = true;
		integer = static_cast<unsigned int>(stoi(match.str(matchFractionNumOrInt)));
		unit = Unit::STATUTE_MILES;
		direction = Direction::NONE;
		if (match.length(matchFractionDen)) {
			denominator = static_cast<unsigned int>(stoi(match.str(matchFractionDen)));
			numerator = integer;
			integer = 0;
			if (numerator >= denominator) {
				integer = numerator / 10;
				numerator %= 10;
			}
		}
		reported = true;
		return(true);
	}
	return(false);
}

bool VisibilityGroup::isValid() const {
	if (unit == Unit::METERS) {
		if (!reported) return(true);
		if (direction == Direction::UNKNOWN) return(false);
		if (incompleteInteger || lessThan) return(false);
		if (!integer || numerator || denominator) return(false);
		return(true);
	}
	if (unit == Unit::STATUTE_MILES) {
		if (!reported) return(true);
		bool fraction = numerator && denominator;
		if ((numerator && !denominator) || (!numerator && denominator)) return(false);
		if (incompleteInteger && (lessThan || !integer || fraction)) return(false);
		return(true);
	}
	return(false);
}

bool VisibilityGroup::nextGroup(const Group & nextGroup) {
	const auto nextVisGroup = std::get_if<VisibilityGroup>(&nextGroup);
	if (!nextVisGroup) return(false);
	if (this->unit != nextVisGroup->unit) return(false);
	if (!isIncompleteInteger() || !nextVisGroup->isIncompleteFraction()) return(false);
	numerator = nextVisGroup->numerator;
	denominator = nextVisGroup->denominator;
	incompleteInteger = false;
	return(true);
}

bool VisibilityGroup::isIncompleteInteger() const {
	if (!reported || !isValid()) return(false);
	return(incompleteInteger);
}

bool VisibilityGroup::isIncompleteFraction() const {
	if (!reported || !isValid()) return(false);
	return (
		!lessThan && 
		!incompleteInteger && 
		!integer &&
		numerator && 
		denominator);
}

bool metaf::operator ==(const VisibilityGroup & lhs, const VisibilityGroup & rhs){
	if (lhs.unit != rhs.unit ||	
		lhs.reported != rhs.reported || 
		lhs.direction != rhs.direction) return(false);
	if (!lhs.reported) return(true);
	if (lhs.incompleteInteger != rhs.incompleteInteger || 
		lhs.integer != rhs.integer) return(false);
	if (lhs.incompleteInteger) return(true);
	if (lhs.lessThan != rhs.lessThan || 
		lhs.numerator != rhs.numerator ||
		lhs.denominator != rhs.denominator) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

CloudGroup::CloudGroup (Amount a, unsigned int h, Type t) :
	amount(a), height(h), type(t), heightReported(true) {}

CloudGroup::CloudGroup (Amount a, Type t) :
	amount(a), height(0), type(t), heightReported(false) {}

bool CloudGroup::parse(const string & group, ReportPart reportPart) {
	static const regex cloudRegex(
	"(?:(FEW)|(SCT)|(BKN)|(OVC)|(///))" //[BFOS/][CEKV/][CNTW/]
	"(?:(\\d\\d\\d)|(?:///))"
	"((TCU)|(CB)|(///))?"
	);
	static const auto expectedMatchGroups = 11;
	static const auto matchFew = 1;
	static const auto matchSct = 2;
	static const auto matchBkn = 3;
	static const auto matchOvc = 4;
	static const auto matchAmountNr = 5;
	static const auto matchHeight = 6;
	static const auto matchType = 7;
	static const auto matchTcu = 8;
	static const auto matchCb = 9;
	static const auto matchTypeNr = 10;
	static const auto heightFactor = 100;
	smatch match;
	if (reportPart != ReportPart::METAR &&
		reportPart != ReportPart::TAF) return(false);
	if (!regex_match(group, match, cloudRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	amount = Amount::UNKNOWN;
	if (match.length(matchFew)) amount = Amount::FEW;
	if (match.length(matchSct)) amount = Amount::SCATTERED;
	if (match.length(matchBkn)) amount = Amount::BROKEN;
	if (match.length(matchOvc)) amount = Amount::OVERCAST;
	if (match.length(matchAmountNr)) amount = Amount::NOT_REPORTED;
	height = 0;
	heightReported = false;
	if (match.length(matchHeight)) {
		height = static_cast<unsigned int>(stoi(match.str(matchHeight))) * heightFactor;
		heightReported = true;
	}
	type = Type::UNKNOWN;
	if (!match.length(matchType)) type = Type::NONE;
	if (match.length(matchTcu)) type = Type::TOWERING_CUMULUS;
	if (match.length(matchCb)) type = Type::CUMULONIMBUS;
	if (match.length(matchTypeNr)) type = Type::NOT_REPORTED;
	return(true);
}

bool metaf::operator ==(const CloudGroup & lhs, const CloudGroup & rhs){
	if (lhs.amount != rhs.amount || 
		lhs.type != rhs.type ||
		lhs.heightReported != rhs.heightReported ||
		lhs.unit != rhs.unit) return(false);
	if (lhs.heightReported && (lhs.height != rhs.height)) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

VerticalVisibilityGroup::VerticalVisibilityGroup(unsigned int vv) : 
	vertVisibility(vv), reported(true) {}

bool VerticalVisibilityGroup::parse(const string & group, ReportPart reportPart) {
	static const regex vertVisRegex("VV(?:(\\d\\d\\d)|(///))");
	static const auto expectedMatchGroups = 3;
	static const auto matchVertVis = 1;
	static const auto vertVisFactor = 100;
	smatch match;
	if (reportPart != ReportPart::METAR &&
		reportPart != ReportPart::TAF) return(false);
	if (!regex_match(group, match, vertVisRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	vertVisibility = 0;
	reported = false;
	if (match.length(matchVertVis)) {
		vertVisibility = static_cast<unsigned int>(stoi(match.str(matchVertVis)));
		vertVisibility *= vertVisFactor;
		reported = true;
	}
	return(true);
}

bool metaf::operator ==(const VerticalVisibilityGroup & lhs, const VerticalVisibilityGroup & rhs) {
	if (lhs.reported != rhs.reported || 
		lhs.unit != rhs.unit) return(false);
	if (lhs.reported && (lhs.vertVisibility != rhs.vertVisibility)) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

WeatherGroup::WeatherGroup(Modifier modifier, 
	const vector<Weather> & weather) 
{
	this->modifier = modifier;
	weatherSize = 0;
	for (const auto w : weather) {
		if (weatherSize < maxWeatherSize) this->weather[weatherSize++] = w;
	}
}

WeatherGroup::WeatherGroup(Modifier modifier, 
	Weather weather1, 
	Weather weather2, 
	Weather weather3)
{
	this->modifier = modifier;
	weatherSize = 0;
	weather[weatherSize++] = weather1;
	if (weather2 != Weather::UNKNOWN) weather[weatherSize++] = weather2;
	if (weather3 != Weather::UNKNOWN) weather[weatherSize++] = weather3;
}

WeatherGroup WeatherGroup::makeNotReported(bool recent){
	WeatherGroup group;
	group.modifier = Modifier::NONE;
	if (recent) group.modifier = Modifier::RECENT;
	group.weather[0] = Weather::NOT_REPORTED;
	group.weatherSize = 1;
	return(group);
}

bool WeatherGroup::parse(const string & group, ReportPart reportPart) {
	static const regex weatherNotReportedRegex("(RE)?//");
	static const auto expectedMatchGroupsNotReported = 2;
	static const regex weatherRegex("(RE|[\\+-]|VC)?"
		"((?:MI|PR|BC|DR|BL|SH|TS|FZ|"
		"DZ|RA|SN|SG|IC|PL|GR|GS|UP|"
		"BR|FG|FU|VA|DU|SA|HZ|PY|"
		"PO|SQ|FC|SS|DS)+)"); // [BDFGHIMPRSTUV][ACGHILNOPQRSUYZ]
	static const auto expectedMatchGroups = 3;
	static const auto matchModifier = 1;
	static const auto matchWeather = 2;
	smatch match;
	if (regex_match(group, match, weatherNotReportedRegex)) {
		if (reportPart != ReportPart::METAR) return(false);
		if (match.size() != (expectedMatchGroupsNotReported)) return(false);
		modifier = modifierFromString(match.str(matchModifier));
		weather[0] = Weather::NOT_REPORTED;
		weatherSize = 1;
		return(true);
	}
	if (reportPart != ReportPart::METAR &&
		reportPart != ReportPart::TAF) return(false);
	if (!regex_match(group, match, weatherRegex)) return(false);
	if (match.size() != expectedMatchGroups) return(false);
	modifier = modifierFromString(match.str(matchModifier));
	weatherSize = 0;
	static const auto weatherStrSize = 2;
	for (auto i=0; i < match.length(matchWeather); i += weatherStrSize) {
		if (weatherSize < maxWeatherSize) {
			const string weatherStr = match.str(matchWeather).substr(i, weatherStrSize);
			weather[weatherSize++] = weatherFromString(weatherStr);
		}
	}
	return(true);
}

WeatherGroup::Modifier WeatherGroup::modifierFromString(const string & str){
	static const vector< pair<Modifier,string> > modifiers = {
		{Modifier::RECENT, "RE"},
		{Modifier::LIGHT, "-"},
		{Modifier::HEAVY, "+"},
		{Modifier::VICINITY, "VC"},
	};
	if (!str.length()) return(Modifier::NONE);
	for (const auto & m: modifiers)
		if (get<string>(m) == str) return(get<Modifier>(m));
	return(Modifier::UNKNOWN);
}

WeatherGroup::Weather WeatherGroup::weatherFromString(const string & str){
	static const vector< pair<Weather,string> > weatherPhenomena = {
		{Weather::SHALLOW, "MI"},
		{Weather::PARTIAL, "PR"},
		{Weather::PATCHES, "BC"},
		{Weather::LOW_DRIFTING, "DR"},
		{Weather::BLOWING, "BL"},
		{Weather::SHOWERS, "SH"},
		{Weather::THUNDERSTORM, "TS"},
		{Weather::FREEZING, "FZ"},
		{Weather::DRIZZLE, "DZ"},
		{Weather::RAIN, "RA"},
		{Weather::SNOW, "SN"},
		{Weather::SNOW_GRAINS, "SG"},
		{Weather::ICE_CRYSTALS, "IC"},
		{Weather::ICE_PELLETS, "PL"},
		{Weather::HAIL, "GR"},
		{Weather::SMALL_HAIL, "GS"},
		{Weather::UNDETERMINED, "UP"},
		{Weather::MIST, "BR"},
		{Weather::FOG, "FG"},
		{Weather::SMOKE, "FU"},
		{Weather::VOLCANIC_ASH, "VA"},
		{Weather::DUST, "DU"},
		{Weather::SAND, "SA"},
		{Weather::HAZE, "HZ"},
		{Weather::SPRAY, "PY"},
		{Weather::DUST_WHIRLS, "PO"},
		{Weather::SQUALLS, "SQ"},
		{Weather::FUNNEL_CLOUD, "FC"},
		{Weather::SANDSTORM, "SS"},
		{Weather::DUSTSTORM, "DS"},
	};
	if (!str.length()) return(Weather::UNKNOWN);
	for (const auto & w: weatherPhenomena)
		if (get<string>(w) == str) return(get<Weather>(w));
	return(Weather::UNKNOWN);
}

bool metaf::operator ==(const WeatherGroup & lhs, const WeatherGroup & rhs){
	if (lhs.weatherSize != rhs.weatherSize ||
		lhs.modifier != rhs.modifier) return(false);
	for (auto i=0; i<std::min<size_t>(lhs.weatherSize, WeatherGroup::maxWeatherSize); i++) {
		if (lhs.weather[i] != rhs.weather[i]) return(false);
	}
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

TemperatureGroup::TemperatureGroup(int t) : 
	temperature(t), temperatureReported(true), dewPointReported(false) {}

TemperatureGroup::TemperatureGroup(int t, int dp) :
	temperature(t), temperatureReported(true), dewPoint(dp), dewPointReported(true) {}

bool TemperatureGroup::parse(const string & group, ReportPart reportPart) {
	static const regex tempRegex("(?:(M)?(\\d\\d)|//)/(?:(M)?(\\d\\d)|//)?");
	static const auto expectedMatchGroups = 5;
	static const auto matchTempM = 1;
	static const auto matchTemp = 2;
	static const auto matchDewPointM = 3;
	static const auto matchDewPoint = 4;
	smatch match;
	if (reportPart != ReportPart::METAR) return(false);
	if (!regex_match(group, match, tempRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	temperature = 0.0;
	dewPoint = 0.0;
	temperatureReported = false;
	dewPointReported = false;
	if (match.length(matchTemp)) {
		temperature = stoi(match.str(matchTemp));
		temperatureReported = true;
	}
	if (match.length(matchTempM)) temperature = -temperature;
	if (match.length(matchDewPoint)) {
		dewPoint = stoi(match.str(matchDewPoint));
		dewPointReported = true;
	}
	if (match.length(matchDewPointM)) dewPoint = -dewPoint;
	return(true);	
}

bool metaf::operator ==(const TemperatureGroup & lhs, const TemperatureGroup & rhs){
	if (lhs.temperatureReported != rhs.temperatureReported ||
		lhs.dewPointReported != rhs.dewPointReported ||
		lhs.unit != rhs.unit) return(false);
	if (lhs.temperatureReported && (lhs.temperature != rhs.temperature)) return(false);
	if (lhs.dewPointReported && (lhs.dewPoint != rhs.dewPoint)) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

MinMaxTemperatureGroup MinMaxTemperatureGroup::makeMin(int temperature,
	unsigned int day, 
	unsigned int hour)
{
	MinMaxTemperatureGroup group;
	group.point = MinMaxTemperatureGroup::Point::MINIMUM;
	group.temperature = temperature;
	group.day = day;
	group.hour = hour;
	return(group);
}

MinMaxTemperatureGroup MinMaxTemperatureGroup::makeMax(int temperature, 
	unsigned int day, 
	unsigned int hour)
{
	MinMaxTemperatureGroup group;
	group.point = MinMaxTemperatureGroup::Point::MAXIMUM;
	group.temperature = temperature;
	group.day = day;
	group.hour = hour;
	return(group);
}

bool MinMaxTemperatureGroup::parse(const string & group, ReportPart reportPart) {
	static const regex minMaxTempRegex ("(?:(TX)|(TN))(M)?(\\d\\d)/(\\d\\d)(\\d\\d)Z");
	static const auto expectedMatchGroups = 7;
	static const auto matchMax = 1;
	static const auto matchMin = 2;
	static const auto matchM = 3;
	static const auto matchTemp = 4;
	static const auto matchDay = 5;
	static const auto matchHour = 6;
	smatch match;
	if (reportPart != ReportPart::TAF) return(false);
		if (!regex_match(group, match, minMaxTempRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	point = Point::UNKNOWN;
	if (match.length(matchMax)) point = Point::MAXIMUM;
	if (match.length(matchMin)) point = Point::MINIMUM;
	temperature = stoi(match.str(matchTemp));
	if (match.length(matchM)) temperature = -temperature;
	day = static_cast<unsigned int>(stoi(match.str(matchDay)));
	hour = static_cast<unsigned int>(stoi(match.str(matchHour)));
	return(true);
}

bool metaf::operator ==(const MinMaxTemperatureGroup & lhs, const MinMaxTemperatureGroup & rhs){
	if (lhs.point != rhs.point ||
		lhs.day != rhs.day ||
		lhs.hour != rhs.hour ||
		lhs.unit != rhs.unit) return(false);
	if (lhs.temperature != rhs.temperature) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

PressureGroup::PressureGroup(Unit u) : reported(false), unit(u) {}

PressureGroup::PressureGroup(float p, Unit u) : pressure(p), reported(true), unit(u) {}

bool PressureGroup::parse(const string & group, ReportPart reportPart){
	static const regex pressureRegex ("([QA])(?:(\\d\\d\\d\\d)|////)");
	static const auto expectedMatchGroups = 3;
	static const auto matchUnit = 1;
	static const auto matchPressure = 2;
	static const auto unitHcPa = 'Q';
	static const auto unitInHg = 'A';
	static const auto factorInHg = 0.01;
	smatch match;
	if (reportPart != ReportPart::METAR) return(false);
	if (!regex_match(group, match, pressureRegex)) return(false);
	if (match.size() != (expectedMatchGroups)) return(false);
	reported = false;
	if (match[matchUnit] == unitHcPa) unit = Unit::HECTOPASCAL;
	if (match[matchUnit] == unitInHg) unit = Unit::INCHES_HG;
	if (match.length(matchPressure)) {
		pressure = stof(match.str(matchPressure));
		reported = true;
	}
	if (unit == Unit::INCHES_HG) pressure *= factorInHg;
	return(true);
}

bool metaf::operator ==(const PressureGroup & lhs, const PressureGroup & rhs){
	static const auto precision = 0.01 / 2;
	if (lhs.unit != rhs.unit || 
		lhs.reported != rhs.reported) return(false);
	if (!lhs.reported) return(true);
	if (std::fabs(lhs.pressure - rhs.pressure) > precision) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

RunwayVisualRangeGroup::RunwayVisualRangeGroup(unsigned int rNo, 
	Designator d, 
	Unit u,
	Trend t) : 
		runwayNumber(rNo), 
		runwayDesignator(d), 
		visibilityReported(false), 
		unit(u),
		trend(t) {}

RunwayVisualRangeGroup::RunwayVisualRangeGroup(unsigned int rNo, 
	Designator d, 
	unsigned int vis, 
	Unit u,
	Modifier m,
	Trend t) :
		runwayNumber(rNo),
		runwayDesignator(d),
		visibility(vis),
		visModifier(m),
		visibilityReported(true),
		varVisibilityReported(false),
		unit(u),
		trend(t) {}

RunwayVisualRangeGroup::RunwayVisualRangeGroup(unsigned int rNo,
	Designator d,
	unsigned int minVis,
	unsigned int maxVis,
	Unit u,
	Modifier minVisMod,
	Modifier maxVisMod,
	Trend t) :
		runwayNumber(rNo),
		runwayDesignator(d),
		visibility(minVis),
		visModifier(minVisMod),
		varVisibility(maxVis),
		varVisModifier(maxVisMod),
		visibilityReported(true),
		varVisibilityReported(true),
		unit(u),
		trend(t) {}

bool RunwayVisualRangeGroup::parse(const string & group, ReportPart reportPart) {
	static const regex runwayVisibilityRegex("R(\\d\\d)([RCL])?/"
		"(?:[PM]?(////)|([PM])?(\\d\\d\\d\\d)(?:V([PM])?(\\d\\d\\d\\d))?)"
		"(FT/?)?([UND/])?");
	static const auto expectedMatchGroups = 10;
	static const auto matchRunwayNo = 1;
	static const auto matchDesignator = 2;
	static const auto matchNotReportedVisibility = 3;
	static const auto matchVisModifier = 4;
	static const auto matchVisibility = 5;
	static const auto matchMaxVisModifier = 6;
	static const auto matchMaxVisibility = 7;
	static const auto matchUnit = 8;
	static const auto matchTrend = 9;
	smatch match;
	if (reportPart!=ReportPart::METAR) return(false);
	if (!regex_match(group, match, runwayVisibilityRegex)) return(false);
	if (match.size() != expectedMatchGroups) return(false);
	runwayNumber = static_cast<unsigned int>(stoi(match.str(matchRunwayNo)));
	runwayDesignator = runwayDesignatorFromString(match.str(matchDesignator));
	unit = unitFromString(match.str(matchUnit));
	trend = trendFromString(match.str(matchTrend));
	visibilityReported = false;
	if (match.length(matchNotReportedVisibility)) return(true);
	visibilityReported = true;
	visModifier = modifierFromString(match.str(matchVisModifier));
	visibility = static_cast<unsigned int>(stoi(match.str(matchVisibility)));
	varVisibilityReported = false;
	if (match.length(matchMaxVisibility)) {
		varVisModifier = modifierFromString(match.str(matchMaxVisModifier));
		varVisibility = static_cast<unsigned int>(stoi(match.str(matchMaxVisibility)));
		varVisibilityReported = true;
	}
	return(true);
}

RunwayVisualRangeGroup::Designator RunwayVisualRangeGroup::runwayDesignatorFromString(
	const string & str)
{
	static const auto left = "L";
	static const auto right = "R";
	static const auto center = "C";
	if (!str.length()) return(Designator::NONE);
	if (str == left) return(Designator::LEFT);
	if (str == right) return(Designator::RIGHT);
	if (str == center) return(Designator::CENTER);
	return(Designator::UNKNOWN);
}

RunwayVisualRangeGroup::Modifier RunwayVisualRangeGroup::modifierFromString(
	const string & str)
{
	static const auto lessThan = "M";
	static const auto moreThan = "P";
	if (!str.length()) return(Modifier::NONE);
	if (str == lessThan) return(Modifier::LESS_THAN);
	if (str == moreThan) return(Modifier::MORE_THAN);
	return(Modifier::UNKNOWN);
}

RunwayVisualRangeGroup::Unit RunwayVisualRangeGroup::unitFromString(
	const string & str)
{
	static const auto unitFeet1 = "FT";
	static const auto unitFeet2 = "FT/";
	if (!str.length()) return(Unit::METERS);
	if (str == unitFeet1 || str == unitFeet2) return(Unit::FEET);
	return(Unit::UNKNOWN);
}

RunwayVisualRangeGroup::Trend RunwayVisualRangeGroup::trendFromString(
	const string & str)
{
	static const auto trendUpward = "U";
	static const auto trendNeutral = "N";
	static const auto trendDownward = "D";
	static const auto trendNotReported = "/";
	if (!str.length() || str == trendNotReported) return(Trend::NONE);
	if (str == trendUpward) return(Trend::UPWARD);
	if (str == trendNeutral) return(Trend::NEUTRAL);
	if (str == trendDownward) return(Trend::DOWNWARD);
	return(Trend::UNKNOWN);
}

bool metaf::operator ==(const RunwayVisualRangeGroup & lhs, 
	const RunwayVisualRangeGroup & rhs)
{
	if (lhs.runwayNumber != rhs.runwayNumber || 
		lhs.runwayDesignator != rhs.runwayDesignator ||
		lhs.unit != rhs.unit ||
		lhs.trend != rhs.trend ||
		lhs.visibilityReported != rhs.visibilityReported) return(false);
	if (!lhs.visibilityReported) return(true);
	if (lhs.visibility != rhs.visibility ||
		lhs.visModifier != rhs.visModifier ||
		lhs.varVisibilityReported != rhs.varVisibilityReported) return(false);
	if (!lhs.varVisibilityReported) return(true);
	if (lhs.varVisibility != rhs.varVisibility ||
		lhs.varVisModifier != rhs.varVisModifier) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

RunwayStateGroup::RunwayStateGroup(unsigned int rNo,
	Designator rd,
	Status s,
	Deposits d,
	Extent e,
	DepositDepth dd,
	SurfaceFriction sf) :
		runwayNumber(rNo),
		runwayDesignator(rd),
		status(s),
		deposits(d),
		contaminationExtent(e),
		depositDepth (std::move(dd)),
		surfaceFriction (std::move(sf)) {}

RunwayStateGroup RunwayStateGroup::makeSnoclo(unsigned int runwayNo,
	Designator designator)
{
	RunwayStateGroup group;
	group.runwayNumber = runwayNo;
	group.runwayDesignator = designator;
	group.status = RunwayStateGroup::Status::SNOCLO;
	return(group);
}

RunwayStateGroup RunwayStateGroup::makeClrd(unsigned int runwayNo,
	Designator designator,
	SurfaceFriction sf)
{
	RunwayStateGroup group;
	group.runwayNumber = runwayNo;
	group.runwayDesignator = designator;
	group.status = RunwayStateGroup::Status::CLRD;
	group.surfaceFriction = std::move(sf);
	return(group);
}


bool RunwayStateGroup::parse(const string & group, ReportPart reportPart) {
	static const regex runwayStateRegex("(?:R(\\d\\d)([RCL])?/)"
		"(?:"
		"(SNOCLO)"
		"|"
		"(?:(\\d|/)(\\d|/)(\\d\\d|//)|(CLRD))(\\d\\d|//)"
		")");
	static const auto expectedMatchGroups = 9;
	static const auto matchRunwayNo = 1;
	static const auto matchDesignator = 2;
	static const auto matchSnoclo = 3;
	static const auto matchDeposits = 4;
	static const auto matchExtent = 5;
	static const auto matchDepositDepth = 6;
	static const auto matchClrd = 7;
	static const auto matchSurfaceFriction = 8;
	smatch match;
	if (reportPart!=ReportPart::METAR) return(false);
	if (!regex_match(group, match, runwayStateRegex)) return(false);
	if (match.size() != expectedMatchGroups) return(false);
	runwayNumber = static_cast<unsigned int>(stoi(match.str(matchRunwayNo)));
	runwayDesignator = runwayDesignatorFromString(match.str(matchDesignator));
	status = Status::NONE;
	if (match.length(matchSnoclo)) status = Status::SNOCLO;
	if (match.length(matchClrd)) status = Status::CLRD;
	deposits = depositsFromString(match.str(matchDeposits));
	contaminationExtent = extentFromString(match.str(matchExtent));
	depositDepth = DepositDepth(match.str(matchDepositDepth));
	surfaceFriction = SurfaceFriction(match.str(matchSurfaceFriction));
	return(true);
}

RunwayStateGroup::Designator RunwayStateGroup::runwayDesignatorFromString(const string & str) {
	static const auto left = "L";
	static const auto right = "R";
	static const auto center = "C";
	if (!str.length()) return(Designator::NONE);
	if (str == left) return(Designator::LEFT);
	if (str == right) return(Designator::RIGHT);
	if (str == center) return(Designator::CENTER);
	return(Designator::UNKNOWN);
}

RunwayStateGroup::Deposits RunwayStateGroup::depositsFromString(const string & str) {
	static const auto expectedStrLength = 1;
	static const auto notReported = "/";
	if (str.length() != expectedStrLength) return(Deposits::UNKNOWN);
	if (str == notReported) return(Deposits::NOT_REPORTED);
	return(static_cast<Deposits>(stoi(str)));
}

RunwayStateGroup::Extent RunwayStateGroup::extentFromString(const string & str){
	static const auto expectedStrLength = 1;
	static const auto notReported = "/";
	if (str.length() != expectedStrLength) return(Extent::UNKNOWN);
	if (str == notReported) return(Extent::NOT_REPORTED);
	return(static_cast<Extent>(stoi(str)));
}

bool metaf::operator ==(const RunwayStateGroup & lhs, const RunwayStateGroup & rhs){
	if (lhs.runwayNumber != rhs.runwayNumber || 
		lhs.runwayDesignator != rhs.runwayDesignator ||
		lhs.status != rhs.status) return(false);
	if (lhs.status != RunwayStateGroup::Status::NONE) return(true);
	return(lhs.deposits == rhs.deposits &&
		lhs.contaminationExtent == rhs.contaminationExtent &&
		lhs.depositDepth == rhs.depositDepth &&
		lhs.surfaceFriction == rhs.surfaceFriction);
}

RunwayStateGroup::DepositDepth::DepositDepth (unsigned int d) :
	status(Status::REPORTED), depth(d) {}

RunwayStateGroup::DepositDepth::DepositDepth(const string & str) {
	static const auto expectedStrLength = 2;
	static const auto notReported = "//";
	static const auto minReservedValue = 91;
	static const auto maxReservedValue = 99;
	depth = 0;
	status = Status::UNKNOWN;
	if (str.length() != expectedStrLength) return;
	if (str == notReported) {
		status = Status::NOT_REPORTED;
		return;
	}
	const auto value = stoi(str);
	status = Status::REPORTED;
	if (value >= minReservedValue && value <= maxReservedValue) {
		switch (static_cast<Reserved>(value)) {
			case Reserved::DEPTH_10CM:
			depth = 100; //millimeters
			return;
			case Reserved::DEPTH_15CM:
			depth = 150; //millimeters
			return;
			case Reserved::DEPTH_20CM:
			depth = 200; //millimeters
			return;
			case Reserved::DEPTH_25CM:
			depth = 250; //millimeters
			return;
			case Reserved::DEPTH_30CM:
			depth = 300; //millimeters
			return;
			case Reserved::DEPTH_35CM:
			depth = 350; //millimeters
			return;
			case Reserved::DEPTH_40CM:
			depth = 400; //millimeters
			return;
			case Reserved::RUNWAY_NOT_OPERATIONAL:
			status = Status::RUNWAY_NOT_OPERATIONAL;
			return;
			case Reserved::RESERVED:
			default:
			return;
		}
	}
	depth = value;
}

RunwayStateGroup::DepositDepth RunwayStateGroup::DepositDepth::makeNotReported() {
	RunwayStateGroup::DepositDepth depositDepth;
	depositDepth.status = RunwayStateGroup::DepositDepth::Status::NOT_REPORTED;
	return(depositDepth);
}

RunwayStateGroup::DepositDepth RunwayStateGroup::DepositDepth::makeRunwayNotOperational() {
	RunwayStateGroup::DepositDepth depositDepth;
	depositDepth.status = RunwayStateGroup::DepositDepth::Status::RUNWAY_NOT_OPERATIONAL;
	return(depositDepth);
}

bool metaf::operator ==(const RunwayStateGroup::DepositDepth & lhs, 
	const RunwayStateGroup::DepositDepth & rhs)
{
	if (lhs.status != rhs.status) return(false);
	if (lhs.status != RunwayStateGroup::DepositDepth::Status::REPORTED) return(true);
	return(lhs.depth == rhs.depth);
}

RunwayStateGroup::SurfaceFriction::SurfaceFriction(BrakingAction ba) :
	status(Status::BRAKING_ACTION_REPORTED), brakingAction(ba) {}

RunwayStateGroup::SurfaceFriction::SurfaceFriction(float с) :
	status(Status::SURFACE_FRICTION_REPORTED), coefficient(с) {}

RunwayStateGroup::SurfaceFriction RunwayStateGroup::SurfaceFriction::makeNotReported() {
	RunwayStateGroup::SurfaceFriction surfaceFrictionData;
	surfaceFrictionData.status = RunwayStateGroup::SurfaceFriction::Status::NOT_REPORTED;
	return(surfaceFrictionData);
}

RunwayStateGroup::SurfaceFriction RunwayStateGroup::SurfaceFriction::makeUnreliable() {
	RunwayStateGroup::SurfaceFriction surfaceFrictionData;
	surfaceFrictionData.status = RunwayStateGroup::SurfaceFriction::Status::UNRELIABLE;
	return(surfaceFrictionData);
}

RunwayStateGroup::SurfaceFriction::SurfaceFriction(const string & str){
	static const auto expectedStrLength = 2;
	static const auto notReported = "//";
	static const auto minReservedValue = 91;
	static const auto maxReservedValue = 99;
	static const auto coefficientFactor = 0.01;
	coefficient = 0.0;
	brakingAction = BrakingAction::UNKNOWN;
	status = Status::UNKNOWN;
	if (str.length() != expectedStrLength) return;
	if (str == notReported) {
		status = Status::NOT_REPORTED;
		return;
	}
	const auto value = stoi(str);
	if (value >= minReservedValue && value <= maxReservedValue) {
		status = Status::BRAKING_ACTION_REPORTED;
		switch (static_cast<Reserved>(value)) {
			case Reserved::BRAKING_ACTION_POOR:
			brakingAction = BrakingAction::POOR;
			return;
			case Reserved::BRAKING_ACTION_MEDIUM_POOR:
			brakingAction = BrakingAction::MEDIUM_POOR;
			return;
			case Reserved::BRAKING_ACTION_MEDIUM:
			brakingAction = BrakingAction::MEDIUM;
			return;
			case Reserved::BRAKING_ACTION_MEDIUM_GOOD:
			brakingAction = BrakingAction::MEDIUM_GOOD;
			return;
			case Reserved::BRAKING_ACTION_GOOD:
			brakingAction = BrakingAction::GOOD;
			return;
			case Reserved::UNRELIABLE:
			status = Status::UNRELIABLE;
			return;
			case Reserved::RESERVED_96:
			case Reserved::RESERVED_97:
			case Reserved::RESERVED_98:
			default:
			status = Status::UNKNOWN;
			return;
		};
	}
	status = Status::SURFACE_FRICTION_REPORTED;
	coefficient = value * coefficientFactor;
}

bool metaf::operator ==(const RunwayStateGroup::SurfaceFriction & lhs, 
	const RunwayStateGroup::SurfaceFriction & rhs)
{
	static const auto precision = 0.01 / 2;
	if (lhs.status != rhs.status) return(false);
	if (lhs.status == RunwayStateGroup::SurfaceFriction::Status::SURFACE_FRICTION_REPORTED &&
		(fabs(lhs.coefficient - rhs.coefficient) > precision)) return(false);
	if (lhs.status == RunwayStateGroup::SurfaceFriction::Status::BRAKING_ACTION_REPORTED &&
		(lhs.brakingAction != rhs.brakingAction)) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

RainfallGroup::RainfallGroup(float l10m, float s9am) : 
	last10Minutes(l10m), since9AM (s9am), last60MinutesReported(false) {}

RainfallGroup::RainfallGroup(float l10m, float s9am, float l60m) :
	last10Minutes(l10m), 
	since9AM (s9am), 
	last60Minutes(l60m),
	last60MinutesReported(true) {}

bool RainfallGroup::parse(const string & group, ReportPart reportPart) {
	static const regex rainfallRegex ("RF(\\d\\d.\\d)/(\\d\\d\\d.\\d)(?:/(\\d\\d\\d.\\d))?");
	static const auto expectedMatchGroups = 4;
	static const auto matchLast10Minutes = 1;
	static const auto matchSince9AM = 2;
	static const auto matchLast60Minutes = 3;
	smatch match;
	if (reportPart!=ReportPart::METAR) return(false);
	if (!regex_match(group, match, rainfallRegex)) return(false);
	if (match.size() != expectedMatchGroups) return(false);
	last10Minutes = stof(match.str(matchLast10Minutes));
	since9AM = stof(match.str(matchSince9AM));
	last60MinutesReported = false;
	if (match.length(matchLast60Minutes)) {
		last60Minutes = stof(match.str(matchLast60Minutes));
		last60MinutesReported = true;
	}
	return(true);
}

bool metaf::operator ==(const RainfallGroup & lhs, const RainfallGroup & rhs){
	static const auto precision = 0.1 / 2;
	if (fabs(lhs.last10Minutes - rhs.last10Minutes) > precision) return(false);
	if (fabs(lhs.since9AM - rhs.since9AM) > precision) return(false);
	if (lhs.last60MinutesReported != rhs.last60MinutesReported ||
		lhs.unit != rhs.unit) return(false);
	if (!lhs.last60MinutesReported) return(true);
	if (fabs(lhs.last60Minutes - rhs.last60Minutes) > precision) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

SeaSurfaceGroup::SeaSurfaceGroup(int t, StateOfSurface s) :
	temperature(t), temperatureReported(true), stateOfSurface(s) {}

SeaSurfaceGroup::SeaSurfaceGroup(StateOfSurface s) :
	temperatureReported(false), stateOfSurface(s) {}

bool SeaSurfaceGroup::parse(const string & group, ReportPart reportPart) {
	static const regex seaSurfaceRegex("W(?:(\\d\\d)|//)/S(?:(\\d)|/)");
	static const auto expectedMatchGroups = 3;
	static const auto matchTemperature = 1;
	static const auto matchStateOfSurface = 2;
	smatch match;
	if (reportPart!=ReportPart::METAR) return(false);
	if (!regex_match(group, match, seaSurfaceRegex)) return(false);
	if (match.size() != expectedMatchGroups) return(false);
	temperature = 0;
	temperatureReported = false;
	if (match.length(matchTemperature)) {
		temperature = stoi(match.str(matchTemperature));		
		temperatureReported = true;
	}
	stateOfSurface = stateOfSurfaceFromString(match.str(matchStateOfSurface));
	return(true);
}

SeaSurfaceGroup::StateOfSurface SeaSurfaceGroup::stateOfSurfaceFromString(const string & str) {
	static const auto maxValue = 9;
	if (!str.length()) return(StateOfSurface::NOT_REPORTED);
	const auto value = stoi(str);
	if (value<0 || value>maxValue) return(StateOfSurface::UNKNOWN);
	return(static_cast<StateOfSurface>(value));
}

bool metaf::operator ==(const SeaSurfaceGroup & lhs, const SeaSurfaceGroup & rhs){
	if (lhs.temperatureReported != rhs.temperatureReported ||
		lhs.stateOfSurface != rhs.stateOfSurface ||
		lhs.temperatureUnit != rhs.temperatureUnit) return(false);
	if (!lhs.temperatureReported) return(true);
	if (lhs.temperature != rhs.temperature) return(false);
	return(true);
}

///////////////////////////////////////////////////////////////////////////////

SeaWavesGroup::SeaWavesGroup(int t, unsigned int wh) :
	temperature(t), 
	temperatureReported(true), 
	waveHeight(wh),
	waveHeightReported(true) {}

SeaWavesGroup SeaWavesGroup::makeTemperature(int temperature) {
	SeaWavesGroup group;
	group.temperature = temperature;
	group.temperatureReported = true;
	return(group);
}

SeaWavesGroup SeaWavesGroup::makeWaveHeight(unsigned int waveHeight) {
	SeaWavesGroup group;
	group.waveHeight = waveHeight;
	group.waveHeightReported = true;
	return(group);
}

bool SeaWavesGroup::parse(const string & group, ReportPart reportPart) {
	static const regex seaWavesRegex ("W(?:(\\d\\d)|//)/H(?:(\\d?\\d\\d)|///?)");
	static const auto expectedMatchGroups = 3;
	static const auto matchTemperature = 1;
	static const auto matchWaveHeight = 2;
	smatch match;
	if (reportPart!=ReportPart::METAR) return(false);
	if (!regex_match(group, match, seaWavesRegex)) return(false);
	if (match.size() != expectedMatchGroups) return(false);
	temperature = 0;
	temperatureReported = false;
	if (match.length(matchTemperature)) {
		temperature = stoi(match.str(matchTemperature));
		temperatureReported = true;
	}
	waveHeight = 0;
	waveHeightReported = false;
	if (match.length(matchWaveHeight)) {
		waveHeightReported = true;
		waveHeight = static_cast<unsigned int>(stoi(match.str(matchWaveHeight)));
	}
	return(true);
}

bool metaf::operator ==(const SeaWavesGroup & lhs, const SeaWavesGroup & rhs){
	if (lhs.temperatureReported != rhs.temperatureReported ||
		lhs.waveHeightReported != rhs.waveHeightReported ||
		lhs.temperatureUnit != rhs.temperatureUnit ||
		lhs.waveHeightUnit != rhs.waveHeightUnit) return(false);
		if (lhs.temperatureReported && 
			(lhs.temperature != rhs.temperature)) return(false);
		if (lhs.waveHeightReported && 
			(lhs.waveHeight != rhs.waveHeight)) return(false);
		return(true);
}

///////////////////////////////////////////////////////////////////////////////

ColourCodeGroup::ColourCodeGroup(Code c, bool cb) : 
	code(c), codeBlack(cb) {}

bool ColourCodeGroup::parse(const string & group, ReportPart reportPart){
	static const regex colourCodeRegex("(BLACK)?(?:(BLU)|(WHT)|(GRN)|(YLO1)|(YLO2)|(AMB)|(RED))");
	//[ABGRWY][EHLMR][BDNOTU][12]?
	static const auto expectedMatchGroups = 9;
	static const auto matchBlack = 1;
	static const auto matchBlu = 2;
	static const auto matchWht = 3;
	static const auto matchGrn = 4;
	static const auto matchYlo1 = 5;
	static const auto matchYlo2 = 6;
	static const auto matchAmb = 7;
	static const auto matchRed = 8;
	smatch match;
	if (reportPart!=ReportPart::METAR) return(false);
	if (!regex_match(group, match, colourCodeRegex)) return(false);
	if (match.size() != expectedMatchGroups) return(false);
	if (match.length(matchBlack)) codeBlack = true;
	if (match.length(matchBlu)) code = Code::BLUE;
	if (match.length(matchWht)) code = Code::WHITE;
	if (match.length(matchGrn)) code = Code::GREEN;
	if (match.length(matchYlo1)) code = Code::YELLOW1;
	if (match.length(matchYlo2)) code = Code::YELLOW2;
	if (match.length(matchAmb)) code = Code::AMBER;
	if (match.length(matchRed)) code = Code::RED;
	return(true);
}

bool metaf::operator ==(const ColourCodeGroup & lhs, const ColourCodeGroup & rhs){
	return(lhs.codeBlack == rhs.codeBlack && lhs.code == rhs.code);
}

///////////////////////////////////////////////////////////////////////////////

Group GroupParser::parse (const string & group, ReportPart reportPart) {
	return(ParseGroupHelper<0, variant_alternative_t<0, Group>>::parse(group, reportPart));
}

template <size_t I, class T> 
Group GroupParser::ParseGroupHelper<I, T>::parse(const string & group, ReportPart reportPart) {
	if (!is_same<T, PlainTextGroup>::value) {
		T temporaryGroup;
		if (temporaryGroup.parse(group, reportPart)) return(temporaryGroup);
	}
	return(ParseGroupHelper<I+1, variant_alternative_t<I+1, Group>>::parse(group, reportPart));
}

template <class T>
Group GroupParser::ParseGroupHelper<GroupParser::groups_total-1, T>::parse(const string & group, ReportPart reportPart) {
	if (!is_same<T, PlainTextGroup>::value) {
		T temporaryGroup;
		if (temporaryGroup.parse(group, reportPart)) return(temporaryGroup);
	}
	PlainTextGroup plainTextGroup;
	plainTextGroup.parse(group, reportPart);
	return(plainTextGroup);
}

bool Parser::parse(const string & metarTafString) {
	static const regex delimiterRegex("\\s+");
	sregex_token_iterator iter(metarTafString.begin(), metarTafString.end(), delimiterRegex, -1);
	bool reportEnd = false;
	State state = State::REPORT_TYPE_OR_LOCATION;
	resetResult();
	while (iter!=sregex_token_iterator() && !reportEnd && state!=State::ERROR) {
		static const char reportEndChar = '=';
		string groupString = *iter;
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
			result.push_back(group);
		}
		iter++;
	}
	state = finalTransition(state);
	return(error == Error::NONE);
}

void Parser::resetResult() {
	result.clear();
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