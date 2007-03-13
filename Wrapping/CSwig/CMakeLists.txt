PROJECT(WrapOTB)

IF(OTB_CSWIG_TCL)
  SET(OTB_INCLUDE_DIRS_SYSTEM ${OTB_INCLUDE_DIRS_SYSTEM} ${TCL_INCLUDE_PATH} ${TK_INCLUDE_PATH})
ENDIF(OTB_CSWIG_TCL)

IF(OTB_CSWIG_PYTHON)
  # Python include directory.
  SET(OTB_INCLUDE_DIRS_SYSTEM ${OTB_INCLUDE_DIRS_SYSTEM}
    ${PYTHON_INCLUDE_PATH})
ENDIF(OTB_CSWIG_PYTHON)

IF(OTB_CSWIG_JAVA)
  # Java include directories.
  SET(OTB_INCLUDE_DIRS_SYSTEM ${OTB_INCLUDE_DIRS_SYSTEM}
      ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} ${JAVA_AWT_INCLUDE_PATH})
ENDIF(OTB_CSWIG_JAVA)

# Choose an install location for the Java wrapper libraries.  This
# must be next to the OTBCommon shared library.
IF(WIN32)
  SET(OTB_INSTALL_JAVA_LIBS_DIR /bin)
ELSE(WIN32)
  SET(OTB_INSTALL_JAVA_LIBS_DIR ${OTB_INSTALL_LIB_DIR})
ENDIF(WIN32)

# We have found CableSwig.  Use the settings.
SET(CABLE_INDEX ${CableSwig_cableidx_EXE})
SET(CSWIG ${CableSwig_cswig_EXE})
SET(GCCXML ${CableSwig_gccxml_EXE})

SET(OTB_WRAP_NEEDS_DEPEND 1)
IF(${CMAKE_MAKE_PROGRAM} MATCHES make)
  SET(OTB_WRAP_NEEDS_DEPEND 0)
ENDIF(${CMAKE_MAKE_PROGRAM} MATCHES make)
SET(OTB_TOP ${OTB_SOURCE_DIR})
SET(OTB_SWIG_DEFAULT_LIB ${OTB_TOP}/Utilities/CableSwig/Swig/Lib )

SET(CSWIG_EXTRA_LINKFLAGS )
IF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
  SET(CSWIG_EXTRA_LINKFLAGS "/IGNORE:4049")
ENDIF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")

IF(CMAKE_SYSTEM MATCHES "IRIX.*")
  IF(CMAKE_CXX_COMPILER MATCHES "CC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -woff 1552")
  ENDIF(CMAKE_CXX_COMPILER MATCHES "CC")
ENDIF(CMAKE_SYSTEM MATCHES "IRIX.*")

