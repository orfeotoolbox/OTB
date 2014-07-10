/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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

int otbWindowedSincInterpolateImageFunctionBaseNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef otb::Image<double, 2>                                                  ImageType;
  typedef Function::SameFunction<double, double>                                 FunctionType;
  typedef otb::WindowedSincInterpolateImageFunctionBase<ImageType, FunctionType> WindowedSincInterpolatorType;

  // Instantiating object
  WindowedSincInterpolatorType::Pointer winInterp = WindowedSincInterpolatorType::New();

  std::cout << winInterp << std::endl;

  return EXIT_SUCCESS;
}
