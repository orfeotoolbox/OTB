IF(WIN32)
  IF(NOT MINGW)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/AllowContinuous.bat.in
                 ${OTB_BINARY_DIR}/AllowContinuous.bat)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/PreventContinuous.bat.in
                 ${OTB_BINARY_DIR}/PreventContinuous.bat)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuous.bat.in
                 ${OTB_BINARY_DIR}/BuildContinuous.bat)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuousWithCTest.bat.in
                 ${OTB_BINARY_DIR}/BuildContinuousWithCTest.bat)
  ELSE(NOT MINGW)
    CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuous.sh.in
                   ${OTB_BINARY_DIR}/BuildContinuous.sh
                   @ONLY IMMEDIATE)
    EXEC_PROGRAM(chmod ARGS 755 \"${OTB_BINARY_DIR}/BuildContinuous.sh\"
                       OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
    CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuousWithCTest.sh.in
                   ${OTB_BINARY_DIR}/BuildContinuousWithCTest.sh
                   @ONLY IMMEDIATE)
    EXEC_PROGRAM(chmod ARGS 755 \"${OTB_BINARY_DIR}/BuildContinuousWithCTest.sh\"
                       OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
    CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/AllowContinuous.sh.in
                   ${OTB_BINARY_DIR}/AllowContinuous.sh
                   @ONLY IMMEDIATE)
    EXEC_PROGRAM(chmod ARGS 755 \"${OTB_BINARY_DIR}/AllowContinuous.sh\"
                       OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
    CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/PreventContinuous.sh.in
                   ${OTB_BINARY_DIR}/PreventContinuous.sh
                   @ONLY IMMEDIATE)
    EXEC_PROGRAM(chmod ARGS 755 \"${OTB_BINARY_DIR}/PreventContinuous.sh\"
                       OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
    CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/cronentry.in
                   ${OTB_BINARY_DIR}/cronentry
                   @ONLY IMMEDIATE)
  ENDIF(NOT MINGW)
ENDIF(WIN32)
IF(UNIX)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuous.sh.in
                 ${OTB_BINARY_DIR}/BuildContinuous.sh
                 @ONLY IMMEDIATE)
  EXEC_PROGRAM(chmod ARGS 755 \"${OTB_BINARY_DIR}/BuildContinuous.sh\"
                     OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuousWithCTest.sh.in
                 ${OTB_BINARY_DIR}/BuildContinuousWithCTest.sh
                 @ONLY IMMEDIATE)
  EXEC_PROGRAM(chmod ARGS 755 \"${OTB_BINARY_DIR}/BuildContinuousWithCTest.sh\"
                       OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/AllowContinuous.sh.in
                 ${OTB_BINARY_DIR}/AllowContinuous.sh
                 @ONLY IMMEDIATE)
  EXEC_PROGRAM(chmod ARGS 755 \"${OTB_BINARY_DIR}/AllowContinuous.sh\"
                     OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/PreventContinuous.sh.in
                 ${OTB_BINARY_DIR}/PreventContinuous.sh
                 @ONLY IMMEDIATE)
  EXEC_PROGRAM(chmod ARGS 755 \"${OTB_BINARY_DIR}/PreventContinuous.sh\"
                     OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/cronentry.in
                 ${OTB_BINARY_DIR}/cronentry
                 @ONLY IMMEDIATE)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/NewSince.csh.in
                 ${OTB_BINARY_DIR}/NewSince.csh
                 @ONLY IMMEDIATE)
  EXEC_PROGRAM(chmod ARGS 755 \"${OTB_BINARY_DIR}/NewSince.csh\"
                 OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/Dart/ChangedSince.csh.in
                 ${OTB_BINARY_DIR}/ChangedSince.csh
                 @ONLY IMMEDIATE)
  EXEC_PROGRAM(chmod ARGS 755 \"${OTB_BINARY_DIR}/ChangedSince.csh\"
                 OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
ENDIF(UNIX)
