find_package(GLFW REQUIRED)

mark_as_advanced(GLFW_INCLUDE_DIR)
mark_as_advanced(GLFW_LIBRARY)

if(NOT GLFW_FOUND)
 message(FATAL_ERROR "Cannot find GLFW. Set GLFW_INCLUDE_DIR and GLFW_LIBRARY")
endif()
