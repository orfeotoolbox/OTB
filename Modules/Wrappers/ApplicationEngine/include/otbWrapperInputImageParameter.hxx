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

#ifndef otbWrapperInputImageParameter_hxx
#define otbWrapperInputImageParameter_hxx

#include "otbWrapperInputImageParameter.h"

#include "itkUnaryFunctorImageFilter.h"
#include "otbClampImageFilter.h"

namespace otb
{
namespace Wrapper
{


#define INPUT_IMAGE_PARAMETER_GET_IMAGE_EXCEPTION 0

template <class TImageType>
TImageType*
InputImageParameter::GetImage()
{
  // Used m_PreviousFileName because if not, when the user call twice GetImage,
  // it without changing the filename, it returns 2 different
  // image pointers
  // Only one image type can be used

  // 2 cases : the user set a filename vs. the user set an image
  if( m_UseFilename )
    {
    if( m_PreviousFileName!=m_FileName &&
	!m_FileName.empty() )
      {
      //////////////////////// Filename case:
      // A new valid filename has been given : a reader is created
      typedef otb::ImageFileReader<TImageType> ReaderType;

      typename ReaderType::Pointer reader = ReaderType::New();

      reader->SetFileName( m_FileName );

      reader->UpdateOutputInformation();

      m_Image = reader->GetOutput();
      m_Reader = reader;

      m_PreviousFileName = m_FileName;

      // Pay attention, don't return m_Image because it is a ImageBase...
      return reader->GetOutput();
      }
    else
      {
#if INPUT_IMAGE_PARAMETER_GET_IMAGE_EXCEPTION
      return dynamic_cast< TImageType* >( m_Image.GetPointer() );

#else // INPUT_IMAGE_PARAMETER_GET_IMAGE_EXCEPTION
      // In this case, the reader and the image should already be there
      if (m_Image.IsNull())
        {
        itkExceptionMacro("No input image or filename detected...");
        }
      else
        {
        // Check if the image type asked here is the same as the one used for the reader
        if (dynamic_cast<TImageType*> (m_Image.GetPointer()))
          {
          return dynamic_cast<TImageType*> (m_Image.GetPointer());
          }
        else
          {
          itkExceptionMacro("Cannot ask a different image type");
          }
        }
#endif // INPUT_IMAGE_PARAMETER_GET_IMAGE_EXCEPTION
      }
    }

  else
    {
    //////////////////////// Image case:
    if (m_Image.IsNull())
      {
#if INPUT_IMAGE_PARAMETER_GET_IMAGE_EXCEPTION
      itkExceptionMacro("No input image or filename detected...");
#else
      return nullptr;
#endif
      }
    else
      {
      if (dynamic_cast<UInt8ImageType*> (m_Image.GetPointer()))
        {
        return CastImage<UInt8ImageType, TImageType> ();
        }
      else if (dynamic_cast<Int16ImageType*> (m_Image.GetPointer()))
        {
        return CastImage<Int16ImageType, TImageType> ();
        }
      else if (dynamic_cast<UInt16ImageType*> (m_Image.GetPointer()))
        {
        return CastImage<UInt16ImageType, TImageType> ();
        }
      else if (dynamic_cast<Int32ImageType*> (m_Image.GetPointer()))
        {
        return CastImage<Int32ImageType, TImageType> ();
        }
      else if (dynamic_cast<UInt32ImageType*> (m_Image.GetPointer()))
        {
        return CastImage<UInt32ImageType, TImageType> ();
        }
      else if (dynamic_cast<FloatImageType*> (m_Image.GetPointer()))
        {
        return CastImage<FloatImageType, TImageType> ();
        }
      else if (dynamic_cast<DoubleImageType*> (m_Image.GetPointer()))
        {
        return CastImage<DoubleImageType, TImageType> ();
        }
      else if (dynamic_cast<ComplexInt16ImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexInt16ImageType, TImageType>();
        }
      else if (dynamic_cast<ComplexInt32ImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexInt32ImageType, TImageType>();
        }
      else if (dynamic_cast<ComplexFloatImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexFloatImageType, TImageType>();
        }
      else if (dynamic_cast<ComplexDoubleImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexDoubleImageType, TImageType>();
        }
      else if (dynamic_cast<UInt8VectorImageType*> (m_Image.GetPointer()))
        {
        return CastImage<UInt8VectorImageType, TImageType> ();
        }
      else if (dynamic_cast<Int16VectorImageType*> (m_Image.GetPointer()))
        {
        return CastImage<Int16VectorImageType, TImageType> ();
        }
      else if (dynamic_cast<UInt16VectorImageType*> (m_Image.GetPointer()))
        {
        return CastImage<UInt16VectorImageType, TImageType> ();
        }
      else if (dynamic_cast<Int32VectorImageType*> (m_Image.GetPointer()))
        {
        return CastImage<Int32VectorImageType, TImageType> ();
        }
      else if (dynamic_cast<UInt32VectorImageType*> (m_Image.GetPointer()))
        {
        return CastImage<UInt32VectorImageType, TImageType> ();
        }
      else if (dynamic_cast<FloatVectorImageType*> (m_Image.GetPointer()))
        {
        return CastImage<FloatVectorImageType, TImageType> ();
        }
      else if (dynamic_cast<DoubleVectorImageType*> (m_Image.GetPointer()))
        {
        return CastImage<DoubleVectorImageType, TImageType> ();
        }
      else if (dynamic_cast<UInt8RGBAImageType*> (m_Image.GetPointer()))
        {
        return CastImage<UInt8RGBAImageType, TImageType> ();
        }
      else if (dynamic_cast<UInt8RGBImageType*> (m_Image.GetPointer()))
        {
        return CastImage<UInt8RGBImageType, TImageType> ();
        }
      else if (dynamic_cast<ComplexInt16VectorImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexInt16VectorImageType, TImageType>();
        }
      else if (dynamic_cast<ComplexInt32VectorImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexInt32VectorImageType, TImageType>();
        }
      else if (dynamic_cast<ComplexFloatVectorImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexFloatVectorImageType, TImageType>();
        }
      else if (dynamic_cast<ComplexDoubleVectorImageType*>(m_Image.GetPointer()))
        {
        return CastImage<ComplexDoubleVectorImageType, TImageType>();
        }
      else
        {
#if INPUT_IMAGE_PARAMETER_GET_IMAGE_EXCEPTION
        itkExceptionMacro("Unknown image type");
#else
	return nullptr;
#endif
        }
      }
    }
}

/** declare a specialization for ImageBaseType */
template <>
ImageBaseType*
InputImageParameter::GetImage();

template <class TInputImage, class TOutputImage>
TOutputImage*
InputImageParameter::CastImage()
{
  if ( dynamic_cast<TOutputImage*> (m_Image.GetPointer()) )
    {
    return dynamic_cast<TOutputImage*> (m_Image.GetPointer());
    }
  else
    {
    TInputImage* realInputImage = dynamic_cast<TInputImage*>(m_Image.GetPointer());

    typedef ClampImageFilter<TInputImage, TOutputImage> CasterType;
    typename CasterType::Pointer caster = CasterType::New();

    caster->SetInput(realInputImage);
    caster->UpdateOutputInformation();

    m_Image = caster->GetOutput();
    m_Caster = caster;

    return caster->GetOutput();
    } 
  // itkExceptionMacro("Cast from "<<typeid(TInputImage).name()<<" to "<<typeid(TOutputImage).name()<<" not authorized.");
}

template <class TInputImage>
void
InputImageParameter::SetImage(TInputImage* image)
{
  m_UseFilename = false;
  m_Image = image;
}


} // End namespace Wrapper
} // End namespace otb

#endif
