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
#ifndef __otbImageList_txx
#define __otbImageList_txx

#include "otbMacro.h"

namespace otb
{

template <class TImage>
void
ImageList<TImage>
::UpdateOutputData()
{
  Superclass::UpdateOutputData();

  for (ConstIterator it = this->Begin(); it!=this->End();++it)
  {
    if (it.Get()->GetUpdateMTime() < it.Get()->GetPipelineMTime()
        || it.Get()->GetDataReleased()
        || it.Get()->RequestedRegionIsOutsideOfTheBufferedRegion())
    {
      if (it.Get()->GetSource())
      {
        it.Get()->GetSource()->UpdateOutputData(it.Get());
      }
    }
  }
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
  for (ConstIterator it = this->Begin(); it!=this->End();++it)
  {
    if (it.Get()->GetSource())
    {
      it.Get()->GetSource()->UpdateOutputInformation();
    }
  }
}


} // end namespace otb

#endif
