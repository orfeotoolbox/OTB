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

#include "otbWrapperOutputImageParameter.h"
#include "otbClampImageFilter.h"
#include "itkCastImageFilter.h"
#include "otbClampVectorImageFilter.h"
#include "otbImageIOFactory.h"
#include "itksys/SystemTools.hxx"
#include "otbTwoNRIBandsImageToNComplexBandsImage.h"

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
  this->SetName("Output Image");
  this->SetKey("out");
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
  m_UInt8Writer = UInt8WriterType::New();
  m_Int16Writer = Int16WriterType::New();
  m_UInt16Writer = UInt16WriterType::New();
  m_Int32Writer = Int32WriterType::New();
  m_UInt32Writer = UInt32WriterType::New();
  m_FloatWriter = FloatWriterType::New();
  m_DoubleWriter = DoubleWriterType::New();

  m_VectorUInt8Writer = VectorUInt8WriterType::New();
  m_VectorInt16Writer = VectorInt16WriterType::New();
  m_VectorUInt16Writer = VectorUInt16WriterType::New();
  m_VectorInt32Writer = VectorInt32WriterType::New();
  m_VectorUInt32Writer = VectorUInt32WriterType::New();
  m_VectorFloatWriter = VectorFloatWriterType::New();
  m_VectorDoubleWriter = VectorDoubleWriterType::New();

  m_RGBUInt8Writer = RGBUInt8WriterType::New();
  m_RGBAUInt8Writer = RGBAUInt8WriterType::New();

  m_ComplexFloatWriter = ComplexFloatWriterType::New();
  m_ComplexDoubleWriter = ComplexDoubleWriterType::New();

  m_ComplexVectorFloatWriter = ComplexVectorFloatWriterType::New();
  m_ComplexVectorDoubleWriter = ComplexVectorDoubleWriterType::New();
}


