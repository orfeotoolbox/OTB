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

#ifndef __otbBasicApplicationView_h
#define __otbBasicApplicationView_h

#include "otbBasicApplicationViewGUI.h"

#include "otbListenerBase.h"
#include "otbBasicApplicationModel.h"
#include "otbBasicApplicationControllerInterface.h"

#include "otbImageView.h"

namespace otb
{
/** \class BasicApplicationView
 *
 */
class ITK_EXPORT BasicApplicationView
  : public ListenerBase, public BasicApplicationViewGUI, public itk::Object
{
public:
  /** Standard class typedefs */
  typedef BasicApplicationView          Self;
  typedef BasicApplicationViewGUI       Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standards macros */
  itkNewMacro(Self);
  itkTypeMacro(BasicApplicationView, Object);

  typedef BasicApplicationModel::VisualizationModelType VisualizationModelType;
  typedef BasicApplicationModel::RGBPixelType           PixelType;

  typedef ImageView<VisualizationModelType> ImageViewType;

  /** Event from the model */
  virtual void Notify();

  /** Fluid call backs*/
  virtual void OpenImage();
  virtual void Exit();

  /** Set the controller */
  itkSetObjectMacro(Controller, BasicApplicationControllerInterface);
  itkGetObjectMacro(Controller, BasicApplicationControllerInterface);

  /** Set the widgets controller */
  itkSetObjectMacro(WidgetsController, ImageWidgetController);

  /** Build the interface */
  virtual void Build();

  /** Get a pointer to the view parts for the controller */
  itkGetObjectMacro(ImageView, ImageViewType);

protected:
  /** Constructor */
  BasicApplicationView();

  /** Destructor */
  virtual ~BasicApplicationView();

  /** Refresh the image widgets */
  void RefreshInterface();
  void RefreshVisualization();

private:
  BasicApplicationView(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Pointer to the controller */
  BasicApplicationControllerInterface::Pointer m_Controller;

  /** Pointer to the widgets controller */
  ImageWidgetController::Pointer m_WidgetsController;

  /** Pointer to the model */
  BasicApplicationModel::Pointer m_Model;

  /** Image view */
  ImageViewType::Pointer m_ImageView;

};
} //end namespace otb

#endif
