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
#include "otbPipelineMemoryPrintCalculator.h"
#include "itkExtractImageFilter.h"

#include "otbImageRegionSquareTileSplitter.h"
#include "itkImageRegionSplitter.h"

namespace otb
{

template <class TImage>
StreamingManager<TImage>::StreamingManager()
  : m_DesiredMode(StreamingManagement::TILED_AVAILABLE_RAM),
    m_ActualMode(StreamingManagement::TILED_AVAILABLE_RAM),
    m_AvailableRAMInMB(0),
    m_DesiredNumberOfLines(0),
    m_DesiredTileDimension(0),
    m_ComputedNumberOfSplits(0)
{
}

template <class TImage>
StreamingManager<TImage>::~StreamingManager()
{
}

template <class TImage>
void
StreamingManager<TImage>::SetStrippedRAMStreamingMode( unsigned int availableRAMInMB )
{
  m_DesiredMode = StreamingManagement::STRIPPED_AVAILABLE_RAM;
  m_AvailableRAMInMB = availableRAMInMB;
}

template <class TImage>
void
StreamingManager<TImage>::SetStrippedNumberOfLinesStreamingMode( unsigned int numberOfLines )
{
  m_DesiredMode = StreamingManagement::STRIPPED_SET_NUMBEROFLINES;
  m_DesiredNumberOfLines = numberOfLines;
}

template <class TImage>
void
StreamingManager<TImage>::SetTiledRAMStreamingMode( unsigned int availableRAMInMB )
{
  otbMsgDevMacro(<< "StreamingManager::SetTiledRAMStreamingMode " << availableRAMInMB)
  m_DesiredMode = StreamingManagement::TILED_AVAILABLE_RAM;
  m_AvailableRAMInMB = availableRAMInMB;
}

template <class TImage>
void
StreamingManager<TImage>::SetTiledTileDimensionStreamingMode( unsigned int tileDimension )
{
  m_DesiredMode = StreamingManagement::TILED_SET_TILE_SIZE;
  m_DesiredTileDimension = tileDimension;
}

template <class TImage>
void
StreamingManager<TImage>::PrepareStreaming( itk::DataObject * input, const RegionType &region )
{
  switch (m_DesiredMode)
  {
    case StreamingManagement::STRIPPED_AVAILABLE_RAM:
      {
      otbMsgDevMacro(<< "Activating STRIPPED_AVAILABLE_RAM streaming mode")
      unsigned long nbDivisions = EstimateOptimalNumberOfDivisions(input, region);
      m_Splitter = itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
      m_ComputedNumberOfSplits = m_Splitter->GetNumberOfSplits(region, nbDivisions);

      otbMsgDevMacro(<< "Number of split : " << m_ComputedNumberOfSplits)
      }
      break;

    case StreamingManagement::STRIPPED_SET_NUMBEROFLINES:
      {
      otbMsgDevMacro(<< "Activating STRIPPED_SET_NUMBEROFLINES streaming mode")
      if (m_DesiredNumberOfLines < 1)
        {
        itkWarningMacro(<< "DesiredNumberOfLines set to 0 : use 1 as strip number of lines")
        m_DesiredNumberOfLines = 1;
        }

      /* Calculate number of split */
      unsigned long numberLinesOfRegion = region.GetSize()[1]; // Y dimension
      unsigned long nbSplit;
      if (numberLinesOfRegion > m_DesiredNumberOfLines)
        {
        nbSplit =
          static_cast<unsigned long>(vcl_ceil(static_cast<double>(numberLinesOfRegion) /
                                              static_cast<double>(m_DesiredNumberOfLines)));
        }
      else
        {
        // Don't stream
        nbSplit = 1;
        }

      m_Splitter = itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
      m_ComputedNumberOfSplits = m_Splitter->GetNumberOfSplits(region, nbSplit);
      otbMsgDevMacro(<< "Number of split : " << m_ComputedNumberOfSplits)
      }
      break;

    case StreamingManagement::TILED_AVAILABLE_RAM:
      {
      otbMsgDevMacro(<< "Activating TILED_AVAILABLE_RAM streaming mode")
      unsigned long nbDivisions = EstimateOptimalNumberOfDivisions(input, region);
      m_Splitter = otb::ImageRegionSquareTileSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
      m_ComputedNumberOfSplits = m_Splitter->GetNumberOfSplits(region, nbDivisions);
      otbMsgDevMacro(<< "Number of split : " << m_ComputedNumberOfSplits)
      }
      break;

    case StreamingManagement::TILED_SET_TILE_SIZE:
      {
      otbMsgDevMacro(<< "Activating TILED_SET_TILE_SIZE streaming mode")
      if (m_DesiredTileDimension < 16)
        {
        itkWarningMacro(<< "DesiredTileDimension inferior to 16 : use 16 as tile dimension")
        m_DesiredTileDimension = 16;
        }

      // Calculate number of split
      m_Splitter = otb::ImageRegionSquareTileSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
      unsigned int nbDesiredTiles = itk::Math::Ceil<unsigned int>( double(region.GetNumberOfPixels()) / (m_DesiredTileDimension * m_DesiredTileDimension) );
      m_ComputedNumberOfSplits = m_Splitter->GetNumberOfSplits(region, nbDesiredTiles);
      otbMsgDevMacro(<< "Number of split : " << m_ComputedNumberOfSplits)
      }
      break;
  }

  // Save the region to generate the splits later
  m_Region = region;
}


template <class TImage>
unsigned int
StreamingManager<TImage>::EstimateOptimalNumberOfDivisions(itk::DataObject * input, const RegionType &region)
{
  otbMsgDevMacro(<< "m_AvailableRAMInMB " << m_AvailableRAMInMB)
  unsigned int availableRAMInBytes = m_AvailableRAMInMB * 1024 * 1024;

  if (availableRAMInBytes == 0)
    {
    otbMsgDevMacro(<< "Retrieving available RAM size from configuration")
    // Retrieve it from the configuration
    try
      {
      typedef otb::ConfigurationFile ConfigurationType;
      ConfigurationType::Pointer conf = ConfigurationType::GetInstance();

      availableRAMInBytes = conf->GetParameter<unsigned int>(
        "OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING");
      }
    catch(...)
      {
      // We should never have to go here if the configuration file is
      // correct and found.
      // In case it is not fallback on the cmake
      // defined constants.
      availableRAMInBytes = OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING;
      }
    }

  otbMsgDevMacro("RAM used to estimate memory footprint : " << availableRAMInBytes / 1024 / 1024  << " MB")

  otb::PipelineMemoryPrintCalculator::Pointer memoryPrintCalculator;
  memoryPrintCalculator = otb::PipelineMemoryPrintCalculator::New();

  memoryPrintCalculator->SetAvailableMemory( availableRAMInBytes );

  // Trick to avoid having the resampler compute the whole
  // deformation field
  double regionTrickFactor = 1;
  ImageType* inputImage = dynamic_cast<ImageType*>(input);
  //inputImage = 0;
  if (inputImage)
    {

    typedef itk::ExtractImageFilter<ImageType,ImageType> ExtractFilterType;
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
      memoryPrintCalculator->SetDataToWrite(extractFilter->GetOutput());
      regionTrickFactor = static_cast<double>( region.GetNumberOfPixels() )
          / static_cast<double>(smallRegion.GetNumberOfPixels());

      memoryPrintCalculator->SetBiasCorrectionFactor(regionTrickFactor);
      }
    else
      {
      otbMsgDevMacro("Using the input region to estimate memory : " << region)
      // the region is not well behaved
      // use the full region
      memoryPrintCalculator->SetDataToWrite(input);
      memoryPrintCalculator->SetBiasCorrectionFactor(1.0);
      }

    memoryPrintCalculator->Compute();
    }
  else
    {
    // Use the original object to estimate memory footprint
    memoryPrintCalculator->SetDataToWrite(input);
    memoryPrintCalculator->SetBiasCorrectionFactor(1.0);

    memoryPrintCalculator->Compute();
    }

  otbMsgDevMacro( "Estimated Memory print for the full image : "
                   << static_cast<unsigned int>(memoryPrintCalculator->GetMemoryPrint() / 1024 / 1024 ) << std::endl)
  otbMsgDevMacro( "Optimal number of stream divisions: "
                   << memoryPrintCalculator->GetOptimalNumberOfStreamDivisions() << std::endl)

  return memoryPrintCalculator->GetOptimalNumberOfStreamDivisions();
}

template <class TImage>
StreamingManagement::StreamingMode
StreamingManager<TImage>::GetStreamingMode()
{
  return m_ActualMode;
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
  return m_Splitter->GetSplit(i, m_ComputedNumberOfSplits, m_Region);
}

} // End namespace otb

#endif
