/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletPacketForwardTransform__txx
#define __otbWaveletPacketForwardTransform__txx

#include "otbWaveletPacketForwardTransform.h"

namespace otb {

template < class TInputImage, class TOutputImage, class TFilter, class TCost >
WaveletPacketForwardTransform< TInputImage, TOutputImage, TFilter, TCost >
::WaveletPacketForwardTransform ()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfOutputs(1);

  m_FilterList = FilterListType::New();
  m_Cost = CostType::New();
}

template < class TInputImage, class TOutputImage, class TFilter, class TCost >
void
WaveletPacketForwardTransform< TInputImage, TOutputImage, TFilter, TCost >
::GenerateData ()
{
  //this->AllocateOutputs();
  
  /*
   * Start with a decomposition
   */
  this->GetFilterList()->PushBack( FilterType::New() );
  FilterType * filter = this->GetFilterList()->GetNthElement( 0 );
  filter->SetInput( this->GetInput() );
  filter->Update();
  for ( unsigned int idx = 0; idx < filter->GetNumberOfOutputs(); idx++ )
    this->GetOutput()->PushBack( filter->GetOutput( idx ) );

  /**
   * Loop decompositions
   */
  for ( OutputImageIterator outputIt = this->GetOutput()->Begin();
      outputIt != this->GetOutput()->End();
      ++outputIt ) 
  {
    PerformDecomposition( 1, outputIt );
  }
}

template < class TInputImage, class TOutputImage, class TFilter, class TCost >
void
WaveletPacketForwardTransform< TInputImage, TOutputImage, TFilter, TCost >
::PerformDecomposition ( unsigned int depth, OutputImageIterator & outputIt )
{
  if ( this->GetCost()->Evaluate( depth, outputIt.Get() ) )
  {
    this->GetFilterList()->PushBack( FilterType::New() );
    FilterType * filter = this->GetFilterList()->GetNthElement( this->GetFilterList()->Size()-1 );
    filter->SetInput( outputIt.Get() );
    filter->Update();

    depth++;
    outputIt.Set( filter->GetOutput(0) );
    PerformDecomposition( depth, outputIt );

    for ( unsigned int idx = 1; idx < filter->GetNumberOfOutputs(); idx++ )
    {
      ++outputIt;
      outputIt = this->GetOutput()->Insert( outputIt, filter->GetOutput( idx ) );
      PerformDecomposition( depth, outputIt );
    }
  }
}

} // end of namespace otb

#endif

