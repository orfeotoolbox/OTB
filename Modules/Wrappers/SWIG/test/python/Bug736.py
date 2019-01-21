# -*- coding: utf-8 -*-
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

def test(otbApplication, argv):
	RadiometricIndices = otbApplication.Registry.CreateApplication('RadiometricIndices')
	RadiometricIndices.SetParameterString("in", argv[1])
	RadiometricIndices.SetParameterString("out", argv[2])
	RadiometricIndices.SetParameterInt("channels.red", 3)
	RadiometricIndices.SetParameterInt("channels.green", 2)
	RadiometricIndices.SetParameterInt("channels.nir", 4)

	RadiometricIndices.SetParameterStringList("list",	["Vegetation:NDVI","Vegetation:RVI","Vegetation:IPVI"])

	print( RadiometricIndices.GetParameterStringList("list") )

	print( RadiometricIndices.GetParameterValue("list") )

	RadiometricIndices.ExecuteAndWriteOutput()
