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
#ifndef otbSVMImageModelEstimator_txx
#define otbSVMImageModelEstimator_txx

#include "otbSVMImageModelEstimator.h"
#include "otbMacro.h"

#include "itkCommand.h"
#include "itkImageRegionConstIterator.h"

namespace otb
{

template<class TInputImage, class TTrainingImage>
SVMImageModelEstimator<TInputImage, TTrainingImage>
::SVMImageModelEstimator()
{
  this->SetNumberOfRequiredInputs(2);
}

template<class TInputImage,
    class TTrainingImage>
SVMImageModelEstimator<TInputImage, TTrainingImage>
::~SVMImageModelEstimator(void)
{}

/*
 * PrintSelf
 */
template<class TInputImage,
    class TTrainingImage>
void
SVMImageModelEstimator<TInputImage, TTrainingImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template<class TInputImage, class TTrainingImage>
void
SVMImageModelEstimator<TInputImage, TTrainingImage>
::SetInputImage(const TInputImage * inputImage)
{
  this->itk::ProcessObject::SetNthInput(0, const_cast<TInputImage*>(inputImage));
}

template<class TInputImage, class TTrainingImage>
void
SVMImageModelEstimator<TInputImage, TTrainingImage>
::SetTrainingImage(const TTrainingImage * trainingImage)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<TTrainingImage*>(trainingImage));
}

template<class TInputImage, class TTrainingImage>
const TInputImage *
SVMImageModelEstimator<TInputImage, TTrainingImage>
::GetInputImage()
{
  if (this->GetNumberOfInputs() < 1)
    {
    return ITK_NULLPTR;
    }
  return static_cast<TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template<class TInputImage, class TTrainingImage>
const TTrainingImage *
SVMImageModelEstimator<TInputImage, TTrainingImage>
::GetTrainingImage()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<TTrainingImage *>(this->itk::ProcessObject::GetInput(1));
}

/**
 * Generate data (start the model building process)
 */
template<class TInputImage,
    class TTrainingImage>
void
SVMImageModelEstimator<TInputImage,  TTrainingImage>
::PrepareData()
{
  // Get input and output pointers
  const TInputImage *              inputImage = this->GetInputImage();
  const TTrainingImage *           trainingImage = this->GetTrainingImage();
  typename Superclass::ModelType * model = this->GetModel();

  // Do some error checking
  typename TInputImage::SizeType
    inputImageSize = inputImage->GetBufferedRegion().GetSize();
  typename TTrainingImage::SizeType
    trainingImageSize = trainingImage->GetBufferedRegion().GetSize();

  // Check if size of the two inputs are same
  for (unsigned int i = 0; i < TInputImage::ImageDimension; ++i)
    {
    if (inputImageSize[i] != trainingImageSize[i])
      throw itk::ExceptionObject(
        __FILE__,
        __LINE__,
        "Input image size is not the same as the training image size.",
        ITK_LOCATION);
    }

  // Declaration of the iterators on the input and training images
  typedef itk::ImageRegionConstIterator<TInputImage>    InputIteratorType;
  typedef itk::ImageRegionConstIterator<TTrainingImage> TrainingIteratorType;

  InputIteratorType inIt(inputImage, inputImage->GetBufferedRegion());
  TrainingIteratorType trIt(trainingImage, trainingImage->GetBufferedRegion());

  inIt.GoToBegin();
  trIt.GoToBegin();

  // Erase previous samples
  model->ClearSamples();

  //This works with Image< itk::Vector > and with VectorImage< scalar >.
  unsigned int numberOfComponents = inIt.Get().Size();

  while (!inIt.IsAtEnd() && !trIt.IsAtEnd())
    {
    if (trIt.Get() != 0)
      {
      typename Superclass::ModelType::MeasurementType v;

      for (unsigned int k = 0; k < numberOfComponents; ++k)
        {
        v.push_back(inIt.Get()[k]);
        }

      model->AddSample(v, trIt.Get());
      }
    ++inIt;
    ++trIt;
    }
}
} //End namespace OTB
#endif
