/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkImage2DViewer.h,v $
  Language:  C++
  Date:      $Date: 2003/07/25 16:47:15 $
  Version:   $Revision: 1.12 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __fltkImage2DViewer_h
#define __fltkImage2DViewer_h

#include "itkObject.h"
#include "itkImage.h"
#include "fltkImage2DViewerGUI.h"
#include "itkCommand.h"

namespace fltk {

template <class ImagePixelType>
class Image2DViewer : public itk::Object, public fltkImage2DViewerGUI
{

public:

   /**
   * Standard "Self" typedef.
   */
  typedef Image2DViewer         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(Image2DViewer, Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

  /**
   * Image Type
   */
  typedef itk::Image< ImagePixelType, 2 >   ImageType;
  
  
  /**
   * Observer Command
   */
  typedef itk::SimpleMemberCommand< Self >      ObserverCommandType;


  /**
   * Observer Command
   */
  virtual void SetImage(const ImageType * image);
  

  /**
   * Update the image rendering
   */
  virtual void Update(void);

  /**
   *  Apply Intensity windowing and Render the image 
   */
  void RenderImage( double selectedMin, double selectedMax );


  /**
   * Return the notifier of the Window, in order
   * to register additional drawers
   */
  itk::Object * GetNotifier(void);

  /**
   * Return the Command responsible for redrawing
   * the window. This command is intended to be registered
   * as observer of the objects whose changes will modify 
   * the display of the image.
   */
  itkGetObjectMacro( RedrawCommand, ObserverCommandType );

   void SetSelectionCallBack(void* ptrObject, 
                             void (*selectionCallBack)
                             (void* ptrObject, 
                             Image2DViewerWindow::SelectionBoxType* box)) ;

   void SetClickCallBack(void* ptrObject, 
                             void (*selectionCallBack)
                             (void* ptrObject, 
                             int & px, int & y ));


  itkSetMacro(FlipY, bool) ;

  void SetInteractionMode( Image2DViewerWindow::InteractionModeType mode );

protected:
  
  Image2DViewer();
  virtual ~Image2DViewer();


private:
  typename ImageType::ConstPointer         m_Image;
  typename ObserverCommandType::Pointer    m_RedrawCommand; 
  unsigned long                            m_Tag;
  bool m_FlipY ;

};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkImage2DViewer.txx>
#endif




#endif
