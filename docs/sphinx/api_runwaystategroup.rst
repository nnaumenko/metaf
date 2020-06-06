RunwayStateGroup
================

.. cpp:namespace-push:: metaf

RunwayStateGroup class
----------------------

	.. cpp:class:: RunwayStateGroup

		Stores information about the state of runway surface and/or accumulation of deposits for a single runway. Alternatively may store information that the deposits of runway ceased to exist or that runway is closed due to snow accumulation.

.. cpp:namespace-push:: RunwayStateGroup


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Type of information about runway state.

		.. cpp:enumerator:: RUNWAY_STATE

			Runway state group. Use :cpp:func:`deposits()` for type of deposits on runway (e.g. water patches, snow, slush, etc.),  :cpp:func:`contaminationExtent()` for the percentage of runway contamination by deposits, :cpp:func:`depositDepth()` for the depth of the deposits, and :cpp:func:`surfaceFriction()` for the surface friction or braking action.

		.. cpp:enumerator:: RUNWAY_NOT_OPERATIONAL

			Runway state group indicating that the runway is not operational. Use :cpp:func:`deposits()` for type of deposits on runway (e.g. water patches, snow, slush, etc.),  :cpp:func:`contaminationExtent()` for the percentage of runway contamination by deposits, and :cpp:func:`surfaceFriction()` for the surface friction or braking action.

		.. cpp:enumerator:: RUNWAY_CLRD

			Runway state group indicating that previously present deposits on runway were cleared or ceased to exist. Use :cpp:func:`surfaceFriction()` for the surface friction or braking action.

		.. cpp:enumerator:: RUNWAY_SNOCLO

			Runway state group indicating that the runway is closed due to snow accumulation (e.g. ``R27/SNOCLO``). No further details are specified. 

		.. cpp:enumerator:: AERODROME_SNOCLO

			Aerodrome is closed due to snow accumulation, coded as  ``SNOCLO`` or ``R/SNOCLO`` in the METAR report. No further details are specified. :cpp:func:`runway()` always returns 'all runways' value.

	.. cpp:enum-class:: Deposits

		Type of deposits on the runway.

		.. cpp:enumerator:: CLEAR_AND_DRY

			No deposits; runway is clear and dry.

		.. cpp:enumerator:: DAMP

			Runway is damp.

		.. cpp:enumerator:: WET_AND_WATER_PATCHES

			Runway is wet and water patches are present.

		.. cpp:enumerator:: RIME_AND_FROST_COVERED

			Runway is covered in frost / rime.

		.. cpp:enumerator:: DRY_SNOW

			Dry snow on runway.

		.. cpp:enumerator:: WET_SNOW

			Wet snow on runway.

		.. cpp:enumerator:: SLUSH

			Slush on runway.

		.. cpp:enumerator:: ICE

			Ice on runway.

		.. cpp:enumerator:: COMPACTED_OR_ROLLED_SNOW

			Compacted or rolled snow on runway.

		.. cpp:enumerator:: FROZEN_RUTS_OR_RIDGES

			Runway covered in frozen mass of snow or ice with ruts and riges.

		.. cpp:enumerator:: NOT_REPORTED

			Deposits on runway are not reported.

	.. cpp:enum-class::  Extent

		The extent of runway contamination with the deposits (:cpp:enum:`Deposits`). Represents how much of total runway surface is contaminated. 

		.. cpp:enumerator:: NONE

			No deposits on the runway.

		.. cpp:enumerator:: LESS_THAN_10_PERCENT

			Less than 10% of runway contaminated.

		.. cpp:enumerator:: FROM_11_TO_25_PERCENT

			Less than 11% to 25% of runway contaminated.

		.. cpp:enumerator:: RESERVED_3

			Reserved value; should not be used.

		.. cpp:enumerator:: RESERVED_4

			Reserved value; should not be used.

		.. cpp:enumerator:: FROM_26_TO_50_PERCENT

			From 26% to 50% of runway contaminated.

		.. cpp:enumerator:: RESERVED_6

			Reserved value; should not be used.

		.. cpp:enumerator:: RESERVED_7

			Reserved value; should not be used.

		.. cpp:enumerator:: RESERVED_8

			Reserved value; should not be used.

		.. cpp:enumerator:: MORE_THAN_51_PERCENT

			More than 51% of runway surface 

		.. cpp:enumerator:: NOT_REPORTED

			Contamination extent not reported.

Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Runway runway() const

		:returns: Runway for which the state is provided.

	.. cpp:function:: Type type() const

		:returns: Type of runway state group. 

	.. cpp:function:: Deposits deposits() const

		:returns: Deposits on the runway.

	.. cpp:function:: Extent contaminationExtent() const

		:returns: Extent (percentage) of runway contamination with deposits.

	.. cpp:function:: Precipitation depositDepth() const

		:returns: Depth of the deposits on the runway or non-reported value.

	.. cpp:function:: SurfaceFriction surfaceFriction() const

		:returns: Surface friction or braking action or not reported value.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if runway state information is valid, and ``false`` otherwise. The information is considered valid if the specified runway is valid and :cpp:enum:`Extent` returned by :cpp:func:`contaminationExtent()` is not a reserved value.

