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
#include "otbWrapperInputImageParameter.h"
#include "itksys/SystemTools.hxx"

namespace otb
{
namespace Wrapper
{

InputImageParameter::InputImageParameter()
{
  this->SetName("Input Image");
  this->SetKey("in");
}

InputImageParameter::~InputImageParameter()
{
}

void
InputImageParameter::SetFromFileName(const std::string& filename)
{
  // First clear previous file choosen
  this->ClearValue();

  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  if (!filename.empty()
      && itksys::SystemTools::FileExists(filename.c_str()))
    {
    ImageFileReaderType::Pointer reader = ImageFileReaderType::New();
    reader->SetFileName(filename);
    try
      {
      reader->UpdateOutputInformation();
      }
    catch(itk::ExceptionObject & err)
      {
      this->ClearValue();
      }

    // everything went fine, store the object references
    m_Reader = reader;
    m_Image = reader->GetOutput();
    SetActive(true);
    }
}

std::string
InputImageParameter::GetFileName() const
{
  if (m_Reader)
    {
    return m_Reader->GetFileName();
    }

  itkExceptionMacro(<< "No filename value");
}


VectorImageType*
InputImageParameter::GetImage() const
{
  return m_Image;
}

void
InputImageParameter::SetImage(VectorImageType* image)
{
   m_Image = image;
   m_Reader = ImageFileReaderType::Pointer();
}

bool
InputImageParameter::HasValue() const
{
  return m_Image.IsNotNull();
}

void
InputImageParameter::ClearValue()
{
  m_Image = VectorImageType::Pointer();
  m_Reader = ImageFileReaderType::Pointer();
}

}
}

