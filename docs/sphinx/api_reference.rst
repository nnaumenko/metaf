API Reference
=============

.. cpp:namespace-push:: metaf

This section describes the APIs related to parsing METAR or TAF report and reading the data from parsed results.

See Extending Metaf Functionality section for APIs related to adding additional features and new groups.

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

		.. cpp:function:: bool hasInteger() const

			:returns: ``true`` if the stored value has integer component, and ``false`` if the stored value does not have an integer component or is not reported. Presence or absence of fraction component is ignored.

		.. cpp:function:: bool isFraction() const

			:returns: ``true`` if the stored value has only fraction component, and ``false`` if the stored value does not have a fraction component or has integer component or is not reported.

		.. cpp:function:: bool hasFraction() const

			:returns: ``true`` if the stored value has fraction component, and ``false`` if the stored value does not have a fraction component or is not reported. Presence or absence of integer component is ignored.

	**Miscellaneous**

		.. cpp:function:: bool isValue()

			:returns: ``true`` if integer component or both numerator and denomerator are non-empty ``std::optional``s; ``false`` otherwise.

		.. cpp:function:: bool isReported()

			:returns: ``true`` if the conditions for :cpp:func:`isValue()` are met, or the modifier is either :cpp:enumerator:`Modifier::DISTANT` or :cpp:enumerator:`Modifier::VICINITY`; ``false`` otherwise.

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

		.. cpp:enumerator:: OHD

			Overhead.

		.. cpp:enumerator:: ALQDS

			All quadrants (i.e. all directions).


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

		.. cpp:enumerator:: OVERHEAD

			Phenomena occurrs directly over the location rather than in a distance.

		.. cpp:enumerator:: ALQDS

			Direction is reported as all quadrants (i.e. in all directions).


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

		.. cpp:function:: static std::vector<Direction> sectorDirectionsToVector(const Direction & dir1, const Direction &dir2)

			:returns: ``std::vector`` if the stored



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

	.. cpp:enum-class:: Status

		The status of surface friction value.

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

		.. cpp:function:: Status status() const

			:returns: Status of surface friction value.

		.. cpp:function:: std::optional<float> coefficient() const

			:returns: The value of friction coefficient or empty ``std::optional`` if the value is not reported, unreliable or unmeasurable.

				If :cpp:enum:`BrakingAction` was reported, then 'worst' (i.e. least) friction coefficient for the range specified by stored braking action value is returned (e.g. if braking action was reported as :cpp:enumerator:`BrakingAction::MEDIUM_GOOD` then 0.36 is returned).

		.. cpp:function:: BrakingAction brakingAction() const

			:returns: Braking action value corresponding to stored value.

				If friction coefficient was reported, a range of :cpp:enum:`BrakingAction` where this value of the friction coefficient fits is returned (e.g. if friction coefficient with value 0.33 was reported, then :cpp:enumerator:`BrakingAction::MEDIUM` is returned).


	**Miscellaneous**

		.. cpp:function:: bool isReported() const

			:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored. Corresponds to :cpp:enumerator:`Status::NOT_REPORTED`.


		.. cpp:function:: bool isUnreliable() const

			:returns: ``true`` if the stored value is unmeasurable or the measurement result is unreliable, and ``false`` otherwise. When the value is not reported, ``false`` is returned. Corresponds to :cpp:enumerator:`Status::UNRELIABLE`.


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

		.. cpp:enumerator:: OMMITTED

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

		.. cpp:function:: bool isOmmitted() const

			:returns: ``true`` if no qualifier, no descriptor, no weather phenomena, no event and no event time is stored in this instance, ``false`` if any of these conditions is not met.


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if the weather phenomena is valid, and ``false`` otherwise.

				The information is considered valid if all of the following conditions is met: 
					 - weather phenomena is non-empty, i.e. at lease one qualifier or descriptor or weather must be specified;
					 - event time is valid if specified;
					 - if descriptor FZ (freezing) is present, the weather phenomena must contain FG (fog), or the precipitation type which is potentially may freeze: UP (undetermined precipitation), or RA(rain), or DZ (drizzle); other precipitation may be present alone with specified above, e.g. ``FZRASN`` (freezing rain and snow) is valid, while ``FZSNPL`` (freezing snow and ice pellets) is not valid;



Group
-----

.. cpp:type:: Group = std::variant<FixedGroup, LocationGroup, ReportTimeGroup, TrendGroup, WindGroup, VisibilityGroup, CloudGroup, WeatherGroup, TemperatureGroup, TemperatureForecastGroup, PressureGroup, RunwayVisualRangeGroup, RunwayStateGroup, SecondaryLocationGroup, RainfallGroup, SeaSurfaceGroup, ColourCodeGroup, MinMaxTemperatureGroup, PrecipitationGroup, LayerForecastGroup, PressureTendencyGroup, CloudTypesGroup, CloudLayersGroup, LightningGroup, VicinityGroup, MiscGroup, UnknownGroup>

	Group is an ``std::variant`` which holds all group classes. It is used by :cpp:class:`metaf::Parser` to return the results of report parsing (see :cpp:class:`metaf::ParseResult`).



Group types
-----------

This section contains the information on each group class which stores information on individual types of METAR/TAF groups.


FixedGroup
^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports (in METAR or TAF report body).

.. image:: fixedgroup.svg

The following syntax corresponds to this group in METAR/TAF reports (in remarks section).

.. image:: fixedgrouprmk.svg

