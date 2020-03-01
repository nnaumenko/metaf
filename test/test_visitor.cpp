/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.hpp"

using namespace std;

// Test reports

static const std::vector<std::string> testReports = {
	"TAF BGTL 060900Z 0609/0715 VRB06KT 8000 -SHRASN OVC003 610105 QNH2980INS"
	" BECMG 0614/0615 VRB06KT 9999 NSW BKN005 OVC015 620105 QNH2983INS "
	"TX03/0620Z TNM02/0709Z"s,

	"METAR EGYD 281050Z 11015KT 5000 -SHSN FEW004 BKN012CB BKN025"
	" M04/M05 Q1020 BLACKGRN"s,

	"METAR PABR 091953Z 28006KT 3/4SM R07/2000V4500FT BR OVC003 06/06 A3006"
	" RMK SLP179 VIS 1/2V1 T00560056 VIA AUTODIAL"s,

	"METAR CYHI 171900Z CCA 34019KT 10SM -SN DRSN VCSH VCFG"
	" FEW011 BKN030 BKN040 BKN060 BKN150 M05/M09 A2994"
	" RMK SC1ST4SC1SC1AC1 SC TR AC TR CIG RAG VIS SW 15 SLP144"s,

	"SPECI UKLL 202304Z 00000MPS 4800 1100SE R31/1900 BR NSC 11/10 Q1020 R31/CLRD//"
	" TEMPO 0400 FG VV002"s,

	"METAR SKBG 232200Z 31006KT 9999 FEW015 26/19 A2989 WS R35"s,

	"METAR YPKG 232300Z AUTO 26013KT 9999 // NCD 17/10 Q1010 RF00.0////./"s,

	"METAR ENSE 201620Z AUTO 24040KT 9999NDV FEW010/// SCT020///"
	" BKN027/// 12/10 Q0988 W///S7"s,

	"SPECI MMMM 171315Z 00000KT 5SM VCFG SCT008 BKN080 BKN230 15/15 A3023"
	" RMK 8/578 HZY"s,

	"METAR EGUL 211156Z 25014G22KT 9999 FEW046 SCT055 14/04 A2971"
	" RMK AO2A PK WND 24029/06 SLP064 70021 T01370043 10145 20103 52016 $"s,

	"METAR KABQ 092052Z 23004KT 10SM FEW080 FEW120 FEW180 SCT300 29/07 A3005"
	" RMK AO2 LTG DSNT SE SLP085 CB DSNT N-NE AND SE T02940067 58025"s
};

/// Get an index from type included in variant type at compile time.
/// @tparam V Variant type to search for index.
/// @tparam T Type which index must be returned.
/// @return Index of type T in variant V. If T is not in V, this won't compile.
template<typename V, typename T, size_t I = 0>
constexpr size_t variant_index() {
    if constexpr (I >= variant_size_v<V>) {
    	return (variant_size_v<V>);
    } else {	
    	if constexpr (is_same_v<variant_alternative_t<I, V>, T>) {
    		return (I);
    	} else {
    		return (variant_index<V, T, I + 1>());
    	}
    }
}

///////////////////////////////////////////////////////////////////////////////
// Visitor which does not return a value
// Purpose: to confirm that Visitor which does not return a value handles all 
// groups correctly.
///////////////////////////////////////////////////////////////////////////////

