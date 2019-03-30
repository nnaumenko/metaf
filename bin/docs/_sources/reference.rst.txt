.. index:: single: Metaf; Reference

Reference
=========

.. cpp:namespace-push:: metaf

This section describes the APIs related to parsing METAR or TAF report and reading the data from parsed results.

See Extending Metaf Functionality section for APIs related to adding additional features and new groups.

.. index:: single: Metaf; Data types

Data Types
----------

This section lists the types which store the data, such as temperature, direction, time, etc.

.. index:: single: Runway

Runway
^^^^^^
.. cpp:class:: Runway

	Provides runway identification in form of runway number and designator.

	.. index:: single: Runway; Number

	The runway number is formed by the first two digits of runway heading (which is 0 to 360 degrees). The valid range for runway number is thus 0 to 36.

	The following special codes can also be used for runway number:

	====== ======================= =================================
	Number Meaning                 Associated method
	====== ======================= =================================
	88     All runways             :cpp:func:`isAllRunways()`
	99     Last message repetition :cpp:func:`isMessageRepetition()`
	====== ======================= =================================

	Parallel runways are distinguished by designating them as a left / center / right runway (e.g. runways ``21 right`` and ``21 left`` are two parallel runways with heading 210 degrees).

	.. index:: single: Runway; Designator

	.. cpp:enum-class:: Designator

		Distinguishes parallel runways.

		.. cpp:enumerator:: NONE

			No designator was specified (not a parallel runway).

		.. cpp:enumerator:: LEFT

			Left runway.

		.. cpp:enumerator:: CENTER

			Center runway.

		.. cpp:enumerator:: RIGHT

			Right runway.


	**Acquiring the data**

		.. cpp:function:: unsigned int number() const
			
			:returns: Runway number or one of special codes (see also :cpp:func:`isAllRunways()` and :cpp:func:`isMessageRepetition()`).

		.. cpp:function:: Designator designator() const

			:returns: Runway designator or :cpp:enumerator:`Designator::NONE` if no runway designator was specified, which means that the runway is not one of the parallel runways.


	**Checking for special runway codes**

		.. cpp:function:: bool isAllRunways() const

			:returns: ``true`` if 'all runways' value is coded in runway information (i.e. runway number 88 and designator :cpp:enumerator:`Designator::NONE`), and ``false`` otherwise.

		.. cpp:function:: bool isMessageRepetition() const

			:returns: ``true`` if 'last message repetition' value is coded in runway information (i.e. runway number 99 and designator :cpp:enumerator:`Designator::NONE`), and ``false`` otherwise.


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored runway information is valid, and ``false`` otherwise.

				The information is considered valid if any of the following conditions is met: 
					 - the runway number is in range 0 to 36;
					 - the runway number is either 88 or 99 and the designator is NONE;


.. index:: single: MetafTime

MetafTime
^^^^^^^^^
.. index:: single: Day-of-month

.. index:: single: Time-of-day

.. cpp:class:: MetafTime

	MetafTime is a time format used in METAR and TAF reports. It contains time-of-day in form of hour and minute and optional day-of-month.

	.. note:: METAR and TAF reports always use GMT time.


	**Acquiring the data**

		.. cpp:function:: std::optional<unsigned int> day() const

			:returns: The value of day-of-month or empty ``std::optional`` if no day was specified.

		.. cpp:function:: unsigned int hour() const

			:returns: Time-of-day hour.

			.. note: Hour value 0 means midnight at the beginning of the specified day and hour value 24 means midnight at the end of the specified day.

		.. cpp:function:: unsigned int minute() const

			:returns: Time-of-day minute.


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored day-of-month and time-of-day information is valid and ``false`` otherwise.

				The information is considered valid if all of the following conditions are met: 
					- The day-of-month value is in range 1 to 31;
					- The hour value is in range 0 to 24;
					- The minute value is in range 0 to 59; 


.. index:: single: Temperature

Temperature
^^^^^^^^^^^

.. cpp:class:: Temperature

	Stores a temperature value. Temperature value may be not reported (i.e. no value).

	If the temperature value is rounded to zero, an additional information can be acquired whether the value represents a freezing or non-freezing temperature (i.e. slightly above or slightly below zero).

	.. index:: single: Temperature; Measurement units

	.. cpp:enum-class:: Unit

		Temperature measurement units.

		.. cpp:enumerator:: C

			Degrees Celsius.

		.. cpp:enumerator:: F

			Degrees Fahrenheit.

		.. note:: Temperature value is always stored in degrees Celsius but may be converted to degrees Fahrenheit using :cpp:func:`toUnit()`.


	**Acquiring the data**

		.. cpp:function:: std::optional<int> temperature() const

			:returns: Stored temperature value or empty ``std::optional`` if temperature value is not reported.


		.. cpp:function:: Unit unit() const

			:returns: Temperature measurement unit which was used with stored value. Currently always returns :cpp:enumerator:`Unit::C` since the value is always stored in degrees Celsius.


	**Converting to other measurement units**

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.
			:returns: Stored temperature value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


	**Miscellaneous**

		.. cpp:function:: bool isFreezing() const

			:returns: ``true`` if the stored temperature value is below water freezing point (0 degrees Celsius or 32 degrees Fahrenheit), and ``false`` if the stored temperature value is above freezing point.

				If the temperature value is rounded to the freezing point, isFreezing() may return either ``true`` or ``false`` based on the following conditions:

				- if the original temperature value was in range (0.0 .. -0.5) which is encoded in METAR/TAF report as ``M00``, then ``isFreezing()`` returns ``true``;

				- if the original temperature value was in range (0.5 .. 0.0] which is encoded in METAR/TAF report as ``00``, then ``isFreezing()`` returns ``false``;


.. index:: single: Speed

Speed
^^^^^

.. cpp:class:: Speed

	Stores a speed value. Speed value can be optionally not reported (i.e. no value).

	.. index:: single: Speed; Measurement units

	.. cpp:enum-class:: Unit

		Speed measurement unit.

		.. cpp:enumerator:: KNOTS

			Knots. 

		.. cpp:enumerator:: METERS_PER_SECOND

			Meters per second.

		.. cpp:enumerator:: KILOMETERS_PER_HOUR
		
			Kilometers per hour.

		.. cpp:enumerator:: MILES_PER_HOUR

			Miles per hour.

		.. note:: Currently the speed value is never stored in miles per hour. :cpp:enumerator:`Unit::MILES_PER_HOUR` is provided only to be able to convert speed values to miles per hour.


	**Acquiring the data**

		.. cpp:function:: std::optional<unsigned int> speed() const
			
			:returns: Stored speed value or empty ``std::optional`` if speed value is not reported.

		.. cpp:function:: Unit unit() const

			:returns: Speed measurement unit which was used with stored value.


	**Converting to other measurement units**

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.
			:returns: Stored speed value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


.. index:: single: Distance

.. index:: single: Height

Distance
^^^^^^^^

.. cpp:class:: Distance

	Stores a distance or height value. The value may be expressed as integer (e.g. 3500) or rational (e.g. 2 1/4) number.

	The value consists of integer and fraction components, fraction component in turn consists of numerator and denominator. Integer or fraction parts can be optionally not reported (i.e. no value).

	.. index:: single: Distance; Measurement units

	.. cpp:enum-class:: Unit

		Distance measurement units.

		.. cpp:enumerator:: METERS

			Meters.

		.. cpp:enumerator:: STATUTE_MILES

			Statute miles.

		.. cpp:enumerator:: FEET

			Feet.

	.. index:: single: Distance; Modifier

	.. cpp:enum-class:: Modifier

		Modifier is used when instead of exact distance, a minimum or maximum value is provided. Modifier is used to report distances such as 'less than 1/4 Statute Mile' or 'more than 10000 meters'.

		.. cpp:enumerator:: NONE

			No modifier; exact value is reported.

		.. cpp:enumerator:: LESS_THAN

			The distance is less than reported value.

		.. cpp:enumerator:: MORE_THAN

			The distance is more than reported value.

	**Acquiring the data**

		.. cpp:function:: std::optional<unsigned int> integer() const

			:returns: Integer component of stored distance value or empty ``std::optional`` if there is no integer component or the value is not reported.

		.. cpp:function:: std::optional<unsigned int> numerator() const

			:returns: Numerator of fraction component of stored distance value or empty ``std::optional`` if there is no fraction part or the value is not reported.

		.. cpp:function:: std::optional<unsigned int> denominator() const

			:returns: Denominator of fraction component of stored distance value or empty ``std::optional`` if there is no fraction part or the value is not reported.

		.. cpp:function:: Modifier modifier() const

			:returns: Modifier of stored distance value (less than / more than).

		.. cpp:function:: Unit unit() const

			:returns: Distance measurement unit which was used with stored value.


	**Integer & fraction components**

		.. cpp:function:: bool isInteger() const

			:returns: ``true`` if the stored value has only integer component, and ``false`` if the stored value does not have an integer component or has fraction component or is not reported.

		.. cpp:function:: bool isFraction() const

			:returns: ``true`` if the stored value has only fraction component, and ``false`` if the stored value does not have a fraction component or has integer component or is not reported.

		.. cpp:function:: bool isReported()

			:returns: ``true`` if the stored value is reported, and ``false`` if the stored value is not reported.


	**Converting to other measurement units**

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.
			:returns: Stored distance value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.

				Both integer and fractional components are used in conversion. For example, attempting to convert value of 1 1/2 statute miles into statute miles will return value 1.5.


	**Validating**

		.. cpp:function:: bool isValid() const

			::returns:: ``true`` if stored distance value is valid, and ``false`` otherwise.

				When fraction component is present, both numerator and denominator must be non-zero for the stored value to be valid. 

				When fraction component is not present, the stored value is always considered valid.


