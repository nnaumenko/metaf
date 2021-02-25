CloudType
=========

.. cpp:namespace-push:: metaf

CloudType class
---------------

	.. cpp:class:: CloudType

		Specifies type of the cloud in the layer, base height, and sky coverage (okta).

.. cpp:namespace-push:: CloudType


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Type/genus of clouds.

		.. note:: Also includes weather phenomena causing obscuration such as rain, fog, blowing snow, etc. used in Canada in addition to cloud types.

		.. cpp:enumerator:: NOT_REPORTED

			Type of the cloud is not reported or not known or not specified in the report.

		.. cpp:enumerator:: CUMULONIMBUS

			Cumulonimbus clouds.

		.. cpp:enumerator:: TOWERING_CUMULUS

			Towering cumulus clouds.

		.. cpp:enumerator:: CUMULUS

			Cumulus clouds.

		.. cpp:enumerator:: CUMULUS_FRACTUS

			Cumulus fractus clouds.

		.. cpp:enumerator:: STRATOCUMULUS

			Stratocumulus clouds.

		.. cpp:enumerator:: NIMBOSTRATUS

			Nimbostratus clouds.

		.. cpp:enumerator:: STRATUS

			Stratus clouds.

		.. cpp:enumerator:: STRATUS_FRACTUS

			Stratus fractus clouds.

		.. cpp:enumerator:: ALTOSTRATUS

			Altostratus clouds.

		.. cpp:enumerator:: ALTOCUMULUS

			Altocumulus clouds.

		.. cpp:enumerator:: ALTOCUMULUS_CASTELLANUS

			Altostratus castellanus clouds.

		.. cpp:enumerator:: CIRRUS

			Cirrus clouds.

		.. cpp:enumerator:: CIRROSTRATUS

			Cirrostratus clouds.

		.. cpp:enumerator:: CIRROCUMULUS

			Cirrostratus clouds.

		.. cpp:enumerator:: BLOWING_SNOW

			Obscuration: blowing snow. Indicates blowing snow rather than snow falling from the clouds.

		.. cpp:enumerator:: BLOWING_DUST

			Obscuration: blowing dust.

		.. cpp:enumerator:: BLOWING_SAND

			Obscuration: blowing sand.

		.. cpp:enumerator:: ICE_CRYSTALS

			Obscuration: ice crystals.

		.. cpp:enumerator:: RAIN

			Obscuration: rain.

		.. cpp:enumerator:: DRIZZLE

			Obscuration: drizzle.

		.. cpp:enumerator:: SNOW

			Obscuration: snow. Snow falling from the clouds. Indicates snow precipitation rather than blowing snow.

		.. cpp:enumerator:: ICE_PELLETS

			Obscuration: ice pellets.

		.. cpp:enumerator:: SMOKE

			Obscuration: smoke.

		.. cpp:enumerator:: FOG

			Obscuration: fog.

		.. cpp:enumerator:: MIST

			Obscuration: mist.

		.. cpp:enumerator:: HAZE

			Obscuration: haze.

		.. cpp:enumerator:: VOLCANIC_ASH

			Obscuration: volcanic ash.

		.. cpp:enumerator:: HAIL

			Obscuration: hail.

		.. cpp:enumerator:: DUSTSTORM

			Obscuration: duststorm.

		.. cpp:enumerator:: SANDSTORM

			Obscuration: sandstorm.

Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Type of the cloud or obscuration or :cpp:enumerator:`Type::NOT_REPORTED` if cloud type is not specified or cannot be included in this format.

	.. cpp:function:: Distance height() const

		:returns: Height of the cloud base; may be a non-reported value if the base height is not reported or cannot be included in this format.

	.. cpp:function:: std::optional<unsigned int> okta() const

		:returns: Sky coverage in 1/8th (e.g. 3 okta means that this cloud covers 3/8 of the sky), or 0 trace amount of clouds is present. If cloud amount is not known, an empty ``std::optional`` is returned.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: always returns ``true``.

.. cpp:namespace-pop::


Formats
-------

Metaf recognises two formats of cloud types: with and without cloud base height.

Okta digit must be in valid range from 1 to 8, otherwise Metaf does not recognise the format. For example, formats ``NS9`` or ``0CU008`` are not recognised.


Format without cloud base height
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This format specifies two- three- or four-character cloud/obscuration type followed by single-digit okta value.

For example, ``SC5`` means stratocumulus clouds covering 5/8 of the sky, and ``NS8`` means nimbostratus clouds covering 8/8 of the sky.


Format with cloud base height
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This format specifies single-digit okta value, followed by two- or three-character cloud type, followed by three-digit cloud base height specified in hundreds of feet. 

For example, ``3ST004`` means stratus clouds covering 3/8 of the sky, with base height 400 feet; ``1TCU020`` means towering cumulus clouds covering 1/8 of the sky, with base height 2000 feet.


Cloud types in METAR
^^^^^^^^^^^^^^^^^^^^

The following cloud types are recognised by Metaf in the METAR reports.

==== ===============================================
Type :cpp:enum:`Type`
==== ===============================================
n/a  :cpp:enumerator:`Type::NOT_REPORTED`
CB   :cpp:enumerator:`Type::CUMULONIMBUS`
TCU  :cpp:enumerator:`Type::TOWERING_CUMULUS`
CU   :cpp:enumerator:`Type::CUMULUS`
CF   :cpp:enumerator:`Type::CUMULUS_FRACTUS`
SC   :cpp:enumerator:`Type::STRATOCUMULUS`
NS   :cpp:enumerator:`Type::NIMBOSTRATUS`
ST   :cpp:enumerator:`Type::STRATUS`
SF   :cpp:enumerator:`Type::STRATUS_FRACTUS`
AS   :cpp:enumerator:`Type::ALTOSTRATUS`
AC   :cpp:enumerator:`Type::ALTOCUMULUS`
ACC  :cpp:enumerator:`Type::ALTOCUMULUS_CASTELLANUS`
CI   :cpp:enumerator:`Type::CIRRUS`
CS   :cpp:enumerator:`Type::CIRROSTRATUS`
CC   :cpp:enumerator:`Type::CIRROCUMULUS`
BLSN :cpp:enumerator:`Type::BLOWING_SNOW`
BLDU :cpp:enumerator:`Type::BLOWING_DUST`
BLSA :cpp:enumerator:`Type::BLOWING_SAND`
IC   :cpp:enumerator:`Type::ICE_CRYSTALS`
RA   :cpp:enumerator:`Type::RAIN`
DZ   :cpp:enumerator:`Type::DRIZZLE`
SN   :cpp:enumerator:`Type::SNOW`
PL   :cpp:enumerator:`Type::ICE_PELLETS`
FU   :cpp:enumerator:`Type::SMOKE`
FG   :cpp:enumerator:`Type::FOG`
BR   :cpp:enumerator:`Type::MIST`
HZ   :cpp:enumerator:`Type::HAZE`
GR   :cpp:enumerator:`Type::HAIL`
DS   :cpp:enumerator:`Type::DUSTSTORM`
SS   :cpp:enumerator:`Type::SANDSTORM`
==== ===============================================


Regional variations
^^^^^^^^^^^^^^^^^^^

Cloud type format without cloud base height is mostly used in North America. 

In Canada in addition to cloud types, the following obscurations may be specified: 'blowing snow', 'blowing dust', 'blowing sand', 'ice crystals', 'rain', 'drizzle', 'snow', 'ice pellets', 'smoke', 'fog', 'mist', 'haze'.

Cloud type format with cloud base height is used in Japan.