IF(CMAKE_COMPILER_IS_GNUCXX)
  STRING(REGEX REPLACE "-Wcast-qual" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

SET(IGNORE_WARNINGS -w362 -w389 -w503 -w508 -w509 -w516)
# define macros for wrapping commands
MACRO(GCCXML_CREATE_XML_FILE Source Bin Input Output Library)
# if the make program is not an IDE then include
# the depend file in a way that will make cmake 
# re-run if it changes
   SET(CABLE_SWIG_DEPEND)
   SET(CABLE_SWIG_DEPEND_REGENERATE)
   IF(${CMAKE_MAKE_PROGRAM} MATCHES "make")
     IF(EXISTS ${Bin}/${Output}.depend)
     ELSE(EXISTS ${Bin}/${Output}.depend)
       CONFIGURE_FILE(
         ${OTB_SOURCE_DIR}/Wrapping/CSwig/empty.depend.in
         ${Bin}/${Output}.depend @ONLY IMMEDIATE)
     ENDIF(EXISTS ${Bin}/${Output}.depend)
     INCLUDE(${Bin}/${Output}.depend)
   ELSE(${CMAKE_MAKE_PROGRAM} MATCHES "make")
# for IDE generators like MS dev only include the depend files
# if they exist.   This is to prevent ecessive reloading of
# workspaces after each build.   This also means
# that the depends will not be correct until cmake
# is run once after the build has completed once.
# the depend files are created in the wrap tcl/python sections
# when the .xml file is parsed.
     INCLUDE(${Bin}/${Output}.depend OPTIONAL)
   ENDIF(${CMAKE_MAKE_PROGRAM} MATCHES "make")

   IF(CABLE_SWIG_DEPEND)
     # There are dependencies.  Make sure all the files are present.
     # If not, force the rule to re-run to update the dependencies.
     FOREACH(f ${CABLE_SWIG_DEPEND})
       IF(EXISTS ${f})
       ELSE(EXISTS ${f})
         SET(CABLE_SWIG_DEPEND_REGENERATE 1)
       ENDIF(EXISTS ${f})
     ENDFOREACH(f)
   ELSE(CABLE_SWIG_DEPEND)
     # No dependencies, make the output depend on the dependency file
     # itself, which should cause the rule to re-run.
     SET(CABLE_SWIG_DEPEND_REGENERATE 1)
   ENDIF(CABLE_SWIG_DEPEND)
   IF(CABLE_SWIG_DEPEND_REGENERATE)
     SET(CABLE_SWIG_DEPEND ${Bin}/${Output}.depend)
     CONFIGURE_FILE(
       ${OTB_SOURCE_DIR}/Wrapping/CSwig/empty.depend.in
       ${Bin}/${Output}.depend @ONLY IMMEDIATE)
   ENDIF(CABLE_SWIG_DEPEND_REGENERATE)

   ADD_CUSTOM_COMMAND(
   COMMENT "${Output} from "
   SOURCE ${Source}/${Input}
   COMMAND ${GCCXML}
   ARGS -fxml-start=_cable_
        -fxml=${Bin}/${Output} --gccxml-gcc-options ${SWIG_INC_FILE}
        -DCSWIG -DCABLE_CONFIGURATION ${Source}/${Input}
   TARGET ${Library}
   OUTPUTS ${Bin}/${Output}
   DEPENDS ${GCCXML} ${CABLE_SWIG_DEPEND})
ENDMACRO(GCCXML_CREATE_XML_FILE)


MACRO(CINDEX_CREATE_IDX_FILE Bin Input Output Library)
   ADD_CUSTOM_COMMAND(
   COMMENT "${Output} from "
   SOURCE ${Bin}/${Input}
   COMMAND ${CABLE_INDEX}
   ARGS ${Bin}/${Input} ${Bin}/${Output}
   TARGET ${Library}
   OUTPUTS ${Bin}/${Output} 
   DEPENDS ${Bin}/${Input} ${CABLE_INDEX}
)
ENDMACRO(CINDEX_CREATE_IDX_FILE)

MACRO(CSWIG_CREATE_TCL_CXX_FILE Bin MasterIdx InputIdx InputXml OutputTclCxx Library LibraryIndexFiles)
   SET(CINDEX)
   FOREACH(MIDX ${MasterIdx})
     SET(CINDEX ${CINDEX} -Cindex "${MIDX}")
   ENDFOREACH(MIDX)

   # Need dependency on ${OTB_BINARY_DIR}/itkConfigure.h so
   # package is rebuilt when the OTB version changes.
   ADD_CUSTOM_COMMAND(
     COMMENT "${OutputTclCxx} from "
     SOURCE ${Bin}/${InputIdx}
     COMMAND ${CSWIG}
     ARGS -l${OTB_TOP}/Wrapping/CSwig/itk.swg
          -I${OTB_SWIG_DEFAULT_LIB}
          -I${OTB_SWIG_DEFAULT_LIB}/tcl
          -noruntime ${CINDEX} ${IGNORE_WARNINGS} -depend ${Bin}/${InputXml}.depend
          -o ${Bin}/${OutputTclCxx} -tcl -pkgversion "${OTB_VERSION_STRING}" -c++ ${Bin}/${InputXml}
     TARGET ${Library}
     OUTPUTS ${Bin}/${OutputTclCxx}
     DEPENDS ${LibraryIndexFiles} ${OTB_TOP}/Wrapping/CSwig/itk.swg ${Bin}/${InputXml} ${CSWIG} ${OTB_BINARY_DIR}/itkConfigure.h)
#     MESSAGE("depends are ${CABLE_SWIG_DEPEND}")
ENDMACRO(CSWIG_CREATE_TCL_CXX_FILE)

MACRO(CSWIG_CREATE_PERL_CXX_FILE Bin MasterIdx InputIdx InputXml OutputPerlCxx Library LibraryIndexFiles)
   SET(CINDEX)
   FOREACH(MIDX ${MasterIdx})
     SET(CINDEX ${CINDEX} -Cindex "${MIDX}")
   ENDFOREACH(MIDX)

   # Need dependency on ${OTB_BINARY_DIR}/itkConfigure.h so
   # package is rebuilt when the OTB version changes.
   ADD_CUSTOM_COMMAND(
     COMMENT "${OutputPerlCxx} from "
     SOURCE ${Bin}/${InputIdx}
     COMMAND ${CSWIG}
     ARGS -perl5 -l${OTB_TOP}/Wrapping/CSwig/itk.swg
          -I${OTB_SWIG_DEFAULT_LIB}
          -noruntime ${CINDEX} ${IGNORE_WARNINGS} -depend ${Bin}/${InputXml}.depend
          -o ${Bin}/${OutputPerlCxx} -c++ ${Bin}/${InputXml}
     TARGET ${Library}
     OUTPUTS ${Bin}/${OutputPerlCxx}
     DEPENDS ${LibraryIndexFiles} ${OTB_TOP}/Wrapping/CSwig/itk.swg ${Bin}/${InputXml} ${CSWIG} ${OTB_BINARY_DIR}/itkConfigure.h)
ENDMACRO(CSWIG_CREATE_PERL_CXX_FILE)

SET(OTB_CSWIG_PYTHON_NO_EXCEPTION_REGEX "(ContinuousIndex|Python)\\.xml$")
SET(OTB_CSWIG_JAVA_NO_EXCEPTION_REGEX "(Java)\\.xml$")

MACRO(CSWIG_CREATE_PYTHON_CXX_FILE Bin MasterIdx InputIdx InputXml OutputTclCxx Library LibraryIndexFiles)
   SET(CINDEX)
   FOREACH(MIDX ${MasterIdx})
     SET(CINDEX ${CINDEX} -Cindex "${MIDX}")
   ENDFOREACH(MIDX)
   IF("${InputXml}" MATCHES "${OTB_CSWIG_PYTHON_NO_EXCEPTION_REGEX}")
     SET(OTB_SWG_FILE "")
   ELSE("${InputXml}" MATCHES "${OTB_CSWIG_PYTHON_NO_EXCEPTION_REGEX}")
     SET(OTB_SWG_FILE "-l${OTB_TOP}/Wrapping/CSwig/itk.swg")
   ENDIF("${InputXml}" MATCHES "${OTB_CSWIG_PYTHON_NO_EXCEPTION_REGEX}")
   ADD_CUSTOM_COMMAND(
     COMMENT "${OutputTclCxx} from "
     SOURCE ${Bin}/${InputIdx}
     COMMAND ${CSWIG}
     ARGS ${OTB_SWG_FILE}
          -I${OTB_SWIG_DEFAULT_LIB}
          -I${OTB_SWIG_DEFAULT_LIB}/python
          -noruntime ${CINDEX} ${IGNORE_WARNINGS} -depend ${Bin}/${InputXml}.depend
          -outdir "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}"
          -o ${Bin}/${OutputTclCxx} -python -c++ ${Bin}/${InputXml}
     TARGET ${Library}
     OUTPUTS ${Bin}/${OutputTclCxx}
     DEPENDS ${LibraryIndexFiles}  ${OTB_TOP}/Wrapping/CSwig/itk.swg ${OTB_TOP}/Wrapping/CSwig/itk.swg ${Bin}/${InputXml} ${CSWIG} )
ENDMACRO(CSWIG_CREATE_PYTHON_CXX_FILE)

MACRO(CSWIG_CREATE_JAVA_CXX_FILE Bin MasterIdx InputIdx InputXml OutputTclCxx Library LibraryIndexFiles)
   SET(CINDEX)
   FOREACH(MIDX ${MasterIdx})
     SET(CINDEX ${CINDEX} -Cindex "${MIDX}")
   ENDFOREACH(MIDX)
   IF("${InputXml}" MATCHES "${OTB_CSWIG_JAVA_NO_EXCEPTION_REGEX}")
     SET(OTB_SWG_FILE "")
   ELSE("${InputXml}" MATCHES "${OTB_CSWIG_JAVA_NO_EXCEPTION_REGEX}")
     SET(OTB_SWG_FILE "-l${OTB_TOP}/Wrapping/CSwig/itk.swg")
   ENDIF("${InputXml}" MATCHES "${OTB_CSWIG_JAVA_NO_EXCEPTION_REGEX}")
   ADD_CUSTOM_COMMAND(
     COMMENT "${OutputTclCxx} from "
     SOURCE ${Bin}/${InputIdx}
     COMMAND ${CSWIG}
     ARGS -I${OTB_SWIG_DEFAULT_LIB}
          -I${OTB_SWIG_DEFAULT_LIB}/java
          ${OTB_SWG_FILE}
          -noruntime ${CINDEX} ${IGNORE_WARNINGS} -depend ${Bin}/${InputXml}.depend
          -outdir ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/InsightToolkit
          -o ${Bin}/${OutputTclCxx} -package InsightToolkit -java -c++ ${Bin}/${InputXml}
     TARGET ${Library}
     OUTPUTS ${Bin}/${OutputTclCxx}
     DEPENDS ${LibraryIndexFiles} ${OTB_TOP}/Wrapping/CSwig/itk.swg  ${Bin}/${InputXml} ${CSWIG} )
ENDMACRO(CSWIG_CREATE_JAVA_CXX_FILE)


# macro to create .xml, .idx and Tcl.cxx files
MACRO(WRAP_TCL_SOURCES Source Bin BaseName LibraryName MasterIdx LibraryIndexFiles)
   GCCXML_CREATE_XML_FILE(${Source} ${Bin} ${BaseName}.cxx ${BaseName}.xml ${LibraryName})
   CINDEX_CREATE_IDX_FILE(${Bin} ${BaseName}.xml  ${BaseName}.idx ${LibraryName})
   CSWIG_CREATE_TCL_CXX_FILE(${Bin} "${MasterIdx}" ${BaseName}.idx ${BaseName}.xml
                             ${BaseName}Tcl.cxx ${LibraryName} "${LibraryIndexFiles}")
ENDMACRO(WRAP_TCL_SOURCES)

# macro to create .xml, .idx and Tcl.cxx files
MACRO(WRAP_PERL_SOURCES Source Bin BaseName LibraryName MasterIdx LibraryIndexFiles)
   GCCXML_CREATE_XML_FILE(${Source} ${Bin} ${BaseName}.cxx ${BaseName}.xml ${LibraryName})
   CINDEX_CREATE_IDX_FILE(${Bin} ${BaseName}.xml  ${BaseName}.idx ${LibraryName})
   CSWIG_CREATE_PERL_CXX_FILE(${Bin} "${MasterIdx}" ${BaseName}.idx ${BaseName}.xml
                             ${BaseName}Perl.cxx ${LibraryName} "${LibraryIndexFiles}")
ENDMACRO(WRAP_PERL_SOURCES)

# macro to create .xml, .idx and Python.cxx files
MACRO(WRAP_PYTHON_SOURCES Source Bin BaseName LibraryName MasterIdx LibraryIndexFiles)
   GCCXML_CREATE_XML_FILE(${Source} ${Bin} ${BaseName}.cxx ${BaseName}.xml ${LibraryName})
   CINDEX_CREATE_IDX_FILE(${Bin} ${BaseName}.xml  ${BaseName}.idx ${LibraryName})
   CSWIG_CREATE_PYTHON_CXX_FILE(${Bin} "${MasterIdx}" ${BaseName}.idx ${BaseName}.xml
                             ${BaseName}Python.cxx ${LibraryName} "${LibraryIndexFiles}")
ENDMACRO(WRAP_PYTHON_SOURCES)

# macro to create .xml, .idx and Python.cxx files
MACRO(WRAP_JAVA_SOURCES Source Bin BaseName LibraryName MasterIdx LibraryIndexFiles)
   GCCXML_CREATE_XML_FILE(${Source} ${Bin} ${BaseName}.cxx ${BaseName}.xml ${LibraryName})
   CINDEX_CREATE_IDX_FILE(${Bin} ${BaseName}.xml  ${BaseName}.idx ${LibraryName})
   CSWIG_CREATE_JAVA_CXX_FILE(${Bin} "${MasterIdx}" ${BaseName}.idx ${BaseName}.xml
                             ${BaseName}Java.cxx ${LibraryName} "${LibraryIndexFiles}")
ENDMACRO(WRAP_JAVA_SOURCES)


# make sure required stuff is set
SET(CSWIG_MISSING_VALUES)
IF(NOT CSWIG)
   SET(CSWIG_MISSING_VALUES "${CSWIG_MISSING_VALUES} CSWIG ")
ENDIF(NOT CSWIG)
IF(NOT CABLE_INDEX)
   SET(CSWIG_MISSING_VALUES "${CSWIG_MISSING_VALUES} CABLE_INDEX ")
ENDIF(NOT CABLE_INDEX)
IF(NOT GCCXML)
   SET(CSWIG_MISSING_VALUES "${CSWIG_MISSING_VALUES} GCCXML ")
ENDIF(NOT GCCXML)
IF(CSWIG_MISSING_VALUES)
  MESSAGE(SEND_ERROR "To use cswig wrapping, CSWIG, CABLE_INDEX, and GCCXML executables must be specified.  If they are all in the same directory, only specifiy one of them, and then run cmake configure again and the others should be found.\nCurrently, you are missing the following:\n ${CSWIG_MISSING_VALUES}")
ENDIF(CSWIG_MISSING_VALUES)


IF(OTB_CSWIG_PYTHON)
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH} )
ENDIF(OTB_CSWIG_PYTHON)
IF(OTB_CSWIG_TCL)
  INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH} ${TK_INCLUDE_PATH})
