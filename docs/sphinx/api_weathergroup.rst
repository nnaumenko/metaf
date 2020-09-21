WeatherGroup
============

.. cpp:namespace-push:: metaf

WeatherGroup class
------------------

	.. cpp:class:: WeatherGroup

		Stores information about recent or current weather phenomena, or on the beginning or ending time of weather phenomena.

		.. note:: If more than 20 beginning/ending times are specified in the group, only first 20 are stored.

.. cpp:namespace-push:: WeatherGroup


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		What kind weather information this group represents.

		.. cpp:enumerator::	CURRENT

			Current weather information is stored in this group.

		.. cpp:enumerator:: RECENT

			Recent weather information is stored in this group.

		.. cpp:enumerator:: EVENT

			Time of beginning or ending of weather phenomena is stored in this group.

		.. cpp:enumerator:: NSW

			Nil significant weather.

			This group is only used in trends and indicates the end of a significant weather phenomena. No further information is provided.

		.. cpp:enumerator:: PWINO

			Indicates that automated station is equipped with present weather identifier and this sensor is not operating. No further information is provided.

		.. cpp:enumerator:: TSNO

			Indicates that automated station is equipped with lightning detector and this sensor is not operating. No further information is provided.

		.. cpp:enumerator:: WX_MISG

			Weather phenomena data is missing. No further information is provided.

		.. cpp:enumerator:: TS_LTNG_TEMPO_UNAVBL

			Thunderstorm/lightning data is missing. No further information is provided.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: What kind of weather information stored in this group; current weather, recent weather, weather event (beginning and ending times of weather phenomena).

	.. cpp:function:: std::vector<WeatherPhenomena> weatherPhenomena() const

		:returns: The vector or weather phenomena; each :cpp:class:`metaf::WeatherPhenomena` includes qualifier, descriptor and weather phenomena reported in this group.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if all weather phenomena included in this group are valid; ``false`` otherwise.

			Invalid weather phenomena (or invalid combinations or descriptor / qualifier / phenomena) may be not recognised as a WeatherGroup.

			For ``WX`` not followed by ``MISG`` and for ``TS/LTNG`` not followed by ``TEMPO`` and ``UNAVBL`` returns ``false``.

.. cpp:namespace-pop::


Formats
-------

Sections below focus on how exactly different weather groups are parsed by Metaf and which values are returned by getter methods of :cpp:class:`WeatherGroup`.

See also Weather Phenomena :ref:`api_weatherphenomena:Formats`.

Current and forecast weather
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Current weather phenomena may be included in the METAR report body. Forecast weather phenomena, included in the METAR report trends and TAF reports have exactly the same format.


Obscurations and other phenomena
""""""""""""""""""""""""""""""""

This section focuses on the weather phenomena which fall into 'obscuration' and 'other' categories.

The following weather groups with vicinity qualifier are recognised by Metaf.

Metaf does not recognise any other combination of the weather phenomena with vicinity qualifier VC.

