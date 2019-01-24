/*
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

#ifndef otbParserConditionDataNodeFeatureFunction_h
#define otbParserConditionDataNodeFeatureFunction_h

#include "otbDataNodeImageFunction.h"
#include "otbPolyLineImageConstIterator.h"
#include "otbMaskMuParserFunctor.h"
#include <string>

namespace otb
{
/** \class ParserConditionDataNodeFeatureFunction
  * \brief Compute a ParserCondition based feature alongside a
  * datanode.
  *
  * This function evaluate a condition alongside a datanode.
  * The descriptor is the percentage of pixels iterated for which
  * the condition is true.
  *
  * \ingroup Functions
  * \sa DataNodeImageFunction
  * \sa SpectralAngleDataNodeFeatureFunction
 *
 * \ingroup OTBMathParser
  */

template <
class TImage,
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT ParserConditionDataNodeFeatureFunction :
    public DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef ParserConditionDataNodeFeatureFunction                         Self;
  typedef DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
                                                              Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ParserConditionDataNodeFeatureFunction, DataNodeImageFunction);

  /** Some typedefs. */
  typedef typename Superclass::DataNodeType           DataNodeType;
  typedef typename DataNodeType::LineType             LineType;

  typedef TImage                                      InputImageType;
  typedef typename InputImageType::ConstPointer       InputImageConstPointer;
  typedef typename InputImageType::PixelType          PixelType;
  typedef typename InputImageType::InternalPixelType  ScalarRealType;
  typedef typename InputImageType::IndexType          IndexType;
  typedef typename InputImageType::IndexValueType     IndexValueType;

  typedef TCoordRep                                   CoordRepType;

  typedef TPrecision                                  PrecisionType;

  typedef PolyLineImageConstIterator<InputImageType, LineType>
                                                      ImageLineIteratorType;

  typedef Functor::MaskMuParserFunctor<PixelType>      ParserConditionFunctorType;
  typedef typename ParserConditionFunctorType::Pointer ParserConditionFunctorPointer;

  typedef std::vector<PrecisionType>                  OutputType;

  OutputType Evaluate( const DataNodeType& node ) const override;

  std::string GetExpression() const
  {
    return m_ParserConditionFunctor->GetExpression();
  }

  void SetExpression(std::string expression)
  {
    m_ParserConditionFunctor->SetExpression(expression);
  }

  void SetSpectralAngleReferencePixel(const PixelType& pixel)
  {
    m_ParserConditionFunctor->SetSpectralAngleReferencePixel(pixel);
  }

protected:
  ParserConditionDataNodeFeatureFunction();
  ~ParserConditionDataNodeFeatureFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ParserConditionDataNodeFeatureFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** ParserCondition Functor */
  ParserConditionFunctorPointer  m_ParserConditionFunctor;
};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbParserConditionDataNodeFeatureFunction.hxx"
#endif

#endif
