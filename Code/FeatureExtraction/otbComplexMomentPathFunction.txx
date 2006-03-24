/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   22 mars 2006
  Version   :   
  Role      :   Geometric Moments Class of path 
  $Id:$

=========================================================================*/
#ifndef _otbComplexMomentPathFunction_txx
#define _otbComplexMomentPathFunction_txx

#include "otbComplexMomentPathFunction.h"
#include "itkImageRegionIterator.h"
#include "itkImage.h"
#include "itkConstNeighborhoodIterator.h"

#include <complex>
namespace otb
{

/**
   * Constructor
   */
template < class TInputImage, class TInputPath, class TOutput>
ComplexMomentPathFunction<TInputImage,TInputPath,TOutput>
::ComplexMomentPathFunction()
{
  m_P = 0;
  m_Q = 0;
}

/**
   *
   */
template < class TInputImage, class TInputPath, class TOutput>
void
ComplexMomentPathFunction<TInputImage,TInputPath,TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " p indice value      : "  << m_P << std::endl;
  os << indent << " q indice value      : "  << m_Q << std::endl;
}


template < class TInputImage, class TInputPath, class TOutput>
typename ComplexMomentPathFunction<TInputImage,TInputPath,TOutput>::ComplexType 
ComplexMomentPathFunction<TInputImage,TInputPath,TOutput>
::EvaluateComplexMomentAtIndex(ImageIndexType index,PixelType PixelValue) const
{
    ComplexType                         ValP;
    ComplexType                         ValQ;
    ValP = std::complex<double>(1.0,0.0);
    ValQ = std::complex<double>(1.0,0.0);
    unsigned int p  = m_P;
    while(p>0)
     {
      ValP *= std::complex<double>(index[0], index[1]);
      --p; 
     }
    unsigned int q  = m_Q;
    while(q>0)
     {
      ValQ *= std::complex<double>(index[0], -index[1]);
      --q; 
     }

    return ( static_cast<ComplexType>
                    ( ValP * ValQ * std::complex<double>( static_cast<double>(PixelValue),0.0) ) );
}


template < class TInputImage, class TInputPath, class TOutput>
typename ComplexMomentPathFunction<TInputImage,TInputPath,
                                   TOutput>::ComplexType
ComplexMomentPathFunction<TInputImage,TInputPath,TOutput>
::Evaluate(const PathType& path) const
{
  typedef float                       RealType;

  typename ImageType::ConstPointer    Image;
  typename ImageType::SizeType        ImageSize;
  PathConstPointer                    Path;
  VertexListPointer                   vertexList;
  VertexType                          cindex;
  ImageIndexType                      IndexOut;
  int                                 nbPath;
  ComplexType  				Value;

  Value = static_cast<ComplexType>(0.0);

  if( !this->GetInputImage() )
    {
      return std::complex<float>(0.,0.);  // A modifier
    }
       
   Image = this->GetInputImage();
   
   ImageSize = Image->GetBufferedRegion().GetSize();

   vertexList = path.GetVertexList();
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
	      if(IsInsideBuffer(IndexOut))
	          Value += EvaluateComplexMomentAtIndex(IndexOut, Image->GetPixel(IndexOut) );
	      }
	   }
           else
	   {
	   RealType Alpha = (x2-x1) / (y2-y1) ;
	   for(RealType j = Ymin; j<=Ymax;j++)
	      {
	      IndexOut[0] = static_cast<int>(Alpha * (j-y1) + x1) ;
	      IndexOut[1] = static_cast<int>(j);
	      if(IsInsideBuffer(IndexOut))
	          Value += EvaluateComplexMomentAtIndex(IndexOut, Image->GetPixel(IndexOut) );
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
	     if(IsInsideBuffer(IndexOut))
	          Value += EvaluateComplexMomentAtIndex(IndexOut, Image->GetPixel(IndexOut) );
	     }
	   }
	   else
	   {
	   IndexOut[0] = static_cast<int>(Xmin); 	 
	   for(RealType j = Ymin; j<=Ymax;j++)
	     {
	     IndexOut[1]=static_cast<int>(j);
	     if(IsInsideBuffer(IndexOut))
	          Value += EvaluateComplexMomentAtIndex(IndexOut, Image->GetPixel(IndexOut) );
	     }
	   }
         }         
       } // FOR loop
    } // IF loop

  return (static_cast<ComplexType>(Value) );

}


} // namespace otb

#endif
