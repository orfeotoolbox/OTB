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

#ifndef otbMollweidMapProjection_h
#define otbMollweidMapProjection_h

#include "otbGenericMapProjection.h"

namespace otb
{
/** \class MollweidMapProjection
 *  \brief This class implements the Mollweid map projection.
 *  It converts coordinates in longitude, latitude (WGS84) to Mollweid map coordinates.
 *
 * \ingroup OTBProjection
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT MollweidMapProjection : public GenericMapProjection<TTransform>
{
public:
  /** Standard class typedefs. */
  typedef MollweidMapProjection             Self;
  typedef GenericMapProjection<TTransform>  Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MollweidMapProjection, GenericMapProjection);
  virtual void SetFalseEasting(double falseEasting);
  virtual void SetFalseNorthing(double falseNorthing);
  virtual double GetFalseNorthing() const;
  virtual double GetFalseEasting() const;
  virtual void SetParameters(double falseEasting, double falseNorthing);
  using Superclass::SetParameters;

protected:
  MollweidMapProjection();
  ~MollweidMapProjection() override;

private:
  MollweidMapProjection(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMollweidMapProjection.hxx"
#endif

#endif
