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
#ifndef __otbRAMDrivenStrippedStreamingManager_txx
#define __otbRAMDrivenStrippedStreamingManager_txx

#include "otbRAMDrivenStrippedStreamingManager.h"
#include "otbMacro.h"

namespace otb
{

template <class TImage>
RAMDrivenStrippedStreamingManager<TImage>::RAMDrivenStrippedStreamingManager()
  : m_AvailableRAMInMB(0),
    m_Bias(1.0)
{
}

template <class TImage>
RAMDrivenStrippedStreamingManager<TImage>::~RAMDrivenStrippedStreamingManager()
{
}

template <class TImage>
void
RAMDrivenStrippedStreamingManager<TImage>::PrepareStreaming( itk::DataObject * input, const RegionType &region )
{
  unsigned long nbDivisions =
      this->EstimateOptimalNumberOfDivisions(input, region, m_AvailableRAMInMB, m_Bias);

  this->m_Splitter = itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbDivisions);
  this->m_Region = region;
}

} // End namespace otb

#endif

