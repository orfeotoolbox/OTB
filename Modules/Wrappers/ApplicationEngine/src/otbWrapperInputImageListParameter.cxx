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
      tmpInputImageParameter->SetDescription( "Image filename" );

      m_InputImageParameterVector.push_back(tmpInputImageParameter);
      }
    }

  this->Modified();
  SetActive(true);
  return true;
}


void
InputImageListParameter
::AddNullElement()
{
  InsertNullElement();
}


void
InputImageListParameter
::InsertNullElement( int index )
{
  InputImageParameterVectorType::iterator it1( m_InputImageParameterVector.begin() );

  if( index>=0 )
    it1 += index;

  m_InputImageParameterVector.insert( it1, ITK_NULLPTR );

  SetActive( false );

  Modified();
}

bool
InputImageListParameter
::Insert( const std::string & filename, int index )
{
  InputImageParameter::Pointer image(
    InputImageParameter::New()
  );

  image->SetDescription( "Image filename" );

  if( !filename.empty() )
    image->SetFromFileName( filename );

  m_InputImageParameterVector.insert(
    index<0
    ? m_InputImageParameterVector.end()
    : m_InputImageParameterVector.begin() + index,
    image
  );


  SetActive( true );

  Modified();

  return true;
}


bool
InputImageListParameter::AddFromFileName(const std::string & filename)
{
  // File existence checked by the reader
  if (!filename.empty())
    {
    InputImageParameter::Pointer tmpInputImageParameter = InputImageParameter::New();

    tmpInputImageParameter->SetFromFileName(filename);
    tmpInputImageParameter->SetDescription( "Image filename" );

    m_InputImageParameterVector.push_back(tmpInputImageParameter);

    this->Modified();
    SetActive(true);
    return true;
    }

  return false;
}

bool
InputImageListParameter
::SetNthFileName( const unsigned int id, const std::string & filename )
{
#if 0
  if( m_InputImageParameterVector.size()<id )
    {
    itkExceptionMacro(<< "No image "<<id<<". Only "<<m_InputImageParameterVector.size()<<" images available.");
    }

  // File existence checked by the reader
  if (!filename.empty())
    {
    InputImageParameter::Pointer tmpInputImageParameter = InputImageParameter::New();

    tmpInputImageParameter->SetFromFileName(filename);
    tmpInputImageParameter->SetDescription( "Image filename" );

    m_InputImageParameterVector[id] = tmpInputImageParameter;

    this->Modified();
    SetActive(true);
    return true;
    }

  return false;

#else
  assert( id<m_InputImageParameterVector.size() );
  assert( !m_InputImageParameterVector[ id ].IsNull() );

  if( !filename.empty() )
    {
    Modified();

    SetActive( true );
    }

  return m_InputImageParameterVector[ id ]->SetFromFileName( filename );
#endif
}


std::vector<std::string>
InputImageListParameter::GetFileNameList() const
{
  std::vector<std::string> filenames;

  for(InputImageParameterVectorType::const_iterator it = m_InputImageParameterVector.begin();
      it!=m_InputImageParameterVector.end();++it)
    {
    filenames.push_back( (*it)->GetFileName() );
    }

  return filenames;
}


const std::string &
InputImageListParameter::GetNthFileName( unsigned int i ) const
{
    if(m_InputImageParameterVector.size()<i)
      {
      itkExceptionMacro(<< "No image "<<i<<". Only "<<m_InputImageParameterVector.size()<<" images available.");
      }

    return m_InputImageParameterVector[i]->GetFileName();
}

FloatVectorImageListType*
InputImageListParameter::GetImageList() const
{
  m_ImageList->Clear();
  for (unsigned int i=0 ; i < this->Size() ; ++i)
    {
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
    tmpInputImageParameter->SetDescription( "Image filename" );

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
  tmpInputImageParameter->SetDescription( "Image filename" );

  m_InputImageParameterVector[i] = tmpInputImageParameter;
}


void
InputImageListParameter::AddImage(ImageBaseType* image)
{
  // Check input availability
  image->UpdateOutputInformation();

  InputImageParameter::Pointer tmpInputImageParameter = InputImageParameter::New();

  tmpInputImageParameter->SetImage(image);
  tmpInputImageParameter->SetDescription( "Image filename" );

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


void
InputImageListParameter
::Erase( unsigned int start, unsigned int count )
{
  assert( start<m_InputImageParameterVector.size() );
  assert( start+count<=m_InputImageParameterVector.size() );

  m_InputImageParameterVector.erase(
    m_InputImageParameterVector.begin() + start,
    m_InputImageParameterVector.begin() + start + count
  );

  Modified();
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

bool
InputImageListParameter
::IsActive( unsigned int i ) const
{
  assert( i<m_InputImageParameterVector.size() );
  assert( !m_InputImageParameterVector[ i ].IsNull() );

  return m_InputImageParameterVector[ i ]->GetActive();
}


const std::string &
InputImageListParameter
::GetToolTip( unsigned int i ) const
{
  assert( i<m_InputImageParameterVector.size() );
  assert( !m_InputImageParameterVector[ i ].IsNull() );

  return m_InputImageParameterVector[ i ]->GetDescription();
}


void
InputImageListParameter
::Swap( unsigned int i1, unsigned int i2 )
{
  assert( !m_InputImageParameterVector.empty() );

  auto clamp = [ this ]( unsigned int i ) -> unsigned int
  {
    return
    i>=m_InputImageParameterVector.size()
    ? m_InputImageParameterVector.size() - 1
    : i;
  };

  std::swap(
    m_InputImageParameterVector[ clamp( i1 ) ],
    m_InputImageParameterVector[ clamp( i2 ) ]
  );
}


}

}
