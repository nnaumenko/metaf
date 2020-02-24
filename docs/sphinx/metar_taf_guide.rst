METAR and TAF parsing guide
===========================

.. cpp:namespace-push:: metaf

This section concentrates on METAR and TAF report syntax, how Metaf checks report validity, various METAR/TAF groups and how exactly they are parsed by Metaf.

METAR and TAF reports
---------------------

Report syntax
^^^^^^^^^^^^^

When parsing a report, the following syntax is expected. 

.. image:: metar.svg

.. image:: taf.svg

Some reports do not have any content but indicate missing (``NIL``) or cancelled (``CNL``) reports. The syntax for such reports is expected as follows.

.. image:: metar_nil.svg

.. image:: taf_nil_cnl.svg

.. image:: unknown_nil.svg

Reports which do not follow the syntax above cause parser to stop and generate error (:cpp:enum:`ReportError`).


Groups
^^^^^^

All groups included in the report are represented by corresponding :cpp:type:`Group` which is a variant type. :cpp:type:`Group` holds one of its alternatives, and each alternative is a class which represents a particular kind of group, e.g. wind, visibility, weather phenomena, etc.

Typically, one group in METAR or TAF report results in one :cpp:type:`Group`.

One group in METAR or TAF report cannot result in multiple :cpp:type:`Group` in the results of parsing but in some cases multiple groups in the report result in a single :cpp:type:`Group`.

For example, group ``1`` in report, followed by group ``1/2SM`` represent integer and fraction parts of the same visibility value, and are represented by a single :cpp:type:`Group` which holds alternative :cpp:class:`VisibilityGroup`.


Group delimiters
^^^^^^^^^^^^^^^^

Report's groups may be separated by any combination of spaces(20h), tabs(09h), line feed (0Ah), carriage return(0Dh).

For example, METAR report structured as follows will be parsed without errors: ::

	METAR
	 UKLL         111900Z
	 14001MPS	6000	NSC	15/14	Q1023	R13/CLRD//	NOSIG=


Case-sensitivity
^^^^^^^^^^^^^^^^

Metaf parses report's groups case-sensitive and does not capitalise the report string. 

For example, parsing report ``mETAR UKLL 111900Z 14001MPS 6000 NSC 15/14 Q1023 R13/CLRD// NOSIG`` will result in error, while the same report in all capital letters ``METAR UKLL 111900Z 14001MPS 6000 NSC 15/14 Q1023 R13/CLRD// NOSIG`` will be parsed without errors.

Report ``METAR UKLL 111900Z 14001mps 6000 NSC 15/14 Q1023 R13/CLRD// NOSIG`` will be parsed without error but group ``14001mps`` will not be recognised as wind group.


Report end indicator
^^^^^^^^^^^^^^^^^^^^

Last group may have report end indicator (= character). This character typically follows the last group without delimiter.

Before parsing group report end indicator is removed and its presence or absence is NOT noted in the results or report parsing or in the last group.

Any characters present in the report string after report end indicator will be ignored.


Report type detection
^^^^^^^^^^^^^^^^^^^^^

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
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Metaf generates error :cpp:enumerator:`ReportError::EXPECTED_LOCATION` if any group other than ``COR`` or ``AMD`` is present between report type indicator (``METAR``, ``SPECI`` or ``TAF``) and ICAO location group. 

If group ``AMD`` is found in METAR or SPECI report, error :cpp:enumerator:`ReportError::AMD_ALLOWED_IN_TAF_ONLY` is generated.

If corectional / amended report indicators (``COR`` or ``AMD`` groups, or groups such as ``CCA``, ``CCB``, ``CCC``, etc.) are present in report body or trends, they can be present at any location within report body or trends; no error will be generated.

``AUTO`` group may be present anywhere within the report.


Missing and cancelled reports
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Missing reports are indicated by group ``NIL`` and cancelled reports are indicated by group ``CNL``. These reports do not have any content.

