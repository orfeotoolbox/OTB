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
#ifndef __otbScalarImageToPanTexTextureFilter_txx
#define __otbScalarImageToPanTexTextureFilter_txx

#include "otbScalarImageToPanTexTextureFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "itkNumericTraits.h"

namespace otb
{
template <class TInputImage, class TOutputImage>
ScalarImageToPanTexTextureFilter<TInputImage, TOutputImage>
::ScalarImageToPanTexTextureFilter() : m_Radius(),
  m_NumberOfBinsPerAxis(8),
  m_HistSize(2),
  m_InputImageMinimum(0),
  m_InputImageMaximum(255)
{
  // There are 1 output corresponding to the Pan Tex texture indice
  this->SetNumberOfRequiredOutputs(1);

  //Fill the offset list for contrast computation
  OffsetType off;
  off[0] = 0;
  off[1] = 1;
  m_OffsetList.push_back(off);   //(0, 1)
  off[1] = 2;
  m_OffsetList.push_back(off);   //(0, 2)
  off[0] = 1;
  off[1] = -2;
  m_OffsetList.push_back(off);   //(1, -2)
  off[1] = -1;
  m_OffsetList.push_back(off);   //(1, -1)
  off[1] = 0;
  m_OffsetList.push_back(off);   //(1, 0)
  off[1] = 1;
  m_OffsetList.push_back(off);   //(1, 1)
  off[1] = 2;
  m_OffsetList.push_back(off);   //(1, 2)
  off[0] = 2;
  off[1] = -1;
  m_OffsetList.push_back(off);   //(2, -1)
  off[1] = 0;
  m_OffsetList.push_back(off);   //(2, 0)
  off[1] = 1;
  m_OffsetList.push_back(off);   //(2, 1)
}

template <class TInputImage, class TOutputImage>
ScalarImageToPanTexTextureFilter<TInputImage, TOutputImage>
::~ScalarImageToPanTexTextureFilter()
{}

template <class TInputImage, class TOutputImage>
void
ScalarImageToPanTexTextureFilter<TInputImage, TOutputImage>
::SetBinsAndMinMax(unsigned int numberOfBinsPerAxis,
                   InputPixelType inputImageMinimum,
                   InputPixelType inputImageMaximum)
{
  /** Initalize m_Histogram with given min, max and number of bins  **/
  MeasurementVectorType lowerBound;
  MeasurementVectorType upperBound;
  m_InputImageMinimum = inputImageMinimum;
  m_InputImageMaximum = inputImageMaximum;
  m_NumberOfBinsPerAxis = numberOfBinsPerAxis;
  m_Histogram = HistogramType::New();
  m_Histogram->SetMeasurementVectorSize( MeasurementVectorSize );
  lowerBound.SetSize( MeasurementVectorSize );
  upperBound.SetSize( MeasurementVectorSize );
  lowerBound.Fill(m_InputImageMinimum);
  upperBound.Fill(m_InputImageMaximum+1);
  m_HistSize.Fill(m_NumberOfBinsPerAxis);
  m_Histogram->Initialize(m_HistSize, lowerBound, upperBound);
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToPanTexTextureFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // First, call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr = const_cast<InputImageType *>(this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }

  // Retrieve the output requested region
  // We use only the first output since requested regions for all outputs are enforced to be equal
  // by the default GenerateOutputRequestedRegiont() implementation
  OutputRegionType outputRequestedRegion = outputPtr->GetRequestedRegion();

  // Build the input requested region
  InputRegionType inputRequestedRegion = outputRequestedRegion;

  // Apply the radius
  SizeType maxOffsetSize;
  maxOffsetSize[0] = 2;
  maxOffsetSize[1] = 2;
  inputRequestedRegion.PadByRadius(m_Radius + maxOffsetSize);

  // Try to apply the requested region to the input image
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    }
  else
    {
    // Build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToPanTexTextureFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve the input and output pointers
  InputImagePointerType inputPtr = const_cast<InputImageType *> (this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();

  itk::ImageRegionIteratorWithIndex<OutputImageType> outputIt(outputPtr, outputRegionForThread);

  // Go to begin
  outputIt.GoToBegin();

  // Set-up progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Iterate on outputs to compute textures
  while (!outputIt.IsAtEnd())
    {
    // Initialise output value
    double out = itk::NumericTraits<double>::max();

    // For each offset
    typename OffsetListType::const_iterator offIt;
    for (offIt = m_OffsetList.begin(); offIt != m_OffsetList.end(); ++offIt)
      {
      OffsetType currentOffset = *offIt;

      // Compute the region on which co-occurence will be estimated
      typename InputRegionType::IndexType inputIndex;
      typename InputRegionType::SizeType inputSize;

      // First, create an window for neighborhood iterator based on m_Radius
      // For example, if xradius and yradius is 2. window size is 5x5 (2 *
      // radius + 1).
      for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
        {
        inputIndex[dim] = outputIt.GetIndex()[dim] - m_Radius[dim];
        inputSize[dim] = 2 * m_Radius[dim] + 1;
        }
      // Build the input  region
      InputRegionType inputRegion;
      inputRegion.SetIndex(inputIndex);
      inputRegion.SetSize(inputSize);
      inputRegion.Crop(inputPtr->GetRequestedRegion());

      CooccurrenceIndexedListPointerType m_GLCIList = CooccurrenceIndexedListType::New();
      m_GLCIList->Initialize(m_HistSize);

      SizeType neighborhoodRadius;
      /** calulate minimum offset and set it as neigborhood radius **/
      unsigned int minRadius = 0;
      for ( unsigned int i = 0; i < currentOffset.GetOffsetDimension(); i++ )
        {
        unsigned int distance = vcl_abs(currentOffset[i]);
        if ( distance > minRadius )
          {
          minRadius = distance;
          }
        }
      neighborhoodRadius.Fill(minRadius);

      typedef itk::ConstNeighborhoodIterator< InputImageType > NeighborhoodIteratorType;
      NeighborhoodIteratorType neighborIt;
      neighborIt = NeighborhoodIteratorType(neighborhoodRadius, inputPtr, inputRegion);
      for ( neighborIt.GoToBegin(); !neighborIt.IsAtEnd(); ++neighborIt )
        {
        const InputPixelType centerPixelIntensity = neighborIt.GetCenterPixel();
        if ( centerPixelIntensity < m_InputImageMinimum
             || centerPixelIntensity > m_InputImageMaximum )
          {
          continue; // don't put a pixel in the histogram if the value
          // is out-of-bounds.
          }

        bool            pixelInBounds;
        const InputPixelType pixelIntensity =
          neighborIt.GetPixel(currentOffset, pixelInBounds);

        if ( !pixelInBounds )
          {
          continue; // don't put a pixel in the histogram if it's out-of-bounds.
          }

        if ( pixelIntensity < m_InputImageMinimum
             || pixelIntensity > m_InputImageMaximum )
          {
          continue; // don't put a pixel in the histogram if the value
          // is out-of-bounds.
          }

        CooccurrenceIndexType instanceIndex;
        MeasurementVectorType measurement( MeasurementVectorSize );
        measurement[0] = centerPixelIntensity;
        measurement[1] = pixelIntensity;
        //Get Index of the histogram for the given pixel pair;
        m_Histogram->GetIndex(measurement, instanceIndex);
        m_GLCIList->AddPairToList(instanceIndex);
        }

      m_GLCIList->Normalize();
      VectorConstIteratorType constVectorIt;
      VectorType glcList = m_GLCIList->GetVector();

      //Compute inertia aka contrast
      double inertia = 0;
      constVectorIt = glcList.begin();
      while( constVectorIt != glcList.end())
        {
        CooccurrenceIndexType index = (*constVectorIt).index;
        RelativeFrequencyType frequency = (*constVectorIt).frequency;
        inertia += ( index[0] - index[1] ) * ( index[0] - index[1] ) * frequency;
        ++constVectorIt;
        }

      if (inertia < out)
        {
        out = inertia;
        }
      }

    outputIt.Set(out);
    ++outputIt;
    progress.CompletedPixel();
    }
}

} // End namespace otb

#endif
