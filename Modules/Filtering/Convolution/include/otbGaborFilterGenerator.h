/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbGaborFilterGenerator_h
#define otbGaborFilterGenerator_h

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
 * of a complex Gabor function.
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
class GaborFilterGenerator : public itk::Object
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

  // useful typedefs
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
  ~GaborFilterGenerator() override
  {
  }

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Generate the filter coefficients */
  void GenerateFilter();

  /** Reimplement the Modified() method
   *to set the NeedToGenerateFilter to true */
  void Modified() const override;

private:
  GaborFilterGenerator(const Self&) = delete;
  void operator=(const Self&) = delete;

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

  /** Whether we need to regenerate the filter */
  mutable bool m_NeedToRegenerateFilter;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGaborFilterGenerator.hxx"
#endif

#endif
