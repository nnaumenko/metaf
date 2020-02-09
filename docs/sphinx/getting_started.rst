.. index:: single: Metaf; Tutorial

.. index:: single: Tutorial

Getting started
===============

.. highlight:: c

This section is organised as a tutorial to demonstrate the basic usage of Metaf library. 

We start with a simple cpp file and proceed step-by-step to integrate Metaf library, to parse a METAR or TAF report, to check if any error occurred during parsing and to add a basic handling of the parse results.

At the end of the tutorial we get a minimalistic yet functional program which parses METAR or TAF reports.

Prerequisites
-------------

To compile the tutorial we need empscripten installed, to set it up please follow the `instructions 
<https://emscripten.org/docs/getting_started/downloads.html>`_.

The tutorial runs in the browser, so we need a browser recent enough to have WebAssembly support.

Get the Metaf repository ::

	git clone --recurse-submodules https://gitlab.com/nnaumenko/metaf.git
	cd metaf


Setting up
----------

The tutorial uses a single file ``tutorial.cpp``.

We start with the following simple template: ::

	#include <iostream>

	int main(int argc, char ** argv) {
		(void) argc; (void) argv;
		return(0);	
	}

We need iostream to print to ``std::cout``. Since we are not going to use ``argc`` and ``argv`` we cast them to ``void`` to suppress the compiler warnings.


Integrating Metaf
-----------------

To integrate Metaf library into your project, just add the include path to directory ``include``. Then add the following line to the beginning of the ``tutorial.cpp`` ::

	#include "metaf.hpp"

	using namespace metaf;

All classes and functions in Metaf library are located in namespace metaf, so we add ``using namespace metaf`` part for simplicity, so that we do not have to write ``metaf::`` before every single type from the library.


Adding report to parse
----------------------

Add the following after the line ``using namespace metaf;`` ::

	std::string report = 
	"KDDC 112052Z AUTO 19023G34KT 7SM CLR 33/16 A2992"
	" RMK AO2 PK WND 20038/2033 SLP096 T03330156 58018";


Parsing a report
----------------

Edit function ``main()`` as follows: ::

	int main(int argc, char ** argv) {
		(void) argc; (void) argv;
		std::cout << "Parsing report: " << report << "\n";
		const auto result = Parser::parse(report);
	}

Here we use static method :cpp:func:`metaf::Parser::parse()` which parses a METAR or TAF report and returns the result of the parsing (:cpp:class:`metaf::ParseResult`).

If the report is malformed and an error occurrs during parsing the parser stops; in such case the report is parsed partially and the result of the parsing contains only the groups parsed before error was encountered.

The groups which are not recognised by parser do not cause parser to stop and report error; they are just noted in the parsing result as :cpp:class:`metaf::UnknownGroup`.


Getting the result of parsing
-----------------------------

Add the following function before ``main()``: ::

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

Then add the following lines to the function ``main()`` before ``return(0);``::

	std::cout << "Detected report type: " 
		<< reportTypeMessage(result.reportMetadata.type) << "\n";
	std::cout << result.groups.size() << " groups parsed\n";

Report parsing result consists of two big parts: metadata and groups.

Metadata are related to entire report, rather than individual groups. They contain autodetected report type, error which occurred during the report parsing, etc. See :cpp:class:`metaf::ReportMetadata` for details.

:cpp:enum:`metaf::ReportType` is an enum for a report type autodetected by parser. :cpp:enumerator:`metaf::ReportType::UNKNOWN` is used when the parser is unable to detect the report type (e.g. due to malformed report).

:cpp:var:`metaf::ReportMetadata::type` contains an autodetected type of the report.

:cpp:var:`metaf::ParseResult::groups` is ``vector`` of struct named :cpp:class:`metaf::GroupInfo`. This struct contains three fields.

1. :cpp:var:`metaf::GroupInfo::group` of type :cpp:type:`metaf::Group`. It contains the information extracted from the METAR or TAF group during parsing. :cpp:type:`metaf::Group` is ``std::variant`` which holds all concrete group classes as variant alternatives. For simplicity we will not use :cpp:type:`metaf::Group` in this tutorial.

#. :cpp:var:`metaf::GroupInfo::reportPart`. It contains a part of report (report header, report body, remarks, etc.). The type is :cpp:enum:`metaf::ReportPart`. We do not use it in this tutorial either.

#. :cpp:var:`metaf::GroupInfo::rawString`. This is the ``std::string`` that contains the group source string from the report. If parser appends two or more logically related groups, their raw strings are also appended.


Checking for errors
-------------------

