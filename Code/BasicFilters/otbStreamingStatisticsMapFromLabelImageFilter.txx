/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

Some parts of this code are derived from ITK. See ITKCopyright.txt
for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbStreamingStatisticsMapFromLabelImageFilter_txx
#define __otbStreamingStatisticsMapFromLabelImageFilter_txx
#include "otbStreamingStatisticsMapFromLabelImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"


namespace otb
{

template<class TInputVectorImage, class TLabelImage>
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::PersistentStreamingStatisticsMapFromLabelImageFilter()
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types
  typename MeanValueMapObjectType::Pointer output
      = static_cast<MeanValueMapObjectType*>(this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(1, output.GetPointer());

  this->Reset();

}

template<class TInputVectorImage, class TLabelImage>
typename itk::DataObject::Pointer
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::MakeOutput(unsigned int output)
{
  switch (output)
    {
    case 0:
      return static_cast<itk::DataObject*>(TInputVectorImage::New().GetPointer());
      break;
    case 1:
      return static_cast<itk::DataObject*>(MeanValueMapObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputVectorImage::New().GetPointer());
      break;
    }
}

template<class TInputVectorImage, class TLabelImage>
void
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::SetInputLabelImage(const LabelImageType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1,
                                   const_cast< LabelImageType * >( input ) );

}

template<class TInputVectorImage, class TLabelImage>
const typename PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::LabelImageType*
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::GetInputLabelImage()
{
  return static_cast< const TLabelImage * >
    (this->itk::ProcessObject::GetInput(1));
}

template<class TInputVectorImage, class TLabelImage>
typename PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::MeanValueMapType
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::GetMeanValueMap() const
{
  return m_RadiometricValueAccumulator;
}

template<class TInputVectorImage, class TLabelImage>
typename PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::LabelPopulationMapType
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::GetLabelPopulationMap() const
{
  return m_LabelPopulation;
}

template<class TInputVectorImage, class TLabelImage>
void
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  
  if (this->GetInput())
    {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

    if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
      {
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
      }
    }
}

template<class TInputVectorImage, class TLabelImage>
void
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputVectorImage, class TLabelImage>
void
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::Synthetize()
{
   typename MeanValueMapType::iterator it;
   for(it = m_RadiometricValueAccumulator.begin(); it != m_RadiometricValueAccumulator.end(); it++)
   {
      it->second = it->second / m_LabelPopulation[it->first];
   }
}

template<class TInputVectorImage, class TLabelImage>
void
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::Reset()
{
  m_RadiometricValueAccumulator.clear();
}

template<class TInputVectorImage, class TLabelImage>
void
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::GenerateData()
{
  /**
   * Grab the input
   */
  InputVectorImagePointer inputPtr =  const_cast<TInputVectorImage *>(this->GetInput());
  LabelImagePointer labelInputPtr =  const_cast<TLabelImage *>(this->GetInputLabelImage());
  
  itk::ImageRegionConstIterator<TInputVectorImage> inIt(inputPtr, inputPtr->GetRequestedRegion());
  itk::ImageRegionConstIterator<TLabelImage> labelIt(labelInputPtr, inputPtr->GetRequestedRegion());
  


  typename VectorImageType::PixelType value;
  typename LabelImageType::PixelType label;
  // do the work
  for (inIt.GoToBegin(), labelIt.GoToBegin();
       !inIt.IsAtEnd() && !labelIt.IsAtEnd();
       ++inIt, ++labelIt)
    {
      value = inIt.Get();
      label = labelIt.Get();
      if (m_RadiometricValueAccumulator.count(label)<=0) //add new element to the map
      {
        m_RadiometricValueAccumulator[label] = value;
        m_LabelPopulation[label] = 1;
      }
      else
      {
        m_RadiometricValueAccumulator[label] += value;
        m_LabelPopulation[label] ++;
      }

    }
}

template<class TInputVectorImage, class TLabelImage>
void
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
