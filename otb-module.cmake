set(DOCUMENTATION "Dimensionality reduction application")
otb_module(CbDimensionalityReduction
	DEPENDS
		OTBCommon
		OTBApplicationEngine
		OTBITK
		OTBShark	
		OTBBoost
		OTBAppClassification
		OTBSOM
		OTBLearningBase
	DESCRIPTION 
		"${DOCUMENTATION}"
)
