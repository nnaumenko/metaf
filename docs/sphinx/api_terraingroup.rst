TerrainGroup
============

.. cpp:namespace-push:: metaf

TerrainGroup class
------------------

	.. cpp:class:: TerrainGroup

		Stores information about visibility of terrain features such as mountains and valleys.

.. cpp:namespace-push:: TerrainGroup


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Type of the stored terrain group.

		.. cpp:enumerator:: TERRAIN_VISIBILITY

			Indicates that this group contains information on terrain features visibility.

Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Currently always returns :cpp:enumerator:`Type::TERRAIN_VISIBILITY`.

	.. cpp:function:: std::vector<TerrainVisibility> terrainVisibility() const

		:returns: Returns ``std::vector`` of terrain visibility data in various directions.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if all of the reported :cpp:class:`TerrainVisibility` are valid. Alternatively returns ``false`` if any of the terrain visibility values are not valid.


Formats
-------

Terrain features visibility
^^^^^^^^^^^^^^^^^^^^^^^^^^^

See :cpp:class:`TerrainVisibility` for the formats of groups recognised by Metaf.

Regional variations
^^^^^^^^^^^^^^^^^^^

Group variant ``MT OBSC`` is used only in Russia.

The other group variants are used only in Italy.
