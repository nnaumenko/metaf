TemperatureGroup
================

.. cpp:namespace-push:: metaf

TemperatureGroup class
----------------------

.. cpp:class:: TemperatureGroup

	Stores information about current ambient air temperature and dew point. Group which reports values rounded to integer of degrees Celsius (e.g. ``10/M00``) is included in METAR report body. Group which reports values in tength of degrees Celsius (e.g. ``T02330206``) is used in North America and is included in remarks.

.. cpp:namespace-push:: TemperatureGroup


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Group type which specifies what kind of data stored within this group.

		.. cpp:enumerator:: TEMPERATURE_AND_DEW_POINT

			Temperature and dew point. Use :cpp:func:`airTemperature()` and :cpp:func:`dewPoint()` for ambient air temperature and dew point.

		.. cpp:enumerator:: T_MISG

			Temperature data is missing. No further details provided.

		.. cpp:enumerator:: TD_MISG

			Dew point data is missing. No further details provided.

Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Type of the group.

	.. cpp:function:: Temperature airTemperature() const

		:returns: Ambient air temperature.

	.. cpp:function:: Temperature dewPoint() const

		:returns: Dew point.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if stored ambient air temperature and dew point information is valid and the group is complete; ``false`` otherwise. The information is considered valid if the dew point is less or equal than ambient air temperature.

.. cpp:namespace-pop::


Formats
-------

Metaf recognises the followsing formats of temperature / dew point groups:
- Standard-precision group specified in METAR report body
- High-precision group specified in METAR remarks
- Missing data groups


Standard-precision temperature/dew point group
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Format ``xx/xx`` is used by groups specified in METAR report body and reports temperature and dew point with resolution of 1°C. 

First two digits (optionally preceded by ``M`` character) specify ambient air temperature, and last two digits (also optionally preceded by ``M`` character) specify dew point. The values are specified in :ref:`api_temperature:Standard-precision temperature format`.

Dew point after ``/`` character may be ommited from the group when it is not reported.

If the temperature and/or dew point value is not reported, its two digits are replaced with ``//``.

Generalised syntax of standard-precision temperature groups recognised by Metaf is below.

.. image:: temperaturegroup_metar.svg

The table below provides the examples of standard-precision temperature group recognised by Metaf. The table shows values returned by getter methods of :cpp:class:`TemperatureGroup`.

