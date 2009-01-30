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
  SetGain(1);
  m_PresentInputImages[0] = false;
  m_PresentInputImages[1] = false;
  m_PresentInputImages[2] = false;
  m_PresentInputImages[3] = false;
  m_ArchitectureType = PolarimetricData::New();
}

template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::GenerateOutputInformation()
{
  // if HH is set, use HH to generate output information
  if (m_PresentInputImages[0])
  {
    this->GetOutput()->CopyInformation(this->GetInput(0));
  }
  // else, use VH
  else if (m_PresentInputImages[2])
  {
    this->GetOutput()->CopyInformation(this->GetInput(2));
  }
  else
  {
    itkExceptionMacro(<<"Bad input polarization images: neither HH image nor VH image is set!");
  }
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
  m_PresentInputImages[0]=true;
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
  m_PresentInputImages[1]=true;
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
  m_PresentInputImages[2]=true;
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
  m_PresentInputImages[3]=true;
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

  os<<"PsiI: "<<m_PsiI<<std::endl;
  os<<"KhiI: "<<m_KhiI<<std::endl;
  os<<"PsiR: "<<m_PsiR<<std::endl;
  os<<"KhiR: "<<m_KhiR<<std::endl;

  os<<"Ei0 im: "<<m_Ei[0].imag()<<std::endl;
  os<<"Ei0 re: "<<m_Ei[0].real()<<std::endl;
  os<<"Ei1 im: "<<m_Ei[1].imag()<<std::endl;
  os<<"Ei1 re: "<<m_Ei[1].real()<<std::endl;

  os<<"Er0 im: "<<m_Er[0].imag()<<std::endl;
  os<<"Er0 re: "<<m_Er[0].real()<<std::endl;
  os<<"Er1 im: "<<m_Er[1].imag()<<std::endl;
  os<<"Er1 re: "<<m_Er[1].real()<<std::endl;
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
  SetKhiR(m_KhiI);
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
  SetKhiR(-m_KhiI);
  SetMode(2);
}

/**
 * Verify and force the inputs, if only 2 or 3 channels are present. Determine the kind of architecture.
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::VerifyAndForceInputs()
{

  switch (m_ArchitectureType->GetArchitectureType())
  {
  case HH_HV :
    // Forcing KhiI=0 PsiI=0
    this->SetKhiI(0);
    this->SetPsiI(0);
    break;

  case VH_VV :
    // Forcing KhiI=0 PsiI=90
    this->SetKhiI(0);
    this->SetPsiI(90);
    break;

  case HH_VV :
    itkExceptionMacro("Only the HH and VV channels are available : Polarimetric synthesis is not supported !");
    return;

  case UNKNOWN :
    itkExceptionMacro("Unknown architecture : Polarimetric synthesis is impossible !");
    return;

  default :
    break;

  }

  if (GetMode()==1)ForceCoPolar();
  else if (GetMode()==2)ForceCrossPolar();

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

  real = vcl_cos(DTOR*m_PsiI)*vcl_cos(DTOR*m_KhiI);
  imag = -vcl_sin(DTOR*m_PsiI)*vcl_sin(DTOR*m_KhiI);
  ComplexType Ei0(real,imag);

  real = vcl_sin(DTOR*m_PsiI)*vcl_cos(DTOR*m_KhiI);
  imag = vcl_cos(DTOR*m_PsiI)*vcl_sin(DTOR*m_KhiI);
  ComplexType Ei1(real,imag);

  real = vcl_cos(DTOR*m_PsiR)*vcl_cos(DTOR*m_KhiR);
  imag = -vcl_sin(DTOR*m_PsiR)*vcl_sin(DTOR*m_KhiR);
  ComplexType Er0(real,imag);

  real = vcl_sin(DTOR*m_PsiR)*vcl_cos(DTOR*m_KhiR);
  imag = vcl_cos(DTOR*m_PsiR)*vcl_sin(DTOR*m_KhiR);
  ComplexType Er1(real,imag);

  AEi[0]=Ei0;
  AEi[1]=Ei1;
  AEr[0]=Er0;
  AEr[1]=Er1;

  this->SetEi(AEi);
  this->SetEr(AEr);

}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::BeforeThreadedGenerateData()
{

  // First Part. Determine the kind of architecture
  m_ArchitectureType->DetermineArchitecture(m_PresentInputImages);

  // Second Part. Verify and force the inputs
  VerifyAndForceInputs();

  // Third Part. Estimation of the incident field Ei and the reflected field Er
  ComputeElectromagneticFields();

}

/**
 * ThreadedGenerateData function. Performs the pixel-wise addition
 */
