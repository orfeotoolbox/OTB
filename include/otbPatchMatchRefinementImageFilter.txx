
#ifndef otbPatchMatchRefinementImageFilter_txx
#define otbPatchMatchRefinementImageFilter_txx

#include <cstdlib>
#include <cmath>

#include <stdio.h>

#include <stdlib.h>

#include "otbPatchMatchRefinementImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConstNeighborhoodIterator.h"


#include "itkTimeProbe.h" 
#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"


#include "otbMacro.h"

namespace otb
{	
	
template <class TInputImage, class TOutputImage >
PatchMatchRefinementImageFilter<TInputImage, TOutputImage>
::PatchMatchRefinementImageFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(2);  

  // Set the outputs
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0,TOutputImage::New()); 
   this->SetNthOutput(1,TOutputImage::New()); 
 
  this->SetNumberOfThreads(1); 
	
m_Radius.Fill(0);
m_PatchSize.Fill(1);

}

template <class TInputImage,  class TOutputImage >
PatchMatchRefinementImageFilter<TInputImage,TOutputImage>
::~PatchMatchRefinementImageFilter()
{}
// =================================== Seters ========================================================


template <class TInputImage,  class TOutputImage >
void
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::SetPatchInputImage( TInputImage * image)
{

  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}


template <class TInputImage,  class TOutputImage >
void
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::SetNormalAndZValueImage(TInputImage * image)
{

  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}



//================================== Geters ====================================================


template <class TInputImage,  class TOutputImage >
 TInputImage *
PatchMatchRefinementImageFilter<TInputImage, TOutputImage >
::GetPatchInputImage() 
{
  if(this->GetNumberOfInputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TInputImage *>(this->itk::ProcessObject::GetInput(0));
}


template <class TInputImage,  class TOutputImage >
const TInputImage *
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::GetNormalAndZValueImage() const
{
if(this->GetNumberOfInputs()<2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
}

// get output 
template <class TInputImage,  class TOutputImage >
 TInputImage *
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::GetOutputPatchImage() 
{
if(this->GetNumberOfInputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast<  TInputImage *>(this->itk::ProcessObject::GetInput(0));
}
template <class TInputImage,  class TOutputImage >
 TInputImage *
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::GetOutputNormalAndZValueImage() 
{
if(this->GetNumberOfInputs()<2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<  TInputImage *>(this->itk::ProcessObject::GetInput(1));
}

     
//============================================  GenerateOutputInformation  ========================================================
template <class TInputImage,  class TOutputImage >
void
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  this->GetOutputPatchImage()->SetNumberOfComponentsPerPixel(3);
  this->GetOutputNormalAndZValueImage()->SetNumberOfComponentsPerPixel(4);

}
//============================================  GenerateInputRequestedRegion  ========================================================
template <class TInputImage,  class TOutputImage >
void
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::GenerateInputRequestedRegion()
{
 Superclass::GenerateInputRequestedRegion();

// get pointers to the input and output
typename TInputImage::Pointer  inNormalPtr = const_cast<InputImageType *>(this->GetNormalAndZValueImage());
typename TInputImage::Pointer  inPatchPtr = const_cast<InputImageType *>(this->GetPatchInputImage());

typename TOutputImage::ConstPointer outputPtrPlane = this->GetOutputPatchImage();
				
//Vérification de pointeur non null            
if ( !inPatchPtr || !inNormalPtr || !outputPtrPlane )
    {
    return;
    } 
 
  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inPatchPtr->GetRequestedRegion();
    SizeType Re;
Re[0] = m_PatchSize[0] ;
Re[1] = m_PatchSize[1] ;
  // pad the input requested region by the plane Size
  inputRequestedRegion.PadByRadius(Re);
  otbMsgDevMacro(<< "Padding by " << Re);
  otbMsgDevMacro(<< "Region is now " << inputRequestedRegion.GetIndex() << ", " << inputRequestedRegion.GetSize());

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inPatchPtr->GetLargestPossibleRegion()))
    {
    inPatchPtr->SetRequestedRegion(inputRequestedRegion);
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inPatchPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (in spatialPro filter)(at least partially) outside the largest possible region.");
    e.SetDataObject(inPatchPtr);
    throw e;
    }


   
   
} // end of GenerateInputRequestedRegion

//============================================  ThreadedGenerateData  ========================================================
template <class TInputImage,  class TOutputImage >
void
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId){


//input iterator 

ConstNeighborhoodIteratorType NormalAndZValueInputIt (this->GetRadius(), this->GetNormalAndZValueImage(), outputRegionForThread );
NormalAndZValueInputIt.GoToBegin();
   

itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());


PixelType coeffs(3);
coeffs.Fill(0);

PixelType NormalAndZValue(4);
NormalAndZValue.Fill(0);

double MaxDz0 = m_DispMax/2;
double MaxDn = 1;



while ( !NormalAndZValueInputIt.IsAtEnd() ){
	
	
IndexType Index = NormalAndZValueInputIt.GetIndex();

		
		double Dz0  = ( rand()/(double)RAND_MAX ) *(MaxDz0 + MaxDz0) - MaxDz0;
		

		double z1 = NormalAndZValueInputIt.GetCenterPixel()[3] + Dz0; 
		
		
		double nx = ( rand()/(double)RAND_MAX ) *(MaxDn + MaxDn) - MaxDn; 
		double ny = ( rand()/(double)RAND_MAX ) *(MaxDn + MaxDn) - MaxDn;  
		double nz =  ( rand()/(double)RAND_MAX ) *(MaxDn + MaxDn) - MaxDn;  
		 

		//double Dn  = (int)rand() % (2*m_MaxDn - m_MaxDn);
		double nx1 = NormalAndZValueInputIt.GetCenterPixel()[0] + nx; 		
		double ny1 = NormalAndZValueInputIt.GetCenterPixel()[1] + ny;
		double nz1 = NormalAndZValueInputIt.GetCenterPixel()[2] + nz;
		
		double NormalNorm = std::sqrt(nx1*nx1 + ny1*ny1 + nz1*nz1);


	// Normalisation
		nx1 = nx1/NormalNorm;
	    ny1 = ny1/NormalNorm;
		nz1 = nz1/NormalNorm;


		coeffs[0] = -nx1 / nz1;
		coeffs[1] = -ny1 / nz1;
		coeffs[2] = (nx1*Index[0] + ny1*Index[1] + nz1*z1) / nz1 ;
if(MaxDz0>0.1){		
MaxDz0 = MaxDz0/2;
MaxDn = MaxDn/2; 
}
else{
MaxDz0 = MaxDz0;
MaxDn = MaxDn;
}		
	    NormalAndZValue[0] = nx1;
		NormalAndZValue[1] = ny1;
		NormalAndZValue[2] = nz1;
		
		NormalAndZValue[3] = z1;	
	
	this->GetOutputPatchImage()->SetPixel(Index, coeffs);
	this->GetOutputNormalAndZValueImage()->SetPixel(Index, NormalAndZValue);
							
	
	++NormalAndZValueInputIt;
	progress.CompletedPixel();
	 
}


 

} //end of threaded generate data


} // End namespace otb

#endif
