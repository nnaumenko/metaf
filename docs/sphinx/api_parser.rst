Parser
======

.. cpp:namespace-push:: metaf

Parser class
------------

	.. cpp:class:: Parser

		Parser class is used to parse strings which contain raw METAR or TAF reports, check for syntax errors, autodetect report type and produce a vector of individual groups.

			.. cpp:function:: static ParseResult parse (const std::string & report, size_t groupLimit = 100)

				Parses a METAR or TAF report, checks its syntax, detects report type and parses each group separately.

				:returns: :cpp:class:`metaf::ParseResult` which contains autodetected type or METAR or TAF report, syntax error type (if occurred) and vector of individual :cpp:type:`metaf::GroupInfo` corresponding to METAR or TAF groups. If syntax error is encountered, this means that only the part of the METAR or TAF report before syntax error was parsed. See `Results of the parsing`_ for details.

				.. note:: If report is parsed successfully, it does not guarantee that all groups were recognised by the parser. Unrecognised groups are treated as Unknown Groups (see :cpp:class:`metaf::UnknownGroup`).

				:param report: String which contains a METAR or TAF report.

				:param groupLimit: Maximum number of the groups allowed in the report. Limiting the number of groups allows detecting large chunks of text/HTML/XML/JSON/etc. errorneously appended at the end of the METAR or TAF report. The default value of 100 is an arbitrarily set large number which would not prevent even large reports from being parsed, and generates error for malformed reports which are beyound reasonable size.

				.. note:: Presence of this parameter also guarantees that the parsing process cannot become an infinite loop in all cases.


Parsing rules and report syntax
-------------------------------

This section concentrates on METAR and TAF report syntax, how Metaf checks report validity and how exactly the reports are parsed by Metaf.


METAR and TAF report structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This section describes general structure of a METAR/TAF report assumed by Metaf parser, how Metaf determines the end of the report, and how Metaf splits METAR/TAF report into its individual groups.


Report end indicator
""""""""""""""""""""

Last group may have report end indicator (``=`` character). This character typically follows the last group without delimiter.

Before parsing group report end indicator is removed and its presence or absence is NOT noted in the results or report parsing or in the last group.

Any characters present in the report string after report end indicator will be ignored.

If report end indicator is not present, Metaf will parse entire report string unless it exceeds maximum number of groups (see also `Maximum report size`_).


Group delimiters
""""""""""""""""

Report's groups may be separated by any combination of spaces(20h), tabs(09h), line feed (0Ah), carriage return(0Dh).

For example, METAR report structured as follows will be parsed without errors: ::

	METAR
	 UKLL         111900Z
	 14001MPS	6000	NSC	15/14	Q1023	R13/CLRD//	NOSIG=


Group ``BLU+`` in practice may be followed by the next group without delimiter.

Due to this, Metaf treats ``+`` character as a delimiter when it is not in front of the group (e.g. in groups such as ``+TSRAGR``). 

For example consider report ``METAR ETHC 251720Z 18005KT 9999 BKN250 05/02 Q1018 BLU+FCST CNL=``, in this case parser will extract ``BLU+``, ``FCST`` and ``CNL`` groups.


Case-sensitivity
""""""""""""""""

Metaf parses report's groups case-sensitive and does not capitalise the report string. 

For example, parsing report ``mETAR UKLL 111900Z 14001MPS 6000 NSC 15/14 Q1023 R13/CLRD// NOSIG`` will result in error, while the same report in all capital letters ``METAR UKLL 111900Z 14001MPS 6000 NSC 15/14 Q1023 R13/CLRD// NOSIG`` will be parsed without errors.

Report ``METAR UKLL 111900Z 14001mps 6000 NSC 15/14 Q1023 R13/CLRD// NOSIG`` will be parsed without error but group ``14001mps`` will not be recognised as wind group.


Maximum report size
"""""""""""""""""""

It is possible to set a limit on the number of the groups in the report to filter out malformed reports and prevent infinite loops.

Default limit is maximum 100 groups per report.

If the limit is exceeded then error `ReportError::REPORT_TOO_LARGE` is generated.

Group number limit is set by the argument ``groupLimit`` of :cpp:func:`Parser::parse()`.



METAR and TAF report syntax
^^^^^^^^^^^^^^^^^^^^^^^^^^^

This section describes which report syntax is expected by Metaf parser, and what parser does when it encounters a syntax error in the report.


Typical METAR and TAF reports
"""""""""""""""""""""""""""""

Typical METAR report has the following syntax.

.. image:: metar.svg

Typical TAF report has the following syntax.

.. image:: taf.svg


Missing and cancelled reports
"""""""""""""""""""""""""""""

Some reports do not have any content but indicate missing (``NIL``) or cancelled (``CNL``) reports. The syntax for such reports is expected as follows.

.. image:: metar_nil.svg

.. image:: taf_nil_cnl.svg

.. image:: unknown_nil.svg

Missing reports are indicated by group ``NIL`` and cancelled reports are indicated by group ``CNL``. These reports do not have any content.

If any group is found following ``NIL`` or ``CNL``, error :cpp:enumerator:`ReportError::UNEXPECTED_GROUP_AFTER_NIL` or :cpp:enumerator:`ReportError::UNEXPECTED_GROUP_AFTER_CNL` is generated.

