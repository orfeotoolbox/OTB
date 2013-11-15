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
#ifndef __otbKMeansImageClassificationFilter_txx
#define __otbKMeansImageClassificationFilter_txx

#include "otbKMeansImageClassificationFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
KMeansImageClassificationFilter<TInputImage, TOutputImage, VMaxSampleDimension, TMaskImage>
::KMeansImageClassificationFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(1);
  m_DefaultLabel = itk::NumericTraits<LabelType>::ZeroValue();
}

template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
void
KMeansImageClassificationFilter<TInputImage, TOutputImage, VMaxSampleDimension, TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<MaskImageType *>(mask));
}

template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
const typename KMeansImageClassificationFilter<TInputImage, TOutputImage, VMaxSampleDimension, TMaskImage>
::MaskImageType *
KMeansImageClassificationFilter<TInputImage, TOutputImage, VMaxSampleDimension, TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return static_cast<const MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
void
KMeansImageClassificationFilter<TInputImage, TOutputImage, VMaxSampleDimension, TMaskImage>
::BeforeThreadedGenerateData()
{
  unsigned int sample_size = MaxSampleDimension;
  unsigned int nb_classes = m_Centroids.Size() / sample_size;

  for (LabelType label = 1; label <= static_cast<LabelType>(nb_classes); ++label)
    {
    SampleType new_centroid;
    new_centroid.Fill(0);
    m_CentroidsMap[label] = new_centroid;

    for (unsigned int i = 0; i < MaxSampleDimension; ++i)
      {
      m_CentroidsMap[label][i] =
        static_cast<ValueType>(m_Centroids[MaxSampleDimension * (static_cast < unsigned int > (label) - 1) + i]);
      }
    }
}

template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
void
KMeansImageClassificationFilter<TInputImage, TOutputImage, VMaxSampleDimension, TMaskImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();

  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType>  MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;

  InputIteratorType inIt(inputPtr, outputRegionForThread);
  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  MaskIteratorType maskIt;
  if (inputMaskPtr)
    {
    maskIt = MaskIteratorType(inputMaskPtr, outputRegionForThread);
    maskIt.GoToBegin();
    }
  unsigned int maxDimension = SampleType::Dimension;
  unsigned int sampleSize = std::min(inputPtr->GetNumberOfComponentsPerPixel(),
                                     maxDimension);

  bool validPoint = true;

  while (!outIt.IsAtEnd())
    {
    outIt.Set(m_DefaultLabel);
    ++outIt;
    }

  outIt.GoToBegin();

  validPoint = true;

  typename DistanceType::Pointer distance = DistanceType::New();

  while (!outIt.IsAtEnd() && (!inIt.IsAtEnd()))
    {
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    if (validPoint)
      {
      LabelType  label = 1;
      LabelType  current_label = 1;
      SampleType pixel;
      pixel.Fill(0);
      for (unsigned int i = 0; i < sampleSize; ++i)
        {
        pixel[i] = inIt.Get()[i];
        }

      double current_distance = distance->Evaluate(pixel, m_CentroidsMap[label]);

      for (label = 2; label <= static_cast<LabelType>(m_CentroidsMap.size()); ++label)
        {
        double tmp_dist = distance->Evaluate(pixel, m_CentroidsMap[label]);
        if (tmp_dist < current_distance)
          {
          current_label = label;
          current_distance = tmp_dist;
          }
        }
      outIt.Set(current_label);
      }
    ++outIt;
    ++inIt;
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension, class TMaskImage>
void
KMeansImageClassificationFilter<TInputImage, TOutputImage, VMaxSampleDimension, TMaskImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
