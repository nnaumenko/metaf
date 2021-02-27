KeywordGroup
============

.. cpp:namespace-push:: metaf

KeywordGroup class
------------------

	.. cpp:class:: KeywordGroup

		KeywordGroup represents standard keywords such as ``METAR``, ``SPECI``, ``TAF``, ``COR``, ``RMK``, ``AUTO``, ``CAVOK``, etc.

		For example, report type groups ``METAR``, ``SPECI`` or ``TAF`` at the beginning of the report or CAVOK in the report body are always used in the same form.

	.. cpp:namespace-push:: KeywordGroup

Type definitions
^^^^^^^^^^^^^^^^

	.. cpp:enum-class:: Type

		Designates the keyword which is represented by this group.

		.. cpp:enumerator:: METAR

			Represents group ``METAR``.

			Specifies that the report type is METAR (weather observation) and this is a scheduled report.

		.. cpp:enumerator:: SPECI

			Represents group ``SPECI``.

			Specifies that the report type is METAR (weather observation) and this is an unscheduled report.

			Unscheduled report is issued dut to sudden changes in weather conditions: wind shift, visibility decrease, severe weather, clouds formed or dissipated, etc.

		.. cpp:enumerator:: TAF

			Represents group ``TAF``.

			Specifies that the report type is TAF (weather forecast).

		.. cpp:enumerator:: AMD

			Represents group ``AMD``.

			Specifies an amended report.

			This group is only used in TAF reports.

		.. cpp:enumerator:: COR

			Represents group ``COR``.

			Specifies a correctional report.

		.. cpp:enumerator:: NIL

			Represents group ``NIL``.

			Specifies a missing report.

			No report body is allowed after this group.

		.. cpp:enumerator:: CNL

			Represents group ``CNL``.

			Specifies a cancelled report.

			No report body is allowed after this group.

			This group is only used in TAF reports.

		.. cpp:enumerator:: AUTO

			Represents group ``AUTO``.

			Specifies a fully automated report produced with no human intervention or oversight.

			This group is only used in METAR reports.

		.. cpp:enumerator:: CAVOK

			Represents group ``CAVOK``.

			Ceiling and visibility OK; all of the following conditions are met:

				- Visibility 10 km or more in all directions.

				- No cloud below 5000 feet (1500 meters).

				- No cumulonimbus or towering cumulus clouds.

				- no significant weather phenomena.

		.. cpp:enumerator:: RMK

			Represents group ``RMK``.

			This group designates the beginning of the remarks.

			Remarks may contain plain-language, manual and automatically generated texts. Remarks typically augment information provided in the METAR or TAF report body.

		.. cpp:enumerator:: MAINTENANCE_INDICATOR

			Represents group ``$``.

			This group indicates that one ore more systems of automated station require maintenance.

		.. cpp:enumerator:: AO1

			Represents group ``AO1`` (or alternative spelling ``A01``).

			Indicates an automated station without precipitation discriminator.

		.. cpp:enumerator:: AO2

			Represents group ``AO2`` (or alternative spelling ``A02``).

			Indicates an automated station with precipitation discriminator.

		.. cpp:enumerator:: AO1A

			Represents group ``AO1A`` (or alternative spelling ``A01A``).

			Indicates an automated station without precipitation discriminator and denotes an automated observation augmented by a human observer.

		.. cpp:enumerator:: AO2A

			Represents group ``AO2A`` (or alternative spelling ``A02A``).

			Indicates an automated station with precipitation discriminator and denotes an automated observation augmented by a human observer.

		.. cpp:enumerator:: NOSPECI

			Represents group ``NOSPECI``.

			Indicates a manual station where SPECI (unscheduled) reports are not issued.


Acquiring group data
^^^^^^^^^^^^^^^^^^^^

	.. cpp:function:: Type type() const

		:returns: Type of the keyword group.


Validating
^^^^^^^^^^

	.. cpp:function:: bool isValid() const

		:returns: Always returns ``true``.


Description
-----------

Parsing
^^^^^^^

Keywords are recognised as such only in (:cpp:enum:`ReportPart`) where they are expected to occur, for example if keyword ``AUTO`` is encountered in TAF report, it will not be recognised as a keyword because ``AUTO`` indicates automated METAR report and makes no sense in TAF.

The table below provides the summary of :cpp:enum:`ReportPart` where each keyword is recognised.

======= ==================================== =================================== ================================= =================================
Keyword :cpp:enumerator:`ReportPart::HEADER` :cpp:enumerator:`ReportPart::METAR` :cpp:enumerator:`ReportPart::TAF` :cpp:enumerator:`ReportPart::RMK`
======= ==================================== =================================== ================================= =================================
METAR   ✔                                    ✘                                   ✘                                 ✘                                
SPECI   ✔                                    ✘                                   ✘                                 ✘                                
TAF     ✔                                    ✘                                   ✘                                 ✘                                
AMD     ✔                                    ✘                                   ✘                                 ✘                                
NIL     ✔                                    ✔                                   ✔                                 ✘                                
CNL     ✔                                    ✔                                   ✔                                 ✘                                
COR     ✔                                    ✔                                   ✘                                 ✘                                
AUTO    ✘                                    ✔                                   ✘                                 ✘                                
CAVOK   ✘                                    ✔                                   ✔                                 ✘                                
RMK     ✔                                    ✔                                   ✔                                 ✘                                
$       ✔                                    ✔                                   ✔                                 ✔                                
AO1     ✘                                    ✘                                   ✘                                 ✔                                
AO2     ✘                                    ✘                                   ✘                                 ✔                                
AO1A    ✘                                    ✘                                   ✘                                 ✔                                
AO2A    ✘                                    ✘                                   ✘                                 ✔                                
NOSPECI ✘                                    ✘                                   ✘                                 ✔                                
======= ==================================== =================================== ================================= =================================


Regional variations
^^^^^^^^^^^^^^^^^^^

``CAVOK`` is not used in North America.

``AO1``, ``AO1A``, ``AO2``, ``AO2A`` and ``NOSPECI`` are used only in North America.

``AO1``, ``AO1A``, ``AO2``, ``AO2A`` are commonly specified using zero instead of letter ``O``: ``A01``, ``A01A``, ``A02``, ``A02A``. Metaf parses these alternative groups exactly as the standard groups.
