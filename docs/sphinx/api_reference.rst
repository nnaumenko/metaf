API Reference
=============

.. cpp:namespace-push:: metaf

This section describes the APIs related to parsing METAR or TAF report and reading the data from parsed results.

Data Types
----------

This section lists the types which store the data, such as temperature, direction, time, etc.


Runway
^^^^^^
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

	**Miscellaneous**

		.. cpp:function:: bool is3hourlyReportTime() const

			:returns: ``true`` if the this time qualifies as 3-hourly report release time in North America (i.e. reports issued within one hour before or after 0300Z, 0900Z, 1500Z, and 2100Z), according to Field Meteorology Handbook Number 1, chapter 12.4.


		.. cpp:function:: bool is6hourlyReportTime() const

			:returns: ``true`` if the this time qualifies as 6-hourly report release time in North America (i.e. reports issued within one hour before or after 0000Z, 0600Z, 1200Z, and 1800Z), according to Field Meteorology Handbook Number 1, chapter 12.4.

		.. cpp:struct Date

			.. cpp:var:: unsigned int year

			Year component of the date. Can be specified as e.g. ``2019`` or just ``19``. Assumed to be a mon-zero value.

			.. cpp:var:: unsigned int month

			Month component of the date. Must be in range 1 to 12.

			.. cpp:var:: unsigned int day

			Day-of-month component of the date.

		.. cpp:function:: Date dateBeforeRef(const Date & refDate) const

			This function compliments MetafTime with year and month, based on supplied reference date, assuming that MetafTime refers to the time point at maximum 1 month before the supplied reference date.

			:param refDate: Reference date; may equal current date for recent METAR or TAF report or date of METAR or TAF report retreival for archived/historical report.

			:returns: Date including year and month for MetafTime instance, assuming that point of time info stored in this MetafTime instance is before refDate.

			.. note:: This method does not validate supplied current date and day-of-month stored in the group. The inconsistensy of both input parameters and returned value (such as day-of-month exceeding max days in this month, month being in range 1 to 12, etc.) must be checked by others than Metaf.

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

	Depending on the reported type, a temperature value may be precise (i.e. in tenth of degrees Celsius) or non-precise (i.e. rounded to integer value in degrees Celsius).

	If the non-precise temperature value is rounded to zero, an additional information can be acquired whether the value represents a freezing or non-freezing temperature (i.e. slightly above or slightly below zero).

	.. cpp:enum-class:: Unit

		Temperature measurement units.

		.. cpp:enumerator:: C

			Degrees Celsius.

		.. cpp:enumerator:: F

			Degrees Fahrenheit.

		.. note:: Temperature value is always stored in degrees Celsius but may be converted to degrees Fahrenheit using :cpp:func:`toUnit()`.


	**Acquiring the data**

		.. cpp:function:: std::optional<float> temperature() const

			:returns: Stored temperature value or empty ``std::optional`` if temperature value is not reported.


		.. cpp:function:: Unit unit() const

			:returns: Temperature measurement unit which was used with stored value. Currently always returns :cpp:enumerator:`Unit::C` since the value is always stored in degrees Celsius.


	**Converting to other measurement units**

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.
			:returns: Stored temperature value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


	**Additional values**

		.. cpp:function:: static std::optional<float> relativeHumidity(const Temperature & airTemperature, const Temperature & dewPoint)

			:param airTemperature: Ambient air temperature.

			:param dewPoint: Dew point.

			:returns: Relative humidity value based on ambient air temperature and dew point or empty ``std::optional`` if ambient air temperature and/or dew point is not reported.

		.. cpp:function:: static Temperature heatIndex(const Temperature & airTemperature, float relativeHumidity)

			:param airTemperature: Ambient air temperature.

			:param relativeHumidity: Relative humidity value in range 0.0 .. 100.0.

			:returns: Heat index (perceived temperature adjusted for humidity) value based on ambient air temperature and relative humidity or empty ``std::optional`` if ambient air temperature is not reported. An empty ``std::optional`` is also returned for the conditions where heat index is not defined, i.e. relative himidity values below 40% or above 100% or the temperature values below 27 degrees Celsius.

		.. cpp:function:: static Temperature heatIndex(const Temperature & airTemperature, const Temperature & dewPoint)

			:param airTemperature: Ambient air temperature.

			:param dewPoint: Dew point.

			:returns: Heat index (perceived temperature adjusted for humidity) value based on ambient air temperature and dew point or empty ``std::optional`` if ambient air temperature and/or dew point is not reported. Dewpoint and ambient air temperature values are used to calculate relative humidity. An empty ``std::optional`` is returned for the conditions where heat index is not defined, i.e. relative himidity values below 40% or above 100% or the temperature values below 27 degrees Celsius.

		.. cpp:function:: windChill(const Temperature & airTemperature, const Speed & windSpeed)

			:param airTemperature: Ambient air temperature.

			:param windSpeed: Wind speed.

			:returns: Wind chill (perceived temperature adjusted for heat loss due to wind) value based on ambient air temperature and wind speed or empty ``std::optional`` if ambient air temperature and/or wind speed is not reported. An empty ``std::optional`` is also returned for the conditions where wind chill is not defined, i.e. temperature values above 10 degrees Celsius and/or wind speed below 4.8 km/h.

	**Miscellaneous**

		.. cpp:function:: bool isFreezing() const

			:returns: ``true`` if the stored temperature value is below water freezing point (0 degrees Celsius or 32 degrees Fahrenheit), and ``false`` if the stored temperature value is above freezing point.

				If the temperature value is rounded to the freezing point, isFreezing() may return either ``true`` or ``false`` based on the following conditions:

				- if the original temperature value was in range (0.0 .. -0.5) which is encoded in METAR/TAF report as ``M00``, then ``isFreezing()`` returns ``true``;

				- if the original temperature value was in range (0.5 .. 0.0] which is encoded in METAR/TAF report as ``00``, then ``isFreezing()`` returns ``false``;


		.. cpp:function:: bool isReported() const

			:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored.


		.. cpp:function:: bool isPrecise() const

			:returns: ``true`` if the value is in tenth of degree Celsius, ``false`` if the value is rounded to integer.


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


	**Miscellaneous**

		.. cpp:function:: bool isReported() const

			:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored.


Distance
^^^^^^^^

.. cpp:class:: Distance

	Stores a distance or height value. The value may be expressed as meters, feet (e.g. 3500 or 1500) or in statute miles (e.g. 2 1/4 or 2.25). The value in statute miles may contain integer and/or fraction parts.

	.. cpp:enum-class:: Unit

		Distance measurement units.

		.. cpp:enumerator:: METERS

			Meters.

		.. cpp:enumerator:: STATUTE_MILES

			Statute miles.

		.. cpp:enumerator:: FEET

			Feet.

	.. cpp:enum-class:: Modifier

		Modifier is used when the value other than the exact distance, is stored (e.g. minimum or maximum value). Modifier is used to report distances such as 'less than 1/4 Statute Mile', 'more than 10000 meters', 'distant', 'in vicinity'.

		.. cpp:enumerator:: NONE

			No modifier; exact value is reported.

		.. cpp:enumerator:: LESS_THAN

			The distance is less than reported value.

		.. cpp:enumerator:: MORE_THAN

			The distance is more than reported value.

		.. cpp:enumerator:: DISTANT

			The distance is 10 to 30 nautical miles (coded ``DSNT`` in remark groups). The exact value is not reported.

		.. cpp:enumerator:: VICINITY

			The distance is 5 to 10 nautical miles (coded ``VC`` in remark groups). The exact value is not reported.

	.. cpp:enum-class:: MilesFraction

		Identifies reportable values for the fraction of mile, as per Table 6-1 of Field Meteorology Handbook Number 1.

		.. cpp:enumerator:: NONE

			The value is integer; no fraction.

		.. cpp:enumerator:: F_1_16

			Fraction value of 1/16.

		.. cpp:enumerator:: F_1_8

			Fraction value of 1/8.

		.. cpp:enumerator:: F_3_16

			Fraction value of 3/16.

		.. cpp:enumerator:: F_1_4

			Fraction value of 1/4.

		.. cpp:enumerator:: F_5_16

			Fraction value of 5/16.

		.. cpp:enumerator:: F_3_8

			Fraction value of 3/8.

		.. cpp:enumerator:: F_1_2

			Fraction value of 1/2.

		.. cpp:enumerator:: F_5_8

			Fraction value of 5/8.

		.. cpp:enumerator:: F_3_4

			Fraction value of 3/4.

		.. cpp:enumerator:: F_7_8

			Fraction value of 7/8.

	**Acquiring the data**

		.. cpp:function:: std::optional<float> distance() const

			:returns: Stored distance value in the units specified by :cpp:func:`Distance::unit()` or empty ``std::optional`` if the value is not reported.

			.. note:: This method returns a decimal value in statute miles (e.g. 2.5). If instead an integer & fraction value in statute miles is required, use :cpp:func:`Distance::miles()`.

		.. cpp:function:: Modifier modifier() const

			:returns: Modifier of stored distance value (less than / more than).

		.. cpp:function:: Unit unit() const

			:returns: Distance measurement unit which was used with stored value.

	**Miscellaneous**

		.. cpp:function:: bool isValue()

			:returns: ``true`` if integer component or both numerator and denomerator are non-empty ``std::optional``s; ``false`` otherwise.

		.. cpp:function:: bool isReported()

			:returns: ``true`` if the conditions for :cpp:func:`isValue()` are met, or the modifier is either :cpp:enumerator:`Modifier::DISTANT` or :cpp:enumerator:`Modifier::VICINITY`; ``false`` otherwise.

		.. cpp:function:: std::optional<std::pair<unsigned int, MilesFraction>> miles() const

			:returns: Value in statute miles in the form of integer and fraction.

				If the value is not reported, an empty ``std::optional`` is returned.

				For the reported values, the ``std::pair`` is returned where ``unsigned int`` component is an integer part, and :cpp:enum:`MilesFraction` component is a fraction part. For example, for the value of ``2 3/4 statute miles``, an ``unsigned int`` component contains ``2`` and :cpp:enum:`MilesFraction` component contains :cpp:enumerator:`MilesFraction::F_3_4`.

					.. note:: This method may only return the following values (as per Table 6-1 of Field Meteorology Handbook Number 1):

						Below 1/2 statute mile: increments of 1/16 statute mile (0, 1/16, 1/8, 3/16, 1/4, 5/16, 3/8).

						From 1/2 to 2 statute miles: increments of 1/8 statute mile (1/2, 5/8, 3/4, 7/8, 1, 1 1/8, 1 1/4, 1 3/8, 1 1/2, 1 5/8, 1 3/4, 1 7/8).

						From 2 to 3 statute miles: increments of 1/4 statute mile (2, 2 1/4 2 1/2, 2 3/4).

						From 3 to 15 statute miles: increments of 1 statute mile (3, 4, 5, etc).

						Above 15 statute miles: increments of 5 statute miles (15, 20, 25, etc).

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

		.. cpp:enumerator:: NOT_REPORTED

			The direction is not reported or not specified.

		.. cpp:enumerator:: VRB

			Direction is Variable.

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

		.. cpp:enumerator:: OHD

			Overhead.

		.. cpp:enumerator:: ALQDS

			All quadrants (i.e. all directions).

		.. cpp:enumerator:: UNKNOWN

			Unknown direction.

	.. cpp:enum-class:: Type

		The type of the direction value reported. If the type is other than :cpp:enumerator:`VALUE_DEGREES` or :cpp:enumerator:`VALUE_CARDINAL`, then no numerical direction value is provided.

		.. cpp:enumerator:: NOT_REPORTED

			Direction is specified as 'not reported' or not specified.

		.. cpp:enumerator:: VARIABLE

			Direction is reported as variable.

		.. cpp:enumerator:: NDV

			Direction is reported as 'No Directional Variation'.

		.. cpp:enumerator:: VALUE_DEGREES

			Direction is reported as value in degrees.

		.. cpp:enumerator:: VALUE_CARDINAL

			Direction is reported as cardinal value.

		.. cpp:enumerator:: OVERHEAD

			Phenomena occurrs directly over the location rather than in a distance.

		.. cpp:enumerator:: ALQDS

			Direction is reported as all quadrants (i.e. in all directions).

		.. cpp:enumerator:: UNKNOWN

			Direction is explicitly reported as unknown (e.g. when direction of movement of phenomena in vicinity is unknown).

	**Acquiring the data**

		.. cpp:function:: Type type() const

			::returns:: Type of stored direction value.

		.. cpp:function:: Cardinal cardinal(bool trueDirections = false) const

			:param trueDirections: If set to ``true`` allows returning :cpp:enumerator:`Cardinal::TRUE_NORTH`, :cpp:enumerator:`Cardinal::TRUE_SOUTH`, :cpp:enumerator:`Cardinal::TRUE_EAST`, :cpp:enumerator:`Cardinal::TRUE_WEST`

			:returns: Cardinal direction corresponding to the stored direction value.

				.. note:: If the direction value in degrees is reported (i.e. value type is :cpp:enumerator:`Type::VALUE_DEGREES`) the corresponding cardinal direction is returned; if the value exceeds 360 degrees then :cpp:enumerator:`Cardinal::NOT_REPORTED` is returned.

		.. cpp:function:: std::optional<unsigned int> degrees() const

			:returns: Stored value in degrees. If cardinal value was stored, then the middle value of the corresponding directional sector is returned as follows:

				================== ====================
				Cardinal direction Direction in degrees
				================== ====================
				North              360
				Northeast          45
				East               90
				Southeast          135
				South              180
				Southwest          225
				West               270
				Northwest          315
				================== ====================

				If the type of the stored value is other than :cpp:enumerator:`Type::VALUE_DEGREES` or :cpp:enumerator:`Type::VALUE_CARDINAL` then an empty ``std::optional`` is returned.

	**Miscellaneous**

		.. cpp:function:: bool isReported() const

			:returns: ``true`` if any directional value is stored in this group (i.e. NDV, VRB, OHD, ALQDS, UNKNOWN or value in degrees or cardinal direction); or ``false`` if non-reported direction is stored in this group.

		.. cpp:function:: bool isValue() const

			:returns: ``true`` if the stored direction contains a value, and ``false`` if the stored direction does not contain a concrete value.

				- ``true`` is returned if ether cardinal direction (:cpp:enumerator:`Type::VALUE_CARDINAL`) or value in degrees (:cpp:enumerator:`Type::VALUE_DEGREES`) is stored.

				- ``false`` is returned if the type of the value is :cpp:enumerator:`Type::NOT_REPORTED`, :cpp:enumerator:`Type::VARIABLE`, :cpp:enumerator:`Type::NDV`, :cpp:enumerator:`Type::OVERHEAD`, :cpp:enumerator:`Type::ALQDS`, or :cpp:enumerator:`Type::UNKNOWN`.

		.. cpp:function:: static std::vector<Direction> sectorDirectionsToVector(const Direction & dir1, const Direction & dir2)

			:returns: ``std::vector`` of the all directions included in the direction sector.

				.. note:: Direction sector is defined clockwise from dir1 to dir2.


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

		.. cpp:enumerator:: MM_HG

			Millimeters mercury.

	**Acquiring the data**

		.. cpp:function:: std::optional<float> pressure() const

		:returns: Stored pressure value or empty ``std::optional`` if pressure value is not reported.

		.. cpp:function:: Unit unit() const

		:returns: Pressure measurement unit which was used with stored value.


	**Converting to other measurement units**

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.
			:returns: Stored pressure value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


	**Miscellaneous**

		.. cpp:function:: bool isReported() const

			:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored.


