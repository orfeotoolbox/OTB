
set(OUT_OF_SOURCE_BUILD FALSE)

if(NOT PROJECT_NAME)
  cmake_minimum_required(VERSION 2.8)
  project(OTBCookBook)
  set(OUT_OF_SOURCE_BUILD TRUE)
endif()

#find OTB
find_package(OTB REQUIRED)

#find Python
find_package(PythonInterp REQUIRED)

set(RST_GENERATED_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_sources)
set(OTB_COPYRIGHT_TEXT "2016, OTB Team")
set(RST_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")
set(RST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/rst)

#make some directories
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${RST_GENERATED_SOURCE_DIR}/Applications)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${RST_GENERATED_SOURCE_DIR}/recipes)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${RST_GENERATED_SOURCE_DIR}/Art)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${RST_GENERATED_SOURCE_DIR}/Art/MonteverdiImages)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${RST_GENERATED_SOURCE_DIR}/Art/QtImages)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${RST_GENERATED_SOURCE_DIR}/_static)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${RST_GENERATED_SOURCE_DIR}/_templates)

configure_file(${CMAKE_SOURCE_DIR}/CMake/RunApplicationsRstGenerator.sh.cmake.in
  ${RST_GENERATED_SOURCE_DIR}/RunApplicationsRstGenerator.sh
  @ONLY)

file(GLOB mvd_images ${CMAKE_SOURCE_DIR}/Art/MonteverdiImages/*.*)
foreach(mvd_image ${mvd_images})
  get_filename_component(out_file ${mvd_image} NAME)
  configure_file(${mvd_image} ${RST_GENERATED_SOURCE_DIR}/Art/MonteverdiImages/${out_file} COPYONLY)
endforeach()

file(GLOB qt_images ${CMAKE_SOURCE_DIR}/Art/QtImages/*.png)
foreach(qt_image ${qt_images})
  get_filename_component(out_file ${qt_image} NAME)
  configure_file(${qt_image} ${RST_GENERATED_SOURCE_DIR}/Art/QtImages/${out_file} COPYONLY)
endforeach()

file(GLOB rst_sources1 ${RST_SOURCES}/*.rst)
foreach(rst_file ${rst_sources1})
  get_filename_component(out_file ${rst_file} NAME)
  configure_file(${rst_file} ${RST_GENERATED_SOURCE_DIR}/${out_file} COPYONLY)
endforeach()

file(GLOB rst_sources2 ${RST_SOURCES}/recipes/*.rst)
foreach(rst_file ${rst_sources2})
  get_filename_component(out_file ${rst_file} NAME)
  configure_file(${rst_file} ${RST_GENERATED_SOURCE_DIR}/recipes/${out_file} COPYONLY)
endforeach()

file(GLOB rst_sources3 ${RST_SOURCES}/Applications/*.rst)
foreach(rst_file ${rst_sources3})
  get_filename_component(out_file ${rst_file} NAME)
  configure_file(${rst_file} ${RST_GENERATED_SOURCE_DIR}/Applications/${out_file} COPYONLY)
endforeach()

configure_file(${RST_SOURCES}/conf.py.in ${RST_GENERATED_SOURCE_DIR}/conf.py @ONLY)
configure_file(${RST_SOURCES}/Makefile.in ${RST_GENERATED_SOURCE_DIR}/Makefile @ONLY)

find_program(BASH_INTERP bash)

add_custom_target(generate_otbapps_rst
  ALL
  COMMAND ${BASH_INTERP} ${RST_GENERATED_SOURCE_DIR}/RunApplicationsRstGenerator.sh
  WORKING_DIRECTORY ${RST_GENERATED_SOURCE_DIR}
  COMMENT "Auto-generating Application Reference Documentation in RST"
  )

add_custom_target(CookBookHTMLFromRST
  ALL
  COMMAND ${CMAKE_MAKE_PROGRAM} html
  WORKING_DIRECTORY ${RST_GENERATED_SOURCE_DIR}
  DEPENDS generate_otbapps_rst
  COMMENT "Building RST documentation in html")

add_custom_target(CookBookHTMLFromPDF
  ALL
  COMMAND ${CMAKE_MAKE_PROGRAM} pdf
  WORKING_DIRECTORY ${RST_SOURCES}/RST
  COMMENT "Building RST documentation in pdf")

add_dependencies(CookBookHTMLFromPDF CookBookHTMLFromRST)
