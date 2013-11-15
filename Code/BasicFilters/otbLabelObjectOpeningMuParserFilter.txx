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
#ifndef __otbLabelObjectOpeningMuParserFilter_txx
#define __otbLabelObjectOpeningMuParserFilter_txx

#include "otbLabelObjectOpeningMuParserFilter.h"
#include <iostream>
#include <string>

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"

namespace otb
{

// constructor
template<class TImage, class TFunction>
LabelObjectOpeningMuParserFilter<TImage, TFunction>::LabelObjectOpeningMuParserFilter()
{
  // create the output image for the removed objects
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(1, static_cast<TImage*> (this->MakeOutput(1).GetPointer()));
}

// Destructor
template<class TImage, class TFunction>
LabelObjectOpeningMuParserFilter<TImage, TFunction>::~LabelObjectOpeningMuParserFilter()
{
}

template<class TImage, class TFunction>
void LabelObjectOpeningMuParserFilter<TImage, TFunction>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Expression: " << m_Expression << std::endl;
}

template<class TImage, class TFunction>
void LabelObjectOpeningMuParserFilter<TImage, TFunction>::SetExpression(const std::string expression)
{
  if (m_Expression != expression) m_Expression = expression;
  m_Functor.SetExpression(m_Expression);
  this->Modified();
}

template<class TImage, class TFunction>
std::string LabelObjectOpeningMuParserFilter<TImage, TFunction>::GetExpression() const
{
  return m_Expression;
}

template<class TImage, class TFunction>
const std::map<std::string, double*>& LabelObjectOpeningMuParserFilter<TImage, TFunction>::GetVar() const
{
  return this->m_Functor.GetVar();
}

template<class TImage, class TFunction>
Parser::FunctionMapType LabelObjectOpeningMuParserFilter<TImage, TFunction>::GetFunList() const
{
  return this->m_Functor.GetFunList();
}

template<class TImage, class TFunction>
void LabelObjectOpeningMuParserFilter<TImage, TFunction>::DisplayVar() const
{
  const std::map<std::string, double*>& variables = this->m_Functor.GetVar();

  // Get the number of variables
  std::map<std::string, double*>::const_iterator item = variables.begin();

  // Query the variables
  for (; item != variables.end(); ++item)
    {
    std::cout << "Name: " << item->first << " Address: [0x" << item->second << "]\n";
    }

}

template<class TImage, class TFunction>
bool LabelObjectOpeningMuParserFilter<TImage, TFunction>::CheckExpression()
{
  return this->m_Functor.CheckExpression();
}

template<class TImage, class TFunction>
void LabelObjectOpeningMuParserFilter<TImage, TFunction>::SetAttributes(std::vector<std::string> shapeAttributes,
                                                                        std::vector<std::string> statAttributes,
                                                                        int nbOfBands)
{
  this->m_Functor.SetAttributes(shapeAttributes, statAttributes, nbOfBands);
}

/** Get the reduced attribute set */
template<class TImage, class TFunction>
void LabelObjectOpeningMuParserFilter<TImage, TFunction>::AllocateOutputs()
{
  // if told to run in place and the types support it,
  if (this->GetInPlace() && this->CanRunInPlace())
    {
    // Graft this first input to the output.  Later, we'll need to
    // remove the input's hold on the bulk data.
    //
    ImagePointer inputAsOutput = dynamic_cast<TImage *> (const_cast<TImage *> (this->GetInput()));

    if (inputAsOutput)
      {

      this->GraftOutput(inputAsOutput);
      this->GetOutput()->SetLargestPossibleRegion(this->GetOutput()->GetLargestPossibleRegion());
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
      this->GetOutput()->SetBufferedRegion(this->GetOutput()->GetBufferedRegion());

      }

    // If there are more than one outputs, allocate the remaining outputs
    for (unsigned int i = 1; i < this->GetNumberOfOutputs(); ++i)
      {
      ImagePointer outputPtr;

      outputPtr = this->GetOutput(i);
      outputPtr->SetBufferedRegion(outputPtr->GetRequestedRegion());
      outputPtr->Allocate();
      }
    }
  else
    {
    Superclass::AllocateOutputs();
    // copy the content of the input image to the output image (will be done by ImageSource AllocateOutputs Method)
    // would never occur : inputasoutput condition is always true, since output and input type is TImage for
    // LabelObjectOpeningMuParserFilter class
    }
}

template<class TImage, class TFunction>
void LabelObjectOpeningMuParserFilter<TImage, TFunction>::GenerateInputRequestedRegion()
{
  itk::ImageToImageFilter<TImage, TImage>::GenerateInputRequestedRegion();
}

template<class TImage, class TFunction>
void LabelObjectOpeningMuParserFilter<TImage, TFunction>::GenerateData()
{

  ImageConstPointer inputPtr = this->GetInput();
  // Allocate the output
  this->AllocateOutputs();

  ImageType * output = this->GetOutput();
  ImageType * output2 = this->GetOutput(1);
  assert( this->GetNumberOfOutputs() == 2 );
  assert( output2 != NULL );

  // set the background value for the second output - this is not done in the superclasses
  output2->SetBackgroundValue(output->GetBackgroundValue());

  itk::ProgressReporter progress(this, 0, output->GetNumberOfLabelObjects());
  typename ImageType::Iterator it(output);

  while (!it.IsAtEnd())
    {
    typename LabelObjectType::LabelType label = it.GetLabel();
    LabelObjectType * labelObject = it.GetLabelObject();

    if (!m_Functor(*labelObject))
      {
      // must increment the iterator before removing the object to avoid invalidating the iterator
      ++it;
      output2->AddLabelObject(labelObject);
      output->RemoveLabel(label);
      }
    else
      {
      ++it;
      }

    progress.CompletedPixel();
    }

}

} // end namespace otb

#endif
