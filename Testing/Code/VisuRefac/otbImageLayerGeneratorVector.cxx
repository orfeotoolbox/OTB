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
#include "otbImageLayerGenerator.h"
#include "otbImageLayer.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"

int otbImageLayerGeneratorVector( int argc, char * argv[] )
{
  typedef otb::VectorImage<double,2>          ImageType;
  typedef otb::ImageFileReader<ImageType>     ReaderType;
  typedef otb::ImageLayer<ImageType>          LayerType;
  typedef otb::ImageLayerGenerator<LayerType> GeneratorType;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  
  // Generator
  GeneratorType::Pointer generator = GeneratorType::New();
  generator->SetImage(reader->GetOutput());
  generator->SetGenerateQuicklook(atoi(argv[2]));
  std::cout<<"Advised quicklook susbsampling rate: "<<generator->GetOptimalSubSamplingRate()<<std::endl;
  generator->GenerateLayer();

  return EXIT_SUCCESS;
}
