PressureGroup
=============

.. cpp:namespace-push:: metaf

PressureGroup class
-------------------

	.. cpp:class:: PressureGroup

		Stores information about observed or forecast atmospheric pressure.

.. cpp:namespace-push:: PressureGroup


Type definitions
^^^^^^^^^^^^^^^^

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


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Type of the pressure value (observed, forecast, and data missing).

	.. cpp:function:: Pressure atmosphericPressure() const

		:returns: Atmospheric pressure value.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: Always returns ``true`` for all groups except ``PRES`` not followed by ``MISG``.

.. cpp:namespace-pop::
