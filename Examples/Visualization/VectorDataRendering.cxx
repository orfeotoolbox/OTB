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

//  Software Guide : BeginCommandLineArgs
//  INPUTS: {${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}, {${INPUTLARGEDATA}/VECTOR/MidiPyrenees/roads.shp}
//  ${OTB_DATA_ROOT}/Examples/DEM_srtm 1 ${OTB_DATA_ROOT}/Baseline/OTB/Files/DejaVuSans.ttf
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example shows how to combine vector data and combine them with an image. Typically, the
// vector data would be a shapefile containing information from open street map (roads, etc), the
// image would be a high resolution image.
//
// This example is able to reproject the vector data on the fly, render them using mapnik (including
// road names) and display it as an overlay to a Quickbird image.
//
// For now the code is a bit convoluted, but it is helpful to illustrate the possibilities that it
// opens up.
//
// Software Guide : EndLatex


#include "otbVectorImage.h"
#include "itkRGBAPixel.h"
#include "otbImageFileReader.h"

#include "otbVectorDataFileReader.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataToMapFilter.h"
#include "otbAlphaBlendingFunction.h"

#include "otbImageLayerRenderingModel.h"
#include "otbImageLayerGenerator.h"
#include "otbImageLayer.h"
#include "otbImageView.h"
#include "otbImageWidgetController.h"
#include "otbWidgetResizingActionHandler.h"
#include "otbChangeScaledExtractRegionActionHandler.h"
#include "otbChangeExtractRegionActionHandler.h"
#include "otbChangeScaleActionHandler.h"
#include "otbPixelDescriptionModel.h"
#include "otbPixelDescriptionActionHandler.h"
#include "otbPixelDescriptionView.h"
#include "otbPackedWidgetManager.h"
#include "otbHistogramCurve.h"
#include "otbCurves2DWidget.h"


