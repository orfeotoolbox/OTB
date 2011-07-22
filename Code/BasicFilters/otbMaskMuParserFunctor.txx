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
#ifndef __otbMaskMuParserFunctor_txx
#define __otbMaskMuParserFunctor_txx

#include "otbMaskMuParserFunctor.h"

namespace otb
{
namespace Functor
{
// constructor
template<class TInputPixel>
MaskMuParserFunctor<TInputPixel>::MaskMuParserFunctor()
: m_Parser(ParserType::New()),
  m_NbOfBands(0),
  m_ParserResult(0),
  m_Intensity(0),
  m_SpectralAngle(0)
{
}

// Destructor
template<class TInputPixel>
MaskMuParserFunctor<TInputPixel>::~MaskMuParserFunctor()
{
}

template<class TInputPixel>
bool MaskMuParserFunctor<TInputPixel>::operator()(const PixelType &p)
{
  double value;

  if (p.Size() != m_NbOfBands)
    {
    this->SetNumberOfBands(p.GetSize());
    }

  // we fill the buffer
  for (unsigned int i = 0; i < m_NbOfBands; ++i)
    {
    m_AImage[i] = static_cast<double> (p[i]);
    }

  // user defined variables
  m_Intensity = 0.0;
  for (unsigned int i = 0; i < m_NbOfBands; ++i)
    {
    m_Intensity += p[i];
    }
  m_Intensity = m_Intensity / (static_cast<double> (m_NbOfBands));

  m_SpectralAngle = m_SpectralAngleFunctor(p, m_SpectralAngleReferencePixel);

  value = m_Parser->Eval();

  return static_cast<bool> (value);

}

template<class TInputPixel>
const std::map<std::string, Parser::ValueType*>&
MaskMuParserFunctor<TInputPixel>::GetVar() const
{
  return this->m_Parser->GetVar();
}

template<class TInputPixel>
Parser::FunctionMapType
MaskMuParserFunctor<TInputPixel>::GetFunList() const
{
  return this->m_Parser->GetFunList();
}


template<class TInputPixel>
void
MaskMuParserFunctor<TInputPixel>::SetExpression(const std::string& expression)
{
  m_Expression = expression;
  m_Parser->SetExpr(m_Expression);
}

/** Return the expression to be parsed */
template<class TInputPixel>
std::string
MaskMuParserFunctor<TInputPixel>::GetExpression() const
{
  return m_Expression;
}

template<class TInputPixel>
void
MaskMuParserFunctor<TInputPixel>::SetNumberOfBands(unsigned int NbOfBands)
{
  m_NbOfBands = NbOfBands;
  std::ostringstream varName;

  m_AImage.resize(NbOfBands, 0.0);

  for (unsigned int i = 0; i < NbOfBands; ++i)
    {
    varName << "b" << i + 1;
    m_Parser->DefineVar(varName.str(), &(m_AImage[i]));
    varName.str("");
    }

  // customized data
  m_Parser->DefineVar("intensity", &m_Intensity);
  m_Parser->DefineVar("spectralAngle", &m_SpectralAngle);
}

/** Set the reference pixel used to compute the "spectralangle" parser variable */
template<class TInputPixel>
void
MaskMuParserFunctor<TInputPixel>::SetSpectralAngleReferencePixel(const PixelType& refPixel)
{
  m_SpectralAngleReferencePixel = refPixel;
}

/** Check the expression */
template<class TInputPixel>
bool
MaskMuParserFunctor<TInputPixel>::CheckExpression()
{
  return m_Parser->CheckExpr();
}

} // end namespace Functor
} // end namespace otb

#endif
