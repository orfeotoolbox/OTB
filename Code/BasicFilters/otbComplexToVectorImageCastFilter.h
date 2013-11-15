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

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"
#include "itkVariableLengthVector.h"


namespace otb
{
  
/** \class ComplexToVectorImageCastFilter
 * \brief Transform a complex image into a 2 channels vector image.
 * The first channel is the real part, the second the imaginary one.
 *
 *
 * \sa ComplexToImaginaryImageFilter
 * \sa ComplexToRealImageFilter
 */
namespace Functor {
 
template<class TInput, class TOutput>
class SingleComplexToVector
{
public:
  typedef typename TOutput::ValueType OutputValueType;

  SingleComplexToVector() {}
  ~SingleComplexToVector() {}

  inline TOutput operator()( const TInput & A ) const
    {
      TOutput output;
      output.SetSize( 2 );
      
      output[0] = static_cast<OutputValueType>(A.real());
      output[1] = static_cast<OutputValueType>(A.imag());

      return output;
    }
};


template<class TInput, class TOutput>
class VectorComplexToVector
{
public:
  typedef typename TOutput::ValueType OutputValueType;
  typedef typename TInput::ValueType ValueType;
  typedef itk::VariableLengthVector<ValueType> TestType;

  VectorComplexToVector() {}
  ~VectorComplexToVector() {}

  inline TOutput operator()( const TInput & A ) const
    {
      TOutput output;
      output.SetSize( 2*A.Size() );
      
      for(unsigned int i=0; i<A.Size(); ++i)
        {
          output[2*i] = static_cast<OutputValueType>(A[i].real());
          output[2*i+1] = static_cast<OutputValueType>(A[i].imag());
        }

      return output;
    }

};


template< class TInput, class TOutput >
class ComplexToVector
{
public:
  typedef SingleComplexToVector<TInput, TOutput> FunctorType;
};
 

template< class TInput, class TOutput >
class ComplexToVector<itk::VariableLengthVector<TInput>, TOutput>
{
public:
  typedef VectorComplexToVector<itk::VariableLengthVector<TInput>, TOutput> FunctorType;
};

}


template <class TInputImage, class TOutputImage>
class ITK_EXPORT ComplexToVectorImageCastFilter :
    public
itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
                        typename Functor::ComplexToVector<
  typename TInputImage::PixelType,
  typename TOutputImage::PixelType>::FunctorType   >
{
public:
  /** Standard class typedefs. */
  typedef ComplexToVectorImageCastFilter  Self;
  typedef itk::UnaryFunctorImageFilter<
    TInputImage, TOutputImage,
    typename Functor::ComplexToVector< typename TInputImage::PixelType,
    typename TOutputImage::PixelType>::FunctorType >                        Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ComplexToVectorImageCastFilter,
               itk::UnaryFunctorImageFilter);

  typedef typename TInputImage::PixelType    InputPixelType;
  typedef typename TOutputImage::PixelType   OutputPixelType;


protected:
  ComplexToVectorImageCastFilter() {}
  virtual ~ComplexToVectorImageCastFilter() {}

  template<class T>
  bool PixelIsSingle(const T& /*dummy*/)
  {
		return true;
  }

  template<class T>
  bool PixelIsSingle(const itk::VariableLengthVector<T>& /*dummy*/)
  {
		return false;
  }

  void GenerateOutputInformation()
  {
		Superclass::GenerateOutputInformation();
		InputPixelType dummy;
		if (!this->PixelIsSingle(dummy))
		{
			this->GetOutput()->SetNumberOfComponentsPerPixel(2 * this->GetInput()->GetNumberOfComponentsPerPixel());
		}
  }

private:
  ComplexToVectorImageCastFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb


#endif
