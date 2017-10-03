set(DOCUMENTATION "Dimensionality reduction application")
otb_module(OTBDimensionalityReductionLearning
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
