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
#ifndef __otbComplexToVectorImageCastFilter_h
#define __otbComplexToVectorImageCastFilter_h

#include "otbUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace otb
{
  
/** \class ComplexToVectorImageCastFilter
 * \brief Transfomr a complex image into a 2 channels vector image.
 * The first channel is the real part, the second the imaginary one.
 *
 *
 * \sa ComplexToImaginaryImageFilter
 * \sa ComplexToRealImageFilter
 */
namespace Functor {
  
template< class TInput, class TOutput>
class ComplexToVector
{
public:
  typedef typename TOutput::ValueType OutputValueType;

 ComplexToVector() {}
  ~ComplexToVector() {}

  // This is an obligation to use the functor in otbUnaryFunctorImageFilter
  unsigned int GetOutputSize()
    {
      return 2;
    }

  inline TOutput operator()( const TInput & A ) const
    {
      TOutput output;
      output.SetSize(2);
      
      output[0] = static_cast<OutputValueType>(A.real());
      output[1] = static_cast<OutputValueType>(A.imag());
      
      return output;
    }
};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ComplexToVectorImageCastFilter :
    public
otb::UnaryFunctorImageFilter<TInputImage,TOutputImage,
                        Functor::ComplexToVector<
  typename TInputImage::PixelType,
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef ComplexToVectorImageCastFilter  Self;
  typedef otb::UnaryFunctorImageFilter<
    TInputImage,TOutputImage,
    Functor::ComplexToVector< typename TInputImage::PixelType,
    typename TOutputImage::PixelType> >                        Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ComplexToVectorImageCastFilter,
               otb::UnaryFunctorImageFilter);

  typedef typename TInputImage::PixelType    InputPixelType;
  typedef typename TOutputImage::PixelType   OutputPixelType;


protected:
  ComplexToVectorImageCastFilter() {}
  virtual ~ComplexToVectorImageCastFilter() {}

private:
  ComplexToVectorImageCastFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb


#endif
