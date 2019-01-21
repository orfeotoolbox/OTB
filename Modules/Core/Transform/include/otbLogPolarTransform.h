/*
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

#ifndef otbLogPolarTransform_h
#define otbLogPolarTransform_h

#include "otbTransform.h"

namespace otb
{
/** \class LogPolarTransform
 * \brief Implement the log-polar coordinate transform.
 *
 * The log-polar coordinate system is a polar system where the
 * radial coordinate has a logarithmic scale.
 *
 * Given \f$(\rho,\theta)\f$ the coordinate in the log-polar system, the
 * corresponding cartesian coordinates are
 *
 * \f$ x-xc=exp(\rho)*cos(\theta) \f$
 * \f$ y-yc=exp(\rho)*sin(\theta) \f$
 *
 * Where (xc, yc) is the center of the log-polar transform in the cartesian space.
 * Please note that this center can NOT be represented in log-polar.
 *
 * \sa InverseLogPolarTransform
 * \ingroup Transform
 *
 * \ingroup OTBTransform
 */
template <class TScalarType>
class ITK_EXPORT LogPolarTransform
  : public Transform<TScalarType, 2, 2>
{
public:
  /** Standard typedef */
  typedef LogPolarTransform                 Self;
  typedef Transform<TScalarType, 2, 2> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  /** Creation through object factory */
  itkNewMacro(Self);
  /** Runtime information */
  itkTypeMacro(LogPolarTransform, Transform);

  /** Template related typedefs */
  typedef TScalarType ScalarType;

  /** Superclass related typedefs */
  typedef typename Superclass::InputPointType      InputPointType;
  typedef typename Superclass::OutputPointType     OutputPointType;
  typedef typename Superclass::InputVectorType     InputVectorType;
  typedef typename Superclass::OutputVectorType    OutputVectorType;
  typedef typename Superclass::InputVnlVectorType  InputVnlVectorType;
  typedef typename Superclass::OutputVnlVectorType OutputVnlVectorType;
  typedef typename Superclass::ParametersType      ParametersType;
  typedef itk::FixedArray<TScalarType, 2>          ScaleType;

  /** Set/Get the origin */
  itkSetMacro(Center, InputPointType);
  itkGetConstReferenceMacro(Center, InputPointType);

  /** Set/Get the scales */
  itkSetMacro(Scale, ScaleType);
  itkGetConstReferenceMacro(Scale, ScaleType);
  /**
   * Set the transform parameters through the standard interface.
   * \param parameters The parameters of the transform.
   */
  void SetParameters(const ParametersType& parameters) override;
  /**
   * Get the transform parameters through the standard interface.
   * \return The parameters of the transform.
   */
  ParametersType& GetParameters(void) const override;

  /**
   * Set the Fixed Parameters
   * \param param The fixed parameters of the transform.
   */
  void SetFixedParameters( const ParametersType & param) override
    { this->m_FixedParameters = param; }

  /**
   * Get the Fixed Parameters
   * \return The Fixed parameters of the transform.
   */
  const ParametersType& GetFixedParameters(void) const override{return this->m_FixedParameters; }
  /**
   * Transform a point.
   * \param point The point to transform.
   * \return The transformed point.
   */
  OutputPointType TransformPoint(const InputPointType& point) const override;
  /**
   * Transform a vector representing a point.
   * \param vector The point to transform.
   * \return The transformed point.
   */
  using Superclass::TransformVector;
  OutputVectorType TransformVector(const InputVectorType& vector) const override;

  /**
   * Transform a vnl vector representing a point.
   * \param vector The point to transform.
   * \return The transformed point.
   */
  OutputVnlVectorType TransformVector(const InputVnlVectorType& vector) const override;

protected:
  /** Constructor */
  LogPolarTransform();
  /** Destructor */
  ~LogPolarTransform() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LogPolarTransform(const Self &) = delete;
  void operator =(const Self&) = delete;

  InputPointType m_Center;
  ScaleType      m_Scale;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLogPolarTransform.hxx"
#endif

#endif
