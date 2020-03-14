MiscGroup
=========

.. cpp:namespace-push:: metaf

.. cpp:class:: MiscGroup

	Stores various data provided in METAR or TAF report.

.. cpp:namespace-push:: MiscGroup


Type definitions
^^^^^^^^^^^^^^^^

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


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

		.. cpp:function:: Type type() const

			:returns: Type of value reported in this group.

		.. cpp:function:: std::optional<float> data() const

			:returns: The value reported in this group, or empty ``std::optional`` if the value is not reported.

				.. note:: empty ``std::optional`` is always returned for colour codes (e.g. BLU or BLACKRED).


Validating
^^^^^^^^^^

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.

.. cpp:namespace-pop::
