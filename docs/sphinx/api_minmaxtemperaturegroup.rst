MinMaxTemperatureGroup
======================

.. cpp:namespace-push:: metaf

MinMaxTemperatureGroup class
----------------------------

	.. cpp:class:: MinMaxTemperatureGroup

		Stores information about observed or forecast minimum and/or maximum temperature.

.. cpp:namespace-push:: MinMaxTemperatureGroup


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Group type which specifies what kind of data stored in this group.

		.. cpp:enumerator:: OBSERVED_24_HOURLY

			Observed 24-hourly minimum and maximum temperature. Use :cpp:func:`minimum()` and :cpp:func:`maximum()` for observed minimum and maximum temperature values.

		.. cpp:enumerator:: OBSERVED_6_HOURLY

			Observed 6-hourly minimum and maximum temperature. Use :cpp:func:`minimum()` and :cpp:func:`maximum()` for observed minimum and maximum temperature values; one or both of the values may be non-reported.

		.. cpp:enumerator:: FORECAST

			Forecast minimum and maximum temperature. Use :cpp:func:`minimum()` and :cpp:func:`maximum()` for observed minimum and maximum temperature values. Use :cpp:func:`minimumTime()` and :cpp:func:`maximumTime()` for the time when the specified temperature is forecast to occur.

			.. note:: If only one temperature point was reported without further specifying whether it is a minimum or maximum value, then both :cpp:func:`minimum()` and :cpp:func:`maximum()` will return the same value; also :cpp:func:`minimumTime()` and :cpp:func:`maximumTime()` will return the same value.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

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


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if the minimum temperature is greater or equal to maximum temperature, and ``false`` otherwise.

.. cpp:namespace-pop::
