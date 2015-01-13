message(STATUS "Setup ITK...")

set(proj ITK)

# Use an external version of ITK by default except on windows (ITK is not packaged in OSGeo4W)
set(DEFAULT_USE_SYSTEM_ITK OFF)

option(USE_SYSTEM_ITK "Use a system build of ITK." ${DEFAULT_USE_SYSTEM_ITK})
mark_as_advanced(USE_SYSTEM_ITK)

if(USE_SYSTEM_ITK)
  message(STATUS "  Using ITK system version")
else()
  set(ITK_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(ITK_SB_SRC_DIR ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  set(ITK_ENABLED_MODULES
    Common
    FiniteDifference
    GPUCommon
    GPUFiniteDifference
    ImageAdaptors
    ImageFunction
    Mesh
    QuadEdgeMesh
    SpatialObjects
    #TestKernel
    Transform

    AnisotropicSmoothing
    AntiAlias
    BiasCorrection
    BinaryMathematicalMorphology
    Colormap
    Convolution
    CurvatureFlow
    Deconvolution
    Denoising
    #DiffusionTensorImage
    DisplacementField
    DistanceMap
    FastMarching
    FFT
    GPUAnisotropicSmoothing
    GPUImageFilterBase
    GPUSmoothing
    GPUThresholding
    ImageCompare
    ImageCompose
    ImageFeature
    ImageFilterBase
    ImageFusion
    ImageGradient
    ImageGrid
    ImageIntensity
    ImageLabel
    ImageSources
    ImageStatistics
    LabelMap
    MathematicalMorphology
    Path
    QuadEdgeMeshFiltering
    Smoothing
    SpatialFunction
    Thresholding

    Eigen
    #FEM
    NarrowBand
    NeuralNetworks
    Optimizers
    Optimizersv4
    Polynomials
    Statistics

    RegistrationCommon
    #FEMRegistration
    GPURegistrationCommon
    GPUPDEDeformableRegistration
    Metricsv4
    PDEDeformableRegistration
    RegistrationMethodsv4

    #BioCell
    Classifiers
    ConnectedComponents
    DeformableMesh
    KLMRegionGrowing
    LabelVoting
    LevelSets
    LevelSetsv4
    #LevelSetsv4Visualization
    MarkovRandomFieldsClassifiers
    RegionGrowing
    SignedDistanceFunction
    Voronoi
    Watersheds
    )
  
  set(ITK_MODULES_CMAKE_CACHE)
  foreach(ITK_MODULE ${ITK_ENABLED_MODULES})
    list(APPEND ITK_MODULES_CMAKE_CACHE -DModule_ITK${ITK_MODULE}:BOOL=ON)
  endforeach()
  
  # By default activate FFTW, but with an external fftw build
  # These variables are used in ITK to initialize the value of the ITK_USE_FFTW_XXX options
  if (WIN32)
    set(FFTW_STATUS OFF)
  else()
    set(FFTW_STATUS ON)
  endif()
  
  # TODO : handle different build type (Release/Debug)
  
  # TODO : handle system zlib
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/itk/files/itk/4.6/InsightToolkit-4.6.1.tar.gz/download"
    URL_MD5 2c84eae50ab2452cdad32aaadced3c37
    BINARY_DIR ${ITK_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DITK_BUILD_DEFAULT_MODULES:BOOL=OFF
      ${ITK_MODULES_CMAKE_CACHE}
      -DITKGroup_Core:BOOL=OFF
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DBUILD_TESTING:BOOL=OFF
      -DBUILD_EXAMPLES:BOOL=OFF
      -DUSE_FFTWF:BOOL=${FFTW_STATUS}
      -DUSE_FFTWD:BOOL=${FFTW_STATUS}
      #-DUSE_SYSTEM_FFTW:BOOL=${FFTW_STATUS}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
      ${CMAKE_SOURCE_DIR}/patches/ITK/hashtable.hxx.in
      ${ITK_SB_SRC_DIR}/Modules/ThirdParty/KWSys/src/KWSys
    )
  
  # write patch in binary dir
  #file(WRITE ${CMAKE_BINARY_DIR}/itk_patch_1.cmake 
  #  "configure_file(${CMAKE_SOURCE_DIR}/patches_ITK/hashtable.hxx.in 
  #   ${ITK_SB_SRC_DIR}/Modules/ThirdParty/KWSys/src/KWSys/hashtable.hxx.in COPYONLY)")
  
  message(STATUS "  Using ITK SuperBuild version")
endif()
