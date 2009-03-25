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
#ifndef __otbImageViewerManagerViewGUI_h 
#define __otbImageViewerManagerViewGUI_h 

#include "otbImageViewerManagerEventsListener.h"

// Disabling deprecation warning
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif
#include "otbImageViewerManagerViewGroup.h"
//#include "otbImageWidgetPackedManager.h"
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <FL/Fl_Group.H>
#include "otbImageViewerManagerModel.h"
#include "otbImageViewerManagerControllerInterface.h"

#include "otbImageView.h"
#include "otbImageWidget.h"
#include "otbPixelDescriptionView.h"

#include "otbCurves2DWidget.h"
#include "otbHistogramCurve.h"

#include "otbPackedWidgetManager.h"

#include "otbObjectList.h"
#include "otbFixedSizeFullImageWidget.h"
#include <FL/Fl_File_Chooser.H>

namespace otb
{
/** \class ImageViewerManagerViewGUI
 *
 */
  class ITK_EXPORT ImageViewerManagerViewGUI
    : public ImageViewerManagerEventsListener, public ImageViewerManagerViewGroup
{
public:
  /** Standard typedefs */
  typedef ImageViewerManagerViewGUI                  Self;
  typedef ImageViewerManagerEventsListener           Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageViewerManagerViewGUI,ImageViewerManagerEventListener);

  /** Controller */
  typedef ImageViewerManagerControllerInterface::Pointer  ImageViewerManagerControllerInterfacePointerType;
  
  /** Model*/
  typedef ImageViewerManagerModel                                 ImageViewerManagerModelType;
  typedef ImageViewerManagerModelType::VisuModelType              VisuModelType;  //rendreing Image
  typedef ImageViewerManagerModelType::VisuModelPointerType       VisuModelPointerType;  //rendreing Image
  typedef ImageViewerManagerModelType::PixelDescriptionModelType  PixelDescriptionModelType;  //rendreing Image
  typedef ImageViewerManagerModelType::LayerType::HistogramType   HistogramType;        
  typedef ImageViewerManagerModelType::StandardRenderingFunctionType StandardRenderingFunctionType;
  
  typedef ImageView<VisuModelType>                                VisuViewType;
  typedef VisuViewType::Pointer                                   VisuViewPointerType;

  typedef PixelDescriptionView<PixelDescriptionModelType>         PixelDescriptionViewType;

  typedef ObjectList<VisuViewType>                                VisuViewListType;
  typedef VisuViewListType::Pointer                               VisuViewListPointerType;

  //typedef ImageWidgetPackedManager                                ImageWidgetPackedManagerType;
  //typedef std::vector<ImageWidgetPackedManagerType*>            WidgetManagerList; 
  
  // 
  typedef PackedWidgetManager                                     PackedWidgetManagerType;
  typedef PackedWidgetManagerType::Pointer                        PackedWidgetManagerPointerType;
  
  /** Widget for the preview*/
  typedef ImageWidget<>                                           ImageWidgetType;
  typedef ImageWidgetType::SizeType                               SizeType;
  typedef ImageWidgetType::Pointer                                ImageWidgetPointerType;

  /** Curves 2D widget */
  typedef Curves2DWidget                            CurvesWidgetType;
  typedef CurvesWidgetType::Pointer                 CurvesWidgetPointerType;
  typedef HistogramCurve<HistogramType>             HistogramCurveType;
  typedef HistogramCurveType::Pointer               HistogramCurvePointerType;

  /** vector to store the status of images : diplayed or not displayed*/
  typedef std::vector<bool>                                       BoolVector;
  
  typedef ObjectList<VisuViewType>                                VisuViewShowedList;

  
  /** Method to set the controller*/
  itkGetObjectMacro(ImageViewerManagerController,ImageViewerManagerControllerInterface);

  void SetImageViewerManagerController(ImageViewerManagerControllerInterface *pController)
  {
    m_ImageViewerManagerController = pController;
    m_VisuView->SetController(m_ImageViewerManagerController->GetVisuController());
    m_PreviewWidget->SetController(m_ImageViewerManagerController->GetPreviewVisuController());
  }
    

  /** VisuModel */
  itkGetMacro(VisuView,VisuViewPointerType);
  //itkGetMacro(VisuViewList,VisuViewListPointerType );
  
  itkGetMacro(PreviewWidget,ImageWidgetPointerType );
  
