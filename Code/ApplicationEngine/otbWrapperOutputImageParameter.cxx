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

  m_VectorInt8Writer = VectorInt8WriterType::New();
  m_VectorUInt8Writer = VectorUInt8WriterType::New();
  m_VectorInt16Writer = VectorInt16WriterType::New();
  m_VectorUInt16Writer = VectorUInt16WriterType::New();
  m_VectorInt32Writer = VectorInt32WriterType::New();
  m_VectorUInt32Writer = VectorUInt32WriterType::New();
  m_VectorFloatWriter = VectorFloatWriterType::New();
  m_VectorDoubleWriter = VectorDoubleWriterType::New();
}


#define otbRescaleAndWriteMacro(InputImageType, OutputImageType, writer) \
  {                                                                     \
    typedef itk::CastImageFilter<InputImageType, OutputImageType> CastFilterType; \
    typename CastFilterType::Pointer caster = CastFilterType::New();    \
    caster->SetInput( dynamic_cast<InputImageType*>(m_Image.GetPointer()) ); \
    writer->SetFileName( this->GetFileName() );                         \
    writer->SetInput(caster->GetOutput());                              \
    writer->Update();                                                   \
  }


template <class TInputImageType>
void 
OutputImageParameter::SwitchImageWrite()
{
 std::cout<<"OutputImageParameter::SwitchImageWrite start"<<std::endl;
  switch(m_PixelType )
    {
    case ImagePixelType_int8:
    {
    otbRescaleAndWriteMacro(TInputImageType, Int8ImageType, m_Int8Writer);
    break;
    }
    case ImagePixelType_uint8:
    {
 std::cout<<"OutputImageParameter::SwitchImageWrite UNIN8"<<std::endl;
    otbRescaleAndWriteMacro(TInputImageType, UInt8ImageType, m_UInt8Writer);
    break;
    }
    case ImagePixelType_int16:
    {
    otbRescaleAndWriteMacro(TInputImageType, Int16ImageType, m_Int16Writer);
    break;
    }
    case ImagePixelType_uint16:
    {
    otbRescaleAndWriteMacro(TInputImageType, UInt16ImageType, m_UInt16Writer);
    break;
    }
    case ImagePixelType_int32:
    {
    otbRescaleAndWriteMacro(TInputImageType, Int32ImageType, m_Int32Writer);
    break;
    }
    case ImagePixelType_uint32:
    {
    otbRescaleAndWriteMacro(TInputImageType, UInt32ImageType, m_UInt32Writer);
    break;
    }
    case ImagePixelType_float:
    {
    otbRescaleAndWriteMacro(TInputImageType, FloatImageType, m_FloatWriter);
    break;
    }
    case ImagePixelType_double:
    {
    otbRescaleAndWriteMacro(TInputImageType, DoubleImageType, m_DoubleWriter);
    break;
    }
    }
 std::cout<<"OutputImageParameter::SwitchImageWrite end"<<std::endl;
}


template <class TInputVectoImageType>
void
OutputImageParameter::SwitchVectorImageWrite()
  {
  switch(m_PixelType )
    {
    case ImagePixelType_int8:
    {
    otbRescaleAndWriteMacro(TInputVectoImageType, Int8VectorImageType, m_VectorInt8Writer);
    break;
    }
    case ImagePixelType_uint8:
    {
    otbRescaleAndWriteMacro(TInputVectoImageType, UInt8VectorImageType, m_VectorUInt8Writer);
    break;
    }
    case ImagePixelType_int16:
    {
    otbRescaleAndWriteMacro(TInputVectoImageType, Int16VectorImageType, m_VectorInt16Writer);
    break;
    }
    case ImagePixelType_uint16:
    {
    otbRescaleAndWriteMacro(TInputVectoImageType, UInt16VectorImageType, m_VectorUInt16Writer);
    break;
    }
    case ImagePixelType_int32:
    {
    otbRescaleAndWriteMacro(TInputVectoImageType, Int32VectorImageType, m_VectorInt32Writer);
    break;
    }
    case ImagePixelType_uint32:
    {
    otbRescaleAndWriteMacro(TInputVectoImageType, UInt32VectorImageType, m_VectorUInt32Writer);
    break;
    }
    case ImagePixelType_float:
    {
    otbRescaleAndWriteMacro(TInputVectoImageType, FloatVectorImageType, m_VectorFloatWriter);
    break;
    }
    case ImagePixelType_double:
    {
    otbRescaleAndWriteMacro(TInputVectoImageType, DoubleVectorImageType, m_VectorDoubleWriter);
    break;
    }
    }
  }