Precipitation
^^^^^^^^^^^^^

.. cpp:class:: Precipitation

	The amount or accumulation of precipitation. The amount/accumulation may be not reported (i.e. no value).

	.. cpp:enum-class:: Unit

		Precipitation amount or accumulation measurement units.

		.. cpp:enumerator:: MM

			Millimeters.
			
		.. cpp:enumerator:: INCHES

			Inches.


	**Acquiring the data**

		.. cpp:function:: std::optional<float> precipitation() const

			:returns: Stored amount/accumulation value or empty ``std::optional`` if the value is not reported.

		.. cpp:function:: Unit unit() const

			:returns: Precipitation amount/accumulation measurement unit which was used with stored value.


	**Converting to other measurement units**

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.
			:returns: Stored precipitation amount/accumulation value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


	**Miscellaneous**

		.. cpp:function:: bool isReported() const

			:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored.


SurfaceFriction
^^^^^^^^^^^^^^^

.. cpp:class:: SurfaceFriction

	Surface friction. The intended use is reporting the surface friction of the runway. Surface friction may be reported in the form of friction coefficient or braking action.

	The value may be optionally not reported, or reported as unreliable/unmeasurable.

	Friction coefficient is a value in range [0.00 .. 1.00]. Lesser values mean more slippery surface.

	.. note:: Surface friction coefficient is a dimensionless value and has no associated measurement units.

	.. cpp:enum-class:: Type

		The type of surface friction value.

		.. cpp:enumerator:: NOT_REPORTED

			Surface friction is not reported.

		.. cpp:enumerator:: SURFACE_FRICTION_REPORTED

			Surface friction reported in form of friction coefficient.

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

		.. cpp:function:: Type type() const

			:returns: Type of surface friction value.

		.. cpp:function:: std::optional<float> coefficient() const

			:returns: The value of friction coefficient or empty ``std::optional`` if the value is not reported, unreliable or unmeasurable.

				If :cpp:enum:`BrakingAction` was reported, then 'worst' (i.e. least) friction coefficient for the range specified by stored braking action value is returned (e.g. if braking action was reported as :cpp:enumerator:`BrakingAction::MEDIUM_GOOD` then 0.36 is returned).

		.. cpp:function:: BrakingAction brakingAction() const

			:returns: Braking action value corresponding to stored value.

				If friction coefficient was reported, a range of :cpp:enum:`BrakingAction` where this value of the friction coefficient fits is returned (e.g. if friction coefficient with value 0.33 was reported, then :cpp:enumerator:`BrakingAction::MEDIUM` is returned).


	**Miscellaneous**

		.. cpp:function:: bool isReported() const

			:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored. Corresponds to :cpp:enumerator:`Type::NOT_REPORTED`.


		.. cpp:function:: bool isUnreliable() const

			:returns: ``true`` if the stored value is unmeasurable or the measurement result is unreliable, and ``false`` otherwise. When the value is not reported, ``false`` is returned. Corresponds to :cpp:enumerator:`Type::UNRELIABLE`.


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

	**Miscellaneous**

		.. cpp:function:: bool isReported() const

			:returns: ``true`` if wave height is reported (either as descriptive state or as actual wave height).

	**Converting to other measurement units**

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.
			:returns: Stored wave height value (or highest wave height value for specified :cpp:enum:`StateOfSurface`) converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.

WeatherPhenomena
^^^^^^^^^^^^^^^^

.. cpp:class:: WeatherPhenomena

	Specifies weather phenomena, such as obscuration, precipitation, etc., along with intensity, proximity, timing, descriptor, event beginning or enging and event occurrence time.

	.. cpp:enum-class:: Qualifier

		Intensity, proximity or time of observation. 

		.. cpp:enumerator:: NONE

			No qualifier. This group reports current weather observed at location.

		.. cpp:enumerator:: RECENT

			This group reports recent weather rather than current weather.

		.. cpp:enumerator:: VICINITY

			This group reports weather in vicinity rather than on site.

		.. cpp:enumerator:: LIGHT

			Light intensity.

		.. cpp:enumerator:: MODERATE

			Moderate intensity. This qualier is used with precipitation only.

		.. cpp:enumerator:: HEAVY

			Heavy intensity.

	.. cpp:enum-class:: Descriptor

		Additional properties of weather phenomena.

		.. cpp:enumerator:: NONE

			No additional descriptor for weather phenomena specified.

		.. cpp:enumerator:: SHALLOW

			This descriptor is only be used to further describe fog that has little vertical extent (less than 6 feet), i.e. ground fog.

		.. cpp:enumerator:: PARTIAL

			This descriptors is only be used to further describe fog that has little vertical extent (normally greater than or equal to 6 feet but less than 20 feet), and reduces horizontal visibility, but to a lesser extent vertically. The stars may often be seen by night and the sun by day. The fog is covering only the part of the aerodrome.

		.. cpp:enumerator:: PATCHES

			This descriptors is only be used to further describe fog that has little vertical extent (normally greater than or equal to 6 feet but less than 20 feet), and reduces horizontal visibility, but to a lesser extent vertically. The stars may often be seen by night and the sun by day. The fog consists of patches randomly covering the aerodrome.

		.. cpp:enumerator:: LOW_DRIFTING

			When dust, sand, or snow is raised by the wind to less than 6 feet, "low drifting" shall be used to further describe the weather phenomenon.

		.. cpp:enumerator:: BLOWING

			When dust, sand, snow, or spray is raised by the wind to a height of 6 feet or more, "blowing" shall be used to further describe the weather phenomenon.

		.. cpp:enumerator:: SHOWERS

			Precipitation characterized by the suddenness with which they start and stop, by the rapid changes of intensity, and usually by rapid changes in the appearance of the sky.

		.. cpp:enumerator:: THUNDERSTORM

			A local storm produced by a cumulonimbus cloud that is accompanied by lightning and/or thunder. Thunderstorm may be reported without any accompanying precipitation.

		.. cpp:enumerator:: FREEZING

			When fog is occurring and the temperature is below 0°C, this descriptor is used to further describe the phenomena.

			..note:: The fog is described as 'freezing' at freezing temperatures, regardless of whether is deposits the rime.

			When drizzle and/or rain freezes upon impact and forms a glaze on the ground or other exposed objects, this descriptor is used to further describe the precipitation.

	.. cpp:enum-class:: Weather

		Describes precipitation, obscuration and other weather phenomena.

		.. cpp:enumerator:: OMITTED

			The weather information is omitted (i.e. not specified at all)..

		.. cpp:enumerator:: NOT_REPORTED

			An automatic observing system is used and the present weather cannot be
			observed.

		.. cpp:enumerator:: DRIZZLE

			Fairly uniform precipitation composed exclusively of fine drops with diameters of less than 0.02 inch (0.5 mm) very close together. Drizzle appears to float while following air currents, although unlike fog droplets, it falls to the ground.

		.. cpp:enumerator:: RAIN

			Precipitation, either in the form of drops larger than 0.02 inch (0.5 mm), or smaller drops which, in contrast to drizzle, are widely separated.

		.. cpp:enumerator:: SNOW

			Precipitation of snow crystals, mostly branched in the form of six-pointed stars.

		.. cpp:enumerator:: SNOW_GRAINS

			Precipitation of very small, white, and opaque grains of ice.

		.. cpp:enumerator:: ICE_CRYSTALS

			A fall of unbranched (snow crystals are branched) ice crystals in the form of needles, columns, or plates.

		.. cpp:enumerator:: ICE_PELLETS

			Precipitation of transparent or translucent pellets of ice, which are round or irregular, rarely conical, and which have a diameter of 0.2 inch (5 mm), or less. There are two main types:

				#. Hard grains of ice consisting of frozen raindrops, or largely melted and refrozen snowflakes.
				
				#. Pellets of snow encased in a thin layer of ice which have formed from the freezing, either of droplets intercepted by the pellets, or of water resulting from the partial melting of the pellets.

		.. cpp:enumerator:: HAIL

			Precipitation in the form of small balls or other pieces of ice falling separately or frozen together in irregular lumps.

		.. cpp:enumerator:: SMALL_HAIL

			Precipitation of white, opaque grains of ice. The grains are round or sometimes conical. Diameters range from about 0.08 to 0.2 inch (2 to 5 mm).

			Small hail is also called 'snow pellets' or 'graupel'.

		.. cpp:enumerator:: UNDETERMINED

			Precipitation type that is reported if the automated station detects the occurrence of precipitation but the precipitation discriminator cannot recognize the type.

		.. cpp:enumerator:: MIST

			A visible aggregate of minute water particles suspended in the atmosphere that reduces visibility to less than 7 statute miles but greater than or equal to 5/8 statute miles. Essentially the same as fog but less dense.

		.. cpp:enumerator:: FOG

			A visible aggregate of minute water particles (droplets) which are based at the Earth's surface and reduces horizontal visibility to less than 5/8 statute mile and, unlike drizzle, it does not fall to the ground.

		.. cpp:enumerator:: SMOKE

			A suspension in the air of small particles produced by combustion. A transition to haze may occur when smoke particles have traveled great distances (25 to 100 miles or more) and when the larger particles have settled out and the remaining particles have become widely scattered through the atmosphere.

		.. cpp:enumerator:: VOLCANIC_ASH

			Fine particles of rock powder that originate from a volcano and that may remain suspended in the atmosphere for long periods.

		.. cpp:enumerator:: DUST

			Widespread dust. Fine particles of earth or other matter raised or suspended in the air by the wind that may have occurred at or far away from the station which may restrict horizontal visibility.

		.. cpp:enumerator:: SAND

			Sand particles raised by the wind to a height sufficient to reduce horizontal visibility.

		.. cpp:enumerator:: HAZE

			A suspension in the air of extremely small, dry particles invisible to the naked eye and sufficiently numerous to give the air an opalescent appearance.

		.. cpp:enumerator:: SPRAY

			An ensemble of water droplets torn by the wind from the surface of an extensive body of water, generally from the crests of waves, and carried up a short distance into the air.

		.. cpp:enumerator:: DUST_WHIRLS

			Well-developed Dust/Sand Whirl. An ensemble of particles of dust or sand, sometimes accompanied by small litter, raised from the ground in the form of a whirling column of varying height with a small diameter and an approximately vertical axis.

		.. cpp:enumerator:: SQUALLS

			A strong wind characterized by a sudden onset in which the wind speed increases at least 16 knots and is sustained at 22 knots or more for at least one minute (see paragraph 12.6.8.e.(1)).

		.. cpp:enumerator:: FUNNEL_CLOUD

			Funnel cloud / tornadic activity.

				#. Tornado. A violent, rotating column of air touching the ground.

				#. Funnel Cloud. A violent, rotating column of air which does not touch the surface.

				#. Waterspout. A violent, rotating column of air that forms over a body of water, and touches the water surface.

		.. cpp:enumerator:: SANDSTORM

			Sandstorm. Particles of sand carried aloft by a strong wind. The sand particles are mostly confined to the lowest ten feet, and rarely rise more than fifty feet above the ground.

		.. cpp:enumerator:: DUSTSTORM

			Duststorm. A severe weather condition characterized by strong winds and dust-filled air over an extensive area.

	.. cpp:enum-class:: Event

		Type of the weather event. 

		.. cpp:enumerator:: NONE

			No event specified.

		.. cpp:enumerator:: BEGINNING

			Indicates beginning of the weather phenomena.

		.. cpp:enumerator:: ENDING

			Indicates ending of the weather phenomena.

	**Acquiring the data**

	.. cpp:function:: Qualifier qualifier() const

		:returns: Weather qualifier which indicates time or intensity or proximity of the weather phenomena.

	.. cpp:function:: Descriptor descriptor() const

		:returns: Weather descriptor which indicates additional properties of weather phenomena.

	.. cpp:function:: std::vector<Weather> weather() const

		:returns: Vector of individual weather phenomena.

	.. cpp:function:: Event event() const

		:returns: Type of event.

	.. cpp:function:: std::optional<MetafTime> time() const

		:returns: Time of the event or empty ``std::optional`` if no event time was specified.

	**Miscellaneous**

		.. cpp:function:: bool isOmitted() const

			:returns: ``true`` if no qualifier, no descriptor, no weather phenomena, no event and no event time is stored in this instance, ``false`` if any of these conditions is not met.


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if the weather phenomena is valid, and ``false`` otherwise.

				The information is considered valid if all of the following conditions is met: 
					 - weather phenomena is non-empty, i.e. at lease one qualifier or descriptor or weather must be specified;
					 - event time is valid if specified;
					 - if descriptor FZ (freezing) is present, the weather phenomena must contain FG (fog), or the precipitation type which is potentially may freeze: UP (undetermined precipitation), or RA(rain), or DZ (drizzle); other precipitation may be present alone with specified above, e.g. ``FZRASN`` (freezing rain and snow) is valid, while ``FZSNPL`` (freezing snow and ice pellets) is not valid;


