# Sources of non-templated classes.
SET(ITKSpatialObject_SRCS
  itkCylinderSpatialObject.cxx
  itkSpatialObjectFactoryBase.cxx
)

ADD_LIBRARY(ITKSpatialObject ${ITKSpatialObject_SRCS})
TARGET_LINK_LIBRARIES(ITKSpatialObject ITKNumerics ITKCommon ITKMetaIO) 
IF(ITK_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(ITKSpatialObject PROPERTIES ${ITK_LIBRARY_PROPERTIES})
ENDIF(ITK_LIBRARY_PROPERTIES)
INSTALL_TARGETS(/lib/otb ITKSpatialObject)
INSTALL_FILES(/include/otb/Utilities/ITK/SpatialObject "(\\.h|\\.txx)$")
