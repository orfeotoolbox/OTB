OTB_CREATE_APPLICATION(
NAME CbDimensionalityReductionTrainer
SOURCES cbDimensionalityReductionTrainer.cxx
LINK_LIBRARIES ${${otb-module}_LIBRARIES} ${OTBCommon_LIBRARIES} ${OTBITK_LIBRARIES} ${OTBBoost_LIBRARIES} ${OTBShark_LIBRARIES} 
)

OTB_CREATE_APPLICATION(
NAME CbDimensionalityReduction
SOURCES cbDimensionalityReduction.cxx
LINK_LIBRARIES ${${otb-module}_LIBRARIES} ${OTBCommon_LIBRARIES} ${OTBITK_LIBRARIES} ${OTBBoost_LIBRARIES} ${OTBShark_LIBRARIES} 
)

OTB_CREATE_APPLICATION(
NAME CbDimensionalityReductionVector
SOURCES cbDimensionalityReductionVector.cxx
LINK_LIBRARIES ${${otb-module}_LIBRARIES} ${OTBCommon_LIBRARIES} ${OTBITK_LIBRARIES} ${OTBBoost_LIBRARIES} ${OTBShark_LIBRARIES} 
)
