IF(OTB_USE_VISU_GUI)
        IF(NOT OTB_USE_EXTERNAL_FLTK)
            MESSAGE(STATUS "Configuring FLTK...")
	        ADD_SUBDIRECTORY( FLTK )
        ENDIF(NOT OTB_USE_EXTERNAL_FLTK)
ENDIF(OTB_USE_VISU_GUI)

IF(NOT OTB_USE_EXTERNAL_ITK)
  MESSAGE(STATUS "Configuring ITK...")

  set(ITK_BUILD_ALL_MODULES OFF CACHE INTERNAL "" FORCE)

  set(OTB_BUILD_EXAMPLES ${BUILD_EXAMPLES})
  set(BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)
  set(OTB_BUILD_TESTING ${BUILD_TESTING})
  set(BUILD_TESTING OFF CACHE INTERNAL "" FORCE)

  set(ITK_MODULES_LIST
    Module_ITKCommon
    Module_ITKFiniteDifference
    Module_ITKGPUCommon
    Module_ITKGPUFiniteDifference
    Module_ITKImageAdaptors
    Module_ITKImageFunction
    Module_ITKMesh
    Module_ITKQuadEdgeMesh
    Module_ITKSpatialObjects
    #Module_ITKTestKernel
    Module_ITKTransform

    Module_ITKAnisotropicSmoothing
    Module_ITKAntiAlias
    Module_ITKBiasCorrection
    Module_ITKBinaryMathematicalMorphology
    Module_ITKColormap
    Module_ITKConvolution
    Module_ITKCurvatureFlow
    Module_ITKDeconvolution
    Module_ITKDenoising
    #Module_ITKDiffusionTensorImage
    Module_ITKDisplacementField
    Module_ITKDistanceMap
    Module_ITKFastMarching
    Module_ITKFFT
    Module_ITKGPUAnisotropicSmoothing
    Module_ITKGPUImageFilterBase
    Module_ITKGPUSmoothing
    Module_ITKGPUThresholding
    Module_ITKImageCompare
    Module_ITKImageCompose
    Module_ITKImageFeature
    Module_ITKImageFilterBase
    Module_ITKImageFusion
    Module_ITKImageGradient
    Module_ITKImageGrid
    Module_ITKImageIntensity
    Module_ITKImageLabel
    Module_ITKImageSources
    Module_ITKImageStatistics
    Module_ITKLabelMap
    Module_ITKMathematicalMorphology
    Module_ITKPath
    Module_ITKQuadEdgeMeshFiltering
    Module_ITKSmoothing
    Module_ITKSpatialFunction
    Module_ITKThresholding

    Module_ITKEigen
    #Module_ITKFEM
    Module_ITKNarrowBand
    Module_ITKNeuralNetworks
    Module_ITKOptimizers
    Module_ITKOptimizersv4
    Module_ITKPolynomials
    Module_ITKStatistics

    Module_ITKRegistrationCommon
    #Module_ITKFEMRegistration
    Module_ITKGPURegistrationCommon
    Module_ITKGPUPDEDeformableRegistration
    Module_ITKMetricsv4
    Module_ITKPDEDeformableRegistration
    Module_ITKRegistrationMethodsv4

    #Module_ITKBioCell
    Module_ITKClassifiers
    Module_ITKConnectedComponents
    Module_ITKDeformableMesh
    Module_ITKKLMRegionGrowing
    Module_ITKLabelVoting
    Module_ITKLevelSets
    Module_ITKLevelSetsv4
    #Module_ITKLevelSetsv4Visualization
    Module_ITKMarkovRandomFieldsClassifiers
    Module_ITKRegionGrowing
    Module_ITKSignedDistanceFunction
    Module_ITKVoronoi
    Module_ITKWatersheds
  )

  foreach(ITK_MODULE ${ITK_MODULES_LIST})
    set(${ITK_MODULE} ON CACHE BOOL "" FORCE)
  endforeach()

  # When running cmake outside of ctest (developer build)
  # ITKGroup_Core if ON by default, thus requiring ITKTestKernel that we don't want
  set(ITKGroup_Core OFF CACHE BOOL "" FORCE)
  
  # By default activate FFTW, but with an external fftw build
  # These variables are used in ITK to initialize the value of the ITK_USE_FFTW_XXX options
  set(USE_FFTWF ON)
  set(USE_FFTWD ON)
  set(USE_SYSTEM_FFTW ON)
  
  set(ITK_INSTALL_RUNTIME_DIR ${OTB_INSTALL_BIN_DIR})
  set(ITK_INSTALL_LIBRARY_DIR ${OTB_INSTALL_LIB_DIR})
  set(ITK_INSTALL_ARCHIVE_DIR ${OTB_INSTALL_LIB_DIR})
  set(ITK_INSTALL_INCLUDE_DIR ${OTB_INSTALL_INCLUDE_DIR}/Utilities/ITK)
  # Keep default values for these :
  # set(ITK_INSTALL_DATA_DIR share/ITK-${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR})
  # set(ITK_INSTALL_DOC_DIR share/doc/ITK-${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR})
  # set(ITK_INSTALL_PACKAGE_DIR "lib/cmake/ITK-${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}")

  add_subdirectory(ITK)

  # Load the ITKConfig generated at configuration time
  # Be sure to avoid loading an external ITK
  find_package(ITK
               REQUIRED
               NO_MODULE
               HINTS
                 ${CMAKE_CURRENT_BINARY_DIR}/ITK
               NO_DEFAULT_PATH
               )

  # Put ITK_INCLUDE_DIRS in a hidden cache variable
  # so that they are accessible in parent CMakeLists
  set(ITK_INCLUDE_DIRS ${ITK_INCLUDE_DIRS} CACHE INTERNAL "" FORCE)
  set(ITK_LIBRARIES ${ITK_LIBRARIES} CACHE INTERNAL "" FORCE)

  # Restore OTB variable state
  set(BUILD_TESTING ${OTB_BUILD_TESTING} CACHE BOOL "Build testing." FORCE)
  set(BUILD_EXAMPLES ${OTB_BUILD_EXAMPLES} CACHE BOOL "Build the Examples directory." FORCE)

  # Hide some exposed cmake var from the ITK build.
  foreach(group "Bridge;Compatibility;Core;Filtering;IO;Nonunit;Numerics;Registration;Segmentation;ThirdParty;Video")
    mark_as_advanced(ITKGroup_${group})
  endforeach()
  mark_as_advanced(ITKV3_COMPATIBILITY)
  mark_as_advanced(ITK_DIR)
  mark_as_advanced(ITK_WRAP_JAVA)
  mark_as_advanced(ITK_WRAP_PYTHON)

