CloudGroup
==========

.. cpp:namespace-push:: metaf

CloudGroup class
----------------

	.. cpp:class:: CloudGroup

		Stores information about a single cloud layer, cloud-like obscuration, lack of cloud cover or vertical visibility.

.. cpp:namespace-push:: CloudGroup


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Specifies what kind of information is stored within this group.

		.. cpp:enumerator::	NO_CLOUDS

			Clear sky condition, no clouds detected, or no significant clouds. Use :cpp:func:`amount()` for exact condition; the possible values are :cpp:enumerator:`Amount::NONE_CLR`, :cpp:enumerator:`Amount::NONE_SKC`, :cpp:enumerator:`Amount::NCD`, or :cpp:enumerator:`Amount::NSC`.

		.. cpp:enumerator:: CLOUD_LAYER

			Cloud layer specified in METAR report, trend or remarks.

			Use :cpp:func:`amount()` for cloud amount, :cpp:func:`height()` for base height, and :cpp:func:`convectiveType()` for significant convective type.

		.. cpp:enumerator:: VERTICAL_VISIBILITY

			Sky is obscured and vertical visibility is indicated instead of cloud data. Use :cpp:func:`verticalVisibility()` for vertical visibility value. :cpp:func:`amount()` will return  :cpp:enumerator:`Amount::OBSCURED`.

		.. cpp:enumerator:: CEILING

			Ceiling height. Use :cpp:func:`height()` for ceiling height value. Use :cpp:func:`runway()` and :cpp:func:`direction()` for the location where ceiling is reported.

			.. note:: CAVOK group (:cpp:enumerator:`metaf::KeywordGroup::Type::CAVOK`) is also used to indicate no cloud below 5000 feet (1500 meters) and no cumulonimbus or towering cumulus clouds.

		.. cpp:enumerator:: VARIABLE_CEILING

			Ceiling height is variable. Use :cpp:func:`minHeight()` and :cpp:func:`maxHeight()` for ceiling height range. Use :cpp:func:`runway()` or :cpp:func:`direction()` for the location where ceiling is reported.

		.. cpp:enumerator:: CHINO

			Indicates the that the ceiling data is not available for a secondary location. Use :cpp:func:`runway()` or :cpp:func:`direction()`.

		.. cpp:enumerator:: CLD_MISG

			Indicates the that cloud data are missing. No further details are provided.

		.. cpp:enumerator:: OBSCURATION

			Indicates the that instead of cloud data, a ground-based or aloft obscuration data is specified.

			Use :cpp:func:`amount()` for obscuration amount, :cpp:func:`height()` for base height (or zero height for ground-based obscuration), and :cpp:func:`cloudType()` for type of obscuration.


	.. cpp:enum-class:: Amount

		Amount (cover) of the cloud layer.

		See also CAVOK (:cpp:enumerator:`metaf::KeywordGroup::Type::CAVOK`) which may be used to specify no cloud below 5000 feet (1500 meters) and no cumulonimbus or towering cumulus clouds.

		.. cpp:enumerator::	NOT_REPORTED

			Amount of cloud (cloud cover) is not reported or not applicable for this type of group.

		.. cpp:enumerator:: NCD

			No cloud detected: automated weather station did not detect any clouds. Either no clouds are present or sensor error occurred.

		.. cpp:enumerator:: NSC

			Nil significant clouds: no cloud below 5000 feet (1500 meters), no cumulonimbus or towering cumulus, and no vertical visibility restriction.

			.. note:: CAVOK group (:cpp:enumerator:`metaf::KeywordGroup::Type::CAVOK`) is also used to indicate no cloud below 5000 feet (1500 meters) and no cumulonimbus or towering cumulus clouds.

		.. cpp:enumerator:: NONE_CLR

			No clouds / clear sky. No cloud layers are detected at or below 12000 feet /3700 meters) (US) or 25000 feet / 7600 meters (Canada).

			Indicates that station is at least partly automated.

		.. cpp:enumerator:: NONE_SKC

			No clouds / clear sky. In North America indicates report producted by human rather than automatic weather station.

		.. cpp:enumerator:: FEW

			Few clouds (1/8 to 2/8 sky covered).

		.. cpp:enumerator:: SCATTERED

			Scattered clouds (3/8 to 4/8 sky covered).

		.. cpp:enumerator:: BROKEN

			Broken clouds (5/8 to 7/8 sky covered).

		.. cpp:enumerator:: OVERCAST

			Overcast (8/8 sky covered)

		.. cpp:enumerator:: OBSCURED

			Sky obscured; vertical visibility reported instead.

		.. cpp:enumerator:: VARIABLE_FEW_SCATTERED

			Cloud cover is variable between :cpp:enumerator:`FEW` and  :cpp:enumerator:`SCATTERED`.

		.. cpp:enumerator:: VARIABLE_SCATTERED_BROKEN

			Cloud cover is variable between :cpp:enumerator:`SCATTERED` and  :cpp:enumerator:`BROKEN`.

		.. cpp:enumerator:: VARIABLE_BROKEN_OVERCAST

			Cloud cover is variable between :cpp:enumerator:`BROKEN` and  :cpp:enumerator:`OVERCAST`.


	.. cpp:enum-class:: ConvectiveType

		Significant convective type of the cloud in the layer.

		.. cpp:enumerator::NONE

			No cloud type specified or not applicable.

		.. cpp:enumerator::NOT_REPORTED

			Convective cloud type is not reported.

		.. cpp:enumerator::TOWERING_CUMULUS

			Convective cloud: towering cumulus.

		.. cpp:enumerator::CUMULONIMBUS

			Convective cloud: cumulonimbus.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Amount amount() const

		:returns: Amount (cover) of clouds in layer or clear sky conditions.

	.. cpp:function:: ConvectiveType type() const

		:returns: Convective type of the cloud in the layer.

	.. cpp:function:: Distance height() const

		:returns: Cloud base or ceiling height or non-reported value if height is not applicable for this group.

		.. note:: When the sky is obscured use :cpp:func:`verticalVisibility()`.

		.. note:: When variable ceiling height is reported use :cpp:func:`minHeight()` or :cpp:func:`maxHeight()`.

	.. cpp:function:: Distance verticalVisibility() const

		:returns: Vertical visibility or value if sky is obscured; non-reported value otherwise.

	.. cpp:function:: Distance minHeight() const

		:returns: Minimum ceiling height if variable ceiling height is reported; non-reported value otherwise.

	.. cpp:function:: Distance maxHeight() const

		:returns: Maximum ceiling height if variable ceiling height is reported; non-reported value otherwise.

	.. cpp::function:: std::optional<Runway> runway() const

		:returns: For location-specific data such as ceiling, .

Miscellaneous
^^^^^^^^^^^^^

	.. cpp:function:: std::optional<CloudType> cloudType() const

		:returns: :cpp:class:`CloudType` corresponding to the information stored in the group (maximum okta value, convective type, and cloud base height or minimum height if variable), or empty std::optional if 'no clouds' conditions or vertical visibility or missing data groups.

Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if stored cloud information is valid, and ``false`` otherwise. The information is considered valid if the value of cloud cover height or vertical visibility is valid (if reported). Zero height of cloud cover base or vertical visibility does not make the information invalid.

.. cpp:namespace-pop::