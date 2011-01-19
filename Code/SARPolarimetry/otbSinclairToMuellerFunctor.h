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
#ifndef __otbSinclairToMuellerFunctor_h
#define __otbSinclairToMuellerFunctor_h

#include "vcl_complex.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToMuellerFunctor
 *  \brief Construct Mueller matrix with Sinclair matrix information.
 *  Elements of the Mueller matrix are extract from Antennas for radar and communications
 *  Harold Mott p 503
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToMuellerFunctor
{
public:
  /** Some typedefs. */
  typedef typename std::complex <double>           ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;
  typedef double                                   RealType;

  inline TOutput operator ()(const TInput1& Shh, const TInput2& Shv,
                             const TInput3& Svh, const TInput4& Svv)
  {
    TOutput result;

    result.SetSize(m_NumberOfComponentsPerPixel);

    ComplexType Txx = static_cast<ComplexType>(-Shh);
    ComplexType Txy = static_cast<ComplexType>(-Shv);
    ComplexType Tyx = static_cast<ComplexType>(Svh);
    ComplexType Tyy = static_cast<ComplexType>(Svv);

    ComplexType conjTxx = vcl_conj(static_cast<ComplexType>(Shh));
    ComplexType conjTxy = vcl_conj(static_cast<ComplexType>(Shv));
    ComplexType conjTyx = vcl_conj(static_cast<ComplexType>(Svh));
    ComplexType conjTyy = vcl_conj(static_cast<ComplexType>(Svv));

    result[0]  = static_cast<OutputValueType>( 0.5 * (Txx*conjTxx + Txy*conjTxy + Tyx*conjTyx + Tyy*conjTyy).real()  );
    result[1]  = static_cast<OutputValueType>( 0.5 * (Txx*conjTxx - Txy*conjTxy + Tyx*conjTyx - Tyy*conjTyy).real()  );
    result[2]  = static_cast<OutputValueType>( (Txx*conjTxy + Tyx*conjTyy).real()  );
    result[3]  = static_cast<OutputValueType>( (Txx*conjTxy + Tyx*conjTyy).imag()  );
    result[4]  = static_cast<OutputValueType>( 0.5 * (Txx*conjTxx + Txy*conjTxy - Tyx*conjTyx - Tyy*conjTyy).real()  );
    result[5]  = static_cast<OutputValueType>( 0.5 * (Txx*conjTxx - Txy*conjTxy - Tyx*conjTyx + Tyy*conjTyy).real()  );
    result[6]  = static_cast<OutputValueType>( (Txx*conjTxy - Tyx*conjTyy).real()  );
    result[7]  = static_cast<OutputValueType>( (Txx*conjTxy - Tyx*conjTyy).imag()  );
    result[8]  = static_cast<OutputValueType>( (Txx*conjTyx + Txy*conjTyy).real()  );
    result[9]  = static_cast<OutputValueType>( (Txx*conjTyx - Txy*conjTyy).real()  );
    result[10] = static_cast<OutputValueType>( (Txx*conjTyy + Txy*conjTyx).real()  );
    result[11] = static_cast<OutputValueType>( (Txx*conjTyy - Txy*conjTyx).imag()  );
    result[12] = static_cast<OutputValueType>( (conjTxx*Tyx + conjTxy*Tyy).imag()  );
    result[13] = static_cast<OutputValueType>( (conjTxx*Tyx - conjTxy*Tyy).imag()  );
    result[14] = static_cast<OutputValueType>( (conjTxx*Tyy + conjTxy*Tyx).imag()  );
    result[15] = static_cast<OutputValueType>( (Txx*conjTyy - Txy*conjTyx).real()  );

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return m_NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToMuellerFunctor() : m_NumberOfComponentsPerPixel(16) {}

  /** Destructor */
  virtual ~SinclairToMuellerFunctor() {}

protected:


private:
    unsigned int m_NumberOfComponentsPerPixel;
};

} // namespace Functor
} // namespace otb

#endif
