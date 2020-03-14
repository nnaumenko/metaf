TrendGroup
==========

.. cpp:namespace-push:: metaf

TrendGroup class
----------------

	.. cpp:class:: TrendGroup

		Stores information about weather trends which may be stored in one or several METAR or TAF groups.

.. cpp:namespace-push:: TrendGroup


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Type of the stored trend group.

		.. cpp:enumerator:: NOSIG

			Indicates that no significant weather changes are expected.

			Does not have any associated time, time span, probability or follow-up groups and used only in METAR reports.

		.. cpp:enumerator:: BECMG

			Indicates that weather conditions are expected to gradually change and transition is expected to occur within the specified time span.

		.. cpp:enumerator:: TEMPO

			Indicates that weather conditions may temporarily arise for the period of less than 60 minutes during the specified time span.

		.. cpp:enumerator:: INTER

			Indicates that weather conditions may temporarily arise for the period of less than 30 minutes during the specified time span.

			This group is only used in Australia.

		.. cpp:enumerator:: FROM

			All previous weather conditions are superseded by the other weather conditions since the specified time. This type of trend is coded as ``FM``, e.g. group ``FM092000`` in TAF or group ``FM2200`` in METAR.

		.. cpp:enumerator:: UNTIL

			The following weather conditions are expected to prevail until the specified time. This type of trend occurs only in METAR and is coded as ``TL``, e.g. group ``TL2215``.

		.. cpp:enumerator:: AT

			The following weather conditions are expected to occur at the specified time. This type of trend occurs only in METAR and is coded as ``AT``, e.g. group ``AT2000``.

		.. cpp:enumerator:: TIME_SPAN

			The following weather conditions are expected to prevail during the specified time period.

			When this group is used in TAF report and must be included before TAF report body to indicate the period when the entire forecast is applicable.

			Altrenatively, time span may be coded in METAR report as a pair of 'FM' and 'TL' groups, e.g. ``FM1300 TL1445``, or as HHMM/HHMM group (only used in Australia), e.g. ``1345/1440``.

		.. cpp:enumerator:: PROB

			Represents groups ``PROB30`` or ``PROB40`` without following time span or ``BECMG`` or ``TEMPO`` or ``INTER``.



	.. cpp:enum-class:: Probability

		Specifies the trend probability.

		.. note:: Only probability of 30% or 40% is explicitly specified. 

			The trends with probability 20% or less are not included in the report. 

			The probability of 50% or more is implicitly specified by :cpp:enumerator:`Type::BECMG` or :cpp:enumerator:`Type::TEMPO` or :cpp:enumerator:`Type::INTER` groups.

		.. cpp:enumerator:: NONE

			Probability is not specified in explicit way.

		.. cpp:enumerator:: PROB_30

			Probability is 30%.

		.. cpp:enumerator:: PROB_40

			Probability is 40%.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Trend type.

	.. cpp:function:: Probability probability() const

		:returns: Specified probability or :cpp:enumerator:`Probability::NONE` if probability was not explicitly specified.

	.. cpp:function:: std::optional<MetafTime> timeFrom() const

		:returns: Begin time of trend's time span or empty ``std::optional`` if no time span or no begin time were specified.

	.. cpp:function:: std::optional<MetafTime> timeUntil() const

		:returns: End time of trend's time span or empty ``std::optional`` if no time span or no end time were specified.

	.. cpp:function:: std::optional<MetafTime> timeAt() const

		:returns: Expected time of event or empty ``std::optional`` if no expected time of event was specified.

	.. note:: Trend group can have *either* begin time, end time, timespan with begin and end time *or* expected time of event. 



Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if all of the reported times (begin time / end time / expected event time) are valid (see :cpp:func:`MetafTime::isValid()`). Alternatively returns ``false`` if any of the time values above are not valid or the group type is :cpp:enumerator:``Type::PROB``.

.. cpp:namespace-pop::



Formats
-------

A variety of weather trend type and time group formats are recognised by Metaf.

METAR trends
^^^^^^^^^^^^

METAR trend consists of the following groups:
 
 - ``NOSIG``: indicates that no significant weather condition changes expected. This group is always used alone.
 - ``TEMPO``: indicates weather conditions that may arise temporarily for less than 60 minutes.
 - ``BECMG``: indicates that weather conditions are expected to change gradually.

