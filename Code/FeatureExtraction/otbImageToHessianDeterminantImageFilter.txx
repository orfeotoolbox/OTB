#ifndef _otbImageToHessianDeterminantImageFilter_txx
#define _otbImageToHessianDeterminantImageFilter_txx

#include "otbImageToHessianDeterminantImageFilter.h"

namespace otb
{
  template <class TInputImage, class TOutputImage, class TPrecision>
    ImageToHessianDeterminantImageFilter<TInputImage,TOutputImage,TPrecision>
  ::ImageToHessianDeterminantImageFilter()
  {
    // Filter intialisation
    m_HessianFilter = ImageToHessianImageFilterType::New();
    m_DeterminantFilter = DeterminantFilterType::New();
    // pipeline wiring
    m_DeterminantFilter->SetInput(m_HessianFilter->GetOutput());
    // default parameter value
    m_Sigma = 1.0;
  }
  
   template <class TInputImage, class TOutputImage, class TPrecision>
    ImageToHessianDeterminantImageFilter<TInputImage,TOutputImage,TPrecision>
  ::~ImageToHessianDeterminantImageFilter()
   {}


  template <class TInputImage, class TOutputImage, class TPrecision>
  void
  ImageToHessianDeterminantImageFilter<TInputImage,TOutputImage,TPrecision>
  ::GenerateData(void)
  {
    // Execute minipipeline
    m_HessianFilter->SetInput(this->GetInput());
    m_HessianFilter->SetSigma(m_Sigma);
    m_DeterminantFilter->GraftOutput(this->GetOutput());
    m_DeterminantFilter->Update();
    this->GraftOutput(m_DeterminantFilter->GetOutput());
  }

  /*PrintSelf*/
  template <class TInputImage, class TOutputImage, class TPrecision  >
  void
  ImageToHessianDeterminantImageFilter<TInputImage,TOutputImage,TPrecision>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent <<"Sigma: " << m_Sigma << std::endl;
  }

}
#endif