.. cpp:class:: FixedGroup

	Fixed group represent a text which is never modified if it is included in the report.

	For example, report types METAR, SPECI or TAF at the beginning of the report or CAVOK in the report body are always spelled exactly the same way and have no modifications.

	.. cpp:enum-class:: Type

		Designates the fixed text which is represented by this group.

		.. cpp:enumerator:: INCOMPLETE

			One or more groups have been recognised by the parser as fixed group(s) but the end of the report was reached unexpectedly.

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

		.. cpp:enumerator:: R_SNOCLO

			Aerodrome is closed due to snow accumulation.

			This group may be used in form of ``SNOCLO`` or ``R/SNOCLO``.

		.. cpp:enumerator:: CAVOK

			Ceiling and visibility OK; all of the following conditions are met:

				- Visibility 10 km or more in all directions.

				- No cloud below 5000 feet (1500 meters).

				- No cumulonimbus or towering cumulus clouds.

				- no significant weather phenomena.

		.. cpp:enumerator:: NSW

			Nil significant weather.

			This group is only used in trends and indicates the end of a significant weather phenomena.

		.. cpp:enumerator:: WSCONDS

			This group indicates that potential wind shear conditions are present but there's not enough information to reliably forecast height, direction and speed of wind shear.

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

		.. cpp:enumerator:: RVRNO

			Runway visual range should be reported but is missing.

		.. cpp:enumerator:: PWINO

			Indicates that automated station is equipped with present weather identifier and this sensor is not operating.

		.. cpp:enumerator:: PNO

			Indicates that automated station is equipped with tipping bucket rain gauge and this sensor is not operating.

		.. cpp:enumerator:: FZRANO

			Indicates that automated station is equipped with freezing rain sensor and this sensor is not operating.

		.. cpp:enumerator:: TSNO

			Indicates that automated station is equipped with lightning detector and this sensor is not operating.

		.. cpp:enumerator:: SLPNO

			Mean sea-level pressure information is not available.

		.. cpp:enumerator:: FROIN

			Frost on the instrument (e.g. due to fog depositing rime).

		.. cpp:enumerator:: CLD_MISG

			Sky condition data (cloud data) is missing.

		.. cpp:enumerator:: ICG_MISG

			Icing data is missing.

		.. cpp:enumerator:: PCPN_MISG

			Precipitation data is missing.

		.. cpp:enumerator:: PRES_MISG

			Atmospheric pressure (altimeter) data is missing.

		.. cpp:enumerator:: RVR_MISG

			Runway visual range data is missing.

		.. cpp:enumerator:: T_MISG

			Temperature data is missing.

		.. cpp:enumerator:: TD_MISG

			Dew point data is missing.

		.. cpp:enumerator:: VIS_MISG

			Visibility data is missing.

		.. cpp:enumerator:: WND_MISG

			Wind data is missing.

		.. cpp:enumerator:: WX_MISG

			Weather phenomena data is missing.

		.. cpp:enumerator:: TS_LTNG_TEMPO_UNAVBL

			Thunderstorm/lightning data is missing.


	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of the fixed text group.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: This method is for compatibility only and always returns ``true`` for this group.


LocationGroup
^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: locationgroup.svg

Examples of the raw report data are ``UKLL`` and ``K2J3``.

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

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: reporttimegroup.svg

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

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: trendgroup.svg

.. note:: Spaces between sequential groups in METAR/TAF report are not shown. 

See :doc:`basics` for more information on weather trends and how they are reported.

Examples of the raw report data are ``NOSIG``, ``BECMG``, ``TEMPO``, ``INTER``, ``3100/3124``, ``FM311000``, ``BECMG AT2000``, ``BECMG TL0040``, ``TEMPO FM2200 TL2215``, ``BECMG 3100/3107``, ``PROB30 3100/3104``, ``PROB30 TEMPO 3107/3109``, ``INTER 3100/3102``, etc.

.. warning:: Old TAF format (before November 2008) uses different format (time without date) for time spans and trends; the current version does not decode this old format.

.. cpp:class:: TrendGroup

	Stores information about weather trends which may be stored in one or several METAR or TAF groups.

	.. cpp:enum-class:: Type

		Type of the stored trend group.

		.. cpp:enumerator:: NONE

			Indicates that this group stores a valid but incomplete trend group or combination of such groups.

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

			All previous weather conditions are superseded by the other weather conditions since the specified time.

		.. cpp:enumerator:: TIME_SPAN

			The following weather conditions are expected to prevail during the specified time period.

			This group is only used in TAF report and must be included before TAF report body to indicate the period when the entire forecast is applicable.

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

		.. cpp:function:: std::optional<MetafTime> timeTill() const

			:returns: End time of trend's time span or empty ``std::optional`` if no time span or no end time were specified.

		.. cpp:function:: std::optional<MetafTime> timeAt() const

			:returns: Expected time of event or empty ``std::optional`` if no expected time of event was specified.

		.. note:: Trend group can have *either* begin time, end time, timespan with begin and end time *or* expected time of event. 

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if all of the reported times (begin time / end time / expected event time) are valid (see :cpp:func:`MetafTime::isValid()`).

				Alternatively returns ``false`` if any of the time values above are not valid.


WindGroup
^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports (in METAR or TAF report body).

.. image:: windgroup.svg

The following syntax corresponds to this group in METAR/TAF reports (in remarks section).

.. image:: windgrouprmk.svg

Examples of the raw report data are ``11003KT``, ``23007G14KT``, ``VRB01MPS``, ``00000KT``, ``/////KT``, ``29003KT 260V330``, ``WS020/05065KT``, ``WSHFT 1851``, ``WSHFT 30 FROPA``, and ``PK WND 29026/2204``.

