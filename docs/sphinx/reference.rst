Reference
=========

.. cpp:namespace-push:: metaf

This section describes the APIs related to parsing METAR or TAF report and reading the data from parsed results.

See Extending Metaf Functionality section for APIs related to adding additional features and new groups.

Data Types
----------

This section lists the types which store the data, such as temperature, direction, time, etc.

Runway
^^^^^^
.. index::
	single: Runway identification

.. cpp:class:: Runway

	Provides runway identification in form of runway number and designator.

	The runway number is formed by the first two digits of runway heading (which is 0 to 360 degrees). The valid range for runway number is thus 0 to 36.

	The following special codes can also be used for runway number:

	====== ======================= =================================
	Number Meaning                 Associated method
	====== ======================= =================================
	88     All runways             :cpp:func:`isAllRunways()`
	99     Last message repetition :cpp:func:`isMessageRepetition()`
	====== ======================= =================================

	Parallel runways are distinguished by designating them as a left / center / right runway (e.g. runways ``21 right`` and ``21 left`` are two parallel runways with heading 210 degrees).

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


MetafTime
^^^^^^^^^

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


Temperature
^^^^^^^^^^^

.. cpp:class:: Temperature

	Stores a temperature value. Temperature value may be not reported (i.e. no value).

	If the temperature value is rounded to zero, an additional information can be acquired whether the value represents a freezing or non-freezing temperature (i.e. slightly above or slightly below zero).

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


Speed
^^^^^
.. cpp:class:: Speed

	Stores a speed value. Speed value can be optionally not reported (i.e. no value).

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


Distance
^^^^^^^^

.. cpp:class:: Distance

	Stores a distance or height value. The value may be expressed as integer (e.g. 3500) or rational (e.g. 2 1/4) number.

	The value consists of integer and fraction components, fraction component in turn consists of numerator and denominator. Integer or fraction parts can be optionally not reported (i.e. no value).

	.. cpp:enum-class:: Unit

		Distance measurement units.

		.. cpp:enumerator:: METERS

			Meters.

		.. cpp:enumerator:: STATUTE_MILES

			Statute miles.

		.. cpp:enumerator:: FEET

			Feet.

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

Direction
^^^^^^^^^

.. cpp:class:: Direction

	Stores a direction value. The value can be specified in degrees or as a cardinal direction. Alternatively the direction value may be omitted (i.e. not specified), specified as not reported (i.e. no value), specified as variable, or specified as No Directional Variation.

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

	.. cpp:enum-class:: Status

			The status of the direction value reported. If the status is other than :cpp:enumerator:`VALUE_DEGREES` or :cpp:enumerator:`VALUE_CARDINAL`, then no numerical direction value is provided.

			.. cpp:enumerator:: OMMITTED

				Direction is omitted (i.e. no direction specified at all).

			.. cpp:enumerator:: NOT_REPORTED

				Direction is specified as 'not reported'.

			.. cpp:enumerator:: VARIABLE

				Direction is reported as variable.

			.. cpp:enumerator:: NDV

				Direction is reported as 'No Directional Variation'.

			.. cpp:enumerator:: VALUE_DEGREES

				Direction is reported as value in degrees.

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

Pressure
^^^^^^^^

.. cpp:class:: Pressure

	Stores a pressure value. The intended use is atmospheric pressure but any absolute pressure value can be stored.

	Pressure value can be optionally not reported (i.e. no value).

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


Precipitation
^^^^^^^^^^^^^

.. cpp:class:: Precipitation

	The amount or accumulation of precipitation. 

	The amount/accumulation may be not reported (i.e. no value) or alternatively it may specify that the runway is not operational due to deposits accumulation (which means that exact accumulation value is not important and is not reported).

	.. cpp:enum-class:: Status

		Status of precipitation value.

		.. cpp:enumerator:: NOT_REPORTED

			The amount or accumulation of precipitation is not reported (i.e. no value).

		.. cpp:enumerator:: REPORTED

			The amount or accumulation of precipitation value is reported.

		.. cpp:enumerator:: RUNWAY_NOT_OPERATIONAL

			The runway is not operational due to deposits accumulation; the exact accumulation value is not important since runway cannot be used.

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


