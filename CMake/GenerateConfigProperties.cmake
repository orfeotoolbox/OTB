
file(GLOB otbconfigfileCreation_SRCS "${OTB_SOURCE_DIR}/CMake/GenerateConfigProperties.cpp")
set(otbconfigfile_DEFAULT "${OTB_BINARY_DIR}/otb.conf")

add_executable(GenerateConfigProperties ${otbconfigfileCreation_SRCS})

add_custom_command(
          TARGET GenerateConfigProperties
          POST_BUILD
          COMMAND GenerateConfigProperties
          ARGS "${otbconfigfile_DEFAULT}" "${OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING}" "${OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING}"
          COMMENT "Generating ${otbconfigfile_DEFAULT}" )
