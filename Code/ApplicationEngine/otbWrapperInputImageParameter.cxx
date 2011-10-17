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
#include "otbWrapperInputImageParameter.h"
#include "itksys/SystemTools.hxx"
#include "otbImageFileReader.h"
#include "itkCastImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbWrapperTypes.h"

namespace otb
{
namespace Wrapper
{

InputImageParameter::InputImageParameter()
{
  this->SetName("Input Image");
  this->SetKey("in");
  m_FileName="";
  this->ClearValue();
}

InputImageParameter::~InputImageParameter()
{
}

void
InputImageParameter::SetFromFileName(const std::string& filename)
{
  // First clear previous file choosen
  this->ClearValue();

  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  if (!filename.empty()
      && itksys::SystemTools::FileExists(filename.c_str()))
    {
    FloatVectorReaderType::Pointer reader = FloatVectorReaderType::New();
    reader->SetFileName(filename);
    try
      {
      reader->UpdateOutputInformation();
      }
    catch(itk::ExceptionObject & err)
      {
      }

    // the specified filename is valid => store the value
    m_FileName = filename;
    SetActive(true);
    }
}



FloatVectorImageType*
InputImageParameter::GetImage()
{
  return this->GetImage<FloatVectorImageType>();
}


#define otbGetImageMacro(image)                       \
  image##Type *                                       \
  InputImageParameter::Get##image ()                  \
  {                                                   \
    return this->GetImage< image##Type > ();          \
  }

otbGetImageMacro(UInt8Image)
otbGetImageMacro(Int8Image);
otbGetImageMacro(UInt16Image);
otbGetImageMacro(Int16Image);
otbGetImageMacro(UInt32Image);
otbGetImageMacro(Int32Image);
otbGetImageMacro(FloatImage);
otbGetImageMacro(DoubleImage);

otbGetImageMacro(UInt8VectorImage);
otbGetImageMacro(Int8VectorImage);
otbGetImageMacro(UInt16VectorImage);
otbGetImageMacro(Int16VectorImage);
otbGetImageMacro(UInt32VectorImage);
otbGetImageMacro(Int32VectorImage);
otbGetImageMacro(FloatVectorImage);
otbGetImageMacro(DoubleVectorImage);

otbGetImageMacro(UInt8RGBAImage);
otbGetImageMacro(Int8RGBAImage);
otbGetImageMacro(UInt16RGBAImage);
otbGetImageMacro(Int16RGBAImage);
otbGetImageMacro(UInt32RGBAImage);
otbGetImageMacro(Int32RGBAImage);
otbGetImageMacro(FloatRGBAImage);
otbGetImageMacro(DoubleRGBAImage);


template <class TOutputImage>
TOutputImage *
InputImageParameter::GetImage()
{
  // 2 cases : the user set a filename vs. the user set an image
  //////////////////////// Filename case:
  if( !m_FileName.empty() )
    {
    typedef otb::ImageFileReader<TOutputImage> ReaderType;
    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(m_FileName);
    try
      {
      reader->UpdateOutputInformation();
      }
    catch(itk::ExceptionObject & err)
      {
      this->ClearValue();
      }
    
    m_Image = reader->GetOutput();
    m_Reader = reader;

    // Pay attention, don't return m_Image because it is a ImageBase...
    return reader->GetOutput();
    }
  //////////////////////// Image case:
  else
    {
    if( m_Image.IsNull() )
      {
      itkExceptionMacro("No input image or filename detected...");
      }
    else
      {
  if (dynamic_cast<Int8ImageType*>(m_Image.GetPointer()))
      {
      return CastImage<Int8ImageType, TOutputImage>();
      }
    else if (dynamic_cast<UInt8ImageType*>(m_Image.GetPointer()))
      {
      return CastImage<UInt8ImageType, TOutputImage>();
      }
    else if (dynamic_cast<Int16ImageType*>(m_Image.GetPointer()))
      {
      return CastImage<Int16ImageType, TOutputImage>();
      }
    else if (dynamic_cast<UInt16ImageType*>(m_Image.GetPointer()))
      {
      return CastImage<UInt16ImageType, TOutputImage>();
      }
    else if (dynamic_cast<Int32ImageType*>(m_Image.GetPointer()))
      {
      return CastImage<Int32ImageType, TOutputImage>();
      }
    else if (dynamic_cast<UInt32ImageType*>(m_Image.GetPointer()))
      {
      return CastImage<UInt32ImageType, TOutputImage>();
      }
    else if (dynamic_cast<FloatImageType*>(m_Image.GetPointer()))
      {
      return CastImage<FloatImageType, TOutputImage>();
      }
    else if (dynamic_cast<DoubleImageType*>(m_Image.GetPointer()))
      {
      return CastImage<DoubleImageType, TOutputImage>();
      }
    else if (dynamic_cast<Int8VectorImageType*>(m_Image.GetPointer()))
      {
      return CastImage<Int8VectorImageType, TOutputImage>();
      }
    else if (dynamic_cast<UInt8VectorImageType*>(m_Image.GetPointer()))
      {
      return CastImage<UInt8VectorImageType, TOutputImage>();
      }
    else if (dynamic_cast<Int16VectorImageType*>(m_Image.GetPointer()))
      {
      return CastImage<Int16VectorImageType, TOutputImage>();
      }
    else if (dynamic_cast<UInt16VectorImageType*>(m_Image.GetPointer()))
      {
      return CastImage<UInt16VectorImageType, TOutputImage>();
      }
    else if (dynamic_cast<Int32VectorImageType*>(m_Image.GetPointer()))
      {
      return CastImage<Int32VectorImageType, TOutputImage>();
      }
    else if (dynamic_cast<UInt32VectorImageType*>(m_Image.GetPointer()))
      {
      return CastImage<UInt32VectorImageType, TOutputImage>();
      }
    else if (dynamic_cast<FloatVectorImageType*>(m_Image.GetPointer()))
      {
      return CastImage<FloatVectorImageType, TOutputImage>();
      }
    else if (dynamic_cast<DoubleVectorImageType*>(m_Image.GetPointer()))
      {
      return CastImage<DoubleVectorImageType, TOutputImage>();
      }
    else if (dynamic_cast<Int8RGBAImageType*>(m_Image.GetPointer()))
      {
      return CastImage<Int8RGBAImageType, TOutputImage>();
      }
    else if (dynamic_cast<UInt8RGBAImageType*>(m_Image.GetPointer()))
      {
      return CastImage<UInt8RGBAImageType, TOutputImage>();
      }
    else if (dynamic_cast<Int16RGBAImageType*>(m_Image.GetPointer()))
      {
      return CastImage<Int16RGBAImageType, TOutputImage>();
      }
    else if (dynamic_cast<UInt16RGBAImageType*>(m_Image.GetPointer()))
      {
      return CastImage<UInt16RGBAImageType, TOutputImage>();
      }
    else if (dynamic_cast<Int32RGBAImageType*>(m_Image.GetPointer()))
      {
      return CastImage<Int32RGBAImageType, TOutputImage>();
      }
    else if (dynamic_cast<UInt32RGBAImageType*>(m_Image.GetPointer()))
      {
      return CastImage<UInt32RGBAImageType, TOutputImage>();
      }
    else if (dynamic_cast<FloatRGBAImageType*>(m_Image.GetPointer()))
      {
      return CastImage<FloatRGBAImageType, TOutputImage>();
      }
    else if (dynamic_cast<DoubleRGBAImageType*>(m_Image.GetPointer()))
      {
      return CastImage<DoubleRGBAImageType, TOutputImage>();
      }
    else
      {
      itkExceptionMacro("Unknown image type");
      }
  
      }
    }
}


template <class TInputImage, class TOutputImage>
TOutputImage*
InputImageParameter::SimpleCastImage()
{
  TInputImage* realInputImage = dynamic_cast<TInputImage*>(m_Image.GetPointer());
  
  typedef itk::CastImageFilter<TInputImage, TOutputImage> CasterType;
  typename CasterType::Pointer caster = CasterType::New();
  
  caster->SetInput(realInputImage);
  caster->UpdateOutputInformation();
  
  m_Image = caster->GetOutput();
  m_Caster = caster;
  
  return caster->GetOutput();
}
  
                       
template <class TInputImage, class TOutputImage>
TOutputImage*
InputImageParameter::CastVectorImageFromImage()
{
  TInputImage* realInputImage = dynamic_cast<TInputImage*>(m_Image.GetPointer());
  
  typedef ImageToVectorImageCastFilter<TInputImage, TOutputImage> CasterType;
  typename CasterType::Pointer caster = CasterType::New();
  
  caster->SetInput(realInputImage);
  caster->UpdateOutputInformation();
  
  m_Image = caster->GetOutput();
  m_Caster = caster;
  
  return caster->GetOutput();
}

/*********************************************************************
********************** Image -> Image
**********************************************************************/
/////////// Int8ImageType->...
template <> Int8ImageType *
InputImageParameter::CastImage<Int8ImageType, Int8ImageType>(){
  return this->SimpleCastImage<Int8ImageType, Int8ImageType>();
}

template <> UInt8ImageType *
InputImageParameter::CastImage<Int8ImageType, UInt8ImageType>(){
  return this->SimpleCastImage<Int8ImageType, UInt8ImageType>();
}

template <>  Int16ImageType*
InputImageParameter::CastImage<Int8ImageType, Int16ImageType>(){
  return this->SimpleCastImage<Int8ImageType, Int16ImageType>();
}

template <> UInt16ImageType*
InputImageParameter::CastImage<Int8ImageType, UInt16ImageType>(){
  return this->SimpleCastImage<Int8ImageType, UInt16ImageType>();
}

template <>  Int32ImageType*
InputImageParameter::CastImage<Int8ImageType, Int32ImageType>(){
  return this->SimpleCastImage<Int8ImageType, Int32ImageType>();
}


template <>  UInt32ImageType*
InputImageParameter::CastImage<Int8ImageType, UInt32ImageType>(){
  return this->SimpleCastImage<Int8ImageType, UInt32ImageType>();
}

template <> FloatImageType *
InputImageParameter::CastImage<Int8ImageType, FloatImageType>(){
  return this->SimpleCastImage<Int8ImageType, FloatImageType>();
}

template <> DoubleImageType *
InputImageParameter::CastImage<Int8ImageType, DoubleImageType>(){
  return this->SimpleCastImage<Int8ImageType, DoubleImageType>();
}


/////////// UInt8ImageType->...
template <> Int8ImageType *
InputImageParameter::CastImage<UInt8ImageType, Int8ImageType>(){
  return this->SimpleCastImage<UInt8ImageType, Int8ImageType>();
}

template <> UInt8ImageType *
InputImageParameter::CastImage<UInt8ImageType, UInt8ImageType>(){
  return this->SimpleCastImage<UInt8ImageType, UInt8ImageType>();
}

template <>  Int16ImageType*
InputImageParameter::CastImage<UInt8ImageType, Int16ImageType>(){
  return this->SimpleCastImage<UInt8ImageType, Int16ImageType>();
}

template <> UInt16ImageType*
InputImageParameter::CastImage<UInt8ImageType, UInt16ImageType>(){
  return this->SimpleCastImage<UInt8ImageType, UInt16ImageType>();
}

template <>  Int32ImageType*
InputImageParameter::CastImage<UInt8ImageType, Int32ImageType>(){
  return this->SimpleCastImage<UInt8ImageType, Int32ImageType>();
}


template <>  UInt32ImageType*
InputImageParameter::CastImage<UInt8ImageType, UInt32ImageType>(){
  return this->SimpleCastImage<UInt8ImageType, UInt32ImageType>();
}

template <> FloatImageType *
InputImageParameter::CastImage<UInt8ImageType, FloatImageType>(){
  return this->SimpleCastImage<UInt8ImageType, FloatImageType>();
}

template <> DoubleImageType *
InputImageParameter::CastImage<UInt8ImageType, DoubleImageType>(){
  return this->SimpleCastImage<UInt8ImageType, DoubleImageType>();
}

/////////// Int16ImageType->...
template <> Int8ImageType *
InputImageParameter::CastImage<Int16ImageType, Int8ImageType>(){
  return this->SimpleCastImage<Int16ImageType, Int8ImageType>();
}

template <> UInt8ImageType *
InputImageParameter::CastImage<Int16ImageType, UInt8ImageType>(){
  return this->SimpleCastImage<Int16ImageType, UInt8ImageType>();
}

template <>  Int16ImageType*
InputImageParameter::CastImage<Int16ImageType, Int16ImageType>(){
  return this->SimpleCastImage<Int16ImageType, Int16ImageType>();
}

template <> UInt16ImageType*
InputImageParameter::CastImage<Int16ImageType, UInt16ImageType>(){
  return this->SimpleCastImage<Int16ImageType, UInt16ImageType>();
}

template <>  Int32ImageType*
InputImageParameter::CastImage<Int16ImageType, Int32ImageType>(){
  return this->SimpleCastImage<Int16ImageType, Int32ImageType>();
}


template <>  UInt32ImageType*
InputImageParameter::CastImage<Int16ImageType, UInt32ImageType>(){
  return this->SimpleCastImage<Int16ImageType, UInt32ImageType>();
}

template <> FloatImageType *
InputImageParameter::CastImage<Int16ImageType, FloatImageType>(){
  return this->SimpleCastImage<Int16ImageType, FloatImageType>();
}

template <> DoubleImageType *
InputImageParameter::CastImage<Int16ImageType, DoubleImageType>(){
  return this->SimpleCastImage<Int16ImageType, DoubleImageType>();
}

/////////// UInt16ImageType->...
template <> Int8ImageType *
InputImageParameter::CastImage<UInt16ImageType, Int8ImageType>(){
  return this->SimpleCastImage<UInt16ImageType, Int8ImageType>();
}

template <> UInt8ImageType *
InputImageParameter::CastImage<UInt16ImageType, UInt8ImageType>(){
  return this->SimpleCastImage<UInt16ImageType, UInt8ImageType>();
}

template <>  Int16ImageType*
InputImageParameter::CastImage<UInt16ImageType, Int16ImageType>(){
  return this->SimpleCastImage<UInt16ImageType, Int16ImageType>();
}

template <> UInt16ImageType*
InputImageParameter::CastImage<UInt16ImageType, UInt16ImageType>(){
  return this->SimpleCastImage<UInt16ImageType, UInt16ImageType>();
}

template <>  Int32ImageType*
InputImageParameter::CastImage<UInt16ImageType, Int32ImageType>(){
  return this->SimpleCastImage<UInt16ImageType, Int32ImageType>();
}


template <>  UInt32ImageType*
InputImageParameter::CastImage<UInt16ImageType, UInt32ImageType>(){
  return this->SimpleCastImage<UInt16ImageType, UInt32ImageType>();
}

template <> FloatImageType *
InputImageParameter::CastImage<UInt16ImageType, FloatImageType>(){
  return this->SimpleCastImage<UInt16ImageType, FloatImageType>();
}

template <> DoubleImageType *
InputImageParameter::CastImage<UInt16ImageType, DoubleImageType>(){
  return this->SimpleCastImage<UInt16ImageType, DoubleImageType>();
}

/////////// Int32ImageType->...
template <> Int8ImageType *
InputImageParameter::CastImage<Int32ImageType, Int8ImageType>(){
  return this->SimpleCastImage<Int32ImageType, Int8ImageType>();
}

template <> UInt8ImageType *
InputImageParameter::CastImage<Int32ImageType, UInt8ImageType>(){
  return this->SimpleCastImage<Int32ImageType, UInt8ImageType>();
}

template <>  Int16ImageType*
InputImageParameter::CastImage<Int32ImageType, Int16ImageType>(){
  return this->SimpleCastImage<Int32ImageType, Int16ImageType>();
}

template <> UInt16ImageType*
InputImageParameter::CastImage<Int32ImageType, UInt16ImageType>(){
  return this->SimpleCastImage<Int32ImageType, UInt16ImageType>();
}

template <>  Int32ImageType*
InputImageParameter::CastImage<Int32ImageType, Int32ImageType>(){
  return this->SimpleCastImage<Int32ImageType, Int32ImageType>();
}


template <>  UInt32ImageType*
InputImageParameter::CastImage<Int32ImageType, UInt32ImageType>(){
  return this->SimpleCastImage<Int32ImageType, UInt32ImageType>();
}

template <> FloatImageType *
InputImageParameter::CastImage<Int32ImageType, FloatImageType>(){
  return this->SimpleCastImage<Int32ImageType, FloatImageType>();
}

template <> DoubleImageType *
InputImageParameter::CastImage<Int32ImageType, DoubleImageType>(){
  return this->SimpleCastImage<Int32ImageType, DoubleImageType>();
}


/////////// UInt32ImageType->...
template <> Int8ImageType *
InputImageParameter::CastImage<UInt32ImageType, Int8ImageType>(){
  return this->SimpleCastImage<UInt32ImageType, Int8ImageType>();
}

template <> UInt8ImageType *
InputImageParameter::CastImage<UInt32ImageType, UInt8ImageType>(){
  return this->SimpleCastImage<UInt32ImageType, UInt8ImageType>();
}

template <>  Int16ImageType*
InputImageParameter::CastImage<UInt32ImageType, Int16ImageType>(){
  return this->SimpleCastImage<UInt32ImageType, Int16ImageType>();
}

template <> UInt16ImageType*
InputImageParameter::CastImage<UInt32ImageType, UInt16ImageType>(){
  return this->SimpleCastImage<UInt32ImageType, UInt16ImageType>();
}

template <>  Int32ImageType*
InputImageParameter::CastImage<UInt32ImageType, Int32ImageType>(){
  return this->SimpleCastImage<UInt32ImageType, Int32ImageType>();
}


template <>  UInt32ImageType*
InputImageParameter::CastImage<UInt32ImageType, UInt32ImageType>(){
  return this->SimpleCastImage<UInt32ImageType, UInt32ImageType>();
}

template <> FloatImageType *
InputImageParameter::CastImage<UInt32ImageType, FloatImageType>(){
  return this->SimpleCastImage<UInt32ImageType, FloatImageType>();
}

template <> DoubleImageType *
InputImageParameter::CastImage<UInt32ImageType, DoubleImageType>(){
  return this->SimpleCastImage<UInt32ImageType, DoubleImageType>();
}

/////////// FloatImageType->...
template <> Int8ImageType *
InputImageParameter::CastImage<FloatImageType, Int8ImageType>(){
  return this->SimpleCastImage<FloatImageType, Int8ImageType>();
}

template <> UInt8ImageType *
InputImageParameter::CastImage<FloatImageType, UInt8ImageType>(){
  return this->SimpleCastImage<FloatImageType, UInt8ImageType>();
}

template <>  Int16ImageType*
InputImageParameter::CastImage<FloatImageType, Int16ImageType>(){
  return this->SimpleCastImage<FloatImageType, Int16ImageType>();
}

template <> UInt16ImageType*
InputImageParameter::CastImage<FloatImageType, UInt16ImageType>(){
  return this->SimpleCastImage<FloatImageType, UInt16ImageType>();
}

template <>  Int32ImageType*
InputImageParameter::CastImage<FloatImageType, Int32ImageType>(){
  return this->SimpleCastImage<FloatImageType, Int32ImageType>();
}


template <>  UInt32ImageType*
InputImageParameter::CastImage<FloatImageType, UInt32ImageType>(){
  return this->SimpleCastImage<FloatImageType, UInt32ImageType>();
}

template <> FloatImageType *
InputImageParameter::CastImage<FloatImageType, FloatImageType>(){
  return this->SimpleCastImage<FloatImageType, FloatImageType>();
}

template <> DoubleImageType *
InputImageParameter::CastImage<FloatImageType, DoubleImageType>(){
  return this->SimpleCastImage<FloatImageType, DoubleImageType>();
}

/////////// DoubleImageType->...
template <> Int8ImageType *
InputImageParameter::CastImage<DoubleImageType, Int8ImageType>(){
  return this->SimpleCastImage<DoubleImageType, Int8ImageType>();
}

template <> UInt8ImageType *
InputImageParameter::CastImage<DoubleImageType, UInt8ImageType>(){
  return this->SimpleCastImage<DoubleImageType, UInt8ImageType>();
}

template <>  Int16ImageType*
InputImageParameter::CastImage<DoubleImageType, Int16ImageType>(){
  return this->SimpleCastImage<DoubleImageType, Int16ImageType>();
}

template <> UInt16ImageType*
InputImageParameter::CastImage<DoubleImageType, UInt16ImageType>(){
  return this->SimpleCastImage<DoubleImageType, UInt16ImageType>();
}

template <>  Int32ImageType*
InputImageParameter::CastImage<DoubleImageType, Int32ImageType>(){
  return this->SimpleCastImage<DoubleImageType, Int32ImageType>();
}


template <>  UInt32ImageType*
InputImageParameter::CastImage<DoubleImageType, UInt32ImageType>(){
  return this->SimpleCastImage<DoubleImageType, UInt32ImageType>();
}

template <> FloatImageType *
InputImageParameter::CastImage<DoubleImageType, FloatImageType>(){
  return this->SimpleCastImage<DoubleImageType, FloatImageType>();
}

template <> DoubleImageType *
InputImageParameter::CastImage<DoubleImageType, DoubleImageType>(){
  return this->SimpleCastImage<DoubleImageType, DoubleImageType>();
}


/*********************************************************************
********************** VectorImage -> VectorImage
**********************************************************************/
/////////// Int8VectorImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<Int8VectorImageType, Int8VectorImageType>(){
  return this->SimpleCastImage<Int8VectorImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<Int8VectorImageType, UInt8VectorImageType>(){
  return this->SimpleCastImage<Int8VectorImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<Int8VectorImageType, Int16VectorImageType>(){
  return this->SimpleCastImage<Int8VectorImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<Int8VectorImageType, UInt16VectorImageType>(){
  return this->SimpleCastImage<Int8VectorImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<Int8VectorImageType, Int32VectorImageType>(){
  return this->SimpleCastImage<Int8VectorImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<Int8VectorImageType, UInt32VectorImageType>(){
  return this->SimpleCastImage<Int8VectorImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<Int8VectorImageType, FloatVectorImageType>(){
  return this->SimpleCastImage<Int8VectorImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<Int8VectorImageType, DoubleVectorImageType>(){
  return this->SimpleCastImage<Int8VectorImageType, DoubleVectorImageType>();
}


/////////// UInt8VectorImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<UInt8VectorImageType, Int8VectorImageType>(){
  return this->SimpleCastImage<UInt8VectorImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<UInt8VectorImageType, UInt8VectorImageType>(){
  return this->SimpleCastImage<UInt8VectorImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<UInt8VectorImageType, Int16VectorImageType>(){
  return this->SimpleCastImage<UInt8VectorImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<UInt8VectorImageType, UInt16VectorImageType>(){
  return this->SimpleCastImage<UInt8VectorImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<UInt8VectorImageType, Int32VectorImageType>(){
  return this->SimpleCastImage<UInt8VectorImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<UInt8VectorImageType, UInt32VectorImageType>(){
  return this->SimpleCastImage<UInt8VectorImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<UInt8VectorImageType, FloatVectorImageType>(){
  return this->SimpleCastImage<UInt8VectorImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<UInt8VectorImageType, DoubleVectorImageType>(){
  return this->SimpleCastImage<UInt8VectorImageType, DoubleVectorImageType>();
}

/////////// Int16VectorImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<Int16VectorImageType, Int8VectorImageType>(){
  return this->SimpleCastImage<Int16VectorImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<Int16VectorImageType, UInt8VectorImageType>(){
  return this->SimpleCastImage<Int16VectorImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<Int16VectorImageType, Int16VectorImageType>(){
  return this->SimpleCastImage<Int16VectorImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<Int16VectorImageType, UInt16VectorImageType>(){
  return this->SimpleCastImage<Int16VectorImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<Int16VectorImageType, Int32VectorImageType>(){
  return this->SimpleCastImage<Int16VectorImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<Int16VectorImageType, UInt32VectorImageType>(){
  return this->SimpleCastImage<Int16VectorImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<Int16VectorImageType, FloatVectorImageType>(){
  return this->SimpleCastImage<Int16VectorImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<Int16VectorImageType, DoubleVectorImageType>(){
  return this->SimpleCastImage<Int16VectorImageType, DoubleVectorImageType>();
}

/////////// UInt16VectorImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<UInt16VectorImageType, Int8VectorImageType>(){
  return this->SimpleCastImage<UInt16VectorImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<UInt16VectorImageType, UInt8VectorImageType>(){
  return this->SimpleCastImage<UInt16VectorImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<UInt16VectorImageType, Int16VectorImageType>(){
  return this->SimpleCastImage<UInt16VectorImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<UInt16VectorImageType, UInt16VectorImageType>(){
  return this->SimpleCastImage<UInt16VectorImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<UInt16VectorImageType, Int32VectorImageType>(){
  return this->SimpleCastImage<UInt16VectorImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<UInt16VectorImageType, UInt32VectorImageType>(){
  return this->SimpleCastImage<UInt16VectorImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<UInt16VectorImageType, FloatVectorImageType>(){
  return this->SimpleCastImage<UInt16VectorImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<UInt16VectorImageType, DoubleVectorImageType>(){
  return this->SimpleCastImage<UInt16VectorImageType, DoubleVectorImageType>();
}

/////////// Int32VectorImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<Int32VectorImageType, Int8VectorImageType>(){
  return this->SimpleCastImage<Int32VectorImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<Int32VectorImageType, UInt8VectorImageType>(){
  return this->SimpleCastImage<Int32VectorImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<Int32VectorImageType, Int16VectorImageType>(){
  return this->SimpleCastImage<Int32VectorImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<Int32VectorImageType, UInt16VectorImageType>(){
  return this->SimpleCastImage<Int32VectorImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<Int32VectorImageType, Int32VectorImageType>(){
  return this->SimpleCastImage<Int32VectorImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<Int32VectorImageType, UInt32VectorImageType>(){
  return this->SimpleCastImage<Int32VectorImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<Int32VectorImageType, FloatVectorImageType>(){
  return this->SimpleCastImage<Int32VectorImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<Int32VectorImageType, DoubleVectorImageType>(){
  return this->SimpleCastImage<Int32VectorImageType, DoubleVectorImageType>();
}


/////////// UInt32VectorImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<UInt32VectorImageType, Int8VectorImageType>(){
  return this->SimpleCastImage<UInt32VectorImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<UInt32VectorImageType, UInt8VectorImageType>(){
  return this->SimpleCastImage<UInt32VectorImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<UInt32VectorImageType, Int16VectorImageType>(){
  return this->SimpleCastImage<UInt32VectorImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<UInt32VectorImageType, UInt16VectorImageType>(){
  return this->SimpleCastImage<UInt32VectorImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<UInt32VectorImageType, Int32VectorImageType>(){
  return this->SimpleCastImage<UInt32VectorImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<UInt32VectorImageType, UInt32VectorImageType>(){
  return this->SimpleCastImage<UInt32VectorImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<UInt32VectorImageType, FloatVectorImageType>(){
  return this->SimpleCastImage<UInt32VectorImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<UInt32VectorImageType, DoubleVectorImageType>(){
  return this->SimpleCastImage<UInt32VectorImageType, DoubleVectorImageType>();
}

/////////// FloatVectorImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<FloatVectorImageType, Int8VectorImageType>(){
  return this->SimpleCastImage<FloatVectorImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<FloatVectorImageType, UInt8VectorImageType>(){
  return this->SimpleCastImage<FloatVectorImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<FloatVectorImageType, Int16VectorImageType>(){
  return this->SimpleCastImage<FloatVectorImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<FloatVectorImageType, UInt16VectorImageType>(){
  return this->SimpleCastImage<FloatVectorImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<FloatVectorImageType, Int32VectorImageType>(){
  return this->SimpleCastImage<FloatVectorImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<FloatVectorImageType, UInt32VectorImageType>(){
  return this->SimpleCastImage<FloatVectorImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<FloatVectorImageType, FloatVectorImageType>(){
  return this->SimpleCastImage<FloatVectorImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<FloatVectorImageType, DoubleVectorImageType>(){
  return this->SimpleCastImage<FloatVectorImageType, DoubleVectorImageType>();
}

/////////// DoubleVectorImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<DoubleVectorImageType, Int8VectorImageType>(){
  return this->SimpleCastImage<DoubleVectorImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<DoubleVectorImageType, UInt8VectorImageType>(){
  return this->SimpleCastImage<DoubleVectorImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<DoubleVectorImageType, Int16VectorImageType>(){
  return this->SimpleCastImage<DoubleVectorImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<DoubleVectorImageType, UInt16VectorImageType>(){
  return this->SimpleCastImage<DoubleVectorImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<DoubleVectorImageType, Int32VectorImageType>(){
  return this->SimpleCastImage<DoubleVectorImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<DoubleVectorImageType, UInt32VectorImageType>(){
  return this->SimpleCastImage<DoubleVectorImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<DoubleVectorImageType, FloatVectorImageType>(){
  return this->SimpleCastImage<DoubleVectorImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<DoubleVectorImageType, DoubleVectorImageType>(){
  return this->SimpleCastImage<DoubleVectorImageType, DoubleVectorImageType>();
}


/*********************************************************************
********************** RGBAImage -> RGBAImage
**********************************************************************/
/////////// Int8RGBAImageType->...
template <> Int8RGBAImageType *
InputImageParameter::CastImage<Int8RGBAImageType, Int8RGBAImageType>(){
  return this->SimpleCastImage<Int8RGBAImageType, Int8RGBAImageType>();
}

template <> UInt8RGBAImageType *
InputImageParameter::CastImage<Int8RGBAImageType, UInt8RGBAImageType>(){
  return this->SimpleCastImage<Int8RGBAImageType, UInt8RGBAImageType>();
}

template <>  Int16RGBAImageType*
InputImageParameter::CastImage<Int8RGBAImageType, Int16RGBAImageType>(){
  return this->SimpleCastImage<Int8RGBAImageType, Int16RGBAImageType>();
}

template <> UInt16RGBAImageType*
InputImageParameter::CastImage<Int8RGBAImageType, UInt16RGBAImageType>(){
  return this->SimpleCastImage<Int8RGBAImageType, UInt16RGBAImageType>();
}

template <>  Int32RGBAImageType*
InputImageParameter::CastImage<Int8RGBAImageType, Int32RGBAImageType>(){
  return this->SimpleCastImage<Int8RGBAImageType, Int32RGBAImageType>();
}


template <>  UInt32RGBAImageType*
InputImageParameter::CastImage<Int8RGBAImageType, UInt32RGBAImageType>(){
  return this->SimpleCastImage<Int8RGBAImageType, UInt32RGBAImageType>();
}

template <> FloatRGBAImageType *
InputImageParameter::CastImage<Int8RGBAImageType, FloatRGBAImageType>(){
  return this->SimpleCastImage<Int8RGBAImageType, FloatRGBAImageType>();
}

template <> DoubleRGBAImageType *
InputImageParameter::CastImage<Int8RGBAImageType, DoubleRGBAImageType>(){
  return this->SimpleCastImage<Int8RGBAImageType, DoubleRGBAImageType>();
}


/////////// UInt8RGBAImageType->...
template <> Int8RGBAImageType *
InputImageParameter::CastImage<UInt8RGBAImageType, Int8RGBAImageType>(){
  return this->SimpleCastImage<UInt8RGBAImageType, Int8RGBAImageType>();
}

template <> UInt8RGBAImageType *
InputImageParameter::CastImage<UInt8RGBAImageType, UInt8RGBAImageType>(){
  return this->SimpleCastImage<UInt8RGBAImageType, UInt8RGBAImageType>();
}

template <>  Int16RGBAImageType*
InputImageParameter::CastImage<UInt8RGBAImageType, Int16RGBAImageType>(){
  return this->SimpleCastImage<UInt8RGBAImageType, Int16RGBAImageType>();
}

template <> UInt16RGBAImageType*
InputImageParameter::CastImage<UInt8RGBAImageType, UInt16RGBAImageType>(){
  return this->SimpleCastImage<UInt8RGBAImageType, UInt16RGBAImageType>();
}

template <>  Int32RGBAImageType*
InputImageParameter::CastImage<UInt8RGBAImageType, Int32RGBAImageType>(){
  return this->SimpleCastImage<UInt8RGBAImageType, Int32RGBAImageType>();
}


template <>  UInt32RGBAImageType*
InputImageParameter::CastImage<UInt8RGBAImageType, UInt32RGBAImageType>(){
  return this->SimpleCastImage<UInt8RGBAImageType, UInt32RGBAImageType>();
}

template <> FloatRGBAImageType *
InputImageParameter::CastImage<UInt8RGBAImageType, FloatRGBAImageType>(){
  return this->SimpleCastImage<UInt8RGBAImageType, FloatRGBAImageType>();
}

template <> DoubleRGBAImageType *
InputImageParameter::CastImage<UInt8RGBAImageType, DoubleRGBAImageType>(){
  return this->SimpleCastImage<UInt8RGBAImageType, DoubleRGBAImageType>();
}

/////////// Int16RGBAImageType->...
template <> Int8RGBAImageType *
InputImageParameter::CastImage<Int16RGBAImageType, Int8RGBAImageType>(){
  return this->SimpleCastImage<Int16RGBAImageType, Int8RGBAImageType>();
}

template <> UInt8RGBAImageType *
InputImageParameter::CastImage<Int16RGBAImageType, UInt8RGBAImageType>(){
  return this->SimpleCastImage<Int16RGBAImageType, UInt8RGBAImageType>();
}

template <>  Int16RGBAImageType*
InputImageParameter::CastImage<Int16RGBAImageType, Int16RGBAImageType>(){
  return this->SimpleCastImage<Int16RGBAImageType, Int16RGBAImageType>();
}

template <> UInt16RGBAImageType*
InputImageParameter::CastImage<Int16RGBAImageType, UInt16RGBAImageType>(){
  return this->SimpleCastImage<Int16RGBAImageType, UInt16RGBAImageType>();
}

template <>  Int32RGBAImageType*
InputImageParameter::CastImage<Int16RGBAImageType, Int32RGBAImageType>(){
  return this->SimpleCastImage<Int16RGBAImageType, Int32RGBAImageType>();
}


template <>  UInt32RGBAImageType*
InputImageParameter::CastImage<Int16RGBAImageType, UInt32RGBAImageType>(){
  return this->SimpleCastImage<Int16RGBAImageType, UInt32RGBAImageType>();
}

template <> FloatRGBAImageType *
InputImageParameter::CastImage<Int16RGBAImageType, FloatRGBAImageType>(){
  return this->SimpleCastImage<Int16RGBAImageType, FloatRGBAImageType>();
}

template <> DoubleRGBAImageType *
InputImageParameter::CastImage<Int16RGBAImageType, DoubleRGBAImageType>(){
  return this->SimpleCastImage<Int16RGBAImageType, DoubleRGBAImageType>();
}

/////////// UInt16RGBAImageType->...
template <> Int8RGBAImageType *
InputImageParameter::CastImage<UInt16RGBAImageType, Int8RGBAImageType>(){
  return this->SimpleCastImage<UInt16RGBAImageType, Int8RGBAImageType>();
}

template <> UInt8RGBAImageType *
InputImageParameter::CastImage<UInt16RGBAImageType, UInt8RGBAImageType>(){
  return this->SimpleCastImage<UInt16RGBAImageType, UInt8RGBAImageType>();
}

template <>  Int16RGBAImageType*
InputImageParameter::CastImage<UInt16RGBAImageType, Int16RGBAImageType>(){
  return this->SimpleCastImage<UInt16RGBAImageType, Int16RGBAImageType>();
}

template <> UInt16RGBAImageType*
InputImageParameter::CastImage<UInt16RGBAImageType, UInt16RGBAImageType>(){
  return this->SimpleCastImage<UInt16RGBAImageType, UInt16RGBAImageType>();
}

template <>  Int32RGBAImageType*
InputImageParameter::CastImage<UInt16RGBAImageType, Int32RGBAImageType>(){
  return this->SimpleCastImage<UInt16RGBAImageType, Int32RGBAImageType>();
}


template <>  UInt32RGBAImageType*
InputImageParameter::CastImage<UInt16RGBAImageType, UInt32RGBAImageType>(){
  return this->SimpleCastImage<UInt16RGBAImageType, UInt32RGBAImageType>();
}

template <> FloatRGBAImageType *
InputImageParameter::CastImage<UInt16RGBAImageType, FloatRGBAImageType>(){
  return this->SimpleCastImage<UInt16RGBAImageType, FloatRGBAImageType>();
}

template <> DoubleRGBAImageType *
InputImageParameter::CastImage<UInt16RGBAImageType, DoubleRGBAImageType>(){
  return this->SimpleCastImage<UInt16RGBAImageType, DoubleRGBAImageType>();
}

/////////// Int32RGBAImageType->...
template <> Int8RGBAImageType *
InputImageParameter::CastImage<Int32RGBAImageType, Int8RGBAImageType>(){
  return this->SimpleCastImage<Int32RGBAImageType, Int8RGBAImageType>();
}

template <> UInt8RGBAImageType *
InputImageParameter::CastImage<Int32RGBAImageType, UInt8RGBAImageType>(){
  return this->SimpleCastImage<Int32RGBAImageType, UInt8RGBAImageType>();
}

template <>  Int16RGBAImageType*
InputImageParameter::CastImage<Int32RGBAImageType, Int16RGBAImageType>(){
  return this->SimpleCastImage<Int32RGBAImageType, Int16RGBAImageType>();
}

template <> UInt16RGBAImageType*
InputImageParameter::CastImage<Int32RGBAImageType, UInt16RGBAImageType>(){
  return this->SimpleCastImage<Int32RGBAImageType, UInt16RGBAImageType>();
}

template <>  Int32RGBAImageType*
InputImageParameter::CastImage<Int32RGBAImageType, Int32RGBAImageType>(){
  return this->SimpleCastImage<Int32RGBAImageType, Int32RGBAImageType>();
}


template <>  UInt32RGBAImageType*
InputImageParameter::CastImage<Int32RGBAImageType, UInt32RGBAImageType>(){
  return this->SimpleCastImage<Int32RGBAImageType, UInt32RGBAImageType>();
}

template <> FloatRGBAImageType *
InputImageParameter::CastImage<Int32RGBAImageType, FloatRGBAImageType>(){
  return this->SimpleCastImage<Int32RGBAImageType, FloatRGBAImageType>();
}

template <> DoubleRGBAImageType *
InputImageParameter::CastImage<Int32RGBAImageType, DoubleRGBAImageType>(){
  return this->SimpleCastImage<Int32RGBAImageType, DoubleRGBAImageType>();
}


/////////// UInt32RGBAImageType->...
template <> Int8RGBAImageType *
InputImageParameter::CastImage<UInt32RGBAImageType, Int8RGBAImageType>(){
  return this->SimpleCastImage<UInt32RGBAImageType, Int8RGBAImageType>();
}

template <> UInt8RGBAImageType *
InputImageParameter::CastImage<UInt32RGBAImageType, UInt8RGBAImageType>(){
  return this->SimpleCastImage<UInt32RGBAImageType, UInt8RGBAImageType>();
}

template <>  Int16RGBAImageType*
InputImageParameter::CastImage<UInt32RGBAImageType, Int16RGBAImageType>(){
  return this->SimpleCastImage<UInt32RGBAImageType, Int16RGBAImageType>();
}

template <> UInt16RGBAImageType*
InputImageParameter::CastImage<UInt32RGBAImageType, UInt16RGBAImageType>(){
  return this->SimpleCastImage<UInt32RGBAImageType, UInt16RGBAImageType>();
}

template <>  Int32RGBAImageType*
InputImageParameter::CastImage<UInt32RGBAImageType, Int32RGBAImageType>(){
  return this->SimpleCastImage<UInt32RGBAImageType, Int32RGBAImageType>();
}


template <>  UInt32RGBAImageType*
InputImageParameter::CastImage<UInt32RGBAImageType, UInt32RGBAImageType>(){
  return this->SimpleCastImage<UInt32RGBAImageType, UInt32RGBAImageType>();
}

template <> FloatRGBAImageType *
InputImageParameter::CastImage<UInt32RGBAImageType, FloatRGBAImageType>(){
  return this->SimpleCastImage<UInt32RGBAImageType, FloatRGBAImageType>();
}

template <> DoubleRGBAImageType *
InputImageParameter::CastImage<UInt32RGBAImageType, DoubleRGBAImageType>(){
  return this->SimpleCastImage<UInt32RGBAImageType, DoubleRGBAImageType>();
}

/////////// FloatRGBAImageType->...
template <> Int8RGBAImageType *
InputImageParameter::CastImage<FloatRGBAImageType, Int8RGBAImageType>(){
  return this->SimpleCastImage<FloatRGBAImageType, Int8RGBAImageType>();
}

template <> UInt8RGBAImageType *
InputImageParameter::CastImage<FloatRGBAImageType, UInt8RGBAImageType>(){
  return this->SimpleCastImage<FloatRGBAImageType, UInt8RGBAImageType>();
}

template <>  Int16RGBAImageType*
InputImageParameter::CastImage<FloatRGBAImageType, Int16RGBAImageType>(){
  return this->SimpleCastImage<FloatRGBAImageType, Int16RGBAImageType>();
}

template <> UInt16RGBAImageType*
InputImageParameter::CastImage<FloatRGBAImageType, UInt16RGBAImageType>(){
  return this->SimpleCastImage<FloatRGBAImageType, UInt16RGBAImageType>();
}

template <>  Int32RGBAImageType*
InputImageParameter::CastImage<FloatRGBAImageType, Int32RGBAImageType>(){
  return this->SimpleCastImage<FloatRGBAImageType, Int32RGBAImageType>();
}


template <>  UInt32RGBAImageType*
InputImageParameter::CastImage<FloatRGBAImageType, UInt32RGBAImageType>(){
  return this->SimpleCastImage<FloatRGBAImageType, UInt32RGBAImageType>();
}

template <> FloatRGBAImageType *
InputImageParameter::CastImage<FloatRGBAImageType, FloatRGBAImageType>(){
  return this->SimpleCastImage<FloatRGBAImageType, FloatRGBAImageType>();
}

template <> DoubleRGBAImageType *
InputImageParameter::CastImage<FloatRGBAImageType, DoubleRGBAImageType>(){
  return this->SimpleCastImage<FloatRGBAImageType, DoubleRGBAImageType>();
}

/////////// DoubleRGBAImageType->...
template <> Int8RGBAImageType *
InputImageParameter::CastImage<DoubleRGBAImageType, Int8RGBAImageType>(){
  return this->SimpleCastImage<DoubleRGBAImageType, Int8RGBAImageType>();
}

template <> UInt8RGBAImageType *
InputImageParameter::CastImage<DoubleRGBAImageType, UInt8RGBAImageType>(){
  return this->SimpleCastImage<DoubleRGBAImageType, UInt8RGBAImageType>();
}

template <>  Int16RGBAImageType*
InputImageParameter::CastImage<DoubleRGBAImageType, Int16RGBAImageType>(){
  return this->SimpleCastImage<DoubleRGBAImageType, Int16RGBAImageType>();
}

template <> UInt16RGBAImageType*
InputImageParameter::CastImage<DoubleRGBAImageType, UInt16RGBAImageType>(){
  return this->SimpleCastImage<DoubleRGBAImageType, UInt16RGBAImageType>();
}

template <>  Int32RGBAImageType*
InputImageParameter::CastImage<DoubleRGBAImageType, Int32RGBAImageType>(){
  return this->SimpleCastImage<DoubleRGBAImageType, Int32RGBAImageType>();
}


template <>  UInt32RGBAImageType*
InputImageParameter::CastImage<DoubleRGBAImageType, UInt32RGBAImageType>(){
  return this->SimpleCastImage<DoubleRGBAImageType, UInt32RGBAImageType>();
}

template <> FloatRGBAImageType *
InputImageParameter::CastImage<DoubleRGBAImageType, FloatRGBAImageType>(){
  return this->SimpleCastImage<DoubleRGBAImageType, FloatRGBAImageType>();
}

template <> DoubleRGBAImageType *
InputImageParameter::CastImage<DoubleRGBAImageType, DoubleRGBAImageType>(){
  return this->SimpleCastImage<DoubleRGBAImageType, DoubleRGBAImageType>();
}


/*********************************************************************
********************** Image -> VectorImage
**********************************************************************/
/////////// Int8ImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<Int8ImageType, Int8VectorImageType>(){
  return this->CastVectorImageFromImage<Int8ImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<Int8ImageType, UInt8VectorImageType>(){
  return this->CastVectorImageFromImage<Int8ImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<Int8ImageType, Int16VectorImageType>(){
  return this->CastVectorImageFromImage<Int8ImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<Int8ImageType, UInt16VectorImageType>(){
  return this->CastVectorImageFromImage<Int8ImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<Int8ImageType, Int32VectorImageType>(){
  return this->CastVectorImageFromImage<Int8ImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<Int8ImageType, UInt32VectorImageType>(){
  return this->CastVectorImageFromImage<Int8ImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<Int8ImageType, FloatVectorImageType>(){
  return this->CastVectorImageFromImage<Int8ImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<Int8ImageType, DoubleVectorImageType>(){
  return this->CastVectorImageFromImage<Int8ImageType, DoubleVectorImageType>();
}


/////////// UInt8ImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<UInt8ImageType, Int8VectorImageType>(){
  return this->CastVectorImageFromImage<UInt8ImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<UInt8ImageType, UInt8VectorImageType>(){
  return this->CastVectorImageFromImage<UInt8ImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<UInt8ImageType, Int16VectorImageType>(){
  return this->CastVectorImageFromImage<UInt8ImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<UInt8ImageType, UInt16VectorImageType>(){
  return this->CastVectorImageFromImage<UInt8ImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<UInt8ImageType, Int32VectorImageType>(){
  return this->CastVectorImageFromImage<UInt8ImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<UInt8ImageType, UInt32VectorImageType>(){
  return this->CastVectorImageFromImage<UInt8ImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<UInt8ImageType, FloatVectorImageType>(){
  return this->CastVectorImageFromImage<UInt8ImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<UInt8ImageType, DoubleVectorImageType>(){
  return this->CastVectorImageFromImage<UInt8ImageType, DoubleVectorImageType>();
}

/////////// Int16ImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<Int16ImageType, Int8VectorImageType>(){
  return this->CastVectorImageFromImage<Int16ImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<Int16ImageType, UInt8VectorImageType>(){
  return this->CastVectorImageFromImage<Int16ImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<Int16ImageType, Int16VectorImageType>(){
  return this->CastVectorImageFromImage<Int16ImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<Int16ImageType, UInt16VectorImageType>(){
  return this->CastVectorImageFromImage<Int16ImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<Int16ImageType, Int32VectorImageType>(){
  return this->CastVectorImageFromImage<Int16ImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<Int16ImageType, UInt32VectorImageType>(){
  return this->CastVectorImageFromImage<Int16ImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<Int16ImageType, FloatVectorImageType>(){
  return this->CastVectorImageFromImage<Int16ImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<Int16ImageType, DoubleVectorImageType>(){
  return this->CastVectorImageFromImage<Int16ImageType, DoubleVectorImageType>();
}

/////////// UInt16VectorImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<UInt16ImageType, Int8VectorImageType>(){
  return this->CastVectorImageFromImage<UInt16ImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<UInt16ImageType, UInt8VectorImageType>(){
  return this->CastVectorImageFromImage<UInt16ImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<UInt16ImageType, Int16VectorImageType>(){
  return this->CastVectorImageFromImage<UInt16ImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<UInt16ImageType, UInt16VectorImageType>(){
  return this->CastVectorImageFromImage<UInt16ImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<UInt16ImageType, Int32VectorImageType>(){
  return this->CastVectorImageFromImage<UInt16ImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<UInt16ImageType, UInt32VectorImageType>(){
  return this->CastVectorImageFromImage<UInt16ImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<UInt16ImageType, FloatVectorImageType>(){
  return this->CastVectorImageFromImage<UInt16ImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<UInt16ImageType, DoubleVectorImageType>(){
  return this->CastVectorImageFromImage<UInt16ImageType, DoubleVectorImageType>();
}

/////////// Int32ImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<Int32ImageType, Int8VectorImageType>(){
  return this->CastVectorImageFromImage<Int32ImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<Int32ImageType, UInt8VectorImageType>(){
  return this->CastVectorImageFromImage<Int32ImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<Int32ImageType, Int16VectorImageType>(){
  return this->CastVectorImageFromImage<Int32ImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<Int32ImageType, UInt16VectorImageType>(){
  return this->CastVectorImageFromImage<Int32ImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<Int32ImageType, Int32VectorImageType>(){
  return this->CastVectorImageFromImage<Int32ImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<Int32ImageType, UInt32VectorImageType>(){
  return this->CastVectorImageFromImage<Int32ImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<Int32ImageType, FloatVectorImageType>(){
  return this->CastVectorImageFromImage<Int32ImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<Int32ImageType, DoubleVectorImageType>(){
  return this->CastVectorImageFromImage<Int32ImageType, DoubleVectorImageType>();
}


/////////// UInt32ImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<UInt32ImageType, Int8VectorImageType>(){
  return this->CastVectorImageFromImage<UInt32ImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<UInt32ImageType, UInt8VectorImageType>(){
  return this->CastVectorImageFromImage<UInt32ImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<UInt32ImageType, Int16VectorImageType>(){
  return this->CastVectorImageFromImage<UInt32ImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<UInt32ImageType, UInt16VectorImageType>(){
  return this->CastVectorImageFromImage<UInt32ImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<UInt32ImageType, Int32VectorImageType>(){
  return this->CastVectorImageFromImage<UInt32ImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<UInt32ImageType, UInt32VectorImageType>(){
  return this->CastVectorImageFromImage<UInt32ImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<UInt32ImageType, FloatVectorImageType>(){
  return this->CastVectorImageFromImage<UInt32ImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<UInt32ImageType, DoubleVectorImageType>(){
  return this->CastVectorImageFromImage<UInt32ImageType, DoubleVectorImageType>();
}

/////////// FloatImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<FloatImageType, Int8VectorImageType>(){
  return this->CastVectorImageFromImage<FloatImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<FloatImageType, UInt8VectorImageType>(){
  return this->CastVectorImageFromImage<FloatImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<FloatImageType, Int16VectorImageType>(){
  return this->CastVectorImageFromImage<FloatImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<FloatImageType, UInt16VectorImageType>(){
  return this->CastVectorImageFromImage<FloatImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<FloatImageType, Int32VectorImageType>(){
  return this->CastVectorImageFromImage<FloatImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<FloatImageType, UInt32VectorImageType>(){
  return this->CastVectorImageFromImage<FloatImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<FloatImageType, FloatVectorImageType>(){
  return this->CastVectorImageFromImage<FloatImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<FloatImageType, DoubleVectorImageType>(){
  return this->CastVectorImageFromImage<FloatImageType, DoubleVectorImageType>();
}

/////////// DoubleImageType->...
template <> Int8VectorImageType *
InputImageParameter::CastImage<DoubleImageType, Int8VectorImageType>(){
  return this->CastVectorImageFromImage<DoubleImageType, Int8VectorImageType>();
}

template <> UInt8VectorImageType *
InputImageParameter::CastImage<DoubleImageType, UInt8VectorImageType>(){
  return this->CastVectorImageFromImage<DoubleImageType, UInt8VectorImageType>();
}

template <>  Int16VectorImageType*
InputImageParameter::CastImage<DoubleImageType, Int16VectorImageType>(){
  return this->CastVectorImageFromImage<DoubleImageType, Int16VectorImageType>();
}

template <> UInt16VectorImageType*
InputImageParameter::CastImage<DoubleImageType, UInt16VectorImageType>(){
  return this->CastVectorImageFromImage<DoubleImageType, UInt16VectorImageType>();
}

template <>  Int32VectorImageType*
InputImageParameter::CastImage<DoubleImageType, Int32VectorImageType>(){
  return this->CastVectorImageFromImage<DoubleImageType, Int32VectorImageType>();
}


template <>  UInt32VectorImageType*
InputImageParameter::CastImage<DoubleImageType, UInt32VectorImageType>(){
  return this->CastVectorImageFromImage<DoubleImageType, UInt32VectorImageType>();
}

template <> FloatVectorImageType *
InputImageParameter::CastImage<DoubleImageType, FloatVectorImageType>(){
  return this->CastVectorImageFromImage<DoubleImageType, FloatVectorImageType>();
}

template <> DoubleVectorImageType *
InputImageParameter::CastImage<DoubleImageType, DoubleVectorImageType>(){
  return this->CastVectorImageFromImage<DoubleImageType, DoubleVectorImageType>();
}


void
InputImageParameter::SetImage(FloatVectorImageType* image)
{
  this->SetImage<FloatVectorImageType>( image );
}


template <class TInputImage>
void
InputImageParameter::SetImage(TInputImage* image)
{
  m_Image = image;
}


bool
InputImageParameter::HasValue() const
{
  if( m_FileName.empty() && m_Image.IsNull() )
    return false;
  else
    return true;
}

void
InputImageParameter::ClearValue()
{
 m_Image   = NULL;
 m_Reader = NULL;
 m_Caster = NULL;
}


}
}

