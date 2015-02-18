project(OTBOpenThreads)

set(OTBOpenThreads_SYSTEM_INCLUDE_DIRS ${OPENTHREADS_INCLUDE_DIRS})
set(OTBOpenThreads_LIBRARIES "${OPENTHREADS_LIBRARIES}")

otb_module_impl()
