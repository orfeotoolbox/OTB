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
#  Example on the use of the Smoothing
#

def test(otb, argv):
	print( "Available applications : " + str(otb.Registry.GetAvailableApplications()) )

	app = otb.Registry.CreateApplication("Smoothing")
	print( app.GetParametersKeys() )

	app.SetParameterString("in", argv[1])

	for type_ in ['mean', 'gaussian', 'anidif']:
		print( 'Testing ' + type_ )
		app.SetParameterString("type", type_)
		app.SetParameterString("out", argv[2] + type_ + ".tif")
		app.ExecuteAndWriteOutput()

		print( dir(app) )
