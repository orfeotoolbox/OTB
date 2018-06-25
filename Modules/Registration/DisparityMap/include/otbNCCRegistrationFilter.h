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

#ifndef otbNCCRegistrationFilter_h
#define otbNCCRegistrationFilter_h

#include "itkPDEDeformableRegistrationFilter.h"
#include "otbNCCRegistrationFunction.h"

namespace otb
{

/** \class NCCRegistrationFilter
 * \brief Deformably register two images using the PDEs and NCC.
 *
 * NCCRegistrationFilter implements the #######
 * register two images by computing the displacement field which will map a
 * moving image onto a fixed image.
 *
 * A displacement field is represented as a image whose pixel type is some
 * vector type with at least N elements, where N is the dimension of
 * the fixed image. The vector type must support element access via operator
 * []. It is assumed that the vector elements behave like floating point
 * scalars.
 *
 * This class is templated over the fixed image type, moving image type
 * and the displacement field type.
 *
 * The input fixed and moving images are set via methods SetFixedImage
 * and SetMovingImage respectively. An initial displacement field maybe set via
 * SetInitialDisplacementField or SetInput. If no initial field is set,
 * a zero field is used as the initial condition.
 *
 * The algorithm has one parameter: the number of iteration to be performed.
 *
 * The output displacement field can be obtained via methods GetOutput
 * or GetDisplacementField.
 *
 * This class make use of the finite difference solver hierarchy. Update
 * for each iteration is computed in NCCRegistrationFunction.
 *
 * \warning This filter assumes that the fixed image type, moving image type
 * and displacement field type all have the same number of dimensions.
 *
 * \sa NCCRegistrationFunction
 * \ingroup DeformableImageRegistration MultiThreaded
 *
 * \example  DisparityMap/NCCRegistrationFilterExample.cxx
 *
 *
 *
 * \ingroup OTBDisparityMap
 */
template<class TFixedImage, class TMovingImage, class TDisplacementField>
class ITK_EXPORT NCCRegistrationFilter :
  public itk::PDEDeformableRegistrationFilter<TFixedImage, TMovingImage,
      TDisplacementField>
{
public:
  /** Standard class typedefs. */
  typedef NCCRegistrationFilter Self;
  typedef itk::PDEDeformableRegistrationFilter<
      TFixedImage, TMovingImage, TDisplacementField>    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(NCCRegistrationFilter,
               itk::PDEDeformableRegistrationFilter);

  /** Inherit types from superclass. */
  typedef typename Superclass::TimeStepType TimeStepType;

  /** FixedImage image type. */
  typedef typename Superclass::FixedImageType    FixedImageType;
  typedef typename Superclass::FixedImagePointer FixedImagePointer;

  /** MovingImage image type. */
  typedef typename Superclass::MovingImageType    MovingImageType;
  typedef typename Superclass::MovingImagePointer MovingImagePointer;

  /** Displacement field type. */
  typedef typename Superclass::DisplacementFieldType
  DisplacementFieldType;
  typedef typename Superclass::DisplacementFieldPointer
  DisplacementFieldPointer;

  /** FiniteDifferenceFunction type. */
  typedef typename Superclass::FiniteDifferenceFunctionType
  FiniteDifferenceFunctionType;

  /** NCCRegistrationFilterFunction type. */
  typedef NCCRegistrationFunction<FixedImageType, MovingImageType,
      DisplacementFieldType>  NCCRegistrationFunctionType;

  typedef typename NCCRegistrationFunctionType::RadiusType RadiusType;

  /** Get the metric value. The metric value is computed over the the
   * overlapping region between the two images.  This is value is only
   * available for the previous iteration and NOT the current
   * iteration.
   *\return metric
   **/
  virtual double GetMetric() const;

  /**
   * Set the NCC radius
   * \param radius
   */
  virtual void SetNCCRadius(RadiusType radius);

  /**
   * Get the radius.
   * \return radius.
   **/
  virtual RadiusType GetNCCRadius() const;

protected:
  NCCRegistrationFilter();
  ~NCCRegistrationFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Initialize the state of filter and equation before each iteration. */
  void InitializeIteration() override;

  /** Apply update. */
  using Superclass::ApplyUpdate;
  virtual void ApplyUpdate(TimeStepType dt);

  /** Update the Input requested region. */
  void GenerateInputRequestedRegion() override;

private:
  NCCRegistrationFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNCCRegistrationFilter.hxx"
#endif

#endif
