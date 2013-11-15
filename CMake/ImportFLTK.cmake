message(STATUS "Importing FLTK...")
option(OTB_USE_VISU_GUI "Generate Gui/Visu tools directory. If ON, find OpenGL and FLTK packages." ON)
mark_as_advanced(OTB_USE_VISU_GUI)

set(OTB_GLU_INCLUDE_PATH "")
if(OTB_USE_VISU_GUI)

        #-------------------------------
        # OpenGL Library
        #-------------------------------
        find_package(OpenGL)

        if( NOT OPENGL_FOUND )
                message(FATAL_ERROR "Unable to find OpenGL on your system.\nCannot build OTB Visu module without OpenGL library.\nInstall OpenGL on your system OR set OTB_USE_VISU_GUI to OFF to disable OTB Visu module generation")
        endif()
        if( NOT OPENGL_GLU_FOUND )
                message(FATAL_ERROR "Unable to find GLU library on your system.\nCannot build OTB Visu module without GLU library.\nInstall GLU library on your system OR set OTB_USE_VISU_GUI to OFF to disable OTB Visu module generation")
        endif()

        if(OPENGL_INCLUDE_DIR)
                include_directories(${OPENGL_INCLUDE_DIR})
        endif()

        #-------------------------------
        # Display images using textures using video card acceleration
        #-------------------------------

        # This option has generated numerous issues
        # It usually does not work when doing remote display.
        # Simply disable the possibility to activate it
        #option(OTB_GL_USE_ACCEL "Use video card acceleration." OFF)
        #mark_as_advanced(OTB_GL_USE_ACCEL)

        #-------------------------------
        # FLTK Library
        #-------------------------------

        # Use an external version of FLTK by default except on windows (FLTK is not packaged in OSGeo4W)
        set(DEFAULT_OTB_USE_EXTERNAL_FLTK ON)
        if(WIN32)
          set(DEFAULT_OTB_USE_EXTERNAL_FLTK OFF)
        endif()

        option(OTB_USE_EXTERNAL_FLTK "Use an outside build of FLTK." ${DEFAULT_OTB_USE_EXTERNAL_FLTK})
        mark_as_advanced(OTB_USE_EXTERNAL_FLTK)

        if(OTB_USE_EXTERNAL_FLTK)

          find_package(FLTK)
          if(NOT FLTK_FOUND)
            message(FATAL_ERROR
                  "Cannot build OTB project without FLTK. Please set FLTK_DIR or set OTB_USE_EXTERNAL_FLTK OFF to use INTERNAL FLTK.")
          endif()

          # Only if we used a FLTK 1.3.0 because in FLTK 1.1.>=9 is already done
          if(FLTK_USE_FILE)
            include_directories(${FLTK_INCLUDE_DIR})

            # UseFLTK is buggy on windows, so we do not import it
            # The definition of FLTK_EXE_LINKER_FLAGS in FLTKConfig.cmake
            # is not valid and we end up with
            # CMAKE_EXE_LINKER_FLAGS = " /STACK:10000000 /machine:X86 ; /STACK:10000000 /machine:X86"
            # The ";" in the middle creates link error :
            # impossible d'ouvrir le fichier en entrΘe ';.obj'

            # include(${FLTK_USE_FILE})
          endif()

        else()

          # Same Set as ITK
          set(FLTK_SOURCE_DIR ${OTB_SOURCE_DIR}/Utilities/FLTK)
          set(FLTK_BINARY_DIR ${OTB_BINARY_DIR}/Utilities/FLTK)
          # Additionnal variables needed by OTB_VISU_GUI_LIBRARIES and FLTK_FLUID_EXECUTABLE
          set(FLTK_LIBRARIES "fltk_images;fltk;fltk_gl;fltk_forms")
          set(FLUID_COMMAND fluid)
          # FLTK CMake needs a variable called JPEG_INCLUDE_DIR
          set(JPEG_INCLUDE_DIR ${JPEG_INCLUDE_DIRS})

          # FindPNG does not define libpng13 as a possible libpng name
          set(PNG_NAMES libpng13)

          # Needed for version 1.1.>=9
          set(FLTK_FLUID_EXECUTABLE ${FLUID_COMMAND})

        endif()

        set(OTB_VISU_GUI_LIBRARIES "${FLTK_LIBRARIES}")
        if(APPLE)
          set(OTB_VISU_GUI_LIBRARIES "${OTB_VISU_GUI_LIBRARIES};-Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib")
        endif()

        if(OTB_USE_EXTERNAL_FLTK)
          message(STATUS "  Using FLTK external version ${FLTK_EXTERNAL_VERSION}")
          message(STATUS "  FLTK includes : ${FLTK_INCLUDE_DIR}")
          message(STATUS "  FLTK libraries: ${FLTK_LIBRARIES}")
          message(STATUS "  FLUID executable = ${FLTK_FLUID_EXECUTABLE}")
        else()
          message(STATUS "  Using FLTK internal version")
        endif()

else()

  set(OTB_VISU_GUI_LIBRARIES "")
  message(STATUS "  Disabling FLTK support")

endif()
