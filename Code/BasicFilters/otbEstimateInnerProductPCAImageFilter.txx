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
#ifndef __otbEstimateInnerProductPCAImageFilter_txx
#define __otbEstimateInnerProductPCAImageFilter_txx

#include "otbEstimateInnerProductPCAImageFilter.h"

#include <vnl/algo/vnl_generalized_eigensystem.h>
#include <vnl/vnl_math.h>

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
EstimateInnerProductPCAImageFilter<TInputImage, TOutputImage>
::EstimateInnerProductPCAImageFilter()
{
  m_NumberOfPrincipalComponentsRequired = 1;
}

/**
 * Printself
 */
template <class TInputImage, class TOutputImage>
void
EstimateInnerProductPCAImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 * GenerateOutputInformation
 */
template<class TInputImage, class TOutputImage>
void
EstimateInnerProductPCAImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfPrincipalComponentsRequired);
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage>
void
EstimateInnerProductPCAImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
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
void
EstimateInnerProductPCAImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
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
  itk::ImageRegionConstIterator<TInputImage>  inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

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
      for (unsigned int vec_number = 0;
           vec_number < m_NumberOfPrincipalComponentsRequired;
           ++vec_number, indexNumberOfTrainingImages--)
        {
        outputPixel[vec_number] += static_cast<OutputInternalPixelType>(static_cast<double>(
                                                                          inputPixel[img_number]) *
                                                                        static_cast<double>(
                                                                          m_EigenVectorsOfInnerProductMatrix[img_number
                                                                          ][indexNumberOfTrainingImages]));
        }
      }
    outputIt.Set(outputPixel);
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }

}

}

#endif
