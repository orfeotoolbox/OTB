set(DOCUMENTATION "Dimensionality reduction application")
otb_module(CbDimensionalityReduction
	DEPENDS
		OTBCommon
		OTBApplicationEngine
		OTBITK
		OTBShark	
		OTBBoost
	DESCRIPTION 
		"${DOCUMENTATION}"
)
