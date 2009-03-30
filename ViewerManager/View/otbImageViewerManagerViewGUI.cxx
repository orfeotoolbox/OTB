/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULA
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <FL/Fl_Text_Buffer.H>
#include "otbImageViewerManagerViewGUI.h"
#include "otbMacro.h"
#include "otbFileName.h"

#include "base/ossimFilename.h"
#include "base/ossimDirectory.h"

namespace otb
{
/**
 * Constructor
 */

ImageViewerManagerViewGUI
::ImageViewerManagerViewGUI():m_TemplateViewerName(""),m_DisplayedLabel("+ "),
			      m_UndisplayedLabel("- "),m_DiaporamaCurrentIndex(0)
{

  m_VisuView            =  VisuViewType::New();
  m_PreviewWidget       =  ImageWidgetType::New();
  m_pRenderingFuntion   =  StandardRenderingFunctionType::New();
  m_WidgetManagerList   =  WidgetManagerList::New();

  //Get an instance of the model
  m_ImageViewerManagerModel = ImageViewerManagerModel::GetInstance();
  m_ImageViewerManagerModel->RegisterListener(this);

  //Create the component of the GUI
  this->CreateGUI();
  bSplitted->activate();
  bPacked->activate();
  
  //Create buffer for the guiViewerInformation
  Fl_Text_Buffer * buffer = new Fl_Text_Buffer();
  this->guiViewerInformation->buffer(buffer);
  
  //init the previewWindow
  m_PreviewWidget->label("PreviewWidget");
  gPreviewWindow->add(m_PreviewWidget);
  gPreviewWindow->box(FL_NO_BOX);
  gPreviewWindow->resizable(gPreviewWindow);
  m_PreviewWidget->resize(gPreviewWindow->x(), gPreviewWindow->y(), gPreviewWindow->w(), gPreviewWindow->h() );
  
  //Color Definition    
  m_Red.Fill(0);
  m_Green.Fill(0);
  m_Blue.Fill(0);
  m_Red[0]  = 1.;   m_Red[3]   = 0.5;
  m_Green[1]= 1.;   m_Green[3] = 0.5;
  m_Blue[2] = 1.;   m_Blue[3]  = 0.5;

  //Slide Show 
  m_WidgetManager  =  PackedWidgetManagerType::New();
}
  
