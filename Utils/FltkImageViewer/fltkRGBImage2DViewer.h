/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkRGBImage2DViewer.h,v $
  Language:  C++
  Date:      $Date: 2003/07/26 18:34:45 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __fltkRGBImage2DViewer_h
#define __fltkRGBImage2DViewer_h

#include "itkObject.h"
#include "itkImage.h"
#include "fltkRGBImage2DViewerGUI.h"
#include "itkCommand.h"
#include <itkColorTable.h>

namespace fltk {

template <class PixelComponentType>
class RGBImage2DViewer : public itk::Object, public fltkRGBImage2DViewerGUI
{

public:

   /**
   * Standard "Self" typedef.
   */
  typedef RGBImage2DViewer         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(RGBImage2DViewer, Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

 
  /**
   * Image Type
   */
  typedef itk::RGBPixel<PixelComponentType> ImagePixelType;
  typedef itk::Image< ImagePixelType, 2 >   ImageType;
  
  /**
   * Overlay Type
   */
  typedef unsigned char                       OverlayPixelType;
  typedef itk::Image< OverlayPixelType, 2 >   OverlayType;

  /**
   * Chanel Image Type
   */
  typedef PixelComponentType             ChannelPixelType ;
  typedef itk::Image< ChannelPixelType > ChannelImageType ;
  
  /**
   * Observer Command
   */
  typedef itk::SimpleMemberCommand< Self >      ObserverCommandType;

  /**
   * Color Table Type
   */
  typedef itk::ColorTable<double> ColorTableType;

  /**
   * Observer Command
   */
  virtual void SetImage(ImageType * image);
  
  /**
   * Set the overlay
   */
  virtual void SetOverlay(OverlayType * overlay);

  virtual void SetRedChannel(ChannelImageType* image) ;
  virtual void SetGreenChannel(ChannelImageType* image) ;
  virtual void SetBlueChannel(ChannelImageType* image) ;

  virtual void FillChannel(int channel, ChannelImageType* image) ;
  virtual void FillChannel(int channel, ChannelPixelType value, 
                           int sizeX, int sizeY) ;
  /**
   * Update the image rendering
   */
  virtual void Update(void);

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

  itkSetMacro(FlipY, bool) ;

  void UpdateOverlay(void);
  void SetOverlayOpacity(float opacity) {m_OverlayOpacity = opacity;}

protected:
  
  RGBImage2DViewer();
  virtual ~RGBImage2DViewer();


private:
  typename ImageType::Pointer              m_Image;
  typename ObserverCommandType::Pointer    m_RedrawCommand; 
  unsigned long                            m_Tag;

  typename ImageType::RegionType m_ImageRegion ;
  typename ImageType::SizeType m_ImageSize ;
  
  typename ChannelImageType::Pointer m_RedImage ;
  typename ChannelImageType::Pointer m_GreenImage ;
  typename ChannelImageType::Pointer m_BlueImage ;
  typename OverlayType::Pointer     m_Overlay;
  typename ColorTableType::Pointer         m_ColorTable;
  bool m_FlipY ;
  float m_OverlayOpacity;

};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkRGBImage2DViewer.txx>
#endif




#endif
