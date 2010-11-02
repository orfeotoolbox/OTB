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

#ifndef __otbSRTMHeightAboveMSLFunction_txx
#define __otbSRTMHeightAboveMSLFunction_txx

#include "otbMacro.h"

#include "otbSRTMHeightAboveMSLFunction.h"

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
SRTMHeightAboveMSLFunction<TOutput, TCoordRep>
::SRTMHeightAboveMSLFunction() :
     m_ElevManager(ossimElevManager::instance())
{
}


/**
 * Standard "PrintSelf" method
 */
template <class TOutput, class TCoordRep>
void
SRTMHeightAboveMSLFunction<TOutput, TCoordRep>
::PrintSelf(
  std::ostream& os,
  itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}

/**
 *
 */
template <class TOutput, class TCoordRep>
typename SRTMHeightAboveMSLFunction<TOutput, TCoordRep>::OutputImageType
SRTMHeightAboveMSLFunction<TOutput, TCoordRep>
::Evaluate(const PointType& point) const
{
  double   height;
  ossimGpt ossimWorldPoint;
  ossimWorldPoint.lon = point[0];
  ossimWorldPoint.lat = point[1];
  height = m_ElevManager->getHeightAboveMSL(ossimWorldPoint);

  return static_cast<OutputImageType>( height );
}

template <class TOutput, class TCoordRep>
void
SRTMHeightAboveMSLFunction<TOutput, TCoordRep>
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
SRTMHeightAboveMSLFunction<TOutput, TCoordRep>
::SetDefaultHeightAboveEllipsoid(double h)
{
  m_ElevManager->setDefaultHeightAboveEllipsoid(h);
}


} // end namespace otb

#endif
