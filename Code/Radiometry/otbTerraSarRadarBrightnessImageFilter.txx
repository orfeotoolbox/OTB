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
#ifndef __otbTerraSarRadarBrightnessImageFilter_txx
#define __otbTerraSarRadarBrightnessImageFilter_txx

#include "otbTerraSarRadarBrightnessImageFilter.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbImageMetadataInterfaceBase.h"


namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
TerraSarRadarBrightnessImageFilter<TInputImage,TOutputImage>
::TerraSarRadarBrightnessImageFilter()
{
//  m_CalFactor = 1.;
}

template <class TInputImage, class TOutputImage>
void
TerraSarRadarBrightnessImageFilter<TInputImage,TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
}

}

#endif
