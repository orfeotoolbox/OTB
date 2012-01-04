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
#ifndef __otbViewerViewGUI_h
#define __otbViewerViewGUI_h

#include "otbListenerBase.h"

// Disabling deprecation warning
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif
#include "otbViewerViewGroup.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <FL/Fl_Group.H>
#include "otbViewerModel.h"
#include "otbViewerControllerInterface.h"

#include "otbImageView.h"
#include "otbImageWidget.h"
#include "otbPixelDescriptionView.h"

#include "otbCurves2DWidget.h"
#include "otbHistogramCurve.h"

#include "otbWidgetManager.h"
#include "otbPackedWidgetManager.h"
#include "otbSplittedWidgetManager.h"

#include "otbObjectList.h"
#include <FL/Fl_File_Chooser.H>

namespace otb
{
/** \class ViewerViewGUI
 *
 */
  class ITK_EXPORT ViewerViewGUI
    : public ListenerBase, public ViewerViewGroup, public itk::Object
{
public:
  /** Standard typedefs */
  typedef ViewerViewGUI                 Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ViewerViewGUI, Object);

  /** Controller */
  typedef ViewerControllerInterface::Pointer  ViewerControllerInterfacePointerType;

  /** Model*/
  typedef ViewerModel                                    ViewerModelType;
  typedef ViewerModelType::VisuModelType                 VisuModelType;
  typedef ViewerModelType::VisuModelPointerType          VisuModelPointerType;
  typedef ViewerModelType::PixelDescriptionModelType     PixelDescriptionModelType;
  typedef ViewerModelType::LayerType::HistogramType      HistogramType;
  typedef ViewerModelType::OffsetType                    OffsetType;
  typedef ViewerModelType::RenderingFunctionType         RenderingFunctionType;
  typedef ViewerModelType::StandardRenderingFunctionType StandardRenderingFunctionType;
  typedef StandardRenderingFunctionType::ChannelListType ChannelListType;
  typedef ImageView<VisuModelType>                       VisuViewType;
  typedef VisuViewType::Pointer                          VisuViewPointerType;
  typedef ViewerModelType::_ObjectsTracked               ObjectsTrackedType;

  /* Method to display the Widget : Packed or Splitted */
  typedef PackedWidgetManager                PackedWidgetManagerType;
  typedef PackedWidgetManagerType::Pointer   PackedWidgetManagerPointer;
  typedef SplittedWidgetManager              SplittedWidgetManagerType;
  typedef SplittedWidgetManagerType::Pointer SplittedWidgetManagerPointer;

  /** Widget for the preview*/
  typedef ImageWidget<>             ImageWidgetType;
  typedef ImageWidgetType::SizeType SizeType;
  typedef ImageWidgetType::Pointer  ImageWidgetPointerType;

  /** Curves 2D widget */
  typedef Curves2DWidget                CurvesWidgetType;
  typedef CurvesWidgetType::Pointer     CurvesWidgetPointerType;
  typedef HistogramCurve<HistogramType> HistogramCurveType;
  typedef HistogramCurveType::Pointer   HistogramCurvePointerType;

  /** Pixel Description View*/
  typedef PixelDescriptionView<PixelDescriptionModelType> PixelDescriptionViewType;

  /** vector to store the status of images : diplayed or not displayed*/
  //---> Note : if the packed view is selected : 2nd boolean false, Splitted view : 2nd boolean true
  typedef std::pair<bool, bool>                  PairType;
  typedef std::pair<unsigned int , unsigned int> UIntPairType;
  typedef std::vector<PairType>                  BoolVector;
  typedef std::vector<UIntPairType>              UIntPairVector;

  /** list in order to store the diplay manager*/
  typedef WidgetManager                 WidgetManagerType;
  typedef ObjectList<WidgetManagerType> WidgetManagerList;


  /** Method to set the controller*/
  itkGetObjectMacro(ViewerController, ViewerControllerInterface);