Groups ``BECMG`` and ``TEMPO`` may be augmented by the groups following them. These groups are:
 - Group ``FMxxxx``: specifies time after which the weather conditions are expected to occur.
 - Group ``TLxxxx``: specifies time before which the weather conditions are expected to occur.
 - Group ``ATxxxx``: specifies time when the weather conditions are expected to occur.
 - Sequence of groups ``FMxxxx TLxxxx``: specifies time span when the weather conditions are expected to occur.

``FMxxxx``, ``TLxxxx`` and ``ATxxxx`` groups code time in :ref:`api_metaftime:4-digit METAR format`, that is two digits for hour and two digits for minute, for example ``FM2345`` means 'from 23:45'.

For example ``BECMG TL1300`` means that the weather conditions are expected to gradually change and that change will occur before 13:00.

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

Trends used in TAF reports are different from METAR trends.

TAF trend consists of the following groups:

 - Time span ``xxxx/xxxx``: indicates period of time during which the weather conditions are expected to prevail. The time format is `api_metaftime:4-digit TAF format`, that is, day and hour. For example, ``2422/2505`` indicates period from 22:00 on 24th day-of-month until 05:00 on 25th day-of-month.
 - ``TEMPO``: indicates weather conditions that may arise temporarily for less than 60 minutes. This group is followed by time span. For example ``TEMPO 2714/2718`` indicates that the weather conditions may temporarily arise from 14:00 until 18:00 on 27th day-of-month.
 - ``BECMG``: indicates gradual change of weather conditions. This group is also followed by time span. For example, ``BECMG 2920/2922`` indicates that the weather conditions are expected to change, and this change is expected to start at 20:00 on 29th day-of-month, and that at 22:00 on 29th day-of-month the new weather conditions are expected to prevail.
 - ``FMxxxxxx``: indicates rapid change of weather conditions and supersedes all previous weather conditions. The format is `api_metaftime:6-digit day/time format` (day, hour, minute). For example, ``FM301730`` indicates that weather conditions rapidly change about 17:30 on 30th day-of-month.

Time span and ``TEMPO`` may be preceded by probability groups. ``PROB30`` and ``PROB40`` indicate that the probability of the weather conditions is 30% and 40% accordingly. The examples of trends with probability are ``PROB30 2422/2505`` and ``PROB40 TEMPO 2714/2718``.

TAF trend type and time groups recognised by Metaf is below.

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
+----------------------+---------------------------------+                                     |- :cpp:func:`MetafTime::day()`.value() == 28       |- :cpp:func:`MetafTime::day()`.value() == 28       |                                                   |
|TEMPO 2812/2818       |:cpp:enumerator:`Type::TEMPO`    |                                     |- :cpp:func:`MetafTime::hour()` == 12              |- :cpp:func:`MetafTime::hour()` == 18              |                                                   |
+----------------------+---------------------------------+                                     |- :cpp:func:`MetafTime::minute()` == 00            |- :cpp:func:`MetafTime::minute()` == 00            |                                                   |
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
+----------------------+---------------------------------+-------------------------------------+                                                   |                                                   |                                                   |
|PROB40                |:cpp:enumerator:`Type::PROB`     |:cpp:enumerator:`Probability::PROB40`|                                                   |                                                   |                                                   |
+----------------------+---------------------------------+-------------------------------------+---------------------------------------------------+---------------------------------------------------+---------------------------------------------------+

Type :cpp:enumerator:`TrendGroup::Type::PROB` represents probability groups ``PROB30`` and ``PROB40`` not followed by any other trend group. This is considered an invalid format and :cpp:func:`TrendGroup::isValid()` will always return ``false`` for group of this type.


Regional variations
^^^^^^^^^^^^^^^^^^^

The following types and formats are used only in Australia:

- Trend type ``INTER`` indicates the weather conditions which may arise temporarily for less than 30 minutes. It is used similarly to ``TEMPO`` and may be included in METAR and TAF.
- Time spans in minute/hour format ``xxxx/xxxx`` are used in METAR. For example group ``1245/1300`` included in METAR means 'from 12:45 to 13:00'. Time spans in TAF follow typical day/hour format.
- Groups such as 'FMxxxx' may be specified without accompanying ``TEMPO``, ``BECMG`` or ``INTER``.


Caveats
^^^^^^^

Old TAF format used before November 2008 employs different format for time spans and trends (time without date); this format is not understood by Metaf.
