
find_package(OTB REQUIRED)
include(${OTB_USE_FILE})

add_executable(completionGenerator completionGenerator.cxx)
target_link_libraries(completionGenerator ${OTB_LIBRARIES})

add_custom_target(generate-completion
    COMMAND bin/completionGenerator lib/otb/applications otbcli_completion
    DEPENDS completionGenerator
    WORKING_DIRECTORY ${OTB_BINARY_DIR}
    )

