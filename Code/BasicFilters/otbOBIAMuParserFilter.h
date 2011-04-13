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

#ifndef __otbOBIAMuParserFilter_h
#define __otbOBIAMuParserFilter_h


#include "itkProgressReporter.h"
#include <iostream>
#include <string>
#include <vector>

#include "itkArray.h"

#include "otbParser.h"
#include "otbMacro.h"
#include "itkMacro.h"
#include "otbMaskMuParserFunctor.h"

#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{
/** \class OBIAMuParserFilter
 * \brief Performs a mathematical operation on the input label object contains in labelmap.
 * Criteria is defined using shape and radiometrics objects characteristics.
 *  *
 * This filter is based on the mathematical parser library muParser.
 * The built in functions and operators list is available at:
 * http://muparser.sourceforge.net/mup_features.html#idDef2
 *
 *
 * OTB additional constants:
 * e - log2e - log10e - ln2 - ln10 - pi - euler
 *
 *
 *
 * \sa Parser
 *
 */

template< class TInputImage,class TOutputImage,class TFunction=Functor::OBIAMuParserFunctor<
	    typename TInputImage::PixelType > >
class ITK_EXPORT OBIAMuParserFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef OBIAMuParserFilter                Self;
  typedef itk::ImageToImageFilter< TInputImage, TOutputImage>             Superclass;
  typedef itk::SmartPointer< Self >                     Pointer;
  typedef itk::SmartPointer< const Self >               ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OBIAMuParserFilter, itk::ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef typename    InputImageType::RegionType   InputImageRegionType;
  typedef typename    InputImageType::PixelType    PixelType;
  typedef typename    InputImageType::Pointer      InputImagePointer;
  typedef typename    InputImageType::ConstPointer InputImageConstPointer;
  typedef TOutputImage                             OutputImageType;
  typedef typename    OutputImageType::RegionType  OutputImageRegionType;

  typedef typename    OutputImageType::Pointer     OutputImagePointer;

  typedef TFunction						                     FunctorType;

  typedef OBIAMuParserFilter<InputImageType,OutputImageType,FunctorType>  MaskMuParserFilterType;

  /** Set the expression to be parsed */
  void SetExpression(const std::string expression);

  /** Return the expression to be parsed */
  std::string GetExpression() const;

protected :
  OBIAMuParserFilter();
  virtual ~OBIAMuParserFilter();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

   void BeforeThreadedGenerateData();
   void ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,int threadId);
   void AfterThreadedGenerateData();


private :
  OBIAMuParserFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::vector<FunctorPointer>           m_VFunctor;
  std::string                           m_Expression;
  long                                  m_UnderflowCount;
  long                                  m_OverflowCount;
  itk::Array<long>                      m_ThreadUnderflow;
  itk::Array<long>                      m_ThreadOverflow;
};

}//end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOBIAMuParserFilter.txx"
#endif

#endif
