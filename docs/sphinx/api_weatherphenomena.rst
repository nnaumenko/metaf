WeatherPhenomena
================

.. cpp:namespace-push:: metaf

Weatherphenomena class
----------------------

	.. cpp:class:: WeatherPhenomena

		Specifies weather phenomena, such as obscuration, precipitation, etc., along with intensity, proximity, timing, descriptor, event beginning or enging and event occurrence time.

.. cpp:namespace-push:: WeatherPhenomena

Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Qualifier

		Intensity, proximity or time of observation. 

		.. cpp:enumerator:: NONE

			No qualifier. This group reports current weather observed at location.

		.. cpp:enumerator:: RECENT

			This group reports recent weather rather than current weather.

		.. cpp:enumerator:: VICINITY

			This group reports weather in vicinity rather than on site.

		.. cpp:enumerator:: LIGHT

			Light intensity.

		.. cpp:enumerator:: MODERATE

			Moderate intensity. This qualier is used with precipitation only.

		.. cpp:enumerator:: HEAVY

			Heavy intensity.


	.. cpp:enum-class:: Descriptor

		Additional properties of weather phenomena.

		.. cpp:enumerator:: NONE

			No additional descriptor for weather phenomena specified.

		.. cpp:enumerator:: SHALLOW

			This descriptor is only be used to further describe fog that has little vertical extent (less than 6 feet), i.e. ground fog.

		.. cpp:enumerator:: PARTIAL

			This descriptors is only be used to further describe fog that has little vertical extent (normally greater than or equal to 6 feet but less than 20 feet), and reduces horizontal visibility, but to a lesser extent vertically. The stars may often be seen by night and the sun by day. The fog is covering only the part of the aerodrome.

		.. cpp:enumerator:: PATCHES

			This descriptors is only be used to further describe fog that has little vertical extent (normally greater than or equal to 6 feet but less than 20 feet), and reduces horizontal visibility, but to a lesser extent vertically. The stars may often be seen by night and the sun by day. The fog consists of patches randomly covering the aerodrome.

		.. cpp:enumerator:: LOW_DRIFTING

			When dust, sand, or snow is raised by the wind to less than 6 feet, "low drifting" shall be used to further describe the weather phenomenon.

		.. cpp:enumerator:: BLOWING

			When dust, sand, snow, or spray is raised by the wind to a height of 6 feet or more, "blowing" shall be used to further describe the weather phenomenon.

		.. cpp:enumerator:: SHOWERS

			Precipitation characterized by the suddenness with which they start and stop, by the rapid changes of intensity, and usually by rapid changes in the appearance of the sky.

		.. cpp:enumerator:: THUNDERSTORM

			A local storm produced by a cumulonimbus cloud that is accompanied by lightning and/or thunder. Thunderstorm may be reported without any accompanying precipitation.

		.. cpp:enumerator:: FREEZING

			When fog is occurring and the temperature is below 0°C, this descriptor is used to further describe the phenomena.

			..note:: The fog is described as 'freezing' at freezing temperatures, regardless of whether is deposits the rime.

			When drizzle and/or rain freezes upon impact and forms a glaze on the ground or other exposed objects, this descriptor is used to further describe the precipitation.


	.. cpp:enum-class:: Weather

		Describes precipitation, obscuration and other weather phenomena.

		.. cpp:enumerator:: NOT_REPORTED

			An automatic observing system is used and the present weather cannot be
			observed.

		.. cpp:enumerator:: DRIZZLE

			Fairly uniform precipitation composed exclusively of fine drops with diameters of less than 0.02 inch (0.5 mm) very close together. Drizzle appears to float while following air currents, although unlike fog droplets, it falls to the ground.

		.. cpp:enumerator:: RAIN

			Precipitation, either in the form of drops larger than 0.02 inch (0.5 mm), or smaller drops which, in contrast to drizzle, are widely separated.

		.. cpp:enumerator:: SNOW

			Precipitation of snow crystals, mostly branched in the form of six-pointed stars.

		.. cpp:enumerator:: SNOW_GRAINS

			Precipitation of very small, white, and opaque grains of ice.

		.. cpp:enumerator:: ICE_CRYSTALS

			A fall of unbranched (snow crystals are branched) ice crystals in the form of needles, columns, or plates.

		.. cpp:enumerator:: ICE_PELLETS

			Precipitation of transparent or translucent pellets of ice, which are round or irregular, rarely conical, and which have a diameter of 0.2 inch (5 mm), or less. There are two main types:

				#. Hard grains of ice consisting of frozen raindrops, or largely melted and refrozen snowflakes.
				
				#. Pellets of snow encased in a thin layer of ice which have formed from the freezing, either of droplets intercepted by the pellets, or of water resulting from the partial melting of the pellets.

		.. cpp:enumerator:: HAIL

			Precipitation in the form of small balls or other pieces of ice falling separately or frozen together in irregular lumps.

		.. cpp:enumerator:: SMALL_HAIL

			Precipitation of white, opaque grains of ice. The grains are round or sometimes conical. Diameters range from about 0.08 to 0.2 inch (2 to 5 mm).

			Small hail is also called 'snow pellets' or 'graupel'.

		.. cpp:enumerator:: UNDETERMINED

			Precipitation type that is reported if the automated station detects the occurrence of precipitation but the precipitation discriminator cannot recognize the type.

		.. cpp:enumerator:: MIST

			A visible aggregate of minute water particles suspended in the atmosphere that reduces visibility to less than 7 statute miles but greater than or equal to 5/8 statute miles. Essentially the same as fog but less dense.

		.. cpp:enumerator:: FOG

			A visible aggregate of minute water particles (droplets) which are based at the Earth's surface and reduces horizontal visibility to less than 5/8 statute mile and, unlike drizzle, it does not fall to the ground.

		.. cpp:enumerator:: SMOKE

			A suspension in the air of small particles produced by combustion. A transition to haze may occur when smoke particles have traveled great distances (25 to 100 miles or more) and when the larger particles have settled out and the remaining particles have become widely scattered through the atmosphere.

		.. cpp:enumerator:: VOLCANIC_ASH

			Fine particles of rock powder that originate from a volcano and that may remain suspended in the atmosphere for long periods.

		.. cpp:enumerator:: DUST

			Widespread dust. Fine particles of earth or other matter raised or suspended in the air by the wind that may have occurred at or far away from the station which may restrict horizontal visibility.

		.. cpp:enumerator:: SAND

			Sand particles raised by the wind to a height sufficient to reduce horizontal visibility.

		.. cpp:enumerator:: HAZE

			A suspension in the air of extremely small, dry particles invisible to the naked eye and sufficiently numerous to give the air an opalescent appearance.

		.. cpp:enumerator:: SPRAY

			An ensemble of water droplets torn by the wind from the surface of an extensive body of water, generally from the crests of waves, and carried up a short distance into the air.

		.. cpp:enumerator:: DUST_WHIRLS

			Well-developed Dust/Sand Whirl. An ensemble of particles of dust or sand, sometimes accompanied by small litter, raised from the ground in the form of a whirling column of varying height with a small diameter and an approximately vertical axis.

		.. cpp:enumerator:: SQUALLS

			A strong wind characterized by a sudden onset in which the wind speed increases at least 16 knots and is sustained at 22 knots or more for at least one minute (see paragraph 12.6.8.e.(1)).

		.. cpp:enumerator:: FUNNEL_CLOUD

			Funnel cloud / tornadic activity.

				#. Tornado. A violent, rotating column of air touching the ground.

				#. Funnel Cloud. A violent, rotating column of air which does not touch the surface.

				#. Waterspout. A violent, rotating column of air that forms over a body of water, and touches the water surface.

		.. cpp:enumerator:: SANDSTORM

			Sandstorm. Particles of sand carried aloft by a strong wind. The sand particles are mostly confined to the lowest ten feet, and rarely rise more than fifty feet above the ground.

		.. cpp:enumerator:: DUSTSTORM

			Duststorm. A severe weather condition characterized by strong winds and dust-filled air over an extensive area.


	.. cpp:enum-class:: Event

		Type of the weather event. 

		.. cpp:enumerator:: NONE

			No event specified.

		.. cpp:enumerator:: BEGINNING

			Indicates beginning of the weather phenomena.

		.. cpp:enumerator:: ENDING

			Indicates ending of the weather phenomena.


Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Qualifier qualifier() const

		:returns: Weather qualifier which indicates time or intensity or proximity of the weather phenomena.

	.. cpp:function:: Descriptor descriptor() const

		:returns: Weather descriptor which indicates additional properties of weather phenomena.

	.. cpp:function:: std::vector<Weather> weather() const

		:returns: Vector of individual weather phenomena.

	.. cpp:function:: Event event() const

		:returns: Type of event.

	.. cpp:function:: std::optional<MetafTime> time() const

		:returns: Time of the event or empty ``std::optional`` if no event time was specified.

