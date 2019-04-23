/*
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

#ifndef otbWrapperInputImageParameter_hxx
#define otbWrapperInputImageParameter_hxx

#include "otbWrapperInputImageParameter.h"

#include "otbWrapperCastImage.h"

namespace otb
{
namespace Wrapper
{


#define CLAMP_IMAGE_IF( Out, In, image_base )	\
  {							\
    In * in_image = dynamic_cast< In * >( image_base );	\
							\
    if( in_image )						\
      return Cast< Out, In >( in_image );			\
  }

#define CLAMP_IMAGE_BASE( T, image_base )				\
  {									\
    CLAMP_IMAGE_IF( T, UInt8VectorImageType, image_base );		\
    CLAMP_IMAGE_IF( T, Int16VectorImageType, image_base );		\
    CLAMP_IMAGE_IF( T, UInt16VectorImageType, image_base );	\
    CLAMP_IMAGE_IF( T, Int32VectorImageType, image_base );		\
    CLAMP_IMAGE_IF( T, UInt32VectorImageType, image_base );	\
									\
    CLAMP_IMAGE_IF( T, FloatVectorImageType, image_base );		\
    CLAMP_IMAGE_IF( T, DoubleVectorImageType, image_base );	\
									\
    CLAMP_IMAGE_IF( T, ComplexInt16VectorImageType, image_base );	\
    CLAMP_IMAGE_IF( T, ComplexInt32VectorImageType, image_base );	\
									\
    CLAMP_IMAGE_IF( T, ComplexFloatVectorImageType, image_base );	\
    CLAMP_IMAGE_IF( T, ComplexDoubleVectorImageType, image_base );	\
    									\
    CLAMP_IMAGE_IF( T, UInt8RGBImageType, image_base );		\
    CLAMP_IMAGE_IF( T, UInt8RGBAImageType, image_base );		\
									\
    CLAMP_IMAGE_IF( T, UInt8ImageType, image_base );		\
    CLAMP_IMAGE_IF( T, Int16ImageType, image_base );		\
    CLAMP_IMAGE_IF( T, UInt16ImageType, image_base );		\
    CLAMP_IMAGE_IF( T, Int32ImageType, image_base );		\
    CLAMP_IMAGE_IF( T, UInt32ImageType, image_base );		\
									\
    CLAMP_IMAGE_IF( T, FloatImageType, image_base );		\
    CLAMP_IMAGE_IF( T, DoubleImageType, image_base );		\
									\
    CLAMP_IMAGE_IF( T, ComplexInt16ImageType, image_base );	\
    CLAMP_IMAGE_IF( T, ComplexInt32ImageType, image_base );	\
									\
    CLAMP_IMAGE_IF( T, ComplexFloatImageType, image_base );	\
    CLAMP_IMAGE_IF( T, ComplexDoubleImageType, image_base );	\
									\
    return nullptr;							\
  }


template< typename TOutputImage,
	  typename TInputImage >
TOutputImage *
InputImageParameter
::Cast( TInputImage * image )
{
  details::CastImage< TOutputImage, TInputImage > clamp( image );

  if( clamp.ocif )
    clamp.ocif->UpdateOutputInformation();

  m_InputCaster = clamp.icif;
  m_OutputCaster = clamp.ocif;

  return clamp.out;
}


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
      }
    }

  else
    {
    //////////////////////// Image case:
    if (m_Image.IsNull())
      {
      return nullptr;
      }
    else
      CLAMP_IMAGE_BASE( TImageType, m_Image.GetPointer() );
    }
}

/** declare a specialization for ImageBaseType */
template <>
OTBApplicationEngine_EXPORT
ImageBaseType*
InputImageParameter::GetImage<ImageBaseType>();


} // End namespace Wrapper
} // End namespace otb

#endif