ENDIF(OTB_CSWIG_TCL)
IF(OTB_CSWIG_PERL)
  INCLUDE_DIRECTORIES(${PERL_INCLUDE_PATH})
ENDIF(OTB_CSWIG_PERL)
IF(OTB_CSWIG_JAVA)
  INCLUDE_DIRECTORIES(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} ${JAVA_AWT_INCLUDE_PATH})
ENDIF(OTB_CSWIG_JAVA)

SET(SWIG_INC
  ${TCL_INCLUDE_PATH}
  ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${OTB_INCLUDE_DIRS_SYSTEM}
  ${OTB_TOP}/Wrapping/CSwig
  ${OTB_TOP}/Wrapping/CSwig/CommonA
  ${OTB_TOP}/Wrapping/CSwig/CommonB
  ${OTB_TOP}/Wrapping/CSwig/VXLNumerics
  ${OTB_TOP}/Wrapping/CSwig/Numerics
  ${OTB_TOP}/Wrapping/CSwig/BasicFiltersA
  ${OTB_TOP}/Wrapping/CSwig/BasicFiltersB
  ${OTB_TOP}/Wrapping/CSwig/IO
  ${OTB_TOP}/Wrapping/CSwig/Algorithms
  ${OTB_TOP}/Wrapping/CSwig/otbCommon
  ${OTB_TOP}/Wrapping/CSwig/otbIO
  ${OTB_TOP}/Wrapping/CSwig/otbVisu
  )
