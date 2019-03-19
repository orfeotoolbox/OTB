/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
  return
    FromData(
      parameter,
      image,
      []( auto p, auto i ) -> void
      {
        assert( p );

	p->SetImage( i );
      },
      "Image filename"
    );
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
  return
    GetObjectList(
      m_ImageList,
      []( auto param ) -> auto
      {
        assert( param );

        return param->GetFloatVectorImage();
      }
    );
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
  assert( imList!=nullptr );
  assert( !m_ImageList.IsNull() );

  SetObjectList(
    *m_ImageList,
    *imList,
    [ this ]( auto p, auto image ) -> auto
    {
      this->FromImage( p, image );
    },
    //
    []( auto p ) -> auto
    {
      assert( p );

      return p->GetFloatVectorImage();
    }
  );
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
  AddData(
    image,
    [ this ]( auto i ) -> auto
    {
      return this->FromImage( i );
    }
  );
}


/*****************************************************************************/
void
InputImageListParameter
::ClearValue()
{
  Superclass::ClearValue();

  assert( m_ImageList );

  m_ImageList->Clear();
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
::GetFilenameFilter() const
{
  return IMAGES_FILTER;
}

}

}
