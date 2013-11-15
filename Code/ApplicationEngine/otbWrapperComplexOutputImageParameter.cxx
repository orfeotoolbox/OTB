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
#include "itkCastImageFilter.h"
#include "itkVectorCastImageFilter.h"

namespace otb
{
namespace Wrapper
{

ComplexOutputImageParameter::ComplexOutputImageParameter()
  : m_ComplexPixelType(ComplexImagePixelType_float), m_RAMValue(0)
{
  this->SetName("Complex Output Image");
  this->SetKey("cout");
}

ComplexOutputImageParameter::~ComplexOutputImageParameter()
{
}

void ComplexOutputImageParameter::InitializeWriters()
{
  m_ComplexFloatWriter = ComplexFloatWriterType::New();
  m_ComplexDoubleWriter = ComplexDoubleWriterType::New();

  m_ComplexVectorFloatWriter = ComplexVectorFloatWriterType::New();
  m_ComplexVectorDoubleWriter = ComplexVectorDoubleWriterType::New();
}


#define otbCastAndWriteImageMacro(InputImageType, OutputImageType, writer)        \
  {                                                                               \
    typedef itk::CastImageFilter<InputImageType, OutputImageType> CastFilterType; \
    typename CastFilterType::Pointer caster = CastFilterType::New();              \
    caster->SetInput( dynamic_cast<InputImageType*>(m_Image.GetPointer()) );      \
    caster->InPlaceOn();                                                          \
    writer->SetFileName( this->GetFileName() );                                   \
    writer->SetInput(caster->GetOutput());                                        \
    if (m_RAMValue != 0)                                                          \
      writer->SetAutomaticAdaptativeStreaming(m_RAMValue);                        \
    writer->Update();                                                             \
  }


template <class TInputImageType>
void
ComplexOutputImageParameter::SwitchImageWrite()
{
  switch(m_ComplexPixelType )
    {
    case ComplexImagePixelType_float:
    {
    otbCastAndWriteImageMacro(TInputImageType, ComplexFloatImageType, m_ComplexFloatWriter);
    break;
    }
    case ComplexImagePixelType_double:
    {
    otbCastAndWriteImageMacro(TInputImageType, ComplexDoubleImageType, m_ComplexDoubleWriter);
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
    otbCastAndWriteImageMacro(TInputVectorImageType, ComplexFloatVectorImageType, m_ComplexVectorFloatWriter);
    break;
    }
    case ComplexImagePixelType_double:
    {
    otbCastAndWriteImageMacro(TInputVectorImageType, ComplexDoubleVectorImageType, m_ComplexVectorDoubleWriter);
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
    SwitchImageWrite<DoubleImageType>();
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
