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
#ifndef __otbComplexMomentPathFunction_txx
#define __otbComplexMomentPathFunction_txx

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
template < class TInputPath, class TOutput, class TPrecision>
ComplexMomentPathFunction<TInputPath,TOutput,TPrecision>
::ComplexMomentPathFunction()
{
  m_P    = 0;
  m_Q    = 0;
  m_Step = 1.0;
}

/**
   *
   */
template < class TInputPath, class TOutput, class TPrecision>
void
ComplexMomentPathFunction<TInputPath,TOutput,TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " p indice value      : "  << m_P << std::endl;
  os << indent << " q indice value      : "  << m_Q << std::endl;
}


template < class TInputPath, class TOutput, class TPrecision>
typename ComplexMomentPathFunction<TInputPath,TOutput,TPrecision>::ComplexType
ComplexMomentPathFunction<TInputPath,TOutput,TPrecision>
::EvaluateComplexMomentAtIndex(VertexType index) const
{
    ComplexPrecisionType                         ValP;
    ComplexPrecisionType                         ValQ;
    ComplexPrecisionType                         Result;
    PrecisionType                                PixelValue(1.0);

    ValP = ComplexPrecisionType(1.0,0.0);
    ValQ = ComplexPrecisionType(1.0,0.0);
    unsigned int p  = m_P;
    while(p>0)
     {
      ValP *= ComplexPrecisionType(index[0], index[1]);
      --p;
     }
    unsigned int q  = m_Q;
    while(q>0)
     {
      ValQ *= ComplexPrecisionType(index[0], -index[1]);
      --q;
     }

    Result = ValP * ValQ * ComplexPrecisionType( static_cast<PrecisionType>(PixelValue), 0.0);
    return ( static_cast<ComplexType>(Result) );
}


template < class TInputPath, class TOutput, class TPrecision>
typename ComplexMomentPathFunction<TInputPath,
                                   TOutput,TPrecision>::OutputType
ComplexMomentPathFunction<TInputPath,TOutput,TPrecision>
::Evaluate(const PathType& path) const
{
  PathConstPointer                    Path;
  VertexListPointer                   vertexList;
  VertexType                          cindex;
  VertexType                          IndexOut;
  int                                 nbPath;
  ComplexType                   Value;

  Value = static_cast<ComplexType>(0.0);

  vertexList = path.GetVertexList();
  nbPath = vertexList->Size();

  if(nbPath >1)
     {
     for(int i =0; i<nbPath-1;i++)
       {
       cindex = vertexList->GetElement(i);
       PrecisionType x1 = cindex[0];
       PrecisionType y1 = cindex[1];
       cindex = vertexList->GetElement(i+1);
       PrecisionType x2 = cindex[0];
       PrecisionType y2 = cindex[1];

       PrecisionType Theta;
       PrecisionType Norm;

       Theta = vcl_atan2(y2-y1,x2-x1);
       Norm  = vcl_sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );

       for(RealType k = 0; k <=Norm; k+=m_Step)
         {
   IndexOut[0] = x1 + k * vcl_cos(Theta);
   IndexOut[1] = y1 + k * vcl_sin(Theta);

   Value += EvaluateComplexMomentAtIndex(IndexOut );
   }
       } // FOR loop
     } // IF loop
  return (static_cast<OutputType>(Value) );

}

template < class TInputPath, class TOutput, class TPrecision>
typename ComplexMomentPathFunction<TInputPath,
                                   TOutput,TPrecision>::OutputType
ComplexMomentPathFunction<TInputPath,TOutput,TPrecision>
::Evaluate() const
{
  if( !this->GetInputPath() )
    {
    otbMsgDevMacro( << "Pb with GetInputPath" );
    return static_cast<OutputType>(ComplexPrecisionType( itk::NumericTraits<PrecisionType>::Zero, itk::NumericTraits<PrecisionType>::Zero ) );
    }

  OutputType Result =  Evaluate( *(this->GetInputPath()) );

  return Result;
}

} // namespace otb

#endif