class VisitorCounter : public metaf::Visitor<void> {
public:
	std::array<int, std::variant_size_v<metaf::Group>> count = {0};
protected:
	virtual void visitKeywordGroup(const metaf::KeywordGroup & group, 
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::KeywordGroup>()]++;
	}
	virtual void visitLocationGroup(const metaf::LocationGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::LocationGroup>()]++;
	}
	virtual void visitReportTimeGroup(const metaf::ReportTimeGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::ReportTimeGroup>()]++;
	}
	virtual void visitTrendGroup(const metaf::TrendGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::TrendGroup>()]++;
	}
	virtual void visitWindGroup(const metaf::WindGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::WindGroup>()]++;
	}
	virtual void visitVisibilityGroup(const metaf::VisibilityGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::VisibilityGroup>()]++;
	}
	virtual void visitCloudGroup(const metaf::CloudGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::CloudGroup>()]++;
	}
	virtual void visitWeatherGroup(const metaf::WeatherGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::WeatherGroup>()]++;
	}
	virtual void visitTemperatureGroup(const metaf::TemperatureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::TemperatureGroup>()]++;
	}
	virtual void visitPressureGroup(const metaf::PressureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::PressureGroup>()]++;
	}
	virtual void visitRunwayStateGroup(const metaf::RunwayStateGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::RunwayStateGroup>()]++;
	}
	virtual void visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::SeaSurfaceGroup>()]++;
	}
	virtual void visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::MinMaxTemperatureGroup>()]++;
	}
	virtual void visitPrecipitationGroup(const metaf::PrecipitationGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::PrecipitationGroup>()]++;
	}
	virtual void visitLayerForecastGroup(const metaf::LayerForecastGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::LayerForecastGroup>()]++;
	}
	virtual void visitPressureTendencyGroup(const metaf::PressureTendencyGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::PressureTendencyGroup>()]++;
	}
	virtual void visitCloudTypesGroup(const metaf::CloudTypesGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::CloudTypesGroup>()]++;
	}
	virtual void visitLowMidHighCloudGroup(const metaf::LowMidHighCloudGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::LowMidHighCloudGroup>()]++;
	}

	virtual void visitLightningGroup(const metaf::LightningGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::LightningGroup>()]++;
	}

	virtual void visitVicinityGroup(const metaf::VicinityGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::VicinityGroup>()]++;
	}

	virtual void visitMiscGroup(const metaf::MiscGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::MiscGroup>()]++;
	}

	virtual void visitUnknownGroup(const metaf::UnknownGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart; (void)rawString;
		count[variant_index<metaf::Group, metaf::UnknownGroup>()]++;
	}
};

TEST(Visitor, visitorVoid) {
	VisitorCounter v;
	for (const auto & report : testReports) {
		const auto result = metaf::Parser::parse(report);
			for (const auto & groupInfo : result.groups) {
				v.visit(groupInfo);
			}
	}

	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::KeywordGroup>()]), 21);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::LocationGroup>()]), 11);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::ReportTimeGroup>()]), 11);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::TrendGroup>()]), 3);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::WindGroup>()]), 14);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::VisibilityGroup>()]), 18);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::CloudGroup>()]), 28);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::WeatherGroup>()]), 9);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::TemperatureGroup>()]), 13);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::PressureGroup>()]), 16);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::RunwayStateGroup>()]), 1);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::SeaSurfaceGroup>()]), 1);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::MinMaxTemperatureGroup>()]), 2);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::PrecipitationGroup>()]), 2);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::LayerForecastGroup>()]), 2);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::PressureTendencyGroup>()]), 2);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::CloudTypesGroup>()]), 1);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::LowMidHighCloudGroup>()]), 1);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::LightningGroup>()]), 1);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::VicinityGroup>()]), 1);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::MiscGroup>()]), 2);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::UnknownGroup>()]), 3);
}

///////////////////////////////////////////////////////////////////////////////
// Visitor which returns a value
// Purpose: to confirm that Visitor which returns a value handles all groups 
// correctly.
///////////////////////////////////////////////////////////////////////////////

class VisitorIndex : public metaf::Visitor<std::string> {
protected:
	virtual std::string visitKeywordGroup(const metaf::KeywordGroup & group, 
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("keyword:"s + rawString);
	}
	virtual std::string visitLocationGroup(const metaf::LocationGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("location:"s + rawString);
	}
	virtual std::string visitReportTimeGroup(const metaf::ReportTimeGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("reporttime:"s + rawString);
	}
	virtual std::string visitTrendGroup(const metaf::TrendGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("trend:"s + rawString);
	}
	virtual std::string visitWindGroup(const metaf::WindGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("wind:"s + rawString);
	}
	virtual std::string visitVisibilityGroup(const metaf::VisibilityGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("visibility:"s + rawString);
	}
	virtual std::string visitCloudGroup(const metaf::CloudGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("cloud:"s + rawString);
	}
	virtual std::string visitWeatherGroup(const metaf::WeatherGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("weather:"s + rawString);
	}
	virtual std::string visitTemperatureGroup(const metaf::TemperatureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("temperature:"s + rawString);
	}
	virtual std::string visitPressureGroup(const metaf::PressureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("pressure:"s + rawString);
	}
	virtual std::string visitRunwayStateGroup(const metaf::RunwayStateGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("runwaystate:"s + rawString);
	}
	virtual std::string visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("seasurface:"s + rawString);
	}
	virtual std::string visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("minmaxtemperature:"s + rawString);
	}
	virtual std::string visitPrecipitationGroup(const metaf::PrecipitationGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("precipitation:"s + rawString);
	}
	virtual std::string visitLayerForecastGroup(const metaf::LayerForecastGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("layerforecast:"s + rawString);
	}
	virtual std::string visitPressureTendencyGroup(const metaf::PressureTendencyGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("pressuretendency:"s + rawString);
	}
	virtual std::string visitCloudTypesGroup(const metaf::CloudTypesGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("cloudtypes:"s + rawString);
	}
	virtual std::string visitLowMidHighCloudGroup(const metaf::LowMidHighCloudGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("lowmidhighcloud:"s + rawString);
	}

	virtual std::string visitLightningGroup(const metaf::LightningGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("lightning:"s + rawString);
	}

	virtual std::string visitVicinityGroup(const metaf::VicinityGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("vicinity:"s + rawString);
	}

	virtual std::string visitMiscGroup(const metaf::MiscGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("misc:"s + rawString);
	}

	virtual std::string visitUnknownGroup(const metaf::UnknownGroup & group,
		metaf::ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("unknown:"s + rawString);
	}
};

