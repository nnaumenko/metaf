/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "metaf.hpp"
#include "testdata_real.h"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <utility>
#include <functional>
#include <regex>
#include <sstream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/// Get an index from type included in variant type at compile time.
/// @tparam V Variant type to search for index.
/// @tparam T Type which index must be returned.
/// @return Index of type T in variant V; or variant size if V does not contain T.
template<typename V, typename T, size_t I = 0>
constexpr size_t variant_index() {
    if constexpr (I >= variant_size_v<V>) {
    	return variant_size_v<V>;
    } else {
    	if constexpr (is_same_v<variant_alternative_t<I, V>, T>) {
    		return I;
    	} else {
    		return variant_index<V, T, I + 1>();
    	}
    }
}

///////////////////////////////////////////////////////////////////////////////

/// @details MUST be modified accordingly if Group is modified
/// @param index Value obtained from index() method of Group variant.
/// @return Name of the group in human-readable form.
string_view groupName (size_t index) {
	switch (index) {
		case variant_index<metaf::Group, metaf::FixedGroup>():
		return "FixedGroup";

		case variant_index<metaf::Group, metaf::LocationGroup>():
		return "LocationGroup";

		case variant_index<metaf::Group, metaf::ReportTimeGroup>():
		return "ReportTimeGroup";

		case variant_index<metaf::Group, metaf::TrendGroup>():
		return "TrendGroup";

		case variant_index<metaf::Group, metaf::WindGroup>():
		return "WindGroup";

		case variant_index<metaf::Group, metaf::VisibilityGroup>():
		return "VisibilityGroup";

		case variant_index<metaf::Group, metaf::CloudGroup>():
		return "CloudGroup";

		case variant_index<metaf::Group, metaf::WeatherGroup>():
		return "WeatherGroup";

		case variant_index<metaf::Group, metaf::TemperatureGroup>():
		return "TemperatureGroup";

		case variant_index<metaf::Group, metaf::TemperatureForecastGroup>():
		return "TemperatureForecastGroup";

		case variant_index<metaf::Group, metaf::PressureGroup>():
		return "PressureGroup";

		case variant_index<metaf::Group, metaf::RunwayStateGroup>():
		return "RunwayStateGroup";

		case variant_index<metaf::Group, metaf::SeaSurfaceGroup>():
		return "SeaSurfaceGroup";

		case variant_index<metaf::Group, metaf::MinMaxTemperatureGroup>():
		return "MinMaxTemperatureGroup";

		case variant_index<metaf::Group, metaf::PrecipitationGroup>():
		return "PrecipitationGroup";

		case variant_index<metaf::Group, metaf::LayerForecastGroup>():
		return "LayerForecastGroup";

		case variant_index<metaf::Group, metaf::PressureTendencyGroup>():
		return "PressureTendencyGroup";

		case variant_index<metaf::Group, metaf::CloudTypesGroup>():
		return "CloudTypesGroup";

		case variant_index<metaf::Group, metaf::CloudLayersGroup>():
		return "CloudLayersGroup";

		case variant_index<metaf::Group, metaf::LightningGroup>():
		return "LightningGroup";

		case variant_index<metaf::Group, metaf::VicinityGroup>():
		return "VicinityGroup";

		case variant_index<metaf::Group, metaf::MiscGroup>():
		return "MiscGroup";

		case variant_index<metaf::Group, metaf::UnknownGroup>():
		return "UnknownGroup";

		default: return "UNDEFINED";
	}
}

///////////////////////////////////////////////////////////////////////////////

/// Performance checker. Runs process() and displays performance stats.
class PerformanceCheckerBase {
public:
	void run(ostream & output);
protected:
	string_view getItemName() { return itemName; }
	void setItemName(string_view name) { itemName = name; }
	virtual int process() = 0;
private:
	string_view itemName = "item";
};