ENDIF(NOT OTB_USE_EXTERNAL_ITK)

IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  MESSAGE(STATUS "Configuring otbopenthreads...")
  ADD_SUBDIRECTORY( otbopenthreads )
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)


IF(OTB_USE_LIBLAS)
IF(NOT OTB_USE_EXTERNAL_LIBLAS)
  MESSAGE(STATUS "Configuring otbliblas...")
  ADD_SUBDIRECTORY( otbliblas )
ENDIF(NOT OTB_USE_EXTERNAL_LIBLAS)
ENDIF(OTB_USE_LIBLAS)

IF(OTB_USE_JPEG2000)
  set(OTB_BUILD_TESTING ${BUILD_TESTING})
  set(BUILD_TESTING OFF CACHE INTERNAL "" FORCE)

  MESSAGE(STATUS "Configuring otbopenjpeg...")
  ADD_SUBDIRECTORY( otbopenjpeg )

  set(BUILD_TESTING ${OTB_BUILD_TESTING} CACHE BOOL "Build testing." FORCE)
  set(BUILD_SHARED_LIBS ${OTB_BUILD_SHARED_LIBS} CACHE BOOL "Build OTB with shared libraries." FORCE)
ENDIF(OTB_USE_JPEG2000)


IF(NOT OTB_USE_EXTERNAL_EXPAT)
    MESSAGE(STATUS "Configuring otbexpat...")
	ADD_SUBDIRECTORY( otbexpat )
ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)

IF(NOT OTB_USE_EXTERNAL_BOOST)
  MESSAGE(STATUS "Configuring internal Boost...")
  ADD_SUBDIRECTORY(BGL)
ENDIF(NOT OTB_USE_EXTERNAL_BOOST)

MESSAGE(STATUS "Configuring otbsvm...")
ADD_SUBDIRECTORY(otbsvm )

MESSAGE(STATUS "Configuring InsightJournal...")
ADD_SUBDIRECTORY(InsightJournal)

IF(NOT OTB_USE_EXTERNAL_OSSIM)
  MESSAGE(STATUS "Configuring otbossim...")
  ADD_SUBDIRECTORY(otbossim)
ENDIF(NOT OTB_USE_EXTERNAL_OSSIM)

MESSAGE(STATUS "Configuring otbossimplugins...")
ADD_SUBDIRECTORY(otbossimplugins)

MESSAGE(STATUS "Configuring otb6S...")
ADD_SUBDIRECTORY(otb6S)

IF(NOT OTB_USE_EXTERNAL_TINYXML)
  MESSAGE(STATUS "Configuring tinyXMLlib...")
  ADD_SUBDIRECTORY(tinyXMLlib)
ENDIF(NOT OTB_USE_EXTERNAL_TINYXML)

IF(NOT OTB_USE_EXTERNAL_MUPARSER)
  MESSAGE(STATUS "Configuring otbmuparser...")
  ADD_SUBDIRECTORY(otbmuparser)
ENDIF(NOT OTB_USE_EXTERNAL_MUPARSER)

IF(NOT OTB_USE_EXTERNAL_LIBKML)
  MESSAGE(STATUS "Configuring otbkml...")
  ADD_SUBDIRECTORY(otbkml)
ENDIF(NOT OTB_USE_EXTERNAL_LIBKML)

MESSAGE(STATUS "Configuring otbedison...")
ADD_SUBDIRECTORY(otbedison)

IF(OTB_USE_SIFTFAST)
  MESSAGE(STATUS "Configuring otbsiftfast...")
  ADD_SUBDIRECTORY(otbsiftfast)
ENDIF(OTB_USE_SIFTFAST)

MESSAGE(STATUS "Configuring otbconfigfile...")
ADD_SUBDIRECTORY(otbconfigfile)

IF(WIN32)
MESSAGE(STATUS "Configuring otbmsinttypes...")
ADD_SUBDIRECTORY(otbmsinttypes)
ENDIF(WIN32)

IF(BUILD_TESTING)
        ADD_SUBDIRECTORY( Dart )
ENDIF(BUILD_TESTING)


#-----------------------------------------------------------------------------
# Install the UseOTB.cmake file configured from the top-level.
IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES ${OTB_BINARY_DIR}/Utilities/OTBConfig.cmake
    DESTINATION ${OTB_INSTALL_PACKAGE_DIR}
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
