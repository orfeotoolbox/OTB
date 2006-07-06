
#FILE(GLOB OTBVisu_SRCS "*.cxx" )
SET ( OTBVisu_SRCS 
        toto.cxx
)

#FILE(GLOB OTBVisu_GUI_SRCS "*.fl" )

SET ( OTBVisu_GUI_SRCS 
#        otbVectorImageViewerGUI.fl
        otbImageViewerGUI.fl
		otbImageViewerASGUI.fl
)


IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
  ${OTB_SOURCE_DIR}/Code/Visu
  ${OTB_BINARY_DIR}/Code/Visu
)

# Take the list of GUI files, generate .cxx and .h from them and add
# them to the build process of the target
FLTK_WRAP_UI(OTBVisu ${OTBVisu_GUI_SRCS} )

ADD_LIBRARY(OTBVisu ${OTBVisu_SRCS} ${OTBVisu_FLTK_UI_SRCS})
#ADD_LIBRARY(OTBVisu ${OTBVisu_SRCS} )
INSTALL_FILES(/include/otb/Visu "(\\.h|\\.txx)$")
INSTALL_TARGETS(/lib/otb OTBVisu)
# A terme, remplacer par une macro qui a partir des sources .fl determine les .h
INSTALL_FILES(/include/otb/Visu FILES ${OTB_BINARY_DIR}/Code/Visu/otbImageViewerGUI.h ${OTB_BINARY_DIR}/Code/Visu/otbImageViewerASGUI.h)

TARGET_LINK_LIBRARIES(OTBVisu OTBLearning ${FLTK_LIBRARIES} ${OPENGL_glu_LIBRARY}
                      ${OPENGL_LIBRARIES})

# The fluid-generated fltk sources have many warnings.  Disable
# warnings for the generated files on some compilers.
IF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 1.6)
  IF(CMAKE_COMPILER_IS_GNUCXX)
    FOREACH(f ${OTBVisu_GUI_SRCS})
      STRING(REGEX REPLACE "\\.fl$" ".cxx" SRC "${f}")
      SET(SRC ${OTB_BINARY_DIR}/Code/Visu/${SRC})
      SET_SOURCE_FILES_PROPERTIES(${SRC} PROPERTIES COMPILE_FLAGS -w)
    ENDFOREACH(f)
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ENDIF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 1.6)
