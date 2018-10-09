/*
 * Copyright (C) 2017-2018 CS Systemes d'Information (CS SI)
 * Copyright (C) 2017 Damia Belazouz

 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef otbCostVolumeFilter_hxx
#define otbCostVolumeFilter_hxx

#include "otbCostVolumeFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConstNeighborhoodIterator.h"

#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "otbVectorImage.h"


namespace otb
{   
template <class TInputImage, class TGradientImage, class TOutputImage >
CostVolumeFilter<TInputImage,TGradientImage,TOutputImage>
::CostVolumeFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(4); 
  // Set the outputs
  this->SetNumberOfRequiredOutputs(0);
  this->SetNthOutput(0,TOutputImage::New());    
  m_VerticalDisparity = 0;
}

template <class TInputImage, class TGradientImage, class TOutputImage >
CostVolumeFilter<TInputImage,TGradientImage,TOutputImage>
::~CostVolumeFilter()
{}
// =================================== Seters ========================================================

template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::SetLeftInputImage(const TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::SetRightInputImage(const TInputImage * image)
{

  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::SetLeftGradientXInput(const TGradientImage * image)
{

  this->SetNthInput(2, const_cast<TGradientImage *>( image ));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::SetRightGradientXInput(const TGradientImage * image)
{

  this->SetNthInput(3, const_cast<TGradientImage *>( image ));
  
}


template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::SetLeftGradientYInput(const TGradientImage * image)
{
  this->SetNthInput(4, const_cast<TGradientImage *>( image ));
}

template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::SetRightGradientYInput(const TGradientImage * image)
{
  this->SetNthInput(5, const_cast<TGradientImage *>( image ));
}


//================================== Geters ====================================================
template <class TInputImage, class TGradientImage, class TOutputImage >
const TInputImage *
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
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
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
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
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
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
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
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
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
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
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
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
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
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
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::GetHorizontalMinDisparity() const
     {
       return m_HorizontalMinDisparity;
    }
    
template <class TInputImage, class TGradientImage, class TOutputImage >
int
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::GetHorizontalMaxDisparity() const
     {
       return m_HorizontalMaxDisparity;
    }

template <class TInputImage, class TGradientImage, class TOutputImage >
float
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::GetAlpha() const
    {
    return m_Alpha;
    }

template <class TInputImage, class TGradientImage, class TOutputImage >
float
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::GetTau1() const
    {
    return m_Tau1;
    }

template <class TInputImage, class TGradientImage, class TOutputImage >
float
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::GetTau2() const
    {
    return m_Tau2;
    }

template <class TInputImage, class TGradientImage, class TOutputImage >
char
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::GetSide() 
    {
    return m_Side;
    }

 //============================================  GenerateOutputInformation  ========================================================
template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::GenerateOutputInformation()
  {
  // Call superclass implementation
  Superclass::GenerateOutputInformation();  
  this->GetOutput()->SetNumberOfComponentsPerPixel(m_HorizontalMaxDisparity - m_HorizontalMinDisparity +1 );  
  }

//============================================  GenerateInputRequestedRegion  ========================================================
template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::GenerateInputRequestedRegion()
{
Superclass::GenerateInputRequestedRegion();  
 
typename TInputImage::Pointer  inLeftPtr = const_cast<InputImageType *>(this->GetLeftInputImage());
typename TInputImage::Pointer  inRightPtr = const_cast<InputImageType *>(this->GetRightInputImage());
typename TGradientImage::Pointer  inLeftGradientXPtr = const_cast<TGradientImage *>(this->GetLeftGradientXInput());
typename TGradientImage::Pointer  inRightGradientXPtr = const_cast<TGradientImage *>(this->GetRightGradientXInput());

typename TOutputImage::ConstPointer outputPtr = this->GetOutputImage();
        
//Non-null Pointer Verification           
if (!inLeftPtr || !inRightPtr  || !inLeftGradientXPtr  || !inRightGradientXPtr ||!outputPtr )
    {
    return;
    }  
RegionType outputRequestedRegion = outputPtr->GetRequestedRegion()  ;
RegionType inputRightRequestedRegion;
RegionType inputLeftRequestedRegion;
ComputeInputRegions( outputRequestedRegion, inputLeftRequestedRegion, inputRightRequestedRegion, 0);    
// The calculation of the non-shifted inputs
inLeftPtr->SetRequestedRegion( outputRequestedRegion);
inLeftGradientXPtr->SetRequestedRegion( outputRequestedRegion);
inRightPtr->SetRequestedRegion(inputRightRequestedRegion);
inRightGradientXPtr->SetRequestedRegion( inputRightRequestedRegion );
  
    
} // end of GenerateInputRequestedRegion

//============================================  ThreadedGenerateData  ========================================================



template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{ 
  
RegionType LeftRegionForThread;
RegionType RightRegionForThread; 
typename TOutputImage::PixelType OutPixel(1); 

int bandNumber = this->GetLeftInputImage()->GetNumberOfComponentsPerPixel() ;

for(int iteration_disp = m_HorizontalMinDisparity; iteration_disp<=m_HorizontalMaxDisparity; iteration_disp++)
  {
  ComputeInputRegions( outputRegionForThread, LeftRegionForThread, RightRegionForThread, iteration_disp); 
          
  itk::ImageRegionConstIterator<TInputImage> LeftInputImageIt ( this->GetLeftInputImage(), LeftRegionForThread );
  LeftInputImageIt.GoToBegin(); 
  itk::ImageRegionConstIterator<TInputImage> RightInputImageIt ( this->GetRightInputImage(), RightRegionForThread );
  RightInputImageIt.GoToBegin();
  itk::ImageRegionConstIterator<TGradientImage> LeftGradientXInputIt ( this->GetLeftGradientXInput(), LeftRegionForThread );
  LeftGradientXInputIt.GoToBegin();
  itk::ImageRegionConstIterator<TGradientImage> RightGradientXInputIt ( this->GetRightGradientXInput(), RightRegionForThread );
  RightGradientXInputIt.GoToBegin();   
          
  itk::ImageRegionIterator<TOutputImage> outputIt ( this->GetOutputImage(), LeftRegionForThread );
  outputIt.GoToBegin();   
          
  //  Cost computation    
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
    

  while ( !outputIt.IsAtEnd() && !LeftInputImageIt.IsAtEnd()  )
    { 
    
    PixelType costColor;
    costColor.Fill(0);
    PixelType costGradient;
    costGradient.Fill(0);    
    double costColorNorm;
    double costGradientNorm;
     
    costColor = LeftInputImageIt.Get() - RightInputImageIt.Get() ;       
    costColorNorm = (costColor.GetNorm())/bandNumber; 
    
    if(costColorNorm > m_Tau1)   
      {
      costColorNorm = m_Tau1;
      } // if  To take the minimum  
               
    costGradient = LeftGradientXInputIt.Get() - RightGradientXInputIt.Get();
    costGradientNorm= (costGradient.GetNorm());             
    
    if(costGradientNorm > m_Tau2 )
      {
      costGradientNorm = m_Tau2; 
      }// if To take the minimum     

    if((outputIt.GetIndex()[0] < abs(m_HorizontalMinDisparity)) || (outputIt.GetIndex()[0] > outputRegionForThread.GetSize()[0]-abs(m_HorizontalMaxDisparity)) )
      { 
      OutPixel.Fill(0);
      }
    else
      {
      // OutPixel.Fill(7);
      OutPixel[0] = static_cast<typename TOutputImage::InternalPixelType>( (1-m_Alpha)*costColorNorm + m_Alpha*costGradientNorm ); 
      }


    if(m_Side=='r')
      {    
      outputIt.Get()[abs(m_HorizontalMinDisparity-iteration_disp)] = OutPixel[0] ;  
      }   
    else
      {     
      outputIt.Get()[abs(m_HorizontalMaxDisparity-iteration_disp)] = OutPixel[0] ;  
      } 


    ++LeftInputImageIt;
    ++RightInputImageIt;
    ++LeftGradientXInputIt;
    ++RightGradientXInputIt;   
    ++outputIt;       
    progress.CompletedPixel();
    }  // end of while 
  }
// }
} //end of threaded generate data


// ================================== ComputeInputRegions ======================================================= 
template <class TInputImage, class TGradientImage, class TOutputImage >
void
CostVolumeFilter<TInputImage, TGradientImage, TOutputImage >
::ComputeInputRegions(const RegionType& outputRegion, RegionType& LeftRegion,RegionType& RightRegion, int iteration_disp)
{
typename TInputImage::Pointer  inLeftPtr = const_cast<InputImageType *>(this->GetLeftInputImage());
typename TInputImage::Pointer  inRightPtr = const_cast<InputImageType *>(this->GetRightInputImage());
//  Regions  
LeftRegion = outputRegion;
RightRegion =  outputRegion;
typename RegionType::IndexType RighttRequestedRegionIndex = outputRegion.GetIndex();  
typename RegionType::IndexType  shift;

shift[0] = iteration_disp;
shift[1] = m_VerticalDisparity;   

RighttRequestedRegionIndex[0]+= shift[0];
RighttRequestedRegionIndex[1]+= shift[1];  
RightRegion.SetIndex(RighttRequestedRegionIndex);
     
// Crop  inputRightRegion
if( RightRegion.Crop(inRightPtr->GetLargestPossibleRegion()))
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
ShiftInv[1] = -m_VerticalDisparity;     
IndexInv[0]+=ShiftInv[0];
IndexInv[1]+=ShiftInv[1];  
LeftRegion.SetIndex(IndexInv);  
LeftRegion.SetSize(RightRegion.GetSize());      
} // End of ComputeInputRegions


} // End namespace otb

#endif
