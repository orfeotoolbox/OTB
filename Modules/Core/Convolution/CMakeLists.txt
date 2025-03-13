#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

project(OTBConvolution)

option(OTB_USE_FFTW "Download and compile FFTW third party (license change to GPLv2)" OFF)
if (OTB_USE_FFTW)
  message(NOTICE "/!\\ /!\\ /!\\ You are actually compiling OTB with FFTW. As FFTW is distributed under GNU GPLv2, OTB is now provided with GNU GPLv2 license /!\\ /!\\ /!\\ ")
else()
  message(STATUS "Compiling with FFTW disabled, otbOverlapSaveConvolutionImageFilter will not be available")
endif()

otb_module_impl()
