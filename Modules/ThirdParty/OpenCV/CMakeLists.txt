project(OTBOpenCV)

set(OTBOpenCV_SYSTEM_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
set(OTBOpenCV_LIBRARIES "${OpenCV_LIBS}")

otb_module_impl()
