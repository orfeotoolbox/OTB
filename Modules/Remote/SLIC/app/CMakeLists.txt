message(${${otb-module}_LIBRARIES})
OTB_CREATE_APPLICATION(
  NAME SLIC
  SOURCES 
  otbSLIC.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES}
)
