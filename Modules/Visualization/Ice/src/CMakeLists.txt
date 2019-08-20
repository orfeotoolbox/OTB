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

set(OTBICE_SRCS
  otbShader.cxx
  otbShaderRegistry.cxx
  otbGeoInterface.cxx
  otbGlActor.cxx
  otbGlImageActor.cxx
  otbGlROIActor.cxx
  otbGlVectorActor.cxx
  otbGlVersionChecker.cxx
  otbGlView.cxx
  otbImageSettings.cxx
  otbStandardShader.cxx
  otbViewSettings.cxx
  otbMinimalShader.cxx
  )

add_library(OTBIce ${OTBICE_SRCS})
target_link_libraries(OTBIce
  ${OTBCommon_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBOpenGL_LIBRARIES}
  ${OTBGlew_LIBRARIES})

otb_module_target(OTBIce)
