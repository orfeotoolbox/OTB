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
#ifndef __otbSimpleRcsPanSharpeningFusionImageFilter_txx
#define __otbSimpleRcsPanSharpeningFusionImageFilter_txx

#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"


namespace otb
{
  template <class TPanImageType, class TXsImageType, class TOutputImageType>
      SimpleRcsPanSharpeningFusionImageFilter
          <TPanImageType, TXsImageType, TOutputImageType>
  ::SimpleRcsPanSharpeningFusionImageFilter()
  {
    this->SetNumberOfRequiredInputs(2);
    m_ConvolutionFilter = ConvolutionFilterType::New();
    m_DivideFilter = DivideFilterType::New();
    m_MultiplyFilter = MultiplyFilterType::New();
    
     
    m_Radius.Fill(3);
    m_Filter.SetSize(7*7);
    m_Filter.Fill(1);
    
    m_DivideFilter->SetInput2(m_ConvolutionFilter->GetOutput());
    m_MultiplyFilter->SetInput1(m_DivideFilter->GetOutput());
    
  }
  
  template <class TPanImageType, class TXsImageType, class TOutputImageType>
      void
          SimpleRcsPanSharpeningFusionImageFilter
          <TPanImageType, TXsImageType, TOutputImageType>
  ::SetPanInput(const TPanImageType *image)
  {
  // We have 2 inputs:  an image and a vector image

  // Process object is not const-correct so the const_cast is required here
    this->itk::ProcessObject::SetNthInput(1, 
                                     const_cast<  TPanImageType* >( image ) );
    this->Modified();
  }

  template <class TPanImageType, class TXsImageType, class TOutputImageType>
      const TPanImageType *
          SimpleRcsPanSharpeningFusionImageFilter
          <TPanImageType, TXsImageType, TOutputImageType>
  ::GetPanInput(void) const
  {
    if (this->GetNumberOfInputs() < 2)
    {
      return 0;
    }
  
    return static_cast<const TPanImageType * >
        (this->itk::ProcessObject::GetInput(1) );
  }
  
  template <class TPanImageType, class TXsImageType, class TOutputImageType>
      void
          SimpleRcsPanSharpeningFusionImageFilter
          <TPanImageType, TXsImageType, TOutputImageType>
  ::SetXsInput(const TXsImageType *image)
  {
  // We have 2 inputs:  an image and a vector image

  // Process object is not const-correct so the const_cast is required here
    this->itk::ProcessObject::SetNthInput(0, 
                         const_cast<  TXsImageType* >( image ) );
    this->Modified();
  }

  template <class TPanImageType, class TXsImageType, class TOutputImageType>
      const TXsImageType *
          SimpleRcsPanSharpeningFusionImageFilter
          <TPanImageType, TXsImageType, TOutputImageType>
  ::GetXsInput(void) const
  {
    if (this->GetNumberOfInputs() < 1)
    {
      return 0;
    }
  
    return static_cast<const TXsImageType * >
        (this->itk::ProcessObject::GetInput(0) );
  }
  
  
  template <class TPanImageType,class TXsImageType,class TOutputImageType>
      void
      SimpleRcsPanSharpeningFusionImageFilter
          <TPanImageType, TXsImageType, TOutputImageType>
  ::GenerateData()
      {
        
        m_ConvolutionFilter->SetInput( this->GetPanInput() );
        m_ConvolutionFilter->SetRadius( this->m_Radius); 
        m_ConvolutionFilter->SetFilter( this->m_Filter);
        
        m_DivideFilter->SetInput1(this->GetXsInput());

        m_MultiplyFilter->SetInput2(this->GetPanInput());

        m_MultiplyFilter->GraftOutput( this->GetOutput() );
        m_MultiplyFilter->Update();
        this->GraftOutput( m_MultiplyFilter->GetOutput() );
      }
      
  template <class TPanImageType,class TXsImageType,class TOutputImageType>
          void
      SimpleRcsPanSharpeningFusionImageFilter
              <TPanImageType, TXsImageType, TOutputImageType>
  ::PrintSelf( std::ostream& os, itk::Indent indent ) const
          {
            Superclass::PrintSelf(os,indent);

            os
                << indent << "Radius:" << this->m_Radius
                << std::endl;
          }
          
} // end namespace otb

#endif
