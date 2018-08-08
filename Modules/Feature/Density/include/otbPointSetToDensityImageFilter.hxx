/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbPointSetToDensityImageFilter_hxx
#define otbPointSetToDensityImageFilter_hxx

#include "otbPointSetToDensityImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "otbMacro.h" //for OTB_DISABLE_DYNAMIC_MT;
namespace otb
{
/**---------------------------------------------------------
 * Constructor
 ----------------------------------------------------------*/
template <class TInputPointSet, class TOutputImage, class TDensityFunction>
PointSetToDensityImageFilter<TInputPointSet, TOutputImage, TDensityFunction>
::PointSetToDensityImageFilter()
{
  OTB_DISABLE_DYNAMIC_MT;
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
  outputPtr->SetSignedSpacing(this->GetSpacing());
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
