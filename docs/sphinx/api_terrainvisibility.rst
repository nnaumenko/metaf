TerrainVisibility
=================

.. cpp:namespace-push:: metaf

TerrainVisibility class
-----------------------

	.. cpp:class:: TerrainVisibility

		Stores information about visibility or obscuration of terrain features. Currently mountain / hill and valley visibility is reported.

        .. cpp:namespace-push:: TerrainVisibility

        Type definitions
        ^^^^^^^^^^^^^^^^

        .. cpp:enum-class:: Description

            Identifies a descriptive visibility or obscuration of terrain features.

            .. cpp:enumerator:: NOT_SPECIFIED

                Indicates that the description is not specified.

            .. cpp:enumerator::MOUNTAINS_NOT_OBSCURED

                The mountains or hills are completely visible and cloud-free, occassional insignificant cloud may be observed. Coded ``MON LIB`` in the report.

            .. cpp:enumerator::MOUNTAINS_IN_SCATTERED_CLOUDS

                The mountains or hills are partially covered in isolated, scattered clouds. Less than half of the summits is observed. Coded ``MON CLD SCT`` in the report. 

            .. cpp:enumerator::MOUNTAINS_SUMMITS_IN_CLOUDS

                The summits of the mountains or hills reach the base of the clouds. Coded ``MON CLD CIME`` in the report.

            .. cpp:enumerator::MOUNTAINS_SLOPES_IN_CLOUDS

                The mountains or hills slopes are in clouds, while summits are visibile. Coded ``MON VERS INC`` in the report.

            .. cpp:enumerator::MOUNTAINS_OBSERVED_SIDE_VISIBILE

                The observed side of mountains or hills is cloud-free (occcassional insignificant clouds may be observed), however dense clouds are observed behind the mountains or hills which obscure the other side of the mountains or hills. Coded ``MON CNS POST`` in the report.

            .. cpp:enumerator::MOUNTAINS_MOSTLY_IN_CLOUDS

                Mountains or hills are mostly in clouds, the slopes are partially or completely obscured and some of the summits are visibile. Coded ``MON GEN INC`` in the report.

            .. cpp:enumerator::MOUNTAINS_IN_CLOUDS

                Mountains or hills are completely obscured by the clouds. Coded ``MON INC`` in the report.

            .. cpp:enumerator::MOUNTAINS_INVISIBLE

                Mountains or hills are invisible due to darkness, fog, precipitation, etc. Coded ``MON INVIS` in the report.

            .. cpp:enumerator::MOUNTAINS_OBSCURED

                Mountain(s) are obscured. Coded ``MT OBSC`` in the report. No additional information such as trend or direction is provided.

            .. cpp:enumerator::VALLEYS_NOT_OBSCURED

                Valleys are not obscured by clouds, fog or mist. Coded ``VAL NIL`` in the report.

            .. cpp:enumerator::VALLEYS_IN_MIST

                Valleys are obscured by mist to some extent. Coded ``VAL FOSCHIA`` in the report.

            .. cpp:enumerator::VALLEYS_IN_LOW_MIST

                Valleys are obscured, to some extent, by layer of mist above ground, but no obscuration is observed on higher levels. Coded ``VAL FOSCHIA SKC SUP`` in the report.

            .. cpp:enumerator::VALLEYS_IN_FOG

                Valleys are obscured by layer of fog. Coded ``VAL NEBBIA`` in the report.

            .. cpp:enumerator::VALLEYS_IN_SCATTERED_FOG

                Valleys are obscured by scattered fog banks. Coded ``VAL NEBBIA SCT`` in the report.

            .. cpp:enumerator::VALLEYS_IN_SCATTERED_CLOUDS

                Valleys are partly obscured by isolated, scattered clouds. Coded ``VAL CLD SCT`` in the report.

            .. cpp:enumerator::VALLEYS_IN_SCATTERED_CLOUDS_FOG_BELOW

                Valleys are partly obscured by isolated, scattered clouds, and the layer of fog is observed below the clouds. Coded ``VAL CLD SCT NEBBIA INF`` in the report.

            .. cpp:enumerator::VALLEYS_IN_CLOUD_LAYER

                Valleys are completely obscured by layer of clouds. Coded ``VAL MAR CLD`` in the report.

            .. cpp:enumerator::VALLEYS_INVISIBLE

                Valleys are invisible due to weather phenomena which causes low visibility downwards.

        Acquiring the data
        ^^^^^^^^^^^^^^^^^^

            .. cpp:function:: Direction direction() const

                :returns: Direction towards which the terrain feature visibility is specified. If no direction was specified, reports :cpp:enumerator:`metaf::Direction::Cardinal::ALQDS`.

            .. cpp:function:: Description description() const

                :returns: Description of the terrain feature visibility (see above).

            .. cpp:function:: bool isTrendNoChanges() const

                :returns: ``true`` if no changes to terrain feature visibility were reported, ``false`` if this condition was not specified. Coded ``NC`` in the report.

	        .. cpp:function:: bool isTrendCumulusFormation() const

                :returns: ``true`` if forming or developing cumulus clouds were reported, ``false`` if this condition was not specified. Coded ``CUF`` in the report.

	        .. cpp:function:: bool isTrendIntermittentFog() const

                :returns: ``true`` if intermittent fog at the station is reported, ``false`` if this condition was not specified. Coded ``NEBBIA INTER`` in the report.

	        .. cpp:function:: bool isTrendChanges() const

                :returns: ``true`` if variable terrain feature visibility conditions were reported, ``false`` if this condition was not specified. Coded ``VAR`` in the report. Typically rapid changes are reported, see also :cpp:function:`bool isTrendRapidly()`.

	        .. cpp:function:: bool isTrendStratification() const

                :returns: ``true`` if cloud stratification was reported, ``false`` if this condition was not specified. Coded ``STF`` in the report.

	        .. cpp:function:: bool isTrendRising() const

                :returns: ``true`` if rising clouds were repored, ``false`` if this condition was not specified. Coded ``ELEV`` in the report.

	        .. cpp:function:: bool isTrendLowering() const

                :returns: ``true`` if lowering clouds were repored, ``false`` if this condition was not specified. Coded ``ABB`` in the report.

	        .. cpp:function:: bool isTrendDiminising() const

                :returns: ``true`` if diminishing or dissipating clouds were repored, ``false`` if this condition was not specified. Coded ``DIM`` in the report.

	        .. cpp:function:: bool isTrendIncreasing() const

                :returns: ``true`` if increasing amount of clouds was repored, ``false`` if this condition was not specified. Coded ``DIM`` in the report.

	        .. cpp:function:: bool isTrendSlowly() const

                :returns: ``true`` if the repored changes are described as slow, ``false`` if this condition was not specified. Coded ``SLW`` in the report.

	        .. cpp:function:: bool isTrendRapidly() const

                :returns: ``true`` if the repored changes are described as rapid, ``false`` if this condition was not specified. Coded ``RAPID`` in the report.

        Validating
        ^^^^^^^^^^

	        .. cpp:function:: bool isValid() const

                :returns: ``true`` if no contradicting groups were found, ``false`` otherwise (for example if contradicting groups ``SLW`` and ``RAPID`` are present).

Formats
-------

Metaf recognises the following terrain feature visibility formats: ``MON LIB``, ``MON CLD SCT``, ``MON CLD CIME``, ``MON VERS INC``, ``MON CNS POST``, ``MON GEN INC``, ``MON INC``, ``MON INVIS``, ``VAL NIL``, ``VAL FOSCHIA``, ``VAL FOSCHIA SKC SUP``, ``VAL NEBBIA``, ``VAL NEBBIA SCT``, ``VAL CLD SCT``, ``VAL CLD SCT NEBBIA INF``, ``VAL MAR CLD``, ``VAL INVIS``.

See :cpp:enum:`Description` for detailed description of the formats.

These descriptions may be optionally followed by direction and/or trend groups. 

Metaf recognises the following terrain feature visibility trend formats: ``NC``, ``CUF``, ``NEBBIA INTER``, ``VAR``, ``STF``, ``ELEV``, ``ABB``, ``DIM``, ``AUM``, ``SLW``, ``RAPID``.

Additionally, Metaf recognises groups ``MT OBSC`` which indicate obscured mountain(s). These groups are not followed by direction or trend.

Regional variations
^^^^^^^^^^^^^^^^^^^
    
Groups ``MON LIB``, ``MON CLD SCT``, ``MON CLD CIME``, ``MON VERS INC``, ``MON CNS POST``, ``MON GEN INC``, ``MON INC``, ``MON INVIS``, ``VAL NIL``, ``VAL FOSCHIA``, ``VAL FOSCHIA SKC SUP``, ``VAL NEBBIA``, ``VAL NEBBIA SCT``, ``VAL CLD SCT``, ``VAL CLD SCT NEBBIA INF``, ``VAL MAR CLD``, ``VAL INVIS``, ``NC``, ``CUF``, ``NEBBIA INTER``, ``VAR``, ``STF``, ``ELEV``, ``ABB``, ``DIM``, ``AUM``, ``SLW``, ``RAPID`` are used only in Italy.

Groups ``MT OBSC`` are used only in Russia.