If any group is found following ``NIL`` or ``CNL``, error :cpp:enumerator:`ReportError::UNEXPECTED_GROUP_AFTER_NIL` or :cpp:enumerator:`ReportError::UNEXPECTED_GROUP_AFTER_CNL` is generated.

If ``NIL`` or ``CNL`` group is found in the report which does have content, error :cpp:enumerator:`ReportError::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY` is generated.

If group ``CNL`` is found in METAR or SPECI report, error :cpp:enumerator:`ReportError::CNL_ALLOWED_IN_TAF_ONLY` is generated.


Maintenance indicator
^^^^^^^^^^^^^^^^^^^^^

Maintenance indicator ($ character) may be present anywhere in METAR report to indicate that automated station requires maintenance.

Typically it is the last group in the METAR report but it does not always happen in practice. Due to this, Metaf does not require maintenance indicator to be located at the end of the report.
	
When maintenance indicator is found in TAF report, error :cpp:enumerator:`ReportError::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY` is generated.


Maximum report size
^^^^^^^^^^^^^^^^^^^

It is possible to set a limit on the number of the groups in the report to filter out malformed reports and prevent infinite loops.

Default limit is maximum 100 groups per report.

If the limit is exceeded then error `ReportError::REPORT_TOO_LARGE` is generated.

Group number limit is set by the argument ``groupLimit`` of :cpp:func:`Parser::parse()`.


Metadata
^^^^^^^^

The results of METAR or TAF report parsing include :cpp:class:`ReportMetadata`, which can be used to sort or filter reports easily.

Metadata contain information such as report type, parsing error (if any), ICAO location, report release time, and attributes.

Each of the attributes is set when the corresponding keyword is found in the report, for example keyword ``SPECI`` sets attribute :cpp:var:`ReportMetadata::isSpeci`.

See :cpp:class:`ReportMetadata` for detailed description.


Keyword groups
--------------

.. cpp:namespace-push:: KeywordGroup

Standard keywords such as ``METAR``, ``SPECI``, ``TAF``, ``COR``, ``RMK``, ``AUTO``, ``CAVOK``, etc. are represented by class :cpp:class:`KeywordGroup`.

Every single keyword is interpreted as a corresponding type of KeywordGroup.

Keywords are recognised as such only in (:cpp:enum:`ReportPart`) where they are expected to occur, for example if keyword ``AUTO`` is encountered in TAF report, it will not be recognised as a keyword because ``AUTO`` indicates automated METAR report and makes no sense in TAF.

The table below provides the summary of :cpp:enum:`ReportPart` where each keyword is recognised.

======= ==================================== =================================== ================================= =================================
Keyword :cpp:enumerator:`ReportPart::HEADER` :cpp:enumerator:`ReportPart::METAR` :cpp:enumerator:`ReportPart::TAF` :cpp:enumerator:`ReportPart::RMK`
======= ==================================== =================================== ================================= =================================
METAR   ✔                                    ✘                                   ✘                                 ✘                                
SPECI   ✔                                    ✘                                   ✘                                 ✘                                
TAF     ✔                                    ✘                                   ✘                                 ✘                                
AMD     ✔                                    ✘                                   ✘                                 ✘                                
NIL     ✔                                    ✔                                   ✔                                 ✘                                
CNL     ✔                                    ✔                                   ✔                                 ✘                                
COR     ✔                                    ✔                                   ✘                                 ✘                                
AUTO    ✘                                    ✔                                   ✘                                 ✘                                
CAVOK   ✘                                    ✔                                   ✔                                 ✘                                
RMK     ✔                                    ✔                                   ✔                                 ✘                                
$       ✔                                    ✔                                   ✔                                 ✔                                
AO1     ✘                                    ✘                                   ✘                                 ✔                                
AO2     ✘                                    ✘                                   ✘                                 ✔                                
AO1A    ✘                                    ✘                                   ✘                                 ✔                                
AO2A    ✘                                    ✘                                   ✘                                 ✔                                
NOSPECI ✘                                    ✘                                   ✘                                 ✔                                
======= ==================================== =================================== ================================= =================================

