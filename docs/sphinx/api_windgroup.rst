WindGroup
=========

.. cpp:namespace-push:: metaf

WindGroup class
---------------

.. cpp:class:: WindGroup

	Stores information about surface wind (including variable wind direction sector if reported), wind shear, wind shift, and peak wind, and missing wind data.

.. cpp:namespace-push:: WindGroup

Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Specifies what kind of information is stored within this group.

		.. cpp:enumerator:: SURFACE_WIND

			Surface wind information. Use :cpp:func:`direction()` for wind direction, :cpp:func:`windSpeed()` for sustained wind speed, and :cpp:func:`gustSpeed()` for gust speed.

		.. cpp:enumerator:: SURFACE_WIND_CALM

			Indicates surface wind calm (i.e. no wind). :cpp:func:`windSpeed()` is always zero.

		.. cpp:enumerator:: VARIABLE_WIND_SECTOR

			Only variable wind direction sector information. Use :cpp:func:`varSectorBegin()` for beginning direction of the sector, and :cpp:func:`varSectorEnd()` for ending direction.

			.. note::Wind direction sector is defined from beginning direction clockwise to the ending direction.

		.. cpp:enumerator:: SURFACE_WIND_WITH_VARIABLE_SECTOR

			Surface wind with variable wind sector. Use :cpp:func:`direction()` for wind direction, :cpp:func:`windSpeed()` for sustained wind speed, :cpp:func:`gustSpeed()` for gust speed, :cpp:func:`varSectorBegin()` for beginning direction of the sector, and :cpp:func:`varSectorEnd()` for ending direction.

		.. cpp:enumerator:: WIND_SHEAR

			Wind shear at certain height. Use :cpp:func:`direction()` for wind direction, :cpp:func:`windSpeed()` for sustained wind speed, :cpp:func:`gustSpeed()` for gust speed, and :cpp:func:`height()` for the height where wind shear occurs.

		.. cpp:enumerator:: WIND_SHEAR_IN_LOWER_LAYERS

			Indicates existence of wind shear along the take-off path or approach path between runway level and 500 metres (1 600 ft) significant to aircraft operations, for the particlar runway or all runways. Use :cpp:func:`runway()` for runway for which wind shear in the lower layers is indicated; :cpp:func:`runway()` may also return 'all runways'.

		.. cpp:enumerator:: WIND_SHIFT

			Wind shift; wind direction changed 45 degrees or more in less than 15 minutes with sustained wind speed of 10 knots. Use :cpp:func:`eventTime()` for the time when wind shift began.

		.. cpp:enumerator:: WIND_SHIFT_FROPA

			Same as :cpp:enumerator:`metaf::WindGroup::Type::WIND_SHIFT` but indicates that wind shift is associated with frontal passage. Use :cpp:func:`eventTime()` for the time when wind shift began.

		.. cpp:enumerator:: PEAK_WIND

			Peak wind information since last METAR. Use :cpp:func:`direction()` for wind direction, :cpp:func:`windSpeed()` for peak wind speed, and :cpp:func:`eventTime()` for the time when peak wind was observed.

		.. cpp:enumerator:: WSCONDS

			Indicates that potential wind shear conditions are present but there's not enough information to reliably forecast height, direction and speed of wind shear.

		.. cpp:enumerator:: WND_MISG

			Indicates that wind data is missing. No further details are provided.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Wind group type, i.e. what kind of information is stored.

	.. cpp:function:: Direction direction() const

		:returns: Mean wind direction.

	.. cpp:function:: Speed windSpeed() const
	
		:returns: Wind speed.

	.. cpp:function:: Speed gustSpeed() const

		:returns: Wind gust speed.

	.. cpp:function:: Direction varSectorBegin() const

		:returns: Start direction point of variable wind direction sector.

	.. cpp:function:: Direction varSectorEnd() const

		:returns: End direction point of variable wind direction sector.

	.. note::Wind direction sector is defined from start point clockwise to end point.

	.. cpp:function:: Distance height() const

		:returns: Height at which wind shear occurs or a non-reported value if surface wind data are specified.

	.. cpp:function:: std::optional<MetafTime> eventTime() const

		:returns: Time when a certain weather event related to wind has happened (e.g. time when wind shift began, etc).


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if stored wind information is valid, and ``false`` otherwise.

			The information is considered valid if all of the following conditions are met: 
				- If both gust speed and wind speed are reported, wind speed is less than gust speed;
				- If gust speed is reported, its value is non-zero;
				- If wind shear height is reported then wind shear height value is non-zero;
				- Wind direction, wind shear height, variable wind sector directions must be valid values if reported.

