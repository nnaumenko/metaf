Runway
======

.. cpp:namespace-push:: metaf

Runway class
------------

	.. cpp:class:: Runway

		Provides runway identification in form of runway number and designator.

	.. cpp:namespace-push:: Runway


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Designator

		Distinguishes parallel runways.

		.. cpp:enumerator:: NONE

			No designator was specified (not a runway parallel to other runway).

		.. cpp:enumerator:: LEFT

			Left runway.

		.. cpp:enumerator:: CENTER

			Center runway.

		.. cpp:enumerator:: RIGHT

			Right runway.


Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: unsigned int number() const
		
		:returns: Runway number or one of special codes (see also :cpp:func:`isAllRunways()` and :cpp:func:`isMessageRepetition()`).

	.. cpp:function:: Designator designator() const

		:returns: Runway designator or :cpp:enumerator:`Designator::NONE` if no runway designator was specified, which means that the runway is not one of the parallel runways.


Checking for special runway codes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: bool isAllRunways() const

		:returns: ``true`` if 'all runways' value is coded in runway information (i.e. runway number 88 and designator :cpp:enumerator:`Designator::NONE`), and ``false`` otherwise.

	.. cpp:function:: bool isMessageRepetition() const

		:returns: ``true`` if 'last message repetition' value is coded in runway information (i.e. runway number 99 and designator :cpp:enumerator:`Designator::NONE`), and ``false`` otherwise.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if stored runway information is valid, and ``false`` otherwise.

			The information is considered valid if any of the following conditions is met: 
				 - the runway number is in range 0 to 36;
				 - the runway number is either 88 or 99 and the designator is NONE;

.. cpp:namespace-pop::


Formats
-------

Runway format
^^^^^^^^^^^^^

The runway number is formed by the first two digits of runway heading (which is 0 to 360 degrees). The valid range for runway number is thus 0 to 36.

The following special codes can also be used for runway number:

====== ======================= =================================
Number Meaning                 Associated method
====== ======================= =================================
88     All runways             :cpp:func:`isAllRunways()`
99     Last message repetition :cpp:func:`isMessageRepetition()`
====== ======================= =================================

Parallel runways are distinguished by designating them as a left / center / right runway (e.g. runways '21 right' and '21 left' are two parallel runways with heading 210 degrees).

Metaf recognises runway specified as a part of the group, in format ``Rxx`` where ``R`` character is followed by two digits of the runway heading. Runways with designator are specified as ``RxxR``, ``RxxC``, ``RxxL`` for Right, Center and Left runway accordingly.

Some groups (mostly remarks) use format ``RWYxx`` along with ``Rxx``.

In some cases, groups ``ALL RWY`` are used to identify all runways instead or ``R88``.
