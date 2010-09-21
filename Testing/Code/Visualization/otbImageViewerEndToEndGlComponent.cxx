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
#include "otbChangeScaleActionHandler.h"

#include "otbCrossGlComponent.h"
#include "otbCircleGlComponent.h"

int otbImageViewerEndToEndGlComponent(int argc, char * argv[])
{
  // params
  const char *       infname = argv[1];
  const unsigned int scrollSize = atoi(argv[2]);
  const unsigned int fullSize = atoi(argv[3]);
  const unsigned int zoomSize = atoi(argv[4]);
  const double       run      = atoi(argv[5]);

  // typedefs
  typedef double                                         PixelType;
  typedef itk::RGBPixel<unsigned char>                   RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>                    OutputImageType;
  typedef otb::VectorImage<PixelType, 2>                 ImageType;
  typedef otb::ImageLayer<ImageType, OutputImageType>    LayerType;
  typedef otb::ImageFileReader<ImageType>                ReaderType;
  typedef otb::ImageLayerGenerator<LayerType>            LayerGeneratorType;
  typedef otb::ImageLayerRenderingModel<OutputImageType> ModelType;
  typedef otb::ImageView<ModelType>                      ViewType;
  typedef otb::ImageWidgetController                     ControllerType;
  typedef otb::WidgetResizingActionHandler
  <ModelType, ViewType>                             ResizingHandlerType;
  typedef otb::ChangeScaledExtractRegionActionHandler
  <ModelType, ViewType>                             ChangeScaledRegionHandlerType;
  typedef otb::ChangeExtractRegionActionHandler
  <ModelType, ViewType>                             ChangeRegionHandlerType;
  typedef otb::ChangeScaleActionHandler
  <ModelType, ViewType>                             ChangeScaleHandlerType;

  // Instantiation
  ModelType::Pointer                 model = ModelType::New();

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

  otb::CrossGlComponent::IndexType crossIndex;
  crossIndex[0] = 100;
  crossIndex[1] = 100;
  otb::CrossGlComponent::ColorType crossColor;
  crossColor[0] = 0.9;
  crossColor[1] = 0.6;
  crossColor[2] = 0.1;
  crossColor[3] = 1.0;
  otb::CrossGlComponent::Pointer crossGlComponent = otb::CrossGlComponent::New();
  crossGlComponent->AddIndex(crossIndex);
  crossGlComponent->ChangeColor(crossColor, crossGlComponent->GetColorList().size() - 1);
  view->GetFullWidget()->AddGlComponent(crossGlComponent);
  view->GetScrollWidget()->AddGlComponent(crossGlComponent);
  view->GetZoomWidget()->AddGlComponent(crossGlComponent);

  crossIndex[0] = 100;
  crossIndex[1] = 150;
  crossColor[0] = 0.1;
  crossColor[1] = 0.9;
  crossColor[2] = 0.1;
  crossColor[3] = 1.0;
  otb::CrossGlComponent::Pointer crossGlComponent2 = otb::CrossGlComponent::New();
  crossGlComponent2->AddIndex(crossIndex);
  crossGlComponent2->ChangeColor(crossColor, crossGlComponent2->GetColorList().size() - 1);
  crossGlComponent2->SetLineWidth(10.0);
  crossGlComponent2->SetCrossWidth(40.0);
  view->GetFullWidget()->AddGlComponent(crossGlComponent2);
  view->GetScrollWidget()->AddGlComponent(crossGlComponent2);
  view->GetZoomWidget()->AddGlComponent(crossGlComponent2);

  otb::CircleGlComponent::IndexType circleIndex;
  circleIndex[0] = 150;
  circleIndex[1] = 100;
  otb::CircleGlComponent::ColorType circleColor;
  circleColor[0] = 0.9;
  circleColor[1] = 0.2;
  circleColor[2] = 0.1;
  circleColor[3] = 1.0;
  otb::CircleGlComponent::Pointer circleGlComponent = otb::CircleGlComponent::New();
  circleGlComponent->AddIndex(circleIndex);
  circleGlComponent->ChangeColor(circleColor, circleGlComponent->GetColorList().size() - 1);
  view->GetFullWidget()->AddGlComponent(circleGlComponent);
  view->GetScrollWidget()->AddGlComponent(circleGlComponent);
  view->GetZoomWidget()->AddGlComponent(circleGlComponent);

  circleIndex[0] = 150;
  circleIndex[1] = 150;
  circleColor[0] = 0.2;
  circleColor[1] = 0.2;
  circleColor[2] = 1.0;
  circleColor[3] = 1.0;

  otb::CircleGlComponent::Pointer circleGlComponent2 = otb::CircleGlComponent::New();
  circleGlComponent2->AddIndex(circleIndex);
  circleGlComponent2->ChangeColor(circleColor, circleGlComponent2->GetColorList().size() - 1);
  circleGlComponent2->SetRadius(40);
  circleGlComponent2->SetLineWidth(3.0);
  circleGlComponent2->SetCenterRepresentation(otb::CircleGlComponent::CIRCLE);

  view->GetFullWidget()->AddGlComponent(circleGlComponent2);
  view->GetScrollWidget()->AddGlComponent(circleGlComponent2);
  view->GetZoomWidget()->AddGlComponent(circleGlComponent2);

  // Add the resizing handler
  ResizingHandlerType::Pointer resizingHandler = ResizingHandlerType::New();
  resizingHandler->SetModel(model);
  resizingHandler->SetView(view);
  controller->AddActionHandler(resizingHandler);

  // Add the change scaled region handler
  ChangeScaledRegionHandlerType::Pointer changeScaledHandler = ChangeScaledRegionHandlerType::New();
  changeScaledHandler->SetModel(model);
  changeScaledHandler->SetView(view);
  controller->AddActionHandler(changeScaledHandler);

  // Add the change extract region handler
  ChangeRegionHandlerType::Pointer changeHandler = ChangeRegionHandlerType::New();
  changeHandler->SetModel(model);
  changeHandler->SetView(view);
  controller->AddActionHandler(changeHandler);

  // Add the change scaled handler
  ChangeScaleHandlerType::Pointer changeScaleHandler = ChangeScaleHandlerType::New();
  changeScaleHandler->SetModel(model);
  changeScaleHandler->SetView(view);
  controller->AddActionHandler(changeScaleHandler);

  Fl_Window scrollWindow(scrollSize, scrollSize);
  if (scrollSize > 0)
    {
    scrollWindow.add(view->GetScrollWidget());
    scrollWindow.resizable(view->GetScrollWidget());
    scrollWindow.show();
    view->GetScrollWidget()->show();
    view->GetScrollWidget()->resize(0, 0, scrollSize, scrollSize);
    }

  Fl_Window fullWindow(fullSize, fullSize);
  if (fullSize > 0)
    {
    fullWindow.add(view->GetFullWidget());
    fullWindow.resizable(view->GetFullWidget());
    fullWindow.show();
    view->GetFullWidget()->show();
    view->GetFullWidget()->resize(0, 0, fullSize, fullSize);
    }

  Fl_Window zoomWindow(zoomSize, zoomSize);
  if (zoomSize > 0)
    {
    zoomWindow.add(view->GetZoomWidget());
    zoomWindow.resizable(view->GetZoomWidget());
    zoomWindow.show();
    view->GetZoomWidget()->show();
    view->GetZoomWidget()->resize(0, 0, zoomSize, zoomSize);
    }



  if (run)
    {
    Fl::run();
    }
  else
    {
    Fl::check();
    }

  zoomWindow.remove(view->GetZoomWidget());
  scrollWindow.remove(view->GetScrollWidget());
  fullWindow.remove(view->GetFullWidget());
//  pixelWindow.remove(pixelView->GetPixelDescriptionWidget());

  return EXIT_SUCCESS;
}