.. index:: single: Direction

Direction
^^^^^^^^^

.. cpp:class:: Direction

	Stores a direction value. The value can be specified in degrees or as a cardinal direction. Alternatively the direction value may be omitted (i.e. not specified), specified as not reported (i.e. no value), specified as variable, or specified as No Directional Variation.

	.. index:: single: Cardinal direction

	.. index:: single: Direction; Cardinal

	.. cpp:enum-class:: Cardinal
		
		Specifies a cardinal or intercardinal direction. No secondary intercardinal directions can be specified. Alternatively may specify No Directional Variation or No Value.

		.. cpp:enumerator:: NONE

			No value or no corresponding cardinal direction can be found.

		.. cpp:enumerator:: NDV

			No directional variation.

		.. cpp:enumerator:: N

			North (0 to 22 degrees or 338 to 360 degrees).

		.. cpp:enumerator:: S

			South (158 to 202 degrees).

		.. cpp:enumerator:: W

			West (248 to 292 degrees).

		.. cpp:enumerator:: E

			East (68 to 112 degrees).

		.. cpp:enumerator:: NW

			Northwest (293 to 337 degrees).

		.. cpp:enumerator:: NE

			Northeast (23 to 67 degrees).

		.. cpp:enumerator:: SW

			Southwest (203 to 247 degrees).

		.. cpp:enumerator:: SE

			Southeast (113 to 157 degrees).

		.. cpp:enumerator:: TRUE_N

			True north (exactly 360 degrees). Value of 0 degrees is not considered as true north.

		.. cpp:enumerator:: TRUE_W

			True west (exactly 270 degrees).

		.. cpp:enumerator:: TRUE_S

			True south (exactly 180 degrees).

		.. cpp:enumerator:: TRUE_E

			True east (exactly 90 degrees).

	.. index:: single: Direction; Status

	.. cpp:enum-class:: Status

		The status of the direction value reported. If the status is other than :cpp:enumerator:`VALUE_DEGREES` or :cpp:enumerator:`VALUE_CARDINAL`, then no numerical direction value is provided.

		.. index:: single: Direction; Omitted

		.. cpp:enumerator:: OMMITTED

			Direction is omitted (i.e. no direction specified at all).

		.. index:: single: Direction; Not reported

		.. cpp:enumerator:: NOT_REPORTED

			Direction is specified as 'not reported'.

		.. index:: single: Direction; Variable

		.. cpp:enumerator:: VARIABLE

			Direction is reported as variable.

		.. index:: single: Direction; No directional variation

		.. cpp:enumerator:: NDV

			Direction is reported as 'No Directional Variation'.

		.. index:: single: Direction; Degrees value

		.. cpp:enumerator:: VALUE_DEGREES

			Direction is reported as value in degrees.

		.. index:: single: Direction; Cardinal value

		.. cpp:enumerator:: VALUE_CARDINAL

			Direction is reported as cardinal value.

	**Acquiring the data**

		.. cpp:function:: Status status() const

			::returns:: Status of stored direction value.

		.. cpp:function:: Cardinal cardinal(bool trueDirections = false) const

			:param trueDirections: If set to ``true`` allows returning :cpp:enumerator:`Cardinal::TRUE_NORTH`, :cpp:enumerator:`Cardinal::TRUE_SOUTH`, :cpp:enumerator:`Cardinal::TRUE_EAST`, :cpp:enumerator:`Cardinal::TRUE_WEST`

			:returns: Cardinal direction corresponding to the stored direction value.

				- If the status of the stored value is :cpp:enumerator:`Status::OMMITTED`, :cpp:enumerator:`Status::NOT_REPORTED`, :cpp:enumerator:`Status::VARIABLE`, then :cpp:enumerator:`Cardinal::NONE` is returned. 

				- If the status is :cpp:enumerator:`Status::NDV` then :cpp:enumerator:`Cardinal::NDV` is returned.

				- If the direction value in degrees is reported (i.e. status is :cpp:enumerator:`Status::VALUE_DEGREES`) and the value exceeds 360 degrees then :cpp:enumerator:`Cardinal::NONE` is returned.

		.. cpp:function:: std::optional<unsigned int> degrees() const

			:returns: Stored value in degrees. If cardinal value was stored, then the middle value of the corresponding directional sector is returned as follows:

				================== ==============
				Cardinal direction Returned value
				================== ==============
				North              360
				Northeast          45
				East               90
				Southeast          135
				South              180
				Southwest          225
				West               270
				Northwest          315
				================== ==============

				If the status of the stored value is :cpp:enumerator:`Status::OMMITTED`, :cpp:enumerator:`Status::NOT_REPORTED`, :cpp:enumerator:`Status::VARIABLE` or :cpp:enumerator:`Status::NDV`, then an empty ``std::optional`` is returned.


	**Miscellaneous**

		.. cpp:function:: bool isValue() const

			:returns: ``true`` if the stored direction contains a value, and ``false`` if the stored direction does not contain a concrete value. 

				- ``true`` is returned if ether cardinal direction (:cpp:enumerator:`Status::VALUE_CARDINAL`) or value in degrees (:cpp:enumerator:`Status::VALUE_DEGREES`) is stored.

				- ``false`` is returned if the status is :cpp:enumerator:`Status::OMMITTED`, :cpp:enumerator:`Status::NOT_REPORTED`, :cpp:enumerator:`Status::VARIABLE` or :cpp:enumerator:`Status::NDV`.


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored direction value is valid, and ``false`` otherwise. 

				The direction value is considered to be valid if value in degrees was specified and the value is less or equal than 360 degrees. If stored direction does not contain a value, it is always considered valid.


.. index:: single: Pressure

Pressure
^^^^^^^^

.. cpp:class:: Pressure

	Stores a pressure value. The intended use is atmospheric pressure but any absolute pressure value can be stored.

	Pressure value can be optionally not reported (i.e. no value).

	.. index:: single: Pressure; Measurement units

	.. cpp:enum-class:: Unit

		Pressure measurement units.

		.. cpp:enumerator:: HECTOPASCAL

			Hectopascal.

		.. cpp:enumerator:: INCHES_HG

			Inches mercury.

	**Acquiring the data**

		.. cpp:function:: std::optional<float> pressure() const

		:returns: Stored pressure value or empty ``std::optional`` if pressure value is not reported.

		.. cpp:function:: Unit unit() const

		:returns: Pressure measurement unit which was used with stored value.


	**Converting to other measurement units**

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.
			:returns: Stored pressure value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


.. index:: single: Precipitation amount

.. index:: single: Accumulation of precipitation

Precipitation
^^^^^^^^^^^^^

.. cpp:class:: Precipitation

	The amount or accumulation of precipitation. 

	The amount/accumulation may be not reported (i.e. no value) or alternatively it may specify that the runway is not operational due to deposits accumulation (which means that exact accumulation value is not important and is not reported).

	.. index:: single: Precipitation amount; Status

	.. cpp:enum-class:: Status

		Status of precipitation value.

		.. index:: single: Precipitation amount; Not reported

		.. cpp:enumerator:: NOT_REPORTED

			The amount or accumulation of precipitation is not reported (i.e. no value).

		.. index:: single: Precipitation amount; Reported

		.. cpp:enumerator:: REPORTED

			The amount or accumulation of precipitation value is reported.

		.. index:: single: Precipitation amount; Runway not operational

		.. cpp:enumerator:: RUNWAY_NOT_OPERATIONAL

			The runway is not operational due to deposits accumulation; the exact accumulation value is not important since runway cannot be used.

	.. index:: single: Precipitation amount; Measurement units

	.. cpp:enum-class:: Unit

		Precipitation amount or accumulation measurement units.

		.. cpp:enumerator:: MM

			Millimeters.
			
		.. cpp:enumerator:: INCHES

			Inches.

		.. note:: Precipitation amount/accumulation value is always stored in millimeters but may be converted to inches using :cpp:func:`toUnit()`.


	**Acquiring the data**

		.. cpp:function:: Status status() const

			:returns: Status of precipitation amount or accumulation value.

		.. cpp:function:: std::optional<float> precipitation() const

			:returns: Stored amount/accumulation value or empty ``std::optional`` if the value is not reported (:cpp:enumerator:`Status::NOT_REPORTED`) or runway is not operational (:cpp:enumerator:`Status::RUNWAY_NOT_OPERATIONAL`).

		.. cpp:function:: Unit unit() const

			:returns: Precipitation amount/accumulation measurement unit which was used with stored value. Currently always returns :cpp:enumerator:`Unit::MM` since the value is always stored in millimeters.


	**Converting to other measurement units**

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.
			:returns: Stored precipitation amount/accumulation value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported or if the runway is not operational.


.. index:: single: Surface friction

SurfaceFriction
^^^^^^^^^^^^^^^

