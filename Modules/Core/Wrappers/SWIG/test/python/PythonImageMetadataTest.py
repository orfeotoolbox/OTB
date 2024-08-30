# -*- coding: utf-8 -*-
#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

def test(otb, argv):
    imd = otb.ImageMetadataBase()

    # Setters
    imd["OrbitNumber"] = 2.0
    imd["SensorID"] = "PHR"
    imd["ProjectionWKT"] = "UTM projRef"
    imd["Comment"] = "Test extra keys"

    # is_xxx
    assert imd.is_num("OrbitNumber")
    assert not imd.is_num("SensorID")

    assert imd.is_str("SensorID")
    assert not imd.is_str("OrbitNumber")

    assert imd.is_l1d("SpectralSensitivity")
    assert not imd.is_l1d("SensorID")

    assert imd.is_time("ProductionDate")
    assert not imd.is_time("SensorID")

    assert imd.is_geom("ProjectionWKT")
    assert not imd.is_geom("SensorID")

    assert imd.is_extra("Comment")
    assert not imd.is_extra("SensorID")

    # get_xxx
    assert imd.get_num("OrbitNumber") == 2.0
    assert imd.get_str("SensorID") == "PHR"
    assert imd.get_extra("Comment") == "Test extra keys"

    # Getters
    assert imd["OrbitNumber"] == 2.0
    assert imd["SensorID"] == "PHR"
    # assert imd["ProjectionWKT"] == "UTM projRef" # This getter is not implemented yet
    assert imd["Comment"] == "Test extra keys"

    # has
    assert imd.has("OrbitNumber")
    assert imd.has("SensorID")
    assert imd.has("Comment")
    assert not imd.has("Comment2")
    assert not imd.has("RadiometricLevel")

    # GetProjectedGeometry
    assert imd.GetProjectedGeometry() == "UTM projRef"

    # GetProjectionProj
    imd["ProjectionProj"] = "Proj String"
    assert imd.GetProjectionProj() == "Proj String"
