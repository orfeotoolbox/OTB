

#ifndef otbMinPlaneRefinementImageFilter_txx
#define otbMinPlaneRefinementImageFilter_txx


#include "otbMinPlaneRefinementImageFilter.h"
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
MinPlaneRefinementImageFilter<TInputImage, TOutputImage>
::MinPlaneRefinementImageFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(8);  

  // Set the outputs
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0,TOutputImage::New()); 
  this->SetNthOutput(1,TOutputImage::New());

  this->SetNumberOfThreads(1); 

m_Cost = AggregatedCostType::New();	
m_Radius.Fill(1);

}

template <class TInputImage,  class TOutputImage >
MinPlaneRefinementImageFilter<TInputImage,TOutputImage>
::~MinPlaneRefinementImageFilter()
{}
// =================================== Seters ========================================================

template <class TInputImage,  class TOutputImage >
void
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::SetLeftInputImage(const TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::SetRightInputImage(const TInputImage * image)
{
 
  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}


template <class TInputImage,  class TOutputImage >
void
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::SetLeftGradientXInput(const TInputImage * image)
{

  this->SetNthInput(2, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::SetRightGradientXInput(const TInputImage * image)
{

  this->SetNthInput(3, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::SetPatchInputImage(TInputImage * image)
{

  this->SetNthInput(4, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::SetNormalAndZValueImage(TInputImage * image)
{

  this->SetNthInput(5, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::SetCostInputImage(TInputImage * image)
{

  this->SetNthInput(6, const_cast<TInputImage *>( image ));
}


template <class TInputImage,  class TOutputImage >
void
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::SetCoefRefinedInputImage(TInputImage * image)
{

  this->SetNthInput(7, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::SetRefinedNormalAndZValueImage(TInputImage * image)
{

  this->SetNthInput(8, const_cast<TInputImage *>( image ));
}

//================================== Geters ====================================================
template <class TInputImage, class TOutputImage >
const TInputImage *
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
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
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
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
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
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
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
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
MinPlaneRefinementImageFilter<TInputImage, TOutputImage >
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
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::GetNormalAndZValueImage() const
{
if(this->GetNumberOfInputs()<6)
    {
    return ITK_NULLPTR;
    }
  return static_cast< const TInputImage *>(this->itk::ProcessObject::GetInput(5));
}


template <class TInputImage,  class TOutputImage >
const TInputImage *
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::GetCostInputImage() const
{
if(this->GetNumberOfInputs()<7)
    {
    return ITK_NULLPTR;
    }
  return static_cast< const TInputImage *>(this->itk::ProcessObject::GetInput(6));
}



template <class TInputImage,  class TOutputImage >
TInputImage *
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::GetCoefRefinedInputImage() 
{
if(this->GetNumberOfInputs()<8)
    {
    return ITK_NULLPTR;
    }
  return static_cast<  TInputImage *>(this->itk::ProcessObject::GetInput(7));
}

template <class TInputImage,  class TOutputImage >
const TInputImage *
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::GetRefinedNormalAndZValueImage() const
{
if(this->GetNumberOfInputs()<9)
    {
    return ITK_NULLPTR;
    }
  return static_cast< const TInputImage *>(this->itk::ProcessObject::GetInput(8));
}

/// Get Output


template <class TInputImage,  class TOutputImage >
TOutputImage *
MinPlaneRefinementImageFilter<TInputImage, TOutputImage >
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
MinPlaneRefinementImageFilter<TInputImage, TOutputImage >
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
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
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
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::GenerateInputRequestedRegion()
{
 Superclass::GenerateInputRequestedRegion();

// get pointers to the input
typename TInputImage::Pointer  inLeftPtr = const_cast<InputImageType *>(this->GetLeftInputImage());
typename TInputImage::Pointer  inRightPtr = const_cast<InputImageType *>(this->GetRightInputImage());
typename TInputImage::Pointer  inLeftGradientXPtr = const_cast<InputImageType *>(this->GetLeftGradientXInput());
typename TInputImage::Pointer  inRightGradientXPtr = const_cast<InputImageType *>(this->GetRightGradientXInput());
typename TInputImage::Pointer  inPatchPtr = const_cast<InputImageType *>(this->GetPatchInputImage());

typename TInputImage::Pointer  inCoefRefinedPtr = const_cast<InputImageType *>(this->GetCoefRefinedInputImage());
typename TInputImage::Pointer  inNormalRefinedPtr = const_cast<InputImageType *>(this->GetRefinedNormalAndZValueImage());
typename TInputImage::Pointer  inNormalPtr = const_cast<InputImageType *>(this->GetNormalAndZValueImage());

// Get Poiters to the output
typename TOutputImage::ConstPointer outputPtrPlane = this->GetOutputPatchImage();
typename TOutputImage::ConstPointer outputPtrNormal = this->GetOutputNormalAndZValueImage();
				
//Vérification de pointeur non null            
if (!inLeftPtr || !inRightPtr  || !inLeftGradientXPtr  || !inRightGradientXPtr  
|| !inPatchPtr || !inCoefRefinedPtr || !inNormalRefinedPtr || !inNormalPtr 
|| !outputPtrPlane || !outputPtrNormal)
    {
    return;
    } 
 
  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inCoefRefinedPtr->GetRequestedRegion();
    SizeType Re;
Re[0] = m_PatchSize[0] +5;
Re[1] = m_PatchSize[1] +5;
  // pad the input requested region by the plane Size
  inputRequestedRegion.PadByRadius(Re);
  otbMsgDevMacro(<< "Padding by " << Re);
  otbMsgDevMacro(<< "Region is now " << inputRequestedRegion.GetIndex() << ", " << inputRequestedRegion.GetSize());

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inCoefRefinedPtr->GetLargestPossibleRegion()))
    {
    inCoefRefinedPtr->SetRequestedRegion(inputRequestedRegion);
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inCoefRefinedPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (in spatialPro filter)(at least partially) outside the largest possible region.");
    e.SetDataObject(inCoefRefinedPtr);
    throw e;
    }


} // end of GenerateInputRequestedRegion

//============================================  ThreadedGenerateData  ========================================================
template <class TInputImage,  class TOutputImage >
void
MinPlaneRefinementImageFilter<TInputImage,  TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId){

//std::cout<<"outputRegionForThread =  " << outputRegionForThread <<std::endl;
//input iterator 
ConstNeighborhoodIteratorType InputImageIt ( this->GetRadius(), this->GetLeftInputImage(), outputRegionForThread );
InputImageIt.GoToBegin();

itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

SizeType ImageSize = this->GetLeftInputImage()->GetLargestPossibleRegion().GetSize();

IndexType CostIndex =  {{0,0}};	

PixelType OutCost(1);
OutCost.Fill(10);
				 
PixelType OutPatch(3);
OutPatch.Fill(10);

PixelType NormalAndZValuePixel(4);
NormalAndZValuePixel.Fill(0);

PixelType NewAggCost(3);
NewAggCost.Fill(0);

PixelType OldAggCost(3);
OldAggCost.Fill(0);

//std::cin.get();


while(!InputImageIt.IsAtEnd() ){
				
	
IndexType InputIndex = InputImageIt.GetIndex();
	//~ std::cout<<"InputIndex =  " << InputIndex <<std::endl;

	

		  m_Cost->SetLeftInputImage(this->GetLeftInputImage());
		  m_Cost->SetRightInputImage(this->GetRightInputImage());  
		  m_Cost->SetLeftGradientXInput(this->GetLeftGradientXInput());
		  m_Cost->SetRightGradientXInput(this->GetRightGradientXInput());  
		  m_Cost->SetPatchInputImage(this->GetCoefRefinedInputImage());		  
		  
		  m_Cost->SetIndex(InputIndex[0], InputIndex[1]);  
		  m_Cost->SetOffsetPatch(m_Offset0,m_Offset0,m_Offset0);
		  m_Cost->SetPatchSize(m_PatchSize[0],m_PatchSize[0]);
          m_Cost->Update();
		  m_Cost->Modified();		  
		  
	NewAggCost = m_Cost->GetOutput()->GetPixel(CostIndex);	
	OldAggCost = this->GetCostInputImage()->GetPixel(InputIndex);



	if(NewAggCost[0] <  OldAggCost[0] ){
	
		OutCost[0]  = NewAggCost[0];
		OutPatch = this->GetCoefRefinedInputImage()->GetPixel(InputIndex);
		NormalAndZValuePixel = this->GetRefinedNormalAndZValueImage()->GetPixel(InputIndex);
		
	 }
	else{

		OutCost[0]  = OldAggCost[0];
		OutPatch = this->GetPatchInputImage()->GetPixel(InputIndex);
		NormalAndZValuePixel = this->GetNormalAndZValueImage()->GetPixel(InputIndex);
	  } 									
		

				
			this->GetOutputNormalAndZValueImage()->SetPixel(InputIndex, NormalAndZValuePixel);				
		
			this->GetOutputPatchImage()->SetPixel(InputIndex, OutPatch);

++InputImageIt;
progress.CompletedPixel();

}  // end of while 


} //end of threaded generate data


} // End namespace otb

#endif
