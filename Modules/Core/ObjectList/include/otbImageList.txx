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
#ifndef otbImageList_txx
#define otbImageList_txx

#include "otbImageList.h"
#include "otbMacro.h"

namespace otb
{

template <class TImage>
void
ImageList<TImage>
::UpdateOutputData()
{
  Superclass::UpdateOutputData();
  for (ConstIterator it = this->Begin(); it != this->End(); ++it)
    {
    if (it.Get()->GetUpdateMTime() < it.Get()->GetPipelineMTime()
        || it.Get()->GetDataReleased()
        || it.Get()->RequestedRegionIsOutsideOfTheBufferedRegion())
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

        it.Get()->GetSource()->UpdateOutputData(it.Get());
        }
      }
    }
}

template <class TImage>
void
ImageList<TImage>
::PropagateRequestedRegion() throw (itk::InvalidRequestedRegionError)
  {
  Superclass::PropagateRequestedRegion();
  }

template <class TImage>
void
ImageList<TImage>
::UpdateOutputInformation()
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
