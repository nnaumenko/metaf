LightningGroup
==============

.. cpp:namespace-push:: metaf

LightningGroup class
--------------------

	.. cpp:class:: LightningGroup

		Stores information about observed lightning flashes. This group is included in the remarks and is used in North America.

.. cpp:namespace-push:: LightningGroup

Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Frequency

		Frequency of lightning flashes.

		.. cpp:enumerator:: NONE

			Frequency was not speficied.

		.. cpp:enumerator:: OCCASIONAL

			Less than 1 flash per minute.

		.. cpp:enumerator:: FREQUENT

			1 to 6 flashes per minute.

		.. cpp:enumerator:: CONSTANT

			More than 6 flashes per minute.

Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Frequency frequency() const

		:returns: Observed frequency of lightning flashes.

	.. cpp:function:: Distance distance() const

		:returns: Currently this function only returns a non-reported value with modifier :cpp:enumerator:`Distance::Modifier::DISTANT` if distant (10 to 30 nautical miles) lightning is reported in this group. Otherwise the function returns a non-reported value with the modifier :cpp:enumerator:`Distance::Modifier::NONE`.

	.. cpp:function:: std::vector<Direction> directions() const

		:returns: Vector of directions where the lightning was reported (may include Overhead direction).


Lightning types
^^^^^^^^^^^^^^^

	.. note:: More than one lightning type may be reported in this group.

	.. cpp:function:: bool isCloudGround() const

		:returns: ``true`` if cloud-to-ground lightning is observed and reported in this group, ``false`` otherwise.

	.. cpp:function:: bool isInCloud() const

		:returns: ``true`` if in-cloud lightning is observed and reported in this group, ``false`` otherwise.

	.. cpp:function:: bool isCloudCloud() const

		:returns: ``true`` if cloud-to-cloud lightning is observed and reported in this group, ``false`` otherwise.

	.. cpp:function:: bool isCloudAir() const

		:returns: ``true`` if cloud to air (without strike to the ground) lightning is observed and reported in this group, ``false`` otherwise.

	.. cpp:function:: bool isUnknownType() const

		:returns: ``true`` if lightning type other than listed above is reported in this group, ``false`` otherwise.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if there are no unknown lightning types in this group.

.. cpp:namespace-pop::