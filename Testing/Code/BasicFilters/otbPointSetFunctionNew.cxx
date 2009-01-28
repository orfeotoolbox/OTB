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

#include <stdio.h>

#include "otbPointSetFunction.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"


int otbPointSetFunctionNew(int, char* [] )
{

  const   unsigned int                                      Dimension = 2;
  typedef float                                             PixelType; 

  typedef itk::VariableLengthVector<PixelType>              RealVectorType;
  typedef itk::PointSet<RealVectorType,Dimension>           PointSetType;
  typedef otb::PointSetFunction <PointSetType,PixelType>   FunctionType;
  
  /**Instancitation of an object*/
  FunctionType   filter();

  return EXIT_SUCCESS;
}

