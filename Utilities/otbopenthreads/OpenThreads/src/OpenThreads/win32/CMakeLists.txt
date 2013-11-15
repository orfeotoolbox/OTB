# This file should only be included when WIN32

set(LIB_NAME OpenThreads)
set(LIB_PUBLIC_HEADERS ${OpenThreads_PUBLIC_HEADERS})

source_group("Header Files" FILES ${LIB_PUBLIC_HEADERS})
set_source_files_properties(${LIB_PUBLIC_HEADERS} PROPERTIES HEADER_FILE_ONLY ON)

add_library(${LIB_NAME}
    ${OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
	HandleHolder.h
	Win32BarrierPrivateData.h
	WIN32Condition.cpp
	Win32Condition.h
	Win32ConditionPrivateData.h
	Win32Mutex.cpp
	Win32MutexPrivateData.h
	Win32Thread.cpp
	Win32ThreadBarrier.cpp
	Win32ThreadPrivateData.h
)


if(OPENTHREADS_SONAMES)
  set_target_properties(${LIB_NAME} PROPERTIES VERSION ${OPENTHREADS_VERSION} SOVERSION ${OPENTHREADS_SOVERSION})
endif(OPENTHREADS_SONAMES)

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
