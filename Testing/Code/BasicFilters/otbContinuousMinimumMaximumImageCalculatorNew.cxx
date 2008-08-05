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
#include "itkExceptionObject.h"

#include "otbContinuousMinimumMaximumImageCalculator.h"
#include "otbImage.h"

int otbContinuousMinimumMaximumImageCalculatorNew( int argc, char * argv[])
{
        const unsigned int Dimension = 2;
        typedef unsigned char PixelType;
      
        typedef otb::Image<PixelType,Dimension> ImageType;
        typedef otb::ContinuousMinimumMaximumImageCalculator<ImageType> ContinuousMinimumMaximumImageCalculatorType;

        // Instantiating object
        ContinuousMinimumMaximumImageCalculatorType::Pointer object = ContinuousMinimumMaximumImageCalculatorType::New();
        return EXIT_SUCCESS;
}
