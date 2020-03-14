LocationGroup
=============

.. cpp:namespace-push:: metaf

LocationGroup class
-------------------

	.. cpp:class:: LocationGroup

		Location group stores an ICAO location code of the site where observation was performed or for which the forecast is provided.

		If the report is issued for the location which does not have an ICAO code, then code ZZZZ is used.

		An ICAO code is a four-character string. First character may only contain latin capital letters; the rest of the character may contain either latin capital letters or digits.

.. cpp:namespace-push:: LocationGroup

Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::string toString() const

		:returns: String with an ICAO location.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: This method is for compatibility only and always returns ``true`` for this group.

.. cpp:namespace-pop::


Description
-----------

Format
^^^^^^

The format of this group is four characters, and contains only uppercase letters and digits. First character must not be a digit.

.. image:: locationgroup.svg


Parsing
^^^^^^^

This group is recognised only when found in report header.

For example groups ``UKLL`` or ``K2J3`` are interpreted as LocationGroup, while groups ``2ABC`` or ``ZZZ$`` are not.

Method :cpp:func:`toString()` returns string with ICAO location code stored in this group.

For example, for group ``UKLL`` represented by :cpp:class:`LocationGroup`, :cpp:func:`toString()` will return std::string with value "UKLL".