  /**
   * 
   */
void
ImageViewerManagerViewGUI
::OpenImage()
{
  std::string pikedFileName="";
  char * cfname ;
  cfname = fl_file_chooser("Pick an image file", "*.*",pikedFileName.c_str());
  
  if (cfname == NULL || strlen(cfname)<1)
    {
      otbMsgDebugMacro(<<"Empty file name!");
      return ;
    }
  
  Fl::check();
  guiMainWindow->redraw();
  
  //Initialise the boolean pair
  PairType      pair(false,false); //Not displayed , Packed View
  
  //Put a new WidgetManager in the list
  if(bSplitted->value() && !bPacked->value())
    {
      SplittedWidgetManagerType::Pointer widgetManager  =  SplittedWidgetManagerType::New(); 
      m_WidgetManagerList->PushBack(widgetManager);
      pair.second = true;
    }
  else
    {
      PackedWidgetManagerType::Pointer widgetManager     =   PackedWidgetManagerType::New(); 
      m_WidgetManagerList->PushBack(widgetManager);
    }
  
  //Put the status of the last added image
  m_DisplayStatusList.push_back(pair);
  
  // Call the Controller
  m_ImageViewerManagerController->OpenInputImage(cfname);

  //Update the Progress Bar 
  this->UpdateDiaporamaProgressBar();
}

/**
 * Handle the notification of the model
 */
void
ImageViewerManagerViewGUI
::ImageViewerManagerNotify()
{
  if(m_ImageViewerManagerModel->GetHasImageOpened())
    this->AddImageListName();
  
  //Update the widget when channel order modified
  if(m_ImageViewerManagerModel->GetHasChangedChannelOrder())
    {
      unsigned int selectedItem = guiImageList->value();
      if (selectedItem == 0)
	return;        // no image selected, return

      //DipalyPreviewWidget
      this->DisplayPreviewWidget(selectedItem);
                
      //Update the widget dispalyed
      if(m_DisplayStatusList[selectedItem-1].first)
	{
	  //m_CurveWidget->ClearAllCurves();//hide();
	  m_WidgetManagerList->GetNthElement(selectedItem-1)->Refresh();
	  this->Display(selectedItem);
	  
	}

      //Update Information
      this->UpdateInformation(selectedItem);
    }
}

/**
 * CloseImage , Send the notification to the controller then to the model
 */
void
ImageViewerManagerViewGUI
::CloseImage()
{
  unsigned int selectedItem = guiImageList->value();
  if (selectedItem == 0)
    return;        // no image selected, return

  //Hide if showned
  if(m_DisplayStatusList[selectedItem-1].first)
    {
      this->Undisplay(selectedItem);
    }
  
  //Call the controller
  m_ImageViewerManagerController->CloseImage(selectedItem);
  
  //Erase the item selected
  m_PreviewWidget->hide();
  guiImageList->remove(selectedItem);
  
  //Erase from the lists
  m_DisplayStatusList.erase( m_DisplayStatusList.begin()+(selectedItem-1));
  m_WidgetManagerList->Erase(selectedItem-1);
  
}
/**
 * Show the mainWindow 
 */
void
ImageViewerManagerViewGUI
::Show()
{
  guiMainWindow->show();
}

/**
 * Update the filename
 */
void
ImageViewerManagerViewGUI
::AddImageListName()
{
  //Update the Image List widget
  unsigned int len     = m_ImageViewerManagerModel->GetObjectList().size();
  std::string fileName = m_ImageViewerManagerModel->GetObjectList().at(len-1).fileName;
  int slashIndex       = fileName.find_last_of("/",fileName.size());
  
  itk::OStringStream oss;
  oss<<m_UndisplayedLabel;
  oss<<m_TemplateViewerName<<fileName.substr(slashIndex+1,fileName.size());
  guiImageList->add(oss.str().c_str());
  guiImageList->redraw();
}

/**
 * Quit GUI
 */
void
ImageViewerManagerViewGUI
::SelectAction()
{
  unsigned int selectedItem = guiImageList->value();
  if (selectedItem == 0)
    {
      // no image selected, return
      return;
    }
  
  //Update the radio button 
  if( m_DisplayStatusList[selectedItem-1].second)
    {
      bSplitted->value(true);
      bPacked->value(false);
    }
  else
    {
      bSplitted->value(false);
      bPacked->value(true);
    }
  
  //DipalyPreviewWidget
  this->DisplayPreviewWidget(selectedItem);
    
  //Update SelectedImageInformation 
  this->UpdateInformation(selectedItem);
  
  //Udpate the ViewerGUISetup
  this->UpdateViewerSetupWindow(selectedItem);
}
/**
 * Quit GUI
 */
void
ImageViewerManagerViewGUI
::DisplayPreviewWidget(unsigned int selectedItem)
{
  //Build the m_PreviewWidget
  VisuModelPointerType rendering = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pRendering;
    
  m_PreviewWidget->ClearBuffer();
  ImageViewerManagerModelType::ViewerImageType * quickLook = rendering->GetRasterizedQuicklook();
  m_PreviewWidget->ReadBuffer(quickLook, quickLook->GetLargestPossibleRegion());

  double newIsotropicZoom = this->UpdatePreviewWidgetIsotropicZoom(quickLook->GetLargestPossibleRegion().GetSize());  
  m_PreviewWidget->SetIsotropicZoom(newIsotropicZoom);
  m_PreviewWidget ->show();
  m_PreviewWidget->redraw();
}



/**
 * Compute the size of the 
 */
double
ImageViewerManagerViewGUI
::UpdatePreviewWidgetIsotropicZoom(SizeType size)
{
  int h = gPreviewWindow->h();
  int w = gPreviewWindow->w();
  
  double zoomW = static_cast<double>(w)/static_cast<double>(size[0]);
  double zoomH = static_cast<double>(h)/static_cast<double>(size[1]);
  
  return std::min(zoomW,zoomH);
}

/**
 * Show Hide
 */
void
ImageViewerManagerViewGUI
::ShowHide()
{
  unsigned int selectedItem = guiImageList->value();
  if (selectedItem == 0)
    {
      // no image selected, return
      return;
    }
  
  //check what to do
  if(!m_DisplayStatusList[selectedItem-1].first)
    {
      //New Display 
      m_DisplayStatusList[selectedItem-1].first = true;
      this->UpdateImageListShowed(selectedItem, m_DisplayedLabel);
      this->Display(selectedItem);
    }
  else
    {
      m_DisplayStatusList[selectedItem-1].first = false;
      this->UpdateImageListShowed(selectedItem, m_UndisplayedLabel);
      this->Undisplay(selectedItem);
    }
}

/**
 * Display the three widget
 */
void
ImageViewerManagerViewGUI
::Display(unsigned int selectedItem)
{
  //Get the view stored in the model 
  CurvesWidgetType::Pointer         curveWidget         =  m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pCurveWidget;
  VisuViewPointerType               currentVisuView     =  m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pVisuView;
  
  //First get the histogram list
  StandardRenderingFunctionType::Pointer pRenderingFuntion = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pRenderFuntion;
  
  HistogramCurveType::Pointer rhistogram = HistogramCurveType::New();
  HistogramCurveType::Pointer ghistogram = HistogramCurveType::New();
  HistogramCurveType::Pointer bhistogram = HistogramCurveType::New();
  
  ghistogram->SetHistogramColor(m_Green);
  ghistogram->SetLabelColor(m_Green);
  bhistogram->SetHistogramColor(m_Blue);
  bhistogram->SetLabelColor(m_Blue);
  rhistogram->SetHistogramColor(m_Red);
  rhistogram->SetLabelColor(m_Red);
  
  rhistogram->SetHistogram(m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pLayer->GetHistogramList()->GetNthElement(pRenderingFuntion->GetRedChannelIndex()));
  ghistogram->SetHistogram(m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pLayer->GetHistogramList()->GetNthElement(pRenderingFuntion->GetGreenChannelIndex()));
  bhistogram->SetHistogram(m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pLayer->GetHistogramList()->GetNthElement(pRenderingFuntion->GetBlueChannelIndex()));
  
  curveWidget->ClearAllCurves();
  curveWidget->AddCurve(bhistogram);
  curveWidget->AddCurve(ghistogram);
  curveWidget->AddCurve(rhistogram);
  curveWidget->SetXAxisLabel("Pixels");
  curveWidget->SetYAxisLabel("Frequency");
  
  //Get the pixelView 
  PixelDescriptionViewType::Pointer pixelView = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pPixelView;

  //Edit the Widget Manager 
  m_WidgetManagerList->GetNthElement(selectedItem-1)->UnRegisterAll();
  m_WidgetManagerList->GetNthElement(selectedItem-1)->RegisterFullWidget(currentVisuView->GetFullWidget());
  m_WidgetManagerList->GetNthElement(selectedItem-1)->RegisterScrollWidget(currentVisuView->GetScrollWidget());
  m_WidgetManagerList->GetNthElement(selectedItem-1)->RegisterZoomWidget(currentVisuView->GetZoomWidget());
  m_WidgetManagerList->GetNthElement(selectedItem-1)->RegisterPixelInformationWidget(pixelView->GetPixelDescriptionWidget());
  m_WidgetManagerList->GetNthElement(selectedItem-1)->RegisterHistogramWidget(curveWidget);
  m_WidgetManagerList->GetNthElement(selectedItem-1)->SetLabel("SplittedView");
  m_WidgetManagerList->GetNthElement(selectedItem-1)->Show();
}

/**
 */
void
ImageViewerManagerViewGUI
::SplittedViewMode()
{
    unsigned int selectedItem = guiImageList->value();
  if (selectedItem == 0)
    {
      // no image selected, return
      return;
    }
  
  //Update the view mode
  if(!m_DisplayStatusList[selectedItem-1].first)
    {
      SplittedWidgetManagerType::Pointer widgetManager  =  SplittedWidgetManagerType::New();
      m_WidgetManagerList->GetNthElement(selectedItem-1)->Hide();
      m_WidgetManagerList->SetNthElement(selectedItem-1,widgetManager);
      m_DisplayStatusList[selectedItem-1].second = true;
    }
    
  //If already displayed : update the view mode and display the new viewMode
  if(m_DisplayStatusList[selectedItem-1].first /*&& !m_DisplayStatusList[selectedItem-1].second*/ )
    {
      SplittedWidgetManagerType::Pointer widgetManager  =  SplittedWidgetManagerType::New();
      m_WidgetManagerList->GetNthElement(selectedItem-1)->Hide();
      m_WidgetManagerList->SetNthElement(selectedItem-1,widgetManager);
      m_DisplayStatusList[selectedItem-1].second = true;
      this->Display(selectedItem);
    }
}

/**
*
*/
void
ImageViewerManagerViewGUI
::PackedViewMode()
{
  unsigned int selectedItem = guiImageList->value();
  if (selectedItem == 0)
    {
      // no image selected, return
      return;
    }
  
  //Update the view mode
  if(!m_DisplayStatusList[selectedItem-1].first)
    {
      PackedWidgetManagerType::Pointer widgetManager  =  PackedWidgetManagerType::New();
      m_WidgetManagerList->GetNthElement(selectedItem-1)->Hide();
      m_WidgetManagerList->SetNthElement(selectedItem-1,widgetManager);
      m_DisplayStatusList[selectedItem-1].second = false;
    }
  
  //If already displayed : update the view mode and display the new viewMode
  std::cout << "Displaying the Packed Mode" << std::endl;
  if(m_DisplayStatusList[selectedItem-1].first /*&& m_DisplayStatusList[selectedItem-1].second*/ )
    {
      PackedWidgetManagerType::Pointer widgetManager  =  PackedWidgetManagerType::New();
      m_WidgetManagerList->GetNthElement(selectedItem-1)->Hide();
      m_WidgetManagerList->SetNthElement(selectedItem-1,widgetManager);
      m_DisplayStatusList[selectedItem-1].second = false;
      this->Display(selectedItem);
    }
  std::cout << "Displaying the Packed Mode" << std::endl;
}

/**
 * Update the guiImageList
 * Put a  "+" if the view is being showed, or a "-" otherwise in the begining of the imageName
 */
void
ImageViewerManagerViewGUI
::UpdateImageListShowed(unsigned int selectedItem, std::string status)
{

  /* Update the ImageList using the status label "+" or "-" */
  std::string fileName = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).fileName;
  int slashIndex = fileName.find_last_of("/",fileName.size());
  