.. cpp:class:: SurfaceFriction

	Surface friction. The intended use is reporting the surface friction of the runway. Surface friction may be reported in the form of friction coefficient or braking action.

	The value may be optionally not reported, or reported as unreliable/unmeasurable.

	.. index:: single: Surface friction; Friction coefficient

	Friction coefficient is a value in range [0.00 .. 1.00]. Lesser values mean more slippery surface.

	.. note:: Surface friction coefficient is a dimensionless value and has no associated measurement units.

	.. index:: single: Surface friction; Status

	.. cpp:enum-class:: Status

		The status of surface friction value.

		.. index:: single: Surface friction; Not reported

		.. cpp:enumerator:: NOT_REPORTED

			Surface friction is not reported.

		.. cpp:enumerator:: SURFACE_FRICTION_REPORTED

			Surface friction reported in form of friction coefficient.

		.. cpp:enumerator:: BRAKING_ACTION_REPORTED

			Surface friction reported in form of braking action.

		.. index:: single: Surface friction; Unreliable

		.. cpp:enumerator:: UNRELIABLE

			The measurement result is unreliable or the value is unmeasurable.

	.. index:: single: Surface friction; Braking action

	.. index:: single: Braking action

	.. cpp:enum-class:: BrakingAction

		Descriptive braking action which specifies approximate range of surface friction coefficient rather than precise coefficient value.

		.. cpp:enumerator:: NONE

			Value is either not reported, unreliable or unmeasurable.

		.. cpp:enumerator:: POOR

			Friction coefficient is 0.25 or lesser.

		.. cpp:enumerator:: MEDIUM_POOR

			Friction coefficient is from 0.26 to 0.29.

		.. cpp:enumerator:: MEDIUM

			Friction coefficient is from 0.30 to 0.35.

		.. cpp:enumerator:: MEDIUM_GOOD

			Friction coefficient is from 0.36 to 0.39.

		.. cpp:enumerator:: GOOD

			Friction coefficient is 0.40 or greater.

	**Acquiring the data**

		.. cpp:function:: Status status() const

			:returns: Status of surface friction value.

		.. cpp:function:: std::optional<float> coefficient() const

			:returns: The value of friction coefficient or empty ``std::optional`` if the value is not reported, unreliable or unmeasurable.

				If :cpp:enum:`BrakingAction` was reported, then 'worst' (i.e. least) friction coefficient for the range specified by stored braking action value is returned (e.g. if braking action was reported as :cpp:enumerator:`BrakingAction::MEDIUM_GOOD` then 0.36 is returned).

		.. cpp:function:: BrakingAction brakingAction() const

			:returns: Braking action value corresponding to stored value.

				If friction coefficient was reported, a range of :cpp:enum:`BrakingAction` where this value of the friction coefficient fits is returned (e.g. if friction coefficient with value 0.33 was reported, then :cpp:enumerator:`BrakingAction::MEDIUM` is returned).


.. index:: single: Wave height

WaveHeight
^^^^^^^^^^

.. cpp:class:: WaveHeight

	WaveHeight or descriptive state of sea surface which specifies the range of wave heights.

	Both state of sea surface and wave height may be optionally not reported (i.e. no value).

	.. index:: single: Wave height; Type

	.. cpp:enum-class:: Type

		.. cpp:enumerator:: STATE_OF_SURFACE

			Descriptive state of surface is specified.

		.. cpp:enumerator:: WAVE_HEIGHT

			Actual numerical wave height is specified.
	
	.. index:: single: Wave height; Measurement units

	.. cpp:enum-class:: Unit

		Wave height measurement unit.

		.. cpp:enumerator:: METERS

			Meters.

		.. cpp:enumerator:: FEET

			Feet.

	.. index:: single: State of sea surface

	.. cpp:enum-class:: StateOfSurface

		.. index:: single: State of sea surface; Not reported

		.. cpp:enumerator:: NOT_REPORTED

			State of sea surface is not reported.

		.. index:: single: State of sea surface; Calm (glassy)

		.. cpp:enumerator:: CALM_GLASSY

			Sea surface calm (glassy), no waves.

		.. index:: single: State of sea surface; Calm (rippled)

		.. cpp:enumerator:: CALM_RIPPLED

			Sea surface calm (rippled), wave height <0.1 meters.

		.. index:: single: State of sea surface; Smooth

		.. cpp:enumerator:: SMOOTH

			Sea surface smooth, wave height 0.1 to 0.5 meters.

		.. index:: single: State of sea surface; Slight

		.. cpp:enumerator:: SLIGHT

			Slight waves with height 0.5 to 1.25 meters.

		.. index:: single: State of sea surface; Moderate

		.. cpp:enumerator:: MODERATE

			Moderate waves with height 1.25 to 2.5 meters.

		.. index:: single: State of sea surface; Rough

		.. cpp:enumerator:: ROUGH

			Sea surface rough, wave height 2.5 to 4 meters.

		.. index:: single: State of sea surface; Very rough

		.. cpp:enumerator:: VERY_ROUGH

			Sea surface very rough, wave height 4 to 6 meters.

		.. index:: single: State of sea surface; High

		.. cpp:enumerator:: HIGH

			High waves with height 6 to 9 meters.

		.. index:: single: State of sea surface; Very high

		.. cpp:enumerator:: VERY_HIGH

			Very high waves with height 9 to 14 meters.

		.. index:: single: State of sea surface; Phenomenal

		.. cpp:enumerator:: PHENOMENAL

			Phenomenal waves with height of 14 meters or more.

	**Acquiring the data**

		.. cpp:function:: Type type() const

			:returns: Type of the value: descriptive sea surface or numerical wave height.

		.. cpp:function:: StateOfSurface stateOfSurface() const

			:returns: State of sea surface corresponding to the value.

				If wave height was specified, a range of :cpp:enum:`StateOfSurface` where this value of the wave height fits is returned (e.g. if wave height of 1.1 meters was reported, then :cpp:enumerator:`StateOfSurface::SLIGHT` is returned).

		.. cpp:function:: std::optional<float> waveHeight() const

			:returns: Wave height value.

				If :cpp:enum:`StateOfSurface` was reported, then highest wave height for the range specified by stored descriptive value is returned (e.g. if state of sea surface was reported as :cpp:enumerator:`StateOfSurface::ROUGH` then 4.0 is returned).

				If :cpp:enumerator:`StateOfSurface::PHENOMENAL` was reported then there is no highest wave height value and lowest value of 14 meters is returned instead.

		.. cpp:function:: Unit unit() const

			:returns: Wave height measurement unit which was used with stored value. Currently always returns :cpp:enumerator:`Unit::METERS` since the value is always specified in decimeters.


	**Converting to other measurement units**

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.
			:returns: Stored wave height value (or highest wave height value for specified :cpp:enum:`StateOfSurface`) converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


.. index:: single: Group

Group
-----

.. cpp:type:: Group = std::variant<PlainTextGroup, FixedGroup, LocationGroup, ReportTimeGroup, TrendGroup, WindGroup, VisibilityGroup, CloudGroup, WeatherGroup, TemperatureGroup, TemperatureForecastGroup, PressureGroup, RunwayVisualRangeGroup, RunwayStateGroup, RainfallGroup, SeaSurfaceGroup, ColourCodeGroup>

	Group is an ``std::variant`` which holds all group classes. It is used by :cpp:class:`metaf::Parser` to return the results of report parsing (see :cpp:func:`metaf::Parser::getResult()`).



.. index:: single: Group; Types

Group types
-----------

This section contains the information on each group class which stores information on individual types of METAR/TAF groups.


.. index:: single: Plain text

.. index:: single: Group; Plain text

.. index:: single: Unrecognised groups

.. index:: single: Group; Unrecognised

PlainTextGroup
^^^^^^^^^^^^^^

.. cpp:class:: PlainTextGroup

	Plain text group is generally a group which has no specific format. This group stores the original group text without changes.

	.. warning:: Only the first 16 characters of the group are stored. Any group which contains text longer than 16 characters will be truncated (no error is generated when truncating the group).

	The groups in METAR or TAF report that were not recognised by the parser, are stored as Plain text groups.

	**Acquiring group data**

		.. cpp:function:: std::string toString() const

			:returns: Content of plain text group in form of ``std::string``.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if the information stored in the group is valid and consistent, and ``false`` otherwise.

				Plain text group is considered valid if it contains a non-empty string.


.. index:: single: Group; Fixed text

FixedGroup
^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: fixedgroup.svg

.. cpp:class:: FixedGroup

	Fixed group represent a text which is never modified if it is included in the report.

	For example, report types METAR, SPECI or TAF at the beginning of the report or CAVOK in the report body are always spelled exactly the same way and have no modifications.

	.. cpp:enum-class:: Type

		Designates the fixed text which is represented by this group.

		.. index:: single: Report type; METAR

		.. cpp:enumerator:: METAR

			Specifies that the report type is METAR (weather observation) and this is a scheduled report.

		.. index:: single: Report type; SPECI

		.. cpp:enumerator:: SPECI

			Specifies that the report type is METAR (weather observation) and this is an unscheduled report.

			Unscheduled report is issued dut to sudden changes in weather conditions: wind shift, visibility decrease, severe weather, clouds formed or dissipated, etc.

		.. index:: single: Report type; TAF

		.. cpp:enumerator:: TAF

			Specifies that the report type is TAF (weather forecast).

		.. index:: single: Report; Amended

		.. cpp:enumerator:: AMD

			Specifies an amended report.

			This group is only used in TAF reports.

		.. index:: single: Report; Correctional

		.. cpp:enumerator:: COR

			Specifies a correctional report.

		.. index:: single: Report; Nil

		.. cpp:enumerator:: NIL

			Specifies a missing report.

			No report body is allowed after this group.

		.. index:: single: Report; Cancelled

		.. cpp:enumerator:: CNL

			Specifies a cancelled report.

			No report body is allowed after this group.

			This group is only used in TAF reports.

		.. index:: single: Report; Automated

		.. index:: single: Automated report

		.. cpp:enumerator:: AUTO

			Specifies a fully automated report produced with no human intervention or oversight.

			This group is only used in METAR reports.

		.. index:: single: Runway state; Aerodrome closed due to snow accumulation

		.. cpp:enumerator:: R_SNOCLO

			Aerodrome is closed due to snow accumulation.

			This group may be used in form of ``SNOCLO`` or ``R/SNOCLO``.

		.. index:: single: CAVOK

		.. index:: single: Visibility; CAVOK

		.. index:: single: Cloud; CAVOK

		.. cpp:enumerator:: CAVOK

			Ceiling and visibility OK; all of the following conditions are met:

				- Visibility 10 km or more in all directions.

				- No cloud below 5000 feet (1500 meters).

				- No cumulonimbus or towering cumulus clouds.

				- no significant weather phenomena.

		.. index:: single: Weather phenomena; Nil significant weather

		.. index:: single: Nil significant weather

		.. cpp:enumerator:: NSW

			Nil significant weather.

			This group is only used in trends and indicates the end of a significant weather phenomena.

		.. index:: single: Remarks

		.. cpp:enumerator:: RMK

			This group designates the beginning of the remarks.

			Remarks are currently interpreted as plain text.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of the fixed text group.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: This method is for compatibility only and always returns ``true`` for this group.


