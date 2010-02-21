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
#include "otbImageLayerRenderingModel.h"
#include "otbVectorImage.h"
#include "itkRGBAPixel.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageLayerGenerator.h"
#include "otbImageFileWriter.h"
#include "otbImageLayer.h"

int otbImageLayerRenderingModelSingleLayer( int argc, char * argv[] )
{
  // params
  const char * infname = argv[1];
  const char * qloutfname   = argv[2];
  const char * extoutfname  = argv[3];
  const char * sextoutfname = argv[4];

  // typedefs
  typedef itk::RGBAPixel<unsigned char>               RGBPixelType;
  typedef otb::Image<RGBPixelType,2>                 OutputImageType;
  typedef otb::VectorImage<double,2>                 ImageType;
  typedef otb::ImageLayer<ImageType,OutputImageType> LayerType;
  typedef otb::ImageFileReader<ImageType>            ReaderType;
  typedef otb::ImageLayerGenerator<LayerType>        LayerGeneratorType;
  typedef otb::ImageLayerRenderingModel<OutputImageType>     ModelType;
  typedef otb::ImageFileWriter<OutputImageType>      WriterType;
  typedef LayerGeneratorType::ResampleFilterType     ResampleFilterType;

  // Instantiation
  ModelType::Pointer model = ModelType::New();

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  // Resampling (generator resampling depends on screen resolution, so
  // we are generating quicklook outside of the generator)
  ResampleFilterType::Pointer resampler = ResampleFilterType::New();
  resampler->SetInput(reader->GetOutput());
  resampler->SetShrinkFactor(atoi(argv[5]));
  resampler->Update();

  // Generate the layer
  LayerGeneratorType::Pointer generator = LayerGeneratorType::New();
  generator->GenerateQuicklookOff();
  generator->SetSubsamplingRate(atoi(argv[5]));
  generator->SetQuicklook(resampler->GetOutput());
  generator->SetImage(reader->GetOutput());
  generator->GenerateLayer();

  // Add the layer to the model
  model->AddLayer(generator->GetLayer());

  // Layer manipulation test
  ModelType::LayerType::Pointer layer = model->GetLayer(0);
  std::string layerName = layer->GetName();
  ModelType::LayerType::Pointer layerByName = model->GetLayerByName(layerName);
  if (layer != layerByName)
  {
    return EXIT_FAILURE;
  }


  // Copute extract and scaled extract region
  ImageType::RegionType lregion = reader->GetOutput()->GetLargestPossibleRegion();
  ImageType::RegionType::IndexType index;
  ImageType::RegionType::SizeType  size;

  size[0]=100;
  size[1]=100;

  index[0] = lregion.GetSize()[0]/4;
  index[1] = lregion.GetSize()[1]/4;

  ImageType::RegionType extractRegion;
  extractRegion.SetSize(size);
  extractRegion.SetIndex(index);

  size[0]=25;
  size[1]=25;

  index[0] = 3 * lregion.GetSize()[0]/8;
  index[1] = 3 * lregion.GetSize()[1]/8;

  ImageType::RegionType sextractRegion;
  sextractRegion.SetSize(size);
  sextractRegion.SetIndex(index);

  // Set the regions
  model->SetExtractRegion(extractRegion);
  model->SetScaledExtractRegion(sextractRegion);

  // Render
  model->Update();

  // Write the rasterized images
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(model->GetRasterizedQuicklook());
  writer->SetFileName(qloutfname);
  writer->Update();

  std::cout<<"Quicklook saved."<<std::endl;

  writer = WriterType::New();
  writer->SetInput(model->GetRasterizedExtract());
  writer->SetFileName(extoutfname);
  writer->Update();

  std::cout<<"Extract saved."<<std::endl;

  writer = WriterType::New();
  writer->SetInput(model->GetRasterizedScaledExtract());
  writer->SetFileName(sextoutfname);
  writer->Update();

  std::cout<<"Scaled extract saved."<<std::endl;

  // Layer manipulation test
  model->DeleteLayerByName(layerName);
  model->AddLayer(generator->GetLayer());
  model->AddLayer(generator->GetLayer());
  model->DeleteLayer(0);
  model->DeleteLayer(0);
  model->ClearLayers();

  return EXIT_SUCCESS;
}
