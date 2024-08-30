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

#ifndef otbGeometricMomentPathFunction_h
#define otbGeometricMomentPathFunction_h

#include "otbPathFunction.h"

namespace otb
{

/**
 * \class GeometricMomentPathFunction
 * \brief Virtual class for the Geometric moments for an path function
 *
 * \ingroup PathFunctions
 *
 * \ingroup OTBMoments
 */

template <class TInputPath, class TOutput, class TPrecision = double>
class ITK_EXPORT GeometricMomentPathFunction : public PathFunction<TInputPath, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef GeometricMomentPathFunction Self;
  typedef PathFunction<TInputPath, TOutput> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometricMomentPathFunction, PathFunction);

  /** InputPathType typedef support. */
  typedef typename Superclass::InputPathType         PathType;
  typedef typename Superclass::InputPathConstPointer PathConstPointer;

  /** OutputPathType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** Type for calculation precision */
  typedef TPrecision PrecisionType;

protected:
  GeometricMomentPathFunction(){};
  ~GeometricMomentPathFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  GeometricMomentPathFunction(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // namespace otb

#endif
