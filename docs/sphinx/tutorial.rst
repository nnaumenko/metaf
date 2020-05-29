########
Tutorial
########

.. highlight:: c

.. cpp:namespace-push:: metaf

`Try it online <https://wandbox.org/permlink/qusrOPN4FSki9g1T>`_.

This section is a tutorial to demonstrate the basic usage of Metaf library.

*************************
Creating tutorial program
*************************

We start with a simple cpp file and proceed step-by-step to integrate Metaf library, to parse a METAR or TAF report, to check if any error occurred during parsing and to add a basic handling of the parse results.

At the end of the tutorial we get a minimalistic yet functional program which parses METAR or TAF reports.

Setting up
==========

Before we start actual implementation of the tutorial, we get the repository locally and create a template .cpp file which eventually turns into a functional program.


Getting Metaf repository
------------------------

Type the following in the command line to get the Metaf repository ::

	git clone https://gitlab.com/nnaumenko/metaf.git
	cd metaf


Template .cpp file
------------------

The tutorial will use a single file ``tutorial.cpp``.

We start with the following simple template: ::

	#include <iostream>

	int main(int argc, char ** argv) {
		(void) argc; (void) argv;
		return 0;	
	}

We need iostream to print to ``std::cout``. Since we are not going to use ``argc`` and ``argv`` we cast them to ``void`` to suppress the compiler warnings.



Parsing a report
================

We are ready to integrate Metaf into our simple .cpp file and add code to parse the actual METAR report. 

Integrating Metaf
-----------------

To integrate Metaf library into your project, just add the include path to directory ``include`` (alternatively just copy file ``include/metaf.hpp`` from repository to your project directory). Then add the following line to the beginning of the ``tutorial.cpp`` ::

	#include "metaf.hpp"

	using namespace metaf;

All classes and functions in Metaf library are located in namespace ``metaf``, and we added ``using namespace metaf`` so that we do not have to write ``metaf::`` before every single class type from the Metaf library.


Adding report to parse
----------------------

Add the following after the line ``using namespace metaf;`` ::

	std::string report = 
		"KDDC 112052Z AUTO 19023G34KT 7SM CLR 33/16 A2992"
		" RMK AO2 PK WND 20038/2033 SLP096 T03330156 58018";

This is our METAR report to parse. Please feel free to enter any other METAR, SPECI or TAF report to test Metaf capabilities.


Using Metaf parser to parse a report
------------------------------------

Add the following to ``main()`` function before ``return 0``: ::

		std::cout << "Parsing report: " << report << "\n";
		const auto result = Parser::parse(report);

Metaf has a single static method :cpp:func:`Parser::parse()` which parses a string with a METAR or TAF report and returns the result of the parsing.

If the report is malformed and a syntax error occurrs during parsing, then the parser stops where it found an error; in such case the report is parsed only partially and the result of the parsing contains only the groups parsed before error was encountered. Metaf does not throw exceptions and uses error codes instead.

The groups which are not recognised by parser do not cause parser to stop and report error as soon as general report syntax is valid; these groups are included in the parsing result as a group of type :cpp:class:`UnknownGroup`.

This is it with parsing! The rest of the tutorial is about interpreting the results.


Interpreting the results
========================

Now after we added some code which parses METAR/TAF report, we can proceed with interpreting the results of the parsing. We start with data related to the entire report and later move on to handling individual METAR/TAF groups.

ParseResult
-----------

The result of the parsing is a struct of type :cpp:class:`ParseResult`. It consists of two parts: metadata and report content.

The first part, metadata, are general data related to entire report. They are stored in the field :cpp:var:`ParseResult::reportMetadata`. The type of this field is :cpp:class:`ReportMetadata`.

Metadata are useful for sorting and filtering the parsed reports. They contain autodetected report type, error which occurred during the report parsing, report release time, ICAO location code for the station which produced the reports, and various report attributes.

The second part is the actual report content in a form of individual METAR and TAF groups that were included in the report, stored in the field :cpp:var:`ParseResult::groups`. The type of the field is ``std::vector`` of type :cpp:class:`GroupInfo`. Each element of the vector contains data stored in the METAR or TAF group (or, in some cases, sequence of groups).

In the tutorial below we will use both parts to identify report's type, parsing error, and the type of each individual group.