template <typename TInput, typename TOutput> 
void 
ClampAndWriteImage( itk::ImageBase<2> * in ,
                    otb::ImageFileWriter<TOutput> * writer , 
                    const std::string & filename , 
                    const unsigned int & ramValue )
{
  typedef ClampImageFilter < TInput , TOutput > ClampFilterType;
  typename ClampFilterType::Pointer clampFilter ( ClampFilterType::New() );

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

/*template <typename TInput, typename TOutput > 
void
ClampAndWriteVectorImage( itk::ImageBase<2> * in , 
                          otb::ImageFileWriter<TOutput > * writer ,
                          const std::string & filename , 
                          const unsigned int & ramValue ,
                          typename itk::ImageToImageFilter< TInput , TOutput > * clampFilter = otb::ClampImageFilter< TInput , TOutput >::New() )
{     
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
}*/

template <class TInput>
void
OutputImageParameter::SwitchImageWrite()
{
  switch(m_PixelType )
    {
    case ImagePixelType_uint8:
    {
    ClampAndWriteImage< TInput , UInt8ImageType >(  
      m_Image ,
      m_UInt8Writer ,
      m_FileName , 
      m_RAMValue );
    break;
    }
    case ImagePixelType_int16:
    {
    ClampAndWriteImage< TInput , Int16ImageType >( 
      m_Image ,
      m_Int16Writer ,
      m_FileName , 
      m_RAMValue );
    break;
    }
    case ImagePixelType_uint16:
    {
    ClampAndWriteImage< TInput , UInt16ImageType >( 
      m_Image ,
      m_UInt16Writer ,
      m_FileName , 
      m_RAMValue );
    break;
    }
    case ImagePixelType_int32:
    {
    ClampAndWriteImage< TInput , Int32ImageType >( 
      m_Image ,
      m_Int32Writer ,
      m_FileName , 
      m_RAMValue );
    break;
    }
    case ImagePixelType_uint32:
    {
    ClampAndWriteImage< TInput , UInt32ImageType >( 
      m_Image ,
      m_UInt32Writer ,
      m_FileName , 
      m_RAMValue );
    break;
    }
    case ImagePixelType_float:
    {
    ClampAndWriteImage< TInput , FloatImageType >( 
      m_Image ,
      m_FloatWriter ,
      m_FileName , 
      m_RAMValue );
    break;
    }
    case ImagePixelType_double:
    {
    ClampAndWriteImage< TInput , DoubleImageType >( 
      m_Image ,
      m_DoubleWriter ,
      m_FileName , 
      m_RAMValue );
    break;
    }
    case ImagePixelType_cfloat:
    {
    ClampAndWriteImage< TInput , ComplexFloatImageType >( 
      m_Image ,
      m_ComplexFloatWriter ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_cdouble:
    {
    ClampAndWriteImage< TInput , ComplexDoubleImageType >( 
      m_Image ,
      m_ComplexDoubleWriter ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    default:
      break;
    }
}

// template <class TInput>
// void
// OutputImageParameter::SwitchCImageWrite()
// {
//   switch(m_PixelType )
//     {
//     case ImagePixelType_cfloat:
//     {
//     ClampAndWriteImage< TInput , ComplexFloatImageType >( 
//       m_Image ,
//       m_ComplexFloatWriter ,
//       m_FileName ,
//       m_RAMValue );
//     break;
//     }
//     case ImagePixelType_cdouble:
//     {
//     ClampAndWriteImage< TInput , ComplexDoubleImageType >( 
//       m_Image ,
//       m_ComplexDoubleWriter ,
//       m_FileName ,
//       m_RAMValue );
//     break;
//     }
//     default:
//       break;
//     }
// }


template <class TInput>
void
OutputImageParameter::SwitchVectorImageWrite()
  {
  switch(m_PixelType )
    {
    case ImagePixelType_uint8:
    {
    ClampAndWriteImage< TInput , UInt8VectorImageType > (
      m_Image ,
      m_VectorUInt8Writer ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_int16:
    {
    ClampAndWriteImage< TInput , Int16VectorImageType > (
      m_Image ,
      m_VectorInt16Writer ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_uint16:
    {
    ClampAndWriteImage< TInput , UInt16VectorImageType > (
      m_Image ,
      m_VectorUInt16Writer ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_int32:
    {
    ClampAndWriteImage< TInput , Int32VectorImageType > (
      m_Image ,
      m_VectorInt32Writer ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_uint32:
    {
    ClampAndWriteImage< TInput , UInt32VectorImageType > (
      m_Image ,
      m_VectorUInt32Writer ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_float:
    {
    ClampAndWriteImage< TInput , FloatVectorImageType > (
      m_Image ,
      m_VectorFloatWriter ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_double:
    {
    ClampAndWriteImage< TInput , DoubleVectorImageType > (
      m_Image ,
      m_VectorDoubleWriter ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    case ImagePixelType_cfloat:
    {
    ClampAndWriteImage < TInput , ComplexFloatVectorImageType > (
      m_Image ,
      m_ComplexVectorFloatWriter ,
      m_FileName ,
      m_RAMValue ); 
    break;
    }
    case ImagePixelType_cdouble:
    {
    ClampAndWriteImage < TInput , ComplexDoubleVectorImageType > (
      m_Image ,
      m_ComplexVectorDoubleWriter ,
      m_FileName ,
      m_RAMValue );
    break;
    }
    default:
      break;
    }
  }

// template <class TInput>
// void
// OutputImageParameter::SwitchVectorCImageWrite()
// {
//   switch(m_PixelType )
//     {
//     case ImagePixelType_cfloat:
//     {
//     ClampAndWriteImage< TInput , ComplexFloatVectorImageType >( 
//         m_Image ,
//         m_ComplexVectorFloatWriter ,
//         m_FileName ,
//         m_RAMValue );
//     break;
//     }
//     case ImagePixelType_cdouble:
//     {
//     ClampAndWriteImage< TInput , ComplexDoubleVectorImageType >( 
//         m_Image ,
//         m_ComplexVectorDoubleWriter ,
//         m_FileName ,
//         m_RAMValue );
//     break;
//     }
//     default:
//       break;
//     }
// }

template <class TInputRGBAImageType>
void
OutputImageParameter::SwitchRGBAImageWrite()
  {
  if( m_PixelType == ImagePixelType_uint8 )
    {
    m_RGBAUInt8Writer->SetFileName( this->GetFileName() );
    m_RGBAUInt8Writer->SetInput(dynamic_cast<UInt8RGBAImageType*>(m_Image.GetPointer()) );
    m_RGBAUInt8Writer->SetAutomaticAdaptativeStreaming(m_RAMValue);
    m_RGBAUInt8Writer->Update();
    }
   else
     itkExceptionMacro("Unknown PixelType for RGBA Image. Only uint8 is supported.");
  }

template <class TInputRGBImageType>
void
OutputImageParameter::SwitchRGBImageWrite()
  {
   if( m_PixelType == ImagePixelType_uint8 )
    {
    m_RGBUInt8Writer->SetFileName( this->GetFileName() );
    m_RGBUInt8Writer->SetInput(dynamic_cast<UInt8RGBImageType*>(m_Image.GetPointer()) );
    m_RGBUInt8Writer->SetAutomaticAdaptativeStreaming(m_RAMValue);
    m_RGBUInt8Writer->Update();
    }
   else
     itkExceptionMacro("Unknown PixelType for RGB Image. Only uint8 is supported.");
  }

void
OutputImageParameter::Write()
{
  m_Image->UpdateOutputInformation();

  if (dynamic_cast<UInt8ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<UInt8ImageType>();
    }
  else if (dynamic_cast<Int16ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<Int16ImageType>();
    }
  else if (dynamic_cast<UInt16ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<UInt16ImageType>();
    }
  else if (dynamic_cast<Int32ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<Int32ImageType>();
    }
  else if (dynamic_cast<UInt32ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<UInt32ImageType>();
    }
  else if (dynamic_cast<FloatImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<FloatImageType>();
    }
  else if (dynamic_cast<DoubleImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<DoubleImageType>();
    }
  else if (dynamic_cast<ComplexFloatImageType*>(m_Image.GetPointer()) )
    {
    SwitchImageWrite<ComplexFloatImageType>();
    }
  else if (dynamic_cast<ComplexDoubleImageType*>(m_Image.GetPointer()) )
    {
    SwitchImageWrite<ComplexDoubleImageType>();
    }
  else if (dynamic_cast<UInt8VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<UInt8VectorImageType>();
    }
  else if (dynamic_cast<Int16VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<Int16VectorImageType>();
    }
  else if (dynamic_cast<UInt16VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<UInt16VectorImageType>();
    }
  else if (dynamic_cast<Int32VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<Int32VectorImageType>();
    }
  else if (dynamic_cast<UInt32VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<UInt32VectorImageType>();
    }
  else if (dynamic_cast<FloatVectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<FloatVectorImageType>();
    }
  else if (dynamic_cast<DoubleVectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<DoubleVectorImageType>();
    }
  else if (dynamic_cast<UInt8RGBImageType*>(m_Image.GetPointer()))
    {
    SwitchRGBImageWrite<UInt8RGBImageType>();
    }
  else if (dynamic_cast<UInt8RGBAImageType*>(m_Image.GetPointer()))
    {
    SwitchRGBAImageWrite<UInt8RGBAImageType>();
    }
  else if (dynamic_cast<ComplexFloatVectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<ComplexFloatVectorImageType>();
    }
  else if (dynamic_cast<ComplexDoubleVectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<ComplexDoubleVectorImageType>();
    }
  else
    {
    itkExceptionMacro("Unknown image type");
    }
  }


itk::ProcessObject*
OutputImageParameter::GetWriter()
{
  int type = 0;
  // 0 : image
  // 1 : VectorImage
  // 2 : RGBAImage
  // 3 : RGBImage
  itk::ProcessObject* writer = ITK_NULLPTR;
  if (dynamic_cast<UInt8VectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<Int16VectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<UInt16VectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<Int32VectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<UInt32VectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<FloatVectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<DoubleVectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<ComplexFloatVectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<ComplexDoubleVectorImageType*> (m_Image.GetPointer()))
    {
    type = 1;
    }
  else if (dynamic_cast<UInt8RGBAImageType*> (m_Image.GetPointer()))
    {
    type = 2;
    writer = m_RGBAUInt8Writer;
    itkWarningMacro("UInt8RGBAImageType will be saved in UInt8 format.");
    return writer;
    }
  else if (dynamic_cast<UInt8RGBImageType*> (m_Image.GetPointer()))
    {
    type = 3;
    writer = m_RGBUInt8Writer;
    itkWarningMacro("UInt8RGBImageType will be saved in UInt8 format.");
    return writer;
    }
  
  switch (GetPixelType())
    {
    case ImagePixelType_uint8:
      {
      switch(type)
        {
        case 0:
          writer = m_UInt8Writer;
          break;
        case 1:
          writer = m_VectorUInt8Writer;
          break;
        case 2:
          writer = m_RGBAUInt8Writer;
          break;
        default:
          writer = m_RGBUInt8Writer;
          break;
        }
      break;
      }
    case ImagePixelType_int16:
      {
      if (type == 1)
        writer = m_VectorInt16Writer;
      else
        if (type == 0) writer = m_Int16Writer;
      break;
      }
    case ImagePixelType_uint16:
      {
      if (type == 1)
        writer = m_VectorUInt16Writer;
      else
        if (type == 0) writer = m_UInt16Writer;
      break;
      }
    case ImagePixelType_int32:
      {
      if (type == 1)
        writer = m_VectorInt32Writer;
      else
        if (type == 0) writer = m_Int32Writer;
      break;
      }
    case ImagePixelType_uint32:
      {
      if (type == 1)
        writer = m_VectorUInt32Writer;
      else
        if (type == 0) writer = m_UInt32Writer;
      break;
      }
    case ImagePixelType_float:
      {
      if (type == 1)
        writer = m_VectorFloatWriter;
      else
        if (type == 0) writer = m_FloatWriter;
      break;
      }
    case ImagePixelType_double:
      {
      if (type == 1)
        writer = m_VectorDoubleWriter;
      else
        if (type == 0) writer = m_DoubleWriter;
      break;
      }
    case ImagePixelType_cfloat:
      {
      if( type == 1 )
        writer = m_ComplexVectorFloatWriter;
      else
        writer = m_ComplexFloatWriter;
      break;
      }
    case ImagePixelType_cdouble:
      {
      if( type == 1 )
        writer = m_ComplexVectorDoubleWriter;
      else
        writer = m_ComplexDoubleWriter;
      break;
      }
    }
  if (ITK_NULLPTR == writer)
    {
    itkExceptionMacro("Unknown Writer type.");
    }

  return writer;
}

OutputImageParameter::ImageBaseType*
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

}
}
