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
#ifndef __otbZoomableImageWidget_h
#define __otbZoomableImageWidget_h

#include "otbImageWidgetBase.h"

namespace otb
{
/** \class ZoomableImageWidget
 * \brief 
 *
 */
template <class TPixel>
class ZoomableImageWidget
  : public ImageWidgetBase<TPixel>
{
 public:
  /** Standard class typedefs */
  typedef ZoomableImageWidget Self;
  typedef ImageWidgetBase<TPixel> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ZoomableImageWidget,ImageWidgetBase);

  /** Superclass typedefs */
  typedef typename Superclass::SizeType SizeType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::RegionType RegionType;

  /** Unlarge OpenGlBuffer */
  virtual void UpdateOpenGlBufferedRegion(void);
  /** Test if the buffer has to be enlarged */
  virtual bool UpdateOpenGlBufferedRegionRequested(void);

  /** Initialize the widget */
  virtual void Init(int x, int y, int w, int h, const char * l);
  /** Resize the widget */
  virtual void resize(int x, int y, int w, int h);

  /** 
   * Set a new zoom factor (>1). 
   * \param zoomFactor The new zoom factor.
   */
  void SetZoomFactor(double zoomFactor);
  /**
   * Set a new center for the viewed region.
   * \param index The upper left corner of the viewed region.
   */
  void SetZoomUpperLeftCorner(IndexType index);
  itkGetMacro(ZoomUpperLeftCorner,IndexType);

 protected:
  /** Constructor. */
    ZoomableImageWidget();
    /** Destructor. */
    ~ZoomableImageWidget();

 private:
    ZoomableImageWidget(const Self&);// purposely not implemented
    void operator=(const Self&);// purposely not implemented

    IndexType m_ZoomUpperLeftCorner;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbZoomableImageWidget.txx"
#endif

#endif
