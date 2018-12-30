/*
* Copyright (C) 2018 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

/// @file 
/// @brief Performance check and stats for metaf library.

#include "metaf.h"
#include "testdata_real.h"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <utility>
#include <functional>

using namespace std;

void checkPerformance() {
	cout << "Checking performance on testdata::realDataSet" << endl;
	metaf::Parser parser;
	auto beginTime = chrono::system_clock::now();
	auto reportCount = 0;
	for (const auto & data : testdata::realDataSet) {
		//Not using strlen to determine empty reports in order exclude strlen 
		//from performance check
 		if (data.metar[0]) {
			parser.parse(data.metar);
			reportCount++;
		}
		if (data.taf[0]) {
			parser.parse(data.taf);
			reportCount++;
		}
	}
	auto endTime = chrono::system_clock::now();
	auto parseTime = chrono::microseconds(endTime - beginTime);
	auto averageReportTime = chrono::microseconds(parseTime / reportCount);
	static const auto microsecondsPerSecond = chrono::microseconds(chrono::seconds(1)).count();
	cout << "Parser performance: ";
	cout << parseTime.count() << " microseconds, ";
	cout << reportCount << " reports, ";
	cout << averageReportTime.count() << " microseconds per report, ";
	cout << microsecondsPerSecond / averageReportTime.count();
	cout << " reports per second" << endl << endl;
}

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

int addPlainTextGroups(const vector<metaf::Group> & src, vector< pair<int, string> > & dst){
	auto count = 0;
	for (const auto & group : src) {
		if (holds_alternative<metaf::FixedGroup>(group)) {
			auto fixedGroup = get_if<metaf::FixedGroup>(&group);
			if (fixedGroup->type == metaf::FixedGroup::Type::RMK) return (count);
		}
		if (holds_alternative<metaf::PlainTextGroup>(group)) {
			auto plainTextGroup = get_if<metaf::PlainTextGroup>(&group);
			if (plainTextGroup) addGroup(plainTextGroup->text, dst);
			count++;
		}
	}
	return(count);
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
	metaf::Parser parser;
	auto metarTotalGroupCount = 0;
	auto tafTotalGroupCount = 0;
	auto metarUnrecognisedGroupCount = 0;
	auto tafUnrecognisedGroupCount = 0;
	for (const auto & data : testdata::realDataSet) {
 		if (strlen(data.metar)) {
			parser.parse(data.metar);
			auto count = addPlainTextGroups(parser.getResult(), flaggedGroups);
			if (count) flaggedReports.push_back(make_pair(count, data.metar));
			metarTotalGroupCount += parser.getResult().size();
			metarUnrecognisedGroupCount += count;
		}
		if (strlen(data.taf)) {
			parser.parse(data.taf);
			auto count = addPlainTextGroups(parser.getResult(), flaggedGroups);
			if (count) flaggedReports.push_back(make_pair(count, data.taf));
			tafTotalGroupCount += parser.getResult().size();
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

int main(int argc, char ** argv) {
	(void) argc; (void) argv;
	checkPerformance();
	checkRecognisedGroups();
}