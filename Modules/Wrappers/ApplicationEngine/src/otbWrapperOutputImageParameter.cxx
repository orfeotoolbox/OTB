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
#include "itksys/SystemTools.hxx"

#ifdef OTB_USE_MPI

#include "otbMPIConfig.h"
#include "otbMPIVrtWriter.h"

#ifdef OTB_USE_SPTW
#include "otbSimpleParallelTiffWriter.h"
#endif

#endif



namespace otb
{
namespace Wrapper
{

OutputImageParameter::OutputImageParameter()
  : m_PixelType(ImagePixelType_float),
    m_DefaultPixelType(ImagePixelType_float),
    m_RAMValue(0)
{
  SetName("Output Image");
  SetKey("out");
}


OutputImageParameter::~OutputImageParameter()
{
}

std::string OutputImageParameter::ConvertPixelTypeToString(ImagePixelType type)
{
  std::string ret;
  switch(type)
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

bool
OutputImageParameter::ConvertStringToPixelType(const std::string &value, ImagePixelType &type)
{
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
  ImageBaseType* imgBase = m_Image.GetPointer();
  // Guess the image type
  std::string className(m_Image->GetNameOfClass());
  if (className == "VectorImage")
    {
    UInt8VectorImageType* imgUInt8 = dynamic_cast<UInt8VectorImageType*>(imgBase);
    if (imgUInt8 && SwitchInput(imgUInt8)) return;

    Int16VectorImageType* imgInt16 = dynamic_cast<Int16VectorImageType*>(imgBase);
    if (imgInt16 && SwitchInput(imgInt16)) return;

    UInt16VectorImageType* imgUInt16 = dynamic_cast<UInt16VectorImageType*>(imgBase);
    if (imgUInt16 && SwitchInput(imgUInt16)) return;

    Int32VectorImageType* imgInt32 = dynamic_cast<Int32VectorImageType*>(imgBase);
    if (imgInt32 && SwitchInput(imgInt32)) return;

    UInt32VectorImageType* imgUInt32 = dynamic_cast<UInt32VectorImageType*>(imgBase);
    if (imgUInt32 && SwitchInput(imgUInt32)) return;

    FloatVectorImageType* imgFloat = dynamic_cast<FloatVectorImageType*>(imgBase);
    if (imgFloat && SwitchInput(imgFloat)) return;

    DoubleVectorImageType* imgDouble = dynamic_cast<DoubleVectorImageType*>(imgBase);
    if (imgDouble && SwitchInput(imgDouble)) return;

    ComplexInt16VectorImageType* imgCInt16 = dynamic_cast<ComplexInt16VectorImageType*>(imgBase);
    if (imgCInt16 && SwitchInput(imgCInt16)) return;

    ComplexInt32VectorImageType* imgCInt32 = dynamic_cast<ComplexInt32VectorImageType*>(imgBase);
    if (imgCInt32 && SwitchInput(imgCInt32)) return;

    ComplexFloatVectorImageType* imgCFloat = dynamic_cast<ComplexFloatVectorImageType*>(imgBase);
    if (imgCFloat && SwitchInput(imgCFloat)) return;

    ComplexDoubleVectorImageType* imgCDouble = dynamic_cast<ComplexDoubleVectorImageType*>(imgBase);
    if (imgCDouble && SwitchInput(imgCDouble)) return;
    }
  else
    {
    UInt8ImageType* imgUInt8 = dynamic_cast<UInt8ImageType*>(imgBase);
    if (imgUInt8 && SwitchInput(imgUInt8)) return;

    Int16ImageType* imgInt16 = dynamic_cast<Int16ImageType*>(imgBase);
    if (imgInt16 && SwitchInput(imgInt16)) return;

    UInt16ImageType* imgUInt16 = dynamic_cast<UInt16ImageType*>(imgBase);
    if (imgUInt16 && SwitchInput(imgUInt16)) return;

    Int32ImageType* imgInt32 = dynamic_cast<Int32ImageType*>(imgBase);
    if (imgInt32 && SwitchInput(imgInt32)) return;

    UInt32ImageType* imgUInt32 = dynamic_cast<UInt32ImageType*>(imgBase);
    if (imgUInt32 && SwitchInput(imgUInt32)) return;

    FloatImageType* imgFloat = dynamic_cast<FloatImageType*>(imgBase);
    if (imgFloat && SwitchInput(imgFloat)) return;

    DoubleImageType* imgDouble = dynamic_cast<DoubleImageType*>(imgBase);
    if (imgDouble && SwitchInput(imgDouble)) return;

    ComplexInt16ImageType* imgCInt16 = dynamic_cast<ComplexInt16ImageType*>(imgBase);
    if (imgCInt16 && SwitchInput(imgCInt16)) return;

    ComplexInt32ImageType* imgCInt32 = dynamic_cast<ComplexInt32ImageType*>(imgBase);
    if (imgCInt32 && SwitchInput(imgCInt32)) return;

    ComplexFloatImageType* imgCFloat = dynamic_cast<ComplexFloatImageType*>(imgBase);
    if (imgCFloat && SwitchInput(imgCFloat)) return;

    ComplexDoubleImageType* imgCDouble = dynamic_cast<ComplexDoubleImageType*>(imgBase);
    if (imgCDouble && SwitchInput(imgCDouble)) return;

    UInt8RGBImageType* imgRGB = dynamic_cast<UInt8RGBImageType*>(imgBase);
    if (imgRGB && SwitchInput(imgRGB)) return;

    UInt8RGBAImageType* imgRGBA = dynamic_cast<UInt8RGBAImageType*>(imgBase);
    if (imgRGBA && SwitchInput(imgRGBA)) return;
    }

  itkExceptionMacro("Unknown image type");
}


template <typename TInput, typename TOutput>
std::pair<itk::ProcessObject::Pointer,itk::ProcessObject::Pointer>
ClampAndWriteVectorImage( TInput * in ,
                    const std::string & filename ,
                    const unsigned int & ramValue )
{
  std::pair<itk::ProcessObject::Pointer,itk::ProcessObject::Pointer> ret;
  typedef ClampImageFilter < TInput , TOutput > ClampFilterType;
  typename ClampFilterType::Pointer clampFilter ( ClampFilterType::New() );

  clampFilter->SetInput( in);
  ret.first = clampFilter.GetPointer();

  bool useStandardWriter = true;

  #ifdef OTB_USE_MPI

  otb::MPIConfig::Pointer mpiConfig = otb::MPIConfig::Instance();

  if (mpiConfig->GetNbProcs() > 1)
    {
    useStandardWriter = false;

    // Get file extension
    std::string extension = itksys::SystemTools::GetFilenameExtension(filename);

    if(extension == ".vrt")
      {
      // Use the MPIVrtWriter
      typedef otb::MPIVrtWriter<TOutput> VRTWriterType;

      typename VRTWriterType::Pointer vrtWriter = VRTWriterType::New();
      vrtWriter->SetInput(clampFilter->GetOutput());
      vrtWriter->SetFileName(filename);
      vrtWriter->SetAvailableRAM(ramValue);
      ret.second = vrtWriter.GetPointer();
      }
    #ifdef OTB_USE_SPTW
    else if (extension == ".tif")
      {
      // Use simple parallel tiff writer
      typedef otb::SimpleParallelTiffWriter<TOutput> SPTWriterType;

      typename SPTWriterType::Pointer sptWriter = SPTWriterType::New();
      sptWriter->SetFileName(filename);
      sptWriter->SetInput(clampFilter->GetOutput());
      sptWriter->GetStreamingManager()->SetDefaultRAM(ramValue);
      ret.second = sptWriter.GetPointer();
      }

    #endif
    else
      {
      itkGenericExceptionMacro("File format "<<extension<<" not supported for parallel writing with MPI. Supported formats are .vrt and .tif. Extended filenames are not supported.");
      }

    }

  #endif

  if(useStandardWriter)
    {
    typename otb::ImageFileWriter<TOutput>::Pointer writer =
      otb::ImageFileWriter<TOutput>::New();
    writer->SetFileName( filename );
    writer->SetInput(clampFilter->GetOutput());
    writer->GetStreamingManager()->SetDefaultRAM(ramValue);
    ret.second = writer.GetPointer();
    }

  return ret;
}


template <class TInput>
int
OutputImageParameter::SwitchInput(TInput * img)
{
  if (! img) return 0;

  std::pair<itk::ProcessObject::Pointer,itk::ProcessObject::Pointer> ret;
  switch(m_PixelType )
    {
    case ImagePixelType_uint8:
    {
    ret = ClampAndWriteVectorImage< TInput , UInt8VectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_int16:
    {
    ret = ClampAndWriteVectorImage< TInput , Int16VectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_uint16:
    {
    ret = ClampAndWriteVectorImage< TInput , UInt16VectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_int32:
    {
    ret = ClampAndWriteVectorImage< TInput , Int32VectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_uint32:
    {
    ret = ClampAndWriteVectorImage< TInput , UInt32VectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_float:
    {
    ret = ClampAndWriteVectorImage< TInput , FloatVectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_double:
    {
    ret = ClampAndWriteVectorImage< TInput , DoubleVectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_cint16:
    {
    ret = ClampAndWriteVectorImage < TInput , ComplexInt16VectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_cint32:
    {
    ret = ClampAndWriteVectorImage < TInput , ComplexInt32VectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_cfloat:
    {
    ret = ClampAndWriteVectorImage < TInput , ComplexFloatVectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_cdouble:
    {
    ret = ClampAndWriteVectorImage < TInput , ComplexDoubleVectorImageType > (
      img ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    default:
      break;
    }
  // Save the caster and writer
  m_Caster = ret.first;
  m_Writer = ret.second;
  return 1;
}


void
OutputImageParameter::Write()
{
  m_Writer->Update();

  // Clean internal filters
  m_Caster = nullptr;
  m_Writer = nullptr;
}


itk::ProcessObject*
OutputImageParameter::GetWriter()
{
  return m_Writer;
}

ImageBaseType*
OutputImageParameter::GetValue( )
{
  return m_Image;
}

void
OutputImageParameter::SetValue(ImageBaseType* image)
{
  m_Image = image;
  SetActive(true);
}

bool
OutputImageParameter::HasValue() const
{
  std::string filename(this->GetFileName());
  return !filename.empty();
}

std::string
OutputImageParameter::CheckFileName(bool fixMissingExtension)
{
  std::string ret("");
  // Check that there is an ImageIO capable of writing the file
  otb::ExtendedFilenameToWriterOptions::Pointer filenameHelper =
    otb::ExtendedFilenameToWriterOptions::New();
  filenameHelper->SetExtendedFileName(this->GetFileName());
  std::string simpleFilename = filenameHelper->GetSimpleFileName();
  // TODO : check if simpleFilename is empty

  otb::ImageIOBase::Pointer imageIO =
    otb::ImageIOFactory::CreateImageIO(simpleFilename.c_str(),
                                       otb::ImageIOFactory::WriteMode);
  if(imageIO.IsNull())
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
        this->SetFileName(simpleFilename+std::string(".tif")+extendedPart);
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
int
OutputImageParameter::SwitchInput(UInt8RGBAImageType * img )
{
  if (! img) return 0;
  if( m_PixelType == ImagePixelType_uint8 )
    {
    typename otb::ImageFileWriter<UInt8RGBAImageType>::Pointer writer =
      otb::ImageFileWriter<UInt8RGBAImageType>::New();
    writer->SetFileName( this->GetFileName() );
    writer->SetInput(img);
    writer->GetStreamingManager()->SetDefaultRAM(m_RAMValue);
    m_Writer = writer.GetPointer();
    }
   else
     itkExceptionMacro("Unknown PixelType for RGBA Image. Only uint8 is supported.");
  return 1;
}

// Specialization for UInt8RGBImageType
template <>
int
OutputImageParameter::SwitchInput(UInt8RGBImageType *img)
{
  if (! img) return 0;
  if( m_PixelType == ImagePixelType_uint8 )
    {
    typename otb::ImageFileWriter<UInt8RGBImageType>::Pointer writer =
      otb::ImageFileWriter<UInt8RGBImageType>::New();
    writer->SetFileName( this->GetFileName() );
    writer->SetInput(img);
    writer->GetStreamingManager()->SetDefaultRAM(m_RAMValue);
    m_Writer = writer.GetPointer();
    }
   else
     itkExceptionMacro("Unknown PixelType for RGB Image. Only uint8 is supported.");
  return 1;
}

void OutputImageParameter::SetFileName (const char* filename)
{
  this->SetFileName(std::string(filename));
}

void OutputImageParameter::SetFileName (const std::string& filename)
{
  m_FileName = filename;
  SetActive(true);
}

}
}
