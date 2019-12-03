Basics
======

.. index:: single: Group

Groups
------

METAR or TAF report consists of a number of tokens called Groups. 

For example in the following report: ::

	METAR LSGG 092050Z VRB01KT CAVOK 19/14 Q1022 NOSIG=

the first group is ``METAR``, the second group is ``LSGG``, the third group is ``092050Z``, and so on. The last group in this report is ``NOSIG``.

Character ``=`` means end of METAR or TAF report. Metaf parser does not require this character at the end of the report, it just ignores everything after ``=`` character. Character ``=`` is not the part of the last group. 

There are many types of groups and each type represents a particular bit of information. In example above, ``METAR`` means that report contains a scheduled weather observation, ``LSGG`` is an ICAO-location where observation was performed (``LSGG`` = Geneva/Cointrin Airport), and ``NOSIG`` means that no significant changes of weather conditions are expected.

Groups are typically separates by a single space. Metaf however supports groups separated by one or more space, tabulation, new line and carriage return characters.

.. index:: single: METAR; Format

METAR report format
-------------------

The syntax of the METAR report interpreted by Metaf parser is as follows:

.. image:: metar.svg

If any part of the report does not follow this format, parser stops and generates an error.

.. index:: single: Report type

METAR or SPECI is a report type. METAR indicates 'normal' scheduled report and SPECI is used for unscheduled report which is issued dut to sudden changes in weather conditions: wind shift, visibility decrease, severe weather, clouds formed or dissipated, etc. If report type is ommitted from the report, Metaf will try to autodetect it by following the report syntax.

See also :cpp:enumerator:`metaf::FixedGroup::Type::METAR` and :cpp:enumerator:`metaf::FixedGroup::Type::SPECI`.

.. index:: single: Report; Correctional

COR means that report is a correctional one. See also :cpp:enumerator:`metaf::FixedGroup::Type::COR`.

.. index:: single: Report; Automated

AUTO means that report was produced by automated station with no human intervention or oversight. See also :cpp:enumerator:`metaf::FixedGroup::Type::AUTO`.

.. index:: single: METAR; Missing report

NIL means missing report. See also :cpp:enumerator:`metaf::FixedGroup::Type::NIL`.

.. index:: single: ICAO Location

.. index:: single: Group; Location  See also :cpp:enumerator:`metaf::FixedGroup::Type::AUTO`.

Location is an ICAO location code (4 characters including letters or digits, first character must be a letter). See also :cpp:class:`metaf::LocationGroup`.

.. index:: single: Group; Report time

Time contains day-of-month and time when the report was released or issued. See also :cpp:class:`metaf::ReportTimeGroup`.

.. index:: single: Group; Maintenance indicator

Maintenance indicator '$' may be included at the end of the report to indicated that automated station requires maintenance. Typically the maintenance indicator is the last group of the report but in practice additional groups may be included after maintenance indicator.

.. index:: single: METAR; Report body format

METAR Report body
^^^^^^^^^^^^^^^^^

This is the main part of the METAR report. Its syntax is as follows:

.. image:: metar_body.svg

.. note:: Since in practice reports do not always adhere to this format precisely, parser would not stop or generate error if the actual METAR report body syntax diverges from the scheme above.

.. index:: single: Group; Surface wind

.. index:: single: Wind; Surface wind

Wind is a surface wind information, including direction, speed and gust speed. Sector of variable wind direction may be optionally included. See also :cpp:class:`metaf::WindGroup`.

.. index:: single: Group; Visibility

.. index:: single: Visibility

Visibility contains a prevailing visibility value along with directional visibility if significant directional variation is observed. Clouds is an information related to cloud layer(s), lack of clouds or unability to detect cloud cover. Alternatively CAVOK may be specified which stands for 'Ceiling And Visibility OK'. See also :cpp:class:`metaf::VisibilityGroup`.

.. index:: single: Group; Weather phenomena

.. index:: single: Weather phenomena

Current weather is essentially a list of weather phenomena complete with intensity/proximity qualifier or description.  See also :cpp:class:`metaf::WeatherGroup`.

.. index:: single: Group; Temperature

Temperature and dew point are actual measurements of ambient air temperature and dew point. See also :cpp:class:`metaf::TemperatureGroup`.

.. index:: single: Group; Atmospheric pressure

Atmospheric pressure is an actual atmospheric pressure value measured at aerodrome. See also :cpp:class:`metaf::PressureGroup`.

.. index:: single: Weather phenomena; Recent

.. index:: single: Recent weather

.. index:: single: Group; Recent weather

Recent weather may be included if significant weather phenomena ended recently. The format is similar to current weather.

.. index:: single: Group; Runway visual range

Runway visual range is a distance over which a pilot of an aircraft on the centreline of the runway can see the runway surface markings delineating the runway or identifying its centre line. It may be reported for one or more runways. See also :cpp:class:`metaf::RunwayVisualRangeGroup`.

.. index:: single: Group; Runway state

Runway state is an information on accumulation of deposits (water, snow, slush, ice, etc.) on the runway. Alternatively R/SNOCLO or SNOCLO may be included to indicate that the aerodrome is closed due to snow accumulation. See also :cpp:class:`metaf::RunwayStateGroup`.

.. index:: single: Group; Wind shear

Wind shear is an information of wind shear along with the height where it occurs. The format is similar to surface wind. See also :cpp:class:`metaf::WindGroup`.