+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+
|Group |Meaning                  |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                                 |
+======+=========================+===============================+===============================================================================================================+
|TS    |Thunderstorm             |:cpp:enumerator:`Type::CURRENT`|- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |(without precipitation)  |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::THUNDERSTORM`   |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 0                                                                                                  |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|//    |Not reported weather     |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::NOT_REPORTED`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|MIFG  |Shallow fog              |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::SHALLOW`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::FOG`                                                    |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|PRFG  |Partial fog              |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::PARTIAL`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::FOG`                                                    |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|BCFG  |Patches of fog           |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::PATCHES`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::FOG`                                                    |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|FZFG  |(Potentially)            |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |freezing fog             |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::FREEZING`       |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::FOG`                                                    |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|FG    |Fog                      |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::FOG`                                                    |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|DRDU  |Low drifting dust        |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::LOW_DRIFTING`   |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUST`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|BLDU  |Blowind dust             |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::BLOWING`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUST`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|DU    |Widespread dust          |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUST`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|DRSA  |Low drifting sand        |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::LOW_DRIFTING`   |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SAND`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|BLSA  |Blowind sand             |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::BLOWING`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SAND`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`  MetafTime`>                                            |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|SA    |Sand                     |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SAND`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|DRSN  |Low drifting snow        |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::LOW_DRIFTING`   |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|BLSN  |Blowind snow             |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::BLOWING`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|BLPY  |Blowind spray            |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::BLOWING`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SPRAY`                                                  |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|BR    |Mist                     |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::MIST`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|FU    |Smoke                    |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SMOKE`                                                  |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VA    |Volcanic ash             |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::VOLCANIC_ASH`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|HZ    |Haze                     |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::HAZE`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|PO    |Well-developed dust or   |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |sand whirls              |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUST_WHIRLS`                                            |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|SQ    |Squalls                  |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SQUALLS`                                                |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|FC    |Funnel cloud             |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::FUNNEL_CLOUD`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+FC   |Tornado or waterspout    |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::FUNNEL_CLOUD`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|DS    |Duststorm                |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUSTSTORM`                                              |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+DS   |Heavy duststorm          |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUSTSTORM`                                              |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|SS    |Sandstorm                |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SANDSTORM`                                              |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+SS   |Heavy sandstorm          |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SANDSTORM`                                              |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|DSSS  |Dust- and sandstorm      |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 2                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUSTSTORM`                                              |
|      |                         |                               |- at(1) == :cpp:enumerator:`WeatherPhenomena::Weather::SANDSTORM`                                              |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+                         |                               +---------------------------------------------------------------------------------------------------------------+
|SSDS  |                         |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`             |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 2                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SANDSTORM`                                              |
|      |                         |                               |- at(1) == :cpp:enumerator:`WeatherPhenomena::Weather::DUSTSTORM`                                              |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+DSSS |Heavy dust- and sandstorm|                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 2                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUSTSTORM`                                              |
|      |                         |                               |- at(1) == :cpp:enumerator:`WeatherPhenomena::Weather::SANDSTORM`                                              |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+                         |                               +---------------------------------------------------------------------------------------------------------------+
|+SSDS |                         |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 2                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SANDSTORM`                                              |
|      |                         |                               |- at(1) == :cpp:enumerator:`WeatherPhenomena::Weather::DUSTSTORM`                                              |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+

.. note:: Though snow (coded ``SN``) is in the precipitation category, low drifting snow (coded ``DRSN``) and blowing snow (coded ``BLSN``) are considered obscurations.

.. note:: Though ice crystals (coded ``IC``) is technically a precipitation, it is used similarly to obscurations.


Weather phenomena in vicinity
"""""""""""""""""""""""""""""

This section focuses on the weather phenomena in vicinity of the station, which means that these phenomena occur within 10 statute miles from the weather station rather than at the station's location.

The following weather groups with vicinity qualifier are recognised by Metaf.

Metaf does not recognise any other combination of the weather phenomena with vicinity qualifier VC.

+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+
|Group |Meaning                  |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                                 |
+======+=========================+===============================+===============================================================================================================+
|VCTS  |Thunderstorm in vicinity |:cpp:enumerator:`Type::CURRENT`|- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::THUNDERSTORM`   |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 0                                                                                                  |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VCSH  |Precipitation in vicinity|                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::SHOWERS`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 0                                                                                                  |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VCFG  |Fog in vicinity          |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::FOG`                                                    |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VCPO  |Dust of sand whirls      |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |in vicinity              |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUST_WHIRLS`                                            |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VCVA  |Volcanic ash in vicinity |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::VOLCANIC_ASH`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VCFC  |Funnel cloud in vicinity |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::FUNNEL_CLOUD`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VCBLDU|Blowing dust in vicinity |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::BLOWING`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUST`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VCBLSA|Blowing sand in vicinity |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::BLOWING`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SAND`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VCBLSN|Blowing snow in vicinity |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::BLOWING`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VCDS  |Duststorm in vicinity    |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DUSTSTORM`                                              |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|VCSS  |Sandstorm in vicinity    |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SANDSTORM`                                              |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+

Single type of precipitation
""""""""""""""""""""""""""""

This section focuses on the precipitation groups which specify single type of precipitation.

