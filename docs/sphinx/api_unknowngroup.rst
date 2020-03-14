UnknownGroup
============

.. cpp:namespace-push:: metaf

UnknownGroup class
------------------

	.. cpp:class:: UnknownGroup

		The groups in METAR or TAF report that were not recognised by the parser, are stored as Unknown Groups.

		Unknown group does not store any information and serves as a placeholder.

		Uninterrupted sequence of two or more groups which were not recognised by parser is represented by a single UnknownGroup rather than sequence of several UnknownGroup. E.g. entire text "LAST NO AMDS" will be represented by a single UnknownGroup.

		.. note:: UnknownGroup does not store any information but the text not recognised by parser may be acquired via :cpp:var:`GroupInfo::rawString`.

.. cpp:namespace-push:: UnknownGroup


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::string toString() const

		:returns: Content of plain text group in form of ``std::string``.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if the information stored in the group is valid and consistent, and ``false`` otherwise.

			Plain text group is considered valid if it contains a non-empty string.

.. cpp:namespace-pop::
