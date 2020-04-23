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

set(DOCUMENTATION "This module is related to the handling of point sets. For
instance, it contains the base class 'otb::ImageToPointSetFilter' from which
inherit detectors such as Harris (note that contrary to the 'corner' module, here the
harris detector outputs a point set instead of an image.). With this module, users
will be able to apply a transform to all the points of a PointSet, extract all
the points of a PointSet that are in a given region, generate a random point set,
and so on.")

otb_module(OTBPointSet
  DEPENDS
    OTBStreaming
    OTBInterpolation
    OTBITK
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
