/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbImageIOBase_h
#define otbImageIOBase_h

#include "itkLightProcessObject.h"
#include "itkIndent.h"
#include "itkImageIORegion.h"
#include "vnl/vnl_vector.h"

#include <string>
#include <typeinfo>
#include <vector>

#include "OTBImageBaseExport.h"

namespace otb
{

/** \class ImageIOBase
 * \brief Abstract superclass defines image IO interface.
 *
 * ImageIOBase is a class that reads and/or writes image data
 * of a particular format (such as PNG or raw binary). The
 * ImageIOBase encapsulates both the reading and writing of data. The
 * ImageIOBase is used by the ImageFileReader class (to read data)
 * and the ImageFileWriter (to write data) into a single file. The
 * ImageSeriesReader and ImageSeriesWriter classes are used to read
 * and write data (in conjunction with ImageIOBase) when the data is
 * represented by a series of files. Normally the user does not directly
 * manipulate this class other than to instantiate it, set the FileName,
 * and assign it to a ImageFileReader/ImageFileWriter or
 * ImageSeriesReader/ImageSeriesWriter.
 *
 * A Pluggable factory pattern is used this allows different kinds of readers
 * to be registered (even at run time) without having to modify the
 * code in this class.
 *
 * \sa ImageFileWriter
 * \sa ImageFileReader
 * \sa ImageSeriesWriter
 * \sa ImageSeriesReader
 *
 * \ingroup IOFilters
 *
 *
 * \ingroup OTBImageBase
 */
class OTBImageBase_EXPORT ImageIOBase : public itk::LightProcessObject
{
public:
  /** Standard class typedefs. */
  typedef ImageIOBase             Self;
  typedef itk::LightProcessObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageIOBase, Superclass);

  /** Set/Get the name of the file to be read. */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  /** Types for managing image size and image index components. */
  typedef long          IndexValueType;
  typedef unsigned long SizeValueType;

  /**
   * \class UnknownType
   * Used to return information when types are unknown.
 *
 * \ingroup OTBImageBase
   */
  class UnknownType
  {
  };

  /** Enums used to manipulate the pixel type. The pixel type provides
   * context for automatic data conversions (for instance, RGB to
   * SCALAR, VECTOR to SCALAR). */
  typedef enum {
    UNKNOWNPIXELTYPE,
    SCALAR,
    RGB,
    RGBA,
    OFFSET,
    VECTOR,
    POINT,
    COVARIANTVECTOR,
    SYMMETRICSECONDRANKTENSOR,
    DIFFUSIONTENSOR3D,
    COMPLEX,
    FIXEDARRAY,
    MATRIX
  } IOPixelType;

  /** Enums used to manipulate the component type. The component type
   * refers to the actual storage class associated with either a
   * SCALAR pixel type or elements of a compound pixel.
   */
  typedef enum { UNKNOWNCOMPONENTTYPE, UCHAR, CHAR, USHORT, SHORT, UINT, INT, ULONG, LONG, FLOAT, DOUBLE, CSHORT, CINT, CFLOAT, CDOUBLE } IOComponentType;

  /** Set/Get the number of independent variables (dimensions) in the
   * image being read or written. Note this is not necessarily what
   * is written, rather the IORegion controls that. */
  void SetNumberOfDimensions(unsigned int);
  itkGetConstMacro(NumberOfDimensions, unsigned int);

  /** Set/Get the image dimensions in the x, y, z, etc. directions.
   * GetDimensions() is typically used after reading the data; the
   * SetDimensions() is used prior to writing the data. */
  virtual void SetDimensions(unsigned int i, unsigned int dim);
  virtual unsigned int GetDimensions(unsigned int i) const
  {
    return m_Dimensions[i];
  }

  /** Set/Get the image origin on a axis-by-axis basis. The SetOrigin() method
   * is required when writing the image. */
  virtual void SetOrigin(unsigned int i, double origin);
  virtual double GetOrigin(unsigned int i) const
  {
    return m_Origin[i];
  }

