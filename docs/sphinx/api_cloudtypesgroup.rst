CloudTypesGroup
===============

.. cpp:namespace-push:: metaf

CloudTypesGroup class
---------------------

	.. cpp:class:: CloudTypesGroup

		Stores information about layers of clouds and their okta (1/8s of sky coverage). This group is included in the remarks and is used in Canada.

.. cpp:namespace-push:: CloudTypesGroup

Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::vector<CloudType> cloudTypes() const

		:returns: A vector of :cpp:class:`CloudType`; contains all cloud layers specified in this group or sequence of groups.

		.. note:: Sum of oktas for all layers may exceed 8 octa if higher cloud layer is observed through the gaps in the lower cloud layer.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: Always returns ``true``.


Formats
-------

Metaf recognises two formats of cloud type groups.

One format specifies cloud types in :ref:`api_cloudtype:Format without cloud base height`. For example, group ``CU1SC5AC2`` means that the lowest layer of clouds is of Cumulus type and covers 1/8 of the sky, the next layer is of Stratocumulus type and covers 5/8 of the sky, and the highest observed cloud layer is of Altocumulus type and covers 2/8 of the sky.

Obscurations may be specified in addition to cloud layers, for example ``BLSN1CF3SC4`` means that Blowing Snow covers 1/8 of the sky, the lowest cloud layer is of type Cumulus Fractus and covers 3/8 of the sky, and the next cloud layer is of type Stratocumulus and covers 4/8 of the sky.

The second format is a :ref:`api_cloudtype:Format with cloud base height`. Each group describes single cloud layer, for example, ``3ST004`` means that cloud layer with base height 400 feet covers 3/8 of the sky and is of Stratocumulus type.

The sequence of Cloud Type groups with base height specified results in a single :cpp:class:`CloudTypesGroup` instance. For example, groups ``1ST006 3CU015 1CB025 6SC060`` are appended by Metaf and result in a single instance of :cpp:class:`CloudTypesGroup`, rather than four different instances.

Examples
^^^^^^^^

The examples below summarise various formats of cloud type remark groups recognised by Metaf.

Example 1: group without base height
""""""""""""""""""""""""""""""""""""

Group ``CU1SC5AC2`` is parsed by Metaf as :cpp:class:`CloudTypesGroup`. Method :cpp:func:`CloudTypesGroup::cloudTypes()` returns an std::vector<:cpp:class:`CloudType`>. The contents of the vector are below.

	- Element [0]: describes part of the group ``CU1``.

		- :cpp:func:`CloudType::type()` returns :cpp:enumerator:`CloudType::Type::CUMULUS`
		- :cpp:func:`CloudType::height()` returns non-reported :cpp:class:`Distance`.
		- :cpp:func:`CloudType::okta` returns 1

	- Element [1]: describes part of the group ``SC5``.

		- :cpp:func:`CloudType::type()` returns :cpp:enumerator:`CloudType::Type::STRATOCUMULUS`
		- :cpp:func:`CloudType::height()` returns non-reported :cpp:class:`Distance`.
		- :cpp:func:`CloudType::okta` returns 5

	- Element [2]: describes part of the group ``AC2``.

		- :cpp:func:`CloudType::type()` returns :cpp:enumerator:`CloudType::Type::ALTOCUMULUS`
		- :cpp:func:`CloudType::height()` returns non-reported :cpp:class:`Distance`.
		- :cpp:func:`CloudType::okta` returns 2

