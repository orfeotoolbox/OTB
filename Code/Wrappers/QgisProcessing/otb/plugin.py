# -*- coding: utf-8 -*-

#	SAGA Modules plugin for Quantum GIS
#
#	plugin.py (C) Camilo Polymeris
#	
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
# 
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#       
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#   MA 02110-1301, USA.

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from qgis.core import *

import os
import processing
import processing.parameters
import otbApplication as otb

class OTBPlugin():
    def __init__(self, iface):
        self._modules = None
        pass
        
    def initGui(self):
        processing.framework.registerModuleProvider(self)
        
    def unload(self):
        pass # TODO : unload the modules
    
    def modules(self):
        if self._modules is None:
            apps = otb.Registry.GetAvailableApplications()
            self._modules = set()
            for app in apps:
                self._modules.add(OTBModule(app))
        return self._modules

class OTBModule(processing.Module):
    def __init__(self, modulename):
        self._app = otb.Registry.CreateApplication(modulename)
        processing.Module.__init__(self,
            self._app.GetName(),
            self._app.GetDescription())
        
        self._parameters = []

        for p in self._app.GetParametersKeys():
            print p
            self.addParameter(p)

    def addParameter(self, otbParamKey):
        otbToQGisParam = {
            otb.ParameterType_Empty: processing.parameters.BooleanParameter,
            otb.ParameterType_Int:   processing.parameters.NumericParameter,
            otb.ParameterType_Float:   processing.parameters.NumericParameter,
            otb.ParameterType_String:   processing.parameters.StringParameter,
            otb.ParameterType_Filename:   processing.parameters.StringParameter,
            otb.ParameterType_Directory:   processing.parameters.StringParameter,
            otb.ParameterType_Choice:   processing.parameters.ChoiceParameter,
            otb.ParameterType_InputImage:   processing.parameters.StringParameter,
            otb.ParameterType_InputComplexImage:   processing.parameters.StringParameter,
            otb.ParameterType_InputVectorData:   processing.parameters.StringParameter,
            otb.ParameterType_OutputImage:   processing.parameters.StringParameter,
            otb.ParameterType_OutputVectorData:   processing.parameters.StringParameter,
            otb.ParameterType_Radius:   processing.parameters.NumericParameter
        }
        
        name = otbParamKey
        descr = self._app.GetParameterName(otbParamKey)
        typ = self._app.GetParameterType(otbParamKey)
        try:
            qgisParam = otbToQGisParam[typ]
            self._parameters.append(qgisParam(name, descr))
        except KeyError:
            #print name + " is of unhandled parameter type."
            pass
    def parameters(self):
        return self._parameters
    def tags(self):
        return processing.Module.tags(self) | set([processing.Tag('otb')])


