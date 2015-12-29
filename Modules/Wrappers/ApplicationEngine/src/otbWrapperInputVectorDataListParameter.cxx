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
#include "otbWrapperInputVectorDataListParameter.h"
#include "itksys/SystemTools.hxx"

#include "otbWrapperMacros.h"

namespace otb
{
namespace Wrapper
{

InputVectorDataListParameter::InputVectorDataListParameter()
{
  this->SetName("Input VectorData List");
  this->SetKey("vdList");
  m_VectorDataList = VectorDataListType::New();
  m_ReaderList = VectorDataFileReaderListType::New();
}

InputVectorDataListParameter::~InputVectorDataListParameter()
{
}

bool
InputVectorDataListParameter::SetListFromFileName(const std::vector<std::string> & filenames)
{
  // First clear previous file chosen
  this->ClearValue();

  bool isOk = true;
  for(unsigned int i=0; i<filenames.size(); i++)
    {
    const std::string filename = filenames[i];
    // TODO : when the logger will be available, redirect the exception
    // in the logger (like what is done in MsgReporter)
    if (!filename.empty())
      {
      VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
      reader->SetFileName(filename);
      try
        {
        reader->UpdateOutputInformation();
        }
      catch(itk::ExceptionObject & /*err*/)
        {
        this->ClearValue();
        isOk = false;
        break;
        }

      // everything went fine, store the object references
      m_ReaderList->PushBack(reader);
      m_VectorDataList->PushBack(reader->GetOutput());
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
InputVectorDataListParameter::AddNullElement()
{
  m_ReaderList->PushBack(NULL);
  m_VectorDataList->PushBack(NULL);
  SetActive(false);
  this->Modified();
}

bool
InputVectorDataListParameter::AddFromFileName(const std::string & filename)
{
  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  if (!filename.empty())
    {
    VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
    reader->SetFileName(filename);
    try
      {
      reader->UpdateOutputInformation();
      }
    catch(itk::ExceptionObject & /*err*/)
      {
      this->ClearValue();
      return false;
      }

    // everything went fine, store the object references
    m_ReaderList->PushBack(reader);
    m_VectorDataList->PushBack(reader->GetOutput());
    SetActive(true);
    this->Modified();
    return true;
    }

  return false;
}

bool
InputVectorDataListParameter::SetNthFileName( const unsigned int id, const std::string & filename )
{
  if( m_ReaderList->Size()<id )
    {
    itkExceptionMacro(<< "No vectordata "<<id<<". Only "<<m_ReaderList->Size()<<" vector data available.");
    }

  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  if (!filename.empty())
    {
    VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
    reader->SetFileName(filename);
    try
      {
      reader->UpdateOutputInformation();
      }
    catch(itk::ExceptionObject &)
      {
      this->ClearValue();
      return false;
      }

    m_ReaderList->SetNthElement(id, reader);
    m_VectorDataList->SetNthElement(id, reader->GetOutput());

    this->Modified();
    return true;
    }

  return false;
}


std::vector<std::string>
InputVectorDataListParameter::GetFileNameList() const
{
  if (m_ReaderList)
    {
    std::vector<std::string> filenames;
    for(unsigned int i=0; i<m_ReaderList->Size(); i++)
      {
      if( m_ReaderList->GetNthElement(i) )
        filenames.push_back( m_ReaderList->GetNthElement(i)->GetFileName() );
      }

    return filenames;
    }

  itkExceptionMacro(<< "No filename value");
}


std::string
InputVectorDataListParameter::GetNthFileName( unsigned int i ) const
{
  if (m_ReaderList)
    {
    if(m_ReaderList->Size()<i)
      {
      itkExceptionMacro(<< "No vector data "<<i<<". Only "<<m_ReaderList->Size()<<" vector data available.");
      }

    return m_ReaderList->GetNthElement(i)->GetFileName();
    }

  itkExceptionMacro(<< "No filename value");
}

VectorDataListType*
InputVectorDataListParameter::GetVectorDataList() const
{
  return m_VectorDataList;
}

VectorDataType*
InputVectorDataListParameter::GetNthVectorData(unsigned int i) const
{
  if(m_VectorDataList->Size()<i)
    {
    itkExceptionMacro(<< "No vector data "<<i<<". Only "<<m_VectorDataList->Size()<<" vector data available.");
    }
  return m_VectorDataList->GetNthElement(i);
}

void
InputVectorDataListParameter::SetVectorDataList(VectorDataListType* vdList)
{
  // Check input availability
  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  try
    {
    for(unsigned int i=0; i<vdList->Size(); i++)
      {
      vdList->GetNthElement( i )->UpdateOutputInformation();
      }
    }
  catch(itk::ExceptionObject &)
    {
    return;
    }

  m_VectorDataList = vdList;
  m_ReaderList = VectorDataFileReaderListType::Pointer();
  for(unsigned int i=0; i<m_VectorDataList->Size(); i++)
    {
    m_ReaderList->PushBack( VectorDataFileReaderType::Pointer() );
    }

  SetActive(true);
  this->Modified();
}

void
InputVectorDataListParameter::AddVectorData(VectorDataType* vectorData)
{
  // Check input availability
  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  try
    {
    vectorData->UpdateOutputInformation();
    }
  catch(itk::ExceptionObject &)
    {
    return;
    }

  m_VectorDataList->PushBack( vectorData );
  m_ReaderList->PushBack( VectorDataFileReaderType::Pointer() );

  this->Modified();
}

bool
InputVectorDataListParameter::HasValue() const
{
  if (m_VectorDataList->Size() == 0)
    {
    return false;
    }

  bool res(true);
  unsigned int i(0);
  while (i < m_VectorDataList->Size() && res == true)
    {
    res = m_VectorDataList->GetNthElement(i).IsNotNull();
    i++;
    }

  return res;
}


void
InputVectorDataListParameter::Erase( unsigned int id )
{
  if(m_VectorDataList->Size()<id)
    {
    itkExceptionMacro(<< "No vector data "<<id<<". Only "<<m_VectorDataList->Size()<<" vector data available.");
    }

  m_VectorDataList->Erase( id );
  m_ReaderList->Erase( id );

  this->Modified();
}

void
InputVectorDataListParameter::ClearValue()
{
  m_VectorDataList->Clear();
  m_ReaderList->Clear();

  SetActive(false);
  this->Modified();
}


}
}