+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+
|Group |Meaning                  |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                                 |
+======+=========================+===============================+===============================================================================================================+
|DZ    |Drizzle                  |:cpp:enumerator:`Type::CURRENT`|- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DRIZZLE`                                                |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|RA    |Rain                     |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|SN    |Snow                     |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|PL    |Ice pellets              |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::ICE_PELLETS`                                            |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|SG    |Snow grains              |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW_GRAINS`                                            |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|GR    |Hail                     |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::HAIL`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|GS    |Small hail               |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SMALL_HAIL`                                             |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|UP    |Undetermined             |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |precipitation            |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::UNDETERMINED`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+

.. note:: For precipitation group without explicit qualifier Metaf uses :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE` instead :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`.

Any of these precipitation types can be supplemented with + or - intensity qualifier, as shown in examples below.

+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+
|Group |Meaning                  |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                                 |
+======+=========================+===============================+===============================================================================================================+
|-DZ   |Light drizzle            |:cpp:enumerator:`Type::CURRENT`|- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::LIGHT`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DRIZZLE`                                                |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+SN   |Heavy snow               |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+UP   |Heavy                    |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |undetermined             |                               |- size() == 1                                                                                                  |
|      |precipitation            |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`           |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::UNDETERMINED`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+

Rain, drizzle and undetermined precipitation can be augmented with descriptor 'freezing', optionally with additional qualifier, as shown in examples below.

+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+
|Group |Meaning                  |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                                 |
+======+=========================+===============================+===============================================================================================================+
|-FZDZ |Light freezing drizzle   |:cpp:enumerator:`Type::CURRENT`|- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::LIGHT`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::FREEZING`       |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DRIZZLE`                                                |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|FZRA  |Freezing rain            |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::FREEZING`       |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+FZUP |Heavy freezing           |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |undetermined             |                               |- size() == 1                                                                                                  |
|      |precipitation            |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::FREEZING`       |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::UNDETERMINED`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+

Rain, snow, ice pellets, hail, small hain and undetermined precipitation can be augmented with descriptor 'showery', optionally with additional qualifier, as shown in examples below.

+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+
|Group |Meaning                  |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                                 |
+======+=========================+===============================+===============================================================================================================+
|-SHPL |Light ice pellet showers |:cpp:enumerator:`Type::CURRENT`|- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::LIGHT`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::SHOWERS`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::ICE_PELLETS`                                            |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|SHRA  |Showery rain             |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::SHOWERS`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+SHUP |Heavy showery            |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |undetermined             |                               |- size() == 1                                                                                                  |
|      |precipitation            |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::SHOWERS`        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::UNDETERMINED`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+

Any precipitation type can be augmented with descriptor 'thunderstorm', optionally with additional qualifier as shown in examples below.

+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+
|Group |Meaning                  |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                                 |
+======+=========================+===============================+===============================================================================================================+
|-TSDZ |Thunderstorm with        |:cpp:enumerator:`Type::CURRENT`|- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |light drizzle            |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::LIGHT`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::THUNDERSTORM`   |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DRIZZLE`                                                |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|TSRA  |Thunderstorm with rain   |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::THUNDERSTORM`   |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                                   |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+TSUP |Heavy                    |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|      |undetermined             |                               |- size() == 1                                                                                                  |
|      |precipitation with       |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|      |thunderstorm             |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::THUNDERSTORM`   |
|      |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|      |                         |                               |- size() == 1                                                                                                  |
|      |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::UNDETERMINED`                                           |
|      |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|      |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+


Mixed precipitation
"""""""""""""""""""

Mixed precipitation can be indicated in a single group, up to three precipitation types per group.

Mixed precipitation groups where precipitation types are repeated are not recognised by Metaf as weather groups, for example, group ``RASNRA`` is not recognised by Metaf because rain (``RA``) is specified more than once.

Any mixed precipitation can be supplemented with - or + intensity qualifier.

Descriptor 'Freezing' can be used with mixed precipitation which contains any of the following precipitation types: 'Rain', 'Drizzle', 'Undetermined precipitation'.

Descriptor 'Showery' can be used with mixed precipitation which contains any of the following precipitation types: 'Rain', 'Snow', 'Ice Pellets', 'Hail', 'Small hail', 'Undetermined precipitation'.

Descriptor 'Thunderstorm' can be used with any mixed precipitation.

Mixed precipitation examples are shown in the table below.

