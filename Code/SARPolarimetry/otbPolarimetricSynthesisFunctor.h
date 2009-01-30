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
#ifndef __otbPolarimetricSynthesisFunctor_h
#define __otbPolarimetricSynthesisFunctor_h
#include "vcl_complex.h"

namespace otb
{
namespace Functor
{
/** \class PolarimetricSynthesisFunctor
 *  \brief This functor calculate the polarimetric synthesis
 *  using the electroMagneticField vectors as follow:
 *  $$ \sigma(\psi_{i},\chi_{i},\psi_{r},\chi_{r}) =
                   \vec(E_{r}}\cdot\left[ S \right] \vec(E_{i}}$$
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TInput3, class TInput4, class TOutput>
class PolarimetricSynthesisFunctor
{
public:
  /** Some typedefs. */
  typedef typename     std::complex <double>       ComplexType;
  typedef typename     itk::FixedArray<ComplexType,2>    ComplexArrayType;

  /** Set the ElectroMagneticField Incident */
  void SetEi( ComplexArrayType ei )
  {
    m_Ei = ei;
  }

  /** Set the ElectroMagneticField Reflected */
  void SetEr( ComplexArrayType er )
  {
    m_Er = er;
  }

  /** Constructor */
  PolarimetricSynthesisFunctor()
  {
    m_Ei.Fill(1);
    m_Er.Fill(1);
  };
  /** Destructor */
  virtual ~PolarimetricSynthesisFunctor() {};
  inline TOutput operator()(const TInput1 &Shh, const TInput2 &Shv, const TInput3 &Svh, const TInput4 &Svv)
  {
    ComplexType tmp;
    double scalar;

    tmp = vcl_conj(m_Er[0])*( m_Ei[0]*Shh + m_Ei[1]*Shv ) + vcl_conj(m_Er[1])*( m_Ei[0]*Svh + m_Ei[1]*Svv );

    scalar=(double) ( vcl_pow( std::abs(tmp),2)  );

    return ( static_cast<TOutput>(scalar) );
  }

private :
  /** Electromagnetic Field Incident */
  ComplexArrayType m_Ei;
  /** Electromagnetic Field Reflected */
  ComplexArrayType m_Er;

};

} // namespace Functor
} // namespace otb

#endif

