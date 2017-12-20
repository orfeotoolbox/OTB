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