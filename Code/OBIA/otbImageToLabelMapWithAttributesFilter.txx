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
#ifndef __otbImageToLabelMapWithAttributesFilter_txx
#define __otbImageToLabelMapWithAttributesFilter_txx

#include "otbImageToLabelMapWithAttributesFilter.h"
#include "itkProcessObject.h"

namespace otb
{
template <class TInputImage, class TLabeledImage, class TOutputLabel,class TObjectLabel>
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>
::ImageToLabelMapWithAttributesFilter()
{
  m_Output = LabelMapType::New();
  
  this->itk::ProcessObject::SetNumberOfRequiredOutputs(2);

}


template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
void
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>
::SetInput( const InputImageType *image)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0, 
                    const_cast< InputImageType * >( image ) );
  
}

template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
void
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>
::SetLabeledImage( const LabeledImageType *image)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1, 
                    const_cast<LabeledImageType * >( image ) );
  
}

template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
const typename ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::InputImageType *
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>
::GetInput()
{
    if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  
  return static_cast<const  InputImageType* >
    (this->itk::ProcessObject::GetInput(0) );
}

template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
const typename ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::LabeledImageType*
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>
::GetLabeledImage()
{
    if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  
  return static_cast<const  LabeledImageType* >
    (this->itk::ProcessObject::GetInput(1) );
}


template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
typename ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>
::LabelMapType*
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>
::GetOutput()
{
  return dynamic_cast< const  LabelMapType* >
    (this->Superclass::GetOutput(0));
}



template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
void
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>
::GenerateData()
{
  std::cout <<"############# ImageToLabelMapWithAttributesFilter::GenerateData #########" << std::endl;
  typename InputImageType::Pointer inputImage    = const_cast<InputImageType *>(this->GetInput());
  typename LabeledImageType::Pointer labeldImage = const_cast<LabeledImageType *>(this->GetLabeledImage());
  
  // Convert to label map
  typename LabelMapFilterType::Pointer lfilter = LabelMapFilterType::New();
  lfilter->SetBackgroundValue(itk::NumericTraits<LabelType>::max());
  lfilter->SetInput(labeldImage);
  
  // Compute shape attributes
  typename ShapeLabelMapFilterType::Pointer shapeLabelMapFilter = ShapeLabelMapFilterType::New();
  shapeLabelMapFilter->SetInput(lfilter->GetOutput());

  // Compute radiometric attributes
  typename RadiometricLabelMapFilterType::Pointer radiometricLabelMapFilter = RadiometricLabelMapFilterType::New();
  radiometricLabelMapFilter->SetInput1(shapeLabelMapFilter->GetOutput());
  radiometricLabelMapFilter->SetInput2(inputImage);

//   if(m_VectorImage->GetNumberOfComponentsPerPixel()==3)
//     {
// //     radiometricLabelMapFilter->SetRedChannelIndex(0);
// //     radiometricLabelMapFilter->SetGreenChannelIndex(1);
// //     radiometricLabelMapFilter->SetBlueChannelIndex(2);
// //     radiometricLabelMapFilter->SetNIRChannelIndex(m_BandId[3]);
//     }
  
  // Get the label map
  radiometricLabelMapFilter->GetOutput()->SetAdjacencyMap(lfilter->GetOutput()->GetAdjacencyMap());
  radiometricLabelMapFilter->GraftOutput( this->GetOutput() );

  // execute the mini-pipeline
  radiometricLabelMapFilter->Update();

  // graft the mini-pipeline output back onto this filter's output.
  this->GraftOutput( radiometricLabelMapFilter->GetOutput() );
}



}

#endif
