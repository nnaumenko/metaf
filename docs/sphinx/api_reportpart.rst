ReportPart
==========

.. cpp:namespace-push:: metaf

.. cpp:enum-class:: ReportPart

	Represents the major part of the report. Report is used by :cpp:class:`metaf::GroupParser` to  parse only those group types which can potentially occur in this report part (e.g. :cpp:class:`metaf::ReportTimeGroup` is only used in report headers and NOSIG is only used in METAR report body).

		.. cpp:enumerator:: UNKNOWN

			Unknown part of the report (e.g. after syntax error was encountered).

		.. cpp:enumerator:: HEADER

			METAR or TAF report header.

		.. cpp:enumerator:: METAR

			METAR report body and trends.

		.. cpp:enumerator:: TAF

			TAF report body and trends.

		.. cpp:enumerator:: RMK

			Remarks at the end of METAR or TAF report.
