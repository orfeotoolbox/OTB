PROJECT(Hyperspectral)

cmake_minimum_required(VERSION 2.6)

IF(NOT LIBRARY_OUTPUT_PATH)
   SET (LIBRARY_OUTPUT_PATH ${Hyperspectral_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries." FORCE)
ENDIF(NOT LIBRARY_OUTPUT_PATH)
IF(NOT EXECUTABLE_OUTPUT_PATH)
   SET (EXECUTABLE_OUTPUT_PATH ${Hyperspectral_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables." FORCE)
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)

INCLUDE_REGULAR_EXPRESSION("^.*$")

OPTION(BUILD_SHARED_LIBS "Build OTB with shared libraries." ON)

# Find OTB and load its settings.
FIND_PACKAGE(OTB)
  IF(OTB_FOUND)
   INCLUDE(${OTB_USE_FILE})
  ELSE(OTB_FOUND)
    MESSAGE(FATAL_ERROR "OTB not found.  Please set OTB_DIR")
ENDIF(OTB_FOUND)

OPTION(OTB_USE_CUBLAS "Use Cuda CUBLAS library." OFF)
IF(OTB_USE_CUBLAS)
  FIND_PACKAGE(CUDA)
  INCLUDE_DIRECTORIES(${CUDA_INCLUDE_DIRS})
ENDIF(OTB_USE_CUBLAS)

INCLUDE_DIRECTORIES(
${Hyperspectral_SOURCE_DIR}/Code/BasicFilters
${Hyperspectral_SOURCE_DIR}/Code/BasicFilters/GPUCuda
${Hyperspectral_SOURCE_DIR}/Code/Hyperspectral
${Hyperspectral_SOURCE_DIR}/Code/Vahine
${Hyperspectral_SOURCE_DIR}/Code/RX
)

ADD_SUBDIRECTORY(Code)
ADD_SUBDIRECTORY(Application)

ENABLE_TESTING()
ADD_SUBDIRECTORY(Testing)