CloudType
^^^^^^^^^

.. cpp:class:: CloudType

	Specifies type of the cloud in the layer, base height, and sky coverage (okta).

	.. cpp:enum-class:: Type

		Type/genus of clouds.

		.. note:: Also includes weather phenomena causing obscuration such as rain, fog, blowing snow, etc. used in Canada in addition to clouds.

		.. cpp:enumerator:: NOT_REPORTED

			Type of the cloud is not reported or not known.

		.. cpp:enumerator:: CUMULONIMBUS

			Cumulonimbus clouds.

		.. cpp:enumerator:: TOWERING_CUMULUS

			Towering Cumulus clouds.

		.. cpp:enumerator:: CUMULUS

			Cumulus clouds.

		.. cpp:enumerator:: CUMULUS_FRACTUS

			Cumulus fractus clouds.

		.. cpp:enumerator:: STRATOCUMULUS

			Stratocumulus clouds.

		.. cpp:enumerator:: NIMBOSTRATUS

			Nimbostratus clouds.

		.. cpp:enumerator:: STRATUS

			Stratus clouds.

		.. cpp:enumerator:: STRATUS_FRACTUS

			Stratus fractus clouds.

		.. cpp:enumerator:: ALTOSTRATUS

			Altostratus clouds clouds.

		.. cpp:enumerator:: ALTOCUMULUS

			Altocumulus clouds clouds.

		.. cpp:enumerator:: ALTOCUMULUS_CASTELLANUS

			Altostratus castellanus clouds.

		.. cpp:enumerator:: CIRRUS

			Cirrus clouds.

		.. cpp:enumerator:: CIRROSTRATUS

			Cirrostratus clouds.

		.. cpp:enumerator:: CIRROCUMULUS

			Cirrostratus clouds.

		.. cpp:enumerator:: BLOWING_SNOW

			Blowing snow (used only in Canada).

		.. cpp:enumerator:: BLOWING_DUST

			Blowing dust (used only in Canada).

		.. cpp:enumerator:: BLOWING_SAND

			Blowing sand (used only in Canada).

		.. cpp:enumerator:: ICE_CRYSTALS

			Ice crystals (used only in Canada).

		.. cpp:enumerator:: RAIN

			Rain (used only in Canada).

		.. cpp:enumerator:: DRIZZLE

			Drizzle (used only in Canada).

		.. cpp:enumerator:: SNOW

			Snow falling from the clouds (used only in Canada).

		.. cpp:enumerator:: ICE_PELLETS

			Ice pellets (used only in Canada).

		.. cpp:enumerator:: SMOKE

			Smoke (used only in Canada).

		.. cpp:enumerator:: FOG

			Fog (used only in Canada).

		.. cpp:enumerator:: MIST

			Mist (used only in Canada).

		.. cpp:enumerator:: HAZE

			Haze (used only in Canada).

	**Acquiring the data**

		.. cpp:function:: Type type() const

			:returns: Type of the cloud or obscuration or :cpp:enumerator:`Type::NOT_REPORTED` if cloud type is not specified or cannot be included in this format.

		.. cpp:function:: Distance height() const

			:returns: Height of the cloud base; may be a non-reported value if the base height is not reported or cannot be included in this format.

		.. cpp:function:: unsigned int okta() const

			:returns: Sky coverage in 1/8th (e.g. 3 okta means that this cloud covers 3/8 of the sky).

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if the octa value is in range 1 to 8, and ``false`` otherwise.


Group
-----

.. cpp:type:: Group = std::variant<KeywordGroup, LocationGroup, ReportTimeGroup, TrendGroup, WindGroup, VisibilityGroup, CloudGroup, WeatherGroup, TemperatureGroup, PressureGroup, RunwayStateGroup, SeaSurfaceGroup, MinMaxTemperatureGroup, PrecipitationGroup, LayerForecastGroup, PressureTendencyGroup, CloudTypesGroup, LowMidHighCloudGroup, LightningGroup, VicinityGroup, MiscGroup, UnknownGroup>

	Group is an ``std::variant`` which holds all group classes. It is used by :cpp:class:`metaf::Parser` to return the results of report parsing (see :cpp:class:`metaf::ParseResult`).



Group types
-----------

This section contains the information on each group class which stores information on individual types of METAR/TAF groups.


KeywordGroup
^^^^^^^^^^^^

.. cpp:class:: KeywordGroup

	KeywordGroup represents a group with a standardised keyword significant for report syntax.

	For example, report types METAR, SPECI or TAF at the beginning of the report or CAVOK in the report body are always spelled exactly the same way and have no modifications.

	.. cpp:enum-class:: Type

		Designates the keyword which is represented by this group.

		.. cpp:enumerator:: METAR

			Specifies that the report type is METAR (weather observation) and this is a scheduled report.

		.. cpp:enumerator:: SPECI

			Specifies that the report type is METAR (weather observation) and this is an unscheduled report.

			Unscheduled report is issued dut to sudden changes in weather conditions: wind shift, visibility decrease, severe weather, clouds formed or dissipated, etc.

		.. cpp:enumerator:: TAF

			Specifies that the report type is TAF (weather forecast).

		.. cpp:enumerator:: AMD

			Specifies an amended report.

			This group is only used in TAF reports.

		.. cpp:enumerator:: COR

			Specifies a correctional report.

		.. cpp:enumerator:: NIL

			Specifies a missing report.

			No report body is allowed after this group.

		.. cpp:enumerator:: CNL

			Specifies a cancelled report.

			No report body is allowed after this group.

			This group is only used in TAF reports.

		.. cpp:enumerator:: AUTO

			Specifies a fully automated report produced with no human intervention or oversight.

			This group is only used in METAR reports.

		.. cpp:enumerator:: CAVOK

			Ceiling and visibility OK; all of the following conditions are met:

				- Visibility 10 km or more in all directions.

				- No cloud below 5000 feet (1500 meters).

				- No cumulonimbus or towering cumulus clouds.

				- no significant weather phenomena.

		.. cpp:enumerator:: RMK

			This group designates the beginning of the remarks.

			Remarks may contain plain-language, manual and automatically generated texts. Remarks typically augment information provided in the METAR or TAF report body.

		.. cpp:enumerator:: MAINTENANCE_INDICATOR

			This group indicates that one ore more systems of automated station require maintenance.

		.. cpp:enumerator:: AO1

			Indicates an automated station without precipitation discriminator.

		.. cpp:enumerator:: AO2

			Indicates an automated station with precipitation discriminator.

		.. cpp:enumerator:: AO1A

			Indicates an automated station without precipitation discriminator and denotes an automated observation augmented by a human observer.

		.. cpp:enumerator:: AO2A

			Indicates an automated station with precipitation discriminator and denotes an automated observation augmented by a human observer.

		.. cpp:enumerator:: NOSPECI

			Indicates a manual station where SPECI (unscheduled) reports are not issued.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of the keyword group.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


LocationGroup
^^^^^^^^^^^^^

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


ReportTimeGroup
^^^^^^^^^^^^^^^

Example of this group is ``302330Z``.

.. cpp:class:: ReportTimeGroup

	Report time stores information about report release date and time.

	**Acquiring group data**

		.. cpp:function:: MetafTime time() const

			:returns: Time when the report was released (GMT time zone).

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if the day-of-month, hour and minute values of report release time belong to valid ranges (see :cpp:func:`MetafTime::isValid()`) and the optional day is included in :cpp:class:`MetafTime`.

				This method returns ``false`` if any of the conditions above is not met.


TrendGroup
^^^^^^^^^^

.. warning:: Old TAF format (before November 2008) uses different format (time without date) for time spans and trends and cannot be decoded.

.. cpp:class:: TrendGroup

	Stores information about weather trends which may be stored in one or several METAR or TAF groups.

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

	**Acquiring group data**

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

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if all of the reported times (begin time / end time / expected event time) are valid (see :cpp:func:`MetafTime::isValid()`).

				Alternatively returns ``false`` if any of the time values above are not valid or the group type is :cpp:enumerator:``Type::PROB``.


WindGroup
^^^^^^^^^

.. cpp:class:: WindGroup

	Stores information about surface wind (including variable wind direction sector if reported), wind shear, wind shift, and peak wind, and missing wind data.

	.. cpp:enum-class:: Type

		Specifies what kind of information is stored within this group.

		.. cpp:enumerator:: SURFACE_WIND

			Surface wind information. Use :cpp:func:`direction()` for wind direction, :cpp:func:`windSpeed()` for sustained wind speed, and :cpp:func:`gustSpeed()` for gust speed.

		.. cpp:enumerator:: SURFACE_WIND_CALM

			Indicates surface wind calm (i.e. no wind). :cpp:func:`windSpeed()` is always zero.

		.. cpp:enumerator:: VARIABLE_WIND_SECTOR

			Only variable wind direction sector information. Use :cpp:func:`varSectorBegin()` for beginning direction of the sector, and :cpp:func:`varSectorEnd()` for ending direction.

			.. note::Wind direction sector is defined from beginning direction clockwise to the ending direction.

		.. cpp:enumerator:: SURFACE_WIND_WITH_VARIABLE_SECTOR

			Surface wind with variable wind sector. Use :cpp:func:`direction()` for wind direction, :cpp:func:`windSpeed()` for sustained wind speed, :cpp:func:`gustSpeed()` for gust speed, :cpp:func:`varSectorBegin()` for beginning direction of the sector, and :cpp:func:`varSectorEnd()` for ending direction.

		.. cpp:enumerator:: WIND_SHEAR

			Wind shear at certain height. Use :cpp:func:`direction()` for wind direction, :cpp:func:`windSpeed()` for sustained wind speed, :cpp:func:`gustSpeed()` for gust speed, and :cpp:func:`height()` for the height where wind shear occurs.

		.. cpp:enumerator:: WIND_SHEAR_IN_LOWER_LAYERS

			Indicates existence of wind shear along the take-off path or approach path between runway level and 500 metres (1 600 ft) significant to aircraft operations, for the particlar runway or all runways. Use :cpp:func:`runway()` for runway for which wind shear in the lower layers is indicated; :cpp:func:`runway()` may also return 'all runways'.

		.. cpp:enumerator:: WIND_SHIFT

			Wind shift; wind direction changed 45 degrees or more in less than 15 minutes with sustained wind speed of 10 knots. Use :cpp:func:`eventTime()` for the time when wind shift began.

		.. cpp:enumerator:: WIND_SHIFT_FROPA

			Same as :cpp:enumerator:`metaf::WindGroup::Type::WIND_SHIFT` but indicates that wind shift is associated with frontal passage. Use :cpp:func:`eventTime()` for the time when wind shift began.

		.. cpp:enumerator:: PEAK_WIND

			Peak wind information since last METAR. Use :cpp:func:`direction()` for wind direction, :cpp:func:`windSpeed()` for peak wind speed, and :cpp:func:`eventTime()` for the time when peak wind was observed.

		.. cpp:enumerator:: WSCONDS

			Indicates that potential wind shear conditions are present but there's not enough information to reliably forecast height, direction and speed of wind shear.

		.. cpp:enumerator:: WND_MISG

			Indicates that wind data is missing. No further details are provided.


	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Wind group type, i.e. what kind of information is stored.

		.. cpp:function:: Direction direction() const

			:returns: Mean wind direction.

		.. cpp:function:: Speed windSpeed() const
		
			:returns: Wind speed.

		.. cpp:function:: Speed gustSpeed() const

			:returns: Wind gust speed.

		.. cpp:function:: Direction varSectorBegin() const

			:returns: Start direction point of variable wind direction sector.

		.. cpp:function:: Direction varSectorEnd() const

			:returns: End direction point of variable wind direction sector.

		.. note::Wind direction sector is defined from start point clockwise to end point.

		.. cpp:function:: Distance height() const

			:returns: Height at which wind shear occurs or a non-reported value if surface wind data are specified.

		.. cpp:function:: std::optional<MetafTime> eventTime() const

			:returns: Time when a certain weather event related to wind has happened (e.g. time when wind shift began, etc).

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored wind information is valid, and ``false`` otherwise.

				The information is considered valid if all of the following conditions are met: 
					- If both gust speed and wind speed are reported, wind speed is less than gust speed;
					- If gust speed is reported, its value is non-zero;
					- If wind shear height is reported then wind shear height value is non-zero;
					- Wind direction, wind shear height, variable wind sector directions must be valid values if reported.


