# NOTE TLA : to move later to upper level.
# Old behavior was controled by otb_module_activation_option in otb-module files
option(OTB_USE_6S "Enable 6S radiometric and atmospheric correction module" ON)
option(OTB_USE_GSL "Enable GSL dependent modules" OFF)
option(OTB_USE_LIBKML "Enable libkml dependent modules" OFF)
option(OTB_USE_SIFTFAST "Enable SiftFast dependent modules" ON)

# NOTE TLA: Make a Dependency resolver file
if(OTBGroup_Core AND NOT OTBGroup_Learning)
  find_package (Boost 1.73.0 REQUIRED COMPONENTS filesystem)
else()
  find_package (Boost 1.73.0 REQUIRED COMPONENTS filesystem serialization)
endif()
if (BUILD_TESTING)
  find_package (Boost 1.73.0 QUIET COMPONENTS filesystem serialization unit_test_framework)
  if (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND)
    message(STATUS "Boost unit_test_framework not found. Hence test depending on this framework will be skipped")
  else()
    message(STATUS "Found Boost components: unit_test_framework")
  endif()
endif() #BUILD_TESTING

find_package ( CURL REQUIRED )
find_package ( GDAL REQUIRED )
find_package ( GeoTIFF REQUIRED )
if (OTB_USE_GSL)
  find_package(GSL REQUIRED)
endif()
if (OTB_USE_LIBKML)
  find_package ( LibKML REQUIRED )
endif()

find_package(TIFF REQUIRED)
find_package (TinyXML REQUIRED )

set(OTBITK_ITK_REQUIRED_MODULE_LIST 
    ITKCommon
    ITKFiniteDifference
    ITKGPUCommon
    ITKGPUFiniteDifference
    ITKImageAdaptors
    ITKImageFunction
    ITKMesh
    ITKQuadEdgeMesh
    ITKSpatialObjects
    #ITKTestKernel
    ITKTransform

    ITKAnisotropicSmoothing
    ITKAntiAlias
    ITKBiasCorrection
    ITKBinaryMathematicalMorphology
    ITKColormap
    ITKConvolution
    ITKCurvatureFlow
    ITKDeconvolution
    ITKDenoising
    #ITKDiffusionTensorImage
    ITKDisplacementField
    ITKDistanceMap
    ITKFastMarching
    ITKFFT
    ITKGPUAnisotropicSmoothing
    ITKGPUImageFilterBase
    ITKGPUSmoothing
    ITKGPUThresholding
    ITKImageCompare
    ITKImageCompose
    ITKImageFeature
    ITKImageFilterBase
    ITKImageFusion
    ITKImageGradient
    ITKImageGrid
    ITKImageIntensity
    ITKImageLabel
    ITKImageSources
    ITKImageStatistics
    ITKLabelMap
    ITKMathematicalMorphology
    ITKPath
    ITKQuadEdgeMeshFiltering
    ITKSmoothing
    ITKSpatialFunction
    ITKThresholding

    ITKEigen
    #ITKFEM
    ITKNarrowBand
    ITKOptimizers
    ITKOptimizersv4
    ITKPolynomials
    ITKStatistics

    ITKRegistrationCommon
    #ITKFEMRegistration
    ITKGPURegistrationCommon
    ITKGPUPDEDeformableRegistration
    ITKMetricsv4
    ITKPDEDeformableRegistration
    ITKRegistrationMethodsv4

    #ITKBioCell
    ITKClassifiers
    ITKConnectedComponents
    ITKDeformableMesh
    ITKKLMRegionGrowing
    ITKLabelVoting
    ITKLevelSets
    ITKLevelSetsv4
    #ITKLevelSetsv4Visualization
    ITKMarkovRandomFieldsClassifiers
    ITKRegionGrowing
    ITKSignedDistanceFunction
    ITKVoronoi
    ITKWatersheds)
find_package(ITK COMPONENTS ${OTBITK_ITK_REQUIRED_MODULE_LIST})
