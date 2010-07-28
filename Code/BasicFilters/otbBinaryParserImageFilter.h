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

#ifndef __otbBinaryParserImageFilter_h
#define __otbBinaryParserImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkArray.h"

#include "otbParser.h"

namespace otb
{

template< class TImage >
class ITK_EXPORT BinaryParserImageFilter 
  : public itk::InPlaceImageFilter< TImage >
{
public:
  /** Standard class typedefs. */
  typedef BinaryParserImageFilter< TImage >                                             Self;
  typedef itk::InPlaceImageFilter< TImage >                                             Superclass;
  typedef itk::SmartPointer< Self >                                                     Pointer;
  typedef itk::SmartPointer< const Self >                                               ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryParserImageFilter, InPlaceImageFilter);

  /** Some convenient typedefs. */
  typedef TImage                                  ImageType;
  typedef typename ImageType::ConstPointer        ImagePointer;
  typedef typename ImageType::RegionType          ImageRegionType; 
  typedef typename ImageType::PixelType           PixelType;
  typedef Parser                                  ParserType;
  
  void SetInput1( const ImageType * image1);
  ImageType * GetInput1();
  void SetInput2( const ImageType * image2);
  ImageType * GetInput2();
  void SetExpression(const std::string& expression);

  std::string GetExpression() const;

protected :
  BinaryParserImageFilter();
  virtual ~BinaryParserImageFilter();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
 
  void BeforeThreadedGenerateData();
  void ThreadedGenerateData(const ImageRegionType& outputRegionForThread, int threadId );
  void AfterThreadedGenerateData();

private :
  BinaryParserImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::string                           m_Expression; 
  std::vector<ParserType::Pointer>      m_VParser;
  std::vector<PixelType>                m_VImage1;
  std::vector<PixelType>                m_VImage2;

  long                                  m_UnderflowCount;
  long                                  m_OverflowCount;
  itk::Array<long>                      m_ThreadUnderflow;
  itk::Array<long>                      m_ThreadOverflow;
};

}//end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbBinaryParserImageFilter.txx"
#endif

#endif
