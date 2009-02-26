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

#ifndef __otbWaveletForwardTransform__txx
#define __otbWaveletForwardTransform__txx

#include "otbWaveletForwardTransform.h"

namespace otb {

template < class TInputImage, class TOutputImage, class TFilter >
WaveletForwardTransform< TInputImage, TOutputImage, TFilter >
::WaveletForwardTransform ()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  m_NumberOfDecompositions = 1;
  m_FilterList = FilterListType::New();

  m_UseSubSampleImage = false;
}

template < class TInputImage, class TOutputImage, class TFilter >
void
WaveletForwardTransform< TInputImage, TOutputImage, TFilter >
::SetNumberOfDecompositions ( unsigned int num )
{
  this->m_NumberOfDecompositions = num;

  m_FilterList->Reserve( this->GetNumberOfDecompositions() );
  for ( unsigned int i = 0; i < this->GetNumberOfDecompositions(); i++ )
    m_FilterList->PushBack( FilterType::New() );

  unsigned int nbOutputsPerFilter = this->GetFilter(0)->GetNumberOfOutputs();
  unsigned int nbOutputs = 1 + this->GetNumberOfDecompositions() * ( nbOutputsPerFilter-1 );

  this->SetNumberOfOutputs( nbOutputs );

  unsigned int idx = 0;
  GetFilter(0)->GraftNthOutput( 0, this->GetOutput(idx++) );
  
  for ( unsigned int i = 0; i < this->GetNumberOfDecompositions(); i++ )
  {
    for ( unsigned int j = 1; j < this->GetFilter(i)->GetNumberOfOutputs(); j++ )
    {
      this->SetNthOutput( idx, OutputImageType::New() );
      GetFilter(i)->GraftNthOutput( j, this->GetOutput( idx++ ) );
    }
  }

  this->Modified();
}

template < class TInputImage, class TOutputImage, class TFilter >
void
WaveletForwardTransform< TInputImage, TOutputImage, TFilter >
::GenerateData ()
{
  this->AllocateOutputs();

  unsigned int nbDecomp = this->GetNumberOfDecompositions()-1;
  GetFilter(nbDecomp)->SetInput( this->GetInput() );

  unsigned int idx;
  unsigned int outputIdxForGraft = this->GetNumberOfOutputs() - 1;
  unsigned int outputIdxToGraft = this->GetNumberOfOutputs() - 1;

  /*
  std::cerr << "outputIdxForGraft = " << outputIdxForGraft << "\n";
  std::cerr << "outputIdxToGraft = " << outputIdxToGraft << "\n";
  */

  unsigned int subSampleFactor = 0;
  while ( nbDecomp > 0 )
  { 
    /*
    std::cerr << "Doing Decomp " << nbDecomp << "\n";
    std::cerr << "outputIdxForGraft = " << outputIdxForGraft << "\n";
    std::cerr << "outputIdxToGraft = " << outputIdxToGraft << "\n";
    std::cerr << "Filter output = " << this->GetFilter(nbDecomp)->GetNumberOfOutputs() << "\n";
    */

    idx = this->GetFilter(nbDecomp)->GetNumberOfOutputs();
    while ( idx-- > 1 )
      GetFilter(nbDecomp)->GraftNthOutput( idx, this->GetOutput( outputIdxForGraft-- ) );

    if ( m_UseSubSampleImage == false )
      GetFilter(nbDecomp)->SetUpSampleFactor( ++subSampleFactor );

    GetFilter( nbDecomp )->Update();

    idx = this->GetFilter(nbDecomp)->GetNumberOfOutputs();
    while ( idx-- > 1 )
      this->GetOutput( outputIdxToGraft-- )->Graft( GetFilter(nbDecomp)->GetOutput( idx ) );

    GetFilter( nbDecomp-1 )->SetInput( GetFilter( nbDecomp )->GetOutput(0) );

    --nbDecomp;
  }

  /*
  std::cerr << "Doing last Decomp (nbDecomp=" << nbDecomp << ")\n";
  std::cerr << "outputIdxForGraft = " << outputIdxForGraft << "\n";
  std::cerr << "outputIdxToGraft = " << outputIdxToGraft << "\n";
  std::cerr << "Filter output = " << this->GetFilter(nbDecomp)->GetNumberOfOutputs() << "\n";
  */

  idx = this->GetFilter(nbDecomp)->GetNumberOfOutputs();
  while ( idx-- > 0 )
    GetFilter(nbDecomp)->GraftNthOutput( idx, this->GetOutput( outputIdxForGraft-- ) );

  if ( m_UseSubSampleImage == false )
      GetFilter(nbDecomp)->SetUpSampleFactor( ++subSampleFactor );

  GetFilter( nbDecomp )->Update();

  idx = this->GetFilter(nbDecomp)->GetNumberOfOutputs();
  while ( idx-- > 0 )
    this->GetOutput( outputIdxToGraft-- )->Graft( GetFilter(nbDecomp)->GetOutput( idx ) );
}

} // end of namespace otb

#endif



