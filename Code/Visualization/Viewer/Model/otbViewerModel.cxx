/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


    This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/
#include "otbViewerModel.h"
#include "otbFltkFilterWatcher.h"
#include <FL/fl_ask.H>
#include "itkExceptionObject.h"
#include "otbMacro.h"

#include "otbImageFileWriter.h"
#include "otbImageSeriesFileReader.h"
#include "otbFltkFilterWatcher.h"

#ifdef OTB_USE_JPEG2000
# include "otbJPEG2000ImageIO.h"
#endif

namespace otb
{

/** Initialize the singleton */
ViewerModel::Pointer ViewerModel::Instance = NULL;

ViewerModel::ViewerModel()
{
  //Set all the boolean to false
  m_HasChangedChannelOrder = false;
  m_HasImageOpened = false;
}

ViewerModel
::~ViewerModel(){}


/** Manage the singleton */
ViewerModel::Pointer
ViewerModel::GetInstance()
{
  if (!Instance)
  {
    Instance = ViewerModel::New();
  }
  return Instance;
}

void
ViewerModel
::NotifyListener(ListenerBase * listener)
{
  listener->Notify();
}


bool
ViewerModel
::IsJPEG2000File(const std::string & filepath)
{
#ifdef OTB_USE_JPEG2000
  bool isJpeg20000 = false;
  JPEG2000ImageIO::Pointer readerJPEG2000 = otb::JPEG2000ImageIO::New();

  readerJPEG2000->SetFileName(filepath);
  if (readerJPEG2000->CanReadFile(filepath.c_str()))
    {
    std::vector<unsigned int> res;
    if (readerJPEG2000->GetAvailableResolutions(res))
      {
      isJpeg20000 = true;
      }
    }

  return isJpeg20000;
#else
  return false;
#endif
}


std::vector<unsigned int>
ViewerModel
::GetJPEG2000Resolution(const std::string & filepath)
{
  std::vector<unsigned int> res;
#ifdef OTB_USE_JPEG2000
  if( !this->IsJPEG2000File(filepath) )
    {
    itkExceptionMacro( "Image "<<filepath<< " is not a JPEG2000." );
    }
  else
    {
    JPEG2000ImageIO::Pointer readerJPEG2000 = otb::JPEG2000ImageIO::New();
    readerJPEG2000->SetFileName(filepath);
    readerJPEG2000->ReadImageInformation();
    readerJPEG2000->GetAvailableResolutions(res);
    }
#endif
  return res;
  
}

void
ViewerModel
::GetJPEG2000ResolutionAndInformations(const std::string & filepath, std::vector<unsigned int>& res, std::vector<std::string> & desc)
{
#ifdef OTB_USE_JPEG2000
  if( !this->IsJPEG2000File(filepath) )
    {
    itkExceptionMacro( "Image "<<filepath<< " is not a JPEG2000." );
    }
  else
    {
    JPEG2000ImageIO::Pointer readerJPEG2000 = otb::JPEG2000ImageIO::New();
    readerJPEG2000->SetFileName(filepath);
    readerJPEG2000->ReadImageInformation();
    readerJPEG2000->GetResolutionInfo(res, desc);
    }
#endif
}

unsigned int
ViewerModel
::OpenImage(std::string filename, const unsigned int id)
{
  std::string otbFilepath = filename;

  bool isJPEG2000 = this->IsJPEG2000File(filename);

  // If jpeg2000, add the selected resolution at the end of the file name
  if( isJPEG2000 )
    {
    otbFilepath += ":";
    std::ostringstream ossRes;
    ossRes << id;
    otbFilepath += ossRes.str();
    }

  /** Reader*/
  ReaderPointerType  reader = ReaderType::New();
  reader->SetFileName(otbFilepath);
  reader->GenerateOutputInformation();

  // Quick look generation
  ImagePointerType quicklook = NULL;
  unsigned int shrinkFactor = 1;
  //// if jpeg2000, try to load the less resolution image as quicklook
  if( isJPEG2000 )
    {
    ReaderPointerType jpeg2000QLReader = ReaderType::New();
    jpeg2000QLReader->SetFileName(filename);
    unsigned int resSize = this->GetJPEG2000Resolution( filename ).size();

    if( resSize > 0 )
      jpeg2000QLReader->SetAdditionalNumber( resSize-1 );
    
    jpeg2000QLReader->Update();
    quicklook= jpeg2000QLReader->GetOutput();
    quicklook->DisconnectPipeline();
    shrinkFactor = (1 << (resSize - 1));

    // Adapt the shrinkFactor to the asked resolution (here it is id)
    shrinkFactor = shrinkFactor/(1<<id);
    }

  if (quicklook.IsNull())
    {
    typedef otb::StreamingShrinkImageFilter<ImageType> StreamingShrinkImageFilterType;
    StreamingShrinkImageFilterType::Pointer shrinker = StreamingShrinkImageFilterType::New();

    // Default shrink factor is 10. Adapt it for small images
    unsigned int maxSize = std::max( reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0],
                                    reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1] );
    if( maxSize > 512 )
      {
      shrinkFactor = static_cast<unsigned int>( vcl_floor( static_cast<double>(maxSize)/static_cast<double>(256) + 0.5) );
      }
   