+-------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+
|Group  |Meaning                  |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                                 |
+=======+=========================+===============================+===============================================================================================================+
|-FZDZPL|Thunderstorm with        |:cpp:enumerator:`Type::CURRENT`|- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|       |light drizzle and ice    |                               |- size() == 1                                                                                                  |
|       |pellets mix              |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::LIGHT`            |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::FREEZING`       |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|       |                         |                               |- size() == 2                                                                                                  |
|       |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DRIZZLE`                                                |
|       |                         |                               |- at(1) == :cpp:enumerator:`WeatherPhenomena::Weather::ICE_PELLETS`                                            |
|       |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|       |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+-------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+TSRAGR|Thunderstorm with heavy  |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|       |rain and hail mix        |                               |- size() == 1                                                                                                  |
|       |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::THUNDERSTORM`   |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|       |                         |                               |- size() == 2                                                                                                  |
|       |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                                   |
|       |                         |                               |- at(1) == :cpp:enumerator:`WeatherPhenomena::Weather::HAIL`                                                   |
|       |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|       |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+-------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|RASNPL |Rain, snow and ice       |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|       |pellets mix              |                               |- size() == 1                                                                                                  |
|       |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE`         |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::THUNDERSTORM`   |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|       |                         |                               |- size() == 3                                                                                                  |
|       |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                                   |
|       |                         |                               |- at(1) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                                   |
|       |                         |                               |- at(2) == :cpp:enumerator:`WeatherPhenomena::Weather::ICE_PELLETS`                                            |
|       |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|       |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+-------+-------------------------+                               +---------------------------------------------------------------------------------------------------------------+
|+SHRASN|Heavy showery rain and   |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                   |
|       |snow mix                 |                               |- size() == 1                                                                                                  |
|       |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                        |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`            |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::SHOWERS`        |
|       |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                  |
|       |                         |                               |- size() == 2                                                                                                  |
|       |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                                   |
|       |                         |                               |- at(1) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                                   |
|       |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                       |
|       |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                              |
+-------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------+

Recent precipitation in METAR report body
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Recent precipitation is coded with qualifier RE and up to three precipitation types (DZ, RA, SN, PL, GS, GR, GS, UP).

Descriptors 'Freezing', 'Showery' or 'Thunderstorm' may be used in accordance with rules specified in sections above.

Examples of recent precipitation are shown in table below.

+--------+-------------------------+-------------------------------+----------------------------------------------------------------------------------------------------------------+
|Group   |Meaning                  |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                                  |
+========+=========================+===============================+================================================================================================================+
|RERA    |Recent weather: rain     |:cpp:enumerator:`Type::RECENT` |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                    |
|        |                         |                               |- size() == 1                                                                                                   |
|        |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                         |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::RECENT`            |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`            |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                   |
|        |                         |                               |- size() == 1                                                                                                   |
|        |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                                    |
|        |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                        |
|        |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                               |
+--------+-------------------------+                               +----------------------------------------------------------------------------------------------------------------+
|RESHSN  |Recent weather: snow     |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                    |
|        |showers                  |                               |- size() == 1                                                                                                   |
|        |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                         |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::RECENT`            |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::SHOWERS`         |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                   |
|        |                         |                               |- size() == 2                                                                                                   |
|        |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                                    |
|        |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                        |
|        |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                               |
+--------+-------------------------+                               +----------------------------------------------------------------------------------------------------------------+
|REFZDZ  |Recent weather: freezing |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                    |
|        |drizzle                  |                               |- size() == 1                                                                                                   |
|        |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                         |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::RECENT`            |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::FREEZING`        |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                   |
|        |                         |                               |- size() == 1                                                                                                   |
|        |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::DRIZZLE`                                                 |
|        |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                        |
|        |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                               |
+--------+-------------------------+                               +----------------------------------------------------------------------------------------------------------------+
|RERASN  |Recent weather: rain and |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                    |
|        |snow mix                 |                               |- size() == 1                                                                                                   |
|        |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                         |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::RECENT`            |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`            |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                   |
|        |                         |                               |- size() == 2                                                                                                   |
|        |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                                    |
|        |                         |                               |- at(1) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                                    |
|        |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                        |
|        |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                               |
+--------+-------------------------+                               +----------------------------------------------------------------------------------------------------------------+
|RETSRAGR|Recent weather:          |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                    |
|        |thunderstorm with rain   |                               |- size() == 1                                                                                                   |
|        |and hail                 |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                         |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::RECENT`            |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::THUNDERSTORM`    |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                   |
|        |                         |                               |- size() == 2                                                                                                   |
|        |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                                    |
|        |                         |                               |- at(1) == :cpp:enumerator:`WeatherPhenomena::Weather::HAIL`                                                    |
|        |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                        |
|        |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                               |
+--------+-------------------------+                               +----------------------------------------------------------------------------------------------------------------+
|RE//    |Recent weather: not      |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                                    |
|        |not reported             |                               |- size() == 1                                                                                                   |
|        |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                         |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::RECENT`            |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`            |
|        |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>                   |
|        |                         |                               |- size() == 1                                                                                                   |
|        |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::NOT_REPORTED`                                            |
|        |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                        |
|        |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                               |
+--------+-------------------------+-------------------------------+----------------------------------------------------------------------------------------------------------------+