ADD_DEFINITIONS(-DSWIG_GLOBAL)

SET(OTB_KITS 
VXLNumerics 
CommonA CommonB 
Numerics
BasicFiltersA BasicFiltersB 
IO 
Algorithms
otbCommon
otbIO
otbVisu
)
SUBDIRS(SwigRuntime ${OTB_KITS} Tests)
IF(OTB_CSWIG_JAVA)
  SUBDIRS(Java)
ENDIF(OTB_CSWIG_JAVA)


IF(UNIX)
  SET(OTB_CSWIG_LIBNAME_PREFIX "lib")
ELSE(UNIX)
  SET(OTB_CSWIG_LIBNAME_PREFIX "")
ENDIF(UNIX)

SET(OTB_CSWIG_PACKAGE_DIR_BUILD "${OTB_LIBRARY_PATH}")

#-----------------------------------------------------------------------------
# Configure pkgIndex.tcl for the build tree.
SET(OTB_CSWIG_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES})
SET(OTB_CSWIG_DATA_ROOT "${OTB_DATA_ROOT}")
SET(OTB_CSWIG_TEST_ROOT "${OTB_BINARY_DIR}/Testing")
SET(OTB_CSWIG_SCRIPT_DIR "${OTB_SOURCE_DIR}/Wrapping/CSwig/Tcl")

IF(OTB_CSWIG_CONFIGURATION_TYPES)
  FOREACH(config ${OTB_CSWIG_CONFIGURATION_TYPES})
    SET(OTB_CSWIG_PACKAGE_DIR "${OTB_CSWIG_PACKAGE_DIR_BUILD}/${config}")
    CONFIGURE_FILE("${OTB_SOURCE_DIR}/Wrapping/CSwig/Tcl/pkgIndex.tcl.in"
                   "${OTB_BINARY_DIR}/Wrapping/CSwig/Tcl/${config}/pkgIndex.tcl"
                   @ONLY IMMEDIATE)
  ENDFOREACH(config)
ELSE(OTB_CSWIG_CONFIGURATION_TYPES)
  SET(OTB_CSWIG_PACKAGE_DIR "${OTB_CSWIG_PACKAGE_DIR_BUILD}")
  CONFIGURE_FILE("${OTB_SOURCE_DIR}/Wrapping/CSwig/Tcl/pkgIndex.tcl.in"
                 "${OTB_BINARY_DIR}/Wrapping/CSwig/Tcl/pkgIndex.tcl"
                 @ONLY IMMEDIATE)
ENDIF(OTB_CSWIG_CONFIGURATION_TYPES)

#-----------------------------------------------------------------------------
# Configure pkgIndex.tcl for the install tree.

SET(OTB_CSWIG_SCRIPT_DIR "[file dirname [info script]]")
SET(OTB_CSWIG_PACKAGE_DIR "[file dirname [file dirname [info script]]]")
SET(OTB_CSWIG_DATA_ROOT "[file dirname [file dirname [info script]]]/Data")
SET(OTB_CSWIG_TEST_ROOT "<NO_DEFAULT>")
CONFIGURE_FILE("${OTB_SOURCE_DIR}/Wrapping/CSwig/Tcl/pkgIndex.tcl.in"
               "${OTB_BINARY_DIR}/Wrapping/CSwig/Tcl/InstallOnly/Hide/pkgIndex.tcl"
               IMMEDIATE @ONLY)
INSTALL(FILES "${OTB_BINARY_DIR}/Wrapping/CSwig/Tcl/InstallOnly/Hide/pkgIndex.tcl"
  DESTINATION ${OTB_INSTALL_LIB_DIR}/tcl
  COMPONENT RuntimeLibraries)

#-----------------------------------------------------------------------------
# Configure python packages.
SET(OTB_CSWIG_DATA_ROOT "'${OTB_DATA_ROOT}'")
SET(OTB_CSWIG_TEST_ROOT "'${OTB_BINARY_DIR}/Testing'")
IF(OTB_CSWIG_CONFIGURATION_TYPES)
  FOREACH(config ${OTB_CSWIG_CONFIGURATION_TYPES})
    SET(OTB_CSWIG_PACKAGE_DIR "'${OTB_CSWIG_PACKAGE_DIR_BUILD}/${config}'")
    CONFIGURE_FILE("${OTB_SOURCE_DIR}/Wrapping/CSwig/Python/itkbase.py.in"
                   "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/${config}/itkbase.py"
                   @ONLY IMMEDIATE)
  ENDFOREACH(config)
ELSE(OTB_CSWIG_CONFIGURATION_TYPES)
  SET(OTB_CSWIG_PACKAGE_DIR "'${OTB_CSWIG_PACKAGE_DIR_BUILD}'")
  CONFIGURE_FILE("${OTB_SOURCE_DIR}/Wrapping/CSwig/Python/itkbase.py.in"
                 "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/itkbase.py"
                 @ONLY IMMEDIATE)
ENDIF(OTB_CSWIG_CONFIGURATION_TYPES)

