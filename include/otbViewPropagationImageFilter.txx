
#ifndef otbViewPropagationImageFilter_txx
#define otbViewPropagationImageFilter_txx


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
m_Radius[0] = 3;// il faut tjr le prendre >1, il définit la fenetre de recherche dans l'autre image
m_Radius[1] = 0;
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
const TInputImage *
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::GetLeftNormalAndZValueImage() const
{
if(this->GetNumberOfInputs()<7)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(6));
}

template <class TInputImage,  class TOutputImage >
const TInputImage *
ViewPropagationImageFilter<TInputImage,  TOutputImage >
::GetRightNormalAndZValueImage() const
{
if(this->GetNumberOfInputs()<8)
    {
    return ITK_NULLPTR;
    }
  return static_cast< const TInputImage *>(this->itk::ProcessObject::GetInput(7));
}

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

//input iterator 


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
double m(0);
 
PixelType OutCost(3);
OutCost.Fill(0);

PixelType OutPatch(3);
OutPatch.Fill(0);


PixelType NormalAndZValuePixel(4);
NormalAndZValuePixel.Fill(0);

PixelType LeftAggCostPixel(3);
LeftAggCostPixel.Fill(0);
PixelType RightAggCostPixel(3);
RightAggCostPixel.Fill(0);

IndexType R_Index;
IndexType L_Index; 

    

std::vector< std::pair<PixelType,double> > duo(9); 
	 
