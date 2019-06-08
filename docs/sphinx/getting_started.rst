.. index:: single: Metaf; Tutorial

.. index:: single: Tutorial

Getting started
===============

.. highlight:: c

This section is organised as a tutorial to demonstrate the basic usage of Metaf library. We start with a simple cpp file and proceed step-by-step to integrate Metaf library, to parse a METAR or TAF report, to check if any error occurred during parsing and to add a basic handling of the parse results.

Setting up
----------

The tutorial uses a single file ``tutorial.cpp``. 

We start with the following: ::

	#include <iostream>

	// This is the report to parse
	const std::string report = "METAR ENNE 160150Z AUTO 26067G81KT 3600NDV BR BKN024/// 06/01 Q0987 W///S7";


	int main(int argc, char ** argv) {
		(void) argc; (void) argv;
		return(0);	
	}

We need iostream to print to ``std::cout``. Since we are not going to use ``argc`` and ``argv`` we cast them to ``void`` to suppress the compiler warnings.

Integrating Metaf
-----------------

To integrate Metaf library into your project, copy file ``src/metaf.h`` to the project's source or include path and add the following line to the beginning of the ``tutorial.cpp`` ::

	#include "metaf.h"

.. note:: All classes and functions in Metaf library are located in namespace metaf.


Parsing a report
----------------

Edit function ``main()`` as follows: ::

	int main(int argc, char ** argv) {
		(void) argc; (void) argv;
		std::cout << "Parsing report: " << report << "\n";
		const auto result = metaf::Parser::parse(report);
	}

Here we use static method :cpp:func:`metaf::Parser::parse()` which parses a METAR or TAF report and returns the result of the parsing.

If the report is malformed and an error occurrs during parsing the parser stops; in this case the report is parsed partially and the result of the parsing contains only the groups parsed before error was encountered.


Getting the result of parsing
-----------------------------

Add the following function before ``main()``: ::

	std::string reportTypeMessage(metaf::ReportType reportType) {
		switch (reportType) {
			case metaf::ReportType::UNKNOWN:
			return("unable to detect");
			
			case metaf::ReportType::METAR:
			return("METAR");

			case metaf::ReportType::TAF:
			return("TAF");
		}
	}

Then add the following lines to the function ``main()`` before ``return(0);``::

	std::cout << "Detected report type: " << reportTypeMessage(result.reportType) << "\n";
	std::cout << result.groups.size() << " groups\n";

:cpp:enum:`metaf::ReportType` is an enum for a report type autodetected by parser. :cpp:enumerator:`metaf::ReportType::UNKNOWN` is used when the parser is unable to detect the report type (e.g. due to malformed report).

:cpp:var:`metaf::Parser::Result::reportType` contains an autodetected type of the report.

:cpp:var:`metaf::Parser::Result::groups` is ``vector`` of :cpp:type:`metaf::Group`. Accordingly, :cpp:type:`metaf::Group` is an ``std::variant`` which holds all concrete group classes as variant alternatives.


Checking for errors
-------------------

Add the following function before ``main()``: ::

	std::string errorMessage(metaf::Parser::Error error) {
		switch (error) {
			case metaf::Parser::Error::NONE:
			return("no error");

			case metaf::Parser::Error::EMPTY_REPORT:
			return("empty report");
					
			case metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION:
			return("expected report type or ICAO location");
					
			case metaf::Parser::Error::EXPECTED_LOCATION:
			return("expected ICAO location");
					
			case metaf::Parser::Error::EXPECTED_REPORT_TIME:
			return("expected report time");
					
			case metaf::Parser::Error::EXPECTED_TIME_SPAN:
			return("expected time span");
					
			case metaf::Parser::Error::UNEXPECTED_REPORT_END:
			return("unexpected report end");
					
			case metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL:
			return("unexpected group after NIL");
					
			case metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_CNL:
			return("unexpected group after CNL");

			case metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_MAINTENANCE_INDICATOR:
			return("unexpected group after maintenance indicator");
					
			case metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY:
			return("unexpected NIL or CNL in report body");
					
			case metaf::Parser::Error::AMD_ALLOWED_IN_TAF_ONLY:
			return("AMD is allowed in TAF only");
					
			case metaf::Parser::Error::CNL_ALLOWED_IN_TAF_ONLY:
			return("CNL is allowed in TAF only");
					
			case metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY:
			return("Maintenance indicator is allowed only in METAR reports");
					
			case metaf::Parser::Error::INTERNAL_PARSER_STATE:
			return("internal error, unknown parser state");
		}
	}