.. cpp:namespace-pop::


Formats
-------

The following surface wind, wind shear, wind shift, and peak wind groups are recognised by Metaf.

- Surface wind, with or without gusts, with or without variable wind sector
- Wind shear at specified height
- Wind shear at the lower levels
- Potential wind shear conditions
- Wind shift, with or without associated frontal passage
- Peak wind.
- Missing wind data.  


Surface wind and wind shear at specified height
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Basic wind group
""""""""""""""""

The basic format for wind group is ``xx0xxKT`` and ``xx0xxMPS``. 

First three digits specify the mean direction where the wind is coming from as :ref:`api_direction:Numeric value in degrees`. Last digit of the direction value is always zero.

Second two digits specify sustained wind speed in :ref:`api_speed:Wind speed format`.

For example, west wind with sustained speed 12 knots is coded as ``27012KT``.

Variable wind group
"""""""""""""""""""

If the wind is variable and mean direction cannot be determined, VRB is specified instead of 3-digit direction, for example ``VRB02MPS`` means variable wind with sustained speed 2 meters per second.

'Wind with gusts' group
"""""""""""""""""""""""

If wind gust speed is specified along with sustained wind speed, format ``xx0xxGxxKT`` and ``xx0xxGxxMPS`` is used. Two digits after character ``G`` specify wind gust speed in :ref:`api_speed:Wind speed format`. For example, northwest wind with sustained speed 12 meters per second and gust speed up to 16 meters per second is coded as ``33012G16MPS``.

Higher values for wind speed and gust speed
"""""""""""""""""""""""""""""""""""""""""""

When sustained wind speed or gust speed exceed 99 knots or 99 meters per second, three digits are specified for the corresponding value. For example group ``08063G112KT`` means eastern wind (direction 80 degrees) with sustained speed 63 knots and gusts up to 112 knots. Exactly this group was included in the SPECI report from USAF Base Tyndall when it was hit by hurricane Michael in October 2018!