Error handling
--------------

Add the following function before ``main()``: ::

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

Then add the following to the function ``main()`` before ``return 0;``::

	std::cout << "Parse error: ";
	std::cout << errorMessage(result.reportMetadata.error) << "\n";

Metaf does not throw exceptions when it encounters the error. Instead the parser stops and error of type :cpp:enum:`ReportError` is returned in metadata.

Use :cpp:var:`ParseResult::reportMetadata::error` to get the error that occurred during parsing of the report. If the parsing was successful its value is :cpp:enumerator:`ReportError::NONE`. If the error occurred it contains one of the actual error values (see above).


Report type
-----------

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

Then add the following to the function ``main()`` before ``return 0;``::

	std::cout << "Detected report type: ";
	std::cout << reportTypeMessage(result.reportMetadata.type) << "\n";

Here we use :cpp:var:`ParseResult::reportMetadata::type` to get an autodetected type of the report.

Metaf automatically identifies the report type as :cpp:enumerator:`ReportType::METAR` or :cpp:enumerator:`ReportType::TAF`). Note that the sample report we have added above does not start with ``METAR``, ``SPECI`` or ``TAF``, but Metaf will be able to detect report type anyway.

SPECI reports are identified with :cpp:enumerator:`ReportType::METAR`. However SPECI reports may be easily singled out by checking the value of bool field :cpp:var:`ParseResult::reportMetadata::isSpeci`.

If Metaf is unable to detect the report type due to syntax error in the report, it identifies the report type as :cpp:enumerator:`ReportType::UNKNOWN`.


Handling groups
===============

After we handle the type of the parsed report, and have checked whether (and which) an error occured. Now we proceed to the actual report contents: METAR/TAF groups included in the report.

METAR/TAF report contents
-------------------------

The actual METAR/TAF report content is stored in :cpp:var:`ParseResult::groups`. This is a ``std::vector`` of :cpp:class:`GroupInfo`. Each element of the vector contains data stored in the METAR or TAF group (or, in some cases, sequence of groups).

:cpp:class:`GroupInfo` contains the following fields.

#. :cpp:var:`GroupInfo::group` of a variant type :cpp:type:`Group`. The information extracted by parser from the METAR or TAF group is stored here. 

#. :cpp:var:`GroupInfo::reportPart`. It identifies part of report, such as report header, report body, remarks, etc where the group belongs.

#. :cpp:var:`GroupInfo::rawString` of type ``std::string`` which contains the raw group string which was parsed by the report.

For example, parsing the first group of our sample report ``KDDC`` will result in :cpp:class:`GroupInfo` with the following content.

#. :cpp:var:`GroupInfo::group` has a value with variant type :cpp:type:`Group`, in this case it stores the alternative of :cpp:class:`LocationGroup` because this group represents ICAO location of the station which produced the report. Let's skip this topic for now; we will go into details in the next section.

#. :cpp:var:`metaf::GroupInfo::reportPart` has a value :cpp:enumerator:`ReportPart::HEADER`.

#. :cpp:var:`metaf::GroupInfo::rawString` of type ``std::string`` which contains the raw group string, in this case its value is ``KDDC``.

Add the following to the function ``main()`` before ``return 0;``::

	std::cout << result.groups.size() << " groups parsed\n";

This will display how many groups the parser found in our sample report.


Group variant
-------------

The information extracted by parser from the METAR or TAF group is stored in  :cpp:var:`GroupInfo::group` of type :cpp:type:`Group`.

:cpp:type:`Group` is an ``std::variant`` which can store many alternatives, such as :cpp:class:`KeywordGroup`, :cpp:class:`LocationGroup`, :cpp:class:`WindGroup`, :cpp:class:`VisibilityGroup`, etc.

Each alternative is associated with particular type of METAR and TAF groups, e.g. :cpp:class:`WindGroup` stores surface wind and wind shear information, :cpp:class:`VisibilityGroup` stores visibility and runway visual range information, :cpp:class:`WeatherGroup` stores recent or current or forecast weather phenomena information, :cpp:class:`PrecipitationGroup` stores information about rainfall, snow accumulation, ice accretion, etc.

