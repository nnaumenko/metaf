Precipitation
=============

.. cpp:namespace-push:: metaf


Precipitation class
-------------------

	.. cpp:class:: Precipitation

		The amount or accumulation of precipitation. The amount/accumulation may be not reported (i.e. no value).

.. cpp:namespace-push:: Precipitation


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Unit

		Precipitation amount or accumulation measurement units.

		.. cpp:enumerator:: MM

			Millimeters.
			
		.. cpp:enumerator:: INCHES

			Inches.


Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::optional<float> precipitation() const

		:returns: Stored amount/accumulation value or empty ``std::optional`` if the value is not reported.

	.. cpp:function:: Unit unit() const

		:returns: Precipitation amount/accumulation measurement unit which was used with stored value.


Converting to other measurement units
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::optional<float> toUnit(Unit unit) const

		:param unit: Measurement unit to convert the value to.

		:returns: Stored precipitation amount/accumulation value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


Miscellaneous
^^^^^^^^^^^^^

	.. cpp:function:: bool isReported() const

		:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored.

.. cpp:namespace-pop::


Formats
-------

A number of precipitation / accumulation / ice accretion formats are recognised by Metaf; the values may be specified in millimeters, inches, tenths of inch and hundredths of inch, see :ref:`api_precipitationgroup:PrecipitationGroup` and :ref:`api_runwaystategroup:RunwayStateGroup` for details.