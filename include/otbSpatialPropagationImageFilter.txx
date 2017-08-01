
#ifndef otbSpatialPropagationImageFilter_txx
#define otbSpatialPropagationImageFilter_txx


#include "otbSpatialPropagationImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConstNeighborhoodIterator.h"


#include "itkTimeProbe.h" 
#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

#include "otbAggregatedCostVectorPixelFilter.h"

#include "otbMacro.h"

namespace otb
{	
	
template <class TInputImage, class TOutputImage >
SpatialPropagationImageFilter<TInputImage, TOutputImage>
::SpatialPropagationImageFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(5);  

  // Set the outputs
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TOutputImage::New()); 
  this->SetNthOutput(1,TOutputImage::New());
  this->SetNthOutput(2,TOutputImage::New());
  this->SetNumberOfThreads(1); 

m_AggregatedCost = AggregatedCostType::New();	
m_Radius.Fill(0);

}

template <class TInputImage,  class TOutputImage >
SpatialPropagationImageFilter<TInputImage,TOutputImage>
::~SpatialPropagationImageFilter()
{}
// =================================== Seters ========================================================

template <class TInputImage,  class TOutputImage >
void
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::SetLeftInputImage(const TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::SetRightInputImage(const TInputImage * image)
{
 
  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}


template <class TInputImage,  class TOutputImage >
void
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::SetLeftGradientXInput(const TInputImage * image)
{

  this->SetNthInput(2, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::SetRightGradientXInput(const TInputImage * image)
{

  this->SetNthInput(3, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::SetPatchInputImage(TInputImage * image)
{

  this->SetNthInput(4, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::SetNormalAndZValueImage(TInputImage * image)
{

  this->SetNthInput(5, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::SetMinPatchOutputImage(const TInputImage * image)
{

  this->SetNthInput(1, const_cast<TInputImage *>( image ));
  
}


//================================== Geters ====================================================
template <class TInputImage, class TOutputImage >
const TInputImage *
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
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
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
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
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
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
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
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
SpatialPropagationImageFilter<TInputImage, TOutputImage >
::GetPatchInputImage()
{
  if(this->GetNumberOfInputs()<5)
    {
    return ITK_NULLPTR;
    }
  return static_cast<TInputImage *>(this->itk::ProcessObject::GetInput(4));
}

template <class TInputImage,  class TOutputImage >
const TInputImage *
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::GetNormalAndZValueImage() const
{
if(this->GetNumberOfInputs()<6)
    {
    return ITK_NULLPTR;
    }
  return static_cast< const TInputImage *>(this->itk::ProcessObject::GetInput(5));
}

template <class TInputImage,  class TOutputImage >
TOutputImage *
SpatialPropagationImageFilter<TInputImage, TOutputImage >
::GetOutputCostImage()
{
  if (this->GetNumberOfOutputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TOutputImage *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage,  class TOutputImage >
TOutputImage *
SpatialPropagationImageFilter<TInputImage, TOutputImage >
::GetOutputPatchImage()
{
  if (this->GetNumberOfOutputs()<2)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TOutputImage *>(this->itk::ProcessObject::GetOutput(1));
}
template <class TInputImage,  class TOutputImage >
TOutputImage *
SpatialPropagationImageFilter<TInputImage, TOutputImage >
::GetOutputNormalAndZValueImage()
{
  if (this->GetNumberOfOutputs()<3)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TOutputImage *>(this->itk::ProcessObject::GetOutput(2));
}    
//============================================  GenerateOutputInformation  ========================================================
template <class TInputImage,  class TOutputImage >
void
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  this->GetOutputPatchImage()->SetNumberOfComponentsPerPixel(3);
  this->GetOutputCostImage()->SetNumberOfComponentsPerPixel(1);
  this->GetOutputNormalAndZValueImage()->SetNumberOfComponentsPerPixel(4);

}
// =================== Enlarge =========
template <class TInputImage,  class TOutputImage >
void
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::EnlargeOutputRequestedRegion(itk::DataObject * /*image*/){
	
this->GetOutputNormalAndZValueImage()->SetRequestedRegionToLargestPossibleRegion();
this->GetOutputCostImage()->SetRequestedRegionToLargestPossibleRegion();	
this->GetOutputPatchImage()->SetRequestedRegionToLargestPossibleRegion();	
	}




//============================================  GenerateInputRequestedRegion  ========================================================
template <class TInputImage,  class TOutputImage >
void
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::GenerateInputRequestedRegion()
{
 Superclass::GenerateInputRequestedRegion();
// get pointers to the input and output
typename TInputImage::Pointer  inLeftPtr = const_cast<InputImageType *>(this->GetLeftInputImage());
typename TInputImage::Pointer  inRightPtr = const_cast<InputImageType *>(this->GetRightInputImage());
typename TInputImage::Pointer  inLeftGradientXPtr = const_cast<InputImageType *>(this->GetLeftGradientXInput());
typename TInputImage::Pointer  inRightGradientXPtr = const_cast<InputImageType *>(this->GetRightGradientXInput());
typename TInputImage::Pointer  inPatchPtr = const_cast<InputImageType *>(this->GetPatchInputImage());




typename TOutputImage::ConstPointer outputPtrCost = this->GetOutputCostImage();
typename TOutputImage::ConstPointer outputPtrPlane = this->GetOutputPatchImage();
				
//Vérification de pointeur non null            
if (!inLeftPtr || !inRightPtr  || !inLeftGradientXPtr  || !inRightGradientXPtr ||!outputPtrCost || !inPatchPtr || !outputPtrPlane )
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
SpatialPropagationImageFilter<TInputImage,  TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId){


//input iterator 
ConstNeighborhoodIteratorType InputImageIt ( this->GetRadius(), this->GetLeftInputImage(), outputRegionForThread );

itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

SizeType ImageSize = this->GetLeftInputImage()->GetLargestPossibleRegion().GetSize();
IndexType NewIndex;
IndexType CostIndex =  {{0,0}};	

PixelType OutCost(1);
OutCost.Fill(10);
				 
PixelType OutPatch(3);
OutPatch.Fill(10);

PixelType NormalAndZValuePixel(4);
NormalAndZValuePixel.Fill(0);

//~ std::cout<< "  m_Iteration spatial avant le if =  " << m_Iteration<< std::endl;
//~ std::cout<< "  m_Iteration % (2) " << m_Iteration % (2) << std::endl;
    		  
if(m_Iteration % (2) == 1  ){	      
//std::cout<< "  m_Iteration spatial  dans le if=  " << m_Iteration<< std::endl;

InputImageIt.GoToBegin();
//std::cin.get();
while(!InputImageIt.IsAtEnd() ){
double m(100000); 	
int val(0);	

PixelType AggCostPixel(3);
AggCostPixel.Fill(0);


	
IndexType InputIndex = InputImageIt.GetIndex();	
//std::cin.get();
          		  
unsigned int x = InputIndex[0];
unsigned int y = InputIndex[1];

	 
		  m_AggregatedCost->SetLeftInputImage(this->GetLeftInputImage());
		  m_AggregatedCost->SetRightInputImage(this->GetRightInputImage());  
		  m_AggregatedCost->SetLeftGradientXInput(this->GetLeftGradientXInput());
		  m_AggregatedCost->SetRightGradientXInput(this->GetRightGradientXInput());  
		  m_AggregatedCost->SetPatchInputImage(this->GetPatchInputImage());		  
		  
		  m_AggregatedCost->SetIndex(x, y);  
		  m_AggregatedCost->SetOffsetPatch(m_Offset0,m_Offset1,m_Offset2);
		  m_AggregatedCost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
          m_AggregatedCost->Update();
		  m_AggregatedCost->Modified();		  
		  
AggCostPixel = m_AggregatedCost->GetOutput()->GetPixel(CostIndex);	
	
//m_AggregatedCost->UpdateOutputInformation();		  
//RegionType CostRegion = m_AggregatedCost->GetOutput()->GetLargestPossibleRegion();
	 

if(InputIndex[0] == 0 || InputIndex[0] == (unsigned)ImageSize[0] ||
 InputIndex[1] == 0 || InputIndex[1] == (unsigned)ImageSize[1] ){
 
    m = AggCostPixel[0];
    val = 0;
 }
else{
	 for(unsigned int b = 0; b<3 ; ++b){		 
					m = std::min(m,AggCostPixel[b]);
					 if ( m == AggCostPixel[b]){
						val = b ;
					 }				 
	}
  } 									
	OutCost[0] = m ;
	switch(val){
					case 0:	
					NewIndex[0] = m_Offset0[0] + InputIndex[0];
					NewIndex[1] = m_Offset0[1] + InputIndex[1];					
					
					//if(NewIndex[0] < (unsigned)ImageSize[0] && NewIndex[1] < (unsigned)ImageSize[1] && NewIndex[0] > 0 && NewIndex[1] > 0){
					OutPatch = this->GetPatchInputImage()->GetPixel(NewIndex);
					NormalAndZValuePixel = this->GetNormalAndZValueImage()->GetPixel(NewIndex);
					//}
			
					break;
					
					case 1:
					NewIndex[0] = m_Offset1[0] + InputIndex[0];
					NewIndex[1] = m_Offset1[1] + InputIndex[1];
					
					//if(NewIndex[0] < (unsigned)ImageSize[0] && NewIndex[1] < (unsigned)ImageSize[1] && NewIndex[0] > 0 && NewIndex[1] > 0){
					OutPatch = this->GetPatchInputImage()->GetPixel(NewIndex);
                    NormalAndZValuePixel = this->GetNormalAndZValueImage()->GetPixel(NewIndex);	
					//}
					
					break;	
					
					case 2:
					NewIndex[0] = m_Offset2[0] + InputIndex[0];
					NewIndex[1] = m_Offset2[1] + InputIndex[1];
					
					//if(NewIndex[0] < (unsigned)ImageSize[0] && NewIndex[1] < (unsigned)ImageSize[1] && NewIndex[0] > 0 && NewIndex[1] > 0){
					OutPatch = this->GetPatchInputImage()->GetPixel(NewIndex);
					NormalAndZValuePixel = this->GetNormalAndZValueImage()->GetPixel(NewIndex);
					//}
					
					break;
					
					
				 } // end of switch
 				
			this->GetPatchInputImage()->SetPixel(InputIndex, OutPatch);	
				
							
			this->GetOutputCostImage()->SetPixel(InputIndex, OutCost);	
			this->GetOutputPatchImage()->SetPixel(InputIndex, OutPatch);
            
            this->GetOutputNormalAndZValueImage()->SetPixel(InputIndex, NormalAndZValuePixel);		
				
++InputImageIt;
 
progress.CompletedPixel();
}  // end of while 
}// end if 

else{
    
        
InputImageIt.GoToEnd();

while (!InputImageIt.IsAtBegin() ){
	
--InputImageIt;

double m(100000); 	
int val(0);	

PixelType AggCostPixel(3);
AggCostPixel.Fill(0);
	
IndexType InputIndex = InputImageIt.GetIndex();	          		  
unsigned int x = InputIndex[0];
unsigned int y = InputIndex[1];

	 
		  m_AggregatedCost->SetLeftInputImage(this->GetLeftInputImage());
		  m_AggregatedCost->SetRightInputImage(this->GetRightInputImage());  
		  m_AggregatedCost->SetLeftGradientXInput(this->GetLeftGradientXInput());
		  m_AggregatedCost->SetRightGradientXInput(this->GetRightGradientXInput());  
		  m_AggregatedCost->SetPatchInputImage(this->GetPatchInputImage());		  
		  
		  m_AggregatedCost->SetIndex(x, y);  
		  m_AggregatedCost->SetOffsetPatch(m_Offset0,m_Offset1,m_Offset2);
		  m_AggregatedCost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
          m_AggregatedCost->Update();
		  m_AggregatedCost->Modified();		  
		  
AggCostPixel = m_AggregatedCost->GetOutput()->GetPixel(CostIndex);

if(InputIndex[0] == 0 || InputIndex[0] == (unsigned)ImageSize[0] ||
 InputIndex[1] == 0 || InputIndex[1] == (unsigned)ImageSize[1] ){
    m = AggCostPixel[0];
    val = 0;
 
}
else{
	 for(unsigned int b = 0; b<3 ; ++b){		 
					m = std::min(m,AggCostPixel[b]);
					 if ( m == AggCostPixel[b]){
						val = b ;
					 }				 
	}
  }  									
	OutCost[0] = m ;
	switch(val){
					case 0:	
					NewIndex[0] = m_Offset0[0] + InputIndex[0];
					NewIndex[1] = m_Offset0[1] + InputIndex[1];					
					
					//if(NewIndex[0] < (unsigned)ImageSize[0] && NewIndex[1] < (unsigned)ImageSize[1] && NewIndex[0] > 0 && NewIndex[1] > 0){
					OutPatch = this->GetPatchInputImage()->GetPixel(NewIndex);
					NormalAndZValuePixel = this->GetNormalAndZValueImage()->GetPixel(NewIndex);
					//}
					
					
						
					break;
					
					case 1:
					NewIndex[0] = m_Offset1[0] + InputIndex[0];
					NewIndex[1] = m_Offset1[1] + InputIndex[1];
					
					//if(NewIndex[0] < (unsigned)ImageSize[0] && NewIndex[1] < (unsigned)ImageSize[1] && NewIndex[0] > 0 && NewIndex[1] > 0){
					OutPatch = this->GetPatchInputImage()->GetPixel(NewIndex);
                    NormalAndZValuePixel = this->GetNormalAndZValueImage()->GetPixel(NewIndex);	
					//}
					
					
					break;	
					
					case 2:
					NewIndex[0] = m_Offset2[0] + InputIndex[0];
					NewIndex[1] = m_Offset2[1] + InputIndex[1];
					
					//if(NewIndex[0] < (unsigned)ImageSize[0] && NewIndex[1] < (unsigned)ImageSize[1] && NewIndex[0] > 0 && NewIndex[1] > 0){
					OutPatch = this->GetPatchInputImage()->GetPixel(NewIndex);
					NormalAndZValuePixel = this->GetNormalAndZValueImage()->GetPixel(NewIndex);
					//}
				
					break;
					
					
				 } // end of switch

 				
			this->GetPatchInputImage()->SetPixel(InputIndex, OutPatch);	
		//	 m_AggregatedCost->SetPatchInputImage(this->GetPatchInputImage());
				
			this->GetOutputNormalAndZValueImage()->SetPixel(InputIndex, NormalAndZValuePixel);				
			this->GetOutputCostImage()->SetPixel(InputIndex, OutCost);	
			this->GetOutputPatchImage()->SetPixel(InputIndex, OutPatch);

progress.CompletedPixel();
}  // end of while 
}// end else

} //end of threaded generate data


} // End namespace otb

#endif
