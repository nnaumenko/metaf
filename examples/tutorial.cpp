#include "metaf.hpp"

using namespace metaf;

#include <iostream>

std::string report = 
	"KDDC 112052Z AUTO 19023G34KT 7SM CLR 33/16 A2992"
	" RMK AO2 PK WND 20038/2033 SLP096 T03330156 58018";

std::string errorMessage(ReportError error) {
	switch (error) {
		case ReportError::NONE:
		return "no error, parsed succesfully";

		case ReportError::EMPTY_REPORT:
		return "empty report";

		case ReportError::EXPECTED_REPORT_TYPE_OR_LOCATION:
		return "expected report type or ICAO location";

		case ReportError::EXPECTED_LOCATION:
		return "expected ICAO location";

		case ReportError::EXPECTED_REPORT_TIME:
		return "expected report time";

		case ReportError::EXPECTED_TIME_SPAN:
		return "expected time span";

		case ReportError::UNEXPECTED_REPORT_END:
		return "unexpected report end";

		case ReportError::UNEXPECTED_GROUP_AFTER_NIL:
		return "unexpected group after NIL";

		case ReportError::UNEXPECTED_GROUP_AFTER_CNL:
		return "unexpected group after CNL";

		case ReportError::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY:
		return "unexpected NIL or CNL in report body";

		case ReportError::AMD_ALLOWED_IN_TAF_ONLY:
		return "AMD is allowed in TAF only";

		case ReportError::CNL_ALLOWED_IN_TAF_ONLY:
		return "CNL is allowed in TAF only";

		case ReportError::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY:
		return "Maintenance indicator is allowed only in METAR reports";

		case ReportError::REPORT_TOO_LARGE:
		return "Report has too many groups and may be malformed";
	}
}

std::string reportTypeMessage(ReportType reportType) {
	switch (reportType) {
		case ReportType::UNKNOWN:
		return "unable to detect";
		
		case ReportType::METAR:
		return "METAR";
		
		case ReportType::TAF:
		return "TAF";
	}
}

class MyVisitor : public Visitor<std::string> {
	virtual std::string visitKeywordGroup(
		const KeywordGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;  
		return ("Keyword: " + rawString);
	}

	virtual std::string visitLocationGroup(
		const LocationGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("ICAO location: " + rawString);
	}

	virtual std::string visitReportTimeGroup(
		const ReportTimeGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Report Release Time: " + rawString);
	}

	virtual std::string visitTrendGroup(
		const TrendGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Trend Header: " + rawString);
	}

	virtual std::string visitWindGroup(
		const WindGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Wind: " + rawString);
	}

	virtual std::string visitVisibilityGroup(
		const VisibilityGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Visibility: " + rawString);
	}

	virtual std::string visitCloudGroup(
		const CloudGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Cloud Data: " + rawString);
	}

	virtual std::string visitWeatherGroup(
		const WeatherGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Weather Phenomena: " + rawString);
	}

	virtual std::string visitTemperatureGroup(
		const TemperatureGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Temperature and Dew Point: " + rawString);
	}

	virtual std::string visitPressureGroup(
		const PressureGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Pressure: " + rawString);
	}

	virtual std::string visitRunwayStateGroup(
		const RunwayStateGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("State of Runway:" + rawString);
	}

	virtual std::string visitSeaSurfaceGroup(
		const SeaSurfaceGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Sea Surface: " + rawString);
	}

	virtual std::string visitMinMaxTemperatureGroup(
		const MinMaxTemperatureGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Minimum/Maximum Temperature: " + rawString);
	}

	virtual std::string visitPrecipitationGroup(
		const PrecipitationGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Precipitation: " + rawString);
	}

	virtual std::string visitLayerForecastGroup(
		const LayerForecastGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Atmospheric Layer Forecast: " + rawString);
	}

	virtual std::string visitPressureTendencyGroup(
		const PressureTendencyGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Pressure Tendency: " + rawString);
	}

	virtual std::string visitCloudTypesGroup(
		const CloudTypesGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Cloud Types: " + rawString);
	}

	virtual std::string visitLowMidHighCloudGroup(
		const LowMidHighCloudGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Low, middle, and high cloud layers: " + rawString);
	}

	virtual std::string visitLightningGroup(
		const LightningGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Lightning data: " + rawString);
	}

	virtual std::string visitVicinityGroup(
		const VicinityGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Events in vicinity: " + rawString);
	}

	virtual std::string visitMiscGroup(
		const MiscGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Miscellaneous Data: " + rawString);
	}

	virtual std::string visitUnknownGroup(
		const UnknownGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Not recognised by the parser: " + rawString);
	}
};

int main(int argc, char ** argv) {
	(void) argc; (void) argv;
	std::cout << "Parsing report: " << report << "\n";
	const auto result = Parser::parse(report);
	std::cout << "Parse error: "; 
	std::cout << errorMessage(result.reportMetadata.error) << "\n";	
	std::cout << "Detected report type: "; 
	std::cout << reportTypeMessage(result.reportMetadata.type) << "\n";
	std::cout << result.groups.size() << " groups parsed\n";
	MyVisitor visitor;
	for (const auto & groupInfo : result.groups) {
		std::cout << visitor.visit(groupInfo) << "\n";
	}
	return 0;	
}
