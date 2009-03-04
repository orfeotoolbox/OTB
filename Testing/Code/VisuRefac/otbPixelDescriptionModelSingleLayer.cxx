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
#include "otbPixelDescriptionModel.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageLayerGenerator.h"
#include "otbImageLayer.h"

int otbPixelDescriptionModelSingleLayer( int argc, char * argv[] )
{
  // params
  const char * infname = argv[1];

  typedef otb::VectorImage<double,2>                             ImageType;
  typedef otb::ImageLayer<ImageType>                             LayerType;
  typedef otb::ImageFileReader<ImageType>                        ReaderType;
  typedef otb::ImageLayerGenerator<LayerType>                    LayerGeneratorType;
  typedef otb::PixelDescriptionModel<LayerType::OutputImageType> ModelType;

  // Instantiation
  ModelType::Pointer model = ModelType::New();

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  
  // Generate the layer
  LayerGeneratorType::Pointer generator = LayerGeneratorType::New();
  generator->SetImage(reader->GetOutput());
  generator->GenerateLayer();

  // Add the layer to the model
  model->AddLayer(generator->GetLayer());

  // Compute extract and scaled extract region
  ImageType::RegionType lregion = reader->GetOutput()->GetLargestPossibleRegion();
  ImageType::RegionType::IndexType index;

  index[0] = lregion.GetSize()[0]/4;
  index[1] = lregion.GetSize()[1]/4;

  model->UpdatePixelDescription(index);

  std::cout<<"Pixel description: "<<std::endl;
  std::cout<<model->GetPixelDescription()<<std::endl;

  return EXIT_SUCCESS;
}
