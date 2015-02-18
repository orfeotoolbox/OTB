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
#include "otbVectorImage.h"
#include "itkRGBPixel.h"
#include "itkMacro.h"
#include <iostream>

int otbPNGIndexedNbBandsTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char *       inputFilename    = argv[1];
  const unsigned int nbBands    = atoi(argv[2]);

  typedef itk::RGBPixel<unsigned char> InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);

  reader->UpdateOutputInformation();

  if (reader->GetOutput()->GetNumberOfComponentsPerPixel() == nbBands)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    std::cout << "Read " << reader->GetOutput()->GetNumberOfComponentsPerPixel() <<
    " but the real number of bands is " << nbBands << std::endl;
    return EXIT_FAILURE;
    }
}
