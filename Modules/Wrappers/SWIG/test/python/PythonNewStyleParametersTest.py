# -*- coding: utf-8 -*-
#
# Copyright (C) 2005-2019 CS Systemes d'Information (CS SI)
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

#  Example on the use of otb "pythonization"
#
def cm_assert(a,b):
	print("debug print before assert check: '%s'== '%s'" %(a, b))
	assert a == b

def test(otb, argv):
	app = otb.Registry.CreateApplication('OrthoRectification')

	# test GetParameterTypeAsString() method in python.
	print( app.GetParameterTypeAsString(otb.ParameterType_InputImage) )
	print( app.GetParameterTypeAsString(otb.ParameterType_String) )

	# one test for each parameter type (string, float, int, ...)

	# # parameter group io.in
	# 1 - input image parameter set
	app.IO.IN = argv[1]
	cm_assert(app.GetParameterString('io.in'), argv[1])

	# 2 - input image parameter get
	cm_assert(app.IO.IN, argv[1])

	# # parameter group io.in
	# 3 - output image parameter set
	app.IO.OUT = argv[2]
	cm_assert(app.GetParameterString('io.out'), argv[2])

	# 4 - output image parameter get
	cm_assert(app.IO.OUT, argv[2])

	# 5 - choice with sub parameters set
	app.MAP = 'lambert2'
	cm_assert(app.GetParameterString('map'), 'lambert2')

	# 5.1 - choice with sub parameters get
	app.SetParameterString('map', 'wgs')
	cm_assert('wgs', app.MAP)

	# 5.2 - choice with sub parameters set
	app.MAP = 'lambert93'
	cm_assert('lambert93', app.GetParameterString('map'))

	# 5.3 - choice with sub parameters set
	app.SetParameterString('map', 'epsg')
	cm_assert(app.MAP, 'epsg')

	# 6 - int type 2nd level sub parameters of choice parameter set
	app.MAP.EPSG.CODE = 2154
	cm_assert(2154, app.GetParameterInt('map.epsg.code'))

	# 7 - another choice with sub parameters set
	app.MAP = 'utm'
	cm_assert('utm', app.GetParameterString('map'))

	# 8 - int type sub parameters of choice parameter set
	app.MAP.UTM.ZONE = 47
	cm_assert(47,app.GetParameterInt('map.utm.zone'))

	# 9 - int type sub parameters of choice parameter get
	app.SetParameterInt('map.utm.zone', 31)
	cm_assert(app.MAP.UTM.ZONE, 31)

	# 10 - bool type sub parameters of choice parameter get
	app.SetParameterInt('map.utm.northhem',0)
	cm_assert(app.MAP.UTM.NORTHHEM, False)

	# 11 - bool type sub parameters of choice parameter set
	app.MAP.UTM.NORTHHEM = True
	cm_assert(True, app.GetParameterInt('map.utm.northhem') )

	#12 - simple choice parameter set
	app.OUTPUTS.MODE = 'auto'
	cm_assert('auto', app.GetParameterString('outputs.mode'))

	#13 - simple choice parameter get
	app.SetParameterString('outputs.mode', 'orthofit')
	cm_assert(app.OUTPUTS.MODE, 'orthofit')

	#16 - parameter float get
	app.SetParameterFloat('elev.default', 5.0)
	cm_assert(5.0, app.ELEV.DEFAULT)

	#17 -parameter float set
	app.ELEV.DEFAULT = -2.5
	cm_assert(app.GetParameterFloat('elev.default'), -2.5)

	#18 - parameter ram get
	app.SetParameterString('opt.ram', '256')
	cm_assert(256, app.OPT.RAM)

	#19 - parameter ram set
	app.OPT.RAM = '512'
	cm_assert(app.GetParameterInt('opt.ram'), 512)

	#20 - parameter bool set
	app.OUTPUTS.ISOTROPIC = True
	cm_assert(app.IsParameterEnabled('outputs.isotropic'), True)

	#21 - parameter bool get
	app.SetParameterInt('outputs.isotropic',0)
	cm_assert(False, app.OUTPUTS.ISOTROPIC)

	#Do not execute. we need LARGEINPUT. so we tried a small application
	#app.Execute()

	app = None

	app = otb.Registry.CreateApplication('Smoothing')
	app.IN = argv[1]
	app.TYPE='anidif'
	app.OUT = argv[2]
	app.ExecuteAndWriteOutput()