    shrinker->SetShrinkFactor(shrinkFactor);
    shrinker->SetInput(reader->GetOutput());
    FltkFilterWatcher qlwatcher(shrinker->GetStreamer(), 0, 0, 200, 20,
                                otbGetTextMacro("Generating QuickLook ..."));
    shrinker->Update();
    shrinkFactor = shrinker->GetShrinkFactor();

    quicklook = shrinker->GetOutput();
    quicklook->DisconnectPipeline();
    }

  /** Generate the layer*/
  LayerGeneratorPointerType visuGenerator = LayerGeneratorType::New();
  visuGenerator->SetImage(reader->GetOutput());
  visuGenerator->GenerateQuicklookOff();
  visuGenerator->SetQuicklook(quicklook);

  visuGenerator->SetSubsamplingRate(shrinkFactor);
  visuGenerator->GenerateLayer();
  RenderingFunctionType::Pointer  rendrerFunction  = visuGenerator->GetRenderingFunction();

  /** Rendering image*/
  VisuModelPointerType rendering = VisuModelType::New();
  rendering->AddLayer(visuGenerator->GetLayer());
  rendering->Update();

  /** View*/
  VisuViewPointerType visuView = this->BuiltVisu(rendering);

  /** Build the pixelDescription View*/
  PixelDescriptionViewType::Pointer pixelView = PixelDescriptionViewType::New();
  PixelDescriptionModelPointerType pixelModel = PixelDescriptionModelType::New();
  pixelModel->SetLayers(rendering->GetLayers());
  pixelView->SetModel(pixelModel);

  /** Controller*/
  WidgetControllerPointerType controller = this->BuiltController(rendering, visuView , pixelModel );

  /** Finish Builting the visu*/
  visuView->SetController(controller);

  /** Build the curve Widget */
  CurvesWidgetType::Pointer   curveWidget = CurvesWidgetType::New();

  /** Store all the information in the structure*/
  ObjectsTracked currentComponent;

  currentComponent.pFileName   = otbFilepath;
  currentComponent.pLayer     = visuGenerator->GetLayer();
  currentComponent.pReader    = reader;
  currentComponent.pQuicklook = quicklook;
  currentComponent.pRendering = rendering;
  currentComponent.pVisuView  = visuView;
  currentComponent.pWidgetController = controller;
  currentComponent.pRenderFunction  = rendrerFunction;
  currentComponent.pPixelView   = pixelView;
  currentComponent.pPixelModel  = pixelModel;
  currentComponent.pCurveWidget = curveWidget;

  assert(currentComponent.pLayer);
  assert(currentComponent.pReader);
  assert(currentComponent.pQuicklook);
  assert(currentComponent.pRendering);
  assert(currentComponent.pVisuView);
  assert(currentComponent.pWidgetController);
  assert(currentComponent.pRenderFunction);
  assert(currentComponent.pPixelView);
  assert(currentComponent.pPixelModel);
  assert(currentComponent.pCurveWidget);

  /** Add the the struct in the list*/
  m_ObjectTrackedList.push_back(currentComponent);

  m_HasImageOpened = true;
  this->NotifyAll();
  m_HasImageOpened = false;

  return 1;
}

/**
 * Read a series of images, including cropping facilities throught
 * the ImageSeriesFileReader
 */
