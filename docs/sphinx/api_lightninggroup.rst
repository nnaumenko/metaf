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


Formats
-------

The following group formats of type, frequency and direction of lightning strikes included in the METAR report remarks are recognised by Metaf.

Lightning group may be specified in remarks as a sequence of groups. The first group is optional and defines frequency of lightning flashes. The second group starts with ``LTG`` followed by one or more lightning strike types. The third group is optional and specifies the distance to the detected lightning strikes. The further groups are also optional and specify the directions in which lightning strikes occur.

The table below shows frequency of lightning strikes recognised by Metaf.

===== ============================== =======================================================
Group Meaning                        :cpp:enum:`LightningGroup::Frequency`
===== ============================== =======================================================
n/a   Flash frequency not specified  :cpp:enumerator:`LightningGroup::Frequency::NONE`
OCNL  Less than 1 flash per minute   :cpp:enumerator:`LightningGroup::Frequency::OCCASIONAL`
FRQ   1 to 6 flashes per minute      :cpp:enumerator:`LightningGroup::Frequency::FREQUENT`
CONS  More than 6 flashes per minute :cpp:enumerator:`LightningGroup::Frequency::CONSTANT`
===== ============================== =======================================================

The table below shows types of lightning strikes recognised by Metaf.

===== =========================================== ============================================
Group Meaning                                     Method of :cpp:class:`LightningGroup`
===== =========================================== ============================================
IC    In-cloud                                    :cpp:func:`LightningGroup::isInCloud()`
CC    Cloud-to-cloud                              :cpp:func:`LightningGroup::isCloudCloud()`
CG    Cloud-to-ground                             :cpp:func:`LightningGroup::isCloudGround()`
CA    Cloud-to-air (without strike to the ground) :cpp:func:`LightningGroup::isCloudAir()`
other Any characters other than specified above   :cpp:func:`LightningGroup::isUnknownType()`
===== =========================================== ============================================

Generalised syntax of the lightning group recognised by Metaf is below. 

.. image:: lightninggroup.svg

Examples
^^^^^^^^

The table below summarises various formats of surface wind and wind shear recognised by Metaf. The table shows values returned by getter methods of :cpp:class:`LightningGroup`.

