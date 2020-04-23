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

set(DOCUMENTATION "Extended filenames provide a way to influence how data are
read and written by Orfeo ToolBox. They can be used to specify an external geom
file for sensor modeling, tell  that a TILED GeoTiff is to be written, read a
different subdataset or resolution (for multi-resolution data), or manage ortho-ready
product by skipping either geographic or sensor-model information.")

otb_module(OTBExtendedFilename
  DEPENDS
    OTBCommon
    OTBIOGDAL
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
