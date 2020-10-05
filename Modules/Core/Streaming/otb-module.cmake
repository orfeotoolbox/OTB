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

set(DOCUMENTATION "This module contains classes related to streaming
functionalities: only the data of a particular requested region are produced. This is
useful when big images must be processed: processing is done piecewise, which prevents
the system from running out of memory.")

otb_module(OTBStreaming
ENABLE_SHARED
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBMetadata
    OTBOSSIMAdapters
    OTBObjectList

  TEST_DEPENDS
    OTBImageIO
    OTBStatistics
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
