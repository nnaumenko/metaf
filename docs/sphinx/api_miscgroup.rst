MiscGroup
=========

.. cpp:namespace-push:: metaf

.. cpp:class:: MiscGroup

	Stores variety of data which semantically do not belong to any other group type. 

.. cpp:namespace-push:: MiscGroup


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Indicates the type of the data reported in this group.

		.. cpp:enumerator:: SUNSHINE_DURATION_MINUTES

			Sunshine duration that occurred the previous calendar day.

			Use :cpp:func:`data()` to obtain sunshine duration in minutes (the value is zero if no sunshine occurred).

		.. cpp:enumerator:: CORRECTED_WEATHER_OBSERVATION

			This group (coded ``CCA``, ``CCB``, ``CCC``, ``CCD``, etc) designates a corrected weather observation; value reports the sequential number of correction, for example 1st, 2nd, 3rd, etc; this group is only used in Canada.

			Use :cpp:func:`data()` to obtain the number of correction (1 for ``CCA``, 2 for ``CCB``, 3 for ``CCC``, 4 for ``CCD``, etc).

		.. cpp:enumerator:: DENSITY_ALTITUDE

			Density altitude (in feet) reported in remarks. 

			Use :cpp:func:`data()` to obtain the reported density altitude in feet. An empty ``std::optional`` indicates missing density altitude data (coded ``DENSITY ALT MISG`` in remarks).

		.. cpp:enumerator:: HAILSTONE_SIZE

			Largest hailstone size in inches with increments of 1/4 inch. 

			Use :cpp:func:`data()` to obtain the hailstone size: 1 for 1/4", 2 for 1/2" (2/4"), 3 for 3/4", 4 for 1" (4/4"), 5 for 1 1/4" (5/4"), etc.

		.. cpp:enumerator:: COLOUR_CODE_BLUE_PLUS

			Visibility >8000 m AND no cloud obscuring 3/8 or more below 20000 feet. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_BLUE

			Visibility >8000 m AND no cloud obscuring 3/8 or more below 2500 feet. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_WHITE

			Visibility >5000 m AND no cloud obscuring 3/8 or more below 1500 feet. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_GREEN

			Visibility >3700 m AND no cloud obscuring 3/8 or more below 700 feet. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_YELLOW

			Visibility >1600 m AND no cloud obscuring 3/8 or more below 300 feet. No data or time are provided.

			.. note:: Instead of :cpp:enumerator:`COLOUR_CODE_YELLOW` some stations may use more precise :cpp:enumerator:`COLOUR_CODE_YELLOW1` and :cpp:enumerator:`COLOUR_CODE_YELLOW2`.

		.. cpp:enumerator:: COLOUR_CODE_YELLOW1

			Visibility >2500 m AND no cloud obscuring 3/8 or more below 500 feet. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_YELLOW2

			Visibility >1600 m AND no cloud obscuring 3/8 or more below 300 feet. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_AMBER

			Visibility >800 m AND no cloud obscuring 3/8 or more below 200 feet. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_RED

			Visibility <800 m OR clouds obscuring 3/8 or more below 200 feet. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_BLACKBLUE_PLUS

			Same as :cpp:enumerator:`COLOUR_CODE_BLUE_PLUS` but also indicates that aerodrome is closed, for example, due to snow accumulation. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_BLACKBLUE

			Same as :cpp:enumerator:`COLOUR_CODE_BLUE` but also indicates that aerodrome is closed, for example, due to snow accumulation.

			No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_BLACKWHITE

			Same as :cpp:enumerator:`COLOUR_CODE_WHITE` but also indicates that aerodrome is closed. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_BLACKGREEN

			Same as :cpp:enumerator:`COLOUR_CODE_GREEN` but also indicates that aerodrome is closed. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_BLACKYELLOW

			Same as :cpp:enumerator:`COLOUR_CODE_YELLOW` but also indicates that aerodrome is closed.

			No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_BLACKYELLOW1

			Same as :cpp:enumerator:`COLOUR_CODE_YELLOW1` but also indicates that aerodrome is closed. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_BLACKYELLOW2

			Same as :cpp:enumerator:`COLOUR_CODE_YELLOW2` but also indicates that aerodrome is closed. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_BLACKAMBER

			Same as :cpp:enumerator:`COLOUR_CODE_AMBER` but also indicates that aerodrome is closed. No data or time are provided.

		.. cpp:enumerator:: COLOUR_CODE_BLACKRED

			Same as :cpp:enumerator:`COLOUR_CODE_RED` but also indicates that aerodrome is closed. No data or time are provided.

		.. cpp:enumerator:: FROIN

			Indicates frost on the instrument (for example due to fog depositing rime). 

			No data or time are provided.

		.. cpp:enumerator:: ISSUER_ID_FS

			Numeric identifier of the person who issued the forecast; indicates that the forecast was issued at The Fleet Weather Center San Diego, CA.

			Use :cpp:func:`data()` to obtain the numeric identifier.

		.. cpp:enumerator:: ISSUER_ID_FN

			Numeric identifier of the person who issued the forecast; indicates that the forecast was issued at The Fleet Weather Center Norfolk, VA.

			Use :cpp:func:`data()` to obtain the numeric identifier.

		.. cpp:enumerator:: FIRST

			Indicates first manned observation. No data or time are provided.

		.. cpp:enumerator:: LAST

			Indicates last manned observation or forecast. 

			Use :cpp:func:`time()` to obtain the time of next scheduled observation (will return empty ``std::optional`` if no next scheduled observation time was specified).

		.. cpp:enumerator:: LAST_STAFFED_OBSERVATION

			Indicates last staffed observation. 

			Use :cpp:func:`time()` to obtain the time of next scheduled observation (will return empty ``std::optional`` if no next scheduled observation time was specified).

		.. cpp:enumerator:: NO_AMENDMENTS_AFTER

			Indicates that no amendments are are scheduled after specified time.

			Use :cpp:func:`time()` to obtain the time after which no amendments are scheduled.

		.. cpp:enumerator:: NEXT:

			Specifies the time when the next report is scheduled.

			Use :cpp:func:`time()` to obtain the time when the next report is scheduled.

		.. cpp:enumerator:: NEXT_FORECAST:

			Specifies the time when the next forecast is scheduled.

			Use :cpp:func:`time()` to obtain the time when the next report is scheduled.

		.. cpp:enumerator:: AMENDED_AT

			Specifies the time when the report was amended.

			Use :cpp:func:`time()` to obtain the time when the amended report was issued.

		.. cpp:enumerator:: CANCELLED_AT

			Specifies the time when the forecast was cancelled.

			Use :cpp:func:`time()` to obtain the time when forecast was cancelled.

		.. cpp:enumerator:: FORECAST_BASED_ON_AUTO_OBSERVATION

			Indicates that the forecast is based on automated observation.

			No data or time are provided.

Acquiring group data
^^^^^^^^^^^^^^^^^^^^

		.. cpp:function:: Type type() const

			:returns: Type of value reported in this group.

		.. cpp:function:: std::optional<float> data() const

			:returns: The value reported in this group, or empty ``std::optional`` if the value is not reported or not applicable for this type of group (for example colour codes).

		.. cpp:function:: std::optional<MetafTime> time() const

			:returns: The time reported in this group, or empty ``std::optional`` if the time is not applicable for this type of group.


Validating
^^^^^^^^^^

		.. cpp:function:: bool isValid() const

			:returns: Always returns ``true``.


Formats
-------

:cpp:class:`MiscGroup` represents various formats of uncommon, rare and regional group formats recognised by Metaf.

Data represented by MiscGroup semantically does not belong to any other group type.

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

