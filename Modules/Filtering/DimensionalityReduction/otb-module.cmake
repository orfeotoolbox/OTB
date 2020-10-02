#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

set(DOCUMENTATION "Dimension reduction is a statistical process, which
concentrates the amount of information in multivariate data into a fewer number of
variables (or dimensions). This module implements methods such as Principal Component
Analysis (PCA), Noise-Adjusted Principal Components Analysis (NAPCA), Maximum
Noise Fraction (MNF), Fast Independent Component Analysis (FICA), Maximum
Autocorrelation Factor (MAF), and so on.")

otb_module(OTBDimensionalityReduction
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBObjectList
    OTBStatistics

  TEST_DEPENDS
    OTBImageIO
    OTBTestKernel
    OTBWavelet

  DESCRIPTION
    "${DOCUMENTATION}"
)