.. cpp:namespace-pop::

Formats
-------

The generalised syntax of runway state groups are recognised by Metaf is shown below.

.. image:: runwaystategroup.svg

In addition to the diagram above, Metaf recognises two groups which indicate that the entire aerodrome is closed due to snow accumulation: ``R/SNOCLO`` and ``SNOCLO``.

Runway state groups
^^^^^^^^^^^^^^^^^^^

Runway state group has format ``Rxx/xxxxxx`` or ``Rxxx/xxxxxx``. There are two parts separated by ``/`` character. First part identifies a runway, and the second part indicates runway state. 

Runway identification
^^^^^^^^^^^^^^^^^^^^^

The first character is always ``R``, followed by two-digit runway number (heading), optionally followed by runway designator ``R`` (right), ``C`` (center), ``L`` (left) used to distinguish parallel runways.

Runway state information
^^^^^^^^^^^^^^^^^^^^^^^^

Runway state information is a 6-digit number. Each digit reports a specific value. Digits may be replaced with ``/`` character if the value is not reported, not applicable or not measurable.

The information starts with one digit for type of deposits on the runway, followed by one digit for runway contamination extent, followed by two digits for deposit depth, followed by two digits for surface friction or braking action.

The type of such groups is :cpp:enumerator:`RunwayStateGroup::Type::RUNWAY_STATE`.

Deposits on the runway
""""""""""""""""""""""

Deposits (or lack of deposits) on the runway are indicated by a single digit or ``/`` character. The table below shows the possible values.

===== ======================== ======================================================================
Digit Runway state / deposits  :cpp:enum:`RunwayStateGroup::Deposits`
===== ======================== ======================================================================
0     Clear and dry            :cpp:enumerator:`RunwayStateGroup::Deposits::CLEAR_AND_DRY`
1     Damp                     :cpp:enumerator:`RunwayStateGroup::Deposits::DAMP`
2     Wet with water patcher   :cpp:enumerator:`RunwayStateGroup::Deposits::WET_AND_WATER_PATCHES`
3     Rime and frost covered   :cpp:enumerator:`RunwayStateGroup::Deposits::RIME_AND_FROST_COVERED`
4     Dry snow                 :cpp:enumerator:`RunwayStateGroup::Deposits::DRY_SNOW`
5     Wet snow                 :cpp:enumerator:`RunwayStateGroup::Deposits::WET_SNOW`
6     Slush                    :cpp:enumerator:`RunwayStateGroup::Deposits::SLUSH`
7     Ice                      :cpp:enumerator:`RunwayStateGroup::Deposits::ICE`
8     Compacted or rolled snow :cpp:enumerator:`RunwayStateGroup::Deposits::COMPACTED_OR_ROLLED_SNOW`
9     Frozen ruts or ridges    :cpp:enumerator:`RunwayStateGroup::Deposits::FROZEN_RUTS_OR_RIDGES`
/     Not reported             :cpp:enumerator:`RunwayStateGroup::Deposits::NOT_REPORTED`
===== ======================== ======================================================================

Runway contamination extent
"""""""""""""""""""""""""""

Extent (percentage) of runway contamination with deposits is indicated by a single digit or ``/`` character. The table below shows the possible values.

