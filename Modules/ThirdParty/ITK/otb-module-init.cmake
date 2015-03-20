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
    ITKNeuralNetworks
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

find_package ( ITK COMPONENTS ${OTBITK_ITK_REQUIRED_MODULE_LIST})

# set ( ITK_NO_IO_FACTORY_REGISTER_MANAGER TRUE )
# include (${ITK_USE_FILE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ITK_REQUIRED_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ITK_REQUIRED_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")
