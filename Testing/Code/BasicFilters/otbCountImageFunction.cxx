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

#include "otbCountImageFunction.h"
#include "otbSiftFastImageFilter.h"
#include "otbSimplePointCountStrategy.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "otbImage.h"

int otbCountImageFunctionNew(int, char* [] )
{

  const   unsigned int                                      Dimension = 2;
  typedef float                                             PixelType; 

  typedef otb::Image< PixelType, Dimension >                ImageType;
  typedef ImageType::IndexType                              IndexType;
  typedef itk::VariableLengthVector<PixelType>              RealVectorType;
  typedef itk::PointSet<RealVectorType,Dimension>           PointSetType;
  typedef otb::SiftFastImageFilter<ImageType,PointSetType>  DetectorType;
  
  typedef otb::Count<PointSetType,unsigned int ,IndexType>  CounterType;
  
  typedef otb::CountImageFunction< ImageType,DetectorType,
                                             CounterType>   FunctionType;
  
  /**Instancitation of an object*/
  FunctionType::Pointer    filter =     FunctionType::New();

  return EXIT_SUCCESS;
}