Class :cpp:class:`KeywordGroup` has a single getter method :cpp:func:`type()`. The table below summarises the keyword groups and their types returned by :cpp:func:`type()`.

======= =============================================
Keyword :cpp:func:`type()`                           
======= =============================================
METAR   :cpp:enumerator:`Type::METAR`                
SPECI   :cpp:enumerator:`Type::SPECI`                
TAF     :cpp:enumerator:`Type::TAF`                  
AMD     :cpp:enumerator:`Type::AMD`                  
NIL     :cpp:enumerator:`Type::NIL`                  
CNL     :cpp:enumerator:`Type::CNL`                  
COR     :cpp:enumerator:`Type::COR`                  
AUTO    :cpp:enumerator:`Type::AUTO`                 
CAVOK   :cpp:enumerator:`Type::CAVOK`                
RMK     :cpp:enumerator:`Type::RMK`                  
$       :cpp:enumerator:`Type::MAINTENANCE_INDICATOR`
AO1     :cpp:enumerator:`Type::AO1`                  
AO2     :cpp:enumerator:`Type::AO2`                  
AO1A    :cpp:enumerator:`Type::AO1A`                 
AO2A    :cpp:enumerator:`Type::AO2A`                 
NOSPECI :cpp:enumerator:`Type::NOSPECI`              
======= =============================================

.. cpp:namespace-pop::



ICAO location groups
--------------------

.. cpp:namespace-push:: LocationGroup

.. image:: locationgroup.svg

ICAO location groups are represented by class :cpp:class:`LocationGroup`.

The format of this group is four characters, and contains only uppercase letters and digits. First character must not be a digit.

.. image:: locationgroup.svg

This group is recognised only when found in report header.

For example groups ``UKLL`` or ``K2J3`` are interpreted as LocationGroup, while groups ``2ABC`` or ``ZZZ$`` are not.

Method :cpp:func:`toString()` returns string with ICAO location code stored in this group.

For example, for group ``UKLL`` represented by :cpp:class:`LocationGroup`, :cpp:func:`toString()` will return std::string with value "UKLL".

.. cpp:namespace-pop::



Report release time group
-------------------------

.. cpp:namespace-push:: ReportTimeGroup

Report release time groups are represented by class :cpp:class:`ReportTimeGroup`.

The format of this group is six digits, followed by Z.

.. image:: reporttimegroup.svg

The first two digits are day-of-month, followed by two digits of hour, followed by two digits of minute, followed by character 'Z'.

Report release time group is recognised only when found in report header.

Method :cpp:func:`time()` to returns the instance of :cpp:class:`MetafTime` which contains day-of-month, hour, and minute.

For example, for group ``301545Z`` represented by :cpp:class:`ReportTimeGroup`, :cpp:func:`time()` will return an instance of cpp:class:`MetafTime` where :cpp:func:`MetafTime::day()` returns a non-empty std::optional<unsigned int> with value 30, :cpp:func:`MetafTime::hour()` returns 15, and :cpp:func:`MetafTime::minute()` returns 45.

.. cpp:namespace-pop::



Trend group
-----------

.. cpp:namespace-push:: TrendGroup

A variety of weather trend type and time groups are represented by class :cpp:class:`TrendGroup`:

- METAR trend type 'no significant changes expected'
- METAR and TAF time span
- TAF probability groups, typically followed by trend type or time span groups
- TAF trend 'from'
- Trend types 'becoming', 'temporary' or 'intermediary' with or without probability,  and with or without time or time span
- METAR trends 'from', 'until', 'at' and their combinations


METAR trends
^^^^^^^^^^^^

Generalised syntax of METAR trend type and time groups recognised by Metaf is below. 

.. image:: trendgroup_metar.svg

The table below summarises various formats of weather trend type and time groups recognised by Metaf in METAR reports. The table shows values returned by getter methods of :cpp:class:`TrendGroup`.

