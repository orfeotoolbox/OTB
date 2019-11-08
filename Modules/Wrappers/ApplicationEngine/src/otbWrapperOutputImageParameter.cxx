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

#include "otbWrapperOutputImageParameter.h"

#include "otbClampImageFilter.h"
#include "otbImageIOFactory.h"
#include "otbWrapperCastImage.h"

#ifdef OTB_USE_MPI
#include "otbMPIConfig.h"
#include "otbMPIVrtWriter.h"
#ifdef OTB_USE_SPTW
#include "otbSimpleParallelTiffWriter.h"
#endif
#endif

#include "itksys/SystemTools.hxx"


#define CAST_IMAGE_BASE(T, image_base)     \
  {                                        \
    T* img = dynamic_cast<T*>(image_base); \
                                           \
    if (img)                               \
    {                                      \
      SwitchInput<T>(img);                 \
                                           \
      return;                              \
    }                                      \
  }


namespace otb
{
namespace Wrapper
{


// Declare specialisation for UInt8RGBAImageType
template <>
void OutputImageParameter::SwitchInput(UInt8RGBAImageType*);

// Declare specialisation for UInt8RGBImageType
template <>
void OutputImageParameter::SwitchInput(UInt8RGBImageType*);


OutputImageParameter::OutputImageParameter() : m_PixelType(ImagePixelType_float), m_DefaultPixelType(ImagePixelType_float), m_RAMValue(0)
{
  SetName("Output Image");
  SetKey("out");
}


OutputImageParameter::~OutputImageParameter()
{
}

std::string OutputImageParameter::ConvertPixelTypeToString(ImagePixelType type)
{
  // TODO: Could be replaced by constant static string array e.g.:
  // return PIXEL_TYPE_NAME[ type ];

  std::string ret;
  switch (type)
  {
  case ImagePixelType_uint8:
  {
    ret = "uint8";
    break;
  }
  case ImagePixelType_int16:
  {
    ret = "int16";
    break;
  }
  case ImagePixelType_uint16:
  {
    ret = "uint16";
    break;
  }
  case ImagePixelType_int32:
  {
    ret = "int32";
    break;
  }
  case ImagePixelType_uint32:
  {
    ret = "uint32";
    break;
  }
  case ImagePixelType_float:
  {
    ret = "float";
    break;
  }
  case ImagePixelType_double:
  {
    ret = "double";
    break;
  }
  case ImagePixelType_cint16:
  {
    ret = "cint16";
    break;
  }
  case ImagePixelType_cint32:
  {
    ret = "cint32";
    break;
  }
  case ImagePixelType_cfloat:
  {
    ret = "cfloat";
    break;
  }
  case ImagePixelType_cdouble:
  {
    ret = "cdouble";
    break;
  }
  }
  return ret;
}

bool OutputImageParameter::ConvertStringToPixelType(const std::string& value, ImagePixelType& type)
{
  // TODO: Could be replaced std::find_if() in constant static string
  // array (see ::ConvertPixelTypeToString().

  if (value == "uint8")
    type = ImagePixelType_uint8;
  else if (value == "int16")
    type = ImagePixelType_int16;
  else if (value == "uint16")
    type = ImagePixelType_uint16;
  else if (value == "int32")
    type = ImagePixelType_int32;
  else if (value == "uint32")
    type = ImagePixelType_uint32;
  else if (value == "float")
    type = ImagePixelType_float;
  else if (value == "double")
    type = ImagePixelType_double;
  else if (value == "cint16")
    type = ImagePixelType_cint16;
  else if (value == "cint32")
    type = ImagePixelType_cint32;
  else if (value == "cfloat")
    type = ImagePixelType_cfloat;
  else if (value == "cdouble")
    type = ImagePixelType_cdouble;
  else
    return false;
  return true;
}


void OutputImageParameter::InitializeWriters()
{
  ImageBaseType* image = m_Image.GetPointer();

  CAST_IMAGE_BASE(UInt8VectorImageType, image);
  CAST_IMAGE_BASE(Int16VectorImageType, image);
  CAST_IMAGE_BASE(UInt16VectorImageType, image);
  CAST_IMAGE_BASE(Int32VectorImageType, image);
  CAST_IMAGE_BASE(UInt32VectorImageType, image);

  CAST_IMAGE_BASE(FloatVectorImageType, image);
  CAST_IMAGE_BASE(DoubleVectorImageType, image);

  CAST_IMAGE_BASE(ComplexInt16VectorImageType, image);
  CAST_IMAGE_BASE(ComplexInt32VectorImageType, image);
  CAST_IMAGE_BASE(ComplexFloatVectorImageType, image);
  CAST_IMAGE_BASE(ComplexDoubleVectorImageType, image);

  CAST_IMAGE_BASE(UInt8ImageType, image);
  CAST_IMAGE_BASE(Int16ImageType, image);
  CAST_IMAGE_BASE(UInt16ImageType, image);
  CAST_IMAGE_BASE(Int32ImageType, image);
  CAST_IMAGE_BASE(UInt32ImageType, image);

  CAST_IMAGE_BASE(FloatImageType, image);
  CAST_IMAGE_BASE(DoubleImageType, image);

  CAST_IMAGE_BASE(ComplexInt16ImageType, image);
  CAST_IMAGE_BASE(ComplexInt32ImageType, image);
  CAST_IMAGE_BASE(ComplexFloatImageType, image);
  CAST_IMAGE_BASE(ComplexDoubleImageType, image);

  CAST_IMAGE_BASE(UInt8RGBImageType, image);
  CAST_IMAGE_BASE(UInt8RGBAImageType, image);

  itkExceptionMacro("Unknown image-base type.");
}


template <typename TOutputImage, typename TInputImage>
void OutputImageParameter::ClampAndWriteVectorImage(TInputImage* in)
{
  assert(in);
  assert(!m_FileName.empty());

  // Use metaprogramming to choose optimized pipeline.
  details::CastImage<TOutputImage, TInputImage> clamp(in);


#ifdef OTB_USE_MPI

  otb::MPIConfig::Pointer mpiConfig = otb::MPIConfig::Instance();

  if (mpiConfig->GetNbProcs() > 1)
  {
    std::string extension = itksys::SystemTools::GetFilenameExtension(m_FileName);

    if (extension == ".vrt")
    {
      // Use the MPIVrtWriter

      auto vrtWriter = otb::MPIVrtWriter<TOutputImage>::New();

      vrtWriter->SetInput(clamp.out);
      vrtWriter->SetFileName(m_FileName);
      vrtWriter->SetAvailableRAM(m_RAMValue);

      // Change internal state only when everything has been setup
      // without raising exception.

      m_InputCaster  = clamp.icif;
      m_OutputCaster = clamp.ocif;

      m_Writer = vrtWriter;

      return;
    }

#ifdef OTB_USE_SPTW

    else if (extension == ".tif")
    {
      // Use simple parallel tiff writer

      auto sptWriter = otb::SimpleParallelTiffWriter<TOutputImage>::New();

      sptWriter->SetFileName(m_FileName);
      sptWriter->SetInput(clamp.out);
      sptWriter->GetStreamingManager()->SetDefaultRAM(m_RAMValue);

      // Change internal state only when everything has been setup
      // without raising exception.

      m_InputCaster  = clamp.icif;
      m_OutputCaster = clamp.ocif;

      m_Writer = sptWriter;

      return;
    }

#endif // OTB_USE_SPTW

    else
    {
      itkGenericExceptionMacro("File format " << extension << " not supported for parallel writing with MPI. Supported formats are "
                                                              ".vrt and .tif. Extended filenames are not supported.");
    }
  }

#endif // OTB_USE_MPI

  //
  // Use default OTB writer.

  auto writer = otb::ImageFileWriter<TOutputImage>::New();

  writer->SetFileName(m_FileName);
  writer->SetInput(clamp.out);
  writer->GetStreamingManager()->SetDefaultRAM(m_RAMValue);

  // Change internal state only when everything has been setup
  // without raising exception.

  m_InputCaster  = clamp.icif;
  m_OutputCaster = clamp.ocif;

  m_Writer = writer;
}


template <typename TInputImage>
void OutputImageParameter::SwitchInput(TInputImage* image)
{
  assert(image);

  switch (m_PixelType)
  {
  case ImagePixelType_uint8:
    ClampAndWriteVectorImage<UInt8VectorImageType>(image);
    break;

  case ImagePixelType_int16:
    ClampAndWriteVectorImage<Int16VectorImageType>(image);
    break;

  case ImagePixelType_uint16:
    ClampAndWriteVectorImage<UInt16VectorImageType>(image);
    break;

  case ImagePixelType_int32:

    ClampAndWriteVectorImage<Int32VectorImageType>(image);
    break;

  case ImagePixelType_uint32:
    ClampAndWriteVectorImage<UInt32VectorImageType>(image);
    break;

  case ImagePixelType_float:
    ClampAndWriteVectorImage<FloatVectorImageType>(image);
    break;

  case ImagePixelType_double:
    ClampAndWriteVectorImage<DoubleVectorImageType>(image);
    break;

  case ImagePixelType_cint16:
    ClampAndWriteVectorImage<ComplexInt16VectorImageType>(image);
    break;

  case ImagePixelType_cint32:
    ClampAndWriteVectorImage<ComplexInt32VectorImageType>(image);
    break;

  case ImagePixelType_cfloat:
    ClampAndWriteVectorImage<ComplexFloatVectorImageType>(image);
    break;

  case ImagePixelType_cdouble:
    ClampAndWriteVectorImage<ComplexDoubleVectorImageType>(image);
    break;

  default:
    assert(false && "Unexpected image-type.");
    break;
  }
}


void OutputImageParameter::Write()
{
  m_Writer->Update();

  // Clean internal filters
  m_InputCaster  = nullptr;
  m_OutputCaster = nullptr;

  m_Writer = nullptr;
}


itk::ProcessObject* OutputImageParameter::GetWriter()
{
  return m_Writer;
}

ImageBaseType* OutputImageParameter::GetValue()
{
  return m_Image;
}

void OutputImageParameter::SetValue(ImageBaseType* image)
{
  m_Image = image;
  SetActive(true);
}

bool OutputImageParameter::HasValue() const
{
  return !m_FileName.empty();
}

std::string OutputImageParameter::CheckFileName(bool fixMissingExtension)
{
  std::string ret("");
  // Check that there is an ImageIO capable of writing the file
  otb::ExtendedFilenameToWriterOptions::Pointer filenameHelper = otb::ExtendedFilenameToWriterOptions::New();
  filenameHelper->SetExtendedFileName(this->GetFileName());
  std::string simpleFilename = filenameHelper->GetSimpleFileName();
  // TODO : check if simpleFilename is empty

  otb::ImageIOBase::Pointer imageIO = otb::ImageIOFactory::CreateImageIO(simpleFilename.c_str(), otb::ImageIOFactory::WriteMode);
  if (imageIO.IsNull())
  {
    // check for missing extension
    std::string outExt = itksys::SystemTools::GetFilenameLastExtension(simpleFilename);
    if (outExt.empty())
    {
      if (fixMissingExtension)
      {
        // try with .tif
        std::string fullFileName(this->GetFileName());
        std::string extendedPart = fullFileName.substr(simpleFilename.size());
        this->SetFileName(simpleFilename + std::string(".tif") + extendedPart);
        ret += std::string("no extension detected, using TIF as default.");
      }
      else
      {
        // TODO : call exception here?
      }
    }
    else
    {
      // TODO : call exception here?
    }
  }
  return ret;
}

// Specialization for UInt8RGBAImageType

template <>
void OutputImageParameter::SwitchInput(UInt8RGBAImageType* img)
{
  assert(img);

  if (m_PixelType != ImagePixelType_uint8)
    itkExceptionMacro("Unknown PixelType for RGBA Image. Only uint8 is supported.");

  auto writer = otb::ImageFileWriter<UInt8RGBAImageType>::New();

  writer->SetFileName(GetFileName());
  writer->SetInput(img);
  writer->GetStreamingManager()->SetDefaultRAM(m_RAMValue);

  m_Writer = writer;
}

// Specialization for UInt8RGBImageType
template <>
void OutputImageParameter::SwitchInput(UInt8RGBImageType* img)
{
  if (m_PixelType != ImagePixelType_uint8)
    itkExceptionMacro("Unknown PixelType for RGB Image. Only uint8 is supported.");

  auto writer = otb::ImageFileWriter<UInt8RGBImageType>::New();

  writer->SetFileName(GetFileName());
  writer->SetInput(img);
  writer->GetStreamingManager()->SetDefaultRAM(m_RAMValue);

  m_Writer = writer;
}

void OutputImageParameter::SetFileName(const char* filename)
{
  this->SetFileName(std::string(filename));
}

void OutputImageParameter::SetFileName(const std::string& filename)
{
  m_FileName = filename;
  SetActive(true);
}
}
}
