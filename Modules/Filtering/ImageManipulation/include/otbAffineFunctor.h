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

#ifndef otbAffineFunctor_h
#define otbAffineFunctor_h

namespace otb
{
namespace Functor
{
/** \class AffineFunctor
 *  \brief This functor performs a linear transform of its input.
 *
 *  \f[ output = a*input + b \f]
 *
 * Coefficient a is a scalar, its type is templated (default is double).
 * Coefficient b has the same type as output type. Standard casting is applied
 * between input and output type.
 *
 * TInput and TOutput type are supposed to be scalar types.
 *
 * \ingroup OTBImageManipulation
 */
template<class TInput, class TOutput, class TScale=double>
class AffineFunctor
{
public:
  /// Constructor
  AffineFunctor()
    :m_A(0.), m_B(0.) {}

  /// Constructor
  virtual ~AffineFunctor() {}

  /// Accessors
  void SetA(TScale value)
  {
    m_A = value;
  }
  TScale GetA()
  {
    return m_A;
  }
  void SetB(TOutput value)
  {
    m_B = value;
  }
  TOutput GetB()
  {
    return m_B;
  }

  bool operator !=(const AffineFunctor& other) const
  {
    return ((m_A != other.GetA()) || (m_B != other.GetB()));
  }

  bool operator==(const AffineFunctor& other) const
  {
    return ((m_A == other.GetA()) && (m_B == other.GetB()));
  }

  // main computation method
  inline TOutput operator()(const TInput & x) const
  {
    return (m_B + static_cast<TOutput>(m_A*x));
  }

private:
  TScale  m_A;
  TOutput m_B;
};
} // End namespace Functor
} // End namespace otb

#endif
