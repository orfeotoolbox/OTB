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
#include "otbWrapperInputImageListParameter.h"
#include "itksys/SystemTools.hxx"

namespace otb
{
namespace Wrapper
{

InputImageListParameter::InputImageListParameter()
{
  this->SetName("Input Image List");
  this->SetKey("inList");
  m_ImageList = FloatVectorImageListType::New();
  m_ReaderList = ImageFileReaderListType::New();
}

InputImageListParameter::~InputImageListParameter()
{
}

bool
InputImageListParameter::SetListFromFileName(const std::vector<std::string> & filenames)
{
  // First clear previous file chosen
  this->ClearValue();

  bool isOk = true;
  for(unsigned int i=0; i<filenames.size(); i++)
    {
    const std::string filename = filenames[i];
    // TODO : when the logger will be available, redirect the exception
    // in the logger (like what is done in MsgReporter)
    // File existance checked by the reader
    if (!filename.empty())
      {
      ImageFileReaderType::Pointer reader = ImageFileReaderType::New();
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
      m_ImageList->PushBack(reader->GetOutput());
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
InputImageListParameter::AddNullElement()
{
  m_ReaderList->PushBack(NULL);
  m_ImageList->PushBack(NULL);
  SetActive(false);
  this->Modified();
}

bool
InputImageListParameter::AddFromFileName(const std::string & filename)
{
  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  // File existance checked by the reader
  if (!filename.empty())
    {
    ImageFileReaderType::Pointer reader = ImageFileReaderType::New();
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
    m_ImageList->PushBack(reader->GetOutput());
    SetActive(true);
    this->Modified();
    return true;
    }

  return false;
}

bool
InputImageListParameter::SetNthFileName( const unsigned int id, const std::string & filename )
{
  if( m_ReaderList->Size()<id )
    {
    itkExceptionMacro(<< "No image "<<id<<". Only "<<m_ReaderList->Size()<<" images available.");
    }

  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  // File existance checked by the reader
  if (!filename.empty())
    {
    ImageFileReaderType::Pointer reader = ImageFileReaderType::New();
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
    m_ImageList->SetNthElement(id, reader->GetOutput());

    this->Modified();
    SetActive(true);
    return true;
    }

  return false;
}


std::vector<std::string>
InputImageListParameter::GetFileNameList() const
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
InputImageListParameter::GetNthFileName( unsigned int i ) const
{
  if (m_ReaderList)
    {
    if(m_ReaderList->Size()<i)
      {
      itkExceptionMacro(<< "No image "<<i<<". Only "<<m_ReaderList->Size()<<" images available.");
      }

    return m_ReaderList->GetNthElement(i)->GetFileName();
    }

  itkExceptionMacro(<< "No filename value");
}

FloatVectorImageListType*
InputImageListParameter::GetImageList() const
{
  return m_ImageList;
}

FloatVectorImageType*
InputImageListParameter::GetNthImage(unsigned int i) const
{
  if(m_ImageList->Size()<i)
    {
    itkExceptionMacro(<< "No image "<<i<<". Only "<<m_ImageList->Size()<<" images available.");
    }
  return m_ImageList->GetNthElement(i);
}

void
InputImageListParameter::SetImageList(FloatVectorImageListType* imList)
{
  // Check input availability
  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  try
    {
    for(unsigned int i=0; i<imList->Size(); i++)
      {
      imList->GetNthElement( i )->UpdateOutputInformation();
      }
    }
  catch(itk::ExceptionObject &)
    {
    return;
    }

  m_ImageList = imList;
  m_ReaderList = ImageFileReaderListType::Pointer();
  for(unsigned int i=0; i<m_ImageList->Size(); i++)
    {
    m_ReaderList->PushBack( ImageFileReaderType::Pointer() );
    }

  SetActive(true);
  this->Modified();
}

void
InputImageListParameter::AddImage(FloatVectorImageType* image)
{
  // Check input availability
  // TODO : when the logger will be available, redirect the exception
  // in the logger (like what is done in MsgReporter)
  try
    {
    image->UpdateOutputInformation();
    }
  catch(itk::ExceptionObject &)
    {
    return;
    }

  m_ImageList->PushBack( image );
  m_ReaderList->PushBack( ImageFileReaderType::Pointer() );

  this->Modified();
}

bool
InputImageListParameter::HasValue() const
{
  if(m_ImageList->Size() == 0)
    {
    return false;
    }

  bool res(true);
  unsigned int i(0);
  while(i < m_ImageList->Size() && res == true)
    {
    res = m_ImageList->GetNthElement(i).IsNotNull();
    i++;
    }

  return res;
}


void
InputImageListParameter::Erase( unsigned int id )
{
  if(m_ImageList->Size()<id)
    {
    itkExceptionMacro(<< "No image "<<id<<". Only "<<m_ImageList->Size()<<" images available.");
    }

  m_ImageList->Erase( id );
  m_ReaderList->Erase( id );

  this->Modified();
}

void
InputImageListParameter::ClearValue()
{
  m_ImageList->Clear();
  m_ReaderList->Clear();

  SetActive(false);
  this->Modified();
}


}
}

