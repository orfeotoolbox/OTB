#goes ignored. The solution for me was to add a .i with just the
#following:

#%module swigruntime

#And compile it without -c, while compiling the others with -c. Then
#using -DSWIG_GLOBAL in my CFLAGS for everything. I expected I might
#get some linker clashes from this but it went smoothly. I'll have to
#dig around the automake docs to see if i can set it for just the
#swigruntime.cc.

SET_SOURCE_FILES_PROPERTIES(${WrapOTB_BINARY_DIR}/SwigRuntime/swigrunTcl.cxx GENERATED)
SET_SOURCE_FILES_PROPERTIES(${WrapOTB_BINARY_DIR}/SwigRuntime/swigrunPython.cxx GENERATED)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

IF(OTB_CSWIG_TCL)
  ADD_LIBRARY(SwigRuntimeTcl SHARED ${WrapOTB_BINARY_DIR}/SwigRuntime/swigrunTcl.cxx)
  IF(OTB_LIBRARY_PROPERTIES)
    SET_TARGET_PROPERTIES(SwigRuntimeTcl PROPERTIES LINK_FLAGS "${CSWIG_EXTRA_LINKFLAGS}" ${OTB_LIBRARY_PROPERTIES})
  ELSE(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(SwigRuntimeTcl PROPERTIES LINK_FLAGS "${CSWIG_EXTRA_LINKFLAGS}")
  ENDIF(OTB_LIBRARY_PROPERTIES)
  TARGET_LINK_LIBRARIES(SwigRuntimeTcl ${TCL_LIBRARY})
  INSTALL(TARGETS SwigRuntimeTcl
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(OTB_CSWIG_TCL)

IF(OTB_CSWIG_PYTHON)
  ADD_LIBRARY(SwigRuntimePython SHARED ${WrapOTB_BINARY_DIR}/SwigRuntime/swigrunPython.cxx)
  TARGET_LINK_LIBRARIES(SwigRuntimePython ${PYTHON_LIBRARY})
  IF(OTB_LIBRARY_PROPERTIES)
    SET_TARGET_PROPERTIES(SwigRuntimePython PROPERTIES ${OTB_LIBRARY_PROPERTIES})
  ENDIF(OTB_LIBRARY_PROPERTIES)
  INSTALL(TARGETS SwigRuntimePython
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(OTB_CSWIG_PYTHON)

IF(OTB_CSWIG_PERL)
  ADD_LIBRARY(SwigRuntimePerl SHARED ${WrapOTB_BINARY_DIR}/SwigRuntime/swigrunPerl.cxx)
  TARGET_LINK_LIBRARIES(SwigRuntimePerl "${PERL_LIBRARY}")
  IF(OTB_LIBRARY_PROPERTIES)
    SET_TARGET_PROPERTIES(SwigRuntimePerl PROPERTIES ${OTB_LIBRARY_PROPERTIES})
  ENDIF(OTB_LIBRARY_PROPERTIES)
  INSTALL(TARGETS SwigRuntimePerl
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(OTB_CSWIG_PERL)

MACRO(CREATE_SWIG_RUNTIME LangOption LibName LangPostFix)
     ADD_CUSTOM_COMMAND(
       SOURCE ${WrapOTB_SOURCE_DIR}/SwigRuntime/swigrun.h
       COMMAND ${GCCXML}
       ARGS -fxml-start=_cable_ -DCABLE_CONFIGURATION ${WrapOTB_SOURCE_DIR}/SwigRuntime/swigrun.h
          -fxml=${WrapOTB_BINARY_DIR}/SwigRuntime/swigrun.xml 
       TARGET ${LibName}
       OUTPUTS ${WrapOTB_BINARY_DIR}/SwigRuntime/swigrun.xml
       DEPENDS ${GCCXML})
     SET(XML_COMMAND_ADDED 1)
   ADD_CUSTOM_COMMAND(
    SOURCE ${WrapOTB_BINARY_DIR}/SwigRuntime/swigrun.xml
    COMMAND ${CSWIG}
    ARGS -o ${WrapOTB_BINARY_DIR}/SwigRuntime/swigrun${LangPostFix}.cxx ${LangOption}
         -c++ ${WrapOTB_BINARY_DIR}/SwigRuntime/swigrun.xml
    TARGET ${LibName}
    OUTPUTS ${WrapOTB_BINARY_DIR}/SwigRuntime/swigrun${LangPostFix}.cxx
    DEPENDS ${CSWIG})
ENDMACRO(CREATE_SWIG_RUNTIME)

IF(OTB_CSWIG_TCL)
   CREATE_SWIG_RUNTIME(-tcl SwigRuntimeTcl Tcl)
ENDIF(OTB_CSWIG_TCL)

IF(OTB_CSWIG_PYTHON)
   CREATE_SWIG_RUNTIME(-python SwigRuntimePython Python)
ENDIF(OTB_CSWIG_PYTHON)

IF(OTB_CSWIG_PERL)
   CREATE_SWIG_RUNTIME(-perl5 SwigRuntimePerl Perl)
ENDIF(OTB_CSWIG_PERL)

IF(OTB_CSWIG_JAVA)
  SET_SOURCE_FILES_PROPERTIES(${WrapOTB_BINARY_DIR}/SwigRuntime/JavaCWDJava.cxx GENERATED)
  ADD_LIBRARY(SwigRuntimeJava MODULE ${WrapOTB_BINARY_DIR}/SwigRuntime/JavaCWDJava.cxx JavaCWD.cxx)
  IF(OTB_LIBRARY_PROPERTIES)
    SET_TARGET_PROPERTIES(SwigRuntimeJava PROPERTIES ${OTB_LIBRARY_PROPERTIES})
  ENDIF(OTB_LIBRARY_PROPERTIES)
  INSTALL(TARGETS SwigRuntimeJava
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
  MAKE_DIRECTORY(${WrapOTB_BINARY_DIR}/Java/InsightToolkit)
  ADD_CUSTOM_COMMAND(
        COMMENT "run native swig on SwigExtras.i"
        SOURCE ${WrapOTB_SOURCE_DIR}/SwigRuntime/JavaCWD.i
        COMMAND ${CSWIG} 
        ARGS -nocable -noruntime ${IGNORE_WARNINGS} -o ${WrapOTB_BINARY_DIR}/SwigRuntime/JavaCWDJava.cxx
        -outdir ${WrapOTB_BINARY_DIR}/Java/InsightToolkit
        -package InsightToolkit
        -java -c++ ${WrapOTB_SOURCE_DIR}/SwigRuntime/JavaCWD.i
        TARGET ${LIBRARY_NAME}Java
        OUTPUTS ${WrapOTB_BINARY_DIR}/SwigRuntime/JavaCWDJava.cxx
        DEPENDS ${WrapOTB_SOURCE_DIR}/SwigRuntime/JavaCWD.i ${CSWIG})
ENDIF(OTB_CSWIG_JAVA)

