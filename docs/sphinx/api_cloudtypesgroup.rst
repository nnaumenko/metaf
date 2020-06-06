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
