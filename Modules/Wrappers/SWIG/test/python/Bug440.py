# -*- coding: utf-8 -*-
#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

# Import the otb applications package

import sys
def test(otbApplication, argv):
	# The following line creates an instance of the Convert application
	Convert = otbApplication.Registry.CreateApplication("Convert")

	# The following lines set all the application parameters:
	Convert.SetParameterString("in", argv[1])

	Convert.SetParameterString("out", argv[2])

	Convert.SetParameterString("type","linear")

	# The following line execute the application
	sys.exit(Convert.ExecuteAndWriteOutput())