===== =========================== ======================================================================
Digit Runway contamination extent :cpp:enum:`RunwayStateGroup::Extent`
===== =========================== ======================================================================
0     None                        :cpp:enumerator:`RunwayStateGroup::Extent::CLEAR_AND_DRY`
0     Less than 10 percent        :cpp:enumerator:`RunwayStateGroup::Extent::LESS_THAN_10_PERCENT`
2     From 11 to 25 percent       :cpp:enumerator:`RunwayStateGroup::Extent::FROM_11_TO_25_PERCENT`
3     Reserved value              :cpp:enumerator:`RunwayStateGroup::Extent::RESERVED_3`
4     Reserved value              :cpp:enumerator:`RunwayStateGroup::Extent::RESERVED_4`
5     From 26 to 50 percent       :cpp:enumerator:`RunwayStateGroup::Extent::FROM_26_TO_50_PERCENT`
6     Reserved value              :cpp:enumerator:`RunwayStateGroup::Extent::RESERVED_6`
7     Reserved value              :cpp:enumerator:`RunwayStateGroup::Extent::RESERVED_7`
8     Reserved value              :cpp:enumerator:`RunwayStateGroup::Extent::RESERVED_8`
9     51 percent or more          :cpp:enumerator:`RunwayStateGroup::Extent::MORE_THAN_51_PERCENT`
/     Not reported                :cpp:enumerator:`RunwayStateGroup::Extent::NOT_REPORTED`
===== =========================== ======================================================================

Depth of deposits
"""""""""""""""""

Depth of the deposits on the runway are indicated by two digits or ``//`` characters.

The format is summarised in the table below. The deposit depth is interpreted by Metaf as a :cpp:class:`Precipitation` value.

====== ==========================================================
Value  Depth of deposits
====== ==========================================================
00     Less than 1 mm
01..90 Deposit depth, 1 mm to 90 mm
91     Reserved value, causes group to be not recognised by Metaf
92     100 mm
93     150 mm
94     200 mm
95     250 mm
96     300 mm
97     350 mm
98     400 mm
99     Indicates that runway is not operational
//     Not reported: depth not applicable or not significant
====== ==========================================================

.. note:: If the value 99 (runway is not operational) is specified, Metaf will use group type :cpp:enumerator:`RunwayStateGroup::Type::RUNWAY_NOT_OPERATIONAL` instead of :cpp:enumerator:`RunwayStateGroup::Type::RUNWAY_STATE`.

Surface friction or braking action
""""""""""""""""""""""""""""""""""

Surface friction coefficient or braking action is indicated by two digits or ``//`` characters.

The format is described in :ref:`api_surfacefriction:Formats`.

Examples of runway state groups
"""""""""""""""""""""""""""""""

