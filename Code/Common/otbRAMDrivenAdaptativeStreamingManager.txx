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
#ifndef __otbRAMDrivenAdaptativeStreamingManager_txx
#define __otbRAMDrivenAdaptativeStreamingManager_txx

#include "otbRAMDrivenAdaptativeStreamingManager.h"
#include "otbMacro.h"
#include "otbImageRegionAdaptativeSplitter.h"

namespace otb
{

template <class TImage>
RAMDrivenAdaptativeStreamingManager<TImage>::RAMDrivenAdaptativeStreamingManager()
  : m_AvailableRAMInMB(0),
    m_Bias(1.0)
{
}

template <class TImage>
RAMDrivenAdaptativeStreamingManager<TImage>::~RAMDrivenAdaptativeStreamingManager()
{
}

template <class TImage>
void
RAMDrivenAdaptativeStreamingManager<TImage>::PrepareStreaming( itk::DataObject * input, const RegionType &region )
{
  unsigned long nbDivisions =
      this->EstimateOptimalNumberOfDivisions(input, region, m_AvailableRAMInMB, m_Bias);

  this->m_Splitter = otb::ImageRegionAdaptativeSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbDivisions);
  otbMsgDevMacro(<< "Number of split : " << this->m_ComputedNumberOfSplits)
  this->m_Region = region;
}

} // End namespace otb

#endif

