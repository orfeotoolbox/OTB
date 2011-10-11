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
import processing.parameters as parameters
import otbApplication as otb

class OTBPlugin():
    def __init__(self, iface):
        self._modules = None
        self._iface = iface
        
    def initGui(self):
        processing.framework.registerModuleProvider(self)
        
    def unload(self):
        pass # TODO : unload the modules
    
    def modules(self):
        if self._modules is None:
            apps = otb.Registry.GetAvailableApplications()
            self._modules = set()
            for app in apps:
                self._modules.add(OTBModule(app, self._iface))
        return self._modules

class OTBModule(processing.Module):
    def __init__(self, modulename, iface):
        self._app = otb.Registry.CreateApplication(modulename)
        self._iface = iface
        processing.Module.__init__(self,
            self._app.GetName(),
            self._app.GetDescription())

        self._blockSignals = False
        self._parameters = []
        self._instance = OTBModuleInstance(self)
        
        self.layerRegistry = QgsMapLayerRegistry.instance()
        
        for p in self._app.GetParametersKeys():
            self.addParameter(p)

    def instance(self):
        return self._instance

    def addParameter(self, otbParamKey):
        otbToQGisParam = {
            otb.ParameterType_Empty:               parameters.BooleanParameter,
            otb.ParameterType_Int:                 parameters.NumericParameter,
            otb.ParameterType_Float:               parameters.NumericParameter,
            otb.ParameterType_String:              parameters.StringParameter,
            otb.ParameterType_Filename:            parameters.PathParameter,
            otb.ParameterType_Directory:           parameters.PathParameter,
            otb.ParameterType_Choice:              parameters.ChoiceParameter,
            otb.ParameterType_InputImage:          parameters.RasterLayerParameter,
            otb.ParameterType_InputComplexImage:   parameters.RasterLayerParameter,
            otb.ParameterType_InputVectorData:     parameters.VectorLayerParameter,
            otb.ParameterType_OutputImage:         parameters.PathParameter,
            otb.ParameterType_OutputVectorData:    parameters.PathParameter,
            otb.ParameterType_Radius:              parameters.NumericParameter
        }
        
        name = otbParamKey
        descr = self._app.GetParameterName(otbParamKey)
        typ = self._app.GetParameterType(otbParamKey)
        role = parameters.Parameter.Role.input # TODO
        mandatory = self._app.IsMandatory(otbParamKey)

        try:
            qgisParamClass = otbToQGisParam[typ]

            if not qgisParamClass: # We are ignoring certain types of parameters
                print "Ignoring type " + str(typ)
                return

            qgisParam = qgisParamClass(name, descr, role=role)
            
            qgisParam._app = self._app
            qgisParam._key = otbParamKey

            try:
              if typ == otb.ParameterType_Empty:
                qgisParam.setDefaultValue(self._app.IsParameterEnabled(otbParamKey))
              elif typ == otb.ParameterType_Int:
                qgisParam.setDefaultValue(self._app.GetParameterInt(otbParamKey))
              elif typ == otb.ParameterType_Float:
                qgisParam.setDefaultValue(self._app.GetParameterFloat(otbParamKey))
              elif typ == otb.ParameterType_String:
                qgisParam.setDefaultValue(self._app.GetParameterString(otbParamKey))
              elif typ == otb.ParameterType_Filename:
                qgisParam.setDefaultValue(self._app.GetParameterString(otbParamKey))
              elif typ == otb.ParameterType_Directory:
                qgisParam.setDefaultValue(self._app.GetParameterString(otbParamKey))
              elif typ == otb.ParameterType_Choice:
                qgisParam.setChoices(self._app.GetChoiceNames(otbParamKey))
                qgisParam.setDefaultValue(self._app.GetParameterInt(otbParamKey))
              elif typ == otb.ParameterType_InputImage:
                qgisParam.setDefaultValue(self._app.GetParameterString(otbParamKey))
              elif typ == otb.ParameterType_InputComplexImage:
                qgisParam.setDefaultValue(self._app.GetParameterString(otbParamKey))
              elif typ == otb.ParameterType_InputVectorData:
                qgisParam.setDefaultValue(self._app.GetParameterString(otbParamKey))
              elif typ == otb.ParameterType_OutputImage:
                qgisParam.setDefaultValue(self._app.GetParameterString(otbParamKey))
              elif typ == otb.ParameterType_OutputVectorData:
                qgisParam.setDefaultValue(self._app.GetParameterString(otbParamKey))
              elif typ == otb.ParameterType_Radius:
                qgisParam.setDefaultValue(self._app.GetParameterInt(otbParamKey))
            except:
              pass
            # TODO validator            

            qgisParam.setMandatory(mandatory)
            self._parameters.append(qgisParam)

            # register callback to instance for parameter
            QObject.connect(self._instance,
                            self._instance.valueChangedSignal(qgisParam),
                            lambda x: self.onParameterChanged(qgisParam, x))

        except KeyError:
            print otbParamKey + " is of unhandled parameter type."
            pass


    def parameters(self):
        return self._parameters

    def tags(self):
        return processing.Module.tags(self) | set([processing.Tag('otb')])
    
    def onParameterChanged(self, qgisParam, value):
        if self._blockSignals == True:
            print "signals blocked"
            return

        self._blockSignals = True

        pc = qgisParam.__class__
        app = qgisParam._app
        key = qgisParam._key
        ptype = app.GetParameterType(key)
        print "onParameterChanged " + key + " " + str(value)

        if ptype == otb.ParameterType_Empty:
            if value :
              print "Enable " + key
              app.EnableParameter(key)
            else :
              print "Disable " + key
              app.DisableParameter(key)

        elif ptype == otb.ParameterType_Int or ptype == otb.ParameterType_Radius:
            print "SetParameterInt " + key
            app.SetParameterInt(key, int(value))

        elif ptype == otb.ParameterType_Float:
            print "SetParameterFloat " + key
            app.SetParameterFloat(key, float(value))

        elif ptype == otb.ParameterType_Choice:
            print "SetParameterInt " + key
            app.SetParameterInt(key, int(value))

        elif ptype == otb.ParameterType_String \
             or ptype == otb.ParameterType_Filename \
             or ptype == otb.ParameterType_Directory:
            print "SetParameterString " + key
            app.SetParameterString(key, str(value))

        elif ptype == otb.ParameterType_InputImage \
             or ptype == otb.ParameterType_InputComplexImage:
            if value is None: return
            qgisParam.layer = value
            dpUri = str(qgisParam.layer.dataProvider().dataSourceUri())
            dpDescription = qgisParam.layer.dataProvider().description()              
            isLocal = dpDescription.startsWith('GDAL provider')
            if isLocal:
              print "Setting " + dpUri + " to param RasterLayerParameter " + key
              app.SetParameterString(key, dpUri)

        elif ptype == otb.ParameterType_InputVectorData:
            if value is None: return
            qgisParam.layer = value
            dpUri = str(qgisParam.layer.dataProvider().dataSourceUri())
            dpDescription = qgisParam.layer.dataProvider().description()              
            isLocal = dpDescription.startsWith('OGR data provider')
            if isLocal:
              print "Setting " + dpUri + " to param VectorLayerParameter " + key
              app.SetParameterString(key, dpUri)

        elif ptype == otb.ParameterType_OutputImage:
            print "SetParameterString " + key
            app.SetParameterString(key, str(value))

        elif ptype == otb.ParameterType_OutputVectorData:
            print "SetParameterString " + key
            app.SetParameterString(key, str(value))

        elif ptype == otb.ParameterType_StringList:
            print "StringList not supported yet"

        elif ptype == otb.ParameterType_InputImageList:
            print "InputImageList not supported yet"

        elif ptype == otb.ParameterType_ListView:
            print "ListView not supported yet"

        # trigger global update
        app.UpdateParameters()
        self.notifyInternalUpdate()
        self._blockSignals = False


    def notifyInternalUpdate(self):
        """ To update the GUI from application instance content 
        """
        for param in self._parameters:
            key = param.name()
            typ = self._app.GetParameterType(key)
            value = None

            try:
              if typ == otb.ParameterType_Empty:
                value = self._app.IsParameterEnabled(key)
              elif typ == otb.ParameterType_Int:
                value = float(self._app.GetParameterInt(key))
              elif typ == otb.ParameterType_Float:
                value = float(self._app.GetParameterFloat(key))
              elif typ == otb.ParameterType_String:
                value = self._app.GetParameterString(key)
              elif typ == otb.ParameterType_Filename:
                value = self._app.GetParameterString(key)
              elif typ == otb.ParameterType_Directory:
                value = self._app.GetParameterString(key)
              elif typ == otb.ParameterType_Choice:
                value = self._app.GetParameterInt(key)
              elif typ == otb.ParameterType_InputImage:
                value = self._app.GetParameterString(key)
              elif typ == otb.ParameterType_InputComplexImage:
                value = self._app.GetParameterString(key)
              elif typ == otb.ParameterType_InputVectorData:
                value = self._app.GetParameterString(key)
              elif typ == otb.ParameterType_OutputImage:
                value = self._app.GetParameterString(key)
              elif typ == otb.ParameterType_OutputVectorData:
                value = self._app.GetParameterString(key)
              elif typ == otb.ParameterType_Radius:
                value = self._app.GetParameterInt(key)
            except:
              pass

            signal = self.instance().valueChangedSignal(param)

            if typ != otb.ParameterType_InputImage \
               and  typ != otb.ParameterType_InputComplexImage \
               and typ != otb.ParameterType_InputVectorData \
               and value is not None:
                QObject.emit( self.instance(), signal, value )