VisibilityGroup
^^^^^^^^^^^^^^^

.. cpp:class:: VisibilityGroup

	Stores information about prevailing visibility, visibility towards cardinal direction, visibility for runway, visibility at surface level, visibility from air trafic control tower, runway visual range, etc.

	See also CAVOK (:cpp:enumerator:`metaf::KeywordGroup::Type::CAVOK`) which may be used to specify visibility of 10 km or more in all directions.

	.. cpp:enum-class:: Type

		Group type which specifies what kind of data stored within this group.

		.. cpp:enumerator:: PREVAILING

			Prevailing visibility. Use :cpp:func:`visibility()`.

		.. cpp:enumerator:: PREVAILING_NDV

			Prevailing visibility; indicates that this station cannot differentiate the directional variation of visibility. Use :cpp:func:`visibility()`. :cpp:func:`direction()` will return direction of type :cpp:enumerator:`Direction::Type::NDV`.

		.. cpp:enumerator:: DIRECTIONAL

			Additional visibility value for a single cardinal direction. Use :cpp:func:`visibility()` and :cpp:func:`direction()`.

		.. cpp:enumerator:: RUNWAY

			Additional visibility value for a runway. Use :cpp:func:`visibility()` and :cpp:func:`runway()`.

		.. cpp:enumerator:: RVR

			Runway visual range. Use :cpp:func:`visibility()`, :cpp:func:`trend()` and :cpp:func:`runway()`.

		.. cpp:enumerator:: SURFACE

			Visibility at surface level. Use :cpp:func:`visibility()`.

		.. cpp:enumerator:: TOWER

			Visibility from aerodrome's air traffic control tower. Use :cpp:func:`visibility()`.

		.. cpp:enumerator:: SECTOR

			Sector visibility for a range of cardinal directions. Use :cpp:func:`visibility()` and :cpp:func:`sectorDirections()`.

		.. cpp:enumerator:: VARIABLE_PREVAILING

			Variable prevailing visibility. Use :cpp:func:`minVisibility()` and :cpp:func:`maxVisibility()`.

		.. cpp:enumerator:: VARIABLE_DIRECTIONAL

			Variable directional visibility. Use :cpp:func:`minVisibility()`, :cpp:func:`maxVisibility()`, and :cpp:func:`direction()`.

		.. cpp:enumerator:: VARIABLE_RUNWAY

			Variable visibility for a runway. Use :cpp:func:`minVisibility()`, :cpp:func:`maxVisibility()`, :cpp:func:`trend()` and :cpp:func:`runway()`.

		.. cpp:enumerator:: VARIABLE_RVR

			Variable runway visual range. Use :cpp:func:`minVisibility()`, :cpp:func:`maxVisibility()`, :cpp:func:`trend()`, and :cpp:func:`runway()`.

		.. cpp:enumerator:: VARIABLE_SECTOR

			Sector visibility for a range of cardinal directions. Use :cpp:func:`minVisibility()`, :cpp:func:`maxVisibility()` and :cpp:func:`sectorDirections()`.

		.. cpp:enumerator:: VIS_MISG

			Indicates that visibility data is missing (coded ``VIS MISG`` in the remarks), no further details are specified.

		.. cpp:enumerator:: RVR_MISG

			Indicates that runway visual range data is missing (coded ``RVR MISG`` in the remarks), no further details are specified.

		.. cpp:enumerator:: RVRNO

			Indicates that runway visual range should be reported but is missing, no further details are specified.

		.. cpp:enumerator:: VISNO

			Indicates that the visibility data is not available for a particular runway or in a particular cardinal direction. Use :cpp:func:``runway()`` or :cpp:func:``direction()``, if both methods return non-reported values, no further details were specified.


	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Visibility group type, i.e. what kind of information is stored.

		.. cpp:function:: Distance visibility() const

			:returns: Visibility value. Values in meters are integer, values in statute miles may be fractional. May contain 'less than' or 'more than' modifiers or may be non-reported value.

		.. cpp:function:: Distance minVisibility() const

			:returns: Minimum visibility value if variable visibility is reported.

		.. cpp:function:: Distance maxVisibility() const

			:returns: Maximum visibility value if variable visibility is reported.

		.. cpp:function:: std::optional<Direction> direction() const

			:returns:  Cardinal direction if directional visibility is specified or empty ``std::optional`` if no directional visibility is specified. Automated stations may also report No Directional Variation if the station is not capable of providing directional visibility.

		.. cpp:function:: std::vector<Direction> sectorDirections() const

			:returns: Cardinal direction vector if sector of directions is specified. Currently always returns an empty ``std::vector<Direction>``.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored visibility information is valid, and ``false`` otherwise.

				The information is considered valid if all of the following conditions are met: 
					- The stored visibility and direction values are valid (if reported);
					- The group does not represent an incomplete integer group (i.e. single digit group not followed by fraction and SM designator).


CloudGroup
^^^^^^^^^^

.. cpp:class:: CloudGroup

	Stores information about a single cloud layer, cloud-like obscuration, lack of cloud cover or vertical visibility.

	.. cpp:enum-class:: Type

		Specifies what kind of information is stored within this group.

		.. cpp:enumerator::	NO_CLOUDS

			Clear sky condition, no clouds detected, or no significant clouds. Use :cpp:func:`amount()` for exact condition; the possible values are :cpp:enumerator:`Amount::NONE_CLR`, :cpp:enumerator:`Amount::NONE_SKC`, :cpp:enumerator:`Amount::NCD`, or :cpp:enumerator:`Amount::NSC`.

		.. cpp:enumerator:: CLOUD_LAYER

			Cloud layer specified in METAR report, trend or remarks.

			Use :cpp:func:`amount()` for cloud amount, :cpp:func:`height()` for base height, and :cpp:func:`convectiveType()` for significant convective type.

		.. cpp:enumerator:: VERTICAL_VISIBILITY

			Sky is obscured and vertical visibility is indicated instead of cloud data. Use :cpp:func:`verticalVisibility()` for vertical visibility value. :cpp:func:`amount()` will return  :cpp:enumerator:`Amount::OBSCURED`.

		.. cpp:enumerator:: CEILING

			Ceiling height. Use :cpp:func:`height()` for ceiling height value. Use :cpp:func:`runway()` and :cpp:func:`direction()` for the location where ceiling is reported.

			.. note:: CAVOK group (:cpp:enumerator:`metaf::KeywordGroup::Type::CAVOK`) is also used to indicate no cloud below 5000 feet (1500 meters) and no cumulonimbus or towering cumulus clouds.

		.. cpp:enumerator:: VARIABLE_CEILING

			Ceiling height is variable. Use :cpp:func:`minHeight()` and :cpp:func:`maxHeight()` for ceiling height range. Use :cpp:func:`runway()` or :cpp:func:`direction()` for the location where ceiling is reported.

		.. cpp:enumerator:: CHINO

			Indicates the that the ceiling data is not available for a secondary location. Use :cpp:func:``runway()`` or :cpp:func:``direction()``.

		.. cpp:enumerator:: CLD_MISG

			Indicates the that cloud data are missing. No further details are provided.

		.. cpp:enumerator:: OBSCURATION

			Indicates the that instead of cloud data, a ground-based or aloft obscuration data is specified.

			Use :cpp:func:`amount()` for obscuration amount, :cpp:func:`height()` for base height (or zero height for ground-based obscuration), and :cpp:func:`cloudType()` for type of obscuration.

	.. cpp:enum-class:: Amount

		Amount (cover) of the cloud layer.

		See also CAVOK (:cpp:enumerator:`metaf::KeywordGroup::Type::CAVOK`) which may be used to specify no cloud below 5000 feet (1500 meters) and no cumulonimbus or towering cumulus clouds.

		.. cpp:enumerator::	NOT_REPORTED

			Amount of cloud (cloud cover) is not reported or not applicable for this type of group.

		.. cpp:enumerator:: NCD

			No cloud detected: automated weather station did not detect any clouds. Either no clouds are present or sensor error occurred.

		.. cpp:enumerator:: NSC

			Nil significant clouds: no cloud below 5000 feet (1500 meters), no cumulonimbus or towering cumulus, and no vertical visibility restriction.

			.. note:: CAVOK group (:cpp:enumerator:`metaf::KeywordGroup::Type::CAVOK`) is also used to indicate no cloud below 5000 feet (1500 meters) and no cumulonimbus or towering cumulus clouds.

		.. cpp:enumerator:: NONE_CLR

			No clouds / clear sky. No cloud layers are detected at or below 12000 feet /3700 meters) (US) or 25000 feet / 7600 meters (Canada).

			Indicates that station is at least partly automated.

		.. cpp:enumerator:: NONE_SKC

			No clouds / clear sky. In North America indicates report producted by human rather than automatic weather station.

		.. cpp:enumerator:: FEW

			Few clouds (1/8 to 2/8 sky covered).

		.. cpp:enumerator:: SCATTERED

			Scattered clouds (3/8 to 4/8 sky covered).

		.. cpp:enumerator:: BROKEN

			Broken clouds (5/8 to 7/8 sky covered).

		.. cpp:enumerator:: OVERCAST

			Overcast (8/8 sky covered)

		.. cpp:enumerator:: OBSCURED

			Sky obscured; vertical visibility reported instead.

		.. cpp:enumerator:: VARIABLE_FEW_SCATTERED

			Cloud cover is variable between :cpp:enumerator:`FEW` and  :cpp:enumerator:`SCATTERED`.

		.. cpp:enumerator:: VARIABLE_SCATTERED_BROKEN

			Cloud cover is variable between :cpp:enumerator:`SCATTERED` and  :cpp:enumerator:`BROKEN`.

		.. cpp:enumerator:: VARIABLE_BROKEN_OVERCAST

			Cloud cover is variable between :cpp:enumerator:`BROKEN` and  :cpp:enumerator:`OVERCAST`.


	.. cpp:enum-class:: ConvectiveType

		Significant convective type of the cloud in the layer.

		.. cpp:enumerator::NONE

			No cloud type specified or not applicable.

		.. cpp:enumerator::NOT_REPORTED

			Convective cloud type is not reported.

		.. cpp:enumerator::TOWERING_CUMULUS

			Convective cloud: towering cumulus.

		.. cpp:enumerator::CUMULONIMBUS

			Convective cloud: cumulonimbus.

	**Acquiring group data**

		.. cpp:function:: Amount amount() const

			:returns: Amount (cover) of clouds in layer or clear sky conditions.

		.. cpp:function:: ConvectiveType type() const

			:returns: Convective type of the cloud in the layer.

		.. cpp:function:: Distance height() const

			:returns: Cloud base or ceiling height or non-reported value if height is not applicable for this group.

			.. note:: When the sky is obscured use :cpp:func:`verticalVisibility()`.

			.. note:: When variable ceiling height is reported use :cpp:func:`minHeight()` or :cpp:func:`maxHeight()`.

		.. cpp:function:: Distance verticalVisibility() const

			:returns: Vertical visibility or value if sky is obscured; non-reported value otherwise.

		.. cpp:function:: Distance minHeight() const

			:returns: Minimum ceiling height if variable ceiling height is reported; non-reported value otherwise.

		.. cpp:function:: Distance maxHeight() const

			:returns: Maximum ceiling height if variable ceiling height is reported; non-reported value otherwise.

		.. cpp::function:: std::optional<Runway> runway() const

			:returns: For location-specific data such as ceiling, .

	**Miscellaneous**

		.. cpp:function: std::optional<CloudType> cloudType() const

			:returns: :cpp:class:`CloudType` corresponding to the information stored in the group (maximum okta value, convective type, and cloud base height or minimum height if variable), or empty std::optional if 'no clouds' conditions or vertical visibility or missing data groups.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored cloud information is valid, and ``false`` otherwise. The information is considered valid if the value of cloud cover height or vertical visibility is valid (if reported). Zero height of cloud cover base or vertical visibility does not make the information invalid.


