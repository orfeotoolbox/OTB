/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRealMomentPathFunction_h
#define otbRealMomentPathFunction_h

#include "otbGeometricMomentPathFunction.h"
#include "itkVectorContainer.h"

namespace otb
{

/**
 * \class RealMomentPathFunction
 * \brief Virtual class for the Real moments for a path
 *
 * \ingroup PathFunctions
 *
 * \ingroup OTBMoments
 */

template <class TInputPath, class TOutput = double, class TPrecision = double>
class ITK_EXPORT RealMomentPathFunction : public GeometricMomentPathFunction<TInputPath, TOutput, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef RealMomentPathFunction Self;
  typedef GeometricMomentPathFunction<TInputPath, TOutput, TPrecision> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RealMomentPathFunction, GeometricMomentPathFunction);

  /** InputPathType typedef support. */
  typedef typename Superclass::PathType          PathType;
  typedef typename Superclass::PathConstPointer  PathConstPointer;
  typedef typename PathType::ContinuousIndexType VertexType;
  typedef itk::VectorContainer<unsigned, VertexType> VertexListType;
  typedef typename VertexListType::ConstPointer VertexListPointer;

  /** Type for calculation precision */
  typedef typename Superclass::PrecisionType PrecisionType;

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType RealType;

protected:
  RealMomentPathFunction()
  {
  }
  ~RealMomentPathFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  RealMomentPathFunction(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // namespace otb

#endif
