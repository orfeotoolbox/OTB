/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletFilterBank_txx
#define __otbWaveletFilterBank_txx
#include "otbWaveletFilterBank.h"

#include "otbMacro.h"
#include "otbSubsampleImageFilter.h"
#include "otbSubsampledImageRegionConstIterator.h"

#include "itkNeighborhoodAlgorithm.h"
#include "itkPeriodicBoundaryCondition.h"
#include "itkProgressReporter.h"

// FIXME
#define __myDebug__ 0
#include "otbImageFileWriter.h"

namespace otb {

/**
 * Template Specialization for the Wavelet::FORWARD case
 */

template <class TInputImage, class TOutputImage, class TWaveletOperator>
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::WaveletFilterBank ()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredInputs(1);

  unsigned int numOfOutputs = 1 << InputImageDimension;

  this->SetNumberOfRequiredOutputs(numOfOutputs);
  for (unsigned int i = 0; i < numOfOutputs; ++i)
    {
    this->SetNthOutput(i, OutputImageType::New());
    }

  m_UpSampleFilterFactor = 0;
  m_SubsampleImageFactor = 1;

  //this->SetNumberOfThreads(1);
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if (GetSubsampleImageFactor() == 1) return;

#if __myDebug__
  otbGenericMsgDebugMacro(<< " down sampling output regions by a factor of " << GetSubsampleImageFactor());
  otbGenericMsgDebugMacro(<< "initial region    " << this->GetInput()->GetLargestPossibleRegion().GetSize()[0]
                          << "," << this->GetInput()->GetLargestPossibleRegion().GetSize()[1]);
#endif