WeatherGroup
^^^^^^^^^^^^

.. cpp:class:: WeatherGroup

	Stores information about recent or current weather phenomena, or on the beginning or ending time of weather phenomena.

	.. note:: If more than 10 beginning/ending times are specified in the group, only first 10 are stored.

	.. cpp:enum-class:: Type

		What kind weather information this group represents.

		.. cpp:enumerator::	CURRENT

			Current weather information is stored in this group.

		.. cpp:enumerator:: RECENT

			Recent weather information is stored in this group.

		.. cpp:enumerator:: EVENT

			Time of beginning or ending of weather phenomena is stored in this group.

		.. cpp:enumerator:: NSW

			Nil significant weather.

			This group is only used in trends and indicates the end of a significant weather phenomena. No further information is provided.

		.. cpp:enumerator:: PWINO

			Indicates that automated station is equipped with present weather identifier and this sensor is not operating. No further information is provided.

		.. cpp:enumerator:: TSNO

			Indicates that automated station is equipped with lightning detector and this sensor is not operating. No further information is provided.

		.. cpp:enumerator:: WX_MISG

			Weather phenomena data is missing. No further information is provided.

		.. cpp:enumerator:: TS_LTNG_TEMPO_UNAVBL

			Thunderstorm/lightning data is missing. No further information is provided.


	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: What kind of weather information stored in this group; current weather, recent weather, weather event (beginning and ending times of weather phenomena).

		.. cpp:function:: std::vector<WeatherPhenomena> weatherPhenomena() const

			:returns: The vector or weather phenomena; each :cpp:class:`metaf::WeatherPhenomena` includes qualifier, descriptor and weather phenomena reported in this group.


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if all weather phenomena included in this group are valid; ``false`` otherwise.

				Invalid weather phenomena (or invalid combinations or descriptor / qualifier / phenomena) may be not recognised as a WeatherGroup.

				For ``WX`` not followed by ``MISG`` and for ``TS/LTNG`` not followed by ``TEMPO`` and ``UNAVBL`` returns ``false``.


TemperatureGroup
^^^^^^^^^^^^^^^^

.. cpp:class:: TemperatureGroup

	Stores information about current ambient air temperature and dew point. Group which reports values rounded to integer of degrees Celsius (e.g. ``10/M00``) is included in METAR report body. Group which reports values in tength of degrees Celsius (e.g. ``T02330206``) is used in North America and is included in remarks.

	.. cpp:enum-class:: Type

		Group type which specifies what kind of data stored within this group.

		.. cpp:enumerator:: TEMPERATURE_AND_DEW_POINT

			Temperature and dew point. Use :cpp:func:`airTemperature()` and :cpp:func:`dewPoint()` for ambient air temperature and dew point.

		.. cpp:enumerator:: T_MISG

			Temperature data is missing. No further details provided.

		.. cpp:enumerator:: TD_MISG

			Dew point data is missing. No further details provided.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of the group.

		.. cpp:function:: Temperature airTemperature() const

			:returns: Ambient air temperature.

		.. cpp:function:: Temperature dewPoint() const

			:returns: Dew point.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored ambient air temperature and dew point information is valid and the group is complete; ``false`` otherwise. 

				The information is considered valid if the dew point is less or equal than ambient air temperature.

				Groups ``TD`` and ``T`` not followed by ``MISG`` are incomplete; ``false`` is returned for such groups.


PressureGroup
^^^^^^^^^^^^^

.. cpp:class:: PressureGroup

	Stores information about observed or forecast atmospheric pressure.

	.. cpp:enum-class:: Type

		Group type which specifies what kind of data stored within this group.

		.. cpp:enumerator:: OBSERVED_QNH

			Indicates that groups contains an observed mean atmospheric pressure normalised to sea level (used in METAR, e.g. ``Q1020``, ``A2981``, or remark ``SLP960``).

		.. cpp:enumerator:: FORECAST_LOWEST_QNH

			Indicates that group contains a forecast lowest sea level pressure, (e.g. ``QNH2981INS``). This group may be reported by military aerodromes of NATO countries.

		.. cpp:enumerator:: OBSERVED_QFE

			Indicates that group contains an observed actual atmospheric pressure (e.g. remark ``QFE761/1015`` or ``QFE741``). This group is used by some countries which were part of Soviet Union.

		.. cpp:enumerator:: SLPNO

			Mean sea-level pressure information is not available. No further details are provided.

		.. cpp:enumerator:: PRES_MISG

			Atmospheric pressure (altimeter) data is missing. No further details are provided.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of the pressure value (observed, forecast, and data missing).

		.. cpp:function:: Pressure atmosphericPressure() const

			:returns: Atmospheric pressure value.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true`` for all groups except ``PRES`` not followed by ``MISG``.



RunwayStateGroup
^^^^^^^^^^^^^^^^

.. cpp:class:: RunwayStateGroup

	Stores information about the state of runway surface and/or accumulation of deposits for a single runway. Alternatively may store information that the deposits of runway ceased to exist or that runway is closed due to snow accumulation.

	.. cpp:enum-class:: Type

		Type of information about runway state.

		.. cpp:enumerator:: RUNWAY_STATE

			Runway state group. Use :cpp:func:`deposits()` for type of deposits on runway (e.g. water patches, snow, slush, etc.),  :cpp:func:`contaminationExtent()` for the percentage of runway contamination by deposits, :cpp:func:`depositDepth()` for the depth of the deposits, and :cpp:func:`surfaceFriction()` for the surface friction or braking action.

		.. cpp:enumerator:: RUNWAY_NOT_OPERATIONAL

			Runway state group indicating that the runway is not operational. Use :cpp:func:`deposits()` for type of deposits on runway (e.g. water patches, snow, slush, etc.),  :cpp:func:`contaminationExtent()` for the percentage of runway contamination by deposits, and :cpp:func:`surfaceFriction()` for the surface friction or braking action.

		.. cpp:enumerator:: RUNWAY_CLRD

			Runway state group indicating that previously present deposits on runway were cleared or ceased to exist. Use :cpp:func:`surfaceFriction()` for the surface friction or braking action.

		.. cpp:enumerator:: RUNWAY_SNOCLO

			Runway state group indicating that the runway is closed due to snow accumulation (e.g. ``R27/SNOCLO``). No further details are specified. 

		.. cpp:enumerator:: AERODROME_SNOCLO

			Aerodrome is closed due to snow accumulation, coded as  ``SNOCLO`` or ``R/SNOCLO`` in the METAR report. No further details are specified. :cpp:func:`runway()` always returns 'all runways' value.

	.. cpp:enum-class:: Deposits

		Type of deposits on the runway.

		.. cpp:enumerator:: CLEAR_AND_DRY

			No deposits; runway is clear and dry.

		.. cpp:enumerator:: DAMP

			Runway is damp.

		.. cpp:enumerator:: WET_AND_WATER_PATCHES

			Runway is wet and water patches are present.

		.. cpp:enumerator:: RIME_AND_FROST_COVERED

			Runway is covered in frost / rime.

		.. cpp:enumerator:: DRY_SNOW

			Dry snow on runway.

		.. cpp:enumerator:: WET_SNOW

			Wet snow on runway.

		.. cpp:enumerator:: SLUSH

			Slush on runway.

		.. cpp:enumerator:: ICE

			Ice on runway.

		.. cpp:enumerator:: COMPACTED_OR_ROLLED_SNOW

			Compacted or rolled snow on runway.

		.. cpp:enumerator:: FROZEN_RUTS_OR_RIDGES

			Runway covered in frozen mass of snow or ice with ruts and riges.

		.. cpp:enumerator:: NOT_REPORTED

			Deposits on runway are not reported.

	.. cpp:enum-class::  Extent

		The extent of runway contamination with the deposits (:cpp:enum:`Deposits`). Represents how much of total runway surface is contaminated. 

		.. cpp:enumerator:: NONE

			No deposits on the runway.

		.. cpp:enumerator:: LESS_THAN_10_PERCENT

			Less than 10% of runway contaminated.

		.. cpp:enumerator:: FROM_11_TO_25_PERCENT

			Less than 11% to 25% of runway contaminated.

		.. cpp:enumerator:: RESERVED_3

			Reserved value; should not be used.

		.. cpp:enumerator:: RESERVED_4

			Reserved value; should not be used.

		.. cpp:enumerator:: FROM_26_TO_50_PERCENT

			From 26% to 50% of runway contaminated.

		.. cpp:enumerator:: RESERVED_6

			Reserved value; should not be used.

		.. cpp:enumerator:: RESERVED_7

			Reserved value; should not be used.

		.. cpp:enumerator:: RESERVED_8

			Reserved value; should not be used.

		.. cpp:enumerator:: MORE_THAN_51_PERCENT

			More than 51% of runway surface 

		.. cpp:enumerator:: NOT_REPORTED

			Contamination extent not reported.

	**Acquiring group data**

		.. cpp:function:: Runway runway() const

			:returns: Runway for which the state is provided.

		.. cpp:function:: Type type() const

			:returns: Type of runway state group. 

		.. cpp:function:: Deposits deposits() const

			:returns: Deposits on the runway.

		.. cpp:function:: Extent contaminationExtent() const

			:returns: Extent (percentage) of runway contamination with deposits.

		.. cpp:function:: Precipitation depositDepth() const

			:returns: Depth of the deposits on the runway or non-reported value.

		.. cpp:function:: SurfaceFriction surfaceFriction() const

			:returns: Surface friction or braking action or not reported value.

	**Validating**

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if runway state information is valid, and ``false`` otherwise. The information is considered valid if the specified runway is valid and :cpp:enum:`Extent` returned by :cpp:func:`contaminationExtent()` is not a reserved value.


SeaSurfaceGroup
^^^^^^^^^^^^^^^

.. cpp:class:: SeaSurfaceGroup

	Stores information about temperature of sea surface along with descriptive state of sea surface or wave height. This group is used by oil platforms.

	**Acquiring group data**

		.. cpp:function:: Temperature surfaceTemperature() const

			:returns: Temperature of the sea surface or non-reported value.

		.. cpp:function:: WaveHeight waves() const

			:returns: Wave height or descriptive state of the sea surface or non-reported value.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


MinMaxTemperatureGroup
^^^^^^^^^^^^^^^^^^^^^^

.. cpp:class:: MinMaxTemperatureGroup

	Stores information about observed or forecast minimum and/or maximum temperature.

	.. cpp:enum-class:: Type

		Group type which specifies what kind of data stored in this group.

		.. cpp:enumerator:: OBSERVED_24_HOURLY

			Observed 24-hourly minimum and maximum temperature. Use :cpp:func:`minimum()` and :cpp:func:`maximum()` for observed minimum and maximum temperature values.

		.. cpp:enumerator:: OBSERVED_6_HOURLY

			Observed 6-hourly minimum and maximum temperature. Use :cpp:func:`minimum()` and :cpp:func:`maximum()` for observed minimum and maximum temperature values; one or both of the values may be non-reported.

		.. cpp:enumerator:: FORECAST

			Forecast minimum and maximum temperature. Use :cpp:func:`minimum()` and :cpp:func:`maximum()` for observed minimum and maximum temperature values. Use :cpp:func:`minimumTime()` and :cpp:func:`maximumTime()` for the time when the specified temperature is forecast to occur.

			.. note:: If only one temperature point was reported without further specifying whether it is a minimum or maximum value, then both :cpp:func:`minimum()` and :cpp:func:`maximum()` will return the same value; also :cpp:func:`minimumTime()` and :cpp:func:`maximumTime()` will return the same value.


	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Minimum/maximum temperature group type which specifies what kind of data is stored in this group.

		.. cpp:function:: Temperature minimum() const

			:returns: Minimum temperature for the specified period (with precision to tenth of degrees Celsius). May return non-reported value.

		.. cpp:function:: Temperature maximum() const

			:returns: Maximum temperature for the specified period (with precision to tenth of degrees Celsius). May return non-reported value.

		.. cpp:function:: std::optional<MetafTime> minimumTime() const

			:returns: Time when the minimum temperature is forecast to occur, or empty ``std::optional`` if the group stores data other than forecast.

			Minimum temperature for the specified period (with precision to tenth of degrees Celsius). May return non-reported value.

		.. cpp:function:: std::optional<MetafTime> maximumTime() const

			:returns: Time when the maximum temperature is forecast to occur, or empty ``std::optional`` if the group stores data other than forecast.


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if the minimum temperature is greater or equal to maximum temperature, and ``false`` otherwise.


