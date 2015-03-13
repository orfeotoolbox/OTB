set(proj ITK)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup ITK...")

if(USE_SYSTEM_ITK)
  find_package ( ITK )
  add_custom_target(${proj})
  message(STATUS "  Using ITK system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using ITK SuperBuild version")
  
  # if(MSVC)
    # set(ITK_SB_SRC "C:/Temp/ITK")
    # set(ITK_SB_BUILD_DIR ${ITK_SB_SRC}/build)
  # endif()
  
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
  
  # declare dependencies
  set(${proj}_DEPENDENCIES TIFF EXPAT PNG ZLIB FFTW)
  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj ITK)
  
  ADD_SUPERBUILD_CMAKE_VAR(TIFF_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(TIFF_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(EXPAT_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(EXPAT_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(PNG_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(PNG_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(ZLIB_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(ZLIB_LIBRARY)
  
  # By default activate FFTW, but with an external fftw build
  # These variables are used in ITK to initialize the value of the ITK_USE_FFTW_XXX options
  if (WIN32)
    list(APPEND ITK_SB_CONFIG
      -DUSE_FFTWF:BOOL=OFF
      -DUSE_FFTWD:BOOL=OFF
      )
  else()
    list(APPEND ITK_SB_CONFIG 
      -DUSE_FFTWF:BOOL=ON
      -DUSE_FFTWD:BOOL=ON
      -DUSE_SYSTEM_FFTW:BOOL=ON
      )
    ADD_SUPERBUILD_CMAKE_VAR(FFTW_INCLUDE_PATH)
  endif()
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/itk/files/itk/4.6/InsightToolkit-4.6.1.tar.gz/download"
    URL_MD5 2c84eae50ab2452cdad32aaadced3c37
    SOURCE_DIR ${ITK_SB_SRC}
    BINARY_DIR ${ITK_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DITK_BUILD_DEFAULT_MODULES:BOOL=OFF
      ${ITK_MODULES_CMAKE_CACHE}
      -DITKGroup_Core:BOOL=OFF
      -DBUILD_SHARED_LIBS:BOOL=${SB_BUILD_SHARED_LIBS}
      -DBUILD_TESTING:BOOL=OFF
      -DBUILD_EXAMPLES:BOOL=OFF
      -DCMAKE_PREFIX_PATH:STRING=${SB_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
      -DITK_USE_SYSTEM_EXPAT:BOOL=ON
      -DITK_USE_SYSTEM_ZLIB:BOOL=ON
      -DITK_USE_SYSTEM_TIFF:BOOL=ON
      -DITK_USE_SYSTEM_PNG:BOOL=ON
      ${ITK_SB_CONFIG}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
      ${CMAKE_SOURCE_DIR}/patches/ITK/hashtable.hxx.in
      ${ITK_SB_SRC}/Modules/ThirdParty/KWSys/src/KWSys
    DEPENDS ${${proj}_DEPENDENCIES}
    )
  
  # write patch in binary dir
  #file(WRITE ${CMAKE_BINARY_DIR}/itk_patch_1.cmake 
  #  "configure_file(${CMAKE_SOURCE_DIR}/patches_ITK/hashtable.hxx.in 
  #   ${ITK_SB_SRC}/Modules/ThirdParty/KWSys/src/KWSys/hashtable.hxx.in COPYONLY)")
  
  set(_SB_ITK_DIR ${SB_INSTALL_PREFIX}/lib/cmake/ITK-4.6)
  
endif()
endif()
