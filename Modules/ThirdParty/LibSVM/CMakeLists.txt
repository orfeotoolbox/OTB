project(OTBLibSVM)
set(OTBLibSVM_THIRD_PARTY 1)

configure_file( src/otb_libsvm.h.in src/otb_libsvm.h )

set(OTBLibSVM_SYSTEM_INCLUDE_DIRS ${LIBSVM_INCLUDE_DIRS})
set(OTBLibSVM_LIBRARIES "${LIBSVM_LIBRARIES}")
set(OTBLibSVM_INCLUDE_DIRS
  ${OTBLibSVM_BINARY_DIR}/src
  )

otb_module_impl()

install(FILES ${OTBLibSVM_BINARY_DIR}/src/otb_libsvm.h
  DESTINATION ${OTBLibSVM_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