Calm wind
"""""""""

Calm wind (i.e. no wind) is coded as ``00000KT`` or ``00000MPS``.

Non-reported values
"""""""""""""""""""

If the wind direction and wind speed are not reported, their digits are replaced with ``/`` character, as per :ref:`api_direction:Numeric value in degrees` and :ref:`api_speed:Wind speed format`. 

For example ``/////KT`` or ``/////MPS``. When gust speed is not reported, it is not included in the group rather than being replaced with ``/`` character, thus group ``/////G//KT`` is not recognised by Metaf.

Variable wind direction sector
""""""""""""""""""""""""""""""

If the wind direction is variable within certain direction sector, the wind group is augmented by additional direction sector group. Such group has a format ``xx0Vxx0``. First three digist specify the beginning of the sector, and last three digits specify end of the sector, clockwise from sector beginning direction. The direction is :ref:`api_direction:Numeric value in degrees` as well.

For example, group sequence ``02002MPS 330V070`` means wind with mean direction 20 degrees (north-north-east), sustained speed 2 m/s and indicates that the wind direction is variable from 330 degrees (northwest) clockwise to 70 degrees (north-east-east).

Wind shear at specified height
""""""""""""""""""""""""""""""

Wind shear occurring at certain height has similar format ``WSxxx/xx0xxKT``. Three digits after WS specify height in hundeds of feet, and the rest of the group is exactly the same as surface wind group. Wind shear group cannot be augmented by variable direction sector group.

For example, group ``WS020/05065KT`` means that wind shear occurs at speed 2000 feet and the wind direction is 50 degrees while sustained speed is 65 knots.

Generalised syntax
""""""""""""""""""

Generalised syntax of surface wind, variable wind sector, and wind shear groups recognised by Metaf is below. 

.. image:: windgroup_surface.svg

.. image:: windgroup_sector.svg

.. image:: windgroup_shear.svg

Examples
""""""""

The table below summarises various formats of surface wind and wind shear recognised by Metaf. The table shows values returned by getter methods of :cpp:class:`WindGroup`.

+----------------------+----------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+-------------------------------------------------------------------+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------+----------------------------------------+
|Group                 |:cpp:func:`type()`                                        |:cpp:func:`direction()`                                                            |:cpp:func:`windSpeed()`                                                        |:cpp:func:`gustSpeed()`                                            |:cpp:func:`height()`                                                             |:cpp:func:`varSectorBegin()`                                                       |:cpp:func:`varSectorEnd()`                                                         |:cpp:func:`eventTime()`                    |:cpp:func:`runway()`                    |
+======================+==========================================================+===================================================================================+===============================================================================+===================================================================+=================================================================================+===================================================================================+===================================================================================+===========================================+========================================+
|00000KT               |:cpp:enumerator:`Type::SURFACE_WIND_CALM`                 |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                                           |- :cpp:class:`Speed`                                               |- :cpp:class:`Distance`                                                          |- :cpp:class:`Direction`                                                           |- :cpp:class:`Direction`                                                           |empty std::optional<:cpp:class:`MetafTime`>|empty std::optional<:cpp:class:`Runway`>|
|                      |                                                          |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |- :cpp:func:`Speed::isReported()` == false                                     |- :cpp:func:`Speed::isReported()` == false                         |- :cpp:func:`Distance::isReported()` == false                                    |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |                                           |                                        |
+----------------------+----------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|07018KT               |:cpp:enumerator:`Type::SURFACE_WIND`                      |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                                           |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_DEGREES`|- :cpp:func:`Speed::unit()` == :cpp:enumerator:`Speed::Unit::KNOTS`            |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::degrees()` == 70                                           |- :cpp:func:`Speed::speed()` == 18                                             |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+                                                          |                                                                                   +-------------------------------------------------------------------------------+                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|07003MPS              |                                                          |                                                                                   |- :cpp:class:`Speed`                                                           |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |                                                                                   |- :cpp:func:`Speed::unit()` == :cpp:enumerator:`Speed::Unit::METERS_PER_SECOND`|                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |                                                                                   |- :cpp:func:`Speed::speed()` == 3                                              |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+                                                          +-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|VRB06KT               |                                                          |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                                           |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_DEGREES`|- :cpp:func:`Speed::unit()` == :cpp:enumerator:`Speed::Unit::KNOTS`            |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |                                                                                   |- :cpp:func:`Speed::speed()` == 6                                              |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+                                                          +-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|/////KT               |                                                          |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                                           |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |- :cpp:func:`Speed::isReported()` == false                                     |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+                                                          +-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+-------------------------------------------------------------------+                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|09022G35KT            |                                                          |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                                           |- :cpp:class:`Speed`                                               |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_DEGREES`|- :cpp:func:`Speed::unit()` == :cpp:enumerator:`Speed::Unit::KNOTS`            |- :cpp:func:`Speed::unit()` == :cpp:enumerator:`Speed::Unit::KNOTS`|                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::degrees()` == 90                                           |- :cpp:func:`Speed::speed()` == 22                                             |- :cpp:func:`Speed::speed()` == 35                                 |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+----------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+-------------------------------------------------------------------+                                                                                 +-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+                                           |                                        |
|25007KT 220V300       |:cpp:enumerator:`Type::SURFACE_WIND_WITH_VARIABLE_SECTOR` |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                                           |- :cpp:class:`Speed`                                               |                                                                                 |- :cpp:class:`Direction`                                                           |- :cpp:class:`Direction`                                                           |                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_DEGREES`|- :cpp:func:`Speed::unit()` == :cpp:enumerator:`Speed::Unit::KNOTS`            |- :cpp:func:`Speed::isReported()` == false                         |                                                                                 |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_DEGREES`|- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_DEGREES`|                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::degrees()` == 250                                          |- :cpp:func:`Speed::speed()` == 7                                              |                                                                   |                                                                                 |- :cpp:func:`Direction::degrees()` == 220                                          |- :cpp:func:`Direction::degrees()` == 300                                          |                                           |                                        |
+----------------------+----------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|220V300               |:cpp:enumerator:`Type::VARIABLE_WIND_SECTOR`              |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                                           |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |- :cpp:func:`Speed::isReported()` == false                                     |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+----------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+                                                                   +---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+                                           |                                        |
|WS020/05065KT         |:cpp:enumerator:`Type::WIND_SHEAR`                        |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                                           |                                                                   |- :cpp:class:`Distance`                                                          |- :cpp:class:`Direction`                                                           |- :cpp:class:`Direction`                                                           |                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_DEGREES`|- :cpp:func:`Speed::unit()` == :cpp:enumerator:`Speed::Unit::KNOTS`            |                                                                   |- :cpp:enumerator:`Distance::Unit::FEET`                                         |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |                                           |                                        |
|                      |                                                          |- :cpp:func:`Direction::degrees()` == 50                                           |- :cpp:func:`Speed::speed()` == 65                                             |                                                                   |- :cpp:func:`Distance::modifier()` == :cpp:enumerator:`Distance::Modifier::NONE` |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                                          |                                                                                   |                                                                               |                                                                   |- :cpp:func:`Distance::distance()` == 2000                                       |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+----------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+-------------------------------------------------------------------+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------+----------------------------------------+


Peak wind and wind shift
^^^^^^^^^^^^^^^^^^^^^^^^

Peak wind and wind shift groups may be included in the remarks. 

Peak wind
"""""""""