Miscellaneous
^^^^^^^^^^^^^

		.. cpp:function:: bool isOmitted() const

			:returns: ``true`` if no qualifier, no descriptor, no weather phenomena, no event and no event time is stored in this instance, ``false`` if any of these conditions is not met.


Validating
^^^^^^^^^^

		.. cpp:function:: bool isValid() const

			:returns: ``true`` if the weather phenomena is valid, and ``false`` otherwise.

				The information is considered valid if all of the following conditions is met: 
					 - weather phenomena is non-empty, i.e. at lease one qualifier or descriptor or weather must be specified;
					 - event time is valid if specified;
					 - if descriptor FZ (freezing) is present, the weather phenomena must contain FG (fog), or the precipitation type which is potentially may freeze: UP (undetermined precipitation), or RA(rain), or DZ (drizzle); other precipitation may be present alone with specified above, e.g. ``FZRASN`` (freezing rain and snow) is valid, while ``FZSNPL`` (freezing snow and ice pellets) is not valid;

.. cpp:namespace-pop::


Formats
-------

See also WeatherGroup :ref:`api_weathergroup:Formats` section for details.

Metaf recognises current and recent weather phenomena information in METAR reports and forecast weather phenomena in METAR trends and in TAF reports.

..note:: Forecast weather phenomena have exactly the same format as current weather phenomena.