  /** Set/Get the image spacing on an axis-by-axis basis. The
   * SetSpacing() method is required when writing the image. */
  virtual void SetSpacing(unsigned int i, double spacing);
  virtual double GetSpacing(unsigned int i) const
  {
    return m_Spacing[i];
  }

  /** Set/Get the image direction on an axis-by-axis basis. The
   * SetDirection() method is required when writing the image. */
  virtual void SetDirection(unsigned int i, std::vector<double>& direction);
  virtual void SetDirection(unsigned int i, vnl_vector<double>& direction);
  virtual std::vector<double> GetDirection(unsigned int i) const
  {
    return m_Direction[i];
  }

  /** Return the directions to be assigned by default to recipient
   *  images whose dimension is smaller than the image dimension in file.  */
  virtual std::vector<double> GetDefaultDirection(unsigned int i) const;

  /** Specify the region of the image data to either read or
   * write. The IORegion specifies the part of the image to read or
   * write. Regions are defined with an index and a size vector. These
   * vectors define the start (lower-left corner) and length of the
   * region within the image. Make sure that the IORegion lies within
   * the image. */
  itkSetMacro(IORegion, itk::ImageIORegion);
  itkGetConstReferenceMacro(IORegion, itk::ImageIORegion);

  /** Set/Get the type of the pixel. The PixelTypes provides context
   * to the IO mechanisms for data conversions.  PixelTypes can be
   * SCALAR, RGB, RGBA, VECTOR, COVARIANTVECTOR, POINT, INDEX. If
   * the PIXELTYPE is SCALAR, then the NumberOfComponents should be 1.
   * Any other of PIXELTYPE will have more than one component. */
  itkSetEnumMacro(PixelType, IOPixelType);
  itkGetEnumMacro(PixelType, IOPixelType);

  /** SetPixelTypeInfo is used by writers to convert from an ITK
   * strongly typed pixel to a ImageIO (weaker) typed pixel. This
   * function sets these PixelType, ComponentType, and
   * NumberOfComponents based on RTTI type_info structure passed
   * in. The function returns false if the pixel type is not
   * supported. */
  virtual bool SetPixelTypeInfo(const std::type_info& ptype);

  /** Set/Get the component type of the image. This is always a native
   * type. */
  itkSetEnumMacro(ComponentType, IOComponentType);
  itkGetEnumMacro(ComponentType, IOComponentType);
  virtual const std::type_info& GetComponentTypeInfo() const;

  /** Set/Get the number of components per pixel in the image. This may
   * be set by the reading process. For SCALAR pixel types,
   * NumberOfComponents will be 1.  For other pixel types,
   * NumberOfComponents will be greater than or equal to one. */
  itkSetMacro(NumberOfComponents, unsigned int);
  itkGetConstReferenceMacro(NumberOfComponents, unsigned int);

  /** Set/Get a boolean to use the compression or not. */
  itkSetMacro(UseCompression, bool);
  itkGetConstMacro(UseCompression, bool);
  itkBooleanMacro(UseCompression);

  /** Set/Get a boolean to use streaming while reading or not. */
  itkSetMacro(UseStreamedReading, bool);
  itkGetConstMacro(UseStreamedReading, bool);
  itkBooleanMacro(UseStreamedReading);

  /** Set/Get a boolean to use streaming while writing or not. */
  itkSetMacro(UseStreamedWriting, bool);
  itkGetConstMacro(UseStreamedWriting, bool);
  itkBooleanMacro(UseStreamedWriting);


  /** Convenience method returns the IOComponentType as a string. This can be
   * used for writing output files. */
  static std::string GetComponentTypeAsString(IOComponentType);

  /** Convenience method returns the IOPixelType as a string. This can be
   * used for writing output files. */
  static std::string GetPixelTypeAsString(IOPixelType);

  /** Enums used to specify write style: whether binary or ASCII. Some
   * subclasses use this, some ignore it. */
  typedef enum { ASCII, Binary, TypeNotApplicable } FileType;

  /** Enums used to specify byte order; whether Big Endian or Little Endian.
   * Some subclasses use this, some ignore it. */
  typedef enum { BigEndian, LittleEndian, OrderNotApplicable } ByteOrder;

