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

set(OTBCarto_SRC
  otbCoordinateToName.cxx
  otbOSMDataToVectorDataGenerator.cxx
  otbWorldFile.cxx
  otbPlaceNameToLonLat.cxx
  )

add_library(OTBCarto ${OTBCarto_SRC})
target_link_libraries(OTBCarto 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBCurlAdapters_LIBRARIES}
  ${OTBVectorDataIO_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  )

otb_module_target(OTBCarto)
