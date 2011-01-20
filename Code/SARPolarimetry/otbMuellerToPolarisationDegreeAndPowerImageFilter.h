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

#ifndef __MuellerToPolarisationDegreeAndPowerImageFilter_h
#define __MuellerToPolarisationDegreeAndPowerImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkNumericTraits.h"

namespace otb
 {

namespace Functor {

/** \class otbMuellerToPolarisationDegreeAndPowerFunctor
 * \brief Evaluate the  min and max polarisation degree and min and max power
 *   from the Mueller image
 *
 */
template< class TInput, class TOutput>
class MuellerToPolarisationDegreeAndPowerFunctor
{
public:
  typedef double                                    RealType;
  typedef typename TOutput::ValueType               OutputValueType;

  inline TOutput operator()( const TInput & Mueller ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);


    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   MuellerToPolarisationDegreeAndPowerFunctor() : m_NumberOfComponentsPerPixel(4),
       m_PowerMin(itk::NumericTraits<RealType>::max()),
       m_PowerMax(itk::NumericTraits<RealType>::min()),
       m_PolarisationDegreeMin(itk::NumericTraits<RealType>::max()),
       m_PolarisationDegreeMax(itk::NumericTraits<RealType>::min()) {}

   /** Destructor */
   ~MuellerToPolarisationDegreeAndPowerFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
    RealType     m_PowerMin;
    RealType     m_PowerMax;
    RealType     m_PolarisationDegreeMin;
    RealType     m_PolarisationDegreeMax;
};
}


/** \class otbMuellerToPolarisationDegreeAndPowerImageFilter
 * \brief Compute the circular polarisation image (3 channels : LL, RR and LR)
 * from the Mueller image (16 real channels)
 */
template <class TInputImage, class TOutputImage, class TFunction = Functor::MuellerToPolarisationDegreeAndPowerFunctor<
    ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT MuellerToPolarisationDegreeAndPowerImageFilter :
   public itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction>
{
public:
   /** Standard class typedefs. */
   typedef MuellerToPolarisationDegreeAndPowerImageFilter  Self;
   typedef itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(MuellerToPolarisationDegreeAndPowerImageFilter,itk::UnaryFunctorImageFilter);


protected:
   MuellerToPolarisationDegreeAndPowerImageFilter() {}
  virtual ~MuellerToPolarisationDegreeAndPowerImageFilter() {}

private:
  MuellerToPolarisationDegreeAndPowerImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);                                 //purposely not implemented
};
  
} // end namespace otb

#endif