The peak wind format recognised by Metaf is ``PK WND xx0xx/xxxx``.

First three digits specify mean wind direction as :ref:`api_direction:Numeric value in degrees`, next two digits specify sustained peak wind speed in knots (not followed by KT).

Four digits after ``/`` character specify time when peak wind occurred as hour and minute. If the hour is the same as report release hour, only two digits for minutes may be specified.

For example, sequence of groups ``PK WND 02052/2112`` means that peak wind with direction of 20 degrees and sustained speed 52 knots was observed at 21:12. Sequence of groups ``PK WND 24029/06`` means that peak wind with direction of 240 degrees and sustained speed 29 knots was observed at 06 minutes past the hour when the report was released.

Generalised syntax of peak wind groups recognised by Metaf is below. 

.. image:: windgroup_peak.svg

Wind shift
""""""""""

Wind shift groups have format ``WSHFT xxxx``. Four digits are time in form of hour and minute, when wind shift began. If wind shift began on the same hour when report is released only two digits for minutes are specified. 

``FROPA`` is added to the group sequence if the wind shift is associated with atmospheric front passage.

For example, group sequence ``WSHFT 1851`` means that wind shift began at 18:51, and group sequence ``WSHFT 30 FROPA`` means that wind shift began on 30 minutes past the hour when the report was released, and this wind shift is associated with the atmospheric front passage.

Peak wind and wind shift examples
"""""""""""""""""""""""""""""""""

The table below summarises various formats of peak wind and wind shift groups recognised by Metaf. The table shows values returned by getter methods of :cpp:class:`WindGroup`.

.. note:: Some of the groups refer to hour value from the report release time. In the examples below assume that report release time is: day-of-month 28, hour 15, minute 44 (coded as ``281544Z`` in the METAR report). Note that hour 15 from report time is used in groups where hour is not specified.

