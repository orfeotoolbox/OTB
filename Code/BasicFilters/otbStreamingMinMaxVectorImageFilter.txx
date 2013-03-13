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
#ifndef __otbStreamingMinMaxVectorImageFilter_txx
#define __otbStreamingMinMaxVectorImageFilter_txx
#include "otbStreamingMinMaxVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TInputImage>
PersistentMinMaxVectorImageFilter<TInputImage>
::PersistentMinMaxVectorImageFilter()
  : m_NoDataFlag(false),
    m_NoDataValue(itk::NumericTraits<InternalPixelType>::Zero)
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types

  for (int i = 1; i < 3; ++i)
    {
    typename PixelObjectType::Pointer output = static_cast<PixelObjectType*>(this->MakeOutput(i).GetPointer());
    this->itk::ProcessObject::SetNthOutput(i, output.GetPointer());
    }
}

template<class TInputImage>
itk::DataObject::Pointer
PersistentMinMaxVectorImageFilter<TInputImage>
::MakeOutput(unsigned int output)
{
  itk::DataObject::Pointer ret;
  switch (output)
    {
    case 0:
      ret = static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
    case 2:
      ret = static_cast<itk::DataObject*>(PixelObjectType::New().GetPointer());
      break;
    }
  return ret;
}

template<class TInputImage>
typename PersistentMinMaxVectorImageFilter<TInputImage>::PixelObjectType*
PersistentMinMaxVectorImageFilter<TInputImage>
::GetMinimumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename PersistentMinMaxVectorImageFilter<TInputImage>::PixelObjectType*
PersistentMinMaxVectorImageFilter<TInputImage>
::GetMinimumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
typename PersistentMinMaxVectorImageFilter<TInputImage>::PixelObjectType*
PersistentMinMaxVectorImageFilter<TInputImage>
::GetMaximumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
const typename PersistentMinMaxVectorImageFilter<TInputImage>::PixelObjectType*
PersistentMinMaxVectorImageFilter<TInputImage>
::GetMaximumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
void
PersistentMinMaxVectorImageFilter<TInputImage>
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
PersistentMinMaxVectorImageFilter<TInputImage>
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
PersistentMinMaxVectorImageFilter<TInputImage>
::Reset()
{
  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  inputPtr->UpdateOutputInformation();

  unsigned int numberOfThreads = this->GetNumberOfThreads();
  unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

  // Variable Initialization
  PixelType tempPixel;
  tempPixel.SetSize(numberOfComponent);
  tempPixel.Fill(itk::NumericTraits<InternalPixelType>::NonpositiveMin());
  this->GetMaximumOutput()->Set(tempPixel);

  tempPixel.Fill(itk::NumericTraits<InternalPixelType>::max());
  this->GetMinimumOutput()->Set(tempPixel);

  PixelType tempTemporiesPixel;
  tempTemporiesPixel.SetSize(numberOfComponent);
  tempTemporiesPixel.Fill(itk::NumericTraits<InternalPixelType>::max());
  m_ThreadMin = ArrayPixelType(numberOfThreads, tempTemporiesPixel);

  tempTemporiesPixel.Fill(itk::NumericTraits<InternalPixelType>::NonpositiveMin());
  m_ThreadMax = ArrayPixelType(numberOfThreads, tempTemporiesPixel);

}

template<class TInputImage>
void
PersistentMinMaxVectorImageFilter<TInputImage>
::Synthetize()
{
  int  i;

  int          numberOfThreads = this->GetNumberOfThreads();
  unsigned int numberOfComponent = this->GetInput()->GetNumberOfComponentsPerPixel();

  PixelType minimumVector;
  minimumVector.SetSize(numberOfComponent);
  minimumVector.Fill(itk::NumericTraits<InternalPixelType>::max());

  PixelType maximumVector;
  maximumVector.SetSize(numberOfComponent);
  maximumVector.Fill(itk::NumericTraits<InternalPixelType>::NonpositiveMin());

  // Find the min/max over all threads and accumulate count, sum and
  // sum of squares
  for (i = 0; i < numberOfThreads; ++i)
    {
    for (unsigned int j = 0; j < numberOfComponent; ++j)
      {
      if (m_ThreadMin[i][j] < minimumVector[j])
        {
        minimumVector[j] = m_ThreadMin[i][j];
        }
      if (m_ThreadMax[i][j] > maximumVector[j])
        {
        maximumVector[j] = m_ThreadMax[i][j];
        }
      }
    } // end for( i = 0; i < numberOfThreads; ++i)

  // Set the outputs
  this->GetMinimumOutput()->Set(minimumVector);
  this->GetMaximumOutput()->Set(maximumVector);
}

template<class TInputImage>
void
PersistentMinMaxVectorImageFilter<TInputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId)
{
  /**
   * Grab the input
   */
  InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  itk::ImageRegionConstIteratorWithIndex<TInputImage> it(inputPtr, outputRegionForThread);
  it.GoToBegin();

  // do the work
  while (!it.IsAtEnd())
    {
    //IndexType index = it.GetIndex();
    PixelType vectorValue = it.Get();
    for (unsigned int j = 0; j < vectorValue.GetSize(); ++j)
      {
      InternalPixelType value = vectorValue[j];

      if( (!m_NoDataFlag) || value!=m_NoDataValue )
        {
        if (value < m_ThreadMin[threadId][j])
          {
          m_ThreadMin[threadId][j] = value;
          }
        if (value > m_ThreadMax[threadId][j])
          {
          m_ThreadMax[threadId][j] = value;
          }
        }
      }
    ++it;
    progress.CompletedPixel();
    }
}

template <class TImage>
void
PersistentMinMaxVectorImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Minimum: "  << this->GetMinimumOutput()->Get() << std::endl;
  os << indent << "Maximum: " <<  this->GetMaximumOutput()->Get() << std::endl;
}

} // end namespace otb
#endif