class OTBModuleInstance(processing.ModuleInstance):
    def __init__(self, module):
        print "OTBModuleInstance  " + module.name()
        processing.ModuleInstance.__init__(self, module)
        QObject.connect(
            self, self.valueChangedSignal(self.stateParameter),
            self.stateParameterValueChanged)

    def stateParameterValueChanged(self, state):
        """ Only reacts to start running state, ignore others.
        """
        print "stateParameterValueChanged " + str(state)
        if state != parameters.StateParameter.State.running:
            return
        modName = self.module().name()
 
        print "Executing " + modName
        app = self.module()._app

        if not app.IsApplicationReady():
          self.setFeedback("Missing mandatory parameters")
          self.setState(parameters.StateParameter.State.stopped)
          return

        command = otb.itkPyCommand.New()
        command.SetCommandCallable(self.newProgressSource)
        app.AddObserver( otb.AddProcessToWatchEvent(), command.GetPointer() )
        #try:
        app.ExecuteAndWriteOutput()

        # umm- what if there is no iface?
        iface = self.module()._iface

        # now import output layers
        for key in app.GetParametersKeys():
            if app.GetParameterType(key) == otb.ParameterType_OutputImage:
                iface.addRasterLayer(app.GetParameterString(key))
            elif app.GetParameterType(key) == otb.ParameterType_OutputVectorData:
                basename = os.path.splitext( os.path.basename(app.GetParameterString(key)) )[0]
                iface.addVectorLayer(app.GetParameterString(key), basename, "ogr")

        self.setFeedback("OTB Module execution suceeded.")
        #except:
        #  self.setFeedback("OTB Module execution failed.")

        self.setState(parameters.StateParameter.State.stopped)

    def newProgressSource(self):
        # Setup the feedback with a null progress
        self.setFeedback(self.module()._app.GetProgressDescription(), 0)
        command = otb.itkPyCommand.New()
        command.SetCommandCallable(self.newProgressCallback)
        self.module()._app.GetProgressSource().AddObserver( otb.itkProgressEvent(), command.GetPointer() )

    def newProgressCallback(self):
        # Setup the feedback and the associated progress
        self.setFeedback(self.module()._app.GetProgressDescription(), False, int(100 * self.module()._app.GetProgressSource().GetProgress()))



