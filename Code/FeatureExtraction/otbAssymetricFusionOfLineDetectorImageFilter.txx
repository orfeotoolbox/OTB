/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   29 mars 2006
  Role      :   Associative Symetrical Sum of Line Detector Image Filter 
  $Id$ 

=========================================================================*/
#ifndef __otbAssymetricFusionOfLineDetectorImageFilter_txx
#define __otbAssymetricFusionOfLineDetectorImageFilter_txx

#include "otbAssymetricFusionOfLineDetectorImageFilter.h"


namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class InterpolatorType>
AssymetricFusionOfLineDetectorImageFilter<TInputImage, TOutputImage, InterpolatorType>
::AssymetricFusionOfLineDetectorImageFilter()
{
  m_LengthLine = 1;
  m_WidthLine = 0;

  
  m_LineRatio         = LineRatioType::New();
  m_LineCorrelation   = LineCorrelationType::New();
  m_AssSymSum         = AssSymSumType::New();
}

template <class TInputImage, class TOutputImage, class InterpolatorType>
void
AssymetricFusionOfLineDetectorImageFilter<TInputImage, TOutputImage, InterpolatorType>
::GenerateData()
{
  m_LineRatio->SetInput( this->GetInput() );
  m_LineRatio->SetLengthLine( this->m_LengthLine );
  m_LineRatio->SetWidthLine( this->m_WidthLine );

  m_LineCorrelation->SetInput( this->GetInput() );
  m_LineCorrelation->SetLengthLine( this->m_LengthLine );
  m_LineCorrelation->SetWidthLine( this->m_WidthLine );  

  m_AssSymSum->SetInput1( m_LineRatio->GetOutput() );
  m_AssSymSum->SetInput2( m_LineCorrelation->GetOutput() );
  
  m_AssSymSum->GraftOutput(this->GetOutput() ); 
  m_AssSymSum->Update();
  this->GraftOutput(m_AssSymSum->GetOutput() );
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput, class InterpolatorType>
void 
AssymetricFusionOfLineDetectorImageFilter<TInputImage, TOutput, InterpolatorType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Length: " << m_LengthLine << std::endl;
  os << indent << "Width: " << m_WidthLine << std::endl;
  
}


} // end namespace otb


#endif
