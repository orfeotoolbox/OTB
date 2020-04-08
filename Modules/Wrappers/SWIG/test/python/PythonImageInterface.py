#!/usr/bin/env python3
#-*- coding: utf-8 -*-
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

def test(otb, argv):
  # Create a smoothing application
  app = otb.Registry.CreateApplication("Smoothing")
  app.SetParameterString("in",argv[1])
  app.Execute()

  # Setup a special requested region
  myRegion = otb.itkRegion()
  myRegion.GetSize()[0] = 20
  myRegion.GetSize()[1] = 25
  myRegion.GetIndex().Fill(10)
  app.PropagateRequestedRegion("out",myRegion)
  print(app.GetImageRequestedRegion("in"))
  
  # Create a ReadImageInfo application and plug the output of app
  app2 = otb.Registry.CreateApplication("ReadImageInfo")
  ex = app.ExportImage("out")
  app2.ImportVectorImage("in", ex)
  app2.Execute()
  someKeys = ['sizex', 'sizey', 'spacingx', 'spacingy', 'sensor', 'projectionref']
  for key in someKeys:
    print(key + ' : ' + str(app2.GetParameterValue(key)) )
