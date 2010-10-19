#
# Current inforamtions:
# Version 2.84 released on April Fools' day, 2007.
# Download site : http://www.csie.ntu.edu.tw/~cjlin/libsvm/
#

PROJECT(OTBSVM)
include(CheckCXXCompilerFlag)

# source files for otbsvm
SET(OTBSVM_SRCS
svm.cpp 
)

ADD_LIBRARY(otbsvm ${OTBSVM_SRCS})
TARGET_LINK_LIBRARIES (otbsvm ITKCommon)
IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(otbsvm PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

# check for OpenMP
if( NOT DEFINED USE_OPENMP OR USE_OPENMP  )

  if( WIN32 )
    CHECK_INCLUDE_FILE(omp.h HAVE_OMP_H)
    if( HAVE_OMP_H )
      check_cxx_compiler_flag(/openmp HAVE_OPENMP)

      if( HAVE_OPENMP )
        message(STATUS "compiling libsvm with openmp support")
        add_definitions("/openmp")
      endif()
    endif()
  elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

    # check if compilers supports -fopenmp
    INCLUDE(CheckCCompilerFlag)
    check_cxx_compiler_flag(-fopenmp HAVE_OPENMP)
    check_library_exists(gomp omp_get_num_threads "" HAS_GOMP_LIB)

    if( HAVE_OPENMP AND HAS_GOMP_LIB )
      message(STATUS "compiling libsvm with openmp support")
      add_definitions("-fopenmp")
      target_link_libraries(otbsvm gomp)
      set(OPENMP_LFLAGS "-lgomp")
    endif()
  endif()
endif()


IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbsvm
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  INSTALL(FILES ${__files}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbsvm
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)

