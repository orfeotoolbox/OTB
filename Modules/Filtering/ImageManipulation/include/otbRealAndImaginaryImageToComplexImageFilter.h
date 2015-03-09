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
#ifndef __otbRealAndImaginaryImageToComplexImageFilter_h
#define __otbRealAndImaginaryImageToComplexImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace otb
{

/** \class RealAndImaginaryImageToComplexImageFilter
 * \brief Computes pixel-wise the complex image of a real and imaginary image.
 *
 * \sa RealAndImaginaryImageToComplexImageFilter
 * \ingroup Multithreaded
 *
 * \ingroup OTBImageManipulation
 */

namespace Function {

template< class TInputImageRealPart, class TInputImageImaginaryPart, class TOutput>
class RealAndImaginaryToComplex
{
public:
  typedef typename TOutput::value_type OutputValueType;

  RealAndImaginaryToComplex() {}
  ~RealAndImaginaryToComplex() {}
  bool operator!=( const RealAndImaginaryToComplex & ) const
    {
    return false;
    }
  bool operator==( const RealAndImaginaryToComplex & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInputImageRealPart & real,
                             const TInputImageImaginaryPart & imaginary ) const
    {
    return TOutput(static_cast<OutputValueType>(real), static_cast<OutputValueType>(imaginary));
    }
};
}

template <class TInputImageRealPart, class TInputImageImaginaryPart, class TOutputImage>
class ITK_EXPORT RealAndImaginaryImageToComplexImageFilter :
    public itk::BinaryFunctorImageFilter<TInputImageRealPart,
                        TInputImageImaginaryPart, TOutputImage,
                        Function::RealAndImaginaryToComplex<
                                  typename TInputImageRealPart::PixelType,
                                  typename TInputImageImaginaryPart::PixelType,
                                  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef RealAndImaginaryImageToComplexImageFilter  Self;
  typedef itk::BinaryFunctorImageFilter<
      TInputImageRealPart, TInputImageImaginaryPart, TOutputImage,
      Function::RealAndImaginaryToComplex< typename TInputImageRealPart::PixelType,
                              typename TInputImageImaginaryPart::PixelType,
                              typename TOutputImage::PixelType> >
                                         Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(RealAndImaginaryImageToComplexImageFilter,
               BinaryFunctorImageFilter);

  typedef typename TInputImageRealPart::PixelType                   InputRealPartPixelType;
  typedef typename TInputImageImaginaryPart::PixelType              InputImaginaryPartPixelType;
  typedef typename TOutputImage::PixelType                          OutputPixelType;
  typedef typename itk::NumericTraits< OutputPixelType >::ValueType OutputPixelValueType;

//#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
//  itkConceptMacro(InputConvertibleToOutputCheck,
//    (Concept::Convertible<InputRealPartPixelType, OutputPixelValueType>));
//  itkConceptMacro(InputConvertibleToOutputCheck,
//    (Concept::Convertible<InputImaginaryPartPixelType, OutputPixelValueType>));
  /** End concept checking */
//#endif

  void SetInputRealPart(const TInputImageRealPart * imageRealPart )
  {
    this->SetInput1( imageRealPart );
  }

  void SetInputImaginaryPart(const TInputImageImaginaryPart * imageImaginaryPart )
  {
    this->SetInput2( imageImaginaryPart );
  }

protected:
  RealAndImaginaryImageToComplexImageFilter() {}
  virtual ~RealAndImaginaryImageToComplexImageFilter() {}

private:
  RealAndImaginaryImageToComplexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
