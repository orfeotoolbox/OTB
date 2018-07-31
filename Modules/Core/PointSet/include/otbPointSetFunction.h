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

#ifndef otbPointSetFunction_h
#define otbPointSetFunction_h

#include "itkSpatialFunction.h"
#include "itkProcessObject.h"

namespace otb
{

/**
 * \class PointSetFunction
 * \brief Calculate the density in the neighborhood of a pixel
 *
 * \ingroup SpatialFunctions
 *
 * \ingroup OTBPointSet
 */
template <class TPointSet, class TOutput>
class ITK_EXPORT PointSetFunction :
  public itk::SpatialFunction<TOutput, 2 /* TODO : change 2 by PointType::PointDimension*/,
      typename TPointSet::PointType>
{
public:
  /** Standard class typedefs. */
  typedef PointSetFunction                                                 Self;
  typedef itk::SpatialFunction<TOutput, 2,  typename TPointSet::PointType> Superclass;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetFunction, itk::SpatialFunction);

  /** PointSet Type typedef Support*/
  typedef TPointSet                              PointSetType;
  typedef typename  PointSetType::Pointer        PointSetPointer;
  typedef typename  PointSetType::ConstPointer   PointSetConstPointer;
  /** TOutput typedef support*/
  typedef TOutput OutputType;

  /** Set the input image (reimplemented since we need to set the detector input) */
  itkGetConstObjectMacro(PointSet, PointSetType);

  void SetPointSet(PointSetPointer PointSet)
  {
    m_PointSet = PointSet;
  }

  /** SetPointSet() to maintain the const correctness of the pointset*/
  void SetPointSet(PointSetConstPointer PointSet)
  {
    m_PointSet = PointSet;
  }

protected:
  PointSetFunction();
  ~PointSetFunction() override {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  PointSetFunction(const Self &) = delete;
  void operator =(const Self&) = delete;

  PointSetConstPointer m_PointSet;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetFunction.hxx"
#endif

#endif
