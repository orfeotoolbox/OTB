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

# -*- coding: utf-8 -*-

#
#  Example on the use of parameters dictionaries
#

def test(otb, argv):
	app = otb.Registry.CreateApplication("Smoothing")

	app.SetParameterString("out", "myOutput.tif")
	app.SetParameterInt("type.mean.radius",4)
	prm = app.GetParameters()
	if prm["out"] != "myOutput.tif":
		exit(1)
	if prm["type.mean.radius"] != 4:
		exit(1)
	prm["in"] = argv[1]
	prm["type"] = "anidif"
	prm["type.anidif.nbiter"] = 12
	app.SetParameters(prm)
	if app.GetParameterString("in") != argv[1]:
		exit(1)
	if app.GetParameterInt("type.anidif.nbiter") != 12:
		exit(1)