.. cpp:class:: WindGroup

	Stores information about surface wind (including variable wind direction sector if reported), wind shear, wind shift, and peak wind.

	.. cpp:enum-class:: Type

		Group type which specifies what kind of data stored within this group.

		.. cpp:enumerator:: SURFACE_WIND

			Surface wind information is stored. Use :cpp:func:`direction()`, :cpp:func:`windSpeed()`, and :cpp:func:`gustSpeed()`.

		.. cpp:enumerator:: SURFACE_WIND_CALM

			Surface wind calm (i.e. no wind) information is stored. :cpp:func:`windSpeed()` is always zero.

		.. cpp:enumerator:: VARIABLE_WIND_SECTOR

			Only variable wind sector information is stored. Use :cpp:func:`varSectorBegin()`, and :cpp:func:`varSectorEnd()`.

		.. cpp:enumerator:: SURFACE_WIND_WITH_VARIABLE_SECTOR

			Surface wind information with variable wind sector information is stored. Use :cpp:func:`direction()`, :cpp:func:`windSpeed()`, :cpp:func:`gustSpeed()`, :cpp:func:`varSectorBegin()`, and :cpp:func:`varSectorEnd()`.

		.. cpp:enumerator:: WIND_SHEAR

			Wind shear information is stored. Use :cpp:func:`direction()`, :cpp:func:`windSpeed()`, :cpp:func:`gustSpeed()`, and :cpp:func:`height()`.

		.. cpp:enumerator:: WIND_SHIFT

			Wind shift information is stored, which means that wind direction changed 45 degrees or more in less than 15 minutes with sustained wind speed of 10 knots. Use :cpp:func:`eventTime()`.

		.. cpp:enumerator:: WIND_SHIFT_FROPA

			Wind shift information is stored, which means that wind direction changed 45 degrees or more in less than 15 minutes with sustained wind speed of 10 knots. Wind shift is associated with frontal passage. Use :cpp:func:`eventTime()`.

		.. cpp:enumerator:: PEAK_WIND

			Information on peak wind since last METAR is stored in this group. Use :cpp:func:`direction()`, :cpp:func:`windSpeed()`, and :cpp:func:`eventTime()`.


	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Wind group type, i.e. what kind of information is stored.

		.. cpp:function:: Direction direction() const

			:returns: Wind direction; typicaly a direction value in degrees but also can be variable or non-reported.

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

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: visibilitygroup.svg

.. note:: Spaces between sequential groups in METAR/TAF report are not shown.

Examples of the raw report data are ``3600``, ``9999``, ``0050``, ``9999NDV``, ``1100W``, ``3SM``, ``25SM``, ``1/4SM``, ``2 1/4SM``, ``M1/4SM``, ``P6SM``, ``////SM``.

.. cpp:class:: VisibilityGroup

	Stores information about prevailing visibility or visibility towards cardinal direction.

	See also CAVOK (:cpp:enumerator:`metaf::FixedGroup::Type::CAVOK`) which may be used to specify visibility of 10 km or more in all directions.

	.. cpp:enum-class:: Type

		Group type which specifies what kind of data stored within this group.

		.. cpp:enumerator:: PREVAILING

			Prevailing visibility information is stored. Use :cpp:func:`visibility()`.

		.. cpp:enumerator:: PREVAILING_NDV

			Prevailing visibility information is stored and this station cannot differentiate the directional variation of visibility. Use :cpp:func:`visibility()`.

		.. cpp:enumerator:: DIRECTIONAL

			Additional directional visibility information is stored. Use :cpp:func:`visibility()` and :cpp:func:`direction()`.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Visibility group type, i.e. what kind of information is stored.

		.. cpp:function:: Distance visibility() const

			:returns: Visibility value. Values in meters are integer, values in statute miles may be fractional. May contain 'less than' or 'more than' modifiers or may be non-reported value.

		.. cpp:function:: Distance minVisibility() const

			:returns: Minimum visibility value if the visibility is variable. Currently always returns a non-reported value.

		.. cpp:function:: Distance maxVisibility() const

			:returns: Maximum visibility value if the visibility is variable. Currently always returns a non-reported value.

		.. cpp:function:: Direction direction() const

			:returns:  Cardinal direction if directional visibility is specified or omitted value if prevailing visibility is specified. Automated stations may also report No Directional Variation if the station is not capable of providing directional visibility.

		.. cpp:function:: Direction sectorBegin() const

			:returns: Cardinal direction if sector of directions is specified. Currently always returns an ommitted value.

		.. cpp:function:: Direction sectorEnd() const

			:returns: Cardinal direction if sector of directions is specified. Currently always returns an ommitted value.


	**Miscellaneous**

		.. cpp:function:: bool isPrevailing() const

			:returns: ``true`` if the group contains prevailing visibility.

		.. cpp:function:: bool isDirectional() const

			:returns: ``true`` if the group contains directional visibility.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored visibility information is valid, and ``false`` otherwise.

				The information is considered valid if all of the following conditions are met: 
					- The stored visibility and direction values are valid (if reported);
					- The group does not represent an incomplete integer group (i.e. single digit group not followed by fraction and SM designator).


CloudGroup
^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: cloudgroup.svg

Examples of the raw report data are ``FEW001``, ``SCT000``, ``BKN300``, ``OVC250``, ``FEW019TCU``, ``FEW013///``, ``//////CB``, ``//////``, ``CLR``, ``SKC``, ``NCD``, ``NSC``, ``VV002``, and ``VV///``.