+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+---------------------------------------------------+
|Group                 |:cpp:func:`type()`               |:cpp:func:`probability()`            |:cpp:func:`timeFrom()`                             |:cpp:func:`timeUntil()`                            |:cpp:func:`timeAt()`                               |
+======================+=================================+=====================================+===================================================+===================================================+===================================================+
|NOSIG                 |:cpp:enumerator:`Type::NOSIG`    |:cpp:enumerator:`Probability::NONE`  |empty std::optional<:cpp:class:`MetafTime`>        |empty std::optional<:cpp:class:`MetafTime`>        |empty std::optional<:cpp:class:`MetafTime`>        |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|BECMG                 |:cpp:enumerator:`Type::BECMG`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|TEMPO                 |:cpp:enumerator:`Type::TEMPO`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|INTER                 |:cpp:enumerator:`Type::INTER`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+---------------------------------------------------+
|BECMG FM1245          |:cpp:enumerator:`Type::BECMG`    |:cpp:enumerator:`Probability::NONE`  |- std::optional<:cpp:class:`MetafTime`>            |empty std::optional<:cpp:class:`MetafTime`>        |empty std::optional<:cpp:class:`MetafTime`>        |
+----------------------+---------------------------------+                                     |- :cpp:func:`MetafTime::day()`.has_value() == false|                                                   |                                                   |
|TEMPO FM1245          |:cpp:enumerator:`Type::TEMPO`    |                                     |- :cpp:func:`MetafTime::hour()` == 12              |                                                   |                                                   |
+----------------------+---------------------------------+                                     |- :cpp:func:`MetafTime::minute()` == 45            |                                                   |                                                   |
|INTER FM1245          |:cpp:enumerator:`Type::INTER`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|FM1245                |:cpp:enumerator:`Type::FROM`     |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+---------------------------------------------------+
|BECMG TL0930          |:cpp:enumerator:`Type::BECMG`    |:cpp:enumerator:`Probability::NONE`  |empty std::optional<:cpp:class:`MetafTime`>        |- std::optional<:cpp:class:`MetafTime`>            |empty std::optional<:cpp:class:`MetafTime`>        |
+----------------------+---------------------------------+                                     |                                                   |- :cpp:func:`MetafTime::day()`.has_value() == false|                                                   |
|TEMPO TL0930          |:cpp:enumerator:`Type::TEMPO`    |                                     |                                                   |- :cpp:func:`MetafTime::hour()` == 9               |                                                   |
+----------------------+---------------------------------+                                     |                                                   |- :cpp:func:`MetafTime::minute()` == 30            |                                                   |
|INTER TL0930          |:cpp:enumerator:`Type::INTER`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|TL0930                |:cpp:enumerator:`Type::UNTIL`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+---------------------------------------------------+
|BECMG AT2000          |:cpp:enumerator:`Type::BECMG`    |:cpp:enumerator:`Probability::NONE`  |empty std::optional<:cpp:class:`MetafTime`>        |empty std::optional<:cpp:class:`MetafTime`>        |- std::optional<:cpp:class:`MetafTime`>            |
+----------------------+---------------------------------+                                     |                                                   |                                                   |- :cpp:func:`MetafTime::day()`.has_value() == false|
|TEMPO AT2000          |:cpp:enumerator:`Type::TEMPO`    |                                     |                                                   |                                                   |- :cpp:func:`MetafTime::hour()` == 20              |
+----------------------+---------------------------------+                                     |                                                   |                                                   |- :cpp:func:`MetafTime::minute()` == 0             |
|INTER AT2000          |:cpp:enumerator:`Type::INTER`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|AT2000                |:cpp:enumerator:`Type::AT`       |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+---------------------------------------------------+
|BECMG FM2215 TL2230   |:cpp:enumerator:`Type::BECMG`    |:cpp:enumerator:`Probability::NONE`  |- std::optional<:cpp:class:`MetafTime`>            |- std::optional<:cpp:class:`MetafTime`>            |empty std::optional<:cpp:class:`MetafTime`>        |
+----------------------+---------------------------------+                                     |- :cpp:func:`MetafTime::day()`.has_value() == false|- :cpp:func:`MetafTime::day()`.has_value() == false|                                                   |
|TEMPO FM2215 TL2230   |:cpp:enumerator:`Type::TEMPO`    |                                     |- :cpp:func:`MetafTime::hour()` == 22              |- :cpp:func:`MetafTime::hour()` == 22              |                                                   |
+----------------------+---------------------------------+                                     |- :cpp:func:`MetafTime::minute()` == 15            |- :cpp:func:`MetafTime::minute()` == 30            |                                                   |
|INTER FM2215 TL2230   |:cpp:enumerator:`Type::INTER`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|FM2215 TL2230         |:cpp:enumerator:`Type::FROM`     |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|BECMG 2215/2230       |:cpp:enumerator:`Type::BECMG`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|TEMPO 2215/2230       |:cpp:enumerator:`Type::TEMPO`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|INTER 2215/2230       |:cpp:enumerator:`Type::INTER`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|2215/2230             |:cpp:enumerator:`Type::TIME_SPAN`|                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+---------------------------------------------------+


