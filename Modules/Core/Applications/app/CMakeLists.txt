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
  NAME           OpticalCalibration
  SOURCES        otbOpticalCalibration.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           BundleToPerfectSensor
  SOURCES        otbBundleToPerfectSensor.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Pansharpening
  SOURCES        otbPansharpening.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ExtractROI
  SOURCES        otbExtractROI.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ReadImageInfo
  SOURCES        otbReadImageInfo.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ConcatenateImages
  SOURCES        otbConcatenateImages.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
    NAME           Rasterization
    SOURCES        otbRasterization.cxx
    LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           CompareImages
  SOURCES        otbCompareImages.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ManageNoData
  SOURCES        otbManageNoData.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           DynamicConvert
  SOURCES        otbDynamicConvert.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

OTB_CREATE_APPLICATION(
  NAME           Mosaic
  SOURCES        otbMosaic.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

OTB_CREATE_APPLICATION(
  NAME           ResetMargin
  SOURCES        otbResetMargin.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  
otb_create_application(
  NAME           VectorDataTransform
  SOURCES        otbVectorDataTransform.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           VectorDataExtractROI
  SOURCES        otbVectorDataExtractROI.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           VectorDataSetField
  SOURCES        otbVectorDataSetField.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ConcatenateVectorData
  SOURCES        otbConcatenateVectorData.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ZonalStatistics
  SOURCES        otbZonalStatistics.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           OrthoRectification
  SOURCES        otbOrthoRectification.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           RigidTransformResample
  SOURCES        otbRigidTransformResample.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ImageEnvelope
  SOURCES        otbImageEnvelope.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Superimpose
  SOURCES        otbSuperimpose.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  otb_create_application(
    NAME           VectorDataReprojection
    SOURCES        otbVectorDataReprojection.cxx
    LINK_LIBRARIES ${${otb-module}_LIBRARIES})
      
# TODO refactor this application
#otb_create_application(
#  NAME           RefineSensorModel
#  SOURCES        otbRefineSensorModel.cxx
#  LINK_LIBRARIES ${${otb-module}_LIBRARIES})