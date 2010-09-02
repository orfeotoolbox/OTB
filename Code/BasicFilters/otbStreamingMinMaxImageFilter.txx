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
#ifndef __otbStreamingMinMaxImageFilter_txx
#define __otbStreamingMinMaxImageFilter_txx
#include "otbStreamingMinMaxImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TInputImage>
PersistentMinMaxImageFilter<TInputImage>
::PersistentMinMaxImageFilter() : m_ThreadMin(1), m_ThreadMax(1)
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types
  for (int i = 1; i < 3; ++i)
    {
    typename PixelObjectType::Pointer output
      = static_cast<PixelObjectType*>(this->MakeOutput(i).GetPointer());
    this->itk::ProcessObject::SetNthOutput(i, output.GetPointer());
    }

  this->GetMinimumOutput()->Set(itk::NumericTraits<PixelType>::max());
  this->GetMaximumOutput()->Set(itk::NumericTraits<PixelType>::NonpositiveMin());

  this->Reset();
}

template<class TInputImage>
typename itk::DataObject::Pointer
PersistentMinMaxImageFilter<TInputImage>
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
typename PersistentMinMaxImageFilter<TInputImage>::PixelObjectType*
PersistentMinMaxImageFilter<TInputImage>
::GetMinimumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename PersistentMinMaxImageFilter<TInputImage>::PixelObjectType*
PersistentMinMaxImageFilter<TInputImage>
::GetMinimumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
typename PersistentMinMaxImageFilter<TInputImage>::PixelObjectType*
PersistentMinMaxImageFilter<TInputImage>
::GetMaximumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
const typename PersistentMinMaxImageFilter<TInputImage>::PixelObjectType*
PersistentMinMaxImageFilter<TInputImage>
::GetMaximumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
void
PersistentMinMaxImageFilter<TInputImage>
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
PersistentMinMaxImageFilter<TInputImage>
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
PersistentMinMaxImageFilter<TInputImage>
::Synthetize()
{
  int      i;
  int numberOfThreads = this->GetNumberOfThreads();

  PixelType minimum = itk::NumericTraits<PixelType>::max();
  PixelType maximum = itk::NumericTraits<PixelType>::NonpositiveMin();
  for (i = 0; i < numberOfThreads; ++i)
    {
    if (m_ThreadMin[i] < minimum)
      {
      minimum = m_ThreadMin[i];
      }
    if (m_ThreadMax[i] > maximum)
      {
      maximum = m_ThreadMax[i];
      }
    }

  // Set the outputs
  this->GetMinimumOutput()->Set(minimum);
  this->GetMaximumOutput()->Set(maximum);
}

template<class TInputImage>
void
PersistentMinMaxImageFilter<TInputImage>
::Reset()
{
  int numberOfThreads = this->GetNumberOfThreads();
  m_ThreadMin.SetSize(numberOfThreads);
  m_ThreadMax.SetSize(numberOfThreads);
  m_ThreadMin.Fill(itk::NumericTraits<PixelType>::max());
  m_ThreadMax.Fill(itk::NumericTraits<PixelType>::NonpositiveMin());
}

template<class TInputImage>
void
PersistentMinMaxImageFilter<TInputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread,
                       int threadId)
{
  InputImagePointer inputPtr =  const_cast<TInputImage *>(this->GetInput(0));
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  PixelType value;

  itk::ImageRegionConstIterator<TInputImage> it(inputPtr, outputRegionForThread);
  it.GoToBegin();
  // do the work
  while (!it.IsAtEnd())
    {
    value = it.Get();
    if (value < m_ThreadMin[threadId])
      {
      m_ThreadMin[threadId] = value;
      }
    if (value > m_ThreadMax[threadId])
      {
      m_ThreadMax[threadId] = value;
      }
    ++it;
    progress.CompletedPixel();
    }
}

template <class TImage>
void
PersistentMinMaxImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Minimum: "
     << static_cast<typename itk::NumericTraits<PixelType>::PrintType>(this->GetMinimum()) << std::endl;
  os << indent << "Maximum: "
     << static_cast<typename itk::NumericTraits<PixelType>::PrintType>(this->GetMaximum()) << std::endl;
}

} // end namespace otb
#endif
