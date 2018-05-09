
#ifndef otbRMSEVectorImageFilter_txx
#define otbRMSEVectorImageFilter_txx


#include "otbRMSEVectorImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkContinuousIndex.h"
#include "itkLinearInterpolateImageFunction.h"


#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

#include "otbMacro.h"

namespace otb
{	
	
template <class TInputImage, class TOutputImage >
RMSEVectorImageFilter<TInputImage, TOutputImage>
::RMSEVectorImageFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(2);  

  // Set the outputs
  this->SetNumberOfRequiredOutputs(0);
  this->SetNthOutput(0,TOutputImage::New()); 
  
 this->SetNumberOfThreads(1); 

	

}

template <class TInputImage,  class TOutputImage >
RMSEVectorImageFilter<TInputImage,TOutputImage>
::~RMSEVectorImageFilter()
{}
// =================================== Seters ========================================================

template <class TInputImage,  class TOutputImage >
void
RMSEVectorImageFilter<TInputImage,  TOutputImage >
::SetEstimatedInputImage(TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
RMSEVectorImageFilter<TInputImage,  TOutputImage >
::SetInputImage( TInputImage * image)
{
 
  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}


//================================== Geters ====================================================
template <class TInputImage, class TOutputImage >
TInputImage *
RMSEVectorImageFilter<TInputImage,  TOutputImage >
::GetEstimatedInputImage() 
{
  if (this->GetNumberOfInputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TOutputImage >
 TInputImage *
RMSEVectorImageFilter<TInputImage,  TOutputImage >
::GetInputImage() 
{
  if (this->GetNumberOfInputs()<2)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TInputImage *>(this->itk::ProcessObject::GetInput(1));
}


    
//============================================  GenerateOutputInformation  ========================================================
template <class TInputImage,  class TOutputImage >
void
RMSEVectorImageFilter<TInputImage,  TOutputImage >
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();
  
  SizeType RegionSize;
  RegionSize[0] = 1;
  RegionSize[1] = 1;
  
  IndexType RegionIndex;
  RegionIndex[0] = 0;
  RegionIndex[1] = 0;
  
  RegionType region;
  region.SetSize(RegionSize);
  region.SetIndex(RegionIndex);
  
  this->GetOutput()->SetNumberOfComponentsPerPixel(1);
 this->GetOutput()->SetLargestPossibleRegion(region);
}


//============================================  ThreadedGenerateData  ========================================================
template <class TInputImage,  class TOutputImage >
void
RMSEVectorImageFilter<TInputImage,  TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
	
 
itk::ImageRegionIterator<TInputImage> EstimatedInIt ( this->GetEstimatedInputImage(), outputRegionForThread );
EstimatedInIt.GoToBegin(); 

itk::ImageRegionIterator<TInputImage> InIt ( this->GetInputImage(), outputRegionForThread );
InIt.GoToBegin(); 
  
	
IndexType Index =  {{0,0}};		
SizeType ImageSize = this->GetEstimatedInputImage()->GetLargestPossibleRegion().GetSize();

 std::cout << "ImageSize  = " << ImageSize << std::endl;
         
		  
itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

    
PixelType OutPixel(1);
OutPixel.Fill(0);

double somme(0.),temp(0.), m(0.);

while( !EstimatedInIt.IsAtEnd() ){		  
				
//temp = (EstimatedInIt.Get()[0] - EstimatedInIt.Get()[0])*(EstimatedInIt.Get()[0] - EstimatedInIt.Get()[0] ) ; 
temp = (EstimatedInIt.Get()[0] - InIt.Get()[0])*(EstimatedInIt.Get()[0] - InIt.Get()[0] ) ; 

somme +=temp ;  							           						
		   
    std::cout << "somme  = " << temp << std::endl;        
    ++InIt;
		++EstimatedInIt;
		progress.CompletedPixel();
       
}  // end of while 
      
      m = std::sqrt (somme/ (ImageSize[0]*ImageSize[1]));
      std::cout << "m   = " << m  << std::endl;   


    OutPixel[0] = static_cast<typename TOutputImage::InternalPixelType>(m) ;
      
      this->GetOutput()->SetPixel(Index, OutPixel);
     
     
} //end of threaded generate data


} // End namespace otb

#endif