.. index:: single: Group; Location

.. index:: single: ICAO location

LocationGroup
^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: locationgroup.svg

.. cpp:class:: LocationGroup

	Location group stores an ICAO location code of the site where observation was performed or for which the forecast is provided.

	If the report is issued for the location which does not have an ICAO code, then code ZZZZ is used.

	An ICAO code is a four-character string. First character may only contain latin capital letters; the rest of the character may contain either latin capital letters or digits.

	**Acquiring group data**

		.. cpp:function:: std::string toString() const

			:returns: String with an ICAO location.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: This method is for compatibility only and always returns ``true`` for this group.


.. index:: single: Group; Report time

.. index:: single: Report; Release time

ReportTimeGroup
^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: reporttimegroup.svg

.. cpp:class:: ReportTimeGroup

	Report time stores information about report release date and time.

	**Acquiring group data**

		.. cpp:function:: const MetafTime & time() const

			:returns: Time when the report was released (GMT time zone).

		.. warning:: The reference to time value is only valid as long as :cpp:class:`ReportTimeGroup` instance exists. Once :cpp:class:`ReportTimeGroup` instance is destroyed, the previously acquired reference to :cpp:class:`MetafTime` is no longer valid.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if the day-of-month, hour and minute values of report release time belong to valid ranges (see :cpp:func:`MetafTime::isValid()`) and the optional day is included in :cpp:class:`MetafTime`.

				This method returns ``false`` if any of the conditions above is not met.


.. index:: single: Group; Trend

.. index:: single: Trend

TrendGroup
^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: trendgroup.svg

.. note:: Spaces between sequential groups in METAR/TAF report are not shown. 

See :doc:`basics` for more information on weather trends and how they are reported.


.. cpp:class:: TrendGroup

	Stores information about weather trends which may be stored in one or several METAR or TAF groups.

	.. cpp:enum-class:: Type

		Type of the stored trend group.

		.. cpp:enumerator:: NONE

			Indicates that this group stores a valid but incomplete trend group or combination of such groups.

		.. index:: single: Trend; NOSIG

		.. cpp:enumerator:: NOSIG

			Indicates that no significant weather changes are expected.

			Does not have any associated time, time span, probability or follow-up groups and used only in METAR reports.

		.. index:: single: Trend; BECMG

		.. cpp:enumerator:: BECMG

			Indicates that weather conditions are expected to gradually change and transition is expected to occur within the specified time span.

		.. index:: single: Trend; TEMPO

		.. cpp:enumerator:: TEMPO

			Indicates that weather conditions may temporarily arise for the period of less than 60 minutes during the specified time span.

		.. index:: single: Trend; INTER

		.. cpp:enumerator:: INTER

			Indicates that weather conditions may temporarily arise for the period of less than 30 minutes during the specified time span.

			This group is only used in Australia.

		.. index:: single: Trend; FROM

		.. cpp:enumerator:: FROM

			All previous weather conditions are superseded by the other weather conditions since the specified time.

		.. index:: single: Trend; Time span

		.. index:: single: Group; Time span

		.. index:: single: Time span

		.. cpp:enumerator:: TIME_SPAN

			The following weather conditions are expected to prevail during the specified time period.

			This group is only used in TAF report and must be included before TAF report body to indicate the period when the entire forecast is applicable.


	.. index:: single: Group; Probability

	.. index:: single: Trend; Probability

	.. index:: single: Probability

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

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Trend type.

		.. cpp:function:: Probability probability() const

			:returns: Specified probability or :cpp:enumerator:`Probability::NONE` if probability was not explicitly specified.

		.. cpp:function:: const std::optional<MetafTime> & timeFrom() const

			:returns: Begin time of trend's time span or empty ``std::optional`` if no time span or no begin time were specified.

		.. cpp:function:: const std::optional<MetafTime> & timeTill() const

			:returns: End time of trend's time span or empty ``std::optional`` if no time span or no end time were specified.

		const std::optional<MetafTime> & timeAt() const

			:returns: Expected time of event or empty ``std::optional`` if no expected time of event was specified.

		.. note:: Trend group can have *either* begin time, end time, timespan with begin and end time *or* expected time of event. 

		.. warning:: The reference to time values are only valid as long as :cpp:class:`TrendGroup` instance exists. Once :cpp:class:`TrendGroup` instance is destroyed, the previously acquired references to :cpp:class:`MetafTime` is no longer valid.


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if all of the reported times (begin time / end time / expected event time) are valid (see :cpp:func:`MetafTime::isValid()`).

				Alternatively returns ``false`` if any of the time values above are not valid.


.. index:: single: Wind

.. index:: single: Group; Surface wind

.. index:: single: Group; Wind shear

.. index:: single: Group; Variable wind direction sector

.. index:: single: Wind shear

.. index:: single: Wind; Surface wind

.. index:: single: Wind; Variable

WindGroup
^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: windgroup.svg

.. cpp:class:: WindGroup

	Stores information about surface wind (including variable wind direction sector if reported) or wind shear.

	**Acquiring group data**

		.. index:: single: Wind; Direction

		.. cpp:function:: const Direction & direction() const

			:returns: Wind direction; typicaly a direction value in degrees but also can be variable or non-reported.

		.. index:: single: Wind; Speed
		
		.. cpp:function:: const Speed & windSpeed() const
		
			:returns: Wind speed.

		.. index:: single: Wind; Gust speedW

		.. cpp:function:: const Speed & gustSpeed() const

			:returns: Wind gust speed.

		.. index:: single: Wind; Variable wind direction sector

		.. cpp:function:: const Direction & varSectorBegin() const

			:returns: Start direction point of variable wind direction sector.

		.. cpp:function:: const Direction & varSectorEnd() const

			:returns: End direction point of variable wind direction sector.

		.. note::Wind direction sector is defined from start point clockwise to end point.
		
		.. index:: single: Wind shear; Height

		.. cpp:function:: const Distance & windShearHeight() const

			:returns: Height at which wind shear occurs or a non-reported value if surface wind data are specified.

	**Miscellaneous**

		.. index:: single: Wind; Calm

		.. cpp:function:: bool isCalm() const

			:returns: ``true`` if calm wind (i.e. no wind) is reported. Calm wind is coded as ``00000KT`` or ``00000MPS`` or ``00000KMH``.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored wind information is valid, and ``false`` otherwise.

				The information is considered valid if all of the following conditions are met: 
					- If both gust speed and wind speed are reported, wind speed is less than gust speed;
					- If gust speed is reported, its value is non-zero;
					- If group contains wind shear information rather than surface wind information (i.e. wind shear height is reported) then wind shear height value is non-zero;
					- Wind direction, wind shear height, variable wind sector directions must be valid values if reported.


.. index:: single: Visibility

.. index:: single: Group; Visibility

VisibilityGroup
^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: visibilitygroup.svg

.. note:: Spaces between sequential groups in METAR/TAF report are not shown.

.. cpp:class:: VisibilityGroup

	Stores information about prevailing visibility or visibility towards cardinal direction.

	See also CAVOK (:cpp:enumerator:`metaf::FixedGroup::Type::CAVOK`) which may be used to specify visibility of 10 km or more in all directions.

	**Acquiring group data**

		.. cpp:function:: const Distance & visibility() const

			:returns: Visibility value. Values in meters are integer, values in statute miles may be fractional. May contain 'less than' or 'more than' modifiers or may be non-reported value.

		.. index:: single: Visibility; Prevailing

		.. index:: single: Visibility; Directional

		.. cpp:function:: const Direction & direction() const

			Cardinal direction if directional visibility is specified or omitted value if prevailing visibility is specified. Automated stations may also report No Directional Variation if the station is not capable of providing directional visibility.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored visibility information is valid, and ``false`` otherwise.

				The information is considered valid if all of the following conditions are met: 
					- The stored visibility and direction values are valid (if reported);
					- The group does not represent an incomplete integer group (i.e. single digit group not followed by fraction and SM designator).


.. index:: single: Cloud

.. index:: single: Group; Cloud layer

.. index:: single: Cloud; Layer

CloudGroup
^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: cloudgroup.svg

