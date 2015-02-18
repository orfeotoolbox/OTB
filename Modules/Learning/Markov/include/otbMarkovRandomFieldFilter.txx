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

#ifndef __otbMarkovRandomFieldFilter_txx
#define __otbMarkovRandomFieldFilter_txx
#include "otbMarkovRandomFieldFilter.h"

namespace otb
{
template<class TInputImage, class TClassifiedImage>
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::MarkovRandomFieldFilter(void) :
  m_NumberOfClasses(0),
  m_MaximumNumberOfIterations(50),
  m_ErrorCounter(0),
  m_ImageDeltaEnergy(0.0),
  m_NeighborhoodRadius(1),
  m_TotalNumberOfValidPixelsInOutputImage(1),
  m_TotalNumberOfPixelsInInputImage(1),
  m_ErrorTolerance(0.0),
  m_SmoothingFactor(1.0),
  m_NumberOfIterations(0),
  m_Lambda(1.0),
  m_ExternalClassificationSet(false),
  m_StopCondition(MaximumNumberOfIterations)
{
  m_Generator = RandomGeneratorType::GetInstance();
  m_Generator->SetSeed();

  this->SetNumberOfRequiredInputs(1);
  if ((int) InputImageDimension != (int) ClassifiedImageDimension)
    {
    std::ostringstream msg;
    msg << "Input image dimension: " << InputImageDimension << " != output image dimension: " <<
    ClassifiedImageDimension;
    throw itk::ExceptionObject(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    }
  m_InputImageNeighborhoodRadius.Fill(m_NeighborhoodRadius);
  //     m_MRFNeighborhoodWeight.resize(0);
  //     m_NeighborInfluence.resize(0);
  //     m_DummyVector.resize(0);
  //     this->SetMRFNeighborhoodWeight( m_DummyVector );
  //     this->SetDefaultMRFNeighborhoodWeight();

  //srand((unsigned)time(0));

}

template<class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::SetTrainingInput(const TrainingImageType * trainingImage)
{
  m_ExternalClassificationSet = true;
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1, const_cast<TrainingImageType *>(trainingImage));
  this->Modified();
}

template <class TInputImage, class TClassifiedImage>
const typename MarkovRandomFieldFilter<TInputImage, TClassifiedImage>::TrainingImageType*
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::GetTrainingInput(void)
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return static_cast<const TrainingImageType *>
           (this->itk::ProcessObject::GetInput(1));
}

template<class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << " MRF Image filter object " << std::endl;

  os << indent << " Number of classes: " << m_NumberOfClasses << std::endl;

  os << indent << " Maximum number of iterations: " <<
  m_MaximumNumberOfIterations << std::endl;

  os << indent << " Error tolerance for convergence: " <<
  m_ErrorTolerance << std::endl;

  os << indent << " Size of the MRF neighborhood radius:" <<
  m_InputImageNeighborhoodRadius << std::endl;

  os << indent << "StopCondition: "
     << m_StopCondition << std::endl;

  os << indent << " Number of iterations: " <<
  m_NumberOfIterations << std::endl;

  os << indent << " Lambda: " <<
  m_Lambda << std::endl;
} // end PrintSelf

/**
 * GenerateInputRequestedRegion method.
 */
template <class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::GenerateInputRequestedRegion()
{
  // this filter requires the all of the input images
  // to be at the size of the output requested region
  InputImagePointer inputPtr =
    const_cast<InputImageType *>(this->GetInput());
  OutputImagePointer outputPtr = this->GetOutput();
  inputPtr->SetRequestedRegion(outputPtr->GetRequestedRegion());
}

/**
 * EnlargeOutputRequestedRegion method.
 */
template <class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::EnlargeOutputRequestedRegion(itk::DataObject *output)
{
  // this filter requires the all of the output image to be in
  // the buffer
  TClassifiedImage *imgData;
  imgData = dynamic_cast<TClassifiedImage*>(output);
  imgData->SetRequestedRegionToLargestPossibleRegion();
}

/**
 * GenerateOutputInformation method.
 */
