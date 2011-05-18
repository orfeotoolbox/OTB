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

#ifndef __ReciprocalCoherencyToMuellerImageFilter_h
#define __ReciprocalCoherencyToMuellerImageFilter_h

#include "otbUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class CoherencyToMuellerFunctor
 * \brief Evaluate the Mueller matrix from the reciprocal coherency matrix image
 *
 * Outpus are:
 * - channel #0 : \f$ 0.5*\mathcal{Re}( Coherency[0]+Coherency[3]+Coherency[5]) \f$
 * - channel #1 : \f$ 0.5*\mathcal{Re}( Coherency[0]+Coherency[3]-Coherency[5]) \f$
 * - channel #2 : \f$ 0.5*\mathcal{Re}( Coherency[0]-Coherency[3]+Coherency[5]) \f$
 * - channel #3 : \f$ 0.5*\mathcal{Re}(-Coherency[0]+Coherency[3]+Coherency[5]) \f$
 * - channel #4 : \f$ \mathcal{Re}(Coherency[1]) \f$
 * - channel #5 : \f$ \mathcal{Re}(Coherency[2]) \f$
 * - channel #6 : \f$ \mathcal{Im}(Coherency[4]) \f$
 * - channel #7 : \f$ \mathcal{Re}(Coherency[4]) \f$
 * - channel #8 : \f$ \mathcal{Im}(Coherency[2]) \f$
 * - channel #9 : \f$ \mathcal{Im}(Coherency[1]) \f$
 *
 * Where Coherency is the input pixel and contains:
 * - channel #0 : \f$ (S_{hh}+S_{vv}).(S_{hh}+S_{vv})^{*} \f$
 * - channel #1 : \f$ (S_{hh}+S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 * - channel #2 : \f$ (S_{hh}+S_{vv}).(2*S_{hv})^{*} \f$
 * - channel #3 : \f$ (S_{hh}-S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 * - channel #4 : \f$ (S_{hh}-S_{vv}).(2*S_{hv})^{*} \f$
 * - channel #5 : \f$ (2*S_{hv}).(2*S_{hv})^{*} \f$
 *
 * The output pixel has 10 channels : the diagonal and the upper element of the matrix.
 * Element are stored from left to right, line by line.
 *
 * \ingroup SARPolarimetry
 *
 */
template< class TInput, class TOutput>
class ReciprocalCoherencyToMuellerFunctor
{
public:
  typedef typename std::complex <double>         ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;

  inline TOutput operator()( const TInput & Coherency ) const
    {
    TOutput result;
    result.SetSize(NumberOfComponentsPerPixel);

    const double T1 = static_cast<double>(Coherency[0].real());
    const double T2 = static_cast<double>(Coherency[3].real());
    const double T3 = static_cast<double>(Coherency[5].real());

    result[0] = 0.5*(T1+T2+T3);                               // A0+B0
    result[1] = 0.5*(T1+T2-T3);                               // A0+B
    result[2] = 0.5*(T1-T2+T3);                               // A0-B
    result[3] = 0.5*(-T1+T2+T3);                              // -A0+B0
    result[4] = static_cast<double>( Coherency[1].real() );  // C
    result[5] = static_cast<double>( Coherency[2].real() );  // H
    result[6] = static_cast<double>( Coherency[4].imag() );  // F
    result[7] = static_cast<double>( Coherency[4].real() );  // E
    result[8] = static_cast<double>( Coherency[2].imag() );  // G
    result[9] = -static_cast<double>( Coherency[1].imag() ); // D

    return result;
    }

   unsigned int GetOutputSize()
   {
     return NumberOfComponentsPerPixel;
   }

   /** Constructor */
   ReciprocalCoherencyToMuellerFunctor() {}

   /** Destructor */
   virtual ~ReciprocalCoherencyToMuellerFunctor() {}

private:
   itkStaticConstMacro(NumberOfComponentsPerPixel, unsigned int, 10);
};
}


/** \class otbReciprocalCoherencyToMuellerImageFilter
 * \brief Compute the Mueller matrix image (10 real channels)
 * from the Reciprocal coherency image (6 complex channels)
 *
 * For more datails, please refer to ReciprocalCoherencyToMuellerFunctor.
 *
 * \ingroup SARPolarimetry
 * \sa ReciprocalCoherencyToMuellerFunctor
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReciprocalCoherencyToMuellerImageFilter :
   public UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::ReciprocalCoherencyToMuellerFunctor<
    ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef ReciprocalCoherencyToMuellerImageFilter  Self;
   typedef typename Functor::ReciprocalCoherencyToMuellerFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctionType;
   typedef UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctionType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(ReciprocalCoherencyToMuellerImageFilter, UnaryFunctorImageFilter);


protected:
   ReciprocalCoherencyToMuellerImageFilter() {}
  virtual ~ReciprocalCoherencyToMuellerImageFilter() {}


private:
  ReciprocalCoherencyToMuellerImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);            //purposely not implemented

};
  
} // end namespace otb

#endif
