/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbDrawPathListFilter_txx
#define __otbDrawPathListFilter_txx

#include "otbDrawPathListFilter.h"

#include "itkContinuousIndex.h"
#include "itkVectorContainer.h"
#include "itkImage.h"
#include "itkImageIteratorWithIndex.h"
#include "itkImageConstIteratorWithIndex.h"
#include "itkImageIteratorWithIndex.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "otbMacro.h"
namespace otb
{

/**
 *
 */
template <class TInputImage, class TInputPath,class TOutputImage>
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::DrawPathListFilter()
{
  // Modify superclass default values, can be overridden by subclasses
//  this->SetNumberOfRequiredInputs(2);
//  this->SetNumberOfRequiredOutputs(1);
  m_PathValue = static_cast<OutputImagePixelType>(250.0); 
}

/**
 *
 */
template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::SetImageInput(const InputImageType *image)
{
  this->ProcessObjectType::SetNthInput(0, 
                                   const_cast< InputImageType * >( image ) );
}


template <class TInputImage, class TInputPath,class TOutputImage>
const typename DrawPathListFilter<TInputImage,TInputPath,TOutputImage>::InputImageType *
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::GetImageInput(void)
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  
  return static_cast<const InputImageType *>
    (this->ProcessObjectType::GetInput(0) );
}



template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::SetPathInput( InputPathListType *path)
{
//  this->ProcessObjectType::SetNthInput(1, 
//                                  const_cast< InputPathListType * >( path ) );

// Attention: A modifier  
  this->ProcessObjectType::SetNthInput(1,NULL);
    m_PathList = path;
}


template <class TInputImage, class TInputPath,class TOutputImage>
typename DrawPathListFilter<TInputImage,TInputPath,TOutputImage>::InputPathListType *
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::GetPathInput(void)
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  InputPathListType * ptr;
//  ptr = this->otb::PathListSource::GetOutput();
  ptr = this->GetOutput();
  
  return (ptr);

//  return static_cast<const TInputPath * >
//    (this->ProcessObjectType::GetInput(1) );
}



template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::SetImageOutput(OutputImageType *image)
{
  this->ProcessObjectType::SetNthOutput(0, 
                                   static_cast< OutputImageType * >( image ) );
}


template <class TInputImage, class TInputPath,class TOutputImage>
typename DrawPathListFilter<TInputImage,TInputPath,TOutputImage>::OutputImageType *
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::GetImageOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  
  return static_cast<OutputImageType * >
    (this->ProcessObjectType::GetOutput(0) );
}


