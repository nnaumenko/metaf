VicinityGroup
=============

.. cpp:namespace-push:: metaf

VicinityGroup class
-------------------

.. cpp:class:: VicinityGroup

	Stores information about significant phenomena observed in the vicinity of the station. This group is included in the remarks.

.. cpp:namespace-push:: VicinityGroup

Type definitions
^^^^^^^^^^^^^^^^

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

		.. cpp:enumerator:: FOG

			Fog.

		.. cpp:enumerator:: HAZE

			Haze.

		.. cpp:enumerator:: SMOKE

			Smoke.

		.. cpp:enumerator:: BLOWING_SNOW

			Blowing snow.

		.. cpp:enumerator:: BLOWING_SAND

			Blowing sand.

		.. cpp:enumerator:: BLOWING_DUST

			Blowing dust.

		.. cpp:enumerator:: LOWER_CEILING

			Lower ceiling towards direction or direction sector.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Type of observed phenomena.

	.. cpp:function:: Distance distance() const

		:returns: Distance at to the observed phenomena (if reported in the group) or non-reported value with modifier :cpp:enumerator:`Distance::Modifier::DISTANT` if distant (10 to 30 nautical miles) phenomena is reported in this group, or non-reported value with modifier :cpp:enumerator:`Distance::Modifier::NONE` if no distance was specified.

	.. cpp:function:: std::vector<Direction> directions() const

		:returns: Vector of directions where the phenomena was observed (may include Overhead direction).

	.. cpp:function:: Direction movingDirection() const

		:returns: Direction in which phenomena is moving or non-reported direction if the phenomena is not moving or direction is not specified.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if group syntax is complete (e.g. no stray ``AND`` or ``MOV`` at the end of the group), and ``false`` otherwise.

.. cpp:namespace-pop::
