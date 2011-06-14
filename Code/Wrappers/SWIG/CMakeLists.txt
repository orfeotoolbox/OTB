
macro(check_PIC_flag Language)
  if ( UNIX AND NOT APPLE )
    if ( NOT ${CMAKE_CXX_FLAGS} MATCHES "-fPIC")
      message ( FATAL_ERROR "${Language} wrapping requires CMAKE_CXX_FLAGS (or equivalent) to include -fPIC and ITK built with this flag" )
    endif()
  endif()
endmacro()


#
# Setup the option for each laguage
#
option ( WRAP_LUA "Wrap Lua" OFF )


find_package ( PythonLibs )
find_package ( PythonInterp )
if ( PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND )
  set( WRAP_PYTHON_DEFAULT ON )
else ( PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND )
  set( WRAP_PYTHON_DEFAULT OFF )
endif ( PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND )
check_PIC_flag ( Python )
option( WRAP_PYTHON "Wrap Python" ${WRAP_PYTHON_DEFAULT} )

find_package ( Java COMPONENTS Development Runtime)
find_package ( JNI  )
if ( JAVA_FOUND AND JNI_FOUND )
  set( WRAP_JAVA_DEFAULT ON )
else ( ${JAVA_FOUND} AND JNI_FOUND )
  set( WRAP_JAVA_DEFAULT OFF )
endif ( JAVA_FOUND AND JNI_FOUND )
check_PIC_flag ( Java )
option ( WRAP_JAVA "Wrap Java" ${WRAP_JAVA_DEFAULT} )

find_package ( TCL )
if ( TCL_FOUND )
  set ( WRAP_TCL_DEFAULT ON )
else ( TCL_FOUND )
  set ( WRAP_TCL_DEFAULT OFF )
endif ( TCL_FOUND )
#option ( WRAP_TCL "Wrap Tcl" ${WRAP_TCL_DEFAULT} )
option ( WRAP_TCL "Wrap Tcl" OFF )

find_package ( Ruby )
if ( RUBY_FOUND )
  set ( WRAP_RUBY_DEFAULT ON )
else ( RUBY_FOUND )
  set ( WRAP_RUBY_DEFAULT OFF )
endif ( RUBY_FOUND )
check_PIC_flag ( Ruby )
#option ( WRAP_RUBY "Wrap Ruby" ${WRAP_RUBY_DEFAULT} )
option ( WRAP_RUBY "Wrap Ruby" OFF )

