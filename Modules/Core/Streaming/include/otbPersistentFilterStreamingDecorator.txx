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
#ifndef __otbPersistentFilterStreamingDecorator_txx
#define __otbPersistentFilterStreamingDecorator_txx

#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{
/**
 * Constructor
 */
template <class TFilter>
PersistentFilterStreamingDecorator<TFilter>
::PersistentFilterStreamingDecorator()
{
  m_Filter = FilterType::New();
  m_Streamer = StreamerType::New();
}

template <class TFilter>
void
PersistentFilterStreamingDecorator<TFilter>
::GenerateData(void)
{
  // Reset the filter before the generation.
  this->GetFilter()->Reset();

  /*
  for (unsigned int idx = 0; idx < this->GetFilter()->GetNumberOfOutputs(); ++idx)
    {
    this->GetStreamer()->SetNthInput(idx, this->GetFilter()->GetOutput(idx));
    }
  */

  this->GetStreamer()->SetInput(this->GetFilter()->GetOutput());
  this->GetStreamer()->Update();

  // Synthetize data after the streaming of the whole image.
  this->GetFilter()->Synthetize();
}

template <class TFilter>
void
PersistentFilterStreamingDecorator<TFilter>
::Update(void)
{
  this->GenerateData();
}

/**
 * PrintSelf Method
 */
template <class TFilter>
void
PersistentFilterStreamingDecorator<TFilter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