TEST(Visitor, visitorNonVoid) {
	VisitorIndex v;

	std::vector<std::string> result;

	for (const auto & report : testReports) {
		std::string r;
		const auto parseResult = metaf::Parser::parse(report);
			for (const auto & groupInfo : parseResult.groups) {
				r += v.visit(groupInfo);
				r += " ";
			}
		result.push_back(std::move(r));
	}

	ASSERT_EQ(result.size(), 11u);
	EXPECT_EQ(result.at(0),
		"keyword:TAF "
		"location:BGTL "
		"reporttime:060900Z "
		"trend:0609/0715 "
		"wind:VRB06KT "
		"visibility:8000 "
		"weather:-SHRASN "
		"cloud:OVC003 "
		"layerforecast:610105 "
		"pressure:QNH2980INS "
		"trend:BECMG 0614/0615 "
		"wind:VRB06KT "
		"visibility:9999 "
		"weather:NSW "
		"cloud:BKN005 "
		"cloud:OVC015 "
		"layerforecast:620105 "
		"pressure:QNH2983INS "
		"minmaxtemperature:TX03/0620Z TNM02/0709Z "s
	);
	EXPECT_EQ(result.at(1),
		"keyword:METAR "
		"location:EGYD "
		"reporttime:281050Z "
		"wind:11015KT "
		"visibility:5000 "
		"weather:-SHSN "
		"cloud:FEW004 "
		"cloud:BKN012CB "
		"cloud:BKN025 "
		"temperature:M04/M05 "
		"pressure:Q1020 "
		"misc:BLACKGRN "s
	);
	EXPECT_EQ(result.at(2),
		"keyword:METAR "
		"location:PABR "
		"reporttime:091953Z "
		"wind:28006KT "
		"visibility:3/4SM "
		"visibility:R07/2000V4500FT "
		"weather:BR "
		"cloud:OVC003 "
		"temperature:06/06 "
		"pressure:A3006 "
		"keyword:RMK "
		"pressure:SLP179 "
		"visibility:VIS 1/2V1 "
		"temperature:T00560056 "
		"unknown:VIA AUTODIAL "s
	);
	EXPECT_EQ(result.at(3),
		"keyword:METAR "
		"location:CYHI "
		"reporttime:171900Z "
		"misc:CCA "
		"wind:34019KT "
		"visibility:10SM "
		"weather:-SN DRSN VCSH VCFG "
		"cloud:FEW011 "
		"cloud:BKN030 "
		"cloud:BKN040 "
		"cloud:BKN060 "
		"cloud:BKN150 "
		"temperature:M05/M09 "
		"pressure:A2994 "
		"keyword:RMK "
		"cloudtypes:SC1ST4SC1SC1AC1 "
		"unknown:SC TR AC TR CIG RAG "
		"visibility:VIS SW 15 "
		"pressure:SLP144 "s
	);
	EXPECT_EQ(result.at(4),
		"keyword:SPECI "
		"location:UKLL "
		"reporttime:202304Z "
		"wind:00000MPS "
		"visibility:4800 "
		"visibility:1100SE "
		"visibility:R31/1900 "
		"weather:BR "
		"cloud:NSC "
		"temperature:11/10 "
		"pressure:Q1020 "
		"runwaystate:R31/CLRD// "
		"trend:TEMPO "
		"visibility:0400 "
		"weather:FG "
		"cloud:VV002 "s
	);
	EXPECT_EQ(result.at(5),
		"keyword:METAR "
		"location:SKBG "
		"reporttime:232200Z "
		"wind:31006KT "
		"visibility:9999 "
		"cloud:FEW015 "
		"temperature:26/19 "
		"pressure:A2989 "
		"wind:WS R35 "s
	);
	EXPECT_EQ(result.at(6),
		"keyword:METAR "
		"location:YPKG "
		"reporttime:232300Z "
		"keyword:AUTO "
		"wind:26013KT "
		"visibility:9999 "
		"weather:// "
		"cloud:NCD "
		"temperature:17/10 "
		"pressure:Q1010 "
		"precipitation:RF00.0////./ "s
	);
	EXPECT_EQ(result.at(7),
		"keyword:METAR "
		"location:ENSE "
		"reporttime:201620Z "
		"keyword:AUTO "
		"wind:24040KT "
		"visibility:9999NDV "
		"cloud:FEW010/// "
		"cloud:SCT020/// "
		"cloud:BKN027/// "
		"temperature:12/10 "
		"pressure:Q0988 "
		"seasurface:W///S7 "s
	);
	EXPECT_EQ(result.at(8),
		"keyword:SPECI "
		"location:MMMM "
		"reporttime:171315Z "
		"wind:00000KT "
		"visibility:5SM "
		"weather:VCFG "
		"cloud:SCT008 "
		"cloud:BKN080 "
		"cloud:BKN230 "
		"temperature:15/15 "
		"pressure:A3023 "
		"keyword:RMK "
		"lowmidhighcloud:8/578 "
		"unknown:HZY "s
	);
	EXPECT_EQ(result.at(9),
		"keyword:METAR "
		"location:EGUL "
		"reporttime:211156Z "
		"wind:25014G22KT "
		"visibility:9999 "
		"cloud:FEW046 "
		"cloud:SCT055 "
		"temperature:14/04 "
		"pressure:A2971 "
		"keyword:RMK "
		"keyword:AO2A "
		"wind:PK WND 24029/06 "
		"pressure:SLP064 "
		"precipitation:70021 "
		"temperature:T01370043 "
		"minmaxtemperature:10145 20103 "
		"pressuretendency:52016 "
		"keyword:$ "s
	);
	EXPECT_EQ(result.at(10),
		"keyword:METAR "
		"location:KABQ "
		"reporttime:092052Z "
		"wind:23004KT "
		"visibility:10SM "
		"cloud:FEW080 "
		"cloud:FEW120 "
		"cloud:FEW180 "
		"cloud:SCT300 "
		"temperature:29/07 "
		"pressure:A3005 "
		"keyword:RMK "
		"keyword:AO2 "
		"lightning:LTG DSNT SE "
		"pressure:SLP085 "
		"vicinity:CB DSNT N-NE AND SE "
		"temperature:T02940067 "
		"pressuretendency:58025 "s
	);
}

