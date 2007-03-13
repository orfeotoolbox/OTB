SET(OTB_JAVA_CLASSPATH ${OTB_BINARY_DIR}/Wrapping/CSwig/Java)
SET(OTB_JAVA_OUTPATH ${OTB_BINARY_DIR}/Wrapping/CSwig/Java)

# Relative path from InsightToolkit.jar installation to dlls.
IF(WIN32)
  SET(OTB_JAVA_INSTALL_DIR "/../../bin")
ELSE(WIN32)
  SET(OTB_JAVA_INSTALL_DIR "")
ENDIF(WIN32)

IF(CMAKE_CONFIGURATION_TYPES)
  SET(OTB_BASE_JAVA_FILE ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/${CMAKE_CFG_INTDIR}/InsightToolkit/itkbase.java)
  FOREACH(config ${CMAKE_CONFIGURATION_TYPES})
    SET(OTB_JAVA_BUILD_DIR ${LIBRARY_OUTPUT_PATH}/${config})
    CONFIGURE_FILE(
      ${OTB_SOURCE_DIR}/Wrapping/CSwig/Java/itkbase.java.in
      ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/${config}/InsightToolkit/itkbase.java
      @ONLY IMMEDIATE
      )
    FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${config}/itk_build_tree.mark
      "File next to InsightToolkit.jar to mark the build tree.\n"
    )
  ENDFOREACH(config)
  SET(OTB_JAVA_CLASSPATH
    "${OTB_JAVA_CLASSPATH}\;${OTB_BINARY_DIR}/Wrapping/CSwig/Java/${CMAKE_CFG_INTDIR}")
  SET(OTB_JAVA_OUTPATH "${OTB_JAVA_OUTPATH}/${CMAKE_CFG_INTDIR}")
ELSE(CMAKE_CONFIGURATION_TYPES)
  SET(OTB_JAVA_BUILD_DIR ${LIBRARY_OUTPUT_PATH})
  SET(OTB_BASE_JAVA_FILE ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/InsightToolkit/itkbase.java)
  CONFIGURE_FILE(
    ${OTB_SOURCE_DIR}/Wrapping/CSwig/Java/itkbase.java.in
    ${OTB_BASE_JAVA_FILE}
    @ONLY IMMEDIATE
    )
  FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/itk_build_tree.mark
    "File next to InsightToolkit.jar to mark the build tree.\n"
    )
ENDIF(CMAKE_CONFIGURATION_TYPES)

SET(ORDERING_DEP)
FOREACH(Kit ${OTB_KITS})
  IF(${Kit} MATCHES VXLNumerics)
    SET(DEP_FILES ${VXLNumerics_JAVA_DEPENDS})
    SET(KIT_JAVA_NAME ${Kit})
  ELSE(${Kit} MATCHES VXLNumerics)
    SET(KIT_JAVA_NAME OTB${Kit})
    SET(DEP_FILES ${OTB${Kit}_JAVA_DEPENDS})
  ENDIF(${Kit} MATCHES VXLNumerics)
  FOREACH(File ${DEP_FILES})
    SET(FULL_DEP_FILES ${FULL_DEP_FILES} ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/InsightToolkit/${File})
  ENDFOREACH(File)

  SET(OTB_JAVA_KIT_LIBS ${OTB_JAVA_KIT_LIBS} ${KIT_JAVA_NAME}Java)
  SET(KIT_FILE_NAME ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/InsightToolkit/${KIT_JAVA_NAME}Java)
  ADD_CUSTOM_COMMAND(
    OUTPUT ${KIT_FILE_NAME}.class
    DEPENDS ${KIT_FILE_NAME}.java ${FULL_DEP_FILES} ${OTB_BASE_JAVA_FILE} ${ORDERING_DEP}
    COMMAND ${JAVA_COMPILE}
    ARGS -classpath "${OTB_JAVA_CLASSPATH}" -d "${OTB_JAVA_OUTPATH}"
    ${KIT_FILE_NAME}.java
    COMMENT "Java Class")
  SET(OTB_JAVA_KITS_FILES ${OTB_JAVA_KITS_FILES} ${KIT_FILE_NAME}.class)
  SET(ORDERING_DEP ${KIT_FILE_NAME}.class)
ENDFOREACH(Kit)

IF(CMAKE_CONFIGURATION_TYPES)
  ADD_CUSTOM_COMMAND(
    OUTPUT ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/${CMAKE_CFG_INTDIR}/InsightToolkit.jar
    DEPENDS ${OTB_JAVA_KITS_FILES}
    COMMAND ${JAVA_ARCHIVE}
    ARGS -cf ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/${CMAKE_CFG_INTDIR}/InsightToolkit.jar
    -C ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/${CMAKE_CFG_INTDIR} InsightToolkit
    COMMENT "Java Archive"
    )
  ADD_CUSTOM_TARGET(OTBJavaJar ALL DEPENDS ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/${CMAKE_CFG_INTDIR}/InsightToolkit.jar)
  SET(DOLLAR "$")
  INSTALL(FILES
    ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/${DOLLAR}{BUILD_TYPE}/InsightToolkit.jar
    DESTINATION ${OTB_INSTALL_LIB_DIR_CM24}
    COMPONENT RuntimeLibraries)
ELSE(CMAKE_CONFIGURATION_TYPES)
  ADD_CUSTOM_COMMAND(
    OUTPUT ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/InsightToolkit.jar
    DEPENDS ${OTB_JAVA_KITS_FILES}
    COMMAND ${JAVA_ARCHIVE}
    ARGS -cf ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/InsightToolkit.jar
    -C ${OTB_BINARY_DIR}/Wrapping/CSwig/Java InsightToolkit
    COMMENT "Java Archive"
    )
  ADD_CUSTOM_TARGET(OTBJavaJar ALL DEPENDS ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/InsightToolkit.jar)
  INSTALL(FILES
    ${OTB_BINARY_DIR}/Wrapping/CSwig/Java/InsightToolkit.jar
    DESTINATION ${OTB_INSTALL_LIB_DIR_CM24}
    COMPONENT RuntimeLibraries)
ENDIF(CMAKE_CONFIGURATION_TYPES)
ADD_DEPENDENCIES(OTBJavaJar ${OTB_JAVA_KIT_LIBS} SwigRuntimeJava)
FOREACH(Kit ${OTB_KITS})
  IF(${Kit} MATCHES VXLNumerics)
    SET(KIT_JAVA_NAME ${Kit})
  ELSE(${Kit} MATCHES VXLNumerics)
    SET(KIT_JAVA_NAME OTB${Kit})
  ENDIF(${Kit} MATCHES VXLNumerics)
  ADD_DEPENDENCIES(OTBJavaJar "${KIT_JAVA_NAME}Java")
ENDFOREACH(Kit)
ADD_LIBRARY(OTBJavaJarDummyLibrary OTBJavaJarDummyLibrary.c)
ADD_DEPENDENCIES(OTBJavaJarDummyLibrary OTBJavaJar)
