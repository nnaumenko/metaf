Group
=====

.. cpp:namespace-push:: metaf

Group is an ``std::variant`` which holds all group classes. It is used by :cpp:class:`metaf::Parser` to return the results of report parsing (see :cpp:class:`metaf::ParseResult`).

Group is defined as follows.

.. cpp:type:: Group = std::variant<KeywordGroup, LocationGroup, ReportTimeGroup, TrendGroup, WindGroup, VisibilityGroup, CloudGroup, WeatherGroup, TemperatureGroup, PressureGroup, RunwayStateGroup, SeaSurfaceGroup, MinMaxTemperatureGroup, PrecipitationGroup, LayerForecastGroup, PressureTendencyGroup, CloudTypesGroup, LowMidHighCloudGroup, LightningGroup, VicinityGroup, TerrainGroup, MiscGroup, UnknownGroup>

