
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

find_program(SH_INTERP sh)
if(NOT SH_INTERP)
  message(FATAL_ERROR "'sh' program not found")
endif()

find_program(SPHINX_BUILD sphinx-build)

if(NOT SPHINX_BUILD)
  message(FATAL_ERROR "The 'sphinx-build' command was not found. Make sure you have Sphinx installed, then set the SPHINXBUILD environment variable to point to the full path of the 'sphinx-build' executable. Alternatively you can add the directory with the executable to your PATH. If you don't have Sphinx installed, grab it from http://sphinx-doc.org")
endif()

string(TIMESTAMP OTB_COPYRIGHT_YEAR  "%Y")
set(OTB_COPYRIGHT_TEXT "${OTB_COPYRIGHT_YEAR}, OTB Team")

set(RST_GENERATED_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/rst)

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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMake/RunApplicationsRstGenerator.sh.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RunApplicationsRstGenerator.sh
  @ONLY)

file(GLOB mvd_images ${CMAKE_CURRENT_SOURCE_DIR}/Art/MonteverdiImages/*.*)
foreach(mvd_image ${mvd_images})
  get_filename_component(out_file ${mvd_image} NAME)
  configure_file(${mvd_image} ${RST_GENERATED_SOURCE_DIR}/Art/MonteverdiImages/${out_file} COPYONLY)
endforeach()

file(GLOB qt_images ${CMAKE_CURRENT_SOURCE_DIR}/Art/QtImages/*.png)
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

add_custom_target(generate_otbapps_rst
  ALL
  COMMAND ${SH_INTERP} ${CMAKE_CURRENT_BINARY_DIR}/RunApplicationsRstGenerator.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Auto-generating Application Reference Documentation in RST"
  )

add_custom_target(CookBookHTMLFromRST
  ALL
  COMMAND ${CMAKE_MAKE_PROGRAM} html
  WORKING_DIRECTORY ${RST_GENERATED_SOURCE_DIR}
  DEPENDS generate_otbapps_rst
  COMMENT "Building RST documentation in html")

# add_custom_target(CookBookHTMLFromPDF
#   ALL
#   COMMAND ${CMAKE_MAKE_PROGRAM} pdf
#   WORKING_DIRECTORY ${RST_GENERATED_SOURCE_DIR}
#   COMMENT "Building RST documentation in pdf")

# add_dependencies(CookBookHTMLFromPDF CookBookHTMLFromRST)
