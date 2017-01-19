##### check if standalone project ######
if(NOT PROJECT_NAME)
  cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
  project(OTBCompletion)
  find_package(OTB REQUIRED)
  message("OTBCompletion")
  include(${OTB_USE_FILE})
endif()

add_executable(completionGenerator EXCLUDE_FROM_ALL completionGenerator.cxx)
target_link_libraries(completionGenerator ${OTB_LIBRARIES})

add_custom_target(generate-completion
    COMMAND bin/completionGenerator lib/otb/applications otbcli_completion
    DEPENDS completionGenerator
    WORKING_DIRECTORY ${OTB_BINARY_DIR}
    )

