
#ifndef otbCostVolumeVectorImageFilter_txx
#define otbCostVolumeVectorImageFilter_txx


#include "otbCostVolumeVectorImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConstNeighborhoodIterator.h"


#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

#include "otbImageFileWriter.h"
#include "otbVectorImage.h"



namespace otb
{	
	
template <class TInputImage, class TGradientImage, class TOutputImage >
CostVolumeVectorImageFilter<TInputImage,TGradientImage,TOutputImage>
::CostVolumeVectorImageFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(4);  

  // Set the outputs
  this->SetNumberOfRequiredOutputs(0);
  this->SetNthOutput(0,TOutputImage::New());  
	
  m_VerticalDisparity = 0;

 // this->SetNumberOfThreads(1);
}

template <class TInputImage, class TGradientImage, class TOutputImage >
CostVolumeVectorImageFilter<TInputImage,TGradientImage,TOutputImage>
::~CostVolumeVectorImageFilter()
{}
// =================================== Seters ========================================================

template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::SetLeftInputImage(const TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::SetRightInputImage(const TInputImage * image)
{

  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::SetLeftGradientXInput(const TGradientImage * image)
{

  this->SetNthInput(2, const_cast<TGradientImage *>( image ));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::SetRightGradientXInput(const TGradientImage * image)
{

  this->SetNthInput(3, const_cast<TGradientImage *>( image ));
  
}



template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::SetLeftGradientYInput(const TGradientImage * image)
{

  this->SetNthInput(4, const_cast<TGradientImage *>( image ));
}


template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::SetRightGradientYInput(const TGradientImage * image)
{

  this->SetNthInput(5, const_cast<TGradientImage *>( image ));

}



//================================== Geters ====================================================
template <class TInputImage, class TGradientImage, class TOutputImage >
const TInputImage *
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GetLeftInputImage() const
{
  if (this->GetNumberOfInputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
const TInputImage *
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GetRightInputImage() const
{
  if(this->GetNumberOfInputs()<2)
    {
    return ITK_NULLPTR;
    }

  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
const TGradientImage *
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GetLeftGradientXInput() const
{
  if(this->GetNumberOfInputs()<3)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TGradientImage *>(this->itk::ProcessObject::GetInput(2));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
const TGradientImage *
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GetRightGradientXInput() const
{
  if(this->GetNumberOfInputs()<4)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TGradientImage *>(this->itk::ProcessObject::GetInput(3));
}


template <class TInputImage, class TGradientImage, class TOutputImage >
const TGradientImage *
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GetLeftGradientYInput() const
{
  if(this->GetNumberOfInputs()<5)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TGradientImage *>(this->itk::ProcessObject::GetInput(4));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
const TGradientImage *
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GetRightGradientYInput() const
{
  if(this->GetNumberOfInputs()<6)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TGradientImage *>(this->itk::ProcessObject::GetInput(5));
}


template <class TInputImage, class TGradientImage, class TOutputImage >
TOutputImage *
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GetOutputImage()
{
  if (this->GetNumberOfOutputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TOutputImage *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
int
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GetHorizontalMinDisparity() const
     {
       return m_HorizontalMinDisparity;
    }
    
template <class TInputImage, class TGradientImage, class TOutputImage >
int
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GetHorizontalMaxDisparity() const
     {
       return m_HorizontalMaxDisparity;
    }


    
//============================================  GenerateOutputInformation  ========================================================
template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();
  
  this->GetOutput()->SetNumberOfComponentsPerPixel(m_HorizontalMaxDisparity - m_HorizontalMinDisparity +1); 
 
}
//============================================  GenerateInputRequestedRegion  ========================================================
template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::GenerateInputRequestedRegion()
{
 Superclass::GenerateInputRequestedRegion();	
 
// 
typename TInputImage::Pointer  inLeftPtr = const_cast<InputImageType *>(this->GetLeftInputImage());
typename TInputImage::Pointer  inRightPtr = const_cast<InputImageType *>(this->GetRightInputImage());
typename TInputImage::Pointer  inLeftGradientXPtr = const_cast<InputImageType *>(this->GetLeftGradientXInput());
typename TInputImage::Pointer  inRightGradientXPtr = const_cast<InputImageType *>(this->GetRightGradientXInput());

//~ typename TInputImage::Pointer  inLeftGradientYPtr = const_cast<InputImageType *>(this->GetLeftGradientYInput());
//~ typename TInputImage::Pointer  inRightGradientYPtr = const_cast<InputImageType *>(this->GetRightGradientYInput());

typename TOutputImage::ConstPointer outputPtr = this->GetOutputImage();
				
//Non-null Pointer Verification           
if (!inLeftPtr || !inRightPtr  || !inLeftGradientXPtr  || !inRightGradientXPtr /*|| !inLeftGradientYPtr  || !inRightGradientYPtr */||!outputPtr )
    {
    return;
    } 
 
RegionType outputRequestedRegion = outputPtr->GetRequestedRegion()  ;
RegionType inputRightRequestedRegion;
RegionType inputLeftRequestedRegion;



for(int iteration_disp=m_HorizontalMinDisparity; iteration_disp<=m_HorizontalMaxDisparity; iteration_disp++)
  {
  ComputeInputRegions( outputRequestedRegion, inputLeftRequestedRegion, inputRightRequestedRegion, iteration_disp);      
     
  // The calculation of the non-shifted inputs
  if(inLeftPtr)
    {
    inLeftPtr->SetRequestedRegion( outputRequestedRegion);
    }
  if(inLeftGradientXPtr)
    {
     inLeftGradientXPtr->SetRequestedRegion( outputRequestedRegion);
    }
  // The calculation of the shifted inputs

  inRightPtr->SetRequestedRegion(inputRightRequestedRegion);
  inRightGradientXPtr->SetRequestedRegion( inputRightRequestedRegion );
  }   
    
} // end of GenerateInputRequestedRegion

//============================================  ThreadedGenerateData  ========================================================


template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{	

 RegionType LeftRegionForThread;
 RegionType RightRegionForThread; 

 typename TOutputImage::PixelType OutPixel(1);
 OutPixel.Fill(0);

 
 //  Setting parameters
	double alpha = 0.9; 
	double taux1 = 7;  
	double taux2 = 2; 


typedef otb::VectorImage<float> FloatVectorImageType;
typedef otb::ImageFileWriter<FloatVectorImageType> ImageWriterType;
ImageWriterType::Pointer img = ImageWriterType::New();


for(int iteration_disp = m_HorizontalMinDisparity; iteration_disp<=m_HorizontalMaxDisparity; iteration_disp++)
  {
  ComputeInputRegions( outputRegionForThread, LeftRegionForThread, RightRegionForThread, iteration_disp); 


   //créer l'intérator     
         
   itk::ImageRegionConstIterator<TInputImage> LeftInputImageIt ( this->GetLeftInputImage(), LeftRegionForThread );
   LeftInputImageIt.GoToBegin(); 
   itk::ImageRegionConstIterator<TInputImage> RightInputImageIt ( this->GetRightInputImage(), RightRegionForThread );
   RightInputImageIt.GoToBegin();
   itk::ImageRegionConstIterator<TInputImage> LeftGradientXInputIt ( this->GetLeftGradientXInput(), LeftRegionForThread );
   LeftGradientXInputIt.GoToBegin();
   itk::ImageRegionConstIterator<TInputImage> RightGradientXInputIt ( this->GetRightGradientXInput(), RightRegionForThread );
   RightGradientXInputIt.GoToBegin();   
            
   itk::ImageRegionIterator<TOutputImage> outputIt ( this->GetOutputImage(), LeftRegionForThread );
   outputIt.GoToBegin();           
   // test 
    //~ if (threadId == 0){
       //~ std::cout<< "outputRegionForThread" <<  outputRegionForThread<< std::endl;    
   //~ std::cout<< "LeftRegionForThread" <<  LeftRegionForThread<< std::endl; 
   //~ std::cout<< "RightRegionForThread" <<  RightRegionForThread<< std::endl; 
   //~ } 
          
   //  Cost computation    

       
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  while ( !outputIt.IsAtEnd() && !LeftInputImageIt.IsAtEnd() )
    { 
    PixelType costColor;
    costColor.Fill(0);
    PixelType costGradient;
    costGradient.Fill(0);
    
    double costColorNorm;
    double costGradientNorm;
     
    costColor = LeftInputImageIt.Get() - RightInputImageIt.Get() ;        
    //b=  costColor.GetSquaredNorm ();
    costColorNorm=  costColor.GetNorm ();                                       
             
               if(costColorNorm > taux1) 
                  costColorNorm = taux1;
                    // if  To take the minimum   
               
    costGradient = LeftGradientXInputIt.Get() - RightGradientXInputIt.Get();
    costGradientNorm= costGradient.GetNorm () ;               
                 
                 if(costGradientNorm > taux2) 
                   costGradientNorm = taux2;
                   // if To take the minimum     
    //std::cout << "costColor : " << costColor << std::endl ;
    //std::cout << "costGradient : " << costGradient << std::endl;                        
                                                           
           
    OutPixel[0] = static_cast<typename TOutputImage::InternalPixelType>(((1-alpha)*costColorNorm + alpha*costGradientNorm) );  
    outputIt.Get()[abs(m_HorizontalMaxDisparity-iteration_disp)] = OutPixel[0] ;

    //outputIt.Get()[abs(m_HorizontalMinDisparity-iteration_disp)]=OutPixel[0] ;
     //  outputIt.Set(OutPixel);
     //  std::cout << " cost volume = "<< OutPixel[0] ;              
    ++LeftInputImageIt;
    ++RightInputImageIt;
    ++LeftGradientXInputIt;
    ++RightGradientXInputIt;   
    ++outputIt;       
    progress.CompletedPixel();
    }  // end of while 


  }

  
  
 
} //end of threaded generate data


// ================================== ComputeInputRegions ======================================================= 
template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeVectorImageFilter<TInputImage, TGradientImage, TOutputImage >
::ComputeInputRegions(const RegionType& outputRegion, RegionType& LeftRegion,RegionType& RightRegion, int iteration_disp)
{
//   

typename TInputImage::Pointer  inLeftPtr = const_cast<InputImageType *>(this->GetLeftInputImage());
typename TInputImage::Pointer  inRightPtr = const_cast<InputImageType *>(this->GetRightInputImage());


//	Regions  

 LeftRegion = outputRegion;
 RightRegion = 	outputRegion;

typename RegionType::IndexType RighttRequestedRegionIndex = outputRegion.GetIndex();	
typename RegionType::IndexType  shift;
  
 
  shift[0] = iteration_disp;
  shift[1] = m_VerticalDisparity;   
  
  RighttRequestedRegionIndex[0]+= shift[0];
  RighttRequestedRegionIndex[1]+= shift[1];

  
  RightRegion.SetIndex(RighttRequestedRegionIndex);
	
  
   
// Crop  inputRightRegion
  if ( RightRegion.Crop(inRightPtr->GetLargestPossibleRegion()))
  {
	
  }
  else
  {           
  // Build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) (je suis dans le cost)outside the largest possible region.");
    e.SetDataObject(inRightPtr);   
    throw e;
   } 
 
   
    	
// shift -1
typename RegionType::IndexType IndexInv = RightRegion.GetIndex();	
typename RegionType::IndexType  ShiftInv;


  ShiftInv[0] = -iteration_disp;
  ShiftInv[1] =  -m_VerticalDisparity;   
  
  IndexInv[0]+=ShiftInv[0];
  IndexInv[1]+=ShiftInv[1];
  
LeftRegion.SetIndex(IndexInv);	



LeftRegion.SetSize(RightRegion.GetSize());		
	
} // End of ComputeInputRegions


} // End namespace otb

#endif
