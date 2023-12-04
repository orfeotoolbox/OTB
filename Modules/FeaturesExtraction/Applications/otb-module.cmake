#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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


set(DOCUMENTATION "OTB Image Processing Applications")

otb_module(OTBAppFeaturesExtraction
  DEPENDS
    OTBApplicationEngine
    OTBCarto
    OTBCommon
    OTBITK
    OTBImageBase
    OTBProjection
    OTBVectorDataBase
    OTBVectorDataManipulation
    OTBImageManipulation
    OTBConversion
    OTBBoostAdapters
    OTBColorMap
    OTBCurlAdapters
    OTBGdalAdapters
    OTBObjectList
    OTBStatistics
    OTBStreaming
    OTBTransform
    OTBFunctor
    OTBIOXML
    OTBContrast
    OTBConvolution
    OTBSmoothing
    OTBMathParser
    OTBMathParserX
    OTBEdge
    OTBIndices
    OTBTextures
    OTBChangeDetection
    OTBDescriptors
    OTBDimensionalityReduction
    OTBGDAL
    
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"

  COMPONENT
    FeaturesExtraction
)