+--------------------------+------------------------------------------------------+--------------------------------------------------------+--------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
|Group(s)                  |:cpp:func:`LightningGroup::frequency()`               | Lightning strike types                                 |:cpp:func:`LightningGroup::directions()`                                              |:cpp:func:`distance()`                                                                |
+==========================+======================================================+========================================================+======================================================================================+======================================================================================+
|LTG DSNT                  |:cpp:enumerator:`LightningGroup::Frequency::NONE`     | - :cpp:func:`LightningGroup::isInCloud()` == false     | - std::vector<:cpp:class:`Direction`>                                                | - :cpp:class:`Distance`                                                              |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudCloud()` == false  | - empty vector                                                                       | - :cpp:func:`Distance::modifier()` == :cpp:enumerator:`Distance::Modifier::DISTANT`  |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudGround()` == false |                                                                                      | - :cpp:func:`Distance::isValue()` == false                                           |
+--------------------------+                                                      | - :cpp:func:`LightningGroup::isCloudAir()` == false    |                                                                                      +--------------------------------------------------------------------------------------+
|LTG VC                    |                                                      | - :cpp:func:`LightningGroup::isUnknownType()` == false |                                                                                      | - :cpp:class:`Distance`                                                              |
|                          |                                                      |                                                        |                                                                                      | - :cpp:func:`Distance::modifier()` == :cpp:enumerator:`Distance::Modifier::VICINITY` |
|                          |                                                      |                                                        |                                                                                      | - :cpp:func:`Distance::isValue()` == false                                           |
+--------------------------+------------------------------------------------------+--------------------------------------------------------+--------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
|FRQ LTGCCCA SW-NW         |:cpp:enumerator:`LightningGroup::Frequency::FREQUENT` | - :cpp:func:`LightningGroup::isInCloud()` == false     | - std::vector<:cpp:class:`Direction`>                                                | - :cpp:class:`Distance`                                                              |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudCloud()` == true   | - size() == 3                                                                        | - :cpp:func:`Distance::isReported()` == false                                        |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudGround()` == false | - element 0                                                                          |                                                                                      |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudAir()` == true     | - :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_CARDINAL` |                                                                                      |
|                          |                                                      | - :cpp:func:`LightningGroup::isUnknownType()` == false | - :cpp:func:`Direction::cardinal()` == :cpp:enumerator:`Direction::Cardinal::SW`     |                                                                                      |
|                          |                                                      |                                                        | - element 1                                                                          |                                                                                      |
|                          |                                                      |                                                        | - :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_CARDINAL` |                                                                                      |
|                          |                                                      |                                                        | - :cpp:func:`Direction::cardinal()` == :cpp:enumerator:`Direction::Cardinal::W`      |                                                                                      |
|                          |                                                      |                                                        | - element 2                                                                          |                                                                                      |
|                          |                                                      |                                                        | - :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_CARDINAL` |                                                                                      |
|                          |                                                      |                                                        | - :cpp:func:`Direction::cardinal()` == :cpp:enumerator:`Direction::Cardinal::NW`     |                                                                                      |
+--------------------------+------------------------------------------------------+--------------------------------------------------------+--------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
|CONS LTGICCG OHD AND NE-E |:cpp:enumerator:`LightningGroup::Frequency::CONSTANT` | - :cpp:func:`LightningGroup::isInCloud()` == true      | - std::vector<:cpp:class:`Direction`>                                                | - :cpp:class:`Distance`                                                              |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudCloud()` == false  | - size() == 3                                                                        | - :cpp:func:`Distance::isReported()` == false                                        |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudGround()` == true  | - element 0                                                                          |                                                                                      |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudAir()` == false    | - :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::OVERHEAD`       |                                                                                      |
|                          |                                                      | - :cpp:func:`LightningGroup::isUnknownType()` == false | - element 1                                                                          |                                                                                      |
|                          |                                                      |                                                        | - :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_CARDINAL` |                                                                                      |
|                          |                                                      |                                                        | - :cpp:func:`Direction::cardinal()` == :cpp:enumerator:`Direction::Cardinal::NE`     |                                                                                      |
|                          |                                                      |                                                        | - element 2                                                                          |                                                                                      |
|                          |                                                      |                                                        | - :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_CARDINAL` |                                                                                      |
|                          |                                                      |                                                        | - :cpp:func:`Direction::cardinal()` == :cpp:enumerator:`Direction::Cardinal::E`      |                                                                                      |
+--------------------------+------------------------------------------------------+--------------------------------------------------------+--------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
|LTG DSNT ALQDS            |:cpp:enumerator:`LightningGroup::Frequency::NONE`     | - :cpp:func:`LightningGroup::isInCloud()` == false     | - std::vector<:cpp:class:`Direction`>                                                | - :cpp:class:`Distance`                                                              |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudCloud()` == false  | - size() == 1                                                                        | - :cpp:func:`Distance::modifier()` == :cpp:enumerator:`Distance::Modifier::DISTANT`  |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudGround()` == false | - element 0                                                                          | - :cpp:func:`Distance::isValue()` == false                                           |
|                          |                                                      | - :cpp:func:`LightningGroup::isCloudAir()` == false    | - :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::ALQDS`          |                                                                                      |
|                          |                                                      | - :cpp:func:`LightningGroup::isUnknownType()` == false |                                                                                      |                                                                                      |
+--------------------------+------------------------------------------------------+--------------------------------------------------------+--------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------+

Regional variations
^^^^^^^^^^^^^^^^^^^

This group is used only in North America.