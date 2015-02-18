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
#ifndef __otbTileDimensionTiledStreamingManager_txx
#define __otbTileDimensionTiledStreamingManager_txx

#include "otbTileDimensionTiledStreamingManager.h"
#include "otbMacro.h"
#include "otbImageRegionSquareTileSplitter.h"

namespace otb
{

template <class TImage>
TileDimensionTiledStreamingManager<TImage>::TileDimensionTiledStreamingManager()
  : m_TileDimension(0)
{
}

template <class TImage>
TileDimensionTiledStreamingManager<TImage>::~TileDimensionTiledStreamingManager()
{
}

template <class TImage>
void
TileDimensionTiledStreamingManager<TImage>::PrepareStreaming( itk::DataObject * /*input*/, const RegionType &region )
{
  if (m_TileDimension < 16)
    {
    itkWarningMacro(<< "TileDimension inferior to 16 : using 16 as tile dimension")
    m_TileDimension = 16;
    }

  // Calculate number of split
  this->m_Splitter = otb::ImageRegionSquareTileSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
  unsigned int nbDesiredTiles =
    itk::Math::Ceil<unsigned int>( double(region.GetNumberOfPixels() ) / (m_TileDimension * m_TileDimension) );
  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbDesiredTiles);
  this->m_Region = region;
}

} // End namespace otb

#endif