if(m_Iteration % (2) == 1){	      
LeftPatchInputIt.GoToBegin();
RightPatchInputIt.GoToBegin();

OutputCostIt.GoToBegin();
OutputPatchIt.GoToBegin();
OutputNormalIt.GoToBegin();

while ( !LeftPatchInputIt.IsAtEnd() ){	
L_Index = LeftPatchInputIt.GetIndex();

	
			// m(l_index, L_plane_L_index)
			  m_LeftCost->SetLeftInputImage(this->GetLeftInputImage());
			  m_LeftCost->SetRightInputImage(this->GetRightInputImage());  
			  m_LeftCost->SetLeftGradientXInput(this->GetLeftGradientXInput());
			  m_LeftCost->SetRightGradientXInput(this->GetRightGradientXInput());  
			  m_LeftCost->SetPatchInputImage(this->GetLeftPatchInputImage());		  
			  
			  m_LeftCost->SetIndex(L_Index[0], L_Index[1]);  
			  m_LeftCost->SetOffsetPatch(Offset0,Offset0,Offset0);
			  m_LeftCost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
			  m_LeftCost->Update();
			  m_LeftCost->Modified();		  
		  
			  LeftAggCostPixel = m_LeftCost->GetOutput()->GetPixel(CostIndex);	
			  m = LeftAggCostPixel[0] ;
		

 for(unsigned int i = 0; i< RightPatchInputIt.Size(); i++){			
	R_Index = RightPatchInputIt.GetIndex(i);
	//~ Offset_j = input.GetOffset(i); 	
			
				//~ I[0] = this->GetLeftInputImage()->GetPixel(L_Index)[1]- this->GetLeftInputImage()->GetPixel(R_Index)[1];
				//~ I[1] = this->GetLeftInputImage()->GetPixel(L_Index)[2]- this->GetLeftInputImage()->GetPixel(R_Index)[2];
				//~ I[2] = this->GetLeftInputImage()->GetPixel(L_Index)[3]- this->GetLeftInputImage()->GetPixel(R_Index)[3];
				
								
				//~ W =  std::exp( -(I.GetSquaredNorm())*m_gamma);
		
		
	

				
		if(RightPatchInputIt.GetPixel(i) == LeftPatchInputIt.Get()){  
						// m(l_index, R_plane_R_index)				 
	 
			  m_RightCost->SetLeftInputImage(this->GetLeftInputImage());
			  m_RightCost->SetRightInputImage(this->GetRightInputImage());  
			  m_RightCost->SetLeftGradientXInput(this->GetLeftGradientXInput());
			  m_RightCost->SetRightGradientXInput(this->GetRightGradientXInput());  
			  m_RightCost->SetPatchInputImage(this->GetRightPatchInputImage());		  
			  
			  m_RightCost->SetIndex(R_Index[0], R_Index[1]);  
			  m_RightCost->SetOffsetPatch(Offset0,Offset0,Offset0);
			  m_RightCost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
			 // m_RightCost->UpdateOutputInformation();
			  m_RightCost->Update();
			  m_RightCost->Modified();		  
		  
			  RightAggCostPixel = m_RightCost->GetOutput()->GetPixel(CostIndex);
				
			
			m = std::min(m,RightAggCostPixel[0]);
			OutPatch = this->GetRightPatchInputImage()->GetPixel(R_Index);	
			NormalAndZValuePixel = this->GetRightNormalAndZValueImage()->GetPixel(R_Index);				 				 
		}//end of If between patchs
		else{
			OutPatch = this->GetLeftPatchInputImage()->GetPixel(L_Index);
			NormalAndZValuePixel = this->GetRightNormalAndZValueImage()->GetPixel(L_Index);			
		}
		
	
		//~ duo[i].first = OutPatch;               
		//~ duo[i].second =  W; 
		//~ Wt += W	;	
}
	

//~ Wt = Wt/2;



   //~ for (std::vector<pairCord>::iterator it=duo.begin(); it !=duo.end(); ++it){
       		 //~ Wtemp += it->second ;
       		 //~ if (Wtemp >= Wt){
				 //~ P = it->first; 
				 //~ //std::cout << "Element found in myvector: " << d << '\n';
				 //~ output[0] = static_cast<typename TOutput::ValueType>(P) ;
				 //~ break;
		//~ }	 
     //~ }	
     
     
     	
		
OutCost[0] = m;
				
//std::cout<<"L_Index" << L_Index<< std::endl;
	
	OutputCostIt.Set(OutCost);
	OutputPatchIt.Set(OutPatch);
	OutputNormalIt.Set(NormalAndZValuePixel);
	
				
	
			++LeftPatchInputIt;			
			++RightPatchInputIt;
			
			++OutputCostIt;
			++OutputPatchIt;
			++OutputNormalIt;
					
			progress.CompletedPixel();
	 
}// end of while

} // end if 

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


			
			// m(l_index, L_plane_L_index)
			  m_LeftCost->SetLeftInputImage(this->GetLeftInputImage());
			  m_LeftCost->SetRightInputImage(this->GetRightInputImage());  
			  m_LeftCost->SetLeftGradientXInput(this->GetLeftGradientXInput());
			  m_LeftCost->SetRightGradientXInput(this->GetRightGradientXInput());  
			  m_LeftCost->SetPatchInputImage(this->GetLeftPatchInputImage());		  
			  
			  m_LeftCost->SetIndex(L_Index[0], L_Index[1]);  
			  m_LeftCost->SetOffsetPatch(Offset0,Offset0,Offset0);
			  m_LeftCost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
			  m_LeftCost->Update();
			  m_LeftCost->Modified();		  
		  
			  LeftAggCostPixel = m_LeftCost->GetOutput()->GetPixel(CostIndex);	
			  m = LeftAggCostPixel[0] ;
		

 for(unsigned int i = 0; i< RightPatchInputIt.Size(); i++){			
	R_Index = RightPatchInputIt.GetIndex(i);

			if(RightPatchInputIt.GetPixel(i) == LeftPatchInputIt.Get()){  
		
			 
			// m(l_index, R_plane_R_index)				 
	 
			  m_RightCost->SetLeftInputImage(this->GetLeftInputImage());
			  m_RightCost->SetRightInputImage(this->GetRightInputImage());  
			  m_RightCost->SetLeftGradientXInput(this->GetLeftGradientXInput());
			  m_RightCost->SetRightGradientXInput(this->GetRightGradientXInput());  
			  m_RightCost->SetPatchInputImage(this->GetRightPatchInputImage());		  
			  
			  m_RightCost->SetIndex(R_Index[0], R_Index[1]);  
			  m_RightCost->SetOffsetPatch(Offset0,Offset0,Offset0);
			  m_RightCost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
			  m_RightCost->Update();
			  m_RightCost->Modified();		  
		  
			  RightAggCostPixel = m_RightCost->GetOutput()->GetPixel(CostIndex);
				
			
			  m = std::min(m,RightAggCostPixel[0]);
			  OutPatch = this->GetRightPatchInputImage()->GetPixel(R_Index);	
			  NormalAndZValuePixel = this->GetRightNormalAndZValueImage()->GetPixel(R_Index);				 				 
		}//end of If between patchs
		else{
			OutPatch = this->GetLeftPatchInputImage()->GetPixel(L_Index);
			NormalAndZValuePixel = this->GetRightNormalAndZValueImage()->GetPixel(L_Index);			
		}
}			


OutCost[0] = m;
				
//std::cout<<"L_Index" << L_Index<< std::endl;
	
	OutputCostIt.Set(OutCost);
	OutputPatchIt.Set(OutPatch);
	OutputNormalIt.Set(NormalAndZValuePixel);
	

				progress.CompletedPixel();
	 
}// end of while 2
} // end if 


} //end of threaded generate data


} // End namespace otb

#endif