Then add the following line to the function ``main()`` before ``return(0);``::

	if (result.error != metaf::Parser::Error::NONE) {
		std::cout << "Detected error: " << errorMessage(result.error) << "\n";	
	}

:cpp:var:`metaf::Parser::Result::error` contains an error that occurred during parsing of the report or :cpp:enumerator:`metaf::Parser::Error::NONE` if there was no error during parsing.


Handling the results of parsing
-------------------------------

Since the METAR or TAF report is parsed into the vector of type :cpp:type:`metaf::Group`, and the :cpp:type:`metaf::Group` is a variant data type, to handle the results one needs to check which variant alternative :cpp:type:`metaf::Group` holds, then get the correct type from variant and then handle the data.

The GroupVisitor is there to check the type and call a method for handling this type of group.

Add to the ``tutorial.cpp`` file a class that inherits :cpp:class:`GroupVisitor`: ::

	class MyVisitor : public metaf::GroupVisitor<std::string> {
	};

Since MyVisitor is inherited from ``GroupVisitor<std::string>``, this means that group handling methods return std::string. Also ``GroupVisitor<void>`` can be used if group handling methods do not return a value.

Now add to class MyVisitor the group handling methods (to keep it simple we just print the type of group here; to avoid unused parameter warnings we cast parameters to ``void``): ::

	virtual std::string visitPlainTextGroup(const metaf::PlainTextGroup & group) {
		(void)group; return("PlainTextGroup");
	}
	virtual std::string visitFixedGroup(const metaf::FixedGroup & group) {
		(void)group; return("FixedTextGroup");
	}
	virtual std::string visitLocationGroup(const metaf::LocationGroup & group) {
		(void)group; return("LocationGroup");
	}
	virtual std::string visitReportTimeGroup(const metaf::ReportTimeGroup & group) {
		(void)group; return("ReportTimeGroup");		
	}
	virtual std::string visitTrendGroup(const metaf::TrendGroup & group) {
		(void)group; return("TrendGroup");
	}
	virtual std::string visitWindGroup(const metaf::WindGroup & group) {
		(void)group; return("WindGroup");
	}
	virtual std::string visitVisibilityGroup(const metaf::VisibilityGroup & group) {
		(void)group; return("VisibilityGroup");
	}
	virtual std::string visitCloudGroup(const metaf::CloudGroup & group) {
		(void)group; return("CloudGroup");
	}
	virtual std::string visitWeatherGroup(const metaf::WeatherGroup & group) {
		(void)group; return("WeatherGroup");
	}
	virtual std::string visitTemperatureGroup(const metaf::TemperatureGroup & group) {
		(void)group; return("TemperatureGroup");
	}
	virtual std::string visitTemperatureForecastGroup(const metaf::TemperatureForecastGroup & group) {
		(void)group; return("TempreatureForecastGroup");
	}
	virtual std::string visitPressureGroup(const metaf::PressureGroup & group) {
		(void)group; return("PressureGroup");
	}
	virtual std::string visitRunwayVisualRangeGroup(const metaf::RunwayVisualRangeGroup & group) {
		(void)group; return("RunwayVisualRangeGroup");
	}
	virtual std::string visitRunwayStateGroup(const metaf::RunwayStateGroup & group) {
		(void)group; return("RunwayStateGroup");
	}
	virtual std::string visitSecondaryLocationGroup(const metaf::SecondaryLocationGroup & group) {
		(void)group; return("SecondaryLocationGroup");
	}
	virtual std::string visitRainfallGroup(const metaf::RainfallGroup & group) {
		(void)group; return("RainfallGroup");
	}
	virtual std::string visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group) {
		(void)group; return("SeaSurfaceGroup");
	}
	virtual std::string visitColourCodeGroup(const metaf::ColourCodeGroup & group) {
		(void)group; return("ColourCodeGroup");
	}
	virtual std::string visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group) {
		(void)group; return("MinMaxTemperatureGroup");
	}
	virtual std::string visitPrecipitationGroup(const metaf::PrecipitationGroup & group) {
		(void)group; return("PrecipitationGroup");
	}
	virtual std::string visitLayerForecastGroup(const metaf::LayerForecastGroup & group) {
		(void)group; return("LayerForecastGroup");
	}
	virtual std::string visitPressureTendencyGroup(const metaf::PressureTendencyGroup & group) {
		(void)group; return("PressureTendencyGroup");
	}
	virtual std::string visitCloudTypesGroup(const metaf::CloudTypesGroup & group) {
		(void)group; return("CloudTypesGroup");
	}
	virtual std::string visitCloudLayersGroup(const metaf::CloudLayersGroup & group) {
		(void)group; return("CloudLayersGroup");
	}
	virtual std::string visitMiscGroup(const metaf::MiscGroup & group) {
		(void)group; return("MiscGroup");
	}
	virtual std::string visitOther(const metaf::Group & group) {
		(void)group; return("Unknown Group");
	}

