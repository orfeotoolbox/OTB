cmake_minimum_required(VERSION 2.8.3)

project(libsvm)

option(BUILD_SHARED_LIBS "Building shared libs. Default is ON" ON)

set(LIBSVM_SRCS "svm.cpp" )
set(LIBSVM_HEADERS "svm.h")

add_library(svm ${LIBSVM_SRCS})

option(BUILD_SAMPLES "Build executables" ON)

add_executable(svm-train svm-train.c)
target_link_libraries(svm-train svm)

add_executable(svm-predict svm-predict.c)
target_link_libraries(svm-predict svm)

add_executable(svm-scale svm-scale.c)
target_link_libraries(svm-scale svm)

install(TARGETS svm
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(FILES ${LIBSVM_HEADERS} DESTINATION include)
