# -*- coding: utf-8 -*-
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

# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale
#  This test checks that the Python wrapping works correctly. For instance, it 
#  ensures that Init() is called on the Swig/Python interface (see bug #440, 
#  https://bugs.orfeo-toolbox.org//view.php?id=440)
#
def test(otb, argv):
	app = otb.Registry.CreateApplication("Rescale")
	app.SetParameterString("in", argv[1])
	app.SetParameterFloat("outmin", 1)
	app.SetParameterFloat("outmax", 100)
	app.SetParameterString("out", argv[2] + ".tif")
	app.ExecuteAndWriteOutput()
