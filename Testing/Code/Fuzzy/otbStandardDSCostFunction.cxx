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


#include "otbStandardDSCostFunction.h"


int otbStandardDSCostFunctionNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef double                          PrecisionType;
  typedef otb::VectorData<PrecisionType>  VectorDataType;
  typedef otb::VectorDataToDSValidatedVectorDataFilter<VectorDataType, PrecisionType>
                                          VectorDataValidationFilterType;
  typedef otb::StandardDSCostFunction<VectorDataValidationFilterType>
                                          CostFunctionType;

  CostFunctionType::Pointer costFunction = CostFunctionType::New();

  std::cout<<costFunction<<std::endl;

  return EXIT_SUCCESS;
}
