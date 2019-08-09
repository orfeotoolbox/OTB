message(${${otb-module}_LIBRARIES})
OTB_CREATE_APPLICATION(
  NAME TrainAutoContext
  SOURCES 
  otbTrainAutoContext.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES}
  )
OTB_CREATE_APPLICATION(
  NAME ClassifyAutoContext
  SOURCES 
  otbClassifyAutoContext.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES}
  
)
