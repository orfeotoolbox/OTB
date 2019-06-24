#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

INCLUDE_ONCE_MACRO(ITK)

SETUP_SUPERBUILD(ITK)

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
  TransformFactory
  IOTransformBase
  IOTransformInsightLegacy
  IOTransformMatlab

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

# These variables are used in ITK to initialize the value of the ITK_USE_FFTW_XXX options
list(APPEND ITK_SB_CONFIG
  -DUSE_FFTWF:BOOL=ON
  -DUSE_FFTWD:BOOL=ON
  -DUSE_SYSTEM_FFTW:BOOL=ON
  )
ADD_SUPERBUILD_CMAKE_VAR(ITK FFTW_INCLUDE_PATH)

if (WIN32)
  list(APPEND ITK_SB_CONFIG
    -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=ON
    )
endif()

#variables are later used in packaging
set(SB_ITK_VERSION_MAJOR "5")
set(SB_ITK_VERSION_MINOR "0")


if(SB_ITK_VERSION_MAJOR STREQUAL "5")
  list(APPEND ITK_SB_CONFIG
    -DITKV4_COMPATIBILITY:BOOL=ON)
endif()

set(_SB_ITK_DIR ${SB_INSTALL_PREFIX}/lib/cmake/ITK-${SB_ITK_VERSION_MAJOR}.${SB_ITK_VERSION_MINOR})

ExternalProject_Add(ITK
  PREFIX ITK
  URL "https://github.com/InsightSoftwareConsortium/ITK/archive/v5.0.0.tar.gz"
  URL_MD5 5bc86a7daf30eb30d78f878f00a29920
  SOURCE_DIR ${ITK_SB_SRC}
  BINARY_DIR ${ITK_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DITK_BUILD_DEFAULT_MODULES:BOOL=OFF
  ${ITK_MODULES_CMAKE_CACHE}
  -DITKGroup_Core:BOOL=OFF
  -DBUILD_TESTING:BOOL=OFF
  -DBUILD_EXAMPLES:BOOL=OFF
  -DITK_USE_SYSTEM_EXPAT:BOOL=ON
  -DITK_USE_SYSTEM_ZLIB:BOOL=ON
  -DITK_USE_SYSTEM_TIFF:BOOL=ON
  -DITK_USE_SYSTEM_PNG:BOOL=ON
  -DITK_FORBID_DOWNLOADS:BOOL=ON
  ${ITK_SB_CONFIG}
  DEPENDS ${ITK_DEPENDENCIES}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(ITK)

