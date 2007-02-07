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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"
#include "otbStreamingShrinkImageFilter.h"


int otbStreamingShrinkImageFilter( int argc, char * argv[] )
{
  try 
    { 
      char * inputFilename = argv[1];      
      char * outputFilename = argv[2];
      unsigned int shrinkFactor = atoi(argv[3]);
      const unsigned int Dimension = 2;

      typedef unsigned int PixelType;
      typedef otb::VectorImage<PixelType,Dimension> ImageType;
      typedef otb::ImageFileReader<ImageType> ReaderType;
      typedef otb::ImageFileWriter<ImageType> WriterType;
      typedef otb::StreamingShrinkImageFilter<ImageType> ShrinkType;

      ReaderType::Pointer reader = ReaderType::New();
      ShrinkType::Pointer shrink = ShrinkType::New();
      WriterType::Pointer writer = WriterType::New();

      reader->SetFileName(inputFilename);
      writer->SetFileName(outputFilename);
      shrink->SetShrinkFactor(shrinkFactor);

      shrink->SetInput(reader->GetOutput());
      writer->SetInput(shrink->GetOutput());

      writer->Update();
     } 

  catch( itk::ExceptionObject & err ) 
    { 
      std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
      std::cout << err << std::endl; 
      return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
      std::cout << "Exception levee inconnue !" << std::endl; 
      return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
