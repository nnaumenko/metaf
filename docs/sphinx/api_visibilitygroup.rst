VisibilityGroup
===============

.. cpp:namespace-push:: metaf

VisibilityGroup class
---------------------

	.. cpp:class:: VisibilityGroup

		Stores information about prevailing visibility, visibility towards cardinal direction, visibility for runway, visibility at surface level, visibility from air trafic control tower, runway visual range, etc.

		See also CAVOK (:cpp:enumerator:`metaf::KeywordGroup::Type::CAVOK`) which may be used to specify visibility of 10 km or more in all directions.

.. cpp:namespace-push:: VisibilityGroup


Type definitions
^^^^^^^^^^^^^^^^

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

			Indicates that the visibility data is not available for a particular runway or in a particular cardinal direction. Use :cpp:func:`runway()` or :cpp:func:`direction()`, if both methods return non-reported values, no further details were specified.

		.. cpp:enumerator:: MINIMUM

			Minimum visibility value. Use :cpp:func:`visibility()` for visibility value and :cpp:func:`direction()` for cardinal direction towards which the minimum visibility is observed. Cardinal direction is not always specified.

		.. cpp:enumerator:: MAXIMUM

			Minimum visibility value. Use :cpp:func:`visibility()` for visibility value and :cpp:func:`direction()` for cardinal direction towards which the maximum visibility is observed. Cardinal direction is not always specified.

		.. cpp:enumerator:: TOWARDS_SEA

			Visibility in direction towards sea. Use :cpp:func:`visibility()`.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Visibility group type, i.e. what kind of information is stored.

	.. cpp:function:: Distance visibility() const

		:returns: Visibility value. Values in meters are integer, values in statute miles may be fractional. May contain 'less than' or 'more than' modifiers or may be non-reported value.

	.. cpp:function:: Distance minVisibility() const

		:returns: Minimum visibility value if variable visibility is reported.

	.. cpp:function:: Distance maxVisibility() const

		:returns: Maximum visibility value if variable visibility is reported.

	.. cpp:function:: std::optional<Direction> direction() const

		:returns: Cardinal direction if directional visibility is specified or empty ``std::optional`` if no directional visibility is specified. Automated stations may also report No Directional Variation if the station is not capable of providing directional visibility.

	.. cpp:function:: std::optional<Runway> runway() const

		:returns: Runway identification if runway visibility is specified or empty ``std::optional`` otherwise.

	.. cpp:function:: std::vector<Direction> sectorDirections() const

		:returns: Cardinal direction vector if sector of directions is specified. Currently always returns an empty ``std::vector<Direction>``.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if stored visibility information is valid, and ``false`` otherwise.

			The information is considered valid if all of the following conditions are met: 
				- The stored visibility and direction values are valid (if reported);
				- The group does not represent an incomplete integer group (i.e. single digit group not followed by fraction and SM designator).

.. cpp:namespace-pop::
