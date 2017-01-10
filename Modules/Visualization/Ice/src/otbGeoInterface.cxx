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
