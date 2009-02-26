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
#include "otbImageViewerModel.h"
#include "otbVectorImage.h"
#include "itkRGBPixel.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageLayerGenerator.h"
#include "otbImageLayer.h"
#include "otbImageView.h"
#include <FL/Fl.H>
#include "otbImageWidgetController.h"
#include "otbWidgetResizingActionHandler.h"
#include "otbChangeScaledExtractRegionActionHandler.h"
#include "otbChangeExtractRegionActionHandler.h"


int otbImageViewerEndToEndSingleLayer( int argc, char * argv[] )
{
  // params
  const char * infname = argv[1];
  const unsigned int scrollSize = atoi(argv[2]);
  const unsigned int fullSize = atoi(argv[3]);
  const unsigned int zoomSize = atoi(argv[4]);
  const double       run      = atoi(argv[5]);


  // typedefs 
  typedef itk::RGBPixel<unsigned char>               RGBPixelType;
  typedef otb::Image<RGBPixelType,2>                 OutputImageType;
  typedef otb::VectorImage<double,2>                 ImageType;
  typedef otb::ImageLayer<ImageType>                 LayerType;
  typedef otb::ImageFileReader<ImageType>            ReaderType;
  typedef otb::ImageLayerGenerator<LayerType>        LayerGeneratorType;
  typedef otb::ImageViewerModel<OutputImageType>     ModelType;
  typedef otb::ImageView<ModelType>                  ViewType;
  typedef otb::ImageWidgetController                 ControllerType;
  typedef otb::WidgetResizingActionHandler
    <ModelType,ViewType>                             ResizingHandlerType;
  typedef otb::ChangeScaledExtractRegionActionHandler
    <ModelType,ViewType>                             ChangeScaledRegionHandlerType;
  typedef otb::ChangeExtractRegionActionHandler
    <ModelType,ViewType>                             ChangeRegionHandlerType;

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
  
  // Build a view
  ViewType::Pointer view = ViewType::New();
  view->SetModel(model);

  // Build a controller
  ControllerType::Pointer controller = ControllerType::New();
  view->SetController(controller);

  // Add the resizing handler
  ResizingHandlerType::Pointer resizingHandler = ResizingHandlerType::New();
  resizingHandler->SetModel(model);
  resizingHandler->SetView(view);
  controller->AddActionHandler(resizingHandler);

// Add the change scaled region handler
  ChangeScaledRegionHandlerType::Pointer changeScaledHandler =ChangeScaledRegionHandlerType::New();
  changeScaledHandler->SetModel(model);
  changeScaledHandler->SetView(view);
  controller->AddActionHandler(changeScaledHandler);

  // Add the change extract region handler
  ChangeRegionHandlerType::Pointer changeHandler =ChangeRegionHandlerType::New();
  changeHandler->SetModel(model);
  changeHandler->SetView(view);
  controller->AddActionHandler(changeHandler);

  view->GetScrollWidget()->show();
  view->GetScrollWidget()->resize(fullSize,0,scrollSize,scrollSize);
  Fl::check();
  
  view->GetFullWidget()->show();
  view->GetFullWidget()->resize(0,0,fullSize,fullSize);

  Fl::check();

  view->GetZoomWidget()->show();
  view->GetZoomWidget()->resize(fullSize,scrollSize,zoomSize,zoomSize);
 
  if(run)
    {
    Fl::run();
    }
  else
    {
    Fl::check();
    }

  return EXIT_SUCCESS;
}
