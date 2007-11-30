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

#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbAtmosphericRadiativeTerms.h"

int otbReflectanceToSurfaceReflectanceImageFilter(int argc, char * argv[])
{
  try
    {
      const char * inputFileName  = argv[1];
      const char * outputFileName = argv[2];


      const unsigned int Dimension = 2;
      typedef double PixelType;
      typedef otb::VectorImage<PixelType,Dimension> InputImageType;
      typedef otb::VectorImage<PixelType,Dimension> OutputImageType;
      typedef otb::ImageFileReader<InputImageType>  ReaderType;
      typedef otb::ImageFileWriter<OutputImageType> WriterType;
      typedef otb::ReflectanceToSurfaceReflectanceImageFilter<InputImageType, OutputImageType> ReflectanceToSurfaceReflectanceImageFilterType;
      otb::AtmosphericRadiativeTerms::Pointer atmo = otb::AtmosphericRadiativeTerms::New();


      ReaderType::Pointer reader  = ReaderType::New();
      WriterType::Pointer writer = WriterType::New();
      reader->SetFileName(inputFileName);
      writer->SetFileName(outputFileName);

      atmo->SetIntrinsicAtmosphericReflectance(static_cast<double>(atof(argv[3])));
      atmo->SetSphericalAlbedo(static_cast<double>(atof(argv[4])));
      atmo->SetTotalGaseousTransmission(static_cast<double>(atof(argv[5])));
      atmo->SetDownwardTransmittance(static_cast<double>(atof(argv[6])));
      atmo->SetUpwardTransmittance(static_cast<double>(atof(argv[7])));

      

      // Instantiating object
      ReflectanceToSurfaceReflectanceImageFilterType::Pointer filter = ReflectanceToSurfaceReflectanceImageFilterType::New();
      filter->SetAtmosphericRadiativeTerms(atmo);
      filter->SetInput(reader->GetOutput());
      writer->SetInput(filter->GetOutput());

      writer->Update();

    }

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  catch( ... ) 
    { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