  itk::OStringStream oss;
  oss<<status;
  oss<<m_TemplateViewerName<<fileName.substr(slashIndex+1,fileName.size());
  guiImageList->text(selectedItem,oss.str().c_str());
  oss.str("");
}

/**
 * Hide all the widget opened
 */
void
ImageViewerManagerViewGUI
::Undisplay(unsigned int selectedItem)
{
  m_WidgetManagerList->GetNthElement(selectedItem-1)->Hide(); 
  m_WidgetManagerList->GetNthElement(selectedItem-1)->UnRegisterAll();
}
/**
 * Hide all the widget opened
 */
void
ImageViewerManagerViewGUI
::HideAll()
{
  // Set the display Label to undislayed
  for(unsigned int i = 0; i<m_DisplayStatusList.size() ; i++)    
    {
      if(m_DisplayStatusList[i].first)
	{
	  std::cout << "cest le " << i+1 << " qui est affiche  "<< std::endl; 
	  this->UpdateImageListShowed(i+1, m_UndisplayedLabel);
	  m_DisplayStatusList[i].first = false;
	  m_WidgetManagerList->GetNthElement(i)->Hide();
	  m_WidgetManagerList->GetNthElement(i)->UnRegisterAll();
	}
    }
 }

/**
 * Quit GUI
 */
