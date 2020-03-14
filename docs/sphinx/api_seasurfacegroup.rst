SeaSurfaceGroup
===============

.. cpp:namespace-push:: metaf

SeaSurfaceGroup class
---------------------

	.. cpp:class:: SeaSurfaceGroup

		Stores information about temperature of sea surface along with descriptive state of sea surface or wave height. This group is used by oil platforms.

.. cpp:namespace-push:: SeaSurfaceGroup


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Temperature surfaceTemperature() const

		:returns: Temperature of the sea surface or non-reported value.

	.. cpp:function:: WaveHeight waves() const

		:returns: Wave height or descriptive state of the sea surface or non-reported value.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: Always returns ``true``.

.. cpp:namespace-pop::
