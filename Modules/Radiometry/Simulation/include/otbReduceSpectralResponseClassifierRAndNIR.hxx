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

#ifndef otbReduceSpectralResponseClassifierRAndNIR_hxx
#define otbReduceSpectralResponseClassifierRAndNIR_hxx

#include "otbReduceSpectralResponseClassifierRAndNIR.h"

#include <algorithm>

namespace otb
{

  template <class TReduceSpectralResponse , class TFunction>
      ReduceSpectralResponseClassifierRAndNIR <TReduceSpectralResponse , TFunction>
  ::ReduceSpectralResponseClassifierRAndNIR()
  {
    //m_Response = VectorType::New();
  }

  template <class TReduceSpectralResponse , class TFunction>
      bool
          ReduceSpectralResponseClassifierRAndNIR<TReduceSpectralResponse , TFunction>
  ::Clear()
  {
    return ( m_InputReduceSpectralResponse->Clear() );
  }


  template <class TReduceSpectralResponse , class TFunction>
      inline typename ReduceSpectralResponseClassifierRAndNIR<TReduceSpectralResponse , TFunction>
  ::ValuePrecisionType
      ReduceSpectralResponseClassifierRAndNIR<TReduceSpectralResponse , TFunction>
  ::operator()()
  {
    itk::VariableLengthVector<ValuePrecisionType> sr(2);
    sr[0]=(*m_InputReduceSpectralResponse)(m_RBandNumber);
    sr[1]=(*m_InputReduceSpectralResponse)(m_NIRBandNumber);
    m_Functor.SetBandIndex(CommonBandNames::RED,1);
    m_Functor.SetBandIndex(CommonBandNames::NIR,2);

    return m_Functor(sr);
  }


  template <class TReduceSpectralResponse , class TFunction>
      void
          ReduceSpectralResponseClassifierRAndNIR<TReduceSpectralResponse , TFunction>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }
} // end namespace otb

#endif
