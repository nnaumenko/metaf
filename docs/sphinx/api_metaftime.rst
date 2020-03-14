MetafTime
=========

.. cpp:namespace-push:: metaf

MetafTime class
---------------

	.. cpp:class:: MetafTime

		MetafTime is a time format used in METAR and TAF reports. It contains time-of-day in form of hour and minute and optional day-of-month.

	.. cpp:namespace-push:: MetafTime


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:struct Date

		.. cpp:var:: unsigned int year

		Year component of the date. Can be specified as e.g. ``2019`` or just ``19``. Assumed to be a mon-zero value.

		.. cpp:var:: unsigned int month

		Month component of the date. Must be in range 1 to 12.

		.. cpp:var:: unsigned int day

		Day-of-month component of the date.


Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::optional<unsigned int> day() const

		:returns: The value of day-of-month or empty ``std::optional`` if no day was specified.

	.. cpp:function:: unsigned int hour() const

		:returns: Time-of-day hour.

		.. note: Hour value 0 means midnight at the beginning of the specified day and hour value 24 means midnight at the end of the specified day.

	.. cpp:function:: unsigned int minute() const

		:returns: Time-of-day minute.


Miscellaneous
^^^^^^^^^^^^^

	.. cpp:function:: bool is3hourlyReportTime() const

		:returns: ``true`` if the this time qualifies as 3-hourly report release time in North America (i.e. reports issued within one hour before or after 0300Z, 0900Z, 1500Z, and 2100Z), according to Field Meteorology Handbook Number 1, chapter 12.4.


	.. cpp:function:: bool is6hourlyReportTime() const

		:returns: ``true`` if the this time qualifies as 6-hourly report release time in North America (i.e. reports issued within one hour before or after 0000Z, 0600Z, 1200Z, and 1800Z), according to Field Meteorology Handbook Number 1, chapter 12.4.

	.. cpp:function:: Date dateBeforeRef(const Date & refDate) const

		This function compliments MetafTime with year and month, based on supplied reference date, assuming that MetafTime refers to the time point at maximum 1 month before the supplied reference date.

		:param refDate: Reference date; may equal current date for recent METAR or TAF report or date of METAR or TAF report retreival for archived/historical report.

		:returns: Date including year and month for MetafTime instance, assuming that point of time info stored in this MetafTime instance is before refDate.

		.. note:: This method does not validate supplied current date and day-of-month stored in the group. The inconsistensy of both input parameters and returned value (such as day-of-month exceeding max days in this month, month being in range 1 to 12, etc.) must be checked by others than Metaf.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if stored day-of-month and time-of-day information is valid and ``false`` otherwise.

			The information is considered valid if all of the following conditions are met: 
				- The day-of-month value is in range 1 to 31;
				- The hour value is in range 0 to 24;
				- The minute value is in range 0 to 59; 


		.. note:: METAR and TAF reports always use GMT time.

.. cpp:namespace-pop::


Formats
-------

6-digit day/time format
^^^^^^^^^^^^^^^^^^^^^^^

For report release time and some TAF trends, a 6-digit time format is used: first 2 digits are day-of-month (01 to 31), next 2 digits are hour (00 to 24), and last 2 digits are minute (00 to 59). For example, time ``281245`` means day-of-month 28, and time 12:45.


4-digit TAF format
^^^^^^^^^^^^^^^^^^

Most TAF groups use 4-digit time format: first two digits are day-of-month (01 to 31), and last two digits are hour (00 to 24). For example, time ``3019`` used in TAF means day-of-month 30 and time 19:00.


4-digit METAR format
^^^^^^^^^^^^^^^^^^^^

METAR trends use different 4-digit time format: first two digits are hour (00 to 24) and last two digits are minutes (00 to 59). For example, time ``2355`` used in METAR means time 23:55.

Where hour value 24 is used, it means '00 hours of the next day'. For example, METAR time ``2430`` means time 00:30 on the next day.
