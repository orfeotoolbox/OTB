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


#ifndef otbSiftFastImageFilter_hxx
#define otbSiftFastImageFilter_hxx

#include "otbSiftFastImageFilter.h"

#include "otb_siftfast.h"
#include "itkContinuousIndex.h"
#include "itkImageRegionConstIterator.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputPointSet>
SiftFastImageFilter<TInputImage, TOutputPointSet>::SiftFastImageFilter() : m_ScalesNumber(1)
{
}

template <class TInputImage, class TOutputPointSet>
void SiftFastImageFilter<TInputImage, TOutputPointSet>::GenerateData()
{

  // Get the input image pointer
  const InputImageType*     inputPtr       = this->GetInput();
  OutputPointSetPointerType outputPointSet = this->GetOutput();

  typename InputImageType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();

  SiftFastImage                              siftInputImage = CreateImage(size[1], size[0]);
  itk::ImageRegionConstIterator<TInputImage> inIt(inputPtr, inputPtr->GetBufferedRegion());

  unsigned int index = 0;

  for (inIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt)
  {
    siftInputImage->pixels[index] = static_cast<float>(inIt.Get());
    ++index;
  }

  Keypoint keypts = GetKeypoints(siftInputImage, m_ScalesNumber);

  Keypoint key = keypts;

  unsigned int numkeys = 0;

  while (key)
  {
    // Get the key location
    itk::ContinuousIndex<float, 2> keyContIndex;
    keyContIndex[0] = key->col;
    keyContIndex[1] = key->row;

    OutputPointType point;
    inputPtr->TransformContinuousIndexToPhysicalPoint(keyContIndex, point);

    // Get the key descriptor
    OutputPixelType data;
    data.SetSize(128);
    for (int i = 0; i < 128; ++i)
    {
      data[i] = key->descrip[i];
    }
    outputPointSet->SetPoint(numkeys, point);
    outputPointSet->SetPointData(numkeys, data);

    // Fill the current point and its orientation
    std::pair<OutputPointType, double> pair;
    pair.first  = point;
    pair.second = key->ori;
    m_OrientationVector.push_back(pair);

    // std::cout << " In SiftFastimageFilter : point " << point << " have orientation   " << key->ori<< std::endl;

    // go to next key
    ++numkeys;
    key = key->next;
  }
  FreeKeypoints(keypts);
  DestroyAllResources();
}
/*
 * PrintSelf Method
 */
template <class TInputImage, class TOutputPointSet>
void SiftFastImageFilter<TInputImage, TOutputPointSet>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