Sequence of weather groups
""""""""""""""""""""""""""

If sequence of two or more than one weather group is included in the METAR report, the entire sequence is represented by a single :cpp:class:`WeatherGroup`, as shown in examples below.

+----------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------+
|Group     |Meaning                  |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                           |
+==========+=========================+===============================+=========================================================================================================+
|+RA BR SQ |Heavy rain, mist, squalls|:cpp:enumerator:`Type::CURRENT`|- std::vector<:cpp:class:`WeatherPhenomena`>                                                             |
|          |                         |                               |- size() == 3                                                                                            |
|          |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`      |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|          |                         |                               |- size() == 1                                                                                            |
|          |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                             |
|          |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                 |
|          |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
|          |                         |                               |- at(1) : :cpp:class:`WeatherPhenomena`                                                                  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|          |                         |                               |- size() == 1                                                                                            |
|          |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::MIST`                                             |
|          |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                 |
|          |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
|          |                         |                               |- at(2) : :cpp:class:`WeatherPhenomena`                                                                  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|          |                         |                               |- size() == 1                                                                                            |
|          |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SQUALLS`                                          |
|          |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                 |
|          |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
+----------+-------------------------+                               +---------------------------------------------------------------------------------------------------------+
|+SHSN BLSN|Heavy showery snow,      |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                             |
|          |blowing snow             |                               |- size() == 1                                                                                            |
|          |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`      |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::SHOWERS`  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|          |                         |                               |- size() == 2                                                                                            |
|          |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                             |
|          |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                 |
|          |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
|          |                         |                               |- at(1) : :cpp:class:`WeatherPhenomena`                                                                  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::BLOWING`  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|          |                         |                               |- size() == 2                                                                                            |
|          |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                             |
|          |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                 |
|          |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
+----------+-------------------------+                               +---------------------------------------------------------------------------------------------------------+
|IC BR BLSN|Ice crystals, mist,      |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                             |
|          |blowing snow             |                               |- size() == 3                                                                                            |
|          |                         |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|          |                         |                               |- size() == 1                                                                                            |
|          |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::ICE_CRYSTALS`                                     |
|          |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                 |
|          |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
|          |                         |                               |- at(1) : :cpp:class:`WeatherPhenomena`                                                                  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|          |                         |                               |- size() == 1                                                                                            |
|          |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::MIST`                                             |
|          |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                 |
|          |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
|          |                         |                               |- at(2) : :cpp:class:`WeatherPhenomena`                                                                  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::BLOWING`  |
|          |                         |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|          |                         |                               |- size() == 1                                                                                            |
|          |                         |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::BLOWING`                                          |
|          |                         |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::NONE`                                 |
|          |                         |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
+----------+-------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------+


Weather events in METAR remarks
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Weather event beginning and ending time may be included in the remarks.

Weather event consists of weather phenomena (as described above), beginning or ending indicator (character ``B`` or ``E``), and event time. The time format is hour and minute.

For example, group ``RAB2328`` means that rain has began at 23:28, and group ``TSRAGSE0743`` means that thunderstorm with rain and graupel ended on 07:43.

If the weather event beginning or ending occurred on the same hour when the report was released, only minutes are specified, for example `RAE52` means that rain ended at 52th minute of the hour when the report was released.

Several weather events can be specified in single group. If the same weather event begins and ends more than one time, only ``B`` or ``E`` character and event time are specified for the subsequent events. For example group ``RAB0835E0848DZB03`` means that rain began at 08:35, ended at 08:48, and drizzle began on 03 minutes past hour when the report was released.

.. note:: Maximum of ten weather events can be are specified. If more than ten events are specified in a single remark group, only first ten are stored in the instance of :cpp:class:`WeatherGroup`.

The examples of the weather event groups are in the table below.

.. note:: Some of the groups refer to hour value from the report release time. In the examples below assume that report release time is: day-of-month 26, hour 15, minute 56 (coded as ``261556Z`` in the METAR report). Note that hour 15 from report time is used where hour is not specified.

+----------------------+--------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------+
|Group                 |Meaning                   |:cpp:func:`type()`             |:cpp:func:`weatherPhenomena()`                                                                           |
+======================+==========================+===============================+=========================================================================================================+
|UPB1458E13RAB13SNE1458|Undetermined precipitation|:cpp:enumerator:`Type::EVENT`  |- std::vector<:cpp:class:`WeatherPhenomena`>                                                             |
|                      |began at 14:58, ended     |                               |- size() == 4                                                                                            |
|                      |at 15:13, rain began at   |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                  |
|                      |15:13, snow ended at      |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|                      |14:58 (hour 15 is from    |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|                      |report release time)      |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|                      |                          |                               |- size() == 1                                                                                            |
|                      |                          |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::UNDETERMINED`                                     |
|                      |                          |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::BEGINNING`                            |
|                      |                          |                               |- :cpp:func:`time()`: std::optional<:cpp:class:`MetafTime`>                                              |
|                      |                          |                               |- :cpp:func:`MetafTime::day()`.has_value() == false                                                      |
|                      |                          |                               |- :cpp:func:`MetafTime::hour()` == 14                                                                    |
|                      |                          |                               |- :cpp:func:`MetafTime::minute()` == 58                                                                  |
|                      |                          |                               |- at(1) : :cpp:class:`WeatherPhenomena`                                                                  |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|                      |                          |                               |- size() == 1                                                                                            |
|                      |                          |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::UNDETERMINED`                                     |
|                      |                          |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::ENDING`                               |
|                      |                          |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
|                      |                          |                               |- :cpp:func:`MetafTime::day()`.has_value() == false                                                      |
|                      |                          |                               |- :cpp:func:`MetafTime::hour()` == 15 (from report time)                                                 |
|                      |                          |                               |- :cpp:func:`MetafTime::minute()` == 13                                                                  |
|                      |                          |                               |- at(2) : :cpp:class:`WeatherPhenomena`                                                                  |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|                      |                          |                               |- size() == 1                                                                                            |
|                      |                          |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                             |
|                      |                          |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::BEGINNING`                            |
|                      |                          |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
|                      |                          |                               |- :cpp:func:`MetafTime::day()`.has_value() == false                                                      |
|                      |                          |                               |- :cpp:func:`MetafTime::hour()` == 15 (from report time)                                                 |
|                      |                          |                               |- :cpp:func:`MetafTime::minute()` == 13                                                                  |
|                      |                          |                               |- at(3) : :cpp:class:`WeatherPhenomena`                                                                  |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|                      |                          |                               |- size() == 1                                                                                            |
|                      |                          |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`                                             |
|                      |                          |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::ENDING`                               |
|                      |                          |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
|                      |                          |                               |- :cpp:func:`MetafTime::day()`.has_value() == false                                                      |
|                      |                          |                               |- :cpp:func:`MetafTime::hour()` == 14                                                                    |
|                      |                          |                               |- :cpp:func:`MetafTime::minute()` == 58                                                                  |
+----------------------+--------------------------+                               +---------------------------------------------------------------------------------------------------------+
|RAB1458GRB1457E25     |Rain began at 14:58, hail |                               |- std::vector<:cpp:class:`WeatherPhenomena`>                                                             |
|                      |began at 14:57, ended     |                               |- size() == 3                                                                                            |
|                      |at 15:25 (hour 15 is from |                               |- at(0) : :cpp:class:`WeatherPhenomena`                                                                  |
|                      |report release time)      |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|                      |                          |                               |- size() == 1                                                                                            |
|                      |                          |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`                                             |
|                      |                          |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::BEGINNING`                            |
|                      |                          |                               |- :cpp:func:`time()`: std::optional<:cpp:class:`MetafTime`>                                              |
|                      |                          |                               |- :cpp:func:`MetafTime::day()`.has_value() == false                                                      |
|                      |                          |                               |- :cpp:func:`MetafTime::hour()` == 14                                                                    |
|                      |                          |                               |- :cpp:func:`MetafTime::minute()` == 58                                                                  |
|                      |                          |                               |- at(1) : :cpp:class:`WeatherPhenomena`                                                                  |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|                      |                          |                               |- size() == 1                                                                                            |
|                      |                          |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::HAIL`                                             |
|                      |                          |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::BEGINNING`                            |
|                      |                          |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
|                      |                          |                               |- :cpp:func:`MetafTime::day()`.has_value() == false                                                      |
|                      |                          |                               |- :cpp:func:`MetafTime::hour()` == 14                                                                    |
|                      |                          |                               |- :cpp:func:`MetafTime::minute()` == 57                                                                  |
|                      |                          |                               |- at(2) : :cpp:class:`WeatherPhenomena`                                                                  |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::qualifier()` == :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`       |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::descriptor()` == :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`     |
|                      |                          |                               |- :cpp:func:`WeatherPhenomena::weather()`: std::vector<:cpp:enum:`WeatherPhenomena::Weather`>            |
|                      |                          |                               |- size() == 1                                                                                            |
|                      |                          |                               |- at(0) == :cpp:enumerator:`WeatherPhenomena::Weather::HAIL`                                             |
|                      |                          |                               |- :cpp:func:`event()` == :cpp:enumerator:`WeatherPhenomena::Event::ENDING`                               |
|                      |                          |                               |- :cpp:func:`time()`: empty std::optional<:cpp:class:`MetafTime`>                                        |
|                      |                          |                               |- :cpp:func:`MetafTime::day()`.has_value() == false                                                      |
|                      |                          |                               |- :cpp:func:`MetafTime::hour()` == 15 (from report time)                                                 |
|                      |                          |                               |- :cpp:func:`MetafTime::minute()` == 25                                                                  |
+----------------------+--------------------------+-------------------------------+---------------------------------------------------------------------------------------------------------+

