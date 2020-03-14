Direction
=========

.. cpp:namespace-push:: metaf

Direction class
---------------

	.. cpp:class:: Direction

		Stores a direction value. The value can be specified in degrees or as a cardinal direction. Alternatively the direction value may be omitted (i.e. not specified), specified as not reported (i.e. no value), specified as variable, or specified as No Directional Variation.

.. cpp:namespace-push:: Direction


Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Cardinal
		
		Specifies a cardinal or intercardinal direction. No secondary intercardinal directions can be specified. Alternatively may specify No Directional Variation, Non-reported Value, Variable Direction, Overhead, All Quadrants(All Directions) or Unknown Direction.

		.. cpp:enumerator:: NOT_REPORTED

			The direction is not reported or not specified.

		.. cpp:enumerator:: VRB

			Direction is Variable (coded ``VRB`` in METAR and TAF reports).

		.. cpp:enumerator:: NDV

			No directional variation (coded ``NDV``). Indicates that the station cannot differentiate directions, for example for directional visibility.

		.. cpp:enumerator:: N

			North (0 to 22 degrees or 338 to 360 degrees or coded ``N``).

		.. cpp:enumerator:: S

			South (158 to 202 degrees or coded ``S``).

		.. cpp:enumerator:: W

			West (248 to 292 degrees or coded ``W``).

		.. cpp:enumerator:: E

			East (68 to 112 degrees or coded ``E``).

		.. cpp:enumerator:: NW

			Northwest (293 to 337 degrees or coded ``NW``).

		.. cpp:enumerator:: NE

			Northeast (23 to 67 degrees or coded ``NE``).

		.. cpp:enumerator:: SW

			Southwest (203 to 247 degrees or coded ``SW``).

		.. cpp:enumerator:: SE

			Southeast (113 to 157 degrees or coded ``SE``).

		.. cpp:enumerator:: TRUE_N

			True north (exactly 360 degrees). Value of 0 degrees is not considered as true north.

		.. cpp:enumerator:: TRUE_W

			True west (exactly 270 degrees).

		.. cpp:enumerator:: TRUE_S

			True south (exactly 180 degrees).

		.. cpp:enumerator:: TRUE_E

			True east (exactly 90 degrees).

		.. cpp:enumerator:: OHD

			Overhead (coded ``OHD``).

		.. cpp:enumerator:: ALQDS

			All quadrants (all directions). Coded ``ALQDS``.

		.. cpp:enumerator:: UNKNOWN

			Unknown direction (coded ``UNKNOWN``).


	.. cpp:enum-class:: Type

		The type of the direction value reported. If the type is other than :cpp:enumerator:`VALUE_DEGREES` or :cpp:enumerator:`VALUE_CARDINAL`, then no numerical direction value can be acquired.

		.. cpp:enumerator:: NOT_REPORTED

			Direction is specified as 'not reported' or not specified.

		.. cpp:enumerator:: VARIABLE

			Direction is reported as variable (coded ``VRB``).

		.. cpp:enumerator:: NDV

			Direction is reported as 'No Directional Variation'.

		.. cpp:enumerator:: VALUE_DEGREES

			Direction is reported as value in degrees.

		.. cpp:enumerator:: VALUE_CARDINAL

			Direction is reported as cardinal value.

		.. cpp:enumerator:: OVERHEAD

			Phenomena occurrs directly over the location rather than in a distance.

		.. cpp:enumerator:: ALQDS

			Direction is reported as all quadrants (i.e. in all directions).

		.. cpp:enumerator:: UNKNOWN

			Direction is explicitly reported as unknown (e.g. when direction of movement of phenomena in vicinity is unknown).


Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		::returns:: Type of stored direction value.

	.. cpp:function:: Cardinal cardinal(bool trueDirections = false) const

		:param trueDirections: If set to ``true`` allows returning :cpp:enumerator:`Cardinal::TRUE_NORTH`, :cpp:enumerator:`Cardinal::TRUE_SOUTH`, :cpp:enumerator:`Cardinal::TRUE_EAST`, :cpp:enumerator:`Cardinal::TRUE_WEST`

		:returns: Cardinal direction corresponding to the stored direction value.

			.. note:: If the direction value in degrees is reported (i.e. value type is :cpp:enumerator:`Type::VALUE_DEGREES`) the corresponding cardinal direction is returned; if the value exceeds 360 degrees then :cpp:enumerator:`Cardinal::NOT_REPORTED` is returned.

	.. cpp:function:: std::optional<unsigned int> degrees() const

		:returns: Stored value in degrees. If cardinal value was stored, then the middle value of the corresponding directional sector is returned as follows:

			================== ====================
			Cardinal direction Direction in degrees
			================== ====================
			North              360
			Northeast          45
			East               90
			Southeast          135
			South              180
			Southwest          225
			West               270
			Northwest          315
			================== ====================

			If the type of the stored value is other than :cpp:enumerator:`Type::VALUE_DEGREES` or :cpp:enumerator:`Type::VALUE_CARDINAL` then an empty ``std::optional`` is returned.

