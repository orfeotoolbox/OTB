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
#ifndef __otbFixedSizeFullImageWidget_h
#define __otbFixedSizeFullImageWidget_h

#include "otbImageWidgetBase.h"

namespace otb
{
/** \class FixedSizeFullImageWidget
 * \brief 
 *
 */
template <class TPixel>
class FixedSizeFullImageWidget
  : public ImageWidgetBase<TPixel>
{
 public:
  /** Standard class typedefs */
  typedef FixedSizeFullImageWidget Self;
  typedef ImageWidgetBase<TPixel> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(FixedSizeFullImageWidget,ImageWidgetBase);

  /** Superclass typedefs */
  typedef typename Superclass::SizeType SizeType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::RegionType RegionType;

  itkSetMacro(WindowSize,SizeType);
  itkGetMacro(WindowSize,SizeType);

  /** Unlarge OpenGlBuffer */
  virtual void UpdateOpenGlBufferedRegion(void);
  /** Test if the buffer has to be enlarged */
  virtual bool UpdateOpenGlBufferedRegionRequested(void);
  /** Initialize the widget */
  virtual void Init(int x, int y, int w, int h, const char * l);
  /** Resize the widget */
  virtual void resize(int x, int y, int w, int h);

 protected:
  /** Constructor. */
    FixedSizeFullImageWidget();
    /** Destructor. */
    ~FixedSizeFullImageWidget();

 private:
    FixedSizeFullImageWidget(const Self&);// purposely not implemented
    void operator=(const Self&);// purposely not implemented

    SizeType m_WindowSize;
    bool m_ImageLoaded;
    bool m_ImageOverlayLoaded;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFixedSizeFullImageWidget.txx"
#endif

#endif
