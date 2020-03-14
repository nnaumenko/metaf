Speed
=====

.. cpp:namespace-push:: metaf

Speed class
-----------

	.. cpp:class:: Speed

		Stores a speed value. Speed value can be optionally not reported (i.e. no value).

.. cpp:namespace-push:: Speed


Type definitions
^^^^^^^^^^^^^^^^

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


Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::optional<unsigned int> speed() const
		
		:returns: Stored speed value or empty ``std::optional`` if speed value is not reported.

	.. cpp:function:: Unit unit() const

		:returns: Speed measurement unit which was used with stored value.


Converting to other measurement units
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::optional<float> toUnit(Unit unit) const

		:param unit: Measurement unit to convert the value to.
		
		:returns: Stored speed value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


Miscellaneous
^^^^^^^^^^^^^

	.. cpp:function:: bool isReported() const

		:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored.

.. cpp:namespace-pop::


Formats
-------

The following speed value formats are recognised by Metaf.

Wind speed format
^^^^^^^^^^^^^^^^^

Wind speed is typically specified as two digits followed by measurement unit ``KT`` (knots) or ``MPS`` (meters per second).

If speed value exceeds 99, three digits are used instead of two digits.

If the speed value is not reported, ``//KT`` or ``//MPS`` is used instead of speed value.


Regional variations
^^^^^^^^^^^^^^^^^^^

Most of the world uses values specified in knots.

Values specified in meters per second are used in China, Mongolia, and some countries which were part of Soviet Union.