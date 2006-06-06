# Sources of non-templated classes.
SET(ITKSpatialObject_SRCS
  itkCylinderSpatialObject.cxx
  itkSpatialObjectFactoryBase.cxx
)

ADD_LIBRARY(ITKSpatialObject ${ITKSpatialObject_SRCS})
TARGET_LINK_LIBRARIES(ITKSpatialObject ITKNumerics ITKCommon ITKMetaIO) 
INSTALL_TARGETS(/lib/otb ITKSpatialObject)
INSTALL_FILES(/include/otb/Utilities/ITK/SpatialObject "(\\.h|\\.txx)$")
