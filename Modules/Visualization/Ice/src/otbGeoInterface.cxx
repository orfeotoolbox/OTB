/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbGeoInterface.h"


namespace otb
{

class StaticInitializer
{
public:
  StaticInitializer() :
    m_Spacing()
  {
    m_Spacing.Fill( 1 );
  }

  const GeoInterface::Spacing2 &
  GetSpacing() const
  {
    return m_Spacing;
  }

private:
  GeoInterface::Spacing2 m_Spacing;
};


static const StaticInitializer INITIALIZER;


GeoInterface
::GeoInterface()
{
}


GeoInterface
::~GeoInterface()
{
}


bool
GeoInterface
::TransformFromViewport( Point2d &,
                         const Point2d &,
                         bool ) const
{
  return false;
}

#if 0

bool
GeoInterface
::TransformFromViewport( Point2f & out,
                         const Point2f & in,
                         bool isPhysical ) const
{
  Point2d p;

  if( !TransformFromViewport( p, Point2d( in ), isPhysical ) )
    return false;

  out = p;

  out[ 0 ] = p[ 0 ];
  out[ 1 ] = p[ 1 ];

  return true;
}

#endif

bool
GeoInterface
::TransformToViewport( Point2d &,
                       const Point2d &,
                       bool ) const
{
  return false;
}


#if 0

bool
GeoInterface
::TransformToViewport( Point2f & out,
                       const Point2f & in,
                       bool isPhysical ) const
{
  Point2d p;

  if( !TransformToViewport( p, Point2d( in ), isPhysical ) )
    return false;
  
  out = p;

  out[ 0 ] = p[ 0 ];
  out[ 1 ] = p[ 1 ];

  return true;
}

#endif


const GeoInterface::Spacing2 &
GeoInterface
::GetSpacing() const
{
  // By default, spacing is (1, 1) if sub-class has no scale-related
  // spacing.
  //
  // This method is especially useful in otb::GlView::ZoomToFull() in
  // order to correct transformed zooming factors.

  return INITIALIZER.GetSpacing();
}


std::string
GeoInterface
::GetWkt() const
{
  return std::string();
}


bool
GeoInterface
::HasKwl() const
{
  return false;
}


bool
GeoInterface
::GetKwl( ImageKeywordlist & ) const
{
  return false;
}


} // End namespace 'otb'.
