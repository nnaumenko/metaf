ParseResult
===========

.. cpp:namespace-push:: metaf

.. cpp:struct:: ParseResult

	Contains result of report parsing returned by :cpp:func:`metaf::Parser::parse()` method.

	.. cpp:var:: ReportMetadata reportMetadata

		Contains information on entire report rather than any individual group.

	.. cpp:var:: std::vector<GroupInfo> groups

		A vector of parsed individual groups from METAR or TAF report.
