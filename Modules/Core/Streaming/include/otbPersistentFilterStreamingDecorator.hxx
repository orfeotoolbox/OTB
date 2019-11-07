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

#ifndef otbPersistentFilterStreamingDecorator_hxx
#define otbPersistentFilterStreamingDecorator_hxx

#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{
/**
 * Constructor
 */
template <class TFilter>
PersistentFilterStreamingDecorator<TFilter>::PersistentFilterStreamingDecorator()
{
  m_Filter   = FilterType::New();
  m_Streamer = StreamerType::New();
}

template <class TFilter>
void PersistentFilterStreamingDecorator<TFilter>::GenerateData(void)
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
void PersistentFilterStreamingDecorator<TFilter>::Update(void)
{
  this->GenerateData();
}

/**
 * PrintSelf Method
 */
template <class TFilter>
void PersistentFilterStreamingDecorator<TFilter>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
