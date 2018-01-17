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

#if OTB_SWIGNUMPY

%extend Application
{
  %pythoncode
    {
    def SetImageFromNumpyArray(self, paramKey, npArray):
      """
      This method takes a numpy array and set ImageIOBase of
      InputImageParameter by creating an otbImage with
      same pixel type as numpyarray.dtype
      """
      if len(npArray.shape) == 3:
         raise ValueError( "(len(npArray.shape) == 3)\n"
                           "Input array given is of 3 dimension.\n"
                           "SetImageFromNumpyArray create ImageIO from otbImage and thus demands a 2d array.\n"
                           "you can either provide an 2d numpy array or use SetVectorImageFromNumpyArray depending on your application.\n")

      dt = npArray.dtype.name
      if dt == 'int8':
        self.SetImageFromInt8NumpyArray_(paramKey, npArray)
      elif dt == 'int16':
        self.SetImageFromInt16NumpyArray_(paramKey, npArray)
      elif dt == 'int32':
        self.SetImageFromInt32NumpyArray_(paramKey, npArray)
      elif dt == 'uint8':
        self.SetImageFromUInt8NumpyArray_(paramKey, npArray)
      elif dt == 'uint16':
        self.SetImageFromUInt16NumpyArray_(paramKey, npArray)
      elif dt == 'uint32':
        self.SetImageFromUInt32NumpyArray_(paramKey, npArray)
      elif dt == 'float':
        self.SetImageFromFloatNumpyArray_(paramKey, npArray)
      elif dt == 'double':
        self.SetImageFromDoubleNumpyArray_(paramKey, npArray)
      else:
        self.SetImageFromFloatNumpyArray_(paramKey, npArray)
      return

    def SetVectorImageFromNumpyArray(self, paramKey, npArray):
      """
      This method takes a numpy array and set ImageIOBase of
      InputImageParameter by creating an otbVectorImage with
      same pixel type as numpyarray.dtype.
      NOTE: Input (npArray) must be an ndarray with 3 dimension,
      len(npArray.shape) must be > 2
      """
      if len(npArray.shape) < 3:
        raise ValueError( "(len(npArray.shape) < 3)\n"
                        "Input array given is not of 3 dimension.\n"
                        "SetVectorImageFromNumpyArray create ImageIO from otbVectorImage and thus demands an array of shape 3.\n"
                        "you can either provide an 3d numpy array or use SetImageFromNumpyArray depending on your application.\n")

      dt = npArray.dtype.name
      if dt == 'int8':
        self.SetVectorImageFromInt8NumpyArray_(paramKey, npArray)
      elif dt == 'int16':
        self.SetVectorImageFromInt16NumpyArray_(paramKey, npArray)
      elif dt == 'int32':
        self.SetVectorImageFromInt32NumpyArray_(paramKey, npArray)
      elif dt == 'uint8':
        self.SetVectorImageFromUInt8NumpyArray_(paramKey, npArray)
      elif dt == 'uint16':
        self.SetVectorImageFromUInt16NumpyArray_(paramKey, npArray)
      elif dt == 'uint32':
        self.SetVectorImageFromUInt32NumpyArray_(paramKey, npArray)
      elif dt == 'float':
        self.SetVectorImageFromFloatNumpyArray_(paramKey, npArray)
      elif dt == 'double':
        self.SetVectorImageFromDoubleNumpyArray_(paramKey, npArray)
      else:
        self.SetVectorImageFromFloatNumpyArray_(paramKey, npArray)
      return

    def GetVectorImageAsNumpyArray(self, paramKey, dt='float'):
      """
      If datatype is unknown this method assumes to numpy.float32
      Valid datatypes are:
      int8, int16, int32, uint8, uint16, uint32, float, double.
      NOTE: This method always return an numpy array with dimension 3
      """
      if dt == 'int8':
        return self.GetVectorImageAsInt8NumpyArray_(paramKey)
      elif dt == 'int16':
        return self.GetVectorImageAsInt16NumpyArray_(paramKey)
      elif dt == 'int32':
        return self.GetVectorImageAsInt32NumpyArray_(paramKey)
      elif dt == 'uint8':
        return self.GetVectorImageAsUInt8NumpyArray_(paramKey)
      elif dt == 'uint16':
        return self.GetVectorImageAsUInt16NumpyArray_(paramKey)
      elif dt == 'uint32':
        return self.GetVectorImageAsUInt32NumpyArray_(paramKey)
      elif dt == 'float':
        return self.GetVectorImageAsFloatNumpyArray_(paramKey)
      elif dt == 'double':
        return self.GetVectorImageAsDoubleNumpyArray_(paramKey)
      else:
        print ("Unknown datatype '" + dt + "'. Using float instead. Available types are:")
        print ("int8, int16, int32, uint8, uint16, uint32, float, double")
        return self.GetVectorImageAsFloatNumpyArray_(paramKey)

    def GetImageAsNumpyArray(self, paramKey, dt='float'):
      """
      If datatype is unknown this method assumes to numpy.float32
      Valid datatypes are:
      int8, int16, int32, uint8, uint16, uint32, float, double.
      NOTE: This method always return an numpy array with dimension 3
      """
      if dt == 'int8':
        numpy_vector_image = self.GetVectorImageAsInt8NumpyArray_(paramKey)
      elif dt == 'int16':
        numpy_vector_image = self.GetVectorImageAsInt16NumpyArray_(paramKey)
      elif dt == 'int32':
        numpy_vector_image = self.GetVectorImageAsInt32NumpyArray_(paramKey)
      elif dt == 'uint8':
        numpy_vector_image = self.GetVectorImageAsUInt8NumpyArray_(paramKey)
      elif dt == 'uint16':
        numpy_vector_image = self.GetVectorImageAsUInt16NumpyArray_(paramKey)
      elif dt == 'uint32':
        numpy_vector_image = self.GetVectorImageAsUInt32NumpyArray_(paramKey)
      elif dt == 'float':
        numpy_vector_image = self.GetVectorImageAsFloatNumpyArray_(paramKey)
      elif dt == 'double':
        numpy_vector_image = self.GetVectorImageAsDoubleNumpyArray_(paramKey)

      else:
        print ("Unknown datatype '" + dt + "'. Using float instead. Available types are:")
        print ("int8, int16, int32, uint8, uint16, uint32, float, double")
        numpy_vector_image = self.GetVectorImageAsFloatNumpyArray_(paramKey)

      if len(numpy_vector_image.shape) > 2:
        raise ValueError("len(numpy_vector_image.shape) > 2\n"
                         "Output image from application is of 3 dimension (len(nparray.shape) > 2). \n"
                         "GetImageFromNumpyArray returns an numpy array of dimension 2 that will result is loss of data.\n"
                         "In this case you must use GetVectorImageFromNumpyArray which is capable of return a 3 dimension image.\n")

      numpy_vector_image = numpy_vector_image[:,:,1]
      return numpy_vector_image


    }
}

#endif /* OTB_SWIGNUMPY */