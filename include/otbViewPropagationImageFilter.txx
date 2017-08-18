
#ifndef otbViewPropagationImageFilter_txx
#define otbViewPropagationImageFilter_txx
#include <math.h> 

#include "otbViewPropagationImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConstNeighborhoodIterator.h"


#include "itkDataObject.h" 
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
ViewPropagationImageFilter<TInputImage, TOutputImage>
::ViewPropagationImageFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(7);  

  // Set the outputs
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TOutputImage::New()); 
  this->SetNthOutput(1,TOutputImage::New());
  this->SetNthOutput(2,TOutputImage::New());
  
  this->SetNumberOfThreads(1); 

m_LeftCost = AggregatedCostType::New();	
m_RightCost = AggregatedCostType::New();	
//~ m_Radius[0] = (unsigned int)((m_dispMax-m_dispMin)-1)/2;// il faut tjr le prendre >1, il définit la fenetre de recherche dans l'autre image
//~ m_Radius[1] = 0;
}

template <class TInputImage,  class TOutputImage >
ViewPropagationImageFilter<TInputImage,TOutputImage>
::~ViewPropagationImageFilter()
{}
// =================================== Seters ========================================================

template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::SetLeftInputImage(const TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::SetRightInputImage(const TInputImage * image)
{
 
  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}


template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::SetLeftGradientXInput(const TInputImage * image)
{

  this->SetNthInput(2, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::SetRightGradientXInput(const TInputImage * image)
{

  this->SetNthInput(3, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::SetLeftPatchInputImage( TInputImage * image)
{

  this->SetNthInput(4, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::SetRightPatchInputImage( TInputImage * image)
{

  this->SetNthInput(5, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::SetLeftNormalAndZValueImage(TInputImage * image)
{

  this->SetNthInput(6, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::SetRightNormalAndZValueImage(TInputImage * image)
{

  this->SetNthInput(7, const_cast<TInputImage *>( image ));
}


template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::SetSpatialCostImage(TInputImage * image)
{

  this->SetNthInput(8, const_cast<TInputImage *>( image ));
}


template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::SetOutputPatchImage(const TInputImage * image)
{

  this->SetNthInput(1, const_cast<TInputImage *>( image ));
  
}


//================================== Geters ====================================================
template <class TInputImage, class TOutputImage >
const TInputImage *
ViewPropagationImageFilter<TInputImage,  TOutputImage >
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
ViewPropagationImageFilter<TInputImage,  TOutputImage >
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
ViewPropagationImageFilter<TInputImage,  TOutputImage >
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
ViewPropagationImageFilter<TInputImage,  TOutputImage >
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
ViewPropagationImageFilter<TInputImage, TOutputImage >
::GetLeftPatchInputImage() 
{
  if(this->GetNumberOfInputs()<5)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TInputImage *>(this->itk::ProcessObject::GetInput(4));
}

template <class TInputImage,  class TOutputImage >
 TInputImage *
ViewPropagationImageFilter<TInputImage, TOutputImage >
::GetRightPatchInputImage() 
{
  if(this->GetNumberOfInputs()<6)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TInputImage *>(this->itk::ProcessObject::GetInput(5));
}
template <class TInputImage,  class TOutputImage >
 TInputImage *
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::GetLeftNormalAndZValueImage() 
{
if(this->GetNumberOfInputs()<7)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TInputImage *>(this->itk::ProcessObject::GetInput(6));
}

template <class TInputImage,  class TOutputImage >
 TInputImage *
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::GetRightNormalAndZValueImage() 
{
if(this->GetNumberOfInputs()<8)
    {
    return ITK_NULLPTR;
    }
  return static_cast<  TInputImage *>(this->itk::ProcessObject::GetInput(7));
}

template <class TInputImage,  class TOutputImage >
 TInputImage *
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::GetSpatialCostImage() 
{
if(this->GetNumberOfInputs()<9)
    {
    return ITK_NULLPTR;
    }
  return static_cast<  TInputImage *>(this->itk::ProcessObject::GetInput(8));
}
// outputs
template <class TInputImage,  class TOutputImage >
TOutputImage *
ViewPropagationImageFilter<TInputImage, TOutputImage >
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
ViewPropagationImageFilter<TInputImage, TOutputImage >
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
ViewPropagationImageFilter<TInputImage, TOutputImage >
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
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  this->GetOutputPatchImage()->SetNumberOfComponentsPerPixel(3);
  this->GetOutputCostImage()->SetNumberOfComponentsPerPixel(1);
this->GetOutputNormalAndZValueImage()->SetNumberOfComponentsPerPixel(4);
}
//============================================  GenerateInputRequestedRegion  ========================================================
template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::GenerateInputRequestedRegion()
{
 Superclass::GenerateInputRequestedRegion();

// get pointers to the input and output
typename TInputImage::Pointer  inLeftPtr = const_cast<InputImageType *>(this->GetLeftInputImage());
typename TInputImage::Pointer  inRightPtr = const_cast<InputImageType *>(this->GetRightInputImage());
typename TInputImage::Pointer  inLeftGradientXPtr = const_cast<InputImageType *>(this->GetLeftGradientXInput());
typename TInputImage::Pointer  inRightGradientXPtr = const_cast<InputImageType *>(this->GetRightGradientXInput());
typename TInputImage::Pointer  inPatchPtr = const_cast<InputImageType *>(this->GetLeftPatchInputImage());




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
Re[0] = m_PatchSize[0] +3;
Re[1] = m_PatchSize[1] +3;
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
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inPatchPtr);
    throw e;
    }


} // end of GenerateInputRequestedRegion
// ======================================== Enlarge =========================
template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::EnlargeOutputRequestedRegion(itk::DataObject * /*image*/){
	
this->GetOutputNormalAndZValueImage()->SetRequestedRegionToLargestPossibleRegion();
this->GetOutputCostImage()->SetRequestedRegionToLargestPossibleRegion();	
this->GetOutputPatchImage()->SetRequestedRegionToLargestPossibleRegion();	
	}


//============================================  ThreadedGenerateData  ========================================================
template <class TInputImage,  class TOutputImage >
void
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId){

RegionType bufferedRegion = this->GetOutputNormalAndZValueImage()->GetBufferedRegion();
//std::cout<< "bufferedRegion"<< bufferedRegion << std::endl;

 /// create an image
//~ PixelType p(3);
//~ p.Fill(0);

 //~ typename TInputImage::Pointer RightPatch = TInputImage::New(); 
    //~ RightPatch->CopyInformation(this->GetRightPatchInputImage());
    //~ RightPatch->SetNumberOfComponentsPerPixel(3);
    //~ RightPatch->SetRegions(this->GetRightPatchInputImage()->GetLargestPossibleRegion());
    //~ RightPatch->Allocate();   
    //~ RightPatch->FillBuffer(p);
   // RightPatch = this->GetRightPatchInputImage();
    
//input iterator

//~ itk::ImageRegionIterator<TInputImage> RightPatchIt ( this->GetRightPatchInputImage(), outputRegionForThread );
//~ RightPatchIt.GoToBegin();

//~ itk::ImageRegionIterator<TInputImage> OutputRightIt ( RightPatch, outputRegionForThread );
//~ OutputRightIt.GoToBegin();

//~ while ( !RightPatchIt.IsAtEnd() && !OutputRightIt.IsAtEnd()){
	
	//~ p = -RightPatchIt.Get();
	//~ OutputRightIt.Set(p);
//~ ++RightPatchIt;
//~ ++OutputRightIt;
//~ }
itk::ImageRegionIterator<TInputImage> LeftPatchInputIt ( this->GetLeftPatchInputImage(), outputRegionForThread );
ConstNeighborhoodIteratorType RightPatchInputIt ( this->GetRadius(), this->GetRightPatchInputImage(), outputRegionForThread);
   

// output Iterator
itk::ImageRegionIterator<TInputImage> OutputCostIt ( this->GetOutputCostImage(), outputRegionForThread );
itk::ImageRegionIterator<TInputImage> OutputPatchIt ( this->GetOutputPatchImage(), outputRegionForThread );
itk::ImageRegionIterator<TInputImage> OutputNormalIt ( this->GetOutputNormalAndZValueImage(), outputRegionForThread );



itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
 
SizeType ImageSize = this->GetLeftInputImage()->GetLargestPossibleRegion().GetSize();    
IndexType CostIndex = {{ 0,0 }};
OffsetType Offset0  = {{ 0,0 }};
OffsetType Offset1;

 
PixelType OutCost(1);
OutCost.Fill(0);

PixelType OutPatch(3);
OutPatch.Fill(0);


PixelType NormalAndZValuePixel(4);
NormalAndZValuePixel.Fill(0);

PixelType LeftAggCostPixel(3);
LeftAggCostPixel.Fill(0);
PixelType RightAggCostPixel(3);
RightAggCostPixel.Fill(0);

PixelType I(3);
I.Fill(0);

PixelType P(3);
P.Fill(0);

PixelType Out(3);
Out.Fill(0);

IndexType R_Index;
IndexType L_Index; 
OffsetType Offset_j;
RadiusType Wradius = this->GetRadius();


	 
if(m_Iteration % (2) == 1){	      
LeftPatchInputIt.GoToBegin();
RightPatchInputIt.GoToBegin();

OutputCostIt.GoToBegin();
OutputPatchIt.GoToBegin();
OutputNormalIt.GoToBegin();

while ( !LeftPatchInputIt.IsAtEnd() ){	
L_Index = LeftPatchInputIt.GetIndex();


 for(unsigned int i = 0; i< RightPatchInputIt.Size(); i++){	

		if(floor(RightPatchInputIt.GetPixel(i)[0]) == floor(LeftPatchInputIt.Get()[0]) &&
				floor(RightPatchInputIt.GetPixel(i)[1]) == floor(LeftPatchInputIt.Get()[1]) &&
				floor(RightPatchInputIt.GetPixel(i)[2]) == floor(LeftPatchInputIt.Get()[2])){		
			
			//std::cout<< "LeftAggCostPixel" << LeftAggCostPixel <<std::endl;	
			
			
			R_Index = RightPatchInputIt.GetIndex(i);
			Offset1[0] = L_Index[0]-R_Index[0];
			Offset1[1] = L_Index[1]-R_Index[1];
			
			// m(p, fp')
			  m_LeftCost->SetLeftInputImage(this->GetLeftInputImage());
			  m_LeftCost->SetRightInputImage(this->GetRightInputImage());  
			  m_LeftCost->SetLeftGradientXInput(this->GetLeftGradientXInput());
			  m_LeftCost->SetRightGradientXInput(this->GetRightGradientXInput());  
			  m_LeftCost->SetPatchInputImage(this->GetLeftPatchInputImage());			  
			  m_LeftCost->SetIndex(R_Index[0], R_Index[1]);  
			  m_LeftCost->SetOffsetPatch(Offset1,Offset1,Offset1);
			  m_LeftCost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
			  m_LeftCost->Update();
			  m_LeftCost->Modified();		  
		  
			  LeftAggCostPixel = m_LeftCost->GetOutput()->GetPixel(CostIndex);	

			// m(p, fp)		 
			  m_RightCost->SetLeftInputImage(this->GetRightInputImage());
			  m_RightCost->SetRightInputImage(this->GetLeftInputImage());  
			  m_RightCost->SetLeftGradientXInput(this->GetRightGradientXInput());
			  m_RightCost->SetRightGradientXInput(this->GetLeftGradientXInput());  
			  m_RightCost->SetPatchInputImage(this->GetRightPatchInputImage());		  
			  
			  m_RightCost->SetIndex(R_Index[0], R_Index[1]);  
			  m_RightCost->SetOffsetPatch(Offset0,Offset0,Offset0);
			  m_RightCost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
			  m_RightCost->Update();
			  m_RightCost->Modified();		  
		  
			  RightAggCostPixel = m_RightCost->GetOutput()->GetPixel(CostIndex);
			  
				//
				
				if(LeftAggCostPixel[0] < RightAggCostPixel[0]){
				OutCost[0]  = LeftAggCostPixel[0];
				OutPatch = this->GetLeftPatchInputImage()->GetPixel(L_Index);	
				NormalAndZValuePixel = this->GetLeftNormalAndZValueImage()->GetPixel(L_Index);
				//~ this->GetRightPatchInputImage()->SetPixel(R_Index,OutPatch);
				//~ this->GetRightNormalAndZValueImage()->SetPixel(R_Index,NormalAndZValuePixel);
				}
				
				else{
				OutCost[0]  = RightAggCostPixel[0];
				OutPatch = this->GetRightPatchInputImage()->GetPixel(R_Index);
				NormalAndZValuePixel = this->GetRightNormalAndZValueImage()->GetPixel(R_Index);						
				}
					 				 
		}//end of If between patchs
		else{
			OutCost  = this->GetSpatialCostImage()->GetPixel(L_Index);
			OutPatch = this->GetRightPatchInputImage()->GetPixel(L_Index);
			NormalAndZValuePixel = this->GetRightNormalAndZValueImage()->GetPixel(L_Index);						
			}

}

     
			
//std::cout<<"L_Index" << L_Index<< std::endl;
	
	OutputCostIt.Set(OutCost);
	OutputPatchIt.Set(OutPatch); //OutPatch
	OutputNormalIt.Set(NormalAndZValuePixel);
	
				
	
			++LeftPatchInputIt;			
			++RightPatchInputIt;
			
			++OutputCostIt;
			++OutputPatchIt;
			++OutputNormalIt;
					
			progress.CompletedPixel();
	 
}// end of while

} // end if 

/*
 * It is necessary to implement this part and browse
 *
 * 
 *  the image in the opposite direction with the good offset
 * 
 * 
 * */
else{	      
LeftPatchInputIt.GoToEnd();
RightPatchInputIt.GoToEnd();

OutputCostIt.GoToEnd();
OutputPatchIt.GoToEnd();
OutputNormalIt.GoToEnd();
  
//std::cout<< "m_Iteration view else" << m_Iteration <<std::endl;
while ( !LeftPatchInputIt.IsAtBegin()  ){
	
		--LeftPatchInputIt;
	    --RightPatchInputIt;
	
			
			--OutputCostIt;
			--OutputPatchIt;
			--OutputNormalIt;
		 L_Index = LeftPatchInputIt.GetIndex();

	
for(unsigned int i = 0; i< RightPatchInputIt.Size(); i++){

	if(floor(RightPatchInputIt.GetPixel(i)[0]) == floor(LeftPatchInputIt.Get()[0]) &&
				floor(RightPatchInputIt.GetPixel(i)[1]) == floor(LeftPatchInputIt.Get()[1]) &&
				floor(RightPatchInputIt.GetPixel(i)[2]) == floor(LeftPatchInputIt.Get()[2])){		
			
		
			R_Index = RightPatchInputIt.GetIndex(i);
			
			//	std::cout<< "R_Index =  " << R_Index <<std::endl;
				
			Offset1[0] = L_Index[0]-R_Index[0];
			Offset1[1] = L_Index[1]-R_Index[1];
			
			// m(p, fp')
			  m_LeftCost->SetLeftInputImage(this->GetLeftInputImage());
			  m_LeftCost->SetRightInputImage(this->GetRightInputImage());  
			  m_LeftCost->SetLeftGradientXInput(this->GetLeftGradientXInput());
			  m_LeftCost->SetRightGradientXInput(this->GetRightGradientXInput());  
			  m_LeftCost->SetPatchInputImage(this->GetLeftPatchInputImage());		  
			  
			  m_LeftCost->SetIndex(R_Index[0], R_Index[1]);  
			  m_LeftCost->SetOffsetPatch(Offset1,Offset1,Offset1);
			  m_LeftCost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
			  m_LeftCost->Update();
			  m_LeftCost->Modified();		  
		  
			  LeftAggCostPixel = m_LeftCost->GetOutput()->GetPixel(CostIndex);	
			  

			// m(p, fp)		 
			  m_RightCost->SetLeftInputImage(this->GetRightInputImage());
			  m_RightCost->SetRightInputImage(this->GetLeftInputImage());  
			  m_RightCost->SetLeftGradientXInput(this->GetRightGradientXInput());
			  m_RightCost->SetRightGradientXInput(this->GetLeftGradientXInput());  
			  m_RightCost->SetPatchInputImage(this->GetRightPatchInputImage());		  
			  
			  m_RightCost->SetIndex(R_Index[0], R_Index[1]);  
			  m_RightCost->SetOffsetPatch(Offset0,Offset0,Offset0);
			  m_RightCost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
			  m_RightCost->Update();
			  m_RightCost->Modified();		  
		  
			  RightAggCostPixel = m_RightCost->GetOutput()->GetPixel(CostIndex);
			  
				//
				
				if(LeftAggCostPixel[0] < RightAggCostPixel[0]){
				OutCost[0]  = LeftAggCostPixel[0];
				OutPatch = this->GetLeftPatchInputImage()->GetPixel(L_Index);	
				NormalAndZValuePixel = this->GetLeftNormalAndZValueImage()->GetPixel(L_Index);
				//~ this->GetRightPatchInputImage()->SetPixel(R_Index,OutPatch);
				//~ this->GetRightNormalAndZValueImage()->SetPixel(R_Index,NormalAndZValuePixel);
				}
				
				else{
				OutCost[0]  = RightAggCostPixel[0];
				OutPatch = this->GetRightPatchInputImage()->GetPixel(R_Index);
				NormalAndZValuePixel = this->GetRightNormalAndZValueImage()->GetPixel(R_Index);						
				}
					 				 
		}//end of If between patchs
		else{
			OutCost  = this->GetSpatialCostImage()->GetPixel(L_Index);
			OutPatch = this->GetLeftPatchInputImage()->GetPixel(L_Index);
			NormalAndZValuePixel = this->GetLeftNormalAndZValueImage()->GetPixel(L_Index);						
			}

}

	
	OutputCostIt.Set(OutCost);
	OutputPatchIt.Set(OutPatch);
	OutputNormalIt.Set(NormalAndZValuePixel);
	

				progress.CompletedPixel();
	 
}// end of while 2
} // end else 


} //end of threaded generate data


} // End namespace otb

#endif
