if(NOT FLTK11_FOUND)
  message(FATAL_ERROR "Something went wrong. You are including FLTKUse.cmake but FLTK was not found")
endif(NOT FLTK11_FOUND)

# -------------------------------------------------------------
# This macro automates wrapping of Fluid files
# Specify the output variable name and the list of sources
# The output variable can be directly added to the target.
#
# For example:
#   FLTK_WRAP_FLUID(CubeView_SRCS CubeViewUI.fl)
#   add_executable(CubeView CubeMain.cxx CubeView.cxx CubeView.h ${CubeView_SRCS})
# -------------------------------------------------------------
macro(FLTK_WRAP_FLUID VARIABLE)
  foreach(src ${ARGN})
    if("${src}" MATCHES ".fl$")
      get_filename_component(fname ${src} NAME_WE)
      get_filename_component(fpath ${src} PATH)
      get_source_file_property(gen ${src} GENERATED)
      if(gen)
        set(fluid_name "${src}")
      else(gen)
        set(fluid_name "${CMAKE_CURRENT_SOURCE_DIR}/${fpath}/${fname}.fl")
        if(NOT EXISTS "${fluid_name}")
          set(fluid_name "${CMAKE_CURRENT_BINARY_DIR}/${fpath}/${fname}.fl")
          if(NOT EXISTS "${fluid_name}")
            set(fluid_name "${fpath}/${fname}.fl")
            if(NOT EXISTS "${fluid_name}")
              message(SEND_ERROR "Cannot find Fluid source file: ${fpath}/${fname}.fl")
            endif(NOT EXISTS "${fluid_name}")
          endif(NOT EXISTS "${fluid_name}")
        endif(NOT EXISTS "${fluid_name}")
      endif(gen)
      set(cxx_name "${CMAKE_CURRENT_BINARY_DIR}/${fname}.cxx")
      set(h_name "${CMAKE_CURRENT_BINARY_DIR}/${fname}.h")
      set(${VARIABLE} "${${VARIABLE}};${cxx_name}")
      add_custom_command(
        OUTPUT ${cxx_name}
        DEPENDS "${fluid_name}" "${FLUID_COMMAND}"
        COMMAND ${FLUID_COMMAND}
        ARGS -c ${fluid_name})
      add_custom_command(
        OUTPUT ${h_name}
        DEPENDS "${fluid_name}" "${FLUID_COMMAND}"
        COMMAND ${FLUID_COMMAND}
        ARGS -c ${fluid_name})
    endif("${src}" MATCHES ".fl$")
  endforeach(src)
endmacro(FLTK_WRAP_FLUID VARIABLE)


# Make FLTK easier to use
include_directories(${FLTK_INCLUDE_DIRS})
link_directories(${FLTK_LIBRARY_DIRS})

# Load the compiler settings used for FLTK.
if(FLTK_BUILD_SETTINGS_FILE)
  include(CMakeImportBuildSettings)
  CMAKE_IMPORT_BUILD_SETTINGS(${FLTK_BUILD_SETTINGS_FILE})
endif(FLTK_BUILD_SETTINGS_FILE)

# Add compiler flags needed to use FLTK.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLTK_REQUIRED_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLTK_REQUIRED_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${FLTK_REQUIRED_EXE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${FLTK_REQUIRED_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${FLTK_REQUIRED_MODULE_LINKER_FLAGS}")

