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
#include "otbImageViewerManagerModel.h"
#include "otbFltkFilterWatcher.h"
#include <FL/fl_ask.H>
#include "itkExceptionObject.h"
#include "otbMacro.h"

#include "otbImageFileWriter.h"


namespace otb
{

/** Initialize the singleton */
ImageViewerManagerModel::Pointer ImageViewerManagerModel::Instance = NULL;

ImageViewerManagerModel::ImageViewerManagerModel()
{
  //Set all the boolean to false
  m_HasChangedChannelOrder = false;
  m_HasImageOpened = false;
}

ImageViewerManagerModel
::~ImageViewerManagerModel(){}



/** Manage the singleton */
ImageViewerManagerModel::Pointer
ImageViewerManagerModel::GetInstance()
{
  if (!Instance)
  {
    Instance = ImageViewerManagerModel::New();
  }
  return Instance;
}

void
ImageViewerManagerModel
::Notify(ImageViewerManagerEventsListener * listener)
{
  listener->ImageViewerManagerNotify();
}

void
ImageViewerManagerModel
::OpenImage(std::string filename)
{
  /** Reader*/
  ReaderPointerType  reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->GenerateOutputInformation();
      
  /** Generate the layer*/
  LayerGeneratorPointerType visuGenerator = LayerGeneratorType::New();
  visuGenerator->SetImage(reader->GetOutput());
  visuGenerator->GenerateLayer();
  StandardRenderingFunctionType::Pointer  rendrerFuntion  = visuGenerator->GetDefaultRenderingFunction();
    
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
  WidgetControllerPointerType controller = this->BuiltController(rendering, visuView ,pixelModel );

  /** Finish Builting the visu*/
  visuView->SetController(controller);
    
  /** Build the curve Widget */
  CurvesWidgetType::Pointer   curveWidget = CurvesWidgetType::New();

  /** Store all the information in the structure*/
  ObjectsTracked currentComponent;

  currentComponent.fileName   = filename;
  currentComponent.pLayer     = visuGenerator->GetLayer();
  currentComponent.pReader    = reader;
  currentComponent.pRendering = rendering;
  currentComponent.pVisuView  = visuView;
  currentComponent.pWidgetController = controller;
  currentComponent.pRenderFuntion  = rendrerFuntion;
  currentComponent.pPixelView   = pixelView;
  currentComponent.pCurveWidget = curveWidget;

  /** Add the the struct in the list*/
  m_ObjectTrackedList.push_back(currentComponent);
  std::cout << "Size of the component Struct is " << m_ObjectTrackedList.size() << std::endl;
  
  m_HasImageOpened = true;
  this->NotifyAll();
  m_HasImageOpened = false;
  
}
/**
 * Built a part of the visu, create a pointer and add a model to the visu
 */
ImageViewerManagerModel
::VisuViewPointerType
ImageViewerManagerModel
::BuiltVisu(VisuModelPointerType pRendering)
{
  VisuViewPointerType visuView = VisuViewType::New();
  visuView->SetModel(pRendering);
  
  return visuView;
}

/**
 * Add Controller
 */
ImageViewerManagerModel
::WidgetControllerPointerType
ImageViewerManagerModel
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
  
  return controller;
}

void
ImageViewerManagerModel
::CloseImage(unsigned int selectedItem)
{
  m_ObjectTrackedList.erase(m_ObjectTrackedList.begin()+selectedItem-1);
}

void
ImageViewerManagerModel
::UpdateRGBChannelOrder(int redChoice , int greenChoice, int BlueChoice, unsigned int selectedItem)
{
  StandardRenderingFunctionType::Pointer renderFunction = m_ObjectTrackedList.at(selectedItem-1).pRenderFuntion;
  renderFunction->SetRedChannelIndex(redChoice);
  renderFunction->SetGreenChannelIndex(greenChoice);
  renderFunction->SetBlueChannelIndex(BlueChoice);
  
  //Update the layer
  m_ObjectTrackedList.at(selectedItem-1).pLayer->SetRenderingFunction(renderFunction);
  m_ObjectTrackedList.at(selectedItem-1).pRendering->Update();
  
  //Notify
  m_HasChangedChannelOrder = true;
  this->NotifyAll();
  m_HasChangedChannelOrder = false;
}

void
ImageViewerManagerModel
::UpdateGrayScaleChannelOrder(int choice, unsigned int selectedItem)
{
  StandardRenderingFunctionType::Pointer renderFunction = m_ObjectTrackedList.at(selectedItem-1).pRenderFuntion;
  renderFunction->SetAllChannels(choice);
  
  //Update the layer
  m_ObjectTrackedList.at(selectedItem-1).pLayer->SetRenderingFunction(renderFunction);
  m_ObjectTrackedList.at(selectedItem-1).pRendering->Update();
  
  //Notify
  m_HasChangedChannelOrder = true;
  this->NotifyAll();
  m_HasChangedChannelOrder = false;
}

void
ImageViewerManagerModel
::UpdateModulusChannelOrder(int realChoice , int imChoice, unsigned int selectedItem )
{
  ModulusRenderingFunction::Pointer modulusFunction = ModulusRenderingFunction::New();
  modulusFunction->SetRedChannelIndex(realChoice);
  modulusFunction->SetGreenChannelIndex(imChoice);
  modulusFunction->Initialize();
  
  //Update the layer
  m_ObjectTrackedList.at(selectedItem-1).pLayer->SetRenderingFunction(modulusFunction);
  m_ObjectTrackedList.at(selectedItem-1).pRendering->Update();
  
  //Notify
  m_HasChangedChannelOrder = true;
  this->NotifyAll();
  m_HasChangedChannelOrder = false;
}


void
ImageViewerManagerModel
::UpdatePhaseChannelOrder(int realChoice , int imChoice, unsigned int selectedItem )
{
  PhaseRenderingFunction::Pointer phaseFunction = PhaseRenderingFunction::New();
  phaseFunction->SetRedChannelIndex(realChoice);
  phaseFunction->SetGreenChannelIndex(imChoice);
  phaseFunction->Initialize();
  
  //Update the layer
  m_ObjectTrackedList.at(selectedItem-1).pLayer->SetRenderingFunction(phaseFunction);
  m_ObjectTrackedList.at(selectedItem-1).pRendering->Update();
  
  //Notify
  m_HasChangedChannelOrder = true;
  this->NotifyAll();
  m_HasChangedChannelOrder = false;
}


}


