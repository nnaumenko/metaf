LowMidHighCloudGroup
====================

.. cpp:namespace-push:: metaf

.. cpp:class:: LowMidHighCloudGroup

	Stores information about predominant cloud types in low, mid, and high cloud layers. This group is included in the remarks and is used in North America.

.. cpp:namespace-push:: LowMidHighCloudGroup

Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: LowLayer

		Type of low-layer clouds.

		.. cpp:enumerator::NONE

			No low-layer clouds.

		.. cpp:enumerator:: CU_HU_CU_FR

			Cumulus humilis or Cumulus fractus of dry weather or both.

		.. cpp:enumerator:: CU_MED_CU_CON

			Cumulus mediocris or congestus, with or without Cumulus of species fractus or humilis or Stratocumulus, all having their bases at the same level.

		.. cpp:enumerator:: CB_CAL

			Cumulonimbus calvus, with or without Cumulus, Stratocumulus or Stratus.

		.. cpp:enumerator:: SC_CUGEN

			Stratocumulus cumulogenitus; Cumulus may also be present.

		.. cpp:enumerator:: SC_NON_CUGEN

			Stratocumulus non-cumulogenitus (not resulting from the spreading out of Cumulus).

		.. cpp:enumerator:: ST_NEB_ST_FR

			Stratus nebulosus or Stratus fractus of dry weather, or both.

		.. cpp:enumerator:: ST_FR_CU_FR_PANNUS

			Stratus fractus or Cumulus fractus of wet weather, or both (pannus), usually below Altostratus or Nimbostratus.

		.. cpp:enumerator:: CU_SC_NON_CUGEN_DIFFERENT_LEVELS

			Cumulus and Stratocumulus other than Stratocumulus cumulogenitus, with bases at different levels.

		.. cpp:enumerator:: CB_CAP

			Cumulonimbus capillatus (often with an anvil), with or without Cumulonimbus calvus, Cumulus, Stratocumulus, Stratus or pannus.

		.. cpp:enumerator:: NOT_OBSERVABLE

			Clouds are not observable due to fog, blowing dust or sand, or other similar phenomena.

	.. cpp:enum-class:: MidLayer

		Type of mid-layer clouds.

		.. cpp:enumerator:: NONE

			No mid-layer clouds.

		.. cpp:enumerator:: AS_TR

			Altostratus translucidus.

		.. cpp:enumerator:: AS_OP_NS

			Altostratus opacus or Nimbostratus.

		.. cpp:enumerator:: AC_TR

			Altocumulus translucidus at a single level.

		.. cpp:enumerator:: AC_TR_LEN_PATCHES

			Patches (often lenticular) of Altocumulus translucidus, continually changing and occurring at one or more levels.

		.. cpp:enumerator:: AC_TR_AC_OP_SPREADING

			Altocumulus translucidus in bands, or one or more layers of Altocumulus translucidus or Altocumulus opacus, progressively invading the sky; these Altocumulus generally thicken as a whole.

		.. cpp:enumerator:: AC_CUGEN_AC_CBGEN

			Altocumulus cumulogenitus (or Altocumulus cumulonimbogenitus).

		.. cpp:enumerator:: AC_DU_AC_OP_AC_WITH_AS_OR_NS

			Altocumulus duplicatus, or Altocumulus opacus in a single layer, not progressively invading the sky, or Altocumulus with Altostratus or Nimbostratus.

		.. cpp:enumerator:: AC_CAS_AC_FLO

			Altocumulus castellanus or Altocumulus floccus.

		.. cpp:enumerator:: AC_OF_CHAOTIC_SKY

			Altocumulus of a chaotic sky, generally at several levels.

		.. cpp:enumerator:: NOT_OBSERVABLE

			Clouds are not observable due to fog, blowing dust or sand, or other similar phenomena or because of a continuous layer of lower clouds.

	.. cpp:enum-class:: HighLayer

		Type of high-layer clouds.

		.. cpp:enumerator:: NONE

			No high-layer clouds.

		.. cpp:enumerator:: CI_FIB_CI_UNC

			Cirrus fibratus and sometimes Cirrus uncinus, not progressively invading the sky.

		.. cpp:enumerator:: CI_SPI_CI_CAS_CI_FLO

			Cirrus spissatus, in patches or entangled sheaves, that usually do not increase and sometimes appear to be the remains of the upper part of a Cumulonimbus; or Cirrus castellanus or Cirrus floccus.

		.. cpp:enumerator:: CI_SPI_CBGEN

			Cirrus spissatus cumulonimbogenitus.

		.. cpp:enumerator:: CI_FIB_CI_UNC_SPREADING

			Cirrus uncinus, Cirrus fibratus or both, progressively invading the sky; they generally thicken as a whole.

		.. cpp:enumerator:: CI_CS_LOW_ABOVE_HORIZON

			Cirrus (often in bands) and Cirrostratus, or Cirrostratus alone, progressively invading the sky; they generally thicken as a whole, but the continuous veil does not reach 45° above the horizon.

		.. cpp:enumerator:: CI_CS_HIGH_ABOVE_HORIZON

			Cirrus (often in bands} and Cirrostratus, or Cirrostratus alone, progressively invading the sky; they generally thicken as a whole; the continuous veil extends more than 45° above the horizon, without the sky being totally covered.

		.. cpp:enumerator:: CS_NEB_CS_FIB_COVERING_ENTIRE_SKY

			Cirrostratus covering the whole sky.

		.. cpp:enumerator:: CS

			Cirrostratus not progressively invading the sky and not entirely covering it.

		.. cpp:enumerator:: CC

			Cirrocumulus alone, or Cirrocumulus predominant among the high-layer clouds.

		.. cpp:enumerator:: NOT_OBSERVABLE

			Clouds are not observable due to fog, blowing dust or sand, or other similar phenomena or because of a continuous layer of lower clouds.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: LowLayer lowLayer() const

		:returns: Predominant type of low-layer clouds.

	.. cpp:function:: MidLayer midLayer() const

		:returns: Predominant type of mid-layer clouds.

	.. cpp:function:: HighLayer highLayer() const

		:returns: Predominant type of high-layer clouds.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: ``true`` if all cloud layers above 'not observable' cloud layer are also 'not observable'; ``false`` otherwise.

.. cpp:namespace-pop::