void PerformanceCheckerBase::run(ostream & output) {
	auto beginTime = chrono::system_clock::now();
	auto itemCount = this->process();
	auto endTime = chrono::system_clock::now();
	if (!itemCount) {
		output << "Test failed.\n";
		return;
	}
	auto totalTime = chrono::microseconds(endTime - beginTime);
	auto averageTimePerItem = chrono::microseconds(totalTime / itemCount);
	if (totalTime.count()) {
		output << totalTime.count() << " microseconds, ";
	} else {
		output << "<1 microsecond, ";
	}
	output << itemCount << " " << getItemName() << "s, ";
	if (averageTimePerItem.count()) {
		output << averageTimePerItem.count() << " microseconds per " << getItemName() << ", ";	
	} else {
		output << "<1 microsecond per " << getItemName() << ", ";	
	}
	static const auto microsecondsPerSecond = chrono::microseconds(chrono::seconds(1)).count();
	if (averageTimePerItem.count()) {
		output << microsecondsPerSecond / averageTimePerItem.count();
		output << " " << getItemName() << "s per second";
	} else {
		output << ">" << microsecondsPerSecond << " " << getItemName() << "s per second";
	}
	output << "\n";
}

///////////////////////////////////////////////////////////////////////////////

/// Parses all METAR and TAF reports from testdata_real.cpp and displays 
/// performance stats of parsing entire report.
class ParserPerformanceChecker : public PerformanceCheckerBase {
public:
	ParserPerformanceChecker() { setItemName("report"); }
protected:
	virtual int process();
};

int ParserPerformanceChecker::process() {
	auto reportCount = 0;
	for (const auto & data : testdata::realDataSet) {
 		if (!data.metar.empty()) {
			metaf::Parser::parse(data.metar);
			reportCount++;
		}
		if (!data.taf.empty()) {
			metaf::Parser::parse(data.taf);
			reportCount++;
		}
	}
	return reportCount;
}

///////////////////////////////////////////////////////////////////////////////

/// Parses a number of groups and displays performance stats.
class GroupPerformanceChecker : public PerformanceCheckerBase {
public:
	GroupPerformanceChecker(const vector<pair<string, metaf::ReportPart>> & srcs, 
		size_t idx) : sources(&srcs), index(idx) {setItemName("group"); }
protected:
	virtual int process();
private:
	const vector<pair<string, metaf::ReportPart>> * sources = nullptr;
	size_t index = 0;
};

int GroupPerformanceChecker::process() {
	int groupCount = 0;
	static const auto testRepetitions = 300;
	for (auto i = 0; i < testRepetitions; i++) {
		for (auto src : *sources) {
			const auto parseResult = 
				metaf::GroupParser::parse(get<string>(src), 
					get<metaf::ReportPart>(src),
					metaf::missingMetadata);
			(void)parseResult;
			(void)index;
			//if (parseResult.index() != index) return 0;
			groupCount++;
		}
	}
	return groupCount;
}

///////////////////////////////////////////////////////////////////////////////


/// Splits METAR or TAF report into individual groups and saves group string 
/// along with associated report parts. The strings associated with a certain
/// group type can be acquired for use in tests.
class GroupsTestSet {
public:
	GroupsTestSet();
	size_t size() { return testSet.size(); }
	const vector<pair<string, metaf::ReportPart>> & groupSet(size_t index) { 
		return testSet.at(index);
	}
	size_t totalGroups() {
		auto count = 0;
		for (const auto gs : testSet) { count += gs.size(); }
		return count;
	}
	void displayStats(ostream & output);
	void runPerformanceTests(ostream & output);
private:
	vector <vector<pair<string, metaf::ReportPart>>> testSet;
	void getGroupsFromReport(const std::string & report);
};

GroupsTestSet::GroupsTestSet() {
	testSet.resize(variant_size_v<metaf::Group>);
	for (const auto & data : testdata::realDataSet) {
 		if (!data.metar.empty()) getGroupsFromReport(data.metar);
		if (!data.taf.empty()) getGroupsFromReport(data.taf);
	}
}

void GroupsTestSet::getGroupsFromReport(const std::string & report)
{
	const auto parseResult = metaf::Parser::parse(report);
	for (const auto gr : parseResult.groups) {
		std::istringstream iss(gr.rawString);
		std::vector<std::string> individualGroups(
			std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>());
		for (const auto grstr : individualGroups) {
			auto resultPair = make_pair(grstr, gr.reportPart);
			const auto index = gr.group.index();
			testSet.at(index).push_back(resultPair);
		}
	}
}

void GroupsTestSet::displayStats(ostream & output) {
	output << "Group statistics\n";
	for (auto i=0u; i<size(); i++) {
		output << groupName(i) << ": " << groupSet(i).size() << " groups";
		output << " (" << (100.0 * groupSet(i).size() / totalGroups()) << "%)";
		output << "\n";
	}
	output << "Total: " << totalGroups() << " groups\n";
}

