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
#ifndef _otbComplexMomentPathFunction_txx
#define _otbComplexMomentPathFunction_txx

#include "otbComplexMomentPathFunction.h"
#include "itkImageRegionIterator.h"
#include "itkImage.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbMacro.h"

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
    ComplexType                         Result;
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

    Result = ValP * ValQ * std::complex<double>( static_cast<double>(PixelValue), 0.0); 
    return ( static_cast<ComplexType>(Result) );
}


template < class TInputPath, class TOutput>
typename ComplexMomentPathFunction<TInputPath,
                                   TOutput>::OutputType
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
  return (static_cast<OutputType>(Value) );

}

template < class TInputPath, class TOutput>
typename ComplexMomentPathFunction<TInputPath,
                                   TOutput>::OutputType
ComplexMomentPathFunction<TInputPath,TOutput>
::Evaluate() const
{
  if( !this->GetInputPath() )
    {
    otbMsgDevMacro( << "Pb with GetInputPath" );
    return static_cast<OutputType>(std::complex<float>( itk::NumericTraits<float>::max(), itk::NumericTraits<float>::max() ) );
    }

  OutputType Result =  Evaluate( *(this->GetInputPath()) );
  
  return Result;
}

} // namespace otb

#endif
