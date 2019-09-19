#ifndef __StreamingMultibandFeatherMosaicFilter_txx
#define __StreamingMultibandFeatherMosaicFilter_txx

#include "otbStreamingMultibandFeatherMosaicFilter.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor: set default values
 */
template <class TInputImage, class TOutputImage, class TDistanceImage>
StreamingMultibandFeatherMosaicFilter<TInputImage, TOutputImage, TDistanceImage>
::StreamingMultibandFeatherMosaicFilter()
{
  m_NumberOfLevels = 2;
  m_FirstLevelTransitionDistance = 3;
  m_FirstLevelVariance = 1;
}

template <class TInputImage, class TOutputImage, class TDistanceImage>
void
StreamingMultibandFeatherMosaicFilter<TInputImage, TOutputImage, TDistanceImage>
::Modified()
{

  // Clear scales parameters
  m_TransitionDistances.clear();
  m_TransitionOffsets.clear();
  m_Variances.clear();

  // Clear internal filters
  m_SubImageFilter.clear();
  m_Filter.clear();
  m_SingleFilter.clear();
  m_MosaicFilter.clear();

  // Compute transition distances
  for (unsigned int level = 0 ; level <= m_NumberOfLevels ; level++)
    {
    DistanceImageValueType distance = std::pow( (DistanceImageValueType) level+1, 2.0)*m_FirstLevelTransitionDistance;
    m_TransitionDistances.push_back(distance);
    }

  // Transition offsets are here to center the blending effect
  for (unsigned int level = 0 ; level <= m_NumberOfLevels ; level++)
    {
    DistanceImageValueType offset = 0.5*(m_TransitionDistances.at(m_NumberOfLevels)
                                         -m_TransitionDistances.at(level) );
    m_TransitionOffsets.push_back(offset);
    }

  //	// When LF transition distance is manually set, change corresponding
  // offset
  //	if (transitionDistanceLF>transitionsDistances[numberOfLevels])
  //	{
  //		// Set new LF transition distance
  //		transitionsDistances[numberOfLevels] = transitionDistanceLF;
  //		// Reset offset value same as level before
  //		transitionOffset[numberOfLevels] = transitionOffset[numberOfLevels-1];
  //	}

  // Prepare mosaic filters
  for (unsigned int level = 0 ; level <= m_NumberOfLevels ; level++)
    {
    itkDebugMacro("Create mosaic filter for level " << level);
    MosaicFilterPointer mosaicFilter = MosaicFilterType::New();

    // Set output origin, size, spacing, AutomaticOutputParametersComputation,
    // scale matrix, shift-scale mode
    mosaicFilter->SetOutputOrigin(this->GetOutputOrigin() );
    mosaicFilter->SetOutputSize(this->GetOutputSize() );
    mosaicFilter->SetOutputSpacing(this->GetOutputSpacing() );
    mosaicFilter->SetAutomaticOutputParametersComputation(this->GetAutomaticOutputParametersComputation() );
    mosaicFilter->SetScaleMatrix(this->GetScaleMatrix() );   // Set scales (HF
                                                             // and LF)
    mosaicFilter->SetShiftScaleInputImages(this->GetShiftScaleInputImages() );
    mosaicFilter->SetInterpolator(this->GetInterpolator() );                 // Interpolator
    mosaicFilter->SetNoDataOutputPixel(this->GetNoDataOutputPixel() );       // No
                                                                             // data
                                                                             // (output)
    mosaicFilter->SetNoDataInputPixel(this->GetNoDataInputPixel() );         // No
                                                                             // data
                                                                             // (input)
    mosaicFilter->SetFeatheringSmoothness(this->GetFeatheringSmoothness() ); // feathering
                                                                             // exponent
    mosaicFilter->SetDistanceInterpolator(this->GetDistanceInterpolator() ); // distance
                                                                             // image
                                                                             // interpolator

    // Set shifts
    if (level == m_NumberOfLevels)
      {
      // ...for LF
      mosaicFilter->SetShiftMatrix(this->GetShiftMatrix() );
      }
    else
      {
      // ... but not for HF
      typename MosaicFilterType::MatrixType nullshifts(this->GetShiftMatrix() );
      nullshifts.fill(0);
      mosaicFilter->SetShiftMatrix(nullshifts);
      }
    m_MosaicFilter.push_back(mosaicFilter);
    }

  // Now prepare all filters
  const unsigned int numberOfImages = 0.5*(this->GetNumberOfInputs() );
  for (unsigned int i = 0 ; i < numberOfImages ; i++)
    {

    const unsigned int inputImageIndex = 2*i;
    const unsigned int inputDistanceImageIndex = 2*i+1;

    itkDebugMacro("Create filters for image " << i );

    std::vector<DiscreteGaussianFilterPointer> gaussianFilters;
    std::vector<PerBandFilterPointer>          gaussianFilterAdapters;
    std::vector<SubImageFilterPointer>         subImageFilters;

    // Create N filters
    for (unsigned int level = 0 ; level < m_NumberOfLevels ; level++)
      {

      itkDebugMacro("\tLevel " << level );

      // compute variance of the level
      const double shrinkFactor = std::pow(2.0, (double) level ) ; // 1 2 4 8 16
                                                                   // ...
      const double variance =  std::pow(0.5 * shrinkFactor, 2.0) ; //

      // discrete gaussian filter
      DiscreteGaussianFilterPointer discreteGaussianFilter = DiscreteGaussianFilterType::New();
      discreteGaussianFilter->SetVariance(variance);
      discreteGaussianFilter->SetUseImageSpacingOff();
      discreteGaussianFilter->SetReleaseDataFlag(true);
      gaussianFilters.push_back(discreteGaussianFilter);

      // filter adapter
      PerBandFilterPointer gaussianFilterAdapter = PerBandFilterType::New();
      gaussianFilterAdapter->SetFilter(discreteGaussianFilter);
      gaussianFilterAdapter->SetInput(this->GetInput(inputImageIndex) );
      gaussianFilterAdapter->SetReleaseDataFlag(true);
      gaussianFilterAdapters.push_back(gaussianFilterAdapter);

      // subtract filter
      SubImageFilterPointer subtractFilter = SubImageFilterType::New();
      if (level == 0)
        {
        // HF band is I* - b0
        subtractFilter->SetInput1(this->GetInput(inputImageIndex) );
        subtractFilter->SetInput2(gaussianFilterAdapters.at(level)->GetOutput() );
        }
      else
        {
        // band is bn-1 - bn
        subtractFilter->SetInput1(gaussianFilterAdapters.at(level-1)->GetOutput() );
        subtractFilter->SetInput2(gaussianFilterAdapters.at(level)->GetOutput() );
        }
      subImageFilters.push_back(subtractFilter);

      // mosaic filter
      m_MosaicFilter[level]->PushBackInputs(subtractFilter->GetOutput(),
                                            static_cast<const DistanceImageType*>(Superclass::ProcessObject::GetInput(
                                                                                    inputDistanceImageIndex) ) );

      }
    m_SingleFilter.push_back(gaussianFilters);
    m_Filter.push_back(gaussianFilterAdapters);
    m_SubImageFilter.push_back(subImageFilters);

    // Last band is lowest low pass filter
    itkDebugMacro("m_Filter size = " << m_Filter.size() << " / i = " << i);
    m_MosaicFilter[m_NumberOfLevels]->PushBackInputs(m_Filter.at(i).at(m_NumberOfLevels-1)->GetOutput(),
                                                     static_cast<const DistanceImageType*>(Superclass::ProcessObject::
                                                                                           GetInput(
                                                                                             inputDistanceImageIndex) ) );

    }

  // Compute extrapolation offset:
  // Avoid extrapolation near borders (gaussian filters blurs "no data" values
  // too...)
  // Because LF gaussian filters have the biggest kernel, one must
  // set an offset equal to this kernel size, in geographic unit. This will
  // avoid the use of blured "no data" values
  const double maximumSpacingInXY = std::max(std::abs(this->GetOutputSpacing()[0]),
                                             std::abs(this->GetOutputSpacing()[1]) );
  const double extrapolationOffset = ( (double)
                                       m_SingleFilter.at(0).at(m_NumberOfLevels-1)->GetMaximumKernelWidth() )
    *maximumSpacingInXY;
  itkDebugMacro(<<"Extrapolation offset: " <<  extrapolationOffset);

  // Set up mosaic filters offsets and transition distances
  itkDebugMacro("Level\tDist\tOffest");
  for (unsigned int level = 0 ; level <= m_NumberOfLevels ; level++)
    {
    const double offset = extrapolationOffset + m_TransitionOffsets.at(level);
    m_MosaicFilter.at(level)->SetDistanceOffset(offset);
    m_MosaicFilter.at(level)->SetFeatheringTransitionDistance(m_TransitionDistances.at(level) );
    itkDebugMacro(<<level << "\t" << m_TransitionDistances.at(level) << "\t" << offset );
    }

  // Recompose mosaic
  m_SummingFilter = SummingFilterType::New();
  for (unsigned int level = 0 ; level <= m_NumberOfLevels ; level++)
    {
    // Sum LF and HF
    m_SummingFilter->PushBackInput(m_MosaicFilter.at(level)->GetOutput() );
    }
  m_SummingFilter->SetReleaseDataFlag(true);

}

/*
 * GenerateData
 */
template <class TInputImage, class TOutputImage, class TDistanceImage>
void
StreamingMultibandFeatherMosaicFilter<TInputImage, TOutputImage, TDistanceImage>
::GenerateData()
{
  itkDebugMacro("Generate data on region " << this->GetOutput()->GetRequestedRegion() );

  m_SummingFilter->GraftOutput(this->GetOutput() );
  m_SummingFilter->Update();
  this->GraftOutput(m_SummingFilter->GetOutput() );

  itkDebugMacro("Generate data OK");
}

}
#endif
