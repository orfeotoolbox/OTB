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

#ifndef __otbLabelImageRegionMergingFilter_txx
#define __otbLabelImageRegionMergingFilter_txx

#include "otbLabelImageRegionMergingFilter.h"
#include "itkProgressReporter.h"


namespace otb
{
template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::LabelImageRegionMergingFilter()
{

  this->SetNumberOfRequiredInputs( 2 );

}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::SetInputLabelImage( const TInputLabelImage * labelImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TInputLabelImage *>( labelImage ));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::SetInputSpectralImage( const TInputSpectralImage * spectralImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TInputSpectralImage *>( spectralImage ));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
TInputLabelImage *
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::GetInputLabelImage()
{
  return dynamic_cast<TInputLabelImage*>(itk::ProcessObject::GetInput(0));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
TInputSpectralImage *
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::GetInputSpectralImage()
{
  return dynamic_cast<TInputSpectralImage*>(itk::ProcessObject::GetInput(1));
}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::~LabelImageRegionMergingFilter()
{

}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
TOutputLabelImage *
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::GetLabelOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
      return 0;
    }
  return static_cast<OutputLabelImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
const TOutputLabelImage *
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::GetLabelOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
      return 0;
    }
  return static_cast<OutputLabelImageType *>(this->itk::ProcessObject::GetOutput(0));
}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::BeforeThreadedGenerateData()
{
  typename InputSpectralImageType::Pointer spectralImage = this->GetInputSpectralImage();
  typename InputLabelImageType::Pointer inputLabelImage = this->GetInputLabelImage();

  m_NumberOfComponentsPerPixel = spectralImage->GetNumberOfComponentsPerPixel();

  // Convert to label map with adjacency
  typename LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(inputLabelImage);
  labelMapFilter->Update();

  // Associate each label to a spectral value
  m_LabelMap = labelMapFilter->GetOutput();
  typename LabelMapType::LabelObjectContainerType & localLabelObjectContainer = m_LabelMap->GetLabelObjectContainer();
  typename LabelMapType::LabelObjectContainerType::iterator labelIt = localLabelObjectContainer.begin();
  while ( labelIt != localLabelObjectContainer.end() )
    {
    typename LabelMapType::LabelType l;
    l = labelIt->first;
    typename AttributeLabelObjectType::Pointer labelObject = labelIt->second;
    labelObject->SetAttribute(
      spectralImage->GetPixel(
        labelObject->GetLine(0).GetIndex()
      )
    );
    ++labelIt;
    }

}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, int threadId)
{
}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::AfterThreadedGenerateData()
{
  typename OutputLabelImageType::Pointer outputLabelImage = this->GetOutput();
  typedef itk::ImageRegionIterator<OutputLabelImageType> OutputLabelIteratorType;
  OutputLabelIteratorType outputLabelIt(outputLabelImage, outputLabelImage->GetRequestedRegion());

  // copy labelMap to output label image
  typename LabelMapToLabelImageFilterType::Pointer mapToImageFilter = LabelMapToLabelImageFilterType::New();
  mapToImageFilter->SetInput(m_LabelMap);
  mapToImageFilter->Update();

  OutputLabelIteratorType mapToImageIt(mapToImageFilter->GetOutput(), outputLabelImage->GetRequestedRegion());

  outputLabelIt.GoToBegin();
  mapToImageIt.GoToBegin();

  while(!outputLabelIt.IsAtEnd())
    {
    outputLabelIt.Set(mapToImageIt.Get());
    ++outputLabelIt;
    ++mapToImageIt;
    }

}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Range bandwidth: "                  << m_RangeBandwidth                 << std::endl;
 }

} // end namespace otb

#endif