.. index:: single: Group; Rainfall

Rainfall for recent period of time is reported by weather stations in Australia. See also :cpp:class:`metaf::RainfallGroup`.

.. index:: single: Group; State of sea surface

.. index:: single: Group; Wave height

State of sea surface is reported by some oil platforms and contains information on sea surface temperature and wave height. See also :cpp:class:`metaf::SeaSurfaceGroup`.

.. index:: single: Group; Colour code

Colour codes may be included in the report for aerodromes operated by military of NATO members and are used to quickly assess visibility and ceiling conditions. See also :cpp:class:`metaf::ColourCodeGroup`.


.. index:: single: TAF; Report format

TAF report format
-----------------

The syntax of the TAF report interpreted by Metaf parser is as follows:

.. image:: taf.svg

If any part of the report does not follow this format, parser stops and generates an error.

TAF report typically begins with TAF to indicated the report type. If report type is ommitted from the report, Metaf will try to autodetect it by following the report syntax. See also :cpp:enumerator:`metaf::FixedGroup::Type::TAF`.

COR means correctional report and AMD means amended report. See also :cpp:enumerator:`metaf::FixedGroup::Type::AMD` and :cpp:enumerator:`metaf::FixedGroup::Type::COR`.

.. index:: single: TAF; Cancelled report

.. index:: single: TAF; Missing report

NIL means missing report and CNL means cancelled report. See also :cpp:enumerator:`metaf::FixedGroup::Type::NIL` and :cpp:enumerator:`metaf::FixedGroup::Type::CNL`.

Location is an ICAO location code and time is the time when the report was released or issued. The format is simlar to METAR reports. See also :cpp:class:`metaf::LocationGroup`.

Time span indicates the time when TAF report is applicable. :cpp:enumerator:`metaf::TrendGroup::Type::TIME_SPAN`.

.. index:: single: TAF; Report body format

TAF Report body
^^^^^^^^^^^^^^^

Main part of TAF report lists prevailing weather conditions expected during the time period when report is applicable.

.. image:: taf_body.svg

.. note:: Since in practice reports do not always adhere to this format precisely, parser would not stop or generate error if the actual TAF report body syntax diverges from the scheme above.

Wind, Visibility, Cloud and Weather information are the same as used in METAR report. 

Temperature forecast indicates maximum and minimum temperature expected during the time period when report is applicable. See also :cpp:class:`metaf::TemperatureForecastGroup`.

.. index:: single: Trend

Trends
------

Trends can be optionally included with both METAR and TAF to indicate the expected weather condition changes.

Trend begins with one or more trend groups which describe what kind of change is expected, when it is expected, what is the probability, etc. Trend groups are followed by usual groups describing which weather conditions are expected (e.g. wind, visibility, weather phenomena, etc).

See also :cpp:class:`metaf::TrendGroup`.

METAR trends differ slightly from TAF trends.

.. index:: single: METAR; Trend format

The syntax of the trends included in METAR reports is as follows:

.. image:: metar_trend.svg

.. index:: single: TAF; Trend format

The syntax of the trends includes in TAF reports are as follows:

.. image:: taf_trend.svg

.. note:: Since in practice reports do not always adhere to this format precisely, parser would not stop or generate error if the actual METAR or TAF trend syntax diverges from the scheme above.

.. index:: single: Trend; NOSIG

NOSIG is used in METAR report only, when no significant weather changes are expected.

.. index:: single: Group; Probability

.. index:: single: Trend; Probability

.. index:: single: Probability

TAF trends use probability groups PROB30 and PROB40 to indicate the probability (in percent) of the following trend. If the probability is 50 percent of higher then probability is not specified. If the probability is 20 percent or lower then trend is not included in TAF report.

.. index:: single: Group; Trend type

.. index:: single: Trend; BECMG

.. index:: single: Trend; TEMPO

.. index:: single: Trend; INTER

.. index:: single: Trend; FROM

BECMG, TEMPO, INTER and FMxxxxxx are trend types. BECMG indicates that weather conditions are about to change gradually. TEMPO indicates that weather phenomena may temporarily arise for the duration of less than 60 minutes. INTER (only used in Australia) is similar to TEMPO but indicates that duration of temporary weather phenomena is less than 30 minutes. FMxxxxxx means that all previous weather conditions are superseded by the following conditions since time xxxxxx.

METAR and TAF use different notation to indicate time interval during which the trend is applicable. 

.. index:: single: Trend; Time span

.. index:: single: Group; Time span

.. index:: single: Time span

In TAF time span is used. It is formatted as xxxx/yyyy where xxxx is time of begin and yyyy is time of end.

In METAR groups FMxxxx / TLxxxx / ATxxxx are used to indicate time 'from', 'till' and 'at' respectively. 

.. note:: Old TAFs use slightly different format for time spans and trends. Metaf currently does not parse this old format.

The main part of the trend is similar to METAR/TAF report body and contains information on weather phenomena expected at certain time,

.. index:: single: Weather phenomena; Nil significant weather

.. index:: single: Nil significant weather

A special group NSW (Nil Significant Weather) may be used to indicate the end of previous weather phenomena. See also :cpp:enumerator:`metaf::FixedGroup::Type::NSW`.

.. index:: single: Remarks

Remarks
-------

.. index:: single: Group; Plain text

Remarks may contain plain-language, manual and automatically generated texts. Remarks typically augment information provided in the METAR or TAF report body.
