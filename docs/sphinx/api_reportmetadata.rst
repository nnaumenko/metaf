ReportMetadata
==============

.. cpp:namespace-push:: metaf

.. cpp:struct:: ReportMetadata

	Stores information related to entire report rather than individual groups.

	.. cpp:var:: ReportType type

		Contains report type (:cpp:enumerator:`metaf::ReportType::METAR` or :cpp:enumerator:`metaf::ReportType::TAF`) autodetected by parser during parse of the last report. :cpp:enumerator:`metaf::ReportType::UNKNOWN` is used if the report is malformed and it is not possible to autodetect its type.

	.. cpp:var:: Error error
	
		Contains syntax error encountered by parser during parsing or :cpp:enumerator:`metaf::Error::NONE` if the report was parsed successfully.

	.. cpp:var:: std::optional<MetafTime> reportTime

		Report release time, acquired from :cpp:class:`ReportTimeGroup` or empty optional if :cpp:class:`ReportTimeGroup` is not present in the report.

	.. cpp:var:: bool isSpeci

		``true`` if report begins with group ``SPECI``, and ``false`` otherwise.

	.. cpp:var:: bool isNospeci

		``true`` if ``NOSPECI`` group is found in the report remarks, and ``false`` otherwise.

	.. cpp:var:: bool isAutomated

		``true`` if ``AUTO`` group is found in the report, and ``false`` otherwise.

	.. cpp:var:: bool isAo1

		``true`` if ``AO1`` group is found in the report remarks, and ``false`` otherwise.

	.. cpp:var:: bool isAo1a

		``true`` if ``AO1A`` group is found in the report remarks, and ``false`` otherwise.

	.. cpp:var:: bool isAo2

		``true`` if ``AO2`` group is found in the report remarks, and ``false`` otherwise.

	.. cpp:var:: bool isAo2a

		``true`` if ``AO2A`` group is found in the report remarks, and ``false`` otherwise.

	.. cpp:var:: bool isNil

		``true`` if ``NIL`` group is found in the report, and ``false`` otherwise.

	.. cpp:var:: bool isCancelled

		``true`` if ``CNL`` group is found in the report, and ``false`` otherwise.

	.. cpp:var:: bool isAmended

		``true`` if ``AMD`` group is found in the report, and ``false`` otherwise.

	.. cpp:var:: bool isCorrectional

		``true`` if ``COR`` group or group such as ``CCA``, ``CCB``, ``CCC``, etc. is found in the report; ``false`` otherwise.

	.. cpp:var:: std::optional<unsigned int> correctionNumber

		If group such as ``CCA``, ``CCB``, ``CCC``, etc. is found in the report, this field has value of a correctional report number (1 for ``CCA``, 2 for ``CCB``, etc.). If these groups are not presented in the report, this field has value 0.

	.. cpp:var:: bool maintenanceIndicator

		``true`` if ``$`` group is found in the report, and ``false`` otherwise.

	.. cpp:var:: std::optional<MetafTime> timeSpanFrom

		For TAF reports contains time from which the TAF report is valid.

		For METAR and SPECI reports contains an empty ``std::optional``.

	.. cpp:var:: std::optional<MetafTime> timeSpanUntil

		For TAF reports contains time until which the TAF report is valid.

		For METAR and SPECI reports contains an empty ``std::optional``.
