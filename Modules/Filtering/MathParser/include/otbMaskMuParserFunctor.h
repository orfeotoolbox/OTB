/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbMaskMuParserFunctor_h
#define otbMaskMuParserFunctor_h

#include "otbParser.h"
#include "otbMacro.h"
#include "otbBinarySpectralAngleFunctor.h"
#include <string>

namespace otb
{
/** \class MaskMuParserFunctor
 * \brief Functor used to create binary mask for input of connected component segmentation module.
 *
 * This functor is based on the mathematical parser library muParser.
 * The built in functions and operators list is available at:
 * http://muparser.sourceforge.net/mup_features.html#idDef2
 *
 * OTB additional functions:
 * ndvi(r, niri)
 *
 * OTB additional constants:
 * e - log2e - log10e - ln2 - ln10 - pi - euler
 *
 * \sa Parser
 *
 *
 * \ingroup OTBMathParser
 */

/** \class MaskMuParserFunctor
 *  \brief This functor use MuParser to evaluate and process mathematical expression
 *
 * \ingroup OTBMathParser
 */
namespace Functor
{

template<class TInputPixel>
class ITK_EXPORT MaskMuParserFunctor: public itk::LightObject
{
public:
  typedef MaskMuParserFunctor           Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MaskMuParserFunctor, itk::LightObject);

  typedef TInputPixel PixelType;
  typedef Parser ParserType;

  typedef BinarySpectralAngleFunctor<PixelType, PixelType, double> SpectralAngleFunctorType;

  bool operator()(const PixelType &p);

  const std::map<std::string, Parser::ValueType*>& GetVar() const;

  Parser::FunctionMapType GetFunList() const;

  void SetExpression(const std::string& expression);

  /** Return the expression to be parsed */
  std::string GetExpression() const;

  void SetNumberOfBands(unsigned int NbOfBands);

  /** Set the reference pixel used to compute the "spectralangle" parser variable */
  void SetSpectralAngleReferencePixel(const PixelType& refPixel);

  /** Check the expression */
  bool CheckExpression();

protected:
  MaskMuParserFunctor();

  ~MaskMuParserFunctor() override;

private:

  MaskMuParserFunctor(const Self &) = delete;
  void operator =(const Self &) = delete;

  std::string m_Expression;
  ParserType::Pointer m_Parser;
  std::vector<double> m_AImage;
  //std::vector<std::string > m_VarName;
  unsigned int m_NbOfBands;
  //unsigned int m_NbVar;
  double m_ParserResult;

  //user defined variables
  double m_Intensity;
  double m_SpectralAngle;

  PixelType m_SpectralAngleReferencePixel;
  SpectralAngleFunctorType m_SpectralAngleFunctor;

};

} // end of Functor namespace
}//end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMaskMuParserFunctor.hxx"
#endif

#endif