template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,class TFunction  >
void
PolarimetricSynthesisFilter<TInputImageHH,TInputImageHV,TInputImageVH,TInputImageVV,TOutputImage,TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{

  OutputImagePointer outputPtr = this->GetOutput(0);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);
  outputIt.GoToBegin();

  switch (m_ArchitectureType->GetArchitectureType())
  {
    // With 4 channels :
  case HH_HV_VH_VV :
  {
    // We use dynamic_cast since inputs are stored as DataObjects.  The
    // ImageToImageFilter::GetInput(int) always returns a pointer to a
    // TInputImage1 so it cannot be used for the second or third input.
    HHInputImagePointer inputPtrHH
    = dynamic_cast<const TInputImageHH*>((itk::ProcessObject::GetInput(0)));
    HVInputImagePointer inputPtrHV
    = dynamic_cast<const TInputImageHV*>((itk::ProcessObject::GetInput(1)));
    VHInputImagePointer inputPtrVH
    = dynamic_cast<const TInputImageVH*>((itk::ProcessObject::GetInput(2)));
    VVInputImagePointer inputPtrVV
    = dynamic_cast<const TInputImageVV*>((itk::ProcessObject::GetInput(3)));

    itk::ImageRegionConstIterator<TInputImageHH> inputItHH(inputPtrHH, outputRegionForThread);
    itk::ImageRegionConstIterator<TInputImageHV> inputItHV(inputPtrHV, outputRegionForThread);
    itk::ImageRegionConstIterator<TInputImageVH> inputItVH(inputPtrVH, outputRegionForThread);
    itk::ImageRegionConstIterator<TInputImageVV> inputItVV(inputPtrVV, outputRegionForThread);

    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

    inputItHH.GoToBegin();
    inputItHV.GoToBegin();
    inputItVH.GoToBegin();
    inputItVV.GoToBegin();

    while ( !inputItHH.IsAtEnd() )
    {
      outputIt.Set( m_Gain * Superclass::GetFunctor()(inputItHH.Get(), inputItHV.Get(), inputItVH.Get(), inputItVV.Get()) );
      ++inputItHH;
      ++inputItHV;
      ++inputItVH;
      ++inputItVV;
      ++outputIt;
      progress.CompletedPixel(); // potential exception thrown here
    }
    break;
  }
  // With 3 channels : HH HV VV
  case HH_HV_VV :

  {
    HHInputImagePointer inputPtrHH
    = dynamic_cast<const TInputImageHH*>((itk::ProcessObject::GetInput(0)));
    HVInputImagePointer inputPtrHV
    = dynamic_cast<const TInputImageHV*>((itk::ProcessObject::GetInput(1)));
    VVInputImagePointer inputPtrVV
    = dynamic_cast<const TInputImageVV*>((itk::ProcessObject::GetInput(3)));

    itk::ImageRegionConstIterator<TInputImageHH> inputItHH(inputPtrHH, outputRegionForThread);
    itk::ImageRegionConstIterator<TInputImageHV> inputItHV(inputPtrHV, outputRegionForThread);
    itk::ImageRegionConstIterator<TInputImageVV> inputItVV(inputPtrVV, outputRegionForThread);
    itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

    inputItHH.GoToBegin();
    inputItHV.GoToBegin();
    inputItVV.GoToBegin();

    while ( !inputItHH.IsAtEnd() )
    {
      outputIt.Set( m_Gain * Superclass::GetFunctor()(inputItHH.Get(), inputItHV.Get(), inputItHV.Get(), inputItVV.Get()) );
      ++inputItHH;
      ++inputItHV;
      ++inputItVV;
      ++outputIt;
      progress.CompletedPixel(); // potential exception thrown here
    }
    break;
  }
  // With 3 channels : HH VH VV
  case HH_VH_VV :
  {

    HHInputImagePointer inputPtrHH
    = dynamic_cast<const TInputImageHH*>((itk::ProcessObject::GetInput(0)));
    VHInputImagePointer inputPtrVH
    = dynamic_cast<const TInputImageVH*>((itk::ProcessObject::GetInput(2)));
    VVInputImagePointer inputPtrVV
    = dynamic_cast<const TInputImageVV*>((itk::ProcessObject::GetInput(3)));
    OutputImagePointer outputPtr = this->GetOutput(0);

    itk::ImageRegionConstIterator<TInputImageHH> inputItHH(inputPtrHH, outputRegionForThread);
    itk::ImageRegionConstIterator<TInputImageVH> inputItVH(inputPtrVH, outputRegionForThread);
    itk::ImageRegionConstIterator<TInputImageVV> inputItVV(inputPtrVV, outputRegionForThread);

    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

    inputItHH.GoToBegin();
    inputItVH.GoToBegin();
    inputItVV.GoToBegin();

    while ( !inputItHH.IsAtEnd() )
    {
      outputIt.Set( m_Gain * Superclass::GetFunctor()(inputItHH.Get(), inputItVH.Get(), inputItVH.Get(), inputItVV.Get()) );
      ++inputItHH;
      ++inputItVH;
      ++inputItVV;
      ++outputIt;
      progress.CompletedPixel(); // potential exception thrown here
    }
    break;
  }
  // With 2 channels : HH HV
  case HH_HV :
  {

    HHInputImagePointer inputPtrHH
    = dynamic_cast<const TInputImageHH*>((itk::ProcessObject::GetInput(0)));
    HVInputImagePointer inputPtrHV
    = dynamic_cast<const TInputImageHV*>((itk::ProcessObject::GetInput(1)));

    itk::ImageRegionConstIterator<TInputImageHH> inputItHH(inputPtrHH, outputRegionForThread);
    itk::ImageRegionConstIterator<TInputImageHV> inputItHV(inputPtrHV, outputRegionForThread);
    itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

    inputItHH.GoToBegin();
    inputItHV.GoToBegin();

    while ( !inputItHH.IsAtEnd() )
    {
      outputIt.Set( m_Gain * Superclass::GetFunctor()(inputItHH.Get(), inputItHV.Get(), 0,0 ));
      ++inputItHH;
      ++inputItHV;
      ++outputIt;
      progress.CompletedPixel(); // potential exception thrown here
    }
    break;
  }
  // With 2 channels : VH VV
  case VH_VV :
  {
    VHInputImagePointer inputPtrVH
    = dynamic_cast<const TInputImageVH*>((itk::ProcessObject::GetInput(2)));
    VVInputImagePointer inputPtrVV
    = dynamic_cast<const TInputImageVV*>((itk::ProcessObject::GetInput(3)));

    itk::ImageRegionConstIterator<TInputImageVH> inputItVH(inputPtrVH, outputRegionForThread);
    itk::ImageRegionConstIterator<TInputImageVV> inputItVV(inputPtrVV, outputRegionForThread);
    itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

    inputItVH.GoToBegin();
    inputItVV.GoToBegin();

    while ( !inputItVH.IsAtEnd() )
    {
      outputIt.Set( m_Gain * Superclass::GetFunctor()(0,0, inputItVH.Get(), inputItVV.Get()) );
      ++inputItVH;
      ++inputItVV;
      ++outputIt;
      progress.CompletedPixel(); // potential exception thrown here
    }
    break;
  }
  default :
    itkExceptionMacro("Unknown architecture : Polarimetric synthesis is impossible !");
    return;

  }
} // end namespace otb

}

#endif