.. cpp:class:: CloudGroup

	Stores information about a single cloud layer, lack of cloud cover or vertical visibility.

	.. cpp:enum-class:: Amount

		Amount (cover) of the cloud layer.

		See also CAVOK (:cpp:enumerator:`metaf::FixedGroup::Type::CAVOK`) which may be used to specify no cloud below 5000 feet (1500 meters) and no cumulonimbus or towering cumulus clouds.

		.. cpp:enumerator::	NOT_REPORTED

			Cloud cover (amount of cloud) is not reported.

		.. index:: single: Cloud; No cloud detected

		.. cpp:enumerator:: NCD

			No cloud detected: automated weather station did not detect any clouds. Either no clouds are present or sensor error occurred.

		.. index:: single: Cloud; Nil significant cloud

		.. cpp:enumerator:: NSC

			Nil significant clouds: no cloud below 5000 feet (1500 meters), no cumulonimbus or towering
			cumulus, and no vertical visibility restriction.

		.. index:: single: Cloud; Clear sky

		.. index:: single: Group; Clear sky

		.. index:: single: Clear sky

		.. cpp:enumerator:: NONE_CLR

			No clouds / clear sky. No cloud layers are detected at or below 12000 feet /3700 meters) (US) or 25000 feet / 7600 meters (Canada).

			Indicates that station is at least partly automated.

			.. note:: CAVOK group (:cpp:enumerator:`metaf::FixedGroup::Type::CAVOK`) is also used to indicate clear sky.

		.. index:: single: Cloud; Clear sky

		.. index:: single: Group; Clear sky

		.. index:: single: Clear sky

		.. cpp:enumerator:: NONE_SKC

			No clouds / clear sky. In North America indicates report producted by human rather than automatic weather station.

			.. note:: CAVOK group (:cpp:enumerator:`metaf::FixedGroup::Type::CAVOK`) is also used to indicate clear sky.

		.. index:: single: Cloud; Few

		.. cpp:enumerator:: FEW

			Few clouds (1/8 to 2/8 sky covered).

		.. index:: single: Cloud; Scattered

		.. cpp:enumerator:: SCATTERED

			Scattered clouds (3/8 to 4/8 sky covered).

		.. index:: single: Cloud; Broken

		.. cpp:enumerator:: BROKEN

			Broken clouds (5/8 to 7/8 sky covered).

		.. index:: single: Cloud; Overcast

		.. cpp:enumerator:: OVERCAST
			
			Overcast (8/8 sky covered)

		.. index:: single: Cloud; Vertical visibility

		.. index:: single: Group; Vertical Visibility

		.. index:: single: Visibility; Vertical

		.. cpp:enumerator:: OBSCURED

			Sky obscured; vertical visibility reported instead.

	.. cpp:enum-class:: Type

		Significant convectional type of the cloud.

		.. cpp:enumerator::NOT_REPORTED

			Convectional cloud type is not reported.

		.. cpp:enumerator::NONE

			No significant convectional clouds.

		.. index:: single: Cloud; Towering cumulus

		.. cpp:enumerator::TOWERING_CUMULUS

			Towering cumulus clouds.

		.. index:: single: Cloud; Cumulonimbus

		.. cpp:enumerator::CUMULONIMBUS

			Cumulonimbus clouds.

	**Acquiring group data**

		Amount amount() const

			:returns: Amount (cover) of clouds in layer or clear sky conditions.

		Type type() const

			:returns: Significant convectional type of cloud layer.

		.. index:: single: Cloud; Base height

		const Distance & height() const

			:returns: Cloud base height in the cloud layer. For clear sky, no cloud detected, nil significant cloud conditions returns a non-reported value. When sky is obscured, returns a non-reported value (use :cpp:func:`verticalVisibility()` instead).

		const Distance & verticalVisibility() const

			:returns: When sky is obscured returns a vertical visibility value (if reported). For any other condition returns a non-reported value.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored cloud information is valid, and ``false`` otherwise. The information is considered valid if the value of cloud cover height or vertical visibility is valid (if reported).


.. index:: single: Group; Weather phenomena

.. index:: single: Group; Recent weather

.. index:: single: Weather phenomena

WeatherGroup
^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: weathergroup.svg

.. cpp:class:: WeatherGroup

	Stores information about recent or current weather phenomena.

	.. index:: single: Weather phenomena; Qualifier

	.. cpp:enum-class:: Qualifier

		.. cpp:enumerator:: NONE

			No qualifier. This group reports current weather observed at location.

		.. index:: single: Weather phenomena; Recent

		.. index:: single: Recent weather

		.. cpp:enumerator:: RECENT

			This group reports recent weather rather than current weather.

		.. index:: single: Weather phenomena; Proximity

		.. cpp:enumerator:: VICINITY

			This group reports weather in vicinity rather than on site.

		.. index:: single: Weather phenomena; Light intensity

		.. cpp:enumerator:: LIGHT

			Light intensity.

		.. index:: single: Weather phenomena; Moderate intensity

		.. cpp:enumerator:: MODERATE

			Moderate intensity. This qualier is used with precipitation only.

		.. index:: single: Weather phenomena; Heavy intensity

		.. cpp:enumerator:: HEAVY

			Heavy intensity.

	.. index:: single: Weather phenomena; Descriptor

	.. index:: single: Weather phenomena

	.. cpp:enum-class:: Descriptor

		.. cpp:enumerator:: NONE

		.. index:: single: Weather phenomena; Descriptor Shallow

		.. cpp:enumerator:: SHALLOW

			This descriptor is only be used to further describe fog that has little vertical extent (less than 6 feet), i.e. ground fog.

		.. index:: single: Weather phenomena; Descriptor Partial

		.. cpp:enumerator:: PARTIAL

			This descriptors is only be used to further describe fog that has little vertical extent (normally greater than or equal to 6 feet but less than 20 feet), and reduces horizontal visibility, but to a lesser extent vertically. The stars may often be seen by night and the sun by day. The fog is covering only the part of the aerodrome.

		.. index:: single: Weather phenomena; Descriptor Patches

		.. cpp:enumerator:: PATCHES

			This descriptors is only be used to further describe fog that has little vertical extent (normally greater than or equal to 6 feet but less than 20 feet), and reduces horizontal visibility, but to a lesser extent vertically. The stars may often be seen by night and the sun by day. The fog consists of patches randomly covering the aerodrome.

		.. index:: single: Weather phenomena; Descriptor Low drifting

		.. cpp:enumerator:: LOW_DRIFTING

			When dust, sand, or snow is raised by the wind to less than 6 feet, "low drifting" shall be used to further describe the weather phenomenon.

		.. index:: single: Weather phenomena; Descriptor Blowing

		.. cpp:enumerator:: BLOWING

			When dust, sand, snow, and/or spray is raised by the wind to a height of 6 feet or more, "blowing" shall be used to further describe the weather phenomenon.

		.. index:: single: Weather phenomena; Descriptor Showers

		.. cpp:enumerator:: SHOWERS

			Precipitation characterized by the suddenness with which they start and stop, by the rapid changes of intensity, and usually by rapid changes in the appearance of the sky.

		.. index:: single: Weather phenomena; Descriptor Thunderstorm

		.. cpp:enumerator:: THUNDERSTORM

			A local storm produced by a cumulonimbus cloud that is accompanied by lightning and/or thunder. Thunderstorm may be reported without any accompanying precipitation.

		.. index:: single: Weather phenomena; Descriptor Freezing

		.. cpp:enumerator:: FREEZING

			When fog is occurring and the temperature is below 0°C, this descriptor is used to further describe the phenomena.

			..note:: The fog is described as 'freezing' at freezing temperatures, regardless of whether is deposits the rime.

			When drizzle and/or rain freezes upon impact and forms a glaze on the ground or other exposed objects, this descriptor is used to further describe the precipitation.

	.. index:: single: Weather phenomena

	.. cpp:enum-class:: Weather

		Precipitation, obscuration and other weather phenomena.

		.. index:: single: Weather phenomena; Not reported

		.. cpp:enumerator:: NOT_REPORTED

			An automatic observing system is used and the present weather cannot be
			observed.

		.. index:: single: Weather phenomena; Drizzle

		.. cpp:enumerator:: DRIZZLE

			Fairly uniform precipitation composed exclusively of fine drops with diameters of less than 0.02 inch (0.5 mm) very close together. Drizzle appears to float while following air currents, although unlike fog droplets, it falls to the ground.

		.. index:: single: Weather phenomena; Rain

		.. cpp:enumerator:: RAIN

			Precipitation, either in the form of drops larger than 0.02 inch (0.5 mm), or smaller drops which, in contrast to drizzle, are widely separated.

		.. index:: single: Weather phenomena; Snow

		.. cpp:enumerator:: SNOW

			Precipitation of snow crystals, mostly branched in the form of six-pointed stars.

		.. index:: single: Weather phenomena; Snow grains

		.. cpp:enumerator:: SNOW_GRAINS

			Precipitation of very small, white, and opaque grains of ice.

		.. index:: single: Weather phenomena; Ice crystals

		.. index:: single: Weather phenomena; Diamond dust

		.. cpp:enumerator:: ICE_CRYSTALS

			A fall of unbranched (snow crystals are branched) ice crystals in the form of needles, columns, or plates.

		.. index:: single: Weather phenomena; Ice pellets

		.. cpp:enumerator:: ICE_PELLETS

			Precipitation of transparent or translucent pellets of ice, which are round or irregular, rarely conical, and which have a diameter of 0.2 inch (5 mm), or less. There are two main types:

				#. Hard grains of ice consisting of frozen raindrops, or largely melted and refrozen snowflakes.
				
				#. Pellets of snow encased in a thin layer of ice which have formed from the freezing, either of droplets intercepted by the pellets, or of water resulting from the partial melting of the pellets.

		.. index:: single: Weather phenomena; Hail

		.. cpp:enumerator:: HAIL

			Precipitation in the form of small balls or other pieces of ice falling separately or frozen together in irregular lumps.

		.. index:: single: Weather phenomena; Small hail

		.. index:: single: Weather phenomena; Snow pellets

		.. index:: single: Weather phenomena; Graupel

		.. cpp:enumerator:: SMALL_HAIL

			Precipitation of white, opaque grains of ice. The grains are round or sometimes conical. Diameters range from about 0.08 to 0.2 inch (2 to 5 mm).

			Small hail is also called 'snow pellets' or 'graupel'.

		.. index:: single: Weather phenomena; Undetermined

		.. cpp:enumerator:: UNDETERMINED

			Precipitation type that is reported if the automated station detects the occurrence of precipitation but the precipitation discriminator cannot recognize the type.

		.. index:: single: Weather phenomena; Mist

		.. cpp:enumerator:: MIST

			A visible aggregate of minute water particles suspended in the atmosphere that reduces visibility to less than 7 statute miles but greater than or equal to 5/8 statute miles.

		.. index:: single: Weather phenomena; Fog

		.. cpp:enumerator:: FOG

			A visible aggregate of minute water particles (droplets) which are based at the Earth's surface and reduces horizontal visibility to less than 5/8 statute mile and, unlike drizzle, it does not fall to the ground.

		.. index:: single: Weather phenomena; Smoke

		.. cpp:enumerator:: SMOKE

			A suspension in the air of small particles produced by combustion. A transition to haze may occur when smoke particles have traveled great distances (25 to 100 miles or more) and when the larger particles have settled out and the remaining particles have become widely scattered through the atmosphere.

		.. index:: single: Weather phenomena; Volcanic ash

		.. cpp:enumerator:: VOLCANIC_ASH

			Fine particles of rock powder that originate from a volcano and that may remain suspended in the atmosphere for long periods.

		.. index:: single: Weather phenomena; Dust

		.. cpp:enumerator:: DUST

			Widespread dust. Fine particles of earth or other matter raised or suspended in the air by the wind that may have occurred at or far away from the station which may restrict horizontal visibility.

		.. index:: single: Weather phenomena; Sand

		.. cpp:enumerator:: SAND

			Sand particles raised by the wind to a height sufficient to reduce horizontal visibility.

		.. index:: single: Weather phenomena; Haze

		.. cpp:enumerator:: HAZE

			A suspension in the air of extremely small, dry particles invisible to the naked eye and sufficiently numerous to give the air an opalescent appearance.

		.. index:: single: Weather phenomena; Spray

		.. cpp:enumerator:: SPRAY

			An ensemble of water droplets torn by the wind from the surface of an extensive body of water, generally from the crests of waves, and carried up a short distance into the air.

		.. index:: single: Weather phenomena; Well-developed dust or sand whirls

		.. index:: single: Weather phenomena; Dust devils

		.. cpp:enumerator:: DUST_WHIRLS

			Well-developed Dust/Sand Whirl. An ensemble of particles of dust or sand, sometimes accompanied by small litter, raised from the ground in the form of a whirling column of varying height with a small diameter and an approximately vertical axis.

		.. index:: single: Weather phenomena; Squalls

		.. cpp:enumerator:: SQUALLS

			A strong wind characterized by a sudden onset in which the wind speed increases at least 16 knots and is sustained at 22 knots or more for at least one minute (see paragraph 12.6.8.e.(1)).

		.. index:: single: Weather phenomena; Funnel cloud

		.. index:: single: Weather phenomena; Tornado

		.. index:: single: Weather phenomena; Waterspout

		.. cpp:enumerator:: FUNNEL_CLOUD

			Funnel cloud / tornadic activity.

				#. Tornado. A violent, rotating column of air touching the ground.

				#. Funnel Cloud. A violent, rotating column of air which does not touch the surface.

				#. Waterspout. A violent, rotating column of air that forms over a body of water, and touches the water surface.

		.. index:: single: Weather phenomena; Sandstorm

		.. cpp:enumerator:: SANDSTORM

			Sandstorm. Particles of sand carried aloft by a strong wind. The sand particles are mostly confined to the lowest ten feet, and rarely rise more than fifty feet above the ground.

		.. index:: single: Weather phenomena; Dust storm

		.. cpp:enumerator:: DUSTSTORM

			Duststorm. A severe weather condition characterized by strong winds and dust-filled air over an extensive area.

	**Acquiring group data**

		.. cpp:function:: Qualifier qualifier() const

			:returns: Weather qualifier which indicates time or intensity or proximity of the weather phenomena.
		
		.. cpp:function:: Descriptor descriptor() const

			:returns: Weather descriptor which indicates additional properties of weather phenomena.

		.. cpp:function:: std::vector<Weather> weather() const

			:returns: Vector of individual weather phenomena included in this group.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Currently always returns ``true``. The actual checks are to be added in future sversions.


