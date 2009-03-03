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

#ifndef __otbAmplitudePhaseToRGBFunctor_h
#define __otbAmplitudePhaseToRGBFunctor_h

#include "itkRGBPixel.h"
#include "otbScalarToRainbowRGBPixelFunctor.h"
#include "otbMath.h"

namespace otb
{

  namespace Functor
  {
    template< class TInput1, class TInput2=TInput1, class TOutput=TInput1>
        class ITK_EXPORT AmplitudePhaseToRGBFunctor
    {
      public:
        typedef TOutput RGBPixelType;
        typedef typename RGBPixelType::ValueType  RGBComponentType;
        typedef HSVToRGBFunctor<RGBPixelType>  HSVToRGBFunctorType;
        typedef TInput1 ScalarType;

        AmplitudePhaseToRGBFunctor()
        {
          m_Minimum = 0;
          m_Maximum = itk::NumericTraits<ScalarType>::max();
        };
        ~AmplitudePhaseToRGBFunctor(){};

        void SetMaximum(ScalarType max)
        {
          this->m_Maximum = max;
        }

        void SetMinimum(ScalarType min)
        {
          this->m_Minimum = min;
        }

        inline TOutput operator()( const TInput1 & amplitude, const TInput2 & phase) const
        {
          double hinc, sinc, vinc;
          hinc=0.6/(2*M_PI);
          sinc=0.0;
          vinc=0.0;

          double hue, sat, val;

          hue = 0.6 - (phase+M_PI)*hinc;
          sat = 0.99;
          val = itk::NumericTraits<RGBComponentType>::max()/(m_Maximum-m_Minimum) * (amplitude-m_Minimum);

          return m_HSVToRGBFunctor(hue, sat, val);

        }
      private:
        ScalarType m_Maximum;
        ScalarType m_Minimum;
        HSVToRGBFunctorType m_HSVToRGBFunctor;
    };
  }

}
#endif