unsigned int
ViewerModel
::OpenImageList(std::string filename)
{
  /** Reader*/
  typedef ImageSeriesFileReader< ImageType > ImageSeriesReaderType;
  ImageSeriesReaderType::Pointer reader = ImageSeriesReaderType::New();
  reader->SetFileName(filename);
  reader->Update();

  for (  unsigned int i = 0; i < reader->GetOutput()->Size(); i++ )
  {
    /** Generate the layer*/
    LayerGeneratorPointerType visuGenerator = LayerGeneratorType::New();
    visuGenerator->SetImage(reader->GetOutput()->GetNthElement(i));
    FltkFilterWatcher qlwatcher(visuGenerator->GetProgressSource(), 0, 0, 200, 20,"Generating QuickLook ...");
    visuGenerator->GenerateLayer();
    RenderingFunctionType::Pointer  rendrerFunction  = visuGenerator->GetRenderingFunction();

    /** Rendering image*/
    VisuModelPointerType rendering = VisuModelType::New();
    rendering->AddLayer(visuGenerator->GetLayer());

    rendering->Update();

    /** View*/
    VisuViewPointerType visuView = this->BuiltVisu(rendering);

    /** Build the pixelDescription View*/
    PixelDescriptionViewType::Pointer pixelView = PixelDescriptionViewType::New();
    PixelDescriptionModelPointerType pixelModel = PixelDescriptionModelType::New();
    pixelModel->SetLayers(rendering->GetLayers());
    pixelView->SetModel(pixelModel);

    /** Controller*/
    WidgetControllerPointerType controller = this->BuiltController(rendering, visuView , pixelModel );

    /** Finish Builting the visu*/
    visuView->SetController(controller);

    /** Build the curve Widget */
    CurvesWidgetType::Pointer   curveWidget = CurvesWidgetType::New();

    /** Store all the information in the structure*/
    ObjectsTracked currentComponent;

    currentComponent.pFileName   = reader->GetFileName(i);
    currentComponent.pLayer     = visuGenerator->GetLayer();
    currentComponent.pReader    = reader->GetImageFileReader(i);
    currentComponent.pRendering = rendering;
    currentComponent.pVisuView  = visuView;
    currentComponent.pWidgetController = controller;
    currentComponent.pRenderFunction  = rendrerFunction;
    currentComponent.pPixelView   = pixelView;
    currentComponent.pPixelModel  = pixelModel;
    currentComponent.pCurveWidget = curveWidget;

    assert(currentComponent.pLayer);
    assert(currentComponent.pReader);
    assert(currentComponent.pRendering);
    assert(currentComponent.pVisuView);
    assert(currentComponent.pWidgetController);
    assert(currentComponent.pRenderFunction);
    assert(currentComponent.pPixelView);
    assert(currentComponent.pPixelModel);
    assert(currentComponent.pCurveWidget);

    /** Add the the struct in the list*/
    m_ObjectTrackedList.push_back(currentComponent);

    m_HasImageOpened = true;
    this->NotifyAll();
    m_HasImageOpened = false;
  }

  return reader->GetOutput()->Size();
}


/**
 * Built a part of the visu, create a pointer and add a model to the visu
 */
ViewerModel
::VisuViewPointerType
ViewerModel
::BuiltVisu(VisuModelPointerType pRendering)
{
  VisuViewPointerType visuView = VisuViewType::New();
  visuView->SetModel(pRendering);

  return visuView;
}

/**
 * Add Controller
 */
ViewerModel
::WidgetControllerPointerType
ViewerModel
::BuiltController(VisuModelPointerType modelRenderingLayer, VisuViewPointerType visuView, PixelDescriptionModelPointerType pixelModel)
{
  WidgetControllerPointerType controller = WidgetControllerType::New();

  // Add the resizing handler
  ResizingHandlerType::Pointer resizingHandler = ResizingHandlerType::New();
  resizingHandler->SetModel(modelRenderingLayer);
  resizingHandler->SetView(visuView);
  controller->AddActionHandler(resizingHandler);

    // Add the change scaled region handler
  ChangeScaledRegionHandlerType::Pointer changeScaledHandler =ChangeScaledRegionHandlerType::New();
  changeScaledHandler->SetModel(modelRenderingLayer);
  changeScaledHandler->SetView(visuView);
  controller->AddActionHandler(changeScaledHandler);

  // Add the change extract region handler
  ChangeRegionHandlerType::Pointer changeHandler =ChangeRegionHandlerType::New();
  changeHandler->SetModel(modelRenderingLayer);
  changeHandler->SetView(visuView);
  controller->AddActionHandler(changeHandler);

  // Add the change scaled handler
  ChangeScaleHandlerType::Pointer changeScaleHandler =ChangeScaleHandlerType::New();
  changeScaleHandler->SetModel(modelRenderingLayer );
  changeScaleHandler->SetView(visuView);
  controller->AddActionHandler(changeScaleHandler);

  //Pixel Description Handling
  PixelDescriptionActionHandlerType::Pointer pixelActionHandler = PixelDescriptionActionHandlerType::New();
  pixelActionHandler->SetView(visuView);
  pixelActionHandler->SetModel(pixelModel);
  controller->AddActionHandler(pixelActionHandler);

  // Add the action handler for the arrow key
  ArrowKeyMoveActionHandlerType::Pointer arrowKeyMoveHandler = ArrowKeyMoveActionHandlerType::New();
  arrowKeyMoveHandler->SetModel(modelRenderingLayer);
  arrowKeyMoveHandler->SetView(visuView);
  controller->AddActionHandler(arrowKeyMoveHandler);

  return controller;
}