  /** These methods control whether the file is written binary or ASCII.
   * Many file formats (i.e., subclasses) ignore this flag. */
  itkSetEnumMacro(FileType, FileType);
  itkGetEnumMacro(FileType, FileType);
  void SetFileTypeToASCII()
  {
    this->SetFileType(ASCII);
  }
  void SetFileTypeToBinary()
  {
    this->SetFileType(Binary);
  }

  /** These methods indicate the byte ordering of the file you are
   * trying to read in. These methods will then either swap or not
   * swap the bytes depending on the byte ordering of the machine it
   * is being run on. For example, reading in a BigEndian file on a
   * BigEndian machine will result in no swapping. Trying to read the
   * same file on a LittleEndian machine will result in swapping.
   * Note: most UNIX machines are BigEndian while PC's and VAX's are
   * LittleEndian. So if the file you are reading in was generated on
   * a VAX or PC, SetByteOrderToLittleEndian() otherwise
   * SetByteOrderToBigEndian().  Some ImageIOBase subclasses
   * ignore these methods. */
  itkSetEnumMacro(ByteOrder, ByteOrder);
  itkGetEnumMacro(ByteOrder, ByteOrder);
  void SetByteOrderToBigEndian()
  {
    this->SetByteOrder(BigEndian);
  }
  void SetByteOrderToLittleEndian()
  {
    this->SetByteOrder(LittleEndian);
  }

  /** Convenience method returns the FileType as a string. This can be
   * used for writing output files. */
  static std::string GetFileTypeAsString(FileType);

  /** Convenience method returns the ByteOrder as a string. This can be
   * used for writing output files. */
  static std::string GetByteOrderAsString(ByteOrder);

  /** Type for representing size of bytes, and or positions along a file */
  typedef std::streamoff SizeType;

  /** Type for representing size of bytes, and or positions along a memory buffer */
  typedef size_t BufferSizeType;

  /** Convenient method for accessing the number of bytes to get to
   * the next pixel. Returns m_Strides[1];
   *
   * Please note that this methods depends the private methods
   * ComputeStrides being called, otherwise this is the incorrect value.
   */
  virtual SizeType GetPixelStride() const;

  /** Return the number of pixels in the image. */
  SizeType GetImageSizeInPixels() const;

  /** Return the number of bytes in the image. */
  SizeType GetImageSizeInBytes() const;

  /** Return the number of pixels times the number
   * of components in the image. */
  SizeType GetImageSizeInComponents() const;


  /** Compute the size (in bytes) of the components of a pixel. For
   * example, and RGB pixel of unsigned char would have a
   * component size of 1 byte. This method can be invoked only after
   * the component type is set. */
  virtual unsigned int GetComponentSize() const;

  /** Get the number of overviews available into the file specified
   * Returns: overview count, zero if none. */
  virtual unsigned int GetOverviewsCount() = 0;

  /** Get information about overviews available into the file specified
   * Returns: overview info, empty if none. */
  virtual std::vector<std::string> GetOverviewsInfo() = 0;

  /** Provide hist about the output container to deal with complex pixel
   *  type */
  virtual void SetOutputImagePixelType(bool isComplexInternalPixelType, bool isVectorImage) = 0;

  /*-------- This part of the interfaces deals with reading data ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*) = 0;

  /** Determine if the ImageIO can stream reading from this
      file. Default is false. */
  virtual bool CanStreamRead()
  {
    return false;
  }

  /** Read the spacing and dimensions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void ReadImageInformation() = 0;

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer) = 0;


  /*-------- This part of the interfaces deals with writing data ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*) = 0;

  /** Determine if the ImageIO can stream writing to this file. Default is false.
   *
   * There are two types of non exclusive streaming: pasting subregions, and iterative
   * If true then
   */
  virtual bool CanStreamWrite()
  {
    return false;
  }

  /** Writes the spacing and dimensions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void WriteImageInformation() = 0;

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. The buffer is cast to a
   * pointer to the beginning of the image data. */
  virtual void Write(const void* buffer) = 0;

  /* --- Support reading and writing data as a series of files. --- */

