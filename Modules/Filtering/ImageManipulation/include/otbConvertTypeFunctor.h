/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbConvertTypeFunctor_h
#define otbConvertTypeFunctor_h

#include <limits>
#include <type_traits>
#include <boost/type_traits/is_complex.hpp>
#include <boost/type_traits/is_scalar.hpp>

#include "itkNumericTraits.h"
#include "otbDefaultConvertPixelTraits.h"

namespace otb
{
namespace Functor
{

template <class TInputPixelType, class TOutputPixelType>
class ConvertTypeFunctor
{
public:
  typedef TInputPixelType    InputPixelType;
  typedef TOutputPixelType   OutputPixelType;
  typedef ConvertTypeFunctor Self;

  typedef typename itk::NumericTraits<InputPixelType>::ValueType  InputInternalPixelType;
  typedef typename itk::NumericTraits<OutputPixelType>::ValueType OutputInternalPixelType;

  typedef typename itk::NumericTraits<InputInternalPixelType>::ValueType  InputPixelValueType;
  typedef typename itk::NumericTraits<OutputInternalPixelType>::ValueType OutputPixelValueType;

  static constexpr bool m_cInPix          = boost::is_complex<InputPixelType>::value;
  static constexpr bool m_cOutPix         = boost::is_complex<OutputPixelType>::value;
  static constexpr bool m_cInInternalPix  = boost::is_complex<InputInternalPixelType>::value;
  static constexpr bool m_cOutInternalPix = boost::is_complex<OutputInternalPixelType>::value;

  ConvertTypeFunctor()
  // m_cInPix ( boost::is_complex < InputPixelType > :: value ) ,
  // m_cOutPix ( boost::is_complex < OutputPixelType > :: value ) ,
  // m_cInInternalPix ( boost::is_complex < InputInternalPixelType > :: value ) ,
  // m_cOutInternalPix ( boost::is_complex < OutputInternalPixelType > :: value )
  {
    m_LowestB  = std::numeric_limits<OutputPixelValueType>::lowest();
    m_HighestB = std::numeric_limits<OutputPixelValueType>::max();

    m_LowestBD  = static_cast<double>(m_LowestB);
    m_HighestBD = static_cast<double>(m_HighestB);

    // m_cInPix = boost::is_complex < InputPixelType > :: value ;
    // m_cOutPix = boost::is_complex < OutputPixelType > :: value ;
    // m_cInInternalPix = boost::is_complex < InputInternalPixelType > :: value ;
    // m_cOutInternalPix = boost::is_complex < OutputInternalPixelType > :: value ;
  }

  // template < class InternalPixelType  >
  void SetInputComponents(unsigned int sizeIn)
  {
    m_CompIn = sizeIn;
    if (m_cInPix)
    {
      // needed as ITK thinks that one complex component is actually
      // two components...
      m_CompIn /= 2;
    }
  }

  unsigned int GetOutputSize()
  {
    if (m_cInInternalPix || m_cInPix)
      m_Scal = 2 * m_CompIn;
    else
      m_Scal = m_CompIn;

    OutputPixelType out;
    unsigned int    size = itk::NumericTraits<OutputPixelType>::GetLength(out);
    if (size == 0) // That means it is a variable size container
    {
      if (m_cOutInternalPix)
        m_CompOut = (m_Scal + 1) / 2;
      else
        m_CompOut = m_Scal;
    }
    // It is a fixed size container, m_CompOut should be equal to its size
    else if (m_cOutPix) // one complex is one component
      m_CompOut = 1;
    else // fized size container or scalar
      m_CompOut = size;


    return m_CompOut;
  }

  void SetLowest(OutputPixelValueType& lowest)
  {
    m_LowestB  = lowest;
    m_LowestBD = static_cast<double>(m_LowestB);
  }

  void SetHighest(OutputPixelValueType& highest)
  {
    m_HighestB  = highest;
    m_HighestBD = static_cast<double>(m_HighestB);
  }

