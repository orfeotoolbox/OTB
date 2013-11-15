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
#ifndef __otbViewerController_h
#define __otbViewerController_h

#include "otbViewerControllerInterface.h"
#include "otbViewerViewGUI.h"

/** NewVisu */
// #include "otbImageWidgetController.h"
#include "otbWidgetResizingActionHandler.h"

#include "otbAutoScaleActionHandler.h"


#include "otbObjectList.h"

namespace otb
{
class ITK_EXPORT ViewerController
      : public ViewerControllerInterface
{
public:
  /** Standard class typedefs */
  typedef ViewerController              Self;
  typedef ViewerControllerInterface     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef ViewerViewGUI                    ViewType;
  typedef ViewType::Pointer                ViewPointerType;
  typedef ViewType::VisuViewType           VisuViewType;
  typedef ViewType::VisuViewPointerType    VisuViewPointerType;
  typedef ViewType::ImageWidgetPointerType VisuWidgetPointerType;

  typedef Superclass::ImageType            InputImageType;
  typedef Superclass::ImagePointerType     InputImagePointerType;

  /** NewVisu */
  typedef ModelType::VisuModelType        VisuModelType;
  typedef ModelType::VisuModelPointerType VisuModelPointerType;

  typedef ObjectList<VisuControllerType> VisuControllerListType;

  typedef WidgetResizingActionHandler<VisuModelType, VisuViewType> ResizingHandlerType;
  typedef ResizingHandlerType::Pointer                             ResizingHandlerPointerType;

  typedef ChangeExtractRegionActionHandler<VisuModelType, VisuViewType> ChangeExtractRegionHandlerType;
  typedef ChangeExtractRegionHandlerType::Pointer                       ChangeExtractRegionHandlerPointerType;

  typedef AutoScaleActionHandler<ViewType::ImageWidgetType> AutoScaleHandlerType;
  typedef AutoScaleHandlerType::Pointer                     AutoScaleHandlerPointerType;

  /** Standard type macros */
  itkTypeMacro(ViewerController, Superclass);
  itkNewMacro(Self);

  /** NewVisu */
  VisuControllerPointerType GetVisuController()
  {
    return m_VisuController;
  }
   VisuControllerPointerType GetPreviewVisuController()
  {
    return m_PreviewVisuController;
  }

  void SetView(ViewPointerType pView)
  {
    m_View = pView;

    m_AutoScaleHandler = AutoScaleHandlerType::New();
    m_AutoScaleHandler->SetPreviewWidget(m_View->GetPreviewWidget());
    m_PreviewVisuController->AddActionHandler(m_AutoScaleHandler);
  }



/*   VisuControllerListPointerType GetVisuControllerList() */
/*   { */
/*     return m_VisuControllerList; */
/*   } */


  //virtual void AddController(VisuModelPointerType  modelRenderingLayer  , VisuViewPointerType visuView);

  virtual unsigned int OpenInputImage(const char * filename, const unsigned int & id=0);
  virtual void CloseImage(unsigned int selectedItem);
  virtual void UpdateRGBChannelOrder(int redChoice , int greenChoice, int BlueChoice, unsigned int selectedItem);
  virtual void UpdateGrayScaleChannelOrder(int choice, unsigned int selectedItem );
  virtual void UpdateAmplitudeChannelOrder(int realChoice , int imChoice , unsigned int selectedItem );
  virtual void UpdatePhaseChannelOrder(int realChoice , int imChoice, unsigned int selectedItem);
  virtual void Link(unsigned int leftChoice, unsigned int rightChoice , OffsetType offset);
  virtual void UpdateImageViewController(unsigned int selectedItem);

protected:
  /** Constructor */
  ViewerController();
  /** Destructor */
  virtual ~ViewerController();

private:
  ViewerController(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Pointer to the view */
  ViewPointerType m_View;

/** NewVisu */
  VisuControllerPointerType m_VisuController;
  VisuControllerPointerType m_PreviewVisuController;

  AutoScaleHandlerPointerType   m_AutoScaleHandler;

  VisuControllerListPointerType   m_VisuControllerList;

  /** Model of the application*/

};
} //end namespace otb

#endif
