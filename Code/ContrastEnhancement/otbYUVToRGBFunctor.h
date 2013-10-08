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
#ifndef __otbYUVToRGBFunctor_h
#define __otbYUVToRGBFunctor_h

#include "itkIndex.h"

namespace otb
{
namespace Functor {

template<class TInput, class TOutput>
class YUVToRGBFunctor {

public:

  typedef itk::Index<3> IndexType;
  typedef typename TInput::ValueType InputType;
  typedef typename TOutput::ValueType OutputType;

  YUVToRGBFunctor()
    :m_rChannel(0),
     m_gChannel(1),
     m_bChannel(2)
  {

  }

  ~YUVToRGBFunctor() { }

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

  inline void SetMaxPixelValue(OutputType maxValue)
  {
    m_MaxPixelValue = maxValue;
  }

  inline TOutput operator ()(const TInput& input)
  {
    TOutput output;
    output.SetSize(3);

    double Y =  input[0];
    double Cb = input[1] - m_Delta;
    double Cr = input[2] - m_Delta;

    double R = Y + 1.402 * Cr;
    double G = Y - 0.344 * Cb - 0.714 *Cr;
    double B = Y + 1.772 * Cb;

    output[0] = ClipToMinMax(R);
    output[1] = ClipToMinMax(G);
    output[2] = ClipToMinMax(B);

    return output;
  }

  OutputType ClipToMinMax(double value)
  {
    if(value > m_MaxPixelValue)
      return m_MaxPixelValue;
    else if(value < 0)
      return 0;
    else
      return (OutputType)value;
  }

private:
  int m_rChannel;
  int m_gChannel;
  int m_bChannel;
  unsigned int m_Delta;
  OutputType m_MaxPixelValue;

};

} //end namespace Functor
} //end namespace otb

#endif
