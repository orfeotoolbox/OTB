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

#ifndef __otbElevDatabaseHeightAboveMSLFunction_txx
#define __otbElevDatabaseHeightAboveMSLFunction_txx

#include "otbMacro.h"

#include "otbElevDatabaseHeightAboveMSLFunction.h"

#include "elevation/ossimElevManager.h"
#include "base/ossimGeoidManager.h"
#include "base/ossimFilename.h"
#include "base/ossimDirectory.h"
#include "base/ossimGeoidEgm96.h"
#include "base/ossimRefPtr.h"


namespace otb
{

/**
 * Constructor
 */
template <class TOutputPixel, class TCoordRep, unsigned int NPointDimension>
ElevDatabaseHeightAboveMSLFunction<TOutputPixel, TCoordRep, NPointDimension>
::ElevDatabaseHeightAboveMSLFunction() :
     m_ElevManager(ossimElevManager::instance())
{
  // Value defined in the norm for points SRTM doesn't have information.
  m_DefaultUnknownValue = static_cast<PixelType>(-32768);

}


/**
 * Standard "PrintSelf" method
 */
template <class TOutputPixel, class TCoordRep, unsigned int NPointDimension>
void
ElevDatabaseHeightAboveMSLFunction<TOutputPixel, TCoordRep, NPointDimension>
::PrintSelf(
  std::ostream& os,
  itk::Indent indent) const
{
  os << indent << " m_DefaultUnknownValue           : "  << m_DefaultUnknownValue << std::endl;

}

/**
 *
 */
template <class TOutputPixel, class TCoordRep, unsigned int NPointDimension>
typename ElevDatabaseHeightAboveMSLFunction<TOutputPixel, TCoordRep, NPointDimension>::PixelType
ElevDatabaseHeightAboveMSLFunction<TOutputPixel, TCoordRep, NPointDimension>
::Evaluate(const PointType& point) const
{
  double   height;
  ossimGpt ossimWorldPoint;
  ossimWorldPoint.lon = point[0];
  ossimWorldPoint.lat = point[1];
  height = m_ElevManager->getHeightAboveMSL(ossimWorldPoint);

  if (!ossim::isnan(height))
    {
    return static_cast<PixelType>( height );
    }
  else
    {
    // Back to the MNT default value
    return static_cast<PixelType>( m_DefaultUnknownValue );
    }

}

template <class TOutputPixel, class TCoordRep, unsigned int NPointDimension>
void
ElevDatabaseHeightAboveMSLFunction<TOutputPixel, TCoordRep, NPointDimension>
::OpenDEMDirectory(const char* DEMDirectory)
{
  ossimFilename ossimDEMDir;
  ossimDEMDir = ossimFilename(DEMDirectory);

  if (!m_ElevManager->loadElevationPath(ossimDEMDir))
    {
    itkExceptionMacro("Failed to open DEM Directory: " << ossimDEMDir);
    }
}

template <class TOutputPixel, class TCoordRep, unsigned int NPointDimension>
void
ElevDatabaseHeightAboveMSLFunction<TOutputPixel, TCoordRep, NPointDimension>
::SetDefaultHeightAboveEllipsoid(double h)
{
  m_ElevManager->setDefaultHeightAboveEllipsoid(h);
}


} // end namespace otb

#endif