SurfaceFriction
^^^^^^^^^^^^^^^
.. cpp:class:: SurfaceFriction

	Surface friction. The intended use is reporting the surface friction of the runway. Surface friction may be reported in the form of friction coefficient or braking action.

	The value may be optionally not reported, or reported as unreliable/unmeasurable.

	.. note:: Surface friction coefficient is a dimensionless value and has no associated measurement units.

	.. cpp:enum-class:: Status

		The status of surface friction value.

		.. cpp:enumerator:: NOT_REPORTED

			Surface friction is not reported.

		.. cpp:enumerator:: SURFACE_FRICTION_REPORTED

			Surface friction reported in form of friction coefficient (a value from 0.00 to 1.00, lesser values mean more slippery surface).

		.. cpp:enumerator:: BRAKING_ACTION_REPORTED

			Surface friction reported in form of braking action.

		.. cpp:enumerator:: UNRELIABLE

			The measurement result is unreliable or the value is unmeasurable.

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


WaveHeight
^^^^^^^^^^
.. cpp:class:: WaveHeight

	WaveHeight or descriptive state of sea surface which specifies the range of wave heights.

	Both state of sea surface and wave height may be optionally not reported (i.e. no value).

	.. cpp:enum-class:: Type

		.. cpp:enumerator:: STATE_OF_SURFACE

			Descriptive state of surface is specified.

		.. cpp:enumerator:: WAVE_HEIGHT

			Actual numerical wave height is specified.
	
	.. cpp:enum-class:: Unit

		Wave height measurement unit.

		.. cpp:enumerator:: METERS

			Meters.

		.. cpp:enumerator:: FEET

			Feet.

	.. cpp:enum-class:: StateOfSurface

		.. cpp:enumerator:: NOT_REPORTED

			State of sea surface is not reported.

		.. cpp:enumerator:: CALM_GLASSY

			Sea surface calm (glassy), no waves.

		.. cpp:enumerator:: CALM_RIPPLED

			Sea surface calm (rippled), wave height <0.1 meters.

		.. cpp:enumerator:: SMOOTH

			Sea surface smooth, wave height 0.1 to 0.5 meters.

		.. cpp:enumerator:: SLIGHT

			Slight waves with height 0.5 to 1.25 meters.

		.. cpp:enumerator:: MODERATE

			Moderate waves with height 1.25 to 2.5 meters.

		.. cpp:enumerator:: ROUGH

			Sea surface rough, wave height 2.5 to 4 meters.

		.. cpp:enumerator:: VERY_ROUGH

			Sea surface very rough, wave height 4 to 6 meters.

		.. cpp:enumerator:: HIGH

			High waves with height 6 to 9 meters.

		.. cpp:enumerator:: VERY_HIGH

			Very high waves with height 9 to 14 meters.

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


Group
-----

Group types
-----------

PlaintextGroup
^^^^^^^^^^^^^^

FixedGroup
^^^^^^^^^^

.. image:: fixedgroup.svg

LocationGroup
^^^^^^^^^^^^^
.. image:: locationgroup.svg

ReportTimeGroup
^^^^^^^^^^^^^^^
.. image:: reporttimegroup.svg

TrendGroup
^^^^^^^^^^

.. image:: trendgroup.svg

WindGroup
^^^^^^^^^

.. image:: windgroup.svg

VisibilityGroup
^^^^^^^^^^^^^^^

.. image:: visibilitygroup.svg

CloudGroup
^^^^^^^^^^

.. image:: cloudgroup.svg

WeatherGroup
^^^^^^^^^^^^

.. image:: weathergroup.svg

TemperatureGroup
^^^^^^^^^^^^^^^^

.. image:: temperaturegroup.svg

TemperatureForecastGroup
^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: temperatureforecastgroup.svg

PressureGroup
^^^^^^^^^^^^^

.. image:: pressuregroup.svg

RunwayVisualRangeGroup
^^^^^^^^^^^^^^^^^^^^^^

.. image:: runwayvisualrangegroup.svg

RunwayStateGroup
^^^^^^^^^^^^^^^^

.. image:: runwaystategroup.svg

RainfallGroup
^^^^^^^^^^^^^
.. image:: rainfallgroup.svg

SeaSurfaceGroup
^^^^^^^^^^^^^^^

.. image:: seasurfacegroup.svg

ColourCodeGroup
^^^^^^^^^^^^^^^

.. image:: colourcodegroup.svg

GroupParser
-----------

Parser
------

GroupVisitor
------------

