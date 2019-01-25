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
#  Example on the use of application connections
#

def test(otb, argv):
  #---------------------------------------------------------------------------
  # First run with in-memory connections by default
  app1 = otb.Registry.CreateApplication("Smoothing")
  app2 = otb.Registry.CreateApplication("Smoothing")
  app3 = otb.Registry.CreateApplication("Smoothing")
  app4 = otb.Registry.CreateApplication("ConcatenateImages")

  app1.IN = argv[1]
  app1.TYPE = "mean"

  app2.ConnectImage("in",app1, "out")
  app2.TYPE = "anidif"

  app3.ConnectImage("in",app1, "out")
  app3.TYPE = "gaussian"

  app4.ConnectImage("il", app2, "out")
  app4.ConnectImage("il", app3, "out")
  app4.AddParameterStringList("il",argv[1])

  app4.OUT = argv[2]
  app4.ExecuteAndWriteOutput()

  #---------------------------------------------------------------------------
  # Second run with on-disk connections
  app1.OUT = argv[2]+"_tmp1.tif"
  app3.OUT = argv[2]+"_tmp3.tif"

  # app2.OUT is left empty and should fallback to memory connection

  app4.PropagateConnectMode(False)
  app4.ExecuteAndWriteOutput()
