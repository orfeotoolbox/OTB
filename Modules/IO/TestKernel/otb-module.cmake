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

set(DOCUMENTATION "This module is concerned with running the tests and
submitting them to the dashboard. For each class, at minimum there is a test which tries
to instantiate it and another one which uses the class. The output of each test
(image, text file, binary file) is controlled against a baseline to make sure that
the result hasn't changed.")

otb_module(OTBTestKernel
  ENABLE_SHARED
  DEPENDS
    OTBGdalAdapters
    OTBImageIO
    OTBITK
    OTBImageBase
    OTBCommon
    OTBGDAL
    OTBMetadata
    OTBImageManipulation

    OPTIONAL_DEPENDS
    OTBMPIConfig

  DESCRIPTION
    "${DOCUMENTATION}"
)
