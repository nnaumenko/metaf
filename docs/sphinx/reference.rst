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

The following syntax of corresponds to this group in METAR/TAF reports.

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

The following syntax of corresponds to this group in METAR/TAF reports.

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

The following syntax of corresponds to this group in METAR/TAF reports.

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

The following syntax of corresponds to this group in METAR/TAF reports.

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

WindGroup
^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: windgroup.svg




.. index:: single: Group; Surface wind

.. index:: single: Wind; Surface wind

.. index:: single: Wind; Variable

.. index:: single: Wind shear

.. index:: single: Group; Wind shear

.. index:: single: Wind; Speed

.. index:: single: Wind; Gust speed

.. index:: single: Wind; Direction

.. index:: single: Wind; Variable wind direction sector

.. index:: single: Group; Variable wind direction sector




.. index:: single: Visibility

.. index:: single: Group; Visibility

VisibilityGroup
^^^^^^^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: visibilitygroup.svg

.. index:: single: Visibility; Prevailing

.. index:: single: Visibility; Directional



.. index:: single: Cloud

CloudGroup
^^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: cloudgroup.svg

.. index:: single: Group; Cloud layer

.. index:: single: Cloud; Layer

.. index:: single: Cloud; Clear sky

.. index:: single: Group; Clear sky

.. index:: single: Clear sky

.. index:: single: Cloud; Nil significant cloud

.. index:: single: Cloud; No cloud detected

.. index:: single: Cloud; Few

.. index:: single: Cloud; Scattered

.. index:: single: Cloud; Broken

.. index:: single: Cloud; Overcast

.. index:: single: Cloud; Vertical visibility

.. index:: single: Group; Vertical Visibility

.. index:: single: Visibility; Vertical

.. index:: single: Cloud; Base height

.. index:: single: Cloud; Towering cumulus

.. index:: single: Cloud; Cumulonimbus


.. index:: single: Weather phenomena

.. index:: single: Group; Weather phenomena

WeatherGroup
^^^^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: weathergroup.svg


.. index:: single: Weather phenomena; Qualifier

.. index:: single: Weather phenomena; Recent

.. index:: single: Recent weather

.. index:: single: Group; Recent weather

.. index:: single: Weather phenomena; Proximity

.. index:: single: Weather phenomena; Intensity

.. index:: single: Precipitation; Intensity

.. index:: single: Weather phenomena; Descriptor

.. index:: single: Weather descriptor

.. index:: single: Weather phenomena; Precipitation

.. index:: single: Precipitation

.. index:: single: Weather phenomena; Obscuration

.. index:: single: Obscuration

.. index:: single: Weather phenomena; Other weather

.. index:: single: Weather descriptor; Shallow

.. index:: single: Weather descriptor; Partial

.. index:: single: Weather descriptor; Patches

.. index:: single: Weather descriptor; Low drifting

.. index:: single: Weather descriptor; Blowing

.. index:: single: Weather descriptor; Showers

.. index:: single: Precipitation; Showers

.. index:: single: Weather phenomena; Showers

.. index:: single: Weather descriptor; Thunderstorm

.. index:: single: Weather phenomena; Thunderstorm

.. index:: single: Weather descriptor; Freezing

.. index:: single: Precipitation; Drizzle

.. index:: single: Precipitation; Rain

.. index:: single: Precipitation; Snow

.. index:: single: Precipitation; Snow grains

.. index:: single: Precipitation; Ice pellets

.. index:: single: Precipitation; Ice crystals (diamond dust)

.. index:: single: Precipitation; Hail

.. index:: single: Precipitation; Small hail (graupel)

.. index:: single: Precipitation; Undetermined

.. index:: single: Obscuration; Blowing snow

.. index:: single: Obscuration; Mist

.. index:: single: Obscuration; Fog

.. index:: single: Obscuration; Smoke

.. index:: single: Obscuration; Volcanic ash

.. index:: single: Obscuration; Widespread dust

.. index:: single: Obscuration; Blowing dust

.. index:: single: Obscuration; Sand

.. index:: single: Obscuration; Haze

.. index:: single: Obscuration; Blowing spray

.. index:: single: Other weather; Dust or sand whirls (dust devils)

.. index:: single: Other weather; Squalls

.. index:: single: Wind; Squalls

.. index:: single: Other weather; Funnel cloud