+-------------+------------------------------------------------------------------+-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+----------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------+
| Group       | :cpp:func:`RunwayStateGroup::type()`                             | :cpp:func:`RunwayStateGroup::runway()`                                            | :cpp:func:`RunwayStateGroup::deposits()`                               | :cpp:func:`RunwayStateGroup::contaminationExtent()`              | :cpp:func:`RunwayStateGroup::depositDepth()`                                     | :cpp:func:`RunwayStateGroup::surfaceFriction()`                                                             |
+-------------+------------------------------------------------------------------+-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+----------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------+
| R28/190068  | :cpp:enumerator:`RunwayStateGroup::Type::RUNWAY_STATE`           | - :cpp:class:`Runway`                                                             | :cpp:enumerator:`RunwayStateGroup::Deposits::DAMP`                     | :cpp:enumerator:`RunwayStateGroup::Extent::MORE_THAN_51_PERCENT` | - :cpp:class:`Precipitation`                                                     | - :cpp:class:`SurfaceFriction`                                                                              |
|             |                                                                  | - :cpp:func:`Runway::number()` == 28                                              |                                                                        |                                                                  | - :cpp:func:`Precipitation::amount()` == 0.0                                     | - :cpp:func:`SurfaceFriction::type()` == :cpp:enumerator:`SurfaceFriction::Type::SURFACE_FRICTION_REPORTED` |
|             |                                                                  | - :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::NONE`  |                                                                        |                                                                  | - :cpp:func:`Precipitation::unit()` == :cpp:enumerator:`Precipitation::Unit::MM` | - :cpp:func:`SurfaceFriction::coefficient()` == 0.68                                                        |
|             |                                                                  |                                                                                   |                                                                        |                                                                  |                                                                                  | - :cpp:func:`SurfaceFriction::brakingAction()` == :cpp:enumerator:`SurfaceFriction::BrakingAction::GOOD`    |
+-------------+                                                                  +-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+----------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------+
| R14R/010095 |                                                                  | - :cpp:class:`Runway`                                                             | :cpp:enumerator:`RunwayStateGroup::Deposits::CLEAR_AND_DRY`            | :cpp:enumerator:`RunwayStateGroup::Extent::LESS_THAN_10_PERCENT` | - :cpp:class:`Precipitation`                                                     | - :cpp:class:`SurfaceFriction`                                                                              |
|             |                                                                  | - :cpp:func:`Runway::number()` == 14                                              |                                                                        |                                                                  | - :cpp:func:`Precipitation::amount()` == 0.0                                     | - :cpp:func:`SurfaceFriction::type()` == :cpp:enumerator:`SurfaceFriction::Type::BRAKING_ACTION_REPORTED`   |
|             |                                                                  | - :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::RIGHT` |                                                                        |                                                                  | - :cpp:func:`Precipitation::unit()` == :cpp:enumerator:`Precipitation::Unit::MM` | - :cpp:func:`SurfaceFriction::coefficient()` == 0.4                                                         |
|             |                                                                  |                                                                                   |                                                                        |                                                                  |                                                                                  | - :cpp:func:`SurfaceFriction::brakingAction()` == :cpp:enumerator:`SurfaceFriction::BrakingAction::GOOD`    |
+-------------+                                                                  +-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+----------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------+
| R05L/91//60 |                                                                  | - :cpp:class:`Runway`                                                             | :cpp:enumerator:`RunwayStateGroup::Deposits::FROZEN_RUTS_OR_RIDGES`    | :cpp:enumerator:`RunwayStateGroup::Extent::LESS_THAN_10_PERCENT` | - :cpp:class:`Precipitation`                                                     | - :cpp:class:`SurfaceFriction`                                                                              |
|             |                                                                  | - :cpp:func:`Runway::number()` == 5                                               |                                                                        |                                                                  | - :cpp:func:`Precipitation::isReported()` == false                               | - :cpp:func:`SurfaceFriction::type()` == :cpp:enumerator:`SurfaceFriction::Type::SURFACE_FRICTION_REPORTED` |
|             |                                                                  | - :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::LEFT`  |                                                                        |                                                                  |                                                                                  | - :cpp:func:`SurfaceFriction::coefficient()` == 0.6                                                         |
|             |                                                                  |                                                                                   |                                                                        |                                                                  |                                                                                  | - :cpp:func:`SurfaceFriction::brakingAction()` == :cpp:enumerator:`SurfaceFriction::BrakingAction::GOOD`    |
+-------------+                                                                  +-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+----------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------+
| R33///////  |                                                                  | - :cpp:class:`Runway`                                                             | :cpp:enumerator:`RunwayStateGroup::Deposits::NOT_REPORTED`             | :cpp:enumerator:`RunwayStateGroup::Extent::NOT_REPORTED`         | - :cpp:class:`Precipitation`                                                     | - :cpp:class:`SurfaceFriction`                                                                              |
|             |                                                                  | - :cpp:func:`Runway::number()` == 33                                              |                                                                        |                                                                  | - :cpp:func:`Precipitation::isReported()` == false                               | - :cpp:func:`SurfaceFriction::type()` == :cpp:enumerator:`SurfaceFriction::Type::NOT_REPORTED`              |
|             |                                                                  | - :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::NONE`  |                                                                        |                                                                  |                                                                                  |                                                                                                             |
+-------------+------------------------------------------------------------------+-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+----------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------+
| R16/8999//  | :cpp:enumerator:`RunwayStateGroup::Type::RUNWAY_NOT_OPERATIONAL` | - :cpp:class:`Runway`                                                             | :cpp:enumerator:`RunwayStateGroup::Deposits::COMPACTED_OR_ROLLED_SNOW` | :cpp:enumerator:`RunwayStateGroup::Extent::MORE_THAN_51_PERCENT` | - :cpp:class:`Precipitation`                                                     | - :cpp:class:`SurfaceFriction`                                                                              |
|             |                                                                  | - :cpp:func:`Runway::number()` == 16                                              |                                                                        |                                                                  | - :cpp:func:`Precipitation::isReported()` == false                               | - :cpp:func:`SurfaceFriction::type()` == :cpp:enumerator:`SurfaceFriction::Type::NOT_REPORTED`              |
|             |                                                                  | - :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::NONE`  |                                                                        |                                                                  |                                                                                  |                                                                                                             |
+-------------+------------------------------------------------------------------+-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+----------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------+


CLRD groups
^^^^^^^^^^^

Runway state groups which indicate that the deposits on the runway were cleared or ceased to exist have format ``Rxx/CLRDxx`` or ``Rxxx/CLRDxx``. There are two parts separated by ``/`` character. First part identifies a runway, and the second part indicates runway state. 

The type of such groups is :cpp:enumerator:`RunwayStateGroup::Type::CLRD`.

Runway identification is the same as described above.

Two digits following ``CLRD`` are surface friction as described in :ref:`api_surfacefriction:Formats`.

Metaf also recognises format ``Rxx/Dxx`` and ``Rxxx/Dxx`` reported by s`AviationWeather <https://aviationweather.gov/>`_. The meaning is the same. If the group has format ``Rxx/D`` and ``Rxxx/D`` and surface friction value is missing, the surface frication is interpreted as a non-reported value.

