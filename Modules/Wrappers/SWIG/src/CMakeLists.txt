#
# General SWIG configuration
#
if ( OTB_WRAP_PYTHON OR OTB_WRAP_JAVA )
  include ( UseSWIGLocal )
  set_source_files_properties ( otbApplication.i PROPERTIES CPLUSPLUS ON )

  list(APPEND SWIG_EXTRA_DEPS
       otbWrapperSWIGIncludes.h
       itkBase.includes
       ${CMAKE_CURRENT_SOURCE_DIR}/itkBase.i
       ${CMAKE_CURRENT_SOURCE_DIR}/itkMacro.i
       )

  include_directories ( ${CMAKE_CURRENT_SOURCE_DIR} )
endif()


#
# Python SWIG configuration
#
if ( OTB_WRAP_PYTHON )
  include_directories ( ${PYTHON_INCLUDE_DIR} )

  # Run swig
  set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS})
  if(NUMPY_FOUND)
  list(APPEND CMAKE_SWIG_FLAGS  "-DOTB_SWIGNUMPY=1")
  endif()
  set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
  set(SWIG_MODULE_otbApplication_EXTRA_DEPS
       ${CMAKE_CURRENT_SOURCE_DIR}/Python.i
       ${CMAKE_CURRENT_SOURCE_DIR}/PyCommand.i
       itkPyCommand.h
       OTBApplicationEngine)
  SWIG_add_module( otbApplication python otbApplication.i otbApplicationPYTHON_wrap.cxx itkPyCommand.cxx )
  SWIG_link_libraries( otbApplication ${PYTHON_LIBRARIES} OTBApplicationEngine )
  set_source_files_properties( ${CMAKE_CURRENT_BINARY_DIR}/otbApplicationPYTHON_wrap.cxx COMPILE_FLAGS "-w" )

  # byte-compile the resulting python file
  add_custom_command(
      TARGET _otbApplication
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E echo "Byte-compiling otbApplication.py"
      COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/CMake/PythonCompile.py otbApplication.py
      DEPENDS _otbApplication
    )

  otb_module_target_label( _otbApplication )

  install( TARGETS _otbApplication
           DESTINATION ${OTB_INSTALL_PYTHON_DIR}
           COMPONENT RuntimeLibraries )

  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/otbApplication.py
                 ${CMAKE_CURRENT_BINARY_DIR}/otbApplication.pyc
           DESTINATION ${OTB_INSTALL_PYTHON_DIR}
           COMPONENT RuntimeLibraries )

endif()

#
# JAVA SWIG configuration
#
if ( OTB_WRAP_JAVA )
  include_directories ( ${JAVA_INCLUDE_PATH} ${JNI_INCLUDE_DIRS} )

  # Make sure the nested directory structure exists
  set(JAVA_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/org/otb/application)
  set(JAVA_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)
  file(MAKE_DIRECTORY ${JAVA_SOURCE_DIRECTORY})
  file(MAKE_DIRECTORY ${JAVA_BINARY_DIRECTORY})

  # Nicely write the bridge code in org/itk/simple
  set(CMAKE_SWIG_OUTDIR ${JAVA_SOURCE_DIRECTORY})
  set(CMAKE_SWIG_FLAGS -package "org.otb.application" ${CMAKE_SWIG_GLOBAL_FLAGS})
  set(SWIG_MODULE_otbApplicationJava_EXTRA_DEPS
       ${CMAKE_CURRENT_SOURCE_DIR}/Java.i
       OTBApplicationEngine)
  SWIG_add_module ( otbApplicationJava java otbApplication.i otbApplicationJAVA_wrap.cxx)
  SWIG_link_libraries(otbApplicationJava OTBApplicationEngine )
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/otbApplicationJAVA_wrap.cxx COMPILE_FLAGS "-w")

  # Add target for org.otb.Application.jar
  add_custom_target(org_otb_Application_jar ALL DEPENDS org.otb.application.jar)

  otb_module_target_label( org_otb_Application_jar )

  # Add custom command and target to compile the generated files and put them in a jar file
  # Make sure the commands depend on the output library from SWIG
  add_custom_command(
    OUTPUT org.otb.application.jar
    COMMENT "Creating jar file..."
    COMMAND ${Java_JAVAC_EXECUTABLE} -d ${JAVA_BINARY_DIRECTORY} ${JAVA_SOURCE_DIRECTORY}/*.java
    COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/org.otb.application.jar -C ${JAVA_BINARY_DIRECTORY} org
    DEPENDS ${SWIG_MODULE_otbApplication_REAL_NAME} otbApplicationJava OTBApplicationEngine
    )

  # Get the location of the extension directory
  string(REGEX REPLACE "include"  "jre/lib/ext" JAVA_EXTENSION_DIR ${JAVA_INCLUDE_PATH} )

  # Add the install target
  # -- install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.otb.application.jar DESTINATION ${JAVA_EXTENSION_DIR})
  # Prefer using OTB_INSTALL_JAVA_DIR which defaults to something that honors CMAKE_INSTALL_PREFIX
  # Using the system-wide Java extension dir usually require admin privileges,
  # and shall better be left to packagers.
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.otb.application.jar
          DESTINATION ${OTB_INSTALL_JAVA_DIR}
          COMPONENT RuntimeLibraries )

endif()