# Handle out-of-source builds correctly.
#
#  1. Create a list of Python files to be installed/copied.
#  2. Copy them to OTB_BINARY_DIR if it is different from OTB_SOURCE_DIR.
#  3. Use Python's compileall to compile the copied files.
#
# *** Step 1 has to be done carefully to avoid missing out files ***
IF(PYTHON_EXECUTABLE AND OTB_CSWIG_PYTHON)

  # Deal with numarray package  Options
  SET(CMAKE_MODULE_PATH ${OTB_SOURCE_DIR}/CMake)
  OPTION(OTB_USE_PYTHON_NUMARRAY "Use the numarray python package" OFF)
  MARK_AS_ADVANCED(OTB_USE_PYTHON_NUMARRAY)
  IF( OTB_USE_PYTHON_NUMARRAY )
      FIND_PACKAGE( NUMARRAY )
  ENDIF( OTB_USE_PYTHON_NUMARRAY )

  IF(PYTHON_NUMARRAY_FOUND)
    INCLUDE_DIRECTORIES( ${PYTHON_NUMARRAY_INCLUDE_DIR} )
    SET(SWIG_INC ${SWIG_INC} ${PYTHON_NUMARRAY_INCLUDE_DIR})
  ENDIF(PYTHON_NUMARRAY_FOUND)


  ADD_CUSTOM_TARGET(itkpython_pyc ALL echo "...")

  # Make the necessary directories.
  MAKE_DIRECTORY(${OTB_BINARY_DIR}/Wrapping/CSwig/Python)

  # Now create a list of Python files.
  SET(OTB_PYTHON_FILES)

  # Wrapping/CSwig/Python/*.py
  SET(OTB_PYTHON_FILES
    ${OTB_PYTHON_FILES}
    InsightToolkit
    OrfeoToolBox
    itkalgorithms
    itkbasicfilters
    itkcommon
    itkio
    itktesting
    itkdata
    itknumerics
    vxlnumerics
    otbio
    otbcommon
    otbvisu
    )
  # Done listing files.

  # Now copy these files if necessary.
  SET(OTB_PYTHON_SOURCE_FILES)
  SET(OTB_PYTHON_OUTPUT_FILES)
  IF(OTB_CSWIG_CONFIGURATION_TYPES)
    FOREACH(file ${OTB_PYTHON_FILES})
      SET(src "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/${CMAKE_CFG_INTDIR}/${file}.py")
      SET(OTB_PYTHON_SOURCE_FILES ${OTB_PYTHON_SOURCE_FILES} ${src})
    ENDFOREACH(file)
  ELSE(OTB_CSWIG_CONFIGURATION_TYPES)
    FOREACH(file ${OTB_PYTHON_FILES})
      SET(src "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/${file}.py")
      SET(OTB_PYTHON_SOURCE_FILES ${OTB_PYTHON_SOURCE_FILES} ${src})
    ENDFOREACH(file)
  ENDIF(OTB_CSWIG_CONFIGURATION_TYPES)
  IF ("${OTB_BINARY_DIR}" MATCHES "^${OTB_SOURCE_DIR}$")
    #MESSAGE("In source build -- no need to copy Python files.")
  ELSE ("${OTB_BINARY_DIR}" MATCHES "^${OTB_SOURCE_DIR}$")
    IF(OTB_CSWIG_CONFIGURATION_TYPES)
      FOREACH(file ${OTB_PYTHON_FILES})
        SET(src "${OTB_SOURCE_DIR}/Wrapping/CSwig/Python/${file}.py")
        SET(tgt "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/${CMAKE_CFG_INTDIR}/${file}.py")
        ADD_CUSTOM_COMMAND(SOURCE ${src}
                           COMMAND ${CMAKE_COMMAND}
                           ARGS -E copy ${src} ${tgt}
                           OUTPUTS ${tgt}
                           TARGET itkpython_pyc
                           COMMENT "source copy")
      ENDFOREACH(file)
    ELSE(OTB_CSWIG_CONFIGURATION_TYPES)
      FOREACH(file ${OTB_PYTHON_FILES})
        SET(src "${OTB_SOURCE_DIR}/Wrapping/CSwig/Python/${file}.py")
        SET(tgt "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/${file}.py")
        ADD_CUSTOM_COMMAND(SOURCE ${src}
              COMMAND ${CMAKE_COMMAND}
                          ARGS -E copy ${src} ${tgt}
                          OUTPUTS ${tgt}
        TARGET itkpython_pyc
                          COMMENT "source copy")
      ENDFOREACH(file)
    ENDIF(OTB_CSWIG_CONFIGURATION_TYPES)
  ENDIF ("${OTB_BINARY_DIR}" MATCHES "^${OTB_SOURCE_DIR}$")

  # Byte compile the Python files.
  WRITE_FILE(${OTB_BINARY_DIR}/Wrapping/CSwig/Python/compile_all_itk
        "import compileall\n"
        "compileall.compile_dir('${OTB_BINARY_DIR}/Wrapping/CSwig/Python')\n"
  "file = open('${OTB_BINARY_DIR}/Wrapping/CSwig/Python/itk_compile_complete', 'w')\n"
  "file.write('Done')\n")

  ADD_CUSTOM_COMMAND(
    SOURCE ${OTB_BINARY_DIR}/Wrapping/CSwig/Python/compile_all_itk
    COMMAND ${PYTHON_EXECUTABLE}
    ARGS ${OTB_BINARY_DIR}/Wrapping/CSwig/Python/compile_all_itk
    DEPENDS ${OTB_PYTHON_SOURCE_FILES}
    OUTPUTS "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/itk_compile_complete"
    TARGET itkpython_pyc
    )

  ADD_CUSTOM_COMMAND(
    SOURCE itkpython_pyc
    DEPENDS "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/itk_compile_complete"
    TARGET itkpython_pyc
    )

ENDIF(PYTHON_EXECUTABLE AND OTB_CSWIG_PYTHON)

IF(OTB_CSWIG_TCL)
  SUBDIRS(Tcl)
ENDIF(OTB_CSWIG_TCL)

IF(OTB_CSWIG_PYTHON)
  CONFIGURE_FILE(
    "${WrapOTB_SOURCE_DIR}/pythonfiles_install.cmake.in"
    "${WrapOTB_BINARY_DIR}/pythonfiles_install.cmake"
    @ONLY IMMEDIATE)
  ADD_CUSTOM_TARGET(python_install)
  SET_TARGET_PROPERTIES(python_install PROPERTIES
    POST_INSTALL_SCRIPT "${WrapOTB_BINARY_DIR}/pythonfiles_install.cmake")

  # Install the package python files.
  FOREACH(file ${OTB_PYTHON_FILES})
    INSTALL(FILES "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/${file}.py"
      DESTINATION ${OTB_INSTALL_LIB_DIR}/python
      COMPONENT RuntimeLibraries)
  ENDFOREACH(file)
  
  SET(OTB_CSWIG_PACKAGE_DIR "os.path.dirname(selfpath)")
  SET(OTB_CSWIG_DATA_ROOT "os.path.join(os.path.dirname(selfpath),'Data')")
  SET(OTB_CSWIG_TEST_ROOT "'<NO_DEFAULT>'")
  CONFIGURE_FILE("${OTB_SOURCE_DIR}/Wrapping/CSwig/Python/itkbase.py.in"
                 "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/InstallOnly/itkbase.py"
                 @ONLY IMMEDIATE)
  INSTALL(FILES "${OTB_BINARY_DIR}/Wrapping/CSwig/Python/InstallOnly/itkbase.py"
    DESTINATION ${OTB_INSTALL_LIB_DIR}/python
    COMPONENT RuntimeLibraries)