.. index:: single: Group; Temperature

TemperatureGroup
^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: temperaturegroup.svg

.. cpp:class:: TemperatureGroup

	Stores information about current ambient air temperature and dew point.

	.. index:: single: Temperature; Ambient air temperature

	.. index:: single: Temperature; Dew Point

	**Acquiring group data**

		.. cpp:function:: const Temperature & airTemperature() const

			:returns: Ambient air temperature.

		.. cpp:function:: const Temperature & dewPoint() const

			:returns: Dew point.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored ambient air temperature and dew point information is valid, and ``false`` otherwise. The information is considered valid if the dew point is less or equal than ambient air temperature.


.. index:: single: Temperature; Forecast

.. index:: single: Group; Temperature forecast

TemperatureForecastGroup
^^^^^^^^^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: temperatureforecastgroup.svg

.. note:: Metaf does not verify in which order minimum and maximum temperature forecast is reported.

.. cpp:class:: TemperatureForecastGroup

	Stores information about forecast ambient air temperature along with the time when it is expected.

		..cpp:enum-class:: Point

			Temperature point for which the forecast is reported.
			
			..cpp:enumerator:: MINIMUM

				Forecast for minimum temperature is reported.

			..cpp:enumerator:: MAXIMUM

				Forecast for maximum temperature is reported.

	**Acquiring group data**

		.. cpp:function:: Point point() const

			Temperature point for which the forecast is reported.

		.. cpp:function:: const Temperature & airTemperature() const

			Forecast ambient air temperature.

		.. cpp:function:: const MetafTime & time() const

			Time when the forecast temperature is expected.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if forecast ambient air temperature information is valid, and ``false`` otherwise. The information is considered valid if the time is a valid value.


.. index:: single: Atmospheric pressure

.. index:: single: Group; Atmospheric pressure

.. index:: single: Pressure; Atmospheric

PressureGroup
^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: pressuregroup.svg

.. cpp:class:: PressureGroup

	Stores information about current atmopheric pressure.

	**Acquiring group data**

		.. cpp:function:: const Pressure & atmosphericPressure() const

			:returns: Atmospheric pressure value.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


.. index:: single: Runway visual range

.. index:: single: Group; Runway visual range

RunwayVisualRangeGroup
^^^^^^^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: runwayvisualrangegroup.svg

.. cpp:class:: RunwayVisualRangeGroup

	Stores information about visual range of a single runway.

	.. index:: single: Runway visual range; Trend

	..cpp:enum-class:: Trend
		
		Trend of runway visual range variation.

		..cpp:enumerator:: NONE

			Trend is not specified in group.

		..cpp:enumerator:: NOT_REPORTED

			Trend is specified as not reported.

		..cpp:enumerator:: UPWARD

			Trend is upward (runway visual range increases / improves).

		..cpp:enumerator:: NEUTRAL

			Trend is neutral (no significant changes to runway visual range).

		..cpp:enumerator:: DOWNWARD

			Trend is downward (runway visual range decreases / deteriorates).

	**Acquiring group data**

		.. cpp:function:: const Runway & runway() const

			:returns: Runway the visual range is provided for.

		.. cpp:function:: const Distance & visualRange() const

			:returns: Runway visual range value (if reported) or low limit of variable visual range.

		.. cpp:function:: const Distance & variableVisualRange() const

			:returns: High limit of variable visual range or non-reported value if no variable visual range is reported.

		.. cpp:function:: Trend trend() const

			:returns: Trend of runway visual range variation.

		.. cpp:function:: bool isVariableVisualRange() const

			:returns: ``true`` if variable visual range is reported and ``false`` otherwise.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if runway visual range information is valid, and ``false`` otherwise. The information is considered valid if the specified runway is valid, and visual range / variable visual range are valid distance values (if reported).


.. index:: single: Runway state

.. index:: single: Group; Runway state

RunwayStateGroup
^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: runwaystategroup.svg

