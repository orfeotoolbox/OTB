/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbGammaFunctor_h
#define __otbGammaFunctor_h

#include "vnl/vnl_math.h"

namespace otb
{
namespace Functor
{
template<class TInput, class TOutput>
class GammaFunctor {

public:

  GammaFunctor() : m_Gamma(1.0)
  {}

  inline void SetGamma(double gamma)
  {
    if(gamma == 0)
      {
      itkGenericExceptionMacro( << "Error: Division by zero" );
      }
    else if(gamma < 0)
      {
      otbMsgDevMacro( << "Gamma Encoding" );
      }
    else
      {
      otbMsgDevMacro( << "Gamma Decoding" );
      }

    m_GammaInverse = 1.0 / gamma;
  }

  inline TOutput operator ()(const TInput& input) const
  {
    typedef typename TInput::ValueType InputType;
    typedef typename TOutput::ValueType OutputType;

    TOutput output;
    output.SetSize(input.Size());

   for (unsigned int i = 0; i < input.Size(); ++i)
     {
     output[i] = vcl_pow(static_cast<double>(input[i]) , m_GammaInverse);
     }
    return output;
  }

private:
  double m_GammaInverse;

};

} //end namespace Functor
} //end namespace otb

#endif
