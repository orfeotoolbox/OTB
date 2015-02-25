project(OTBLibSVM)
set(OTBLibSVM_THIRD_PARTY 1)

set(OTBLibSVM_INCLUDE_DIRS ${OTBLibSVM_SOURCE_DIR}/src)
set(OTBLibSVM_LIBRARIES otblibsvm)

otb_module_impl()
