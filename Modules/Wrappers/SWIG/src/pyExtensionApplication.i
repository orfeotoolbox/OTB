/*Maybe TODO: change use a dict to  GetParameterTypeAsString */
%extend Application
{

/*The following code (python) will be add to the class application as method */

  %pythoncode
    {

    def __str__(self):
      s  = self.GetDocName()

    def GetParameterTypeAsString(self, parameter_type):
      return {
        ParameterType_InputProcessXML : 'ParameterType_InputProcessXML',
        ParameterType_String : 'ParameterType_String',
        ParameterType_InputFilename : 'ParameterType_InputFilename',
        ParameterType_OutputImage : 'ParameterType_OutputImage',
        ParameterType_OutputVectorData : 'ParameterType_OutputVectorData',
        ParameterType_OutputProcessXML : 'ParameterType_OutputProcessXML',
        ParameterType_OutputFilename : 'ParameterType_OutputFilename',
        ParameterType_Directory : 'ParameterType_Directory',
        ParameterType_InputImage : 'ParameterType_InputImage',
        ParameterType_ComplexInputImage : 'ParameterType_ComplexInputImage',
        ParameterType_InputVectorData : 'ParameterType_InputVectorData',
        ParameterType_InputImageList : 'ParameterType_InputImageList',
        ParameterType_InputVectorDataList : 'ParameterType_InputImageList',
        ParameterType_InputFilenameList : 'ParameterType_InputFilenameList',
        ParameterType_StringList : 'ParameterType_StringList',
        ParameterType_ListView : 'ParameterType_ListView',
        ParameterType_Int : 'ParameterType_Int',
        ParameterType_Radius : 'ParameterType_Radius',
        ParameterType_RAM : 'ParameterType_RAM',
        ParameterType_Float : 'ParameterType_Float',
        ParameterType_Empty : 'ParameterType_Empty',
        ParameterType_Choice : 'ParameterType_Choice',
        ParameterType_Group : 'ParameterType_Group',
      }.get(parameter_type, 'ParameterType_UNKNOWN')

    def __str__(self):
      s  = self.GetDocName()
      s += '\n'
      s += self.GetDocLongDescription()
      return s

    def SetParameterValue(self, paramKey, value):
      paramType = self.GetParameterType(paramKey)
      if paramType in [ParameterType_InputProcessXML, ParameterType_RAM,
                       ParameterType_String, ParameterType_InputFilename,
                       ParameterType_OutputImage, ParameterType_OutputVectorData,
                       ParameterType_OutputProcessXML, ParameterType_OutputFilename,
                       ParameterType_Directory, ParameterType_InputImage,
                       ParameterType_ComplexInputImage, ParameterType_InputVectorData]:
        return self.SetParameterString(paramKey, value)
      elif paramType in [ParameterType_InputImageList, ParameterType_InputVectorDataList,
                         ParameterType_InputFilenameList, ParameterType_StringList,
                         ParameterType_ListView]:
        return self.SetParameterStringList(paramKey, value)
      elif paramType in [ParameterType_Int, ParameterType_Radius]:
        return self.SetParameterInt(paramKey, value)
      elif paramType in [ParameterType_Float]:
        return self.SetParameterFloat(paramKey, value)
      elif paramType in [ParameterType_Empty]:
        return self.EnableParameter(paramKey)
      elif paramType in [ParameterType_Group]:
        return ApplicationProxy(self, paramKey)
      elif paramType in [ParameterType_Choice]:
        return ApplicationProxy(self, paramKey, value)
      else:
        print ("Unsupported parameter type '%s' with key '%s'" %(self.GetParameterTypeAsString(paramType) ,paramKey))
      return

    def GetParameterValue(self, paramKey):
      paramType = self.GetParameterType(paramKey)
      if paramType in [ParameterType_InputProcessXML,
                       ParameterType_String, ParameterType_InputFilename,
                       ParameterType_OutputImage, ParameterType_OutputVectorData,
                       ParameterType_OutputProcessXML, ParameterType_OutputFilename,
                       ParameterType_Directory, ParameterType_InputImage,
                       ParameterType_ComplexInputImage, ParameterType_InputVectorData]:
        return self.GetParameterString(paramKey)
      elif paramType in [ParameterType_InputImageList, ParameterType_InputVectorDataList,
                         ParameterType_InputFilenameList, ParameterType_StringList,
                         ParameterType_ListView]:
        return self.GetParameterStringList(paramKey)
      elif paramType in [ParameterType_Int, ParameterType_Radius, ParameterType_RAM]:
        return self.GetParameterInt(paramKey)
      elif paramType in [ParameterType_Float]:
        return self.GetParameterFloat(paramKey)
      elif paramType in [ParameterType_Empty]:
        return self.IsParameterEnabled(paramKey)
      elif paramType in [ParameterType_Group, ParameterType_Choice]:
        return ApplicationProxy(self, paramKey)
      else:
        print ("Unsupported parameter type '%s' with key '%s'" %(self.GetParameterTypeAsString(paramType) ,paramKey))
      return None

    def __getattr__(self,name):
      """
      __get_attribute__ is called whenever an instance request an attribute.
      eg: App.SetParameterString(), App.GetName() ..
      __getattr__ is only called if the attribute is not found by __get_attribute__ call
      So we keep hide the GetParameter** calls within this method so that it seems like
      an obivous call for users. App.IN , App.OUT , where 'in' and 'out' are
      parameters in the 'otb application' with instance App
      Since SWIG also uses this function, we have to copy their code before
      using custom OTB behaviour
      """
      if (name == "thisown"):
        return self.this.own()
      method = Application.__swig_getmethods__.get(name, None)
      if method:
        return method(self)
      key_list = [k.upper() for k in self.GetParametersKeys(True)]
      if name in key_list:
        return self.GetParameterValue(name.lower())
      raise AttributeError("'%s' object has no attribute '%s'" % (Application.__name__, name))

    def __setattr__(self, name, value):
      """
      __setattr__ is called if the attribute requested is not found in the attribute list.
      So these attributes are supposed to be 'key' of parameters used. Here we
      keep hide the SetParameter** calls within this method so that it seems like
      an obivous call for users. App.IN='my-input-file-name' , App.OUT='my-output-file-name'w
      here 'in' and 'out' are    parameters in the 'otb application' with instance App
      Ofcourse, we don't blindly accept any attributes as python, we check them against
      list of existing parameters for application with 'self.GetParametersKeys(True)'
      Since SWIG also uses this function, we have to copy their code before
      using custom OTB behaviour
      """
      if (name == "thisown"):
        return self.this.own(value)
      if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
          self.__dict__[name] = value
          return
      method = Application.__swig_setmethods__.get(name, None)
      if method:
        return method(self, value)
      key_list = [k.upper() for k in self.GetParametersKeys(True)]
      if name in key_list:
        self.SetParameterValue(name.lower(), value)
      else:
        raise AttributeError("You cannot add attributes to %s" % self)

    }
}