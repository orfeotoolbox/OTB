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
template < class TInputImage, class TInputPath, class TOutput, class TCoordRep>
ComplexMomentPathFunction<TInputImage,TInputPath,TOutput,TCoordRep>
::ComplexMomentPathFunction()
{
  m_P = 0;
  m_Q = 0;
}

/**
   *
   */
template < class TInputImage, class TInputPath, class TOutput, class TCoordRep>
void
ComplexMomentPathFunction<TInputImage,TInputPath,TOutput,TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " p indice value      : "  << m_P << std::endl;
  os << indent << " q indice value      : "  << m_Q << std::endl;
}


template < class TInputImage, class TInputPath, class TOutput, class TCoordRep>
typename ComplexMomentPathFunction<TInputImage,TInputPath,
                                   TOutput,TCoordRep>::ComplexType
ComplexMomentPathFunction<TInputImage,TInputPath,TOutput,TCoordRep>
::Evaluate() const
{
  typename InputType::ConstPointer    Image;
  typename InputType::SizeType        ImageSize;
  
  ComplexType                         Sum;
  ComplexType                         ValP;
  ComplexType                         ValQ;
  IndexType                           IndexValue;
  IndexType                           indexPos = index;
  typename InputType::SizeType        kernelSize;

  if( !this->GetInputImage() )
    {
      return std::complex<float>(0.,0.);  // A modifier
    }
  if( !this->GetInputPath() )
    {
      return std::complex<float>(0.,0.);  // A modifier
    }
  
     
   Image = this->GetInputImage();
   Path  = this->GetInputPath();
   
   ImageSize = this->GetInputImage()->GetBufferedRegion().GetSize();

   VertexDebut = 
   
   while()
   

   if(m_NeighborhoodRadius<0)
     {
     indexPos[0] = ImageSize[0] / 2 ;
     indexPos[1] = ImageSize[1] / 2;
     
       kernelSize[0] = indexPos[0];
       kernelSize[1] = indexPos[1];          
     }
     else
     {
       kernelSize.Fill( m_NeighborhoodRadius );
     }  
 
  itk::ConstNeighborhoodIterator<InputType>
    it(kernelSize, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());

  // Set the iterator at the desired location
  it.SetLocation(indexPos);
  Sum = std::complex<float>(0.0,0.0); 

  const unsigned int size = it.Size();
  for (unsigned int i = 0; i < size; ++i)
  {
    IndexValue = it.GetIndex(i);
    ValP = std::complex<float>(1.0,0.0);
    ValQ = std::complex<float>(1.0,0.0);
    unsigned int p  = m_P;
    while(p>0)
     {
      ValP *= std::complex<float>(IndexValue[0], IndexValue[1]);
      --p; 
     }
    unsigned int q  = m_Q;
    while(q>0)
     {
      ValQ *= std::complex<float>(IndexValue[0], -IndexValue[1]);
      --q; 
     }
//    std::cout<< i <<"--> "<< IndexValue << " p:"<<ValP <<" Q: "<<ValQ;  
          
    Sum += ( ValP * ValQ * std::complex<float>(static_cast<float>(it.GetPixel(i)),0.0) ); 
//    std::cout<< "Val Pixel: " << static_cast<float>(it.GetPixel(i)) <<" Result :" << Sum<<std::endl;
  }

//   std::cout<<"Result dans la procedure: " <<Sum <<std::endl;
           
  return (static_cast<ComplexType>(Sum) );

#endif

}


} // namespace otb

#endif
