/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbPointSetToDensityImageFilter_txx
#define __otbPointSetToDensityImageFilter_txx

#include "otbPointSetToDensityImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
/**---------------------------------------------------------
 * Constructor
 ----------------------------------------------------------*/
template <class TInputPointSet, class TOutputImage, class TDensityFunction>
PointSetToDensityImageFilter<TInputPointSet, TOutputImage, TDensityFunction>
::PointSetToDensityImageFilter()
{
  m_Radius = 1;
}

/*-------------------------------------------------------
 * GenerateData
 --------------------------------------------------------*/
template <class TInputPointSet, class TOutputImage, class TDensityFunction>
void
PointSetToDensityImageFilter<TInputPointSet, TOutputImage, TDensityFunction>
::GenerateData(void)
{
  //Call the GenerateData() from itk::ImageSource which allow threading
  Superclass::Superclass::GenerateData();
}

/*-------------------------------------------------------
 * ThreadedGenerateData
 --------------------------------------------------------*/
template <class TInputPointSet, class TOutputImage, class TDensityFunction>
void
PointSetToDensityImageFilter<TInputPointSet, TOutputImage, TDensityFunction>
::ThreadedGenerateData(
  const OutputImageRegionType&     outputRegionForThread,
  itk::ThreadIdType threadId)
{
//  sleep(threadId);
//  std::cerr << threadId << " -> " << outputRegionForThread.GetIndex() << std::endl;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  typename OutputImageType::Pointer outPtr = this->GetOutput();

  PointSetDensityFunctionPointerType densityComputeFunction = PointSetDensityFunctionType::New();
  densityComputeFunction->SetPointSet(this->GetInput());
  densityComputeFunction->SetRadius(m_Radius);

  /** Point*/
  InputType pCenter;
  IndexType index;
  itk::ImageRegionIterator<OutputImageType> itOut(outPtr,
                                                  outputRegionForThread);
  itOut.GoToBegin();

  while (!itOut.IsAtEnd())
    {
    index = itOut.GetIndex();
    outPtr->TransformIndexToPhysicalPoint(index, pCenter);

    itOut.Set(densityComputeFunction->Evaluate(pCenter));
    ++itOut;
    progress.CompletedPixel();
    }
}

/*----------------------------------------------------------------
  GenerateOutputInformation
  -----------------------------------------------------------------*/
template <class TInputPointSet, class TOutputImage, class TDensityFunction>
void
PointSetToDensityImageFilter<TInputPointSet, TOutputImage, TDensityFunction>
::GenerateOutputInformation(void)
{
  //Superclass::GenerateOutputInformation();
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if (!outputPtr)
    {
    return;
    }

  typename OutputImageType::RegionType region;
  IndexType                            start;
  start.Fill(0);

  region.SetSize(this->GetSize());
  region.SetIndex(start);

  outputPtr->SetOrigin(this->GetOrigin());
  outputPtr->SetSpacing(this->GetSpacing());
  outputPtr->SetRegions(region);
}

/*----------------------------------------------------------------
  PrintSelf
  -----------------------------------------------------------------*/
template <class TInputPointSet, class TOutputImage, class TDensityFunction>
void
PointSetToDensityImageFilter<TInputPointSet, TOutputImage, TDensityFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}
#endif