PrecipitationGroup
^^^^^^^^^^^^^^^^^^

.. cpp:class:: PrecipitationGroup

	Stores various information about precipitation, rainfall, snowfall, snow depth, and icing (typically caused by freezing precipitation). This group is used regionally and may be included in METAR report body or in remarks.

	.. cpp:enum-class:: Type

		Indicates the type of data reported in this group.

		.. cpp:enumerator:: TOTAL_PRECIPITATION_HOURLY

			Water equivalent of all precipitation for last hour.

		.. cpp:enumerator:: SNOW_DEPTH_ON_GROUND

			Actual snow depth on the ground

		.. cpp:enumerator:: FROZEN_PRECIP_3_OR_6_HOURLY

			Water equivalent of frozen precipitation accumulated for last 3 or 6 hours. Used when this kind of precipitation parameter is reported at the time of day that qualifies neither as 3-hourly nor as 6-hourly report.

		.. cpp:enumerator:: FROZEN_PRECIP_3_HOURLY

			Water equivalent of frozen precipitation accumulated for last 3 hours (included in reports near 0300Z, 0900Z, 1500Z, or 2100Z, i.e. 3-hourly reports).

		.. cpp:enumerator:: FROZEN_PRECIP_6_HOURLY

			Water equivalent of frozen precipitation accumulated for last 3 hours (included in reports near 0000Z, 0600Z, 1200Z, or 1800Z, i.e. 6-hourly reports).

		.. cpp:enumerator:: FROZEN_PRECIP_24_HOURLY

			Water equivalent of frozen precipitation accumulated for last 24 hours.

		.. cpp:enumerator:: SNOW_6_HOURLY

			Snow accumulation for the last 6 hours.

		.. cpp:enumerator:: WATER_EQUIV_OF_SNOW_ON_GROUND

			Water equivalent of snow on the ground (including other solid precipitation such as snow grains, ice pellets, ice crystals, hail, etc).

		.. cpp:enumerator:: ICE_ACCRETION_FOR_LAST_HOUR

			Amount of ice accretion during the preceding hour.

		.. cpp:enumerator:: ICE_ACCRETION_FOR_LAST_3_HOURS

			Amount of ice accretion during the last 3 hours.

		.. cpp:enumerator:: ICE_ACCRETION_FOR_LAST_6_HOURS

			Amount of ice accretion during the last 6 hours.

		.. cpp:enumerator:: SNOW_INCREASING_RAPIDLY

			Indicates that snow is increasing rapidly; use :cpp:func:`total()` for total snowfall, and :cpp:func:`recent()` for snow increase during last hour.

		.. cpp:enumerator:: PRECIPITATION_ACCUMULATION_SINCE_LAST_REPORT

			Accumulation of precipitation since previous released weather report.

		.. cpp:enumerator:: RAINFALL_9AM_10MIN

			Amount of rainfall; use :cpp:func:`total()` for rainfall since 9AM (9:00) local time, and :cpp:func:`recent()` for rainfall in the last 10 minutes.

		.. cpp:enumerator:: PNO

			Indicates that automated station is equipped with tipping bucket rain gauge and this sensor is not operating. No further details are provided.

		.. cpp:enumerator:: FZRANO

			Indicates that automated station is equipped with freezing rain sensor and this sensor is not operating. No further details are provided.

		.. cpp:enumerator:: ICG_MISG

			Icing data is missing.

		.. cpp:enumerator:: PCPN_MISG

			Precipitation data is missing.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of value reported in this group.

		.. cpp:function:: Precipitation total() const

			:returns: Total amount of precipitation of specified type. May be a non-reported value.

		.. cpp:function:: Precipitation recent() const

			:returns: Amount or increase of precipitation during recent period.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


LayerForecastGroup
^^^^^^^^^^^^^^^^^^

.. cpp:class:: LayerForecastGroup

	Stores various information about forecast atmospheric layer (span of heights where certain conditions such as icing or turbulence are forecast). This group may be present in TAFs issued at military aerodromes of NATO countries.

	.. cpp:enum-class:: Type

		Provides description of the atmospheric layer reported in this group.

		.. cpp:enumerator:: ICING_TRACE_OR_NONE

			Trace Icing or No Icing.

		.. cpp:enumerator:: ICING_LIGHT_MIXED

			Light Mixed Icing.

		.. cpp:enumerator:: ICING_LIGHT_RIME_IN_CLOUD

			Light Rime Icing In Cloud.

		.. cpp:enumerator:: ICING_LIGHT_CLEAR_IN_PRECIPITATION

			Light Clear Icing In Precipitation.

		.. cpp:enumerator:: ICING_MODERATE_MIXED

			Moderate Mixed Icing.

		.. cpp:enumerator:: ICING_MODERATE_RIME_IN_CLOUD

			Moderate Rime Icing In Cloud.

		.. cpp:enumerator:: ICING_MODERATE_CLEAR_IN_PRECIPITATION

			Moderate Clear Icing In Precipitation.

		.. cpp:enumerator:: ICING_SEVERE_MIXED

			Severe Mixed Icing.

		.. cpp:enumerator:: ICING_SEVERE_RIME_IN_CLOUD

			Severe Rime Icing In Cloud.

		.. cpp:enumerator:: ICING_SEVERE_CLEAR_IN_PRECIPITATION

			Severe Rime Icing In Cloud.

		.. cpp:enumerator:: TURBULENCE_NONE

			No turbulence.

		.. cpp:enumerator:: TURBULENCE_LIGHT

			Light turbulence.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASIONAL

			Moderate turbulence in clear air, occasional.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT

			Moderate turbulence in clear air, frequent.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLOUD_OCCASIONAL

			Moderate turbulence in cloud, occasional.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLOUD_FREQUENT

			Moderate turbulence in cloud, frequent.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASIONAL

			Severe turbulence in clear air, occasional.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT

			Severe turbulence in clear air, frequent.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLOUD_OCCASIONAL

			Severe turbulence in cloud, occasional.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLOUD_FREQUENT

			Severe turbulence in cloud, frequent.

		.. cpp:enumerator:: TURBULENCE_EXTREME

			Extreme turbulence.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of atmospheric layers reported in this group.

		.. cpp:function:: Distance baseHeight() const

			:returns: Height of atmospheric layer base (bottom range).

		.. cpp:function:: Distance topHeight() const

			:returns: Height of atmospheric layer top (top range).

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.



PressureTendencyGroup
^^^^^^^^^^^^^^^^^^^^^

.. cpp:class:: PressureTendencyGroup

	Stores information about atmospheric pressure tendency for the last 3 hours. This group is used in North America and is included in remarks.

	.. cpp:enum-class:: Type

		Indicates the nature of atmospheric pressure change reported in this group.

		.. cpp:enumerator:: NOT_REPORTED

			Atmospheric pressure tendency is not reported.

		.. cpp:enumerator:: INCREASING_THEN_DECREASING

			Atmospheric pressure was increasing, then decreasing.

		.. cpp:enumerator:: INCREASING_MORE_SLOWLY

			Atmospheric pressure was increasing, then steady, or increasing then increasing more slowly.

		.. cpp:enumerator:: INCREASING

			Atmospheric pressure was increasing steadily or unsteadily.

		.. cpp:enumerator:: INCREASING_MORE_RAPIDLY

			Atmospheric pressure was decreasing or steady, then increasing; or increasing then increasing more rapidly.

		.. cpp:enumerator:: STEADY

			Atmospheric pressure was steady.

		.. cpp:enumerator:: DECREASING_THEN_INCREASING

			Atmospheric pressure was decreasing, then increasing.

		.. cpp:enumerator:: DECREASING_MORE_SLOWLY

			Atmospheric pressure was decreasing then steady; or decreasing then decreasing more slowly.

		.. cpp:enumerator:: DECREASING

			Atmospheric pressure was decreasing steadily or unsteadily.

		.. cpp:enumerator:: DECREASING_MORE_RAPIDLY

			Atmospheric pressure was steady or increasing, then decreasing; or decreasing then decreasing more rapidly.

		.. cpp:enumerator:: PRESFR

			Atmospheric pressure is rapidly falling at a rate of at least 0.06 inch of mercury (2.03 hectopascal) per hour and the pressure change totals 0.02 inch of mercury (0.68 hectopascal) or more at the time of the observation.

		.. cpp:enumerator:: PRESRR

			Atmospheric pressure is rapidly rising at a rate of at least 0.06 inch of mercury (2.03 hectopascal) per hour and the pressure change totals 0.02 inch of mercury (0.68 hectopascal) or more at the time of the observation.


	.. cpp:enum-class:: Trend

		Indicates the trend of atmospheric pressure changes reported in this group.

		.. cpp:enumerator:: NOT_REPORTED

			Atmospheric pressure tendency is not reported.

		.. cpp:enumerator:: MORE

			Atmospheric pressure is higher than 3 hours ago.

		.. cpp:enumerator:: MORE_OR_SAME

			Atmospheric pressure is higher than or the same as 3 hours ago.

		.. cpp:enumerator:: SAME

			Atmospheric pressure is the same as 3 hours ago.

		.. cpp:enumerator:: LESS_OR_SAME

			Atmospheric pressure is lower than or the same as 3 hours ago.

		.. cpp:enumerator:: LESS

			Atmospheric pressure is lower than 3 hours ago.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Nature of atmospheric pressure changes for last 3 hours.

		.. cpp:function:: Trend trend() const

			:returns: Trend of atmospheric pressure changes for last 3 hours.

		.. cpp:function:: Pressure difference() const

			:returns: Absolute value of atmospheric pressure change for the last 3 hours or emply ``std::optional`` if pressure is not reported.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


CloudTypesGroup
^^^^^^^^^^^^^^^

.. cpp:class:: CloudTypesGroup

	Stores information about layers of clouds and their okta (1/8s of sky coverage). This group is included in the remarks and is used in Canada.

	.. cpp:enum-class:: Type

		Type of clouds.

		.. note:: Weather phenomena causing obscuration such as rain, fog, blowing snow, etc. used in Canada in addition to clouds.

		.. cpp:enumerator:: CUMULONIMBUS

			Cumulonimbus cloud(s).

		.. cpp:enumerator:: TOWERING_CUMULUS

			Towering Cumulus clouds.

		.. cpp:enumerator:: CUMULUS

			Cumulus clouds.

		.. cpp:enumerator:: CUMULUS_FRACTUS

			Cumulus fractus clouds.

		.. cpp:enumerator:: STRATOCUMULUS

			Stratocumulus clouds.

		.. cpp:enumerator:: NIMBOSTRATUS

			Nimbostratus clouds.

		.. cpp:enumerator:: STRATUS

			Stratus clouds.

		.. cpp:enumerator:: STRATUS_FRACTUS

			Stratus fractus clouds.

		.. cpp:enumerator:: ALTOSTRATUS

			Altostratus clouds clouds.

		.. cpp:enumerator:: ALTOCUMULUS

			Altocumulus clouds clouds.

		.. cpp:enumerator:: ALTOCUMULUS_CASTELLANUS

			Altostratus castellanus clouds.

		.. cpp:enumerator:: CIRRUS

			Cirrus clouds.

		.. cpp:enumerator:: CIRROSTRATUS

			Cirrostratus clouds.

		.. cpp:enumerator:: CIRROCUMULUS

			Cirrostratus clouds.

		.. cpp:enumerator:: BLOWING_SNOW

			Blowing snow (used only in Canada).

		.. cpp:enumerator:: BLOWING_DUST

			Blowing dust (used only in Canada).

		.. cpp:enumerator:: BLOWING_SAND

			Blowing sand (used only in Canada).

		.. cpp:enumerator:: ICE_CRYSTALS

			Ice crystals (used only in Canada).

		.. cpp:enumerator:: RAIN

			Rain (used only in Canada).

		.. cpp:enumerator:: DRIZZLE

			Drizzle (used only in Canada).

		.. cpp:enumerator:: SNOW

			Snow falling from the clouds (used only in Canada).

		.. cpp:enumerator:: ICE_PELLETS

			Ice pellets (used only in Canada).

		.. cpp:enumerator:: SMOKE

			Smoke (used only in Canada).

		.. cpp:enumerator:: FOG

			Fog (used only in Canada).

		.. cpp:enumerator:: MIST

			Mist (used only in Canada).

		.. cpp:enumerator:: HAZE

			Haze (used only in Canada).

	**Acquiring group data**

		.. cpp:function:: std::vector<std::pair<Type, unsigned int>> toVector() const

			:returns: A vector of pairs Type/okta, i.e. types of clouds forming cloud layers and associated sky coverage for each layer. Sky coverage is reported in oktas or 1/8s, e.g. 1 okta means that cloud layer covers 1/8 of sky and 8 okta means that cloud layer covers entire sky (8/8 of sky).

			.. note:: Sum of oktas for all layers may exceed 8 octa if higher cloud layer is observed through the gaps in the lower cloud layer.

		.. cpp:function:: Distance baseHeight() const

			:returns: Height of the base of the cloud layer, if a single cloud layer along with its base height is reported.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


