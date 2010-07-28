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
#ifndef __otbBinaryParserImageFilter_txx
#define __otbBinaryParserImageFilter_txx
#include "otbBinaryParserImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

/** Constructor */
template <class TImage>
BinaryParserImageFilter<TImage>
::BinaryParserImageFilter()
{
  this->SetNumberOfRequiredInputs( 2 );
  this->InPlaceOff();

  m_UnderflowCount = 0;
  m_OverflowCount = 0;
  m_ThreadUnderflow.SetSize(1);
  m_ThreadOverflow.SetSize(1);
}

/** Destructor */
template <class TImage>
BinaryParserImageFilter<TImage>
::~BinaryParserImageFilter()
{
}

template <class TImage>
void BinaryParserImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Expression: "      << m_Expression                  << std::endl;
  os << indent << "Computed values follow:"                            << std::endl;
  os << indent << "UnderflowCount: "  << m_UnderflowCount              << std::endl;
  os << indent << "OverflowCount: "   << m_OverflowCount               << std::endl;
  os << indent << "itk::NumericTraits<PixelType>::NonpositiveMin()  :  " 
               << itk::NumericTraits<PixelType>::NonpositiveMin()      << std::endl;
  os << indent << "itk::NumericTraits<PixelType>::max()  :             " 
	       << itk::NumericTraits<PixelType>::max()                 << std::endl;
}

template <class TImage>
void BinaryParserImageFilter<TImage>
::SetInput1( const ImageType * image1)
{  
  this->SetNthInput(0, const_cast<TImage *>( image1 ));
}

template <typename TImage>
TImage * BinaryParserImageFilter<TImage>
::GetInput1()
{
  return const_cast<TImage *>(this->GetInput(0));
}

template <class TImage>
void BinaryParserImageFilter<TImage>
::SetInput2( const ImageType * image2)
{
  this->SetNthInput(1, const_cast<TImage *>( image2 ));
}

template <typename TImage>
TImage * BinaryParserImageFilter<TImage>
::GetInput2()
{
  return const_cast<TImage *>(this->GetInput(1));
}

template< typename TImage >
void BinaryParserImageFilter<TImage>
::SetExpression(const std::string& expression)
{
  if (m_Expression != expression)
    m_Expression = expression;
  this->Modified();
}

template< typename TImage >
std::string BinaryParserImageFilter<TImage>
::GetExpression() const
{
  return m_Expression;
}

template< typename TImage >
void BinaryParserImageFilter<TImage>
::BeforeThreadedGenerateData()
{
  typename std::vector<ParserType::Pointer>::iterator itParser;
  int nbThreads = this->GetNumberOfThreads();
  unsigned int i;
  
  //  Allocate and initialize the thread temporaries
  m_ThreadUnderflow.SetSize(nbThreads);
  m_ThreadUnderflow.Fill(0);
  m_ThreadOverflow.SetSize(nbThreads);
  m_ThreadOverflow.Fill(0);
  m_VParser.resize(nbThreads);
  m_VImage1.resize(nbThreads);
  m_VImage2.resize(nbThreads);
  for(itParser = m_VParser.begin(); itParser < m_VParser.end(); itParser++)
    *itParser = ParserType::New();

  for(i = 0; i < nbThreads; i++)
    {
    m_VParser.at(i)->SetExpr(m_Expression);
    m_VParser.at(i)->DefineVar("Image1", &m_VImage1.at(i));
    m_VParser.at(i)->DefineVar("Image2", &m_VImage2.at(i));
    }
}

template< typename TImage >
void BinaryParserImageFilter<TImage>
::AfterThreadedGenerateData()
{
  int nbThreads = this->GetNumberOfThreads();

  m_UnderflowCount = 0;
  m_OverflowCount = 0;

  // Accumulate counts for each thread
  for( int i = 0; i < nbThreads; i++)
    {
    m_UnderflowCount += m_ThreadUnderflow[i];
    m_OverflowCount += m_ThreadOverflow[i];
    }

  if((m_UnderflowCount != 0) || (m_OverflowCount!=0))
    otbWarningMacro(<< std::endl 
		    << "The Following Parsed Expression  :  "
		    << this->GetExpression()                                 << std::endl
		    << "Generated " << m_UnderflowCount << " Underflow(s) " 
		    << "And " << m_OverflowCount        << " Overflow(s) "   << std::endl
		    << "The Parsed Expression, The Inputs And The Output "  
		    << "Type May Be Incompatible !");
		    
}

template< typename TImage >
void BinaryParserImageFilter<TImage>
::ThreadedGenerateData(const ImageRegionType& outputRegionForThread,
		       int threadId)
{
  PixelType value;
  itk::ImageRegionConstIterator<TImage>  it1 (this->GetInput1(), outputRegionForThread);
  itk::ImageRegionConstIterator<TImage>  it2 (this->GetInput2(), outputRegionForThread);
  itk::ImageRegionIterator<TImage> ot (this->GetOutput(), outputRegionForThread);
  
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
          
  while (!it1.IsAtEnd())
    {
    m_VImage1.at(threadId) = it1.Get();
    m_VImage2.at(threadId) = it2.Get();

    value = static_cast<double>(m_VParser.at(threadId)->Eval());
     
    if (value < itk::NumericTraits<PixelType>::NonpositiveMin())
      {
      ot.Set(itk::NumericTraits<PixelType>::NonpositiveMin());
      m_ThreadUnderflow[threadId]++;
      }
    else if (value > itk::NumericTraits<PixelType>::max())
      {
      ot.Set(itk::NumericTraits<PixelType>::max());
      m_ThreadOverflow[threadId]++;
      }
    else
      {
      ot.Set(static_cast<PixelType>(value));
      }
    
    ++it1;
    ++it2;
    ++ot;

    progress.CompletedPixel();
    }
}

}// end namespace otb

#endif