+-------+-------------------------------------------------+---------------------------------------------------------------------------+---------------------------------------------------------------------------+
|Group  |:cpp:func:`type()`                               |:cpp:func:`airTemperature()`                                               |:cpp:func:`dewPoint()`                                                     |
+=======+=================================================+===========================================================================+===========================================================================+
|12/09  |:cpp:enumerator:`Type::TEMPERATURE_AND_DEW_POINT`|- :cpp:class:`Temperature`                                                 |- :cpp:class:`Temperature`                                                 |
|       |                                                 |- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|
|       |                                                 |- :cpp:func:`Temperature::temperature()`.value() == 12.0                   |- :cpp:func:`Temperature::temperature()`.value() == 9.0                    |
|       |                                                 |- :cpp:func:`Temperature::isFreezing()` == false                           |- :cpp:func:`Temperature::isFreezing()` == false                           |
|       |                                                 |- :cpp:func:`Temperature::isPrecise()` == false                            |- :cpp:func:`Temperature::isPrecise()` == false                            |
+-------+                                                 +---------------------------------------------------------------------------+---------------------------------------------------------------------------+
|M05/M12|                                                 |- :cpp:class:`Temperature`                                                 |- :cpp:class:`Temperature`                                                 |
|       |                                                 |- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|
|       |                                                 |- :cpp:func:`Temperature::temperature()`.value() == -5.0                   |- :cpp:func:`Temperature::temperature()`.value() == -12.0                  |
|       |                                                 |- :cpp:func:`Temperature::isFreezing()` == true                            |- :cpp:func:`Temperature::isFreezing()` == true                            |
|       |                                                 |- :cpp:func:`Temperature::isPrecise()` == false                            |- :cpp:func:`Temperature::isPrecise()` == false                            |
+-------+                                                 +---------------------------------------------------------------------------+---------------------------------------------------------------------------+
|00/M00 |                                                 |- :cpp:class:`Temperature`                                                 |- :cpp:class:`Temperature`                                                 |
|       |                                                 |- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|
|       |                                                 |- :cpp:func:`Temperature::temperature()`.value() == 0.0                    |- :cpp:func:`Temperature::temperature()`.value() == 0.0                    |
|       |                                                 |- :cpp:func:`Temperature::isFreezing()` == false                           |- :cpp:func:`Temperature::isFreezing()` == true                            |
|       |                                                 |- :cpp:func:`Temperature::isPrecise()` == false                            |- :cpp:func:`Temperature::isPrecise()` == false                            |
+-------+                                                 +---------------------------------------------------------------------------+---------------------------------------------------------------------------+
|M00/M04|                                                 |- :cpp:class:`Temperature`                                                 |- :cpp:class:`Temperature`                                                 |
|       |                                                 |- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|
|       |                                                 |- :cpp:func:`Temperature::temperature()`.value() == 0.0                    |- :cpp:func:`Temperature::temperature()`.value() == -4.0                   |
|       |                                                 |- :cpp:func:`Temperature::isFreezing()` == true                            |- :cpp:func:`Temperature::isFreezing()` == true                            |
|       |                                                 |- :cpp:func:`Temperature::isPrecise()` == false                            |- :cpp:func:`Temperature::isPrecise()` == false                            |
+-------+                                                 +---------------------------------------------------------------------------+---------------------------------------------------------------------------+
|M70/   |                                                 |- :cpp:class:`Temperature`                                                 |- :cpp:class:`Temperature`                                                 |
+-------+                                                 |- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|- :cpp:func:`Temperature::isReported()` == false                           |
|M70/// |                                                 |- :cpp:func:`Temperature::temperature()`.value() == -70.0                  |                                                                           |
|       |                                                 |- :cpp:func:`Temperature::isFreezing()` == true                            |                                                                           |
|       |                                                 |- :cpp:func:`Temperature::isPrecise()` == false                            |                                                                           |
+-------+                                                 +---------------------------------------------------------------------------+---------------------------------------------------------------------------+
|| /////|                                                 |- :cpp:class:`Temperature`                                                 |- :cpp:class:`Temperature`                                                 |
|       |                                                 |- :cpp:func:`Temperature::isReported()` == false                           |- :cpp:func:`Temperature::isReported()` == false                           |
+-------+-------------------------------------------------+---------------------------------------------------------------------------+---------------------------------------------------------------------------+

High-precision temperature/dew point group
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Group ``Txxxxxxxx`` may be specified in METAR report remarks. It reports temperature and dew point in a :ref:`api_temperature:High-precision temperature format`.

Eight digits after ``T`` character are two values, four digits each; first four digits specify temperature, and last four digits specify dew point.

For example, in group ``T03491042``, digits ``0349`` indicate ambient air temperature of 34.9°C and digits ``1042`` indicate dew point of -4.2°C.

Generalised syntax of high-precision temperature groups recognised by Metaf is below. 

.. image:: temperaturegroup_rmk.svg

The table below provideds examples of high-precision temperature group recognised by Metaf. The table shows values returned by getter methods of :cpp:class:`TemperatureGroup`.

