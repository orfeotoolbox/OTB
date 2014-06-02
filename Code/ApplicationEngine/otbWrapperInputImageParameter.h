/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbWrapperInputImageParameter_h
#define __otbWrapperInputImageParameter_h

#include "otbImageFileReader.h"
#include "itkImageBase.h"
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{
/** \class InputImageParameter
 *  \brief This class represents a InputImage parameter
 */

class ITK_ABI_EXPORT InputImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputImageParameter           Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ImageBase<2> ImageBaseType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputImageParameter, Parameter);

  /** Set value from filename */
  bool SetFromFileName(const std::string& filename);
  itkGetConstMacro(FileName, std::string);


  /** Get the input image as FloatVectorImageType. */
  FloatVectorImageType* GetImage();

  /** Get the input image as XXXImageType */
  UInt8ImageType* GetUInt8Image();
  UInt16ImageType* GetUInt16Image();
  Int16ImageType* GetInt16Image();
  UInt32ImageType* GetUInt32Image();
  Int32ImageType* GetInt32Image();
  FloatImageType* GetFloatImage();
  DoubleImageType* GetDoubleImage();

  UInt8VectorImageType* GetUInt8VectorImage();
  UInt16VectorImageType* GetUInt16VectorImage();
  Int16VectorImageType* GetInt16VectorImage();
  UInt32VectorImageType* GetUInt32VectorImage();
  Int32VectorImageType* GetInt32VectorImage();
  FloatVectorImageType* GetFloatVectorImage();
  DoubleVectorImageType* GetDoubleVectorImage();

  UInt8RGBImageType* GetUInt8RGBImage();
  UInt8RGBAImageType* GetUInt8RGBAImage();


  /** Get the input image as templated image type. */
  template <class TImageType>
    TImageType* GetImage();

  /** Set a FloatVectorImageType image.*/
  void SetImage(FloatVectorImageType* image);

  /** Set a templated image.*/
  template <class TImageType>
    void SetImage(TImageType* image);


  /** Generic cast method that will be specified for each image type. */
  template <class TInputImage, class TOutputImage>
  ITK_ABI_EXPORT TOutputImage*  CastImage();
    
  /** Cast an image to an image of the same type
  * Image to Image, VectorImage to VectorImage, RGBAImage to RGBAImage. */
  template <class TInputImage, class TOutputImage>
    TOutputImage* SimpleCastImage();


  /** Cast an image to a vector image. */
  template <class TInputImage, class TOutputImage>
    TOutputImage* CastVectorImageFromImage();


  bool HasValue() const;

  void ClearValue();


protected:
  /** Constructor */
  InputImageParameter();

  /** Destructor */
  virtual ~InputImageParameter();

  ImageBaseType::Pointer m_Image;
  std::string m_FileName;

  /** Readers typedefs */

  typedef otb::ImageFileReader<UInt8ImageType> UInt8ReaderType;
  typedef otb::ImageFileReader<Int16ImageType> Int16ReaderType;
  typedef otb::ImageFileReader<UInt16ImageType> UInt16ReaderType;
  typedef otb::ImageFileReader<Int32ImageType> Int32ReaderType;
  typedef otb::ImageFileReader<UInt32ImageType> UInt32ReaderType;
  typedef otb::ImageFileReader<FloatImageType> FloatReaderType;
  typedef otb::ImageFileReader<DoubleImageType> DoubleReaderType;

  typedef otb::ImageFileReader<UInt8VectorImageType> UInt8VectorReaderType;
  typedef otb::ImageFileReader<Int16VectorImageType> Int16VectorReaderType;
  typedef otb::ImageFileReader<UInt16VectorImageType> UInt16VectorReaderType;
  typedef otb::ImageFileReader<Int32VectorImageType> Int32VectorReaderType;
  typedef otb::ImageFileReader<UInt32VectorImageType> UInt32VectorReaderType;
  typedef otb::ImageFileReader<FloatVectorImageType> FloatVectorReaderType;
  typedef otb::ImageFileReader<DoubleVectorImageType> DoubleVectorReaderType;


  typedef otb::ImageFileReader<UInt8RGBImageType>  UInt8RGBReaderType;
  typedef otb::ImageFileReader<UInt8RGBAImageType> UInt8RGBAReaderType;

  itk::ProcessObject::Pointer m_Reader;
  itk::ProcessObject::Pointer m_Caster;

private:
  InputImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

  /** Store the loaded image filename */
  std::string m_PreviousFileName;

  /** flag : are we using a filename or an image pointer as an input */
  bool m_UseFilename;

}; // End class InputImage Parameter


// template specializations of CastImage<> should be declared in header
// so that the linker knows they exist when building OTB Applications

#define otbDeclareCastImageMacro(InputImageType, OutputImageType)   \
  template<> OutputImageType *                                          \
  InputImageParameter::CastImage<InputImageType , OutputImageType>();    \

#define otbGenericDeclareCastImageMacro(InputImageType, prefix)     \
  otbDeclareCastImageMacro(InputImageType, UInt8##prefix##ImageType) \
  otbDeclareCastImageMacro(InputImageType, UInt16##prefix##ImageType) \
  otbDeclareCastImageMacro(InputImageType, Int16##prefix##ImageType) \
  otbDeclareCastImageMacro(InputImageType, UInt32##prefix##ImageType) \
  otbDeclareCastImageMacro(InputImageType, Int32##prefix##ImageType) \
  otbDeclareCastImageMacro(InputImageType, Float##prefix##ImageType) \
  otbDeclareCastImageMacro(InputImageType, Double##prefix##ImageType)

  
/*********************************************************************
********************** Image -> Image
**********************************************************************/
otbGenericDeclareCastImageMacro(UInt8ImageType, )
otbGenericDeclareCastImageMacro(Int16ImageType, )
otbGenericDeclareCastImageMacro(UInt16ImageType, )
otbGenericDeclareCastImageMacro(Int32ImageType, )
otbGenericDeclareCastImageMacro(UInt32ImageType, )
otbGenericDeclareCastImageMacro(FloatImageType, )
otbGenericDeclareCastImageMacro(DoubleImageType, )


/*********************************************************************
********************** VectorImage -> VectorImage
**********************************************************************/
otbGenericDeclareCastImageMacro(UInt8VectorImageType, Vector)
otbGenericDeclareCastImageMacro(Int16VectorImageType, Vector)
otbGenericDeclareCastImageMacro(UInt16VectorImageType, Vector)
otbGenericDeclareCastImageMacro(Int32VectorImageType, Vector)
otbGenericDeclareCastImageMacro(UInt32VectorImageType, Vector)
otbGenericDeclareCastImageMacro(FloatVectorImageType, Vector)
otbGenericDeclareCastImageMacro(DoubleVectorImageType, Vector)


/*********************************************************************
********************** Image -> VectorImage
**********************************************************************/
otbGenericDeclareCastImageMacro(UInt8ImageType, Vector)
otbGenericDeclareCastImageMacro(Int16ImageType, Vector)
otbGenericDeclareCastImageMacro(UInt16ImageType, Vector)
otbGenericDeclareCastImageMacro(Int32ImageType, Vector)
otbGenericDeclareCastImageMacro(UInt32ImageType, Vector)
otbGenericDeclareCastImageMacro(FloatImageType, Vector)
otbGenericDeclareCastImageMacro(DoubleImageType, Vector)

#undef otbDeclareCastImageMacro
#undef otbGenericDeclareCastImageMacro


} // End namespace Wrapper
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWrapperInputImageParameter.txx"
#endif

#endif
