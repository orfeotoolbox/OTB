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

namespace otb
{
namespace Wrapper
{

OutputImageParameter::OutputImageParameter()
{
  this->SetName("Output Image");
  this->SetKey("out");
  m_Writer = WriterType::New();
}

OutputImageParameter::~OutputImageParameter()
{
}


void
OutputImageParameter::InitializeWriter( )
{
  m_Writer = WriterType::New();
}

void
OutputImageParameter::Write( )
{
  if(!m_Writer.IsNotNull())
    {
    itkExceptionMacro(<< "The Writer is null, please call explicitly the OutputImageParameter::InitializeWriter() method");
    }

  if (m_Image.IsNotNull())
    {
    m_Writer->SetInput(m_Image);
    m_Writer->SetFileName(this->GetFileName());
    m_Writer->Update();
    }
}

VectorImageType*
OutputImageParameter::GetValue( )
{
  return m_Image;
}

void
OutputImageParameter::SetValue(VectorImageType* image)
{
  m_Image = image;
}

bool
OutputImageParameter::HasValue() const
{
  std::string filename(this->GetFileName());
  return !filename.empty();
}

}
}

