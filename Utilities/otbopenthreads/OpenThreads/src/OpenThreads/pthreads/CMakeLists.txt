# This file should only be included when using Pthreads

SET(LIB_NAME OpenThreads)
SET(LIB_PUBLIC_HEADERS ${OpenThreads_PUBLIC_HEADERS})

ADD_LIBRARY(${LIB_NAME}
    ${OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
	PThread.c++
	PThreadBarrier.c++
	PThreadBarrierPrivateData.h
	PThreadCondition.c++
	PThreadConditionPrivateData.h
	PThreadMutex.c++
	PThreadMutexPrivateData.h
	PThreadPrivateData.h
)

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

# OTB Modifications
#INSTALL(
#	TARGETS OpenThreads
#	ARCHIVE DESTINATION lib${LIB_POSTFIX}
#	LIBRARY DESTINATION lib${LIB_POSTFIX}
#	RUNTIME DESTINATION bin
#)
INSTALL(TARGETS OpenThreads
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

#INSTALL(
#	FILES ${OpenThreads_PUBLIC_HEADERS}
#	DESTINATION include/OpenThreads
#)
INSTALL(FILES ${OpenThreads_PUBLIC_HEADERS}
	      DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbopenthreads/OpenThreads/include
	      COMPONENT Development)


#commented out# INCLUDE(ModuleInstall OPTIONAL)
