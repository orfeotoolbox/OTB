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

otb_create_application(
  NAME           Segmentation
  SOURCES        otbSegmentation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ConnectedComponentSegmentation
  SOURCES        otbConnectedComponentSegmentation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           LSMSSegmentation
  SOURCES        otbLSMSSegmentation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           LSMSVectorization
  SOURCES        otbLSMSVectorization.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           LSMSSmallRegionsMerging
  SOURCES        otbLSMSSmallRegionsMerging.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           HooverCompareSegmentation
  SOURCES        otbHooverCompareSegmentation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           MeanShiftSmoothing
  SOURCES        otbMeanShiftSmoothing.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           LargeScaleMeanShift
  SOURCES        otbLargeScaleMeanShift.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SmallRegionsMerging
  SOURCES        otbSmallRegionsMerging.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
