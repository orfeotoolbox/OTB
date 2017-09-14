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


#include <itksys/SystemTools.hxx>


#include "otbCast.h"


namespace otb
{

namespace Wrapper
{


const std::string
IMAGES_FILTER(
  "All files (*);;"
  "TIFF file (*tif);;"
  "PNG File (*.png);;"
  "JPEG File (*.jpg)"
);


/*****************************************************************************/
InputImageParameter::Pointer
InputImageListParameter
::FromImage( ImageBaseType * image )
{
    assert( image!=nullptr );

    InputImageParameter::Pointer p;

    return FromImage( p, image );
}


/*****************************************************************************/
InputImageParameter::Pointer &
InputImageListParameter
::FromImage( InputImageParameter::Pointer & parameter,
	     ImageBaseType * image )
{
    assert( image!=nullptr );

    parameter = InputImageParameter::New();

    parameter->SetImage( image );
    parameter->SetDescription( "Image filename" );

    assert( parameter->GetImage()!=nullptr );

    return parameter;
}


/*****************************************************************************/
InputImageListParameter
::InputImageListParameter() :
  m_ImageList( FloatVectorImageListType::New() )
{
  SetName( "Input Image List" );
  SetKey( "inList" );
}


/*****************************************************************************/
InputImageListParameter
::~InputImageListParameter()
{
}


/*****************************************************************************/
const FloatVectorImageListType *
InputImageListParameter
::GetImageList() const
{
  return const_cast< InputImageListParameter * >( this )->GetImageList();
}


/*****************************************************************************/
FloatVectorImageListType *
InputImageListParameter
::GetImageList()
{
  m_ImageList->Clear();

  std::for_each(
    begin(),
    end(),
    [ this ]( auto parameter ) -> void
    {
      assert( !parameter.IsNull() );

      assert( parameter==otb::DynamicCast< InputImageParameter >( parameter ) );

      assert(
	DynamicCast< InputImageParameter >( parameter )
	->GetFloatVectorImage()!=nullptr
      );

      m_ImageList->PushBack(
	DynamicCast< InputImageParameter >( parameter )
	->GetFloatVectorImage()
      );
    }
  );

  return m_ImageList;
}


/*****************************************************************************/
const FloatVectorImageType *
InputImageListParameter
::GetNthImage( std::size_t i ) const
{
  return const_cast< InputImageListParameter * >( this )->GetNthImage( i );
}


/*****************************************************************************/
FloatVectorImageType *
InputImageListParameter::GetNthImage( std::size_t i )
{
  assert( i<Size() );
  assert( !m_Parameters[ i ].IsNull() );
  assert( m_Parameters[ i ]->GetFloatVectorImage()!=nullptr );

  return m_Parameters[ i ]->GetFloatVectorImage();
}


/*****************************************************************************/
void
InputImageListParameter
::SetImageList( FloatVectorImageListType * imList )
{
  // Check input availability
  for( std::size_t i=0; i<imList->Size(); i++ )
  {
    assert( imList->GetNthElement( i )!=nullptr );

    imList->GetNthElement( i )->UpdateOutputInformation();
  }

  // Clear previous values
  ClearValue();

  for( std::size_t i=0; i<imList->Size(); i++ )
    {
    assert( imList->GetNthElement( i )!=nullptr );

    InputImageParameter::Pointer parameter;

    FromImage( parameter, imList->GetNthElement( i ) );

    assert( parameter->GetFloatVectorImage()!=nullptr );

    m_Parameters.push_back( parameter );

    m_ImageList->PushBack( parameter->GetFloatVectorImage() );
    }

  SetActive( true );

  Modified();
}


/*****************************************************************************/
void
InputImageListParameter
::SetNthImage( std::size_t i, ImageBaseType * image )
{
  assert( i<Size() );
  assert( image!=nullptr );

  // Check input availability
  image->UpdateOutputInformation();

  // Build parameter.
  FromImage( m_Parameters[ i ], image );
}


/*****************************************************************************/
void
InputImageListParameter
::AddImage( ImageBaseType * image )
{
  assert( image!=nullptr );

  // Check input availability
  image->UpdateOutputInformation();

  // Build & add parameter.
  m_Parameters.push_back( FromImage( image ) );

  Modified();
}


/*****************************************************************************/
void
InputImageListParameter::ClearValue()
{
  Superclass::ClearValue();

  m_ImageList->Clear();
}


/*****************************************************************************/
Role
InputImageListParameter
::GetDirection( std::size_t ) const
{
#if 0
  assert( i<m_InputImageParameterVector.size() );
  assert( !m_InputImageParameterVector[ i ].IsNull() );

  return m_InputImageParameterVector[ i ]->GetRole();

#else
  // otb::Parameter::GetRole() does not necessarily stand for
  // direction of parameter.
  return GetDirection();

#endif
}


/*****************************************************************************/
Role
InputImageListParameter
::GetDirection() const
{
  return Role_Input;
}


/*****************************************************************************/
const std::string &
InputImageListParameter
::GetFilenameFilter( std::size_t ) const
{
  return GetFilenameFilter();
}


/*****************************************************************************/
const std::string &
InputImageListParameter
::GetFilenameFilter() const
{
  return IMAGES_FILTER;
}


/*****************************************************************************/
const std::string &
InputImageListParameter
::ToString( const ParameterType::Pointer & p ) const
{
  assert( !p.IsNull() );

  return p->GetFileName();
}

/*****************************************************************************/
void
InputImageListParameter
::FromString( const ParameterType::Pointer & p,
	      const std::string & s ) const
{
  assert( !p.IsNull() );

  p->SetFromFileName( s );
}


}

}
