Distance
========

.. cpp:namespace-push:: metaf

Distance class
--------------

	.. cpp:class:: Distance

		Stores a distance or height value. The value may be expressed as meters, feet (e.g. 3500 or 1500) or in statute miles (e.g. 2 1/4 or 2.25). The value in statute miles may contain integer and/or fraction parts.

.. cpp:namespace-push:: Distance


Type definitions
^^^^^^^^^^^^^^^^

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


Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::optional<float> distance() const

		:returns: Stored distance value in the units specified by :cpp:func:`Distance::unit()` or empty ``std::optional`` if the value is not reported.

		.. note:: This method returns a decimal value in statute miles (e.g. 2.5). If instead an integer & fraction value in statute miles is required, use :cpp:func:`Distance::miles()`.

	.. cpp:function:: Modifier modifier() const

		:returns: Modifier of stored distance value (less than / more than).

	.. cpp:function:: Unit unit() const

		:returns: Distance measurement unit which was used with stored value.

Miscellaneous
^^^^^^^^^^^^^

	.. cpp:function:: bool isValue()

		:returns: ``true`` if integer component or both numerator and denomerator are non-empty ``std::optional``s; ``false`` otherwise.

	.. cpp:function:: bool isReported()

		:returns: ``true`` if the conditions for :cpp:func:`isValue()` are met, or the modifier is either :cpp:enumerator:`Modifier::DISTANT` or :cpp:enumerator:`Modifier::VICINITY`; ``false`` otherwise.

	.. cpp:function:: std::optional<std::pair<unsigned int, MilesFraction>> miles() const

		:returns: Value in statute miles in the form of integer and fraction.

			If the value is not reported, an empty ``std::optional`` is returned.

			For the reported values, the ``std::pair`` is returned where ``unsigned int`` component is an integer part, and :cpp:enum:`MilesFraction` component is a fraction part. For example, for the value of ``2 3/4 statute miles``, an ``unsigned int`` component contains ``2`` and :cpp:enum:`MilesFraction` component contains :cpp:enumerator:`MilesFraction::F_3_4`.

				.. note:: This method may only return the following values (as per Table 6-1 of Field Meteorology Handbook Number 1):

					Below 3/8 statute mile: increments of 1/16 statute mile (0, 1/16, 1/8, 3/16, 1/4, 5/16, 3/8).

					From 1/2 to 1 7/8 statute miles: increments of 1/8 statute mile (1/2, 5/8, 3/4, 7/8, 1, 1 1/8, 1 1/4, 1 3/8, 1 1/2, 1 5/8, 1 3/4, 1 7/8).

					From 2 to 2 3/4 statute miles: increments of 1/4 statute mile (2, 2 1/4 2 1/2, 2 3/4).

					From 3 to 15 statute miles: increments of 1 statute mile (3, 4, 5, etc).

					Above 15 statute miles: increments of 5 statute miles (15, 20, 25, etc).


Converting to other measurement units
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::optional<float> toUnit(Unit unit) const

		:param unit: Measurement unit to convert the value to.

		:returns: Stored distance value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.

		.. note:: This method returns a decimal value if the unit is statute miles (e.g. 2.5). If instead an integer & fraction value in statute miles is required, use :cpp:func:`Distance::miles()`.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: Always returns ``true``; this method is for compatibility only.

.. cpp:namespace-pop::


Formats
-------

Metaf recognises four numeric distance formats: distance in meters, distance in feet, distance in statute miles, height in hundreds feet.

In addition special distance values 'Distant' and 'In vicinity' are recognised in remark groups.


Distance specified in meters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Distance specified in meters is four-digit value, for example ``3500`` means distance of 3500 meters in certain contexts. Typically last two digits are zeros. Value of ``9999`` means 'more than 10000 meters'.

Value ``////`` may be used instead of distance in meters to indicate non-reported value.


Distance specified in feet
^^^^^^^^^^^^^^^^^^^^^^^^^^

Distance specified in feet is a four-digit value. Typically last two digits are zeros and ``FT`` follows the value. For example ``4600FT`` means distance of 4600 feet; in some cases this value may also be specified as ``4600`` when the measurement unit is implicit.

Value ``////`` may be used instead of distance in feet to indicate non-reported value.


Distance specified in statute miles
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Distance specified in statute miles consists of as integer and/or fraction part of a statute mile, for example ``7``, ``15``, ``3/8``, ``1/16``. In some groups ``SM`` follows the distance value, for example ``7SM``, ``15SM``, ``3/8SM``, ``1/16SM``. The meanings are accordingly, distances of 7, 15, 3/8 and 1/16 of statute mile.

In some cases, both integer and fraction are specified. This format is one digit of integer part, followed by space and followed by fraction part, for example ``1 1/4`` or ``1 1/4SM`` means distance of 1 1/4 statute mile.

Distances in statute miles may be preceded by modifiers ``P`` (:cpp:enumerator:`Distance::Modifier::MORE_THAN`) and ``M`` (:cpp:enumerator:`Distance::Modifier::LESS_THAN`). For example. value ``P6SM`` means 'more than 6 statute miles', and value ``M1/8SM`` means 'less than 1/8 statute miles'.

In some cases value ``////SM`` may be used instead of distance in statute miles to indicate non-reported value.


Height
^^^^^^

Height is always specified as the three-digit value in hundreds feet. For example, height value ``305`` means height 30500 feet, and value ``020`` means height 2000 feet.

Value ``///`` may be used instead of height to indicate non-reported value.


Variable height
^^^^^^^^^^^^^^^

Variable height is specified as two three-digit values in hundreds feet separated by ``V`` character. For example ``039V042`` means that height is variable from 3900 feet to 4200 feet.

Non-reported values are not recognised by Metaf as a part of variable height. For example, combinations ``039V///`` or ``///V042`` are not recognised by Metaf.


Special distance values
^^^^^^^^^^^^^^^^^^^^^^^

Special distance value 'in vicinity' is coded ``VC`` in the METAR groups and is indicated by modifier value :cpp:enumerator:`Distance::Modifier::VICINITY`. It means distance of 5 to 10 nautical miles.

Special distance value 'distant' is coded ``DSNT`` in the METAR groups and is indicated by modifier value :cpp:enumerator:`Distance::Modifier::DISTANT`. It means distance of 10 to 30 nautical miles.