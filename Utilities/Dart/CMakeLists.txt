if(WIN32)
  if(NOT MINGW)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/AllowContinuous.bat.in
                 ${OTB_BINARY_DIR}/AllowContinuous.bat)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/PreventContinuous.bat.in
                 ${OTB_BINARY_DIR}/PreventContinuous.bat)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuous.bat.in
                 ${OTB_BINARY_DIR}/BuildContinuous.bat)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuousWithCTest.bat.in
                 ${OTB_BINARY_DIR}/BuildContinuousWithCTest.bat)
  else()
    configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuous.sh.in
                   ${OTB_BINARY_DIR}/BuildContinuous.sh
                   @ONLY IMMEDIATE)
    exec_program(chmod ARGS 755 \"${OTB_BINARY_DIR}/BuildContinuous.sh\"
                       OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
    configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuousWithCTest.sh.in
                   ${OTB_BINARY_DIR}/BuildContinuousWithCTest.sh
                   @ONLY IMMEDIATE)
    exec_program(chmod ARGS 755 \"${OTB_BINARY_DIR}/BuildContinuousWithCTest.sh\"
                       OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
    configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/AllowContinuous.sh.in
                   ${OTB_BINARY_DIR}/AllowContinuous.sh
                   @ONLY IMMEDIATE)
    exec_program(chmod ARGS 755 \"${OTB_BINARY_DIR}/AllowContinuous.sh\"
                       OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
    configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/PreventContinuous.sh.in
                   ${OTB_BINARY_DIR}/PreventContinuous.sh
                   @ONLY IMMEDIATE)
    exec_program(chmod ARGS 755 \"${OTB_BINARY_DIR}/PreventContinuous.sh\"
                       OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
    configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/cronentry.in
                   ${OTB_BINARY_DIR}/cronentry
                   @ONLY IMMEDIATE)
  endif()
endif()
if(UNIX)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuous.sh.in
                 ${OTB_BINARY_DIR}/BuildContinuous.sh
                 @ONLY IMMEDIATE)
  exec_program(chmod ARGS 755 \"${OTB_BINARY_DIR}/BuildContinuous.sh\"
                     OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/BuildContinuousWithCTest.sh.in
                 ${OTB_BINARY_DIR}/BuildContinuousWithCTest.sh
                 @ONLY IMMEDIATE)
  exec_program(chmod ARGS 755 \"${OTB_BINARY_DIR}/BuildContinuousWithCTest.sh\"
                       OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/AllowContinuous.sh.in
                 ${OTB_BINARY_DIR}/AllowContinuous.sh
                 @ONLY IMMEDIATE)
  exec_program(chmod ARGS 755 \"${OTB_BINARY_DIR}/AllowContinuous.sh\"
                     OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/PreventContinuous.sh.in
                 ${OTB_BINARY_DIR}/PreventContinuous.sh
                 @ONLY IMMEDIATE)
  exec_program(chmod ARGS 755 \"${OTB_BINARY_DIR}/PreventContinuous.sh\"
                     OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/cronentry.in
                 ${OTB_BINARY_DIR}/cronentry
                 @ONLY IMMEDIATE)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/NewSince.csh.in
                 ${OTB_BINARY_DIR}/NewSince.csh
                 @ONLY IMMEDIATE)
  exec_program(chmod ARGS 755 \"${OTB_BINARY_DIR}/NewSince.csh\"
                 OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
  configure_file(${OTB_SOURCE_DIR}/Utilities/Dart/ChangedSince.csh.in
                 ${OTB_BINARY_DIR}/ChangedSince.csh
                 @ONLY IMMEDIATE)
  exec_program(chmod ARGS 755 \"${OTB_BINARY_DIR}/ChangedSince.csh\"
                 OUTPUT_VARIABLE OTB_EXEC_PLEASE_BE_QUIET)
endif()