///////////////////////////////////////////////////////////////////////////////
// Invalid group passed to visitors
// Purpose: to confirm that passing an invalid group to Visitor::visit handles
// this situation correctly: no virtual method for groups is called, and for
// value-returned visitor, a default value of this type is returned 
///////////////////////////////////////////////////////////////////////////////

// Initialises valueless std::variant Group, based on example from 
// https://en.cppreference.com/w/cpp/utility/variant/valueless_by_exception
metaf::Group valuelessGroup() {
	struct S {
	    operator metaf::KeywordGroup() { throw 42; }
	};
	metaf::Group group = metaf::UnknownGroup();
	try {
		group.emplace<0>(S());
	}
	catch(...) {}
	return group;
}

const metaf::GroupInfo groupInfoInvalid(
	valuelessGroup(), 
	metaf::ReportPart::METAR,
	"TEST");

TEST(VisitorInvalidGroup, visitorVoid) {
	VisitorCounter v;
	ASSERT_TRUE(groupInfoInvalid.group.valueless_by_exception());
	v.visit(groupInfoInvalid);

	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::KeywordGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::LocationGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::ReportTimeGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::TrendGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::WindGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::VisibilityGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::CloudGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::WeatherGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::TemperatureGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::PressureGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::RunwayStateGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::SeaSurfaceGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::MinMaxTemperatureGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::PrecipitationGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::LayerForecastGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::PressureTendencyGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::CloudTypesGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::LowMidHighCloudGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::LightningGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::VicinityGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::MiscGroup>()]), 0);
	EXPECT_EQ((v.count[variant_index<metaf::Group, metaf::UnknownGroup>()]), 0);
}

TEST(VisitorInvalidGroup, visitorNonVoid) {
	VisitorIndex v;
	ASSERT_TRUE(groupInfoInvalid.group.valueless_by_exception());
	const std::string result = v.visit(groupInfoInvalid);
	EXPECT_TRUE(result.empty());
}
