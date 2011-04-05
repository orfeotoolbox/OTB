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

#ifndef __MuellerToCircularPolarisationImageFilter_h
#define __MuellerToCircularPolarisationImageFilter_h

#include "otbUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class otbMuellerToCircularPolarisationFunctor
 * \brief Evaluate the  Circular Polarisation image from the Mueller image.
 *
 * The input Mueller image has 16 channels, one for each matrix element.
 * The order of the channels corresponds to :
 * \f$  \begin{pmatrix} 
 * {channel #0 }&{channel #1 }&{channel #2 }&{channel #3 } \\
 * {channel #4 }&{channel #5 }&{channel #6 }&{channel #7 } \\
 * {channel #8 }&{channel #9 }&{channel #10}&{channel #11} \\
 * {channel #12}&{channel #13}&{channel #14}&{channel #15} \\
 * \end{pmatrix}
 *
 *  Output value are:
 *   channel #0 : \f$ LL = M_{11}+M_{14}+M_{41}+M_{44} \f$ \\
 *   channel #1 : \f$ RR = M_{11}-M_{14}+M_{41}+M_{44} \f$ \\
 *   channel #2 : \f$ LR = M_{11}-M_{44}\f$ \\
 *
 * \ingroup Functor
 * \ingroup SARPolarimetry
 *
 * \sa MuellerToMLCImageFilter
 * \sa MuellerToPolarisationDegreeAndPowerImageFilter
 *
 */
template< class TInput, class TOutput>
class MuellerToCircularPolarisationFunctor
{
public:
  typedef double                                    RealType;
  typedef typename TOutput::ValueType               OutputValueType;

  inline TOutput operator()( const TInput & Mueller ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);

    const RealType M11 =  static_cast<RealType>(Mueller[0]);
    const RealType M14 =  static_cast<RealType>(Mueller[3]);
    const RealType M41 =  static_cast<RealType>(Mueller[12]);
    const RealType M44 =  static_cast<RealType>(Mueller[15]);

    result[0] = static_cast<OutputValueType>( M11 + M14 + M41 + M44 ); // LL
    result[1] = static_cast<OutputValueType>( M11 - M14 - M41 + M44 ); // RR
    result[2] = static_cast<OutputValueType>( M11 -M44 );              // LR

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   MuellerToCircularPolarisationFunctor() : m_NumberOfComponentsPerPixel(3)  {}

   /** Destructor */
   virtual ~MuellerToCircularPolarisationFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbMuellerToCircularPolarisationImageFilter
 * \brief Compute the circular polarisation image (3 channels : LL, RR and LR)
 * from the Mueller image (16 real channels)
 */
template <class TInputImage, class TOutputImage, class TFunction = Functor::MuellerToCircularPolarisationFunctor<
    ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT MuellerToCircularPolarisationImageFilter :
   public UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
   /** Standard class typedefs. */
   typedef MuellerToCircularPolarisationImageFilter  Self;
   typedef UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(MuellerToCircularPolarisationImageFilter, UnaryFunctorImageFilter);


protected:
   MuellerToCircularPolarisationImageFilter() {}
  virtual ~MuellerToCircularPolarisationImageFilter() {}

private:
  MuellerToCircularPolarisationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);                           //purposely not implemented
};
  
} // end namespace otb

#endif