void
ViewerModel
::CloseImage(unsigned int selectedItem)
{
  m_ObjectTrackedList.erase(m_ObjectTrackedList.begin()+selectedItem-1);
}

void
ViewerModel
::UpdateRGBChannelOrder(int redChoice , int greenChoice, int blueChoice, unsigned int selectedItem)
{

  StandardRenderingFunctionType::ChannelListType channels;
  channels.resize(3);
  channels[0]=redChoice;
  channels[1]=greenChoice;
  channels[2]=blueChoice;

   StandardRenderingFunctionType::Pointer renderFunction;
   renderFunction = StandardRenderingFunctionType::New();

   renderFunction->SetChannelList(channels);


  //Update the layer
  m_ObjectTrackedList.at(selectedItem-1).pLayer->SetRenderingFunction(renderFunction);
  m_ObjectTrackedList.at(selectedItem-1).pRenderFunction = renderFunction;
  //FIXME Initialize should disappear from the renderingFunction
  renderFunction->Initialize(m_ObjectTrackedList.at(selectedItem-1).pLayer->GetMetaDataDictionary());
  m_ObjectTrackedList.at(selectedItem-1).pRendering->Update();

  //Notify
  m_HasChangedChannelOrder = true;
  this->NotifyAll();
  m_HasChangedChannelOrder = false;
}

void
ViewerModel
::UpdateGrayScaleChannelOrder(int choice, unsigned int selectedItem)
{
  StandardRenderingFunctionType::ChannelListType channels;
  channels.resize(3);
  channels[0]=choice;
  channels[1]=choice;
  channels[2]=choice;

  StandardRenderingFunctionType::Pointer renderFunction;
  renderFunction = StandardRenderingFunctionType::New();

  renderFunction->SetChannelList(channels);

  //Update the layer
  m_ObjectTrackedList.at(selectedItem-1).pLayer->SetRenderingFunction(renderFunction);
  m_ObjectTrackedList.at(selectedItem-1).pRenderFunction = renderFunction;
  //FIXME Initialize should disappear from the renderingFunction
  renderFunction->Initialize(m_ObjectTrackedList.at(selectedItem-1).pLayer->GetMetaDataDictionary());
  m_ObjectTrackedList.at(selectedItem-1).pRendering->Update();

  //Notify
  m_HasChangedChannelOrder = true;
  this->NotifyAll();
  m_HasChangedChannelOrder = false;
}

void
ViewerModel
::UpdateAmplitudeChannelOrder(int realChoice , int imChoice, unsigned int selectedItem )
{
  AmplitudeRenderingFunction::PixelRepresentationFunctionType::ChannelListType channels;
  channels.push_back(realChoice);
  channels.push_back(imChoice);

   AmplitudeRenderingFunction::Pointer modulusFunction;
  modulusFunction = AmplitudeRenderingFunction::New();

  modulusFunction->SetChannelList(channels);


  //Update the layer
  m_ObjectTrackedList.at(selectedItem-1).pLayer->SetRenderingFunction(modulusFunction);
  m_ObjectTrackedList.at(selectedItem-1).pRenderFunction = modulusFunction;
  //FIXME Initialize should disappear from the renderinFunction
  modulusFunction->Initialize(m_ObjectTrackedList.at(selectedItem-1).pLayer->GetMetaDataDictionary());
  m_ObjectTrackedList.at(selectedItem-1).pRendering->Update();

  //Notify
  m_HasChangedChannelOrder = true;
  this->NotifyAll();
  m_HasChangedChannelOrder = false;
}


