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

#include "itkMacro.h"

#include "otbWindowedSincInterpolateImageFunctionBase.h"
#include "itkConstantBoundaryCondition.h"
#include "otbImage.h"

namespace Function
{
template<class TInput = double, class TOutput = double>
class SameFunction
{
public:
  void SetRadius(unsigned int rad)
  {
    m_Radius = rad;
  }
  unsigned int GetRadius() const
  {
    return m_Radius;
  }
  inline TOutput operator ()(const TInput& A) const
  {
    return static_cast<TOutput>(A);
  }
  unsigned int m_Radius;
};

}

int otbWindowedSincInterpolateImageFunctionBaseNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Image<double, 2>                                                  ImageType;
  typedef Function::SameFunction<double, double>                                 FunctionType;
  typedef otb::WindowedSincInterpolateImageFunctionBase<ImageType, FunctionType> WindowedSincInterpolatorType;

  // Instantiating object
  WindowedSincInterpolatorType::Pointer winInterp = WindowedSincInterpolatorType::New();

  std::cout << winInterp << std::endl;

  return EXIT_SUCCESS;
}
