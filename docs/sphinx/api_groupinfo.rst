GroupInfo
=========

.. cpp:namespace-push:: metaf

.. cpp:struct:: GroupInfo

	Contains data on the single METAR or TAF group processed by parser.

	.. cpp:var:: Group group

		Contains all information included in the METAR or TAR group which is recognised by parser.

	.. cpp:var:: ReportPart reportPart

		To which part of the report this group belongs (e.g. header, METAR or TAF report body, remarks).

	.. cpp:var:: std::string rawString

		METAR or TAF group source string which was parsed to extract info.
