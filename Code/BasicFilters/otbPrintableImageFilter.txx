/*=========================================================================
	
  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbPrintableImageFilter_txx
#define __otbPrintableImageFilter_txx

#include "otbPrintableImageFilter.h"


namespace otb
{
  
  template <class TInputImage>
      PrintableImageFilter<TInputImage>
  ::PrintableImageFilter()
  {
    
    m_Rescaler = VectorRescalerType::New();
    m_Extractor = ChannelExtractorType::New();
    
    m_Extractor->SetInput( m_Rescaler->GetOutput() );
    
  }
  
  template <class TInputImage>
      void
      PrintableImageFilter<TInputImage>
  ::SetChannel(unsigned int channel)
  {
    m_Extractor->SetChannel(channel);
    this->Modified();
  }
  
  template <class TInputImage>
      const typename PrintableImageFilter<TInputImage>::ChannelsType
      PrintableImageFilter<TInputImage>
  ::GetChannels() const
  {
    return m_Extractor->GetChannels();
  }
  
  
  template <class TInputImage>
      void
      PrintableImageFilter<TInputImage>
  ::GenerateData()
  {
    
    if(m_Extractor->GetNbChannels() == 0){
      m_Extractor->SetChannel(2);
      m_Extractor->SetChannel(3);
      m_Extractor->SetChannel(4); 
    }
    
    typename TInputImage::PixelType minimum,maximum;
    minimum.SetSize(this->GetInput()->GetNumberOfComponentsPerPixel());
    maximum.SetSize(this->GetInput()->GetNumberOfComponentsPerPixel());
    minimum.Fill(0);
    maximum.Fill(255);
    
    m_Rescaler->SetInput(this->GetInput());
    m_Rescaler->SetOutputMinimum(minimum);
    m_Rescaler->SetOutputMaximum(maximum);
    m_Rescaler->SetClampThreshold(0.01);
        
    m_Extractor->GraftOutput( this->GetOutput() );
    m_Extractor->Update();
    this->GraftOutput( m_Extractor->GetOutput() );
  }
      
  template <class TInputImage>
      void
      PrintableImageFilter<TInputImage>
  ::PrintSelf( std::ostream& os, itk::Indent indent ) const
  {
    Superclass::PrintSelf(os,indent);
  }
          
} // end namespace otb

#endif
