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

#include "otbGenericInterpolateImageFunction.h"
#include "otbImage.h"
#include "itkConstantBoundaryCondition.h"

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

int otbGenericInterpolateImageFunctionNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType, Dimension>                  ImageType;
  typedef Function::SameFunction<InputPixelType, InputPixelType> FunctionType;
  typedef itk::ConstantBoundaryCondition<ImageType>              BoundaryConditionType;
  typedef double                                                 CoordRepType;

  typedef otb::GenericInterpolateImageFunction<ImageType, FunctionType, BoundaryConditionType,
      CoordRepType> GenericFunctionType;

  // Instantiating object
  GenericFunctionType::Pointer generic = GenericFunctionType::New();

  std::cout << generic << std::endl;

  return EXIT_SUCCESS;
}
