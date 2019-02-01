/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbAttributesMapOpeningLabelMapFilter_hxx
#define otbAttributesMapOpeningLabelMapFilter_hxx

#include "otbAttributesMapOpeningLabelMapFilter.h"

namespace otb
{

template<class TImage >
AttributesMapOpeningLabelMapFilter<TImage>
::AttributesMapOpeningLabelMapFilter()
{
}

template<class TImage >
AttributesMapOpeningLabelMapFilter<TImage>
::~AttributesMapOpeningLabelMapFilter()
{
}


/** Set the name of the attribute to perform opening on */
template<class TImage >
void
AttributesMapOpeningLabelMapFilter<TImage>
::SetAttributeName(const char * name)
{
 this->GetAccessor().SetAttributeName(name);
}

/** Get the name of the attribute to perform opening on */
template<class TImage >
const char *
AttributesMapOpeningLabelMapFilter<TImage>
::GetAttributeName()
{
 return this->GetAccessor().GetAttributeName();
}


template<class TImage >
void
AttributesMapOpeningLabelMapFilter<TImage>
::SetAccessor(const AttributeAccessorType& accessor)
{
 m_Accessor = accessor;
 this->Modified();
}

template<class TImage >
typename AttributesMapOpeningLabelMapFilter<TImage>::AttributeAccessorType &
AttributesMapOpeningLabelMapFilter<TImage>
::GetAccessor()
{
 return m_Accessor;
}


template<class TImage >
void
AttributesMapOpeningLabelMapFilter<TImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  typename Superclass::ImageType * output = this->GetOutput();
  typename Superclass::ImageType * output2 = this->GetOutput( 1 );

  // set the background value for the second output - this is not done in the superclasses
  output2->SetBackgroundValue( output->GetBackgroundValue() );

  //AttributeAccessorType accessor;

  itk::ProgressReporter progress( this, 0, output->GetNumberOfLabelObjects() );

  typename Superclass::ImageType::Iterator it( output );
  while( ! it.IsAtEnd() )
    {
    typename Superclass::LabelObjectType::LabelType label = it.GetLabel();
    typename Superclass::LabelObjectType * labelObject = it.GetLabelObject();

    if( ( !this->GetReverseOrdering() && m_Accessor( labelObject ) < this->GetLambda() )
      || ( this->GetReverseOrdering() && m_Accessor( labelObject ) > this->GetLambda() ) )
      {
      // must increment the iterator before removing the object to avoid invalidating the iterator
      ++it;
      output2->AddLabelObject( labelObject );
      output->RemoveLabel( label );
      }
    else
      {
      ++it;
      }

    progress.CompletedPixel();
    }
}


template<class TImage >
void
AttributesMapOpeningLabelMapFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
