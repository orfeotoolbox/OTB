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

set(DOCUMENTATION "Cartography related module. For instance, users can retrieve
geographical information for longitude and latitude coordinates. compute the
extent of the input image and use it to fill the bbox for the OSM request. produce
Map file product (ie a file .map, the tiles to draw in a mapserver, and finally a
shapefile). request OSM Server using the OSM API 0.6 to get the xml file storing
the vector elements available. retrieve Longitude and Latitude coordinates from a
place name. And so on.")

otb_module(OTBCarto
ENABLE_SHARED
  DEPENDS
    OTBCommon
    OTBCurlAdapters
    OTBITK
    OTBImageBase
    OTBImageIO
    OTBImageManipulation
    OTBProjection
    OTBTinyXML
    OTBTransform
    OTBVectorDataBase
    OTBVectorDataIO

  TEST_DEPENDS
    OTBCommandLineParser
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