TAF trends
^^^^^^^^^^

Generalised syntax of TAF trend type and time groups recognised by Metaf is below. 

.. image:: trendgroup_taf.svg

The table below summarises various formats of weather trend type and time groups recognised by Metaf in TAF reports. The table shows values returned by getter methods of :cpp:class:`TrendGroup`.

+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+---------------------------------------------------+
|Group                 |:cpp:func:`type()`               |:cpp:func:`probability()`            |:cpp:func:`timeFrom()`                             |:cpp:func:`timeUntil()`                            |:cpp:func:`timeAt()`                               |
+======================+=================================+=====================================+===================================================+===================================================+===================================================+
|FM291230              |:cpp:enumerator:`Type::FROM`     |:cpp:enumerator:`Probability::NONE`  |- std::optional<:cpp:class:`MetafTime`>            |empty std::optional<:cpp:class:`MetafTime`>        |empty std::optional<:cpp:class:`MetafTime`>        |
|                      |                                 |                                     |- :cpp:func:`MetafTime::day()`.value() == 29       |                                                   |                                                   |
|                      |                                 |                                     |- :cpp:func:`MetafTime::hour()` == 12              |                                                   |                                                   |
|                      |                                 |                                     |- :cpp:func:`MetafTime::minute()` == 30            |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+                                                   |
|BECMG 2812/2818       |:cpp:enumerator:`Type::BECMG`    |:cpp:enumerator:`Probability::NONE`  |- std::optional<:cpp:class:`MetafTime`>            |- std::optional<:cpp:class:`MetafTime`>            |                                                   |
+----------------------+---------------------------------+                                     |- :cpp:func:`MetafTime::day()`.value() == 29       |- :cpp:func:`MetafTime::day()`.value() == 29       |                                                   |
|TEMPO 2812/2818       |:cpp:enumerator:`Type::TEMPO`    |                                     |- :cpp:func:`MetafTime::hour()` == 12              |- :cpp:func:`MetafTime::hour()` == 12              |                                                   |
+----------------------+---------------------------------+                                     |- :cpp:func:`MetafTime::minute()` == 30            |- :cpp:func:`MetafTime::minute()` == 30            |                                                   |
|INTER 2812/2818       |:cpp:enumerator:`Type::INTER`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|2812/2818             |:cpp:enumerator:`Type::TIME_SPAN`|                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+                                                   |                                                   |                                                   |
|PROB30 TEMPO 2812/2818|:cpp:enumerator:`Type::TEMPO`    |:cpp:enumerator:`Probability::PROB30`|                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|PROB30 INTER 2812/2818|:cpp:enumerator:`Type::INTER`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|PROB30 2812/2818      |:cpp:enumerator:`Type::TIME_SPAN`|                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+                                                   |                                                   |                                                   |
|PROB40 TEMPO 2812/2818|:cpp:enumerator:`Type::TEMPO`    |:cpp:enumerator:`Probability::PROB40`|                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|PROB40 INTER 2812/2818|:cpp:enumerator:`Type::INTER`    |                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+                                     |                                                   |                                                   |                                                   |
|PROB40 2812/2818      |:cpp:enumerator:`Type::TIME_SPAN`|                                     |                                                   |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+                                                   |
|PROB30                |:cpp:enumerator:`Type::PROB`     |:cpp:enumerator:`Probability::PROB30`|empty std::optional<:cpp:class:`MetafTime`>        |empty std::optional<:cpp:class:`MetafTime`>        |                                                   |
+----------------------+---------------------------------+-------------------------------------|                                                   |                                                   |                                                   |
|PROB40                |:cpp:enumerator:`Type::PROB`     |:cpp:enumerator:`Probability::PROB40`|                                                   |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+---------------------------------------------------+

