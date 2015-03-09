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
#ifndef __otbWrapperComplexInputImageParameter_txx
#define __otbWrapperComplexInputImageParameter_txx

#include "otbWrapperComplexInputImageParameter.h"

#include "itkCastImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"

namespace otb
{
namespace Wrapper
{


template <class TOutputImage>
TOutputImage *
ComplexInputImageParameter::GetImage()
{
  // Used m_PreviousFileName because if not, when the user call twice GetImage,
  // it without changing the filename, it returns 2 different
  // image pointers
  // Only one image type can be used

  // 2 cases : the user set a filename vs. the user set an image
  if (m_UseFilename)
    {
    if( m_PreviousFileName!=m_FileName && !m_FileName.empty() )
      {
      //////////////////////// Filename case:
      // A new valid filename has been given : a reader is created
      m_PreviousFileName = m_FileName;
      typedef otb::ImageFileReader<TOutputImage> ReaderType;
      typename ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(m_FileName);
      try
        {
        reader->UpdateOutputInformation();
        }
      catch(itk::ExceptionObject &)
        {
        this->ClearValue();
        }

      m_Image = reader->GetOutput();
      m_Reader = reader;

      // Pay attention, don't return m_Image because it is a ImageBase...
      return reader->GetOutput();
      }
    else
      {
      // In this case, the reader and the image should already be there
      if (m_Image.IsNull())
        {
        itkExceptionMacro("No input image or filename detected...");
        }
      else
        {
        // Check if the image type asked here is the same as the one used for the reader
        if (dynamic_cast<TOutputImage*> (m_Image.GetPointer()))
          {
          return dynamic_cast<TOutputImage*> (m_Image.GetPointer());
          }
        else
          {
          itkExceptionMacro("Cannot ask a different image type");
          }
        }
      }
    }
  else
    {
    //////////////////////// Image case:
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
ComplexInputImageParameter::CastImage()
{
  itkExceptionMacro("Cast from "<<typeid(TComplexInputImage).name()
                    <<" to "<<typeid(TOutputImage).name()<<" not authorized.");
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


} // End namespace Wrapper
} // End namespace otb

#endif
