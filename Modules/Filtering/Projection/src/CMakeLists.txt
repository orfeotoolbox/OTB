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

set(OTBProjection_SRC
  otbGeometriesProjectionFilter.cxx
  otbPleiadesPToXSAffineTransformCalculator.cxx
  )

add_library(OTBProjection ${OTBProjection_SRC})
target_link_libraries(OTBProjection 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBMetadata_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBProjection)
