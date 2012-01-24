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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

//Code adapted from submission from Jordi INGLADA
// http://bugs.orfeo-toolbox.org/view.php?id=132

int main(int argc, char *argv[])
{
  if (argc < 1)
    {
    std::cout << "Usage : inputImage" << std::endl;
    return EXIT_FAILURE;
    }

  char * filename = argv[1];

  typedef double                          PixelType;
  typedef otb::VectorImage<PixelType>     ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  // check for input images
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->UpdateOutputInformation();
  std::cout << reader << std::endl;

  return EXIT_SUCCESS;
}
