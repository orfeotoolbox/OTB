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
#ifndef __otbImageView_h
#define __otbImageView_h

#include "otbImageWidget.h"
#include "otbImageViewerModelListener.h"
#include "otbImageViewerModel.h"
#include "otbImageWidgetController.h"

namespace otb
{

/** \class ImageView
*   \brief 
*   \Todo: Rename ImageViewer when refactoring will be completed.
*/

template <class TViewerModel >
class ImageView
  : public ImageViewerModelListener, public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ImageView                            Self;
  typedef itk::Object                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ImageView,itk::Object);

  /** Model typedef */
  typedef TViewerModel                         ModelType;
  typedef typename ModelType::Pointer          ModelPointerType;
  typedef typename ModelType::OutputImageType  ImageType;
  
  /** Controller typedef */
  typedef ImageWidgetController                ControllerType;
  typedef typename ControllerType::Pointer     ControllerPointerType;

 /** Image region typedef */
  typedef typename ImageType::RegionType       RegionType;
  
  /** Region size & index typedef */
  typedef typename RegionType::SizeType        SizeType;
  typedef typename RegionType::IndexType       IndexType;

  /** Image widget */
  typedef otb::ImageWidget<ImageType>          ImageWidgetType;
  typedef typename ImageWidgetType::Pointer    ImageWidgetPointerType;

  /**
   * This method unregister with previous model if any, and 
   * register with the new one.
   */
  void SetModel(ModelType * model);
  
  /**
   * This method sets the controller and passes it to the widgets.
   */
  void SetController(ControllerType * controller);
 
  /**
   * Manually trigger a refresh
   */
  void Update();

  /** Get pointers the widgets */
  itkGetObjectMacro(ScrollWidget,ImageWidgetType);
  itkGetObjectMacro(FullWidget,ImageWidgetType);
  itkGetObjectMacro(ZoomWidget,ImageWidgetType);

 protected:
  /** Constructor */
  ImageView();
  /** Destructor */
  ~ImageView();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Update Scroll widget */
  void UpdateScrollWidget();

  /** Update Full widget */
  void UpdateFullWidget();

  /** Update Zoom widget */
  void UpdateZoomWidget();

  /** Handle notification from the viewer */
  virtual void ImageViewerNotify();
  
private:
  ImageView(const Self&);      // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** The three classical widgets */
  ImageWidgetPointerType m_ScrollWidget;
  ImageWidgetPointerType m_FullWidget;
  ImageWidgetPointerType m_ZoomWidget;

  /** Model pointer */
  ModelPointerType       m_Model;

  /** Controller pointer */
  ControllerPointerType  m_Controller;

}; // end class
} // end namespace otb

#ifndef  OTB_MANUAL_INSTANTIATION
#include "otbImageView.txx"
#endif

#endif


