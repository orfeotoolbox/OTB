MESSAGE(STATUS "Importing FLTK...")
OPTION(OTB_USE_VISU_GUI "Generate Gui/Visu tools directory. If ON, find OpenGL and FLTK packages." ON)
MARK_AS_ADVANCED(OTB_USE_VISU_GUI)

SET(OTB_GLU_INCLUDE_PATH "")
IF(OTB_USE_VISU_GUI)

        #-------------------------------
        # OpenGL Library
        #-------------------------------
        FIND_PACKAGE(OpenGL)

        IF( NOT OPENGL_FOUND )
                MESSAGE(FATAL_ERROR "Unable to find OpenGL on your system.\nCannot build OTB Visu module without OpenGL library.\nInstall OpenGL on your system OR set OTB_USE_VISU_GUI to OFF to disable OTB Visu module generation")
        ENDIF( NOT OPENGL_FOUND )
        IF( NOT OPENGL_GLU_FOUND )
                MESSAGE(FATAL_ERROR "Unable to find GLU library on your system.\nCannot build OTB Visu module without GLU library.\nInstall GLU library on your system OR set OTB_USE_VISU_GUI to OFF to disable OTB Visu module generation")
        ENDIF( NOT OPENGL_GLU_FOUND )

        IF(OPENGL_INCLUDE_DIR)
                INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
        ENDIF(OPENGL_INCLUDE_DIR)

        #-------------------------------
        # Display images using textures using video card acceleration
        #-------------------------------

        # This option has generated numerous issues
        # It usually does not work when doing remote display.
        # Simply disable the possibility to activate it
        #OPTION(OTB_GL_USE_ACCEL "Use video card acceleration." OFF)
        #MARK_AS_ADVANCED(OTB_GL_USE_ACCEL)

        #-------------------------------
        # FLTK Library
        #-------------------------------

        # Use an external version of FLTK by default
        # FLTK is now packaged on most systems
        # Internal FLTK version is 1.1 and there is some issue Monteverdi
        OPTION(OTB_USE_EXTERNAL_FLTK "Use an outside build of FLTK." ON)
        MARK_AS_ADVANCED(OTB_USE_EXTERNAL_FLTK)

        IF(OTB_USE_EXTERNAL_FLTK)

          FIND_PACKAGE(FLTK)
          IF(NOT FLTK_FOUND)
            MESSAGE(FATAL_ERROR
                  "Cannot build OTB project without FLTK. Please set FLTK_DIR or set OTB_USE_EXTERNAL_FLTK OFF to use INTERNAL FLTK.")
          ENDIF(NOT FLTK_FOUND)

          # Only if we used a FLTK 1.3.0 because in FLTK 1.1.>=9 is already done
          IF(FLTK_USE_FILE)
            INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})

            # UseFLTK is buggy on windows, so we do not import it
            # The definition of FLTK_EXE_LINKER_FLAGS in FLTKConfig.cmake
            # is not valid and we end up with
            # CMAKE_EXE_LINKER_FLAGS = " /STACK:10000000 /machine:X86 ; /STACK:10000000 /machine:X86"
            # The ";" in the middle creates link error :
            # impossible d'ouvrir le fichier en entrΘe ';.obj'

            # INCLUDE(${FLTK_USE_FILE})
          ENDIF(FLTK_USE_FILE)

        ELSE(OTB_USE_EXTERNAL_FLTK)

          # Same Set as ITK
          SET(FLTK_SOURCE_DIR ${OTB_SOURCE_DIR}/Utilities/FLTK)
          SET(FLTK_BINARY_DIR ${OTB_BINARY_DIR}/Utilities/FLTK)
          # Additionnal variables needed by OTB_VISU_GUI_LIBRARIES and FLTK_FLUID_EXECUTABLE
          SET(FLTK_LIBRARIES "fltk_images;fltk;fltk_gl;fltk_forms")
          SET(FLUID_COMMAND fluid)
          # FLTK CMake needs a variable called JPEG_INCLUDE_DIR
          SET(JPEG_INCLUDE_DIR ${JPEG_INCLUDE_DIRS})

          # Needed for version 1.1.>=9
          SET(FLTK_FLUID_EXECUTABLE ${FLUID_COMMAND})

        ENDIF(OTB_USE_EXTERNAL_FLTK)

        SET(OTB_VISU_GUI_LIBRARIES "${FLTK_LIBRARIES}")
        IF(APPLE)
          SET(OTB_VISU_GUI_LIBRARIES "${OTB_VISU_GUI_LIBRARIES};-Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib")
        ENDIF(APPLE)

        IF(OTB_USE_EXTERNAL_FLTK)
          MESSAGE(STATUS "  Using FLTK external version ${FLTK_EXTERNAL_VERSION}")
          MESSAGE(STATUS "  FLTK includes : ${FLTK_INCLUDE_DIR}")
          MESSAGE(STATUS "  FLTK libraries: ${FLTK_LIBRARIES}")
          MESSAGE(STATUS "  FLUID executable = ${FLTK_FLUID_EXECUTABLE}")
        ELSE(OTB_USE_EXTERNAL_FLTK)
          MESSAGE(STATUS "  Using FLTK internal version")
        ENDIF(OTB_USE_EXTERNAL_FLTK)

ELSE(OTB_USE_VISU_GUI)

  SET(OTB_VISU_GUI_LIBRARIES "")
  MESSAGE(STATUS "  Disabling FLTK support")

ENDIF(OTB_USE_VISU_GUI)
