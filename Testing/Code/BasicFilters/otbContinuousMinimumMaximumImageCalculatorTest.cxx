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
#include "otbImageFileReader.h"
#include "otbImage.h"

int otbContinuousMinimumMaximumImageCalculatorTest( int argc, char * argv[])
{
        const char * infname = argv[1];
        const unsigned int Dimension = 2;
        typedef unsigned char PixelType;
      
        typedef otb::Image<PixelType,Dimension> ImageType;
        typedef otb::ContinuousMinimumMaximumImageCalculator<ImageType> ContinuousMinimumMaximumImageCalculatorType;
        typedef otb::ImageFileReader<ImageType> ReaderType;

        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName(infname);
        
        //Tes with a small image
        reader->Update();

        ContinuousMinimumMaximumImageCalculatorType::Pointer continuous = ContinuousMinimumMaximumImageCalculatorType::New();
        continuous->SetImage(reader->GetOutput());
        //Compute values
//        continuous->ComputeMinimum();
// PB Timeout. Test provisoirement en FAILURE
        
//        std::cout << continuous->GetMinimum() <<std::endl;
        
        
        
        return EXIT_FAILURE;
}
