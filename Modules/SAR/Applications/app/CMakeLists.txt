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

otb_create_application(
  NAME           SARCalibration
  SOURCES        otbSARCalibration.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SARDeburst
  SOURCES        otbSARDeburst.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SARBurstExtraction
  SOURCES        otbSARBurstExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SARConcatenateBursts
  SOURCES        otbSARConcatenateBursts.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  otb_create_application(
  NAME           SARDecompositions
  SOURCES        otbSARDecompositions.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  otb_create_application(
  NAME           SARPolarMatrixConvert
  SOURCES        otbSARPolarMatrixConvert.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  otb_create_application(
  NAME           SARPolarSynth
  SOURCES        otbSARPolarSynth.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  otb_create_application(
  NAME           ComputeModulusAndPhase
  SOURCES        otbComputeModulusAndPhase.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Despeckle
  SOURCES        otbDespeckle.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})