/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbEstimateInnerProductPCAImageFilter_hxx
#define otbEstimateInnerProductPCAImageFilter_hxx

#include "otbEstimateInnerProductPCAImageFilter.h"

#include <vnl/algo/vnl_generalized_eigensystem.h>
#include <vnl/vnl_math.h>

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
EstimateInnerProductPCAImageFilter<TInputImage, TOutputImage>::EstimateInnerProductPCAImageFilter()
  : m_NumberOfPrincipalComponentsRequired(1), m_CenterData(true)
{
}

/**
 * Printself
 */
template <class TInputImage, class TOutputImage>
void EstimateInnerProductPCAImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 * GenerateOutputInformation
 */
template <class TInputImage, class TOutputImage>
void EstimateInnerProductPCAImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfPrincipalComponentsRequired);
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage>
void EstimateInnerProductPCAImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
{
  // Instantiation object
  StreamingInnerProductPointer streamingInnerProduct = StreamingInnerProductType::New();
  streamingInnerProduct->SetInput(const_cast<InputImageType*>(this->GetInput()));
  streamingInnerProduct->SetCenterData(m_CenterData);
  streamingInnerProduct->Update();
  m_InnerProduct = streamingInnerProduct->GetInnerProduct();

  MatrixType identityMatrix(m_InnerProduct.rows(), m_InnerProduct.columns());
  identityMatrix.set_identity();
  vnl_generalized_eigensystem eigenVectors_eigenValues(m_InnerProduct, identityMatrix);
  m_EigenVectorsOfInnerProductMatrix = eigenVectors_eigenValues.V;
}

template <class TInputImage, class TOutputImage>
void EstimateInnerProductPCAImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                                         itk::ThreadIdType threadId)
{
  typename InputImageType::ConstPointer inputPtr  = this->GetInput();
  typename OutputImageType::Pointer     outputPtr = this->GetOutput();

  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  unsigned int numberOfTrainingImages = inputPtr->GetNumberOfComponentsPerPixel();

  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIterator<TInputImage> inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<TOutputImage>     outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while (!inputIt.IsAtEnd())
  {
    InputPixelType  inputPixel = inputIt.Get();
    OutputPixelType outputPixel;
    outputPixel.SetSize(m_NumberOfPrincipalComponentsRequired);
    outputPixel.Fill(0);

    for (unsigned int img_number = 0; img_number < numberOfTrainingImages; ++img_number)
    {
      unsigned int indexNumberOfTrainingImages = numberOfTrainingImages - 1;
      for (unsigned int vec_number = 0; vec_number < m_NumberOfPrincipalComponentsRequired; ++vec_number, indexNumberOfTrainingImages--)
      {
        outputPixel[vec_number] += static_cast<OutputInternalPixelType>(
            static_cast<double>(inputPixel[img_number]) * static_cast<double>(m_EigenVectorsOfInnerProductMatrix[img_number][indexNumberOfTrainingImages]));
      }
    }
    outputIt.Set(outputPixel);
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel(); // potential exception thrown here
  }
}
}

#endif
