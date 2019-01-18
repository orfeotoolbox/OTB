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


#ifndef otbMaskMuParserFilter_h
#define otbMaskMuParserFilter_h


#include "itkProgressReporter.h"
#include <string>
#include <vector>

#include "itkArray.h"


#include "otbMaskMuParserFunctor.h"

#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{
/** \class MaskMuParserImageFilter
 * \brief Performs a mathematical operation on the input images
 * according to the formula specified by the user. values different from 0 are set to 1
 *
 * This filter is based on the mathematical parser library muParser.
 * The built in functions and operators list is available at:
 * http://muparser.sourceforge.net/mup_features.html#idDef2
 *
 * OTB additional functions:
 * ndvi(r, niri)
 *
 * OTB additional constants:
 * e - log2e - log10e - ln2 - ln10 - pi - euler
 *
 * an input vector image and a Mu Parser compliant fomula is needed
 * each band of vector image is  name bX, where X is the band index
 * for example b2 correspond to the second band of the input image.
 * Next step is to set the expression according to the variable
 * names. For example, in the default case with three input images the
 * following expression is valid :
 * "b1<140"
 *
 *
 * \sa Parser
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBMathParser
 */

template<class TInputImage, class TOutputImage, class TFunction = Functor::MaskMuParserFunctor<
    typename TInputImage::PixelType> >
class ITK_EXPORT MaskMuParserFilter: public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MaskMuParserFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MaskMuParserFilter, itk::ImageToImageFilter);
;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::PixelType PixelType;
  typedef typename InputImageType::IndexType IndexType;
  typedef typename InputImageType::Pointer InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef typename OutputImageType::Pointer OutputImagePointer;
  typedef TFunction FunctorType;
  typedef typename FunctorType::Pointer FunctorPointer;

  typedef MaskMuParserFilter<InputImageType, OutputImageType, FunctorType> MaskMuParserFilterType;

  /** Set the expression to be parsed */
  void SetExpression(const std::string expression);

  /**Check  expression  */
  bool CheckExpression();

  /** Return the expression to be parsed */
  std::string GetExpression() const;

  std::vector<std::string> GetVar();

  Parser::FunctionMapType GetFunList();

protected:
  MaskMuParserFilter();
  ~MaskMuParserFilter() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void BeforeThreadedGenerateData() override;
  void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, itk::ThreadIdType threadId) override;
  void AfterThreadedGenerateData() override;

private:
  MaskMuParserFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::vector<FunctorPointer> m_VFunctor;
  std::string m_Expression;
  long m_UnderflowCount;
  long m_OverflowCount;
  itk::Array<long> m_ThreadUnderflow;
  itk::Array<long> m_ThreadOverflow;
};

}//end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMaskMuParserFilter.hxx"
#endif

#endif
