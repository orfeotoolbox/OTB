/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSinclairImageFilter_h
#define otbSinclairImageFilter_h

#include "otbFunctorImageFilter.h"
#include "otbSinclairToCovarianceMatrixFunctor.h"
#include <complex>

namespace otb
{

/** \class SinclairImageFilter
 * \brief Computes the Covariance matrix for the sinclair one.
 *
 * This class is parameterized over the type of the input images and
 * the type of the output image. It is also parameterized by the
 * operation to be applied, using a Functor style.
 *
 * The output image has 10 channels : the diagonal and the upper element of the matrix.
 * Element are stored from left to right, line by line.
 *
 * The class is templated by the 4 input image (HH, HV, VH and VV) and the used functor.
 * Available functors are :
 * - SinclairToCircularCovarianceMatrixFunctor (default one)
 * - SinclairToCoherencyMatrixFunctor
 * - SinclairToCovarianceMatrixFunctor
 * - SinclairToMuelleMatrixrFunctor
 *
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCoherencyMatrixFunctor
 *  \sa SinclairToCovarianceMatrixFunctor
 *  \sa SinclairToMuelleMatrixrFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */

template <class TFunction>
class ITK_EXPORT SinclairImageFilter :  public FunctorImageFilter<TFunction>
{
public:
  /** Standard typedefs */
  typedef SinclairImageFilter Self;
  typedef FunctorImageFilter<TFunction> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SinclairImageFilter, FunctorImageFilter);

  // TODO uncomment
  static_assert(Superclass::NumberOfInputs == 4, "");

  void SetInputHH(const typename Superclass::template InputImageType<0> * image)
  {
    this->SetInput1(image);
  }

  void SetInputHV(const typename Superclass::template InputImageType<1> * image)
  {
    this->SetInput2(image);
  }

  void SetInputVH(const typename Superclass::template InputImageType<2> * image)
  {
    this->SetInput3(image);
  }

  void SetInputVV(const typename Superclass::template InputImageType<3> * image)
  {
    this->SetInput4(image);
  }

protected:
  /**  Constructor */
  SinclairImageFilter(): Superclass(TFunction{}, {{0,0}}) {}
  /**  Destructor */
  ~SinclairImageFilter() override {}

private:

  SinclairImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};


} // end namespace otb

#endif