template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::GenerateData(void)
{
  OutputImagePointerType      OutputImage  = OutputImageType::New();
  this->SetImageOutput(OutputImage);
  
  
  
  InputImageConstPointerType  InputImage   = this->GetImageInput();
//  InputPathListPointerType    InputPath    = this->GetPathInput();
  InputPathListPointerType    InputPath    = m_PathList;
//  OutputImagePointerType      OutputImage  = this->GetImageOutput();
  
  InputImageSizeType          Taille;
  OutputImageIndexType        IndexOut;
  typedef double                                        RealType; 
  typedef typename InputPathType::ContinuousIndexType  VertexType;
//  typedef itk::ContinuousIndex<RealType,2>             VertexType; 
  typedef itk::VectorContainer< unsigned,VertexType >  VertexListType;
  typedef typename VertexListType::ConstPointer        VertexListTypePointer;
  typedef itk::ImageRegionIteratorWithIndex< OutputImageType > OutputIteratorType; 
  typedef itk::ImageRegionConstIteratorWithIndex< InputImageType >  InputIteratorType; 
	
  VertexListTypePointer vertexList;
  VertexType            cindex;
  Taille = InputImage->GetLargestPossibleRegion().GetSize();
  
  
  typename OutputImageType::RegionType region;
  
  region.SetSize(InputImage->GetLargestPossibleRegion().GetSize());
  region.SetIndex(InputImage->GetLargestPossibleRegion().GetIndex());
  OutputImage->SetRegions( region );
  OutputImage->SetOrigin(InputImage->GetOrigin());
  OutputImage->SetSpacing(InputImage->GetSpacing());
  OutputImage->Allocate();
  
  OutputIteratorType    outputIt( OutputImage, OutputImage->GetRequestedRegion() );
  InputIteratorType     inputIt(  InputImage,  OutputImage->GetRequestedRegion() );

  outputIt.GoToBegin();
  inputIt.GoToBegin();

  for ( outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt,++inputIt)
    {
     outputIt.Set( inputIt.Get() );
    }
    
  int nbPath = InputPath->Size();
  
  otbMsgDevMacro( <<"nbPath: " <<nbPath);
 
  for(int i =0 ; i<nbPath ;i++)
     {
     vertexList = InputPath->GetNthElement(i)->GetVertexList();
     cindex = vertexList->GetElement(0);
     RealType x1 = cindex[0];
     RealType y1 = cindex[1];
     cindex = vertexList->GetElement(1);
     RealType x2 = cindex[0];
     RealType y2 = cindex[1];
     
     /*RealType DeltaX = std::abs(x1-x2);
     RealType DeltaY = std::abs(y1-y2);
     RealType Xmin   = std::min( x1 , x2 );
     RealType Xmax   = std::max( x1 , x2 );
     RealType Ymin   = std::min( y1 , y2 );
     RealType Ymax   = std::max( y1 , y2 );*/
     RealType DeltaX = fabs(x1-x2);
     RealType DeltaY = fabs(y1-y2);
     RealType Xmin   = x1<x2 ? x1 : x2;
     RealType Xmax   = x1>x2 ? x1 : x2;
     RealType Ymin   = y1<y2 ? y1 : y2;
     RealType Ymax   = y1>y2 ? y1 : y2;
     
     if(DeltaX>0 && DeltaY>0)
       {
       if(DeltaX>DeltaY)
         {
	 RealType Alpha = (y2-y1) / (x2-x1) ;
	 for(RealType j = Xmin; j<=Xmax;j++)
	    {
	    IndexOut[0] = static_cast<int>(j) ;
	    IndexOut[1] = static_cast<int>(Alpha * (j-x1) + y1) ;
	    if(IndexOut[0]>=0 && IndexOut[0]<(int)Taille[0] && 
	       IndexOut[1]>=0 && IndexOut[1]<(int)Taille[1])
	          OutputImage->SetPixel(IndexOut,m_PathValue);
	    }
	 }
         else
	 {
	 RealType Alpha = (x2-x1) / (y2-y1) ;
	 for(RealType j = Ymin; j<=Ymax;j++)
	    {
	    IndexOut[0] = static_cast<int>(Alpha * (j-y1) + x1) ;
	    IndexOut[1] = static_cast<int>(j);
	    if(IndexOut[0]>=0 && IndexOut[0]<(int)Taille[0] && 
	       IndexOut[1]>=0 && IndexOut[1]<(int)Taille[1])
	          OutputImage->SetPixel(IndexOut,m_PathValue);
	    }
	 
	 }
       }
       else
       {
       if(DeltaX>0)
         {
	 IndexOut[1] = static_cast<int>(Ymin); 
	 for(RealType j = Xmin; j<=Xmax;j++)
	   {
	   IndexOut[0]=static_cast<int>(j);
	   if(IndexOut[0]>=0 && IndexOut[0]<(int)Taille[0] && 
	      IndexOut[1]>=0 && IndexOut[1]<(int)Taille[1])
	          OutputImage->SetPixel(IndexOut,m_PathValue);
	  
	   }
	 }
	 else
	 {
	 IndexOut[0] = static_cast<int>(Xmin); 
	 for(RealType j = Ymin; j<=Ymax;j++)
	   {
	   IndexOut[1]=static_cast<int>(j);
	   if(IndexOut[0]>=0 && IndexOut[0]<(int)Taille[0] && 
	      IndexOut[1]>=0 && IndexOut[1]<(int)Taille[1])
	          OutputImage->SetPixel(IndexOut,m_PathValue);
	   }
	 }
       }         
     }


}

  
/**
 *
 */
template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace otb

#endif
 
