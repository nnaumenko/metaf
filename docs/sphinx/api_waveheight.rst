WaveHeight
==========

.. cpp:namespace-push:: metaf

WaveHeight class
----------------

	.. cpp:class:: WaveHeight

		WaveHeight or descriptive state of sea surface which specifies the range of wave heights.

		Both state of sea surface and wave height may be optionally not reported (i.e. no value).

.. cpp:namespace-push:: WaveHeight

Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		.. cpp:enumerator:: STATE_OF_SURFACE

			Descriptive state of surface is specified.

		.. cpp:enumerator:: WAVE_HEIGHT

			Actual numerical wave height is specified.


	.. cpp:enum-class:: Unit

		Wave height measurement unit.

		.. cpp:enumerator:: METERS

			Meters.

		.. cpp:enumerator:: FEET

			Feet.


	.. cpp:enum-class:: StateOfSurface

		.. cpp:enumerator:: NOT_REPORTED

			State of sea surface is not reported.

		.. cpp:enumerator:: CALM_GLASSY

			Sea surface calm (glassy), no waves.

		.. cpp:enumerator:: CALM_RIPPLED

			Sea surface calm (rippled), wave height <0.1 meters.

		.. cpp:enumerator:: SMOOTH

			Sea surface smooth, wave height 0.1 to 0.5 meters.

		.. cpp:enumerator:: SLIGHT

			Slight waves with height 0.5 to 1.25 meters.

		.. cpp:enumerator:: MODERATE

			Moderate waves with height 1.25 to 2.5 meters.

		.. cpp:enumerator:: ROUGH

			Sea surface rough, wave height 2.5 to 4 meters.

		.. cpp:enumerator:: VERY_ROUGH

			Sea surface very rough, wave height 4 to 6 meters.

		.. cpp:enumerator:: HIGH

			High waves with height 6 to 9 meters.

		.. cpp:enumerator:: VERY_HIGH

			Very high waves with height 9 to 14 meters.

		.. cpp:enumerator:: PHENOMENAL

			Phenomenal waves with height of 14 meters or more.


Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Type of the value: descriptive sea surface or numerical wave height.

	.. cpp:function:: StateOfSurface stateOfSurface() const

		:returns: State of sea surface corresponding to the value.

			If wave height was specified, a range of :cpp:enum:`StateOfSurface` where this value of the wave height fits is returned (e.g. if wave height of 1.1 meters was reported, then :cpp:enumerator:`StateOfSurface::SLIGHT` is returned).

	.. cpp:function:: std::optional<float> waveHeight() const

		:returns: Wave height value.

			If :cpp:enum:`StateOfSurface` was reported, then highest wave height for the range specified by stored descriptive value is returned (e.g. if state of sea surface was reported as :cpp:enumerator:`StateOfSurface::ROUGH` then 4.0 is returned).

			If :cpp:enumerator:`StateOfSurface::PHENOMENAL` was reported then there is no highest wave height value and lowest value of 14 meters is returned instead.

	.. cpp:function:: Unit unit() const

		:returns: Wave height measurement unit which was used with stored value. Currently always returns :cpp:enumerator:`Unit::METERS` since the value is always specified in decimeters.


Miscellaneous
^^^^^^^^^^^^^

	.. cpp:function:: bool isReported() const

		:returns: ``true`` if wave height is reported (either as descriptive state or as actual wave height).


Converting to other measurement units
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: std::optional<float> toUnit(Unit unit) const

		:param unit: Measurement unit to convert the value to.

		:returns: Stored wave height value (or highest wave height value for specified :cpp:enum:`StateOfSurface`) converted into specified measurement unit or empty ``std::optional`` if conversion failed or the stored value was not reported.

.. cpp:namespace-pop::


Formats
-------

Metaf recognises two formats for wave height: descriptive state of sea surface and wave height in decimeters.


State of sea surface
^^^^^^^^^^^^^^^^^^^^

State of sea surface is specified as character ``S`` followed by single-digit code of sea surface state. The table below summarises the codes.

==== ========================================================== ===================
Code :cpp:enum:`WaveHeight::StateOfSurface`                     Wave height
==== ========================================================== ===================
0    :cpp:enumerator:`WaveHeight::StateOfSurface::CALM_GLASSY`  0 meters
1    :cpp:enumerator:`WaveHeight::StateOfSurface::CALM_RIPPLED` <0.1 meters
2    :cpp:enumerator:`WaveHeight::StateOfSurface::SMOOTH`       0.1 to 0.5 meters
3    :cpp:enumerator:`WaveHeight::StateOfSurface::SLIGHT`       0.5 to 1.25 meters
4    :cpp:enumerator:`WaveHeight::StateOfSurface::MODERATE`     1.25 to 2.5 meters
5    :cpp:enumerator:`WaveHeight::StateOfSurface::ROUGH`        2.5 to 4 meters
6    :cpp:enumerator:`WaveHeight::StateOfSurface::VERY_ROUGH`   4 to 6 meters
7    :cpp:enumerator:`WaveHeight::StateOfSurface::HIGH`         6 to 9 meters
8    :cpp:enumerator:`WaveHeight::StateOfSurface::VERY_HIGH`    9 to 14 meters
9    :cpp:enumerator:`WaveHeight::StateOfSurface::PHENOMENAL`   more than 14 meters
/    :cpp:enumerator:`WaveHeight::StateOfSurface::NOT_REPORTED` n/a
==== ========================================================== ===================

For example, ``S4`` means moderate waves (wave height 1.25 to 2.5 meters).


Wave height value
^^^^^^^^^^^^^^^^^

Wave height is specified as ``H`` character followed by three-digit wave height in decimeters.

For example, ``H038`` means wave height of 3.8 meters.

Value ``H///`` means 'wave height is not reported'. 