LowMidHighCloudGroup
^^^^^^^^^^^^^^^^^^^^

.. cpp:class:: LowMidHighCloudGroup

	Stores information about predominant cloud types in low, mid, and high cloud layers. This group is included in the remarks and is used in North America.

	.. cpp:enum-class:: LowLayer

		Type of low-layer clouds.

		.. cpp:enumerator::NONE

			No low-layer clouds.

		.. cpp:enumerator:: CU_HU_CU_FR

			Cumulus humilis or Cumulus fractus of dry weather or both.

		.. cpp:enumerator:: CU_MED_CU_CON

			Cumulus mediocris or congestus, with or without Cumulus of species fractus or humilis or Stratocumulus, all having their bases at the same level.

		.. cpp:enumerator:: CB_CAL

			Cumulonimbus calvus, with or without Cumulus, Stratocumulus or Stratus.

		.. cpp:enumerator:: SC_CUGEN

			Stratocumulus cumulogenitus; Cumulus may also be present.

		.. cpp:enumerator:: SC_NON_CUGEN

			Stratocumulus non-cumulogenitus (not resulting from the spreading out of Cumulus).

		.. cpp:enumerator:: ST_NEB_ST_FR

			Stratus nebulosus or Stratus fractus of dry weather, or both.

		.. cpp:enumerator:: ST_FR_CU_FR_PANNUS

			Stratus fractus or Cumulus fractus of wet weather, or both (pannus), usually below Altostratus or Nimbostratus.

		.. cpp:enumerator:: CU_SC_NON_CUGEN_DIFFERENT_LEVELS

			Cumulus and Stratocumulus other than Stratocumulus cumulogenitus, with bases at different levels.

		.. cpp:enumerator:: CB_CAP

			Cumulonimbus capillatus (often with an anvil), with or without Cumulonimbus calvus, Cumulus, Stratocumulus, Stratus or pannus.

		.. cpp:enumerator:: NOT_OBSERVABLE

			Clouds are not observable due to fog, blowing dust or sand, or other similar phenomena.

	.. cpp:enum-class:: MidLayer

		Type of mid-layer clouds.

		.. cpp:enumerator:: NONE

			No mid-layer clouds.

		.. cpp:enumerator:: AS_TR

			Altostratus translucidus.

		.. cpp:enumerator:: AS_OP_NS

			Altostratus opacus or Nimbostratus.

		.. cpp:enumerator:: AC_TR

			Altocumulus translucidus at a single level.

		.. cpp:enumerator:: AC_TR_LEN_PATCHES

			Patches (often lenticular) of Altocumulus translucidus, continually changing and occurring at one or more levels.

		.. cpp:enumerator:: AC_TR_AC_OP_SPREADING

			Altocumulus translucidus in bands, or one or more layers of Altocumulus translucidus or Altocumulus opacus, progressively invading the sky; these Altocumulus generally thicken as a whole.

		.. cpp:enumerator:: AC_CUGEN_AC_CBGEN

			Altocumulus cumulogenitus (or Altocumulus cumulonimbogenitus).

		.. cpp:enumerator:: AC_DU_AC_OP_AC_WITH_AS_OR_NS

			Altocumulus duplicatus, or Altocumulus opacus in a single layer, not progressively invading the sky, or Altocumulus with Altostratus or Nimbostratus.

		.. cpp:enumerator:: AC_CAS_AC_FLO

			Altocumulus castellanus or Altocumulus floccus.

		.. cpp:enumerator:: AC_OF_CHAOTIC_SKY

			Altocumulus of a chaotic sky, generally at several levels.

		.. cpp:enumerator:: NOT_OBSERVABLE

			Clouds are not observable due to fog, blowing dust or sand, or other similar phenomena or because of a continuous layer of lower clouds.

	.. cpp:enum-class:: HighLayer

		Type of high-layer clouds.

		.. cpp:enumerator:: NONE

			No high-layer clouds.

		.. cpp:enumerator:: CI_FIB_CI_UNC

			Cirrus fibratus and sometimes Cirrus uncinus, not progressively invading the sky.

		.. cpp:enumerator:: CI_SPI_CI_CAS_CI_FLO

			Cirrus spissatus, in patches or entangled sheaves, that usually do not increase and sometimes appear to be the remains of the upper part of a Cumulonimbus; or Cirrus castellanus or Cirrus floccus.

		.. cpp:enumerator:: CI_SPI_CBGEN

			Cirrus spissatus cumulonimbogenitus.

		.. cpp:enumerator:: CI_FIB_CI_UNC_SPREADING

			Cirrus uncinus, Cirrus fibratus or both, progressively invading the sky; they generally thicken as a whole.

		.. cpp:enumerator:: CI_CS_LOW_ABOVE_HORIZON

			Cirrus (often in bands) and Cirrostratus, or Cirrostratus alone, progressively invading the sky; they generally thicken as a whole, but the continuous veil does not reach 45° above the horizon.

		.. cpp:enumerator:: CI_CS_HIGH_ABOVE_HORIZON

			Cirrus (often in bands} and Cirrostratus, or Cirrostratus alone, progressively invading the sky; they generally thicken as a whole; the continuous veil extends more than 45° above the horizon, without the sky being totally covered.

		.. cpp:enumerator:: CS_NEB_CS_FIB_COVERING_ENTIRE_SKY

			Cirrostratus covering the whole sky.

		.. cpp:enumerator:: CS

			Cirrostratus not progressively invading the sky and not entirely covering it.

		.. cpp:enumerator:: CC

			Cirrocumulus alone, or Cirrocumulus predominant among the high-layer clouds.

		.. cpp:enumerator:: NOT_OBSERVABLE

			Clouds are not observable due to fog, blowing dust or sand, or other similar phenomena or because of a continuous layer of lower clouds.

	**Acquiring group data**

		.. cpp:function:: LowLayer lowLayer() const

			:returns: Predominant type of low-layer clouds.

		.. cpp:function:: MidLayer midLayer() const

			:returns: Predominant type of mid-layer clouds.

		.. cpp:function:: HighLayer highLayer() const

			:returns: Predominant type of high-layer clouds.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if all cloud layers above 'not observable' cloud layer are also 'not observable'; ``false`` otherwise.


LightningGroup
^^^^^^^^^^^^^^

.. cpp:class:: LightningGroup

	Stores information about observed lightning flashes. This group is included in the remarks and is used in North America.

	.. cpp:enum-class:: Frequency

		Frequency of lightning flashes.

		.. cpp:enumerator:: NONE

			Frequency was not speficied.

		.. cpp:enumerator:: OCCASIONAL

			Less than 1 flash per minute.

		.. cpp:enumerator:: FREQUENT

			1 to 6 flashes per minute.

		.. cpp:enumerator:: CONSTANT

			More than 6 flashes per minute.

	**Acquiring group data**

		.. cpp:function:: Frequency frequency() const

			:returns: Observed frequency of lightning flashes.

		.. cpp:function:: Distance distance() const

			:returns: Currently this function only returns a non-reported value with modifier :cpp:enumerator:`Distance::Modifier::DISTANT` if distant (10 to 30 nautical miles) lightning is reported in this group. Otherwise the function returns a non-reported value with the modifier :cpp:enumerator:`Distance::Modifier::NONE`.

		.. cpp:function:: std::vector<Direction> directions() const

			:returns: Vector of directions where the lightning was reported (may include Overhead direction).

	**Lightning types**

		.. note:: More than one lightning type may be reported in this group.

		.. cpp:function:: bool isCloudGround() const

			:returns: ``true`` if cloud-to-ground lightning is observed and reported in this group, ``false`` otherwise.

		.. cpp:function:: bool isInCloud() const

			:returns: ``true`` if in-cloud lightning is observed and reported in this group, ``false`` otherwise.

		.. cpp:function:: bool isCloudCloud() const

			:returns: ``true`` if cloud-to-cloud lightning is observed and reported in this group, ``false`` otherwise.

		.. cpp:function:: bool isCloudAir() const

			:returns: ``true`` if cloud to air (without strike to the ground) lightning is observed and reported in this group, ``false`` otherwise.

		.. cpp:function:: bool isUnknownType() const

			:returns: ``true`` if lightning type other than listed above is reported in this group, ``false`` otherwise.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if there are no unknown lightning types in this group.


VicinityGroup
^^^^^^^^^^^^^

.. cpp:class:: VicinityGroup

	Stores information about significant phenomena observed in the vicinity of the station. This group is included in the remarks.

	.. cpp:enum-class:: Type

		Type of the phenomena observed.

		.. cpp:enumerator:: THUNDERSTORM

			Thunderstorm.

		.. cpp:enumerator:: CUMULONIMBUS

			Cumulonimbus cloud.

		.. cpp:enumerator:: CUMULONIMBUS_MAMMATUS

			Cumulonimbus cloud with mammatus.

		.. cpp:enumerator:: TOWERING_CUMULUS

			Towering cumulus cloud.

		.. cpp:enumerator:: ALTOCUMULUS_CASTELLANUS

			Altocumulus castellanus clouds.

		.. cpp:enumerator:: STRATOCUMULUS_STANDING_LENTICULAR

			Standing lenticular cloud of genus stratocumulus.

		.. cpp:enumerator:: ALTOCUMULUS_STANDING_LENTICULAR

			Standing lenticular cloud of genus altocumulus.

		.. cpp:enumerator:: CIRROCUMULUS_STANDING_LENTICULAR

			Standing lenticular cloud of genus cirrocumulus.

		.. cpp:enumerator:: ROTOR_CLOUD

			Rotor cloud.

		.. cpp:enumerator:: VIRGA

			Virga.

		.. cpp:enumerator:: PRECIPITATION_IN_VICINITY

			Precipitation.

		.. cpp:enumerator:: FOG

			Fog.

		.. cpp:enumerator:: HAZE

			Haze.

		.. cpp:enumerator:: SMOKE

			Smoke.

		.. cpp:enumerator:: BLOWING_SNOW

			Blowing snow.

		.. cpp:enumerator:: BLOWING_SAND

			Blowing sand.

		.. cpp:enumerator:: BLOWING_DUST

			Blowing dust.


	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of observed phenomena.

		.. cpp:function:: Distance distance() const

			:returns: Distance at to the observed phenomena (if reported in the group) or non-reported value with modifier :cpp:enumerator:`Distance::Modifier::DISTANT` if distant (10 to 30 nautical miles) phenomena is reported in this group, or non-reported value with modifier :cpp:enumerator:`Distance::Modifier::NONE` if no distance was specified.

		.. cpp:function:: std::vector<Direction> directions() const

			:returns: Vector of directions where the phenomena was observed (may include Overhead direction).

		.. cpp:function:: Direction movingDirection() const

			:returns: Direction in which phenomena is moving or non-reported direction if the phenomena is not moving or direction is not specified.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if group syntax is complete (e.g. no stray ``AND`` or ``MOV`` at the end of the group), and ``false`` otherwise.


MiscGroup
^^^^^^^^^

.. cpp:class:: MiscGroup

	Stores various data provided in METAR or TAF report.

	.. cpp:enum-class:: Type

		Indicates the type of the data reported in this group.

		.. cpp:enumerator:: SUNSHINE_DURATION_MINUTES

			Sunshine duration in minutes that occurred the previous calendar day (or zero if no sunshine occurred).

		.. cpp:enumerator:: CORRECTED_WEATHER_OBSERVATION

			This group designates a corrected weather observation; value reports the sequential number of correction, e.g. 1st, 2nd, 3rd, etc; this group is only used in Canada.

		.. cpp:enumerator:: DENSITY_ALTITUDE

			Density altitude (in feet) reported in remarks. An empty ``std::optional`` indicates missing density altitude data (coded ``DENSITY ALT MISG`` in remarks).

		.. cpp:enumerator:: HAILSTONE_SIZE

			Largest hailstone size in inches with increments of 1/4 inch.

		.. cpp:enumerator:: COLOUR_CODE_BLUE

			Visibility >8000 m AND no cloud obscuring 3/8 or more below 2500 feet. 

		.. cpp:enumerator:: COLOUR_CODE_WHITE

			Visibility >5000 m AND no cloud obscuring 3/8 or more below 1500 feet.

		.. cpp:enumerator:: COLOUR_CODE_GREEN

			Visibility >3700 m AND no cloud obscuring 3/8 or more below 700 feet.

		.. cpp:enumerator:: COLOUR_CODE_YELLOW1

			Visibility >2500 m AND no cloud obscuring 3/8 or more below 500 feet.

		.. cpp:enumerator:: COLOUR_CODE_YELLOW2

			Visibility >1600 m AND no cloud obscuring 3/8 or more below 300 feet.

		.. cpp:enumerator:: COLOUR_CODE_AMBER

			Visibility >800 m AND no cloud obscuring 3/8 or more below 200 feet.

		.. cpp:enumerator:: COLOUR_CODE_RED

			Visibility <800 m OR clouds obscuring 3/8 or more below 200 feet.

		.. cpp:enumerator:: COLOUR_CODE_BLACKBLUE

			Same as :cpp:enumerator:`COLOUR_CODE_BLUE` but also indicates that aerodrome is closed e.g. due to snow accumulation.

		.. cpp:enumerator:: COLOUR_CODE_BLACKWHITE

			Same as :cpp:enumerator:`COLOUR_CODE_WHITE` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKGREEN

			Same as :cpp:enumerator:`COLOUR_CODE_GREEN` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKYELLOW1

			Same as :cpp:enumerator:`COLOUR_CODE_YELLOW1` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKYELLOW2

			Same as :cpp:enumerator:`COLOUR_CODE_YELLOW2` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKAMBER

			Same as :cpp:enumerator:`COLOUR_CODE_AMBER` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKRED

			Same as :cpp:enumerator:`COLOUR_CODE_RED` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: FROIN

			Indicates frost on the instrument (e.g. due to fog depositing rime). No data are provided.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of value reported in this group.

		.. cpp:function:: std::optional<float> data() const

			:returns: The value reported in this group, or empty ``std::optional`` if the value is not reported.

				.. note:: empty ``std::optional`` is always returned for colour codes (e.g. BLU or BLACKRED).


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


