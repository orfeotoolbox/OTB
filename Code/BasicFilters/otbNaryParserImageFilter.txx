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
#ifndef __otbNaryParserImageFilter_txx
#define __otbNaryParserImageFilter_txx
#include "otbNaryParserImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"
#include "itkMacro.h"

#include <iostream>
#include <string>

namespace otb
{

/** Constructor */
template <class TImage>
NaryParserImageFilter<TImage>
::NaryParserImageFilter()
{
  //This number will be incremented each time an image
  //is added over the one minimumrequired
  this->SetNumberOfRequiredInputs( 1 );
  this->InPlaceOff();

  m_UnderflowCount = 0;
  m_OverflowCount = 0;
  m_ThreadUnderflow.SetSize(1);
  m_ThreadOverflow.SetSize(1);
}

/** Destructor */
template <class TImage>
NaryParserImageFilter<TImage>
::~NaryParserImageFilter()
{
}

template <class TImage>
void NaryParserImageFilter<TImage>
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
void NaryParserImageFilter<TImage>
::SetNthInput(unsigned int idx, const ImageType * image)
{  
  this->SetInput(idx, const_cast<TImage *>( image ));
  unsigned int nbInput = this->GetNumberOfInputs();
  m_VVarName.resize(nbInput);
  std::ostringstream varName; 
  varName << "b" << nbInput;
  m_VVarName[idx] = varName.str();  
}

template <class TImage>
void NaryParserImageFilter<TImage>
::SetNthInput(unsigned int idx, const ImageType * image, const std::string& varName)
{
  this->SetInput(idx, const_cast<TImage *>( image ));
  m_VVarName.resize(this->GetNumberOfInputs());
  m_VVarName[idx] = varName;
}

template <class TImage>
void NaryParserImageFilter<TImage>
::SetNthInputName(unsigned int idx, const std::string& varName)
{
  m_VVarName[idx] = varName;
}


template <typename TImage>
TImage * NaryParserImageFilter<TImage>
::GetNthInput(unsigned int idx)
{
  return const_cast<TImage *>(this->GetInput(idx));
}

template< typename TImage >
void NaryParserImageFilter<TImage>
::SetExpression(const std::string& expression)
{
  if (m_Expression != expression)
    m_Expression = expression;
  this->Modified();
}

template< typename TImage >
std::string NaryParserImageFilter<TImage>
::GetExpression() const
{
  return m_Expression;
}

template< typename TImage >
std::string NaryParserImageFilter<TImage>
::GetNthInputName(unsigned int idx) const
{
  return m_VVarName.at(idx);
}

template< typename TImage >
void NaryParserImageFilter<TImage>
::BeforeThreadedGenerateData()
{
  typename std::vector<ParserType::Pointer>::iterator        itParser;
  typename std::vector< std::vector<PixelType> >::iterator   itVImage;
  unsigned int nbThreads = this->GetNumberOfThreads();
  unsigned int nbInputImages = this->GetNumberOfInputs();
  unsigned int i, j;
  
  //Allocate and initialize the thread temporaries
  m_ThreadUnderflow.SetSize(nbThreads);
  m_ThreadUnderflow.Fill(0);
  m_ThreadOverflow.SetSize(nbThreads);
  m_ThreadOverflow.Fill(0);
  m_VParser.resize(nbThreads);
  m_AImage.resize(nbThreads);
  
  for(itParser = m_VParser.begin(); itParser < m_VParser.end(); itParser++)
    { 
    *itParser = ParserType::New();
    }

  for(i = 0; i < nbThreads; i++)
    {
    m_AImage.at(i).resize(nbInputImages);
    m_VParser.at(i)->SetExpr(m_Expression);
    for(j=0; j < nbInputImages; j++)
      {
      m_VParser.at(i)->DefineVar(m_VVarName.at(j), &(m_AImage.at(i).at(j)));
      }
    }
}

template< typename TImage >
void NaryParserImageFilter<TImage>
::AfterThreadedGenerateData()
{
  unsigned int nbThreads = this->GetNumberOfThreads();
  unsigned int i;
  
  m_UnderflowCount = 0;
  m_OverflowCount = 0;

  // Accumulate counts for each thread
  for(i = 0; i < nbThreads; i++)
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
void NaryParserImageFilter<TImage>
::ThreadedGenerateData(const ImageRegionType& outputRegionForThread,
           int threadId)
{
  double value; 
  unsigned int j;
  unsigned int nbInputImages = this->GetNumberOfInputs();
  
  typedef itk::ImageRegionConstIterator<TImage> ImageRegionConstIteratorType;
  
  std::vector< ImageRegionConstIteratorType > Vit;
  Vit.resize(nbInputImages);
  for(j=0; j < nbInputImages; j++)
    {
    Vit[j] = ImageRegionConstIteratorType (this->GetNthInput(j), outputRegionForThread);
    }
  
  itk::ImageRegionIterator<TImage> ot (this->GetOutput(), outputRegionForThread);
  
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  while(!Vit.at(0).IsAtEnd())
    {
    for(j=0; j < nbInputImages; j++)
      {
      m_AImage.at(threadId).at(j) = static_cast<double>(Vit.at(j).Get());
      }

    try
      {
      value = m_VParser.at(threadId)->Eval();
      }
    catch(itk::ExceptionObject& err)
      {
      itkExceptionMacro(<< err);
      }
    
    // Case value is equal to -inf or inferior to the minimum value
    // allowed by the pixelType cast
    if (value < double(itk::NumericTraits<PixelType>::NonpositiveMin()))
      {
      ot.Set(itk::NumericTraits<PixelType>::NonpositiveMin());
      m_ThreadUnderflow[threadId]++;
      }
    // Case value is equal to inf or superior to the maximum value
    // allowed by the pixelType cast
    else if (value > double(itk::NumericTraits<PixelType>::max()))
      {
      ot.Set(itk::NumericTraits<PixelType>::max());
      m_ThreadOverflow[threadId]++;
      }
    else
      {
      ot.Set(static_cast<PixelType>(value));
      }

    for(j=0; j < nbInputImages; j++)
      {
      ++(Vit.at(j));
      }

    ++ot;

    progress.CompletedPixel();
    }
}

}// end namespace otb

#endif
