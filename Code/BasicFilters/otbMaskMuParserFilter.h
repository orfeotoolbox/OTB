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

#ifndef __otbMaskMuParserFilter_h
#define __otbMaskMuParserFilter_h


#include "itkProgressReporter.h"
#include <iostream>
#include <string>
#include <vector>

#include "itkArray.h"

#include "otbParser.h"
#include "otbMacro.h"

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
  itkNewMacro(Self)
;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MaskMuParserFilter, itk::ImageToImageFilter)
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
  virtual ~MaskMuParserFilter();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void BeforeThreadedGenerateData();
  void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, itk::ThreadIdType threadId);
  void AfterThreadedGenerateData();

private:
  MaskMuParserFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::vector<FunctorPointer> m_VFunctor;
  std::string m_Expression;
  long m_UnderflowCount;
  long m_OverflowCount;
  itk::Array<long> m_ThreadUnderflow;
  itk::Array<long> m_ThreadOverflow;
};

}//end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMaskMuParserFilter.txx"
#endif

#endif