Since all these virtual methods are pure in :cpp:class:`GroupVisitor` there is no risk that some group type would be ommitted; this will result in compilation error.

Now add the following lines to the function ``main()`` before ``return(0);``::

	for (const auto group : result.groups) {
		std::cout << "Group parsed: " << visitor.visit(group) << "\n";
	}

:cpp:func:`GroupVisitor::visit()` will check the alternative stored in Group variant and call the corresponding virtual method.

Conclusion
----------

At this point the file ``tutorial.cpp`` file looks like this: ::

	#include "metaf.h"

	#include <iostream>

	// This is the report to parse
	const std::string report = "METAR ENNE 160150Z AUTO 26067G81KT 3600NDV BR BKN024/// 06/01 Q0987 W///S7";

	std::string reportTypeMessage(metaf::ReportType reportType) {
		switch (reportType) {
			case metaf::ReportType::UNKNOWN:
			return("unable to detect");
			
			case metaf::ReportType::METAR:
			return("METAR");

			case metaf::ReportType::TAF:
			return("TAF");
		}
	}

	std::string errorMessage(metaf::Parser::Error error) {
		switch (error) {
			case metaf::Parser::Error::NONE:
			return("no error");

			case metaf::Parser::Error::EMPTY_REPORT:
			return("empty report");
					
			case metaf::Parser::Error::EXPECTED_REPORT_TYPE_OR_LOCATION:
			return("expected report type or ICAO location");
					
			case metaf::Parser::Error::EXPECTED_LOCATION:
			return("expected ICAO location");
					
			case metaf::Parser::Error::EXPECTED_REPORT_TIME:
			return("expected report time");
					
			case metaf::Parser::Error::EXPECTED_TIME_SPAN:
			return("expected time span");
					
			case metaf::Parser::Error::UNEXPECTED_REPORT_END:
			return("unexpected report end");
					
			case metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_NIL:
			return("unexpected group after NIL");
					
			case metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_CNL:
			return("unexpected group after CNL");

			case metaf::Parser::Error::UNEXPECTED_GROUP_AFTER_MAINTENANCE_INDICATOR:
			return("unexpected group after maintenance indicator");
					
			case metaf::Parser::Error::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY:
			return("unexpected NIL or CNL in report body");
					
			case metaf::Parser::Error::AMD_ALLOWED_IN_TAF_ONLY:
			return("AMD is allowed in TAF only");
					
			case metaf::Parser::Error::CNL_ALLOWED_IN_TAF_ONLY:
			return("CNL is allowed in TAF only");
					
			case metaf::Parser::Error::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY:
			return("Maintenance indicator is allowed only in METAR reports");
					
			case metaf::Parser::Error::INTERNAL_PARSER_STATE:
			return("internal error, unknown parser state");
		}
	}

	class MyVisitor : public metaf::GroupVisitor<std::string> {
		virtual std::string visitPlainTextGroup(const metaf::PlainTextGroup & group) {
			(void)group; return("PlainTextGroup");
		}
		virtual std::string visitFixedGroup(const metaf::FixedGroup & group) {
			(void)group; return("FixedTextGroup");
		}
		virtual std::string visitLocationGroup(const metaf::LocationGroup & group) {
			(void)group; return("LocationGroup");
		}
		virtual std::string visitReportTimeGroup(const metaf::ReportTimeGroup & group) {
			(void)group; return("ReportTimeGroup");		
		}
		virtual std::string visitTrendGroup(const metaf::TrendGroup & group) {
			(void)group; return("TrendGroup");
		}
		virtual std::string visitWindGroup(const metaf::WindGroup & group) {
			(void)group; return("WindGroup");
		}
		virtual std::string visitVisibilityGroup(const metaf::VisibilityGroup & group) {
			(void)group; return("VisibilityGroup");
		}
		virtual std::string visitCloudGroup(const metaf::CloudGroup & group) {
			(void)group; return("CloudGroup");
		}
		virtual std::string visitWeatherGroup(const metaf::WeatherGroup & group) {
			(void)group; return("WeatherGroup");
		}
		virtual std::string visitTemperatureGroup(const metaf::TemperatureGroup & group) {
			(void)group; return("TemperatureGroup");
		}
		virtual std::string visitTemperatureForecastGroup(const metaf::TemperatureForecastGroup & group) {
			(void)group; return("TempreatureForecastGroup");
		}
		virtual std::string visitPressureGroup(const metaf::PressureGroup & group) {
			(void)group; return("PressureGroup");
		}
		virtual std::string visitRunwayVisualRangeGroup(const metaf::RunwayVisualRangeGroup & group) {
			(void)group; return("RunwayVisualRangeGroup");
		}
		virtual std::string visitRunwayStateGroup(const metaf::RunwayStateGroup & group) {
			(void)group; return("RunwayStateGroup");
		}
		virtual std::string visitSecondaryLocationGroup(const metaf::SecondaryLocationGroup & group) {
			(void)group; return("SecondaryLocationGroup");
		}
		virtual std::string visitRainfallGroup(const metaf::RainfallGroup & group) {
			(void)group; return("RainfallGroup");
		}
		virtual std::string visitSeaSurfaceGroup(const metaf::SeaSurfaceGroup & group) {
			(void)group; return("SeaSurfaceGroup");
		}
		virtual std::string visitColourCodeGroup(const metaf::ColourCodeGroup & group) {
			(void)group; return("ColourCodeGroup");
		}
		virtual std::string visitMinMaxTemperatureGroup(const metaf::MinMaxTemperatureGroup & group) {
			(void)group; return("MinMaxTemperatureGroup");
		}
		virtual std::string visitPrecipitationGroup(const metaf::PrecipitationGroup & group) {
			(void)group; return("PrecipitationGroup");
		}
		virtual std::string visitLayerForecastGroup(const metaf::LayerForecastGroup & group) {
			(void)group; return("LayerForecastGroup");
		}
		virtual std::string visitPressureTendencyGroup(const metaf::PressureTendencyGroup & group) {
			(void)group; return("PressureTendencyGroup");
		}
		virtual std::string visitCloudTypesGroup(const metaf::CloudTypesGroup & group) {
			(void)group; return("CloudTypesGroup");
		}
		virtual std::string visitCloudLayersGroup(const metaf::CloudLayersGroup & group) {
			(void)group; return("CloudLayersGroup");
		}
		virtual std::string visitMiscGroup(const metaf::MiscGroup & group) {
			(void)group; return("MiscGroup");
		}
		virtual std::string visitOther(const metaf::Group & group) {
			(void)group; return("Unknown Group");
		}
	};

	int main(int argc, char ** argv) {
		(void) argc; (void) argv;
		std::cout << "Parsing report: " << report << "\n";
		const auto result = metaf::Parser::parse(report);
		std::cout << "Detected report type: " << reportTypeMessage(result.reportType) << "\n";
		std::cout << result.groups.size() << " groups\n";
		if (result.error != metaf::Parser::Error::NONE) {
			std::cout << "Detected error: " << errorMessage(result.error) << "\n";	
		}
		MyVisitor visitor;
		for (const auto group : result.groups) {
			std::cout << "Group parsed: " << visitor.visit(group) << "\n";
		}
		return(0);
	}

Compile it and run; it will print the following:

| Parsing report: METAR ENNE 160150Z AUTO 26067G81KT 3600NDV BR BKN024/// 06/01 Q0987 W///S7
| Detected report type: METAR
| 11 groups
| Group parsed: FixedTextGroup
| Group parsed: LocationGroup
| Group parsed: ReportTimeGroup
| Group parsed: FixedTextGroup
| Group parsed: WindGroup
| Group parsed: VisibilityGroup
| Group parsed: WeatherGroup
| Group parsed: CloudGroup
| Group parsed: TemperatureGroup
| Group parsed: PressureGroup
| Group parsed: SeaSurfaceGroup


Further reading
---------------

Please refer to the examples and reference for more information.