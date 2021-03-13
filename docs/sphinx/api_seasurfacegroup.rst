SeaSurfaceGroup
===============

.. cpp:namespace-push:: metaf

SeaSurfaceGroup class
---------------------

	.. cpp:class:: SeaSurfaceGroup

		Stores information about temperature of sea surface along with descriptive state of sea surface or wave height. This group is used by oil platforms.

.. cpp:namespace-push:: SeaSurfaceGroup


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Swell

		Specifies the height (low is 0 to 2 meters, moderate is 2 to 4 meters, high is more than 4 meters) and length (short is 0 to 100 meters, medium is 100 to 200 meters and long is more than 200 meters) of swell.

		.. cpp:enumerator:: NONE

			No swell observed.

		.. cpp:enumerator:: LOW_SHORT_MEDIUM

			Low, and short or medium length.

		.. cpp:enumerator:: LOW_LONG

			Low and long.

		.. cpp:enumerator:: MODERATE_SHORT

			Moderate and short.

		.. cpp:enumerator:: MODERATE_MEDIUM

			Moderate and medium.

		.. cpp:enumerator:: MODERATE_LONG

			Moderate and long.

		.. cpp:enumerator:: HIGH_SHORT

			High and short.

		.. cpp:enumerator:: HIGH_MEDIUM

			High and medium.

		.. cpp:enumerator:: HIGH_LONG

			High and long.

		.. cpp:enumerator:: MIXED

			Mixed heights and lengths.

		.. cpp:enumerator:: NOT_REPORTED

			Swell was specified as not reported.

		.. cpp:enumerator:: NOT_SPECIFIED

			Swell was not specified in this group.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Temperature surfaceTemperature() const

		:returns: Temperature of the sea surface or non-reported value.

	.. cpp:function:: WaveHeight waves() const

		:returns: Wave height or descriptive state of the sea surface or non-reported value.

	.. cpp:function:: Swell swell() const

		:returns: Swell or non-specified value.

	.. cpp:function:: Direction swellDirection() const

		:returns: Cardinal direction of swell or non-reported value.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: Always returns ``true``.

.. cpp:namespace-pop::
