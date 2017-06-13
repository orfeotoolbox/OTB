
#ifndef otbBoxImageFilter_txx
#define otbBoxImageFilter_txx


#include "otbBoxImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConstNeighborhoodIterator.h"


#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"




namespace otb
{	
	
template <class TInputImage,  class TOutputImage >
BoxImageFilter<TInputImage,TOutputImage>
::BoxImageFilter()
{
  // Set the outputs
  this->SetNumberOfRequiredOutputs(0);
  this->SetNthOutput(0,TOutputImage::New());
  
  this->SetNumberOfThreads(1);
  

  // Default parameters
  m_Radius.Fill(1);   // The size of this radius is fixed at one, it serves just to calculate the integral image



}

template <class TInputImage,  class TOutputImage >
BoxImageFilter<TInputImage,TOutputImage>
::~BoxImageFilter()
{}
// ========================================================== Seters ==================================

template <class TInputImage,  class TOutputImage >
void
BoxImageFilter<TInputImage,  TOutputImage >
::SetImageInput(const TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}


//========================================================== Geters ======================================
template <class TInputImage,  class TOutputImage >
const TInputImage *
BoxImageFilter<TInputImage,  TOutputImage >
::GetImageInput() const
{
  if (this->GetNumberOfInputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}


template <class TInputImage,  class TOutputImage >
TOutputImage *
BoxImageFilter<TInputImage,  TOutputImage >
::GetOutputImage()
{
	
  if (this->GetNumberOfOutputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TOutputImage *>(this->itk::ProcessObject::GetOutput(0));
}




//================================================= GenerateOutputInformation  ===================================================
template <class TInputImage,  class TOutputImage >
void
BoxImageFilter<TInputImage,  TOutputImage >
::GenerateOutputInformation()
{  Superclass::GenerateOutputInformation();
	
  unsigned int Ncomp = this->GetInput()->GetNumberOfComponentsPerPixel();
  
  this->GetOutput()->SetNumberOfComponentsPerPixel(Ncomp);
  
 
}


//================================================ ThreadedGenerateData ====================================
template <class TInputImage,  class TOutputImage >
void
BoxImageFilter<TInputImage, TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{

//~ this->GetOutput->SetRequestedRegionToLargestPossibleRegion ();	

  //~ IndexType regionIndex;
  //~ regionIndex[0] = 0;
  //~ regionIndex[1] = 0;

//~ typename TOutputImage::ConstPointer outputPtr = this->GetOutputImage();
//~ RegionType LargestPossibleRegion = outputPtr->GetLargestPossibleRegion()  ;
//~ RegionType OutputLargestPossibleRegion ;
//~ OutputLargestPossibleRegion.SetSize(LargestPossibleRegion.GetSize());
//~ OutputLargestPossibleRegion.SetIndex(regionIndex);
 
 //std::cout << "la Region utilisé=  " << this->GetImageInput()->GetRequestedRegion()<< std::endl;

//créer l'intérator
ConstNeighborhoodIteratorType InputImageIt( this->GetRadius(), this->GetImageInput(), outputRegionForThread  );
InputImageIt.GoToBegin();

unsigned int Nband = InputImageIt.GetPixel(0).Size() ;
   
 BoundaryConditionType c;
 PixelType V(Nband);
 V.Fill(0);
 c.SetConstant(V);

NeighborhoodIteratorType outputIt( this->GetRadius(), this->GetOutputImage(), outputRegionForThread);
outputIt.GoToBegin();
outputIt.SetBoundaryCondition(c);

// Box filter computation    
  
itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

    
typename TOutputImage::PixelType OutPixel(Nband);
OutPixel.Fill(0);

 

      while ( !outputIt.IsAtEnd() && !InputImageIt.IsAtEnd() )
      {
		  
		  typename TInputImage::IndexType index;
		  index =InputImageIt.GetIndex();
		  for (unsigned int b  = 0; b< Nband; b++){		
					
					double somme(0.);		
						
					somme = InputImageIt.GetPixel(4)[b]+outputIt.GetPixel(1)[b]+outputIt.GetPixel(3)[b]-outputIt.GetPixel(0)[b] ; 
							           						
		
				//~ if((index[0] == 100 && index[1] == 97)){
					
					//~ ///refaire le calcul de II pour cette ligne
				//~ std::cout << "veleur d'entrée pour l'image integrales =  " << InputImageIt.GetPixel(4)[b]<< std::endl;
				//~ std::cout << "veleur de II position 1 =  " << outputIt.GetPixel(1)[b]<< std::endl;
				//~ std::cout << "veleur de II position 3 =  " << outputIt.GetPixel(3)[b]<< std::endl;
				//~ std::cout << "veleur de II position 0 =  " << outputIt.GetPixel(0)[b]<< std::endl;
				//~ std::cout << "somme =  " << somme<< std::endl;

				
				//~ }
			
			OutPixel[b] = static_cast<typename TOutputImage::InternalPixelType>(somme) ;
			
	
					
					  } // end of b	
					  outputIt.SetCenterPixel( OutPixel );
		          
        ++InputImageIt;
		++outputIt;	     

        progress.CompletedPixel();
      }  // end of while 

        
} //end of threaded generate data



} // End namespace otb

#endif