+----------------------+----------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------+--------------------------------------------------------+----------------------------------------------+-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------+----------------------------------------+
|Group                 |:cpp:func:`type()`                      |:cpp:func:`direction()`                                                            |:cpp:func:`windSpeed()`                                            |:cpp:func:`eventTime()`                                 |:cpp:func:`height()`                          |:cpp:func:`varSectorBegin()`                                                       |:cpp:func:`varSectorEnd()`                                                         |:cpp:func:`gustSpeed()`                    |:cpp:func:`runway()`                    |
+======================+========================================+===================================================================================+===================================================================+========================================================+==============================================+===================================================================================+===================================================================================+===========================================+========================================+
|PK WND 02052/2112     |:cpp:enumerator:`Type::PEAK_WIND`       |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                               |std::optional<:cpp:class:`MetafTime`>                   |- :cpp:class:`Distance`                       |- :cpp:class:`Direction`                                                           |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                       |empty std::optional<:cpp:class:`Runway`>|
|                      |                                        |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_DEGREES`|- :cpp:func:`Speed::unit()` == :cpp:enumerator:`Speed::Unit::KNOTS`|- :cpp:func:`MetafTime::day()`.has_value() == false     |- :cpp:func:`Distance::isReported()` == false |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |- :cpp:func:`Speed::isReported()` == false |                                        |
|                      |                                        |- :cpp:func:`Direction::degrees()` == 20                                           |- :cpp:func:`Speed::speed()` == 52                                 |- :cpp:func:`MetafTime::hour()` == 21                   |                                              |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                        |                                                                                   |                                                                   |- :cpp:func:`MetafTime::minute()` == 12                 |                                              |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+                                        +-----------------------------------------------------------------------------------+-------------------------------------------------------------------+--------------------------------------------------------+                                              |                                                                                   |                                                                                   |                                           |                                        |
|PK WND 24029/06       |                                        |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                               |std::optional<:cpp:class:`MetafTime`>                   |                                              |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                        |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::VALUE_DEGREES`|- :cpp:func:`Speed::unit()` == :cpp:enumerator:`Speed::Unit::KNOTS`|- :cpp:func:`MetafTime::day()`.has_value() == false     |                                              |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                        |- :cpp:func:`Direction::degrees()` == 240                                          |- :cpp:func:`Speed::speed()` == 29                                 |- :cpp:func:`MetafTime::hour()` == 15 (from report time)|                                              |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                        |                                                                                   |                                                                   |- :cpp:func:`MetafTime::minute()` == 06                 |                                              |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+----------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------+--------------------------------------------------------+                                              |                                                                                   |                                                                                   |                                           |                                        |
|WSHFT 1851            |:cpp:enumerator:`Type::WIND_SHIFT`      |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                               |std::optional<:cpp:class:`MetafTime`>                   |                                              |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                        |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |- :cpp:func:`Speed::isReported()` == false                         |- :cpp:func:`MetafTime::day()`.has_value() == false     |                                              |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                        |                                                                                   |                                                                   |- :cpp:func:`MetafTime::hour()` == 18                   |                                              |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                        |                                                                                   |                                                                   |- :cpp:func:`MetafTime::minute()` == 51                 |                                              |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+----------------------------------------+                                                                                   |                                                                   +--------------------------------------------------------+                                              |                                                                                   |                                                                                   |                                           |                                        |
|WSHFT 30 FROPA        |:cpp:enumerator:`Type::WIND_SHIFT_FROPA`|                                                                                   |                                                                   |std::optional<:cpp:class:`MetafTime`>                   |                                              |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                        |                                                                                   |                                                                   |- :cpp:func:`MetafTime::day()`.has_value() == false     |                                              |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                        |                                                                                   |                                                                   |- :cpp:func:`MetafTime::hour()` == 15 (from report time)|                                              |                                                                                   |                                                                                   |                                           |                                        |
|                      |                                        |                                                                                   |                                                                   |- :cpp:func:`MetafTime::minute()` == 30                 |                                              |                                                                                   |                                                                                   |                                           |                                        |
+----------------------+----------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------+--------------------------------------------------------+----------------------------------------------+-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------+----------------------------------------+

Miscellaneous wind groups
^^^^^^^^^^^^^^^^^^^^^^^^^

Wind shear at the lower layers is indicated by the group sequence ``WS Rxx`` or ``WS RWYxx`` (for example, ``WS RWY11`` or ``WS RWY28L``) when it is specified for the single runway, or by group sequence ``WS ALL RWY`` if specified for all runways (see also :ref:`api_runway:Runway format`).

Generalised syntax of wind shear at lower layers group recognised by Metaf is below. 

.. image:: windgroup_ws.svg

