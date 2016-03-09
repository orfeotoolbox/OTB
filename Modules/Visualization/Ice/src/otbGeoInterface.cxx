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
#include "otbGeoInterface.h"


namespace otb
{


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
