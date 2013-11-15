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
#ifndef __otbStreamingCompareImageFilter_txx
#define __otbStreamingCompareImageFilter_txx
#include "otbStreamingCompareImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TInputImage>
PersistentCompareImageFilter<TInputImage>
::PersistentCompareImageFilter() : m_SquareOfDifferences(1), m_AbsoluteValueOfDifferences(1),
 m_ThreadMinRef(1), m_ThreadMaxRef(1), m_Count(1)
{
  this->SetNumberOfRequiredInputs( 2 );
  // first output is a copy of the image, DataObject created by
  // superclass

  // allocate the data objects for the outputs which are
  // just decorators around real types
  for (int i = 1; i < 4; ++i)
  {
    typename RealObjectType::Pointer output
    = static_cast<RealObjectType*>(this->MakeOutput(i).GetPointer());
    this->itk::ProcessObject::SetNthOutput(i, output.GetPointer());
  }

  this->GetPSNROutput()->Set(itk::NumericTraits<RealType>::max());
  this->GetMSEOutput()->Set(itk::NumericTraits<RealType>::max());
  this->GetMAEOutput()->Set(itk::NumericTraits<RealType>::max());

  this->Reset();
}

/**
 * Connect one of the first operand
 */
template<class TInputImage>
void
PersistentCompareImageFilter<TInputImage>
::SetInput1(const TInputImage *image)
{
  // The ProcessObject is not const-correct so the const_cast is required here
  this->SetNthInput(0, const_cast<TInputImage *>(image));
}

/**
 * Connect one of the second operand
 */
template<class TInputImage>
void
PersistentCompareImageFilter<TInputImage>
::SetInput2(const TInputImage *image)
{
  // The ProcessObject is not const-correct so the const_cast is required here
  this->SetNthInput(1, const_cast<TInputImage *>(image));
}