template <class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::GenerateOutputInformation()
{
  typename TInputImage::ConstPointer input = this->GetInput();
  typename TClassifiedImage::Pointer output = this->GetOutput();
  output->SetLargestPossibleRegion(input->GetLargestPossibleRegion());
}

template<class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::GenerateData()
{

//   InputImageConstPointer inputImage = this->GetInput();

  //Allocate memory for the labelled images
  this->Allocate();

  //Branch the pipeline
  this->Initialize();

  //Run the Markov random field
  this->ApplyMarkovRandomFieldFilter();

} // end GenerateData

/**
* Set the neighborhood radius from a single value
*/
template<class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::SetNeighborhoodRadius(const unsigned long radiusValue)
{
  //Set up the neighbor hood
  NeighborhoodRadiusType radius;
  for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
    radius[i] = radiusValue;
    }
  this->SetNeighborhoodRadius(radius);

} // end SetNeighborhoodRadius

/**
* Set the neighborhood radius from an array
*/
template<class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::SetNeighborhoodRadius(const unsigned long *radiusArray)
{
  NeighborhoodRadiusType radius;
  for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
    radius[i] = radiusArray[i];
    }
  //Set up the neighbor hood
  this->SetNeighborhoodRadius(radius);

} // end SetNeighborhoodRadius

/**
* Set the neighborhood radius from a radius object
*/
template<class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::SetNeighborhoodRadius(const NeighborhoodRadiusType& radius)
{
  //Set up the neighbor hood
  for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
    m_InputImageNeighborhoodRadius[i] = radius[i];
    m_LabelledImageNeighborhoodRadius[i] = radius[i];
    }

} // end SetNeighborhoodRadius
//-------------------------------------------------------

/**
* Allocate algorithm specific resources
*/
template<class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::Allocate()
{
  if (m_NumberOfClasses <= 0)
    {
    throw itk::ExceptionObject(__FILE__, __LINE__, "NumberOfClasses <= 0.", ITK_LOCATION);
    }

  //Set the output labelled and allocate the memory
  LabelledImagePointer outputPtr = this->GetOutput();

  //Allocate the output buffer memory
  outputPtr->SetBufferedRegion(outputPtr->GetRequestedRegion());
  outputPtr->Allocate();

  //Copy input data in the output buffer memory or
  //initialize to random values if not set
  LabelledImageRegionIterator
  outImageIt(outputPtr, outputPtr->GetRequestedRegion());

  if (m_ExternalClassificationSet)
    {
    typename TrainingImageType::ConstPointer trainingImage = this->GetTrainingInput();
    LabelledImageRegionConstIterator
    trainingImageIt(trainingImage, outputPtr->GetRequestedRegion());

    while (!outImageIt.IsAtEnd())
      {
      LabelledImagePixelType labelvalue =  static_cast<LabelledImagePixelType> (trainingImageIt.Get());

      outImageIt.Set(labelvalue);
      ++trainingImageIt;
      ++outImageIt;
      } // end while
    }
  else //set to random value
    {
//       srand((unsigned)time(0));
    while (!outImageIt.IsAtEnd())
      {
      LabelledImagePixelType randomvalue = static_cast<LabelledImagePixelType>(
        m_Generator->GetIntegerVariate() % static_cast<int>(m_NumberOfClasses)
        );
      outImageIt.Set(randomvalue);
      ++outImageIt;
      } // end while

    }

} // Allocate

