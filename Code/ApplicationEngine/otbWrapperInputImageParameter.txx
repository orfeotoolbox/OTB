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
#ifndef __otbWrapperInputImageParameter_txx
#define __otbWrapperInputImageParameter_txx

#include "otbWrapperInputImageParameter.h"

#include "otbImageFileReader.h"
#include "itkCastImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"

namespace otb
{
namespace Wrapper
{


template <class TImageType>
TImageType*
InputImageParameter::GetImage()
{
  otbMsgDevMacro(<< "GetImage()");

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
      typedef otb::ImageFileReader<TImageType> ReaderType;
      typename ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(m_FileName);

      try
        {
        reader->UpdateOutputInformation();
        }
      catch (itk::ExceptionObject &)
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
      itkExceptionMacro("No input image or filename detected...");
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
      else
        {
        itkExceptionMacro("Unknown image type");
        }
      }
    }
}


template <class TInputImage, class TOutputImage>
TOutputImage*
InputImageParameter::CastImage()
{
  itkExceptionMacro("Cast from "<<typeid(TInputImage).name()<<" to "<<typeid(TInputImage).name()<<" not authorized.");
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
