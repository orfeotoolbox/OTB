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
#ifndef __otbRGBToYUVFunctor_h
#define __otbRGBToYUVFunctor_h

#include "itkIndex.h"

namespace otb
{
namespace Functor {

template<class TInput, class TOutput>
class RGBToYUVFunctor {

public:

  typedef itk::Index<3> IndexType;

  RGBToYUVFunctor()
    :m_rChannel(0),
     m_gChannel(1),
     m_bChannel(2)
  {
  }

  ~RGBToYUVFunctor() { }

  void SetRGBIndex(IndexType index)
  {
    m_rChannel = index[0];
    m_gChannel = index[1];
    m_bChannel = index[2];

  }

  inline void SetDelta(unsigned int delta)
  {
    m_Delta = delta;
  }

  inline TOutput operator ()(const TInput& input) const
  {
    TOutput output;
    output.SetSize(3);

    typedef typename TInput::ValueType InputType;
    typedef typename TOutput::ValueType OutputType;

    InputType R = input[m_rChannel];
    InputType G = input[m_gChannel];
    InputType B = input[m_bChannel];

    double Y  =  0.299 * R + 0.587 * G + 0.114  * B;
    double Cb = -0.169 * R - 0.331 * G + 0.499  * B + m_Delta;
    double Cr =  0.499 * R - 0.418 * G - 0.0813 * B + m_Delta;

    output[0] = static_cast<OutputType>(Y); // Y
    output[1] = static_cast<OutputType>(Cb); // Cb
    output[2] = static_cast<OutputType>(Cr); // Cr

    return output;
  }

private:
  int m_rChannel;
  int m_gChannel;
  int m_bChannel;
  unsigned int m_Delta;
};

} //end namespace Functor
} //end namespace otb

#endif