void
ViewerModel
::UpdatePhaseChannelOrder(int realChoice , int imChoice, unsigned int selectedItem )
{
  PhaseRenderingFunction::PixelRepresentationFunctionType::ChannelListType channels;
  channels.push_back(realChoice);
  channels.push_back(imChoice);

  PhaseRenderingFunction::Pointer phaseFunction;
  phaseFunction = PhaseRenderingFunction::New();

  phaseFunction->SetChannelList(channels);


  //Update the layer
  m_ObjectTrackedList.at(selectedItem-1).pLayer->SetRenderingFunction(phaseFunction);
  m_ObjectTrackedList.at(selectedItem-1).pRenderFunction = phaseFunction;
  //FIXME Initialize should disappear from the renderinFunction
  phaseFunction->Initialize(m_ObjectTrackedList.at(selectedItem-1).pLayer->GetMetaDataDictionary());
  m_ObjectTrackedList.at(selectedItem-1).pRendering->Update();

  //Notify
  m_HasChangedChannelOrder = true;
  this->NotifyAll();
  m_HasChangedChannelOrder = false;
}

/**
 *
 */
void
ViewerModel
::Link(unsigned int leftChoice, unsigned int rightChoice, OffsetType offset)
{

  //Create A null offset
  OffsetType nullOffset;
  nullOffset.Fill(0);

  //Get the controllers of the selected images
  WidgetControllerPointerType rightController = m_ObjectTrackedList.at(rightChoice-1).pWidgetController;
  WidgetControllerPointerType leftController = m_ObjectTrackedList.at(leftChoice-1).pWidgetController;

  //Get the models related to the choosen images
  VisuModelPointerType rightRenderModel       = m_ObjectTrackedList.at(rightChoice-1).pRendering;
  VisuModelPointerType leftRenderModel        = m_ObjectTrackedList.at(leftChoice-1).pRendering;

  //Get the views related to the choosen images
  VisuViewPointerType  pRightVisuView         = m_ObjectTrackedList.at(rightChoice-1).pVisuView;
  VisuViewPointerType  pLeftVisuView          = m_ObjectTrackedList.at(leftChoice-1).pVisuView;

  //Pixel View
  PixelDescriptionModelPointerType rightPixelModel = m_ObjectTrackedList.at(rightChoice-1).pPixelModel;
  PixelDescriptionModelPointerType leftPixelModel  = m_ObjectTrackedList.at(leftChoice-1).pPixelModel;

/*
  pLeftVisuView->GetScrollWidget()->SetIdentifier("Scroll_l");
  pLeftVisuView->GetFullWidget()->SetIdentifier("Full_l");
  pLeftVisuView->GetZoomWidget()->SetIdentifier("Zoom_l");

  pRightVisuView->GetScrollWidget()->SetIdentifier("Scroll_r");
  pRightVisuView->GetFullWidget()->SetIdentifier("Full_r");
  pRightVisuView->GetZoomWidget()->SetIdentifier("Zoom_r");


  leftController->RemoveActionHandler(3);
  rightController->RemoveActionHandler(3);

  // Add the change scaled handler
  ChangeScaleHandlerType::Pointer rightChangeScaleHandler =ChangeScaleHandlerType::New();
  rightChangeScaleHandler->SetModel(rightRenderModel );
  rightChangeScaleHandler->SetView(pLeftVisuView);
  rightChangeScaleHandler->SetView2(pRightVisuView);

  ChangeScaleHandlerType::Pointer leftChangeScaleHandler =ChangeScaleHandlerType::New();
  leftChangeScaleHandler->SetModel(leftRenderModel );
  leftChangeScaleHandler->SetView(pRightVisuView);
  leftChangeScaleHandler->SetView2(pLeftVisuView);

  rightController->AddActionHandler( leftChangeScaleHandler);
  leftController->AddActionHandler(rightChangeScaleHandler);

  // Add the change scaled handler
  ChangeScaleHandlerType::Pointer rChangeScaleHandler =ChangeScaleHandlerType::New();
  rChangeScaleHandler->SetModel(leftRenderModel );
  rChangeScaleHandler->SetView(pLeftVisuView);

  ChangeScaleHandlerType::Pointer lChangeScaleHandler =ChangeScaleHandlerType::New();
  lChangeScaleHandler->SetModel(rightRenderModel );
  lChangeScaleHandler->SetView(pRightVisuView);

  rightController->AddActionHandler( lChangeScaleHandler);
  leftController->AddActionHandler(rChangeScaleHandler);

*/






  // Add the resizing handler
  ResizingHandlerType::Pointer rightResizingHandler = ResizingHandlerType::New();
  rightResizingHandler->SetModel(rightRenderModel);
  rightResizingHandler->SetView(pLeftVisuView);

  ResizingHandlerType::Pointer leftResizingHandler = ResizingHandlerType::New();
  leftResizingHandler->SetModel(leftRenderModel);
  leftResizingHandler->SetView(pRightVisuView);

  rightController->AddActionHandler( leftResizingHandler);
  leftController->AddActionHandler(rightResizingHandler);

  // Add the change scaled region handler--
  ChangeScaledRegionHandlerType::Pointer rightChangeScaledHandler =ChangeScaledRegionHandlerType::New();
  rightChangeScaledHandler->SetModel(rightRenderModel);
  rightChangeScaledHandler->SetView(pLeftVisuView);
  rightChangeScaledHandler->SetOffset(nullOffset-offset);

  ChangeScaledRegionHandlerType::Pointer leftChangeScaledHandler =ChangeScaledRegionHandlerType::New();
  leftChangeScaledHandler->SetModel(leftRenderModel);
  leftChangeScaledHandler->SetView(pRightVisuView);
  leftChangeScaledHandler->SetOffset(offset);

  rightController->AddActionHandler(leftChangeScaledHandler);
  leftController->AddActionHandler( rightChangeScaledHandler);

  // Add the change extract region handler--
  ChangeRegionHandlerType::Pointer rightChangeHandler =ChangeRegionHandlerType::New();
  rightChangeHandler->SetModel(rightRenderModel);
  rightChangeHandler->SetView(pLeftVisuView);
  rightChangeHandler->SetOffset(nullOffset-offset);

  ChangeRegionHandlerType::Pointer leftChangeHandler =ChangeRegionHandlerType::New();
  leftChangeHandler->SetModel(leftRenderModel);
  leftChangeHandler->SetView(pRightVisuView);
  leftChangeHandler->SetOffset(offset);

  rightController->AddActionHandler( leftChangeHandler);
  leftController->AddActionHandler(rightChangeHandler);


  // Add the change scaled handler
  ChangeScaleHandlerType::Pointer rightChangeScaleHandler =ChangeScaleHandlerType::New();
  rightChangeScaleHandler->SetModel(rightRenderModel );
  rightChangeScaleHandler->SetView(pLeftVisuView);
  rightChangeScaleHandler->SetViewToUpdate(pRightVisuView);

  ChangeScaleHandlerType::Pointer leftChangeScaleHandler =ChangeScaleHandlerType::New();
  leftChangeScaleHandler->SetModel(leftRenderModel );
  leftChangeScaleHandler->SetView(pRightVisuView);
  leftChangeScaleHandler->SetViewToUpdate(pLeftVisuView);

  // This handler has to be set before the classical zoom handler of
  // the ImageView to avoid confusion
  rightController->InsertActionHandler(0, leftChangeScaleHandler);
  leftController->InsertActionHandler(0, rightChangeScaleHandler);

 //Pixel Description Handling--
  PixelDescriptionActionHandlerType::Pointer rightPixelActionHandler = PixelDescriptionActionHandlerType::New();
  rightPixelActionHandler->SetView(pLeftVisuView );
  rightPixelActionHandler->SetModel(rightPixelModel);
  rightPixelActionHandler->SetOffset(nullOffset-offset);

  PixelDescriptionActionHandlerType::Pointer leftPixelActionHandler = PixelDescriptionActionHandlerType::New();
  leftPixelActionHandler->SetView(pRightVisuView);
  leftPixelActionHandler->SetModel(leftPixelModel);
  leftPixelActionHandler->SetOffset(offset);

  //rightController->AddActionHandler(leftPixelActionHandler );
  //leftController->AddActionHandler(rightPixelActionHandler);

}

/**
 *
 */
void
ViewerModel
::InitializeImageViewController(unsigned int selectedItem)
{
  VisuModelPointerType  render = m_ObjectTrackedList.at(selectedItem-1).pRendering;
  VisuViewPointerType   view   = m_ObjectTrackedList.at(selectedItem-1).pVisuView;
  PixelDescriptionModelPointerType pixelModel = m_ObjectTrackedList.at(selectedItem-1).pPixelModel;

  m_ObjectTrackedList.at(selectedItem-1).pWidgetController = this->BuiltController(render, view, pixelModel);
  m_ObjectTrackedList.at(selectedItem-1).pVisuView->SetController(m_ObjectTrackedList.at(selectedItem-1).pWidgetController);
}

}


