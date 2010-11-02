
FILE(GLOB otbconfigfileCreation_SRCS "${OTB_SOURCE_DIR}/CMake/GenerateConfigProperties.cpp")
SET(otbconfigfile_DEFAULT "${OTB_BINARY_DIR}/otb.conf")

ADD_EXECUTABLE(GenerateConfigProperties ${otbconfigfileCreation_SRCS})

ADD_CUSTOM_COMMAND(
          TARGET GenerateConfigProperties
          POST_BUILD
          COMMAND GenerateConfigProperties
          ARGS "${otbconfigfile_DEFAULT}" "${OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING}" "${OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING}"
          COMMENT "Generating ${otbconfigfile_DEFAULT}" )
