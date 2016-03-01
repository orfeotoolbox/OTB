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

#include "otbOGRDataResampler.h"

namespace otb
{

void
OGRDataResampler
::Reset()
{
  m_NbPixelsGlobal = 0UL;
  m_ElmtsInClass.clear();
  m_Polygon.clear();
}

const OGRDataResampler::ClassCountMapType&
OGRDataResampler
::GetClassCountMap()
{
  return m_ElmtsInClass;
}
  
const OGRDataResampler::PolygonSizeMapType&
OGRDataResampler
::GetPolygonSizeMap()
{
  return m_Polygon;
}

unsigned long
OGRDataResampler
::GetNumberOfPixels()
{
  return m_NbPixelsGlobal;
}


} // end of namespace otb
