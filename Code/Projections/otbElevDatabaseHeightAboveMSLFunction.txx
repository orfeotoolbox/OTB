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
template <class TOutput, class TCoordRep>
ElevDatabaseHeightAboveMSLFunction<TOutput, TCoordRep>
::ElevDatabaseHeightAboveMSLFunction() :
     m_ElevManager(ossimElevManager::instance())
{
  // Value defined in the norm for points SRTM doesn't have information.
  m_DefaultUnknownValue = static_cast<PixelType>(-32768);

}


/**
 * Standard "PrintSelf" method
 */
template <class TOutput, class TCoordRep>
void
ElevDatabaseHeightAboveMSLFunction<TOutput, TCoordRep>
::PrintSelf(
  std::ostream& os,
  itk::Indent indent) const
{
  os << indent << " m_DefaultUnknownValue           : "  << m_DefaultUnknownValue << std::endl;

}

/**
 *
 */
template <class TOutput, class TCoordRep>
typename ElevDatabaseHeightAboveMSLFunction<TOutput, TCoordRep>::PixelType
ElevDatabaseHeightAboveMSLFunction<TOutput, TCoordRep>
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

template <class TOutput, class TCoordRep>
void
ElevDatabaseHeightAboveMSLFunction<TOutput, TCoordRep>
::OpenDEMDirectory(const char* DEMDirectory)
{
  ossimFilename ossimDEMDir;
  ossimDEMDir = ossimFilename(DEMDirectory);

  if (!m_ElevManager->loadElevationPath(ossimDEMDir))
    {
    itkExceptionMacro("Failed to open DEM Directory: " << ossimDEMDir);
    }
}

template <class TOutput, class TCoordRep>
void
ElevDatabaseHeightAboveMSLFunction<TOutput, TCoordRep>
::SetDefaultHeightAboveEllipsoid(double h)
{
  m_ElevManager->setDefaultHeightAboveEllipsoid(h);
}


} // end namespace otb

#endif