int main( int argc, char * argv[] )
{
  // params
  const string infname = argv[1];
  const string vectorfname = argv[2];
  const string demdirectory = argv[3];
  std::string fontfilename;
  int run   = 1;
  bool inFont = false;

  if (argc == 5)
    {
    run = atoi(argv[4]);
    }
  else if (argc == 6)
    {
    run = atoi(argv[4]);
    inFont = true;
    std::string fontFilenameArg = argv[5];
    fontfilename.assign(fontFilenameArg );
    }
  else if (argc != 4)
    {
    std::cout << "Invalid parameters: " << std::endl;
    std::cout << argv[0] << " <image filename>  <vector filename> <DEM directory> [<run> = 1] [<font filename> = default font]" << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::VectorImage<double, 2>                  ImageType;
  typedef ImageType::PixelType                        PixelType;
  typedef itk::RGBAPixel<unsigned char>               RGBAPixelType;
  typedef otb::Image<RGBAPixelType, 2>                 OutputImageType;

  // Reading input image
  typedef otb::ImageFileReader<ImageType>            ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();
  //std::cout << "NumBands: " << reader->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;

  // Instantiation of the visualization elements
  typedef otb::ImageLayerRenderingModel<OutputImageType>     ModelType;
  ModelType::Pointer model = ModelType::New();


  typedef otb::ImageView<ModelType>                   ViewType;
  typedef otb::ImageWidgetController                  ControllerType;
  typedef otb::WidgetResizingActionHandler
    <ModelType, ViewType>                              ResizingHandlerType;
  typedef otb::ChangeScaledExtractRegionActionHandler
    <ModelType, ViewType>                              ChangeScaledRegionHandlerType;
  typedef otb::ChangeExtractRegionActionHandler
    <ModelType, ViewType>                              ChangeRegionHandlerType;
  typedef otb::ChangeScaleActionHandler
    <ModelType, ViewType>                              ChangeScaleHandlerType;
  typedef otb::PixelDescriptionModel<OutputImageType> PixelDescriptionModelType;
  typedef otb::PixelDescriptionActionHandler
    < PixelDescriptionModelType, ViewType>            PixelDescriptionActionHandlerType;
  typedef otb::PixelDescriptionView
    < PixelDescriptionModelType >                     PixelDescriptionViewType;

  PixelDescriptionModelType::Pointer pixelModel = PixelDescriptionModelType::New();
  pixelModel->SetLayers(model->GetLayers());

  // Generate the first layer: the remote sensing image
  typedef otb::ImageLayer<ImageType, OutputImageType>                 LayerType;
  typedef otb::ImageLayerGenerator<LayerType>        LayerGeneratorType;
  LayerGeneratorType::Pointer generator = LayerGeneratorType::New();
  generator->SetImage(reader->GetOutput());
  generator->GetLayer()->SetName("Image");
  generator->GenerateLayer();


  // Generate the second layer: the rendered vector data

  //Read the vector data
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer vectorDataReader = VectorDataFileReaderType::New();
  vectorDataReader->SetFileName(vectorfname);

  //Reproject the vector data in the proper projection, ie, the remote sensing image projection
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> ProjectionFilterType;
  ProjectionFilterType::Pointer projection = ProjectionFilterType::New();
  projection->SetInput(vectorDataReader->GetOutput());

  projection->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  projection->SetOutputOrigin(reader->GetOutput()->GetOrigin());
  projection->SetOutputSpacing(reader->GetOutput()->GetSpacing());

  // Manage the DEM through the DEMHandler which is the unique entry point for DEM
  // The registration will be better.
  otb::DEMHandler::Instance()->OpenDEMDirectory(demdirectory);

  // get some usefull information from the image to make sure that we are
  // going to render the vector data in the same geometry
  ImageType::SizeType size;
  size[0] = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
  size[1] = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];

  ImageType::PointType origin;
  origin[0] = reader->GetOutput()->GetOrigin()[0];
  origin[1] = reader->GetOutput()->GetOrigin()[1];

  ImageType::SpacingType spacing;
  spacing[0] = reader->GetOutput()->GetSpacing()[0];
  spacing[1] = reader->GetOutput()->GetSpacing()[1];

  // set up the rendering of the vector data, the VectorDataToMapFilter uses the
  // mapnik library to obtain a nice rendering
  typedef itk::RGBAPixel<unsigned char>                                AlphaPixelType;
  typedef otb::Image<AlphaPixelType, 2>                                 AlphaImageType;
  typedef otb::VectorDataToMapFilter<VectorDataType, AlphaImageType> VectorDataToMapFilterType;
  VectorDataToMapFilterType::Pointer vectorDataRendering = VectorDataToMapFilterType::New();
  vectorDataRendering->SetInput(projection->GetOutput());

  vectorDataRendering->SetSize(size);
  vectorDataRendering->SetOrigin(origin);
  vectorDataRendering->SetSpacing(spacing);
  vectorDataRendering->SetScaleFactor(2.4);
  if (inFont)
    {
    vectorDataRendering->SetFontFileName(fontfilename);
    vectorDataRendering->AddStyle("roads-text");
    }
  // set up the style we want to use
  vectorDataRendering->AddStyle("minor-roads-casing");
  vectorDataRendering->AddStyle("minor-roads");
  vectorDataRendering->AddStyle("roads");

  // rendering of the quicklook: the quicklook is at an entire different scale, so we don't want to
  // render the same roads (only the main one).
  VectorDataToMapFilterType::Pointer vectorDataRenderingQL = VectorDataToMapFilterType::New();
  vectorDataRenderingQL->SetInput(projection->GetOutput());

  double qlRatio = generator->GetOptimalSubSamplingRate();
  //std::cout << "Subsampling for QL: " << qlRatio << std::endl;
  ImageType::SizeType sizeQL;
  sizeQL[0] = size[0]/qlRatio;
  sizeQL[1] = size[1]/qlRatio;

  ImageType::SpacingType spacingQL;
  spacingQL[0] = spacing[0]*qlRatio;
  spacingQL[1] = spacing[1]*qlRatio;

  vectorDataRenderingQL->SetSize(sizeQL);
  vectorDataRenderingQL->SetOrigin(origin);
  vectorDataRenderingQL->SetSpacing(spacingQL);
  vectorDataRenderingQL->SetScaleFactor(2.4*qlRatio);
  if (inFont)
    {
    vectorDataRenderingQL->SetFontFileName(fontfilename);
    vectorDataRenderingQL->AddStyle("roads-text");
    }
  vectorDataRenderingQL->AddStyle("minor-roads-casing");
  vectorDataRenderingQL->AddStyle("minor-roads");
  vectorDataRenderingQL->AddStyle("roads");


  // Now we are ready to create this second layer
  typedef otb::ImageLayer<OutputImageType, OutputImageType>                 LayerRGBAType;
  typedef otb::ImageLayerGenerator<LayerRGBAType>        LayerGeneratorRGBAType;
  LayerGeneratorRGBAType::Pointer generator2 = LayerGeneratorRGBAType::New();
  generator2->SetImage(vectorDataRendering->GetOutput());
  generator2->GetLayer()->SetName("OSM");

  // with slight transparency of the vector data layer
  typedef otb::Function::AlphaBlendingFunction<AlphaPixelType, RGBAPixelType> BlendingFunctionType;
  BlendingFunctionType::Pointer blendingFunction = BlendingFunctionType::New();
  blendingFunction->SetAlpha(0.8);
  generator2->SetBlendingFunction(blendingFunction);

  typedef otb::Function::StandardRenderingFunction<AlphaPixelType, RGBAPixelType> RenderingFunctionType;
  RenderingFunctionType::Pointer renderingFunction = RenderingFunctionType::New();
  renderingFunction->SetAutoMinMax(false);
  generator2->SetRenderingFunction(renderingFunction);


  generator2->SetQuicklook(vectorDataRenderingQL->GetOutput());

  generator2->GenerateQuicklookOff();

  generator2->GenerateLayer();

   // Add the layer to the model
  model->AddLayer(generator->GetLayer());
  model->AddLayer(generator2->GetLayer());

  // All the following code is the manual building of the viewer system

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
  ChangeScaleHandlerType::Pointer changeScaleHandler =ChangeScaleHandlerType::New();
  changeScaleHandler->SetModel(model);
  changeScaleHandler->SetView(view);
  controller->AddActionHandler(changeScaleHandler);

  // Add the pixel description action handler
  PixelDescriptionActionHandlerType::Pointer pixelActionHandler = PixelDescriptionActionHandlerType::New();
  pixelActionHandler->SetView(view);
  pixelActionHandler->SetModel(pixelModel);
  controller->AddActionHandler(pixelActionHandler);

  // Build a pixel description view
  PixelDescriptionViewType::Pointer pixelView = PixelDescriptionViewType::New();
  pixelView->SetModel(pixelModel);


  // adding histograms rendering
  model->Update();

  // Colors
  typedef LayerType::HistogramType            HistogramType;
  typedef otb::HistogramCurve<HistogramType>  HistogramCurveType;

  HistogramCurveType::ColorType red, green, blue;
  red.Fill(0);
  red[0]=1.;
  red[3]=0.5;

  green.Fill(0);
  green[1]=1.;
  green[3]=0.5;

  blue.Fill(0);
  blue[2]=1.;
  blue[3]=0.5;

  HistogramCurveType::Pointer rhistogram = HistogramCurveType::New();
  rhistogram->SetHistogram(generator->GetLayer()->GetHistogramList()->GetNthElement(2));
  rhistogram->SetHistogramColor(red);
  rhistogram->SetLabelColor(red);

  HistogramCurveType::Pointer ghistogram = HistogramCurveType::New();
  ghistogram->SetHistogram(generator->GetLayer()->GetHistogramList()->GetNthElement(1));
  ghistogram->SetHistogramColor(green);
  ghistogram->SetLabelColor(green);

  HistogramCurveType::Pointer bhistogram = HistogramCurveType::New();
  bhistogram->SetHistogram(generator->GetLayer()->GetHistogramList()->GetNthElement(0));
  bhistogram->SetHistogramColor(blue);
  bhistogram->SetLabelColor(blue);

  typedef otb::Curves2DWidget                            CurvesWidgetType;
  typedef CurvesWidgetType::Pointer        CurvesWidgetPointerType;
  CurvesWidgetPointerType     m_CurveWidget = CurvesWidgetType::New();
  m_CurveWidget->AddCurve(rhistogram);
  m_CurveWidget->AddCurve(ghistogram);
  m_CurveWidget->AddCurve(bhistogram);
  m_CurveWidget->SetXAxisLabel("Pixels");
  m_CurveWidget->SetYAxisLabel("Frequency");

  otb::PackedWidgetManager::Pointer windowManager = otb::PackedWidgetManager::New();
  windowManager->RegisterFullWidget(view->GetFullWidget());
  windowManager->RegisterZoomWidget(view->GetZoomWidget());
  windowManager->RegisterScrollWidget(view->GetScrollWidget());
  windowManager->RegisterPixelDescriptionWidget(pixelView->GetPixelDescriptionWidget());
  windowManager->RegisterHistogramWidget(m_CurveWidget);
  windowManager->Show();

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
