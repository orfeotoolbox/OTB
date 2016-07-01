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
#include "otbWrapperComplexOutputImageParameter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkVectorCastImageFilter.h"

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

void ComplexOutputImageParameter::InitializeWriters()
{
  m_ComplexFloatWriter = ComplexFloatWriterType::New();
  m_ComplexDoubleWriter = ComplexDoubleWriterType::New();

  m_ComplexVectorFloatWriter = ComplexVectorFloatWriterType::New();
  m_ComplexVectorDoubleWriter = ComplexVectorDoubleWriterType::New();
}

template <typename TInput, typename TOutput> void CastAndWriteImage(itk::ImageBase<2> * in, otb::ImageFileWriter<TOutput> * writer, const std::string & filename, const unsigned int & ramValue)
{
  typedef itk::CastImageFilter<TInput, TOutput> ClampFilterType; 
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
    case ComplexImagePixelType_float:
    {
    CastAndWriteImage<TInputImageType,ComplexFloatImageType>(m_Image,m_ComplexFloatWriter,m_FileName,m_RAMValue);
    break;
    }
    case ComplexImagePixelType_double:
    {
    CastAndWriteImage<TInputImageType,ComplexDoubleImageType>(m_Image,m_ComplexDoubleWriter,m_FileName,m_RAMValue);
    break;
    }
    }
}


template <class TInputVectorImageType>
void
ComplexOutputImageParameter::SwitchVectorImageWrite()
  {
  switch(m_ComplexPixelType )
    {
    case ComplexImagePixelType_float:
    {
    CastAndWriteImage<TInputVectorImageType,ComplexFloatVectorImageType>(m_Image,m_ComplexVectorFloatWriter,m_FileName,m_RAMValue);
    break;
    }
    case ComplexImagePixelType_double:
    {
    CastAndWriteImage<TInputVectorImageType,ComplexDoubleVectorImageType>(m_Image,m_ComplexVectorDoubleWriter,m_FileName,m_RAMValue);
    break;
    }
    }
  }


void
ComplexOutputImageParameter::Write()
{
  m_Image->UpdateOutputInformation();

  if (dynamic_cast<ComplexFloatImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<ComplexFloatImageType>();
    }
  else if (dynamic_cast<ComplexDoubleImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<ComplexDoubleImageType>();
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
    itkExceptionMacro("Unknown complex image type");
    }
  }

itk::ProcessObject*
ComplexOutputImageParameter::GetWriter()
{
  int type = 0;
  // 0 : image
  // 1 : VectorImage

  if ( dynamic_cast<FloatVectorImageType*>( m_Image.GetPointer()) ||
       dynamic_cast<DoubleVectorImageType*>(m_Image.GetPointer()))
    {
    type = 1;
    }

  itk::ProcessObject* writer = 0;
  switch ( GetComplexPixelType() )
    {
    case ComplexImagePixelType_float:
    {
    if( type == 1 )
      writer = m_ComplexVectorFloatWriter;
    else
      writer = m_ComplexFloatWriter;
    break;
    }
    case ComplexImagePixelType_double:
    {
    if( type == 1 )
      writer = m_ComplexVectorDoubleWriter;
    else
      writer = m_ComplexDoubleWriter;
    break;
    }
    }
  return writer;
}

ComplexOutputImageParameter::ImageBaseType*
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

}
}