.. cpp:class:: RunwayStateGroup

	Stores information about the state of runway surface and/or accumulation of deposits for a single runway. Alternatively may store information that the deposits of runway ceased to exist or that runway is closed due to snow accumulation.

	.. index:: single: Runway state; Status

	.. cpp:enum-class:: Status

		Option for the type of runway state reported: normal group with all values, CLRD group with surface friction value only, and SNOCLO group without any values.

		..cpp:enumerator:: NORMAL

			Normal type of runway state group. Runway deposits, runway contamination extent, deposit depth, and surface friction are specified in this group (any value or values may be non-reported).

		.. index:: single: Runway state; Deposits cleared

		..cpp:enumerator:: CLRD

			Runway state group indicating that previously present deposits on runway were cleared or ceased to exist. Only surface friction is specified (as an actual value or non-reported value).

		.. index:: single: Runway state; Runway closed due to snow accumulation

		..cpp:enumerator:: SNOCLO

			Runway state group indicating that the runway is closed due to snow accumulation. No further values are specified.

	.. index:: single: Runway state; Deposits

	.. cpp:enum-class:: Deposits

		Deposits on the runway.

		..cpp:enumerator:: CLEAR_AND_DRY

			No deposits; runway is clear and dry.

		..cpp:enumerator:: DAMP

			Runway is damp.

		..cpp:enumerator:: WET_AND_WATER_PATCHES

			Runway is wet and water patches are present.

		..cpp:enumerator:: RIME_AND_FROST_COVERED

			Runway is covered in frost / rime.

		..cpp:enumerator:: DRY_SNOW

			Dry snow on runway.

		..cpp:enumerator:: WET_SNOW

			Wet snow on runway.

		..cpp:enumerator:: SLUSH

			Slush on runway.

		..cpp:enumerator:: ICE

			Ice on runway.

		..cpp:enumerator:: COMPACTED_OR_ROLLED_SNOW

			Compacted or rolled snow on runway.

		..cpp:enumerator:: FROZEN_RUTS_OR_RIDGES

			Runway covered in frozen mass of snow or ice with ruts and riges.

		..cpp:enumerator:: NOT_REPORTED

			Deposits on runway are not reported.

	.. index:: single: Runway state; Contamination extent

	.. cpp:enum-class::  Extent

		The extent of runway contamination with the deposits (:cpp:enum:`Deposits`). Represents how much of total runway surface is contaminated. 

		..cpp:enumerator:: NONE

			No deposits on the runway.

		..cpp:enumerator:: LESS_THAN_10_PERCENT

			Less than 10% of runway contaminated.

		..cpp:enumerator:: FROM_11_TO_25_PERCENT

			Less than 11% to 25% of runway contaminated.

		..cpp:enumerator:: RESERVED_3

			Reserved value; should not be used.

		..cpp:enumerator:: RESERVED_4

			Reserved value; should not be used.

		..cpp:enumerator:: FROM_26_TO_50_PERCENT

			From 26% to 50% of runway contaminated.

		..cpp:enumerator:: RESERVED_6

			Reserved value; should not be used.

		..cpp:enumerator:: RESERVED_7

			Reserved value; should not be used.

		..cpp:enumerator:: RESERVED_8

			Reserved value; should not be used.

		..cpp:enumerator:: MORE_THAN_51_PERCENT

			More than 51% of runway surface 

		..cpp:enumerator:: NOT_REPORTED

			Contamination extent not reported.

	**Acquiring group data**

		.. cpp:function:: const Runway & runway() const

			:returns: Runway for which the state is provided.

		.. cpp:function:: Status status() const

			:returns: Status of runway state group. 

			If the status is :cpp:enumerator:`Status::NORMAL` then group reports runway state and Deposits, Contamination Extent, Deposit Depth and Surface Friction may be reported or non-reported.

			If the status is :cpp:enumerator:`Status::CLRD` then group indicates that deposits on the runway were cleared or ceased to exist; only Surface Friction may be reported; Deposits, Contamination Extent and Deposit Depth are never reported.

			If the status is :cpp:enumerator:`Status::SNOCLO` then group indicates that runway is closed due to snow accumulation. All parameters (Deposits, Contamination Extent, Deposit Depth and Surface Friction) are never reported.

		.. index:: single: Runway state; Deposits

		.. cpp:function:: Deposits deposits() const

			:returns: Deposits on the runway. Not reported if the status is :cpp:enumerator:`Status::CLRD` or :cpp:enumerator:`Status::SNOCLO`.

		.. index:: single: Runway state; Contamination extent

		.. cpp:function:: Extent contaminationExtent() const

			:returns: Extent (percentage) of runway contamination with deposits. Not reported if the status is :cpp:enumerator:`Status::CLRD` or :cpp:enumerator:`Status::SNOCLO`.

		.. index:: single: Runway state; Deposit depth

		.. cpp:function:: const Precipitation & depositDepth() const

			:returns: Depth of the deposits on the runway or non-reported value. Not reported if the status is :cpp:enumerator:`Status::CLRD` or :cpp:enumerator:`Status::SNOCLO`.

		.. index:: single: Runway state; Surface friction

		.. cpp:function:: const SurfaceFriction & surfaceFriction() const

		.	:returns: Surface friction or braking action or not reported value. Not reported if the status is :cpp:enumerator:`Status::SNOCLO`.

	**Validating**

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if runway state information is valid, and ``false`` otherwise. The information is considered valid if the specified runway is valid.


.. index:: single: Rainfall

.. index:: single: Group; Rainfall

.. index:: single: Precipitation; Rainfall

RainfallGroup
^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: rainfallgroup.svg

.. cpp:class:: RainfallGroup

	Stores information about recent rainfall. This group is only used in Australia.

	**Acquiring group data**

		.. cpp:function:: const Precipitation & rainfallLast10Minutes() const

			:returns: Rainfall for the last 10 minutes (or non-reported value).

		.. cpp:function:: const Precipitation & rainfallLast60Minutes() const

			:returns: Rainfall for the last 60 minutes or non-reported value.

		.. cpp:function:: const Precipitation & rainfallSince9AM() const

			:returns: Rainfall since 9:00AM (09:00) or non-reported value.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


.. index:: single: State of sea surface

.. index:: single: Group; State of sea surface

.. index:: single: Group; Wave height

SeaSurfaceGroup
^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: seasurfacegroup.svg

.. cpp:class:: SeaSurfaceGroup

	Stores information about temperature of sea surface along with descriptive state of sea surface or wave height. This group is used by oil platforms.

	**Acquiring group data**

		.. cpp:function:: const Temperature & surfaceTemperature() const

			:returns: Temperature of the sea surface or non-reported value.

		.. cpp:function:: const WaveHeight & waves() const

			:returns: Wave height or descriptive state of the sea surface or non-reported value.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


.. index:: single: Group; Colour code

ColourCodeGroup
^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: colourcodegroup.svg

.. cpp:class:: ColourCodeGroup

	Stores colour code information which allows quick assess of visibility and ceiling conditions. This group is used by military aerodromes of NATO countries.

	.. index:: single: Colour code

	..cpp:enum-class: Code

		.. index:: single: Colour code; Blue

		..cpp:enumerator:: BLUE

			Visibility >8000 m AND no cloud obscuring 3/8 or more below 2500 feet.
		
		.. index:: single: Colour code; White

		..cpp:enumerator:: WHITE

			Visibility >5000 m AND no cloud obscuring 3/8 or more below 1500 feet.

		.. index:: single: Colour code; Green

		..cpp:enumerator:: GREEN

			Visibility >3700 m AND no cloud obscuring 3/8 or more below 700 feet.

		.. index:: single: Colour code; Yellow1

		..cpp:enumerator:: YELLOW1

			Visibility >2500 m AND no cloud obscuring 3/8 or more below 500 feet.

		.. index:: single: Colour code; Yellow2

		..cpp:enumerator:: YELLOW2

			Visibility >1600 m AND no cloud obscuring 3/8 or more below 300 feet.

		.. index:: single: Colour code; Amber

		..cpp:enumerator:: AMBER

			Visibility >800 m AND no cloud obscuring 3/8 or more below 200 feet.

		.. index:: single: Colour code; Red

		..cpp:enumerator:: RED

			Visibility <800 m OR clouds obscuring 3/8 or more below 200 feet.

	**Validating**

	.. cpp:function:: bool isValid() const

		:returns: Always returns ``true``.


.. index:: single: Report part

ReportPart
----------

.. cpp:enum-class:: ReportPart

	Represents the major part of the report. Report is used by :cpp:class:`metaf::GroupParser` to  parse only group types which can potentially occur in this report part (e.g. :cpp:class:`metaf::ReportTimeGroup` is only used in report headers and NOSIG is only used in METAR report body).

		.. cpp:enumerator:: UNKNOWN

			Unknown part of the report (e.g. after syntax error was encountered).

		.. cpp:enumerator:: HEADER

			METAR or TAF report header (see :doc:`basics` for format).

		.. cpp:enumerator:: METAR

			METAR report body (see :doc:`basics` for format).

		.. cpp:enumerator:: TAF

			TAF report body (see :doc:`basics` for format).

		.. cpp:enumerator:: RMK

			Remarks at the end of METAR or TAF report.


.. index:: single: Group parser

GroupParser
-----------

.. cpp:class:: GroupParser

	The purpose of this class is to parse a single METAR or TAF group. To parse entire METAR or TAF report use :cpp:class:`metaf::Parser`.

	.. cpp:function:: static Group parse(const std::string & group, ReportPart reportPart)

		:param group: Source string which contains a single METAR or TAF group.

		:param reportPart: Report part to which the source string belongs.

		:returns: :cpp:type:`metaf::Group` holding a particular group type or :cpp:class:`metaf::PlainTextGroup` if the format was not recognised.



.. index:: single: Parser

Parser
------

