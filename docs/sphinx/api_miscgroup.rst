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

			This group designates a corrected weather observation; value reports the sequential number of correction, for example 1st, 2nd, 3rd, etc; this group is only used in Canada.

		.. cpp:enumerator:: DENSITY_ALTITUDE

			Density altitude (in feet) reported in remarks. An empty ``std::optional`` indicates missing density altitude data (coded ``DENSITY ALT MISG`` in remarks).

		.. cpp:enumerator:: HAILSTONE_SIZE

			Largest hailstone size in inches with increments of 1/4 inch.

		.. cpp:enumerator:: COLOUR_CODE_BLUE_PLUS

			Visibility >8000 m AND no cloud obscuring 3/8 or more below 20000 feet. 

		.. cpp:enumerator:: COLOUR_CODE_BLUE

			Visibility >8000 m AND no cloud obscuring 3/8 or more below 2500 feet. 

		.. cpp:enumerator:: COLOUR_CODE_WHITE

			Visibility >5000 m AND no cloud obscuring 3/8 or more below 1500 feet.

		.. cpp:enumerator:: COLOUR_CODE_GREEN

			Visibility >3700 m AND no cloud obscuring 3/8 or more below 700 feet.

		.. cpp:enumerator:: COLOUR_CODE_YELLOW

			Visibility >1600 m AND no cloud obscuring 3/8 or more below 300 feet.

			.. note:: Instead of :cpp:enumerator:`COLOUR_CODE_YELLOW` some stations may use more precise :cpp:enumerator:`COLOUR_CODE_YELLOW1` and :cpp:enumerator:`COLOUR_CODE_YELLOW2`.

		.. cpp:enumerator:: COLOUR_CODE_YELLOW1

			Visibility >2500 m AND no cloud obscuring 3/8 or more below 500 feet.

		.. cpp:enumerator:: COLOUR_CODE_YELLOW2

			Visibility >1600 m AND no cloud obscuring 3/8 or more below 300 feet.

		.. cpp:enumerator:: COLOUR_CODE_AMBER

			Visibility >800 m AND no cloud obscuring 3/8 or more below 200 feet.

		.. cpp:enumerator:: COLOUR_CODE_RED

			Visibility <800 m OR clouds obscuring 3/8 or more below 200 feet.

		.. cpp:enumerator:: COLOUR_CODE_BLACKBLUE_PLUS

			Same as :cpp:enumerator:`COLOUR_CODE_BLUE_PLUS` but also indicates that aerodrome is closed, for example, due to snow accumulation.

		.. cpp:enumerator:: COLOUR_CODE_BLACKBLUE

			Same as :cpp:enumerator:`COLOUR_CODE_BLUE` but also indicates that aerodrome is closed, for example, due to snow accumulation.

		.. cpp:enumerator:: COLOUR_CODE_BLACKWHITE

			Same as :cpp:enumerator:`COLOUR_CODE_WHITE` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKGREEN

			Same as :cpp:enumerator:`COLOUR_CODE_GREEN` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKYELLOW

			Same as :cpp:enumerator:`COLOUR_CODE_YELLOW` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKYELLOW1

			Same as :cpp:enumerator:`COLOUR_CODE_YELLOW1` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKYELLOW2

			Same as :cpp:enumerator:`COLOUR_CODE_YELLOW2` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKAMBER

			Same as :cpp:enumerator:`COLOUR_CODE_AMBER` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: COLOUR_CODE_BLACKRED

			Same as :cpp:enumerator:`COLOUR_CODE_RED` but also indicates that aerodrome is closed.

		.. cpp:enumerator:: FROIN

			Indicates frost on the instrument (for example due to fog depositing rime). No data are provided.

		.. cpp:enumerator:: ISSUER_ID_FS

			Identifier of the person who issued the forecast; indicates that the forecast was issued at The Fleet Weather Center San Diego, CA.

		.. cpp:enumerator:: ISSUER_ID_FN

			Identifier of the person who issued the forecast; indicates that the forecast was issued at The Fleet Weather Center Norfolk, VA.



Acquiring group data
^^^^^^^^^^^^^^^^^^^^

		.. cpp:function:: Type type() const

			:returns: Type of value reported in this group.

		.. cpp:function:: std::optional<float> data() const

			:returns: The value reported in this group, or empty ``std::optional`` if the value is not reported.

				.. note:: empty ``std::optional`` is always returned for colour codes (for example BLU or BLACKRED).


Validating
^^^^^^^^^^

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


Formats
-------

:cpp:class:`MiscGroup` represents various formats of uncommon and rare groups recognised by Metaf.

Colour codes
^^^^^^^^^^^^

Colour codes are used to quickly assess visibility and ceiling conditions. The colour code values specified in the reports are shown below.

