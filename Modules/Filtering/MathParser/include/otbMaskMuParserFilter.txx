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
#ifndef __otbMaskMuParserFilter_txx
#define __otbMaskMuParserFilter_txx

#include "otbMaskMuParserFilter.h"
#include <iostream>
#include <string>

#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"

namespace otb
{

// constructor
template<class TInputImage, class TOutputImage, class TFunction>
MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::MaskMuParserFilter()
{
  m_UnderflowCount = 0;
  m_OverflowCount = 0;
  m_ThreadUnderflow.SetSize(1);
  m_ThreadOverflow.SetSize(1);
  m_Expression="";
}

// Destructor
template<class TInputImage, class TOutputImage, class TFunction>
MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::~MaskMuParserFilter()
{

}

template<class TInputImage, class TOutputImage, class TFunction>
void MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Expression: " << m_Expression << std::endl;
}

template<class TInputImage, class TOutputImage, class TFunction>
void MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::SetExpression(const std::string expression)
{
  if (m_Expression != expression) m_Expression = expression;
  this->Modified();
}

template<class TInputImage, class TOutputImage, class TFunction>
std::string MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::GetExpression() const
{
  return m_Expression;
}

template<class TInputImage, class TOutputImage, class TFunction>
std::vector<std::string> MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::GetVar()
{
  std::vector<std::string> varList;
  FunctorPointer tempFunctor = FunctorType::New();
  tempFunctor->SetExpression(m_Expression);
  FunctorType& functor = *tempFunctor;

  try
    {
    functor(this->GetInput()->GetPixel(this->GetInput()->GetBufferedRegion().GetIndex()));
    }
  catch (itk::ExceptionObject& err)
    {
    itkDebugMacro(<< err);
    }

  const std::map<std::string, double*>& varMap = functor.GetVar();
  std::map<std::string, double*>::const_iterator it;
  for (it = varMap.begin(); it != varMap.end(); ++it)
    {
    varList.push_back( it->first );
    }
  return varList;
}


template<class TInputImage, class TOutputImage, class TFunction>
Parser::FunctionMapType MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::GetFunList()
{
  FunctorPointer tempFunctor = FunctorType::New();
  tempFunctor->SetExpression(m_Expression);

  FunctorType& functor = *tempFunctor;

  try
    {
    functor(this->GetInput()->GetPixel(this->GetInput()->GetBufferedRegion().GetIndex()));
    }
  catch (itk::ExceptionObject& err)
    {
    itkDebugMacro(<< err);
    }

  return functor.GetFunList();
}

template<class TInputImage, class TOutputImage, class TFunction>
bool MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::CheckExpression()
{
  FunctorPointer checkFunctor = FunctorType::New();
  checkFunctor->SetExpression(m_Expression);

  FunctorType& functor = *checkFunctor;

  try
    {
    functor(this->GetInput()->GetPixel(this->GetInput()->GetBufferedRegion().GetIndex()));
    }
  catch (itk::ExceptionObject& err)
    {
    itkDebugMacro(<< err);
    return false;
    }
  return true;
}

/**
 * BeforeThreadedGenerateData
 */
template<class TInputImage, class TOutputImage, class TFunction>
void MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::BeforeThreadedGenerateData()
{

  typename std::vector<FunctorPointer>::iterator itFunctor;
  unsigned int nbThreads = this->GetNumberOfThreads();
  unsigned int thread_index;
  std::ostringstream varName;

  // Allocate and initialize the thread temporaries
  m_ThreadUnderflow.SetSize(nbThreads);
  m_ThreadUnderflow.Fill(0);
  m_ThreadOverflow.SetSize(nbThreads);
  m_ThreadOverflow.Fill(0);
  m_VFunctor.resize(nbThreads);

  for (itFunctor = m_VFunctor.begin(); itFunctor < m_VFunctor.end(); itFunctor++)
    *itFunctor = FunctorType::New();

  for (thread_index = 0; thread_index < nbThreads; thread_index++)
    {
    m_VFunctor.at(thread_index)->SetExpression(m_Expression);
    }

}

template<class TInputImage, class TOutputImage, class TFunction>
void MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::ThreadedGenerateData(
                                                                                    const OutputImageRegionType &outputRegionForThread,
                                                                                    itk::ThreadIdType threadId)
{

  InputImageConstPointer inputPtr = this->GetInput();
  OutputImagePointer outputPtr = this->GetOutput(0);

  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIterator<TInputImage> inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  FunctorPointer functorP = m_VFunctor.at(threadId);
  FunctorType& functor = *functorP;
  while (!inputIt.IsAtEnd())
    {
    outputIt.Set(functor(inputIt.Get()));
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel(); // potential exception thrown here
    }
}

template<class TInputImage, class TOutputImage, class TFunction>
void MaskMuParserFilter<TInputImage, TOutputImage, TFunction>::AfterThreadedGenerateData()
{
}

} // end namespace otb

#endif
