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

#ifndef __otbMaskMuParserFunctor_h
#define __otbMaskMuParserFunctor_h

#include "otbParser.h"
#include "otbMacro.h"



#include "otbBinarySpectralAngleFunctor.h"

#include "muParserCallback.h"

namespace otb
{
/** \class MaskMuParserFunctor
 * \brief functor used to create binary mask for input of connected component segmentation module.
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
 */

/** \class MaskMuParserFunctor
 *  \brief This functor use MuParser to evaluate and process mathematical expression
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

  virtual ~MaskMuParserFunctor();

private:

  MaskMuParserFunctor(const Self &); //purposely not implemented
  void operator =(const Self &); //purposely not implemented

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
#include "otbMaskMuParserFunctor.txx"
#endif

#endif
