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

#include "otbPolygonClassStatisticsAccumulator.h"

namespace otb
{

void
PolygonClassStatisticsAccumulator
::Reset()
{
  m_NbPixelsGlobal = 0UL;
  m_ElmtsInClass.clear();
  m_Polygon.clear();
}

const PolygonClassStatisticsAccumulator::ClassCountMapType&
PolygonClassStatisticsAccumulator
::GetClassCountMap()
{
  return m_ElmtsInClass;
}
  
const PolygonClassStatisticsAccumulator::PolygonSizeMapType&
PolygonClassStatisticsAccumulator
::GetPolygonSizeMap()
{
  return m_Polygon;
}

unsigned long
PolygonClassStatisticsAccumulator
::GetNumberOfPixels()
{
  return m_NbPixelsGlobal;
}


} // end of namespace otb
