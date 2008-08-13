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
#ifndef __otbMultiChannelsPolarimetricSynthesisFilter_txx
#define __otbMultiChannelsPolarimetricSynthesisFilter_txx

#include "otbMultiChannelsPolarimetricSynthesisFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include <complex>

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction >
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::MultiChannelsPolarimetricSynthesisFilter()
{
  this->SetNumberOfRequiredInputs( 1 );
  //this->InPlaceOff();
}

 /** PolarimetricSynthesisFilter
   *
   *
 * \sa ProcessObject::GenerateOutputInformaton() 
 */
template <class TInputImage, class TOutputImage, class TFunction>
void 
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::GenerateOutputInformation()
{
  // do not call the superclass' implementation of this method since
  // this filter allows the input the output to be of different dimensions
 
  // get pointers to the input and output
  typename Superclass::OutputImagePointer      outputPtr = this->GetOutput();
  typename Superclass::InputImageConstPointer  inputPtr  = this->GetInput();

  if ( !outputPtr || !inputPtr)
    {
    return;
    }

  // Set the output image largest possible region.  Use a RegionCopier
  // so that the input and output images can be different dimensions.
  OutputImageRegionType outputLargestPossibleRegion;
  this->CallCopyInputRegionToOutputRegion(outputLargestPossibleRegion,
                                          inputPtr->GetLargestPossibleRegion());
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );

  // Set the output spacing and origin
  const itk::ImageBase<Superclass::InputImageDimension> *phyData;

  phyData
    = dynamic_cast<const itk::ImageBase<Superclass::InputImageDimension>*>(this->GetInput());

  if (phyData)
    {
    // Copy what we can from the image from spacing and origin of the input
    // This logic needs to be augmented with logic that select which
    // dimensions to copy
    unsigned int i, j;
    const typename InputImageType::SpacingType&
      inputSpacing = inputPtr->GetSpacing();
    const typename InputImageType::PointType&
      inputOrigin = inputPtr->GetOrigin();
    const typename InputImageType::DirectionType&
      inputDirection = inputPtr->GetDirection();

    typename OutputImageType::SpacingType outputSpacing;
    typename OutputImageType::PointType outputOrigin;
    typename OutputImageType::DirectionType outputDirection;

    // copy the input to the output and fill the rest of the
    // output with zeros.
    for (i=0; i < Superclass::InputImageDimension; ++i)
      {
      outputSpacing[i] = inputSpacing[i];
      outputOrigin[i] = inputOrigin[i];
      for (j=0; j < Superclass::OutputImageDimension; j++)
        {
        if (j < Superclass::InputImageDimension)
          {
          outputDirection[j][i] = inputDirection[j][i];
          }
        else
          {
          outputDirection[j][i] = 0.0;          
          }
        }
      }
    for (; i < Superclass::OutputImageDimension; ++i)
      {
      outputSpacing[i] = 1.0;
      outputOrigin[i] = 0.0;
      for (j=0; j < Superclass::OutputImageDimension; j++)
        {
        if (j == i)
          {
          outputDirection[j][i] = 1.0;
          }
        else
          {
          outputDirection[j][i] = 0.0;          
          }
        }
      }

    // set the spacing and origin
    outputPtr->SetSpacing( outputSpacing );
    outputPtr->SetOrigin( outputOrigin );
    outputPtr->SetDirection( outputDirection );
    
    }
  else
    {
    // pointer could not be cast back down
    itkExceptionMacro(<< "otb::MultiChannelRAndNIRVegetationIndexImageFilter::GenerateOutputInformation "
                      << "cannot cast input to "
                      << typeid(itk::ImageBase<Superclass::InputImageDimension>*).name() );
    }
}


/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{
  InputImagePointer  inputPtr = this->GetInput();
  OutputImagePointer outputPtr = this->GetOutput(0);
  
  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIterator<TInputImage>  inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  // Computation with 4 channels
  if ( inputPtr->GetNumberOfComponentsPerPixel()==4 )
    {
          while( !inputIt.IsAtEnd() ) 
            {
            outputIt.Set( m_Functor( inputIt.Get()[0], inputIt.Get()[1], inputIt.Get()[2], inputIt.Get()[3] ) );
            ++inputIt;
            ++outputIt;
            progress.CompletedPixel();  // potential exception thrown here
            }
    }            
            
}

/**
 * Computation of the electromagnetic fields Ei Er
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
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
 * Verify and force the inputs, if only  2 or 3 channels are present
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::VerifyAndForceInputs()
{
/*  InputPixelType pix;
  pix.imag()=0;
  pix.real()=0;

  // With 3 channels : HH VH VV
  if ( ( this->GetInput(0)!=0 && this->GetInput(1)==0 )&&
       ( this->GetInput(2)!=0 && this->GetInput(3)!=0 ) )
    {
      typename HVInputImageType::ConstPointer hvImage =  dynamic_cast<const HVInputImageType*>((itk::ProcessObject::GetInput(2)));;
      this->SetInputHV(hvImage);
      std::cout<<"Case 3 channels !!"<<std::endl;
    }
  else 
  // With 3 channels : HH HV VV  
  if ( ( this->GetInput(0)!=0 && this->GetInput(1)!=0 )&&
       ( this->GetInput(2)==0 && this->GetInput(3)!=0 ) )
  {
    //this->SetInputVH(static_cast<typename HVInputImageType::Pointer>(this->GetInput(1)));
	typename VHInputImageType::ConstPointer vhImage =  dynamic_cast<const VHInputImageType*>((itk::ProcessObject::GetInput(1)));;
	this->SetInputVH(vhImage);
        std::cout<<"Case 3 channels !!"<<std::endl;
  }
  else
  // Only VH and VV are present
  if ( ( this->GetInput(0)==0 && this->GetInput(1)==0 ) &&
       ( this->GetInput(2)!=0 && this->GetInput(3)!=0 ) )
  {
  
        std::cout<<"Case VH VV present !!"<<std::endl;  
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
        std::cout<<"Case HH HV present !!"<<std::endl;      
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
        std::cout<<"Case HH VV present !!"<<std::endl;  
        itkExceptionMacro("Only the HH and VV channels are available : Polarimetric synthesis is impossible !");
        return;
  } 
        std::cout<<"Fin VerifyAndForceInputs !!"<<std::endl;    
        
        */
}

/**
 *
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::BeforeThreadedGenerateData()
{
  try{
        // First Part. Verify and force the inputs
          VerifyAndForceInputs();   
  
        /*  std::cout<<"image 1 "<<this->GetInput(0)<<std::endl;
          std::cout<<"image 2 "<<this->GetInput(1)<<std::endl;
          std::cout<<"image 3 "<<this->GetInput(2)<<std::endl;
          std::cout<<"image 4 "<<this->GetInput(3)<<std::endl; 
  */
          // Second Part. Estimation of the incident field Ei and the reflected field Er
          ComputeElectromagneticFields();
  }
  catch( itk::ExceptionObject & err )
  {
          std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
          std::cout << "Exception in PolarimetricSynthesisFilter class :"<< err << std::endl; 
          return;
  }
  catch (...) 
  {
         // itkGenericExceptionMacro( <<"Unknown exception in PolarimetricSynthesisFilter class (catch(...)");
          std::cout << "Exception levee inconnue !" << std::endl; 
          return;
  }

}

/**
 * Printself
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "Psi I: "<<m_PsiI<<std::endl;
  os << indent << "Tau I: "<<m_TauI<<std::endl;
  os << indent << "Psi R: "<<m_PsiR<<std::endl;
  os << indent << "Tau R: "<<m_TauR<<std::endl;    
}

} // end namespace otb

#endif
