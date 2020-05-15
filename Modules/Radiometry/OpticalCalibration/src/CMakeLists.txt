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

set(OTBOpticalCalibration_SRC
  otbAtmosphericCorrectionParameters.cxx
  otbAeronetData.cxx
  otbWavelengthSpectralBands.cxx
  otbSpectralSensitivityReader.cxx
  otbAeronetFileReader.cxx
  otbSIXSTraits.cxx
  otbAtmosphericRadiativeTerms.cxx
  otbImageMetadataCorrectionParameters.cxx
  )

add_library(OTBOpticalCalibration ${OTBOpticalCalibration_SRC})
target_link_libraries(OTBOpticalCalibration
  ${OTB6S_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBMetadata_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  )

otb_module_target(OTBOpticalCalibration)