.. index:: single: Cloud; Funnel

.. index:: single: Other weather; Tornado

.. index:: single: Other weather; Waterspout

.. index:: single: Other weather; Dust storm

.. index:: single: Other weather; Sand storm


.. index:: single: Group; Temperature

TemperatureGroup
^^^^^^^^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: temperaturegroup.svg

.. index:: single: Temperature; Ambient air temperature

.. index:: single: Temperature; Dew Point



.. index:: single: Temperature; Forecast

.. index:: single: Group; Temperature forecast

TemperatureForecastGroup
^^^^^^^^^^^^^^^^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: temperatureforecastgroup.svg


.. index:: single: Atmospheric pressure

.. index:: single: Group; Atmospheric pressure

.. index:: single: Pressure; Atmospheric

PressureGroup
^^^^^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: pressuregroup.svg



.. index:: single: Runway visual range

.. index:: single: Group; Runway visual range

RunwayVisualRangeGroup
^^^^^^^^^^^^^^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: runwayvisualrangegroup.svg

.. index:: single: Runway visual range; Trend


.. index:: single: Runway state

.. index:: single: Group; Runway state

RunwayStateGroup
^^^^^^^^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: runwaystategroup.svg

.. index:: single: Runway state; Deposits

.. index:: single: Runway state; Contamination extent

.. index:: single: Runway state; Deposit depth

.. index:: single: Runway state; Surface friction

.. index:: single: Runway state; Runway closed due to snow accumulation

.. index:: single: Runway state; Deposits cleared



.. index:: single: Rainfall

.. index:: single: Group; Rainfall

RainfallGroup
^^^^^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: rainfallgroup.svg

.. index:: single: Precipitation; Rainfall


.. index:: single: State of sea surface

.. index:: single: Group; State of sea surface

.. index:: single: Group; Wave height

SeaSurfaceGroup
^^^^^^^^^^^^^^^

The following syntax of corresponds to this group in METAR/TAF reports.

.. image:: seasurfacegroup.svg

.. index:: single: Colour code

.. index:: single: Group; Colour code

ColourCodeGroup
^^^^^^^^^^^^^^^

.. image:: colourcodegroup.svg

.. index:: single: Colour code; Blue

.. index:: single: Colour code; White

.. index:: single: Colour code; Green

.. index:: single: Colour code; Yellow1

.. index:: single: Colour code; Yellow2

.. index:: single: Colour code; Amber

.. index:: single: Colour code; Red



.. index:: single: Report part

ReportPart
----------

.. cpp:enum-class:: ReportPart

	Represents the major part of the report. Report is used by :cpp:class:`metaf::GroupParser` to only parse group types which can potentially occur in this report part (e.g. :cpp:class:`metaf::ReportTimeGroup` is only used in report headers and NOSIG is only used in METAR report body).

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

	The purpose of this class is to parse a single METAR or TAF group.

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

				.. note: `No error` only means that the report overall syntax is correct and the report is not malformed. `No error` does not mean that all groups were recognised by the parser. Unrecognised groups are treated as Plain Text Groups (see :cpp:class:`metaf::PlainTextGroup`).


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


		.. cpp:function:: void resetResult()


		.. cpp:function:: ReportType getReportType() const


		.. cpp:function:: Error getError() const


		.. cpp:function:: const std::vector<Group> & getResult() const


		.. cpp:function:: const std::vector<std::string> & getSourceGroups() const


.. index:: single: Group visitor

GroupVisitor
------------

The purpose of Group Visitor is to simplify processing the results of the parsing.

Since the METAR or TAF report is parsed into the vector of type :cpp:type:`metaf::Group`, and the :cpp:type:`metaf::Group` is a variant data type, in order to handle the results the variant alternative check is required, after which the correct type can be extracted from the variant and the concrete group type can be handled.

The Group Visitor checks the type of an alternative in :cpp:type:`metaf::Group` and calls a corresponding virtual method for the concrete group type. The virtual methods are pure so there is no risk of accidentally missing the handling of a particular group type.

.. cpp:class:: template <typename T> GroupVisitor

	:tparam T: Type returned by virtual methods for concrete group types. Can be ``void`` if virtual methods do not need to return a value.

	.. cpp:function:: T visit(const Group & group)

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
	

	.. cpp:function:: protected virtual T visitOther(const Group & group) = 0