Example 2: single group with base height
""""""""""""""""""""""""""""""""""""""""

Group ``5CI230`` is parsed by Metaf as :cpp:class:`CloudTypesGroup`. Method :cpp:func:`CloudTypesGroup::cloudTypes()` returns an std::vector<:cpp:class:`CloudType`>. The contents of the vector are below.

	- Element [0]: describes group ``5CI230``.

		- :cpp:func:`CloudType::type()` returns :cpp:enumerator:`CloudType::Type::CIRRUS`
		- :cpp:func:`CloudType::okta` returns 1
		- :cpp:func:`CloudType::height()` returns instance of :cpp:class:`Distance` with the following content:

			- :cpp:func:`Distance::modifier()` returns :cpp:enumerator:`Distance::Modifier::NONE`.
			- :cpp:func:`Distance::distance()` returns 23000.
			- :cpp:func:`Distance::unit()` returns :cpp:enumerator:`Distance::Unit::FEET`.

Example 3: sequence of groups with base height
""""""""""""""""""""""""""""""""""""""""""""""

Sequence of groups ``1ST006 3CU015 1CB025 6SC060`` is parsed by Metaf as a single :cpp:class:`CloudTypesGroup`. Method :cpp:func:`CloudTypesGroup::cloudTypes()` returns an std::vector<:cpp:class:`CloudType`>. The contents of the vector are below.

	- Element [0]: describes group ``1ST006``.

		- :cpp:func:`CloudType::type()` returns :cpp:enumerator:`CloudType::Type::STRATOCUMULUS`
		- :cpp:func:`CloudType::okta` returns 1
		- :cpp:func:`CloudType::height()` returns instance of :cpp:class:`Distance` with the following content:

			- :cpp:func:`Distance::modifier()` returns :cpp:enumerator:`Distance::Modifier::NONE`.
			- :cpp:func:`Distance::distance()` returns 600.
			- :cpp:func:`Distance::unit()` returns :cpp:enumerator:`Distance::Unit::FEET`.

	- Element [1]: describes group ``3CU015``.

		- :cpp:func:`CloudType::type()` returns :cpp:enumerator:`CloudType::Type::CUMULUS`
		- :cpp:func:`CloudType::okta` returns 3
		- :cpp:func:`CloudType::height()` returns instance of :cpp:class:`Distance` with the following content:

			- :cpp:func:`Distance::modifier()` returns :cpp:enumerator:`Distance::Modifier::NONE`.
			- :cpp:func:`Distance::distance()` returns 1500.
			- :cpp:func:`Distance::unit()` returns :cpp:enumerator:`Distance::Unit::FEET`.

	- Element [2]: describes group ``1CB025``.

		- :cpp:func:`CloudType::type()` returns :cpp:enumerator:`CloudType::Type::CUMULONIMBUS`
		- :cpp:func:`CloudType::okta` returns 1
		- :cpp:func:`CloudType::height()` returns instance of :cpp:class:`Distance` with the following content:

			- :cpp:func:`Distance::modifier()` returns :cpp:enumerator:`Distance::Modifier::NONE`.
			- :cpp:func:`Distance::distance()` returns 2500.
			- :cpp:func:`Distance::unit()` returns :cpp:enumerator:`Distance::Unit::FEET`.

	- Element [2]: describes group ``6SC060``.

		- :cpp:func:`CloudType::type()` returns :cpp:enumerator:`CloudType::Type::STRATOCUMULUS`
		- :cpp:func:`CloudType::okta` returns 6
		- :cpp:func:`CloudType::height()` returns instance of :cpp:class:`Distance` with the following content:

			- :cpp:func:`Distance::modifier()` returns :cpp:enumerator:`Distance::Modifier::NONE`.
			- :cpp:func:`Distance::distance()` returns 6000.
			- :cpp:func:`Distance::unit()` returns :cpp:enumerator:`Distance::Unit::FEET`.


Regional variations
^^^^^^^^^^^^^^^^^^^

Group format without cloud base height is mostly used in North America. Obscurations (such as blowing snow, etc.) within this group are specified only in Canada.

Group format with cloud base height is used in Japan.

See also :ref:`api_cloudtype:Regional variations` section of :cpp:class:`CloudType` documentation.


Caveats
^^^^^^^

If the group without cloud base height contains more than 8 cloud layers, Metaf saves only first 8, and the rest are ignored.