void GroupsTestSet::runPerformanceTests(ostream & output) {
	for (auto i=0u; i<size(); i++) {
		output << "Checking performance of " << groupName(i) << "\n";
		GroupPerformanceChecker checker(groupSet(i), i);
		checker.run(output);
		output << "\n";
	}
}

///////////////////////////////////////////////////////////////////////////////

void addGroup(const string & group, vector< pair<int, string> > & dst) {
	for (auto i=0u; i<dst.size(); i++) {
		if (get<string>(dst[i]) == group) {
			//found group in dst, increment counter
			auto count = get<int>(dst[i]);
			auto temp = make_pair(++count, get<string>(dst[i]));
			swap(dst[i], temp);
			return;
		}
	}
	//group not found in dst, add
	dst.push_back(make_pair(1, group));
}

int addPlainTextGroups(const metaf::ParseResult & src, vector< pair<int, string> > & dst){
	auto count = 0;	
	for (const auto & gr : src.groups) {
		if (holds_alternative<metaf::UnknownGroup>(gr.group)) {
			std::istringstream iss(gr.rawString);
			std::istream_iterator<std::string> iter(iss);
			while (iter != std::istream_iterator<std::string>()) {
				addGroup(*iter, dst);
				iter++;	
			    count++;
			}
		}
	}
	return count;
}

void printFlaggedGroups(vector< pair<int, string> > flaggedGroups) {
	cout << "Groups not recognised by parser, sorted by occurrence frequency:" << endl;
	static const string occurredHeader = "Occurred";
	static const string groupHeader = "Group text";
	static const string separator = "    ";
	cout << occurredHeader << separator << groupHeader << endl;
	for (const auto & data : flaggedGroups) {
		static const auto minCountSize = occurredHeader.length();
		string count = to_string(get<int>(data));
		if (count.length() < minCountSize) {
			count = string(minCountSize - count.length(), ' ') + count;
		}
		cout << count << separator << get<string>(data) << endl;
	}
	cout << endl;
}

void printFlaggedReports(vector< pair<int, string> > flaggedReports) {
	cout << "Reports which contain groups not recognised by parser, sorted by number or groups:" << endl;
	for (const auto & data : flaggedReports) {
		cout << "Unrecognised groups: " << get<int>(data) << endl;
		cout << "Report: " << get<string>(data) << endl << endl;
	}
	cout << endl;
}

void printGroupStats(int total, int unrecognised, const string & type) {
	const auto recognised = total - unrecognised;
	cout << "Total " << total << " " << type << " groups, ";
	cout << unrecognised << " groups not recognised, ";
	cout << recognised << " groups parsed successfully" << endl;
	static const float roundingFactor = 100.0; //round to 2 digits after decimal point
	const float unrecognisedPercent = 
		static_cast<int>((unrecognised * 100.0 / total) * roundingFactor) / roundingFactor;
	const float recognisedPercent = 
		static_cast<int>((recognised * 100.0 / total) * roundingFactor) / roundingFactor;
	cout << unrecognisedPercent << " % not recognised, ";
	cout << recognisedPercent << " % parsed successfully";
	cout << endl << endl;
}

void checkRecognisedGroups() {
	cout << "Detecting non-recognised groups in testdata::realDataSet" << endl;
	vector< pair<int, string> > flaggedGroups;	
	vector< pair<int, string> > flaggedReports;
	auto metarTotalGroupCount = 0;
	auto tafTotalGroupCount = 0;
	auto metarUnrecognisedGroupCount = 0;
	auto tafUnrecognisedGroupCount = 0;
	for (const auto & data : testdata::realDataSet) {
 		if (!data.metar.empty()) {
			const auto parseResult = metaf::Parser::parse(data.metar);
			auto count = addPlainTextGroups(parseResult, flaggedGroups);
			if (count) flaggedReports.push_back(make_pair(count, data.metar));
			metarTotalGroupCount += parseResult.groups.size();
			metarUnrecognisedGroupCount += count;
		}
		if (!data.taf.empty()) {
			const auto parseResult = metaf::Parser::parse(data.taf);
			auto count = addPlainTextGroups(parseResult, flaggedGroups);
			if (count) flaggedReports.push_back(make_pair(count, data.taf));
			tafTotalGroupCount += parseResult.groups.size();
			tafUnrecognisedGroupCount += count;
		}
	}
	printGroupStats(metarTotalGroupCount, metarUnrecognisedGroupCount, "METAR");
	printGroupStats(tafTotalGroupCount, tafUnrecognisedGroupCount, "TAF");
	sort(flaggedGroups.begin(), flaggedGroups.end(), greater< pair<int, string> >());
	sort(flaggedReports.begin(), flaggedReports.end(), greater< pair<int, string> >());
	printFlaggedGroups(flaggedGroups);
	printFlaggedReports(flaggedReports);
}

