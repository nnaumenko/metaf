Visitor
=======

.. cpp:namespace-push:: metaf

The purpose of Visitor is to simplify processing the results of the parsing.

Since the METAR or TAF report is parsed into the vector of type :cpp:type:`metaf::Group`, and the :cpp:type:`metaf::Group` is a variant data type, in order to handle the results the variant alternative check is required, after which the correct type can be extracted from the variant and the concrete group type can be handled.

The Group Visitor checks the type of an alternative in :cpp:type:`metaf::Group` and calls a corresponding virtual method for the concrete group type. The virtual methods are pure to make sure there is no risk of accidentally missing the handling of a particular group type.

See :doc:`tutorial` for the tutorial which uses a Visitor.

.. cpp:class:: template <typename T> Visitor

	:tparam T: Type returned by virtual methods for concrete group types. Can be ``void`` if virtual methods do not need to return a value.

	.. cpp:function:: T visit(const Group & group, ReportPart reportPart = ReportPart::UNKNOWN, const std::string & rawString = std::string())

		Checks concrete group type and calls one of the virtual methods below.

		:return: Value returned by corresponding virtual method or T() if the suitable method cannot be found for the Group variant alternative.

	.. cpp:function:: T visit(const GroupInfo & groupInfo)

		Checks type of group stored in GroupInfo and calls one of the virtual methods below.

		:return: Value returned by corresponding virtual method or T() if the suitable method cannot be found for the Group variant alternative.

	.. cpp:function:: protected virtual T visitKeywordGroup(const KeywordGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitLocationGroup(const LocationGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitReportTimeGroup(const ReportTimeGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitTrendGroup(const TrendGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitWindGroup(const WindGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitVisibilityGroup(const VisibilityGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitCloudGroup(const CloudGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitWeatherGroup(const WeatherGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitTemperatureGroup(const TemperatureGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitPressureGroup(const PressureGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitRunwayStateGroup(const RunwayStateGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitSeaSurfaceGroup(const SeaSurfaceGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitMinMaxTemperatureGroup(const MinMaxTemperatureGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitPrecipitationGroup(const PrecipitationGroup & group, ReportPart reportPart, const std::string & rawString) = 0
	
	.. cpp:function:: protected virtual T visitLayerForecastGroup(const LayerForecastGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitPressureTendencyGroup(const PressureTendencyGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitCloudTypesGroup(const CloudTypesGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected T visitLowMidHighCloudGroup(const LowMidHighCloudGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected T visitLightningGroup(const LightningGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected T visitVicinityGroup(const VicinityGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitMiscGroup(const MiscGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	.. cpp:function:: protected virtual T visitUnknownGroup(const UnknownGroup & group, ReportPart reportPart, const std::string & rawString) = 0

	These methods are called by :cpp:func:`visit()` for the concrete group types. See :doc:`tutorial` for minimalistic usage example.
