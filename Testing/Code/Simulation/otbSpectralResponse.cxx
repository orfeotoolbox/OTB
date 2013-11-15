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

#include "otbMacro.h"

#include "otbSpectralResponse.h"
#include "otbVectorImage.h"

int otbSpectralResponse(int argc, char * argv[])
{
  typedef otb::SpectralResponse< double, double>  ResponseType;
  typedef ResponseType::Pointer  ResponsePointerType;
  typedef otb::VectorImage<double, 2> ImageType;

  if ( argc!=3 )
  {
    std::cout << argv[0] << std::endl << "\t" << "<Spectrum_filename>"  << "\t" << "<Lambda>"<< std::endl;
    return EXIT_FAILURE;
  }
  //Instantiation
  ResponsePointerType  myResponse=ResponseType::New();
  //Load file into vector
  const std::string file(argv[1]);
  myResponse->Load(file, 100.0);
  //itk::Indent ind;
  double lambda = atof(argv[2]);

  ImageType::Pointer image = ImageType::New();
  myResponse->GetImage(image);

  std::cout<<myResponse<<std::endl;
  std::cout << "SpectResponse("<<lambda<<")= " << (*myResponse)(lambda) << std::endl;
  return EXIT_SUCCESS;
}
