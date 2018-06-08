#ifndef otbPatchMatchRefinementImageFilter_txx
#define otbPatchMatchRefinementImageFilter_txx

/*===================================================================================================

Implémentation of coeffs of patches 

=====================================================================================================*/

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>

#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionReverseConstIterator.h"
#include "otbPatchMatchRefinementImageFilter.h"


#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

#include "otbUnaryFunctorNeighborhoodImageFilter.h"



namespace otb
{	
	
template <class TInputImage, class TOutputImage >
PatchMatchRefinementImageFilter<TInputImage,TOutputImage>
::PatchMatchRefinementImageFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(1);  

  // Set the outputs
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0,TOutputImage::New()); 
  this->SetNthOutput(1,TOutputImage::New()); 

  
this->SetNumberOfThreads(1);
m_Radius.Fill(1);
 
}

template <class TInputImage,  class TOutputImage >
PatchMatchRefinementImageFilter<TInputImage,TOutputImage>
::~PatchMatchRefinementImageFilter()
{}
// =================================== Seters ========================================================

template <class TInputImage,  class TOutputImage >
void
PatchMatchRefinementImageFilter<TInputImage, TOutputImage >
::SetNormalAndZValueImage(const TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}




//================================== Geters ====================================================
template <class TInputImage, class TOutputImage >
const TInputImage *
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::GetNormalAndZValueImage() const
{
  if (this->GetNumberOfInputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}


// get output

template <class TInputImage,  class TOutputImage >
TOutputImage *
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::GetOutputPatchImage()
{
  if (this->GetNumberOfOutputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TOutputImage *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage,  class TOutputImage >
TOutputImage *
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::GetOutputNormalAndZValueImage()
{
  if (this->GetNumberOfOutputs()<2)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TOutputImage *>(this->itk::ProcessObject::GetOutput(1));
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


//============================================  ThreadedGenerateData  ========================================================
template <class TInputImage,  class TOutputImage >
void
PatchMatchRefinementImageFilter<TInputImage,  TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{	
 
//std::cout<< "Radius"<< this->GetRadius()<< std::endl;
	std::cout << "ThreadedGenerateData : PatchMatchRefinementImageFilter.txx " << std::endl;

//input iterator 

ConstNeighborhoodIteratorType NormalAndZValueInputIt (this->GetRadius(), this->GetNormalAndZValueImage(), outputRegionForThread );
NormalAndZValueInputIt.GoToBegin();
   

itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());


PixelType coeffs(3);
coeffs.Fill(0);

PixelType NormalAndZValue(4);
NormalAndZValue.Fill(0);



double Dz0(0.),z1(0.);

while ( !NormalAndZValueInputIt.IsAtEnd() ){
	
	
IndexType Index = NormalAndZValueInputIt.GetIndex();

		 Dz0  = ( rand()/(double)RAND_MAX ) *(m_MaxDz0 + m_MaxDz0) - m_MaxDz0;
		 z1 = NormalAndZValueInputIt.GetCenterPixel()[3] + Dz0;
		 
		if( z1>m_DispMax || z1< m_DispMin){
			 z1 = NormalAndZValueInputIt.GetCenterPixel()[3];
			// std::cout<< "z1= " << z1 << std::endl;
		 }
		  
//~ std::cout<< "m_DispMax= " << m_DispMax << std::endl;
//~ std::cout<< "m_DispMin= " << m_DispMin << std::endl;
	//~ while( z1>m_DispMax || z1< m_DispMin){	
		//~ int i = 0;
		//~ i= i+1;
		 //~ Dz0  = ( rand()/(double)RAND_MAX ) *(m_MaxDz0 + m_MaxDz0) - m_MaxDz0;
		 //~ z1 = NormalAndZValueInputIt.GetCenterPixel()[3] + Dz0; 
		 
		 
		 //~ if(i>2){
			 //~ z1 = NormalAndZValueInputIt.GetCenterPixel()[3];
			 //~ std::cout<< "z1= " << z1 << std::endl;	
			 //~ break;
		 //~ }
	//~ }	
	//~ if(z1>m_DispMax || z1< m_DispMin){
		//~ std::cout<< "z2= " << z1 << std::endl;
	//~ }
		
	
		double nx = ( rand()/(double)RAND_MAX ) *( m_MaxDn + m_MaxDn) - m_MaxDn; 
		double ny = ( rand()/(double)RAND_MAX ) *(m_MaxDn + m_MaxDn) - m_MaxDn;  
		double nz =  ( rand()/(double)RAND_MAX ) *(m_MaxDn + m_MaxDn) - m_MaxDn;  
		 

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