Type :cpp:enumerator:`TrendGroup::Type::PROB` represents probability groups ``PROB30`` and ``PROB40`` not followed by any other trend group. This is considered an invalid format and :cpp:func:`TrendGroup::isValid()` will always return ``false`` for group of this type.

.. cpp:namespace-pop::



Wind and Wind Shear groups
--------------------------

.. cpp:namespace-push:: WindGroup

Surface wind, wind shear, wind shift, and peak wind groups are represented by :cpp:class:`WindGroup`:

- Surface wind, with or without gusts, with or without variable wind sector
- Wind shear at specified height
- Wind shear at the lower levels
- Potential wind shear conditions
- Wind shift, with or without associated frontal passage
- Peak wind.
- Missing wind data.  

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Visibility and Runway Visual Range groups
-----------------------------------------

.. cpp:namespace-push:: VisibilityGroup

Prevailing or directional visibility, runway visual range or visibility, surface visibility, and visibility from control tower groups are represented by :cpp:class:`VisibilityGroup`:

- Prevailing visibility or variable prevailing visibility value
- Visibility value from automated stations specified without directional variation
- Directional visibility or variable directional visibility value
- Runway visibility or variable runway visibility value
- Runway visual range
- Visibility at surface level
- Visibility from air traffic control tower
- Missing visibility or runway visual range data

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Cloud layer, ceiling and vertical visibility groups
---------------------------------------------------

.. cpp:namespace-push:: CloudGroup

Cloud layer, 'no cloud' condition, vertical visibility, and ceiling groups are represented by :cpp:class:`CloudGroup`:

- Clear sky conditions
- 'nil significant cloud' conditions
- 'no cloud detected' conditions
- Cloud layer or variable cloud cover information
- Celing height or variable ceiling height
- Vertical visibility specified when sky is obscured 
- Ground-based or aloft obscurations
- Missing cloud or ceiling data

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Weather phenomena groups
------------------------

.. cpp:namespace-push:: WeatherGroup

Current, recent or forecast weather groups are represented by :cpp:class:`WeatherGroup`:

- Current or forecast weather phenomena
- Recent weather information, with or without weather event beginning and ending time
- 'nil significant weather' condition
- Missing weather or thunderstorm data

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Current ambient air temperature and dew point groups
----------------------------------------------------

.. cpp:namespace-push:: TemperatureGroup

Current ambient air temperature and dew point with temperature resolution to 1°C or 0.1°C, and missing temperature and dew point data groups are represented by :cpp:class:`TemperatureGroup`.

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Current or forecast atmospheric pressure groups
-----------------------------------------------

.. cpp:namespace-push:: PressureGroup

Current or forecast atmospheric pressure groups are represented by :cpp:class:`PressureGroup`:

- Current atmospheric pressure normalised to sea-level
- Lowest forecast atmospheric pressure
- Actually observed atmospheric pressure (non-normalised)
- Missing atmospheric pressure data

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Runway state groups
-------------------

.. cpp:namespace-push:: RunwayStateGroup

