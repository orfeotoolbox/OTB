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
#include "itkMetaDataObject.h"

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

  typename otb::ImageRegionAdaptativeSplitter<itkGetStaticConstMacro(ImageDimension)>::SizeType tileHint;

  unsigned int tileHintX, tileHintY;

  itk::ExposeMetaData<unsigned int>(input->GetMetaDataDictionary(),
                                    MetaDataKey::TileHintX,
                                    tileHintX);

  itk::ExposeMetaData<unsigned int>(input->GetMetaDataDictionary(),
                                    MetaDataKey::TileHintY,
                                    tileHintY);

  tileHint[0] = tileHintX;
  tileHint[1] = tileHintY;

  typename otb::ImageRegionAdaptativeSplitter<itkGetStaticConstMacro(ImageDimension)>::Pointer splitter =
      otb::ImageRegionAdaptativeSplitter<itkGetStaticConstMacro(ImageDimension)>::New();

  splitter->SetTileHint(tileHint);

  this->m_Splitter = splitter;

  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbDivisions);
  otbMsgDevMacro(<< "Number of split : " << this->m_ComputedNumberOfSplits)
  this->m_Region = region;
}

} // End namespace otb

#endif