Nil significant weather, missing data and non-operating devices
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Group ``NSW`` (nil significant weather) indicates ending of previously reported weather phenomena.

Group ``WX MISG`` included in remarks indicates missing weather data and group sequense ``TS/LTNG TEMPO UNAVBL`` indicates missing lightning/thunderstorm data.

Groups ``TSNO`` included in remarks indicates that automated station is equipped with lightning detector and this detector is not operating.

Group ``PWINO`` included in remarks indicates that automated station is equipped with present weather identifier and this identifier is not operating.

The table below summarises parsing result for the miscellaneous weather groups recognised by Metaf. The table shows values returned by getter methods of :cpp:class:`WeatherGroup`.

+--------------------+--------------------------------------------+---------------------------------------------+
|Group               |:cpp:func:`type()`                          |:cpp:func:`weatherPhenomena()`               |
+====================+============================================+=============================================+
|NSW                 |:cpp:enumerator:`Type::NSW`                 |- std::vector<:cpp:class:`WeatherPhenomena`> |
+--------------------+--------------------------------------------+- size() == 0                                |
|WX MISG             |:cpp:enumerator:`Type::WX_MISG`             |                                             |
+--------------------+--------------------------------------------+                                             |
|TSNO                |:cpp:enumerator:`Type::TSNO`                |                                             |
+--------------------+--------------------------------------------+                                             |
|PWINO               |:cpp:enumerator:`Type::PWINO`               |                                             |
+--------------------+--------------------------------------------+                                             |
|TS/LTNG TEMPO UNAVBL|:cpp:enumerator:`Type::TS_LTNG_TEMPO_UNAVBL`|                                             |
+--------------------+--------------------------------------------+---------------------------------------------+


Regional variations
^^^^^^^^^^^^^^^^^^^

Weather event groups specified in remarks (for example ``SNB2247E2255DZB2258``) are used only in North America or on air bases of NATO countries.
