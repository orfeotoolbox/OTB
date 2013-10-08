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

  GammaFunctor() {  }

  ~GammaFunctor() {  }

  inline void SetGamma(double gamma)
  {
    if(gamma == 0)
      {
      std::cerr << "Error: Division by zero " << __FILE__ << ":" << __LINE__ << std::endl;
      }
    else if(gamma < 0)
      {
      std::cerr << "Gamma Encoding"  << std::endl;
      }
    else
      {
      std::cerr << "Gamma Decoding"  << std::endl;
      }

    m_Gamma = 1.0 / gamma;
  }

  inline TOutput operator ()(const TInput& input) const
  {
    typedef typename TInput::ValueType InputType;
    typedef typename TOutput::ValueType OutputType;

    TOutput output;
    output.SetSize(input.Size());

   for (unsigned int i = 0; i < input.Size(); ++i)
     {
     //output[i] = vcl_pow(m_Range *  (input[i] / m_Range) , m_Gamma);
     output[i] = vcl_pow(input[i] , m_Gamma);
     }
    return output;
  }

private:
  double m_Gamma;

};

} //end namespace Functor
} //end namespace otb

#endif
