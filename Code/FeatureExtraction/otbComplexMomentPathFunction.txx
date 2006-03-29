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
template < class TInputPath, class TOutput>
ComplexMomentPathFunction<TInputPath,TOutput>
::ComplexMomentPathFunction()
{
  m_P    = 0;
  m_Q    = 0;
  m_Step = 1.0;
}

/**
   *
   */
template < class TInputPath, class TOutput>
void
ComplexMomentPathFunction<TInputPath,TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " p indice value      : "  << m_P << std::endl;
  os << indent << " q indice value      : "  << m_Q << std::endl;
}


template < class TInputPath, class TOutput>
typename ComplexMomentPathFunction<TInputPath,TOutput>::ComplexType 
ComplexMomentPathFunction<TInputPath,TOutput>
::EvaluateComplexMomentAtIndex(VertexType index) const
{
    ComplexType                         ValP;
    ComplexType                         ValQ;
    double                              PixelValue = 1.0;

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


template < class TInputPath, class TOutput>
typename ComplexMomentPathFunction<TInputPath,
                                   TOutput>::ComplexType
ComplexMomentPathFunction<TInputPath,TOutput>
::Evaluate(const PathType& path) const
{
  typedef float                       RealType;

  PathConstPointer                    Path;
  VertexListPointer                   vertexList;
  VertexType                          cindex;
  VertexType                          IndexOut;
  int                                 nbPath;
  ComplexType  	     		      Value;

  Value = static_cast<ComplexType>(0.0);
  
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
       
       RealType Theta;
       RealType Norm;
       
       Theta = atan2(y2-y1,x2-x1);
       Norm  = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );

       for(RealType k = 0 ; k <=Norm ; k+=m_Step)
         {
	 IndexOut[0] = x1 + k * cos(Theta);
	 IndexOut[1] = y1 + k * sin(Theta);
	 
	 Value += EvaluateComplexMomentAtIndex(IndexOut );
	 }
       } // FOR loop
     } // IF loop

  return (static_cast<ComplexType>(Value) );

}


} // namespace otb

#endif
