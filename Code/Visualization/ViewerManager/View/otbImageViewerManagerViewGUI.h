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
#ifndef __otbImageViewerManagerViewGUI_h
#define __otbImageViewerManagerViewGUI_h

#include "otbListenerBase.h"

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

#include "otbWidgetManager.h"
#include "otbPackedWidgetManager.h"
#include "otbSplittedWidgetManager.h"

#include "otbObjectList.h"
#include <FL/Fl_File_Chooser.H>

namespace otb
{
/** \class ImageViewerManagerViewGUI
 *
 */
  class ITK_EXPORT ImageViewerManagerViewGUI
    : public ListenerBase, public ImageViewerManagerViewGroup, public itk::Object
{
public:
  /** Standard typedefs */
  typedef ImageViewerManagerViewGUI                  Self;
  typedef itk::Object          Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageViewerManagerViewGUI, Object);

  /** Controller */
  typedef ImageViewerManagerControllerInterface::Pointer  ImageViewerManagerControllerInterfacePointerType;

  /** Model*/
  typedef ImageViewerManagerModel                                 ImageViewerManagerModelType;
  typedef ImageViewerManagerModelType::VisuModelType              VisuModelType;  //rendreing Image
  typedef ImageViewerManagerModelType::VisuModelPointerType       VisuModelPointerType;  //rendreing Image
  typedef ImageViewerManagerModelType::PixelDescriptionModelType  PixelDescriptionModelType;  //rendreing Image
  typedef ImageViewerManagerModelType::LayerType::HistogramType   HistogramType;
  typedef ImageViewerManagerModelType::OffsetType                 OffsetType;
  typedef ImageViewerManagerModelType::RenderingFunctionType      RenderingFunctionType;
  typedef ImageViewerManagerModelType::StandardRenderingFunctionType StandardRenderingFunctionType;
  typedef StandardRenderingFunctionType::ChannelListType          ChannelListType;
  typedef ImageView<VisuModelType>                                VisuViewType;
  typedef VisuViewType::Pointer                                   VisuViewPointerType;

  /* Method to display the Widget : Packed or Splitted */
  typedef PackedWidgetManager                                     PackedWidgetManagerType;
  typedef PackedWidgetManagerType::Pointer                        PackedWidgetManagerPointerType;
  typedef SplittedWidgetManager                                   SplittedWidgetManagerType;
  typedef SplittedWidgetManagerType::Pointer                      SplittedWidgetManagerPointerType;

  /** Widget for the preview*/
  typedef ImageWidget<>                                           ImageWidgetType;
  typedef ImageWidgetType::SizeType                               SizeType;
  typedef ImageWidgetType::Pointer                                ImageWidgetPointerType;

  /** Curves 2D widget */
  typedef Curves2DWidget                                           CurvesWidgetType;
  typedef CurvesWidgetType::Pointer                                CurvesWidgetPointerType;
  typedef HistogramCurve<HistogramType>                            HistogramCurveType;
  typedef HistogramCurveType::Pointer                              HistogramCurvePointerType;

  /** Pixel Description View*/
  typedef PixelDescriptionView<PixelDescriptionModelType>         PixelDescriptionViewType;

  /** vector to store the status of images : diplayed or not displayed*/
  //---> Note : if the packed view is selected : 2nd boolean false, Splitted view : 2nd boolean true
  typedef std::pair<bool, bool>                                   PairType;
  typedef std::pair<unsigned int , unsigned int>                   UIntPairType;
  typedef std::vector<PairType>                                   BoolVector;
  typedef std::vector<UIntPairType>                               UIntPairVector;

  /** list in order to store the diplay manager*/
  typedef WidgetManager                                           WidgetManagerType;
  typedef ObjectList<WidgetManagerType>                           WidgetManagerList;


  /** Method to set the controller*/
  itkGetObjectMacro(ImageViewerManagerController, ImageViewerManagerControllerInterface);

  void SetImageViewerManagerController(ImageViewerManagerControllerInterface *pController)
  {
    m_ImageViewerManagerController = pController;
    m_VisuView->SetController(m_ImageViewerManagerController->GetVisuController());
    m_PreviewWidget->SetController(m_ImageViewerManagerController->GetPreviewVisuController());
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
  ImageViewerManagerViewGUI();
  /** Destructor */
  virtual ~ImageViewerManagerViewGUI(){};
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageViewerManagerViewGUI(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Pointer to the model */
  ImageViewerManagerModel::Pointer               m_ImageViewerManagerModel;

  /** Pointer to the controller */
  ImageViewerManagerControllerInterface::Pointer m_ImageViewerManagerController;

  //
  BoolVector                                     m_DisplayStatusList;
  std::vector<bool>                              m_LinkedDisplayStatusList;
  UIntPairVector                                 m_LinkedImageList;
  std::string                                    m_TemplateViewerName;
  std::string                                    m_DisplayedLabel;
  std::string                                    m_UndisplayedLabel;

  //Widget Manager
  WidgetManagerList::Pointer                     m_WidgetManagerList;
  WidgetManagerList::Pointer                     m_LinkWidgetManagerList;

  //SlideShow widget Manager
  PackedWidgetManagerType::Pointer               m_WidgetManager;

  VisuViewPointerType                            m_VisuView;

  /**ImageWidget for my preview*/
  ImageWidgetPointerType                         m_PreviewWidget;


  /** Histogram */
  StandardRenderingFunctionType::Pointer        m_pRenderingFunction;
  HistogramCurveType::ColorType                 m_Red;
  HistogramCurveType::ColorType                 m_Green;
  HistogramCurveType::ColorType                 m_Blue;
  HistogramCurveType::ColorType                 m_Grey;

  /** Store the component number of a pixel*/
  unsigned int                                  m_DiaporamaCurrentIndex;

};
}// End namespace otb

#endif

