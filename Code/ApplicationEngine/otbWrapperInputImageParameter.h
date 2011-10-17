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

class ITK_EXPORT InputImageParameter : public Parameter
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
  void SetFromFileName(const std::string& filename);
  itkGetConstMacro(FileName, std::string);


  /** Get the input image as FloatVectorImageType. */
  FloatVectorImageType* GetImage();

  /** Get the input image as XXXImageType */
  UInt8ImageType* GetUInt8Image();
  Int8ImageType* GetInt8Image();
  UInt16ImageType* GetUInt16Image();
  Int16ImageType* GetInt16Image();
  UInt32ImageType* GetUInt32Image();
  Int32ImageType* GetInt32Image();
  FloatImageType* GetFloatImage();
  DoubleImageType* GetDoubleImage();

  UInt8VectorImageType* GetUInt8VectorImage();
  Int8VectorImageType* GetInt8VectorImage();
  UInt16VectorImageType* GetUInt16VectorImage();
  Int16VectorImageType* GetInt16VectorImage();
  UInt32VectorImageType* GetUInt32VectorImage();
  Int32VectorImageType* GetInt32VectorImage();
  FloatVectorImageType* GetFloatVectorImage();
  DoubleVectorImageType* GetDoubleVectorImage();
 
  UInt8RGBAImageType* GetUInt8RGBAImage();
  Int8RGBAImageType* GetInt8RGBAImage();
  UInt16RGBAImageType* GetUInt16RGBAImage();
  Int16RGBAImageType* GetInt16RGBAImage();
  UInt32RGBAImageType* GetUInt32RGBAImage();
  Int32RGBAImageType* GetInt32RGBAImage();
  FloatRGBAImageType* GetFloatRGBAImage();
  DoubleRGBAImageType* GetDoubleRGBAImage();

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
    TOutputImage* CastImage()
  {
    itkExceptionMacro("Cast from "<<typeid(TInputImage).name()<<" to "<<typeid(TInputImage).name()<<" not authorized.");
  }

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

  typedef otb::ImageFileReader<Int8ImageType> Int8ReaderType;
  typedef otb::ImageFileReader<UInt8ImageType> UInt8ReaderType;
  typedef otb::ImageFileReader<Int16ImageType> Int16ReaderType;
  typedef otb::ImageFileReader<UInt16ImageType> UInt16ReaderType;
  typedef otb::ImageFileReader<Int32ImageType> Int32ReaderType;
  typedef otb::ImageFileReader<UInt32ImageType> UInt32ReaderType;
  typedef otb::ImageFileReader<FloatImageType> FloatReaderType;
  typedef otb::ImageFileReader<DoubleImageType> DoubleReaderType;

  typedef otb::ImageFileReader<Int8VectorImageType> Int8VectorReaderType;
  typedef otb::ImageFileReader<UInt8VectorImageType> UInt8VectorReaderType;
  typedef otb::ImageFileReader<Int16VectorImageType> Int16VectorReaderType;
  typedef otb::ImageFileReader<UInt16VectorImageType> UInt16VectorReaderType;
  typedef otb::ImageFileReader<Int32VectorImageType> Int32VectorReaderType;
  typedef otb::ImageFileReader<UInt32VectorImageType> UInt32VectorReaderType;
  typedef otb::ImageFileReader<FloatVectorImageType> FloatVectorReaderType;
  typedef otb::ImageFileReader<DoubleVectorImageType> DoubleVectorReaderType;

  typedef otb::ImageFileReader<Int8RGBAImageType> Int8RGBAReaderType;
  typedef otb::ImageFileReader<UInt8RGBAImageType> UInt8RGBAReaderType;
  typedef otb::ImageFileReader<Int16RGBAImageType> Int16RGBAReaderType;
  typedef otb::ImageFileReader<UInt16RGBAImageType> UInt16RGBAReaderType;
  typedef otb::ImageFileReader<Int32RGBAImageType> Int32RGBAReaderType;
  typedef otb::ImageFileReader<UInt32RGBAImageType> UInt32RGBAReaderType;
  typedef otb::ImageFileReader<FloatRGBAImageType> FloatRGBAReaderType;
  typedef otb::ImageFileReader<DoubleRGBAImageType> DoubleRGBAReaderType;

/*
  Int8ReaderType::Pointer   m_Int8Reader;
  UInt8ReaderType::Pointer  m_UInt8Reader;
  Int16ReaderType::Pointer  m_Int16Reader;
  UInt16ReaderType::Pointer m_UInt16Reader;
  Int32ReaderType::Pointer  m_Int32Reader;
  UInt32ReaderType::Pointer m_UInt32Reader;
  FloatReaderType::Pointer  m_FloatReader;
  DoubleReaderType::Pointer m_DoubleReader;

  VectorInt8ReaderType::Pointer   m_VectorInt8Reader;
  VectorUInt8ReaderType::Pointer  m_VectorUInt8Reader;
  VectorInt16ReaderType::Pointer  m_VectorInt16Reader;
  VectorUInt16ReaderType::Pointer m_VectorUInt16Reader;
  VectorInt32ReaderType::Pointer  m_VectorInt32Reader;
  VectorUInt32ReaderType::Pointer m_VectorUInt32Reader;
  VectorFloatReaderType::Pointer  m_VectorFloatReader;
  VectorDoubleReaderType::Pointer m_VectorDoubleReader;

  RGBAInt8ReaderType::Pointer   m_RGBAInt8Reader;
  RGBAUInt8ReaderType::Pointer  m_RGBAUInt8Reader;
  RGBAInt16ReaderType::Pointer  m_RGBAInt16Reader;
  RGBAUInt16ReaderType::Pointer m_RGBAUInt16Reader;
  RGBAInt32ReaderType::Pointer  m_RGBAInt32Reader;
  RGBAUInt32ReaderType::Pointer m_RGBAUInt32Reader;
  RGBAFloatReaderType::Pointer  m_RGBAFloatReader;
  RGBADoubleReaderType::Pointer m_RGBADoubleReader;
*/
  itk::ProcessObject::Pointer m_Reader;
  itk::ProcessObject::Pointer m_Caster;

private:
  InputImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class InputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
