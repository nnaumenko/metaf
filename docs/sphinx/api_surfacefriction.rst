SurfaceFriction
===============

.. cpp:namespace-push:: metaf

SurfaceFriction class
---------------------

	.. cpp:class:: SurfaceFriction

		Surface friction. The intended use is reporting the surface friction of the runway. Surface friction may be reported in the form of friction coefficient or braking action.

		The value may be optionally not reported, or reported as unreliable/unmeasurable.

		Friction coefficient is a value in range [0.00 .. 1.00]. Lesser values mean more slippery surface.

		.. note:: Surface friction coefficient is a dimensionless value and has no associated measurement units.

.. cpp:namespace-push:: SurfaceFriction

Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		The type of surface friction value.

		.. cpp:enumerator:: NOT_REPORTED

			Surface friction is not reported.

		.. cpp:enumerator:: SURFACE_FRICTION_REPORTED

			Surface friction reported in form of friction coefficient.

		.. cpp:enumerator:: BRAKING_ACTION_REPORTED

			Surface friction reported in form of braking action.

		.. cpp:enumerator:: UNRELIABLE

			The measurement result is unreliable or the value is unmeasurable.


	.. cpp:enum-class:: BrakingAction

		Descriptive braking action which specifies approximate range of surface friction coefficient rather than precise coefficient value.

		.. cpp:enumerator:: NONE

			Value is either not reported, unreliable or unmeasurable.

		.. cpp:enumerator:: POOR

			Friction coefficient is 0.25 or lesser.

		.. cpp:enumerator:: MEDIUM_POOR

			Friction coefficient is from 0.26 to 0.29.

		.. cpp:enumerator:: MEDIUM

			Friction coefficient is from 0.30 to 0.35.

		.. cpp:enumerator:: MEDIUM_GOOD

			Friction coefficient is from 0.36 to 0.39.

		.. cpp:enumerator:: GOOD

			Friction coefficient is 0.40 or greater.


Acquiring the data
^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Type of surface friction value.

	.. cpp:function:: std::optional<float> coefficient() const

		:returns: The value of friction coefficient or empty ``std::optional`` if the value is not reported, unreliable or unmeasurable.

			If :cpp:enum:`BrakingAction` was reported, then 'worst' (i.e. least) friction coefficient for the range specified by stored braking action value is returned (e.g. if braking action was reported as :cpp:enumerator:`BrakingAction::MEDIUM_GOOD` then 0.36 is returned).

	.. cpp:function:: BrakingAction brakingAction() const

		:returns: Braking action value corresponding to stored value.

			If friction coefficient was reported, a range of :cpp:enum:`BrakingAction` where this value of the friction coefficient fits is returned (e.g. if friction coefficient with value 0.33 was reported, then :cpp:enumerator:`BrakingAction::MEDIUM` is returned).


Miscellaneous
^^^^^^^^^^^^^

	.. cpp:function:: bool isReported() const

		:returns: ``true`` if the actual value is stored or ``false`` if non-reported value is stored. Corresponds to :cpp:enumerator:`Type::NOT_REPORTED`.


	.. cpp:function:: bool isUnreliable() const

		:returns: ``true`` if the stored value is unmeasurable or the measurement result is unreliable, and ``false`` otherwise. When the value is not reported, ``false`` is returned. Corresponds to :cpp:enumerator:`Type::UNRELIABLE`.

.. cpp:namespace-pop::


Formats
-------

Metaf recognises surface friction as two digits.

Values ``00`` to ``90`` specify the friction coefficient in hundredths, for example value of ``62`` means friction coefficient 0.62. Values ``91`` to ``99`` and ``//`` have special meanings.

The table below gives examples of different surface friction values.

===== =========================== ============ =================================================
Value Meaning                     Coefficient  :cpp:enum:`Type` 
===== =========================== ============ =================================================
00    Friction coefficient        0.00         :cpp:enumerator:`Type::SURFACE_FRICTION_REPORTED`
90    Friction coefficient        0.90         :cpp:enumerator:`Type::SURFACE_FRICTION_REPORTED`
91    Braking action poor         0.25 or less :cpp:enumerator:`Type::BRAKING_ACTION_REPORTED`
92    Braking action medium-poor  0.26 .. 0.29 :cpp:enumerator:`Type::BRAKING_ACTION_REPORTED`
93    Braking action medium       0.30 .. 0.35 :cpp:enumerator:`Type::BRAKING_ACTION_REPORTED`
94    Braking action medium-good  0.36 .. 0.39 :cpp:enumerator:`Type::BRAKING_ACTION_REPORTED`
95    Braking action good         0.40 or more :cpp:enumerator:`Type::BRAKING_ACTION_REPORTED`
96    Reserved value                           not recognised by Metaf
97    Reserved value                           not recognised by Metaf
98    Reserved value                           not recognised by Metaf
99    Unrealiable or unmeasurable              :cpp:enumerator:`Type::UNRELIABLE`
//    Not reported                             :cpp:enumerator:`Type::NOT_REPORTED`
===== =========================== ============ =================================================