/**
* Initialize pipeline and values
*/
template<class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::Initialize() throw (itk::ExceptionObject)
  {

  m_ImageDeltaEnergy = 0.0;

  InputImageSizeType inputImageSize =
    this->GetInput()->GetBufferedRegion().GetSize();

  //---------------------------------------------------------------------
  //Get the number of valid pixels in the output MRF image
  //---------------------------------------------------------------------

  m_TotalNumberOfPixelsInInputImage = 1;
  m_TotalNumberOfValidPixelsInOutputImage = 1;

  for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
    m_TotalNumberOfPixelsInInputImage *= static_cast<int>(inputImageSize[i]);

    m_TotalNumberOfValidPixelsInOutputImage *=
      (static_cast<int>(inputImageSize[i])
       - 2 * m_InputImageNeighborhoodRadius[i]);
    }

  srand((unsigned) time(0));

  if (!m_EnergyRegularization)
    {
    itkExceptionMacro(<< "EnergyRegularization is not present");
    }

  if (!m_EnergyFidelity)
    {
    itkExceptionMacro(<< "EnergyFidelity is not present");
    }

  if (!m_Optimizer)
    {
    itkExceptionMacro(<< "Optimizer is not present");
    }

  if (!m_Sampler)
    {
    itkExceptionMacro(<< "Sampler is not present");
    }

  m_Sampler->SetLambda(m_Lambda);
  m_Sampler->SetEnergyRegularization(m_EnergyRegularization);
  m_Sampler->SetEnergyFidelity(m_EnergyFidelity);
  m_Sampler->SetNumberOfClasses(m_NumberOfClasses);
  }

/**
*Apply the MRF image filter
*/
template<class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::ApplyMarkovRandomFieldFilter()
{

  //Note: error should be defined according to the number of valid pixel in the output
  int maxNumPixelError = itk::Math::Round<int, double> (m_ErrorTolerance *
                                              m_TotalNumberOfPixelsInInputImage);

  m_NumberOfIterations = 0;
  m_ErrorCounter = m_TotalNumberOfValidPixelsInOutputImage;

  while ((m_NumberOfIterations < m_MaximumNumberOfIterations) &&
         (m_ErrorCounter >= maxNumPixelError))
    {
    otbMsgDevMacro(<< "Iteration No." << m_NumberOfIterations);

    this->MinimizeOnce();

    otbMsgDevMacro(<< "m_ErrorCounter/m_TotalNumberOfPixelsInInputImage: "
                   << m_ErrorCounter / ((double) (m_TotalNumberOfPixelsInInputImage)));
    otbMsgDevMacro(<< "m_ImageDeltaEnergy: " << m_ImageDeltaEnergy);

    ++m_NumberOfIterations;

    }

  otbMsgDevMacro(<< "m_NumberOfIterations: " << m_NumberOfIterations);
  otbMsgDevMacro(<< "m_MaximumNumberOfIterations: " << m_MaximumNumberOfIterations);
  otbMsgDevMacro(<< "m_ErrorCounter: " << m_ErrorCounter);
  otbMsgDevMacro(<< "maxNumPixelError: " << maxNumPixelError);

  //Determine stop condition
  if (m_NumberOfIterations >= m_MaximumNumberOfIterations)
    {
    m_StopCondition = MaximumNumberOfIterations;
    }
  else if (m_ErrorCounter <= maxNumPixelError)
    {
    m_StopCondition = ErrorTolerance;
    }

} // ApplyMarkovRandomFieldFilter

/**
*Apply the MRF image filter on the whole image once
*/
template<class TInputImage, class TClassifiedImage>
void
MarkovRandomFieldFilter<TInputImage, TClassifiedImage>
::MinimizeOnce()
{
  LabelledImageNeighborhoodIterator
  labelledIterator(m_LabelledImageNeighborhoodRadius, this->GetOutput(),
                   this->GetOutput()->GetLargestPossibleRegion());
  InputImageNeighborhoodIterator
  dataIterator(m_InputImageNeighborhoodRadius, this->GetInput(),
               this->GetInput()->GetLargestPossibleRegion());
  m_ErrorCounter = 0;

  for (labelledIterator.GoToBegin(), dataIterator.GoToBegin();
       !labelledIterator.IsAtEnd();
       ++labelledIterator, ++dataIterator)
    {

    LabelledImagePixelType value;
    bool                   changeValueBool;
    m_Sampler->Compute(dataIterator, labelledIterator);
    value = m_Sampler->GetValue();
    changeValueBool = m_Optimizer->Compute(m_Sampler->GetDeltaEnergy());
    if (changeValueBool)
      {
      labelledIterator.SetCenterPixel(value);
      ++m_ErrorCounter;
      m_ImageDeltaEnergy += m_Sampler->GetDeltaEnergy();
      }
    }

}

} // namespace otb

#endif
