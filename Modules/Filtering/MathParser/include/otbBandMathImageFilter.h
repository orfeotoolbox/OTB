/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbBandMathImageFilter_h
#define otbBandMathImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkArray.h"

#include "otbParser.h"

namespace otb
{
/** \class BandMathImageFilter
 * \brief Performs a mathematical operation on the input images
 * according to the formula specified by the user.
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
 * In order to use this filter, at least one input image is to be
 * set. An associated variable name can be specified or not by using
 * the corresponding SetNthInput method. For the nth input image, if
 * no associated variable name has been spefified, a default variable
 * name is given by concatenating the letter "b" (for band) and the
 * corresponding input index.
 * Next step is to set the expression according to the variable
 * names. For example, in the default case with three input images the
 * following expression is valid :
 * "ndvi(b1, b2)*b3"
 *
 * As an additional functionality, the filter also granted access to
 * indexes information under special virtual bands named idxX, idxY
 * for the images indexes and idxPhyX, idxPhyY for the physical
 * indexes.
 * It allows the user to perform, for example a spatial processing
 * aiming to suppress a determined area :
 * "if(sqrt((idxPhyX-105.3)*(idxPhyX-105.3)+
 *          (idxPhyY-207.1)*(idxPhyY-207.1))>100, b1, 0)"
 * This expression replace the physical zone around the point of
 * physical index (105.3; 207.1) by a black area
 * This functionality assumes that all the band involved have the same
 * spacing and origin.
 *
 *
 * \sa Parser
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBMathParser
 */

template< class TImage >
class ITK_EXPORT BandMathImageFilter
  : public itk::InPlaceImageFilter< TImage >
{
public:
  /** Standard class typedefs. */
  typedef BandMathImageFilter< TImage >                 Self;
  typedef itk::InPlaceImageFilter< TImage >             Superclass;
  typedef itk::SmartPointer< Self >                     Pointer;
  typedef itk::SmartPointer< const Self >               ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BandMathImageFilter, InPlaceImageFilter);

  /** Some convenient typedefs. */
  typedef TImage                                  ImageType;
  typedef typename ImageType::ConstPointer        ImagePointer;
  typedef typename ImageType::RegionType          ImageRegionType;
  typedef typename ImageType::PixelType           PixelType;
  typedef typename ImageType::IndexType           IndexType;
  typedef typename ImageType::PointType           OrigineType;
  typedef typename ImageType::SpacingType         SpacingType;
  typedef Parser                                  ParserType;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Set the nth filter input with or without a specified associated variable name */
  using Superclass::SetNthInput;
  void SetNthInput( DataObjectPointerArraySizeType idx, const ImageType * image);
  void SetNthInput( DataObjectPointerArraySizeType idx, const ImageType * image, const std::string& varName);

  /** Change the nth filter input associated variable name */
  void SetNthInputName(DataObjectPointerArraySizeType idx, const std::string& expression);

  /** Set the expression to be parsed */
  void SetExpression(const std::string& expression);

  /** Return the expression to be parsed */
  std::string GetExpression() const;

  /** Return the nth filter input associated variable name */
  std::string GetNthInputName(DataObjectPointerArraySizeType idx) const;

  /** Return a pointer on the nth filter input */
  ImageType * GetNthInput(DataObjectPointerArraySizeType idx);

protected :
  BandMathImageFilter();
  ~BandMathImageFilter() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void BeforeThreadedGenerateData() override;
  void ThreadedGenerateData(const ImageRegionType& outputRegionForThread, itk::ThreadIdType threadId ) override;
  void AfterThreadedGenerateData() override;

private :
  BandMathImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::string                           m_Expression;
  std::vector<ParserType::Pointer>      m_VParser;
  std::vector< std::vector<double> >    m_AImage;
  std::vector< std::string >            m_VVarName;
  unsigned int                          m_NbVar;

  SpacingType                           m_Spacing;
  OrigineType                           m_Origin;

  long                                  m_UnderflowCount;
  long                                  m_OverflowCount;
  itk::Array<long>                      m_ThreadUnderflow;
  itk::Array<long>                      m_ThreadOverflow;
};

}//end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBandMathImageFilter.hxx"
#endif

#endif