UnknownGroup
^^^^^^^^^^^^

.. cpp:class:: UnknownGroup

	The groups in METAR or TAF report that were not recognised by the parser, are stored as Unknown Groups.

	Unknown group does not store any information and serves as a placeholder.

	Uninterrupted sequence of two or more groups which were not recognised by parser is represented by a single UnknownGroup rather than sequence of several UnknownGroup. E.g. entire text "LAST NO AMDS" will be represented by a single UnknownGroup.

	.. note:: UnknownGroup does not store any information but the text not recognised by parser may be acquired via :cpp:var:`GroupInfo::rawString`.

	**Acquiring group data**

		.. cpp:function:: std::string toString() const

			:returns: Content of plain text group in form of ``std::string``.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if the information stored in the group is valid and consistent, and ``false`` otherwise.

				Plain text group is considered valid if it contains a non-empty string.



Parsers and related types
-------------------------

ReportType
^^^^^^^^^^

.. cpp:enum-class:: ReportType

		Autodetected METAR or TAF report type.

		.. cpp:enumerator:: UNKNOWN

			Unable to detect a report type (e.g. due to malformed report).

		.. cpp:enumerator:: METAR

			Report is METAR.

		.. cpp:enumerator:: TAF

			Report is TAF.


ReportError
^^^^^^^^^^^

.. cpp:enum-class:: ReportError

	Errors which may occur when parsing a METAR or TAF report.

	.. cpp:enumerator:: NONE

		No error, report parsed successfully.

		.. note: This enumerator only means that the report overall syntax is correct and the report is not malformed. It does not guarantee that all groups were recognised by the parser. Unrecognised groups are treated as Plain Text Groups (see :cpp:class:`metaf::PlainTextGroup`).

	.. cpp:enumerator:: EMPTY_REPORT

		The report source string is empty or contains only report end designator ``=``.

	.. cpp:enumerator:: EXPECTED_REPORT_TYPE_OR_LOCATION

		The parser is expecting the report to start with either report type or ICAO location but the report starts with some other group.

		At the beginning of the report the report type should be specified as follows:
		
			- METAR or SPECI: the report type is METAR.
			- TAF: the report type is TAF.

		However in practice this report type in both METARs and TAFs may be missing.

		If the report type is missing then the report must begin with the ICAO location group (see :cpp:class:`metaf::LocationGroup`).

		AMD or COR groups are not allowed at the beginning of the report (i.e. if report type is missing).

	.. cpp:enumerator:: EXPECTED_LOCATION

		The parser expects an ICAO location group (see :cpp:class:`metaf::LocationGroup`) in this position but encounters some other group.

	.. cpp:enumerator:: EXPECTED_REPORT_TIME

		The parser expects a report release time group (see :cpp:class:`metaf::ReportTimeGroup`) in this position but encounters some other group.

	.. cpp:enumerator:: EXPECTED_TIME_SPAN

		The parser expects a time span group (see :cpp:class:`metaf::TrendGroup` and :cpp:enumerator:`metaf::TrendGroup::Type::TIME_SPAN`) in this position but encounters some other group.

		This error occurs when the validity time is not specified for the TAF report.

	.. cpp:enumerator:: UNEXPECTED_REPORT_END

		The report should not end at this position, more groups are expected according to the report format but actually are missing. This error occurs if either stray report end designator (``=``) is placed in the middle of the report or if only part of the report is included in the source string.

	.. cpp:enumerator:: UNEXPECTED_GROUP_AFTER_NIL

		This error occurs if any group is encountered after NIL. (see :cpp:enumerator:`metaf::KeywordGroup::NIL`).

		.. note: NIL means missing report and must not be included in non-empty report.

	.. cpp:enumerator:: UNEXPECTED_GROUP_AFTER_CNL

		This error occurs if any group is encountered after CNL. (see :cpp:enumerator:`metaf::KeywordGroup::CNL`).

		.. note: CNL means canceled report and must not be included in non-empty report.

	.. cpp:enumerator:: UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY

		This error occurs if NIL or CNL are found in the middle of non-empty reports (see :cpp:enumerator:`metaf::KeywordGroup::NIL` and :cpp:enumerator:`metaf::KeywordGroup::CNL`).

		.. note: NIL means missing report and CNL means canceled report; these groups must not be included is the report which contains any actual observation or forecast.

	.. cpp:enumerator:: AMD_ALLOWED_IN_TAF_ONLY

		Group AMD which designates amended report (see :cpp:enumerator:`metaf::KeywordGroup::AMD`) is only used in TAF reports. This error occurs if AMD is encountered in a METAR report.

		.. note: COR (see :cpp:enumerator:`metaf::KeywordGroup::COR`) may be used in both METAR and TAF reports.

	.. cpp:enumerator:: CNL_ALLOWED_IN_TAF_ONLY

		Group CNL which designates canceled report (see :cpp:enumerator:`metaf::KeywordGroup::CNL`) is only used in TAF reports. 

		Since METAR reports contain the actual weather observation, canceling a METAR report is a semantic error.

		This error occurs if CNL is encountered in a METAR report in place of NIL.

	.. cpp:enumerator:: MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY

		Maintenance indicator is used for weather reports produced by automated station and cannot be included in TAF.

		This error occurs if maintenance indicator ($) is encountered in a TAF report.

	.. cpp:enumerator:: REPORT_TOO_LARGE

		Too many groups included in the report. Group number limit is specified in the argument ``groupLimit`` of :cpp:func:`Parser::parse()`.


ReportPart
^^^^^^^^^^

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


ReportMetadata
^^^^^^^^^^^^^^

.. cpp:struct:: ReportMetadata

	Stores information related to entire report rather than individual groups.

	.. cpp:var:: ReportType type

		Contains report type (:cpp:enumerator:`metaf::ReportType::METAR` or :cpp:enumerator:`metaf::ReportType::TAF`) autodetected by parser during parse of the last report. :cpp:enumerator:`metaf::ReportType::UNKNOWN` is used if the report is malformed and it is not possible to autodetect its type.

	.. cpp:var:: Error error
	
		Contains syntax error encountered by parser during parsing or :cpp:enumerator:`metaf::Error::NONE` if the report was parsed successfully.

	.. cpp:var:: std::optional<MetafTime> reportTime

		Report release time, acquired from :cpp:class:`ReportTimeGroup` or empty optional if :cpp:class:`ReportTimeGroup` has not been encountered yet during the report parsing or is not present in the report.


GroupInfo
^^^^^^^^^

.. cpp:struct:: GroupInfo

	Contains data on the single METAR or TAF group processed by parser.

	.. cpp:var:: Group group

		Contains all information included in the METAR or TAR group which is recognised by parser.

	.. cpp:var:: ReportPart reportPart

		To which part of the report this group belongs (e.g. header, METAR or TAF report body, remarks).

	.. cpp:var:: std::string rawString

		METAR or TAF group source string which was parsed to extract info.


GroupParser
^^^^^^^^^^^

.. cpp:class:: GroupParser

	The purpose of this class is to parse a single METAR or TAF group. To parse entire METAR or TAF report use :cpp:class:`metaf::Parser`.

	.. cpp:function:: static Group parse(const std::string & group, ReportPart reportPart, const ReportGlobalData & reportData)

		:param group: Source string which contains a single METAR or TAF group.

		:param reportPart: Report part to which the source string belongs.

		:param reportData: Information related to the entire report rather than individual group. ``metaf::noReportData`` may be used for parsing individual groups if such information is not needed.

		:returns: :cpp:type:`metaf::Group` holding a particular group type or :cpp:class:`metaf::PlainTextGroup` if the format was not recognised.


ParseResult
^^^^^^^^^^^

.. cpp:struct:: ParseResult

	Contains result of report parsing returned by :cpp:func:`metaf::Parser::parse()` method.

	.. cpp:var:: ReportMetadata reportMetadata

		Contains information on entire report rather than any individual group.

	.. cpp:var:: std::vector<GroupInfo> groups

		A vector of parsed individual groups from METAR or TAF report.


Parser
^^^^^^

.. cpp:class:: Parser

	Parser class is used to parse strings which contain raw METAR or TAF reports, check for syntax errors, autodetect report type and produce a vector of individual groups.

		.. cpp:function:: static ParseResult parse (const std::string & report, size_t groupLimit = 100)

			Parses a METAR or TAF report, checks its syntax, detects report type and parses each group separately.

			:returns: :cpp:class:`metaf::ParseResult` which contains autodetected type or METAR or TAF report, syntax error type (if occurred) and vector of individual :cpp:type:`metaf::GroupInfo` corresponding to METAR or TAF groups. If syntax error is encountered, this means that only the part of the METAR or TAF report before syntax error was parsed.

			.. note:: If report is parsed successfully, it does not guarantee that all groups were recognised by the parser. Unrecognised groups are treated as Plain Text Groups (see :cpp:class:`metaf::PlainTextGroup`).

			:param report: String which contains a METAR or TAF report.

			:param groupLimit: Maximum number of the groups allowed in the report. Limiting the number of groups allows detecting large chunks of text/HTML/XML/JSON/etc. errorneously appended at the end of the METAR or TAF report. The default value of 100 is an arbitrarily set large number which would not prevent even large reports from being parsed, and generates error for malformed reports which are beyound reasonable size.

			.. note:: Presence of this parameter also guarantees that the parsing process cannot become an infinite loop in all cases.


Visitor
^^^^^^^

The purpose of Visitor is to simplify processing the results of the parsing.

Since the METAR or TAF report is parsed into the vector of type :cpp:type:`metaf::Group`, and the :cpp:type:`metaf::Group` is a variant data type, in order to handle the results the variant alternative check is required, after which the correct type can be extracted from the variant and the concrete group type can be handled.

The Group Visitor checks the type of an alternative in :cpp:type:`metaf::Group` and calls a corresponding virtual method for the concrete group type. The virtual methods are pure to make sure there is no risk of accidentally missing the handling of a particular group type.

See :doc:`tutorial` for the tutorial which uses a Visitor.

.. cpp:class:: template <typename T> Visitor

	:tparam T: Type returned by virtual methods for concrete group types. Can be ``void`` if virtual methods do not need to return a value.

	.. cpp:function:: T visit(const Group & group, ReportPart reportPart = ReportPart::UNKNOWN, const std::string & rawString = std::string())

		Checks concrete group type and calls one of the virtual methods below.

		:return: Value returned by corresponding virtual method or T() if the suitable method cannot be found for the Group variant alternative.

	.. cpp:function:: T visit(const GroupInfo & groupInfo)

		Checks type of group stored in GroupInfo and calls one of the virtual methods below.

		:return: Value returned by corresponding virtual method or T() if the suitable method cannot be found for the Group variant alternative.

	.. cpp:function:: protected virtual T visitKeywordGroup(const KeywordGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitLocationGroup(const LocationGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitReportTimeGroup(const ReportTimeGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitTrendGroup(const TrendGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitWindGroup(const WindGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitVisibilityGroup(const VisibilityGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitCloudGroup(const CloudGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitWeatherGroup(const WeatherGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitTemperatureGroup(const TemperatureGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitPressureGroup(const PressureGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitRunwayStateGroup(const RunwayStateGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitSeaSurfaceGroup(const SeaSurfaceGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitMinMaxTemperatureGroup(const MinMaxTemperatureGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitPrecipitationGroup(const PrecipitationGroup & group, ReportPart reportPart, const std::string & rawString) = 0
	
	.. cpp:function:: protected virtual T visitLayerForecastGroup(const LayerForecastGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitPressureTendencyGroup(const PressureTendencyGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitCloudTypesGroup(const CloudTypesGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected T visitLowMidHighCloudGroup(const LowMidHighCloudGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected T visitLightningGroup(const LightningGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected T visitVicinityGroup(const VicinityGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitMiscGroup(const MiscGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitUnknownGroup(const UnknownGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	These methods are called by :cpp:func:`visit()` for the concrete group types. See :doc:`tutorial` for minimalistic usage example.
