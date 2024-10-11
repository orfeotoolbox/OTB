/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbPointSetDensityFunction_h
#define otbPointSetDensityFunction_h

#include "otbPointSetFunction.h"

namespace otb
{

/**
 * \class PointSetDensityFunction
 * \brief Calculate the density in the neighborhood of a pixel using a simple cut off method
 *
 *
 * \ingroup PointSetFunctions
 *
 * \ingroup OTBDensity
 */
template <class TPointSet, class TOutput>
class ITK_EXPORT PointSetDensityFunction : public PointSetFunction<TPointSet, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef PointSetDensityFunction Self;
  typedef PointSetFunction<TPointSet, TOutput> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetDensityFunction, PointSetFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** PointSet Type typedef Support*/
  typedef TPointSet                      PointSetType;
  typedef typename Superclass::InputType InputType;
  typedef typename PointSetType::Pointer PointSetPointerType;

  /** TOutput typedef support*/
  typedef TOutput OutputType;

  /** Set/Get the number of scales*/
  itkSetMacro(Radius, unsigned int);
  itkGetMacro(Radius, unsigned int);

  /** Evaluate Method */
  OutputType Evaluate(const InputType& input) const override;

protected:
  PointSetDensityFunction() : m_Radius(1){};
  ~PointSetDensityFunction() override
  {
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  PointSetDensityFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  unsigned int m_Radius;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetDensityFunction.hxx"
#endif

#endif
