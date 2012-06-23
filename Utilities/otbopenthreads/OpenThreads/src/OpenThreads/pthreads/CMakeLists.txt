# This file should only be included when using Pthreads

SET(LIB_NAME OpenThreads)
SET(LIB_PUBLIC_HEADERS ${OpenThreads_PUBLIC_HEADERS})

SET(OPENTHREADS_SRCS
        PThread.c++
        PThreadBarrier.c++
        PThreadBarrierPrivateData.h
        PThreadCondition.c++
        PThreadConditionPrivateData.h
        PThreadMutex.c++
        PThreadMutexPrivateData.h
        PThreadPrivateData.h
)
 
ADD_LIBRARY(${LIB_NAME}
    ${OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
    ${OPENTHREADS_SRCS}
)

IF(CMAKE_COMPILER_IS_GNUCXX)
  FOREACH(f ${OPENTHREADS_SRCS} )
        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -w )
  ENDFOREACH(f)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


IF(OPENTHREADS_SONAMES)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES VERSION ${OPENTHREADS_VERSION} SOVERSION ${OPENTHREADS_SOVERSION})
ENDIF(OPENTHREADS_SONAMES)

TARGET_LINK_LIBRARIES(${LIB_NAME}
	${CMAKE_THREAD_LIBS_INIT}
)

# Since we're building different platforms binaries in 
# their respective directories, we need to set the 
# link directory so it can find this location.
LINK_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS ${LIB_NAME}
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

#IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
#  INSTALL(FILES ${OpenThreads_PUBLIC_HEADERS}
#              DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbopenthreads/OpenThreads/include/OpenThreads
#              COMPONENT Development)
#ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)

#commented out# INCLUDE(ModuleInstall OPTIONAL)