void
ImageViewerManagerViewGUI
::Quit()
{
  guiMainWindow->hide();

  for(unsigned int i = 0; i<m_DisplayStatusList.size() ; i++)    
    if(m_DisplayStatusList[i].first)
      {
	m_WidgetManagerList->GetNthElement(i)->Hide();
	m_WidgetManagerList->GetNthElement(i)->UnRegisterAll();
      }
}

/**
 *
 */
void
ImageViewerManagerViewGUI
::UpdateInformation(unsigned int selectedItem)
{
  itk::OStringStream oss;
  oss.str("");
  std::string selectedImageName = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).fileName;
  // Clear the info buffer
  guiViewerInformation->buffer()->remove(0,guiViewerInformation->buffer()->length());
  oss<<"Filename: "<<selectedImageName<<std::endl;
  guiViewerInformation->insert(oss.str().c_str());
  oss.str("");
  oss<<"Image information:"<<std::endl;
  guiViewerInformation->insert(oss.str().c_str());
  oss.str("");
  oss<<"Number of bands: "<<m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pReader->GetOutput()->GetNumberOfComponentsPerPixel();
  oss<<" - Size: "<<m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pReader->GetOutput()->GetLargestPossibleRegion().GetSize()<<std::endl;

  guiViewerInformation->insert(oss.str().c_str());
  oss.str("");

  //update band information
  if(m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pReader->GetOutput()->GetNumberOfComponentsPerPixel()>=3)
    {
      oss<<"RGB Composition: ";
      oss<<" Band 1: "<<m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pRenderFuntion->GetRedChannelIndex();
      oss<<" Band 2: "<<m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pRenderFuntion->GetGreenChannelIndex();
      oss<<" Band 3: "<<m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pRenderFuntion->GetBlueChannelIndex()<<std::endl;
    }
  
  guiViewerInformation->insert(oss.str().c_str());
  oss.str("");
}

