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


#ifndef otbLabelObjectOpeningMuParserFilter_h
#define otbLabelObjectOpeningMuParserFilter_h

#include "itkProgressReporter.h"
#include <string>
#include <vector>

#include "itkArray.h"


#include "itkInPlaceLabelMapFilter.h"
#include "itkLabelObjectAccessors.h"
#include "itkProgressReporter.h"
#include "otbOBIAMuParserFunctor.h"

namespace otb
{

/** \class LabelObjectOpeningMuParserImageFilter
 * \brief Label Object opening using shape and radiometric attributes. Acception/rejection
 * criteria is user defined via MuParser function.
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
 *
 * \sa Parser
 *
 *
 * \ingroup OTBCCOBIA
 */
template<class TImage, class TFunction = Functor::OBIAMuParserFunctor<typename TImage::LabelObjectType> >
class ITK_EXPORT LabelObjectOpeningMuParserFilter: public itk::InPlaceLabelMapFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelObjectOpeningMuParserFilter Self;
  typedef itk::InPlaceLabelMapFilter<TImage> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer ImagePointer;
  typedef typename ImageType::ConstPointer ImageConstPointer;
  typedef typename ImageType::PixelType PixelType;
  typedef typename ImageType::IndexType IndexType;
  typedef typename ImageType::RegionType InputImageRegionType;
  typedef typename ImageType::LabelObjectType LabelObjectType;
  typedef typename LabelObjectType::ConstPointer LabelObjectConstPointer;
  typedef TFunction FunctorType;
  typedef typename LabelObjectType::AttributeType AttributeType;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LabelObjectOpeningMuParserFilter, InPlaceLabelMapFilter);

  /** Set the expression to be parsed */
  void SetExpression(const std::string expression);

  /** Return the expression to be parsed */
  std::string GetExpression() const;

  /** Check expression to be parsed **/
  bool CheckExpression();

  /** Manual variables setting **/
  void SetAttributes(std::vector<std::string> shapeAttributes, std::vector<std::string> statAttributes, int nbOfBands);

  /** Display varname and address **/
  void DisplayVar() const;

  /** return list of Mu Parser variables and address**/
  const std::map<std::string, double*>& GetVar() const;

  /** return list of Mu Parser functions**/
  Parser::FunctionMapType GetFunList() const;

  void AllocateOutputs() override;

  void GenerateInputRequestedRegion() override;

  void EnlargeOutputRequestedRegion(itk::DataObject *) override {}

  void GenerateData() override;

protected:
  LabelObjectOpeningMuParserFilter();
  ~LabelObjectOpeningMuParserFilter() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LabelObjectOpeningMuParserFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  FunctorType m_Functor;
  std::string m_Expression;

};

}//end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelObjectOpeningMuParserFilter.hxx"
#endif

#endif
