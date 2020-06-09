/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageList_hxx
#define otbImageList_hxx

#include "otbImageList.h"
#include "otbMacro.h"

namespace otb
{

template <class TImage>
void ImageList<TImage>::UpdateOutputData()
{
  Superclass::UpdateOutputData();
  for (ConstIterator it = this->Begin(); it != this->End(); ++it)
  {
    if (it.Get()->GetUpdateMTime() < it.Get()->GetPipelineMTime() || it.Get()->GetDataReleased() || it.Get()->RequestedRegionIsOutsideOfTheBufferedRegion())
    {
      if (it.Get()->GetSource())
      {
        it.Get()->GetSource()->UpdateOutputData(it.Get());
      }
    }
  }
}

template <class TImage>
void ImageList<TImage>::PropagateRequestedRegion()
{
  Superclass::PropagateRequestedRegion();

  for (ConstIterator it = this->Begin(); it != this->End(); ++it)
  {
    if (it.Get()->GetUpdateMTime() < it.Get()->GetPipelineMTime() || it.Get()->GetDataReleased() || it.Get()->RequestedRegionIsOutsideOfTheBufferedRegion())
    {

      if (it.Get()->GetSource())
      {
        it.Get()->GetSource()->PropagateRequestedRegion(it.Get());

        // Check that the requested region lies within the largest possible region
        if (!it.Get()->VerifyRequestedRegion())
        {
          // invalid requested region, throw an exception
          itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
          e.SetLocation(ITK_LOCATION);
          e.SetDataObject(it.Get());
          e.SetDescription("Requested region is (at least partially) outside the largest possible region.");

          throw e;
        }
      }
    }
  }
}

template <class TImage>
void ImageList<TImage>::SetRequestedRegion(const itk::DataObject* source)
{
  for (ConstIterator it = this->Begin(); it != this->End(); ++it)
  {
    it.Get()->SetRequestedRegion(source);
  }
}

template <class TImage>
void ImageList<TImage>::UpdateOutputInformation()
{
  //   otbMsgDebugMacro(<<"ImageList: Call to UpdateOutputInformation()");
  Superclass::UpdateOutputInformation();

  if (this->GetSource())
  {
    this->GetSource()->UpdateOutputInformation();
  }
  for (ConstIterator it = this->Begin(); it != this->End(); ++it)
  {
    if (it.Get()->GetSource())
    {
      it.Get()->GetSource()->UpdateOutputInformation();
    }
  }
}

} // end namespace otb

#endif