  /// Show()
  virtual void Show();

   // Update the display
  virtual void ImageViewerManagerNotify();
  
  /// Inherited methods
  virtual void OpenImage();
  //virtual VisuViewPointerType  AddView(VisuModelPointerType renderingLayer);
  
protected:
  virtual void   CloseImage(); 
  virtual void   ViewerSetup();
  virtual void   ViewerSetupOk(); 
  virtual void   ViewerSetupCancel(); 
  /*   virtual void LinkSetup(); */
  /*   virtual void LinkSetupRemove(); */
  /*   virtual void LinkSetupClear(); */
  /*   virtual void LinkSetupOk(); */
  /*   virtual void LinkSetupSave(); */
  virtual void   AddImageListName();
  virtual void   Quit(); 
  virtual void   SelectAction(); 
  virtual double UpdatePreviewWidgetIsotropicZoom(SizeType size);
  virtual void   ShowHide(); 
  virtual void   HideAll(); 
  virtual void   Display(unsigned int selectedItem);
  virtual void   Undisplay(unsigned int selectedItem); 
  virtual void   UpdateImageListShowed(unsigned int selectedItem, std::string status);
  virtual void   GrayScaleSet(); 
  virtual void   RGBSet(); 
  virtual void   ComplexSet(); 
  /*   virtual void ZoomSmallImagesHook(); */
  /*   virtual void Diaporama(); */
  /*   virtual void DiaporamaPrevious(); */
  /*   virtual void DiaporamaNext(); */
  /*   virtual void DiaporamaQuit(); */
  
  virtual void UpdateInformation(unsigned int selectedItem); 
  virtual void UpdateViewerSetupWindow(unsigned int selectedItem); 
  virtual void DisplayPreviewWidget(unsigned int selectedItem);
  
  /*   virtual void UpdateLinkSetupWindow(unsigned int selectedItem); */
  /*   virtual void UpdatePreviewWindow(unsigned int selectedItem); */
  /*   virtual void UpdateDiaporamaProgressBar(); */
  

  /** Constructor */
  ImageViewerManagerViewGUI();
  /** Destructor */
  virtual ~ImageViewerManagerViewGUI()
    {
      
      delete        m_FullWindow;
      delete        m_ScrollWindow;
      delete        m_ZoomWindow;
      delete        m_PixelWindow;
    };
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageViewerManagerViewGUI(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  /** Pointer to the model */
  
  ImageViewerManagerModel::Pointer               m_ImageViewerManagerModel;
  /** Pointer to the controller */
  ImageViewerManagerControllerInterface::Pointer m_ImageViewerManagerController;
  
  BoolVector                                     m_DisplayStatusList;
  std::string                                    m_TemplateViewerName ;
  std::string                                    m_DisplayedLabel;
  std::string                                    m_UndisplayedLabel ;
  VisuViewShowedList::Pointer                    m_VisuViewShowedList;
  
  /** FL_Windows*/
  Fl_Window*                                     m_FullWindow;
  Fl_Window*                                     m_ScrollWindow;
  Fl_Window*                                     m_ZoomWindow;
  Fl_Window*                                     m_PixelWindow;
  Fl_Window*                                     m_HistogramWindow;

  PackedWidgetManagerType::Pointer               m_PackedWindow;

  //Packed View
  //ImageWidgetPackedManagerType *                 packedWindow;
  //WidgetManagerList                              m_DisplayedWidgetList;
  
  /** Curve widget */
  CurvesWidgetPointerType                        m_CurveWidget;
  HistogramCurveType::Pointer                    m_Bhistogram;
  HistogramCurveType::Pointer                    m_Ghistogram;
  HistogramCurveType::Pointer                    m_Rhistogram;

  unsigned int                                   m_NbComponent;
  /** NewVisu */
  VisuViewPointerType                            m_VisuView;
  
  /**ImageWidget for my preview*/
  ImageWidgetPointerType                         m_PreviewWidget;

  /**view associated to the pixelDescription*/
  PixelDescriptionViewType::Pointer             m_PixelView ;



  //Histogram 
  StandardRenderingFunctionType::Pointer  m_pRenderingFuntion;
  HistogramCurveType::ColorType    m_Red;
  HistogramCurveType::ColorType    m_Green;
  HistogramCurveType::ColorType    m_Blue;
  
  

};
}// End namespace otb

#endif

