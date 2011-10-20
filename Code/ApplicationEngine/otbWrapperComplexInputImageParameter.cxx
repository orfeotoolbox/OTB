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
#include "otbWrapperComplexInputImageParameter.h"
#include "itksys/SystemTools.hxx"
#include "otbImageFileReader.h"
#include "itkCastImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbWrapperTypes.h"

namespace otb
{
namespace Wrapper
{

ComplexInputImageParameter::ComplexInputImageParameter()
{
  this->SetName("Complex Input Image");
  this->SetKey("cin");
  m_FileName="";
  this->ClearValue();
}

ComplexInputImageParameter::~ComplexInputImageParameter()
{
}

void
ComplexInputImageParameter::SetFromFileName(const std::string& filename)
{
  // First clear previous file choosen
  this->ClearValue();

  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  if (!filename.empty()
      && itksys::SystemTools::FileExists(filename.c_str()))
    {
    ComplexFloatVectorReaderType::Pointer reader = ComplexFloatVectorReaderType::New();
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


ComplexFloatVectorImageType*
ComplexInputImageParameter::GetImage()
{
  return this->GetImage<ComplexFloatVectorImageType>();
}


#define otbGetImageMacro(image)                       \
  image##Type *                                       \
  ComplexInputImageParameter::Get##image ()           \
  {                                                   \
    return this->GetImage< image##Type > ();          \
  }

otbGetImageMacro(ComplexFloatImage);
otbGetImageMacro(ComplexDoubleImage);

otbGetImageMacro(ComplexFloatVectorImage);
otbGetImageMacro(ComplexDoubleVectorImage);

template <class TOutputImage>
TOutputImage *
ComplexInputImageParameter::GetImage()
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
      if (dynamic_cast<ComplexFloatVectorImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexFloatVectorImageType, TOutputImage>();
        }
      else if (dynamic_cast<ComplexDoubleVectorImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexDoubleVectorImageType, TOutputImage>();
        }
      else if (dynamic_cast<ComplexFloatImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexFloatImageType, TOutputImage>();
        }
      else if (dynamic_cast<ComplexDoubleImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexDoubleImageType, TOutputImage>();
        }
      else
        {
        itkExceptionMacro("Unknown image type");
        }
      }
    }
}


template <class TComplexInputImage, class TOutputImage>
TOutputImage*
ComplexInputImageParameter::SimpleCastImage()
{
  TComplexInputImage* realComplexInputImage = dynamic_cast<TComplexInputImage*>(m_Image.GetPointer());
  
  typedef itk::CastImageFilter<TComplexInputImage, TOutputImage> CasterType;
  typename CasterType::Pointer caster = CasterType::New();
  
  caster->SetInput(realComplexInputImage);
  caster->UpdateOutputInformation();
  
  m_Image = caster->GetOutput();
  m_Caster = caster;
  
  return caster->GetOutput();
}
  
                       
template <class TComplexInputImage, class TOutputImage>
TOutputImage*
ComplexInputImageParameter::CastVectorImageFromImage()
{
  TComplexInputImage* realComplexInputImage = dynamic_cast<TComplexInputImage*>(m_Image.GetPointer());
  
  typedef ImageToVectorImageCastFilter<TComplexInputImage, TOutputImage> CasterType;
  typename CasterType::Pointer caster = CasterType::New();
  
  caster->SetInput(realComplexInputImage);
  caster->UpdateOutputInformation();
  
  m_Image = caster->GetOutput();
  m_Caster = caster;
  
  return caster->GetOutput();
}


#define otbCastImageMacro(ComplexInputImageType, OutputImageType, theMethod)   \
  template<> OutputImageType *                                          \
  ComplexInputImageParameter::CastImage<ComplexInputImageType , OutputImageType>()    \
  {                                                                     \
    return this->theMethod<ComplexInputImageType , OutputImageType>();         \
  }

#define otbGenericCastImageMacro(ComplexInputImageType, theMethod, prefix)     \
  otbCastImageMacro(ComplexInputImageType, ComplexFloat##prefix##ImageType, theMethod) \
  otbCastImageMacro(ComplexInputImageType, ComplexDouble##prefix##ImageType, theMethod)


/*********************************************************************
********************** Image -> Image
**********************************************************************/

  otbGenericCastImageMacro(ComplexFloatImageType, SimpleCastImage, )
  otbGenericCastImageMacro(ComplexDoubleImageType, SimpleCastImage, )


/*********************************************************************
********************** VectorImage -> VectorImage
**********************************************************************/
  otbGenericCastImageMacro(ComplexFloatVectorImageType, SimpleCastImage, Vector)
  otbGenericCastImageMacro(ComplexDoubleVectorImageType, SimpleCastImage, Vector)

  void
ComplexInputImageParameter::SetImage(ComplexFloatVectorImageType* image)
{
  this->SetImage<ComplexFloatVectorImageType>( image );
}


template <class TComplexInputImage>
void
ComplexInputImageParameter::SetImage(TComplexInputImage* image)
{
  m_Image = image;
}


bool
ComplexInputImageParameter::HasValue() const
{
  if( m_FileName.empty() && m_Image.IsNull() )
    return false;
  else
    return true;
}

void
ComplexInputImageParameter::ClearValue()
{
  m_Image   = NULL;
  m_Reader = NULL;
  m_Caster = NULL;
}


}
}