  /** The different types of ImageIO's can support data of varying
   * dimensionality. For example, some file formats are strictly 2D
   * while others can support 2D, 3D, or even n-D. This method returns
   * true/false as to whether the ImageIO can support the dimension
   * indicated. */
  virtual bool SupportsDimension(unsigned long dim)
  {
    return (dim == 2);
  }

  /** Method for supporting streaming.  Given a requested region, determine what
   * could be the region that we can read from the file. This is called the
   * streamable region, which will be equal or smaller than the
   * LargestPossibleRegion (unless it was dimensionaly clipped) and
   * greater or equal to the RequestedRegion
   *
   * the resulting IORegion may be a greater dimensions the the
   * requested IORegion, if the the derived class is unable to read
   * the requested region. For example if the file has a size of [ 10,
   * 10, 10] but the requested region is [10, 10] the return may be 3 dimensions.
   */
  virtual itk::ImageIORegion GenerateStreamableReadRegionFromRequestedRegion(const itk::ImageIORegion& requested) const;


  /** Before this method is called all the configuration will be done,
   * that is Streaming/PasteRegion/Compression/Filename etc
   * If pasting is being used the number of requested splits is for that
   * region not the largest. The derived ImageIO class should verify that
   * the file is capable of being written with this configuration.
   * If pasted is enabled and is not support or does not work with the file,
   * then an excepetion should be thrown.
   *
   * The default implementation depends on CanStreamWrite.
   * If false then 1 is returned (unless pasting is indicated), so that the whole file will be updated in one region.
   * If true then its assumed that any arbitrary region can be written
   * to any file. So the users request will be respected. If a derived
   * class has more restictive conditions then they should be checked
   */
  virtual unsigned int GetActualNumberOfSplitsForWriting(unsigned int numberOfRequestedSplits, const itk::ImageIORegion& pasteRegion,
                                                         const itk::ImageIORegion& largestPossibleRegion);

  /** returns the ith IORegion
   *
   * numberOfActualSplits should be the value returned from GetActualNumberOfSplitsForWriting with the same parameters
   *
   * Derieved classes should overload this method to return a compatible region
   */
  virtual itk::ImageIORegion GetSplitRegionForWriting(unsigned int ithPiece, unsigned int numberOfActualSplits, const itk::ImageIORegion& pasteRegion,
                                                      const itk::ImageIORegion& largestPossibleRegion);

  /** Type for the list of strings to be used for extensions.  */
  typedef std::vector<std::string> ArrayOfExtensionsType;

  /** This method returns an array with the list of filename extensions
   * supported for reading by this ImageIO class. This is intended to
   * facilitate GUI and application level integration.
   */
  const ArrayOfExtensionsType& GetSupportedReadExtensions() const;

  /** This method returns an array with the list of filename extensions
   * supported for writing by this ImageIO class. This is intended to
   * facilitate GUI and application level integration.
   */
  const ArrayOfExtensionsType& GetSupportedWriteExtensions() const;

  /** Remap band order in an input buffer using band mapping bandList
   *  This operation is done in-place. The buffer size should enough to
   *  contain extracted bands before and after mapping. bandList mapping
   * between origin components and output components (before any
   * conversion)*/
  void DoMapBuffer(void* buffer, size_t numberOfPixels, std::vector<unsigned int>& bandList);

  /** Returns a const ref to the list of attached files*/
  itkGetConstReferenceMacro(AttachedFileNames, std::vector<std::string>);

protected:
  ImageIOBase();
  ~ImageIOBase() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Used internally to keep track of the type of the pixel. */
  IOPixelType m_PixelType;

  /** Used internally to keep track of the type of the component. It is set
   * when ComputeStrides() is invoked. */
  IOComponentType m_ComponentType;

  /** Big or Little Endian, and the type of the file. (May be ignored.) */
  ByteOrder m_ByteOrder;
  FileType  m_FileType;

  /** Does the ImageIOBase object have enough info to be of use? */
  bool m_Initialized;

  /** Filename to read */
  std::string m_FileName;

  /** Stores the number of components per pixel. This will be 1 for
   * grayscale images, 3 for RGBPixel images, and 4 for RGBPixelA images. */
  unsigned int m_NumberOfComponents;