Current or forecast weather phenomena are represented by class :cpp:class:`WeatherPhenomena` and consist of:
- one qualifier (:cpp:enum:`WeatherPhenomena::Qualifier`) or no qualifier.
- one descriptor (:cpp:enum:`WeatherPhenomena::Descriptor`) or no desciptor
- obscuration or precipitation or other phenomena, or mixed precipitation (this part may be missing in some cases discussed below).

Recent weather included in METAR report body consists of:
- mandatory qualifier 'Recent'
- one descriptor or no descriptor
- precipitation or mixed precipitation (this part may be missing in group ``RETS`` descussed below).

Qualifiers augment weather information with intensity, proximity or time. The weather qualifiers used by Metaf, their corresponding types and meanings are below.


Weather phenomena in METAR & TAF body and trends
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

METAR reports may include current or recent weather phenomena information, represented by class :cpp:class:`WeatherPhenomena`. They consist of:
- one qualifier (:cpp:enum:`WeatherPhenomena::Qualifier`) or no qualifier.
- one descriptor (:cpp:enum:`WeatherPhenomena::Descriptor`) or no desciptor
- obscuration or precipitation or other phenomena, or mixed precipitation (this part may be missing in some cases discussed below).

Recent weather included in METAR report body consists of:
- mandatory qualifier 'Recent'
- one descriptor or no descriptor
- precipitation or mixed precipitation (this part may be missing in group ``RETS`` descussed below).


