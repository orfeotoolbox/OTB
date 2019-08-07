cmake_minimum_required(VERSION 2.8.9)

project(OTBAutoContext)
list(APPEND CMAKE_MODULE_PATH ${OTBAutoContext_SOURCE_DIR})

# set(OTBAutoContext_LIBRARIES OTBAutoContext)

if(NOT OTB_SOURCE_DIR)
  find_package(OTB REQUIRED)
  list(APPEND CMAKE_MODULE_PATH ${OTB_CMAKE_DIR})
  include(OTBModuleExternal)
else()
  otb_module_impl()
endif() 
