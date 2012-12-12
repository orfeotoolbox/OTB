/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mvdImageView_h
#define __mvdImageView_h

#include "mvdGLImageWidget.h"
#include "otbRegionGlComponent.h"

namespace mvd
{

/** \class ImageView
*   \brief todo
*   \todo: Rename ImageViewer when refactoring will be completed.
*  \ingroup Visualization
 */

template <class TViewerModel>
class ImageView
  : /*public ListenerBase, */public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ImageView                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ImageView, itk::Object);

  /** Model typedef */
  typedef TViewerModel                        ModelType;
  typedef typename ModelType::Pointer         ModelPointerType;
  typedef typename ModelType::OutputImageType ImageType;

  /** Controller typedef */
  //typedef ImageWidgetController            ControllerType;
  //typedef typename ControllerType::Pointer ControllerPointerType;

  /** Image region typedef */
  typedef typename ImageType::RegionType RegionType;

  /** Region size & index typedef */
  typedef typename RegionType::SizeType  SizeType;
  typedef typename RegionType::IndexType IndexType;

  /** Offset typedef */
  typedef typename ImageType::OffsetType OffsetType;

  /** Image widget */
  typedef mvd::GLImageWidget               ImageWidgetType;
  //typedef typename ImageWidgetType::Pointer ImageWidgetPointerType;

  /** Region gl component typedef */
  typedef otb::RegionGlComponent                       RegionGlComponentType;
  typedef typename otb::RegionGlComponent::ColorType   ColorType;
  
  typedef typename RegionGlComponentType::Pointer RegionGlComponentPointerType;

  /**
   * This method unregister with previous model if any, and
   * register with the new one.
   */
  void SetModel(ModelType * model);
  void SetExtractRegion(const RegionType& region);

  /**
   * This method sets the controller and passes it to the widgets.
   */
  //void SetController(ControllerType * controller);

  /**
   * Manually trigger a refresh
   */
  void Update();

  /** Get pointers the widgets */
  // itkGetObjectMacro(ScrollWidget, ImageWidgetType);
  // itkGetObjectMacro(FullWidget, ImageWidgetType);
  // itkGetObjectMacro(ZoomWidget, ImageWidgetType);

   ImageWidgetType * GetScrollWidget()
  {
    return m_ScrollWidget;
  }

   ImageWidgetType * GetFullWidget()
  {
    return m_FullWidget;
  }

   ImageWidgetType * GetZoomWidget()
  {
    return m_ZoomWidget;
  }
  

  itkGetObjectMacro(ExtractRegionGlComponent, RegionGlComponentType);
  itkGetObjectMacro(Model, ModelType);

  /** Clear the widgets buffers  */
  void ClearBuffer();

  /** Method to set the ExtractRegionGlComponent color */
  void SetExtractRegionGlComponentColor(const ColorType& color)
  {
    m_ExtractRegionGlComponent->SetColor(color);
  }
  
  /** Method to set the ScaledExtractRegionGlComponent color */
  void SetScaledExtractRegionGlComponent(const ColorType& color)
  {
    m_ScaledExtractRegionGlComponent->SetColor(color);
  }
  

protected:
  /** Constructor */
  ImageView();
  /** Destructor */
  virtual ~ImageView();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Update Scroll widget */
  void UpdateScrollWidget();

  /** Update Full widget */
  void UpdateFullWidget();

  /** Update Zoom widget */
  void UpdateZoomWidget();

  /** Handle notification from the viewer */
  virtual void Notify();

private:
  ImageView(const Self&);      // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  /** The three classical widgets */
  ImageWidgetType *m_ScrollWidget;
  ImageWidgetType *m_FullWidget;
  ImageWidgetType *m_ZoomWidget;

  /** Model pointer */
  ModelPointerType m_Model;

  /** Controller pointer */
  //ControllerPointerType m_Controller;

  /** Viewed region gl components */
  RegionGlComponentPointerType m_ExtractRegionGlComponent;
  RegionGlComponentPointerType m_ScaledExtractRegionGlComponent;

}; // end class
} // end namespace otb

#ifndef  OTB_MANUAL_INSTANTIATION
#include "mvdImageView.txx"
#endif

#endif
