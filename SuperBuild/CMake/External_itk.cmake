INCLUDE_ONCE_MACRO(ITK)

SETUP_SUPERBUILD(ITK)

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
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(ITK ZLIB FFTW)
ADD_SUPERBUILD_CMAKE_VAR(ITK ZLIB_INCLUDE_DIR)
ADD_SUPERBUILD_CMAKE_VAR(ITK ZLIB_LIBRARY)

# By default activate FFTW, but with an external fftw build
# These variables are used in ITK to initialize the value of the ITK_USE_FFTW_XXX options
if (WIN32)
  list(APPEND ITK_SB_CONFIG
    -DUSE_FFTWF:BOOL=OFF
    -DUSE_FFTWD:BOOL=OFF
    )
  if(MSVC AND BUILD_SHARED_LIBS)
    list(APPEND ITK_SB_CONFIG
      -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=TRUE)
  endif()
else()
  list(APPEND ITK_SB_CONFIG
    -DUSE_FFTWF:BOOL=ON
    -DUSE_FFTWD:BOOL=ON
    -DUSE_SYSTEM_FFTW:BOOL=ON
    )
  ADD_SUPERBUILD_CMAKE_VAR(ITK FFTW_INCLUDE_PATH)
endif()

# forward compilation flags
set(ITK_SB_COMPILATION_FLAGS
  -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
  -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
  -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
  -DCMAKE_MODULE_LINKER_FLAGS:STRING=${CMAKE_MODULE_LINKER_FLAGS}
  -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
  -DCMAKE_STATIC_LINKER_FLAGS:STRING=${CMAKE_STATIC_LINKER_FLAGS}
  )

#later used below for _SB_ITK_DIR and then in packaging
set(ITK_SB_VERSION 4.10)
set(ITK_SB_VERSION_FULL ${ITK_SB_VERSION}.0)
set(_SB_ITK_DIR ${SB_INSTALL_PREFIX}/lib/cmake/ITK-${ITK_SB_VERSION})

ExternalProject_Add(ITK
  PREFIX ITK
  URL "http://downloads.sourceforge.net/project/itk/itk/4.10/InsightToolkit-4.10.0.tar.gz"
  URL_MD5 8c67ba296da3835fb67bb29d98dcff3e
  SOURCE_DIR ${ITK_SB_SRC}
  BINARY_DIR ${ITK_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  ${ITK_SB_COMPILATION_FLAGS}
  -DITK_BUILD_DEFAULT_MODULES:BOOL=OFF
  ${ITK_MODULES_CMAKE_CACHE}
  -DITKGroup_Core:BOOL=OFF
  -DBUILD_TESTING:BOOL=OFF
  -DBUILD_EXAMPLES:BOOL=OFF
  -DITK_USE_SYSTEM_EXPAT:BOOL=ON
  -DITK_USE_SYSTEM_ZLIB:BOOL=ON
  -DITK_USE_SYSTEM_TIFF:BOOL=ON
  -DITK_USE_SYSTEM_PNG:BOOL=ON
  ${ITK_SB_CONFIG}
  DEPENDS ${ITK_DEPENDENCIES}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  )

SUPERBUILD_PATCH_SOURCE(ITK)
