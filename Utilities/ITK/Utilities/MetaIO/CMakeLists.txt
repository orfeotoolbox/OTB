ADD_LIBRARY(ITKMetaIO
  metaArrow.cxx
  metaBlob.cxx
  metaCommand.h
  metaCommand.cxx
  metaDTITube.cxx
  metaEllipse.cxx
  metaGroup.cxx
  metaGaussian.cxx
  metaImage.cxx
  metaImageUtils.cxx
  metaLandmark.cxx
  metaLine.cxx
  metaMesh.cxx
  metaMesh.h
  metaObject.cxx
  metaScene.cxx
  metaSurface.cxx
  metaTube.cxx
  metaTypes.h
  metaUtils.cxx
  metaVesselTube.cxx
  metaTransform.cxx
  metaTubeGraph.cxx
)

#SUBDIRS(tests)

INSTALL_TARGETS(/lib/otb ITKMetaIO)
INSTALL_FILES(/include/otb/Utilities/ITK/Utilities/MetaIO "(\\.h)$")
TARGET_LINK_LIBRARIES(ITKMetaIO ${ITK_ZLIB_LIBRARIES})
