/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbMultiChannelsPolarimetricSynthesisFilter_hxx
#define otbMultiChannelsPolarimetricSynthesisFilter_hxx

#include <complex>

#include "otbMultiChannelsPolarimetricSynthesisFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "otbMath.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
MultiChannelsPolarimetricSynthesisFilter<TInputImage, TOutputImage, TFunction>::MultiChannelsPolarimetricSynthesisFilter()
  : m_PsiI(0.0), m_KhiI(0.0), m_PsiR(0.0), m_KhiR(0.0), m_Gain(1.0), m_Mode(0), m_EmissionH(false), m_EmissionV(false)
{
  this->SetNumberOfRequiredInputs(1);
  this->InPlaceOff();
  m_ArchitectureType = PolarimetricData::New();
}

/**
 * GenerateOutputInformation()
 */
template <class TInputImage, class TOutputImage, class TFunction>
void MultiChannelsPolarimetricSynthesisFilter<TInputImage, TOutputImage, TFunction>::GenerateOutputInformation()
{
  // do not call the superclass' implementation of this method since
  // this filter allows the input the output to be of different dimensions

  // get pointers to the input and output
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();
  typename Superclass::InputImageConstPointer inputPtr  = this->GetInput();

  if (!outputPtr || !inputPtr)
  {
    return;
  }

  // Set the output image largest possible region.  Use a RegionCopier
  // so that the input and output images can be different dimensions.
  OutputImageRegionType outputLargestPossibleRegion;
  this->CallCopyInputRegionToOutputRegion(outputLargestPossibleRegion, inputPtr->GetLargestPossibleRegion());
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);

  // Set the output spacing and origin
  const itk::ImageBase<Superclass::InputImageDimension>* phyData;

  phyData = dynamic_cast<const itk::ImageBase<Superclass::InputImageDimension>*>(this->GetInput());

  if (phyData)
  {
    // Copy what we can from the image from spacing and origin of the input
    // This logic needs to be augmented with logic that select which
    // dimensions to copy
    unsigned int                                  i, j;
    const typename InputImageType::SpacingType&   inputSpacing   = inputPtr->GetSignedSpacing();
    const typename InputImageType::PointType&     inputOrigin    = inputPtr->GetOrigin();
    const typename InputImageType::DirectionType& inputDirection = inputPtr->GetDirection();

    typename OutputImageType::SpacingType   outputSpacing;
    typename OutputImageType::PointType     outputOrigin;
    typename OutputImageType::DirectionType outputDirection;

    // copy the input to the output and fill the rest of the
    // output with zeros.
    for (i = 0; i < Superclass::InputImageDimension; ++i)
    {
      outputSpacing[i] = inputSpacing[i];
      outputOrigin[i]  = inputOrigin[i];
      for (j = 0; j < Superclass::OutputImageDimension; ++j)
      {
        if (j < Superclass::InputImageDimension)
        {
          outputDirection[j][i] = inputDirection[j][i];
        }
        else
        {
          outputDirection[j][i] = 0.0;
        }
      }
    }
    for (; i < Superclass::OutputImageDimension; ++i)
    {
      outputSpacing[i] = 1.0;
      outputOrigin[i]  = 0.0;
      for (j = 0; j < Superclass::OutputImageDimension; ++j)
      {
        if (j == i)
        {
          outputDirection[j][i] = 1.0;
        }
        else
        {
          outputDirection[j][i] = 0.0;
        }
      }
    }

    // set the spacing and origin
    outputPtr->SetSignedSpacing(outputSpacing);
    outputPtr->SetOrigin(outputOrigin);
    outputPtr->SetDirection(outputDirection);
  }
  else
  {
    // pointer could not be cast back down
    itkExceptionMacro(<< "otb::MultiChannelsPolarimetricSynthesisFilter::GenerateOutputInformation "
                      << "cannot cast input to " << typeid(itk::ImageBase<Superclass::InputImageDimension>*).name());
  }
}

/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage, class TFunction>
void MultiChannelsPolarimetricSynthesisFilter<TInputImage, TOutputImage, TFunction>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                                                          itk::ThreadIdType threadId)
{

  InputImagePointer  inputPtr  = this->GetInput();
  OutputImagePointer outputPtr = this->GetOutput(0);

  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIterator<TInputImage> inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<TOutputImage>     outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  ArchitectureType val = m_ArchitectureType->GetArchitectureType();

  // Computation with 4 channels
  switch (val)
  {
  case HH_HV_VH_VV:
    while (!inputIt.IsAtEnd())
    {
      outputIt.Set(m_Gain * GetFunctor()(inputIt.Get()[0], inputIt.Get()[1], inputIt.Get()[2], inputIt.Get()[3]));
      ++inputIt;
      ++outputIt;
      progress.CompletedPixel(); // potential exception thrown here
    }
    break;

  // With 3 channels : HH HV VV or HH VH VV
  case HH_HV_VV:
    while (!inputIt.IsAtEnd())
    {
      outputIt.Set(m_Gain * GetFunctor()(inputIt.Get()[0], inputIt.Get()[1], inputIt.Get()[1], inputIt.Get()[2]));
      ++inputIt;
      ++outputIt;
      progress.CompletedPixel(); // potential exception thrown here
    }
    break;

  // Only HH and HV are present
  case HH_HV:
    while (!inputIt.IsAtEnd())
    {
      outputIt.Set(m_Gain * GetFunctor()(inputIt.Get()[0], inputIt.Get()[1], 0, 0));
      ++inputIt;
      ++outputIt;
      progress.CompletedPixel(); // potential exception thrown here
    }
    break;

  // Only VH and VV are present
  case VH_VV:
    while (!inputIt.IsAtEnd())
    {
      outputIt.Set(m_Gain * GetFunctor()(0, 0, inputIt.Get()[2], inputIt.Get()[3]));
      ++inputIt;
      ++outputIt;
      progress.CompletedPixel(); // potential exception thrown here
    }
    break;

  default:
    itkExceptionMacro("Unknown architecture : Polarimetric synthesis is impossible !");
    return;
  }
}

