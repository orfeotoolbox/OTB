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
#include "otbImageLayer.h"
#include "otbRGBRenderingFunction.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbStreamingShrinkImageFilter.h"

int otbImageLayerVector( int argc, char * argv[] )
{
  
  const char * infname      = argv[1];
  const char * qloutfname   = argv[2];
  const char * extoutfname  = argv[3];
  const char * sextoutfname = argv[4];
  const unsigned int ssrate = atoi(argv[5]);

  typedef double                                                            ScalarType;
  typedef otb::VectorImage<ScalarType,2>                                    ImageType;
  typedef ImageType::PixelType                                              VectorPixelType;
  typedef otb::ImageLayer<ImageType>                                        LayerType;
  typedef LayerType::OutputImageType                                        OutputImageType;
  typedef OutputImageType::PixelType                                        OutputPixelType;
  typedef otb::ImageFileReader<ImageType>                                   ReaderType;
  typedef otb::StreamingShrinkImageFilter<ImageType,ImageType>              ShrinkFilterType;
  typedef otb::ImageFileWriter<OutputImageType>                             WriterType;
  typedef otb::Function::RGBRenderingFunction<double,OutputPixelType>       RenderingFunctionType;
  // Instantiation
  ReaderType::Pointer reader = ReaderType::New();
  ShrinkFilterType::Pointer shrinker = ShrinkFilterType::New();

  // Input iamge
  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();

  unsigned int nbComponents = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  // min & max
  VectorPixelType min(nbComponents);
  VectorPixelType max(nbComponents);

  for(unsigned int i = 0; i<nbComponents;++i)
    {
    min[i]=atof(argv[6+i]);
    max[i]=atof(argv[6+nbComponents+i]);
    }


  // Quicklook
  shrinker->SetInput(reader->GetOutput());
  shrinker->SetShrinkFactor(ssrate);

  // new layer
  LayerType::Pointer layer = LayerType::New();
  layer->SetName(infname);
  layer->SetExtent(reader->GetOutput()->GetLargestPossibleRegion());
  layer->SetVisible(true);
  layer->SetHasQuicklook(true);
  layer->SetQuicklook(shrinker->GetOutput());
  layer->SetQuicklookSubsamplingRate(ssrate);
  layer->SetQuicklookSize(shrinker->GetOutput()->GetLargestPossibleRegion().GetSize());
  layer->SetImage(reader->GetOutput());
  layer->SetHasExtract(true);
  layer->SetHasScaledExtract(true);
  RenderingFunctionType::Pointer function = RenderingFunctionType::New();
  function->SetMinimum(min);
  function->SetMaximum(max);
  layer->SetRenderingFunction(function);

  ImageType::RegionType lregion = reader->GetOutput()->GetLargestPossibleRegion();
  ImageType::RegionType::IndexType index;
  ImageType::RegionType::SizeType  size;

  size[0]=lregion.GetSize()[0]/2;
  size[1]=lregion.GetSize()[1]/2;

  index[0] = lregion.GetSize()[0]/4;
  index[1] = lregion.GetSize()[1]/4;
  
  ImageType::RegionType extractRegion;
  extractRegion.SetSize(size);
  extractRegion.SetIndex(index);
  
  layer->SetExtractRegion(extractRegion);
  
  size[0]=lregion.GetSize()[0]/4;
  size[1]=lregion.GetSize()[1]/4;

  index[0] = 3 * lregion.GetSize()[0]/8;
  index[1] = 3 * lregion.GetSize()[1]/8;
  
  ImageType::RegionType sextractRegion;
  sextractRegion.SetSize(size);
  sextractRegion.SetIndex(index);
  
  layer->SetScaledExtractRegion(sextractRegion);
  
  // Render the layer
  layer->Render();

  // Write the image views
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(layer->GetRenderedQuicklook());
  writer->SetFileName(qloutfname);
  writer->Update();

  writer = WriterType::New();
  writer->SetInput(layer->GetRenderedExtract());
  writer->SetFileName(extoutfname);
  writer->Update();
  
  writer = WriterType::New();
  writer->SetInput(layer->GetRenderedScaledExtract());
  writer->SetFileName(sextoutfname);
  writer->Update();
  
  return EXIT_SUCCESS;
}
