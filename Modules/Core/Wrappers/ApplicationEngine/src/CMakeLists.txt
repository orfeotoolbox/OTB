
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

set( OTBApplicationEngine_SRC
  otbWrapperInputVectorDataListParameter.cxx
  otbWrapperListViewParameter.cxx
  otbWrapperParameterGroup.cxx
  otbWrapperInputXML.cxx
  otbWrapperOutputXML.cxx
  otbWrapperInputImageListParameter.cxx
  otbWrapperElevationParametersHandler.cxx
  otbWrapperInputFilenameListParameter.cxx
  otbWrapperOutputImageParameter.cxx
  otbWrapperInputImageParameter.cxx
  otbWrapperInputImageParameterUInt8.cxx
  otbWrapperInputImageParameterUInt16.cxx
  otbWrapperInputImageParameterUInt32.cxx
  otbWrapperInputImageParameterInt16.cxx
  otbWrapperInputImageParameterInt32.cxx
  otbWrapperInputImageParameterCInt16.cxx
  otbWrapperInputImageParameterCInt32.cxx
  otbWrapperInputImageParameterCFloat.cxx
  otbWrapperInputImageParameterCDouble.cxx
  otbWrapperInputImageParameterFloat.cxx
  otbWrapperInputImageParameterDouble.cxx
  otbWrapperParameterKey.cxx
  otbWrapperDocExampleStructure.cxx
  otbWrapperInputVectorDataParameter.cxx
  otbWrapperOutputVectorDataParameter.cxx
  otbWrapperMapProjectionParametersHandler.cxx
  otbWrapperApplication.cxx
  otbWrapperChoiceParameter.cxx
  otbWrapperApplicationRegistry.cxx
  otbWrapperApplicationFactoryBase.cxx
  otbWrapperCompositeApplication.cxx
  otbWrapperStringListInterface.cxx
  otbWrapperStringListParameter.cxx
  otbWrapperAbstractParameterList.cxx
  otbWrapperParameterList.cxx
  otbWrapperBoolParameter.cxx
  otbWrapperMetaDataHelper.cxx
  otbWrapperParameter.cxx
  otbWrapperCastImage.cxx
  otbWrapperTypes.cxx
  )

add_library(OTBApplicationEngine ${OTBApplicationEngine_SRC})
target_link_libraries(OTBApplicationEngine
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBVectorDataIO_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  )

otb_module_target(OTBApplicationEngine COMPONENT_Core)