  /** The number of independent dimensions in the image. */
  unsigned int m_NumberOfDimensions;

  /** Should we compress the data? */
  bool m_UseCompression;

  /** Should we use streaming for reading */
  bool m_UseStreamedReading;

  /** Should we use streaming for writing */
  bool m_UseStreamedWriting;

  /** The region to read or write. The region contains information about the
   * data within the region to read or write. */
  itk::ImageIORegion m_IORegion;

  /** The array which stores the number of pixels in the x, y, z directions. */
  std::vector<SizeValueType> m_Dimensions;

  /** The array which stores the spacing of pixels in the
   * x, y, z directions. */
  std::vector<double> m_Spacing;

  /** The array which stores the origin of the image. */
  std::vector<double> m_Origin;

  /** The arrays which store the direction cosines of the image. */
  std::vector<std::vector<double>> m_Direction;

  /** Stores the number of bytes it takes to get to the next 'thing'
   * e.g. component, pixel, row, slice, etc. */
  std::vector<SizeType> m_Strides;

  /** Return the object to an initialized state, ready to be used */
  virtual void Reset(const bool freeDynamic = true);

  /** Resize the ImageIOBase object to new dimensions. */
  void Resize(const unsigned int numDimensions, const unsigned int* dimensions);

  /** Compute the size (in bytes) of the pixel. For
   * example, and RGB pixel of unsigned char would have size 3 bytes. */
  virtual unsigned int GetPixelSize() const;

  /** Calculates the different strides (distance from one thing to the next).
   * Upon return,
   * strides[0] = bytes to get to the next component of a pixel,
   * strides[1] = bytes to get to the next pixel in x direction,
   * strides[2] = bytes to get to the next row in y direction,
   * strides[3] = bytes to get to the next slice in z direction, etc. */
  void ComputeStrides();

  /** Convenient method for accessing number of bytes to get to the next pixel
   * component. Returns m_Strides[0]. */
  SizeType GetComponentStride() const;

  /** Convenient method for accessing the number of bytes to get to the
   * next row. Returns m_Strides[2]. */
  SizeType GetRowStride() const;

  /** Convenient method for accessing the number of bytes to get to the
   * next slice. Returns m_Strides[3]. */
  SizeType GetSliceStride() const;

  /** Convenient method to write a buffer as ASCII text. */
  void WriteBufferAsASCII(std::ostream& os, const void* buffer, IOComponentType ctype, SizeType numberOfBytesToWrite);

  /** Convenient method to read a buffer as ASCII text. */
  void ReadBufferAsASCII(std::istream& os, void* buffer, IOComponentType ctype, SizeType numberOfBytesToBeRead);

  /** Convenient method to read a buffer as binary. Return true on success. */
  bool ReadBufferAsBinary(std::istream& os, void* buffer, SizeType numberOfBytesToBeRead);


  /** Insert an extension to the list of supported extensions for reading. */
  void AddSupportedReadExtension(const char* extension);

  /** Insert an extension to the list of supported extensions for writing. */
  void AddSupportedWriteExtension(const char* extension);

  /** an implementation of ImageRegionSplitter:GetNumberOfSplits
   */
  virtual unsigned int GetActualNumberOfSplitsForWritingCanStreamWrite(unsigned int numberOfRequestedSplits, const itk::ImageIORegion& pasteRegion) const;

  /** an implementation of  ImageRegionSplitter:GetSplit
   */
  virtual itk::ImageIORegion GetSplitRegionForWritingCanStreamWrite(unsigned int ithPiece, unsigned int numberOfActualSplits,
                                                                    const itk::ImageIORegion& pasteRegion) const;

  /** List of files part of the same dataset as the input filename */
  std::vector<std::string> m_AttachedFileNames;

private:
  ImageIOBase(const Self&) = delete;
  void operator=(const Self&) = delete;

  ArrayOfExtensionsType m_SupportedReadExtensions;
  ArrayOfExtensionsType m_SupportedWriteExtensions;
};

} // end namespace itk

#endif // otbImageIOBase_h
