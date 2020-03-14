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