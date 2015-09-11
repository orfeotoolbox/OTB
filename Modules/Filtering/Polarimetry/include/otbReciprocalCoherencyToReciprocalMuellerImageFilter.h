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

#ifndef __ReciprocalCoherencyToReciprocalMuellerImageFilter_h
#define __ReciprocalCoherencyToReciprocalMuellerImageFilter_h

#include "otbUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class CoherencyToReciprocalMuellerFunctor
 * \brief Evaluate the reciprocal Mueller matrix from the reciprocal coherency matrix image
 *
 * Outpus are:
 * - channel #0 : \f$ 0.5*( C_{11}+C_{22}+C_{33} ) \f$
 * - channel #1 : \f$ Re(C_{12}) + Im(C_{22}) \f$
 * - channel #2 : \f$ Re(C_{13}) \f$
 * - channel #3 : \f$ Im(C_{23}) \f$
 * - channel #4 : \f$ Re(C_{12}) \f$
 * - channel #5 : \f$ 0.5*( C_{11}+C_{22}-C_{33} ) \f$
 * - channel #6 : \f$ Re(C_{23}) \f$
 * - channel #7 : \f$ Im(C_{13}) \f$
 * - channel #8 : \f$ -Re(C_{13}) \f$
 * - channel #9 : \f$ -Re(C_{23}) \f$
 * - channel #10 : \f$ 0.5.Re(VAL1) \f$
 * - channel #11 : \f$ 0.5.Im(VAL0) \f$
 * - channel #12 : \f$ Im(C_{23}) \f$
 * - channel #13 : \f$ Im(C_{13}) \f$
 * - channel #14 : \f$ 0.5.Im(VAL1^{*}) \f$
 * - channel #15 : \f$ 0.5.Re(VAL0) \f$
 *
 * With:
 * VAL0 = C_{33}+C_{12}-C_{11}-(C_{12}-C_{22})^{*}   
 * VAL1 = -C_{33}+C_{12}-C_{11}-(C_{12}-C_{22})^{*} 
 * 
 * Where Coherency is the input pixel and contains:
 * - channel #0 : \f$ 0.5*(S_{hh}+S_{vv}).(S_{hh}+S_{vv})^{*} \f$
 * - channel #1 : \f$ 0.5*(S_{hh}+S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 * - channel #2 : \f$ 0.5*(S_{hh}+S_{vv}).(2*S_{hv})^{*} \f$
 * - channel #3 : \f$ 0.5*(S_{hh}-S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 * - channel #4 : \f$ 0.5*(S_{hh}-S_{vv}).(2*S_{hv})^{*} \f$
 * - channel #5 : \f$ 0.5*(2*S_{hv}).(2*S_{hv})^{*} \f$
 *
 * The output pixel has 16 channels
 * Element are stored from left to right, line by line.
 *
 * \ingroup SARPolarimetry
 *
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class ReciprocalCoherencyToReciprocalMuellerFunctor
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
    
    ComplexType VAL4 = static_cast<ComplexType>( (Coherency[1] - Coherency[3]) );
    ComplexType VAL5 = static_cast<ComplexType>( (Coherency[1] - Coherency[0]) );
	ComplexType VAL0 = static_cast<ComplexType>( Coherency[5] + VAL5 - vcl_conj(VAL4) );
    ComplexType VAL1 = static_cast<ComplexType>( -Coherency[5] + VAL5 - vcl_conj(VAL4) );

    result[0] = 0.5*(T1+T2+T3);                               
    result[1] = static_cast<double>( Coherency[1].real()+Coherency[3].imag() );
    result[2] = static_cast<double>( Coherency[2].real() );   
    result[3] = static_cast<double>( Coherency[4].imag() );                           
    result[4] = static_cast<double>( Coherency[1].real() );  
    result[5] = 0.5*(T1+T2-T3); 
    result[6] = static_cast<double>( Coherency[4].real() );
    result[7] = static_cast<double>( Coherency[2].imag() ); 
    result[8] = static_cast<double>( -Coherency[2].real() );
    result[9] = static_cast<double>( -Coherency[4].real() );
	result[10] = static_cast<double>( 0.5*VAL1.real() ); 
	result[11] = static_cast<double>( 0.5*VAL0.imag() ); 
	result[12] = static_cast<double>( Coherency[4].imag() ); 
	result[13] = static_cast<double>( Coherency[2].imag() );
	result[14] = static_cast<double>( 0.5*vcl_conj(VAL1).imag() ); 
	result[15] = static_cast<double>( 0.5*VAL0.real() ); 

    return result;
    }

   unsigned int GetOutputSize()
   {
     return NumberOfComponentsPerPixel;
   }

   /** Constructor */
   ReciprocalCoherencyToReciprocalMuellerFunctor() {}

   /** Destructor */
   virtual ~ReciprocalCoherencyToReciprocalMuellerFunctor() {}

private:
   itkStaticConstMacro(NumberOfComponentsPerPixel, unsigned int, 16);
};
}


/** \class otbReciprocalCoherencyToReciprocalMuellerImageFilter
 * \brief Compute the reciprocal Mueller matrix image (10 real channels)
 * from the Reciprocal coherency image (6 complex channels)
 *
 * For more datails, please refer to ReciprocalCoherencyToReciprocalMuellerFunctor.
 *
 * \ingroup SARPolarimetry
 * \sa ReciprocalCoherencyToReciprocalMuellerFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReciprocalCoherencyToReciprocalMuellerImageFilter :
   public UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::ReciprocalCoherencyToReciprocalMuellerFunctor<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef ReciprocalCoherencyToReciprocalMuellerImageFilter  Self;
   typedef typename Functor::ReciprocalCoherencyToReciprocalMuellerFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctionType;
   typedef UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctionType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(ReciprocalCoherencyToReciprocalMuellerImageFilter, UnaryFunctorImageFilter);


protected:
   ReciprocalCoherencyToReciprocalMuellerImageFilter() {}
  virtual ~ReciprocalCoherencyToReciprocalMuellerImageFilter() {}


private:
  ReciprocalCoherencyToReciprocalMuellerImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);            //purposely not implemented

};

} // end namespace otb

#endif
