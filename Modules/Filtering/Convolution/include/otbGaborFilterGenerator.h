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
#ifndef __otbGaborFilterGenerator_h
#define __otbGaborFilterGenerator_h

#include "itkObject.h"
#include "itkObjectFactory.h"

#include "itkArray.h"
#include "itkSize.h"

namespace otb
{

/** \class GaborFilterGenerator
 * This class allows generating a gabor filter convolution mask. The mask is stored
 * in a linear buffer of type itk::Array returned by the GetFilter() method.
 *
 * When calling the GetFilter() method, if the filter has to be regenerated, it will be
 * done seamlessly.
 *
 * Gabor filters are commonly used in image processing for textures description for instance
 * The complex Gabor function is the product in spatial domain of a complex sinusoidal, also
 * known as the carrier, and a gaussian-shaped function, also knwown as the envelop.
 *
 * Please note that this helper class generates a filter whose coefficients are the real part
 * of a complex Gabor fucntion.
 *
 * The formula used to compute these coefficients is as follows:
 *
 * \f$ g(x, y) = \exp\Big(-\pi((a x_\theta)^{2} +(b y_\theta)^{2})\Big) * \cos\big(2\pi(u_0*x + v_0*y)+\phi\big) \f$
 *
 * with:
 *
 * \f$ x_\theta = x \cos(\theta)+y \sin(\theta) \f$
 * and
 * \f$ y_\theta = y \cos(\theta)-x \sin(\theta) \f$
 *
 * Where:
 *
 * - Radius         : Radius of the generated filter (GetRadius()/SetRadius())
 * - \f$ a      \f$ : Scale the x axis of the gaussian envelop. (GetA()/SetA())
 * - \f$ b      \f$ : Scale the y axis of the gaussian envelop (GetB()/SetB())
 * - \f$ \theta \f$ : Rotation angle of the gaussian envelop (in degree) (GetTheta()/SetTheta())
 * - \f$ u_0    \f$ : x spatial frenquency of the sinusoidal carrier (GetU0()/SetU0())
 * - \f$ v_0    \f$ : y spatial frenquency of the sinusoidal carrier (GetV0()/SetV0())
 * - \f$ \phi   \f$ : Phase of the sinusoidal carrier (GetPhi()/SetPhi())
 *
 *
 * \ingroup OTBConvolution
 */

template <class TPrecision>
class GaborFilterGenerator
  : public itk::Object
{
public:
  // Standard class typedefs
  typedef GaborFilterGenerator          Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  // Standard new and type macro
  itkNewMacro(Self);
  itkTypeMacro(GaborFilterGenerator, Object);

  // usefull typedefs
  typedef TPrecision                PrecisionType;
  typedef itk::Array<PrecisionType> ArrayType;
  typedef itk::Size<2>              RadiusType;

  /** Setters/Getters */
  itkSetMacro(A, PrecisionType);
  itkGetMacro(A, PrecisionType);
  itkSetMacro(B, PrecisionType);
  itkGetMacro(B, PrecisionType);
  itkSetMacro(Theta, PrecisionType);
  itkGetMacro(Theta, PrecisionType);
  itkSetMacro(Phi, PrecisionType);
  itkGetMacro(Phi, PrecisionType);
  itkSetMacro(U0, PrecisionType);
  itkGetMacro(U0, PrecisionType);
  itkSetMacro(V0, PrecisionType);
  itkGetMacro(V0, PrecisionType);
  itkSetMacro(Radius, RadiusType);
  itkGetConstReferenceMacro(Radius, RadiusType);

  /** Get The generated filter */
  const ArrayType& GetFilter();

protected:
  /** constructor */
  GaborFilterGenerator();
  /** destructor */
  virtual ~GaborFilterGenerator() {}

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Generate the filter coefficients */
  void GenerateFilter();

  /** Reimplement the Modified() method
   *to set the NeedToGenerateFilter to true */
  virtual void Modified() const;

private:
  GaborFilterGenerator(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Radius of the filter */
  RadiusType m_Radius;

  /** Gabor parameters */
  PrecisionType m_A;
  PrecisionType m_B;
  PrecisionType m_Theta;
  PrecisionType m_U0;
  PrecisionType m_V0;
  PrecisionType m_Phi;

  /** Output filter coefficient array */
  ArrayType m_Filter;

  /** Wether we need to regenerate the filter */
  mutable bool m_NeedToRegenerateFilter;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGaborFilterGenerator.txx"
#endif

#endif
