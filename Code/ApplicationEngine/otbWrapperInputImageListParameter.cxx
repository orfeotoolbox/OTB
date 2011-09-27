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

void
InputImageListParameter::SetListFromFileName(const std::vector<std::string> & filenames)
{
  for(unsigned int i=0; i<filenames.size(); i++)
    {
    ImageFileReaderType::Pointer reader = ImageFileReaderType::New();
    reader->SetFileName(filenames[i]);
    reader->UpdateOutputInformation();
    
    // everything went fine, store the object references
    m_ReaderList->PushBack(reader);
    m_ImageList->PushBack(reader->GetOutput());
    }
  SetActive(true);
  
  this->Modified();
}

void
InputImageListParameter::AddFromFileName(const std::string & filename)
{
  ImageFileReaderType::Pointer reader = ImageFileReaderType::New();
  reader->SetFileName(filename);
  reader->UpdateOutputInformation();
  
  // everything went fine, store the object references
  m_ReaderList->PushBack(reader);
  m_ImageList->PushBack(reader->GetOutput());
  SetActive(true);
  this->Modified();
}

void
InputImageListParameter::SetNthFileName( const unsigned int id, const std::string & filename )
{
  if( m_ReaderList->Size()<id )
    {
    itkExceptionMacro(<< "No image "<<id<<". Only "<<m_ReaderList->Size()<<" images available.");
    }
  
  ImageFileReaderType::Pointer reader = ImageFileReaderType::New();
  reader->SetFileName(filename);
  reader->UpdateOutputInformation();
  m_ReaderList->SetNthElement(id, reader);
  m_ImageList->SetNthElement(id, reader->GetOutput());
}


std::vector<std::string>
InputImageListParameter::GetFileNameList() const
{
  if (m_ReaderList)
    {
    std::vector<std::string> filenames;
    for(unsigned int i=0; i<m_ReaderList->Size(); i++)
      {
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
  m_ImageList = imList;
  m_ReaderList = ImageFileReaderListType::Pointer();
  for(unsigned int i=0; i<m_ImageList->Size(); i++)
    {
    m_ReaderList->PushBack( ImageFileReaderType::Pointer() );
    }
  
  this->Modified();
}

void
InputImageListParameter::AddImage(FloatVectorImageType* image)
{
  m_ImageList->PushBack( image );
  m_ReaderList->PushBack( ImageFileReaderType::Pointer() );
}

bool
InputImageListParameter::HasValue() const
{
  if(m_ImageList->Size() == 0)
    {
    return false;
    }
  
  return m_ImageList->GetNthElement(0).IsNotNull();
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
}

void
InputImageListParameter::Clear()
{
  m_ImageList->Clear();
  m_ReaderList->Clear();
}


}
}

