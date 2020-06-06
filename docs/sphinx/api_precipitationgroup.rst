PrecipitationGroup
==================

.. cpp:namespace-push:: metaf

PrecipitationGroup class
------------------------

	.. cpp:class:: PrecipitationGroup

		Stores various information about precipitation, rainfall, snowfall, snow depth, and icing (typically caused by freezing precipitation). This group is used regionally and may be included in METAR report body or in remarks.

.. cpp:namespace-push:: PrecipitationGroup


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Indicates the type of data reported in this group.

		.. cpp:enumerator:: TOTAL_PRECIPITATION_HOURLY

			Water equivalent of all precipitation for last hour.

		.. cpp:enumerator:: SNOW_DEPTH_ON_GROUND

			Actual snow depth on the ground

		.. cpp:enumerator:: FROZEN_PRECIP_3_OR_6_HOURLY

			Water equivalent of frozen precipitation accumulated for last 3 or 6 hours. Used when this kind of precipitation parameter is reported at the time of day that qualifies neither as 3-hourly nor as 6-hourly report.

		.. cpp:enumerator:: FROZEN_PRECIP_3_HOURLY

			Water equivalent of frozen precipitation accumulated for last 3 hours (included in reports near 0300Z, 0900Z, 1500Z, or 2100Z, i.e. 3-hourly reports).

		.. cpp:enumerator:: FROZEN_PRECIP_6_HOURLY

			Water equivalent of frozen precipitation accumulated for last 3 hours (included in reports near 0000Z, 0600Z, 1200Z, or 1800Z, i.e. 6-hourly reports).

		.. cpp:enumerator:: FROZEN_PRECIP_24_HOURLY

			Water equivalent of frozen precipitation accumulated for last 24 hours.

		.. cpp:enumerator:: SNOW_6_HOURLY

			Snow accumulation for the last 6 hours.

		.. cpp:enumerator:: WATER_EQUIV_OF_SNOW_ON_GROUND

			Water equivalent of snow on the ground (including other solid precipitation such as snow grains, ice pellets, ice crystals, hail, etc).

		.. cpp:enumerator:: ICE_ACCRETION_FOR_LAST_HOUR

			Amount of ice accretion during the preceding hour.

		.. cpp:enumerator:: ICE_ACCRETION_FOR_LAST_3_HOURS

			Amount of ice accretion during the last 3 hours.

		.. cpp:enumerator:: ICE_ACCRETION_FOR_LAST_6_HOURS

			Amount of ice accretion during the last 6 hours.

		.. cpp:enumerator:: SNOW_INCREASING_RAPIDLY

			Indicates that snow is increasing rapidly; use :cpp:func:`total()` for total snowfall, and :cpp:func:`recent()` for snow increase during last hour.

		.. cpp:enumerator:: PRECIPITATION_ACCUMULATION_SINCE_LAST_REPORT

			Accumulation of precipitation since previous released weather report.

		.. cpp:enumerator:: RAINFALL_9AM_10MIN

			Amount of rainfall; use :cpp:func:`total()` for rainfall since 9AM (9:00) local time, and :cpp:func:`recent()` for rainfall in the last 10 minutes.

		.. cpp:enumerator:: PNO

			Indicates that automated station is equipped with tipping bucket rain gauge and this sensor is not operating. No further details are provided.

		.. cpp:enumerator:: FZRANO

			Indicates that automated station is equipped with freezing rain sensor and this sensor is not operating. No further details are provided.

		.. cpp:enumerator:: ICG_MISG

			Icing data is missing.

		.. cpp:enumerator:: PCPN_MISG

			Precipitation data is missing.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Type of value reported in this group.

	.. cpp:function:: Precipitation total() const

		:returns: Total amount of precipitation of specified type. May be a non-reported value.

	.. cpp:function:: Precipitation recent() const

		:returns: Amount or increase of precipitation during recent period. Only reported for :cpp:enumerator:`Type::SNOW_INCREASING_RAPIDLY`, is a non-reported value for all other types. 


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: Always returns ``true``.
