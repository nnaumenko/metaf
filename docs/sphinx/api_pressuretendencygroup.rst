PressureTendencyGroup
=====================

.. cpp:namespace-push:: metaf

PressureTendencyGroup class
---------------------------

	.. cpp:class:: PressureTendencyGroup

		Stores information about atmospheric pressure tendency for the last 3 hours. This group is used in North America and is included in remarks.

.. cpp:namespace-push:: PressureTendencyGroup


Type definitions
^^^^^^^^^^^^^^^^

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


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Nature of atmospheric pressure changes for last 3 hours.

	.. cpp:function:: Trend trend() const

		:returns: Trend of atmospheric pressure changes for last 3 hours.

	.. cpp:function:: Pressure difference() const

		:returns: Absolute value of atmospheric pressure change for the last 3 hours or emply ``std::optional`` if pressure is not reported.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: Always returns ``true``.

.. cpp:namespace-pop::