void
OutputImageParameter::Write()
{
  std::cout<<"OutputImageParameter::Write"<<std::endl;
  m_Image->UpdateOutputInformation();
  std::cout<<"OutputImageParameter::Write1"<<std::endl;
  if (dynamic_cast<Int8ImageType*>(m_Image.GetPointer())) 
    {
    SwitchImageWrite<Int8ImageType>();
    }
  else if (dynamic_cast<UInt8ImageType*>(m_Image.GetPointer()))
    {
 std::cout<<"OutputImageParameter::Write UNIN8"<<std::endl;
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
  else if (dynamic_cast<Int8VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<Int8VectorImageType>();
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
  else
    {
    itkExceptionMacro("Unknown image type");
    }

 std::cout<<"OutputImageParameter::Write2"<<std::endl;
  }


itk::ProcessObject*
OutputImageParameter::GetWriter( )
{
  bool isVectorImage = true;
  if ( dynamic_cast<Int8ImageType*>(m_Image.GetPointer())  || dynamic_cast<UInt8ImageType*>(m_Image.GetPointer()) ||
       dynamic_cast<Int16ImageType*>(m_Image.GetPointer()) ||  dynamic_cast<UInt16ImageType*>(m_Image.GetPointer()) ||
       dynamic_cast<Int32ImageType*>(m_Image.GetPointer()) || dynamic_cast<UInt32ImageType*>(m_Image.GetPointer()) ||
       dynamic_cast<FloatImageType*>(m_Image.GetPointer()) || dynamic_cast<DoubleImageType*>(m_Image.GetPointer())    )         
    {
    isVectorImage = false;
    }                  
  
  itk::ProcessObject* writer = 0;
  switch ( GetPixelType() )
    {
    case ImagePixelType_int8:
    {
    if( isVectorImage )
      writer = m_VectorInt8Writer;
    else
      writer = m_Int8Writer;
    break;
    }
    case ImagePixelType_uint8:
    {
    if( isVectorImage )
      writer = m_VectorUInt8Writer;
    else
      writer = m_UInt8Writer;
    break;
    }
    case ImagePixelType_int16:
    {
    if( isVectorImage )
      writer = m_VectorInt16Writer;
    else
      writer = m_Int16Writer;
    break;
    }
    case ImagePixelType_uint16:
    {
    if( isVectorImage )
      writer = m_VectorUInt16Writer;
    else
      writer = m_UInt16Writer;
    break;
    }
    case ImagePixelType_int32:
    {
    if( isVectorImage )
      writer = m_VectorInt32Writer;
    else
      writer = m_Int32Writer;
    break;
    }
    case ImagePixelType_uint32:
    {
    if( isVectorImage )
      writer = m_VectorUInt32Writer;
    else
      writer = m_UInt32Writer;
    break;
    }
    case ImagePixelType_float:
    {
    if( isVectorImage )
      writer = m_VectorFloatWriter;
    else
      writer = m_FloatWriter;
    break;
    }
    case ImagePixelType_double:
    {
    if( isVectorImage )
      writer = m_VectorDoubleWriter;
    else
      writer = m_DoubleWriter;
    break;
    }
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

}
}
