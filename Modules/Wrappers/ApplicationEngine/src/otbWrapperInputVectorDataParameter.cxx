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

#include "otbWrapperInputVectorDataParameter.h"

namespace otb
{
namespace Wrapper
{

InputVectorDataParameter::InputVectorDataParameter()
{
  this->SetName("Input VectorData");
  this->SetKey("in");
  this->ClearValue();
}

InputVectorDataParameter::~InputVectorDataParameter()
{
}

bool
InputVectorDataParameter::SetFromFileName(const std::string& filename)
{
  // First clear previous file chosen
  this->ClearValue();

  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  if (!filename.empty()
      && itksys::SystemTools::FileExists(filename.c_str()))
    {
    VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
    reader->SetFileName(filename);
    try
      {
      reader->UpdateOutputInformation();
      }
    catch(itk::ExceptionObject & /*err*/)
      {
      return false;
      }

    // the specified filename is valid => store the value
    m_FileName = filename;
    SetActive(true);
    return true;
    }
 return false;
}


VectorDataType*
InputVectorDataParameter::GetVectorData()
{
  // 2 cases : the user sets a filename vs. the user sets a vector data
  //////////////////////// Filename case:
  if (!m_FileName.empty())
    {
    //typedef otb::ImageFileReader<TOutputImage> ReaderType;
    //typename ReaderType::Pointer reader = ReaderType::New();
    m_Reader = VectorDataFileReaderType::New();
    m_Reader->SetFileName(m_FileName);
    try
      {
      // Update the viewer here to load the file => no streaming for VectorData
      m_Reader->Update();
      }
    catch (itk::ExceptionObject &)
      {
      this->ClearValue();
      }

    m_VectorData = m_Reader->GetOutput();
    }
  //////////////////////// VectorData case:
  else
    {
    if (m_VectorData.IsNull())
      {
      // Else : error
      itkExceptionMacro("No input vector data or filename detected...");
      }
    }

  return m_VectorData;
}

void
InputVectorDataParameter::SetVectorData(VectorDataType* vectorData)
{
  m_VectorData = vectorData;
}


bool
InputVectorDataParameter::HasValue() const
{
  if( m_FileName.empty() && m_VectorData.IsNull() )
    return false;
  else
    return true;
}

void
InputVectorDataParameter::ClearValue()
{
 m_VectorData  = NULL;
 m_Reader = NULL;
 m_FileName = "";
}


}
}


