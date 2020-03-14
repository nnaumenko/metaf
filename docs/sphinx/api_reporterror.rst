ReportError
===========

.. cpp:namespace-push:: metaf

.. cpp:enum-class:: ReportError

	Errors which may occur when parsing a METAR or TAF report.

	.. cpp:enumerator:: NONE

		No error, report parsed successfully.

		.. note: This enumerator only means that the report overall syntax is correct and the report is not malformed. It does not guarantee that all groups were recognised by the parser. Unrecognised groups are treated as Plain Text Groups (see :cpp:class:`metaf::PlainTextGroup`).

	.. cpp:enumerator:: EMPTY_REPORT

		The report source string is empty or contains only report end designator ``=``.

	.. cpp:enumerator:: EXPECTED_REPORT_TYPE_OR_LOCATION

		The parser is expecting the report to start with either report type or ICAO location but the report starts with some other group.

		At the beginning of the report the report type should be specified as follows:
		
			- METAR or SPECI: the report type is METAR.
			- TAF: the report type is TAF.

		However in practice this report type in both METARs and TAFs may be missing.

		If the report type is missing then the report must begin with the ICAO location group (see :cpp:class:`metaf::LocationGroup`).

		AMD or COR groups are not allowed at the beginning of the report (i.e. if report type is missing).

	.. cpp:enumerator:: EXPECTED_LOCATION

		The parser expects an ICAO location group (see :cpp:class:`metaf::LocationGroup`) in this position but encounters some other group.

	.. cpp:enumerator:: EXPECTED_REPORT_TIME

		The parser expects a report release time group (see :cpp:class:`metaf::ReportTimeGroup`) in this position but encounters some other group.

	.. cpp:enumerator:: EXPECTED_TIME_SPAN

		The parser expects a time span group (see :cpp:class:`metaf::TrendGroup` and :cpp:enumerator:`metaf::TrendGroup::Type::TIME_SPAN`) in this position but encounters some other group.

		This error occurs when the validity time is not specified for the TAF report.

	.. cpp:enumerator:: UNEXPECTED_REPORT_END

		The report should not end at this position, more groups are expected according to the report format but actually are missing. This error occurs if either stray report end designator (``=``) is placed in the middle of the report or if only part of the report is included in the source string.

	.. cpp:enumerator:: UNEXPECTED_GROUP_AFTER_NIL

		This error occurs if any group is encountered after NIL. (see :cpp:enumerator:`metaf::KeywordGroup::NIL`).

		.. note: NIL means missing report and must not be included in non-empty report.

	.. cpp:enumerator:: UNEXPECTED_GROUP_AFTER_CNL

		This error occurs if any group is encountered after CNL. (see :cpp:enumerator:`metaf::KeywordGroup::CNL`).

		.. note: CNL means canceled report and must not be included in non-empty report.

	.. cpp:enumerator:: UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY

		This error occurs if NIL or CNL are found in the middle of non-empty reports (see :cpp:enumerator:`metaf::KeywordGroup::NIL` and :cpp:enumerator:`metaf::KeywordGroup::CNL`).

		.. note: NIL means missing report and CNL means canceled report; these groups must not be included is the report which contains any actual observation or forecast.

	.. cpp:enumerator:: AMD_ALLOWED_IN_TAF_ONLY

		Group AMD which designates amended report (see :cpp:enumerator:`metaf::KeywordGroup::AMD`) is only used in TAF reports. This error occurs if AMD is encountered in a METAR report.

		.. note: COR (see :cpp:enumerator:`metaf::KeywordGroup::COR`) may be used in both METAR and TAF reports.

	.. cpp:enumerator:: CNL_ALLOWED_IN_TAF_ONLY

		Group CNL which designates canceled report (see :cpp:enumerator:`metaf::KeywordGroup::CNL`) is only used in TAF reports. 

		Since METAR reports contain the actual weather observation, canceling a METAR report is a semantic error.

		This error occurs if CNL is encountered in a METAR report in place of NIL.

	.. cpp:enumerator:: MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY

		Maintenance indicator is used for weather reports produced by automated station and cannot be included in TAF.

		This error occurs if maintenance indicator ($) is encountered in a TAF report.

	.. cpp:enumerator:: REPORT_TOO_LARGE

		Too many groups included in the report. Group number limit is specified in the argument ``groupLimit`` of :cpp:func:`Parser::parse()`.