For example, the first group of our sample report ``KDDC`` results in :cpp:type:`Group` which stores an alternative of :cpp:class:`LocationGroup` because KDDC is an ICAO code for the station which produced this report.

Handling of :cpp:type:`Group` is no different of any ``std::variant``. For example one can use ``std::holds_alternative`` to check the group type and then ``std::get`` to extract a concrete type from :cpp:type:`Group`. This handling can be simplified by using Visitor class.

In the next sections we will look into the details of :cpp:class:`Visitor` and add use it for interpreting the contents of our sample report.


Visitor class
-------------

The Visitor class is provided by Metaf library to simplify handling of :cpp:type:`Group` and :cpp:class:`GroupInfo`.

:cpp:class:`Visitor` has a number of virtual methods, one method for every alternative which can potentially be stored in variant type :cpp:type:`Group`. Pass a :cpp:type:`Group` to :cpp:func:`Visitor::visit`, and it will detect the type of the alternative stored in the :cpp:type:`Group` and call a particular virtual method for every alternative.

For example if an instance of :cpp:type:`Group` stores an alternative :cpp:class:`LocationGroup`, the visitor will call virtual method ``visitLocationGroup`` and pass an instance of :cpp:class:`LocationGroup` to this method.

Add the following before the function ``main()``: ::

	class MyVisitor : public Visitor<std::string> {
	};

Here our class MyVisitor is inherited from ``Visitor<std::string>``. Since we specified ``std::string`` for template, all virtual method of our ``MyVisitor`` class will return a ``std::string``. Alternatively we could use ``Visitor<void>`` if we needed a Visitor class that does not return the value.

Add the following lines to the function ``main()`` before ``return 0;``::

	MyVisitor visitor;
	for (const auto groupInfo : result.groups) {
		std::cout << visitor.visit(groupInfo) << "\n";
	}

:cpp:class:`Visitor` has two overloaded :cpp:func:`Visitor::visit()` methods. One of these methods takes :cpp:class:`metaf::GroupInfo` as a parameter, and other takes :cpp:type:`Group`, :cpp:enum:`ReportPart` and ``std::string`` with raw group string. In this report we use the first one.


Adding functionality to Visitor class
-------------------------------------

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

Each virtual method is very basic; they just return a description of the concrete type stored in :cpp:type:`Group` and a raw string that was parsed into this concrete type. To avoid compiler warnings it casts unused parameters to ``void``.

In real-life applications you want to check the fields of the concrete group classes and print or process their data; please check `examples section <https://nnaumenko.gitlab.io/nnaumenko/examples/>` for such usage.

Since all these virtual methods are pure in :cpp:class:`Visitor` there is no risk that some group type is not processed because this will result in compilation error.

Done!
=====

We have created a very basic yet functional program which uses Metaf library to parse METAR/TAF report and interprets results.


*********************
Compiling and running
*********************

At this point the file would look like `this <https://gitlab.com/nnaumenko/metaf/blob/master/examples/tutorial.cpp>`_.

Compile the file ``tutorial.cpp`` and run it.

`Or try it online <https://wandbox.org/permlink/5Sw5vEgPSkHkHKIa>`_.

The expected printout is as follows:

| Parsing report: KDDC 112052Z AUTO 19023G34KT 7SM CLR 33/16 A2992 RMK AO2 PK WND 20038/2033 SLP096 T03330156 58018
| Parse error: no error, parsed succesfully
| Detected report type: METAR
| 14 groups parsed
| ICAO location: KDDC
| Report Release Time: 112052Z
| Keyword: AUTO
| Wind: 19023G34KT
| Visibility: 7SM
| Cloud Data: CLR
| Temperature and Dew Point: 33/16
| Pressure: A2992
| Keyword: RMK
| Keyword: AO2
| Wind: PK WND 20038/2033
| Pressure: SLP096
| Temperature and Dew Point: T03330156
| Pressure Tendency: 58018


**********
Conclusion
**********

In this tutorial we created a minimalistic but working example which parses METAR and TAF reports using Metaf library. We parsed a report, checked for errors, determined the autodetected report type and used Visitor to process the report content.


***************
Further reading
***************

Please check the `Examples
<https://nnaumenko.gitlab.io/metaf/examples.html>`_ and :ref:`api:API Reference` for further details.