  OutputImageRegionType newRegion;
  this->CallCopyInputRegionToOutputRegion(newRegion, this->GetInput()->GetLargestPossibleRegion());

  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
    {
    this->GetOutput(i)->SetRegions(newRegion);
    }

#if __myDebug__
  otbGenericMsgDebugMacro(<< "new region output " << newRegion.GetSize()[0] << "," << newRegion.GetSize()[1]);
#endif
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::GenerateInputRequestedRegion()
throw (itk::InvalidRequestedRegionError)
  {
  Superclass::GenerateInputRequestedRegion();

  // Filter length calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(0);
  lowPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  lowPassOperator.CreateDirectional();

  unsigned int radius = lowPassOperator.GetRadius()[0];

  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(0);
  highPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  highPassOperator.CreateDirectional();

  if (radius < highPassOperator.GetRadius()[0]) radius = highPassOperator.GetRadius()[0];

  // Get the requested region and pad it
  InputImagePointerType input = const_cast<InputImageType*>(this->GetInput());
  InputImageRegionType  inputRegion = input->GetRequestedRegion();
  inputRegion.PadByRadius(radius);

  if (inputRegion.Crop(input->GetLargestPossibleRegion()))
    {
    input->SetRequestedRegion(inputRegion);
    }
  else
    {
    input->SetRequestedRegion(inputRegion);
    itk::InvalidRequestedRegionError err(__FILE__, __LINE__);
    err.SetLocation(ITK_LOCATION);
    err.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    err.SetDataObject(input);
    throw err;
    }
  }

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::BeforeThreadedGenerateData()
{
  if (m_SubsampleImageFactor > 1)
    {
    // Check the dimension
    for (unsigned int i = 0; i < InputImageDimension; ++i)
      {
      if ((m_SubsampleImageFactor
           * (this->GetInput()->GetRequestedRegion().GetSize()[i] / m_SubsampleImageFactor))
          != this->GetInput()->GetRequestedRegion().GetSize()[i])
        {
        itk::InvalidRequestedRegionError err(__FILE__, __LINE__);
        err.SetLocation(ITK_LOCATION);
        err.SetDescription("Requested region dimension cannot be used in multiresolution analysis (crop it).");
        err.SetDataObject(const_cast<InputImageType*>(this->GetInput()));
        throw err;
        }
      }

    if (InputImageDimension > 1)
      {
      // Internal images will be used only if m_SubsampledInputImages != 1
      m_InternalImages.resize(InputImageDimension - 1);
      for (unsigned int i = 0; i < m_InternalImages.size(); ++i)
        {
        // the size is linked to the SubsampleImageFactor that is assume to be 2!!!
        m_InternalImages[InputImageDimension - 2 - i].resize(1 << (i + 1));
        }

      OutputImageRegionType intermediateRegion;
      this->Superclass::CallCopyInputRegionToOutputRegion(intermediateRegion,
                                                          this->GetInput()->GetLargestPossibleRegion());

      AllocateInternalData(intermediateRegion);
      }
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::AllocateInternalData
  (const OutputImageRegionType& outputRegion)
{
  OutputImageRegionType smallerRegion;
  OutputImageRegionType largerRegion = outputRegion;

  for (unsigned int direction = 0; direction < InputImageDimension - 1; direction++)
    {
    this->CallCopyInputRegionToOutputRegion(InputImageDimension - 1 - direction,
                                            smallerRegion, largerRegion);

    for (unsigned int i = 0; i < m_InternalImages[direction].size(); ++i)
      {
      m_InternalImages[InputImageDimension - 2 - direction][i] = OutputImageType::New();
      m_InternalImages[InputImageDimension - 2 - direction][i]->SetRegions(smallerRegion);
      m_InternalImages[InputImageDimension - 2 - direction][i]->Allocate();
      m_InternalImages[InputImageDimension - 2 - direction][i]->FillBuffer(0);
      }

    largerRegion = smallerRegion;
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::AfterThreadedGenerateData()
{
  if (m_SubsampleImageFactor > 1 && InputImageDimension > 1)
    {
    m_InternalImages.clear();
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::CallCopyOutputRegionToInputRegion
  (InputImageRegionType& destRegion, const OutputImageRegionType& srcRegion)
{
  Superclass::CallCopyOutputRegionToInputRegion(destRegion, srcRegion);

  if (GetSubsampleImageFactor() > 1)
    {
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType  srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType  destSize;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
      {
      destIndex[i] = srcIndex[i] * GetSubsampleImageFactor();
      destSize[i] = srcSize[i] * GetSubsampleImageFactor();
      }

    destRegion.SetIndex(destIndex);
    destRegion.SetSize(destSize);

#if 0
    // Contrairement a Wavelet::INVERSE, ici ca ne sera a rien apparemment...

    // Region Padding
    LowPassOperatorType lowPassOperator;
    lowPassOperator.SetDirection(0);
    lowPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
    lowPassOperator.CreateDirectional();

    unsigned long radius[InputImageDimension];
    radius[0] = lowPassOperator.GetRadius()[0];

    HighPassOperatorType highPassOperator;
    highPassOperator.SetDirection(0);
    highPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
    highPassOperator.CreateDirectional();

    if (radius[0] < highPassOperator.GetRadius()[0]) radius[0] = highPassOperator.GetRadius()[0];

    for (unsigned int i = 1; i < InputImageDimension; ++i)
      radius[i] = 0;

    InputImageRegionType paddedRegion = destRegion;
    paddedRegion.PadByRadius(radius);

    if (paddedRegion.Crop(this->GetInput()->GetLargestPossibleRegion()))
      {
      destRegion = paddedRegion;
      }
#endif

    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::CallCopyOutputRegionToInputRegion
  (unsigned int direction,
  InputImageRegionType& destRegion, const OutputImageRegionType& srcRegion)
{
  Superclass::CallCopyOutputRegionToInputRegion(destRegion, srcRegion);

  if (GetSubsampleImageFactor() > 1)
    {
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType  srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType  destSize;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
      {
      if (i == direction)
        {
        destIndex[i] = srcIndex[i] * GetSubsampleImageFactor();
        destSize[i] = srcSize[i] * GetSubsampleImageFactor();
        }
      else
        {
        destIndex[i] = srcIndex[i];
        destSize[i] = srcSize[i];
        }
      }

    destRegion.SetIndex(destIndex);
    destRegion.SetSize(destSize);
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::CallCopyInputRegionToOutputRegion
  (OutputImageRegionType& destRegion, const InputImageRegionType& srcRegion)
{
  Superclass::CallCopyInputRegionToOutputRegion(destRegion, srcRegion);

  if (GetSubsampleImageFactor() > 1)
    {
    typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename InputImageRegionType::SizeType  srcSize = srcRegion.GetSize();

    typename OutputImageRegionType::IndexType destIndex;
    typename OutputImageRegionType::SizeType  destSize;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
      {
      // TODO: This seems not right in odd index cases
      destIndex[i] = srcIndex[i] / GetSubsampleImageFactor();
      destSize[i] = srcSize[i] / GetSubsampleImageFactor();
      }

    destRegion.SetIndex(destIndex);
    destRegion.SetSize(destSize);
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::CallCopyInputRegionToOutputRegion
  (unsigned int direction,
  OutputImageRegionType& destRegion, const InputImageRegionType& srcRegion)
{
  Superclass::CallCopyInputRegionToOutputRegion(destRegion, srcRegion);

  if (GetSubsampleImageFactor() > 1)
    {
    typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename InputImageRegionType::SizeType  srcSize = srcRegion.GetSize();

    typename OutputImageRegionType::IndexType destIndex;
    typename OutputImageRegionType::SizeType  destSize;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
      {
      if (i == direction)
        {
        // TODO: This seems not right in odd index cases
        destIndex[i] = srcIndex[i] / GetSubsampleImageFactor();
        destSize[i] = srcSize[i] / GetSubsampleImageFactor();
        }
      else
        {
        destIndex[i] = srcIndex[i];
        destSize[i] = srcSize[i];
        }
      }

    destRegion.SetIndex(destIndex);
    destRegion.SetSize(destSize);
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::ThreadedGenerateData
  (const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  unsigned int dir = InputImageDimension - 1;

  if ((1 << dir) >= static_cast<int>(this->GetNumberOfOutputs()))
    {
    std::ostringstream msg;
    msg << "Output number 1<<" << dir << " = " << (1 << dir) << " not allocated\n";
    msg << "Number of excpected outputs " << this->GetNumberOfOutputs() << "\n";
    throw itk::ExceptionObject(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    }

  itk::ProgressReporter reporter(this, threadId,
                                 outputRegionForThread.GetNumberOfPixels() * this->GetNumberOfOutputs() * 2);

  const InputImageType * input = this->GetInput();
  InputImageRegionType   inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  OutputImagePointerType outputLowPass = this->GetOutput(0);
  OutputImagePointerType outputHighPass = this->GetOutput(1 << dir);

  // On multidimensional case, if m_SubsampleImageFactor != 1, we need internal images of different size
  if (dir != 0 && m_SubsampleImageFactor > 1)
    {
    outputLowPass = m_InternalImages[dir - 1][0];
    outputHighPass = m_InternalImages[dir - 1][1];
    }

  // typedef for the iterations over the input image
  typedef itk::ConstNeighborhoodIterator<InputImageType>                                    NeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct<InputImageType>                                     InnerProductType;
  typedef itk::ImageRegionIterator<OutputImageType>                                         IteratorType;

  // Prepare the subsampling image factor, if any.
  typedef SubsampledImageRegionConstIterator<InputImageType> SubsampleIteratorType;
  typename SubsampleIteratorType::IndexType subsampling;
  subsampling.Fill(1);
  subsampling[dir] = GetSubsampleImageFactor();

  // Inner product
  InnerProductType innerProduct;

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(dir);
  highPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  highPassOperator.CreateDirectional();

  SubsampleIteratorType subItHighPass(input, inputRegionForThread);
  subItHighPass.SetSubsampleFactor(subsampling);
  subItHighPass.GoToBegin();

  NeighborhoodIteratorType itHighPass(highPassOperator.GetRadius(), input, inputRegionForThread);
  itk::PeriodicBoundaryCondition<InputImageType> boundaryCondition;
  itHighPass.OverrideBoundaryCondition(&boundaryCondition);

  IteratorType outHighPass(outputHighPass, subItHighPass.GenerateOutputInformation());
  outHighPass.GoToBegin();

  while (!subItHighPass.IsAtEnd() && !outHighPass.IsAtEnd())
    {
    itHighPass.SetLocation(subItHighPass.GetIndex());
    outHighPass.Set(innerProduct(itHighPass, highPassOperator));

    ++subItHighPass;
    ++outHighPass;

    reporter.CompletedPixel();
    }

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(dir);
  lowPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  lowPassOperator.CreateDirectional();

  SubsampleIteratorType subItLowPass(input, inputRegionForThread);
  subItLowPass.SetSubsampleFactor(subsampling);
  subItLowPass.GoToBegin();

  NeighborhoodIteratorType itLowPass(lowPassOperator.GetRadius(), input, inputRegionForThread);
  itLowPass.OverrideBoundaryCondition(&boundaryCondition);

  IteratorType outLowPass(outputLowPass, subItLowPass.GenerateOutputInformation());
  outLowPass.GoToBegin();

  while (!subItLowPass.IsAtEnd() && !outLowPass.IsAtEnd())
    {
    itLowPass.SetLocation(subItLowPass.GetIndex());
    outLowPass.Set(innerProduct(itLowPass, lowPassOperator));

    ++subItLowPass;
    ++outLowPass;

    reporter.CompletedPixel();
    }

  if (dir > 0)
    {
    // Note that outputImageRegion correspond to the actual region of (local) input !
    OutputImageRegionType outputImageRegion;
    this->CallCopyInputRegionToOutputRegion(dir, outputImageRegion, inputRegionForThread);

    ThreadedGenerateDataAtDimensionN(0, dir - 1, reporter, outputImageRegion, threadId);
    ThreadedGenerateDataAtDimensionN(1 << dir, dir - 1, reporter, outputImageRegion, threadId);
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
::ThreadedGenerateDataAtDimensionN
  (unsigned int idx, unsigned int direction, itk::ProgressReporter& reporter,
  const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Note that outputRegionForThread correspond to the actual region of input !
  OutputImagePointerType input = this->GetOutput(idx);
  OutputImagePointerType outputHighPass = this->GetOutput(idx + (1 << direction));
  OutputImagePointerType outputLowPass = OutputImageType::New();

  OutputImageRegionType outputImageRegion;
  this->CallCopyInputRegionToOutputRegion(direction, outputImageRegion, outputRegionForThread);

  if (m_SubsampleImageFactor > 1)
    {
    input = m_InternalImages[direction][idx / (1 << (direction + 1))];

    if (direction != 0)
      {
      outputLowPass = m_InternalImages[direction - 1][idx / (1 << direction)];
      outputHighPass = m_InternalImages[direction - 1][idx / (1 << direction) + 1];
      }
    }

  if (direction == 0)
    {
    // The output image has to be allocated
    // May be not valid on multithreaded process ???
    outputLowPass->SetRegions(outputImageRegion);
    outputLowPass->Allocate(); // FIXME Check this line...
    outputLowPass->FillBuffer(0);
    }

  /** Inner product iterators */
  typedef itk::ConstNeighborhoodIterator<OutputImageType> NeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct<OutputImageType>  InnerProductType;
  typedef itk::ImageRegionIterator<OutputImageType>       IteratorType;

  // Prepare the subsampling image factor, if any.
  typedef SubsampledImageRegionConstIterator<InputImageType> SubsampleIteratorType;
  typename SubsampleIteratorType::IndexType subsampling;
  subsampling.Fill(1);
  subsampling[direction] = GetSubsampleImageFactor();

  // Inner products
  InnerProductType innerProduct;

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(direction);
  highPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  highPassOperator.CreateDirectional();

  SubsampleIteratorType subItHighPass(input, outputRegionForThread);
  subItHighPass.SetSubsampleFactor(subsampling);
  subItHighPass.GoToBegin();

  NeighborhoodIteratorType itHighPass(highPassOperator.GetRadius(), input, outputRegionForThread);
  itk::PeriodicBoundaryCondition<OutputImageType> boundaryCondition;
  itHighPass.OverrideBoundaryCondition(&boundaryCondition);

  IteratorType outHighPass(outputHighPass, subItHighPass.GenerateOutputInformation());
  outHighPass.GoToBegin();

  while (!subItHighPass.IsAtEnd() && !outHighPass.IsAtEnd())
    {
    itHighPass.SetLocation(subItHighPass.GetIndex());
    outHighPass.Set(innerProduct(itHighPass, highPassOperator));

    ++subItHighPass;
    ++outHighPass;

    reporter.CompletedPixel();
    }

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(direction);
  lowPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  lowPassOperator.CreateDirectional();

  SubsampleIteratorType subItLowPass(input, outputRegionForThread);
  subItLowPass.SetSubsampleFactor(subsampling);
  subItLowPass.GoToBegin();

  NeighborhoodIteratorType itLowPass(lowPassOperator.GetRadius(), input, outputRegionForThread);
  itLowPass.OverrideBoundaryCondition(&boundaryCondition);

  IteratorType outLowPass(outputLowPass, subItLowPass.GenerateOutputInformation());
  outLowPass.GoToBegin();

  while (!subItLowPass.IsAtEnd() && !outLowPass.IsAtEnd())
    {
    itLowPass.SetLocation(subItLowPass.GetIndex());
    outLowPass.Set(innerProduct(itLowPass, lowPassOperator));

    ++subItLowPass;
    ++outLowPass;

    reporter.CompletedPixel();
    }

  // Swap input and lowPassOutput
  itk::ImageRegionConstIterator<OutputImageType> inIt(outputLowPass, outputImageRegion);
  IteratorType outIt((direction != 0 && m_SubsampleImageFactor > 1) ?
                     static_cast<OutputImageType*>(m_InternalImages[direction - 2][idx / (1 << (direction - 1))])
                     : this->GetOutput(idx),
                     outputImageRegion);

  for (inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt, ++outIt)
    {
    outIt.Set(inIt.Get());
    }

  if (direction != 0)
    {
    ThreadedGenerateDataAtDimensionN(idx, direction - 1, reporter, outputImageRegion, threadId);
    ThreadedGenerateDataAtDimensionN(idx + (1 << direction), direction - 1, reporter, outputImageRegion, threadId);
    }
}

/**
 * Template Specialization for the Wavelet::INVERSE case
 */

template <class TInputImage, class TOutputImage, class TWaveletOperator>
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::WaveletFilterBank ()
{
  this->SetNumberOfRequiredInputs(1 << InputImageDimension);

  m_UpSampleFilterFactor = 0;
  m_SubsampleImageFactor = 1;

  // TODO: For now, we force the number threads to 1 because there is a bug with multithreading in INVERSE transform
  // Resulting in discontinuities in the reconstructed images
  this->SetNumberOfThreads(1);
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  for (unsigned int i = 1; i < this->GetNumberOfInputs(); ++i)
    {
    for (unsigned int dim = 0; dim < InputImageDimension; dim++)
      {
      if (this->GetInput(0)->GetLargestPossibleRegion().GetSize()[dim]
          != this->GetInput(i)->GetLargestPossibleRegion().GetSize()[dim])
        {
        throw itk::ExceptionObject(__FILE__, __LINE__,
                                   "Input images are not of the same dimension", ITK_LOCATION);
        }
      }
    }

#if __myDebug__
  otbGenericMsgDebugMacro(<< " up sampling output regions by a factor of " << GetSubsampleImageFactor());

  otbGenericMsgDebugMacro(<< "initial region    "
                          << this->GetInput(0)->GetLargestPossibleRegion().GetSize()[0]
                          << "," << this->GetInput(0)->GetLargestPossibleRegion().GetSize()[1]);
#endif

  OutputImageRegionType newRegion;
  this->CallCopyInputRegionToOutputRegion(newRegion, this->GetInput(0)->GetLargestPossibleRegion());
  this->GetOutput()->SetRegions(newRegion);

#if __myDebug__
  otbGenericMsgDebugMacro(<< "new region output " << newRegion.GetSize()[0] << "," << newRegion.GetSize()[1]);
#endif
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::GenerateInputRequestedRegion()
throw (itk::InvalidRequestedRegionError)
  {
  Superclass::GenerateInputRequestedRegion();

  // Filter length calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(0);
  lowPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  lowPassOperator.CreateDirectional();

  unsigned int radius = lowPassOperator.GetRadius()[0];

  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(0);
  highPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  highPassOperator.CreateDirectional();

  if (radius < highPassOperator.GetRadius()[0]) radius = highPassOperator.GetRadius()[0];

  // Get the requested regionand pad it
  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
    {
    InputImagePointerType input = const_cast<InputImageType*>(this->GetInput(idx));
    InputImageRegionType  inputRegion = input->GetRequestedRegion();
    inputRegion.PadByRadius(radius);

    if (inputRegion.Crop(input->GetLargestPossibleRegion()))
      {
      input->SetRequestedRegion(inputRegion);
      }
    else
      {
      input->SetRequestedRegion(inputRegion);
      itk::InvalidRequestedRegionError err(__FILE__, __LINE__);
      err.SetLocation(ITK_LOCATION);
      err.SetDescription("Requested region is (at least partially) outside the largest possible region.");
      err.SetDataObject(input);
      throw err;
      }
    }
  }

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::BeforeThreadedGenerateData()
{
  if (InputImageDimension > 1)
    {
    // Internal images will be used only if m_SubsampleImageFactor != 1
    m_InternalImages.resize(InputImageDimension - 1);
    for (unsigned int i = 0; i < m_InternalImages.size(); ++i)
      {
      // the size is linked to the SubsampleImageFactor that is assume to be 2!!!
      m_InternalImages[i].resize(1 << (i + 1));
      }

    OutputImageRegionType intermediateRegion;
    Superclass::CallCopyInputRegionToOutputRegion(intermediateRegion,
                                                  this->GetInput(0)->GetLargestPossibleRegion());

    AllocateInternalData(intermediateRegion);
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::AllocateInternalData
  (const OutputImageRegionType& outputRegion)
{
  OutputImageRegionType largerRegion;
  OutputImageRegionType smallerRegion = outputRegion;

  for (unsigned int direction = 0; direction < InputImageDimension - 1; direction++)
    {
    this->CallCopyInputRegionToOutputRegion(direction,
                                            largerRegion, smallerRegion);

    for (unsigned int i = 0; i < m_InternalImages[direction].size(); ++i)
      {
      m_InternalImages[direction][i] = OutputImageType::New();
      m_InternalImages[direction][i]->SetRegions(largerRegion);
      m_InternalImages[direction][i]->Allocate();
      m_InternalImages[direction][i]->FillBuffer(0);
      }

    smallerRegion = largerRegion;
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::AfterThreadedGenerateData()
{
  if (m_SubsampleImageFactor > 1 && InputImageDimension > 1)
    {
    m_InternalImages.clear();
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::CallCopyOutputRegionToInputRegion
  (InputImageRegionType& destRegion, const OutputImageRegionType& srcRegion)
{
  Superclass::CallCopyOutputRegionToInputRegion(destRegion, srcRegion);

  if (GetSubsampleImageFactor() > 1)
    {
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType  srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType  destSize;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
      {
      // TODO: This seems not right in odd index cases
      destIndex[i] = srcIndex[i] / GetSubsampleImageFactor();
      destSize[i] = srcSize[i] / GetSubsampleImageFactor();
      }

    destRegion.SetIndex(destIndex);
    destRegion.SetSize(destSize);

#if 1
    // Region Padding
    LowPassOperatorType lowPassOperator;
    lowPassOperator.SetDirection(0);
    lowPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
    lowPassOperator.CreateDirectional();

    typename InputImageRegionType::SizeType radius;
    radius[0] = lowPassOperator.GetRadius()[0];

    HighPassOperatorType highPassOperator;
    highPassOperator.SetDirection(0);
    highPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
    highPassOperator.CreateDirectional();

    if (radius[0] < highPassOperator.GetRadius()[0]) radius[0] = highPassOperator.GetRadius()[0];

    for (unsigned int i = 1; i < InputImageDimension; ++i)
      radius[i] = 0;

//     for ( unsigned int i = 0; i < InputImageDimension; ++i )
//     {
//       radius[i] = lowPassOperator.GetRadius()[i];
//       if ( radius[i] < highPassOperator.GetRadius()[i] )
//         radius[i] = highPassOperator.GetRadius()[i];
//     }

    InputImageRegionType paddedRegion = destRegion;
    paddedRegion.PadByRadius(radius);

    if (paddedRegion.Crop(this->GetInput(0)->GetLargestPossibleRegion()))
      {
      destRegion = paddedRegion;
      }
#endif
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::CallCopyInputRegionToOutputRegion
  (OutputImageRegionType& destRegion, const InputImageRegionType& srcRegion)
{
  Superclass::CallCopyInputRegionToOutputRegion(destRegion, srcRegion);

  if (GetSubsampleImageFactor() > 1)
    {
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType  srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType  destSize;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
      {
      destIndex[i] = srcIndex[i] * GetSubsampleImageFactor();
      destSize[i] = srcSize[i] * GetSubsampleImageFactor();
      }

    destRegion.SetIndex(destIndex);
    destRegion.SetSize(destSize);
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::CallCopyOutputRegionToInputRegion
  (unsigned int direction,
  InputImageRegionType& destRegion, const OutputImageRegionType& srcRegion)
{
  Superclass::CallCopyOutputRegionToInputRegion(destRegion, srcRegion);

  if (GetSubsampleImageFactor() > 1)
    {
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType  srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType  destSize;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
      {
      if (i == direction)
        {
        // TODO: This seems not right in odd index cases
        destIndex[i] = srcIndex[i] / GetSubsampleImageFactor();
        destSize[i] = srcSize[i] / GetSubsampleImageFactor();
        }
      else
        {
        destIndex[i] = srcIndex[i];
        destSize[i] = srcSize[i];
        }
      }

    destRegion.SetIndex(destIndex);
    destRegion.SetSize(destSize);
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::CallCopyInputRegionToOutputRegion
  (unsigned int direction,
  OutputImageRegionType& destRegion, const InputImageRegionType& srcRegion)
{
  Superclass::CallCopyInputRegionToOutputRegion(destRegion, srcRegion);

  if (GetSubsampleImageFactor() > 1)
    {
    typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename InputImageRegionType::SizeType  srcSize = srcRegion.GetSize();

    typename OutputImageRegionType::IndexType destIndex;
    typename OutputImageRegionType::SizeType  destSize;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
      {
      if (i == direction)
        {
        destIndex[i] = srcIndex[i] * GetSubsampleImageFactor();
        destSize[i] = srcSize[i] * GetSubsampleImageFactor();
        }
      else
        {
        destIndex[i] = srcIndex[i];
        destSize[i] = srcSize[i];
        }
      }

    destRegion.SetIndex(destIndex);
    destRegion.SetSize(destSize);
    }
}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::ThreadedGenerateData
  (const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  itk::ProgressReporter reporter(this, threadId,
                                 outputRegionForThread.GetNumberOfPixels() * this->GetNumberOfInputs());

  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  unsigned int dir = 0;

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(dir);
  lowPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  lowPassOperator.CreateDirectional();

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(dir);
  highPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  highPassOperator.CreateDirectional();

  // typedef for the iterations over the input image
  typedef itk::ConstNeighborhoodIterator<OutputImageType>                                    NeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct<OutputImageType>                                     InnerProductType;
  typedef itk::ImageRegionIterator<OutputImageType>                                          IteratorType;

  // Faces iterations
  typename NeighborhoodIteratorType::RadiusType radiusMax;
  for (unsigned int idx = 0; idx < OutputImageDimension; ++idx)
    {
    radiusMax[idx] = lowPassOperator.GetRadius(idx);
    if (radiusMax[idx] < highPassOperator.GetRadius(idx)) radiusMax[idx] = highPassOperator.GetRadius(idx);
    }

  // The multiresolution case requires a SubsampleImageFilter step
  if (m_SubsampleImageFactor > 1)
    {
    for (unsigned int i = 0; i < this->GetNumberOfInputs(); i += 2)
      {
      InputImagePointerType imgLowPass = const_cast<InputImageType*>(this->GetInput(i));
      InputImagePointerType imgHighPass = const_cast<InputImageType*>(this->GetInput(i + 1));

      OutputImagePointerType outputImage = this->GetOutput();
      if (dir != InputImageDimension - 1)
        {
        outputImage = m_InternalImages[0][i / 2];
        }

      typedef otb::SubsampleImageFilter<InputImageType, OutputImageType, Wavelet::INVERSE> FilterType;
      typename FilterType::InputImageIndexType delta;
      delta.Fill(1);
      delta[dir] = this->GetSubsampleImageFactor();

      InputImagePointerType cropedLowPass = InputImageType::New();
      cropedLowPass->SetRegions(inputRegionForThread);
      cropedLowPass->Allocate();
      cropedLowPass->FillBuffer(0.);
      itk::ImageRegionIterator<InputImageType> cropedLowPassIt(cropedLowPass, inputRegionForThread);
      itk::ImageRegionIterator<InputImageType> imgLowPassIt(imgLowPass, inputRegionForThread);
      for (cropedLowPassIt.GoToBegin(), imgLowPassIt.GoToBegin();
           !cropedLowPassIt.IsAtEnd() && !imgLowPassIt.IsAtEnd();
           ++cropedLowPassIt, ++imgLowPassIt)
        {
        cropedLowPassIt.Set(imgLowPassIt.Get());
        }

      typename FilterType::Pointer overSampledLowPass = FilterType::New();
      overSampledLowPass->SetInput(cropedLowPass);
      overSampledLowPass->SetSubsampleFactor(delta);
      overSampledLowPass->Update();

      InputImagePointerType cropedHighPass = InputImageType::New();
      cropedHighPass->SetRegions(inputRegionForThread);
      cropedHighPass->Allocate();
      cropedHighPass->FillBuffer(0.);
      itk::ImageRegionIterator<InputImageType> cropedHighPassIt(cropedHighPass, inputRegionForThread);
      itk::ImageRegionIterator<InputImageType> imgHighPassIt(imgHighPass, inputRegionForThread);
      for (cropedHighPassIt.GoToBegin(), imgHighPassIt.GoToBegin();
           !cropedHighPassIt.IsAtEnd() && !imgHighPassIt.IsAtEnd();
           ++cropedHighPassIt, ++imgHighPassIt)
        {
        cropedHighPassIt.Set(imgHighPassIt.Get());
        }

      typename FilterType::Pointer overSampledHighPass = FilterType::New();
      overSampledHighPass->SetInput(cropedHighPass);
      overSampledHighPass->SetSubsampleFactor(delta);
      overSampledHighPass->Update();

      InnerProductType innerProduct;

      itk::ImageRegionIterator<OutputImageType> out
        (outputImage, overSampledLowPass->GetOutput()->GetRequestedRegion());

      NeighborhoodIteratorType lowIter(lowPassOperator.GetRadius(),
                                       overSampledLowPass->GetOutput(),
                                       overSampledLowPass->GetOutput()->GetRequestedRegion());
      itk::PeriodicBoundaryCondition<OutputImageType> boundaryCondition;
      lowIter.OverrideBoundaryCondition(&boundaryCondition);

      NeighborhoodIteratorType highIter(highPassOperator.GetRadius(),
                                        overSampledHighPass->GetOutput(),
                                        overSampledHighPass->GetOutput()->GetRequestedRegion());
      highIter.OverrideBoundaryCondition(&boundaryCondition);

      lowIter.GoToBegin();
      highIter.GoToBegin();
      out.GoToBegin();

      while (!out.IsAtEnd())
        {
        out.Set(innerProduct(lowIter, lowPassOperator)
                + innerProduct(highIter, highPassOperator));

        ++lowIter;
        ++highIter;
        ++out;

        reporter.CompletedPixel();
        }
      } // end for each overSampledLowPass/overSampledhighPass pair of entry
    }
  else // multiscale case
    {
    for (unsigned int i = 0; i < this->GetNumberOfInputs(); i += 2)
      {
      InputImagePointerType imgLowPass = const_cast<InputImageType*>(this->GetInput(i));
      InputImagePointerType imgHighPass = const_cast<InputImageType*>(this->GetInput(i + 1));

      OutputImagePointerType outputImage = this->GetOutput();
      if (dir != InputImageDimension - 1)
        {
        outputImage = m_InternalImages[0][i / 2];
        }

      InnerProductType innerProduct;

      itk::ImageRegionIterator<OutputImageType> out(outputImage, imgLowPass->GetRequestedRegion());

      NeighborhoodIteratorType lowIter(lowPassOperator.GetRadius(), imgLowPass, imgLowPass->GetRequestedRegion());
        itk::PeriodicBoundaryCondition<OutputImageType> boundaryCondition;
        lowIter.OverrideBoundaryCondition(&boundaryCondition);

        NeighborhoodIteratorType highIter(highPassOperator.GetRadius(), imgHighPass, imgLowPass->GetRequestedRegion());
        highIter.OverrideBoundaryCondition(&boundaryCondition);

        lowIter.GoToBegin();
        highIter.GoToBegin();
        out.GoToBegin();

        while (!out.IsAtEnd())
          {
          out.Set((innerProduct(lowIter, lowPassOperator)
                   + innerProduct(highIter, highPassOperator)) / 2.);

          ++lowIter;
          ++highIter;
          ++out;

          reporter.CompletedPixel();
          }
      } // end for each imgLowPass/imghighPass pair of entry
    } // end multiscale case

  if (dir != InputImageDimension - 1)
    {
    // Note that outputImageRegion correspond to the actual region of (local) input !
    OutputImageRegionType outputImageRegion;
    this->CallCopyInputRegionToOutputRegion(dir, outputImageRegion, inputRegionForThread);

    ThreadedGenerateDataAtDimensionN(dir + 1, reporter, outputImageRegion, threadId);
    }

}

template <class TInputImage, class TOutputImage, class TWaveletOperator>
void
WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
::ThreadedGenerateDataAtDimensionN
  (unsigned int direction,
  itk::ProgressReporter& reporter,
  const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  OutputImageRegionType outputImageRegion;
  this->CallCopyInputRegionToOutputRegion(direction, outputImageRegion, outputRegionForThread);

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(direction);
  lowPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  lowPassOperator.CreateDirectional();

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(direction);
  highPassOperator.SetUpSampleFactor(this->GetUpSampleFilterFactor());
  highPassOperator.CreateDirectional();

  // typedef for the iterations over the input image
  typedef itk::ConstNeighborhoodIterator<OutputImageType>                                    NeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct<OutputImageType>                                     InnerProductType;
  typedef itk::ImageRegionIterator<OutputImageType>                                          IteratorType;

  // Faces iterations
  typename NeighborhoodIteratorType::RadiusType radiusMax;
  for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
    radiusMax[i] = lowPassOperator.GetRadius(i);
    if (radiusMax[i] < highPassOperator.GetRadius(i)) radiusMax[i] = highPassOperator.GetRadius(i);
    }

  // The multiresolution case requires a SubsampleImageFilter step
  if (m_SubsampleImageFactor > 1)
    {
    for (unsigned int i = 0; i < m_InternalImages[direction - 1].size(); i += 2)
      {
      InputImagePointerType imgLowPass = m_InternalImages[direction - 1][i];
      InputImagePointerType imgHighPass = m_InternalImages[direction - 1][i + 1];

      OutputImagePointerType outputImage = this->GetOutput();
      if (direction < InputImageDimension - 1)
        {
        outputImage = m_InternalImages[direction][i / 2];
        }

      typedef otb::SubsampleImageFilter<OutputImageType, OutputImageType, Wavelet::INVERSE> FilterType;
      typename FilterType::InputImageIndexType delta;
      delta.Fill(1);
      delta[direction] = this->GetSubsampleImageFactor();

      InputImagePointerType cropedLowPass = InputImageType::New();
      cropedLowPass->SetRegions(outputRegionForThread);
      cropedLowPass->Allocate();
      cropedLowPass->FillBuffer(0.);
      itk::ImageRegionIterator<InputImageType> cropedLowPassIt(cropedLowPass, outputRegionForThread);
      itk::ImageRegionIterator<InputImageType> imgLowPassIt(imgLowPass, outputRegionForThread);
      for (cropedLowPassIt.GoToBegin(), imgLowPassIt.GoToBegin();
           !cropedLowPassIt.IsAtEnd() && !imgLowPassIt.IsAtEnd();
           ++cropedLowPassIt, ++imgLowPassIt)
        {
        cropedLowPassIt.Set(imgLowPassIt.Get());
        }

      typename FilterType::Pointer overSampledLowPass = FilterType::New();
      overSampledLowPass->SetInput(cropedLowPass);
      overSampledLowPass->SetSubsampleFactor(delta);
      overSampledLowPass->Update();

      InputImagePointerType cropedHighPass = InputImageType::New();
      cropedHighPass->SetRegions(outputRegionForThread);
      cropedHighPass->Allocate();
      cropedHighPass->FillBuffer(0.);
      itk::ImageRegionIterator<InputImageType> cropedHighPassIt(cropedHighPass, outputRegionForThread);
      itk::ImageRegionIterator<InputImageType> imgHighPassIt(imgHighPass, outputRegionForThread);
      for (cropedHighPassIt.GoToBegin(), imgHighPassIt.GoToBegin();
           !cropedHighPassIt.IsAtEnd() && !imgHighPassIt.IsAtEnd();
           ++cropedHighPassIt, ++imgHighPassIt)
        {
        cropedHighPassIt.Set(imgHighPassIt.Get());
        }

      typename FilterType::Pointer overSampledHighPass = FilterType::New();
      overSampledHighPass->SetInput(cropedHighPass);
      overSampledHighPass->SetSubsampleFactor(delta);
      overSampledHighPass->Update();

      InnerProductType innerProduct;

      itk::ImageRegionIterator<OutputImageType> out(outputImage,
                                                    overSampledLowPass->GetOutput()->GetRequestedRegion());

      // TODO: This might be the cause of the multithreading bug : we use a neighborhood iterator on cropped data
      // Are we sure that we have cropped enough data to access the neighborhood ?
      NeighborhoodIteratorType lowIter(lowPassOperator.GetRadius(),
                                       overSampledLowPass->GetOutput(),
                                       overSampledLowPass->GetOutput()->GetRequestedRegion());
      itk::PeriodicBoundaryCondition<OutputImageType> boundaryCondition;
      lowIter.OverrideBoundaryCondition(&boundaryCondition);

      NeighborhoodIteratorType highIter(highPassOperator.GetRadius(),
                                        overSampledHighPass->GetOutput(),
                                        overSampledHighPass->GetOutput()->GetRequestedRegion());
      highIter.OverrideBoundaryCondition(&boundaryCondition);

      lowIter.GoToBegin();
      highIter.GoToBegin();
      out.GoToBegin();

      while (!out.IsAtEnd())
        {
        out.Set(innerProduct(lowIter, lowPassOperator)
                + innerProduct(highIter, highPassOperator));

        ++lowIter;
        ++highIter;
        ++out;

        reporter.CompletedPixel();
        }
      } // end for each overSampledLowPass/overSampledhighPass pair of entry
    }
  else // multiscale case
    {
    for (unsigned int i = 0; i < m_InternalImages[direction - 1].size(); i += 2)
      {
      InputImagePointerType imgLowPass = m_InternalImages[direction - 1][i];
      InputImagePointerType imgHighPass = m_InternalImages[direction - 1][i + 1];

      OutputImagePointerType outputImage = this->GetOutput();
      if (direction < InputImageDimension - 1)
        {
        outputImage = m_InternalImages[direction][i / 2];
        }

      InnerProductType   innerProduct;

      itk::ImageRegionIterator<OutputImageType> out(outputImage, imgLowPass->GetRequestedRegion());

      NeighborhoodIteratorType lowIter(lowPassOperator.GetRadius(), imgLowPass, imgLowPass->GetRequestedRegion());
        itk::PeriodicBoundaryCondition<OutputImageType> boundaryCondition;
        lowIter.OverrideBoundaryCondition(&boundaryCondition);

        NeighborhoodIteratorType highIter(highPassOperator.GetRadius(), imgHighPass, imgLowPass->GetRequestedRegion());
        highIter.OverrideBoundaryCondition(&boundaryCondition);

        lowIter.GoToBegin();
        highIter.GoToBegin();
        out.GoToBegin();

        while (!out.IsAtEnd())
          {
          out.Set((innerProduct(lowIter, lowPassOperator)
                   + innerProduct(highIter, highPassOperator)) / 2.);

          ++lowIter;
          ++highIter;
          ++out;

          reporter.CompletedPixel();
          }
      } // end for each imgLowPass/imghighPass pair of entry
    }

  if (direction < InputImageDimension - 1)
    {
    ThreadedGenerateDataAtDimensionN(direction + 1, reporter, outputImageRegion, threadId);
    }
}

} // end of namespace

#endif
