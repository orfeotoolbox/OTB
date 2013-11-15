# This file should only be included when using Pthreads

set(LIB_NAME OpenThreads)
set(LIB_PUBLIC_HEADERS ${OpenThreads_PUBLIC_HEADERS})

set(OPENTHREADS_SRCS
        PThread.c++
        PThreadBarrier.c++
        PThreadBarrierPrivateData.h
        PThreadCondition.c++
        PThreadConditionPrivateData.h
        PThreadMutex.c++
        PThreadMutexPrivateData.h
        PThreadPrivateData.h
)

add_library(${LIB_NAME}
    ${OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
    ${OPENTHREADS_SRCS}
)

if(CMAKE_COMPILER_IS_GNUCXX)
  foreach(f ${OPENTHREADS_SRCS} )
        set_source_files_properties( ${f} PROPERTIES COMPILE_FLAGS -w )
  endforeach()
endif()


if(OPENTHREADS_SONAMES)
  set_target_properties(${LIB_NAME} PROPERTIES VERSION ${OPENTHREADS_VERSION} SOVERSION ${OPENTHREADS_SOVERSION})
endif()

target_link_libraries(${LIB_NAME}
	${CMAKE_THREAD_LIBS_INIT}
)

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
endif()

#if(NOT OTB_INSTALL_NO_DEVELOPMENT)
#  install(FILES ${OpenThreads_PUBLIC_HEADERS}
#              DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbopenthreads/OpenThreads/include/OpenThreads
#              COMPONENT Development)
#endif()

#commented out# include(ModuleInstall OPTIONAL)
