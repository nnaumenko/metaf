Pressure
========

.. cpp:namespace-push:: metaf

Pressure class
--------------

	.. cpp:class:: Pressure

		Stores a pressure value. The intended use is atmospheric pressure but any absolute pressure value can be stored.

		Optionally pressure value can be non-reported (i.e. no value).


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Unit

		Pressure measurement units.

		.. cpp:enumerator:: HECTOPASCAL

			Hectopascal.

		.. cpp:enumerator:: INCHES_HG

			Inches mercury.

		.. cpp:enumerator:: MM_HG

			Millimeters mercury.


Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::optional<float> pressure() const

	:returns: Stored pressure value or empty ``std::optional`` if pressure value is not reported.

	.. cpp:function:: Unit unit() const

	:returns: Pressure measurement unit which was used with stored value.


Converting to other measurement units
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::optional<float> toUnit(Unit unit) const

		:param unit: Measurement unit to convert the value to.

		:returns: Stored pressure value converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.


Miscellaneous
^^^^^^^^^^^^^

	.. cpp:function:: bool isReported() const

		:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored.

.. cpp:namespace-pop::


Formats
-------

Metaf recognises the following pressure values.


Pressure in hectopascals
^^^^^^^^^^^^^^^^^^^^^^^^

Pressure in hectopascals is a 4-digit integer value, for example ``0998`` means pressure of 998 hPa, and ``1012`` means 1012 hectopascals.

Value ``////`` may be used instead of pressure in hectopascals to indicate non-reported value.


Pressure in inches mercury
^^^^^^^^^^^^^^^^^^^^^^^^^^

Pressure in inches mercury is a 4-digit value where first two digits are integer and last two digits are decimal fraction. For example, ``2939`` means pressure of 29.39 inches of mercury.

Value ``////`` may be used instead of pressure in inches mercury to indicate non-reported value.


Pressure change
^^^^^^^^^^^^^^^

Pressure change is a 3-digit value where first two digits are integer and last digit is decimal fraction. Pressure change is always specified in hectopascals. For example, ``032`` means pressure change of 3.2 hectopascals.

Value ``///`` may be used instead of pressure change value to indicate non-reported value.


Additional value in hectopascals
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In some stations in the regions where main pressure unit is inches of mercury, an additional pressure value in hectopascals may be specified in remarks. It is a 3-digit value where two first digits specify hectopascals and last digit specifies tenths of hectopascal. The first digits of the integer part are not specified.

For example, value ``982`` means 998.2 hectopascals, value ``015`` means 1001.5 hectopascals, ``221`` means 1022.1 hectopascals. Note that leading digit 9 in values less than 1000 hPa and leading 10 in values of 1000 hPa or more are not specified in the report.


Pressure in millimeters mercury
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Pressure in millimeters mercury is a 3-digit integer value. For example, ``765`` means pressure of 765 millimeters of mercury.