Qualifiers
""""""""""

Qualifiers augment weather information with intensity, proximity or time. The weather qualifiers used by Metaf, their corresponding types and meanings are below.

============================ ======================================================= =======================================================
Qualifier                    Metaf type                                              Meaning
============================ ======================================================= =======================================================
No qualifier                 :cpp:enumerator:`WeatherPhenomena::Qualifier::NONE`     No qualifier specified
No qualifier (precipitation) :cpp:enumerator:`WeatherPhenomena::Qualifier::MODERATE` Moderate precipitation intensity
``-``                        :cpp:enumerator:`WeatherPhenomena::Qualifier::LIGHT`    Light precipitation intensity
``+``                        :cpp:enumerator:`WeatherPhenomena::Qualifier::HEAVY`    Heavy intensity
VC                           :cpp:enumerator:`WeatherPhenomena::Qualifier::VICINITY` Phenomena occurs within 10 miles but not at the station
RE                           :cpp:enumerator:`WeatherPhenomena::Qualifier::RECENT`   Recent weather rather than current weather specified
============================ ======================================================= =======================================================


Descriptors
"""""""""""

Descriptors further augment weather information with the type of precipitation or obscuration.

============= ============================================================ ===========================================================
Descriptor    Metaf type                                                   Meaning
============= ============================================================ ===========================================================
No descriptor :cpp:enumerator:`WeatherPhenomena::Descriptor::NONE`         No descriptor specified
MI            :cpp:enumerator:`WeatherPhenomena::Descriptor::SHALLOW`      Shallow, having little vertical extent
PR            :cpp:enumerator:`WeatherPhenomena::Descriptor::PARTIAL`      Partial, covering only part of the location
BC            :cpp:enumerator:`WeatherPhenomena::Descriptor::PATCHES`      Patches randomly covering the location
DR            :cpp:enumerator:`WeatherPhenomena::Descriptor::LOW_DRIFTING` Low drifting, raised by wind to little height above ground
BL            :cpp:enumerator:`WeatherPhenomena::Descriptor::BLOWING`      Blowing, raised by wind to significant height above ground
SH            :cpp:enumerator:`WeatherPhenomena::Descriptor::SHOWERS`      Showery, sudden and of rapidly varying intensity
TS            :cpp:enumerator:`WeatherPhenomena::Descriptor::THUNDERSTORM` Thunderstorm, accompanied by lightning and/or thunder
FZ            :cpp:enumerator:`WeatherPhenomena::Descriptor::FREEZING`     Freezing, depositing rime or ice glaze on the ground
============= ============================================================ ===========================================================

Precipitation
"""""""""""""

Precipitation is particles of water, snow or ice falling to the ground.

============= ========================================================= =====================================================================
Precipitation   Metaf type                                                Meaning
============= ========================================================= =====================================================================
DZ            :cpp:enumerator:`WeatherPhenomena::Weather::DRIZZLE`      Fine water particles falling to the ground (contrary to for and mist)
RA            :cpp:enumerator:`WeatherPhenomena::Weather::RAIN`         Widely separated water droplets or large drops
SN            :cpp:enumerator:`WeatherPhenomena::Weather::SNOW`         Branched ice crystals (snowflakes)
SG            :cpp:enumerator:`WeatherPhenomena::Weather::SNOW_GRAINS`  Very small, white, and opaque grains of ice
IC            :cpp:enumerator:`WeatherPhenomena::Weather::ICE_CRYSTALS` Unbranched ice crystals (ice needles)
PL            :cpp:enumerator:`WeatherPhenomena::Weather::ICE_PELLETS`  Ice pellets, formed by partially melted and refrozen snowflakes
GR            :cpp:enumerator:`WeatherPhenomena::Weather::HAIL`         Larger pieces of ice falling separately or frozen together
GS            :cpp:enumerator:`WeatherPhenomena::Weather::SMALL_HAIL`   Pices of ice similar to hail but smaller in size
UP            :cpp:enumerator:`WeatherPhenomena::Weather::UNDETERMINED` Automated station detects precipitation but cannot determine its type
============= ========================================================= =====================================================================

Obscurations
""""""""""""

Obscurations cause reduction of visibility.

