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
    #ITKNeuralNetworks #5.0
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
mark_as_advanced(ITK_DIR)

# set ( ITK_NO_IO_FACTORY_REGISTER_MANAGER TRUE )
# include (${ITK_USE_FILE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ITK_REQUIRED_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ITK_REQUIRED_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")
