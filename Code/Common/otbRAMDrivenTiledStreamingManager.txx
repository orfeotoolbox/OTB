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
#ifndef __otbRAMDrivenTiledStreamingManager_txx
#define __otbRAMDrivenTiledStreamingManager_txx

#include "otbRAMDrivenTiledStreamingManager.h"
#include "otbMacro.h"
#include "otbImageRegionSquareTileSplitter.h"

namespace otb
{

template <class TImage>
RAMDrivenTiledStreamingManager<TImage>::RAMDrivenTiledStreamingManager()
  : m_AvailableRAMInMB(0),
    m_Bias(1.0)
{
}

template <class TImage>
RAMDrivenTiledStreamingManager<TImage>::~RAMDrivenTiledStreamingManager()
{
}

template <class TImage>
void
RAMDrivenTiledStreamingManager<TImage>::PrepareStreaming( itk::DataObject * input, const RegionType &region )
{
  unsigned long nbDivisions =
      this->EstimateOptimalNumberOfDivisions(input, region, m_AvailableRAMInMB, m_Bias);

  this->m_Splitter = otb::ImageRegionSquareTileSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbDivisions);
  otbMsgDevMacro(<< "Number of split : " << this->m_ComputedNumberOfSplits)
  this->m_Region = region;
}

} // End namespace otb

#endif

