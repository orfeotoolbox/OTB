find_package(LibSVM REQUIRED)

mark_as_advanced(LIBSVM_INCLUDE_DIR)
mark_as_advanced(LIBSVM_LIBRARY)

if(NOT LIBSVM_FOUND)
 message(FATAL_ERROR "Cannot find LibSVM. Set LIBSVM_INCLUDE_DIR and LIBSVM_LIBRARY")
endif()