/**
 *
 */
void
ImageViewerManagerViewGUI
::UpdateViewerSetupWindow(unsigned int selectedItem)
{
  
  ImageViewerManagerModelType::ReaderPointerType reader = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pReader;
  unsigned int nbComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  std::cout <<" NOMBRE  NbComponent  " << nbComponent  << std::endl;

  itk::OStringStream oss;
  oss.str("");

  //Clear all the choices
  guiGrayscaleChannelChoice->clear();
  guiRedChannelChoice->clear();
  guiGreenChannelChoice->clear();
  guiBlueChannelChoice->clear();
  guiGrayscaleChannelChoice->clear();
  guiRealChannelChoice->clear();
  guiImaginaryChannelChoice->clear();
  
  for (unsigned int i = 0;i<nbComponent;++i)
  {
    oss.str("");
    oss<<i+1;
    guiGrayscaleChannelChoice->add(oss.str().c_str());
    guiRedChannelChoice->add(oss.str().c_str());
    guiGreenChannelChoice->add(oss.str().c_str());
    guiBlueChannelChoice->add(oss.str().c_str());
    guiGrayscaleChannelChoice->add(oss.str().c_str());
    guiRealChannelChoice->add(oss.str().c_str());
    guiImaginaryChannelChoice->add(oss.str().c_str());
  }
  guiViewerSetupWindow->redraw();
  guiGrayscaleChannelChoice->redraw();

  switch(nbComponent){
  case 1 :
    this->GrayScaleSet();
    break;
  case 4 : 
    this->RGBSet();
    break;
  case 3 : 
    this->RGBSet();
    break;    
  case 2:
    this->ComplexSet();
    break;
  default : 
    break;
  }
  
  guiViewerSetupName->value(m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).fileName.c_str());
  
}

/**
 * RGBSet();
 */
void 
ImageViewerManagerViewGUI
::RGBSet()
{
  unsigned int selectedItem = guiImageList->value();
  if (selectedItem == 0)
    {
      // no image selected, return
      return;
    }
  
  ImageViewerManagerModelType::ReaderPointerType reader = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pReader;
  unsigned int nbComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  
  StandardRenderingFunctionType::Pointer renderingFunction = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pRenderFuntion;
  
  guiViewerSetupColorMode->set();
  guiViewerSetupComplexMode->clear();
  guiViewerSetupGrayscaleMode->clear();
  guiGrayscaleChannelChoice->deactivate();
  guiRealChannelChoice->deactivate();
  guiImaginaryChannelChoice->deactivate();
  bModulus->deactivate();
  bPhase->deactivate();
  
  guiRedChannelChoice->activate();
  guiGreenChannelChoice->activate();
  guiBlueChannelChoice->activate();

  guiRedChannelChoice->value(std::min(renderingFunction->GetRedChannelIndex(),nbComponent-1));
  guiGreenChannelChoice->value(std::min(renderingFunction->GetGreenChannelIndex(),nbComponent-1));
  guiBlueChannelChoice->value(std::min(renderingFunction->GetBlueChannelIndex(),nbComponent-1));
  
}

/**
 * GrayScale();
 */
void 
ImageViewerManagerViewGUI
::GrayScaleSet()
{
  unsigned int selectedItem = guiImageList->value();
  if (selectedItem == 0)
    {
      // no image selected, return
      return;
    }
  ImageViewerManagerModelType::ReaderPointerType reader = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pReader;
  unsigned int nbComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  
  StandardRenderingFunctionType::Pointer renderingFunction = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pRenderFuntion;
  
  guiViewerSetupGrayscaleMode->set();
  guiViewerSetupComplexMode->clear();
  guiViewerSetupColorMode->clear();
  
  guiRealChannelChoice->deactivate();
  guiImaginaryChannelChoice->deactivate();
  bModulus->deactivate();
  bPhase->deactivate();
  guiRedChannelChoice->deactivate();
  guiGreenChannelChoice->deactivate();
  guiBlueChannelChoice->deactivate();

  guiGrayscaleChannelChoice->activate();
  guiGrayscaleChannelChoice->value(std::min(renderingFunction->GetRedChannelIndex(),nbComponent-1));
}