#
# General SWIG configuration
#
if ( WRAP_LUA OR WRAP_PYTHON OR WRAP_JAVA OR WRAP_CSHARP OR WRAP_TCL OR WRAP_R OR WRAP_RUBY )
  find_package ( SWIG 2 REQUIRED )
  include ( UseSWIGLocal )
  set_source_files_properties ( otbApplication.i PROPERTIES CPLUSPLUS ON )
  file ( GLOB SWIG_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*/*.h)

  set ( SWIG_HEADERS ${SWIG_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/itkBase.i
    ${CMAKE_CURRENT_SOURCE_DIR}/itkMacro.i
    ${CMAKE_CURRENT_SOURCE_DIR}/RefCountMacro.i

    ${CMAKE_CURRENT_SOURCE_DIR}/Python.i
    ${CMAKE_CURRENT_SOURCE_DIR}/Java.i
#    ${CMAKE_CURRENT_SOURCE_DIR}/CSharp.i
#    ${CMAKE_CURRENT_SOURCE_DIR}/CSharpTypemapHelper.i
    ${CMAKE_CURRENT_SOURCE_DIR}/Lua.i
#    ${CMAKE_CURRENT_SOURCE_DIR}/Tcl.i
     )

  set ( SWIG_MODULE_otbApplication_EXTRA_DEPS ${SWIG_HEADERS} )
  
  include_directories ( ${CMAKE_CURRENT_SOURCE_DIR} )

endif()

#
# python SWIG configuration
#
if ( WRAP_PYTHON )
  find_package ( PythonLibs REQUIRED )
  find_package ( PythonInterp REQUIRED )
  include_directories ( ${PYTHON_INCLUDE_DIR} )

  # Run swig
  set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS})
  set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
  set(SWIG_MODULE_otbApplication_EXTRA_DEPS OTBWrapperCore ${SWIG_HEADERS})
  SWIG_add_module ( otbApplication python otbApplication.i otbApplicationPYTHON_wrap.cxx )
  SWIG_link_libraries ( otbApplication ${PYTHON_LIBRARIES} OTBWrapperCore)
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/otbApplicationPYTHON_wrap.cxx COMPILE_FLAGS "-w")

endif()

#
# JAVA SWIG configuration
#
if ( WRAP_JAVA )
  find_package ( Java REQUIRED )
  find_package ( JNI REQUIRED )
  include_directories ( ${JAVA_INCLUDE_PATH} ${JNI_INCLUDE_DIRS} )

  # Make sure the nested directory structure exists
  set(JAVA_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/org/otb/application)
  set(JAVA_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)
  file(MAKE_DIRECTORY ${JAVA_SOURCE_DIRECTORY})
  file(MAKE_DIRECTORY ${JAVA_BINARY_DIRECTORY})

  # Nicely write the bridge code in org/itk/simple
  set(CMAKE_SWIG_OUTDIR ${JAVA_SOURCE_DIRECTORY})
  set(CMAKE_SWIG_FLAGS -package "org.otb.application" ${CMAKE_SWIG_GLOBAL_FLAGS})
  set(SWIG_MODULE_otbApplicationJava_EXTRA_DEPS OTBWrapperCore  ${SWIG_HEADERS})
  SWIG_add_module ( otbApplicationJava java otbApplication.i otbApplicationJAVA_wrap.cxx)
  SWIG_link_libraries(otbApplicationJava OTBWrapperCore)
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/otbApplicationJAVA_wrap.cxx COMPILE_FLAGS "-w")

  # Add target for org.otb.Application.jar
  add_custom_target(org_otb_Application_jar ALL DEPENDS org.otb.application.jar)

  # Add custom command and target to compile the generated files and put them in a jar file
  # Make sure the commands depend on the output library from SWIG
  add_custom_command(
    OUTPUT org.otb.application.jar
    COMMENT "Creating jar file..."
    COMMAND ${Java_JAVAC_EXECUTABLE} -d ${JAVA_BINARY_DIRECTORY} ${JAVA_SOURCE_DIRECTORY}/*.java
    COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/org.otb.application.jar -C ${JAVA_BINARY_DIRECTORY} org
    DEPENDS ${SWIG_MODULE_otbApplication_REAL_NAME} otbApplicationJava OTBWrapperCore
    )

  # Get the location of the extension directory
  string(REGEX REPLACE "include"  "jre/lib/ext" JAVA_EXTENSION_DIR ${JAVA_INCLUDE_PATH} )

  # Add the install target
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.otb.application.jar DESTINATION ${JAVA_EXTENSION_DIR})

endif()

#
# lua SWIG configuration
#
if ( WRAP_LUA )
  option ( USE_SYSTEM_LUA "Use a system provided lua" OFF )

#  if ( USE_SYSTEM_LUA )
    find_package ( Lua51 REQUIRED )
    include_directories ( ${LUA_INCLUDE_DIR} )
    set ( LUA_LIB ${LUA_LIBRARIES} )
#  else()
#    set ( LUA_LIB lua5 )
#    include_directories ( ${otbApplication_SOURCE_DIR}/Utilities/lua-5.1.4/src )
#  endif()


  # Run swig
  set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS})
  set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
  set(SWIG_MODULE_otbApplicationLua_EXTRA_DEPS OTBWrapperCore ${SWIG_HEADERS})
  SWIG_module_initialize ( otbApplicationLua lua )
  SWIG_add_source_to_module ( otbApplicationLua swig_generated_source otbApplication.i ${SWIG_HEADERS} )
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/otbApplicationLUA_wrap.cxx COMPILE_FLAGS "-w")

  add_executable ( otbApplicationLua otbApplicationLuaMain.cxx otbApplicationLUA_wrap.cxx )
  target_link_libraries ( otbApplicationLua ${LUA_LIB} OTBWrapperCore )

  if ( UNIX )
    target_link_libraries ( otbApplicationLua curses )
  endif()

endif()

#
# ruby SWIG configuration
#
if ( WRAP_RUBY )
  find_package ( Ruby REQUIRED )
  include_directories ( ${RUBY_INCLUDE_DIRS} )

  # Run swig
  set(CMAKE_SWIG_FLAGS -autorename -module otbapplication ${CMAKE_SWIG_GLOBAL_FLAGS})
  set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
  set(SWIG_MODULE_otbApplication_EXTRA_DEPS OTBWrapperCore ${SWIG_HEADERS})
  SWIG_MODULE_INITIALIZE ( otbApplication ruby )

  SWIG_add_module ( otbApplication ruby otbApplication.i otbApplicationRUBY_wrap.cxx )
  SWIG_link_libraries ( otbApplication ${RUBY_LIBRARY} OTBWrapperCore)
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/otbApplicationRUBY_wrap.cxx COMPILE_FLAGS "-w")

endif()

