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
#ifndef __otbStreamingMatrixTransposeMatrixImageFilter_txx
#define __otbStreamingMatrixTransposeMatrixImageFilter_txx

#include "otbStreamingMatrixTransposeMatrixImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"

namespace otb
{

template<class TInputImage, class TInputImage2>
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::PersistentMatrixTransposeMatrixImageFilter()
{
  this->SetNumberOfRequiredInputs(2);

  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types

  typename ImageType::Pointer output1 = static_cast<ImageType*>(this->MakeOutput(0).GetPointer());
  this->itk::ProcessObject::SetNthOutput(0, output1.GetPointer());
  typename MatrixObjectType::Pointer output2 = static_cast<MatrixObjectType*>(this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(1, output2.GetPointer());

  // false means no pad added
  m_UsePadFirstInput = false;
  m_UsePadSecondInput = false;

  // Number of component initialization
  m_NumberOfComponents1 = 0;
  m_NumberOfComponents2 = 0;
}

template<class TInputImage, class TInputImage2>
itk::DataObject::Pointer
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::MakeOutput(unsigned int output)
{
  switch (output)
    {
    case 0:
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
      return static_cast<itk::DataObject*>(MatrixObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }

}
template<class TInputImage, class TInputImage2>
typename PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>::MatrixObjectType*
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::GetResultOutput()
{
  return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TInputImage2>
const typename PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>::MatrixObjectType*
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::GetResultOutput() const
{
  return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TInputImage2>
void
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();

  if (this->GetFirstInput() && this->GetSecondInput())
    {
    InputImagePointer image = const_cast<typename Superclass::InputImageType *>(this->GetFirstInput());
    InputImagePointer image2 = const_cast<typename Superclass::InputImageType *>(this->GetSecondInput());
    image->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
    image2->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
    }
}
template<class TInputImage, class TInputImage2>
void
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (this->GetFirstInput())
    {
    this->GetOutput()->CopyInformation(this->GetFirstInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetFirstInput()->GetLargestPossibleRegion());
    }

  if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
    {
    this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
    }
}

template<class TInputImage, class TInputImage2>
void
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage, class TInputImage2>
void
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::Reset()
{

  TInputImage * inputPtr1 = const_cast<TInputImage *>(this->GetFirstInput());
  inputPtr1->UpdateOutputInformation();
  TInputImage2 * inputPtr2 = const_cast<TInputImage2 *>(this->GetSecondInput());
  inputPtr2->UpdateOutputInformation();

  if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
    {
    this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
    }

  if (inputPtr1->GetLargestPossibleRegion().GetSize() !=  inputPtr2->GetLargestPossibleRegion().GetSize())
    {
    itkExceptionMacro(<< " Can't multiply the transposed matrix of a "
                      << inputPtr1->GetLargestPossibleRegion().GetSize()
                      << " and a "
                      << inputPtr2->GetLargestPossibleRegion().GetSize()
                      << " matrix ");
    }

  m_NumberOfComponents1 = inputPtr1->GetNumberOfComponentsPerPixel();
  m_NumberOfComponents2 = inputPtr2->GetNumberOfComponentsPerPixel();
  unsigned int numberOfThreads = this->GetNumberOfThreads();

  if (m_UsePadFirstInput == true)
    {
    m_NumberOfComponents1++;
    }
  if (m_UsePadSecondInput == true)
    {
    m_NumberOfComponents2++;
    }

  MatrixType tempMatrix, initMatrix;
  tempMatrix.SetSize(m_NumberOfComponents1, m_NumberOfComponents2);
  tempMatrix.Fill(itk::NumericTraits<RealType>::Zero);
  m_ThreadSum = ArrayMatrixType(numberOfThreads, tempMatrix);

  initMatrix.SetSize(m_NumberOfComponents2, m_NumberOfComponents2);
  initMatrix.Fill(itk::NumericTraits<RealType>::Zero);
  this->GetResultOutput()->Set(initMatrix);
}

template<class TInputImage, class TInputImage2>
void
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::Synthetize()
{
  unsigned int numberOfThreads = this->GetNumberOfThreads();
  MatrixType   resultMatrix;
  resultMatrix.SetSize(m_NumberOfComponents1, m_NumberOfComponents2);
  resultMatrix.Fill(itk::NumericTraits<RealType>::Zero);

  for (unsigned int thread = 0; thread < numberOfThreads; thread++)
    {
    /** TODO
     * To modify using + method operator. If we use it now -> exceptionmacro (no GetClassName...)
     * resultMatrix += m_ThreadSum[thread];
     **/
    for (unsigned int i = 0; i < resultMatrix.Rows(); ++i)
      {
      for (unsigned int j = 0; j < resultMatrix.Cols(); ++j)
        {
        resultMatrix(i, j) += m_ThreadSum[thread](i, j);
        }
      }
    /********END TODO ******/
    }
  this->GetResultOutput()->Set(resultMatrix);
}

template<class TInputImage, class TInputImage2>
void
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  /**
   * Grab the input
   */
  InputImagePointer input1Ptr = const_cast<TInputImage *>(this->GetFirstInput());
  InputImagePointer input2Ptr = const_cast<TInputImage2 *>(this->GetSecondInput());

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  input1Ptr->SetRequestedRegion(outputRegionForThread);
  input2Ptr->SetRequestedRegion(outputRegionForThread);
  input1Ptr->PropagateRequestedRegion();
  input1Ptr->UpdateOutputData();
  input2Ptr->PropagateRequestedRegion();
  input2Ptr->UpdateOutputData();

  itk::ImageRegionConstIterator<TInputImage> it1(input1Ptr, outputRegionForThread);
  itk::ImageRegionConstIterator<TInputImage2> it2(input2Ptr, outputRegionForThread);
  it1.GoToBegin();
  it2.GoToBegin();

  // loop the second image and get one pixel a time
  while (!it1.IsAtEnd())
    {
    PixelType  vectorValue1 = it1.Get();
    PixelType2 vectorValue2 = it2.Get();

    // Add a first component to vectorValue2 and vectorValue1 filled with ones.
    if (m_UsePadFirstInput == true)
      {
      PixelType vectortemp1(vectorValue1.Size() + 1);
      vectortemp1[0] = 1;
      for (unsigned int n = 0; n < vectorValue1.Size(); ++n)
        {
        vectortemp1[n + 1] = vectorValue1[n];

        }
      vectorValue1.SetSize(vectortemp1.Size());
      vectorValue1 = vectortemp1;
      }

    if (m_UsePadSecondInput == true)
      {
      PixelType2 vectortemp2(vectorValue2.Size() + 1);
      vectortemp2[0] = 1;
      for (unsigned int m = 0; m < vectorValue2.Size(); m++)
        {
        vectortemp2[m + 1] = vectorValue2[m];

        }
      vectorValue2.SetSize(vectortemp2.Size());
      vectorValue2 = vectortemp2;
      }

    for (unsigned int i = 0; i < vectorValue1.Size(); ++i)
      {
      for (unsigned int j = 0; j < vectorValue2.Size(); ++j)
        {
        m_ThreadSum[threadId](i, j) += static_cast<RealType>(vectorValue1[i]) * static_cast<RealType>(vectorValue2[j]);
        }

      }
    ++it1;
    ++it2;
    progress.CompletedPixel();
    }
}

template<class TInputImage, class TInputImage2>
void
PersistentMatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Result: "  << this->GetResultOutput()->Get() << std::endl;
}

} // end namespace otb
#endif
