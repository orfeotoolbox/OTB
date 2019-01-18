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

#include "otbWrapperComplexOutputImageParameter.h"
#include "otbClampImageFilter.h"

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

ComplexOutputImageParameter::ComplexOutputImageParameter()
  : m_ComplexPixelType(ComplexImagePixelType_float),
    m_DefaultComplexPixelType(ComplexImagePixelType_float),
    m_RAMValue(0)
{
  this->SetName("Complex Output Image");
  this->SetKey("cout");
}

ComplexOutputImageParameter::~ComplexOutputImageParameter()
{
}

std::string
ComplexOutputImageParameter::ConvertPixelTypeToString(ComplexImagePixelType type)
{
  std::string ret;
  switch(type)
    {
    case ComplexImagePixelType_int16:
      {
      ret = "cint16";
      break;
      }
    case ComplexImagePixelType_int32:
      {
      ret = "cint32";
      break;
      }
    case ComplexImagePixelType_float:
      {
      ret = "cfloat";
      break;
      }
    case ComplexImagePixelType_double:
      {
      ret = "cdouble";
      break;
      }
    }
  return ret;
}

bool
ComplexOutputImageParameter::ConvertStringToPixelType(const std::string &value, ComplexImagePixelType &type)
{
  if (value == "cint16")
    type = ComplexImagePixelType_int16;
  if (value == "cint32")
    type = ComplexImagePixelType_int32;
  else if (value == "cfloat")
    type = ComplexImagePixelType_float;
  else if (value == "cdouble")
    type = ComplexImagePixelType_double;
  else
    return false;
  return true;
}

void ComplexOutputImageParameter::InitializeWriters()
{
  m_ComplexVectorInt16Writer = ComplexVectorInt16WriterType::New();
  m_ComplexVectorInt32Writer = ComplexVectorInt32WriterType::New();
  m_ComplexVectorFloatWriter = ComplexVectorFloatWriterType::New();
  m_ComplexVectorDoubleWriter = ComplexVectorDoubleWriterType::New();
}

template <typename TInput, typename TOutput> void CastAndWriteImage(itk::ImageBase<2> * in, otb::ImageFileWriter<TOutput> * writer, const std::string & filename, const unsigned int & ramValue)
{
  typedef ClampImageFilter<TInput, TOutput> ClampFilterType; 
  typename ClampFilterType::Pointer clampFilter = ClampFilterType::New();         
  clampFilter->SetInput( dynamic_cast<TInput*>(in));
  
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
      // Use the WriteMPI function
      WriteMPI(clampFilter->GetOutput(),filename,ramValue);      
      }
    #ifdef OTB_USE_SPTW
    else if (extension == ".tif")
      {
      // Use simple parallel tiff writer
      typedef otb::SimpleParallelTiffWriter<TOutput> SPTWriterType;

      typename SPTWriterType::Pointer sptWriter = SPTWriterType::New();
      sptWriter->SetFileName(filename);
      sptWriter->SetInput(clampFilter->GetOutput());
      sptWriter->SetAutomaticAdaptativeStreaming(ramValue);
      sptWriter->Update();
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
    
    writer->SetFileName( filename );                                     
    writer->SetInput(clampFilter->GetOutput());                                     
    writer->SetAutomaticAdaptativeStreaming(ramValue);
    writer->Update();
    }
}

template <class TInputImageType>
void
ComplexOutputImageParameter::SwitchImageWrite()
{
  switch(m_ComplexPixelType )
    {
    case ComplexImagePixelType_int16:
    {
    CastAndWriteImage<TInputImageType,ComplexInt16VectorImageType>(
      m_Image ,
      m_ComplexVectorInt16Writer ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ComplexImagePixelType_int32:
    {
    CastAndWriteImage<TInputImageType,ComplexInt32VectorImageType>(
      m_Image ,
      m_ComplexVectorInt32Writer ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ComplexImagePixelType_float:
    {
    CastAndWriteImage<TInputImageType,ComplexFloatVectorImageType>(
      m_Image ,
      m_ComplexVectorFloatWriter ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ComplexImagePixelType_double:
    {
    CastAndWriteImage<TInputImageType,ComplexDoubleVectorImageType>(
      m_Image ,
      m_ComplexVectorDoubleWriter ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    }
}

void
ComplexOutputImageParameter::Write()
{
  m_Image->UpdateOutputInformation();
  if (dynamic_cast<ComplexInt16ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<ComplexInt16ImageType>();
    }
  else if (dynamic_cast<ComplexInt16ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<ComplexInt16ImageType>();
    }
  else if (dynamic_cast<ComplexFloatImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<ComplexFloatImageType>();
    }
  else if (dynamic_cast<ComplexDoubleImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<ComplexDoubleImageType>();
    }
  else if (dynamic_cast<ComplexInt16VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<ComplexInt16VectorImageType>();
    }
  else if (dynamic_cast<ComplexInt32VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<ComplexInt32VectorImageType>();
    }
  else if (dynamic_cast<ComplexFloatVectorImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<ComplexFloatVectorImageType>();
    }
  else if (dynamic_cast<ComplexDoubleVectorImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<ComplexDoubleVectorImageType>();
    }
  else
    {
    itkExceptionMacro("Unknown complex image type");
    }
  }

itk::ProcessObject*
ComplexOutputImageParameter::GetWriter()
{
  itk::ProcessObject* writer = nullptr;
  switch ( GetComplexPixelType() )
    {
    case ComplexImagePixelType_int16:
    {
      writer = m_ComplexVectorInt16Writer;
      break;
    }
    case ComplexImagePixelType_int32:
    {
      writer = m_ComplexVectorInt32Writer;
      break;
    }
    case ComplexImagePixelType_float:
    {
      writer = m_ComplexVectorFloatWriter;
      break;
    }
    case ComplexImagePixelType_double:
    {
      writer = m_ComplexVectorDoubleWriter;
      break;
    }
    }
  return writer;
}

ImageBaseType*
ComplexOutputImageParameter::GetValue( )
{
  return m_Image;
}

void
ComplexOutputImageParameter::SetValue(ImageBaseType* image)
{
  m_Image = image;
  SetActive(true);
}

bool
ComplexOutputImageParameter::HasValue() const
{
  std::string filename(this->GetFileName());
  return !filename.empty();
}

void ComplexOutputImageParameter::SetFileName (const char* filename)
{
  this->SetFileName(std::string(filename));
}

void ComplexOutputImageParameter::SetFileName (const std::string& filename)
{
  m_FileName = filename;
  SetActive(true);
}

}
}
