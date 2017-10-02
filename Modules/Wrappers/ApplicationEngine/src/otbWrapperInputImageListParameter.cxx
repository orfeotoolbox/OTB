/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperInputImageListParameter.h"
#include "itksys/SystemTools.hxx"

namespace otb
{
namespace Wrapper
{

InputImageListParameter::InputImageListParameter()
  : m_InputImageParameterVector(),
    m_ImageList(FloatVectorImageListType::New())
{
  this->SetName("Input Image List");
  this->SetKey("inList");
}

InputImageListParameter::~InputImageListParameter()
{
}

bool
InputImageListParameter::SetListFromFileName(const std::vector<std::string> & filenames)
{
  // First clear previous file chosen
  this->ClearValue();

  for(unsigned int i=0; i<filenames.size(); i++)
    {
    const std::string filename = filenames[i];

    // File existence checked by the reader
    if (!filename.empty())
      {
      // Try to build a new ParameterInputImage
      InputImageParameter::Pointer tmpInputImageParameter = InputImageParameter::New();
      tmpInputImageParameter->SetFromFileName(filename);

      m_InputImageParameterVector.push_back(tmpInputImageParameter);
      }
    }

  this->Modified();
  SetActive(true);
  return true;
}


void
InputImageListParameter::AddNullElement()
{
  m_InputImageParameterVector.push_back(ITK_NULLPTR);
  SetActive(false);
  this->Modified();
}

bool
InputImageListParameter::AddFromFileName(const std::string & filename)
{
  // File existence checked by the reader
  if (!filename.empty())
    {
    InputImageParameter::Pointer tmpInputImageParameter = InputImageParameter::New();
    tmpInputImageParameter->SetFromFileName(filename);

    m_InputImageParameterVector.push_back(tmpInputImageParameter);

    this->Modified();
    SetActive(true);
    return true;
    }

  return false;
}

bool
InputImageListParameter::SetNthFileName( const unsigned int id, const std::string & filename )
{
  if( m_InputImageParameterVector.size()<id )
    {
    itkExceptionMacro(<< "No image "<<id<<". Only "<<m_InputImageParameterVector.size()<<" images available.");
    }

  // File existence checked by the reader
  if (!filename.empty())
    {
    InputImageParameter::Pointer tmpInputImageParameter = InputImageParameter::New();
    tmpInputImageParameter->SetFromFileName(filename);

    m_InputImageParameterVector[id] = tmpInputImageParameter;

    this->Modified();
    SetActive(true);
    return true;
    }

  return false;
}


std::vector<std::string>
InputImageListParameter::GetFileNameList() const
{
  std::vector<std::string> filenames;

  for(InputImageParameterVectorType::const_iterator it = m_InputImageParameterVector.begin();
      it!=m_InputImageParameterVector.end();++it)
    {
    if(it->IsNull())
      {
      itkExceptionMacro(<< "Empty image in InputImageListParameter.");
      }
    filenames.push_back( (*it)->GetFileName() );
    }

  return filenames;
}


std::string
InputImageListParameter::GetNthFileName( unsigned int i ) const
{
    if(m_InputImageParameterVector.size()<i)
      {
      itkExceptionMacro(<< "No image "<<i<<". Only "<<m_InputImageParameterVector.size()<<" images available.");
      }
    if(m_InputImageParameterVector[i].IsNull())
      {
      itkExceptionMacro(<< "Requested image "<<i<<" has no filename");
      }
    return m_InputImageParameterVector[i]->GetFileName();
}

FloatVectorImageListType*
InputImageListParameter::GetImageList() const
{
  m_ImageList->Clear();
  for (unsigned int i=0 ; i < this->Size() ; ++i)
    {
    if(m_InputImageParameterVector[i].IsNull())
      {
      itkExceptionMacro(<< "Image "<<i<<" is empty.");
      }
    m_ImageList->PushBack(m_InputImageParameterVector[i]->GetFloatVectorImage());
    }
  return m_ImageList;
}

FloatVectorImageType*
InputImageListParameter::GetNthImage(unsigned int i) const
{
  if(this->Size()<=i)
    {
    itkExceptionMacro(<< "No image "<<i<<". Only "<<this->Size()<<" images available.");
    }
  if(m_InputImageParameterVector[i].IsNull())
    {
    itkExceptionMacro(<< "Image "<<i<<" is empty.");
    }
  return m_InputImageParameterVector[i]->GetFloatVectorImage();
}

void
InputImageListParameter::SetImageList(FloatVectorImageListType* imList)
{
  // Check input availability
  for(unsigned int i = 0; i < imList->Size(); i++)
  {
    imList->GetNthElement(i)->UpdateOutputInformation();
  }

  // Clear previous values
  this->ClearValue();

  for(unsigned int i = 0; i<imList->Size(); i++)
    {
    // Try to build a new ParameterInputImage
    InputImageParameter::Pointer tmpInputImageParameter = InputImageParameter::New();

    tmpInputImageParameter->SetImage(imList->GetNthElement(i));

    m_InputImageParameterVector.push_back(tmpInputImageParameter);
    m_ImageList->PushBack(tmpInputImageParameter->GetFloatVectorImage());
    }

  SetActive(true);
  this->Modified();
}

void InputImageListParameter::SetNthImage(unsigned int i, ImageBaseType * img)
{
  if(this->Size()<i)
  {
    itkExceptionMacro(<< "No image "<<i<<". Only "<<this->Size()<<" images available.");
  }

  // Check input availability
  img->UpdateOutputInformation();

  // Try to build a new ParameterInputImage
  InputImageParameter::Pointer tmpInputImageParameter = InputImageParameter::New();

  tmpInputImageParameter->SetImage(img);

  m_InputImageParameterVector[i] = tmpInputImageParameter;
}


void
InputImageListParameter::AddImage(ImageBaseType* image)
{
  // Check input availability
  image->UpdateOutputInformation();

  InputImageParameter::Pointer tmpInputImageParameter = InputImageParameter::New();

  tmpInputImageParameter->SetImage(image);

  m_InputImageParameterVector.push_back(tmpInputImageParameter);

  this->Modified();
}

bool
InputImageListParameter::HasValue() const
{
  if(this->Size() == 0)
    {
    return false;
    }

  bool res(true);
  unsigned int i(0);
  while(i < this->Size() && res == true)
    {
    res = (m_InputImageParameterVector[i] ?
           m_InputImageParameterVector[i]->HasValue() :
           false);
    i++;
    }

  return res;
}


void
InputImageListParameter::Erase( unsigned int id )
{
  if(this->Size()<id)
    {
    itkExceptionMacro(<< "No image "<<id<<". Only "<<this->Size()<<" images available.");
    }

  m_InputImageParameterVector.erase(m_InputImageParameterVector.begin()+id);

  this->Modified();
}

unsigned int
InputImageListParameter::Size() const
{
  return m_InputImageParameterVector.size();
}


void
InputImageListParameter::ClearValue()
{
  m_ImageList->Clear();
  m_InputImageParameterVector.clear();

  SetActive(false);
  this->Modified();
}


}
}