.. cpp:class:: CloudGroup

	Stores information about a single cloud layer, cloud-like obscuration, lack of cloud cover or vertical visibility.

	.. cpp:enum-class:: Amount

		Amount (cover) of the cloud layer.

		See also CAVOK (:cpp:enumerator:`metaf::FixedGroup::Type::CAVOK`) which may be used to specify no cloud below 5000 feet (1500 meters) and no cumulonimbus or towering cumulus clouds.

		.. cpp:enumerator::	NOT_REPORTED

			Cloud cover (amount of cloud) is not reported.

		.. cpp:enumerator:: NCD

			No cloud detected: automated weather station did not detect any clouds. Either no clouds are present or sensor error occurred.

		.. cpp:enumerator:: NSC

			Nil significant clouds: no cloud below 5000 feet (1500 meters), no cumulonimbus or towering cumulus, and no vertical visibility restriction.

			.. note:: CAVOK group (:cpp:enumerator:`metaf::FixedGroup::Type::CAVOK`) is also used to indicate no cloud below 5000 feet (1500 meters) and no cumulonimbus or towering cumulus clouds.

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

	.. cpp:enum-class:: Type

		Significant convective type of the cloud.

		.. cpp:enumerator::NOT_REPORTED

			Convective cloud type is not reported.

		.. cpp:enumerator::NONE

			No significant convective cloud type.

		.. cpp:enumerator::TOWERING_CUMULUS

			Towering cumulus clouds.

		.. cpp:enumerator::CUMULONIMBUS

			Cumulonimbus clouds.

	**Acquiring group data**

		.. cpp:function:: Amount amount() const

			:returns: Amount (cover) of clouds in layer or clear sky conditions.

		Type type() const

			:returns: Significant convective type of cloud layer.

		.. cpp:function:: Distance height() const

			:returns: Cloud base height in the cloud layer. For clear sky, no cloud detected, nil significant cloud conditions returns a non-reported value. When sky is obscured, returns a non-reported value (use :cpp:func:`verticalVisibility()` instead).

		.. cpp:function:: Distance minHeight() const

			:returns: Minimum cloud base height if cloud base height is variable. Currently always returns a non-reported value.

		.. cpp:function:: Distance maxHeight() const

			:returns: Maximum cloud base height if cloud base height is variable. Currently always returns a non-reported value.

		.. cpp:function:: Distance verticalVisibility() const

			:returns: When sky is obscured returns a vertical visibility value (if reported). For any other condition returns a non-reported value.

	**Miscellaneous**

		.. cpp:function:: bool isVerticalVisibility() const

			:returns: ``true`` if this group contains a vertical visibility information (including non-reported vertical visibility value) rather than cloud layer information or 'no clouds' condition, and ``false`` otherwise.

		.. cpp:function:: bool isNoClouds() const

			:returns: ``true`` if this group contains an information related to 'no clouds' conditions, i.e. amount value is :cpp:enumerator:`Amount::NONE_CLR`, :cpp:enumerator:`Amount::NONE_SKC`, :cpp:enumerator:`Amount::NCD`, :cpp:enumerator:`Amount::NSC`. For any other amount value returns ``false``.

		.. cpp:function:: bool isCloudLayer() const

			:returns: ``true`` if this group contains a cloud layer information (including non-reported amount, height or type) rather than vertical visibility information or 'no clouds' condition, and ``false`` otherwise.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored cloud information is valid, and ``false`` otherwise. The information is considered valid if the value of cloud cover height or vertical visibility is valid (if reported). Zero height of cloud cover base or vertical visibility does not make the information invalid.


WeatherGroup
^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: weathergroup.svg

Examples of the raw report data are ``+RA``, ``IC``, ``-SHRASN``, ``VCSH``, ``FU``, ``VCTS``, ``RESHRA``, ``-FZDZ``, ``MIFG``, ``BLDU``, ``HZ``, ``-SHPL``, ``+TSRAGR``, ``SHRAGS``, ``//``, ``RE//``, ``RAB12``, ``RAB1153E1158DZB04``, ``RAE31SNB04PLB04E15`` etc.

.. cpp:class:: WeatherGroup

	Stores information about recent or current weather phenomena.

	.. cpp:enum-class:: Type

		What kind weather information this group represents.

		.. cpp:enumerator::	CURRENT

			Current weather information is stored in this group.

		.. cpp:enumerator:: RECENT

			Recent weather information is stored in this group.

		.. cpp:enumerator:: EVENT

			Time of beginning or ending of weather phenomena is stored in this group.


	**Acquiring group data**

		.. cpp:function:: Type type() const;

			:returns: What kind of weather information stored in this group; current weather, recent weather, weather event (beginning and ending times of weather phenomena).

		.. cpp:function:: std::vector<WeatherPhenomena> weatherPhenomena() const

			:returns: The vector or weather phenomena; each :cpp:class:`metaf::WeatherPhenomena` includes qualifier, descriptor and weather phenomena reported in this group.


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: This method is for compatibility only and always returns ``true`` for this group. All groups that are not valid weather phenomena (or invalid combinations or descriptor / qualifier / phenomena) are not recognised as a WeatherGroup.


TemperatureGroup
^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: temperaturegroup.svg

Examples of the raw report data are ``12/10``, ``20/M07``, ``10/M00``, ``00/M02``, ``M00/M00``, ``00/M00``, ``/////``, ``T02330206``, ``T10171028``, and ``T0261``.

.. cpp:class:: TemperatureGroup

	Stores information about current ambient air temperature and dew point. Group which reports values rounded to integer of degrees Celsius (e.g. ``10/M00``) is included in METAR report body. Group which reports values in tength of degrees Celsius (e.g. ``T02330206``) is used in North America and is included in remarks.

	**Acquiring group data**

		.. cpp:function:: Temperature airTemperature() const

			:returns: Ambient air temperature.

		.. cpp:function:: Temperature dewPoint() const

			:returns: Dew point.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if stored ambient air temperature and dew point information is valid, and ``false`` otherwise. The information is considered valid if the dew point is less or equal than ambient air temperature.


TemperatureForecastGroup
^^^^^^^^^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: temperatureforecastgroup.svg

Examples of the raw report data are ``TX15/3111Z``, ``TN00/3103Z``, and ``T30/1218Z``.

.. cpp:class:: TemperatureForecastGroup

	Stores information about forecast ambient air temperature along with the time when it is expected.

		.. cpp:enum-class:: Point

			Temperature point for which the forecast is reported.
			
			.. cpp:enumerator:: NOT_SPECIFIED

				Forecast temperature point is not specified; temperature expected at certain time is reported.

			.. cpp:enumerator:: MINIMUM

				Forecast for minimum temperature is reported.

			.. cpp:enumerator:: MAXIMUM

				Forecast for maximum temperature is reported.

	**Acquiring group data**

		.. cpp:function:: Point point() const

			Temperature point for which the forecast is reported.

		.. cpp:function:: Temperature airTemperature() const

			Forecast ambient air temperature.

		.. cpp:function:: MetafTime time() const

			Time when the forecast temperature is expected.

	**Additional data**

		.. cpp:function:: std::optional<float> relativeHumidity()

			:returns: Relative humidity value based on ambient air temperature and dew point stored in this group, or empty ``std::optional`` if ambient air temperature and/or dew point is not reported.


	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if forecast ambient air temperature information is valid, and ``false`` otherwise. The information is considered valid if the time is a valid value.


PressureGroup
^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: pressuregroup.svg

Examples of the raw report data are ``Q1020``, ``A2981``, ``Q////``, ``A////``, ``QNH2981INS``, ``SLP960``, ``QFE761/1015``, and ``QFE741``.

.. cpp:class:: PressureGroup

	Stores information about observed or forecast atmospheric pressure.

	.. cpp:enum-class:: Type

		.. cpp:enumerator:: OBSERVED_QNH

			Indicates that groups contains an observed mean atmospheric pressure normalised to sea level (used in METAR, e.g. ``Q1020``, ``A2981``, or remark ``SLP960``).

		.. cpp:enumerator:: FORECAST_LOWEST_QNH

			Indicates that group contains a forecast lowest sea level pressure, (e.g. ``QNH2981INS``). This group may be reported by military aerodromes of NATO countries.

		.. cpp:enumerator:: OBSERVED_QFE

			Indicates that group contains an observed actual atmospheric pressure (e.g. remark ``QFE761/1015`` or ``QFE741``). This group is used by some countries which were part of Soviet Union.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of the pressure value (observed or forecast).

		.. cpp:function:: Pressure atmosphericPressure() const

			:returns: Atmospheric pressure value.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


RunwayVisualRangeGroup
^^^^^^^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: runwayvisualrangegroup.svg

Examples of the raw report data are ``R24/P6000FT``, ``R31/0200N``, ``R26/0325N``, ``R23/3000V4500FT/D``, etc.

.. cpp:class:: RunwayVisualRangeGroup

	Stores information about visual range of a single runway.

	.. cpp:enum-class:: Trend

		Trend of runway visual range variation.

		.. cpp:enumerator:: NONE

			Trend is not specified in group.

		.. cpp:enumerator:: NOT_REPORTED

			Trend is specified as not reported.

		.. cpp:enumerator:: UPWARD

			Trend is upward (runway visual range increases / improves).

		.. cpp:enumerator:: NEUTRAL

			Trend is neutral (no significant changes to runway visual range).

		.. cpp:enumerator:: DOWNWARD

			Trend is downward (runway visual range decreases / deteriorates).

	**Acquiring group data**

		.. cpp:function:: Runway runway() const

			:returns: Runway the visual range is provided for.

		.. cpp:function:: Distance visualRange() const

			:returns: Runway visual range value (if reported) or non-reported value if variable visual range is reported.

		.. cpp:function:: Distance minVisualRange() const

			:returns: Low limit of variable visual range or non-reported value if no variable visual range is reported.

		.. cpp:function:: Distance maxVisualRange() const

			:returns: High limit of variable visual range or non-reported value if no variable visual range is reported.

		.. cpp:function:: Trend trend() const

			:returns: Trend of runway visual range variation.

		.. cpp:function:: bool isVariableVisualRange() const

			:returns: ``true`` if variable visual range is reported and ``false`` otherwise.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if runway visual range information is valid, and ``false`` otherwise. The information is considered valid if the specified runway is valid, and visual range / variable visual range are valid distance values (if reported).


RunwayStateGroup
^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: runwaystategroup.svg

Examples of the raw report data are ``R36/090060``, ``R01/810365``, ``R10/91//60``, ``R21/SNOCLO``, ``R34L/CLRD70``, etc.

.. cpp:class:: RunwayStateGroup

	Stores information about the state of runway surface and/or accumulation of deposits for a single runway. Alternatively may store information that the deposits of runway ceased to exist or that runway is closed due to snow accumulation.

	.. cpp:enum-class:: Status

		Option for the type of runway state reported: normal group with all values, CLRD group with surface friction value only, and SNOCLO group without any values.

		.. cpp:enumerator:: NORMAL

			Normal type of runway state group. Runway deposits, runway contamination extent, deposit depth, and surface friction are specified in this group (any value or values may be non-reported).

		.. cpp:enumerator:: RUNWAY_NOT_OPERATIONAL

			Runway state group indicating that the runway is not operational. Runway deposits, runway contamination extent, and surface friction may be specified in this group (any value or values may be non-reported).

		.. cpp:enumerator:: CLRD

			Runway state group indicating that previously present deposits on runway were cleared or ceased to exist. Only surface friction is specified (as an actual value or non-reported value).

		.. cpp:enumerator:: SNOCLO

			Runway state group indicating that the runway is closed due to snow accumulation. No further values are specified.

	.. cpp:enum-class:: Deposits

		Deposits on the runway.

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

		.. cpp:function:: Status status() const

			:returns: Status of runway state group. 

			If the status is :cpp:enumerator:`Status::NORMAL` then group reports runway state and Deposits, Contamination Extent, Deposit Depth and Surface Friction may be reported or non-reported.

			If the status is :cpp:enumerator:`Status::RUNWAY_NOT_OPERATIONAL` then group reports runway state and Deposits, Contamination Extent, and Surface Friction may be reported or non-reported; Deposit Depth is always not reported.

			If the status is :cpp:enumerator:`Status::CLRD` then group indicates that deposits on the runway were cleared or ceased to exist; only Surface Friction may be reported; Deposits, Contamination Extent and Deposit Depth are never reported.

			If the status is :cpp:enumerator:`Status::SNOCLO` then group indicates that runway is closed due to snow accumulation. All parameters (Deposits, Contamination Extent, Deposit Depth and Surface Friction) are never reported.

		.. cpp:function:: Deposits deposits() const

			:returns: Deposits on the runway. Not reported if the status is :cpp:enumerator:`Status::CLRD` or :cpp:enumerator:`Status::SNOCLO`.

		.. cpp:function:: Extent contaminationExtent() const

			:returns: Extent (percentage) of runway contamination with deposits. Not reported if the status is :cpp:enumerator:`Status::CLRD` or :cpp:enumerator:`Status::SNOCLO`.

		.. cpp:function:: Precipitation depositDepth() const

			:returns: Depth of the deposits on the runway or non-reported value. Not reported if the status is :cpp:enumerator:`Status::CLRD` or :cpp:enumerator:`Status::SNOCLO`.

		.. cpp:function:: SurfaceFriction surfaceFriction() const

			:returns: Surface friction or braking action or not reported value. Not reported if the status is :cpp:enumerator:`Status::SNOCLO`.

	**Validating**

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if runway state information is valid, and ``false`` otherwise. The information is considered valid if the specified runway is valid and :cpp:enum:`Extent` returned by :cpp:func:`contaminationExtent()` is not a reserved value.



SecondaryLocationGroup
^^^^^^^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR reports.

.. image:: secondarylocationgroup.svg

Examples of the raw report data are ``WS R32``, ``WS R27C``, and ``WS ALL RWY``.

.. cpp:class:: SecondaryLocationGroup

	Stores additional info details in the secondary locations (e.g. runway).

	.. cpp:enum-class::  Type

		Type of information actually stored. 

		.. cpp:enumerator:: INCOMPLETE

			One or more groups have been recognised by the parser but the end of the report was reached unexpectedly.

		.. cpp:enumerator:: WIND_SHEAR_IN_LOWER_LAYERS

			This group indicates existence of wind shear along the take-off path or approach path between runway level and 500 metres (1 600 ft) significant to aircraft operations, for the particlar runway or all runways.

	**Acquiring group data**

	.. cpp:function:: Runway runway() const

		:returns: Runway data if this secondary location is runway, and empty ``std::optional`` otherwise.

	.. cpp:function:: Direction direction() const

		:returns: Direction data if this secondary location is a cardinal direction, and empty ``std::optional`` otherwise.

	.. cpp:function:: std::string incompleteText() const

		:returns: Raw string of groups partially recognised by parser as a secondary location group, or empty string if the group is not an incomplete one (i.e. return value of :cpp:func:`type()` is other than :cpp:enumerator:`Type::INCOMPLETE`).

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``false`` for incomplete groups. For complete groups returns ``true`` if the specified runway or direction is valid, and ``false`` otherwise.


RainfallGroup
^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: rainfallgroup.svg

Examples of the raw report data are ``RF00.0/000.0``, ``RF00.2/011.2``, ``RF00.0////./``, and ``RF21.5/112.4/031.8``.

.. cpp:class:: RainfallGroup

	Stores information about recent rainfall. This group is only used in Australia.

	**Acquiring group data**

		.. cpp:function:: Precipitation rainfallLast10Minutes() const

			:returns: Rainfall for the last 10 minutes (or non-reported value).

		.. cpp:function:: Precipitation rainfallLast60Minutes() const

			:returns: Rainfall for the last 60 minutes or non-reported value.

		.. cpp:function:: Precipitation rainfallSince9AM() const

			:returns: Rainfall since 9:00AM (09:00) or non-reported value.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


SeaSurfaceGroup
^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: seasurfacegroup.svg

Examples of the raw report data are ``W02/S6``, ``W08/H5``, ``W04/S/``, ``W///S6``, ``W///S/``, ``W04/H///``, and ``W///H///``.

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


ColourCodeGroup
^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: colourcodegroup.svg

Examples of the raw report data are ``BLU``, ``WHT``, ``GRN``, ``YLO1``, ``YLO2``, ``AMB``, ``RED``, ``BLACKBLU``, ``BLACKWHT``, ``BLACKGRN``, ``BLACKYLO1``, ``BLACKYLO2``, ``BLACKAMB``, and ``BLACKRED``. 

.. cpp:class:: ColourCodeGroup

	Stores colour code information which allows quick assess of visibility and ceiling conditions. This group is used by military aerodromes of NATO countries.

	.. cpp:enum-class:: Code

		.. cpp:enumerator:: BLUE

			Visibility >8000 m AND no cloud obscuring 3/8 or more below 2500 feet.

		.. cpp:enumerator:: WHITE

			Visibility >5000 m AND no cloud obscuring 3/8 or more below 1500 feet.

		.. cpp:enumerator:: GREEN

			Visibility >3700 m AND no cloud obscuring 3/8 or more below 700 feet.

		.. cpp:enumerator:: YELLOW1

			Visibility >2500 m AND no cloud obscuring 3/8 or more below 500 feet.

		.. cpp:enumerator:: YELLOW2

			Visibility >1600 m AND no cloud obscuring 3/8 or more below 300 feet.

		.. cpp:enumerator:: AMBER

			Visibility >800 m AND no cloud obscuring 3/8 or more below 200 feet.

		.. cpp:enumerator:: RED

			Visibility <800 m OR clouds obscuring 3/8 or more below 200 feet.

	**Acquiring group data**

		.. cpp:function:: Code code() const

			:returns: Colour code.

		.. cpp:function:: bool isCodeBlack() const

			:returns: ``true`` if code BLACK (which means that aerodrome is closed e.g. due to snow accumulation) was specified additionally to main colour code, ``false`` otherwise.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


MinMaxTemperatureGroup
^^^^^^^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: minmaxtemperaturegroup.svg

Examples of the raw report data are ``401120084``, ``20012``, and ``11021``. 

.. cpp:class:: MinMaxTemperatureGroup

	Stores information about 24-hourly or 6-hourly minimum and/or maximum temperature. This group is only used in North America and included in remarks.

	.. cpp:enum-class:: ObservationPeriod

		Indicates the period of observation for which the minimum and/or maximum value is reported.

		.. cpp:enumerator:: HOURS6

			Indicates that group stores information about 6-hourly minimum/maximum temperatuer.

		.. cpp:enumerator:: HOURS24

			Indicates that group stores information about 24-hourly minimum/maximum temperatuer.

	**Acquiring group data**

		.. cpp:function:: ObservationPeriod observationPeriod() const

			:returns: Period of observation for which the minimum and/or maximum value is reported.

		.. cpp:function:: Temperature minimum() const

			:returns: Minimum temperature for the specified period (with precision to tenth of degrees Celsius). May return non-reported value if minimum temperature was not specified in this group.

		.. cpp:function:: Temperature maximum() const

			:returns: Maximum temperature for the specified period (with precision to tenth of degrees Celsius). May return non-reported value if maximum temperature was not specified in this group.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


PrecipitationGroup
^^^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: precipitationgroup.svg

Examples of the raw report data are ``P0009``, ``P////``, ``4/010``, ``60217``, ``6////``, ``70021``, ``931011``, ``933021``, ``I1001``, ``I1////``, ``I3008``, ``I6012``, and ``SNINCR 2/12``.

.. cpp:class:: PrecipitationGroup

	Stores various information about precipitation, snowfall, snow depth, and icing (typically caused by freezing precipitation). This group is only used in North America and included in remarks.

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

			Indicates that snow is increasing rapidly; use :cpp:func:`amount()` for total snowfall, and :cpp:func:`tendency()` for snow increase during last hour.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of value reported in this group.

		.. cpp:function:: Precipitation amount() const

			:returns: Amount of precipitation of specified type. May be a non-reported value.

		.. cpp:function:: Precipitation tendency() const

			:returns: Increase of decrease of precipitation during recent period.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


LayerForecastGroup
^^^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: layerforecastgroup.svg

Examples of the raw report data are ``520004``, and ``620304``. 

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

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASSIONAL

			Moderate turbulence in clear air, occasional.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT

			Moderate turbulence in clear air, frequent.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLOUD_OCCASSIONAL

			Moderate turbulence in cloud, occasional.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLOUD_FREQUENT

			Moderate turbulence in cloud, frequent.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASSIONAL

			Severe turbulence in clear air, occasional.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT

			Severe turbulence in clear air, frequent.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLOUD_OCCASSIONAL

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

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: pressuretendencygroup.svg

Example of the raw report data is ``52032``, ``5////``, ``PRESRR``, and ``PRESFR``. 

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

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: cloudtypesgroup.svg

Example of the raw report data is ``AC1CI1``, ``BLSN2SC4SC3`` and ``SC1SC1SC3AC2``.

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

			:returns: Height of the base of the cloud layer, if a single cloud layer along with its base height is reported. In current version always returns a non-reported value.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


CloudLayersGroup
^^^^^^^^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: cloudlayersgroup.svg

Example of the raw report data is ``8/578``, and ``8/903``.

.. cpp:class:: CloudLayersGroup

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

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: lightninggroup.svg

Example of the raw report data is ``LTG DSNT``, ``CONS LTGICCG OHD AND NE-SE``, etc.

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

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: vicinitygroup.svg

Example of the raw report data is ``VIRGA N``, ``SCSL ALQDS``, ``TCU 5KM S-SW MOV NE``, ``TCU DSNT W-NW AND S MOV W``, ``ACC ALQDS``, ``CB OHD AND E MOV E`` etc.

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

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of observed phenomena.

		.. cpp:function:: Distance distance() const

			:returns: Distance at to the observed phenomena (if reported in the group) or non-reported value with modifier :cpp:enumerator:`Distance::Modifier::DISTANT` if distant (10 to 30 nautical miles) phenomena is reported in this group, or non-reported value with modifier :cpp:enumerator:`Distance::Modifier::NONE` if no distance was specified.

		.. cpp:function:: std::vector<Direction> directions() const

			:returns: Vector of directions where the phenomena was observed (may include Overhead direction).

		.. cpp:function:: Direction::Cardinal movingDirection() const

			:returns: Direction in which phenomena is moving or :cpp:enumerator:`Direction::Cardinal::NONE` if the phenomena is not moving or direction is not specified.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if group syntax is complete (e.g. no stray ``AND`` or ``MOV`` at the end of the group), and ``false`` otherwise.


MiscGroup
^^^^^^^^^

The following syntax corresponds to this group in METAR/TAF reports.

.. image:: miscgroup.svg

Examples of the raw report data are ``98096``, ``CCA``, ``CCB``, and ``CCC``. 

.. cpp:class:: MiscGroup

	Stores various values provided in METAR or TAF report.

	.. cpp:enum-class:: Type

		Indicates the type of the value reported in this group.

		.. cpp:enumerator:: SUNSHINE_DURATION_MINUTES

			Sunshine duration in minutes that occurred the previous calendar day (or zero if no sunshine occurred).

		.. cpp:enumerator:: CORRECTED_WEATHER_OBSERVATION

			This group designates a corrected weather observation; value reports the sequential number of correction, e.g. 1st, 2nd, 3rd, etc; this group is only used in Canada.

	**Acquiring group data**

		.. cpp:function:: Type type() const

			:returns: Type of value reported in this group.

		.. cpp:function:: std::optional<float> value() const

			:returns: The value reported in this group, or empty ``std::optional`` if the value is not reported.

	**Validating**

		.. cpp:function:: bool isValid() const

			:returns: The following values are returned depending on type of the value:

				* If sunshine duration value is reported, always returns ``true``.

				* If number of corrected weather observation is reported, always returns ``true``.


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


	.. cpp:enumerator:: UNEXPECTED_GROUP_AFTER_MAINTENANCE_INDICATOR

		This error occurs if any group is encountered after maintenance indicator '$'. (see :cpp:enumerator:`metaf::FixedGroup::MAINTENANCE_INDICATOR`).


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


	.. cpp:enumerator:: MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY

		Maintenance indicator is used for weather reports produced by automated station and cannot be included in TAF.

		This error occurs if maintenance indicator ($) is encountered in a TAF report.


	.. cpp:enumerator:: GROUP_LIMIT_EXCEEDED

		Too many groups included in the report. Group number limit is specified in the parameter ``groupLimit`` of :cpp:func:`Parser::parse()`.


ReportPart
^^^^^^^^^^

.. cpp:enum-class:: ReportPart

	Represents the major part of the report. Report is used by :cpp:class:`metaf::GroupParser` to  parse only those group types which can potentially occur in this report part (e.g. :cpp:class:`metaf::ReportTimeGroup` is only used in report headers and NOSIG is only used in METAR report body).

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

See :doc:`getting_started` for the tutorial which uses a Visitor.

.. cpp:class:: template <typename T> Visitor

	:tparam T: Type returned by virtual methods for concrete group types. Can be ``void`` if virtual methods do not need to return a value.

	.. cpp:function:: T visit(const Group & group, ReportPart reportPart = ReportPart::UNKNOWN, const std::string & rawString = std::string())

		Checks concrete group type and calls one of the virtual methods below.

		:return: Value returned by corresponding virtual method or T() if the suitable method cannot be found for the Group variant alternative.

	.. cpp:function:: T visit(const GroupInfo & groupInfo)

		Checks type of group stored in GroupInfo and calls one of the virtual methods below.

		:return: Value returned by corresponding virtual method or T() if the suitable method cannot be found for the Group variant alternative.

	.. cpp:function:: protected virtual T visitFixedGroup(const FixedGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitLocationGroup(const LocationGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitReportTimeGroup(const ReportTimeGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitTrendGroup(const TrendGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitWindGroup(const WindGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitVisibilityGroup(const VisibilityGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitCloudGroup(const CloudGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitWeatherGroup(const WeatherGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitTemperatureGroup(const TemperatureGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitTemperatureForecastGroup(const TemperatureForecastGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitPressureGroup(const PressureGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitRunwayVisualRangeGroup(const RunwayVisualRangeGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitRunwayStateGroup(const RunwayStateGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitSecondaryLocationGroup(const SecondaryLocationGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitRainfallGroup(const RainfallGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitSeaSurfaceGroup(const SeaSurfaceGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitColourCodeGroup(const ColourCodeGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitMinMaxTemperatureGroup(const MinMaxTemperatureGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitPrecipitationGroup(const PrecipitationGroup & group, ReportPart reportPart, const std::string & rawString) = 0
	
	.. cpp:function:: protected virtual T visitLayerForecastGroup(const LayerForecastGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitPressureTendencyGroup(const PressureTendencyGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitCloudTypesGroup(const CloudTypesGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected T visitCloudLayersGroup(const CloudLayersGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected T visitLightningGroup(const LightningGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected T visitVicinityGroup(const VicinityGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitMiscGroup(const MiscGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitUnknownGroup(const UnknownGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	These methods are called by :cpp:func:`visit()` for the concrete group types. See :doc:`getting_started` for usage example.
