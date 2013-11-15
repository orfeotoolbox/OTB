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
#ifndef __otbStreamingManager_txx
#define __otbStreamingManager_txx

#include "otbStreamingManager.h"
#include "otbMacro.h"
#include "otbConfigure.h"
#include "otbConfigurationFile.h"
#include "itkExtractImageFilter.h"

namespace otb
{

template <class TImage>
StreamingManager<TImage>::StreamingManager()
  : m_ComputedNumberOfSplits(0)
{
}

template <class TImage>
StreamingManager<TImage>::~StreamingManager()
{
}

template <class TImage>
typename StreamingManager<TImage>::MemoryPrintType
StreamingManager<TImage>::GetActualAvailableRAMInBytes(MemoryPrintType availableRAMInMB)
{
  MemoryPrintType availableRAMInBytes = availableRAMInMB * 1024 * 1024;

  if (availableRAMInBytes == 0)
    {
    otbMsgDevMacro(<< "Retrieving available RAM size from configuration")
    // Retrieve it from the configuration
    try
      {
      typedef otb::ConfigurationFile ConfigurationType;
      ConfigurationType::Pointer conf = ConfigurationType::GetInstance();

      availableRAMInBytes = conf->GetParameter<MemoryPrintType>(
          "OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING");
      }
    catch(...)
      {
      // We should never have to go here if the configuration file is
      // correct and found.
      // In case it is not, fallback on the cmake
      // defined constants.
      availableRAMInBytes = OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING;
      }
    }

  otbMsgDevMacro("RAM used to estimate memory footprint : " << availableRAMInBytes / 1024 / 1024  << " MB")
  return availableRAMInBytes;
}

template <class TImage>
unsigned int
StreamingManager<TImage>::EstimateOptimalNumberOfDivisions(itk::DataObject * input, const RegionType &region,
                                                           MemoryPrintType availableRAM,
                                                           double bias)
{
  otbMsgDevMacro(<< "availableRAM " << availableRAM)

  MemoryPrintType availableRAMInBytes = GetActualAvailableRAMInBytes(availableRAM);

  otb::PipelineMemoryPrintCalculator::Pointer memoryPrintCalculator;
  memoryPrintCalculator = otb::PipelineMemoryPrintCalculator::New();

  // Trick to avoid having the resampler compute the whole
  // displacement field
  double     regionTrickFactor = 1;
  ImageType* inputImage = dynamic_cast<ImageType*>(input);

  MemoryPrintType pipelineMemoryPrint;
  if (inputImage)
    {

    typedef itk::ExtractImageFilter<ImageType, ImageType> ExtractFilterType;
    typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
    extractFilter->SetInput(inputImage);

    // Define a small region to run the memory footprint estimation,
    // around the image center, 100 pixels wide in each dimension
    SizeType smallSize;
    smallSize.Fill(100);
    IndexType index;
    index[0] = region.GetIndex()[0] + region.GetSize()[0]/2 - 50;
    index[1] = region.GetIndex()[1] + region.GetSize()[1]/2 - 50;

    RegionType smallRegion;
    smallRegion.SetSize(smallSize);
    smallRegion.SetIndex(index);

    // In case the image is smaller than 100 pixels in a direction
    smallRegion.Crop(region);

    extractFilter->SetExtractionRegion(smallRegion);

    bool smallRegionSuccess = smallRegion.Crop(region);

    if (smallRegionSuccess)
      {
      otbMsgDevMacro("Using an extract to estimate memory : " << smallRegion)
      // the region is well behaved, inside the largest possible region
      memoryPrintCalculator->SetDataToWrite(extractFilter->GetOutput() );

      regionTrickFactor = static_cast<double>( region.GetNumberOfPixels() )
        / static_cast<double>(smallRegion.GetNumberOfPixels() );

      memoryPrintCalculator->SetBiasCorrectionFactor(regionTrickFactor * bias);
      }
    else
      {
      otbMsgDevMacro("Using the input region to estimate memory : " << region)
      // the region is not well behaved
      // use the full region
      memoryPrintCalculator->SetDataToWrite(input);
      memoryPrintCalculator->SetBiasCorrectionFactor(bias);
      }

    memoryPrintCalculator->Compute();

    pipelineMemoryPrint = memoryPrintCalculator->GetMemoryPrint();

    if (smallRegionSuccess)
      {
      // remove the contribution of the ExtractImageFilter
      MemoryPrintType extractContrib =
          memoryPrintCalculator->EvaluateDataObjectPrint(extractFilter->GetOutput());

      pipelineMemoryPrint -= extractContrib;
      }
    }
  else
    {
    // Use the original object to estimate memory footprint
    memoryPrintCalculator->SetDataToWrite(input);
    memoryPrintCalculator->SetBiasCorrectionFactor(1.0);

    memoryPrintCalculator->Compute();

    pipelineMemoryPrint = memoryPrintCalculator->GetMemoryPrint();
    }

  unsigned int optimalNumberOfDivisions =
      otb::PipelineMemoryPrintCalculator::EstimateOptimalNumberOfStreamDivisions(pipelineMemoryPrint, availableRAMInBytes);

  otbMsgDevMacro( "Estimated Memory print for the full image : "
                  << static_cast<unsigned int>(pipelineMemoryPrint * otb::PipelineMemoryPrintCalculator::ByteToMegabyte ) << std::endl)
  otbMsgDevMacro( "Optimal number of stream divisions: "
                  << optimalNumberOfDivisions << std::endl)

  return optimalNumberOfDivisions;
}

template <class TImage>
unsigned int
StreamingManager<TImage>::GetNumberOfSplits()
{
  return m_ComputedNumberOfSplits;
}

template <class TImage>
typename StreamingManager<TImage>::RegionType
StreamingManager<TImage>::GetSplit(unsigned int i)
{
  typename StreamingManager<TImage>::RegionType region( m_Region );
  m_Splitter->GetSplit(i, m_ComputedNumberOfSplits, region);
  return region;
}

} // End namespace otb

#endif

