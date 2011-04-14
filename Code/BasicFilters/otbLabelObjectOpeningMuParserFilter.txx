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
template        < class TImage, class TFunction>
LabelObjectOpeningMuParserFilter<TImage, TFunction>::LabelObjectOpeningMuParserFilter()
{

  // create the output image for the removed objects
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput( 1, static_cast<TImage*>( this->MakeOutput(1).GetPointer() ) );
}

// Destructor
template        < class TImage, class TFunction>
LabelObjectOpeningMuParserFilter<TImage, TFunction>::~LabelObjectOpeningMuParserFilter()
{

}

template        < class TImage, class TFunction>
void LabelObjectOpeningMuParserFilter<TImage, TFunction>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Expression: "      << m_Expression                  << std::endl;
}


template< class TImage, class TFunction>
void LabelObjectOpeningMuParserFilter<TImage, TFunction>
::SetExpression(const std::string expression)
 {
  if (m_Expression != expression)
    m_Expression = expression;
    m_Functor.SetExpression(m_Expression);
  this->Modified();
 }

template< class TImage, class TFunction>
std::string LabelObjectOpeningMuParserFilter<TImage, TFunction>
::GetExpression() const
 {
  return m_Expression;
 }


template  < class TImage, class TFunction>
void LabelObjectOpeningMuParserFilter<TImage, TFunction>
 ::GenerateData()
    {

    ImageConstPointer  inputPtr = this->GetInput();
    // Allocate the output
    this->AllocateOutputs();

    ImageType * output = this->GetOutput();
    ImageType * output2 = this->GetOutput( 1 );
    assert( this->GetNumberOfOutputs() == 2 );
    assert( output2 != NULL );

    // set the background value for the second output - this is not done in the superclasses
    output2->SetBackgroundValue( output->GetBackgroundValue() );

    const typename ImageType::LabelObjectContainerType & labelObjectContainer = output->GetLabelObjectContainer();

        itk::ProgressReporter progress( this, 0, labelObjectContainer.size() );

        typename ImageType::LabelObjectContainerType::const_iterator it = labelObjectContainer.begin();

        while( it != labelObjectContainer.end() )
          {
          typename LabelObjectType::LabelType label = it->first;
          LabelObjectType * labelObject = it->second;

          if( !m_Functor(*labelObject) )
            {
            // must increment the iterator before removing the object to avoid invalidating the iterator
            it++;
            output2->AddLabelObject( labelObject );
            output->RemoveLabel( label );
            }
          else
            {
            it++;
            }

         progress.CompletedPixel();
      }


      }


} // end namespace otb

#endif