.. cpp:class:: Parser

	Parser class is used to parse strings which contain raw METAR or TAF reports, check for syntax errors, autodetect report type and produce a vector of :cpp:type:`metaf::Group`.

	.. index:: single: Parser; Error

	The complete list of the errors reported by the parser is as follows.

	.. cpp:enum-class:: Error

		Errors which may occur when parsing a METAR or TAF report.

		.. cpp:enumerator:: NONE

			No error, report parsed successfully.

			.. note: `No error` only means that the report overall syntax is correct and the report is not malformed. `No error` does not guarantee that all groups were recognised by the parser. Unrecognised groups are treated as Plain Text Groups (see :cpp:class:`metaf::PlainTextGroup`).


		.. cpp:enumerator:: EMPTY_REPORT

			The report source string is empty or contains only report end designator ``=``.


		.. cpp:enumerator:: EXPECTED_REPORT_TYPE_OR_LOCATION

			The parser is expecting the report to start with either report type or ICAO location but the report starts with some other group.

			At the beginning of the report the report type should be specified as follows:
			
				- METAR or SPECI: the report type is METAR.
				- TAF: the report type is TAF.

			However in practice report type in both METARs and TAFs may be omitted.

			If the report type is missing then the report must begin with the ICAO location group (see :cpp:class:`metaf::LocationGroup`).

			AMD or COR groups are not allowed at the beginning of the report (i.e. if report type is missing).

			See also :doc:`basics` for the complete METAR and TAF report format.


		.. cpp:enumerator:: EXPECTED_LOCATION

			The parser expects an ICAO location group (see :cpp:class:`metaf::LocationGroup`) in this position but encounters some other group.


		.. cpp:enumerator:: EXPECTED_REPORT_TIME

			The parser expects a report release time group (see :cpp:class:`metaf::ReportTimeGroup`) in this position but encounters some other group.


		.. cpp:enumerator:: EXPECTED_TIME_SPAN

			The parser expects a time span group (see :cpp:class:`metaf::TrendGroup` and :cpp:enumerator:`metaf::TrendGroup::Type::TIME_SPAN`) in this position but encounters some other group.

			This error occurs when the validity time is not specified for the TAF report.


		.. cpp:enumerator:: UNEXPECTED_REPORT_END

			The report should not end at this position, more groups are expected according to the report format (see :doc:`basics`) but actually are missing. This error occurs if either stray report end designator (``=``) is placed in the middle of the report or if only part of the report is included in the source string.


		.. cpp:enumerator:: UNEXPECTED_GROUP_AFTER_NIL

			This error occurs if any group is encountered after NIL. (see :cpp:enumerator:`metaf::FixedGroup::NIL`).

			.. note: NIL means missing report, thus including groups in report body are not allowed.


		.. cpp:enumerator:: UNEXPECTED_GROUP_AFTER_CNL

			This error occurs if any group is encountered after CNL. (see :cpp:enumerator:`metaf::FixedGroup::CNL`).

			.. note: CNL means canceled report, thus including groups in report body are not allowed.


		.. cpp:enumerator:: UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY

			This error occurs if NIL or CNL are found in the middle of non-empty reports (see :cpp:enumerator:`metaf::FixedGroup::NIL` and :cpp:enumerator:`metaf::FixedGroup::CNL`).

			.. note: NIL means missing report and CNL means canceled report; these groups must not be included is the report which contains any actual observation or forecast.


		.. cpp:enumerator:: AMD_ALLOWED_IN_TAF_ONLY

			Group AMD which designates amended report (see :cpp:enumerator:`metaf::FixedGroup::AMD`) is only used in TAF reports. This error occurs if AMD is encountered in a METAR report.

			.. note: COR (see :cpp:enumerator:`metaf::FixedGroup::COR`) may be used in both METAR and TAF reports.

		.. cpp:enumerator:: CNL_ALLOWED_IN_TAF_ONLY

			Group CNL which designates canceled report (see :cpp:enumerator:`metaf::FixedGroup::CNL`) is only used in TAF reports. 

			Since METAR reports contain the actual weather observation, canceling a METAR report is a semantic error.

			This error occurs if CNL is encountered in a METAR report in place of NIL.

		.. cpp:enumerator:: INTERNAL_PARSER_STATE

			This error means that the state machine used by parser to detect the report syntax has switched to undefined state. This error is a safeguard for the extension of the syntax parser and should never occur in current version.

			.. note: If such error is ever encountered please report it to author at `Gitlab <https://gitlab.com/nnaumenko/metaf>` or `Github <https://github.com/nnaumenko/metaf>` along with the source report string that has caused it.


		.. cpp:function:: bool parse(const std::string & report, bool keepSourceGroup = false)

			Parses a METAR or TAF report, checks its syntax, detects report type and parses each group separately.

			After calling this method, the results are stored within ``Parser`` class. Use :cpp:func:`getReportType()`, :cpp:func:`getError()`, :cpp:func:`getResult()` and :cpp:func:`getSourceGroups()` to access the parsed report or :cpp:func:`resetResult()` to release memory occupied by the results of the parse and reset the results of the parse to default state.

			.. note:: There is no need to call :cpp:func:`resetResult()` before parsing the next METAR or TAF report.

			:returns: ``true`` if METAR or TAF report was parsed successfully and ``false`` if syntax error was encountered and only the part of the METAR or TAF report before syntax error was parsed.

			.. note:: If report is parsed successfully, it does not guarantee that all groups were recognised by the parser. Unrecognised groups are treated as Plain Text Groups (see :cpp:class:`metaf::PlainTextGroup`).

			.. note:: Use :cpp:func:`getError()` to check for exact error.

			:param report: String which contains a METAR or TAF report.

			:param keepSourceGroups: if ``true`` then the parser will also keep the unparsed string for each METAR/TAF group. If the groups are combined, the strings for both groups are concanenated (groups separated by a single space). For example, groups ``1`` and ``1/2SM`` will be combined and source string ``1 1/2SM`` will be kept.


		.. cpp:function:: Error getError() const

			:returns: Syntax error encountered by parser during parse of last report or :cpp:enumerator:`metaf::Parser::Error::NONE` if the report was parsed successfully. If no report was previously parsed or :cpp:func:`resetResult()` was previously called, :cpp:enumerator:`metaf::Parser::Error::NONE` is returned.


		.. cpp:function:: ReportType getReportType() const

			:returns: Report type (:cpp:enumerator:`metaf::ReportType::METAR` or :cpp:enumerator:`metaf::ReportType::TAF`) autodetected by parser during parse of the last report. If no report was previously parsed or :cpp:func:`resetResult()` was previously called, :cpp:enumerator:`metaf::ReportType::UNKNOWN` is returned.


		.. cpp:function:: const std::vector<Group> & getResult() const

			:returns: A reference to vector of parsed individual groups from last parsed METAR or TAF report. If no report was previously parsed or :cpp:func:`resetResult()` was previously called, an empty vector is returned.

			.. warning:: The reference obtained by this method is only valid as long as an instance of :cpp:class:`metaf::Parser` still exists and no other METAR or TAF report is parsed by the same instance. If :cpp:func:`metaf::Parser::parse()` is executed again, the reference returned by this method becomes invalid.


		.. cpp:function:: const std::vector<std::string> & getSourceGroups() const

			:returns: A reference to vector of unparsed individual group strings from last parsed METAR or TAF report. If no report was previously parsed or :cpp:func:`resetResult()` was previously called or during last , an empty vector is returned.

			.. warning:: The reference obtained by this method is only valid as long as an instance of :cpp:class:`metaf::Parser` still exists and no other METAR or TAF report is parsed by the same instance. If :cpp:func:`metaf::Parser::parse()` is executed again, the reference returned by this method becomes invalid.


		.. cpp:function:: void resetResult()

			Resets last parse result. After this method is called, the parse result obtained by :cpp:func:`getReportType()`, :cpp:func:`getError()`, :cpp:func:`getResult()` and :cpp:func:`getSourceGroups()`. 

			The report type will be set to :cpp:enumerator:`metaf::ReportType::UNKNOWN`, the parse error set to :cpp:enumerator:`metaf::Parser::Error::NONE` and Result and SourceGroups vectors are emptied and memory occupied by their contents is released.

.. index:: single: Group visitor

GroupVisitor
------------

The purpose of Group Visitor is to simplify processing the results of the parsing.

Since the METAR or TAF report is parsed into the vector of type :cpp:type:`metaf::Group`, and the :cpp:type:`metaf::Group` is a variant data type, in order to handle the results the variant alternative check is required, after which the correct type can be extracted from the variant and the concrete group type can be handled.

The Group Visitor checks the type of an alternative in :cpp:type:`metaf::Group` and calls a corresponding virtual method for the concrete group type. The virtual methods are pure to make sure there is no risk of accidentally missing the handling of a particular group type.

See :doc:`getting_started` for more information.

.. cpp:class:: template <typename T> GroupVisitor

	:tparam T: Type returned by virtual methods for concrete group types. Can be ``void`` if virtual methods do not need to return a value.

	.. cpp:function:: T visit(const Group & group)

		Checks concrete group type and calls one of the virtual methods below.

	.. cpp:function:: protected virtual T visitPlainTextGroup(const PlainTextGroup & group) = 0


	.. cpp:function:: protected virtual T visitFixedGroup(const FixedGroup & group) = 0


	.. cpp:function:: protected virtual T visitLocationGroup(const LocationGroup & group) = 0


	.. cpp:function:: protected virtual T visitReportTimeGroup(const ReportTimeGroup & group) = 0


	.. cpp:function:: protected virtual T visitTrendGroup(const TrendGroup & group) = 0


	.. cpp:function:: protected virtual T visitWindGroup(const WindGroup & group) = 0


	.. cpp:function:: protected virtual T visitVisibilityGroup(const VisibilityGroup & group) = 0
	

	.. cpp:function:: protected virtual T visitCloudGroup(const CloudGroup & group) = 0
	

	.. cpp:function:: protected virtual T visitWeatherGroup(const WeatherGroup & group) = 0
	

	.. cpp:function:: protected virtual T visitTemperatureGroup(const TemperatureGroup & group) = 0
	

	.. cpp:function:: protected virtual T visitTemperatureForecastGroup(const TemperatureForecastGroup & group) = 0


	.. cpp:function:: protected virtual T visitPressureGroup(const PressureGroup & group) = 0
	

	.. cpp:function:: protected virtual T visitRunwayVisualRangeGroup(const RunwayVisualRangeGroup & group) = 0	


	.. cpp:function:: protected virtual T visitRunwayStateGroup(const RunwayStateGroup & group) = 0
	

	.. cpp:function:: protected virtual T visitRainfallGroup(const RainfallGroup & group) = 0


	.. cpp:function:: protected virtual T visitSeaSurfaceGroup(const SeaSurfaceGroup & group) = 0
	

	.. cpp:function:: protected virtual T visitColourCodeGroup(const ColourCodeGroup & group) = 0
	

	These methods are called by :cpp:func:`visit()` for the concrete group types. See :doc:`getting_started` for usage example.

	.. cpp:function:: protected virtual T visitOther(const Group & group) = 0

	This method is called if :cpp:func:`visit()` cannot recognise an alternative in the Group variant.