///////////////////////////////////////////////////////////////////////////////

void printSize(string_view name, size_t size) {
	cout << "Size of " << name << ' ' << size << endl;
}

void printDataSize(){
	cout << "Data type sizes:" << endl; 	
	printSize("Runway", sizeof(metaf::Runway));
	printSize("MetafTime", sizeof(metaf::MetafTime));
	printSize("Temperature", sizeof(metaf::Temperature));
	printSize("Speed", sizeof(metaf::Speed));
	printSize("Distance", sizeof(metaf::Distance));
	printSize("Direction", sizeof(metaf::Direction));
	printSize("Pressure", sizeof(metaf::Pressure));
	printSize("Precipitation", sizeof(metaf::Precipitation));
	printSize("SurfaceFriction", sizeof(metaf::SurfaceFriction));
	printSize("WaveHeight", sizeof(metaf::WaveHeight));
	printSize("WeatherPhenomena", sizeof(metaf::WeatherPhenomena));
	cout << endl;
	printSize("Group", sizeof(metaf::Group));
	cout << endl;
	cout << "Group sizes:" << endl; 	
	printSize(groupName(0), sizeof(variant_alternative_t<0, metaf::Group>));
	printSize(groupName(1), sizeof(variant_alternative_t<1, metaf::Group>));
	printSize(groupName(2), sizeof(variant_alternative_t<2, metaf::Group>));
	printSize(groupName(3), sizeof(variant_alternative_t<3, metaf::Group>));
	printSize(groupName(4), sizeof(variant_alternative_t<4, metaf::Group>));
	printSize(groupName(5), sizeof(variant_alternative_t<5, metaf::Group>));
	printSize(groupName(6), sizeof(variant_alternative_t<6, metaf::Group>));
	printSize(groupName(7), sizeof(variant_alternative_t<7, metaf::Group>));
	printSize(groupName(8), sizeof(variant_alternative_t<8, metaf::Group>));
	printSize(groupName(9), sizeof(variant_alternative_t<9, metaf::Group>));
	printSize(groupName(10), sizeof(variant_alternative_t<10, metaf::Group>));
	printSize(groupName(11), sizeof(variant_alternative_t<11, metaf::Group>));
	printSize(groupName(12), sizeof(variant_alternative_t<12, metaf::Group>));
	printSize(groupName(13), sizeof(variant_alternative_t<13, metaf::Group>));
	printSize(groupName(14), sizeof(variant_alternative_t<14, metaf::Group>));
	printSize(groupName(15), sizeof(variant_alternative_t<15, metaf::Group>));
	printSize(groupName(16), sizeof(variant_alternative_t<16, metaf::Group>));
	printSize(groupName(17), sizeof(variant_alternative_t<17, metaf::Group>));
	printSize(groupName(18), sizeof(variant_alternative_t<18, metaf::Group>));
	printSize(groupName(19), sizeof(variant_alternative_t<19, metaf::Group>));
	printSize(groupName(20), sizeof(variant_alternative_t<20, metaf::Group>));
	printSize(groupName(21), sizeof(variant_alternative_t<21, metaf::Group>));
	printSize(groupName(22), sizeof(variant_alternative_t<22, metaf::Group>));
	cout << endl;
}

int main(int argc, char ** argv) {
	(void) argc; (void) argv;
	{
		cout << "Checking parser performance\n";
		ParserPerformanceChecker checker;
		checker.run(cout);
		cout << "\n";
	}
	{
		cout << "Checking group performance\n";
		GroupsTestSet groupsTestSet;
		groupsTestSet.displayStats(cout);
		cout << "\n";
		groupsTestSet.runPerformanceTests(cout);
		cout << "\n";
	}
	checkRecognisedGroups();
	printDataSize();
}