/**
 * Computation of the electromagnetic fields Ei Er
 */
template <class TInputImage, class TOutputImage, class TFunction>
void MultiChannelsPolarimetricSynthesisFilter<TInputImage, TOutputImage, TFunction>::ComputeElectromagneticFields()
{
  ComplexArrayType AEi, AEr;

  /** Conversion coefficient Degre To Radian */
  double DTOR = CONST_PI_180;
  double real, imag;

  real = std::cos(DTOR * m_PsiI) * std::cos(DTOR * m_KhiI);
  imag = -std::sin(DTOR * m_PsiI) * std::sin(DTOR * m_KhiI);
  ComplexType Ei0(real, imag);

  real = std::sin(DTOR * m_PsiI) * std::cos(DTOR * m_KhiI);
  imag = std::cos(DTOR * m_PsiI) * std::sin(DTOR * m_KhiI);
  ComplexType Ei1(real, imag);

  real = std::cos(DTOR * m_PsiR) * std::cos(DTOR * m_KhiR);
  imag = -std::sin(DTOR * m_PsiR) * std::sin(DTOR * m_KhiR);
  ComplexType Er0(real, imag);

  real = std::sin(DTOR * m_PsiR) * std::cos(DTOR * m_KhiR);
  imag = std::cos(DTOR * m_PsiR) * std::sin(DTOR * m_KhiR);
  ComplexType Er1(real, imag);

  AEi[0] = Ei0;
  AEi[1] = Ei1;
  AEr[0] = Er0;
  AEr[1] = Er1;

  this->SetEi(AEi);
  this->SetEr(AEr);
}

/**
 * Verify and force the inputs, if only  2 or 3 channels are present
 */
template <class TInputImage, class TOutputImage, class TFunction>
void MultiChannelsPolarimetricSynthesisFilter<TInputImage, TOutputImage, TFunction>::VerifyAndForceInputs()
{

  ArchitectureType val = m_ArchitectureType->GetArchitectureType();

  switch (val)
  {

  case HH_HV_VH_VV:
    break;
  case HH_HV_VV:
    break;
  case HH_VH_VV:
    break;
  // Only HH and HV are present
  case HH_HV:

    // Forcing KhiI=0 PsiI=0
    this->SetKhiI(0);
    this->SetPsiI(0);
    break;

  // Only VH and VV are present
  case VH_VV:

    // Forcing KhiI=0 PsiI=90
    this->SetKhiI(0);
    this->SetPsiI(90);
    break;

  default:
    itkExceptionMacro("Unknown architecture : Polarimetric synthesis is impossible !!");
    return;
  }

  if (GetMode() == 1)
    ForceCoPolar();
  else if (GetMode() == 2)
    ForceCrossPolar();
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class TFunction>
void MultiChannelsPolarimetricSynthesisFilter<TInputImage, TOutputImage, TFunction>::BeforeThreadedGenerateData()
{

  int NumberOfImages = this->GetInput()->GetNumberOfComponentsPerPixel();

  // First Part. Determine the kind of architecture of the input picture
  m_ArchitectureType->DetermineArchitecture(NumberOfImages, GetEmissionH(), GetEmissionV());

  // Second Part. Verify and force the inputs
  VerifyAndForceInputs();

  // Third Part. Estimation of the incident field Ei and the reflected field Er
  ComputeElectromagneticFields();
}

/**
 * Force Copolar mode
 */
template <class TInputImage, class TOutputImage, class TFunction>
void MultiChannelsPolarimetricSynthesisFilter<TInputImage, TOutputImage, TFunction>::ForceCoPolar()
{
  SetPsiR(m_PsiI);
  SetKhiR(m_KhiI);
}

/**
 * Force Crosspolar mode
 */
template <class TInputImage, class TOutputImage, class TFunction>
void MultiChannelsPolarimetricSynthesisFilter<TInputImage, TOutputImage, TFunction>::ForceCrossPolar()
{
  SetPsiR(m_PsiI + 90);
  SetKhiR(-m_KhiI);
  SetMode(2);
}

/**
 * Printself
 */
template <class TInputImage, class TOutputImage, class TFunction>
void MultiChannelsPolarimetricSynthesisFilter<TInputImage, TOutputImage, TFunction>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "PsiI: " << m_PsiI << std::endl;
  os << indent << "KhiI: " << m_KhiI << std::endl;
  os << indent << "PsiR: " << m_PsiR << std::endl;
  os << indent << "KhiR: " << m_KhiR << std::endl;

  os << indent << "Ei0 im: " << m_Ei[0].imag() << std::endl;
  os << indent << "Ei0 re: " << m_Ei[0].real() << std::endl;
  os << indent << "Ei1 im: " << m_Ei[1].imag() << std::endl;
  os << indent << "Ei1 re: " << m_Ei[1].real() << std::endl;

  os << indent << "Er0 im: " << m_Er[0].imag() << std::endl;
  os << indent << "Er0 re: " << m_Er[0].real() << std::endl;
  os << indent << "Er1 im: " << m_Er[1].imag() << std::endl;
  os << indent << "Er1 re: " << m_Er[1].real() << std::endl;
}

} // end namespace otb

#endif
