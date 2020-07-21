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

set(OTBIOGDAL_SRC
  otbGDALDatasetWrapper.cxx
  otbGDALDriverManagerWrapper.cxx
  otbGDALImageIO.cxx
  otbGDALImageIOFactory.cxx
  otbGDALOverviewsBuilder.cxx
  otbOGRIOHelper.cxx
  otbOGRVectorDataIO.cxx
  otbOGRVectorDataIOFactory.cxx
  otbDEMHandler.cxx
  )

add_library(OTBIOGDAL ${OTBIOGDAL_SRC})
target_link_libraries(OTBIOGDAL 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
   ${Boost_LIBRARIES}
  )

otb_module_target(OTBIOGDAL)
