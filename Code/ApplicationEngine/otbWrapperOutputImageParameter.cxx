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
#include "otbWrapperOutputImageParameter.h"
//#include "itkCastImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"

namespace otb
{
namespace Wrapper
{

OutputImageParameter::OutputImageParameter()
: m_PixelType(ImagePixelType_float)
{
  this->SetName("Output Image");
  this->SetKey("out");

  this->InitializeWriters();

}

OutputImageParameter::~OutputImageParameter()
{
}

void OutputImageParameter::InitializeWriters()
{
  m_Int8Writer = Int8WriterType::New();
  m_UInt8Writer = UInt8WriterType::New();
  m_Int16Writer = Int16WriterType::New();
  m_UInt16Writer = UInt16WriterType::New();
  m_Int32Writer = Int32WriterType::New();
  m_UInt32Writer = UInt32WriterType::New();
  m_FloatWriter = FloatWriterType::New();
  m_DoubleWriter = DoubleWriterType::New();
}


#define otbRescaleAndWriteMacro(OutputImageType, writer)                       \
  {                                                                     \
    typedef VectorRescaleIntensityImageFilter<FloatVectorImageType, OutputImageType> RescaleFilterType; \
    RescaleFilterType::Pointer rescaler = RescaleFilterType::New();     \
    OutputImageType::PixelType outputMinimum(nbChannel);                \
    outputMinimum.Fill(itk::NumericTraits<OutputImageType::InternalPixelType>::min()); \
    OutputImageType::PixelType outputMaximum(nbChannel);                \
    outputMaximum.Fill(itk::NumericTraits<OutputImageType::InternalPixelType>::max()); \
    rescaler->SetOutputMinimum(outputMinimum);                          \
    rescaler->SetOutputMaximum(outputMaximum);                          \
    rescaler->SetAutomaticInputMinMaxComputation( true );               \
    rescaler->SetInput( this->GetImage() );                             \
    writer->SetFileName( this->GetFileName() );                   \
    writer->SetInput(rescaler->GetOutput());                      \
    writer->Update();                                             \
  }

void
OutputImageParameter::Write( )
{
  this->GetImage()->UpdateOutputInformation();
  const unsigned int nbChannel( this->GetImage()->GetNumberOfComponentsPerPixel() );

  switch(m_PixelType )
  {
    case ImagePixelType_int8:
      {
      otbRescaleAndWriteMacro(Int8VectorImageType, m_Int8Writer);
      break;
      }
    case ImagePixelType_uint8:
      {
      otbRescaleAndWriteMacro(UInt8VectorImageType, m_UInt8Writer);
      break;
      }
    case ImagePixelType_int16:
      {
      otbRescaleAndWriteMacro(Int16VectorImageType, m_Int16Writer);
      break;
      }
    case ImagePixelType_uint16:
      {
      otbRescaleAndWriteMacro(UInt16VectorImageType, m_UInt16Writer);
      break;
      }
    case ImagePixelType_int32:
      {
      otbRescaleAndWriteMacro(Int32VectorImageType, m_Int32Writer);
      break;
      }
    case ImagePixelType_uint32:
      {
      otbRescaleAndWriteMacro(UInt32VectorImageType, m_UInt32Writer);
      break;
      }
    case ImagePixelType_float:
      {
      m_FloatWriter->SetFileName( this->GetFileName() );
      m_FloatWriter->SetInput(this->GetImage());
      m_FloatWriter->Modified();
      m_FloatWriter->Update();
      break;
      }
    case ImagePixelType_double:
      {
      otbRescaleAndWriteMacro(DoubleVectorImageType, m_DoubleWriter);
      break;
      }
  }
}

itk::ProcessObject*
OutputImageParameter::GetWriter( )
{
  itk::ProcessObject* writer = 0;
  switch ( GetPixelType() )
  {
    case ImagePixelType_int8:
      writer = m_Int8Writer;
      break;
    case ImagePixelType_uint8:
      writer = m_UInt8Writer;
      break;
    case ImagePixelType_int16:
      writer = m_Int16Writer;
      break;
    case ImagePixelType_uint16:
      writer = m_UInt16Writer;
      break;
    case ImagePixelType_int32:
      writer = m_Int32Writer;
      break;
    case ImagePixelType_uint32:
      writer = m_UInt32Writer;
      break;
    case ImagePixelType_float:
      writer = m_FloatWriter;
      break;
    case ImagePixelType_double:
      writer = m_DoubleWriter;
      break;
  }
  return writer;
}

FloatVectorImageType*
OutputImageParameter::GetValue( )
{
  return m_Image;
}

void
OutputImageParameter::SetValue(FloatVectorImageType* image)
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

}
}