+---------+-------------------------------------------------+---------------------------------------------------------------------------+---------------------------------------------------------------------------+
|Group    |:cpp:func:`type()`                               |:cpp:func:`airTemperature()`                                               |:cpp:func:`dewPoint()`                                                     |
+=========+=================================================+===========================================================================+===========================================================================+
|T03491042|:cpp:enumerator:`Type::TEMPERATURE_AND_DEW_POINT`|- :cpp:class:`Temperature`                                                 |- :cpp:class:`Temperature`                                                 |
|         |                                                 |- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|
|         |                                                 |- :cpp:func:`Temperature::temperature()`.value() == 34.9                   |- :cpp:func:`Temperature::temperature()`.value() == -4.2                   |
|         |                                                 |- :cpp:func:`Temperature::isFreezing()` == false                           |- :cpp:func:`Temperature::isFreezing()` == true                            |
|         |                                                 |- :cpp:func:`Temperature::isPrecise()` == true                             |- :cpp:func:`Temperature::isPrecise()` == true                             |
+---------+                                                 +---------------------------------------------------------------------------+---------------------------------------------------------------------------+
|T00060006|                                                 |- :cpp:class:`Temperature`                                                 |- :cpp:class:`Temperature`                                                 |
|         |                                                 |- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|
|         |                                                 |- :cpp:func:`Temperature::temperature()`.value() == 0.6                    |- :cpp:func:`Temperature::temperature()`.value() == 0.6                    |
|         |                                                 |- :cpp:func:`Temperature::isFreezing()` == false                           |- :cpp:func:`Temperature::isFreezing()` == false                           |
|         |                                                 |- :cpp:func:`Temperature::isPrecise()` == true                             |- :cpp:func:`Temperature::isPrecise()` == true                             |
+---------+                                                 +---------------------------------------------------------------------------+---------------------------------------------------------------------------+
|T00001011|                                                 |- :cpp:class:`Temperature`                                                 |- :cpp:class:`Temperature`                                                 |
|         |                                                 |- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|
|         |                                                 |- :cpp:func:`Temperature::temperature()`.value() == 0.0                    |- :cpp:func:`Temperature::temperature()`.value() == -1.1                   |
|         |                                                 |- :cpp:func:`Temperature::isFreezing()` == false                           |- :cpp:func:`Temperature::isFreezing()` == true                            |
|         |                                                 |- :cpp:func:`Temperature::isPrecise()` == true                             |- :cpp:func:`Temperature::isPrecise()` == true                             |
+---------+                                                 +---------------------------------------------------------------------------+---------------------------------------------------------------------------+
|T12061233|                                                 |- :cpp:class:`Temperature`                                                 |- :cpp:class:`Temperature`                                                 |
|         |                                                 |- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|- :cpp:func:`Temperature::unit()` == :cpp:enumerator:`Temperature::Unit::C`|
|         |                                                 |- :cpp:func:`Temperature::temperature()`.value() == -20.6                  |- :cpp:func:`Temperature::temperature()`.value() == -23.3                  |
|         |                                                 |- :cpp:func:`Temperature::isFreezing()` == true                            |- :cpp:func:`Temperature::isFreezing()` == true                            |
|         |                                                 |- :cpp:func:`Temperature::isPrecise()` == true                             |- :cpp:func:`Temperature::isPrecise()` == true                             |
+---------+-------------------------------------------------+---------------------------------------------------------------------------+---------------------------------------------------------------------------+


Missing data remarks
^^^^^^^^^^^^^^^^^^^^

Missing data groups ``T MISG`` (temperature data missing) and ``TD MISG`` (dew point data missing) may be specified in remarks.

The table below summarises these groups interpreted by Metaf as :cpp:class:`TemperatureGroup`.

+-------+-------------------------------+------------------------------------------------+------------------------------------------------+
|Group  |:cpp:func:`type()`             |:cpp:func:`airTemperature()`                    |:cpp:func:`dewPoint()`                          |
+=======+===============================+================================================+================================================+
|T MISG |:cpp:enumerator:`Type::T_MISG` |- :cpp:class:`Temperature`                      |- :cpp:class:`Temperature`                      |
+-------+-------------------------------+- :cpp:func:`Temperature::isReported()` == false|- :cpp:func:`Temperature::isReported()` == false|
|TD MISG|:cpp:enumerator:`Type::TD_MISG`|                                                |                                                |
+-------+-------------------------------+------------------------------------------------+------------------------------------------------+



Regional variations
^^^^^^^^^^^^^^^^^^^

High-precision temperature/dew point groups specified in remarks are used only in North America.
