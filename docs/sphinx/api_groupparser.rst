GroupParser
===========

.. cpp:namespace-push:: metaf

.. cpp:class:: GroupParser

	The purpose of this class is to parse a single METAR or TAF group. To parse entire METAR or TAF report use :cpp:class:`metaf::Parser`.

	.. cpp:function:: static Group parse(const std::string & group, ReportPart reportPart, const ReportGlobalData & reportData)

		:param group: Source string which contains a single METAR or TAF group.

		:param reportPart: Report part to which the source string belongs.

		:param reportData: Information related to the entire report rather than individual group. ``metaf::noReportData`` may be used for parsing individual groups if such information is not needed.

		:returns: :cpp:type:`metaf::Group` holding a particular group type or :cpp:class:`metaf::PlainTextGroup` if the format was not recognised.