Add the following function before ``main()``: ::

	std::string errorMessage(ReportError error) {
		switch (error) {
			case ReportError::NONE:
			return "no error";

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

			case ReportError::UNEXPECTED_GROUP_AFTER_MAINTENANCE_INDICATOR:
			return "unexpected group after maintenance indicator";

			case ReportError::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY:
			return "unexpected NIL or CNL in report body";

			case ReportError::AMD_ALLOWED_IN_TAF_ONLY:
			return "AMD is allowed in TAF only";

			case ReportError::CNL_ALLOWED_IN_TAF_ONLY:
			return "CNL is allowed in TAF only";

			case ReportError::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY:
			return "Maintenance indicator is allowed only in METAR reports";
		}
	}

Then add the following line to the function ``main()`` before ``return(0);``::

	if (result.reportMetadata.error != ReportError::NONE) {
		std::cout << "Detected error: " << 
			errorMessage(result.reportMetadata.error) << "\n";
	}

:cpp:var:`metaf::ParseResult::reportMetadata::error` contains an error that occurred during parsing of the report. If there was no error during parsing it is equal to :cpp:enumerator:`metaf::ReportError::NONE`.


Handling the results of parsing
-------------------------------

Since the METAR or TAF report is parsed into the vector of type :cpp:class:`metaf::GroupInfo`, and the :cpp:type:`metaf::Group` is a variant data type, to handle the results one needs to check which variant alternative :cpp:var:`metaf::GroupInfo::group` holds, then get the correct type from variant and then handle the data.

The :cpp:class:`Visitor` is there to help you check the type and call a correct method for handling this type of group.

Add to the ``tutorial.cpp`` file a class that inherits :cpp:class:`Visitor`: ::

	class MyVisitor : public Visitor<std::string> {
	};

Since MyVisitor is inherited from ``Visitor<std::string>``, this means that all methods for group handling will return std::string. Also ``Visitor<void>`` can be used if group handling methods do not return a value.

Now add to class MyVisitor the following group handling methods ::

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

	virtual std::string visitCloudLayersGroup(
		const CloudLayersGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Cloud Layers: " + rawString);
	}

	virtual std::string visitLightningGroup(
		const LightningGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Lightning data: " + rawString);
	}

	virtual std::string visitWeatherBeginEndGroup(
		const WeatherBeginEndGroup & group,
		ReportPart reportPart,
		const std::string & rawString)
	{
		(void)group; (void)reportPart;
		return ("Weather phenomena beginning and ending: " + rawString);
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

We just print the type of group and its raw string here. To avoid unused parameter warnings we cast parameters to ``void``). In more complex applications you want to check the fields of the groups and print or process their data; check examples section for such usage.

Since all these virtual methods are pure in :cpp:class:`Visitor` there is no risk that some group type would be ommitted; this will result in compilation error.

Now we can use ``MyVisitor`` to process all groups in the ``vector``. Add the following lines to the function ``main()`` before ``return(0);``::

	MyVisitor visitor;
	for (const auto groupInfo : result.groups) {
		std::cout << visitor.visit(groupInfo) << "\n";
	}

:cpp:func:`Visitor::visit()` will check the alternative stored in Group variant and call the corresponding virtual method.

For flexibility there are two :cpp:func:`Visitor::visit()` methods. One of them takes :cpp:class:`metaf::GroupInfo` as a parameter, and other takes group, reportPart and rawString. In this tutorial we only use the former.

At this point the file would look like `this <https://gitlab.com/nnaumenko/metaf/blob/master/examples/tutorial.cpp>`_.


Compiling and running
---------------------

Compile the file ``tutorial.cpp`` as follows ::

	emcc tutorial.cpp -o tutorial.html

Then run it ::

	emrun tutorial.html

The expected printout is as follows:

| Parsing report: KDDC 112052Z AUTO 19023G34KT 7SM CLR 33/16 A2992 RMK AO2 PK WND 20038/2033 SLP096 T03330156 58018
| Detected report type: METAR
| 14 groups parsed
| ICAO location: KDDC
| Report Release Time: 112052Z
| Fixed Text: AUTO
| Wind: 19023G34KT
| Visibility: 7SM
| Cloud Data: CLR
| Temperature and Dew Point: 33/16
| Pressure: A2992
| Fixed Text: RMK
| Fixed Text: AO2
| Wind: PK WND 20038/2033
| Pressure: SLP096
| Temperature and Dew Point: T03330156
| Pressure Tendency: 58018


Conclusion
----------

In this tutorial we created a minimalistic working example which parses METAR and TAF reports using Metaf library.


Further reading
---------------

Please refer to the `Examples
<https://nnaumenko.gitlab.io/metaf/examples.html>`_ and `Documentation <https://nnaumenko.gitlab.io/metaf/docs/index.html>`_ for details.