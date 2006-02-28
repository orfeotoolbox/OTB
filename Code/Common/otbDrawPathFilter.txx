/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   28 fevrier 2006
  Version   :   
  Role      :   Classe de base d'une PathList 
  $Id:$

=========================================================================*/

#ifndef __otbDrawPathFilter_txx
#define __otbDrawPathFilter_txx

#include "otbDrawPathFilter.h"
#include "itkImageIteratorWithIndex.h"
#include "itkImageConstIteratorWithIndex.h"
#include "itkImageIteratorWithIndex.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TInputPath,class TOutputImage>
DrawPathFilter<TInputImage,TInputPath,TOutputImage>
::DrawPathFilter()
{
  m_PathValue = static_cast<OutputImagePixelType>(15.0);
}


template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathFilter<TInputImage,TInputPath,TOutputImage>
::GenerateData(void)
{  
  typedef typename InputPathType::ContinuousIndexType               VertexType;
  typedef itk::VectorContainer< unsigned,VertexType >               VertexListType;
  typedef typename VertexListType::ConstPointer                     VertexListTypePointer;
  typedef float                                                     RealType;
  
  typedef itk::ImageRegionIteratorWithIndex< OutputImageType > OutputIteratorType; 
  typedef itk::ImageRegionConstIteratorWithIndex< InputImageType >  InputIteratorType; 

  InputImageConstPointer               InputImage   = this->GetImageInput();
  InputPathConstPointer                InputPath    = this->GetPathInput();
  OutputImagePointer                   OutputImage  = this->GetOutput();

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

  typename InputImageType::SizeType    Taille;
  typename OutputImageType::IndexType  IndexOut;
  VertexListTypePointer                vertexList;
  VertexType                           cindex;
  int                                  nbPath;
  
  Taille = InputImage->GetLargestPossibleRegion().GetSize();
  vertexList = InputPath->GetVertexList();
  nbPath = vertexList->Size();
    
  if(nbPath >1)
   {
   for(int i =0 ; i<nbPath-1 ;i++)
     {
     cindex = vertexList->GetElement(i);
     RealType x1 = cindex[0];
     RealType y1 = cindex[1];
     cindex = vertexList->GetElement(i+1);
     RealType x2 = cindex[0];
     RealType y2 = cindex[1];
     
     RealType DeltaX = std::abs(x1-x2);
     RealType DeltaY = std::abs(y1-y2);
     RealType Xmin   = std::min( x1 , x2 );
     RealType Xmax   = std::max( x1 , x2 );
     RealType Ymin   = std::min( y1 , y2 );
     RealType Ymax   = std::max( y1 , y2 );
          
     if(DeltaX>0 && DeltaY>0)
       {
       if(DeltaX>DeltaY)
         {
	 RealType Alpha = (y2-y1) / (x2-x1) ;
	 for(RealType j = Xmin; j<=Xmax;j++)
	    {
	    IndexOut[0] = static_cast<int>(j) ;
	    IndexOut[1] = static_cast<int>(Alpha * (j-x1) + y1) ;
	    if(IndexOut[0]>=0 && IndexOut[0]<Taille[0] && 
	       IndexOut[1]>=0 && IndexOut[1]<Taille[1])
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
	    if(IndexOut[0]>=0 && IndexOut[0]<Taille[0] && 
	       IndexOut[1]>=0 && IndexOut[1]<Taille[1])
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
	   if(IndexOut[0]>=0 && IndexOut[0]<Taille[0] && 
	      IndexOut[1]>=0 && IndexOut[1]<Taille[1])
	          OutputImage->SetPixel(IndexOut,m_PathValue);
	  
	   }
	 }
	 else
	 {
	 IndexOut[0] = static_cast<int>(Xmin); 
	 
	 for(RealType j = Ymin; j<=Ymax;j++)
	   {
	   IndexOut[1]=static_cast<int>(j);
	   if( (IndexOut[0]>=0) && (IndexOut[0]<Taille[0]) && 
	      (IndexOut[1]>=0) && (IndexOut[1]<Taille[1]))
	          OutputImage->SetPixel(IndexOut,m_PathValue);
	   }
	 }
       }         
     } // FOR loop
    } // IF loop
}

  
/**
 *
 */
template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathFilter<TInputImage,TInputPath,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Path Value: " << m_PathValue << std::endl;
}


} // end namespace otb

#endif
 