When potential wind shear conditions are present but there's not enough information to reliably forecast height, direction and speed of wind shear, group ``WSCONDS`` is included in TAF reports.

Missing wind data coded as group sequence ``WND MISG``.

The table below summarises various formats of peak wind and wind shift groups recognised by Metaf. The table shows values returned by getter methods of :cpp:class:`WindGroup`.

+----------------------+--------------------------------------------------+----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+-------------------------------------------------------------------+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------+
|Group                 |:cpp:func:`type()`                                |:cpp:func:`runway()`                                                              |:cpp:func:`direction()`                                                            |:cpp:func:`windSpeed()`                                                        |:cpp:func:`gustSpeed()`                                            |:cpp:func:`height()`                                                             |:cpp:func:`varSectorBegin()`                                                       |:cpp:func:`varSectorEnd()`                                                         |:cpp:func:`eventTime()`                    |
+======================+==================================================+==================================================================================+===================================================================================+===============================================================================+===================================================================+=================================================================================+===================================================================================+===================================================================================+===========================================+
|WS R32                |:cpp:enumerator:`Type::WIND_SHEAR_IN_LOWER_LAYERS`|- std::optional<:cpp:class:`Runway`>                                              |- :cpp:class:`Direction`                                                           |- :cpp:class:`Speed`                                                           |- :cpp:class:`Speed`                                               |- :cpp:class:`Distance`                                                          |- :cpp:class:`Direction`                                                           |- :cpp:class:`Direction`                                                           |empty std::optional<:cpp:class:`MetafTime`>|
+----------------------+                                                  |- :cpp:func:`Runway::number()` == 32                                              |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |- :cpp:func:`Speed::isReported()` == false                                     |- :cpp:func:`Speed::isReported()` == false                         |- :cpp:func:`Distance::isReported()` == false                                    |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |- :cpp:func:`Direction::type()` == :cpp:enumerator:`Direction::Type::NOT_REPORTED` |                                           |
|WS RWY32              |                                                  |- :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::NONE`  |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
+----------------------+                                                  +----------------------------------------------------------------------------------+                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|WS R26L               |                                                  |- std::optional<:cpp:class:`Runway`>                                              |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|                      |                                                  |- :cpp:func:`Runway::number()` == 26                                              |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|                      |                                                  |- :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::LEFT`  |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
+----------------------+                                                  +----------------------------------------------------------------------------------+                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|WS R26C               |                                                  |- std::optional<:cpp:class:`Runway`>                                              |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|                      |                                                  |- :cpp:func:`Runway::number()` == 26                                              |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|                      |                                                  |- :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::CENTER`|                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
+----------------------+                                                  +----------------------------------------------------------------------------------+                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|WS R26R               |                                                  |- std::optional<:cpp:class:`Runway`>                                              |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|                      |                                                  |- :cpp:func:`Runway::number()` == 26                                              |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|                      |                                                  |- :cpp:func:`Runway::designator()` == :cpp:enumerator:`Runway::Designator::RIGHT` |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
+----------------------+                                                  +----------------------------------------------------------------------------------+                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|WS ALL RWY            |                                                  |- std::optional<:cpp:class:`Runway`>                                              |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|                      |                                                  |- :cpp:func:`Runway::isAllRunways()` == true                                      |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
+----------------------+--------------------------------------------------+----------------------------------------------------------------------------------+                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|WSCONDS               |:cpp:enumerator:`Type::WSCONDS`                   |empty std::optional<:cpp:class:`Runway`>                                          |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
+----------------------+--------------------------------------------------+                                                                                  |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
|WND MISG              |:cpp:enumerator:`Type::WND_MISG`                  |                                                                                  |                                                                                   |                                                                               |                                                                   |                                                                                 |                                                                                   |                                                                                   |                                           |
+----------------------+--------------------------------------------------+----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------------------------------------------+-------------------------------------------------------------------+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+-------------------------------------------+


Regional variations
^^^^^^^^^^^^^^^^^^^

Wind shift and peak wind groups are used mostly in North America and by military aerodromes of NATO countries.

Depending on the region, the wind/gust speed units vary between knots and meters per second.

.. cpp:namespace-pop::
