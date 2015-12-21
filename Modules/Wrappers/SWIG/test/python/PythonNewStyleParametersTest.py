# -*- coding: utf-8 -*-
# Authors: Rashad Kanavath <rashad.kanavath@c-s.fr>
#          Julien Malik <julien.malik@c-s.fr>
#
# Copyright: (c) CS Systemes d'information. All rights reserved
#
#  Example on the use of otb "pythonization"
#
from sys import argv
import otbApplication as otb

def cm_assert(a,b):
	print "debug print before assert check: '%s'== '%s'" %(a, b)
	assert a == b

app = otb.Registry.CreateApplication('OrthoRectification')

#test GetParameterTypeAsString() method in python.
print app.GetParameterTypeAsString(otb.ParameterType_InputImage)
print app.GetParameterTypeAsString(otb.ParameterType_String)
print app.GetParameterTypeAsString(otb.ParameterType_Empty)

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
app.MAP.EPSG.CODE = 32768
cm_assert(32768, app.GetParameterInt('map.epsg.code'))

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
app.DisableParameter('map.utm.northhem')
cm_assert(app.MAP.UTM.NORTHHEM, False)

# 11 - bool type sub parameters of choice parameter set
app.MAP.UTM.NORTHHEM = True
cm_assert(True, app.IsParameterEnabled('map.utm.northhem') )

#12 - simple choice parameter set
app.OUTPUTS.MODE = 'auto'
cm_assert('auto', app.GetParameterString('outputs.mode'))

#13 - simple choice parameter get
app.SetParameterString('outputs.mode', 'orthofit')
cm_assert(app.OUTPUTS.MODE, 'orthofit')

#14 - inputxml parameter set
app.INXML = argv[3]
cm_assert(app.GetParameterString('inxml'), argv[3])

#15 - outputxml parameter get
app.SetParameterString('outxml', 'output.xml')
cm_assert("output.xml", app.OUTXML)

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
app.DisableParameter('outputs.isotropic')
cm_assert(False, app.OUTPUTS.ISOTROPIC)

#Do not execute. we need LARGEINPUT. so we tried a small application
#app.Execute()

app = None

app = otb.Registry.CreateApplication('Smoothing')
app.IN = argv[1]
app.TYPE='anidif'
app.OUT = argv[2]
app.ExecuteAndWriteOutput()
