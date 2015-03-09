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
#ifndef __otbAssymmetricFusionOfLineDetectorImageFilter_txx
#define __otbAssymmetricFusionOfLineDetectorImageFilter_txx

#include "otbAssymmetricFusionOfLineDetectorImageFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
AssymmetricFusionOfLineDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>
::AssymmetricFusionOfLineDetectorImageFilter()
{
  this->m_Radius.Fill(1);
  this->m_LengthLine = 1;
  this->m_WidthLine = 0;
  this->m_NumberOfDirections = 8;

  m_LineRatio         = LineRatioType::New();
  m_LineCorrelation   = LineCorrelationType::New();
  m_AssSymSum         = AssSymSumType::New();
}

template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
void
AssymmetricFusionOfLineDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>
::GenerateData()
{
  m_LineRatio->SetInput( this->GetInput() );
  m_LineRatio->SetLengthLine( this->m_LengthLine );
  m_LineRatio->SetWidthLine( this->m_WidthLine );
  m_LineRatio->SetNumberOfDirections( this->m_NumberOfDirections );

  m_LineCorrelation->SetInput( this->GetInput() );
  m_LineCorrelation->SetLengthLine( this->m_LengthLine );
  m_LineCorrelation->SetWidthLine( this->m_WidthLine );
  m_LineCorrelation->SetNumberOfDirections( this->m_NumberOfDirections );

  m_AssSymSum->SetInput1( m_LineRatio->GetOutput() );
  m_AssSymSum->SetInput2( m_LineCorrelation->GetOutput() );

  m_AssSymSum->GraftOutput(this->GetOutput() );
  m_AssSymSum->Update();
  this->GraftOutput(m_AssSymSum->GetOutput() );
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
void
AssymmetricFusionOfLineDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} //namespace otb
#endif


