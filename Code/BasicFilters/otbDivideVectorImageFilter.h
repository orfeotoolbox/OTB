/*=========================================================================
	
  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbDivideVectorImageFilter_h
#define __otbDivideVectorImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkNumericTraits.h"

namespace otb
{
  
/** \class DivideVectorImageFilter
 * \brief Implements an operator for pixel-wise division of two images.
 *
 * This class is only a temporary fix to use in otbSimpleRcsPanSharpeningFusionImageFilter. Should be useless after 2007-12-11
 * 
 * \ingroup IntensityImageFilters  Multithreaded
 */

namespace Function {  
  
template< class TInput1, class TInput2, class TOutput>
class Div
{
public:
  Div() {};
  ~Div() {};
  bool operator!=( const Div & ) const
  {
    return false;
  }
  bool operator==( const Div & other ) const
  {
    return !(*this != other);
  }
  inline TOutput operator()( const TInput1 & A, const TInput2 & B)
  {
//     if(B != (TInput2) 0)
      return (TOutput)(A / B);
//     else
//       return NumericTraits<TOutput>::max();
  }
}; 
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT DivideVectorImageFilter :
    public
itk::BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                         Function::Div< 
  typename TInputImage1::PixelType, 
  typename TInputImage2::PixelType,
  typename TOutputImage::PixelType>   >
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef DivideVectorImageFilter  Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef itk::BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                                   Function::Div< 
    typename TInputImage1::PixelType, 
    typename TInputImage2::PixelType,
    typename TOutputImage::PixelType>   
  > Superclass;

  /** 
   * Smart pointer typedef support 
   */
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

// #ifdef ITK_USE_CONCEPT_CHECKING
//   /** Begin concept checking */
//   itkConceptMacro(IntConvertibleToInput2Check,
//     (itk::Concept::Convertible<int, typename TInputImage2::PixelType>));
//   itkConceptMacro(Input1Input2OutputDivisionOperatorsCheck,
//     (itk::Concept::DivisionOperators<typename TInputImage1::PixelType,
//                                 typename TInputImage2::PixelType,
//                                 typename TOutputImage::PixelType>));
//   /** End concept checking */
// #endif

protected:
  DivideVectorImageFilter() {}
  virtual ~DivideVectorImageFilter() {}
  DivideVectorImageFilter(const Self&) {}
  void operator=(const Self&) {}

};

} // end namespace otb


#endif
