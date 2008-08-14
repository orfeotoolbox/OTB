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
#ifndef __otbPolarimetricSynthesisFilter_txx
#define __otbPolarimetricSynthesisFilter_txx

#include "otbPolarimetricSynthesisFilter.h"


namespace otb
{

/**
 * Constructor
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::PolarimetricSynthesisFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfInputs(4);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::SetInputHH( const TInputImageHH * image ) 
{
  // Process object is not const-correct so the const casting is required.
  SetInput1( image );
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::SetInputHV( const TInputImageHV * image ) 
{
  SetInput2( image );
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::SetInputVH( const TInputImageVH * image ) 
{
  SetInput3( image );
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::SetInputVV( const TInputImageVV * image ) 
{
  SetInput4( image );
}

/**
 * Printself
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

/**
 * Computation of the electromagnetic fields Ei Er
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::ComputeElectromagneticFields()
{
  ComplexType Ei0,Ei1,Er0,Er1;
  ComplexArrayType AEi, AEr;
  
  Ei0.real() = vcl_cos(m_DTOR*m_PsiI)*vcl_cos(m_DTOR*m_TauI);
  Ei0.imag() = -vcl_sin(m_DTOR*m_PsiI)*vcl_sin(m_DTOR*m_TauI);

  Ei1.real() = vcl_sin(m_DTOR*m_PsiI)*vcl_cos(m_DTOR*m_TauI);
  Ei1.imag() = vcl_cos(m_DTOR*m_PsiI)*vcl_sin(m_DTOR*m_TauI);

  Er0.real() = vcl_cos(m_DTOR*m_PsiR)*vcl_cos(m_DTOR*m_TauR);
  Er0.imag() = -vcl_sin(m_DTOR*m_PsiR)*vcl_sin(m_DTOR*m_TauR);

  Er1.real() = vcl_sin(m_DTOR*m_PsiR)*vcl_cos(m_DTOR*m_TauR);
  Er1.imag() = vcl_cos(m_DTOR*m_PsiR)*vcl_sin(m_DTOR*m_TauR);
  
  AEi[0]=Ei0;
  AEi[1]=Ei1;
  AEr[0]=Er0;
  AEr[1]=Er1;

  this->SetEi(AEi);
  this->SetEr(AEr);  
    
  std::cout<<"PsiI: "<<m_PsiI<<std::endl;
  std::cout<<"TauI: "<<m_TauI<<std::endl;
  std::cout<<"PsiR: "<<m_PsiR<<std::endl;
  std::cout<<"TauR: "<<m_TauR<<std::endl;
  
  std::cout<<"Ei0 im: "<<m_Ei[0].imag()<<std::endl;
  std::cout<<"Ei0 re: "<<m_Ei[0].real()<<std::endl;
  std::cout<<"Ei1 im: "<<m_Ei[1].imag()<<std::endl;
  std::cout<<"Ei1 re: "<<m_Ei[1].real()<<std::endl;
  
  std::cout<<"Er0 im: "<<m_Er[0].imag()<<std::endl;
  std::cout<<"Er0 re: "<<m_Er[0].real()<<std::endl;
  std::cout<<"Er1 im: "<<m_Er[1].imag()<<std::endl;
  std::cout<<"Er1 re: "<<m_Er[1].real()<<std::endl;
    
  std::cout<<"DTOR: "<<m_DTOR<<std::endl;       
}

/**
 * Verify and force the inputs, if only 2 or 3 channels are present
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::VerifyAndForceInputs()
{
  InputPixelType pix;
  pix.imag()=0;
  pix.real()=0;

  // With 3 channels : HH VH VV
  if ( ( this->GetInput(0)!=0 && this->GetInput(1)==0 )&&
       ( this->GetInput(2)!=0 && this->GetInput(3)!=0 ) )
    {
      typename HVInputImageType::ConstPointer hvImage =  dynamic_cast<const HVInputImageType*>((itk::ProcessObject::GetInput(2)));;
      this->SetInputHV(hvImage);
    }
  else 
  // With 3 channels : HH HV VV  
  if ( ( this->GetInput(0)!=0 && this->GetInput(1)!=0 )&&
       ( this->GetInput(2)==0 && this->GetInput(3)!=0 ) )
  {
	typename VHInputImageType::ConstPointer vhImage =  dynamic_cast<const VHInputImageType*>((itk::ProcessObject::GetInput(1)));;
	this->SetInputVH(vhImage);
  }
  else
  // Only VH and VV are present
  if ( ( this->GetInput(0)==0 && this->GetInput(1)==0 ) &&
       ( this->GetInput(2)!=0 && this->GetInput(3)!=0 ) )
  {
  
        // Forcing HH and HV to zero 
        typename HHInputImageType::Pointer inputHH = TInputImageHH::New();
        typename HVInputImageType::Pointer inputHV = TInputImageHV::New();        

        typename VHInputImageType::IndexType start;
        start[0]=0;
        start[1]=0;

	typename VHInputImageType::SizeType size;
	typename VHInputImageType::ConstPointer vhImage =  dynamic_cast<const VHInputImageType*>((itk::ProcessObject::GetInput(2)));
	size = vhImage->GetLargestPossibleRegion().GetSize();

        typename VHInputImageType::RegionType region;
        region.SetSize(size);
        region.SetIndex(start);
        inputHH->SetRegions(region);
        inputHH->Allocate();
        
        inputHH->FillBuffer(pix);
        
        inputHV->SetRegions(region);
        inputHV->Allocate();
        inputHV->FillBuffer(pix);        
                   
        this->SetInputHH(inputHH);
        this->SetInputHV(inputHV);

        // Forcing TauI=0 PsiI=90          
        this->SetTauI(0);
        this->SetPsiI(90);
  }
  else
  // Only HH and HV are present
  if ( ( this->GetInput(0)!=0 && this->GetInput(1)!=0 ) &&
       ( this->GetInput(2)==0 && this->GetInput(3)==0 ) )
    {
        // Forcing HH and HV to zero 
        typename VVInputImageType::Pointer inputVV = TInputImageVV::New();
        typename VHInputImageType::Pointer inputVH = TInputImageVH::New();        
        typename VHInputImageType::IndexType start;
        start[0]=0;
        start[1]=0;
        typename VHInputImageType::SizeType size;
	typename VHInputImageType::ConstPointer vhImage =  dynamic_cast<const VHInputImageType*>((itk::ProcessObject::GetInput(1)));

	size = vhImage->GetLargestPossibleRegion().GetSize();
  
	typename VHInputImageType::RegionType region;
        region.SetSize(size);
        region.SetIndex(start);
        inputVV->SetRegions(region);
        inputVV->Allocate();
        inputVV->FillBuffer(pix);
   
	inputVH->SetRegions(region);
        inputVH->Allocate();
        inputVH->FillBuffer(pix);        
       
	this->SetInputVV(inputVV);
        this->SetInputVH(inputVH);
        
	// Forcing TauI=0 PsiI=0
        this->SetTauI(0);
        this->SetPsiI(0);                      
  }
  else
  // Only HH and VV are present  
  if ( ( this->GetInput(0)!=0 && this->GetInput(1)==0 )&&
       ( this->GetInput(2)==0 && this->GetInput(3)!=0 ) )  
  {
        itkExceptionMacro("Only the HH and VV channels are available : Polarimetric synthesis is impossible !");
        return;
  }
  else
  {
        itkExceptionMacro("Unknown architecture : Polarimetric synthesis is impossible !");
        return;
  }  

}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::BeforeThreadedGenerateData()
{
   // First Part. Verify and force the inputs
   VerifyAndForceInputs();   
  
   // Second Part. Estimation of the incident field Ei and the reflected field Er
   ComputeElectromagneticFields();
}

}

#endif

