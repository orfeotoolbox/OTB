#ifndef otbCoefOfThePatchFilter_txx
#define otbCoefOfThePatchFilter_txx

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
#include "otbCoefOfThePatchFilter.h"


#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

#include "otbUnaryFunctorNeighborhoodImageFilter.h"



namespace otb
{	
	
template <class TInputImage, class TOutputImage >
CoefOfThePatchFilter<TInputImage,TOutputImage>
::CoefOfThePatchFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(1);  

  // Set the outputs
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0,TOutputImage::New()); 
  this->SetNthOutput(1,TOutputImage::New());  
  
this->SetNumberOfThreads(1); 
}

template <class TInputImage,  class TOutputImage >
CoefOfThePatchFilter<TInputImage,TOutputImage>
::~CoefOfThePatchFilter()
{}
// =================================== Seters ========================================================

template <class TInputImage,  class TOutputImage >
void
CoefOfThePatchFilter<TInputImage, TOutputImage >
::SetInputImage(const TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}




//================================== Geters ====================================================
template <class TInputImage, class TOutputImage >
const TInputImage *
CoefOfThePatchFilter<TInputImage,  TOutputImage >
::GetInputImage() const
{
  if (this->GetNumberOfInputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}




template <class TInputImage,  class TOutputImage >
TOutputImage *
CoefOfThePatchFilter<TInputImage,  TOutputImage >
::GetOutputCoefImage()
{
  if (this->GetNumberOfOutputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TOutputImage *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage,  class TOutputImage >
TOutputImage *
CoefOfThePatchFilter<TInputImage,  TOutputImage >
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
CoefOfThePatchFilter<TInputImage,  TOutputImage >
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();
  
  this->GetOutputCoefImage()->SetNumberOfComponentsPerPixel(3);
   this->GetOutputNormalAndZValueImage()->SetNumberOfComponentsPerPixel(4);
}


//============================================  ThreadedGenerateData  ========================================================
template <class TInputImage,  class TOutputImage >
void
CoefOfThePatchFilter<TInputImage,  TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{	
 
PixelType coeffs(3);
 coeffs.Fill(0);
 
 
 //double d(0.);
 
 PixelType NormalAndZValue(4);
 NormalAndZValue.Fill(0);
 
double z(0.),nx(0.),ny(0.),nz(0.),NormalNorm(0.);

itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
//créer l'intérator  
itk::ImageRegionIterator<TOutputImage> outputIt ( this->GetOutputCoefImage(), outputRegionForThread );
outputIt.GoToBegin(); 
   	      
itk::ImageRegionConstIterator<TInputImage> InputImageIt ( this->GetInputImage(), outputRegionForThread );
InputImageIt.GoToBegin();

while ( !outputIt.IsAtEnd() && !InputImageIt.IsAtEnd() ){
		IndexType index = InputImageIt.GetIndex();
		
		 z = ( rand()/(double)RAND_MAX )*(m_DispMax - m_DispMin) + m_DispMin;	
		 
		  
		// le vecteur normal au plan valeurs aléatoires
			
		
		//std::cout<< "d =  " <<d << std::endl;		
		//std::cout<< "index  " <<index << std::endl;
		
		 nx = ( rand()/(double)RAND_MAX ) *(2) - 1; 
		 ny = ( rand()/(double)RAND_MAX ) *(2) - 1;  
		 nz =  ( rand()/(double)RAND_MAX ) *(2) - 1; 
		
		 NormalNorm = std::sqrt(nx*nx + ny*ny + nz*nz);
		nx = nx/NormalNorm;
		ny = ny/NormalNorm;
		nz = nz/NormalNorm;
				
		NormalAndZValue[0] = nx;
		NormalAndZValue[1] = ny;
		NormalAndZValue[2] = nz;
		
		NormalAndZValue[3] = z;
		
		coeffs[0] = -nx / nz;
		coeffs[1] = -ny / nz;
		coeffs[2] = (nx*index[0] + ny*index[1] + nz*z) / nz ;
	
		
			 
	//~ while( d>m_DispMax || d< m_DispMin){
		 //~ z = (int)rand() % (m_DispMax - m_DispMin) + m_DispMin;	 
		//~ // le vecteur normal au plan valeurs aléatoires
			
			//~ std::cout<< "d2 =  " <<d << std::endl;	
			
		//~ //std::cout<< "index  " <<index << std::endl;
		
		 //~ nx = ( rand()/(double)RAND_MAX ) *(2) - 1; 
		 //~ ny = ( rand()/(double)RAND_MAX ) *(2) - 1;  
		 //~ nz =  ( rand()/(double)RAND_MAX ) *(2) - 1; 
		
		 //~ NormalNorm = std::sqrt(nx*nx + ny*ny + nz*nz);
		//~ nx = nx/NormalNorm;
		//~ ny = ny/NormalNorm;
		//~ nz = nz/NormalNorm;
				
		//~ NormalAndZValue[0] = nx;
		//~ NormalAndZValue[1] = ny;
		//~ NormalAndZValue[2] = nz;
		
		//~ NormalAndZValue[3] = z;
		
		//~ coeffs[0] = -nx / nz;
		//~ coeffs[1] = -ny / nz;
		//~ coeffs[2] = (nx*index[0] + ny*index[1] + nz*z) / nz ;

	 //~ d = coeffs[0]*index[0] + coeffs[1]*index[1] + coeffs[2] ;
	//~ }
	

								
	    outputIt.Set(coeffs);
		//std::cout << " cost volume = "<< OutPixel[0] ;
		
		this->GetOutputNormalAndZValueImage()->SetPixel(index, NormalAndZValue);	
				        
        ++InputImageIt;         
        ++outputIt;

        progress.CompletedPixel();
}  // end of while 




} //end of threaded generate data


} // End namespace otb

#endif
