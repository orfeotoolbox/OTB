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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif


#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include <iostream>

#include "otbSVMImageModelEstimator.h"


int otbSVMImageModelEstimatorNew( int argc, char* argv[] )
{
  typedef double                               InputPixelType;
  const   unsigned int        	                    Dimension = 2;

  typedef otb::VectorImage< InputPixelType,  Dimension >        InputImageType;

  typedef otb::Image< InputPixelType,  Dimension >     TrainingImageType;

  typedef std::vector<double>   VectorType;

  typedef otb::SVMImageModelEstimator< InputImageType,
    TrainingImageType >   EstimatorType;


  EstimatorType::Pointer svmModel = EstimatorType::New();


  return EXIT_SUCCESS;
}


