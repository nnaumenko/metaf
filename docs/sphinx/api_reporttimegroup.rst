ReportTimeGroup
===============

.. cpp:namespace-push:: metaf

ReportTimeGroup class
---------------------

	.. cpp:class:: ReportTimeGroup

		Report time stores information about report release date and time.

.. cpp:namespace-push:: ReportTimeGroup


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: MetafTime time() const

		:returns: Time when the report was released (GMT time zone).


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if the day-of-month, hour and minute values of report release time belong to valid ranges (see :cpp:func:`MetafTime::isValid()`) and the optional day is included in :cpp:class:`MetafTime`.

			This method returns ``false`` if any of the conditions above is not met.


Description
-----------

Format
^^^^^^

The format of this group is six digits of (:ref:`api_metaftime:6-digit day/time format`), followed by ``Z`` character.

.. image:: reporttimegroup.svg



Parsing
^^^^^^^

Report release time group is recognised only when found in report header.

Method :cpp:func:`time()` to returns the instance of :cpp:class:`MetafTime` which contains day-of-month, hour, and minute.

For example, for group ``301545Z`` represented by :cpp:class:`ReportTimeGroup`, :cpp:func:`time()` will return an instance of cpp:class:`MetafTime` where :cpp:func:`MetafTime::day()` returns a non-empty std::optional<unsigned int> with value 30, :cpp:func:`MetafTime::hour()` returns 15, and :cpp:func:`MetafTime::minute()` returns 45.
