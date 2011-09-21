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
#include "itkCastImageFilter.h"

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


void
OutputImageParameter::Write( )
{
  switch(m_PixelType )
  {
    case ImagePixelType_int8:
      {
      typedef itk::CastImageFilter<FloatVectorImageType, Int8VectorImageType> CastFilterType;
      CastFilterType::Pointer cast = CastFilterType::New();
      cast->SetInput( this->GetImage() );
      m_Int8Writer->SetFileName( this->GetFileName() );
      m_Int8Writer->SetInput(cast->GetOutput());
      m_Int8Writer->Update();
      }
      break;
    case ImagePixelType_uint8:
      {
      typedef itk::CastImageFilter<FloatVectorImageType, UInt8VectorImageType> CastFilterType;
      CastFilterType::Pointer cast = CastFilterType::New();
      cast->SetInput( this->GetImage() );
      m_UInt8Writer->SetFileName( this->GetFileName() );
      m_UInt8Writer->SetInput(cast->GetOutput());
      m_UInt8Writer->Update();
      }
      break;
    case ImagePixelType_int16:
      {
      typedef itk::CastImageFilter<FloatVectorImageType, Int16VectorImageType> CastFilterType;
      CastFilterType::Pointer cast = CastFilterType::New();
      cast->SetInput( this->GetImage() );
      m_Int16Writer->SetFileName( this->GetFileName() );
      m_Int16Writer->SetInput(cast->GetOutput());
      m_Int16Writer->Update();
      }
      break;
    case ImagePixelType_uint16:
      {
      typedef itk::CastImageFilter<FloatVectorImageType, UInt16VectorImageType> CastFilterType;
      CastFilterType::Pointer cast = CastFilterType::New();
      cast->SetInput( this->GetImage() );
      m_UInt16Writer->SetFileName( this->GetFileName() );
      m_UInt16Writer->SetInput(cast->GetOutput());
      m_UInt16Writer->Update();
      }
      break;
    case ImagePixelType_int32:
      {
      typedef itk::CastImageFilter<FloatVectorImageType, Int32VectorImageType> CastFilterType;
      CastFilterType::Pointer cast = CastFilterType::New();
      cast->SetInput( this->GetImage() );
      m_Int32Writer->SetFileName( this->GetFileName() );
      m_Int32Writer->SetInput(cast->GetOutput());
      m_Int32Writer->Update();
      }
      break;
    case ImagePixelType_uint32:
      {
      typedef itk::CastImageFilter<FloatVectorImageType, UInt32VectorImageType> CastFilterType;
      CastFilterType::Pointer cast = CastFilterType::New();
      cast->SetInput( this->GetImage() );
      m_UInt32Writer->SetFileName( this->GetFileName() );
      m_UInt32Writer->SetInput(cast->GetOutput());
      m_UInt32Writer->Update();
      }
      break;
    case ImagePixelType_float:
      {
      m_FloatWriter->SetFileName( this->GetFileName() );
      m_FloatWriter->SetInput(this->GetImage());
      m_FloatWriter->Modified();
      m_FloatWriter->Update();
      }
      break;
    case ImagePixelType_double:
      {
      typedef itk::CastImageFilter<FloatVectorImageType, DoubleVectorImageType> CastFilterType;
      CastFilterType::Pointer cast = CastFilterType::New();
      cast->SetInput( this->GetImage() );
      m_DoubleWriter->SetFileName( this->GetFileName() );
      m_DoubleWriter->SetInput(cast->GetOutput());
      m_DoubleWriter->Update();
      }
      break;
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
