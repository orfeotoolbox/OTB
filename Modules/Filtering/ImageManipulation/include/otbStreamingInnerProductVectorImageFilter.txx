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
#ifndef __otbStreamingInnerProductVectorImageFilter_txx
#define __otbStreamingInnerProductVectorImageFilter_txx
#include "otbStreamingInnerProductVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TInputImage>
PersistentInnerProductVectorImageFilter<TInputImage>
::PersistentInnerProductVectorImageFilter()
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types

  // allocate the data objects for the outputs which are
  // just decorators around matrix types
  typename ImageType::Pointer output1 = static_cast<ImageType*>(this->MakeOutput(0).GetPointer());
  this->itk::ProcessObject::SetNthOutput(0, output1.GetPointer());
  typename MatrixObjectType::Pointer output2 = static_cast<MatrixObjectType*>(this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(1, output2.GetPointer());

  m_CenterData = true;
}

template<class TInputImage>
itk::DataObject::Pointer
PersistentInnerProductVectorImageFilter<TInputImage>
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

template<class TInputImage>
typename PersistentInnerProductVectorImageFilter<TInputImage>::MatrixObjectType*
PersistentInnerProductVectorImageFilter<TInputImage>
::GetInnerProductOutput()
{
  return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename PersistentInnerProductVectorImageFilter<TInputImage>::MatrixObjectType*
PersistentInnerProductVectorImageFilter<TInputImage>
::GetInnerProductOutput() const
{
  return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
void
PersistentInnerProductVectorImageFilter<TInputImage>
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

template<class TInputImage>
void
PersistentInnerProductVectorImageFilter<TInputImage>
::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage>
void
PersistentInnerProductVectorImageFilter<TInputImage>
::Reset()
{
  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  inputPtr->UpdateOutputInformation();

  if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
    {
    this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
    }

  unsigned int numberOfThreads = this->GetNumberOfThreads();
  unsigned int numberOfTrainingImages = inputPtr->GetNumberOfComponentsPerPixel();
  // Set the number of training image
  MatrixType tempMatrix;
  tempMatrix.set_size(numberOfTrainingImages, numberOfTrainingImages);
  tempMatrix.fill(0);
  m_ThreadInnerProduct = ArrayMatrixType(numberOfThreads, tempMatrix);

  MatrixType initMatrix;
  initMatrix.set_size(numberOfTrainingImages, numberOfTrainingImages);
  initMatrix.fill(0);
  this->GetInnerProductOutput()->Set(initMatrix);

}

template<class TInputImage>
void
PersistentInnerProductVectorImageFilter<TInputImage>
::Synthetize()
{
  // Compute Inner product Matrix
  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  unsigned int  numberOfTrainingImages = inputPtr->GetNumberOfComponentsPerPixel();
  unsigned int  numberOfThreads = this->GetNumberOfThreads();
  MatrixType    innerProduct;
  innerProduct.set_size(numberOfTrainingImages, numberOfTrainingImages);
  innerProduct.fill(0);

  // Concatenate threaded matrix
  for (unsigned int thread = 0; thread < numberOfThreads; thread++)
    {
    innerProduct += m_ThreadInnerProduct[thread];
    }

  //---------------------------------------------------------------------
  // Fill the rest of the inner product matrix and make it symmetric
  //---------------------------------------------------------------------
  for (unsigned int band_x = 0; band_x < (numberOfTrainingImages - 1); ++band_x)
    {
    for (unsigned int band_y = band_x + 1; band_y < numberOfTrainingImages; ++band_y)
      {
      innerProduct[band_x][band_y] = innerProduct[band_y][band_x];
      }  // end band_y loop
    } // end band_x loop

  if ((numberOfTrainingImages - 1) != 0)
    {
    innerProduct /= (numberOfTrainingImages - 1);
    }
  else
    {
    innerProduct.fill(0);
    }

  // Set the output
  this->GetInnerProductOutput()->Set(innerProduct);
}

template<class TInputImage>
void
PersistentInnerProductVectorImageFilter<TInputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  /**
   * Grab the input
   */
  InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  unsigned int numberOfTrainingImages = inputPtr->GetNumberOfComponentsPerPixel();

  itk::ImageRegionConstIterator<TInputImage> it(inputPtr, outputRegionForThread);
  if (m_CenterData == true)
    {
    it.GoToBegin();
    // do the work
    while (!it.IsAtEnd())
      {
      PixelType vectorValue = it.Get();
      double mean(0.);
      for (unsigned int i = 0; i < vectorValue.GetSize(); ++i)
        {
        mean += static_cast<double>(vectorValue[i]);
        }
      mean /= static_cast<double>(vectorValue.GetSize());

      // Matrix iteration
      for (unsigned int band_x = 0; band_x < numberOfTrainingImages; ++band_x)
        {
        for (unsigned int band_y = 0; band_y <= band_x; ++band_y)
          {
          m_ThreadInnerProduct[threadId][band_x][band_y] +=
            (static_cast<double>(vectorValue[band_x]) - mean) * (static_cast<double>(vectorValue[band_y]) - mean);
          }   // end: band_y loop
        } // end: band_x loop
      ++it;
      progress.CompletedPixel();
      } // end: looping through the image
    }
  else
    {
    it.GoToBegin();
    // do the work
    while (!it.IsAtEnd())
      {
      PixelType vectorValue = it.Get();
      // Matrix iteration
      for (unsigned int band_x = 0; band_x < numberOfTrainingImages; ++band_x)
        {
        for (unsigned int band_y = 0; band_y <= band_x; ++band_y)
          {
          m_ThreadInnerProduct[threadId][band_x][band_y] +=
            (static_cast<double>(vectorValue[band_x])) * (static_cast<double>(vectorValue[band_y]));
          }   // end: band_y loop
        } // end: band_x loop
      ++it;
      progress.CompletedPixel();
      } // end: looping through the image
    }
}

template <class TImage>
void
PersistentInnerProductVectorImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "m_CenterData: " << m_CenterData << std::endl;
  os << indent << "InnerProduct: " << this->GetInnerProductOutput()->Get() << std::endl;

}

} // end namespace otb
#endif