template<class TInputImage>
const TInputImage *
PersistentCompareImageFilter<TInputImage>
::GetInput1()
{
  if (this->GetNumberOfInputs()<1)
  {
    return 0;
  }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template<class TInputImage>
const TInputImage *
PersistentCompareImageFilter<TInputImage>
::GetInput2()
{
  if (this->GetNumberOfInputs()<2)
  {
    return 0;
  }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
}

template<class TInputImage>
typename itk::DataObject::Pointer
PersistentCompareImageFilter<TInputImage>
::MakeOutput(unsigned int output)
{
  switch (output)
    {
    case 0:
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
    case 2:
    case 3:
      return static_cast<itk::DataObject*>(RealObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
}

template<class TInputImage>
typename PersistentCompareImageFilter<TInputImage>::RealObjectType*
PersistentCompareImageFilter<TInputImage>
::GetPSNROutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename PersistentCompareImageFilter<TInputImage>::RealObjectType*
PersistentCompareImageFilter<TInputImage>
::GetPSNROutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
typename PersistentCompareImageFilter<TInputImage>::RealObjectType*
PersistentCompareImageFilter<TInputImage>
::GetMSEOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
const typename PersistentCompareImageFilter<TInputImage>::RealObjectType*
PersistentCompareImageFilter<TInputImage>
::GetMSEOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
typename PersistentCompareImageFilter<TInputImage>::RealObjectType*
PersistentCompareImageFilter<TInputImage>
::GetMAEOutput()
{
  return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
const typename PersistentCompareImageFilter<TInputImage>::RealObjectType*
PersistentCompareImageFilter<TInputImage>
::GetMAEOutput() const
{
  return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
void
PersistentCompareImageFilter<TInputImage>
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
PersistentCompareImageFilter<TInputImage>
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
PersistentCompareImageFilter<TInputImage>
::Synthetize()
{
  int      i;
  long     count;
  RealType squareOfDifferences, absoluteValueOfDifferences;
  
  int numberOfThreads = this->GetNumberOfThreads();

  PixelType minimumRef,  maximumRef;
  RealType  mse;
  RealType  mae;
  RealType  psnr;

  squareOfDifferences = absoluteValueOfDifferences = itk::NumericTraits<RealType>::Zero;
  count = 0;

  // Find min/max and the accumulate count and difference of squares over all threads
  minimumRef = itk::NumericTraits<PixelType>::max();
  maximumRef = itk::NumericTraits<PixelType>::NonpositiveMin();
  
  for (i = 0; i < numberOfThreads; ++i)
    {
    count += m_Count[i];
    squareOfDifferences += m_SquareOfDifferences[i];
    absoluteValueOfDifferences += m_AbsoluteValueOfDifferences[i];
    
    if (m_ThreadMinRef[i] < minimumRef)
      {
      minimumRef = m_ThreadMinRef[i];
      }
    if (m_ThreadMaxRef[i] > maximumRef)
      {
      maximumRef = m_ThreadMaxRef[i];
      }
    }
  
  // compute mse
  mse = (count != 0) ? squareOfDifferences / static_cast<RealType>(count) : 0.;
  // compute mse
  mae = (count != 0) ? absoluteValueOfDifferences / static_cast<RealType>(count) : 0.;
  
  //compute psnr
  psnr = (vcl_abs(mse)>0.0000000001 && (maximumRef-minimumRef)>0.0000000001) ? 10.*vcl_log10(((maximumRef-minimumRef)*(maximumRef-minimumRef))/mse):0.;
  // Set the output
  this->GetMSEOutput()->Set(mse);
  this->GetMAEOutput()->Set(mae);
  this->GetPSNROutput()->Set(psnr);
}

template<class TInputImage>
void
PersistentCompareImageFilter<TInputImage>
::Reset()
{
  int numberOfThreads = this->GetNumberOfThreads();

  // Resize the thread temporaries
  m_Count.SetSize(numberOfThreads);
  m_SquareOfDifferences.SetSize(numberOfThreads);
  m_AbsoluteValueOfDifferences.SetSize(numberOfThreads);

  m_ThreadMinRef.SetSize(numberOfThreads);
  m_ThreadMaxRef.SetSize(numberOfThreads);
  
  // Initialize the temporaries
  m_Count.Fill(itk::NumericTraits<long>::Zero);
  m_SquareOfDifferences.Fill(itk::NumericTraits<RealType>::Zero);
  m_AbsoluteValueOfDifferences.Fill(itk::NumericTraits<RealType>::Zero);
  m_ThreadMinRef.Fill(itk::NumericTraits<PixelType>::max());
  m_ThreadMaxRef.Fill(itk::NumericTraits<PixelType>::NonpositiveMin());
}

template<class TInputImage>
void
PersistentCompareImageFilter<TInputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  /**
   * Grab the input
   */
  InputImagePointer inputPtr1 =  const_cast<TInputImage *>(this->GetInput(0));
  InputImagePointer inputPtr2 =  const_cast<TInputImage *>(this->GetInput(1));
  
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  RealType  realValue1, realValue2;
  PixelType value1, value2;
  
  itk::ImageRegionConstIterator<TInputImage> it1(inputPtr1, outputRegionForThread);
  itk::ImageRegionConstIterator<TInputImage> it2(inputPtr2, outputRegionForThread);

  it1.GoToBegin();
  it2.GoToBegin();
  // do the work
  while (!it1.IsAtEnd() && !it2.IsAtEnd())
    {
    value1 = it1.Get();
    realValue1 = static_cast<RealType>(value1);

    value2 = it2.Get();
    realValue2 = static_cast<RealType>(value2);

    if (value1 < m_ThreadMinRef[threadId])
      {
      m_ThreadMinRef[threadId] = value1;
      }
    if (value1 > m_ThreadMaxRef[threadId])
      {
      m_ThreadMaxRef[threadId] = value1;
      }
    
    m_SquareOfDifferences[threadId] += ( realValue1 - realValue2 ) * ( realValue1 - realValue2 );
    m_AbsoluteValueOfDifferences[threadId] += vcl_abs( realValue1 - realValue2 );
    m_Count[threadId]++;
    ++it1;
    ++it2;
    progress.CompletedPixel();
    }
}
template <class TImage>
void
PersistentCompareImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "PSNR: "     << this->GetPSNR() << std::endl;
  os << indent << "MSE: "    << this->GetMSE() << std::endl;
  os << indent << "MAE: " << this->GetMAE() << std::endl;
}
} // end namespace otb
#endif
