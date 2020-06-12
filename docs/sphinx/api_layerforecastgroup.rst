LayerForecastGroup
==================

.. cpp:namespace-push:: metaf

LayerForecastGroup class
------------------------

	.. cpp:class:: LayerForecastGroup

		Stores various information about forecast atmospheric layer (span of heights where certain conditions such as icing or turbulence are forecast). This group may be present in TAFs issued at military aerodromes of NATO countries.

.. cpp:namespace-push:: LayerForecastGroup

Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Provides description of the atmospheric layer reported in this group.

		.. cpp:enumerator:: ICING_TRACE_OR_NONE

			Trace Icing or No Icing.

		.. cpp:enumerator:: ICING_LIGHT_MIXED

			Light Mixed Icing.

		.. cpp:enumerator:: ICING_LIGHT_RIME_IN_CLOUD

			Light Rime Icing In Cloud.

		.. cpp:enumerator:: ICING_LIGHT_CLEAR_IN_PRECIPITATION

			Light Clear Icing In Precipitation.

		.. cpp:enumerator:: ICING_MODERATE_MIXED

			Moderate Mixed Icing.

		.. cpp:enumerator:: ICING_MODERATE_RIME_IN_CLOUD

			Moderate Rime Icing In Cloud.

		.. cpp:enumerator:: ICING_MODERATE_CLEAR_IN_PRECIPITATION

			Moderate Clear Icing In Precipitation.

		.. cpp:enumerator:: ICING_SEVERE_MIXED

			Severe Mixed Icing.

		.. cpp:enumerator:: ICING_SEVERE_RIME_IN_CLOUD

			Severe Rime Icing In Cloud.

		.. cpp:enumerator:: ICING_SEVERE_CLEAR_IN_PRECIPITATION

			Severe Rime Icing In Cloud.

		.. cpp:enumerator:: TURBULENCE_NONE

			No turbulence.

		.. cpp:enumerator:: TURBULENCE_LIGHT

			Light turbulence.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASIONAL

			Moderate turbulence in clear air, occasional.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT

			Moderate turbulence in clear air, frequent.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLOUD_OCCASIONAL

			Moderate turbulence in cloud, occasional.

		.. cpp:enumerator:: TURBULENCE_MODERATE_IN_CLOUD_FREQUENT

			Moderate turbulence in cloud, frequent.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASIONAL

			Severe turbulence in clear air, occasional.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT

			Severe turbulence in clear air, frequent.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLOUD_OCCASIONAL

			Severe turbulence in cloud, occasional.

		.. cpp:enumerator:: TURBULENCE_SEVERE_IN_CLOUD_FREQUENT

			Severe turbulence in cloud, frequent.

		.. cpp:enumerator:: TURBULENCE_EXTREME

			Extreme turbulence.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Type of atmospheric layers reported in this group.

	.. cpp:function:: Distance baseHeight() const

		:returns: Height of atmospheric layer base (bottom range).

	.. cpp:function:: Distance topHeight() const

		:returns: Height of atmospheric layer top (top range).


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: Always returns ``true``.

.. cpp:namespace-pop::


Formats
-------

This group is included in TAF reports and has the following format: character ``6`` (icing information) or ``5`` (turbulence information) followed by single digit or ``X`` character indicating the type of the group, followed by three-digit base height in hundreds of feet, followed by single digit of layer depth in thousands of feet. 

For example, group ``620304`` means light rime icing in cloud, at starting at base height of 3000 feet and extending 4000 feet above base level. 

Metaf does not recognise non-reported height values in this group. For example, groups ``62///4`` or ``62030/`` are not recognised by Metaf.

Generalised syntax
^^^^^^^^^^^^^^^^^^

Generalised syntax of this type of group recognised by Metaf is below. 

.. image:: layerforecastgroup.svg

The type of the group is determined by its first two characters. The first character must be either ``5`` or ``6``, otherwise Metaf will not recognise the group as :cpp:class:`LayerForecastGroup`. The second character must be a digit or ``X``. The valid combinations of first two characters of the group are specified below.

Group type
^^^^^^^^^^

==================== =======================================================================================
First two characters :cpp:enum:`LayerForecastGroup::Type`
==================== =======================================================================================
``50``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_NONE`
``51``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_LIGHT`
``52``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_OCCASIONAL`
``53``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLEAR_AIR_FREQUENT`
``54``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLOUD_OCCASIONAL`
``55``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_MODERATE_IN_CLOUD_FREQUENT`
``56``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_OCCASIONAL`
``57``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLEAR_AIR_FREQUENT`
``58``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLOUD_OCCASIONAL`
``59``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_SEVERE_IN_CLOUD_FREQUENT`
``5X``               :cpp:enumerator:`LayerForecastGroup::Type::TURBULENCE_EXTREME`
``60``               :cpp:enumerator:`LayerForecastGroup::Type::ICING_TRACE_OR_NONE`
``61``               :cpp:enumerator:`LayerForecastGroup::Type::ICING_LIGHT_MIXED`
``62``               :cpp:enumerator:`LayerForecastGroup::Type::ICING_LIGHT_RIME_IN_CLOUD`
``63``               :cpp:enumerator:`LayerForecastGroup::Type::ICING_LIGHT_CLEAR_IN_PRECIPITATION`
``64``               :cpp:enumerator:`LayerForecastGroup::Type::ICING_MODERATE_MIXED`
``65``               :cpp:enumerator:`LayerForecastGroup::Type::ICING_MODERATE_RIME_IN_CLOUD`
``66``               :cpp:enumerator:`LayerForecastGroup::Type::ICING_MODERATE_CLEAR_IN_PRECIPITATION`
``67``               :cpp:enumerator:`LayerForecastGroup::Type::ICING_SEVERE_MIXED`
``68``               :cpp:enumerator:`LayerForecastGroup::Type::ICING_SEVERE_RIME_IN_CLOUD`
``69``               :cpp:enumerator:`LayerForecastGroup::Type::ICING_SEVERE_CLEAR_IN_PRECIPITATION`
==================== =======================================================================================


Example
^^^^^^^

The example below demonstrates the atmospheric layer forecast group recognised by Metaf.

Group ``620304`` is parsed by Metaf as :cpp:class:`LayerForecastGroup`. 

Method :cpp:func:`LayerForecastGroup::type()` returns :cpp:enumerator:`ICING_LIGHT_RIME_IN_CLOUD`.

Method :cpp:func:`LayerForecastGroup::baseHeight()` returns instance of :cpp:class:`Distance` with the following content.

		- :cpp:func:`Distance::modifier()` returns :cpp:enumerator:`Distance::Modifier::NONE`.
		- :cpp:func:`Distance::distance()` returns 3000.
		- :cpp:func:`Distance::unit()` returns :cpp:enumerator:`Distance::Unit::FEET`.

Metaf adds layer depth to the base height to produce the layer's top height. Method :cpp:func:`LayerForecastGroup::topHeight()` returns instance of :cpp:class:`Distance` with the following content.

		- :cpp:func:`Distance::modifier()` returns :cpp:enumerator:`Distance::Modifier::NONE`.
		- :cpp:func:`Distance::distance()` returns 7000.
		- :cpp:func:`Distance::unit()` returns :cpp:enumerator:`Distance::Unit::FEET`.

Regional variations
^^^^^^^^^^^^^^^^^^^

This group is used only in TAF reports issued at aerodromes operated by USAF or USMC.
