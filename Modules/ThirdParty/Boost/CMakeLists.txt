project(OTBBoost)

set(OTBBoost_SYSTEM_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
set(OTBBoost_INCLUDE_DIRS ${OTBBoost_SOURCE_DIR}/src)

if(Boost_LIBRARIES)
  set(OTBBoost_LIBRARIES "${Boost_LIBRARIES}")
endif()

otb_module_impl()

install(DIRECTORY ${OTBBoost_SOURCE_DIR}/src/boost
  DESTINATION ${OTBBoost_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
