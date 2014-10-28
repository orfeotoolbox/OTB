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

#ifndef __otbLabelObjectOpeningMuParserFilter_h
#define __otbLabelObjectOpeningMuParserFilter_h

#include "itkProgressReporter.h"
#include <iostream>
#include <string>
#include <vector>

#include "itkArray.h"

#include "otbMacro.h"


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

  virtual void AllocateOutputs();

  void GenerateInputRequestedRegion();

  void EnlargeOutputRequestedRegion(itk::DataObject *) {}

  void GenerateData();

protected:
  LabelObjectOpeningMuParserFilter();
  virtual ~LabelObjectOpeningMuParserFilter();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LabelObjectOpeningMuParserFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  FunctorType m_Functor;
  std::string m_Expression;

};

}//end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelObjectOpeningMuParserFilter.txx"
#endif

#endif