  OutputPixelType operator()(InputPixelType const& in) const
  {
    std::vector<double> vPixel;
    for (unsigned int i = 0; i < m_CompIn; i++)
      FillIn<InputPixelType>(i, in, vPixel);
    assert(m_Scal == vPixel.size());
    if ((m_cOutPix || m_cOutInternalPix) && vPixel.size() % 2)
    {
      vPixel.push_back(0); // last component has no imaginary part
    }
    Clamp(vPixel);
    OutputPixelType out;

    int hack = 1;
    if (m_cOutPix)
      hack += 1; // needed in case we have OutputPixelType == complex<t> as
    // itk::NumericTraits::SetLength() will ask a length of 2!
    itk::NumericTraits<OutputPixelType>::SetLength(out, hack * m_CompOut);

    for (unsigned int i = 0; i < m_CompOut; i++)
      FillOut<OutputPixelType>(i, out, vPixel);
    return out;
  }

  ~ConvertTypeFunctor(){};

protected:
  template <class PixelType, std::enable_if_t<std::is_arithmetic<PixelType>::value, int> = 0>
  void FillIn(unsigned int i, InputPixelType const& pix, std::vector<double>& vPix) const
  {
    vPix.push_back(DefaultConvertPixelTraits<InputPixelType>::GetNthComponent(i, pix));
  }

  template <class PixelType, std::enable_if_t<boost::is_complex<PixelType>::value, int> = 0>
  void FillIn(unsigned int i, InputPixelType const& pix, std::vector<double>& vPix) const
  {
    PixelType comp = DefaultConvertPixelTraits<InputPixelType>::GetNthComponent(i, pix);
    vPix.push_back(static_cast<double>(real(comp)));
    vPix.push_back(static_cast<double>(imag(comp)));
  }

  template <class PixelType, std::enable_if_t<!(boost::is_complex<PixelType>::value || std::is_arithmetic<PixelType>::value), int> = 0>
  void FillIn(unsigned int i, InputPixelType const& pix, std::vector<double>& vPix) const
  {
    FillIn<InputInternalPixelType>(i, pix, vPix);
  }

  void Clamp(std::vector<double>& vPixel) const
  {
    for (double& comp : vPixel)
    {
      if (comp >= m_HighestBD)
        comp = m_HighestBD;
      else if (comp <= m_LowestBD)
        comp = m_LowestBD;
    }
  }

  template <class PixelType, std::enable_if_t<std::is_arithmetic<PixelType>::value, int> = 0>
  void FillOut(unsigned int i, OutputPixelType& pix, std::vector<double>& vPix) const
  {
    DefaultConvertPixelTraits<OutputPixelType>::SetNthComponent(i, pix, vPix[i]);
  }

  template <class PixelType, std::enable_if_t<boost::is_complex<PixelType>::value, int> = 0>
  void FillOut(unsigned int i, OutputPixelType& pix, std::vector<double>& vPix) const
  {
    DefaultConvertPixelTraits<OutputPixelType>::SetNthComponent(i, pix, PixelType(vPix[2 * i], vPix[2 * i + 1]));
  }

  template <class PixelType, std::enable_if_t<!(boost::is_complex<PixelType>::value || std::is_arithmetic<PixelType>::value), int> = 0>
  void FillOut(unsigned int i, OutputPixelType& pix, std::vector<double>& vPix) const
  {
    FillOut<OutputInternalPixelType>(i, pix, vPix);
  }

private:
  ConvertTypeFunctor(const Self&) = delete;
  void operator=(const Self&) = delete;

  double               m_LowestBD, m_HighestBD;
  OutputPixelValueType m_LowestB, m_HighestB;
  unsigned int         m_CompIn, m_CompOut, m_Scal;
  // const bool m_cInPix , m_cOutPix , m_cInInternalPix , m_cOutInternalPix ;
};

} // end namespace Functor

} // end namespace otb

#endif