Examples of CLRD groups
"""""""""""""""""""""""

+-------------+------------------------------------------------+-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+--------------------------------------------------+-------------------------------------------------------------------------------------------------------------+
| Group       | :cpp:func:`RunwayStateGroup::type()`           | :cpp:func:`RunwayStateGroup::runway()`                                            | :cpp:func:`RunwayStateGroup::deposits()`                               | :cpp:func:`RunwayStateGroup::contaminationExtent()`              | :cpp:func:`RunwayStateGroup::depositDepth()`     | :cpp:func:`RunwayStateGroup::surfaceFriction()`                                                             |
+-------------+------------------------------------------------+-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+--------------------------------------------------+-------------------------------------------------------------------------------------------------------------+
| R13/CLRD//  | :cpp:enumerator:`RunwayStateGroup::Type::CLRD` | - :cpp:class:`Runway`                                                             | :cpp:enumerator:`RunwayStateGroup::Deposits::NOT_REPORTED`             | :cpp:enumerator:`RunwayStateGroup::Extent::MORE_THAN_51_PERCENT` | - :cpp:class:`Precipitation`                     | - :cpp:class:`SurfaceFriction`                                                                              |
|             |                                                | - :cpp:func:`Runway::number()` == 13                                              |                                                                        |                                                                  | - :cpp:func:`Precipitation::isReported` == false | - :cpp:func:`SurfaceFriction::type()` == :cpp:enumerator:`SurfaceFriction::Type::NOT_REPORTED`              |
|             |                                                | - :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::NONE`  |                                                                        |                                                                  |                                                  |                                                                                                             |
|             |                                                |                                                                                   |                                                                        |                                                                  |                                                  |                                                                                                             |
+-------------+                                                +-----------------------------------------------------------------------------------+                                                                        |                                                                  |                                                  +-------------------------------------------------------------------------------------------------------------+
| R26/CLRD70  |                                                | - :cpp:class:`Runway`                                                             |                                                                        |                                                                  |                                                  | - :cpp:class:`SurfaceFriction`                                                                              |
|             |                                                | - :cpp:func:`Runway::number()` == 26                                              |                                                                        |                                                                  |                                                  | - :cpp:func:`SurfaceFriction::type()` == :cpp:enumerator:`SurfaceFriction::Type::SURFACE_FRICTION_REPORTED` |
|             |                                                | - :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::NONE`  |                                                                        |                                                                  |                                                  | - :cpp:func:`SurfaceFriction::coefficient()` == 0.7                                                         |
|             |                                                |                                                                                   |                                                                        |                                                                  |                                                  | - :cpp:func:`SurfaceFriction::brakingAction()` == :cpp:enumerator:`SurfaceFriction::BrakingAction::GOOD`    |
+-------------+                                                +-----------------------------------------------------------------------------------+                                                                        |                                                                  |                                                  +-------------------------------------------------------------------------------------------------------------+
| R26/70D     |                                                | - :cpp:class:`Runway`                                                             |                                                                        |                                                                  |                                                  | - :cpp:class:`SurfaceFriction`                                                                              |
|             |                                                | - :cpp:func:`Runway::number()` == 26                                              |                                                                        |                                                                  |                                                  | - :cpp:func:`SurfaceFriction::type()` == :cpp:enumerator:`SurfaceFriction::Type::SURFACE_FRICTION_REPORTED` |
|             |                                                | - :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::NONE`  |                                                                        |                                                                  |                                                  | - :cpp:func:`SurfaceFriction::coefficient()` == 0.7                                                         |
|             |                                                |                                                                                   |                                                                        |                                                                  |                                                  | - :cpp:func:`SurfaceFriction::brakingAction()` == :cpp:enumerator:`SurfaceFriction::BrakingAction::GOOD`    |
+-------------+                                                +-----------------------------------------------------------------------------------+                                                                        |                                                                  |                                                  +-------------------------------------------------------------------------------------------------------------+
| R13/D       |                                                | - :cpp:class:`Runway`                                                             |                                                                        |                                                                  |                                                  | - :cpp:class:`SurfaceFriction`                                                                              |
|             |                                                | - :cpp:func:`Runway::number()` == 13                                              |                                                                        |                                                                  |                                                  | - :cpp:func:`SurfaceFriction::type()` == :cpp:enumerator:`SurfaceFriction::Type::NOT_REPORTED`              |
+-------------+------------------------------------------------+-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+--------------------------------------------------+-------------------------------------------------------------------------------------------------------------+


SNOCLO groups
^^^^^^^^^^^^^

Runway state groups which indicate that the runway is closed due to snow accumulation have format ``Rxx/SNOCLO`` or ``Rxxx/SNOCLO`` The type of such groups is :cpp:enumerator:`RunwayStateGroup::Type::SNOCLO`.

Runway identification is the same as described above.

Metaf also recognises groups coded ``R/SNOCLO`` or ``SNOCLO``, which mean that aerodrome is closed due to snow accumulation. The type of such groups is :cpp:enumerator:`RunwayStateGroup::Type::AERODROME_SNOCLO`.

Examples of SNOCLO groups
"""""""""""""""""""""""""

+-------------+------------------------------------------------------------+-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+--------------------------------------------------+------------------------------------------------------------------------------------------------+
| Group       | :cpp:func:`RunwayStateGroup::type()`                       | :cpp:func:`RunwayStateGroup::runway()`                                            | :cpp:func:`RunwayStateGroup::deposits()`                               | :cpp:func:`RunwayStateGroup::contaminationExtent()`              | :cpp:func:`RunwayStateGroup::depositDepth()`     | :cpp:func:`RunwayStateGroup::surfaceFriction()`                                                |
+-------------+------------------------------------------------------------+-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+--------------------------------------------------+------------------------------------------------------------------------------------------------+
| R21/SNOCLO  | :cpp:enumerator:`RunwayStateGroup::Type::SNOCLO`           | - :cpp:class:`Runway`                                                             | :cpp:enumerator:`RunwayStateGroup::Deposits::NOT_REPORTED`             | :cpp:enumerator:`RunwayStateGroup::Extent::MORE_THAN_51_PERCENT` | - :cpp:class:`Precipitation`                     | - :cpp:class:`SurfaceFriction`                                                                 |
|             |                                                            | - :cpp:func:`Runway::number()` == 21                                              |                                                                        |                                                                  | - :cpp:func:`Precipitation::isReported` == false | - :cpp:func:`SurfaceFriction::type()` == :cpp:enumerator:`SurfaceFriction::Type::NOT_REPORTED` |
|             |                                                            | - :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::NONE`  |                                                                        |                                                                  |                                                  |                                                                                                |
+-------------+------------------------------------------------------------+-----------------------------------------------------------------------------------+                                                                        |                                                                  |                                                  |                                                                                                |
| R/SNOCLO    | :cpp:enumerator:`RunwayStateGroup::Type::AERODROME_SNOCLO` | - :cpp:class:`Runway`                                                             | :cpp:enumerator:`RunwayStateGroup::Deposits::NOT_REPORTED`             | :cpp:enumerator:`RunwayStateGroup::Extent::MORE_THAN_51_PERCENT` | - :cpp:class:`Precipitation`                     |                                                                                                |
|             |                                                            | - :cpp:func:`Runway::isAllRunways()` == true                                      |                                                                        |                                                                  | - :cpp:func:`Precipitation::isReported` == false |                                                                                                |
+-------------+                                                            |                                                                                   |                                                                        |                                                                  |                                                  |                                                                                                |
| SNOCLO      |                                                            |                                                                                   |                                                                        |                                                                  |                                                  |                                                                                                |
+-------------+------------------------------------------------------------+-----------------------------------------------------------------------------------+------------------------------------------------------------------------+------------------------------------------------------------------+--------------------------------------------------+------------------------------------------------------------------------------------------------+