+-------+----------------------------------------------+------------+----------+---------------------+
| Group |:cpp:func:`type()`                            | Visibility | Ceiling  | :cpp:func:`data()`  |
+=======+==============================================+============+==========+=====================+
| BLU+  | :cpp:enumerator:`Type::COLOUR_CODE_BLUE_PLUS`| >8000 m    | >20000 ft| empty std::optional |
+-------+----------------------------------------------+------------+----------+                     |
| BLU   | :cpp:enumerator:`Type::COLOUR_CODE_BLUE`     | >8000 m    | >2500 ft |                     |
+-------+----------------------------------------------+------------+----------+                     |
| WHT   | :cpp:enumerator:`Type::COLOUR_CODE_WHITE`    | >5000 m    | >1500 ft |                     |
+-------+----------------------------------------------+------------+----------+                     |
| GRN   | :cpp:enumerator:`Type::COLOUR_CODE_GREEN`    | >3700 m    | >700 ft  |                     |
+-------+----------------------------------------------+------------+----------+                     |
| YLO   | :cpp:enumerator:`Type::COLOUR_CODE_YELLOW`   | >1600 m    | >300 ft  |                     |
+-------+----------------------------------------------+------------+----------+                     |
| YLO1  | :cpp:enumerator:`Type::COLOUR_CODE_YELLOW1`  | >2500 m    | >500 ft  |                     |
+-------+----------------------------------------------+------------+----------+                     |
| YLO2  | :cpp:enumerator:`Type::COLOUR_CODE_YELLOW2`  | >1600 m    | >300 ft  |                     |
+-------+----------------------------------------------+------------+----------+                     |
| AMB   | :cpp:enumerator:`Type::COLOUR_CODE_AMBER`    | >800 m     | >200 ft  |                     |
+-------+----------------------------------------------+------------+----------+                     |
| RED   | :cpp:enumerator:`Type::COLOUR_CODE_RED`      | <800 m     | <200 ft  |                     |
+-------+----------------------------------------------+------------+----------+---------------------+

Code BLACK may be specified along with the main colour code. It indicates that the aerodrome is closed e.g. due to snow accumulation. The summary of colour codes with code BLACK age shown below.

+-----------+---------------------------------------------------+---------------------+
| Group     |:cpp:func:`type()`                                 | :cpp:func:`data()`  |
+===========+===================================================+=====================+
| BLACKBLU+ | :cpp:enumerator:`Type::COLOUR_CODE_BLACKBLUE_PLUS`| empty std::optional |
+-----------+---------------------------------------------------+                     |
| BLACKBLU  | :cpp:enumerator:`Type::COLOUR_CODE_BLACKBLUE`     |                     |
+-----------+---------------------------------------------------+                     |
| BLACKWHT  | :cpp:enumerator:`Type::COLOUR_CODE_BLACKWHITE`    |                     |
+-----------+---------------------------------------------------+                     |
| BLACKGRN  | :cpp:enumerator:`Type::COLOUR_CODE_BLACKGREEN`    |                     |
+-----------+---------------------------------------------------+                     |
| BLACKYLO  | :cpp:enumerator:`Type::COLOUR_CODE_BLACKYELLOW`   |                     |
+-----------+---------------------------------------------------+                     |
| BLACKYLO1 | :cpp:enumerator:`Type::COLOUR_CODE_BLACKYELLOW1`  |                     |
+-----------+---------------------------------------------------+                     |
| BLACKYLO2 | :cpp:enumerator:`Type::COLOUR_CODE_BLACKYELLOW2`  |                     |
+-----------+---------------------------------------------------+                     |
| BLACKAMB  | :cpp:enumerator:`Type::COLOUR_CODE_BLACKAMBER`    |                     |
+-----------+---------------------------------------------------+                     |
| BLACKRED  | :cpp:enumerator:`Type::COLOUR_CODE_BLACKRED`      |                     |
+-----------+---------------------------------------------------+---------------------+

.. note:: Colour codes are specified in METAR report body or trends or in the METAR report remarks. 


Corrected weather observation number
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Corrected weather observation numbers are specified in METAR reports and have format ``CCA``, ``CCB``, ``CCC``, ``CCD``, etc. 

Last letter in the group indicates corrected observation number: 

	* ``CCA`` means that the report is the first correction of weather observation which replaces the initial report.
	* ``CCB`` means that the report is the second correction of weather observation which replaces the first corrected report (that is report marked with group ``CCA``).

Metaf can recognise up to 26 corrected observations, from ``CCA`` to ``CCZ``, though in practice this number is much lower.

The examples of corrected weather observation number groups recognised by Metaf are as follows.

+-------+-------------------------------------------------------+------------------------+
| Group |:cpp:func:`type()`                                     | :cpp:func:`data()`     |
+=======+=======================================================+========================+
| CCA   | :cpp:enumerator:`Type::CORRECTED_WEATHER_OBSERVATION` | - std::optional<float> |
|       |                                                       | - value() == 1.0       |
+-------+                                                       +------------------------+
| CCB   |                                                       | - std::optional<float> |
|       |                                                       | - value() == 2.0       |
+-------+                                                       +------------------------+
| CCC   |                                                       | - std::optional<float> |
|       |                                                       | - value() == 3.0       |
+-------+                                                       +------------------------+
| CCD   |                                                       | - std::optional<float> |
|       |                                                       | - value() == 4.0       |
+-------+                                                       +------------------------+
| CCZ   |                                                       | - std::optional<float> |
|       |                                                       | - value() == 26.0      |
+-------+-------------------------------------------------------+------------------------+