=============== ========================================================= =============================================================
Obscuration     Metaf type                                                Meaning
=============== ========================================================= =============================================================
FG              :cpp:enumerator:`WeatherPhenomena::Weather::FOG`          Water particles suspended in atmosphere
BR              :cpp:enumerator:`WeatherPhenomena::Weather::MIST`         Similar to fog but causing less reducing of visibility
FU              :cpp:enumerator:`WeatherPhenomena::Weather::SMOKE`        Smoke causing significant visibility reduction
VA              :cpp:enumerator:`WeatherPhenomena::Weather::VOLCANIC_ASH` Fine rock powder released by volcano and suspended in the air
DU              :cpp:enumerator:`WeatherPhenomena::Weather::DUST`         Widespread dust in the air
SA              :cpp:enumerator:`WeatherPhenomena::Weather::SAND`         Sand particles suspended in the air
HZ              :cpp:enumerator:`WeatherPhenomena::Weather::HAZE`         Suspension of extremely small dry particles in the air
PY              :cpp:enumerator:`WeatherPhenomena::Weather::SPRAY`        Water droplets torn from bodies of water by strong wind
=============== ========================================================= =============================================================

Other weather phenomena
"""""""""""""""""""""""

Other weather phenomena cannot be classifed as obscuration or precipitation.

=============== ========================================================= ===========================================================
Other phenomena Metaf type                                                   Meaning
=============== ========================================================= ===========================================================
//              :cpp:enumerator:`WeatherPhenomena::Weather::NOT_REPORTED` Weather phenomena is not reported
PO              :cpp:enumerator:`WeatherPhenomena::Weather::DUST_WHIRLS`  Well-developed dust or sand whirls
SQ              :cpp:enumerator:`WeatherPhenomena::Weather::SQUALLS`      Sudden strong winds
FC              :cpp:enumerator:`WeatherPhenomena::Weather::FUNNEL_CLOUD` Tornadic activity: funnel cloud, tornado, waterspout
SS              :cpp:enumerator:`WeatherPhenomena::Weather::SANDSTORM`    Particles of sand carried aloft by a strong wind
DS              :cpp:enumerator:`WeatherPhenomena::Weather::DUSTSTORM`    Strong winds and dust-filled air at large area
=============== ========================================================= ===========================================================

The obscuration, precipitation and other phenomena combined with descriptors and qualifiers can potentially make a lot of combination but only small portion of the potential combinations is used. Most of them are just semantically incorrect (such as 'MIRA', shallow rain), or not descriptive enough.

Combinations of weather phenomena recognised by Metaf
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Metar recognises only the following combinations of weather phenomena:

 - Obscuration or other phenomena groups: BR, FU, VA, HZ, PO, SQ, FC
 - Tornado or waterspout group +FC
 - Fog group FG or fog with descriptor groups MIFG, PRFG, BCFG, FZFG
 - Widespread dust group DU or dust with descriptor groups DRDU and BLDU
 - Sand group SA or sand with descriptor groups DRSA and BLSA
 - Blowing spray: BLPY
 - Ice crystals: IC
 - Duststorm, sandstorm, or their combination groups: only DS, SS, DSSS, SSDS 
 - Heavy duststorm, sandstorm, or their combination groups: only +DS, +SS, +DSSS, +SSDS
 - Precipitation groups: DZ, RA, SN, SG, PL, GR, GS, UP.
 - Mixed precipitation, up to 3 precipitation types per group (may be augmented with intensity qualfier): RASN, +RASNPL, RAGR, -SNPL, etc.
 - Rain or drizzle or undetermined precipitation with descriptor 'freezing'(may be augmented with intensity qualfier): FZRA, -FZDZ, FZRADZ, FZDZRA, +FZUP.
 - Any mixed precipitations which contains freezing rain or drizzle (may be augmented with intensity qualfier): +FZRASN, FZDZPL, -FZRAPL etc.
 - Precipitation or mixed precipitation accompanied by thunderstorm (may be augmented with intensity qualfier): -TSRA, TSDZ, +TSRAGR, etc.
 - Showery precipitation or mixed precipitation (may be augmented with intensity qualfier): SHPL, +SHRAGS, -SHRASN, etc.
 - Recent precipitation or mixed precipitation: RERA, RESN, RERAPL, REDZ, etc.
 - Weather phenomena in vicinity: only VCTS VCFG VCSH VCPO VCFC VCVA VCBLDU VCBLSA VCBLSN VCDS VCSS
 - Non-reported weather phenomena or non reported recent weather phenomena: // or RE//
