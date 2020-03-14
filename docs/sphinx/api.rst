#############
API Reference
#############

This section describes the APIs related to parsing METAR or TAF report and reading the data from parsed results.

**************************
Parser and parsing results
**************************

The following groups are used to parse METAR/TAF report and interpret parsing results.

.. toctree::
   :maxdepth: 4

   api_parser
   api_parseresult
   api_reportmetadata
   api_reporttype
   api_reporterror
   api_groupinfo
   api_group
   api_reportpart
   api_groupparser
   api_visitor

*************
Group classes
*************

Each group class represents a particular type of the group which may be included in the METAR/TAF report.

.. toctree::
   :maxdepth: 5

   api_cloudgroup
   api_cloudtypesgroup
   api_keywordgroup
   api_layerforecastgroup
   api_lightninggroup
   api_locationgroup
   api_lowmidhighcloudgroup
   api_minmaxtemperaturegroup
   api_miscgroup
   api_precipitationgroup
   api_pressuregroup
   api_pressuretendencygroup
   api_reporttimegroup
   api_runwaystategroup
   api_seasurfacegroup
   api_temperaturegroup
   api_trendgroup
   api_unknowngroup
   api_vicinitygroup
   api_visibilitygroup
   api_weathergroup
   api_windgroup

**********
Data types
**********

Data types store values representing physical quantities (pressure, temperature, surface friction, etc.) or descriptive states (runway, weather phenomena, etc).

Data values are returned by getter methods of the classes which represent groups included in METAR/TAF reports. 

.. toctree::
   :maxdepth: 3

   api_cloudtype
   api_direction
   api_distance
   api_metaftime
   api_precipitation
   api_pressure
   api_runway
   api_speed
   api_surfacefriction
   api_temperature
   api_waveheight
   api_weatherphenomena
