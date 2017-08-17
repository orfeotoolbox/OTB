
#ifndef otbAggregatedCostVectorPixelFilter_txx
#define otbAggregatedCostVectorPixelFilter_txx


#include "otbAggregatedCostVectorPixelFilter.h"
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
AggregatedCostVectorPixelFilter<TInputImage, TOutputImage>
::AggregatedCostVectorPixelFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(4);  

  // Set the outputs
  this->SetNumberOfRequiredOutputs(0);
  this->SetNthOutput(0,TOutputImage::New()); 
  
 this->SetNumberOfThreads(1); 

	

}

template <class TInputImage,  class TOutputImage >
AggregatedCostVectorPixelFilter<TInputImage,TOutputImage>
::~AggregatedCostVectorPixelFilter()
{}
// =================================== Seters ========================================================

template <class TInputImage,  class TOutputImage >
void
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::SetLeftInputImage(const TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::SetRightInputImage(const TInputImage * image)
{
 
  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}


template <class TInputImage,  class TOutputImage >
void
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::SetLeftGradientXInput(const TInputImage * image)
{

  this->SetNthInput(2, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::SetRightGradientXInput(const TInputImage * image)
{

  this->SetNthInput(3, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::SetPatchInputImage(TInputImage * image)
{

  this->SetNthInput(4, image); //const_cast<TInputImage *>( image)

}
//================================== Geters ====================================================
template <class TInputImage, class TOutputImage >
const TInputImage *
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::GetLeftInputImage() const
{
  if (this->GetNumberOfInputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TOutputImage >
const TInputImage *
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::GetRightInputImage() const
{
  if (this->GetNumberOfInputs()<2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
}


template <class TInputImage,  class TOutputImage >
const TInputImage *
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::GetLeftGradientXInput() const
{
  if(this->GetNumberOfInputs()<3)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(2));
}
template <class TInputImage,  class TOutputImage >
const TInputImage *
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::GetRightGradientXInput() const
{
  if(this->GetNumberOfInputs()<4)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(3));
}


template <class TInputImage,  class TOutputImage >
TInputImage *
AggregatedCostVectorPixelFilter<TInputImage, TOutputImage >
::GetPatchInputImage()
{
  if(this->GetNumberOfInputs()<5)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TInputImage *>(this->itk::ProcessObject::GetInput(4)); 
  //static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(4)
}





    
//============================================  GenerateOutputInformation  ========================================================
template <class TInputImage,  class TOutputImage >
void
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
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
  
  this->GetOutput()->SetNumberOfComponentsPerPixel(3);
 this->GetOutput()->SetLargestPossibleRegion(region);
}
//============================================  GenerateInputRequestedRegion  ========================================================
template <class TInputImage,  class TOutputImage >
void
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::GenerateInputRequestedRegion()
{
 Superclass::GenerateInputRequestedRegion();	
 //this->GetOutput()->SetRequestedRegionToLargestPossibleRegion();
 
 
/// get pointers to the input and output
typename TInputImage::Pointer  inLeftPtr = const_cast<InputImageType *>(this->GetLeftInputImage());
typename TInputImage::Pointer  inRightPtr = const_cast<InputImageType *>(this->GetRightInputImage());
typename TInputImage::Pointer  inLeftGradientXPtr = const_cast<InputImageType *>(this->GetLeftGradientXInput());
typename TInputImage::Pointer  inRightGradientXPtr = const_cast<InputImageType *>(this->GetRightGradientXInput());
typename TInputImage::Pointer  inPatchPtr = const_cast<InputImageType *>(this->GetPatchInputImage());

typename TOutputImage::ConstPointer outputPtr = this->GetOutput();
				
//Vérification de pointeur non null            
if (!inLeftPtr || !inRightPtr  || !inLeftGradientXPtr  || !inRightGradientXPtr ||!outputPtr || !inPatchPtr)
    {
    return;
    } 
 
  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inPatchPtr->GetRequestedRegion();
  
  SizeType Re;
Re[0] = m_PatchSize[0] +3 ;
Re[1] = m_PatchSize[1] +3;
  // pad the input requested region by the operator radius
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
    e.SetDescription("Requested region is (in Aggregated cost filter)(at least partially) outside the largest possible region.");
    e.SetDataObject(inPatchPtr);
    throw e;
    }

} // end of GenerateInputRequestedRegion

//============================================  ThreadedGenerateData  ========================================================
template <class TInputImage,  class TOutputImage >
void
AggregatedCostVectorPixelFilter<TInputImage,  TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{	  
itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

SizeType ImageSize = this->GetLeftInputImage()->GetLargestPossibleRegion().GetSize();
		
itk::ContinuousIndex<double,2>  ShiftedIndex;
			
typedef itk::LinearInterpolateImageFunction
    <TInputImage, double>  LinearInterpolationType;
		
		    IndexType InputIndex = m_Index;
		    IndexType AggregatedIndex = {{0,0}};
		  //  std::cout<< "m_Index =  " << m_Index<< std::endl;
		  		    
		    IndexType PatchIndex0;
		    IndexType PatchIndex1;
		    IndexType PatchIndex2;
		    
		    PatchIndex0[0]= m_PatchOffset0[0] + m_Index[0];
		    PatchIndex0[1]= m_PatchOffset0[1] + m_Index[1];
		    
		    PatchIndex1[0]= m_PatchOffset1[0] + m_Index[0];
		    PatchIndex1[1]= m_PatchOffset1[1] + m_Index[1];
		    
		    PatchIndex2[0]= m_PatchOffset2[0] + m_Index[0];
		    PatchIndex2[1]= m_PatchOffset2[1] + m_Index[1];
		    
		  
		    PixelType OutPixel(1);
			OutPixel.Fill(0);

		  // initialize
			PixelType costColor;
			costColor.Fill(0.);
			PixelType costGradient;
			costGradient.Fill(0.);
			
			double costColorNorm(0.), costGradientNorm(0.),Cost(0.), W(0.), m(0.);
						
			
			std::vector<double> Somme;
			Somme.resize(3,0);
			
			std::vector<IndexType> NewIndexList(3);
			//OffsetList.resize(3,m_Index);
			
			NewIndexList[0] = PatchIndex0;
			NewIndexList[1] = PatchIndex1;
			NewIndexList[2] = PatchIndex2;			
		
			//  Réglage des paramètres	
			double alpha (0.9);
			double taux1 (10); 
			double taux2 (2); 	
			double gamma (10);		 		
			
			SizeType PatchSize = this->GetPatchSize();
			int rx = (PatchSize[0] - 1)/2 ;
			int ry = (PatchSize[1] - 1)/2 ;
for(int p=0; p<3; p++){		
Somme[p] = 0;		
			
 for(int Vx = -rx; Vx <= rx ; ++Vx ){
	for(int Vy = -ry; Vy <= ry ; ++Vy ){			
				
	OffsetType NeighborhoodOffset = {{ Vx, Vy}};			
	IndexType NeighborhoodIndex;
				
	 if(NewIndexList[p][0] < (unsigned)ImageSize[0] && NewIndexList[p][1] < (unsigned)ImageSize[1]
					 && NewIndexList[p][0] >= 0 && NewIndexList[p][1] >= 0){
							  
						NeighborhoodIndex[0] = NeighborhoodOffset[0] + InputIndex[0];
						NeighborhoodIndex[1] = NeighborhoodOffset[1] + InputIndex[1];			 
				
						ShiftedIndex[0] = NeighborhoodIndex[0] - 
							(this->GetPatchInputImage()->GetPixel(NewIndexList[p])[0]*NeighborhoodIndex[0]+
							this->GetPatchInputImage()->GetPixel(NewIndexList[p])[1]* NeighborhoodIndex[1]+
							this->GetPatchInputImage()->GetPixel(NewIndexList[p])[2] ) ;				   
											   
						ShiftedIndex[1] = NeighborhoodIndex[1];	
						
		
				
		if( ShiftedIndex[0] < (unsigned)ImageSize[0] && ShiftedIndex[1] < (unsigned)ImageSize[1] 
			&& ShiftedIndex[0] >=0 && ShiftedIndex[1] >= 0 ){
					
		typename LinearInterpolationType::Pointer interpImage = LinearInterpolationType::New();
		interpImage->SetInputImage(this->GetRightInputImage());
		PixelType ShiftedRightPixel = interpImage->EvaluateAtContinuousIndex(ShiftedIndex);

		typename LinearInterpolationType::Pointer interpGradient = LinearInterpolationType::New();
		interpGradient->SetInputImage(this->GetRightGradientXInput());
		PixelType ShiftedRightGradientPixel = interpGradient->EvaluateAtContinuousIndex(ShiftedIndex);


		costColor = this->GetLeftInputImage()->GetPixel(NeighborhoodIndex)- ShiftedRightPixel ;			 
		costColorNorm=	costColor.GetNorm();																				
									
										if(costColorNorm > taux1) 
											costColorNorm = taux1;
													
						
		costGradient =   this->GetLeftGradientXInput()->GetPixel(NeighborhoodIndex)- ShiftedRightGradientPixel; 
		costGradientNorm =	costGradient.GetNorm() ;								
										
										if(costGradientNorm > taux2) 
											costGradientNorm = taux2;
								
										  
		Cost= (1-alpha)*costColorNorm + alpha*costGradientNorm ;
		
		
		W = ((this->GetLeftInputImage()->GetPixel(InputIndex)) - 
		(this->GetLeftInputImage()->GetPixel(NeighborhoodIndex))).GetNorm();	
		
		W = std::exp(W/(-gamma))	;// Weight computation 
		} // if ShiftedIndex

		else{				
			Cost = (1-alpha)*taux1 + alpha*taux2 ;
			W= 1.	;	//*					  
		 } //end of else
		
		m= W*Cost;//  m computation	
		
				
		Somme[p] += m;	
				
	 } // if NewIndexList
		
		else{ 
			Somme[p]  = 10e9;
		}		
	}// end of for  Vy
 } // end of for Vx	


					  
OutPixel[p] = static_cast<typename TOutputImage::InternalPixelType>(Somme[p]) ;				



} //end of loop for p

 	
     
this->GetOutput()->SetPixel(AggregatedIndex, OutPixel);
        progress.CompletedPixel();

} //end of threaded generate data


} // End namespace otb

#endif