void
ImageViewerManagerViewGUI
::ComplexSet()
{
  unsigned int selectedItem = guiImageList->value();
  if (selectedItem == 0)
  {
    // no image selected, return
    return;
  }
  ImageViewerManagerModelType::ReaderPointerType reader = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pReader;
  unsigned int nbComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  
  StandardRenderingFunctionType::Pointer renderingFunction = m_ImageViewerManagerModel->GetObjectList().at(selectedItem-1).pRenderFuntion;
  
  guiViewerSetupComplexMode->set();
  guiViewerSetupColorMode->clear();
  guiViewerSetupGrayscaleMode->clear();
  guiGrayscaleChannelChoice->deactivate();
  guiRedChannelChoice->deactivate();
  guiGreenChannelChoice->deactivate();
  guiBlueChannelChoice->deactivate();
  guiRealChannelChoice->activate();
  guiImaginaryChannelChoice->activate();
  bModulus->activate();
  bPhase->activate();
  guiRealChannelChoice->value(std::min(renderingFunction->GetRedChannelIndex(),nbComponent-1));
  guiImaginaryChannelChoice->value(std::min(renderingFunction->GetGreenChannelIndex(),nbComponent-1));
}


/**
 * ViewerSetup();
 */
void 
ImageViewerManagerViewGUI
::ViewerSetup()
{
  unsigned int selectedItem = guiImageList->value();
  if (selectedItem == 0)
  {
    // no image selected, return
    return;
  }
  guiViewerSetupWindow->show();
}

void 
ImageViewerManagerViewGUI
::ViewerSetupOk()
{
  unsigned int selectedItem = guiImageList->value();
  if (selectedItem == 0)
  {
    // no image selected, return
    return;
  }
  
  if (guiViewerSetupColorMode->value())
    {
      m_ImageViewerManagerController->UpdateRGBChannelOrder(atoi(guiRedChannelChoice->value())-1,
							    atoi(guiGreenChannelChoice->value())-1,
							    atoi(guiBlueChannelChoice->value())-1,
							    selectedItem);
    }
  else if (guiViewerSetupGrayscaleMode->value())
    {
      m_ImageViewerManagerController->UpdateGrayScaleChannelOrder(atoi(guiGrayscaleChannelChoice->value())-1,
								  selectedItem);
    }
  else if (guiViewerSetupComplexMode->value())
    {
      if (bModulus->value())
	{
	  m_ImageViewerManagerController->UpdateModulusChannelOrder(atoi(guiRealChannelChoice->value())-1,
								   atoi(guiImaginaryChannelChoice->value())-1,
								   selectedItem);
	}
      else
	{
	  m_ImageViewerManagerController->UpdatePhaseChannelOrder(atoi(guiRealChannelChoice->value())-1,
								  atoi(guiImaginaryChannelChoice->value())-1,
								  selectedItem);
	}
    }
}

void 
ImageViewerManagerViewGUI
::ViewerSetupCancel()
{
  guiViewerSetupWindow->hide();
}



void
ImageViewerManagerViewGUI
::Diaporama()
{
  if (guiImageList->size()  == 0 || guiDiaporama->shown())
  {
    // no image selected, return
    return;
  }
   guiDiaporama->show();
   //guiDiaporama->set_modal();
   guiDiaporamaProgressBar->minimum(1);
   guiDiaporamaProgressBar->maximum(static_cast<float>(guiImageList->size()));
   
   this->DisplayDiaporama();
   UpdateDiaporamaProgressBar();
}