If ``NIL`` or ``CNL`` group is found in the report which does have content, error :cpp:enumerator:`ReportError::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY` is generated.

If group ``CNL`` is found in METAR or SPECI report, error :cpp:enumerator:`ReportError::CNL_ALLOWED_IN_TAF_ONLY` is generated.


Reports with syntax errors
""""""""""""""""""""""""""

Reports which do not follow the syntax above cause parser to stop and generate error (:cpp:enum:`ReportError`).

If an error is specified in report metadata (:cpp:var:`ReportMetadata::error` has value other than :cpp:enumerator:`ReportError::NONE`), only part of the report was parsed and parsing result does not contain all groups included in the report.


Report type detection
"""""""""""""""""""""

Metaf automatically detects report type as :cpp:enumerator:`ReportType::METAR` or :cpp:enumerator:`ReportType::TAF`.

If report type cannot be detected due to syntax error or report structure (for example, report such as ``ZZZZ NIL``), :cpp:enumerator:`ReportType::UNKNOWN` is used.

The following rules are used for detecting the report type:

 - If report starts with one of the groups ``METAR`` or ``SPECI`` then its type is :cpp:enumerator:`ReportType::METAR`.

 - If report start with group ``TAF`` then its type is :cpp:enumerator:`ReportType::TAF`.

 - If report starts with ICAO location group, Metaf parses the followsing group which is report release time group, and then checks the type of the group which follows report release time group. If time span group in format xxxx/xxxx (such as ``3023/3102``) follows, report type is :cpp:enumerator:`ReportType::TAF`, otherwise report type is :cpp:enumerator:`ReportType::METAR`.

Below are two examples illustrating the last rule.

Report ``ENNE 091100Z 0912/0917 11020KT 9999 FEW040`` does not start with ``METAR``, ``SPECI`` or ``TAF``, and its type cannot be detected right away. The first group is ``ENNE`` which is interpreted as an ICAO location. The second group is ``091100Z`` which is interpreted as report release time (day 9, time 11:00 GMT). The third group is ``0912/0917``, which fits time span format ``xxxx/xxxx``, and the type of the report is detected as :cpp:enumerator:`ReportType::TAF`.

Report ``MBPV 082000Z 13005KT 9999 SCT016 30/23 Q1011`` does not start with ``METAR``, ``SPECI`` or ``TAF``, and its type cannot be detected right away. The first group is ``MBPV`` which is interpreted as an ICAO location. The second group is ``082000Z`` which is interpreted as report release time (day 8, time 20:00 GMT). The third group is ``13005KT``, which does not fit time span format ``xxxx/xxxx``, and the type of the report is detected as :cpp:enumerator:`ReportType::METAR`.


Correctional, amended, and automated reports
""""""""""""""""""""""""""""""""""""""""""""

Metaf generates error :cpp:enumerator:`ReportError::EXPECTED_LOCATION` if any group other than ``COR`` or ``AMD`` is present between report type indicator (``METAR``, ``SPECI`` or ``TAF``) and ICAO location group. 

If group ``AMD`` is found in METAR or SPECI report, error :cpp:enumerator:`ReportError::AMD_ALLOWED_IN_TAF_ONLY` is generated.

If corectional / amended report indicators (``COR`` or ``AMD`` groups, or groups such as ``CCA``, ``CCB``, ``CCC``, etc.) are present in report body or trends, they can be present at any location within report body or trends; no error will be generated.

``AUTO`` group may be present anywhere within the report.


Maintenance indicator
"""""""""""""""""""""

Maintenance indicator (``$`` character) may be present anywhere in METAR report to indicate that automated station requires maintenance.

Typically it is the last group in the METAR report but it does not always happen in practice. Due to this, Metaf does not require maintenance indicator to be located at the end of the report.
	
When maintenance indicator is found in TAF report, error :cpp:enumerator:`ReportError::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY` is generated.



Results of the parsing
^^^^^^^^^^^^^^^^^^^^^^

This section describes general logic of the parsing results returned by parser.


Metadata
""""""""

The results of METAR or TAF report parsing include :cpp:class:`ReportMetadata`, which can be used to sort or filter reports easily.

Metadata contain information such as report type, parsing error (if any), ICAO location, report release time, and attributes.

Each of the attributes is set when the corresponding keyword is found in the report, for example keyword ``SPECI`` sets attribute :cpp:var:`ReportMetadata::isSpeci`.

See :cpp:class:`ReportMetadata` for detailed description.


Groups
""""""

All groups included in the report are represented by corresponding :cpp:type:`Group` which is a variant type. :cpp:type:`Group` holds one of its alternatives, and each alternative is a class which represents a particular kind of group, e.g. wind, visibility, weather phenomena, etc.

Typically, one group in METAR or TAF report results in one :cpp:type:`Group`.

One group in METAR or TAF report cannot result in multiple :cpp:type:`Group` in the results of parsing but in some cases multiple groups in the report result in a single :cpp:type:`Group`.

For example, group ``1`` in report, followed by group ``1/2SM`` represent integer and fraction parts of the same visibility value, and are represented by a single :cpp:type:`Group` which holds alternative :cpp:class:`VisibilityGroup`.

