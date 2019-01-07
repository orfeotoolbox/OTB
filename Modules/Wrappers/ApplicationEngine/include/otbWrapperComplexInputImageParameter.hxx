/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperComplexInputImageParameter_hxx
#define otbWrapperComplexInputImageParameter_hxx

#include "otbWrapperComplexInputImageParameter.h"
#include "otbClampImageFilter.h"

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
      typedef otb::ImageFileReader<TOutputImage> ReaderType;
      typename ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(m_FileName);
      reader->UpdateOutputInformation();

      m_Image = reader->GetOutput();
      m_Reader = reader;
      m_PreviousFileName = m_FileName;

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
      if (dynamic_cast<ComplexInt16VectorImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexInt16VectorImageType, TOutputImage>();
        }
      else if (dynamic_cast<ComplexInt32VectorImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexInt32VectorImageType, TOutputImage>();
        }
      else if (dynamic_cast<ComplexFloatVectorImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexFloatVectorImageType, TOutputImage>();
        }
      else if (dynamic_cast<ComplexDoubleVectorImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexDoubleVectorImageType, TOutputImage>();
        }
      else if (dynamic_cast<ComplexInt16ImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexInt16ImageType, TOutputImage>();
        }
      else if (dynamic_cast<ComplexInt32ImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexInt32ImageType, TOutputImage>();
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

/** declare a specialization for ImageBaseType */
template <>
ImageBaseType*
ComplexInputImageParameter::GetImage();

template <class TComplexInputImage, class TOutputImage>
TOutputImage*
ComplexInputImageParameter::CastImage()
{
  TComplexInputImage* realComplexInputImage = dynamic_cast<TComplexInputImage*>(m_Image.GetPointer());

  typedef ClampImageFilter<TComplexInputImage, TOutputImage> CasterType;
  typename CasterType::Pointer caster = CasterType::New();

  caster->SetInput(realComplexInputImage);
  caster->UpdateOutputInformation();

  m_Image = caster->GetOutput();
  m_Caster = caster;

  return caster->GetOutput();
  // itkExceptionMacro("Cast from "<<typeid(TComplexInputImage).name()
  //                   <<" to "<<typeid(TOutputImage).name()<<" not authorized.");
}

template <class TComplexInputImage>
void
ComplexInputImageParameter::SetImage(TComplexInputImage* image)
{
  m_UseFilename = false;
  m_Image = image;
}



} // End namespace Wrapper
} // End namespace otb

#endif