Groups reporting runway state, accumulation of deposits on the runway, surface friction, and braking action are represented by :cpp:class:`RunwayStateGroup`.

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



State of sea surface groups
---------------------------

.. cpp:namespace-push:: SeaSurfaceGroup

Groups reporting temperature of the sea surface along with descriptive surface state or wave height are represented by :cpp:class:`SeaSurfaceGroup`.

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::


Historical or forecast minimum and maximum temperature groups
-------------------------------------------------------------

.. cpp:namespace-push:: MinMaxTemperatureGroup

Historical 6-hourly and 24-hourly maximum and minimum ambient air temperature, and forecast minimum and maximum ambient air temperature groups are represented by :cpp:class:`MinMaxTemperatureGroup`.

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Precipitation, icing and snow accumulation groups
-------------------------------------------------

.. cpp:namespace-push:: PrecipitationGroup

Various groups reporting precipitation and icing are represented by :cpp:class:`PrecipitationGroup`:

- 1-hourly total precipitation
- Snow depth on the ground and water equivalent of snow on the ground
- 3-hourly, 6-hourly, and 24-hourly frozen precipitation
- 6-hourly snowfall
- 1-hourly, 3-hourly, and 6-hourly ice accretion
- Rainfall for last 10 minutes and since 9AM local time.
- Precipitation accumulation since last report
- Missing precipitation or icing data and inoperative freezing precipitation system 

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Forecast of icing or turbulence within atmospheric layer groups
---------------------------------------------------------------

.. cpp:namespace-push:: LayerForecastGroup

Groups reporting precipitation and icing within specified range of altitudes are represented by :cpp:class:`LayerForecastGroup`.

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Atmospheric pressure tendency groups
------------------------------------

.. cpp:namespace-push:: PressureTendencyGroup

Atmospheric pressure tendency and change groups are represented by :cpp:class:`PressureTendencyGroup`.

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Cloud type groups
-----------------

.. cpp:namespace-push:: CloudTypesGroup

Groups reporting type (genus) of clouds in the cloud layers are represented by :cpp:class:`CloudTypesGroup`.

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Low-level, middle-level and high-level cloud condition groups
-------------------------------------------------------------

.. cpp:namespace-push:: LowMidHighCloudGroup

Groups reporting cloud conditions (according to cloud atlas) in the low, middle and high layers are represented by :cpp:class:`CloudTypesGroup`.

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::



Lightning conditions groups
---------------------------

.. cpp:namespace-push:: LightningGroup

Groups reporting lightning conditions such a frequency, lightning type, and direction are represented by :cpp:class:`LightningGroup`.

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::




Weather phenomena in vicinity from station groups
-------------------------------------------------

.. cpp:namespace-push:: VicinityGroup

Phenomena observed in vicinity groups are represented by :cpp:class:`VicinityGroup`:

- Thunderstorms or precipitation in vicinity
- Cumulonimbus clouds, with or without mammatus
- Towering cumulus and altocumulus castellanus
- Various standing lenticular and rotor clouds
- Virga
- Fog, haze and smoke
- Blowing snow, sand and dust

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::




Weather phenomena in vicinity of station groups
-----------------------------------------------

.. cpp:namespace-push:: VicinityGroup

Phenomena observed in vicinity groups are represented by :cpp:class:`VicinityGroup`:

- Thunderstorms or precipitation in vicinity
- Cumulonimbus clouds, with or without mammatus
- Towering cumulus and altocumulus castellanus
- Various standing lenticular and rotor clouds
- Virga
- Fog, haze and smoke
- Blowing snow, sand and dust

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::




Miscellaneous groups
--------------------

.. cpp:namespace-push:: MiscGroup

Various uncommon groups are represented by :cpp:class:`MiscGroup`:

- Sunshine duration
- Number of correction to weather observation
- Density altitude
- Largest hailstone size
- Colour codes
- 'frost on the instrument' condition

This section is under construction; more information is coming soon.

.. cpp:namespace-pop::