ENDIF(OTB_CSWIG_PYTHON)


MACRO(ITK_WRAP_LIBRARY WRAP_SOURCES LIBRARY_NAME DIRECTORY DEPEND_LIBRARY EXTRA_SOURCES OTB_LINK_LIBRARIES)
  # loop over cable config files creating two lists:
  # WRAP_TCL_SOURCES: list of generated files
  SET(INDEX_FILE_CONTENT "%JavaLoader=InsightToolkit.itkbase.LoadLibrary(\"${LIBRARY_NAME}Java\")\n")
  FOREACH(Source ${WRAP_SOURCES})
    SET(WRAP_PERL_SOURCES ${WRAP_PERL_SOURCES} ${Source}Perl.cxx)
    SET(WRAP_TCL_SOURCES ${WRAP_TCL_SOURCES} ${Source}Tcl.cxx)
    SET(WRAP_PYTHON_SOURCES ${WRAP_PYTHON_SOURCES} ${Source}Python.cxx)
    SET(WRAP_JAVA_SOURCES ${WRAP_JAVA_SOURCES} ${Source}Java.cxx)
    STRING(REGEX REPLACE wrap_ "" JAVA_DEP ${Source})
    SET(${LIBRARY_NAME}_JAVA_DEPENDS_INIT ${${LIBRARY_NAME}_JAVA_DEPENDS_INIT} ${JAVA_DEP}.java)
    SET(ALL_IDX_FILES ${ALL_IDX_FILES} ${WrapOTB_BINARY_DIR}/${DIRECTORY}/${Source}.idx )
    SET(INDEX_FILE_CONTENT "${INDEX_FILE_CONTENT}${WrapOTB_BINARY_DIR}/${DIRECTORY}/${Source}.idx\n")
  ENDFOREACH(Source)
  SET(${LIBRARY_NAME}_JAVA_DEPENDS  "${${LIBRARY_NAME}_JAVA_DEPENDS_INIT}" CACHE INTERNAL "" FORCE)
  # add the package wrappers 
  SET(WRAP_PERL_SOURCES ${WRAP_PERL_SOURCES} wrap_${LIBRARY_NAME}PerlPerl.cxx)
  SET(WRAP_TCL_SOURCES ${WRAP_TCL_SOURCES} wrap_${LIBRARY_NAME}TclTcl.cxx)
  SET(WRAP_PYTHON_SOURCES ${WRAP_PYTHON_SOURCES} wrap_${LIBRARY_NAME}PythonPython.cxx)
  SET(WRAP_JAVA_SOURCES ${WRAP_JAVA_SOURCES} wrap_${LIBRARY_NAME}JavaJava.cxx)
  IF(OTB_EXTRA_TCL_WRAP)
    SET(WRAP_TCL_SOURCES ${WRAP_TCL_SOURCES} ${OTB_EXTRA_TCL_WRAP}Tcl.cxx)
  ENDIF(OTB_EXTRA_TCL_WRAP)
  IF(OTB_EXTRA_PYTHON_WRAP)
    FOREACH( extraPython ${OTB_EXTRA_PYTHON_WRAP})
      SET(WRAP_PYTHON_SOURCES ${WRAP_PYTHON_SOURCES} ${extraPython}Python.cxx)
    ENDFOREACH( extraPython )
  ENDIF(OTB_EXTRA_PYTHON_WRAP)
  IF(OTB_EXTRA_JAVA_WRAP)
    SET(WRAP_JAVA_SOURCES ${WRAP_JAVA_SOURCES} ${OTB_EXTRA_JAVA_WRAP}Java.cxx)
  ENDIF(OTB_EXTRA_JAVA_WRAP)
  IF(OTB_EXTRA_PERL_WRAP)
    SET(WRAP_PERL_SOURCES ${WRAP_PERL_SOURCES} ${OTB_EXTRA_PERL_WRAP}Java.cxx)
  ENDIF(OTB_EXTRA_PERL_WRAP)

  # set the generated sources as generated
  SET_SOURCE_FILES_PROPERTIES(
    ${WRAP_PERL_SOURCES} 
    ${WRAP_TCL_SOURCES} 
    ${WRAP_PYTHON_SOURCES} 
    ${WRAP_JAVA_SOURCES} GENERATED )
  SET(EXTRA_LIBS ${OTB_LINK_LIBRARIES})
  IF("${OTB_LINK_LIBRARIES}" MATCHES "^$")
    SET(EXTRA_LIBS ${LIBRARY_NAME})
  ENDIF("${OTB_LINK_LIBRARIES}" MATCHES "^$")
  IF(OTB_CSWIG_TCL)
    IF(OTB_SWIG_FILE)
      SET(SWIG_INC ${SWIG_INC} ${TCL_INCLUDE_PATH})
      SET_SOURCE_FILES_PROPERTIES(${OTB_SWIG_FILE_CXX}Tcl.cxx GENERATED)
      SET(WRAP_FILE ${OTB_SWIG_FILE_CXX}Tcl.cxx )
    ENDIF(OTB_SWIG_FILE)
      
    ADD_LIBRARY(${LIBRARY_NAME}Tcl SHARED 
      ${WRAP_TCL_SOURCES} 
      ${OTB_EXTRA_TCL_SOURCES} 
      ${WRAP_FILE}
      ${EXTRA_SOURCES})
    IF(OTB_WRAP_NEEDS_DEPEND)
      FOREACH(lib ${DEPEND_LIBRARY})
        ADD_DEPENDENCIES(${LIBRARY_NAME}Tcl ${lib}Tcl)
      ENDFOREACH(lib)
    ENDIF(OTB_WRAP_NEEDS_DEPEND)
    IF(OTB_LIBRARY_PROPERTIES)
      SET_TARGET_PROPERTIES(${LIBRARY_NAME}Tcl PROPERTIES LINK_FLAGS "${CSWIG_EXTRA_LINKFLAGS}" ${OTB_LIBRARY_PROPERTIES})
    ELSE(OTB_LIBRARY_PROPERTIES)
    SET_TARGET_PROPERTIES(${LIBRARY_NAME}Tcl PROPERTIES LINK_FLAGS "${CSWIG_EXTRA_LINKFLAGS}")
    ENDIF(OTB_LIBRARY_PROPERTIES)
    TARGET_LINK_LIBRARIES(${LIBRARY_NAME}Tcl ${EXTRA_LIBS} SwigRuntimeTcl ${TCL_LIBRARY})
    INSTALL(TARGETS ${LIBRARY_NAME}Tcl
      RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
      LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
      ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
    IF(OTB_SWIG_FILE)
      ADD_CUSTOM_COMMAND(
        COMMENT "run native swig on ${OTB_SWIG_FILE}"
        SOURCE ${OTB_SWIG_FILE}
        COMMAND ${CSWIG} 
        ARGS -nocable -noruntime ${IGNORE_WARNINGS} -o ${WRAP_FILE}
        -c++ ${OTB_SWIG_FILE} 
        TARGET ${LIBRARY_NAME}Tcl
        OUTPUTS ${WRAP_FILE}
        DEPENDS ${OTB_SWIG_FILE} ${CSWIG})
    ENDIF(OTB_SWIG_FILE)
  ENDIF(OTB_CSWIG_TCL)


  IF(OTB_CSWIG_PERL)
    IF(OTB_SWIG_FILE)
      SET(SWIG_INC ${SWIG_INC} ${PERL_INCLUDE_PATH})
      SET_SOURCE_FILES_PROPERTIES(${OTB_SWIG_FILE_CXX}Perl.cxx GENERATED)
      SET(WRAP_FILE ${OTB_SWIG_FILE_CXX}Perl.cxx )
    ENDIF(OTB_SWIG_FILE)
      
    ADD_LIBRARY(${LIBRARY_NAME}Perl SHARED 
      ${WRAP_PERL_SOURCES} 
      ${OTB_EXTRA_PERL_SOURCES} 
      ${WRAP_FILE}
      ${EXTRA_SOURCES})
    IF(OTB_LIBRARY_PROPERTIES)
      SET_TARGET_PROPERTIES(${LIBRARY_NAME}Perl PROPERTIES LINK_FLAGS "${CSWIG_EXTRA_LINKFLAGS}" ${OTB_LIBRARY_PROPERTIES})
    ELSE(OTB_LIBRARY_PROPERTIES)
      SET_TARGET_PROPERTIES(${LIBRARY_NAME}Perl PROPERTIES LINK_FLAGS "${CSWIG_EXTRA_LINKFLAGS}")
    ENDIF(OTB_LIBRARY_PROPERTIES)
    TARGET_LINK_LIBRARIES(${LIBRARY_NAME}Perl ${EXTRA_LIBS} SwigRuntimePerl ${PERL_LIBRARY})
    IF(OTB_WRAP_NEEDS_DEPEND)
      FOREACH(lib ${DEPEND_LIBRARY})
        ADD_DEPENDENCIES(${LIBRARY_NAME}Perl ${lib}Perl)
      ENDFOREACH(lib)
    ENDIF(OTB_WRAP_NEEDS_DEPEND)
    INSTALL(TARGETS ${LIBRARY_NAME}Perl
      RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
      LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
      ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
    IF(OTB_SWIG_FILE)
      ADD_CUSTOM_COMMAND(
        COMMENT "run native swig on ${OTB_SWIG_FILE}"
        SOURCE ${OTB_SWIG_FILE}
        COMMAND ${CSWIG} 
        ARGS -nocable -noruntime ${IGNORE_WARNINGS} -o ${WRAP_FILE}
        -perl5 -c++ ${OTB_SWIG_FILE}
        TARGET ${LIBRARY_NAME}Perl
        OUTPUTS ${WRAP_FILE}
        DEPENDS ${OTB_SWIG_FILE} ${CSWIG})
    ENDIF(OTB_SWIG_FILE)
  ENDIF(OTB_CSWIG_PERL)


  IF(OTB_CSWIG_PYTHON)  
    IF(OTB_SWIG_FILE)
      SET(SWIG_INC ${SWIG_INC} ${PYTHON_INCLUDE_PATH})
      SET_SOURCE_FILES_PROPERTIES(${OTB_SWIG_FILE_CXX}Python.cxx GENERATED)
      SET(WRAP_FILE ${OTB_SWIG_FILE_CXX}Python.cxx )
    ENDIF(OTB_SWIG_FILE)
    
    ADD_LIBRARY(_${LIBRARY_NAME}Python MODULE
      ${WRAP_PYTHON_SOURCES}
      ${OTB_EXTRA_PYTHON_SOURCES}
      ${WRAP_FILE}
      ${EXTRA_SOURCES})
    IF(OTB_WRAP_NEEDS_DEPEND)
      FOREACH(lib ${DEPEND_LIBRARY})
        ADD_DEPENDENCIES(_${LIBRARY_NAME}Python _${lib}Python)
      ENDFOREACH(lib)
    ENDIF(OTB_WRAP_NEEDS_DEPEND)
    IF(OTB_LIBRARY_PROPERTIES)
      SET_TARGET_PROPERTIES( _${LIBRARY_NAME}Python PROPERTIES PREFIX "" ${OTB_LIBRARY_PROPERTIES})
    ELSE(OTB_LIBRARY_PROPERTIES)
    SET_TARGET_PROPERTIES( _${LIBRARY_NAME}Python PROPERTIES PREFIX "")
    ENDIF(OTB_LIBRARY_PROPERTIES)
    TARGET_LINK_LIBRARIES(_${LIBRARY_NAME}Python ${EXTRA_LIBS} SwigRuntimePython ${PYTHON_LIBRARY})
    INSTALL(TARGETS _${LIBRARY_NAME}Python
      RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
      LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
      ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
    IF(OTB_SWIG_FILE)
      ADD_CUSTOM_COMMAND(
        COMMENT "run native swig on ${OTB_SWIG_FILE}"
        SOURCE ${OTB_SWIG_FILE}
        COMMAND ${CSWIG} 
        ARGS -nocable -noruntime ${IGNORE_WARNINGS} -o ${WRAP_FILE}
         -outdir "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}"
         -python -c++ ${OTB_SWIG_FILE}
        TARGET _${LIBRARY_NAME}Python
        OUTPUTS ${WRAP_FILE}
        DEPENDS ${OTB_SWIG_FILE} ${CSWIG})
    ENDIF(OTB_SWIG_FILE)
  ENDIF(OTB_CSWIG_PYTHON)

  IF(OTB_CSWIG_JAVA)  
    IF(OTB_SWIG_FILE)
      SET(SWIG_INC ${SWIG_INC} ${JAVA_INCLUDE_PATH})
      SET_SOURCE_FILES_PROPERTIES(${OTB_SWIG_FILE_CXX}Java.cxx GENERATED)
      SET(WRAP_FILE ${OTB_SWIG_FILE_CXX}Java.cxx )
    ENDIF(OTB_SWIG_FILE)
    MAKE_DIRECTORY("${OTB_BINARY_DIR}/Wrapping/CSwig/Java/InsightToolkit")
    ADD_LIBRARY(${LIBRARY_NAME}Java MODULE 
      ${WRAP_JAVA_SOURCES} 
      ${OTB_EXTRA_JAVA_SOURCES} 
      ${WRAP_FILE}
      ${EXTRA_SOURCES})
    TARGET_LINK_LIBRARIES(${LIBRARY_NAME}Java ${JAVA_LIBRARY} ${EXTRA_LIBS} )   
    IF(OTB_WRAP_NEEDS_DEPEND)
      FOREACH(lib ${DEPEND_LIBRARY})
        ADD_DEPENDENCIES(${LIBRARY_NAME}Java ${lib}Java)
      ENDFOREACH(lib)
    ENDIF(OTB_WRAP_NEEDS_DEPEND)
    IF(OTB_LIBRARY_PROPERTIES)
      SET_TARGET_PROPERTIES(${LIBRARY_NAME}Java PROPERTIES ${OTB_LIBRARY_PROPERTIES})
    ENDIF(OTB_LIBRARY_PROPERTIES)
    INSTALL(TARGETS ${LIBRARY_NAME}Java
      RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
      LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
      ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
    IF(OTB_SWIG_FILE)
      ADD_CUSTOM_COMMAND(
        COMMENT "run native swig on ${OTB_SWIG_FILE}"
        SOURCE ${OTB_SWIG_FILE}
        COMMAND ${CSWIG} 
        ARGS -nocable -noruntime ${IGNORE_WARNINGS} -o ${WRAP_FILE} 
        -I${OTB_TOP}/Code/Common -DOTBCommon_EXPORT=
        -outdir ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/InsightToolkit
        -package InsightToolkit -java -c++ ${OTB_SWIG_FILE}
        TARGET ${LIBRARY_NAME}Java
        OUTPUTS ${WRAP_FILE}
        DEPENDS ${OTB_SWIG_FILE} ${CSWIG})
    ENDIF(OTB_SWIG_FILE)
  ENDIF(OTB_CSWIG_JAVA)
  
  CONFIGURE_FILE(
    ${WrapOTB_SOURCE_DIR}/Master.mdx.in
    ${WrapOTB_BINARY_DIR}/${DIRECTORY}/${LIBRARY_NAME}.mdx IMMEDIATE
    )

  SET(SWIG_INC_FILE ${WrapOTB_BINARY_DIR}/${DIRECTORY}/SwigInc.txt)
  SET(SWIG_INC_CONTENTS)
  FOREACH(dir ${SWIG_INC})
    SET(SWIG_INC_CONTENTS "${SWIG_INC_CONTENTS}-I${dir}\n")
  ENDFOREACH(dir)
  CONFIGURE_FILE(${WrapOTB_SOURCE_DIR}/SwigInc.txt.in ${SWIG_INC_FILE}
    @ONLY IMMEDIATE)
  
  FOREACH(Source ${WRAP_SOURCES})
    IF(OTB_CSWIG_TCL)
      # tcl 
      WRAP_TCL_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY}
        ${Source} ${LIBRARY_NAME}Tcl "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
    ENDIF(OTB_CSWIG_TCL)

    IF(OTB_CSWIG_PERL)
      # tcl 
      WRAP_PERL_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY}
        ${Source} ${LIBRARY_NAME}Perl "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
    ENDIF(OTB_CSWIG_PERL)
    
    IF(OTB_CSWIG_PYTHON)
      # python
      WRAP_PYTHON_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY} 
        ${Source} _${LIBRARY_NAME}Python "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
    ENDIF(OTB_CSWIG_PYTHON)
    
    IF(OTB_CSWIG_JAVA)
      # java
      WRAP_JAVA_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY}
        ${Source} ${LIBRARY_NAME}Java "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
    ENDIF(OTB_CSWIG_JAVA)
  ENDFOREACH(Source)
  
  
  # wrap the package files for tcl and python
  IF(OTB_CSWIG_TCL)
    # tcl 
    WRAP_TCL_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY}
      wrap_${LIBRARY_NAME}Tcl ${LIBRARY_NAME}Tcl "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
    IF(OTB_EXTRA_TCL_WRAP)
      WRAP_TCL_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY}
        "${OTB_EXTRA_TCL_WRAP}" ${LIBRARY_NAME}Tcl "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
    ENDIF(OTB_EXTRA_TCL_WRAP)
  ENDIF(OTB_CSWIG_TCL)

  IF(OTB_CSWIG_PERL)
    # perl
    WRAP_PERL_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY}
      wrap_${LIBRARY_NAME}Perl ${LIBRARY_NAME}Perl "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
    IF(OTB_EXTRA_PERL_WRAP)
      WRAP_PERL_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY}
        "${OTB_EXTRA_PERL_WRAP}" ${LIBRARY_NAME}Perl "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
    ENDIF(OTB_EXTRA_PERL_WRAP)
  ENDIF(OTB_CSWIG_PERL)

  IF(OTB_CSWIG_PYTHON)
    # python
    WRAP_PYTHON_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY}
      wrap_${LIBRARY_NAME}Python _${LIBRARY_NAME}Python "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
    IF(OTB_EXTRA_PYTHON_WRAP)
      FOREACH( extraPython ${OTB_EXTRA_PYTHON_WRAP})
        WRAP_PYTHON_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY}
          ${extraPython} _${LIBRARY_NAME}Python "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
      ENDFOREACH( extraPython )
    ENDIF(OTB_EXTRA_PYTHON_WRAP)

  ENDIF(OTB_CSWIG_PYTHON)

  IF(OTB_CSWIG_JAVA)
    # python
    WRAP_JAVA_SOURCES(${OTB_TOP}/Wrapping/CSwig/${DIRECTORY} ${WrapOTB_BINARY_DIR}/${DIRECTORY}
      wrap_${LIBRARY_NAME}Java ${LIBRARY_NAME}Java "${MASTER_INDEX_FILES}" "${ALL_IDX_FILES}")
  ENDIF(OTB_CSWIG_JAVA)

ENDMACRO(ITK_WRAP_LIBRARY)