Density altitude
^^^^^^^^^^^^^^^^

Density altitude may be specified in remark section of METAR report.

Automated station may specify ``DENSITY ALT MISG`` if the related devices are not operational and it is not possible to calculate density altitude based on the available data.

The examples are shown below.

+--------------------+------------------------------------------+------------------------+
| Groups             | :cpp:func:`type()`                       | :cpp:func:`data()`     |
+====================+==========================================+========================+
| DENSITY ALT 300FT  | :cpp:enumerator:`Type::DENSITY_ALTITUDE` | - std::optional<float> |
|                    |                                          | - value() == 300.0     |
+--------------------+                                          +------------------------+
| DENSITY ALT 1100FT |                                          | - std::optional<float> |
|                    |                                          | - value() == 1100.0    |
+--------------------+                                          +------------------------+
| DENSITY ALT MISG   |                                          | empty std::optional    |
+--------------------+------------------------------------------+------------------------+

Metaf recognises any value of density altitude as soon as it is a valid number with at least one digit and ends with ``FT``.


Frost on the instrument
^^^^^^^^^^^^^^^^^^^^^^^

A single FROIN group may be specified in the remarks of METAR report to indicate frost on the instrument (for example due to fog depositing rime).

+--------+----------------------------------+---------------------+
| Group  | :cpp:func:`type()`               | :cpp:func:`data()`  |
+========+==================================+=====================+
| FROIN  | :cpp:enumerator:`Type::FROIN`    | empty std::optional |
+--------+----------------------------------+---------------------+


Sunshine duration
^^^^^^^^^^^^^^^^^

Sunshine duration occurred on the previous day may be specified in remark section of METAR reports.

The group format is digits 98 followed by three-digit value of sunshine duration in minutes. If no sunshine occurred, the group is coded ``98000``.

The examples of sunshine duration groups recognised by Metaf are as follows.

+-------+---------------------------------------------------+------------------------+
| Group | :cpp:func:`type()`                                | :cpp:func:`data()`     |
+=======+===================================================+========================+
| 98112 | :cpp:enumerator:`Type::SUNSHINE_DURATION_MINUTES` | - std::optional<float> |
|       |                                                   | - value() == 112.0     |
+-------+                                                   +------------------------+
| 98000 |                                                   | - std::optional<float> |
|       |                                                   | - value() == 0         |
+-------+---------------------------------------------------+------------------------+


Largest hailstone size
^^^^^^^^^^^^^^^^^^^^^^

Largest hailstone size may be specified in remark section of METAR reports.

The format is group ``GR`` followed by combination of single-digit integer and/or fraction in inches.

Metaf recognises fraction part specified in quarters of inch: 1/4, 1/2, 3/4.

The examples of such groups are shown below.

+--------------------+----------------------------------------+------------------------+
| Groups             | :cpp:func:`type()`                     | :cpp:func:`data()`     |
+====================+========================================+========================+
| GR 1/2             | :cpp:enumerator:`Type::HAILSTONE_SIZE` | - std::optional<float> |
|                    |                                        | - value() == 0.5       |
+--------------------+                                        +------------------------+
| GR 2 1/4           |                                        | - std::optional<float> |
|                    |                                        | - value() == 2.25      |
+--------------------+                                        +------------------------+
| GR 1 3/4           |                                        | - std::optional<float> |
|                    |                                        | - value() == 1.75      |
+--------------------+----------------------------------------+------------------------+


Issuer identifiers
^^^^^^^^^^^^^^^^^^

Identifier of the person who issued the TAF forecast.

This group is added to military forecasts issued either at The Fleet Weather Center San Diego, CA (FS) or at The Fleet Weather Center Norfolk, VA (FN).

+---------+-------------------------------------------------------+------------------------+
| Group   | :cpp:func:`type()`                                    | :cpp:func:`data()`     |
+=========+=======================================================+========================+
| FN00205 | :cpp:enumerator:`Type::ISSUER_ID_FN`                  | - std::optional<float> |
|         |                                                       | - value() == 205.0     |
+---------+-------------------------------------------------------+------------------------+
| FS30175 | :cpp:enumerator:`Type::ISSUER_ID_FS`                  | - std::optional<float> |
|         |                                                       | - value() == 30175.0   |
+---------+-------------------------------------------------------+------------------------+


Regional variations
^^^^^^^^^^^^^^^^^^^

Colour codes are used in aerodromes operated by air forces of NATO countries.

Instead of colour code Yellow more precise Yellow1 and Yellow2 may be used.

Corrected weather observation number groups and density altitude groups are used only in Canada.

Hailstone size group, sunshine duration group and ``FROIN`` group are used in North America only.

Issuer identifiers FN and FS are used only by US military.

.. cpp:namespace-pop::

