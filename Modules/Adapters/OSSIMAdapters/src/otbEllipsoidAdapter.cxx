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

#include "otbEllipsoidAdapter.h"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#include "ossim/base/ossimEllipsoid.h"
#pragma GCC diagnostic pop
#else
#include "ossim/base/ossimEllipsoid.h"
#endif



namespace otb
{

EllipsoidAdapter::EllipsoidAdapter()
{
  m_Ellipsoid = new ossimEllipsoid();
}

EllipsoidAdapter::~EllipsoidAdapter()
{
  if (m_Ellipsoid != NULL)
    {
    delete m_Ellipsoid;
    }
}

void
EllipsoidAdapter::XYZToLonLatHeight(double x, double y, double z,
                                    double& lon, double& lat, double& h) const
{
   // Note the lat/lon convension for ossim vs lon/lat for OTB
  m_Ellipsoid->XYZToLatLonHeight(x, y, z, lat, lon, h);
}

void
EllipsoidAdapter::LonLatHeightToXYZ(double lon, double lat, double h,
                                    double& x, double& y, double& z) const
{
  // Note the lat/lon convension for ossim vs lon/lat for OTB
  m_Ellipsoid->latLonHeightToXYZ(lat, lon, h, x, y, z);
}

} // namespace otb
