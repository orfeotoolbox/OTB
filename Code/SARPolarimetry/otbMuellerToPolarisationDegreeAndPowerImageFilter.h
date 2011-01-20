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
#include "itkMatrix.h"
#include "itkVector.h"
#include "otbMath.h"

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
  typedef itk::Matrix<RealType,4,4>                 MuellerMatrixType;
  typedef itk::Vector<RealType,4>                   StokesVectorType;

  inline TOutput operator()( const TInput & Mueller ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);

    MuellerMatrixType muellerMatrix;
    muellerMatrix[0][0] = Mueller[0];
    muellerMatrix[0][1] = Mueller[1];
    muellerMatrix[0][2] = Mueller[2];
    muellerMatrix[0][3] = Mueller[3];
    muellerMatrix[1][0] = Mueller[4];
    muellerMatrix[1][1] = Mueller[5];
    muellerMatrix[1][2] = Mueller[6];
    muellerMatrix[1][3] = Mueller[7];
    muellerMatrix[2][0] = Mueller[8];
    muellerMatrix[2][1] = Mueller[9];
    muellerMatrix[2][2] = Mueller[10];
    muellerMatrix[2][3] = Mueller[11];
    muellerMatrix[3][0] = Mueller[12];
    muellerMatrix[3][1] = Mueller[13];
    muellerMatrix[3][2] = Mueller[14];
    muellerMatrix[3][3] = Mueller[15];

    RealType P;
    RealType deg_pol;
    RealType tau;
    RealType psi;
    StokesVectorType Si;
    StokesVectorType Sr;

    RealType PI_90;

    PI_90 = static_cast<RealType>( 2 * CONST_PI_180);

    tau = -45.0;
    while (tau < 46.0)
      {
      psi = -90.0;
      while (psi < 91.0)
        {

        // Define the incident Stokes vector
        Si[0] = 1.0;
        Si[1] = cos(psi*PI_90)*cos(tau*PI_90);
        Si[2] = sin(psi*PI_90)*cos(tau*PI_90);
        Si[3] = sin(tau*PI_90);

        // Evaluate the received Stokes vector
        Sr = muellerMatrix * Si;

        psi += 5.0;
        }
      tau += 5.0;
      }

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
