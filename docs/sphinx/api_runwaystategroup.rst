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
