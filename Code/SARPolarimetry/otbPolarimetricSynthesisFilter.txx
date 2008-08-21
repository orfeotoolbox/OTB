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
  this->SetNumberOfRequiredInputs(0);
  this->SetNumberOfInputs(4);
  SetMode(0);
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

template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::Print()
{
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
}

/**
 * Force Copolar mode
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::ForceCoPolar()
{
        SetPsiR(m_PsiI);
        SetTauR(m_TauI);        
        SetMode(1);
}

/**
 * Force Crosspolar mode
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::ForceCrossPolar()
{
        SetPsiR(m_PsiI+90);
        SetTauR(-m_TauI);
        SetMode(2);        
}

/**
 * Computation of the electromagnetic fields Ei Er
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::ComputeElectromagneticFields()
{
  ComplexArrayType AEi, AEr;
  
  /** Conversion coefficient Degre To Radian */
  double DTOR=M_PI/180;  
  double real,imag;
  
  real = vcl_cos(DTOR*m_PsiI)*vcl_cos(DTOR*m_TauI);
  imag = -vcl_sin(DTOR*m_PsiI)*vcl_sin(DTOR*m_TauI);
  ComplexType Ei0(real,imag);
  
  real = vcl_sin(DTOR*m_PsiI)*vcl_cos(DTOR*m_TauI);
  imag = vcl_cos(DTOR*m_PsiI)*vcl_sin(DTOR*m_TauI);
  ComplexType Ei1(real,imag);
  
  real = vcl_cos(DTOR*m_PsiR)*vcl_cos(DTOR*m_TauR);
  imag = -vcl_sin(DTOR*m_PsiR)*vcl_sin(DTOR*m_TauR);
  ComplexType Er0(real,imag);
  
  real = vcl_sin(DTOR*m_PsiR)*vcl_cos(DTOR*m_TauR);
  imag = vcl_cos(DTOR*m_PsiR)*vcl_sin(DTOR*m_TauR);
  ComplexType Er1(real,imag);
  
  AEi[0]=Ei0;
  AEi[1]=Ei1;
  AEr[0]=Er0;
  AEr[1]=Er1;

  this->SetEi(AEi);
  this->SetEr(AEr);  
           
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

  // With all the channels
  if ( ( this->GetInput(0)!=0 && this->GetInput(1)!=0 )&&
       ( this->GetInput(2)!=0 && this->GetInput(3)!=0 ) )
    {
    //nothing
    }
  else 
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
        if(GetMode()==1)ForceCoPolar();
        else if(GetMode()==2)ForceCrossPolar();      
  }
  else
  // Only HH and HV are present
  if ( ( this->GetInput(0)!=0 && this->GetInput(1)!=0 ) &&
       ( this->GetInput(2)==0 && this->GetInput(3)==0 ) )
    {
        // Forcing VH and VV to zero 
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
        if(GetMode()==1)ForceCoPolar();
        else if(GetMode()==2)ForceCrossPolar();        
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
   
   Print();
}

}

#endif

