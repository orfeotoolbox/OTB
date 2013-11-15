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
#ifndef __otbReduceSpectralResponseClassifierRAndNIR_txx
#define __otbReduceSpectralResponseClassifierRAndNIR_txx

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
      return m_Functor((*m_InputReduceSpectralResponse)(m_RBandNumber), (*m_InputReduceSpectralResponse)(m_NIRBandNumber));
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