Miscellaneous
^^^^^^^^^^^^^

	.. cpp:function:: bool isReported() const

		:returns: ``true`` if any directional value is stored in this group (i.e. NDV, VRB, OHD, ALQDS, UNKNOWN or value in degrees or cardinal direction); or ``false`` if non-reported direction is stored in this group.

	.. cpp:function:: bool isValue() const

		:returns: ``true`` if the stored direction contains a value, and ``false`` if the stored direction does not contain a concrete value.

			- ``true`` is returned if ether cardinal direction (:cpp:enumerator:`Type::VALUE_CARDINAL`) or value in degrees (:cpp:enumerator:`Type::VALUE_DEGREES`) is stored.

			- ``false`` is returned if the type of the value is :cpp:enumerator:`Type::NOT_REPORTED`, :cpp:enumerator:`Type::VARIABLE`, :cpp:enumerator:`Type::NDV`, :cpp:enumerator:`Type::OVERHEAD`, :cpp:enumerator:`Type::ALQDS`, or :cpp:enumerator:`Type::UNKNOWN`.

	.. cpp:function:: static std::vector<Direction> sectorDirectionsToVector(const Direction & dir1, const Direction & dir2)

		:returns: ``std::vector`` of the all directions included in the direction sector.

			.. note:: Direction sector is defined clockwise from dir1 to dir2.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if stored direction value is valid, and ``false`` otherwise. 

			The direction value is considered to be valid if value in degrees was specified and the value is less or equal than 360 degrees. If stored direction does not contain a value, it is always considered valid.

.. cpp:namespace-pop::


Formats
-------

Direction can be specified as a numeric value in degrees, as a cardinal direction, or as a special direction value.


Numeric value in degrees
^^^^^^^^^^^^^^^^^^^^^^^^

Metaf recognises numeric values in degrees, in range 000 to 360. Typically last digit of the value is zero.

In some cases ``///`` may be specified instead of three-digit value. This is interpreted by Metaf as non-reported value (:cpp:enumerator:`Type::NOT_REPORTED`).


Cardinal directions
^^^^^^^^^^^^^^^^^^^

Direction may be specified as cardinal direction. The following cardinal and intercardinal directions are recognised by Metaf: ``N``, ``S``, ``W``, ``E``, ``NW``, ``SW``, ``NE``, ``SE``.


Special direction values
^^^^^^^^^^^^^^^^^^^^^^^^

The following special direction values are recognised by Metaf.

======= ===============================================
Value   :cpp:enum:`Direction::Type`
======= ===============================================
///     :cpp:enumerator:`Direction::Type::NOT_REPORTED`
VRB     :cpp:enumerator:`Direction::Type::VARIABLE`
NDV     :cpp:enumerator:`Direction::Type::NDV`
OHD     :cpp:enumerator:`Direction::Type::OVERHEAD`
ALQDS   :cpp:enumerator:`Direction::Type::ALQDS`
UNKNOWN :cpp:enumerator:`Direction::Type::UNKNOWN`
======= ===============================================
