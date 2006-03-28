/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   28 mars 2006
  Role      :   Harris image filter 
  $Id$ 

=========================================================================*/
#ifndef __otbHarrisImageFilter_txx
#define __otbHarrisImageFilter_txx

#include "otbHarrisImageFilter.h"


namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
HarrisImageFilter<TInputImage, TOutputImage>
::HarrisImageFilter()
{
  m_SigmaD = 1.0;
  m_SigmaI = 1.0;
  m_Alpha  = 1.0;
  
  m_HessianFilter         = HessianFilterType::New();
  m_GaussianFilter        = GaussianFilterType::New();  
  m_HessianToScalarFilter = HessianToScalarFilterType::New();
}

template <class TInputImage, class TOutputImage>
void
HarrisImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  m_HessianFilter->SetInput( this->GetInput() );
  m_HessianFilter->SetSigma( this->m_SigmaD );
  m_HessianFilter->SetNormalizeAcrossScale(false);

  m_GaussianFilter->SetInput(m_HessianFilter->GetOutput() );
  m_GaussianFilter->SetSigma( this->m_SigmaI );
  m_GaussianFilter->SetOrder( GaussianFilterType::ZeroOrder );
  m_GaussianFilter->SetNormalizeAcrossScale(false);
  
  m_HessianToScalarFilter->SetInput( m_GaussianFilter->GetOutput() );
  m_HessianToScalarFilter->SetAlpha( this->m_Alpha );

  m_HessianToScalarFilter->GraftOutput(this->GetOutput() ); 
  m_HessianToScalarFilter->Update();
  this->GraftOutput(m_HessianToScalarFilter->GetOutput() );
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void 
HarrisImageFilter<TInputImage, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Sigma_D : " << this->m_SigmaD << std::endl;
  os << indent << "Sigma_I : " << this->m_SigmaI << std::endl;
  os << indent << "Alpha   : " << this->m_Alpha  << std::endl;
}


} // end namespace otb


#endif
