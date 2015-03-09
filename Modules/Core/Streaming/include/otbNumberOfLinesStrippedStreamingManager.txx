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
#ifndef __otbNumberOfLinesStrippedStreamingManager_txx
#define __otbNumberOfLinesStrippedStreamingManager_txx

#include "otbNumberOfLinesStrippedStreamingManager.h"
#include "otbMacro.h"

namespace otb
{

template <class TImage>
NumberOfLinesStrippedStreamingManager<TImage>::NumberOfLinesStrippedStreamingManager()
  : m_NumberOfLinesPerStrip(0)
{
}

template <class TImage>
NumberOfLinesStrippedStreamingManager<TImage>::~NumberOfLinesStrippedStreamingManager()
{
}

template <class TImage>
void
NumberOfLinesStrippedStreamingManager<TImage>::PrepareStreaming( itk::DataObject * /*input*/, const RegionType &region )
{
  otbMsgDevMacro(<< "Activating STRIPPED_SET_NUMBEROFLINES streaming mode")
  if (m_NumberOfLinesPerStrip < 1)
    {
    itkWarningMacro(<< "DesiredNumberOfLines set to 0 : streaming disabled")
    }

  // Calculate number of splits
  unsigned long numberLinesOfRegion = region.GetSize()[1]; // Y dimension
  unsigned long nbSplit;
  if (numberLinesOfRegion > m_NumberOfLinesPerStrip && m_NumberOfLinesPerStrip > 0)
    {
    nbSplit =
      static_cast<unsigned long>(vcl_ceil(static_cast<double>(numberLinesOfRegion) /
                                          static_cast<double>(m_NumberOfLinesPerStrip) ) );
    }
  else
    {
    // Don't stream
    nbSplit = 1;
    }

  this->m_Splitter = itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbSplit);
  otbMsgDevMacro(<< "Computed number of split : " << this->m_ComputedNumberOfSplits)

  // Save the region to generate the splits later
  this->m_Region = region;
}

} // End namespace otb

#endif

