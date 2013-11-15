cmake_minimum_required(VERSION 2.6)
project(HelloWorld)

find_package(OTB)
if(OTB_FOUND)
  include(${OTB_USE_FILE})
else()
  message(FATAL_ERROR
          "Cannot build HelloWorld application without OTB.  Please set OTB_DIR.")
endif()


add_executable(HelloWorld HelloWorld.cxx )

target_link_libraries(HelloWorld OTBIO)
