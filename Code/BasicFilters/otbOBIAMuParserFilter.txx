/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbOBIAMuParserFilter_txx
#define __otbOBIAMuParserFilter_txx

#include "otbOBIAMuParserFilter.h"
#include <iostream>
#include <string>

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"

namespace otb
{

// constructor
template        < class TInputImage, class TOutputImage, class TFunction>
OBIAMuParserFilter<TInputImage, TOutputImage, TFunction>::OBIAMuParserFilter()
{
  m_UnderflowCount = 0;
  m_OverflowCount = 0;
  m_ThreadUnderflow.SetSize(1);
  m_ThreadOverflow.SetSize(1);
}

// Destructor
template        < class TInputImage, class TOutputImage, class TFunction>
OBIAMuParserFilter<TInputImage, TOutputImage, TFunction>::~OBIAMuParserFilter()
{

}

template        < class TInputImage, class TOutputImage, class TFunction>
void OBIAMuParserFilter<TInputImage, TOutputImage, TFunction>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Expression: "      << m_Expression                  << std::endl;
}


template< class TInputImage, class TOutputImage, class TFunction>
void OBIAMuParserFilter<TInputImage, TOutputImage, TFunction>
::SetExpression(const std::string expression)
 {
  if (m_Expression != expression)
    m_Expression = expression;
  this->Modified();
 }

template< class TInputImage, class TOutputImage, class TFunction>
std::string OBIAMuParserFilter<TInputImage, TOutputImage, TFunction>
::GetExpression() const
 {
  return m_Expression;
 }


/**
 * BeforeThreadedGenerateData
 */
template        < class TInputImage, class TOutputImage, class TFunction>
void OBIAMuParserFilter<TInputImage, TOutputImage, TFunction>
::BeforeThreadedGenerateData()
 {

  typename std::vector<FunctorPointer>::iterator        itFunctor;
  unsigned int nbThreads = this->GetNumberOfThreads();
  unsigned int nbInputImages = this->GetNumberOfInputs();
  unsigned int nbOfBands =this->GetInput()->GetNumberOfComponentsPerPixel();
  unsigned int thread_index;
  std::ostringstream varName;

  // Allocate and initialize the thread temporaries
  m_ThreadUnderflow.SetSize(nbThreads);
  m_ThreadUnderflow.Fill(0);
  m_ThreadOverflow.SetSize(nbThreads);
  m_ThreadOverflow.Fill(0);
  m_VFunctor.resize(nbThreads);


  for(itFunctor = m_VFunctor.begin(); itFunctor < m_VFunctor.end(); itFunctor++)
    {
    *itFunctor = FunctorType::New();
    }

  for(thread_index = 0; thread_index < nbThreads; thread_index++)
    {

    m_VFunctor.at(thread_index)->SetExpression(m_Expression);
    m_VFunctor.at(thread_index)->SetNumberOfBands(nbOfBands);
    // we associate image band with its name im1b1 for band 1
    for(unsigned int i=0; i<nbOfBands; i++)
      {
      varName << "b" << i+1; // +1 var[0]=im1b1
      m_VFunctor.at(thread_index)->SetVarName(i, varName.str());
      varName.str("");
      }

    if(!m_VFunctor.at(thread_index)->CheckExpression())
      {
      std::cerr<<"parser can't evaluate formula "<<this->GetExpression()<<"in thread "<<thread_index<<std::endl;
      }
    }

 }

template        < class TInputImage, class TOutputImage, class TFunction>
void OBIAMuParserFilter<TInputImage, TOutputImage, TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, int threadId)
 {


  InputImageConstPointer  inputPtr = this->GetInput();
  OutputImagePointer outputPtr = this->GetOutput(0);

  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIterator<TInputImage>  inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  FunctorPointer functorP = m_VFunctor.at(threadId);
  FunctorType&   functor = *functorP;
  while( !inputIt.IsAtEnd() )
    {
    outputIt.Set( functor( inputIt.Get() ) );
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
 }

template        < class TInputImage, class TOutputImage, class TFunction>
void OBIAMuParserFilter<TInputImage, TOutputImage, TFunction>
::AfterThreadedGenerateData()
 {
 }

} // end namespace otb

#endif