  void SetViewerController(ViewerControllerInterface *pController)
  {
    m_ViewerController = pController;
    m_VisuView->SetController(m_ViewerController->GetVisuController());
    m_PreviewWidget->SetController(m_ViewerController->GetPreviewVisuController());
  }

  // Visu
  itkGetMacro(VisuView, VisuViewPointerType);
  itkGetMacro(PreviewWidget, ImageWidgetPointerType );

  // Show()
  virtual void Show();

  // Update the display
  virtual void Notify();

  //
  virtual void OpenImage(const char * inputFileName);
  virtual void   Initialize(const unsigned int & numberOfOpenedImages);

protected:
  virtual void   OpenImage();
  virtual void   CloseImage();
  virtual void   ViewerSetup();
  virtual void   ViewerSetupOk();
  virtual void   ViewerSetupCancel();

  virtual void   AddImageListName();
  virtual void   Quit();
  virtual void   SelectAction();
  virtual double UpdatePreviewWidgetIsotropicZoom(SizeType size);
  virtual void   ShowHide();
  virtual void   CloseAllDisplayedImages(bool showHideEvent);
  virtual void   ShowTemporaryClosedDisplay();
  virtual void   HideAll();
  virtual void   Display(WidgetManagerList::Pointer  widgetList, unsigned int selectedItem);
  virtual void   Undisplay(unsigned int selectedItem);
  virtual void   UpdateImageListShowed(unsigned int selectedItem, std::string status);
  virtual void   GrayScaleSet();
  virtual void   RGBSet();
  virtual void   ComplexSet();
  virtual void   Diaporama();
  virtual void   DisplayDiaporama();
  virtual void   DiaporamaNext();
  virtual void   DiaporamaPrevious();
  virtual void   DiaporamaQuit();

  virtual void   UpdateInformation(unsigned int selectedItem);
  virtual void   UpdateViewerSetupWindow(unsigned int selectedItem);
  virtual void   DisplayPreviewWidget(unsigned int selectedItem);
  virtual void   SplittedViewMode();
  virtual void   PackedViewMode();
  virtual void   UpdateDiaporamaProgressBar();

  virtual const char * CutFileName(unsigned int selectedItem);

  virtual void LinkSetup();
  virtual void LinkSetupSave();
  virtual void UpdateLinkSetupWindow();
  virtual void LinkSetupOk();
  virtual void LinkSetupRemove();
  virtual void InitializeImageController(unsigned int selectedItem );

  virtual void OpenJpeg2000Image();
 

  /** Constructor */
  ViewerViewGUI();
  /** Destructor */
  virtual ~ViewerViewGUI(){};
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ViewerViewGUI(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Pointer to the model */
  ViewerModel::Pointer m_ViewerModel;

  /** Pointer to the controller */
  ViewerControllerInterface::Pointer m_ViewerController;

  //
  BoolVector        m_DisplayStatusList;
  std::vector<bool> m_LinkedDisplayStatusList;
  UIntPairVector    m_LinkedImageList;
  std::string       m_TemplateViewerName;
  std::string       m_DisplayedLabel;
  std::string       m_UndisplayedLabel;

  //Widget
  WidgetManagerList::Pointer m_WidgetManagerList;
  WidgetManagerList::Pointer m_LinkWidgetManagerList;

  //SlideShow widget
  PackedWidgetManagerPointer m_Widget;
  VisuViewPointerType        m_VisuView;

  /**ImageWidget for my preview*/
  ImageWidgetPointerType m_PreviewWidget;


  /** Histogram */
  HistogramCurveType::ColorType                 m_Red;
  HistogramCurveType::ColorType                 m_Green;
  HistogramCurveType::ColorType                 m_Blue;
  HistogramCurveType::ColorType                 m_Grey;

  /** Store the component number of a pixel*/
  unsigned int m_DiaporamaCurrentIndex;

};
}// End namespace otb

#endif