void
ImageViewerManagerViewGUI
::DisplayDiaporama()
{
  std::cout << "m_DiaporamaCurrentIndex  : " << m_DiaporamaCurrentIndex<< std::endl;
  //guiDiapoDisplay->clear();
  
  //PackedWidgetManagerType::Pointer  m_WidgetManager     =   PackedWidgetManagerType::New(); 
  
  //Get the view stored in the model 
  CurvesWidgetType::Pointer         curveWidget         =  m_ImageViewerManagerModel->GetObjectList().at(m_DiaporamaCurrentIndex).pCurveWidget;
  VisuViewPointerType               currentVisuView     =  m_ImageViewerManagerModel->GetObjectList().at(m_DiaporamaCurrentIndex).pVisuView;
  
  //First get the histogram list
  StandardRenderingFunctionType::Pointer pRenderingFuntion = m_ImageViewerManagerModel->GetObjectList().at(m_DiaporamaCurrentIndex).pRenderFuntion;
  
  HistogramCurveType::Pointer rhistogram = HistogramCurveType::New();
  HistogramCurveType::Pointer ghistogram = HistogramCurveType::New();
  HistogramCurveType::Pointer bhistogram = HistogramCurveType::New();
  
  ghistogram->SetHistogramColor(m_Green);
  ghistogram->SetLabelColor(m_Green);
  bhistogram->SetHistogramColor(m_Blue);
  bhistogram->SetLabelColor(m_Blue);
  rhistogram->SetHistogramColor(m_Red);
  rhistogram->SetLabelColor(m_Red);
  
  rhistogram->SetHistogram(m_ImageViewerManagerModel->GetObjectList().at(m_DiaporamaCurrentIndex).pLayer->GetHistogramList()->GetNthElement(pRenderingFuntion->GetRedChannelIndex()));
  ghistogram->SetHistogram(m_ImageViewerManagerModel->GetObjectList().at(m_DiaporamaCurrentIndex).pLayer->GetHistogramList()->GetNthElement(pRenderingFuntion->GetGreenChannelIndex()));
  bhistogram->SetHistogram(m_ImageViewerManagerModel->GetObjectList().at(m_DiaporamaCurrentIndex).pLayer->GetHistogramList()->GetNthElement(pRenderingFuntion->GetBlueChannelIndex()));
  
  curveWidget->ClearAllCurves();
  curveWidget->AddCurve(bhistogram);
  curveWidget->AddCurve(ghistogram);
  curveWidget->AddCurve(rhistogram);
  curveWidget->SetXAxisLabel("Pixels");
  curveWidget->SetYAxisLabel("Frequency");
  
  //Get the pixelView 
  PixelDescriptionViewType::Pointer pixelView = m_ImageViewerManagerModel->GetObjectList().at(m_DiaporamaCurrentIndex).pPixelView;
  
  //Edit the Widget Manager 
  m_WidgetManager->UnRegisterAll();
  m_WidgetManager->RegisterFullWidget(currentVisuView->GetFullWidget());
  m_WidgetManager->RegisterScrollWidget(currentVisuView->GetScrollWidget());
  m_WidgetManager->RegisterZoomWidget(currentVisuView->GetZoomWidget());
  m_WidgetManager->RegisterPixelInformationWidget(pixelView->GetPixelDescriptionWidget());
  m_WidgetManager->RegisterHistogramWidget(curveWidget);
  
  m_WidgetManager->Refresh();
  m_WidgetManager->Show();
}
void
ImageViewerManagerViewGUI
::DiaporamaNext()
{
  if (m_DiaporamaCurrentIndex < static_cast<unsigned int>(guiImageList->size())-1)
  {
    ++m_DiaporamaCurrentIndex;
    this->DisplayDiaporama();
  }
  UpdateDiaporamaProgressBar();
}

void
ImageViewerManagerViewGUI
::DiaporamaPrevious()
{
  if (m_DiaporamaCurrentIndex>0)
  {
    --m_DiaporamaCurrentIndex;
    this->DisplayDiaporama();
  }
  UpdateDiaporamaProgressBar();
}


void
ImageViewerManagerViewGUI
::UpdateDiaporamaProgressBar()
{
  itk::OStringStream oss;
  oss.str("");
  oss<<m_DiaporamaCurrentIndex+1<<"/"<<guiImageList->size();
  guiDiaporamaProgressBar->copy_label(oss.str().c_str());
  guiDiaporamaProgressBar->value(static_cast<float>(m_DiaporamaCurrentIndex));
}

void
ImageViewerManagerViewGUI
::DiaporamaQuit()
{
  //guiDiaporama->set_non_modal();
    guiDiaporama->hide();
}

/**
 * PrintSelf Method
 */

void
ImageViewerManagerViewGUI
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb

