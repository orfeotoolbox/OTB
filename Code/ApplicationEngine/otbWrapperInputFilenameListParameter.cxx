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
#include "otbWrapperInputFilenameListParameter.h"
#include "itksys/SystemTools.hxx"

namespace otb
{
namespace Wrapper
{

InputFilenameListParameter::InputFilenameListParameter()
{
  this->SetName("Input Filename List");
  this->SetKey("inList");
  m_FilenameList = StringParameterListType::New();
}

InputFilenameListParameter::~InputFilenameListParameter()
{
}

bool
InputFilenameListParameter::SetListFromFileName(const std::vector<std::string> & filenames)
{
  // First clear previous file chosen
  this->ClearValue();

  bool isOk = true;
  for(unsigned int i=0; i<filenames.size(); i++)
    {
    std::string filename = filenames[i];
    // TODO : when the logger will be available, redirect the exception
    // in the logger (like what is done in MsgReporter)
    // File existence checked by the reader
    if (!filename.empty())
      {
      StringParameter::Pointer strParameter = StringParameter::New();
      strParameter->SetValue(filename);
      try
        {
        strParameter->HasValue();
        }
      catch(itk::ExceptionObject & /*err*/)
        {
        this->ClearValue();
        isOk = false;
        break;
        }

      // everything went fine, store the object reference
      m_FilenameList->PushBack(strParameter);
      }
    }

  if( !isOk )
    {
    return false;
    }

  SetActive(true);
  this->Modified();
  return true;
}



void
InputFilenameListParameter::AddNullElement()
{
  m_FilenameList->PushBack(NULL);
  SetActive(false);
  this->Modified();
}

bool
InputFilenameListParameter::AddFromFileName(const std::string & filename)
{
  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  // File existence checked by the reader
  if (!filename.empty())
    {
    StringParameter::Pointer strParameter = StringParameter::New();
    strParameter->SetValue(filename);
    try
      {
      strParameter->HasValue();
      }
    catch(itk::ExceptionObject & /*err*/)
      {
      this->ClearValue();
      return false;
      }

    // everything went fine, store the object references
    m_FilenameList->PushBack(strParameter);
    SetActive(true);
    this->Modified();
    return true;
    }

  return false;
}

bool
InputFilenameListParameter::SetNthFileName( const unsigned int id, const std::string & filename )
{
  if( m_FilenameList->Size()<id )
    {
    itkExceptionMacro(<< "No file "<<id<<". Only "<<m_FilenameList->Size()<<" filenames available.");
    }

  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  // File existence checked by the reader
  if (!filename.empty())
    {
    StringParameter::Pointer strParameter = StringParameter::New();
    strParameter->SetValue(filename);
    try
      {
      strParameter->HasValue();
      }
    catch(itk::ExceptionObject &)
      {
      this->ClearValue();
      return false;
      }

    m_FilenameList->SetNthElement(id, strParameter);

    this->Modified();
    return true;
    }

  return false;
}


std::vector<std::string>
InputFilenameListParameter::GetFileNameList() const
{
  if (m_FilenameList)
    {
    std::vector<std::string> filenames;
    for(unsigned int i=0; i<m_FilenameList->Size(); i++)
      {
      if( m_FilenameList->GetNthElement(i) )
        filenames.push_back( m_FilenameList->GetNthElement(i)->GetValue() );
      }

    return filenames;
    }

  itkExceptionMacro(<< "No filename value");
}


std::string
InputFilenameListParameter::GetNthFileName( unsigned int i ) const
{
  if (m_FilenameList)
    {
    if(m_FilenameList->Size()<i)
      {
      itkExceptionMacro(<< "No file "<<i<<". Only "<<m_FilenameList->Size()<<" filenames available.");
      }

    return m_FilenameList->GetNthElement(i)->GetValue();
    }

  itkExceptionMacro(<< "No filename value");
}



bool
InputFilenameListParameter::HasValue() const
{
  if(m_FilenameList->Size() == 0)
    {
    return false;
    }

  bool res(true);
  unsigned int i(0);
  while(i < m_FilenameList->Size() && res == true)
    {
    res = m_FilenameList->GetNthElement(i).IsNotNull();
    i++;
    }

  return res;
}


void
InputFilenameListParameter::Erase( unsigned int id )
{
  if(m_FilenameList->Size()<id)
    {
    itkExceptionMacro(<< "No file "<<id<<". Only "<<m_FilenameList->Size()<<" filenames available.");
    }

  m_FilenameList->Erase( id );

  this->Modified();
}

void
InputFilenameListParameter::ClearValue()
{
  m_FilenameList->Clear();

  SetActive(false);
  this->Modified();
}


}
}

