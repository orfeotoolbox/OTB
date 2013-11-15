# This file should only be included when using Sproc

set(LIB_NAME OpenThreads)
set(LIB_PUBLIC_HEADERS ${OpenThreads_PUBLIC_HEADERS})

add_library(${LIB_NAME}
    ${OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
	SharedArena.c++
	SharedArena.h
	SprocBarrier.c++
	SprocBarrierPrivateData.h
	SprocCondition.c++
	SprocConditionPrivateData.h
	SprocMutex.c++
	SprocMutexPrivateData.h
	SprocThread.c++
	SprocThreadPrivateActions.h
	SprocThreadPrivateData.h
)

if(OPENTHREADS_SONAMES)
  set_target_properties(${LIB_NAME} PROPERTIES VERSION ${OPENTHREADS_VERSION} SOVERSION ${OPENTHREADS_SOVERSION})
endif(OPENTHREADS_SONAMES)

# Do we need to link against anything for Sproc?
#target_link_libraries(${LIB_NAME}
#)

# Since we're building different platforms binaries in
# their respective directories, we need to set the
# link directory so it can find this location.
link_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS ${LIB_NAME}
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

# OpenThreads headers are not needed by OTB headers, so avoid exporting them
#if(NOT OTB_INSTALL_NO_DEVELOPMENT)
#  install(FILES ${OpenThreads_PUBLIC_HEADERS}
#              DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbopenthreads/OpenThreads/include/OpenThreads
#              COMPONENT Development)
#endif(NOT OTB_INSTALL_NO_DEVELOPMENT)


#commented out# include(ModuleInstall OPTIONAL)
