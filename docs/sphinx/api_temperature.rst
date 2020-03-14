Temperature
===========

.. cpp:namespace-push:: metaf

Temperature class
-----------------

	.. cpp:class:: Temperature

		Stores a temperature value. Temperature value may be not reported (i.e. no value).

		Depending on the reported type, a temperature value may be precise (i.e. in tenth of degrees Celsius) or non-precise (i.e. rounded to integer value in degrees Celsius).

		If the non-precise temperature value is rounded to zero, an additional information can be acquired whether the value represents a freezing or non-freezing temperature (i.e. slightly above or slightly below zero).

.. cpp:namespace-push:: Temperature

Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Unit

		Temperature measurement units.

		.. cpp:enumerator:: C

			Degrees Celsius.

		.. cpp:enumerator:: F

			Degrees Fahrenheit.

		.. note:: Temperature value is always stored in degrees Celsius but may be converted to degrees Fahrenheit using :cpp:func:`toUnit()`.


Acquiring the data
^^^^^^^^^^^^^^^^^^

		.. cpp:function:: std::optional<float> temperature() const

			:returns: Stored temperature value or empty ``std::optional`` if temperature value is not reported.


		.. cpp:function:: Unit unit() const

			:returns: Temperature measurement unit which was used with stored value. Currently always returns :cpp:enumerator:`Unit::C` since the value is always stored in degrees Celsius.


Converting to other measurement units
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

		.. cpp:function:: std::optional<float> toUnit(Unit unit) const

			:param unit: Measurement unit to convert the value to.

			:returns: Stored temperature value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


Additional values
^^^^^^^^^^^^^^^^^

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


Miscellaneous
^^^^^^^^^^^^^

		.. cpp:function:: bool isFreezing() const

			:returns: ``true`` if the stored temperature value is below water freezing point (0 degrees Celsius or 32 degrees Fahrenheit), and ``false`` if the stored temperature value is above freezing point.

				If the temperature value is rounded to the freezing point, isFreezing() may return either ``true`` or ``false`` based on the following conditions:

				- if the original temperature value was in range (0.0 .. -0.5) which is encoded in METAR/TAF report as ``M00``, then ``isFreezing()`` returns ``true``;

				- if the original temperature value was in range (0.5 .. 0.0] which is encoded in METAR/TAF report as ``00``, then ``isFreezing()`` returns ``false``;


		.. cpp:function:: bool isReported() const

			:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored.


		.. cpp:function:: bool isPrecise() const

			:returns: ``true`` if the value is in tenth of degree Celsius, ``false`` if the value is rounded to integer.

.. cpp:namespace-pop::


Formats
-------

Metaf recognises two temperature formats below. All formats specify values in degrees Celsius.


Standard-precision temperature format
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Standard-precision temperature format specfies value with precision 1°C. The value is two-digit integer-rounded temperature value. Negative values are preceded by ``M`` character.

For example, value ``12`` means 12, and value ``M12`` means -12°C.

Zero value also may be preceded by ``M`` character. Value ``00`` means temperature above or equal to 0°C and below 0.5°C, while value ``M00`` means temperature below 0°C and above -0.5°C. The presence of ``M`` character in the value can be queried with :cpp:func:`Temperature::isFreezing()` for both zero and non-zero values.

:cpp:func:`Temperature::isPrecise()` will return ``false`` for this format.


High-precision temperature format 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

High-precision temperature format specfies value with precision 0.1°C. 

The value is four-digit. First digit specifies value sign, ``0`` (positive value) or ``1`` (negative value). If the first digit in any such four-digit sequence is neither ``0`` nor ``1``, the group is not recognised by Metaf as a precise temperature remark.

Remaining three digits are temperature value in tenths of degree Celsius.

For example, value ``0349`` indicates temperature 34.9°C and value ``1042`` indicates temperature -4.2°C.

:cpp:func:`Temperature::isPrecise()` will return ``true`` for this format.


Regional variations
^^^^^^^^^^^^^^^^^^^

High-precision temperature format is used only in North America.
